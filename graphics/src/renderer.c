#include <stdio.h>
#include <math.h>

#include <glad/glad.h>

#include "renderer.h"

struct renderer* new_renderer(struct shader_config config) {
	struct renderer* renderer = calloc(1, sizeof(struct renderer));

	renderer->selected = 0;

	renderer->shaders = config;

	renderer->ambient = make_v3f(1.0, 1.0, 1.0);
	renderer->ambient_intensity = 0.1f;

	init_render_target(&renderer->postprocess_ignore_fb, screen_w, screen_h);
	init_render_target(&renderer->scene_fb, screen_w, screen_h);
	init_render_target(&renderer->fb0, screen_w, screen_h);
	init_render_target(&renderer->fb1, screen_w, screen_h);

	init_depth_map(&renderer->shadowmap, 1024, 1024);

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
	deinit_render_target(&renderer->scene_fb);
	deinit_render_target(&renderer->postprocess_ignore_fb);
	deinit_render_target(&renderer->fb0);
	deinit_render_target(&renderer->fb1);

	free(renderer);
}

/* Transform an AABB by a matrix. This could be used so that
 * a rotated or scaled object could fit inside an AABB. */
static struct aabb transform_aabb(struct aabb aabb, m4f matrix) {
	v3f corners[] = {
		aabb.min,
		make_v3f(aabb.min.x, aabb.max.y, aabb.min.z),
		make_v3f(aabb.min.x, aabb.max.y, aabb.max.z),
		make_v3f(aabb.min.x, aabb.min.y, aabb.max.z),
		make_v3f(aabb.max.x, aabb.min.y, aabb.min.z),
		make_v3f(aabb.max.x, aabb.max.y, aabb.min.z),
		aabb.max,
		make_v3f(aabb.max.x, aabb.min.y, aabb.max.z)
	};

	struct aabb result = {
		.min = { INFINITY, INFINITY, INFINITY },
		.max = { -INFINITY, -INFINITY, -INFINITY }
	};

	for (u32 i = 0; i < 8; i++) {
		v4f point = m4f_transform(matrix, make_v4f(corners[i].x, corners[i].y, corners[i].z, 1.0f));

		result.min.x = minimum(result.min.x, point.x);
		result.min.y = minimum(result.min.y, point.y);
		result.min.z = minimum(result.min.z, point.z);
		result.max.x = maximum(result.max.x, point.x);
		result.max.y = maximum(result.max.y, point.y);
		result.max.z = maximum(result.max.z, point.z);
	}

	return result;
}

static m4f get_light_matrix(struct light* light, struct aabb scene) {
	m4f view = m4f_lookat(
		light->as.directional.direction,
		make_v3f(0.0f, 0.0f, 0.0f),
		make_v3f(0.0f, 1.0f, 0.0f));

	scene = transform_aabb(scene, view);

	float z_mul = 3.0f;
	if (scene.min.z < 0.0f) {
		scene.min.z *= z_mul;
	} else {
		scene.min.z /= z_mul;
	}

	if (scene.max.z < 0.0f) {
		scene.max.z /= z_mul;
	} else {
		scene.max.z *= z_mul;
	}

	m4f proj = m4f_orth(
		scene.min.x, scene.max.x,
		scene.min.y, scene.max.y,
		scene.min.z, scene.max.z);
	
	return m4f_mul(proj, view);
}

