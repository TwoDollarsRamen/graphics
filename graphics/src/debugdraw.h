#pragma once

#include "common.h"
#include "maths.h"
#include "video.h"

struct debug_pixel {
	u8 r, g, b, a;
};

struct debug_draw_ctx {
	struct vertex_buffer vb;
	struct shader shader;
	struct texture texture;

	struct debug_pixel* pixels;
};

struct debug_draw_ctx* new_debug_draw_ctx(struct shader shader);
void free_debug_draw_ctx(struct debug_draw_ctx* ctx);
void debug_draw(struct debug_draw_ctx* ctx);
void debug_draw_line(struct debug_draw_ctx* ctx, v3f start, v3f end);
