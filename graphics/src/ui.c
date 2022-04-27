#include <stdlib.h>

#include "ui.h"

struct ui {
	v2f cursor_pos;

	bool hovered;

	struct renderer2d* renderer;
	struct font* font;

	f32 padding;

	bool mouse_clicked;
	v2f mouse_pos;
};

struct ui* new_ui_ctx(struct renderer2d* renderer, struct font* font) {
	struct ui* ctx = calloc(1, sizeof(struct ui));

	ctx->renderer = renderer;
	ctx->font = font;

	ctx->padding = 3.0f;

	return ctx;
}

void free_ui_ctx(struct ui* ctx) {
	free(ctx);
}

void ui_begin_frame(struct ui* ctx, v2f mouse_pos, bool mouse_clicked) {
	ctx->cursor_pos = make_v2f(ctx->padding, ctx->padding);
	ctx->mouse_pos = mouse_pos;
	ctx->mouse_clicked = mouse_clicked;
	ctx->hovered = false;
}

void ui_end_frame(struct ui* ctx) {
	renderer2d_flush(ctx->renderer);
}

void ui_text(struct ui* ctx, const char* text) {
	ui_text_ex(ctx, text, make_v3f(1.0f, 1.0f, 1.0f));
}

void ui_text_ex(struct ui* ctx, const char* text, v3f color) {
	v2f d = calc_dimentions(ctx->font, text);

	render_text(ctx->renderer, ctx->font, text, ctx->cursor_pos, color);

	ctx->cursor_pos.y += d.y + ctx->padding;
}

bool ui_button(struct ui* ctx, const char* text) {
	renderer2d_set_texture(ctx->renderer, null);

	v2f d = calc_dimentions(ctx->font, text);

	struct quad background = {
			.position = ctx->cursor_pos,
			.dimentions = v2f_add(d, make_v2f(ctx->padding * 2.0f, ctx->padding * 2.0f)),
			.color = make_v3f(0.6f, 0.6f, 0.6f)
	};

	bool hovered =
		ctx->mouse_pos.x > background.position.x &&
		ctx->mouse_pos.y > background.position.y &&
		ctx->mouse_pos.x < background.position.x + background.dimentions.x &&
		ctx->mouse_pos.y < background.position.y + background.dimentions.y;
	
	if (hovered) {
		background.color = make_v3f(1.0f, 1.0f, 1.0f);

		ctx->hovered = true;
	}

	renderer2d_push(ctx->renderer, &background);

	render_text(ctx->renderer, ctx->font, text,
			v2f_add(ctx->cursor_pos, make_v2f(ctx->padding, ctx->padding)),
			v3f_zero());

	ctx->cursor_pos.y += background.dimentions.y + ctx->padding;

	return hovered && ctx->mouse_clicked;
}

bool ui_anything_hovered(struct ui* ctx) {
	return ctx->hovered;
}
