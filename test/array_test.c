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
    varray_create(int, d, 3, 1, 2, 3);
    varray_sort(d, int, sort_ftn_varray_d);
    //varray_reverse(d, int);
    printf("%d",d[2]);
    return 0;
}