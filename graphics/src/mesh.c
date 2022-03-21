#include "mesh.h"

#define els_per_vert 8

static bool vertex_equal(v3f pos, v3f norm, v2f uv, float* verts) {
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

static void process_mesh(struct mesh* mesh, struct obj_model* omodel, struct obj_mesh* omesh) {
	mesh->ambient = mesh->diffuse = mesh->specular = make_v3f(1.0f, 1.0f, 1.0f);
	mesh->shininess = 32.0f;

	f32* verts = malloc(vector_count(omesh->vertices) * els_per_vert * sizeof(f32));
	u32* indices = malloc(vector_count(omesh->vertices) * sizeof(u32));

	u32 vertex_count = 0;
	u32 index_count = 0;

	for (u32 i = 0; i < vector_count(omesh->vertices); i++) {
		bool new = true;

		v3f pos = omodel->positions[omesh->vertices[i].position];
		v3f norm = omodel->normals[omesh->vertices[i].normal];
		v2f uv = omodel->uvs[omesh->vertices[i].uv];

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

	init_vb(&mesh->vb, vb_static | (omesh->triangulated ? vb_tris : vb_quads));
	bind_vb_for_edit(&mesh->vb);
	push_vertices(&mesh->vb, verts, vertex_count * els_per_vert);
	push_indices(&mesh->vb, indices, index_count);
	configure_vb(&mesh->vb, 0, 3, els_per_vert, 0); /* position (vec3) */
	configure_vb(&mesh->vb, 1, 2, els_per_vert, 3); /* uv (vec2) */
	configure_vb(&mesh->vb, 2, 3, els_per_vert, 5); /* normal (vec3) */
	bind_vb_for_edit(null);

	free(verts);
	free(indices);

	/* Material */
	if (omesh->material_name) {
		struct obj_material* material = table_get(omodel->materials, omesh->material_name);

		if (material) {
			mesh->ambient = material->ambient;
			mesh->diffuse = material->diffuse;
			mesh->specular = material->specular;
			mesh->shininess = material->specular_exponent;

			if (material->diffuse_map_path) {
				mesh->use_diffuse_map = true;

				init_texture(&mesh->diffuse_map, material->diffuse_map_path);
			}

			if (material->specular_map_path) {
				mesh->use_specular_map = true;

				init_texture(&mesh->specular_map, material->specular_map_path);
			}
		}
	}
}

struct model* new_model_from_obj(struct obj_model* omodel) {
	struct model* model = calloc(1, sizeof(struct model));

	model->transform = m4f_identity();

	if (omodel->has_root_mesh) {	
		struct mesh mesh = { 0 };
		vector_push(model->meshes, mesh);

		process_mesh(vector_end(model->meshes), omodel, &omodel->root_mesh);
	}

	for (u32 i = 0; i < vector_count(omodel->meshes); i++) {
		struct mesh mesh = { 0 };
		vector_push(model->meshes, mesh);

		process_mesh(vector_end(model->meshes), omodel, omodel->meshes + i);
	}

	return model;
}

void free_model(struct model* model) {
	for (struct mesh* vector_iter(model->meshes, mesh)) {
		if (mesh->use_diffuse_map)  { deinit_texture(&mesh->diffuse_map); }
		if (mesh->use_specular_map) { deinit_texture(&mesh->specular_map); }
		deinit_vb(&mesh->vb);
	}

	free_vector(model->meshes);

	free(model);
}

void draw_model(struct model* model, struct shader* shader) {
	for (u32 i = 0; i < vector_count(model->meshes); i++) {
		struct mesh* mesh = model->meshes + i;

		shader_set_b(shader, "use_diffuse_map", mesh->use_diffuse_map);
		if (mesh->use_diffuse_map) {
			bind_texture(&model->meshes[i].diffuse_map, 0);
			shader_set_i(shader, "diffuse_map", 0);
		}

		shader_set_b(shader, "use_specular_map", mesh->use_specular_map);
		if (mesh->use_specular_map) {
			bind_texture(&model->meshes[i].specular_map, 1);
			shader_set_i(shader, "specular_map", 1);
		}

		shader_set_v3f(shader, "material.ambient",   mesh->ambient);
		shader_set_v3f(shader, "material.diffuse",   mesh->diffuse);
		shader_set_v3f(shader, "material.specular",  mesh->specular);
		shader_set_f(  shader, "material.shininess", mesh->shininess);

		bind_vb_for_draw(&model->meshes[i].vb);
		draw_vb(&model->meshes[i].vb);
	}
}
