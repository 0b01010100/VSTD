#include <vstack.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
typedef struct vstack
{
    VSTACK_SIZE_T ver;
    VSTACK_SIZE_T stride;       // Size of each item
    VSTACK_SIZE_T size;         // Number of items
    VSTACK_SIZE_T capacity;     // Current capacity
    double scale;        // Resize scale factor
    unsigned char* data; // Stack data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)
} vstack;

typedef struct _vstack1
{
    VSTACK_SIZE_T ver;
    VSTACK_SIZE_T stride;          // Size of each item
    VSTACK_SIZE_T size;            // Number of items
    VSTACK_SIZE_T capacity;        // Current capacity
    double scale;           // Resize scale factor
    unsigned char* data;    // Stack data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)

    vstack1_el_ctor  ctor;  // Element Constructor
    vstack1_el_cctor cctor; // Element Copy constructor
    vstack1_el_dtor  dtor;  // Element Destructor
} _vstack1;

int __def_vstack_ctor(void *location, VSTACK_SIZE_T size, va_list* args, VSTACK_SIZE_T count) { 
    void *arg = va_arg(*args, void *);
    memcpy(location, arg, size);
    return 0;
}

int __def_vstack_cctor(void * location, const void * original, VSTACK_SIZE_T size)
{
    memcpy(location, original, size);
    return 0;
}

void __def_vstack_dtor(void * location, VSTACK_SIZE_T size)
{
    if (location) {
        memset(location, '\0', size);
    }
}

vstack* _vstack_create(VSTACK_SIZE_T stride, VSTACK_SIZE_T initial_capacity, double scale_factor)
{    
    vstack* sk = (vstack*)malloc(sizeof(vstack));
    if (!sk) return NULL;

    sk->ver = VSTACK_VER_0_0;
    sk->stride = stride;
    sk->size = 0;
    sk->capacity = initial_capacity;
    sk->scale = scale_factor;
    sk->data = (unsigned char*)calloc(initial_capacity, stride);
    if (!sk->data) {
        free(sk);
        return NULL;
    }

    return sk;
}

vstack1* _vstack1_create(VSTACK_SIZE_T stride, VSTACK_SIZE_T initial_capacity, double scale_factor, vstack1_el_ctor ctor, vstack1_el_cctor cctor, vstack1_el_dtor dtor)
{    
    _vstack1* sk = (_vstack1*)malloc(sizeof(_vstack1));
    if (!sk) return NULL;

    sk->ver = VSTACK_VER_1_0;
    sk->stride = stride;
    sk->size = 0;
    sk->capacity = initial_capacity;
    sk->scale = scale_factor;
    sk->data = (unsigned char*)malloc(initial_capacity * stride);

    // version 1.0 stuff

    sk->ctor = (ctor)? ctor : __def_vstack_ctor;
    sk->cctor = (cctor)? cctor : __def_vstack_cctor;
    sk->dtor = (dtor)? dtor : __def_vstack_dtor;
    if (!sk->data) {
        free(sk);
        return NULL;
    }

    return (vstack1*)sk;
}

VSTACK_SIZE_T vstack_get_field(vstack* sk, VSTACK_FIELD field)
{
    if (!sk) return 0;
    switch (field)
    {
        case VSTACK_FIELD_VER:          return sk->ver;
        case VSTACK_FIELD_STRIDE:       return sk->stride;
        case VSTACK_FIELD_LENGTH:       return sk->size;
        case VSTACK_FIELD_CAPACITY:     return sk->capacity;
        case VSTACK_FIELD_SCALE_PERCENT:return sk->scale;
        default:                        break;
    }
    if(sk->ver == VSTACK_VER_0_0) return 0;
    // version 1.0 stuff
    switch (field)
    {
        case VSTACK1_FIELD_CTOR:        return (VSTACK_SIZE_T)((_vstack1*)sk)->ctor;
        case VSTACK1_FIELD_CCTOR:       return (VSTACK_SIZE_T)((_vstack1*)sk)->cctor;
        case VSTACK1_FIELD_DTOR:        return (VSTACK_SIZE_T)((_vstack1*)sk)->dtor;
        default:                        return 0;
    }
}

inline int vstack_to_ver(vstack* sk, VSTACK_SIZE_T ver){
    if(ver == VSTACK_VER_1_0){
        _vstack1 * new_data = (_vstack1 *)realloc(sk, sizeof(vstack) + (sizeof(vstack) + sizeof(_vstack1)));
        new_data->ctor = __def_vstack_ctor;
        new_data->cctor = __def_vstack_cctor;
        new_data->dtor = __def_vstack_dtor;
        if(!new_data) return -1;
    }else{//VSTACK_VER_0_0
        vstack * new_data = (vstack*)realloc(sk, sizeof(vstack));
        if(!new_data) return -1;
    }
    return 0;
}

inline int vstack_resize(vstack* sk, VSTACK_SIZE_T new_size) {
    if (new_size > sk->capacity) {
        
        void* new_data = realloc(sk->data, new_size * sk->stride);
        if (!new_data) return -1; 

        sk->data = new_data;
        sk->capacity = new_size; 
    } else if (new_size < sk->size) {
        
        for (VSTACK_SIZE_T i = sk->size; i != new_size; i--) {
            vstack_pop(sk);
        }
    }

    sk->size = new_size; 
    return 0;
}

