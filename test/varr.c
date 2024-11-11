#include <stdio.h>
#include <varray.h>
int main() 
{   
    varray_create(int, d, 3, 1, 2, 3);
    varray_sort(d, int, sort_ftn_varray_d);
    varray_reverse(d, int);
    printf("%d\n",varray_get(d, 0));
    printf("%d",varray_get(d, 10));
    return 0;
}