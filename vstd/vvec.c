#include <vvec.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct vvec_t
{
    size_t ver;
    size_t stride;       // Size of each item
    size_t size;         // Number of items
    size_t capacity;     // Current capacity
    double scale;        // Resize scale factor
    char* data; // vector data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)
} vvec_t;

typedef struct _vvec1_t
{
    size_t ver;
    size_t stride;          // Size of each item
    size_t size;            // Number of items
    size_t capacity;        // Current capacity
    double scale;           // Resize scale factor
    char* data;    // vector data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)

    // version 1.0 stuff

    vvec1_ctor_t  ctor;  // Element Constructor
    vvec1_cctor_t cctor; // Element Copy constructor
    vvec1_dtor_t  dtor;  // Element Destructor
} _vvec1_t;

int __def_vvec_ctor(void * self, size_t size, va_list args, size_t count) { 
    if(!count) memcpy(self, 0, size);
    void *arg = va_arg(args, void *);
    memcpy(self, arg, size);
    return 0;
}

int __def_vvec_cctor(void * self, const void * original, size_t size)
{
    memcpy(self, original, size);
    return 0;
}

void __def_vvec_dtor(void * self, size_t size)
{
    if (self) {
        memset(self, '\0', size);
    }
}

vvec_t* _vvec_create(size_t stride, size_t initial_capacity, double scale_factor)
{    
    vvec_t* vec = (vvec_t*)malloc(sizeof(vvec_t));
    if (!vec) return NULL;

    vec->ver = VVEC_VER_0_0;
    vec->stride = stride;
    vec->size = 0;
    vec->capacity = initial_capacity;
    vec->scale = (scale_factor)? scale_factor : 0.5;
    vec->data = (unsigned char*)calloc(initial_capacity, stride);
    if (!vec->data) {
        free(vec);
        return NULL;
    }

    return vec;
}

vvec1_t* _vvec1_create(size_t stride, size_t initial_capacity, double scale_factor, vvec1_ctor_t ctor, vvec1_cctor_t cctor, vvec1_dtor_t dtor)
{    
    _vvec1_t* vec = (_vvec1_t*)malloc(sizeof(_vvec1_t));
    if (!vec) return NULL;

    vec->ver = VVEC_VER_1_0;
    vec->stride = stride;
    vec->size = 0;
    vec->capacity = initial_capacity;
    vec->scale = (scale_factor)? scale_factor : 0.5;
    vec->data = (unsigned char*)malloc(initial_capacity * stride);

    // version 1.0 stuff

    vec->ctor = (ctor)? ctor : __def_vvec_ctor;
    vec->cctor = (cctor)? cctor : __def_vvec_cctor;
    vec->dtor = (dtor)? dtor : __def_vvec_dtor;
    if (!vec->data) {
        free(vec);
        return NULL;
    }

    return (vvec1_t*)vec;
}

size_t vvec_get_field(vvec_t* vec, VVEC_FIELD field)
{
    if (!vec) return 0;
    switch (field)
    {
        case VVEC_FIELD_VER:          return vec->ver;
        case VVEC_FIELD_STRIDE:       return vec->stride;
        case VVEC_FIELD_LENGTH:       return vec->size;
        case VVEC_FIELD_CAPACITY:     return vec->capacity;
        case VVEC_FIELD_SCALE_PERCENT:return vec->scale;
        default:                      break;
    }

    if(vec->ver == VVEC_VER_0_0) return 0;

    // version 1.0 stuff
    switch (field)
    {
        case VVEC1_FIELD_CTOR:        return (size_t)((_vvec1_t*)vec)->ctor;
        case VVEC1_FIELD_CCTOR:       return (size_t)((_vvec1_t*)vec)->cctor;
        case VVEC1_FIELD_DTOR:        return (size_t)((_vvec1_t*)vec)->dtor;
        default:                      return 0;
    }
}

