#pragma once

#include "common.h"
#include "maths.h"
#include "vector.h"

struct obj_vertex { /* Indexes into the position, normal and UV vectors. */
	u32 position, uv, normal;
};

struct obj_model {
	vector(struct obj_vertex) vertices;

	vector(v3f) positions;
	vector(v2f) uvs;
	vector(v3f) normals;

	bool triangulated;
};

bool load_obj(const char* filename, struct obj_model* model);
void deinit_obj(struct obj_model* model);
