#include <stdlib.h>

#include "debugdraw.h"

struct debug_draw_ctx* new_debug_draw_ctx(struct shader shader) {
	struct debug_draw_ctx* ctx = malloc(sizeof(struct debug_draw_ctx) + sizeof(struct debug_pixel) * screen_w * screen_h);

	u32 indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	f32 verts[] = {
		 1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f
	};

	init_vb(&ctx->vb, vb_static | vb_tris);
	bind_vb_for_edit(&ctx->vb);
	push_vertices(&ctx->vb, verts, 16 * sizeof(f32));
	push_indices(&ctx->vb, indices, 6);
	configure_vb(&ctx->vb, 0, 2, 4 * sizeof(f32), 0); /* position (vec2) */
	configure_vb(&ctx->vb, 1, 2, 4 * sizeof(f32), 8); /* uv (vec2) */
	bind_vb_for_edit(null);

	return ctx;
}

void free_debug_draw_ctx(struct debug_draw_ctx* ctx) {

}

void debug_draw(struct debug_draw_ctx* ctx) {

}

void debug_draw_line(struct debug_draw_ctx* ctx, v3f start, v3f end) {

}
