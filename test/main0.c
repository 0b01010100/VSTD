#include <stdlib.h>
#include <cstring.h>
#include <string.h>

int main() 
{
	cstring s = malloc(sizeof(cstring) * 10ULL);
	memset(&s, 0, sizeof(char));
	cstring_concat("1", "2", s);
	return 0;
}