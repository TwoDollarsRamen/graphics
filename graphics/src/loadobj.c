#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loadobj.h"

#ifdef _WIN32
/* Because Windows is weird and bad.
 * strtok_r is standard everywhere else. */
#define strtok_r strtok_s
#endif

struct obj_index {
	u32 v, uv, n;
};

static bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

const char* parse_float(const char* start, f32* out) {
	const char* c = start;

	if (*c == '-') {
		c++;
	}

	while (*c && is_digit(*c)) {
		c++;
	}

	if (*c == '.') {
		c++;
	}

	while (*c && is_digit(*c)) {
		c++;
	}

	*out = (f32)strtod(start, null);

	return c + 1;
}

static v3f parse_v3(const char* start) {
	v3f r;

	start = parse_float(start, &r.x);
	start = parse_float(start, &r.y);
	start = parse_float(start, &r.z);

	return r;
}

static v2f parse_v2(const char* start) {
	v2f r;

	start = parse_float(start, &r.x);
	start = parse_float(start, &r.y);

	return r;
}

static struct obj_vertex parse_vertex(struct obj_mesh* mesh, const char* start) {
	char* mod = copy_string(start);

	struct obj_vertex r;

	char* save = mod;
	char* token;

	u32 i = 0;

	while ((token = strtok_r(save, "/", &save)) && i < 3) {
		u32* v = null;

		switch (i) {
			case 0:
				v = &r.position;
				break;
			case 1:
				v = &r.uv;
				break;
			case 2:
				v = &r.normal;
				break;
			default: break;
		}

		*v = strtol(token, null, 10);
		*v -= 1;

		i++;
	}

	free(mod);

	return r;
}

static void parse_face(struct obj_mesh* mesh, vector(struct obj_vertex)* verts, const char* start) {
	char* mod = copy_string(start);

	char* save = mod;
	char* token;

	u32 i = 0;

	while ((token = strtok_r(save, " ", &save))) {
		vector_push(*verts, parse_vertex(mesh, token));

		i++;

		if (i >= 4) {
			mesh->triangulated = false;
		}
	}

	free(mod);
}

static void parse_mtl(struct obj_model* model, const char* file_path, const char* mtl_path) {
	FILE* file = fopen(mtl_path, "r");
	if (!file) {
		fprintf(stderr, "Failed to fopen `%s'\n", mtl_path);
		return;
	}

	char* line = malloc(256);

	struct obj_material* cur = null;

#define read_path(p_, o_) \
	do { \
		(p_) = malloc(256); \
		strcpy((p_), file_path); \
		strcat((p_), line + (o_)); \
	} while (0)

	while (fgets(line, 256, file)) {
		u32 line_len = (u32)strlen(line);

		/* Strip the newline that fgets reads as well as the line. */
		if (line[line_len - 1] == '\n') {
			line[line_len - 1] = '\0';
		}

		if (memcmp(line, "newmtl", 6) == 0) {
			char* name = copy_string(line + 7);

			cur = table_set(model->materials, name, &(struct obj_material) { 0 });

			free(name);
		} else if (cur && memcmp(line, "Ns", 2) == 0) { /* Specular exponent. */
			parse_float(line + 3, &cur->specular_exponent);
		} else if (cur && memcmp(line, "Ka", 2) == 0) { /* Ambient colour. */
			cur->ambient = parse_v3(line + 3);
		} else if (cur && memcmp(line, "Kd", 2) == 0) { /* Diffuse colour. */
			cur->diffuse = parse_v3(line + 3);
		} else if (cur && memcmp(line, "Ks", 2) == 0) { /* Specular colour. */
			cur->specular = parse_v3(line + 3);
		} else if (cur && memcmp(line, "map_Ka", 6) == 0) { /* Ambient map. */
			read_path(cur->ambient_map_path, 7);
		} else if (cur && memcmp(line, "map_Kd", 6) == 0) { /* Diffuse map. */
			read_path(cur->diffuse_map_path, 7);
		} else if (cur && memcmp(line, "map_Ks", 6) == 0) { /* Specular map. */
			read_path(cur->specular_map_path, 7);
		} else if (cur && memcmp(line, "map_bump", 8) == 0) { /* Bump map. */
			read_path(cur->bump_map_path, 9);
		} else if (cur && memcmp(line, "bump", 4) == 0) { /* Bump map. */
			read_path(cur->bump_map_path, 5);
		} else if (cur && memcmp(line, "disp", 4) == 0) { /* Displacement map. */
			read_path(cur->displacement_map_path, 5);
		} else if (cur && memcmp(line, "decal", 5) == 0) { /* Stencil map. */
			read_path(cur->displacement_map_path, 6);
		}
	}

	free(line);

#undef read_path
}

bool load_obj(const char* filename, struct obj_model* model) {
	*model = (struct obj_model) { 0 };

	FILE* file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "Failed to fopen `%s'\n", filename);
		return false;
	}

	model->materials = new_table(sizeof(struct obj_material));

	char* file_path = get_file_path(filename);

	char* line = malloc(256);

	struct obj_mesh* current_mesh = &model->root_mesh;

	while (fgets(line, 256, file)) {
		u32 line_len = (u32)strlen(line);

		/* Strip the newline that fgets reads as well as the line. */
		if (line[line_len - 1] == '\n') {
			line[line_len - 1] = '\0';
		}

		if (memcmp(line, "usemtl", 6) == 0) {
			current_mesh->material_name = copy_string(line + 7);
		} else if (memcmp(line, "mtllib", 6) == 0) {
			char* mtl_name = copy_string(line + 7);
			char* mtl_path = malloc(256);

			strcpy(mtl_path, file_path);
			strcat(mtl_path, mtl_name);
			free(mtl_name);

			parse_mtl(model, file_path, mtl_path);

			free(mtl_path);
		} else {
			switch (line[0]) {
				case 'o':
					vector_push(model->meshes, (struct obj_mesh) { 0 });
					current_mesh = vector_end(model->meshes);
					current_mesh->triangulated = true;
					break;
				case 'v':
					switch (line[1]) {
						case 't':
							vector_push(model->uvs, parse_v2(line + 3));
							break;
						case 'n':
							vector_push(model->normals, parse_v3(line + 3));
							break;
						case ' ':
						case '\t':
							vector_push(model->positions, parse_v3(line + 2));
							break;
						default: break;
					}
					break;
				case 'f':
					parse_face(current_mesh, &current_mesh->vertices, line + 2);
					break;
				default: break;
			}
		}
	}

	model->has_root_mesh = vector_count(model->root_mesh.vertices) > 0;

	fclose(file);

	free(line);
	free(file_path);

	return true;
}

void deinit_obj(struct obj_model* model) {
	for (u32 i = 0; i < vector_count(model->meshes); i++) {
		struct obj_mesh* mesh = model->meshes + i;

		free_vector(mesh->vertices);

		if (mesh->material_name) {
			free(mesh->material_name);
		}
	}

	free_vector(model->positions);
	free_vector(model->uvs);
	free_vector(model->normals);

	free_vector(model->meshes);

	for (table_iter(model->materials, iter)) {
		struct obj_material* m = iter.value;

		if (m->ambient_map_path)      { free(m->ambient_map_path); }
		if (m->diffuse_map_path)      { free(m->diffuse_map_path); }
		if (m->specular_map_path)     { free(m->specular_map_path); }
		if (m->bump_map_path)         { free(m->bump_map_path); }
		if (m->displacement_map_path) { free(m->displacement_map_path); }
		if (m->stencil_map_path)      { free(m->stencil_map_path); }
	}

	free_table(model->materials);
}
