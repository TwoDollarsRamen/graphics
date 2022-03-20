#include <stdio.h>

#include "renderer.h"

struct renderer* new_renderer(struct shader_config config) {
	struct renderer* renderer = calloc(1, sizeof(struct renderer));

	renderer->shaders = config;

	renderer->ambient = make_v3f(0.0, 1.0, 1.0);
	renderer->ambient_intensity = 0.1f;

	return renderer;
}

void free_renderer(struct renderer* renderer) {
	free(renderer);
}

void renderer_draw(struct renderer* renderer) {
	for (struct model** vector_iter(renderer->drawlist, model_ptr)) {
		struct model* model = *model_ptr;

		struct shader* s = &renderer->shaders.lit;

		bind_shader(s);

		m4f projection = m4f_pers(75.0f, 1366.0f / 768.0f, 0.0f, 100.0f);
		shader_set_m4f(s, "projection", projection);

		m4f view = m4f_translate(m4f_identity(), make_v3f(0.0f, 0.0f, -5.0f));
		shader_set_m4f(s, "view", view);

		shader_set_m4f(s, "transform", model->transform);

		shader_set_v3f(s, "world.camera_pos", make_v3f(0.0f, 0.0f, -5.0f));
		shader_set_v3f(s, "world.ambient", renderer->ambient);
		shader_set_f(s, "world.ambient_intensity", renderer->ambient_intensity);

		/* Apply lights */
		u32 point_count = 0;
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
				default: break;
			}
		}

		shader_set_u(s, "point_light_count", point_count);

		draw_model(model, s);
	}
}
