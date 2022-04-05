#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#include "stb_truetype.h"

#include "renderer2d.h"

struct renderer2d* new_renderer2d(struct shader* shader) {
	struct renderer2d* self = calloc(1, sizeof(struct renderer2d));

	self->shader = shader;

	self->projection = m4f_orth(0.0f, screen_w, screen_h, 0.0f, -1.0f, 1.0);

	init_vb(&self->vb, vb_dynamic | vb_tris);
	bind_vb_for_edit(&self->vb);
	push_vertices(&self->vb, self->verts,
		sizeof(struct vertex2d) * renderer2d_batch_size * renderer2d_verts_per_quad);
	push_indices(&self->vb, self->indices,
		sizeof(struct vertex2d) * renderer2d_batch_size * renderer2d_indices_per_quad);
	configure_vb(&self->vb, 0, 2, sizeof(struct vertex2d), offsetof(struct vertex2d, position));
	configure_vb(&self->vb, 1, 2, sizeof(struct vertex2d), offsetof(struct vertex2d, uv));
	bind_vb_for_edit(null);

	return self;
}

void free_renderer2d(struct renderer2d* self) {
	deinit_vb(&self->vb);
	free(self);
}

void renderer2d_push(struct renderer2d* self, struct quad* quad) {
	m4f transform = m4f_translate(m4f_identity(), (v3f) { quad->position.x, quad->position.y, 0.0f });
	transform = m4f_scale(transform, (v3f) { quad->dimentions.x, quad->dimentions.y, 0.0f });

	const f32 tx = quad->rect.x / self->texture->width;
	const f32 ty = quad->rect.y / self->texture->height;
	const f32 tw = quad->rect.w / self->texture->width;
	const f32 th = quad->rect.h / self->texture->height;

	/* Vertices are transformed on the CPU instead of the shader, to avoid
	 * having a transform matrix in an array on the shader or as a vertex
	 * array layout, because of batching. */
	const v4f p0 = m4f_transform(transform, make_v4f(0.0f, 0.0f, 0.0f, 1.0f));
	const v4f p1 = m4f_transform(transform, make_v4f(1.0f, 0.0f, 0.0f, 1.0f));
	const v4f p2 = m4f_transform(transform, make_v4f(1.0f, 1.0f, 0.0f, 1.0f));
	const v4f p3 = m4f_transform(transform, make_v4f(0.0f, 1.0f, 0.0f, 1.0f));

	struct vertex2d verts[] = {
		{ { p0.x, p0.y }, { tx,      ty }       },
		{ { p1.x, p1.y }, { tx + tw, ty }      },
		{ { p2.x, p2.y }, { tx + tw, ty + th } },
		{ { p3.x, p3.y }, { tx,      ty + th } },
	};

	const u32 idx_off = self->quad_count * renderer2d_verts_per_quad;

	u32 indices[] = {
		idx_off + 3, idx_off + 2, idx_off + 1,
		idx_off + 3, idx_off + 1, idx_off + 0
	};

	memcpy(self->verts + (self->quad_count * renderer2d_verts_per_quad), verts,
		renderer2d_verts_per_quad * sizeof(struct vertex2d));
	memcpy(self->indices + (self->quad_count * renderer2d_indices_per_quad), indices,
		renderer2d_indices_per_quad * sizeof(u32));

	self->quad_count++;

	if (self->quad_count >= renderer2d_batch_size) {
		renderer2d_flush(self);
	}
}

void renderer2d_flush(struct renderer2d* self) {
	if (self->quad_count == 0) { return; }

	bind_shader(self->shader);
	shader_set_m4f(self->shader, "projection", self->projection);
	bind_texture(self->texture, 0);
	shader_set_i(self->shader, "image", 0);

	/* Send the vertices to the GPU and draw them. */
	bind_vb_for_edit(&self->vb);
	update_vertices(&self->vb, self->verts, 0,
		sizeof(struct vertex2d) * self->quad_count * renderer2d_verts_per_quad);
	update_indices(&self->vb, self->indices, 0, self->quad_count * renderer2d_indices_per_quad);

	bind_vb_for_draw(&self->vb);
	draw_vb_n(&self->vb, self->quad_count * renderer2d_indices_per_quad);
	bind_vb_for_draw(null);

	self->quad_count = 0;
}

void renderer2d_set_texture(struct renderer2d* self, struct texture* texture) {
	if ((self->texture && self->texture->id != texture->id) || !self->texture) {
		renderer2d_flush(self);
		self->texture = texture;
	}
}

struct glyph_set {
	struct texture atlas;
	stbtt_bakedchar glyphs[256];
};

#define max_glyphset 256

struct font {
	void* data;
	stbtt_fontinfo info;
	struct glyph_set* sets[max_glyphset];
	f32 size;
	i32 height;
};

/* This font renderer actually supports UTF-8! This function takes a
 * UTF-8 encoded string and gives the position in the glyph set of the
 * character. Note that the character is not guaranteed to exist, so
 * null text may be rendered, however the font defines that.
 *
 * This implementation is based on the man page for utf-8(7) */
