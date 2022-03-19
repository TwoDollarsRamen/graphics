#pragma once

#include "common.h"

/* This is a basic hash table implementation that uses
 * a generic value and a string key.
 *
 * It uses open addressing with linear probing. */

struct table;
struct table_iter;

struct table* new_table(u32 element_size);
void free_table(struct table* table);

void* table_get(struct table* table, const char* key);
void* table_set(struct table* table, const char* key, const void* val);
void table_delete(struct table* table, const char* key);

u32 get_table_count(struct table* table);

const char* table_get_key(struct table* table, const char* key);

struct table_iter {
	struct table* table;
	u32 i;
	const char* key;
	void* value;
};

struct table_iter new_table_iter(struct table* table);
bool table_iter_next(struct table_iter* iter);

#define table_iter(t_, n_) \
	struct table_iter n_ = new_table_iter((t_));\
	table_iter_next(&(n_));

