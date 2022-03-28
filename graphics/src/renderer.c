#include <stdio.h>
#include <math.h>

#include "renderer.h"

struct renderer* new_renderer(struct shader_config config) {
	struct renderer* renderer = calloc(1, sizeof(struct renderer));

	renderer->shaders = config;

	renderer->ambient = make_v3f(1.0, 1.0, 1.0);
	renderer->ambient_intensity = 0.1f;

	init_render_target(&renderer->fb0, screen_w, screen_h);
	init_render_target(&renderer->fb1, screen_w, screen_h);

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

	init_vb(&renderer->fullscreen_quad, vb_static | vb_tris);
	bind_vb_for_edit(&renderer->fullscreen_quad);
	push_vertices(&renderer->fullscreen_quad, verts, 16 * sizeof(f32));
	push_indices(&renderer->fullscreen_quad, indices, 6);
	configure_vb(&renderer->fullscreen_quad, 0, 2, 4 * sizeof(f32), 0); /* position (vec2) */
	configure_vb(&renderer->fullscreen_quad, 1, 2, 4 * sizeof(f32), 8); /* uv (vec2) */
	bind_vb_for_edit(null);

	return renderer;
}

void free_renderer(struct renderer* renderer) {
	deinit_render_target(&renderer->fb0);
	deinit_render_target(&renderer->fb1);

	free(renderer);
}

void renderer_draw(struct renderer* renderer, struct camera* camera) {
	if (vector_count(renderer->postprocessors) > 0) {
		bind_render_target(&renderer->fb0);
		clear_render_target(&renderer->fb0);
	}

	enable_cull_face();

	for (struct model** vector_iter(renderer->drawlist, model_ptr)) {
		struct model* model = *model_ptr;

		struct shader* s = &renderer->shaders.lit;

		bind_shader(s);

		shader_set_m4f(s, "projection", get_camera_proj(camera, make_v2i(screen_w, screen_h)));
		shader_set_m4f(s, "view", get_camera_view(camera));

		shader_set_m4f(s, "transform", model->transform);

		shader_set_v3f(s, "world.camera_pos", camera->position);
		shader_set_v3f(s, "world.ambient", renderer->ambient);
		shader_set_f(s, "world.ambient_intensity", renderer->ambient_intensity);

		/* Apply lights */
		u32 point_count = 0;
		u32 directional_count = 0;
		for (struct light* vector_iter(renderer->lights, light)) {
			char name[256];

			switch (light->type) {
				case light_point:
					sprintf(name, "point_lights[%d].position", point_count);
					shader_set_v3f(s, name, light->as.point.position);

					sprintf(name, "point_lights[%d].range", point_count);
					shader_set_f(s, name, light->as.point.range);

					sprintf(name, "point_lights[%d].specular", point_count);
					shader_set_v3f(s, name, light->specular);

					sprintf(name, "point_lights[%d].diffuse", point_count);
					shader_set_v3f(s, name, light->diffuse);

					sprintf(name, "point_lights[%d].intensity", point_count);
					shader_set_f(s, name, light->intensity);

					point_count++;
					break;
				case light_directional:
					sprintf(name, "directional_lights[%d].direction", directional_count);
					shader_set_v3f(s, name, light->as.directional.direction);

					sprintf(name, "directional_lights[%d].specular", directional_count);
					shader_set_v3f(s, name, light->specular);

					sprintf(name, "directional_lights[%d].diffuse", directional_count);
					shader_set_v3f(s, name, light->diffuse);

					sprintf(name, "directional_lights[%d].intensity", directional_count);
					shader_set_f(s, name, light->intensity);

					directional_count++;
					break;
				default: break;
			}
		}

		shader_set_u(s, "point_light_count", point_count);
		shader_set_u(s, "directional_light_count", directional_count);

		draw_model(model, s);
	}
	
	disable_cull_face();

	if (vector_count(renderer->postprocessors) > 0) {
		struct render_target* last_target = &renderer->fb0;

		for (u32 i = 0; i < vector_count(renderer->postprocessors); i++) {
			struct shader* shader = renderer->postprocessors + i;

			struct render_target* target;
			if (i == vector_count(renderer->postprocessors) - 1) {
				target = null;
			} else if (i % 2 == 0) {
				target = &renderer->fb1;
			} else {
				target = &renderer->fb0;
			}

			bind_render_target(target);
			clear_render_target(target);

			bind_shader(shader);

			bind_render_target_output(last_target, 0);
			shader_set_i(shader, "input_texture", 0);
			shader_set_v2f(shader, "screen_size", make_v2f(screen_w, screen_h));

			bind_vb_for_draw(&renderer->fullscreen_quad);
			draw_vb(&renderer->fullscreen_quad);
			bind_vb_for_draw(null);

			last_target = target;
		}
	}
}

void update_camera(GLFWwindow* window, struct camera* camera, f64 ts) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->position = v3f_add(camera->position, v3f_scale(camera->front, camera->speed * ts));
	}
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->position = v3f_sub(camera->position, v3f_scale(camera->front, camera->speed * ts));
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->position = v3f_sub(camera->position, v3f_scale(v3f_normalised(v3f_cross(camera->front, camera->up)), camera->speed * ts));
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->position = v3f_add(camera->position, v3f_scale(v3f_normalised(v3f_cross(camera->front, camera->up)), camera->speed * ts));
	}
}

m4f get_camera_view(struct camera* camera) {
	return m4f_lookat(camera->position, v3f_add(camera->position, camera->front), camera->up);
}

m4f get_camera_proj(struct camera* camera, v2i screen_size) {
	return m4f_pers(75.0f, (f32)screen_size.x / (f32)screen_size.y, 0.0f, 100.0f);
}

void camera_look(GLFWwindow* window, f64 x, f64 y) {
	struct camera* camera = glfwGetWindowUserPointer(window);
	if (!camera) { return; }

	if (camera->first_mouse) {
		camera->last_x = (f32)x;
		camera->last_y = (f32)y;
		camera->first_mouse = false;
	}

	float xoffset = x - camera->last_x;
	float yoffset = camera->last_y - y;
	camera->last_x = x;
	camera->last_y = y;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera->yaw += xoffset;
	camera->pitch += yoffset;

	if (camera->pitch > 89.0f) {
		camera->pitch = 89.0f;
	}

	if (camera->pitch < -89.0f) {
		camera->pitch = -89.0f;
	}

	camera->front = v3f_normalised(
		make_v3f(
			cos(torad(camera->yaw)) * cos(torad(camera->pitch)),
			sin(torad(camera->pitch)),
			sin(torad(camera->yaw)) * cos(torad(camera->pitch))
		)
	);
}