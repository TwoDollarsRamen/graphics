#pragma once

#include "common.h"
#include "maths.h"
#include "mesh.h"
#include "vector.h"

enum {
	light_point = 0
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
void renderer_draw(struct renderer* renderer);
