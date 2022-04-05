#pragma once

#include "common.h"
#include "maths.h"
#include "video.h"

#define renderer2d_batch_size 800
#define renderer2d_verts_per_quad 4
#define renderer2d_indices_per_quad 6

struct vertex2d {
	v2f position;
	v2f uv;
};

struct renderer2d {
	m4f projection;

	struct shader* shader;
	struct vertex_buffer vb;

	struct vertex2d verts[renderer2d_batch_size * renderer2d_verts_per_quad];
	u32 indices[renderer2d_batch_size * renderer2d_indices_per_quad];

	struct texture* texture;

	u32 quad_count;
};

struct rect {
	f32 x, y, w, h;
};

struct quad {
	v2f position;
	v2f dimentions;
	struct rect rect;
};

struct font;

struct font* new_font_from_file(const char* path, f32 size);
void free_font(struct font* font);

struct renderer2d* new_renderer2d(struct shader* shader);
void free_renderer2d(struct renderer2d* self);
void renderer2d_push(struct renderer2d* self, struct quad* quad);
void renderer2d_flush(struct renderer2d* self);
void renderer2d_set_texture(struct renderer2d* self, struct texture* texture);
void render_text(struct renderer2d* renderer, struct font* font, const char* text, v2f position);
