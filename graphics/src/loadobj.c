#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loadobj.h"

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

static struct obj_vertex parse_vertex(struct obj_model* model, const char* start) {
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

static void parse_face(struct obj_model* model, vector(struct obj_vertex)* verts, const char* start) {
	char* mod = copy_string(start);

	char* save = mod;
	char* token;

	u32 i = 0;

	while ((token = strtok_r(save, " ", &save))) {
		vector_push(*verts, parse_vertex(model, token));

		i++;

		if (i >= 4) {
			model->triangulated = false;
		}
	}

	free(mod);
}

bool load_obj(const char* filename, struct obj_model* model) {
	*model = (struct obj_model) { 0 };

	model->triangulated = true;

	FILE* file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "Failed to fopen `%s'\n", filename);
		return false;
	}

	char* line = malloc(4096);

	while (fgets(line, 4096, file)) {
		u32 line_len = (u32)strlen(line);

		/* Strip the newline that fgets reads as well as the line. */
		if (line[line_len - 1] == '\n') {
			line[line_len - 1] = '\0';
		}

		switch (line[0]) {
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
				parse_face(model, &model->vertices, line + 2);
				break;
			default: break;
		}
	}

	fclose(file);

	free(line);

	return true;
}

void deinit_obj(struct obj_model* model) {
	free_vector(model->positions);
	free_vector(model->uvs);
	free_vector(model->normals);
	free_vector(model->vertices);
}