void vstack_set_field(vstack* sk, VSTACK_FIELD field, void* value)
{
    if (!sk) return;

    switch (field)
    {
        case VSTACK_FIELD_VER:           {vstack_to_ver(sk, *((VSTACK_SIZE_T*)value)); return;}
        case VSTACK_FIELD_CAPACITY:
        case VSTACK_FIELD_LENGTH:        {(vstack_resize(sk, *((VSTACK_SIZE_T*)value))); return;}
        case VSTACK_FIELD_SCALE_PERCENT: {(sk->scale = *((double*)value)); return;};
        default: break;
    }
    if(sk->ver == VSTACK_VER_0_0) return;
    // version 1.0 stuff
    switch (field)
    {
        case VSTACK1_FIELD_CTOR:        {((_vstack1*)sk)->ctor; return;}
        case VSTACK1_FIELD_CCTOR:       {((_vstack1*)sk)->cctor; return;}
        case VSTACK1_FIELD_DTOR:        {((_vstack1*)sk)->dtor; return;}
        default:                        return;
    }
}

int vstack_scale(vstack* sk)
{
    if (!sk) return -1;

    VSTACK_SIZE_T new_capacity = (VSTACK_SIZE_T)(sk->capacity * (1 + sk->scale));
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
    return (sk)? &sk->data[(sk->size - 1) * sk->stride] : NULL;
}

int vstack_push(vstack* sk, const void* original)
{
    if (!sk || !original) return -1;

    if (sk->size >= sk->capacity)
    {
        if (vstack_scale(sk) == -1) return -1;
    }
    int ret = 0;
    if (sk->ver == VSTACK_VER_0_0){ 
        ret = __def_vstack_cctor(sk->data + (sk->size * sk->stride), original, sk->stride);
    } 
    else 
    {
        ret = ((_vstack1*)sk)->cctor(sk->data + (sk->size * sk->stride), original, sk->stride);
    }

    if(ret == -1) return -1;

    sk->size++;
    
    return 0;
}

int vstack_emplace(vstack* sk, VSTACK_SIZE_T count, ...)
{
    if(!sk || !count) return -1;

    if (sk->size >= sk->capacity)
    {
        if (vstack_scale(sk) == -1) return -1;
    }

    VSTACK_SIZE_T el_size = sk->stride;
    va_list args;
    va_start(args, count);
    
    int ret = 0;
    if (sk->ver == VSTACK_VER_0_0){ 
        __def_vstack_ctor(sk->data + ((sk->size) * sk->stride), sk->stride, &args, count);
    } 
    else 
    {
        ((_vstack1*)sk)->ctor(sk->data + ((sk->size) * sk->stride), sk->stride, &args, count);
    } 

    va_end(args);

    if(ret == -1) return -1;

    sk->size++;
    return 0;
}

inline void vstack_pop_NO_CHECK(vstack* sk)
{
    if (sk->ver == VSTACK_VER_0_0){ 
        __def_vstack_dtor(sk->data + ((sk->size - 1) * sk->stride), sk->stride);
    } 
    else 
    {
        ((_vstack1*)sk)->dtor(sk->data + ((sk->size - 1) * sk->stride), sk->stride);
    }

    sk->size--;
    sk->capacity++;
}

void vstack_pop(vstack* sk)
{
    if (!sk) return;
    vstack_pop_NO_CHECK(sk);
}

void vstack_erase(vstack* sk, VSTACK_SIZE_T last)
{
    if (!sk || sk->size == 0 || last >= sk->size) return;
    
    //top of the stack
    VSTACK_SIZE_T first = sk->size - 1;
    
    for (VSTACK_SIZE_T i = first; i >= last; i--)
    {
        if (sk->ver == VSTACK_VER_0_0) {
            __def_vstack_dtor(sk->data + (i * sk->stride), sk->stride);
        } else {
            ((_vstack1*)sk)->dtor(sk->data + (i * sk->stride), sk->stride);
        }
    }
    
    if (last < sk->size - 1) {
        memmove(sk->data + (last * sk->stride), 
                sk->data + ((last + 1) * sk->stride), 
                (sk->size - last - 1) * sk->stride);
    }
    
    sk->size -= (first - last + 1);
}

void vstack_clear(vstack* sk)
{
    if (!sk || sk->size == 0) return;
    for (VSTACK_SIZE_T i = 0; i < sk->size; i++)
    {
        vstack_pop_NO_CHECK(sk);
    }
    sk->size = 0x0;
}

void vstack_swap(vstack* lhs, vstack* rhs)
{
    assert(lhs == rhs && "pointers are restricted from pointing to the same address");
    if(lhs->stride != rhs->stride) printf("vstack Warning: stride is not the same size for {lhs} and {rhs} in file: %s line: %d", __FILE__, __LINE__);
    //swaping things like meta data and the arrary data pointers
    if(lhs->ver == VSTACK_VER_1_0 && rhs->ver == VSTACK_VER_1_0){
        vstack1 temp = *lhs;
        *lhs = *rhs;
        *rhs = temp;
        return;
    }
    vstack temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
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
