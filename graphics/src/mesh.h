#pragma once

#include "loadobj.h"
#include "video.h"

struct mesh {
	struct vertex_buffer vb;
};

struct model {
	vector(struct mesh) meshes;
};

struct model* new_model_from_obj(struct obj_model* model);
void free_model(struct model* model);
void draw_model(struct model* model);
