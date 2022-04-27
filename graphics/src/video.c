#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "video.h"
#include "vector.h"

void video_init() {
	if (!gladLoadGL()) {
		print_log("Failed to load OpenGL.\n");
		abort();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_SCISSOR_TEST);
}

void video_clear() {
	glDisable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void enable_cull_face() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void disable_cull_face() {
	glDisable(GL_CULL_FACE);
}

void enable_depth_test() {
	glEnable(GL_DEPTH_TEST);
}

void disable_depth_test() {
	glDisable(GL_DEPTH_TEST);
}

static bool init_shader(struct shader* shader, const char* source, const char* name) {
	const u32 source_len = (u32)strlen(source);

	char* vertex_source = malloc(source_len);
	char* fragment_source = malloc(source_len);
	char* geometry_source = malloc(source_len);
	memset(vertex_source, 0, source_len);
	memset(fragment_source, 0, source_len);
	memset(geometry_source, 0, source_len);

	bool has_geometry = false;

	char* line = null;
	usize line_cap = 0;

	u32 count = 0;
	i32 adding_to = -1;
	for (const char* current = source; *current != '\0'; current++) {
		if (*current == '\n' && *(current + 1) != '\0') {
			i32 minus = 1;

			current++;

			if (count + 1 > line_cap) {
				line_cap = count + 1;
				line = realloc(line, line_cap);
			}
			memcpy(line, current - count - minus, count);
			line[count] = '\0';

			if (strstr(line, "#begin VERTEX")) {
				adding_to = 0;

				strcat(vertex_source, "#version 450 core\n");
			} else if (strstr(line, "#begin FRAGMENT")) {
				adding_to = 1;

				strcat(fragment_source, "#version 450 core\n");
			} else if (strstr(line, "#begin GEOMETRY")) {
				adding_to = 2;
				has_geometry = true;

				strcat(geometry_source, "#version 450 core\n");
			} else if (strstr(line, "#end VERTEX") ||
				strstr(line, "#end FRAGMENT") ||
				strstr(line, "#end GEOMETRY")) {
				adding_to = -1;
			} else if (adding_to == 0) {
				strcat(vertex_source, line);
				strcat(vertex_source, "\n");
			} else if (adding_to == 1) {
				strcat(fragment_source, line);
				strcat(fragment_source, "\n");
			} else if (adding_to == 2) {
				strcat(geometry_source, line);
				strcat(geometry_source, "\n");
			}

			count = 0;
		}

		count++;
	}

	free(line);

	const char* vsp = vertex_source;
	const char* fsp = fragment_source;
	const char* gsp = geometry_source;

	i32 success;
	u32 v, f, g;
	v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v, 1, &vsp, NULL);
	glCompileShader(v);

	glGetShaderiv(v, GL_COMPILE_STATUS, &success);
	if (!success) {
		char info_log[1024];
		glGetShaderInfoLog(v, 1024, null, info_log);
		print_log("Vertex shader of `%s' failed to compile with the following errors:\n%s\n", name,
			info_log);
		shader->panic = true;
	}

	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f, 1, &fsp, null);
	glCompileShader(f);

	glGetShaderiv(f, GL_COMPILE_STATUS, &success);
	if (!success) {
		char info_log[1024];
		glGetShaderInfoLog(f, 1024, null, info_log);
		print_log("Fragment shader of `%s' failed to compile with the following errors:\n%s\n", name,
			info_log);
		shader->panic = true;
	}

	if (has_geometry) {
		g = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(g, 1, &gsp, null);
		glCompileShader(g);

		glGetShaderiv(g, GL_COMPILE_STATUS, &success);
		if (!success) {
			char info_log[1024];
			glGetShaderInfoLog(g, 1024, null, info_log);
			print_log("Geometry shader of `%s' failed to compile with the following errors:\n%s\n", name,
				info_log);
			shader->panic = true;
		}
	}

	u32 id;
	id = glCreateProgram();
	glAttachShader(id, v);
	glAttachShader(id, f);
	if (has_geometry) {
		glAttachShader(id, g);
	}
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		char info_log[1024];
		glGetProgramInfoLog(id, 1024, null, info_log);
		print_log("Shader `%s' failed to link with the following errors:\n%s\n", name,
			info_log);
		shader->panic = true;
	}

	glDeleteShader(v);
	glDeleteShader(f);

	if (has_geometry) {
		glDeleteShader(g);
	}

	free(vertex_source);
	free(fragment_source);
	free(geometry_source);

	shader->id = id;

	return !shader->panic;
}

struct shader* new_shader(const char* source, const char* name) {
	struct shader* shader = calloc(1, sizeof(struct shader));

	init_shader(shader, source, name);

	return shader;
}

struct shader* new_shader_from_file(const char* filename) {
	char* buffer = null;
	u64 mod_time = 0;
	if (read_raw(filename, (u8**)&buffer, null, true)) {
		mod_time = file_mod_time(filename);
	}

	struct shader* s = new_shader(buffer, filename);
	s->mod_time = mod_time;
	s->filename = copy_string(filename);

	if (buffer) { free(buffer); }