static int _vvec_to_ver(vvec_t* vec, size_t ver){
    if(ver == VVEC_VER_1_0){
        _vvec1_t* new_data = (_vvec1_t*)realloc(vec, sizeof(_vvec1_t));
        new_data->ctor = __def_vvec_ctor;
        new_data->cctor = __def_vvec_cctor;
        new_data->dtor = __def_vvec_dtor;
        if(!new_data) return -1;
    }else{//VVEC_VER_0_0
        vvec_t* new_data = (vvec_t*)realloc(vec, sizeof(vvec_t));
        if(!new_data) return -1;
    }
    return 0;
}

static void* _vvec_at(vvec_t* vec, ssize_t index)
{
    size_t nindex = (index < 0) ? (size_t)(vec->size + index) : (size_t)index;
    assert(nindex < vec->size && "index out-of-range");
    return &vec->data[vec->stride * nindex];
}

static void _vvec_pop_at(vvec_t* vec, ssize_t index)
{
    size_t nindex = (index < 0) ? (size_t)(vec->size + index) : (size_t)index;
    assert(nindex < vec->size && "index out-of-range");

    if (vec->ver == VVEC_VER_0_0)
    { 
        __def_vvec_dtor(vec->data + (nindex * vec->stride), vec->stride);
    } 
    else 
    {
        ((_vvec1_t*)vec)->dtor(vec->data + (nindex * vec->stride), vec->stride);
    }

    if (nindex < vec->size - 1) 
    {
        memmove(vec->data + (nindex * vec->stride),
                vec->data + ((nindex + 1) * vec->stride),
               (vec->size - nindex - 1) * vec->stride);
    }

    vec->size--;
    vec->capacity++;
}

static void _vvec_erase(vvec_t* vec, ssize_t start, ssize_t end)
{
    size_t Ustart = (start < 0) ? (size_t)(vec->size + start) : (size_t)start;
    size_t Uend = (end < 0) ? (size_t)(vec->size + end) : (size_t)end;
    assert(Ustart < Uend && "index out-of-range");
    assert(Uend <= vec->size && "index out-of-range");

    for (size_t i = Ustart; i < Uend; i++)
    {
        if (vec->ver == VVEC_VER_0_0) 
        {
            __def_vvec_dtor(vec->data + (i * vec->stride), vec->stride);
        } else {
            ((_vvec1_t*)vec)->dtor(vec->data + (i * vec->stride), vec->stride);
        }
    }
    if (Uend < vec->size) {
        memmove(vec->data + (Ustart * vec->stride), 
                vec->data + (Uend * vec->stride), 
                (vec->size - Uend) * vec->stride);
    }
    vec->size -= (Uend - Ustart);
}

static int _vvec_resize(vvec_t* vec, size_t new_size, int is_capacity) 
{
    if (is_capacity)
    {
        if (new_size < vec->size)
        {
            vvec1_dtor_t dtor = (vec->ver == VVEC_VER_1_0) ? ((_vvec1_t*)vec)->dtor : __def_vvec_dtor;
            for (size_t i = new_size; i < vec->size; i++)
            {
                dtor(vec->data + (i * vec->stride), vec->stride);
            }
            vec->size = new_size;
            
            void* new_data = realloc(vec->data, new_size * vec->stride);
            if (!new_data) return -1;
            vec->data = new_data;
            vec->capacity = new_size;
        }
        else if (new_size > vec->capacity)
        {
            void* new_data = realloc(vec->data, new_size * vec->stride);
            if (!new_data) return -1;
            vec->data = new_data;
            
            vec->capacity = new_size;
        }
    }
    else
    {
        if (new_size > vec->size) 
        {   
            if (new_size > vec->capacity) {
                if (_vvec_resize(vec, new_size, 1) == -1) return -1;
            }
            
            vvec1_ctor_t ctor = (vec->ver == VVEC_VER_1_0) ? ((_vvec1_t*)vec)->ctor : __def_vvec_ctor;
            for (size_t i = vec->size; i < new_size; i++) {
                ctor(vec->data + (i * vec->stride), vec->stride, NULL, 0);
            }
            
            vec->size = new_size;
        } 
        else if (new_size < vec->size)
        {
            _vvec_erase(vec, new_size, vec->size);

            if (vec->capacity > new_size * 2) {
                size_t new_capacity = new_size * 1.5;
                if (_vvec_resize(vec, new_capacity, 1) == -1) return -1;
            }
        }
    }

    return 0;
}

