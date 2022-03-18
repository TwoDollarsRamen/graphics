#include "mesh.h"

#define els_per_vert 8

bool vertex_equal(v3f pos, v3f norm, v2f uv, float* verts) {
	return (
		pos.x  == verts[0] &&
		pos.y  == verts[1] &&
		pos.z  == verts[2] &&
		uv.x   == verts[3] &&
		uv.y   == verts[4] &&
		norm.x == verts[5] &&
		norm.y == verts[6] &&
		norm.z == verts[7]);
}

struct mesh* new_mesh_from_obj(struct obj_model* model) {
	struct mesh* mesh = calloc(1, sizeof(struct mesh));

	f32* verts = malloc(vector_count(model->vertices) * els_per_vert * sizeof(f32));
	u32* indices = malloc(vector_count(model->vertices) * sizeof(u32));

	u32 vertex_count = 0;
	u32 index_count = 0;

	for (u32 i = 0; i < vector_count(model->vertices); i++) {
		bool new = true;

		v3f pos = model->positions[model->vertices[i].position];
		v3f norm = model->normals[model->vertices[i].normal];
		v2f uv = model->uvs[model->vertices[i].uv];

		for (u32 ii = 0; ii < vertex_count; ii++) {
			if (vertex_equal(pos, norm, uv, verts + ii * els_per_vert)) {
				indices[index_count++] = ii;
				new = false;
				break;
			}
		}

		if (new) {
			u32 idx = (vertex_count) * els_per_vert;

			verts[idx + 0] = pos.x;
			verts[idx + 1] = pos.y;
			verts[idx + 2] = pos.z;
			verts[idx + 3] = uv.x;
			verts[idx + 4] = uv.y;
			verts[idx + 5] = norm.x;
			verts[idx + 6] = norm.y;
			verts[idx + 7] = norm.z;

			indices[index_count++] = vertex_count;
			vertex_count++;
		}
	}

	init_vb(&mesh->vb, vb_static | (model->triangulated ? vb_tris : vb_quads));
	bind_vb_for_edit(&mesh->vb);
	push_vertices(&mesh->vb, verts, vertex_count * els_per_vert);
	push_indices(&mesh->vb, indices, index_count);
	configure_vb(&mesh->vb, 0, 3, els_per_vert, 0); /* position (vec3) */
	configure_vb(&mesh->vb, 1, 2, els_per_vert, 3); /* uv (vec2) */
	configure_vb(&mesh->vb, 2, 3, els_per_vert, 5); /* normal (vec3) */
	bind_vb_for_edit(null);

	free(verts);
	free(indices);

	return mesh;
}

void free_mesh(struct mesh* mesh) {
	deinit_vb(&mesh->vb);
	free(mesh);
}

void draw_mesh(struct mesh* mesh) {
	bind_vb_for_draw(&mesh->vb);
	draw_vb(&mesh->vb);
}
