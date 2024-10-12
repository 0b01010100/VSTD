#ifndef __varr__
#define __varr__
/**
 * @file varr.h
 * @brief A set of macros for managing and manipulating fixed-size arrays in C and C++.
 *
 * This header file provides a variety of macros to simplify working with arrays,
 * including creating arrays with fixed maximum sizes, accessing elements safely,
 * filling arrays, finding elements, reversing the order of elements, and sorting arrays.
 *
 * It is designed to be compatible with both C and C++ environments, providing
 * C++-specific error handling and standard library functions when compiled as C++.
 *
 * Key macros:
 * - `varr_create`: Creates a fixed-size array.
 * - `varr_size`: Returns the size of an array.
 * - `varr_get`: Accesses an element with bounds checking.
 * - `varr_set`: Sets an element with bounds checking.
 * - `varr_fill`: Fills the array with a specific value.
 * - `varr_find`: Finds an element in the array.
 * - `varr_reverse`: Reverses the order of elements in the array.
 * - `varr_sort`: Sorts the array in any order you want
 *
 * The macros handle errors by printing messages to stderr and exiting the program
 * if an out-of-bounds access occurs.
 *
 * This file is intended to be used as a header and should be included with `#include "varr.h"`.
 *
 * Usage example:
 * @code
 * varr_create(int, my_array, 10, 1, 2, 3, 4, 5);
 * //____________________va args^
 * varr_fill(my_array, 0);
 * varr_set(my_array, 1, 42);
 * int val = varr_get(my_array, 1);
 * @endcode
 */

#include <assert.h>

// Create an array with a fixed maximum size
#define varr_create(T, name, size_max, ...) \
    T name[size_max] = {__VA_ARGS__}

// Returns the size of the array
#define varr_size(name) \
    (sizeof(name)/sizeof((name)[0]))

// Access an element in the array
#define varr_get(name, index) \
    ((index) >= varr_size(name)) ? (assert(0 && "index out-of range\n"), -1) : ((name)[index])

// Set an element in the array
#define varr_set(name, index, value) \
    ((index) >= sizeof(name)/sizeof((name)[0])) ? (assert(0 && "index out-of range\n")) : ((name)[index] = (value))

// Fill the array with a specific value
#define varr_fill(name, value) \
    do { \
        for (unsigned long long arri = 0; arri < sizeof(name)/sizeof((name)[0]); arri++) { \
            (name)[arri] = (value); \
        } \
    } while(0)

// Find an element in the array
#define varr_find(name, value, out_index) \
    do { \
        *out_index = -1; /* for not found */ \
        for (unsigned long long arri = 0; arri < sizeof(name)/sizeof((name)[0]); arri++) { \
            if((name)[arri] == (value)) { \
                *out_index = arri; /* the index */ \
                break; \
            } \
        } \
    } while(0)

// Macro to reverse the elements of a generic array
#define varr_reverse(name, index_type) \
    do { \
        size_t count = sizeof(name)/sizeof((name)[0]);  \
        index_type *start = (index_type *)(name); \
        index_type *end = start + (count - 1); \
        while (start < end) { \
            index_type temp = *start; \
            *start = *end; \
            *end = temp; \
            ++start; \
            --end; \
        } \
    } while (0)

// Sorting for Ascending Order
#define sort_ftn_varr_a(a, b) ((a) < (b))

// Sorting for Descending Order
#define sort_ftn_varr_d(a, b) ((a) > (b))

// Macro to sort the elements of a generic array
#define varr_sort(self, index_type, sort_ftn) do { \
        size_t count = varr_size(self);  \
        for (size_t i = 0; i < count - 1; i++) { \
            for (size_t j = i + 1; j < count; j++) { \
                if ((sort_ftn((self)[i], (self)[j]))) { \
                    index_type temp = (self)[i]; \
                    (self)[i] = (self)[j]; \
                    (self)[j] = temp; \
                } \
            } \
        } \
    } while (0)

#endif // __varr__
