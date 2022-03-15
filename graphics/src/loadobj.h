#pragma once

#include "common.h"
#include "maths.h"

struct obj_vertex {
	v3f position;
	v3f normal;
	v2f uv;
};

struct obj_model {
	struct obj_vertex* positions;
	u32 position_count;

	u32* indices;
	u32 index_count;
};

void load_obj(const char* filename, struct obj_model* model);