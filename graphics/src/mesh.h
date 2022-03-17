#pragma once

#include "loadobj.h"
#include "video.h"

struct mesh {
	struct vertex_buffer vb;
};

struct mesh* new_mesh_from_obj(struct obj_model* model);
void free_mesh(struct mesh* mesh);

void draw_mesh(struct mesh* mesh);
