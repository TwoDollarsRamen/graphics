#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "common.h"
#include "loadobj.h"
#include "mesh.h"
#include "renderer.h"
#include "renderer2d.h"
#include "table.h"
#include "vector.h"
#include "video.h"

vector(char) log_buffer;

void show_loading_screen(v2i screen_size) {
	struct texture image;
	glClearColor(0.0705882352941f, 0.0823529411765f, 0.262745098039f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	init_texture(&image, "res/textures/loading.png");
	struct shader* shader = new_shader_from_file("res/shaders/loading.glsl");

	struct vertex_buffer vb;

	u32 indices[] = {
		0, 1, 3,
		1, 2, 3 
	};

	f32 verts[] = {
		 0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f
	};

	init_vb(&vb, vb_static | vb_tris);
	bind_vb_for_edit(&vb);
	push_vertices(&vb, verts, 16 * sizeof(f32));
	push_indices(&vb, indices, 6);
	configure_vb(&vb, 0, 2, 4 * sizeof(f32), 0); /* position (vec2) */
	configure_vb(&vb, 1, 2, 4 * sizeof(f32), 8); /* uv (vec2) */
	bind_vb_for_edit(null);

	bind_shader(shader);
	bind_texture(&image, 0);
	shader_set_i(shader, "t", 0);

	shader_set_m4f(shader, "camera", m4f_orth(0.0f, (f32)screen_size.x, (f32)screen_size.y, 0.0f, -1.0f, 1.0f));
	shader_set_m4f(shader, "transform",
		m4f_mul(
			m4f_translate(m4f_identity(), make_v3f(
				((f32)screen_size.x / 2.0f),
				((f32)screen_size.y / 2.0f), 0.0f
			)),
			m4f_scale(m4f_identity(), make_v3f((f32)image.width, (f32)image.height, 0.0f))
			)
		);

	bind_vb_for_draw(&vb);
	draw_vb(&vb);
	
	deinit_texture(&image);
	free_shader(shader);
	deinit_vb(&vb);

	glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
}


i32 main() {
	glfwInit();

	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(1366, 768, "Graphics", null, null);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);

	gladLoadGL();

	show_loading_screen(make_v2i(screen_w, screen_h));
	glfwSwapBuffers(window);

	glEnable(GL_DEPTH_TEST);

	struct camera camera = {
		.speed = 5.0f,
		.position = { 0.0f, 0.0f, 5.0f },
		.front = { 0.0f, 0.0f, -1.0f },
		.up = { 0.0f, 1.0f, 0.0f },
		.first_mouse = true,
		.near = 0.1f, .far = 1000.0f
	};

	glfwSetWindowUserPointer(window, &camera);
	glfwSetCursorPosCallback(window, camera_look);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	struct shader* invert_shader = { 0 };
	struct shader* toon_shader = { 0 };
	struct shader* crt_shader = { 0 };
	struct shader* tonemap_shader = { 0 };
	struct shader* antialias_shader = { 0 };
	struct shader* blur_shader = { 0 };
	struct shader* ca_shader = { 0 };
	struct shader* bright_extract_shader = { 0 };
	struct shader* bloom_shader = { 0 };
	struct shader* outline_shader = { 0 };
	struct shader* sprite_shader = { 0 };

	struct shader_config shaders = { 0 };
	shaders.lit           = new_shader_from_file("res/shaders/lit.glsl");
	shaders.shadowmap     = new_shader_from_file("res/shaders/shadowmap.glsl");
	shaders.pick          = new_shader_from_file("res/shaders/pick.glsl");
	shaders.normal        = new_shader_from_file("res/shaders/normal.glsl");
	invert_shader         = new_shader_from_file("res/shaders/invert.glsl");
	toon_shader           = new_shader_from_file("res/shaders/toon.glsl");
	crt_shader            = new_shader_from_file("res/shaders/crt.glsl");
	tonemap_shader        = new_shader_from_file("res/shaders/tonemap.glsl");
	antialias_shader      = new_shader_from_file("res/shaders/antialias.glsl");
	blur_shader           = new_shader_from_file("res/shaders/blur.glsl");
	ca_shader             = new_shader_from_file("res/shaders/ca.glsl");
	bright_extract_shader = new_shader_from_file("res/shaders/bright_extract.glsl");
	bloom_shader          = new_shader_from_file("res/shaders/bloom.glsl");
	outline_shader        = new_shader_from_file("res/shaders/outline.glsl");
	sprite_shader         = new_shader_from_file("res/shaders/sprite.glsl");

	struct obj_model model = { 0 };
	load_obj("res/monkey.obj", &model);
	struct model* monkey = new_model_from_obj(&model);
	deinit_obj(&model);

	load_obj("res/soulspear.obj", &model);
	struct model* soulspear = new_model_from_obj(&model);
	deinit_obj(&model);

	load_obj("res/torus.obj", &model);
	struct model* torus = new_model_from_obj(&model);
	deinit_obj(&model);
	
	load_obj("res/simplescene.obj", &model);
	struct model* scene = new_model_from_obj(&model);
	deinit_obj(&model);

	struct renderer* renderer = new_renderer(shaders);
	struct renderer2d* renderer2d = new_renderer2d(sprite_shader);

	vector_push(renderer->postprocessors, bright_extract_shader);
	vector_push(renderer->postprocessors, blur_shader);
	vector_push(renderer->postprocessors, blur_shader);
	vector_push(renderer->postprocessors, bloom_shader);
	/* vector_push(renderer->postprocessors, ca_shader); */
	vector_push(renderer->postprocessors, tonemap_shader);
//	vector_push(renderer->postprocessors, toon_shader);
//	vector_push(renderer->postprocessors, outline_shader);
	vector_push(renderer->postprocessors, antialias_shader);

	vector_push(renderer->drawlist, ((struct drawlist_item) { monkey, m4f_translate(m4f_identity(), make_v3f(3.0f, 1.5f, 1.0f)) }));
	vector_push(renderer->drawlist, ((struct drawlist_item) { scene, m4f_identity() }));
	vector_push(renderer->drawlist, ((struct drawlist_item) { soulspear, m4f_translate(m4f_identity(), make_v3f(0.0f, 1.0f, 3.0f)) }));
	vector_push(renderer->drawlist, ((struct drawlist_item) { soulspear, m4f_translate(m4f_identity(), make_v3f(3.0f, 1.0f, 3.0f)) }));
	vector_push(renderer->drawlist, ((struct drawlist_item) { soulspear, m4f_translate(m4f_identity(), make_v3f(6.0f, 1.0f, 3.0f)) }));
	vector_push(renderer->drawlist, ((struct drawlist_item) { torus,     m4f_translate(m4f_identity(), make_v3f(0.0f, 1.0f, 1.0f)) }));

	vector_push(renderer->lights, ((struct light) {
		.type = light_point,
		.ambient = { 0 },
		.specular = { 1.0f, 0.0f, 0.0f },
		.diffuse = { 1.0f, 0.0f, 0.0f },
		.intensity = 10.0f,
		.as.point = {
			.position = { 1.0f, 0.5f, 6.0f },
			.range = 3.0f
		}
	}));

	vector_push(renderer->lights, ((struct light) {
		.type = light_point,
		.ambient = { 0 },
		.specular = { 0.0f, 1.0f, 1.0f },
		.diffuse = { 0.0f, 1.0f, 1.0f },
		.intensity = 10.0f,
		.as.point = {
			.position = { 0.0f, 2.0f, -3.0f },
			.range = 10.0f
		}
	}));

	vector_push(renderer->lights, ((struct light) {
		.type = light_directional,
			.ambient = { 0 },
			.specular = { 1.0f, 1.0f, 1.0f },
			.diffuse = { 1.0f, 1.0f, 1.0f },
			.intensity = 1.0f,
			.as.directional = {
				.direction = { 0.3f, 1.0f, 0.3f },
				.cast_shadows = true
		}
	}));

	struct font* consolas = new_font_from_file("res/consolas.ttf", 14.0f);

	f64 ts = 0.0f;
	f64 now = glfwGetTime(), last = glfwGetTime();
	bool holding_mouse = true;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		shader_reload(shaders.lit);
		shader_reload(shaders.shadowmap);
		shader_reload(shaders.pick);
		shader_reload(shaders.normal);
		shader_reload(invert_shader);
		shader_reload(toon_shader);
		shader_reload(crt_shader);
		shader_reload(tonemap_shader);
		shader_reload(antialias_shader);
		shader_reload(blur_shader);
		shader_reload(ca_shader);
		shader_reload(bright_extract_shader);
		shader_reload(bloom_shader);
		shader_reload(outline_shader);
		shader_reload(sprite_shader);

		if (glfwGetKey(window, GLFW_KEY_C) && vector_count(log_buffer)) {
			/* Clear the log. */
			log_buffer[0] = '\0';
			vector_clear(log_buffer);
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {	
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			holding_mouse = false;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			holding_mouse = true;
			camera.first_mouse = true;
		}

		if (!holding_mouse && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
			f64 x, y;
			glfwGetCursorPos(window, &x, &y);
			renderer_mouse_pick(renderer, &camera, make_v2i((i32)x, (i32)y));
		}

		if (!holding_mouse && renderer->selected) {
			struct drawlist_item* item = &renderer->drawlist[renderer->selected - 1];

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				item->transform = m4f_translate(item->transform,
					v3f_scale(make_v3f(0.0f, 0.0f, 3.0f), (f32)ts));
			}
			
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				item->transform = m4f_translate(item->transform,
					v3f_scale(make_v3f(0.0f, 0.0f, -3.0f), (f32)ts));
			}

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				item->transform = m4f_translate(item->transform,
					v3f_scale(make_v3f(3.0f, 0.0f, 0.0f), (f32)ts));
			}

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				item->transform = m4f_translate(item->transform,
					v3f_scale(make_v3f(-3.0f, 0.0f, 0.0f), (f32)ts));
			}

			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
				item->transform = m4f_translate(item->transform,
					v3f_scale(make_v3f(0.0f, 3.0f, 0.0f), (f32)ts));
			}

			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
				item->transform = m4f_translate(item->transform,
					v3f_scale(make_v3f(0.0f, -3.0f, 0.0f), (f32)ts));
			}
		} else {
			update_camera(window, &camera, ts);
		}

		camera.look_enable = holding_mouse;

		/* 3-D pass. */
		clear_render_target(null);
		renderer_draw(renderer, &camera);

		/* 2-D pass. */
		bind_render_target(null);
		disable_cull_face();
		disable_depth_test();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (log_buffer) {
			render_text(renderer2d, consolas, log_buffer, make_v2f(0.0f, 0.0f));
		}

		enable_depth_test();
		glDisable(GL_BLEND);

		glfwSwapBuffers(window);

		now = glfwGetTime();
		ts = now - last;
		last = now;
	}

	free_font(consolas);

	free_renderer(renderer);
	free_renderer2d(renderer2d);

	free_model(monkey);
	free_model(soulspear);
	free_model(torus);
	free_model(scene);

	free_shader(shaders.lit);
	free_shader(shaders.shadowmap);
	free_shader(shaders.pick);
	free_shader(shaders.normal);
	free_shader(invert_shader);
	free_shader(toon_shader);
	free_shader(crt_shader);
	free_shader(tonemap_shader);
	free_shader(antialias_shader);
	free_shader(blur_shader);
	free_shader(ca_shader);
	free_shader(bright_extract_shader);
	free_shader(bloom_shader);
	free_shader(outline_shader);
	free_shader(sprite_shader);

	glfwDestroyWindow(window);

	glfwTerminate();
}

