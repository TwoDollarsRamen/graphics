#include <stdlib.h>
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "common.h"
#include "loadobj.h"
#include "mesh.h"
#include "renderer.h"
#include "table.h"
#include "vector.h"
#include "video.h"

void show_loading_screen(v2i screen_size) {
	struct texture image;
	struct shader shader;
	glClearColor(0.0705882352941f, 0.0823529411765f, 0.262745098039f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	init_texture(&image, "res/textures/loading.png");
	init_shader_from_file(&shader, "res/shaders/loading.glsl");

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

	bind_shader(&shader);
	bind_texture(&image, 0);
	shader_set_i(&shader, "t", 0);

	shader_set_m4f(&shader, "camera", m4f_orth(0.0f, (f32)screen_size.x, (f32)screen_size.y, 0.0f, -1.0f, 1.0f));
	shader_set_m4f(&shader, "transform",
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
	deinit_shader(&shader);
	deinit_vb(&vb);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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

	struct shader invert_shader = { 0 };
	struct shader toon_shader = { 0 };
	struct shader crt_shader = { 0 };
	struct shader tonemap_shader = { 0 };
	struct shader antialias_shader = { 0 };
	struct shader blur_shader = { 0 };
	struct shader ca_shader = { 0 };
	struct shader bright_extract_shader = { 0 };
	struct shader bloom_shader = { 0 };

	struct shader_config shaders = { 0 };
	init_shader_from_file(&shaders.lit, "res/shaders/lit.glsl");
	init_shader_from_file(&shaders.shadowmap, "res/shaders/shadowmap.glsl");
	init_shader_from_file(&invert_shader, "res/shaders/invert.glsl");
	init_shader_from_file(&toon_shader, "res/shaders/toon.glsl");
	init_shader_from_file(&crt_shader, "res/shaders/crt.glsl");
	init_shader_from_file(&tonemap_shader, "res/shaders/tonemap.glsl");
	init_shader_from_file(&antialias_shader, "res/shaders/antialias.glsl");
	init_shader_from_file(&blur_shader, "res/shaders/blur.glsl");
	init_shader_from_file(&ca_shader, "res/shaders/ca.glsl");
	init_shader_from_file(&bright_extract_shader, "res/shaders/bright_extract.glsl");
	init_shader_from_file(&bloom_shader, "res/shaders/bloom.glsl");

	struct obj_model model = { 0 };
	load_obj("res/monkey.obj", &model);
	struct model* monkey = new_model_from_obj(&model);
	deinit_obj(&model);

	load_obj("res/soulspear.obj", &model);
	struct model* soulspear = new_model_from_obj(&model);
	struct model* soulspear2 = new_model_from_obj(&model);
	soulspear->transform = m4f_translate(m4f_identity(), make_v3f(0.0f, 1.0f, 3.0f));
	soulspear2->transform = m4f_translate(m4f_identity(), make_v3f(2.5f, 1.0f, 3.0f));
	soulspear2->meshes[0].ambient = make_v3f(50.0f, 50.0f, 50.0f);
	deinit_obj(&model);

	struct renderer* renderer = new_renderer(shaders);
	vector_push(renderer->postprocessors, bright_extract_shader);
	vector_push(renderer->postprocessors, blur_shader);
	vector_push(renderer->postprocessors, blur_shader);
	vector_push(renderer->postprocessors, bloom_shader);
//	vector_push(renderer->postprocessors, ca_shader);
	vector_push(renderer->postprocessors, tonemap_shader);
	vector_push(renderer->postprocessors, antialias_shader);
	vector_push(renderer->drawlist, monkey);
	vector_push(renderer->drawlist, soulspear);
	vector_push(renderer->drawlist, soulspear2);
	vector_push(renderer->lights, ((struct light) {
		.type = light_point,
		.ambient = { 0 },
		.specular = { 1.0f, 0.0f, 0.0f },
		.diffuse = { 1.0f, 0.0f, 0.0f },
		.intensity = 1.0f,
		.as.point = {
			.position = { 1.0f, 0.5f, 2.0f },
			.range = 10.0f
		}
	}));

	vector_push(renderer->lights, ((struct light) {
		.type = light_point,
		.ambient = { 0 },
		.specular = { 0.0f, 1.0f, 1.0f },
		.diffuse = { 0.0f, 1.0f, 1.0f },
		.intensity = 1.0f,
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
			.intensity = 0.5f,
			.as.directional = {
				.direction = { 0.3f, 1.0f, 0.3f },
				.cast_shadows = true
		}
	}));

	f64 ts = 0.0f;
	f64 now = glfwGetTime(), last = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		clear_render_target(null);

		update_camera(window, &camera, ts);

		renderer_draw(renderer, &camera);

		glfwSwapBuffers(window);

		now = glfwGetTime();
		ts = now - last;
		last = now;
	}

	free_model(monkey);
	free_model(soulspear);

	deinit_shader(&shaders.lit);
	deinit_shader(&shaders.shadowmap);
	deinit_shader(&invert_shader);
	deinit_shader(&toon_shader);
	deinit_shader(&crt_shader);
	deinit_shader(&tonemap_shader);
	deinit_shader(&antialias_shader);
	deinit_shader(&blur_shader);
	deinit_shader(&ca_shader);
	deinit_shader(&bright_extract_shader);
	deinit_shader(&bloom_shader);

	free_renderer(renderer);

	glfwDestroyWindow(window);

	glfwTerminate();
}

bool read_raw(const char* filename, u8** buf, u64* size, bool term) {
	if (!buf) { return false; }

	*buf = null;
	size ? *size = 0 : 0;

	FILE* file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "Failed to fopen file %s\n", filename);
		return false;
	}

	fseek(file, 0, SEEK_END);
	const u64 file_size = ftell(file);
	rewind(file);

	*buf = malloc(file_size + (term ? 1 : 0));
	const u64 bytes_read = fread(*buf, sizeof(char), file_size, file);
	if (bytes_read < file_size) {
		printf("Failed to read file: %s\n", filename);
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

#else

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

#endif