static const char* utf8_to_codepoint(const char* p, u32* dst) {
	u32 res, n;
	switch (*p & 0xf0) {
		case 0xf0 : res = *p & 0x07; n = 3; break;
		case 0xe0 : res = *p & 0x0f; n = 2; break;
		case 0xd0 :
		case 0xc0 : res = *p & 0x1f; n = 1; break;
		default   : res = *p;        n = 0; break;
	}
	while (n--) {
		res = (res << 6) | (*(++p) & 0x3f);
	}
	*dst = res;
	return p + 1;
}

/* Draw the trueype font into a bitmap and extract the glyph information
 * from it.*/
static struct glyph_set* load_glyph_set(struct font* font, i32 idx) {
	struct glyph_set* set = calloc(1, sizeof(struct glyph_set));

	u32 width = 128;
	u32 height = 128;

	struct color* pixels;
retry:
	pixels = malloc(width * height * 4);

	f32 s = stbtt_ScaleForMappingEmToPixels(&font->info, 1) /
		stbtt_ScaleForPixelHeight(&font->info, 1);
	i32 r = stbtt_BakeFontBitmap(font->data, 0, font->size * s,
			(void*)pixels, width, height, idx * 256, 256, set->glyphs);

	if (r <= 0) {
		width *= 2;
		height *= 2;
		free(pixels);
		goto retry;
	}

	i32 ascent, descent, linegap, scaled_ascent;
	stbtt_GetFontVMetrics(&font->info, &ascent, &descent, &linegap);
	f32 scale = stbtt_ScaleForMappingEmToPixels(&font->info, font->size);
	scaled_ascent = (i32)(ascent * scale + 0.5);
	for (u32 i = 0; i < 256; i++) {
		set->glyphs[i].yoff += scaled_ascent;
		set->glyphs[i].xadvance = (f32)floor(set->glyphs[i].xadvance);
	}

	for (i32 i = width * height - 1; i >= 0; i--) {
		unsigned char n = *((u8*)pixels + i);
		pixels[i] = (struct color) {255, 255, 255, n};
	}

	init_texture_from_memory(&set->atlas, pixels, width, height, 4);

	free(pixels);

	return set;
}

static struct glyph_set* get_glyph_set(struct font* font, i32 code_poi32) {
	i32 idx = (code_poi32 >> 8) % max_glyphset;
	if (!font->sets[idx]) {
		font->sets[idx] = load_glyph_set(font, idx);
	}
	return font->sets[idx];
}


struct font* new_font_from_file(const char* path, f32 size) {
	u64 filesize;

	struct font* font = calloc(1, sizeof(struct font));
	if (!read_raw(path, (u8**)&font->data, &filesize, false)) {
		free(font);
		return null;
	}

	font->size = size;

	i32 r = stbtt_InitFont(&font->info, font->data, 0);
	if (!r) {
		goto fail;
	}

	i32 ascent, descent, linegap;
	stbtt_GetFontVMetrics(&font->info, &ascent, &descent, &linegap);
	f32 scale = stbtt_ScaleForMappingEmToPixels(&font->info, size);
	font->height = (i32)((ascent - descent + linegap) * scale + 0.5);

	stbtt_bakedchar* g = get_glyph_set(font, '\n')->glyphs;
	g['\t'].x1 = g['\t'].x0;
	g['\n'].x1 = g['\n'].x0;

	return font;

fail:
	if (font) {
		if (font->data) {
			free(font->data);
		}
		free(font);
	}
	return null;
}

void free_font(struct font* font) {
	i32 i;
	struct glyph_set* set;

	for (i = 0; i < max_glyphset; i++) {
		set = font->sets[i];
		if (set) {
			deinit_texture(&set->atlas);
			free(set);
		}
	}

	free(font->data);
	free(font);
}

void render_text(struct renderer2d* renderer, struct font* font, const char* text, v2f position) {
	f32 x = position.x;
	f32 y = position.y;

	const char* p;
	u32 codepoint;
	struct glyph_set* set;
	stbtt_bakedchar* g;
	f32 ori_x = x;

	p = text;
	while (*p) {
		if (*p == '\n') {
			x = ori_x;
			y += font->height;

			p++;
			continue;
		}

		p = utf8_to_codepoint(p, &codepoint);

		set = get_glyph_set(font, codepoint);
		g = &set->glyphs[codepoint & 0xff];

		f32 w = (f32)(g->x1 - g->x0);
		f32 h = (f32)(g->y1 - g->y0);

		renderer2d_set_texture(renderer, &set->atlas);
		
		struct quad quad = {
			.position = { x + g->xoff, y + g->yoff },
			.dimentions = { w, h },
			.rect = { g->x0, g->y0, w, h },
		};

		renderer2d_push(renderer, &quad);
		x += g->xadvance;
	}

	renderer2d_flush(renderer);
}
