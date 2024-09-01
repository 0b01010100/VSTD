/**
 * @file varray.h
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
 * - `varray_create`: Creates a fixed-size array.
 * - `varray_size`: Returns the size of an array.
 * - `varray_get`: Accesses an element with bounds checking.
 * - `varray_set`: Sets an element with bounds checking.
 * - `varray_fill`: Fills the array with a specific value.
 * - `varray_find`: Finds an element in the array.
 * - `varray_reverse`: Reverses the order of elements in the array.
 * - `varray_sort`: Sorts the array in any order you want
 *
 * The macros handle errors by printing messages to stderr and exiting the program
 * if an out-of-bounds access occurs.
 *
 * This file is intended to be used as a header and should be included with `#include "varray.h"`.
 *
 * Usage example:
 * @code
 * varray_create(int, my_array, 10, 1, 2, 3, 4, 5);
 * //____________________va args^
 * varray_fill(my_array, 0);
 * varray_set(my_array, 1, 42);
 * int val = varray_get(my_array, 1);
 * @endcode
 */

#ifndef __varray__
#define __varray__

#if __cplusplus
#include <cstdlib>
#include <iostream>
#define __vErr_varray_Err(msg)  std::cerr << msg
#define __vSTD_varray_ std::
#else
#include <stdlib.h>
#include <stdio.h>
#define __vErr_varray_Err(msg)  fprintf(stderr, "%s", msg)
#define __vSTD_varray_
#endif

// Create an array with a fixed maximum size
#define varray_create(T, name, size_max, ...) \
    T name[size_max] = {__VA_ARGS__}

// Returns the size of the array
#define varray_size(name) \
    (sizeof(name)/sizeof((name)[0]))

// Access an element in the array
#define varray_get(name, index) \
    ((index) >= varray_size(name)) ? (__vErr_varray_Err("Index out of range\n"), __vSTD_varray_ exit(EXIT_FAILURE), (name)[0]) : (name)[index]

// Set an element in the array
#define varray_set(name, index, value) \
    ((index) >= sizeof(name)/sizeof((name)[0])) ? (__vErr_varray_Err("Index out of range\n"), __vSTD_varray_ exit(EXIT_FAILURE)) : ((name)[index] = (value))

// Fill the array with a specific value
#define varray_fill(name, value) \
    do { \
        for (unsigned long long arri = 0; arri < sizeof(name)/sizeof((name)[0]); arri++) { \
            (name)[arri] = (value); \
        } \
    } while(0)

// Find an element in the array
#define varray_find(name, value, out_index) \
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
#define varray_reverse(name, index_type) \
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

//sorting for Ascending Order
#define sort_ftn_varray_a(a, b) ((a) < (b))

//sorting for Descending Order
#define sort_ftn_varray_d(a, b) ((a) > (b))

// Macro to sort the elements of a generic array
#define varray_sort(self, index_type, sort_ftn) \
    do { \
        size_t count = varray_size(self);  \
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

#undef __vErr_varray_Err
#undef __vSTD_varray_

#endif // __varray__
