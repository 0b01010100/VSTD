#include <stdlib.h>
#include <cstring.h>
#include <string.h>
#include <filesystem.h>
int main() 
{
	char * t = FS_abs("d");
	bool d = FS_exist(t);
	return 0;
}