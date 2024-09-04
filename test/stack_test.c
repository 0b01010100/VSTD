#include <stdio.h>
#include <vblock.h>
#include <vstr.h>
#include <vstring.h>
#include <vfs.h>
#include <vstack.h>
#include <vdef.h>
#include <varray.h>

int main() 
{   
    vstack1 * sk = vstack1_create(int, 3, .75, NULL, NULL, NULL);
    int item = 8;
    int* d = NULL;
    vstack_emplace(sk, 1, &item);
    d = vstack_peek(sk);
    printf("top is %d\n", *d);
    item = 7;
    vstack_push(sk, &item);
    vstack_push(sk, &item);
    vstack_push(sk, &item);
    d = vstack_peek(sk);
    printf("top is %d\n", *d);
    vstack_destroy(&sk);
    d = vstack_peek(sk);
    printf("top is %d\n", *d);
    return 0;
}