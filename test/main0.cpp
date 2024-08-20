#include <stdio.h>
#include <block.h>
#include <vstr.h>
#include <vstring.h>
#include <vfs.h>
#include <vstack.h>

int main() 
{   
    vstack * s = vstack_create(int, 2, .75, NULL);
    
    int d = 2;
    
    vstack_push(s, &d);
    vstack_push(s, &d);
    vstack_push(s, &d);
    vstack_push(s, &d);
    vstack_pop(s, 0);
 
    int * e = (int*)vstack_peek(s, 0);
    printf("%d",  *e);
    e = (int*)vstack_peek(s, 1);
    printf("%d",  *e);//won't work right
    return 0;
}