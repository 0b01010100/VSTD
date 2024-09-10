#include <vvector.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
typedef struct vvector
{
    size_t ver;
    size_t stride;       // Size of each item
    size_t size;         // Number of items
    size_t capacity;     // Current capacity
    double scale;        // Resize scale factor
    unsigned char* data; // Stack data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)
} vvector;

typedef struct _vvector1
{
    size_t ver;
    size_t stride;          // Size of each item
    size_t size;            // Number of items
    size_t capacity;        // Current capacity
    double scale;           // Resize scale factor
    unsigned char* data;    // Stack data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)

    // version 1.0 stuff

    vvector1_el_ctor  ctor;  // Element Constructor
    vvector1_el_cctor cctor; // Element Copy constructor
    vvector1_el_dtor  dtor;  // Element Destructor
} _vvector1;

int __def_vvector_ctor(void *location, size_t size, va_list* args, size_t count) { 
    void *arg = va_arg(*args, void *);
    memcpy(location, arg, size);
    return 0;
}

int __def_vvector_cctor(void * location, const void * original, size_t size)
{
    memcpy(location, original, size);
    return 0;
}

void __def_vvector_dtor(void * location, size_t size)
{
    if (location) {
        memset(location, '\0', size);
    }
}

vvector* _vvector_create(size_t stride, size_t initial_capacity, double scale_factor)
{    
    vvector* vec = (vvector*)malloc(sizeof(vvector));
    if (!vec) return NULL;

    vec->ver = VVECTOR_VER_0_0;
    vec->stride = stride;
    vec->size = 0;
    vec->capacity = initial_capacity;
    vec->scale = scale_factor;
    vec->data = (unsigned char*)calloc(initial_capacity, stride);
    if (!vec->data) {
        free(vec);
        return NULL;
    }

    return vec;
}

vvector1* _vvector1_create(size_t stride, size_t initial_capacity, double scale_factor, vvector1_el_ctor ctor, vvector1_el_cctor cctor, vvector1_el_dtor dtor)
{    
    _vvector1* vec = (_vvector1*)malloc(sizeof(_vvector1));
    if (!vec) return NULL;

    vec->ver = VVECTOR_VER_1_0;
    vec->stride = stride;
    vec->size = 0;
    vec->capacity = initial_capacity;
    vec->scale = scale_factor;
    vec->data = (unsigned char*)malloc(initial_capacity * stride);

    // version 1.0 stuff

    vec->ctor = (ctor)? ctor : __def_vvector_ctor;
    vec->cctor = (cctor)? cctor : __def_vvector_cctor;
    vec->dtor = (dtor)? dtor : __def_vvector_dtor;
    if (!vec->data) {
        free(vec);
        return NULL;
    }

    return (vvector1*)vec;
}

size_t vvector_get_field(vvector* vec, VVECTOR_FIELD field)
{
    if (!vec) return 0;
    switch (field)
    {
        case VVECTOR_FIELD_VER:          return vec->ver;
        case VVECTOR_FIELD_STRIDE:       return vec->stride;
        case VVECTOR_FIELD_LENGTH:       return vec->size;
        case VVECTOR_FIELD_CAPACITY:     return vec->capacity;
        case VVECTOR_FIELD_SCALE_PERCENT:return vec->scale;
        default:                        break;
    }

    if(vec->ver == VVECTOR_VER_0_0) return 0;

    // version 1.0 stuff
    switch (field)
    {
        case VVECTOR1_FIELD_CTOR:        return (size_t)((_vvector1*)vec)->ctor;
        case VVECTOR1_FIELD_CCTOR:       return (size_t)((_vvector1*)vec)->cctor;
        case VVECTOR1_FIELD_DTOR:        return (size_t)((_vvector1*)vec)->dtor;
        default:                         return 0;
    }
}

inline int vvector_to_ver(vvector* vec, size_t ver){
    if(ver == VVECTOR_VER_1_0){
        _vvector1 * new_data = (_vvector1 *)realloc(vec, sizeof(vvector) + (sizeof(vvector) + sizeof(_vvector1)));
        new_data->ctor = __def_vvector_ctor;
        new_data->cctor = __def_vvector_cctor;
        new_data->dtor = __def_vvector_dtor;
        if(!new_data) return -1;
    }else{//VVECTOR_VER_0_0
        vvector * new_data = (vvector*)realloc(vec, sizeof(vvector));
        if(!new_data) return -1;
    }
    return 0;
}

