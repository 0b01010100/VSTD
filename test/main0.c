
#include <stdio.h>
#include <block.h>
int main() 
{
    int* b = block_ctor(int, 2);

    size_t size = block_meta_get(b, BLOCK_SIZE_AND_META_SIZE_FIELD);
    printf("%zu", size);
    block_dtor(b);
    return 0;
}
