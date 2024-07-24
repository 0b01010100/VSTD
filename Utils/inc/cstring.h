#include <stdint.h>
typedef char* cstring;

size_t cstring_len(cstring str);

size_t cstring_cat(cstring str0, cstring str1);

size_t cstring_trim(cstring src, cstring unwanted);