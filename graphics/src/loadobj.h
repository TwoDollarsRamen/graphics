#pragma once

#include <string.h>

#include "common.h"
#include "maths.h"
#include "vector.h"
#include "table.h"

struct obj_vertex { /* Indexes into the position, normal and UV vectors. */
	u32 position, uv, normal;
}; 

struct obj_material {
	v3f ambient;
	v3f diffuse;
	v3f specular;
	v3f emissive;
	f32 specular_exponent;
	f32 alpha;

	char* ambient_map_path;
	char* diffuse_map_path;
	char* specular_map_path;
	char* bump_map_path;
	char* displacement_map_path;
	char* stencil_map_path;
};

struct obj_mesh {
	vector(struct obj_vertex) vertices;

	/* Key into the `materials' table in the
	 * `obj_model' struct. */
	char* material_name;
};

/* An intermediate format between the raw
 * text and an actual mesh that can be drawn
 * to the screen. Further pre-processing is done
 * in the `process_mesh' function in `mesh.c'. */
struct obj_model {
	bool has_root_mesh;
	struct obj_mesh root_mesh;

	vector(v3f) positions;
	vector(v2f) uvs;
	vector(v3f) normals;

	vector(struct obj_mesh) meshes;

	struct table* materials;
};

bool load_obj(const char* filename, struct obj_model* model);
void deinit_obj(struct obj_model* model);
