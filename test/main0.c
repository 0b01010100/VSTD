#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stringex.h>


int main() {
    char* initial_buffer = "D"; // String literal, treated as read-only
    char* result = strfastinit_snprintf_ex(initial_buffer, 2, "%s", "WORD");



	printf(result);
    return 0;
}
