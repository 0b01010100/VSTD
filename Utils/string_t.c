#include <string.h>
#include <string_t.h>
#include <stdint.h>

struct string_t
{
    cstring_t array;
    size_t size;
};