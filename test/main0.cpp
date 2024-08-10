#include <stdio.h>
#include <block.h>
#include <vstr.h>
#include <filesystem.h>
int main() 
{
    char * d = NULL;
    FS_text_to_str(FS_PATH("test.cpp"), &d);
    printf(d);
    return 0;
}