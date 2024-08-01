#include <stdint.h>

/*
   block_ structure:
   Contiguous block of memory
     16 BYTES     STRIDE * COUNT
   [META PART]  +  [BLOCK PART] 
*/

/*
   Meta data for the block
*/
typedef enum BLOCK_META{
   // Index for the count of elements in the block
   BLOCK_COUNT_FIELD = 0, 
   // Index for the stride (size of each element) in the block
   BLOCK_STRIDE_FIELD = 1,
   // Index for the total size of the block (excluding metadata)
   BLOCK_SIZE_FIELD = 2,
   // Index for the size of hidden meta data part of the block
   BLOCK_SIZE_META = 3,
   // Index for the total size of the block (including metadata)
   BLOCK_SIZE_AND_META_SIZE_FIELD = 4 
}BLOCK_META;

/* 
   Represents a block of memory
   Our api will you to esaily 
   track the size of heap block

   block_ structure:
   Contiguous block of memory
   [META PART]  +  [BLOCK PART] 
*/
typedef void * block_;

// Creates a new block of memory with a given type size and count
// Allocates memory to hold type_size * type_count elements + secret metadata on the block
// Returns a pointer to the newly allocated block
block_ _block_ctor(size_t stride, size_t count);

// Creates a copy of memory the given block 
// Allocates memory to hold type_size * type_count elements + secret metadata on the block
// Returns a pointer to the newly allocated block
block_ _block_cctor(block_ block);

size_t block_meta_get(block_ block, BLOCK_META index);

// Creates a new block of memory with a given type size and count
// Allocates memory to hold sizeof(type) * type_count elements + secret metadata on the block
// Returns a pointer to the newly allocated block, skiping meta data
#define block_ctor(type, type_count) _block_ctor(sizeof(type), type_count)

// Reallocates a block of memory, updating its size based on type size and count
// Adjusts the size of the existing block to accommodate new type_size * type_count elements
// Returns a pointer to the reallocated block
block_ block_rector(block_ block, size_t stride, size_t count);

/* 
   Similar to block_rector but reallocates and pushes the existing data to the 
   back of the block buffer, leaving a gap at the start of the block.
   Useful for scenarios where a gap needs to be filled with default values.
   Returns a pointer to the reallocated block
*/
block_ block_rector_pb(block_ block, size_t stride, size_t count);

// Destroys a block of memory, freeing up any allocated resources
// Deallocates the memory block and cleans up resources
// Takes a pointer to the block to be destroyed
void block_dtor(block_ block);