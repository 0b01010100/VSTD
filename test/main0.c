#include <stdlib.h>
#include <stdio.h>
#include <stringex.h>
int main() 
{
	const char * d = "hey";
	const char * d1 = "fellow";
	const char * d2 = "bro";
	const char * c[3] = {d, d1, d2};
	char * f = strjoin_ex(c, 3, "_");
	printf(f);
	return 0;
}