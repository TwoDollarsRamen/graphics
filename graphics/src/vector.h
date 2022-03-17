#pragma once

#include <stdlib.h>
#include <string.h>

#include "common.h"

#define vector_default_capacity 8

struct vector_header {
	u32 count;
	u32 capacity;
	u64 element_size;
};

#define vector(t_) t_*

#define vector_push(v_, e_) \
	do { \
		if (!(v_)) { \
			struct vector_header h_ = { \
				.count = 1, \
				.capacity = 8, \
				.element_size = sizeof(*(v_)) \
			}; \
			\
			(v_) = malloc(sizeof(struct vector_header) + h_.element_size * h_.capacity); \
			\
			memcpy((v_), &h_, sizeof(struct vector_header)); \
			\
			(v_) = (void*)((struct vector_header*)(v_) + 1); \
			(v_)[0] = (e_); \
		} else { \
			struct vector_header* h_ = ((struct vector_header*)(v_)) - 1; \
			\
			if (h_->count >= h_->capacity) { \
				h_->capacity = h_->capacity * 2; \
				(v_) = realloc(h_, sizeof(struct vector_header) + h_->element_size * h_->capacity); \
				h_ = (struct vector_header*)v_; \
				(v_) = (void*)(h_ + 1); \
			} \
			\
			(v_)[h_->count++] = (e_); \
		} \
	} while (0)

#define vector_count(v_) ((v_) != null ? (((struct vector_header*)(v_)) - 1)->count : 0)

#define free_vector(v_) if ((v_)) { free(((struct vector_header*)(v_)) - 1); }
