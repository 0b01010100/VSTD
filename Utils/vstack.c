#include <vstack.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct vstack
{
    size_t stride;       // Size of each item
    size_t size;         // Number of items
    size_t capacity;     // Current capacity
    double scale;        // Resize scale factor
    unsigned char* data; // Stack data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)
    vstack_el_dtor dtor; // Element Destructor
} vstack;

void __def_vstack_dtor(void * element, size_t size)
{
    if (element) {
        memset(element, '\0', size);
    }
}

vstack* _vstack_create(size_t stride, size_t initial_capacity, double scale_factor, vstack_el_dtor dtor)
{    
    vstack* sk = (vstack*)malloc(sizeof(vstack));
    if (!sk) return NULL;

    sk->stride = stride;
    sk->size = 0;
    sk->capacity = initial_capacity;
    sk->scale = scale_factor;
    sk->data = (unsigned char*)malloc(initial_capacity * stride);
    sk->dtor = (dtor)? dtor : __def_vstack_dtor;
    if (!sk->data) {
        free(sk);
        return NULL;
    }

    return sk;
}

const void* vstack_get_field(vstack* sk, VSTACK_FIELD field)
{
    if (!sk) return 0;

    switch (field)
    {
        case VSTACK_FIELD_STRIDE:       return &sk->stride;
        case VSTACK_FIELD_LENGTH:       return &sk->size;
        case VSTACK_FIELD_CAPACITY:     return &sk->capacity;
        case VSTACK_FIELD_SCALE_PERCENT:return &sk->scale;
        case VSTACK_FIELD_DTOR:         return &sk->dtor;
        default: return 0;
    }
}

void vstack_set_field(vstack* sk, VSTACK_FIELD field, void* value)
{
    if (!sk) return;

    switch (field)
    {
        case VSTACK_FIELD_SCALE_PERCENT:sk->scale    = *((double*)value);
        case VSTACK_FIELD_DTOR:         sk->dtor     = *((vstack_el_dtor)value);
        default: return;
    }
}

int vstack_scale(vstack* sk)
{
    if (!sk) return -1;

    size_t new_capacity = (size_t)(sk->capacity * (1 + sk->scale));
    unsigned char* new_data = (unsigned char*)realloc(sk->data, new_capacity * sk->stride);

    if (!new_data) return -1;

    sk->data = new_data;
    sk->capacity = new_capacity;

    if (sk->size > sk->capacity) {
        sk->size = sk->capacity;
    }

    return 0;
}

void* vstack_peek(vstack* sk)
{   
    return (sk)? (void*)(sk->data[(sk->size - 1) * sk->stride]) : NULL;
}

int vstack_push(vstack* sk, const void* item)
{
    if (!sk || !item) return -1;

    if (sk->size >= sk->capacity)
    {
        if (vstack_scale(sk) == -1) return -1;
    }

    memcpy((sk->data) + sk->size * sk->stride, item, sk->stride);
    sk->size++;
    
    return 0;
}

void vstack_pop(vstack* sk)
{
    if (!sk) return;
    
    sk->dtor(sk->data + ((sk->size - 1) * sk->stride), sk->stride);

    sk->size--;
    sk->capacity++;
}

void vstack_destroy(vstack** sk)
{
    if (sk && *sk)
    {
        while((*sk)->size != 0){
            vstack_pop(*sk);
        }
        free((*sk)->data);
        free(*sk);
        *sk = NULL;
    }
}
