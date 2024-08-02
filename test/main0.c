
#include <stdio.h>
#include <block.h>
#include <vstr.h>
int main() 
{
    vstr s = NULL;
    vstr_create(&s, "CO.", 3);
    vstr g;
    vstr_concat(&g, &s, &s);
    printf(g);
    vstr_fill(&g, '4');
    printf(g);
    return 0;
}
