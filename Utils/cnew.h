#include <stdlib.h>
#define cnew(type, count) (type*)calloc(count, 4)

#define cdelete(data) free(data)