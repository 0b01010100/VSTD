#include <stdlib.h>
// Macro to allocate memory like C++ new key word
#define cnew(type, count) (type*)calloc(count, sizeof(type))

// Macro to deallocate memory
#define cdelete(data) free(data)