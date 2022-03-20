#pragma once

#include "loadobj.h"
#include "maths.h"
#include "video.h"

struct mesh {
	struct vertex_buffer vb;

	v3f ambient, diffuse, specular;

	bool use_diffuse_map;
	struct texture diffuse_map;

	bool use_specular_map;
	struct texture specular_map;
};

struct model {
	vector(struct mesh) meshes;

	m4f transform;
};

struct model* new_model_from_obj(struct obj_model* model);
void free_model(struct model* model);
void draw_model(struct model* model, struct shader* shader);
