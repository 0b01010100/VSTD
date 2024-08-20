#include <vstack.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct vstack
{
    size_t stride;  // Size of each item
    size_t size;    // Number of items
    size_t capacity; // Current capacity
    double scale;   // Resize scale factor
    unsigned char* data; // Stack data
    vstack_el_dtor dtor; // Element Destructor
} vstack;

void vstack_defdtor(void * element, size_t size)
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
    sk->dtor = (dtor)? dtor : vstack_defdtor;
    if (!sk->data) {
        free(sk);
        return NULL;
    }

    return sk;
}

size_t vstack_get_field(vstack* sk, VSTACK_FIELD field)
{
    if (!sk) return 0;

    switch (field)
    {
        case VSTACK_FIELD_STRIDE: return sk->stride;
        case VSTACK_FIELD_LENGTH: return sk->size;
        case VSTACK_FIELD_CAPACITY: return sk->capacity;
        case VSTACK_FIELD_SCALE_PERCENT: return (size_t)(sk->scale * 100);
        default: return 0;
    }
}

int vstack_diolate(vstack* sk)
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

int vstack_resize(vstack* sk, size_t capacity)
{
    if (!sk) return -1;
    if(capacity < sk->size)
    {
        for (size_t i = 0; i < sk->size - capacity; i++)
        {
            sk->dtor((sk->data+(sk->stride * i)), sk->stride);
            //memset((sk->data+(sk->stride * i)), 0, sk->stride);
            sk->size--;
        }
    }
    unsigned char* new_data = (unsigned char*)realloc(sk->data, capacity * sk->stride);
    if (!new_data) return -1;

    sk->data = new_data;
    sk->capacity = capacity;

    return 0;
}

const void* vstack_data(vstack* sk)
{
    return sk->data;
}

void* vstack_peek(vstack* sk, size_t index)
{
    return (index >= sk->size) ? NULL : sk->data + ((sk->size - 1 - index) * sk->stride);
}

int vstack_push(vstack* sk, const void* item)
{
    if (!sk || !item) return -1;

    if (sk->size >= sk->capacity)
    {
        if (vstack_diolate(sk) != 0) return -1;
    }

    memcpy((sk->data) + sk->size * sk->stride, item, sk->stride);
    sk->size++;
    
    return 0;
}

void vstack_pop(vstack* sk, size_t index)
{
    if (!sk || index >= sk->size) return;
    
    sk->dtor(sk->data + ((sk->size - 1 - index) * sk->stride), sk->stride);
    
    memmove(sk->data + index * sk->stride, sk->data + (index + 1) * sk->stride, (sk->size - index - 1) * sk->stride);

    sk->size--;
    sk->capacity++;
}

void vstack_destroy(vstack** sk)
{
    if (sk && *sk)
    {
        free((*sk)->data);
        free(*sk);
        *sk = NULL;
    }
}
