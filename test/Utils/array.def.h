#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
/*

	

*/


#define def_array_index(val_t) \
    typedef val_t val_t##_array_index_t;

#define def_array(val_t, len_t) \
    typedef struct val_t##len_t##_array_t { \
        val_t##_array_index_t array[len_t]; \
		size_t size;	\
    } val_t##len_t##_array_t;

#define def_array_create(val_t, len_t) \
    val_t##len_t##_array_t val_t##len_t##_array_t_create(){ \
        val_t##len_t##_array_t array = {{0}}; \
        return array; \
    }

#define def_array_set(val_t) \
    void val_t##len_t##_array_t_set(val_t##len_t##_array_t *array, size_t index, val_t##_array_index_t value) { \
        if (index < sizeof(array->array) / sizeof(val_t)) { \
            array->array[index] = value; \
        } else { \
            fprintf(stderr, "Index out of bounds: %zu\n", index); \
            assert(false); \
        } \
    }

#define def_array_get(val_t) \
    val_t##_array_index_t val_t##len_t##_array_t_get(val_t##len_t##_array_t *array, size_t index) { \
        if (index < sizeof(array->array) / sizeof(val_t)) { \
            return array->array[index]; \
        } else { \
            fprintf(stderr, "Index out of bounds: %zu\n", index); \
            assert(false); \
            return 0; /* Return a default value */ \
        } \
    }

#define def_all(val_t, len_t) \
    def_array_index(val_t) \
    def_array(val_t, len_t) \
    def_array_create(val_t, len_t) \
    def_array_set(val_t, len_t) \
    def_array_get(val_t, len_t)
