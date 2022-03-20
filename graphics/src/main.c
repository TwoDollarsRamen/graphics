#include <stdlib.h>
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "common.h"
#include "loadobj.h"
#include "mesh.h"
#include "vector.h"
#include "video.h"
#include "table.h"
#include "renderer.h"

i32 main() {
	glfwInit();

	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow* window = glfwCreateWindow(1366, 768, "Graphics", null, null);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);

	gladLoadGL();

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	struct obj_model model = { 0 };
	load_obj("res/monkey.obj", &model);
	struct model* monkey = new_model_from_obj(&model);
	deinit_obj(&model);

	struct shader lit_shader = { 0 };

	struct shader_config shaders = { 0 };
	init_shader_from_file(&shaders.lit, "res/lit.glsl");

	struct renderer* renderer = new_renderer(shaders);
	vector_push(renderer->drawlist, monkey);
	vector_push(renderer->lights, ((struct light) {
		.type = light_point,
		.ambient = { 0 },
		.specular = { 1.0f, 1.0f, 1.0f },
		.diffuse = { 1.0f, 1.0f, 1.0f },
		.intensity = 2.0f,
		.as.point = {
			.position = { 1.0f, 0.5f, 3.0f },
			.range = 10.0f
		}
	}));

	vector_push(renderer->lights, ((struct light) {
		.type = light_point,
		.ambient = { 0 },
		.specular = { 1.0f, 1.0f, 1.0f },
		.diffuse = { 1.0f, 0.0f, 1.0f },
		.intensity = 1.0f,
		.as.point = {
			.position = { 0.0f, -3.0f, 0.0f },
			.range = 10.0f
		}
	}));

	vector_push(renderer->lights, ((struct light) {
		.type = light_point,
		.ambient = { 0 },
		.specular = { 1.0f, 1.0f, 1.0f },
		.diffuse = { 0.0f, 1.0f, 1.0f },
		.intensity = 1.0f,
		.as.point = {
			.position = { 0.0f, 3.0f, 0.0f },
			.range = 10.0f
		}
	}));

	f64 ts = 0.0f;
	f64 now = glfwGetTime(), last = glfwGetTime();

	f32 rotation = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		monkey->transform = m4f_rotate(m4f_identity(), rotation, make_v3f(0.0f, 1.0f, 0.0f));

		renderer_draw(renderer);

		rotation += ts;

		glfwSwapBuffers(window);

		now = glfwGetTime();
		ts = now - last;
		last = now;
	}

	free_model(monkey);

	deinit_shader(&shaders.lit);

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

}

#else

char* get_file_path(const char* name) {
	char* r = realpath(name, null);

	u32 len = (u32)strlen(r);

	char* cut = r + len;
	while (cut > r && *cut != '/') {
		*cut = '\0';
		cut--;
	}

	return r;
}

#endif
