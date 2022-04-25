#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "mesh.h"

#pragma pack(push, 1)
struct vertex {
	v3f position, normal, tangent, bitangent;
	v2f uv;
};
#pragma pack(pop)

static bool vertex_equal(v3f pos, v3f norm, v2f uv, struct vertex vert) {
	return (
		pos.x  == vert.position.x &&
		pos.y  == vert.position.y &&
		pos.z  == vert.position.z &&
		uv.x   == vert.uv.x &&
		uv.y   == vert.uv.y &&
		norm.x == vert.normal.x &&
		norm.y == vert.normal.y &&
		norm.z == vert.normal.z);
}

static void process_mesh(struct model* model, struct mesh* mesh, struct obj_model* omodel, struct obj_mesh* omesh) {
	mesh->ambient = mesh->diffuse = mesh->specular = make_v3f(1.0f, 1.0f, 1.0f);
	mesh->shininess = 32.0f;

	struct vertex* verts = calloc(vector_count(omesh->vertices), sizeof(struct vertex));
	u32* indices = malloc(vector_count(omesh->vertices) * sizeof(u32));

	u32 vertex_count = 0;
	u32 index_count = 0;

	/* When parsed from the Wavefront file, the mesh is indexed with separate
	 * indices for the position, normal and UV. We can't render like this, so
	 * it must be processed into a single buffer of vertices with indices to
	 * go along with it. Here, the model's AABB is also updated if necessary. */
	for (u32 i = 0; i < vector_count(omesh->vertices); i++) {
		bool new = true;

		v3f pos = omodel->positions[omesh->vertices[i].position];
		v3f norm = omodel->normals[omesh->vertices[i].normal];
		v2f uv = omodel->uvs[omesh->vertices[i].uv];

		if (pos.x < model->aabb.min.x) { model->aabb.min.x = pos.x; }
		if (pos.y < model->aabb.min.y) { model->aabb.min.y = pos.y; }
		if (pos.z < model->aabb.min.z) { model->aabb.min.z = pos.z; }
		if (pos.x > model->aabb.max.x) { model->aabb.max.x = pos.x; }
		if (pos.y > model->aabb.max.y) { model->aabb.max.y = pos.y; }
		if (pos.z > model->aabb.max.z) { model->aabb.max.z = pos.z; }

		for (u32 ii = 0; ii < vertex_count; ii++) {
			if (vertex_equal(pos, norm, uv, verts[ii])) {
				indices[index_count++] = ii;
				new = false;
				break;
			}
		}

		if (new) {
			verts[vertex_count] = (struct vertex){
				.position = pos,
				.normal = norm,
				.uv = uv,
			};

			indices[index_count++] = vertex_count;
			vertex_count++;
		}
	}

	/* Calculate tangents and bitangents */
	for (u32 i = 0; i < index_count; i += 3) {
		v3f pos1 = verts[indices[i + 0]].position;
		v3f pos2 = verts[indices[i + 1]].position;
		v3f pos3 = verts[indices[i + 2]].position;

		v2f uv1 = verts[indices[i + 0]].uv;
		v2f uv2 = verts[indices[i + 1]].uv;
		v2f uv3 = verts[indices[i + 2]].uv;

		v2f delta_uv_1 = v2f_sub(uv2, uv1);
		v2f delta_uv_2 = v2f_sub(uv3, uv1);

		v3f edge_1 = v3f_sub(pos2, pos1);
		v3f edge_2 = v3f_sub(pos3, pos1);

		f32 f = 1.0f / (delta_uv_1.x * delta_uv_2.y - delta_uv_2.x * delta_uv_1.y);

		v3f tangent = {
			.x = f * (delta_uv_2.y * edge_1.x - delta_uv_1.y * edge_2.x),
			.y = f * (delta_uv_2.y * edge_1.y - delta_uv_1.y * edge_2.y),
			.z = f * (delta_uv_2.y * edge_1.z - delta_uv_1.y * edge_2.z)
		};

		v3f bitangent = {
			.x = f * (-delta_uv_2.x * edge_1.x + delta_uv_1.x * edge_2.x),
			.y = f * (-delta_uv_2.x * edge_1.y + delta_uv_1.x * edge_2.y),
			.z = f * (-delta_uv_2.x * edge_1.z + delta_uv_1.x * edge_2.z)
		};

		verts[indices[i + 0]].tangent = tangent;
		verts[indices[i + 1]].tangent = tangent;
		verts[indices[i + 2]].tangent = tangent;

		verts[indices[i + 0]].bitangent = bitangent;
		verts[indices[i + 1]].bitangent = bitangent;
		verts[indices[i + 2]].bitangent = bitangent;
	}

	/* Create the vertex buffer and send the vertices to the GPU. */
	init_vb(&mesh->vb, vb_static | vb_tris);
	bind_vb_for_edit(&mesh->vb);
	push_vertices(&mesh->vb, verts, vertex_count * sizeof(struct vertex));
	push_indices(&mesh->vb, indices, index_count);
	configure_vb(&mesh->vb, 0, 3, sizeof(struct vertex), offsetof(struct vertex, position)); /* position (vec3) */
	configure_vb(&mesh->vb, 1, 2, sizeof(struct vertex), offsetof(struct vertex, uv)); /* uv (vec2) */
	configure_vb(&mesh->vb, 2, 3, sizeof(struct vertex), offsetof(struct vertex, normal)); /* normal (vec3) */
	configure_vb(&mesh->vb, 3, 3, sizeof(struct vertex), offsetof(struct vertex, tangent)); /* tangent (vec3) */
	configure_vb(&mesh->vb, 4, 3, sizeof(struct vertex), offsetof(struct vertex, bitangent)); /* bitangent (vec3) */
	bind_vb_for_edit(null);

	/* No longer needed. */
	free(verts);
	free(indices);

	/* Material */
	if (omesh->material_id) {
		struct obj_material* material = table_get(omodel->materials, omesh->material_id);

		if (material) {
			mesh->ambient = material->ambient;
			mesh->diffuse = material->diffuse;
			mesh->specular = material->specular;
			mesh->emissive = material->emissive;
			mesh->alpha = material->alpha;
			mesh->shininess = material->specular_exponent;

			if (material->diffuse_map_path) {
				mesh->use_diffuse_map = true;

				init_texture(&mesh->diffuse_map, material->diffuse_map_path);
			}

			if (material->specular_map_path) {
				mesh->use_specular_map = true;

				init_texture(&mesh->specular_map, material->specular_map_path);
			}

			if (material->bump_map_path) {
				mesh->use_normal_map = true;

				init_texture(&mesh->normal_map, material->bump_map_path);
			}
		}
	}
}