int _vvector_resize(vvector* vec, size_t new_size) {
    if (new_size > vec->capacity) {
        
        void* new_data = realloc(vec->data, new_size * vec->stride);
        if (!new_data) return -1; 

        vec->data = new_data;
        vec->capacity = new_size; 
    } else if (new_size < vec->size) {
        
        for (size_t i = vec->size; i != new_size; i--) {
            vvector_pop_at(vec, -1);
        }
    }

    vec->size = new_size; 
    return 0;
}

void vvector_set_field(vvector* vec, VVECTOR_FIELD field, void* value)
{
    if (!vec) return;

    switch (field)
    {
        case VVECTOR_FIELD_VER:           {vvector_to_ver(vec, *((size_t*)value)); return;}
        case VVECTOR_FIELD_CAPACITY:
        case VVECTOR_FIELD_LENGTH:        {(_vvector_resize(vec, *((size_t*)value))); return;}
        case VVECTOR_FIELD_SCALE_PERCENT: {(vec->scale = *((double*)value)); return;};
        default: break;
    }
    if(vec->ver == VVECTOR_VER_0_0) return;
    // version 1.0 stuff
    switch (field)
    {
        case VVECTOR1_FIELD_CTOR:        {((_vvector1*)vec)->ctor; return;}
        case VVECTOR1_FIELD_CCTOR:       {((_vvector1*)vec)->cctor; return;}
        case VVECTOR1_FIELD_DTOR:        {((_vvector1*)vec)->dtor; return;}
        default:                        return;
    }
}

void vvector_reserve(vvector* vec, VVECTOR_SIZE_T reserves)
{
    size_t val = vec->size + reserves;
    vvector_set_field(vec, VVECTOR_FIELD_LENGTH, &val);
}

int vvector_scale(vvector* vec)
{
    if (!vec) return -1;

    size_t new_capacity = (size_t)(vec->capacity * (1 + vec->scale));
    unsigned char* new_data = (unsigned char*)realloc(vec->data, new_capacity * vec->stride);

    if (!new_data) return -1;

    vec->data = new_data;
    vec->capacity = new_capacity;

    if (vec->size > vec->capacity) {
        vec->size = vec->capacity;
    }

    return 0;
}

int vvector_insert(vvector* vec, VVECTOR_SSIZE_T index, const void* original)
{
    if (!vec || !original) return -1;

    VVECTOR_SIZE_T nindex = (index < 0) ? (VVECTOR_SIZE_T)(vec->size + index) : (VVECTOR_SIZE_T)index;
    if (nindex > vec->size) return -1;

    if (vec->size >= vec->capacity)
    {
        if (vvector_scale(vec) == -1) return -1;
    }

    //memmov for non -1 indies
    if (nindex < vec->size) 
    {
        memmove(vec->data + (nindex + 1) * vec->stride, 
                vec->data + nindex * vec->stride, 
                (vec->size - nindex) * vec->stride);
    }

    int ret = 0;
    if (vec->ver == VVECTOR_VER_0_0){ 
        ret = __def_vvector_cctor(vec->data + (nindex * vec->stride), original, vec->stride);
    } 
    else 
    {
        ret = ((_vvector1*)vec)->cctor(vec->data + (nindex * vec->stride), original, vec->stride);
    }

    if(ret == -1) return -1;

    vec->size++;
    
    return 0;
}

int vvector_push_back(vvector* vec, const void* original)
{
    if (!vec || !original) return -1;

    if (vec->size >= vec->capacity)
    {
        if (vvector_scale(vec) == -1) return -1;
    }

    int ret = 0;
    if (vec->ver == VVECTOR_VER_0_0){ 
        ret = __def_vvector_cctor(vec->data + (vec->size * vec->stride), original, vec->stride);
    } 
    else 
    {
        ret = ((_vvector1*)vec)->cctor(vec->data + (vec->size * vec->stride), original, vec->stride);
    }

    if(ret == -1) return -1;

    vec->size++;
    
    return 0;
}

int vvector_emplace(vvector* vec, VVECTOR_SSIZE_T index, size_t arg_count, ...)
{
    if(!vec || !arg_count) return -1;

    VVECTOR_SIZE_T nindex = (index < 0) ? (VVECTOR_SIZE_T)(vec->size + index) : (VVECTOR_SIZE_T)index;
    if (nindex > vec->size) return -1;

    if (vec->size >= vec->capacity)
    {
        if (vvector_scale(vec) == -1) return -1;
    }

    size_t el_size = vec->stride;
    va_list args;
    va_start(args, arg_count);
    
    //memmov for non -1 indies
    if (nindex < vec->size) //letting nindex == vec->size slide
    {
        memmove(vec->data + (nindex + 1) * vec->stride, 
                vec->data + nindex * vec->stride, 
                (vec->size - nindex) * vec->stride);
    }

    int ret = 0x0;
    if (vec->ver == VVECTOR_VER_0_0){ 
        ret = __def_vvector_ctor(vec->data + (nindex * vec->stride), vec->stride, &args, arg_count);
    } 
    else 
    {
        ret = ((_vvector1*)vec)->ctor(vec->data + (nindex * vec->stride), vec->stride, &args, arg_count);
    } 

    va_end(args);

    if(ret != 0) return ret;

    vec->size++;
    return 0;
}

