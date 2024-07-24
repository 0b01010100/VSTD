#pragma once
#pragma warning(push)
#pragma warning(disable: 4715)
// NOTE: BAD >:(
/////////////////////
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string.h>
/////////////////////
#define HT_TABLE_DEFAULT_SIZE 10000
#define HT_TABLE_DEFAULT_MAX_LOAD_FACTOR 0.75

#define __HASHFUNC_FNV1A(ret, key, keysize) \
  ret = 0x811c9dc5; \
  for (size_t ___hash_i = 0; ___hash_i < keysize; ___hash_i++) { \
    ret ^= ((uint8_t *)key)[___hash_i]; \
    ret *= 0x01000193; \
  } \

#define ___define_ht_entry(key_t, val_t) \
	typedef struct ___ht_entry_##key_t##_##val_t { \
		key_t key; \
		val_t val; \
		bool is_set; \
	} ___ht_entry_##key_t##_##val_t;

#define ___define_ht(key_t, val_t) \
	typedef struct __ht_##key_t##_##val_t { \
		___ht_entry_##key_t##_##val_t *entries; \
		size_t size; \
		size_t length; \
		double max_load_factor; \
	} key_t##_##val_t##_ht_t;

#define ___define_ht_create(key_t, val_t) \
	key_t##_##val_t##_ht_t key_t##_##val_t##_ht_create();
#define ___define_ht_create_impl(key_t, val_t) \
	key_t##_##val_t##_ht_t key_t##_##val_t##_ht_create() {	\
		return key_t##_##val_t##_ht_create_ex(HT_TABLE_DEFAULT_SIZE, HT_TABLE_DEFAULT_MAX_LOAD_FACTOR);	\
	}
#define ___define_ht_create_ex(key_t, val_t) \
	key_t##_##val_t##_ht_t key_t##_##val_t##_ht_create_ex(size_t ht_arr_capacity, double max_load_factor);