struct model* new_model_from_obj(struct obj_model* omodel) {
	struct model* model = calloc(1, sizeof(struct model));

	model->aabb = (struct aabb) {
		.min = { INFINITY, INFINITY, INFINITY },
		.max = { -INFINITY, -INFINITY, -INFINITY }
	};

	/* Initiate preprocessing. */
	if (omodel->has_root_mesh) {	
		struct mesh mesh = { 0 };
		vector_push(model->meshes, mesh);

		process_mesh(model, vector_end(model->meshes), omodel, &omodel->root_mesh);
	}

	for (u32 i = 0; i < vector_count(omodel->meshes); i++) {
		struct mesh mesh = { 0 };
		vector_push(model->meshes, mesh);

		process_mesh(model, vector_end(model->meshes), omodel, omodel->meshes + i);
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
	/* Apply material to the shader and draw the vertex buffer in
	 * each mesh inside the model. */

	for (u32 i = 0; i < vector_count(model->meshes); i++) {
		struct mesh* mesh = model->meshes + i;

		shader_set_b(shader, "use_diffuse_map", mesh->use_diffuse_map);
		if (mesh->use_diffuse_map) {
			bind_texture(&model->meshes[i].diffuse_map, 3);
			shader_set_i(shader, "diffuse_map", 3);
		}

		shader_set_b(shader, "use_specular_map", mesh->use_specular_map);
		if (mesh->use_specular_map) {
			bind_texture(&model->meshes[i].specular_map, 4);
			shader_set_i(shader, "specular_map", 4);
		}

		shader_set_b(shader, "use_normal_map", mesh->use_normal_map);
		if (mesh->use_normal_map) {
			bind_texture(&model->meshes[i].normal_map, 5);
			shader_set_i(shader, "normal_map", 5);
		}

		shader_set_v3f(shader, "material.ambient",   mesh->ambient);
		shader_set_v3f(shader, "material.diffuse",   mesh->diffuse);
		shader_set_v3f(shader, "material.specular",  mesh->specular);
		shader_set_v3f(shader, "material.emissive",  mesh->emissive);
		shader_set_f(  shader, "material.shininess", mesh->shininess);
		shader_set_f(  shader, "material.alpha",     mesh->alpha);

		bind_vb_for_draw(&model->meshes[i].vb);
		draw_vb(&model->meshes[i].vb);
	}
}