bool read_raw(const char* filename, u8** buf, u64* size, bool term) {
	if (!buf) { return false; }

	*buf = null;
	size ? *size = 0 : 0;

	FILE* file = fopen(filename, "rb");
	if (!file) {
		print_log("Failed to fopen file %s\n", filename);
		return false;
	}

	fseek(file, 0, SEEK_END);
	const u64 file_size = ftell(file);
	rewind(file);

	*buf = malloc(file_size + (term ? 1 : 0));
	const u64 bytes_read = fread(*buf, sizeof(char), file_size, file);
	if (bytes_read < file_size) {
		print_log("Failed to read file: %s\n", filename);
	}

	if (term) {
		*((*buf) + file_size) = '\0';
	}

	if (size) {
		*size = file_size + (term ? 1 : 0);
	}

	fclose(file);

	return true;
}

char* copy_string(const char* src) {
	const u32 len = (u32)strlen(src);

	char* s = malloc(len + 1);
	memcpy(s, src, len);
	s[len] = 0;

	return s;
}

u64 elf_hash(const u8* data, u32 size) {
	u64 hash = 0, x = 0;

	for (u32 i = 0; i < size; i++) {
		hash = (hash << 4) + data[i];
		if ((x = hash & 0xF000000000LL) != 0) {
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}

	return (hash & 0x7FFFFFFFFF);
}

void print_log(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	char message[1024];
	vsnprintf(message, 1023, fmt, args);

	va_end(args);

	u32 len = (u32)strlen(message);

	if (vector_count(log_buffer)) {
		*vector_end(log_buffer) = message[0];
	}

	for (u32 i = 0; i < len; i++) {
		vector_push(log_buffer, message[i]);
	}

	if (vector_count(log_buffer)) {
		*vector_end(log_buffer) = '\0';
	}

	printf("%s\n", message);
}

#ifdef _WIN32

#include <windows.h>

char* get_file_path(const char* name) {
	char* r = malloc(256);

	if (!GetFullPathNameA(name, 256, r, null)) {
		free(r);
		return null;
	}

	u32 len = (u32)strlen(r);

	char* cut = r + len;
	while (cut > r && *cut != '\\') {
		*cut = '\0';
		cut--;
	}

	return r;
}

u64 file_mod_time(const char* name) {
	HANDLE file = CreateFileA(name, GENERIC_READ, 0, null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null);

	if (file == INVALID_HANDLE_VALUE) {
		return 0;
	}

	FILETIME ft;

	if (!GetFileTime(file, null, null, &ft)) {
		return 0;
	}

	CloseHandle(file);

	LARGE_INTEGER date, adjust;
	date.HighPart = ft.dwHighDateTime;
	date.LowPart = ft.dwLowDateTime;

	adjust.QuadPart = 11644473600000 * 10000;

	date.QuadPart -= adjust.QuadPart;

	return date.QuadPart / 10000000;
}

#else

#include <sys/stat.h>

char* get_file_path(const char* name) {
	char* r = realpath(name, null);
	if (!r) { return null; }

	u32 len = (u32)strlen(r);

	char* cut = r + len;
	while (cut > r && *cut != '/') {
		*cut = '\0';
		cut--;
	}

	return r;
}

u64 file_mod_time(const char* name) {
	struct stat s;
	if (stat(name, &s) != -1) {
		return s.st_mtime;
	}

	return 0;
}

#endif
