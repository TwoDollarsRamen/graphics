#pragma once

#include <GLFW/glfw3.h>

#include "common.h"
#include "maths.h"
#include "mesh.h"
#include "vector.h"

struct camera;

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
			bool cast_shadows;
		} directional;
	} as;
};

struct shader_config {
	struct shader* lit;
	struct shader* shadowmap;
	struct shader* pick;
	struct shader* g;
	struct shader* ao;
	struct shader* ao_blur;
};

struct drawlist_item {
	struct model* model;
	m4f transform;
};

struct renderer {
	vector(struct drawlist_item) drawlist;
	vector(struct light) lights;

	struct depth_map shadowmap;

	struct shader_config shaders;

	v3f ambient;
	float ambient_intensity;

	u32 selected;

	struct texture ao_noise;
	vector(v3f) ao_kernel;

	struct vertex_buffer fullscreen_quad;
	struct render_target postprocess_ignore_fb;
	struct render_target gbuffer;
	struct render_target* scene_fb;
	struct render_target* fb0, * fb1; /* Ping-pong framebuffers, for post-processing. */

	vector(struct shader*) postprocessors;
};

struct renderer* new_renderer(struct shader_config config);
void free_renderer(struct renderer* renderer);
void renderer_draw(struct renderer* renderer, struct camera* camera);
void renderer_mouse_pick(struct renderer* renderer, struct camera* camera, v2i mouse_pos);

struct camera {
	f32 speed;

	v3f position;
	v3f front, up;

	bool first_mouse;
	bool look_enable;

	f32 last_x, last_y;
	f32 yaw, pitch;

	float near, far;
};

void update_camera(GLFWwindow* window, struct camera* camera, f64 ts);
m4f get_camera_view(struct camera* camera);
m4f get_camera_proj(struct camera* camera, v2i screen_size);
void camera_look(GLFWwindow* window, f64 x, f64 y);
