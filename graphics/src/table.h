#pragma once

#include <string.h>

#include "common.h"

/* Generic hash table.
 *
 * Keys should be integers and should be no larger than 64 bits.
 * Values can be any type. Tables should be initialised to zero
 * before use.
 *
 * Usage example:
 *
 * table(u32, i32) test_table = { 0 };
 *
 * table_set(test_table, 10, 33);
 * table_set(test_table, 45, 3);
 * table_set(test_table, 16, 5);
 * table_set(test_table, 1, 9);
 *
 * table_delete(test_table, 1);
 *
 * table_set(test_table, 1, 10);
 *
 * for (u32* i = table_first(test_table); i; i = table_next(test_table, *i)) {
 *     printf("%u: %d\n", *i, *(i32*)table_get(test_table, *i));
 * }
 *
 * free_table(test_table;
 */

#define table_null_key UINT64_MAX
#define table_load_factor 0.75

#define voffsetof(v_, m_) \
	(usize)(((u8*)(&((v_).m_))) - ((u8*)(&v_)))

u64 elf_hash(const void* ptr, usize size);
u64 hash_string(const char* str);

enum {
	table_el_state_active = 0,
	table_el_state_inactive = 1
};

#define _table_el(kt_, vt_) \
	struct { \
		kt_ key; \
		vt_ value; \
		u8 state; \
	}

void* _find_table_el(void* els, usize el_size, usize capacity, usize key_size, void* key,
	usize key_off, usize val_off, usize state_off, usize* ind);
void* _table_get(void* els, usize el_size, usize capacity, usize count, usize key_size, void* key,
	usize key_off, usize val_off, usize state_off);
void* _table_first_key(void* els, usize el_size, usize capacity, usize count, usize key_size,
	usize key_off, usize val_off, usize state_off);
void* _table_next_key(void* els, usize el_size, usize capacity, usize count, usize key_size, void* key,
	usize key_off, usize val_off, usize state_off);

#define _table_resize(t_, cap_) \
	do { \
		u64 nk_ = table_null_key; \
		u8* els_ = malloc((cap_) * sizeof (t_).e); \
		for (usize i = 0; i < cap_; i++) { \
			memcpy(&els_[i * sizeof (t_).e + voffsetof((t_).e, key)], &nk_, sizeof (t_).k); \
			els_[i * sizeof (t_).e + voffsetof((t_).e, state)] = table_el_state_active; \
		} \
		\
		for (usize i_ = 0; i_ < (t_).capacity; i_++) { \
			u8* el_ = (((u8*)(t_).entries) + i_ * sizeof (t_).e); \
			if (memcmp(el_, &nk_, sizeof (t_).k) == 0) { continue; } \
			u8* dst_ = _find_table_el(els_, sizeof *(t_).entries, (t_).capacity, sizeof (t_).k, \
				el_ + voffsetof(*(t_).entries, key),\
				voffsetof(*(t_).entries, key), voffsetof(*(t_).entries, value), voffsetof((t_).e, state), \
				NULL); \
			memcpy(dst_ + voffsetof((t_).e, key),   el_ + voffsetof((t_).e, key),   sizeof (t_).k); \
			memcpy(dst_ + voffsetof((t_).e, value), el_ + voffsetof((t_).e, value), sizeof (t_).v); \
		} \
		if ((t_).entries) { free((t_).entries); } \
		(t_).entries = (void*)els_; \
		(t_).capacity = (cap_); \
	} while (0)

#define table(kt_, vt_) \
	struct { \
		_table_el(kt_, vt_)* entries; \
		usize count, capacity; \
		_table_el(kt_, vt_) e; \
		kt_ k; \
		vt_ v; \
	}

#define free_table(t_) \
	do { \
		if ((t_).entries) { \
			free((t_).entries); \
		} \
	} while (0)

#define table_set(t_, k_, v_) \
	do { \
		if ((t_).count >= (t_).capacity * table_load_factor) { \
			usize new_cap_ = (t_).capacity < 8 ? 8 : (t_).capacity * 2; \
			_table_resize((t_), new_cap_); \
		} \
		(t_).k = (k_); \
		u64 nk_ = table_null_key; \
		u8* el_ = _find_table_el((t_).entries, sizeof *(t_).entries, (t_).capacity, sizeof (t_).k, \
			&(t_).k,\
			voffsetof((t_).e, key), voffsetof((t_).e, value), voffsetof((t_).e, state), NULL); \
		if (memcmp(el_ + voffsetof((t_).e, key), &nk_, sizeof (t_).k) == 0) { \
			(t_).count++; \
		} \
		(t_).v = (v_); \
		memcpy(el_ + voffsetof((t_).e, key),   &(t_).k, sizeof (t_).k); \
		memcpy(el_ + voffsetof((t_).e, value), &(t_).v, sizeof (t_).v); \
	} while (0)

#define table_get(t_, k_) \
	((t_).k = (k_), \
		_table_get((t_).entries, sizeof *(t_).entries, (t_).capacity, (t_).count, sizeof (t_).k, \
			&(t_).k,\
			voffsetof((t_).e, key), voffsetof((t_).e, value), voffsetof((t_).e, state)))

#define table_delete(t_, k_) \
	do { \
		if ((t_).count > 0) { \
			(t_).k = (k_); \
			u8* el_ = _find_table_el((t_).entries, sizeof *(t_).entries, (t_).capacity, sizeof (t_).k, \
				&(t_).k,\
				voffsetof((t_).e, key), voffsetof((t_).e, value), voffsetof((t_).e, state), NULL); \
			u64 nk_ = table_null_key; \
			memcpy(el_ + voffsetof((t_).e, key), &nk_, sizeof (t_).k); \
			*(el_ + voffsetof((t_).e, state)) = table_el_state_inactive; \
			(t_).count--; \
		} \
	} while (0)

#define table_first(t_) \
	_table_first_key((t_).entries, sizeof *(t_).entries, (t_).capacity, (t_).count, sizeof (t_).k, \
			voffsetof((t_).e, key), voffsetof((t_).e, value), voffsetof((t_).e, state))

#define table_next(t_, k_) \
	((t_).k = (k_), \
		_table_next_key((t_).entries, sizeof *(t_).entries, (t_).capacity, (t_).count, sizeof (t_).k, \
			&(t_).k, \
			voffsetof((t_).e, key), voffsetof((t_).e, value), voffsetof((t_).e, state)))
