#pragma once

#include "common.h"
#include "maths.h"
#include "vector.h"

struct obj_model {
	u32* indices;
	u32 index_count;

	bool has_uvs;
	bool has_normals;
};

bool load_obj(const char* filename, struct obj_model* model);
void deinit_obj(struct obj_model* model);
