#include <stdio.h>
#include <block.h>
#include <vstr.h>

int main() 
{
    vstr s = {0};
    vstr_create_ex(&s, "100", 3, 1);


    int x = 8, y = 7;

    vstr t = VSTR("__");
    vstr w = vstr_join(&t, 3, VSTR("Well"), VSTR("Well"), VSTR("Well"));
    vstr t1 = VSTR("We");
    vstr_trim(&w, &t1);
    printf(w.str);
    size_t count = 0;
    vstr** d = vstr_split(&w, &t, &count);
    printf(d[0]->str);
    printf(d[1]->str);
    printf(d[2]->str);
    return 0;
}