#define ___define_ht_create_ex_impl(key_t, val_t) \
	key_t##_##val_t##_ht_t key_t##_##val_t##_ht_create_ex(size_t ht_arr_capacity, double max_load_factor) {	\
		key_t##_##val_t##_ht_t ht = {0};	\
		ht.entries = (___ht_entry_##key_t##_##val_t*)malloc(sizeof(___ht_entry_##key_t##_##val_t) * ht_arr_capacity);	\
		memset(ht.entries, 0, sizeof(___ht_entry_##key_t##_##val_t) * ht_arr_capacity);	\
		ht.size = ht_arr_capacity;	\
		ht.max_load_factor = max_load_factor;	\
		return ht;	\
	}
#define ___define_ht_destroy(key_t, val_t) \
	void key_t##_##val_t##_ht_destroy(key_t##_##val_t##_ht_t *ht);
#define ___define_ht_destroy_impl(key_t, val_t) \
	void key_t##_##val_t##_ht_destroy(key_t##_##val_t##_ht_t *ht) {	\
		for (int i = 0; i < ht->size; i++) {	\
			___ht_entry_##key_t##_##val_t cur = ht->entries[i];	\
			if(!cur.is_set) { continue; }	\
			key_t##_ht_dtor(cur.key);	\
			val_t##_ht_dtor(cur.val);	\
			ht->entries[i] = (___ht_entry_##key_t##_##val_t){0};	\
		}	\
		free(ht->entries);\
		ht->entries = NULL;	\
		ht->size = 0;	\
		ht->length = 0;	\
		ht->max_load_factor = 0;	\
	}

#define ___define_ht_set(key_t, val_t) \
	void key_t##_##val_t##_ht_set(key_t##_##val_t##_ht_t *ht, key_t key, val_t val);
#define ___define_ht_set_impl(key_t, val_t) \
	void key_t##_##val_t##_ht_set(key_t##_##val_t##_ht_t *ht, key_t key, val_t val) {	\
		const uint64_t powers_of_2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296};	\
		size_t index = key_t##_ht_hash(key) % ht->size;	\
		___ht_entry_##key_t##_##val_t* current_entry = &ht->entries[index];	\
		uint32_t power_of_two = 0;	\
		while (current_entry->is_set) {	\
			bool equal = key_t##_ht_equal(current_entry->key, key);	\
			if (equal) {	\
				val_t##_ht_dtor(current_entry->val);	\
				current_entry->val = val;	\
				break;	\
			} else {	\
				if (power_of_two == 33) {	power_of_two = 0; } \
				index = (index + powers_of_2[power_of_two++]) % ht->size; \
				current_entry = &ht->entries[index]; \
			} \
		} \
		if (!current_entry->is_set) {	\
			ht->length++;	\
			current_entry->is_set = true;	\
			current_entry->key = key_t##_copy(key);	\
			current_entry->val = val;	\
		} \
		if ((double)ht->length / ht->size > ht->max_load_factor) { key_t##_##val_t##_ht_resize(ht); }	\
	}

#define ___define_ht_get(key_t, val_t) \
	val_t key_t##_##val_t##_ht_get(key_t##_##val_t##_ht_t *ht, key_t key);

#define ___define_ht_get_impl(key_t, val_t) \
	val_t key_t##_##val_t##_ht_get(key_t##_##val_t##_ht_t *ht, key_t key) {	\
		const uint64_t powers_of_2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296};	\
		size_t index = key_t##_ht_hash(key) % ht->size;	\
		___ht_entry_##key_t##_##val_t* current_entry = &ht->entries[index];	\
		uint32_t power_of_two = 0;	\
		while (current_entry->is_set) {	\
			bool equal = key_t##_ht_equal(current_entry->key, key);	\
			if (equal) {	\
				return val_t##_copy(current_entry->val);	\
			} else {	\
				if (power_of_two == 33) {	power_of_two = 0; } \
				index = (index + powers_of_2[power_of_two++]) % ht->size; \
				current_entry = &ht->entries[index]; \
			} \
		} \
		assert(false && "key not found");	\
	}

		
#define ___define_ht_remove(key_t, val_t) \
	void key_t##_##val_t##_ht_remove(key_t##_##val_t##_ht_t *ht, key_t key);
#define ___define_ht_remove_impl(key_t, val_t) \
	void key_t##_##val_t##_ht_remove(key_t##_##val_t##_ht_t *ht, key_t key) {	\
		const uint64_t powers_of_2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296};	\
		size_t index = key_t##_ht_hash(key) % ht->size;	\
		___ht_entry_##key_t##_##val_t* current_entry = &ht->entries[index];	\
		uint32_t power_of_two = 0;	\
		while (current_entry->is_set) {	\
			bool equal = key_t##_ht_equal(current_entry->key, key);	\
			if (equal) {	\
				val_t##_ht_dtor(current_entry->val); \
				current_entry->is_set = false;	\
				ht->length--;	\
				break;	\
			} else {	\
				if (power_of_two == 33) {	power_of_two = 0; } \
				index = (index + powers_of_2[power_of_two++]) % ht->size; \
				current_entry = &ht->entries[index]; \
			} \
		} \
		if ((double)ht->length / ht->size > ht->max_load_factor) { key_t##_##val_t##_ht_resize(ht); }	\
	}

#define ___define_ht_exists(key_t, val_t) \
	bool key_t##_##val_t##_ht_exists(key_t##_##val_t##_ht_t *ht, key_t key);
#define ___define_ht_exists_impl(key_t, val_t) \
	bool key_t##_##val_t##_ht_exists(key_t##_##val_t##_ht_t *ht, key_t key) {	\
		const uint64_t powers_of_2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296};	\
		size_t index = key_t##_ht_hash(key) % ht->size;	\
		___ht_entry_##key_t##_##val_t* current_entry = &ht->entries[index];	\
		uint32_t power_of_two = 0;	\
		while (current_entry->is_set) {	\
			bool equal = key_t##_ht_equal(current_entry->key, key);	\
			if (equal) {	\
				return true;	\
			} else {	\
				if (power_of_two == 33) {	power_of_two = 0; } \
				index = (index + powers_of_2[power_of_two++]) % ht->size; \
				current_entry = &ht->entries[index]; \
			} \
		} \
		return false;	\
	}

