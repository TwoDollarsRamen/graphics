#include "table.h"

u64 elf_hash(const void* ptr, usize size) {
	const u8* data = ptr;

	usize hash = 0, x = 0;

	for (usize i = 0; i < size; i++) {
		hash = (hash << 4) + data[i];
		if ((x = hash & 0xF000000000LL) != 0) {
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}

	return (hash & 0x7FFFFFFFFF);
}

u64 hash_string(const char* str) {
	return elf_hash(str, strlen(str));
}

void* _find_table_el(void* els_v, usize el_size, usize capacity, usize key_size, void* key_ptr,
	usize key_off, usize val_off, usize state_off, usize* ind) {
	u64 nk = table_null_key;

	usize idx = elf_hash(key_ptr, key_size) % capacity;

	u8* tombstone = NULL;

	u8* els = els_v;

	for (;;) {
		u8* el = els + idx * el_size;
		if (memcmp(el + key_off, &nk, key_size) == 0) {
			if (*(el + state_off) != table_el_state_inactive) {
				if (ind) { *ind = idx; }
				return tombstone != NULL ? tombstone : el;
			} else if (tombstone == NULL) {
				tombstone = el;
			}
		} else if (memcmp(el + key_off, key_ptr, key_size) == 0) {
			if (ind) { *ind = idx; }
			return el;
		}

		idx = (idx + 1) % capacity;
	}
}

void* _table_get(void* els, usize el_size, usize capacity, usize count, usize key_size, void* key,
	usize key_off, usize val_off, usize state_off) {
	if (count == 0) { return NULL; }
	
	u64 nk = table_null_key;

	u8* el = _find_table_el(els, el_size, capacity, key_size, key, key_off, val_off, state_off, NULL);
	if (memcmp(el + key_off, &nk, key_size) == 0) {
		return NULL;
	}

	return el + val_off;
}

void* _table_first_key(void* els, usize el_size, usize capacity, usize count, usize key_size,
	usize key_off, usize val_off, usize state_off) {
	if (count == 0) { return NULL; }

	u64 nk = table_null_key;

	for (usize i = 0; i < capacity; i++) {
		u8* el = (u8*)els + i * el_size;
		if (memcmp(el + key_off, &nk, key_size) != 0) {
			return el + key_off;
		}
	}

	return NULL;
}

void* _table_next_key(void* els, usize el_size, usize capacity, usize count, usize key_size, void* key,
	usize key_off, usize val_off, usize state_off) {
	if (count == 0) { return NULL; }

	u64 nk = table_null_key;

	usize idx = 0;

	u8* el = _find_table_el(els, el_size, capacity, key_size, key, key_off, val_off, state_off, &idx);
	if (memcmp(el + key_off, &nk, key_size) == 0) {
		return NULL;
	}

	if (idx >= capacity) {
		return NULL;
	}

	for (usize i = idx + 1; i < capacity; i++) {
		el = (u8*)els + i * el_size;
		if (memcmp(el + key_off, &nk, key_size) != 0) {
			return el + key_off;
		}
	}

	return NULL;
}
