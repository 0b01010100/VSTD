#include <stdlib.h>

/**
 * @brief A simple memory management library with macros for memory allocation, resizing, and deallocation.
 * 
 * This header provides macros to simplify memory management by mimicking C++ style memory allocation, resizing, and deallocation.
 */

// Macro to allocate memory, similar to C++'s `new` keyword
// @param type The type of object to allocate.
// @param count The number of objects to allocate.
// @return A pointer to the allocated memory, or NULL if the allocation fails.
#define vnew(type, count) (type*)calloc(count, sizeof(type))

// Macro to deallocate memory, similar to C++'s `delete` keyword
// @param data Pointer to the memory to free.
#define vdelete(data) free(data)

// Macro to resize a memory block, similar to C's `realloc` function
// @param ptr Pointer to the previously allocated memory block.
// @param type The type of object, used to compute the new size.
// @param new_count The new number of objects.
// @return A pointer to the reallocated memory block, or NULL if the allocation fails.
#define vresize(ptr, type, new_count) (type*)realloc(ptr, (new_count) * sizeof(type))