int vvec_scale(vvec_t* vec)
{
    if (!vec) return -1;

    size_t new_capacity = vec->capacity ? vec->capacity * (1 + (size_t)vec->scale) : 1;

    unsigned char* new_data = (unsigned char*)realloc(vec->data, new_capacity * vec->stride);
    if (!new_data) return -1; 

    vec->data = new_data;
    vec->capacity = new_capacity;

    return 0;
}

void vvec_set_field(vvec_t* vec, VVEC_FIELD field, size_t value)
{
    if (!vec) return;

    switch (field)
    {
        case VVEC_FIELD_VER:           {_vvec_to_ver(vec, value); return;}
        case VVEC_FIELD_CAPACITY:      {_vvec_resize(vec, value, 1/*yes*/); return;}
        case VVEC_FIELD_LENGTH:        {_vvec_resize(vec, value, 0/*no*/); return;}
        case VVEC_FIELD_SCALE_PERCENT: {vec->scale = ((double)value); return;};
        default: break;
    }

    if(vec->ver == VVEC_VER_0_0) return;

    // version 1.0 stuff

    switch (field)
    {
        case VVEC1_FIELD_CTOR:        {((_vvec1_t*)vec)->ctor; return;}
        case VVEC1_FIELD_CCTOR:       {((_vvec1_t*)vec)->cctor; return;}
        case VVEC1_FIELD_DTOR:        {((_vvec1_t*)vec)->dtor; return;}
        default:                        return;
    }
}

int vvec_reserve(vvec_t* vec, size_t count)
{
    return _vvec_resize(vec, vec->capacity + count, 1/*yes*/);
}

int vvec_push_back(vvec_t* vec, const void* original)
{
    if (!vec || !original) return -1;

    if (vec->size >= vec->capacity)
    {
        if (vvec_scale(vec) == -1) return -1;
    }

    int ret = 0;
    if (vec->ver == VVEC_VER_0_0){ 
        ret = __def_vvec_cctor(vec->data + (vec->size * vec->stride), original, vec->stride);
    } 
    else 
    {
        ret = ((_vvec1_t*)vec)->cctor(vec->data + (vec->size * vec->stride), original, vec->stride);
    }

    if(ret != 0) 
    {
        return ret; // -1 or -2
    }

    vec->size++;
    
    return 0;
}

int vvec_emplace_back(vvec_t* vec, size_t arg_count, ...)
{
    if(!vec || !arg_count) return -1;

    if (vec->size >= vec->capacity)
    {
        if (vvec_scale(vec) == -1) return -1;
    }

    size_t el_size = vec->stride;
    va_list args;
    va_start(args, arg_count);

    int ret = 0;
    if (vec->ver == VVEC_VER_0_0){ 
        ret = __def_vvec_ctor(vec->data + (vec->size * vec->stride), vec->stride, args, arg_count);
    }else{
        ret = ((_vvec1_t*)vec)->ctor(vec->data + (vec->size * vec->stride), vec->stride, args, arg_count);
    } 
    
    va_end(args);

    if(ret != 0) 
    {
        return ret; // -1 or -2
    }

    vec->size++;
    return 0;
}

