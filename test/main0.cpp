#include <stdio.h>
#include <block.h>
#include <vstr.h>
#include <vstring.h>
#include <vfs.h>
int main() 
{

    vstr s;
    vstr_create_ex(&s, STR_("ssd"), strlen("ssd"), true);
    vstr bias = VSTR(" ");
    vstr_remove(&s, 0, 1, 1);
    printf("%d", s.len);
    return 0;
}