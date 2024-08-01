#include <stack.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void stack_set_field(stack sk, STACK_FIELD field, size_t value);

// Init the stack
stack stack_ctor(size_t index_size, size_t initial_capacity) {
    stack sk = (stack)calloc(STACK_FIELDS_SIZE + index_size * initial_capacity, 1);
    stack_set_field(sk, STACK_FIELD_CAPACITY, initial_capacity);
    
}

size_t stack_get_field(stack sk, STACK_FIELD field)
{
    size_t* meta = (size_t*)(sk - STACK_FIELDS_SIZE);
    return meta[field];
}

void stack_set_field(stack sk, STACK_FIELD field, size_t value) {
    size_t* meta = (size_t*)(sk - STACK_FIELDS_SIZE);
    meta[field] = value;
}