void renderer_draw(struct renderer* renderer, struct camera* camera) {
	m4f camera_proj = get_camera_proj(camera, make_v2i(screen_w, screen_h));
	m4f camera_view = get_camera_view(camera);

	enable_cull_face();

	m4f light_matrix;

	struct aabb scene_aabb = {
		.min = { INFINITY, INFINITY, INFINITY },
		.max = { -INFINITY, -INFINITY, -INFINITY }
	};

	/* Generate an AABB of the scene so that the shadow map can get a tight fit. */
	for (struct drawlist_item* vector_iter(renderer->drawlist, item)) {
		struct model* model = item->model;

		struct aabb model_aabb = transform_aabb(model->aabb, item->transform);

		scene_aabb.min.x = minimum(scene_aabb.min.x, model_aabb.min.x);
		scene_aabb.min.y = minimum(scene_aabb.min.y, model_aabb.min.y);
		scene_aabb.min.z = minimum(scene_aabb.min.z, model_aabb.min.z);
		scene_aabb.max.x = maximum(scene_aabb.max.x, model_aabb.max.x);
		scene_aabb.max.y = maximum(scene_aabb.max.y, model_aabb.max.y);
		scene_aabb.max.z = maximum(scene_aabb.max.z, model_aabb.max.z);
	}

	/* Render the shadow-map. */
	bool shadowmap_exists = false;
	for (struct light* vector_iter(renderer->lights, light)) {
		if (light->type == light_directional && light->as.directional.cast_shadows) {
			light_matrix = get_light_matrix(light, scene_aabb);

			clear_depth_map(&renderer->shadowmap);

			struct shader* s = renderer->shaders.shadowmap;
			bind_shader(s);

			shader_set_m4f(s, "light_matrix", light_matrix);

			for (struct drawlist_item* vector_iter(renderer->drawlist, item)) {
				struct model* model = item->model;
	
				shader_set_m4f(s, "transform", item->transform);

				draw_model(model, s);
			}

			unbind_depth_map(&renderer->shadowmap);

			shadowmap_exists = true;
			break;
		}
	}

	if (vector_count(renderer->postprocessors) > 0) {
		if (renderer->selected) {
			/* Draw only the selected object into a framebuffer
			 * so that the post-processing step can ignore it. */
			struct drawlist_item* item = &renderer->drawlist[renderer->selected - 1];

			struct model* model = item->model;

			struct shader* s = renderer->shaders.pick;
			bind_shader(s);

			clear_render_target(&renderer->postprocess_ignore_fb);

			shader_set_m4f(s, "projection", camera_proj);
			shader_set_m4f(s, "view", camera_view);
			shader_set_m4f(s, "transform", item->transform);

			shader_set_v3f(s, "color", make_v3f(1.0f, 1.0f, 1.0f));

			draw_model(model, s);
		}

		clear_render_target(&renderer->scene_fb);
	}

	u32 i = 0;
	for (struct drawlist_item* vector_iter(renderer->drawlist, item)) {
		struct model* model = item->model;

		struct shader* s = renderer->shaders.lit;

		bind_shader(s);

		shader_set_m4f(s, "projection", camera_proj);
		shader_set_m4f(s, "view", camera_view);

		shader_set_m4f(s, "transform", item->transform);

		shader_set_v3f(s, "world.camera_pos", camera->position);
		shader_set_v3f(s, "world.ambient", renderer->ambient);
		shader_set_f(s, "world.ambient_intensity", renderer->ambient_intensity);

		shader_set_b(s, "selected", renderer->selected == i + 1);

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

		shader_set_b(s, "use_shadows", shadowmap_exists);
		if (shadowmap_exists) {
			bind_depth_map_output(&renderer->shadowmap, 0);
			shader_set_i(s, "shadowmap", 0);
			shader_set_m4f(s, "light_matrix", light_matrix);
		}

		shader_set_u(s, "point_light_count", point_count);
		shader_set_u(s, "directional_light_count", directional_count);

		draw_model(model, s);

		i++;
	}
	
	disable_cull_face();

	/* Run the post-processing on the scene, using the ping-pong frame buffers. */
	if (vector_count(renderer->postprocessors) > 0) {
		struct render_target* last_target = &renderer->fb0;

		for (u32 i = 0; i < vector_count(renderer->postprocessors); i++) {
			struct shader* shader = renderer->postprocessors[i];

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

			if (i == 0) {
				bind_render_target_output(&renderer->scene_fb, 0);
			} else {
				bind_render_target_output(last_target, 0);
			}

			shader_set_i(shader, "input_texture", 0);
			bind_render_target_output(&renderer->scene_fb, 1);
			shader_set_i(shader, "original_texture", 1);
			bind_render_target_output(&renderer->postprocess_ignore_fb, 2);
			shader_set_i(shader, "ignore", 2);
			shader_set_v2f(shader, "screen_size", make_v2f(screen_w, screen_h));

			bind_vb_for_draw(&renderer->fullscreen_quad);
			draw_vb(&renderer->fullscreen_quad);
			bind_vb_for_draw(null);

			last_target = target;
		}
	}
}