#define ___define_ht_resize(key_t, val_t) \
	void key_t##_##val_t##_ht_resize(key_t##_##val_t##_ht_t *ht);
#define ___define_ht_resize_impl(key_t, val_t) \
	void key_t##_##val_t##_ht_resize(key_t##_##val_t##_ht_t *ht) {	\
		const uint64_t powers_of_2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296};	\
		uint32_t power_of_two = 0;	\
		key_t##_##val_t##_ht_t new_ht = key_t##_##val_t##_ht_create_ex(ht->size * 2, ht->max_load_factor);	\
		for (size_t i = 0; i < ht->size; i++) {	\
			if(!ht->entries[i].is_set) { continue; }	\
			___ht_entry_##key_t##_##val_t cur = ht->entries[i];	\
			size_t index = key_t##_ht_hash(cur.key) % new_ht.size;	\
			___ht_entry_##key_t##_##val_t* current_entry = &new_ht.entries[index];	\
			uint32_t power_of_two = 0;	\
			while (current_entry->is_set) {	\
				if (power_of_two == 33) {	power_of_two = 0; } \
				index = (index + powers_of_2[power_of_two++]) % new_ht.size; \
				current_entry = &new_ht.entries[index]; \
			} \
			current_entry->is_set = true;	\
			current_entry->key = cur.key;	\
			current_entry->val = cur.val;	\
			new_ht.length++;	\
		}	\
		free(ht->entries);	\
		*ht = new_ht;	\
	}

#define ___define_ht_clear(key_t, val_t) \
	void key_t##_##val_t##_ht_clear(key_t##_##val_t##_ht_t *ht);
#define ___define_ht_clear_impl(key_t, val_t) \
	void key_t##_##val_t##_ht_clear(key_t##_##val_t##_ht_t *ht) {	\
		for (int i = 0; i < ht->size; i++) {	\
			___ht_entry_##key_t##_##val_t cur = ht->entries[i];	\
			if(!cur.is_set) { continue; }	\
			key_t##_ht_dtor(cur.key);	\
			val_t##_ht_dtor(cur.val);	\
			ht->entries[i] = (___ht_entry_##key_t##_##val_t){0};	\
		}	\
		ht->length = 0;	\
	}

#define ___define_ht_merge(key_t, val_t) \
	key_t##_##val_t##_ht_t key_t##_##val_t##_ht_merge(key_t##_##val_t##_ht_t *ht1, key_t##_##val_t##_ht_t *ht2);