	return s;
}

void shader_reload(struct shader* shader) {
	if (!shader->filename) { return; }

	u64 mod_time = file_mod_time(shader->filename);
	if (mod_time > shader->mod_time) {
		char* buffer = null;
		u64 mod_time = 0;
		if (read_raw(shader->filename, (u8**)&buffer, null, true)) {
			mod_time = file_mod_time(shader->filename);
		}

		u32 old_id = shader->id;
		if (init_shader(shader, buffer, shader->filename)) {
			glDeleteProgram(old_id);
		} else {
			shader->id = old_id;
		}

		shader->mod_time = mod_time;
		shader->panic = false;

		if (buffer) { free(buffer); }
	}
}

void free_shader(struct shader* shader) {
	glDeleteProgram(shader->id);

	if (shader->filename) { free(shader->filename); }

	free(shader);
}

void bind_shader(const struct shader* shader) {
	glUseProgram(shader ? shader->id : 0);
}

void shader_set_f(const struct shader* shader, const char* name, const f32 v) {
	if (shader->panic) { return; }

	u32 location = glGetUniformLocation(shader->id, name);
	glUniform1f(location, v);
}

void shader_set_i(const struct shader* shader, const char* name, const i32 v) {
	if (shader->panic) { return; }

	u32 location = glGetUniformLocation(shader->id, name);
	glUniform1i(location, v);
}

void shader_set_u(const struct shader* shader, const char* name, const u32 v) {
	if (shader->panic) { return; }

	u32 location = glGetUniformLocation(shader->id, name);
	glUniform1ui(location, v);
}

void shader_set_b(const struct shader* shader, const char* name, const bool v) {
	if (shader->panic) { return; }

	u32 location = glGetUniformLocation(shader->id, name);
	glUniform1i(location, v);
}

void shader_set_v2f(const struct shader* shader, const char* name, const v2f v) {
	if (shader->panic) { return; }

	u32 location = glGetUniformLocation(shader->id, name);
	glUniform2f(location, v.x, v.y);
}

void shader_set_v3f(const struct shader* shader, const char* name, const v3f v) {
	if (shader->panic) { return; }

	u32 location = glGetUniformLocation(shader->id, name);
	glUniform3f(location, v.x, v.y, v.z);
}

void shader_set_v4f(const struct shader* shader, const char* name, const v4f v) {
	if (shader->panic) { return; }

	u32 location = glGetUniformLocation(shader->id, name);
	glUniform4f(location, v.x, v.y, v.z, v.w);
}

void shader_set_m4f(const struct shader* shader, const char* name, const m4f v) {
	if (shader->panic) { return; }

	u32 location = glGetUniformLocation(shader->id, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, (f32*)v.m);
}

void init_vb(struct vertex_buffer* vb, const i32 flags) {
	vb->flags = flags;

	glGenVertexArrays(1, &vb->va_id);
	glGenBuffers(1, &vb->vb_id);
	glGenBuffers(1, &vb->ib_id);
}

void deinit_vb(struct vertex_buffer* vb) {
	glDeleteVertexArrays(1, &vb->va_id);
	glDeleteBuffers(1, &vb->vb_id);
	glDeleteBuffers(1, &vb->ib_id);
}

void bind_vb_for_draw(const struct vertex_buffer* vb) {
	glBindVertexArray(vb ? vb->va_id : 0);
}

void bind_vb_for_edit(const struct vertex_buffer* vb) {
	if (!vb) {
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else {
		glBindVertexArray(vb->va_id);
		glBindBuffer(GL_ARRAY_BUFFER, vb->vb_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb->ib_id);
	}
}

void push_vertices(const struct vertex_buffer* vb, void* vertices, u64 size) {
	const u32 mode = vb->flags & vb_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

	glBufferData(GL_ARRAY_BUFFER, size, vertices, mode);
}

void push_indices(struct vertex_buffer* vb, u32* indices, u32 count) {
	const u32 mode = vb->flags & vb_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

	vb->index_count = count;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, mode);
}

void update_vertices(const struct vertex_buffer* vb, void* vertices, u64 offset, u64 size) {
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
}

void update_indices(struct vertex_buffer* vb, u32* indices, u32 offset, u32 count) {
	vb->index_count = count;

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(u32),
		count * sizeof(u32), indices);
}

void configure_vb(const struct vertex_buffer* vb, u32 index, u32 component_count,
	u64 stride, u64 offset) {

	glVertexAttribPointer(index, component_count, GL_FLOAT, GL_FALSE,
		(GLsizei)stride, (void*)(u64)(offset));
	glEnableVertexAttribArray(index);
}

void draw_vb(const struct vertex_buffer* vb) {
	u32 draw_type = GL_TRIANGLES;
	if (vb->flags & vb_lines) {
		draw_type = GL_LINES;
	} else if (vb->flags & vb_line_strip) {
		draw_type = GL_LINE_STRIP;
	}

	glDrawElements(draw_type, vb->index_count, GL_UNSIGNED_INT, 0);
}

