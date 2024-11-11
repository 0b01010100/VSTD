#include <vsk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct vsk_t
{
    size_t ver;
    size_t stride;       // Size of each item
    size_t size;         // Number of items
    size_t capacity;     // Current capacity
    double scale;        // Resize scale factor
    unsigned char* data; // Stack data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)
} vsk_t;

typedef struct _vsk1_t
{
    size_t ver;
    size_t stride;          // Size of each item
    size_t size;            // Number of items
    size_t capacity;        // Current capacity
    double scale;           // Resize scale factor
    unsigned char* data;    // Stack data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)

    vsk1_ctor_t  ctor;  // Element Constructor
    vsk1_cctor_t cctor; // Element Copy constructor
    vsk1_dtor_t  dtor;  // Element Destructor
} _vsk1_t;

int __def_vsk_ctor(void *self, size_t size, va_list* args, size_t count) { 
    void *arg = va_arg(*args, void *);
    memcpy(self, arg, size);
    return 0;
}

int __def_vsk_cctor(void * self, const void * original, size_t size)
{
    memcpy(self, original, size);
    return 0;
}

void __def_vsk_dtor(void * self, size_t size)
{
    if (self) {
        memset(self, '\0', size);
    }
}

vsk_t* _vsk_create(size_t stride, size_t initial_capacity, double scale_factor)
{    
    vsk_t* sk = (vsk_t*)malloc(sizeof(vsk_t));
    if (!sk) return NULL;

    sk->ver = VSK_VER_0_0;
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

vsk1_t* _vsk1_create(size_t stride, size_t initial_capacity, double scale_factor, vsk1_ctor_t ctor, vsk1_cctor_t cctor, vsk1_dtor_t dtor)
{    
    _vsk1_t* sk = (_vsk1_t*)malloc(sizeof(_vsk1_t));
    if (!sk) return NULL;

    sk->ver = VSK_VER_1_0;
    sk->stride = stride;
    sk->size = 0;
    sk->capacity = initial_capacity;
    sk->scale = scale_factor;
    sk->data = (unsigned char*)malloc(initial_capacity * stride);

    // version 1.0 stuff

    sk->ctor = (ctor)? ctor : __def_vsk_ctor;
    sk->cctor = (cctor)? cctor : __def_vsk_cctor;
    sk->dtor = (dtor)? dtor : __def_vsk_dtor;
    if (!sk->data) {
        free(sk);
        return NULL;
    }

    return (vsk1_t*)sk;
}

size_t vsk_get_field(vsk_t* sk, VSK_FIELD field)
{
    if (!sk) return 0;
    switch (field)
    {
        case VSK_FIELD_VER:          return sk->ver;
        case VSK_FIELD_STRIDE:       return sk->stride;
        case VSK_FIELD_LENGTH:       return sk->size;
        case VSK_FIELD_CAPACITY:     return sk->capacity;
        case VSK_FIELD_SCALE_PERCENT:return sk->scale;
        default:                        break;
    }
    if(sk->ver == VSK_VER_0_0) return 0;
    // version 1.0 stuff
    switch (field)
    {
        case VSK1_FIELD_CTOR:        return (size_t)((_vsk1_t*)sk)->ctor;
        case VSK1_FIELD_CCTOR:       return (size_t)((_vsk1_t*)sk)->cctor;
        case VSK1_FIELD_DTOR:        return (size_t)((_vsk1_t*)sk)->dtor;
        default:                        return 0;
    }
}

inline int vsk_to_ver(vsk_t* sk, size_t ver){
    if(ver == VSK_VER_1_0){
        _vsk1_t * new_data = (_vsk1_t *)realloc(sk, sizeof(_vsk1_t));
        new_data->ctor = __def_vsk_ctor;
        new_data->cctor = __def_vsk_cctor;
        new_data->dtor = __def_vsk_dtor;
        if(!new_data) return -1;
    }else{//VSK_VER_0_0
        vsk_t * new_data = (vsk_t*)realloc(sk, sizeof(vsk_t));
        if(!new_data) return -1;
    }
    return 0;
}

inline int vsk_resize(vsk_t* sk, size_t new_size) {
    if (new_size > sk->capacity) {
        
        void* new_data = realloc(sk->data, new_size * sk->stride);
        if (!new_data) return -1; 

        sk->data = new_data;
        sk->capacity = new_size; 
    } else if (new_size < sk->size) {
        
        for (size_t i = sk->size; i != new_size; i--) {
            vsk_pop(sk);
        }
    }

    sk->size = new_size; 
    return 0;
}

void vsk_set_field(vsk_t* sk, VSK_FIELD field, void* value)
{
    if (!sk) return;

    switch (field)
    {
        case VSK_FIELD_VER:           {vsk_to_ver(sk, *((size_t*)value)); return;}
        case VSK_FIELD_CAPACITY:
        case VSK_FIELD_LENGTH:        {(vsk_resize(sk, *((size_t*)value))); return;}
        case VSK_FIELD_SCALE_PERCENT: {(sk->scale = *((double*)value)); return;};
        default: break;
    }
    if(sk->ver == VSK_VER_0_0) return;
    // version 1.0 stuff
    switch (field)
    {
        case VSK1_FIELD_CTOR:        {((_vsk1_t*)sk)->ctor; return;}
        case VSK1_FIELD_CCTOR:       {((_vsk1_t*)sk)->cctor; return;}
        case VSK1_FIELD_DTOR:        {((_vsk1_t*)sk)->dtor; return;}
        default:                        return;
    }
}

int vsk_scale(vsk_t* sk)
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

void* vsk_peek(vsk_t* sk)
{   
    return (sk)? &sk->data[(sk->size - 1) * sk->stride] : NULL;
}

int vsk_push(vsk_t* sk, const void* original)
{
    if (!sk || !original) return -1;

    if (sk->size >= sk->capacity)
    {
        if (vsk_scale(sk) == -1) return -1;
    }
    int ret = 0;
    if (sk->ver == VSK_VER_0_0){ 
        ret = __def_vsk_cctor(sk->data + (sk->size * sk->stride), original, sk->stride);
    } 
    else 
    {
        ret = ((_vsk1_t*)sk)->cctor(sk->data + (sk->size * sk->stride), original, sk->stride);
    }

    if(ret == -1) return -1;

    sk->size++;
    
    return 0;
}

int vsk_emplace(vsk_t* sk, size_t count, ...)
{
    if(!sk || !count) return -1;

    if (sk->size >= sk->capacity)
    {
        if (vsk_scale(sk) == -1) return -1;
    }

    size_t el_size = sk->stride;
    va_list args;
    va_start(args, count);
    
    int ret = 0;
    if (sk->ver == VSK_VER_0_0){ 
        __def_vsk_ctor(sk->data + ((sk->size) * sk->stride), sk->stride, &args, count);
    } 
    else 
    {
        ((_vsk1_t*)sk)->ctor(sk->data + ((sk->size) * sk->stride), sk->stride, &args, count);
    } 

    va_end(args);

    if(ret == -1) return -1;

    sk->size++;
    return 0;
}

inline void vsk_pop_NO_CHECK(vsk_t* sk)
{
    if (sk->ver == VSK_VER_0_0){ 
        __def_vsk_dtor(sk->data + ((sk->size - 1) * sk->stride), sk->stride);
    } 
    else 
    {
        ((_vsk1_t*)sk)->dtor(sk->data + ((sk->size - 1) * sk->stride), sk->stride);
    }

    sk->size--;
    sk->capacity++;
}

void vsk_pop(vsk_t* sk)
{
    if (!sk) return;
    vsk_pop_NO_CHECK(sk);
}

void vsk_erase(vsk_t* sk, size_t last)
{
    if (!sk || sk->size == 0 || last >= sk->size) return;
    
    //top of the stack
    size_t first = sk->size - 1;
    
    for (size_t i = first; i >= last; i--)
    {
        if (sk->ver == VSK_VER_0_0) {
            __def_vsk_dtor(sk->data + (i * sk->stride), sk->stride);
        } else {
            ((_vsk1_t*)sk)->dtor(sk->data + (i * sk->stride), sk->stride);
        }
    }
    
    if (last < sk->size - 1) {
        memmove(sk->data + (last * sk->stride), 
                sk->data + ((last + 1) * sk->stride), 
                (sk->size - last - 1) * sk->stride);
    }
    
    sk->size -= (first - last + 1);
}

void vsk_clear(vsk_t* sk)
{
    if (!sk || sk->size == 0) return;
    for (size_t i = 0; i < sk->size; i++)
    {
        vsk_pop_NO_CHECK(sk);
    }
    sk->size = 0x0;
}

void vsk_swap(vsk_t* lhs, vsk_t* rhs)
{
    assert(lhs == rhs && "pointers are restricted from pointing to the same address");
    if(lhs->stride != rhs->stride) printf("vsk Warning: stride is not the same size for {lhs} and {rhs} in file: %s line: %d", __FILE__, __LINE__);
    //swaping things like meta data and the arrary data pointers
    if(lhs->ver == VSK_VER_1_0 && rhs->ver == VSK_VER_1_0){
        _vsk1_t temp = *(_vsk1_t*)lhs;
        *(_vsk1_t*)lhs = *(_vsk1_t*)rhs;
        *(_vsk1_t*)rhs = temp;
        return;
    }
    vsk_t temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

void vsk_destroy(vsk_t** sk)
{
    if (sk && *sk)
    {
        while((*sk)->size != 0){
            vsk_pop(*sk);
        }
        free((*sk)->data);
        free(*sk);
        *sk = NULL;
    }
}