#define ___define_ht_merge_impl(key_t, val_t) \
	key_t##_##val_t##_ht_t key_t##_##val_t##_ht_merge(key_t##_##val_t##_ht_t *ht1, key_t##_##val_t##_ht_t *ht2) {	\
		const uint64_t powers_of_2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296};	\
		key_t##_##val_t##_ht_t new_ht = {0};	\
		const key_t##_##val_t##_ht_t biggest_ht = ht1->size > ht2->size ? *ht1 : *ht2;	\
		const bool length_not_fits_load = (ht1->length + ht2->length) / biggest_ht.size > biggest_ht.max_load_factor;	\
		if(length_not_fits_load) {	\
			new_ht = key_t##_##val_t##_ht_create_ex(biggest_ht.size, biggest_ht.max_load_factor);	\
			for (size_t i = 0; i < biggest_ht.size; i++) {	\
				___ht_entry_##key_t##_##val_t cur = biggest_ht.entries[i];	\
				if(!cur.is_set) { continue; }	\
				size_t index = key_t##_ht_hash(cur.key) % new_ht.size;	\
				___ht_entry_##key_t##_##val_t* current_entry = &new_ht.entries[index];	\
				uint32_t power_of_two = 0;	\
				while (current_entry->is_set) {	\
					if (power_of_two == 33) {	power_of_two = 0; } \
					index = (index + powers_of_2[power_of_two++]) % new_ht.size; \
					current_entry = &new_ht.entries[index]; \
				} \
				current_entry->is_set = true;	\
				current_entry->key = cur.key;	\
				current_entry->val = cur.val;	\
				new_ht.length++;	\
			}	\
		}	else {	\
			new_ht = ht1->size > ht2->size ? *ht1 : *ht2;	\
		}	\
		const key_t##_##val_t##_ht_t* smallest_ht = ht1->size > ht2->size ? ht2 : ht1;	\
		for (size_t i = 0; i < smallest_ht->size; i++) {	\
			___ht_entry_##key_t##_##val_t cur = smallest_ht->entries[i];	\
			if(!cur.is_set) { continue; }	\
			size_t index = key_t##_ht_hash(cur.key) % new_ht.size;	\
			___ht_entry_##key_t##_##val_t* current_entry = &new_ht.entries[index];	\
			uint32_t power_of_two = 0;	\
			while (current_entry->is_set) {	\
				bool equal = key_t##_ht_equal(cur.key, current_entry->key);	\
				if (equal) {	\
					key_t##_ht_dtor(current_entry->key);	\
					val_t##_ht_dtor(current_entry->val);	\
					current_entry->key = cur.key;	\
					current_entry->val = cur.val;	\
					break;	\
				}	\
				if (power_of_two == 33) {	power_of_two = 0; } \
				index = (index + powers_of_2[power_of_two++]) % new_ht.size; \
				current_entry = &new_ht.entries[index]; \
			} \
			if (!current_entry->is_set) {	\
				current_entry->is_set = true;	\
				current_entry->key = cur.key;	\
				current_entry->val = cur.val;	\
				new_ht.length++;	\
			}	\
		}	\
		if(length_not_fits_load) {	\
			free(ht1->entries);	\
			free(ht2->entries);	\
		} else {	\
			free(smallest_ht->entries);	\
		}	\
		return new_ht;	\
	}

#define define_ht_all(key_t, val_t) \
	___define_ht_entry(key_t, val_t) \
	___define_ht(key_t, val_t) \
	___define_ht_create(key_t, val_t) \
	___define_ht_create_ex(key_t, val_t) \
	___define_ht_destroy(key_t, val_t) \
	___define_ht_set(key_t, val_t) \
	___define_ht_get(key_t, val_t) \
	___define_ht_exists(key_t, val_t) \
	___define_ht_remove(key_t, val_t)	\
	___define_ht_resize(key_t, val_t)	\
	___define_ht_clear(key_t, val_t)	\
	___define_ht_merge(key_t, val_t)

#define define_ht_all_impl(key_t, val_t)	\
	___define_ht_create_impl(key_t, val_t)	\
	___define_ht_create_ex_impl(key_t, val_t)	\
	___define_ht_destroy_impl(key_t, val_t)	\
	___define_ht_set_impl(key_t, val_t) \
	___define_ht_get_impl(key_t, val_t)	\
	___define_ht_exists_impl(key_t, val_t)	\
	___define_ht_remove_impl(key_t, val_t)	\
	___define_ht_resize_impl(key_t, val_t)	\
	___define_ht_clear_impl(key_t, val_t)	\
	___define_ht_merge_impl(key_t, val_t)	

#define ht_foreach(ht, iter_name, index_name, key_t, val_t) for(size_t index_name = 0; index_name < ht.size; index_name++) { ___ht_entry_##key_t##_##val_t* iter_name = &ht.entries[index_name]; if(!iter_name->is_set) {continue;} else {

#define ht_foreach_end } }

#pragma warning(pop)