void draw_vb_n(const struct vertex_buffer* vb, u32 count) {
	u32 draw_type = GL_TRIANGLES;
	if (vb->flags & vb_lines) {
		draw_type = GL_LINES;
	} else if (vb->flags & vb_line_strip) {
		draw_type = GL_LINE_STRIP;
	}

	glDrawElements(draw_type, count, GL_UNSIGNED_INT, 0);
}

void init_noise_texture(struct texture* texture, u32 w, u32 h) {
	v3f* noise = malloc(w * h * sizeof(v3f));

	for (u32 i = 0; i < w * h; i++) {
		noise[i] = make_v3f(
			random_float(-1.0f, 1.0f),
			random_float(-1.0f, 1.0f),
			random_float(-1.0f, 1.0f)
		);
	}

	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
		w, h, 0, GL_RGB,
		GL_UNSIGNED_BYTE, noise);

	texture->width = w;
	texture->height = h;

	free(noise);
}

void init_texture(struct texture* texture, const char* path) {
	i32 w, h, n;
	stbi_set_flip_vertically_on_load(true);
	u8* src = stbi_load(path, &w, &h, &n, 4);

	if (!src) {
		print_log("Failed to load texture: `%s': %s\n", path, stbi_failure_reason());
		return;
	}

	init_texture_from_memory(texture, src, w, h, n);

	free(src);
}

void init_texture_from_memory(struct texture* texture, void* data, u32 w, u32 h, u32 cc) {
	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);

	texture->width = w;
	texture->height = h;
}

void update_texture_contents(struct texture* texture, void* data, u32 w, u32 h, u32 cc) {
	glBindTexture(GL_TEXTURE_2D, texture->id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);
}

void deinit_texture(struct texture* texture) {
	glDeleteTextures(1, &texture->id);
}

void bind_texture(const struct texture* texture, u32 unit) {
	if (!texture) {
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture->id);
}

void init_render_target(struct render_target* target, u32 color_attachment_count, bool has_depth, u32 width, u32 height) {
	target->color_count = color_attachment_count;
	target->has_depth = has_depth;

	glGenFramebuffers(1, &target->id);

	glBindFramebuffer(GL_FRAMEBUFFER, target->id);

	u32 attachments[16];
	for (u32 i = 0; i < maximum(1, color_attachment_count); i++) {
		glGenTextures(1, &target->output[i]);
		glBindTexture(GL_TEXTURE_2D, target->output[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, null);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, target->output[i], 0);

		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(color_attachment_count, attachments);
	
	if (has_depth) {
		glGenTextures(1, &target->depth);
		glBindTexture(GL_TEXTURE_2D, target->depth);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, null);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, target->depth, 0);
	}

	target->width = width;
	target->height = height;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		print_log("Failed to create render target.\n");
	}

	bind_render_target(null);
}

void deinit_render_target(struct render_target* target) {
	glDeleteFramebuffers(1, &target->id);
	glDeleteTextures(target->color_count, target->output);
	glDeleteTextures(1, &target->depth);
}

void clear_render_target(struct render_target* target) {
	bind_render_target(target);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void resize_render_target(struct render_target* target, u32 width, u32 height) {
	if (target->width == width && target->height == height) { return; }

	target->width = width;
	target->height = height;

	for (u32 i = 0; i < target->color_count; i++) {
		glBindTexture(GL_TEXTURE_2D, target->output[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, null);
	}

	glBindTexture(GL_TEXTURE_2D, target->depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, null);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void bind_render_target(struct render_target* target) {
	if (!target) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, target->id);
}

void bind_render_target_output(struct render_target* target, u32 attachment, u32 unit) {
	if (!target) {
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, target->output[attachment]);
}

void bind_render_target_output_depth(struct render_target* target, u32 unit) {
	if (!target || !target->has_depth) {
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, target->depth);
}

void init_depth_map(struct depth_map* dm, u32 width, u32 height) {
	glGenFramebuffers(1, &dm->id);

	glBindFramebuffer(GL_FRAMEBUFFER, dm->id);

	glGenTextures(1, &dm->output);
	glBindTexture(GL_TEXTURE_2D, dm->output);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dm->output, 0);

	dm->width = width;
	dm->height = height;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		print_log("Failed to create depth map.\n");
	}

	bind_depth_map(null);
}

void deinit_depth_map(struct depth_map* dm) {
	glDeleteFramebuffers(1, &dm->id);
	glDeleteTextures(1, &dm->output);
}

void bind_depth_map(struct depth_map* dm) {
	if (!dm) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}

	glGetIntegerv(GL_VIEWPORT, dm->old_viewport);
	glViewport(0, 0, dm->width, dm->height);

	glBindFramebuffer(GL_FRAMEBUFFER, dm->id);
}

void unbind_depth_map(struct depth_map* dm) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(
		dm->old_viewport[0],
		dm->old_viewport[1],
		dm->old_viewport[2],
		dm->old_viewport[3]);
}

void clear_depth_map(struct depth_map* dm) {
	bind_depth_map(dm);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void bind_depth_map_output(struct depth_map* dm, u32 unit) {
	if (!dm) {
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, dm->output);
}
