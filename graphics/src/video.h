#pragma once

#include "common.h"
#include "maths.h"

#define max_lights 100

/* OpenGL abstraction. I think it makes the rest of the
 * program look more consistent. */

void video_init();
void video_clear();

void enable_cull_face();
void disable_cull_face();

struct shader {
	bool panic;

	u32 id;

	u64 mod_time;
	char* filename;
};

struct shader* new_shader(const char* source, const char* name);
struct shader* new_shader_from_file(const char* filename);
void shader_reload(struct shader* shader);
void free_shader(struct shader* shader);
void bind_shader(const struct shader* shader);
void shader_set_f(const struct shader* shader, const char* name, const f32 v);
void shader_set_i(const struct shader* shader, const char* name, const i32 v);
void shader_set_u(const struct shader* shader, const char* name, const u32 v);
void shader_set_b(const struct shader* shader, const char* name, const bool v);
void shader_set_v2f(const struct shader* shader, const char* name, const v2f v);
void shader_set_v3f(const struct shader* shader, const char* name, const v3f v);
void shader_set_v4f(const struct shader* shader, const char* name, const v4f v);
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
void push_vertices(const struct vertex_buffer* vb, void* vertices, u64 size);
void push_indices(struct vertex_buffer* vb, u32* indices, u32 count);
void update_vertices(const struct vertex_buffer* vb, void* vertices, u64 offset, u64 size);
void update_indices(struct vertex_buffer* vb, u32* indices, u32 offset, u32 count);
void configure_vb(const struct vertex_buffer* vb, u32 index, u32 component_count,
	u64 stride, u64 offset);
void draw_vb(const struct vertex_buffer* vb);
void draw_vb_n(const struct vertex_buffer* vb, u32 count);

struct texture {
	u32 id;
	u32 width, height;
};

void init_texture(struct texture* texture, const char* path);
void init_texture_from_memory(struct texture* texture, void* data, u32 w, u32 h, u32 cc);
void update_texture_contents(struct texture* texture, void* data, u32 w, u32 h, u32 cc);
void deinit_texture(struct texture* texture);
void bind_texture(const struct texture* texture, u32 unit);

struct render_target {
	u32 id;
	u32 width, height;

	u32 output;
	u32 depth;
};

void clear_render_target(struct render_target* target);
void init_render_target(struct render_target* target, u32 width, u32 height);
void deinit_render_target(struct render_target* target);
void resize_render_target(struct render_target* target, u32 width, u32 height);
void bind_render_target(struct render_target* target);
void bind_render_target_output(struct render_target* target, u32 unit);
void bind_render_target_output_depth(struct render_target* target, u32 unit);

struct depth_map {
	u32 id;
	u32 width, height;

	u32 output;

	i32 old_viewport[4];
};

void clear_depth_map(struct depth_map* dm);
void init_depth_map(struct depth_map* dm, u32 width, u32 height);
void deinit_depth_map(struct depth_map* dm);
void bind_depth_map(struct depth_map* dm);
void unbind_depth_map(struct depth_map* dm);
void bind_depth_map_output(struct depth_map* dm, u32 unit);

struct color {
	u8 r, g, b, a;
};
