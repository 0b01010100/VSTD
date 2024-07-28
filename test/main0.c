#include <stdlib.h>
#include <stdio.h>
#include <stringex.h>
int main() 
{
	char * temp = strfastinit_ex("string");
	temp = strninsert_ex(temp, strlen(temp), 7, "my");
	printf("%s",temp);
	printf("%zu",strlen(temp));
	return 0;
}