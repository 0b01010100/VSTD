
#include <stdio.h>
#include <block.h>
#include <vstr.h>
int main() 
{
    vstr s = NULL;
    vstr_create(&s, "100", 3);
    vstr g = NULL;
    vstr_concat(&g, &s, &s);
    vstr_insert(&g, 6, &s);

    int64_t t = 0;
    vstr_toint(&s, &t);

    printf("%lld", t);
    return 0;
}
