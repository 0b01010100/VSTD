#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum STACK_FIELD
{
    STACK_FIELD_STRIDE = sizeof(size_t) * 1,
    STACK_FIELD_LENGTH = sizeof(size_t) * 2,
    STACK_FIELD_CAPACITY=sizeof(size_t) * 3,
    
}STACK_FIELD;
#define STACK_FIELDS_SIZE (STACK_FIELD_CAPACITY + STACK_FIELD_LENGTH + STACK_FIELD_STRIDE)

typedef unsigned char * stack;

stack stack_ctor(size_t index_size, size_t initial_capacity);

size_t stack_get_field(stack sk, STACK_FIELD field);

size_t stack_capacity(stack vec);

size_t stack_index_size(stack vec);

stack  stack_resize(stack vec, size_t capacity);

void * stack_data(stack vec, size_t capacity);

void * stack_get(stack vec, size_t index);

void stack_push(stack vec, size_t index, void * item);