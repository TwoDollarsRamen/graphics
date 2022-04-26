#pragma once

#include "loadobj.h"
#include "maths.h"
#include "video.h"

struct aabb {
	v3f min, max;
};

struct mesh {
	struct vertex_buffer vb;

	v3f ambient, diffuse, specular;
	v3f emissive;
	f32 shininess;
	f32 alpha;

	bool use_diffuse_map;
	struct texture diffuse_map;

	bool use_specular_map;
	struct texture specular_map;

	bool use_normal_map;
	struct texture normal_map;
};

struct model {
	vector(struct mesh) meshes;

	struct aabb aabb;
};

struct model* new_model_from_obj(struct obj_model* model);
void free_model(struct model* model);
void draw_model(struct model* model, struct shader* shader);