void renderer_mouse_pick(struct renderer* renderer, struct camera* camera, v2i mouse_pos) {
	/* Mouse picking is implemented by rendering the scene into a framebuffer,
	 * where each object's colour represents it's ID. Then, the colour of the
	 * pixel on the framebuffer at the mouse position is sampled and converted
	 * back into an object ID. */

	m4f camera_proj = get_camera_proj(camera, make_v2i(screen_w, screen_h));
	m4f camera_view = get_camera_view(camera);

	clear_render_target(&renderer->fb0);

	struct shader* s = renderer->shaders.pick;

	bind_shader(s);

	shader_set_m4f(s, "projection", camera_proj);
	shader_set_m4f(s, "view", camera_view);

	u32 i = 0;
	for (struct drawlist_item* vector_iter(renderer->drawlist, item)) {
		struct model* model = item->model;

		u32 id = i + 1;

		i32 r = (id & 0x000000FF) >> 0;
		i32 g = (id & 0x0000FF00) >> 8;
		i32 b = (id & 0x00FF0000) >> 16;

		shader_set_m4f(s, "transform", item->transform);
		shader_set_v3f(s, "color",
			make_v3f((f32)r / 255.0f, (f32)g / 255.0f, (f32)b / 2.0f));

		draw_model(model, s);

		i++;
	}

	glReadBuffer(GL_COLOR_ATTACHMENT0);

	u8 data[3];
	glReadPixels(mouse_pos.x, screen_h - mouse_pos.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &data);

	if (data[0] == 0 && data[1] == 0 && data[2] == 0) {
		renderer->selected = 0;
	} else {
		renderer->selected =
			data[0] + 
			data[1] * 256 +
			data[2] * 256 * 256;
	}

	glReadBuffer(GL_NONE);

	bind_render_target(null);
}

void update_camera(GLFWwindow* window, struct camera* camera, f64 ts) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->position = v3f_add(camera->position, v3f_scale(camera->front, camera->speed * (f32)ts));
	}
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->position = v3f_sub(camera->position, v3f_scale(camera->front, camera->speed * (f32)ts));
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->position = v3f_sub(camera->position, v3f_scale(v3f_normalised(v3f_cross(camera->front, camera->up)), camera->speed * (f32)ts));
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->position = v3f_add(camera->position, v3f_scale(v3f_normalised(v3f_cross(camera->front, camera->up)), camera->speed * (f32)ts));
	}
}

m4f get_camera_view(struct camera* camera) {
	return m4f_lookat(camera->position, v3f_add(camera->position, camera->front), camera->up);
}

m4f get_camera_proj(struct camera* camera, v2i screen_size) {
	return m4f_pers(75.0f, (f32)screen_size.x / (f32)screen_size.y, camera->near, camera->far);
}

void camera_look(GLFWwindow* window, f64 x, f64 y) {
	struct camera* camera = glfwGetWindowUserPointer(window);
	if (!camera) { return; }

	if (!camera->look_enable) { return; }

	if (camera->first_mouse) {
		camera->last_x = (f32)x;
		camera->last_y = (f32)y;
		camera->first_mouse = false;
	}

	f32 xoffset = (f32)x - camera->last_x;
	f32 yoffset = camera->last_y - (f32)y;
	camera->last_x = (f32)x;
	camera->last_y = (f32)y;

	f32 sensitivity = 0.1f;
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
			cosf(toradf(camera->yaw)) * cosf(toradf(camera->pitch)),
			sinf(toradf(camera->pitch)),
			sinf(toradf(camera->yaw)) * cosf(toradf(camera->pitch))
		)
	);
}