int vvector_emplace_back(vvector* vec, size_t arg_count, ...)
{
    if(!vec || !arg_count) return -1;

    if (vec->size >= vec->capacity)
    {
        if (vvector_scale(vec) == -1) return -1;
    }

    size_t el_size = vec->stride;
    va_list args;
    va_start(args, arg_count);

    int ret = 0x0;
    if (vec->ver == VVECTOR_VER_0_0){ 
        ret = __def_vvector_ctor(vec->data + (vec->size * vec->stride), vec->stride, &args, arg_count);
    } 
    else 
    {
        ret = ((_vvector1*)vec)->ctor(vec->data + (vec->size * vec->stride), vec->stride, &args, arg_count);
    } 

    va_end(args);

    if(ret != 0) return ret;

    vec->size++;
    return 0;
}

void * vvector_at(vvector* vec, VVECTOR_SSIZE_T index)
{
    VVECTOR_SIZE_T nindex = (index < 0) ? (VVECTOR_SIZE_T)(vec->size + index) : (VVECTOR_SIZE_T)index;
    return (nindex >= vec->size)? NULL : &(vec->data[vec->stride * nindex]);
}

/*internal vvector_pop_at*/
static void _vvector_pop_at(vvector* vec, VVECTOR_SSIZE_T index)
{
    VVECTOR_SIZE_T nindex = (index < 0) ? (VVECTOR_SIZE_T)(vec->size + index) : (VVECTOR_SIZE_T)index;
    if (nindex >= vec->size) return;

    if (vec->ver == VVECTOR_VER_0_0){ 
        __def_vvector_dtor(vec->data + (nindex * vec->stride), vec->stride);
    } 
    else 
    {
        ((_vvector1*)vec)->dtor(vec->data + (nindex * vec->stride), vec->stride);
    }

    // only shift if it's not the last element
    if (nindex < vec->size - 1) 
    {
        memmove(vec->data + (nindex * vec->stride),
                vec->data + ((nindex + 1) * vec->stride),
               (vec->size - nindex - 1) * vec->stride);
    }

    vec->size--;
    vec->capacity++;
}

void vvector_pop_at(vvector* vec, VVECTOR_SSIZE_T index)
{
    if (!vec || vec->size == 0) return;
    _vvector_pop_at(vec, index);
}

void vvector_erase(vvector* vec, VVECTOR_SIZE_T first, VVECTOR_SIZE_T last)
{
    if (!vec || vec->size == 0 || first >= last || last > vec->size) return;
    for (VVECTOR_SIZE_T i = first; i < last + 1; i++)
    {
        if (vec->ver == VVECTOR_VER_0_0) {
            __def_vvector_dtor(vec->data + (i * vec->stride), vec->stride);
        } else {
            ((_vvector1*)vec)->dtor(vec->data + (i * vec->stride), vec->stride);
        }
    }
    if (last < vec->size) {
        memmove(vec->data + (first * vec->stride), 
                vec->data + (last * vec->stride), 
                (vec->size - last) * vec->stride);
    }
    vec->size -= (last - first + 1);
}

void vvector_clear(vvector* vec)
{
    if (!vec || vec->size == 0) return;
    for (size_t i = 0; i < vec->size; i++)
    {
        _vvector_pop_at(vec, i);
    }
    vec->size = 0x0;
}

void vvector_swap(vvector* lhs, vvector* rhs)
{
    assert(lhs == rhs && "pointer are restricted from pointing to the same address");
    assert(lhs->stride != rhs->stride);
    //swaping things like meta data and the arrary data pointers
    if(lhs->ver == VVECTOR_VER_1_0 && rhs->ver == VVECTOR_VER_1_0){
        vvector1 temp = *lhs;
        *lhs = *rhs;
        *rhs = temp;
        return;
    }
    vvector temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
    
}

void vvector_shrink_to_fit(vvector* vec)
{
    if(!vec) return;
    if(vec->capacity == vec->size) return;
    //else
    vec->capacity = vec->size;
    void * new_data = realloc(vec->data, vec->capacity);
}

void vvector_destroy(vvector** vec)
{
    if (vec && *vec)
    {
        while((*vec)->size != 0){
            _vvector_pop_at(*vec, -1);
        }
        free((*vec)->data);
        free(*vec);
        *vec = NULL;
    }
}
