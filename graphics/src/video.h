#pragma once

#include "common.h"
#include "maths.h"

#define max_lights 100

/* Basic renderer; Graphics abstraction. */

void video_init();
void video_clear();

struct shader {
	bool panic;

	u32 id;
};

void init_shader(struct shader* shader, const char* source, const char* name);
void init_shader_from_file(struct shader* shader, const char* filename);
void deinit_shader(struct shader* shader);
void bind_shader(const struct shader* shader);
void shader_set_f(const struct shader* shader, const char* name, const f32 v);
void shader_set_i(const struct shader* shader, const char* name, const i32 v);
void shader_set_u(const struct shader* shader, const char* name, const u32 v);
void shader_set_b(const struct shader* shader, const char* name, const bool v);
void shader_set_v2f(const struct shader* shader, const char* name, const v2f v);
void shader_set_v3f(const struct shader* shader, const char* name, const v3f v);
void shader_set_v4f(const struct shader* shader, const char* name, const v4f v);
void shader_set_m3f(const struct shader* shader, const char* name, const m3f v);
void shader_set_m4f(const struct shader* shader, const char* name, const m4f v);

enum {
	vb_static     = 1 << 0,
	vb_dynamic    = 1 << 1,
	vb_lines      = 1 << 2,
	vb_line_strip = 1 << 3,
	vb_tris       = 1 << 4
};

struct vertex_buffer {
	u32 va_id;
	u32 vb_id;
	u32 ib_id;
	u32 index_count;

	i32 flags;
};

void init_vb(struct vertex_buffer* vb, const i32 flags);
void deinit_vb(struct vertex_buffer* vb);
void bind_vb_for_draw(const struct vertex_buffer* vb);
void bind_vb_for_edit(const struct vertex_buffer* vb);
void push_vertices(const struct vertex_buffer* vb, f32* vertices, u32 count);
void push_indices(struct vertex_buffer* vb, u32* indices, u32 count);
void update_vertices(const struct vertex_buffer* vb, f32* vertices, u32 offset, u32 count);
void update_indices(struct vertex_buffer* vb, u32* indices, u32 offset, u32 count);
void configure_vb(const struct vertex_buffer* vb, u32 index, u32 component_count,
	u32 stride, u32 offset);
void draw_vb(const struct vertex_buffer* vb);
void draw_vb_n(const struct vertex_buffer* vb, u32 count);

struct texture {
	u32 id;
	u32 width, height;
};

void init_texture(struct texture* texture, u8* src, u64 size);
void init_texture_no_bmp(struct texture* texture, u8* src, u32 w, u32 h, bool flip);
void update_texture(struct texture* texture, u8* data, u64 size);
void update_texture_no_bmp(struct texture* texture, u8* src, u32 w, u32 h, bool flip);
void deinit_texture(struct texture* texture);
void bind_texture(const struct texture* texture, u32 unit);

struct render_target {
	u32 id;
	u32 width, height;

	u32 output;
};

void init_render_target(struct render_target* target, u32 width, u32 height);
void deinit_render_target(struct render_target* target);
void resize_render_target(struct render_target* target, u32 width, u32 height);
void bind_render_target(struct render_target* target);
void bind_render_target_output(struct render_target* target, u32 unit);

struct color {
	u8 r, g, b, a;
};