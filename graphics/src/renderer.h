#pragma once

#include <GLFW/glfw3.h>

#include "common.h"
#include "maths.h"
#include "mesh.h"
#include "vector.h"

enum {
	light_point = 0,
	light_directional
};

struct light {
	u32 type;

	v3f ambient, specular, diffuse;
	
	f32 intensity;

	union {
		struct {
			v3f position;
			f32 range;
		} point;

		struct {
			v3f direction;
		} directional;
	} as;
};

struct shader_config {
	struct shader lit;
};

struct renderer {
	vector(struct model*) drawlist;
	vector(struct light) lights;

	struct shader_config shaders;

	v3f ambient;
	float ambient_intensity;
};

struct renderer* new_renderer(struct shader_config config);
void free_renderer(struct renderer* renderer);
void renderer_draw(struct renderer* renderer, struct camera* camera);

struct camera {
	f32 speed;

	v3f position;
	v3f front, up;

	bool first_mouse;

	f32 last_x, last_y;
	f32 yaw, pitch;
};

void update_camera(GLFWwindow* window, struct camera* camera, f64 ts);
m4f get_camera_view(struct camera* camera);
m4f get_camera_proj(struct camera* camera, v2i screen_size);
void camera_look(GLFWwindow* window, f64 x, f64 y);