int vvec_insert(vvec_t* vec, ssize_t index, const void* original)
{
    if (!vec || !original) return -1;

    size_t nindex = (index < 0) ? vec->size + index : (size_t)index;
    assert(nindex < vec->size && "index out-of-range");

    if (vec->size >= vec->capacity)
    {
        if (vvec_scale(vec) == -1) return -1;
    }

    memmove(vec->data + (nindex + 1) * vec->stride, 
            vec->data + nindex * vec->stride, 
            (vec->size - nindex) * vec->stride);

    int ret = 0;
    if (vec->ver == VVEC_VER_0_0){
        ret = __def_vvec_cctor(vec->data + (nindex * vec->stride), original, vec->stride);
    }else{
        ret = ((_vvec1_t*)vec)->cctor(vec->data + (nindex * vec->stride), original, vec->stride);
    }

    if (ret != 0) {
        memmove(vec->data + nindex * vec->stride, 
                vec->data + (nindex + 1) * vec->stride, 
                (vec->size - nindex) * vec->stride);
        return -1;
    }

    vec->size++;
    
    return 0;
}

int vvec_emplace(vvec_t* vec, ssize_t index, size_t arg_count, ...)
{
    if(!vec || !arg_count) return -1;

    size_t nindex = (index < 0) ? (size_t)(vec->size + index) : (size_t)index;
    assert(nindex < vec->size && "index out-of-range");

    if (vec->size >= vec->capacity)
    {
        if (vvec_scale(vec) == -1) return -1;
    }

    size_t el_size = vec->stride;
    va_list args;
    va_start(args, arg_count);
    

    memmove(vec->data + (nindex + 1) * vec->stride, 
            vec->data + nindex * vec->stride, 
            (vec->size - nindex) * vec->stride);
    

    int ret = 0;
    if (vec->ver == VVEC_VER_0_0){ 
        ret = __def_vvec_ctor(vec->data + (nindex * vec->stride), vec->stride, args, arg_count);
    }else{
        ret = ((_vvec1_t*)vec)->ctor((vec->data + (nindex * vec->stride)), vec->stride, args, arg_count);
    } 

    va_end(args);

    if (ret != 0) {
        memmove(vec->data + nindex * vec->stride, 
                vec->data + (nindex + 1) * vec->stride, 
                (vec->size - nindex) * vec->stride);
        return -1;
    }

    vec->size++;
    return 0;
}

void * vvec_at(vvec_t* vec, ssize_t index)
{
    if (!vec) return NULL;
    size_t nindex = (index < 0) ? (size_t)(vec->size + index) : (size_t)index;
    return _vvec_at(vec, index);
}

void vvec_pop_at(vvec_t* vec, ssize_t index)
{
    if (!vec) return;
    _vvec_pop_at(vec, index);
}

void vvec_erase(vvec_t* vec, ssize_t start, ssize_t end)
{
    if (!vec) return;
    _vvec_erase(vec, start, end);
}

void vvec_clear(vvec_t* vec)
{
    if (!vec) return;
    _vvec_erase(vec, 0, vec->size);
}

void vvec_swap(vvec_t* lhs, vvec_t* rhs)
{
    assert(lhs == rhs && "pointer are restricted from pointing to the same address");
    if(lhs->stride != rhs->stride) printf("vvec Warning: stride is not the same size for {lhs} and {rhs} in file: %s line: %d", __FILE__, __LINE__);
    //swaping things like meta data and the arrary data pointers
    if(lhs->ver == VVEC_VER_1_0 && rhs->ver == VVEC_VER_1_0){
        _vvec1_t temp = *(_vvec1_t*)lhs;
        *lhs = *rhs;

        vvec_t * t = (vvec_t*)&temp;
        *rhs = *t;
        return;
    }
    vvec_t temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

void vvec_shrink_to_fit(vvec_t* vec)
{
    if(!vec) return;
    if(vec->capacity == vec->size) return;
    //else
    vec->capacity = vec->size;
    void * new_data = realloc(vec->data, vec->capacity);
}

void vvec_destroy(vvec_t** vec)
{
    if (vec && *vec)
    {    
        _vvec_erase(*vec, 0, (*vec)->size);
        free(*vec);
        *vec = NULL;
    }
}