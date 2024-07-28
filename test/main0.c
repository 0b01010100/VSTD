#include <stdlib.h>
#include <stdio.h>
#include <stringex.h>
int main() 
{
	char * temp = strfastinit_ex("string");
	temp = strninsert_ex(temp, strlen(temp), 11, "josj");
	printf("%s",temp);
	return 0;
}