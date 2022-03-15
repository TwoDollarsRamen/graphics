#include <stdlib.h>
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "common.h"
#include "video.h"

i32 main() {
	glfwInit();

	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics", null, null);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);

	gladLoadGL();

	struct shader shader = { 0 };
	init_shader_from_file(&shader, "res/basic.glsl");

	f32 verts[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f
	};

	u32 indices[] = {
		0, 1, 2
	};

	struct vertex_buffer vb = { 0 };
	init_vb(&vb, vb_static | vb_tris);
	bind_vb_for_edit(&vb);
	push_vertices(&vb, verts, 9);
	push_indices(&vb, indices, 3);
	configure_vb(&vb, 0, 3, 3, 0);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		bind_shader(&shader);
		bind_vb_for_draw(&vb);
		draw_vb(&vb);

		glfwSwapBuffers(window);
	}

	deinit_vb(&vb);
	deinit_shader(&shader);

	glfwDestroyWindow(window);

	glfwTerminate();
}

bool read_raw(const char* filename, u8** buffer, u64* size, bool term) {
	if (!*buffer) { return false; }

	FILE* file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "Failed to `fopen' file `%s'\n", filename);
		return false;
	}

	fseek(file, 0, SEEK_END);
	u64 fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	*buffer = malloc(fsize + term ? 1 : 0);

	u64 read = fread(*buffer, 1, fsize, file);

	if (size) {
		*size = read + term ? 1 : 0;
	}

	if (read < fsize) {
		fprintf(stderr, "`fread' of `%s' failed.\n", filename);
		fclose(file);
		return false;
	}

	if (term) {
		(*buffer)[read] = '\0';
	}

	fclose(file);

	return true;
}