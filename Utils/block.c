#include <block.h>
#include <stdlib.h>
#include <string.h>
#define META_SIZE (sizeof(size_t) * 2)

void block_meta_set(block_ block, BLOCK_META index, size_t val);

block_ _block_ctor(size_t stride, size_t count)
{
    size_t block_size = stride * count;
    // Allocate memory for metadata and the actual block
    unsigned char* ptr = (unsigned char*)calloc(1, META_SIZE + block_size);
    if (!ptr) return NULL;
    // Point to the actual block(skiping the meta data)
    block_ block = (block_)(ptr + META_SIZE);

    // Store the block count and stride in the metadata area
    block_meta_set(block, BLOCK_COUNT_FIELD, count);
    block_meta_set(block, BLOCK_STRIDE_FIELD, stride);

    // Return the pointer to the block part
    return block;
}

void block_meta_set(block_ block, BLOCK_META index, size_t val)
{
    size_t * meta = (size_t*)(((unsigned char*)block) - META_SIZE);
    meta[index] = val;
}

size_t block_meta_get(block_ block, BLOCK_META index)
{
    // Cast block to an unsigned char pointer to perform byte-wise arithmetic
    // Subtract META_SIZE to get to the start of the metadata
    size_t * meta = (size_t*)(((unsigned char*)block) - META_SIZE);
    
    // Check the index and return the appropriate metadata value
    switch (index) {
        case BLOCK_SIZE_FIELD:
            // Calculate the total size of the block (stride * count)
            return meta[BLOCK_STRIDE_FIELD] * meta[BLOCK_COUNT_FIELD];
        
        case BLOCK_SIZE_META:
            // Return the size of the metadata itself
            return META_SIZE;
        
        case BLOCK_SIZE_AND_META_SIZE_FIELD:
            // Calculate the total size including metadata
            return META_SIZE + (meta[BLOCK_STRIDE_FIELD] * meta[BLOCK_COUNT_FIELD]);
        
        default:
            // Return the value from the metadata array for other indices
            return meta[index];
    }
}

block_ block_rector(block_ block, size_t stride, size_t count) {
    if (block == NULL) return NULL;

    // Calculate new block size
    size_t new_block_size = stride * count;

    // Reallocate the memory block with the new size including metadata
    block = (block_)realloc(block, new_block_size);
    if (block == NULL) return NULL; 
    // Store the block count and stride in the metadata area
    block_meta_set(block, BLOCK_COUNT_FIELD, count);
    block_meta_set(block, BLOCK_STRIDE_FIELD, stride);

    return block;
}

block_ block_rector_pb(block_ block, size_t stride, size_t count) {
    // If the provided block is NULL, return NULL
    if (block == NULL) return NULL;

    // Retrieve the old size of the block from metadata
    size_t old_block_size = block_meta_get(block, BLOCK_SIZE_FIELD);
    // Calculate the new size of the block based on type size and count
    size_t new_block_size = stride * count;
    block = (block_)realloc(block, new_block_size);

    if (block == NULL) return NULL;  
    
    // If the new block size is larger than the old block size, handle the gap space
    if (new_block_size > old_block_size) {
        // Calculate the size of the gap that needs to be filled with zeros
        size_t gap_size = (new_block_size - old_block_size);
        // Move the existing data to the back of the block buffer
        memmove((void*)(((unsigned char*)block) + gap_size), block, old_block_size);
        // Zero out the newly gaped space in the block
        memset(block, 0, gap_size);
    }

    // Store the block count and stride in the metadata area
    block_meta_set(block, BLOCK_COUNT_FIELD, count);
    block_meta_set(block, BLOCK_STRIDE_FIELD, stride);

    // Return the pointer to the reallocated block of memory, excluding metadata
    return block;
}

void block_dtor(block_ block)
{
    free((void*)(((unsigned char*)block) - META_SIZE));
}
