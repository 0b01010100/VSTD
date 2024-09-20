#include <vchain.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*

    +------------------+
    |  User Data       |  // User-defined size (depends on stride var)
    +------------------+
    |  prev (pointer)  |  // sizeof(vlink*) bytes
    +------------------+
    |  next (pointer)  |  // sizeof(vlink*) bytes
    +------------------+

    will be in the same data block
    vlink ptrs will reference that block
    is unsigned char for to avoid compiling 
    errors when doing pointer arithmetic on 
    compilers like MSVS that don't like
    arithmetic with void ptrs
*/
typedef unsigned char vlink;

// creates a new link/node using the size of the data the user will be dealing with
#define _vlink_create(user_data_size) (vlink*)malloc(user_data_size + sizeof(vlink*) * 2)

// Gets the previous node (stored after 'next' pointer in the memory block)
#define get_prev(link, stride) (vlink*)(link + stride)

// Gets the next node (stored directly after user data)
#define get_next(link, stride) (vlink*)(link + stride + sizeof(vlink*))

// Sets the previous node (store the pointer to 'prev' in the correct memory location)
#define set_prev(link, prev_link, stride) (*(vlink**)((unsigned char*)(link) + (stride) + sizeof(vlink*))) = (prev_link)

// Sets the next node (store the pointer to 'next' in the correct memory location)
#define set_next(link, next_link, stride) (*(vlink**)((unsigned char*)(link) + (stride))) = (next_link)


typedef struct vchain
{
    size_t ver;             // Version of the list
    size_t stride;          // Size of each item
    size_t size;            // Number of items
    vlink* first;           // Pointer to the first link in the chain
    vlink* last;            // Pointer to the last link in the chain
} vchain;

typedef struct _vchain1
{
    size_t ver;
    size_t stride;          // Size of each item
    size_t size;            // Number of items
    vlink* first;           // Pointer to the first link in the chain
    vlink* last;            // Pointer to the last link in the chain

    // version 1.0 stuff

    vchain1_el_ctor  ctor;  // Element Constructor
    vchain1_el_cctor cctor; // Element Copy constructor
    vchain1_el_dtor  dtor;  // Element Destructor
} _vchain1;

/*@note memory is preallocated*/
int __def_vchain_ctor(void **this, size_t size, va_list* args, size_t count) {

    if (args && count > 0) {
        void *arg = va_arg(*args, void *);
        memcpy(*this, arg, size);
    } else {
        memset(*this, 0, size);
    }
    return 0;
}

/*@note memory is preallocated*/
int __def_vchain_cctor(void **this, const void *original, size_t size)
{
    memcpy(*this, original, size);
    return 0;
}

void __def_vchain_dtor(void **this, size_t size)
{
    if (*this) {
        free(*this);
        *this = NULL;
    }
}

vchain* _vchain_create(size_t stride, size_t initial_size)
{    
    vchain* ch = (vchain*)malloc(sizeof(vchain));
    if (!ch) return NULL;

    ch->ver = 0;
    ch->stride = stride;
    ch->size = 0;
    ch->first = NULL;
    ch->last = NULL;

    if (initial_size > 0) {
        vlink* cur = NULL;

        for (size_t i = 0; i < initial_size; ++i) {
            vlink* new_link = _vlink_create(stride);
            if (!new_link) {
                while (ch->first) {
                    vlink* temp = ch->first;
                    ch->first = *(vlink**)(temp + stride + sizeof(vlink*));
                    free(temp);
                }
                free(ch);
                return NULL;
            }

            __def_vchain_ctor((void**)new_link, stride, NULL, 0);
            if (!new_link) {
                free(new_link);
                while (ch->first) {
                    vlink* temp = ch->first;
                    ch->first = *(vlink**)(temp + stride + sizeof(vlink*));
                    free(temp);
                }
                free(ch);
                return NULL;
            }

            
            *(vlink**)(new_link + stride) = NULL;
            *(vlink**)(new_link + stride + sizeof(vlink*)) = cur;

            if (!ch->first) {
                ch->first = new_link; // first link
            } else {
                *(vlink**)(cur + stride) = new_link; 
            }
            cur = new_link;
        }

        ch->last = cur; 
    }

    return ch;
}



vchain1* _vchain1_create(size_t stride, size_t initial_size, vchain1_el_ctor ctor, vchain1_el_cctor cctor, vchain1_el_dtor dtor)
{    
    if(stride == 0) return NULL;
    _vchain1* ch = (_vchain1*)malloc(sizeof(_vchain1));
    if (!ch) return NULL;

    ch->ver = 0;
    ch->stride = stride;
    ch->size = 0;
    ch->first = NULL;
    ch->last = NULL;
    ch->ctor = (ctor)? ctor : __def_vchain_ctor;  
    ch->cctor = (cctor)? cctor : __def_vchain_cctor;  

    if (initial_size > 0) {
        vlink* cur = NULL;

        for (size_t i = 0; i < initial_size; ++i) {
            
            vlink* new_link = _vlink_create(stride);
            if (!new_link) {
                
                while (ch->first) {
                    vlink* temp = ch->first;
                    ch->first = get_next(temp, stride);
                    ch->dtor((void**)temp, stride);
                }
                free(ch);
                return NULL;
            }

            ch->ctor((void**)new_link, stride, NULL, 0);

            if (!new_link) {
                free(new_link);
                
                while (ch->first) {
                    vlink* temp = ch->first;
                    ch->first = *(vlink**)(temp + stride + sizeof(vlink*)); 
                    free(temp); 
                }
                free(ch);
                return NULL;
            }

            *(vlink**)(new_link + stride) = NULL; 
            *(vlink**)(new_link + stride + sizeof(vlink*)) = cur;

            if (!ch->first) {
                ch->first = new_link; // first link
            } else {
                *(vlink**)(cur + stride) = new_link;
            }
            cur = new_link;
        }

        ch->last = cur;
    }

    return (vchain*)ch;
}

size_t vchain_get_field(vchain* ch, VCHAIN_FIELD field)
{       
    if (!ch) return 0;
    switch (field)
    {
        case VCHAIN_FIELD_VER:          return ch->ver;
        case VCHAIN_FIELD_STRIDE:       return ch->stride;
        case VCHAIN_FIELD_LENGTH:       return ch->size;
        default:                        break;
    }

    if(ch->ver == VCHAIN_VER_0_0) return 0;

    // version 1.0 stuff
    switch (field)
    {
        case VCHAIN1_FIELD_CTOR:        return (size_t)((_vchain1*)ch)->ctor;
        case VCHAIN1_FIELD_CCTOR:       return (size_t)((_vchain1*)ch)->cctor;
        case VCHAIN1_FIELD_DTOR:        return (size_t)((_vchain1*)ch)->dtor;
        default:                        return 0;
    }
}

int vchain_to_ver(vchain* ch, size_t ver){
    if(!ch) return -1;
    if(ver == VCHAIN_VER_1_0){
        _vchain1 * new_data = (_vchain1 *)realloc(ch, sizeof(_vchain1));
        new_data->ctor = __def_vchain_ctor;
        new_data->cctor = __def_vchain_cctor;
        new_data->dtor = __def_vchain_dtor;
        new_data->ver = ver;
        if(!new_data) return -1;
    }else if(ver == VCHAIN_VER_0_0){
        vchain * new_data = (vchain*)realloc(ch, sizeof(vchain));
        new_data->ver = ver;
        if(!new_data) return -1;
    }else{
        return -1;
    }
    
    return 0; //Things went well
}

int _vchain_resize(vchain* ch, size_t new_size) 
{
    size_t current_size = ch->size;

    if (new_size == current_size) return 0;

    if (new_size > current_size) 
    {
        for (size_t i = 0; i < new_size - current_size; ++i) {
            if (vchain_push_back(ch, NULL) == -1) {
                return -1;
            }
        }
    }

    else if (new_size < current_size) 
    {

        vlink* temp = NULL; 
        vlink* cur = ch->last;
        vchain1_el_dtor __dtor = (ch->ver == VCHAIN_VER_0_0)? __def_vchain_dtor : ((_vchain1*)ch)->dtor;
        for (size_t i = 0; i < current_size - new_size; ++i) 
        {

            if (cur) {
                temp = get_prev(cur, ch->stride);

                __dtor((void**)cur, ch->stride);

                cur = temp;
            }

            ch->size--;
        }
    }

    return 0;
}

void vchain_set_field(vchain* ch, VCHAIN_FIELD field, void* value)
{
    if (!ch) return;

    switch (field)
    {
        case VCHAIN_FIELD_VER:           {vchain_to_ver(ch, *((size_t*)value)); return;}
        case VCHAIN_FIELD_LENGTH:        {_vchain_resize(ch, *((size_t*)value)); return;}
        default: break;
    }
    if(ch->ver == VCHAIN_VER_0_0) return;

    // version 1.0 stuff

    switch (field)
    {
        case VCHAIN1_FIELD_CTOR:        {((_vchain1*)ch)->ctor = value; return;}
        case VCHAIN1_FIELD_CCTOR:       {((_vchain1*)ch)->cctor = value; return;}
        case VCHAIN1_FIELD_DTOR:        {((_vchain1*)ch)->dtor = value; return;}
        default:                        return;
    }
}

int vchain_insert(vchain* ch, VCHAIN_SSIZE_T index, const void* original)
{
    if (!ch || !original) return -1;

    VCHAIN_SIZE_T nindex = (index < 0) ? (VCHAIN_SIZE_T)(ch->size + index) : (VCHAIN_SIZE_T)index;
    if (nindex > ch->size) return -1;

    if (_vchain_resize(ch, ch->size + 1) == -1) return -1;

    vchain1_el_cctor __cctor = (ch->ver == VCHAIN_VER_0_0)? __def_vchain_cctor : ((_vchain1*)ch)->cctor;

    vlink* cur = (nindex == 0) ? ch->first : (nindex < ch->size / 2 ? ch->first : ch->last);
    int step = (nindex < ch->size / 2) ? 1 : -1;

    for (size_t i = (nindex < ch->size / 2 ? 0 : ch->size - 1); 
         (step == 1) ? (i < nindex) : (i > nindex); 
         i += step
        )
    {
        cur = (step == 1) ? get_next(cur, ch->stride) : get_prev(cur, ch->stride);
    }

    if (__cctor((void**)cur, original, ch->stride) == -1) return -1;

    ch->size++;
    return 0;
}

int vchain_push_back(vchain* ch, const void* original)
{
    if (!ch || !original) return -1;

    if (_vchain_resize(ch, ch->size + 1) == -1) return -1;

    vchain1_el_cctor __cctor = (ch->ver == VCHAIN_VER_0_0)? __def_vchain_cctor : ((_vchain1*)ch)->cctor;

    if(__cctor((void**)ch->last, original, ch->stride) == -1) return -1;

    ch->size++;
    return 0;
}

int vchain_emplace(vchain* ch, VCHAIN_SSIZE_T index, size_t arg_count, ...)
{
    if(!ch || !arg_count) return -1;

    VCHAIN_SIZE_T nindex = (index < 0) ? (VCHAIN_SIZE_T)(ch->size + index) : (VCHAIN_SIZE_T)index;
    if (nindex > ch->size) return -1;

    if (_vchain_resize(ch, ch->size + 1) == -1) return -1;

    vchain1_el_ctor __ctor = (ch->ver == VCHAIN_VER_0_0)? __def_vchain_ctor : ((_vchain1*)ch)->ctor;

    vlink* cur = (nindex == 0) ? ch->first : (nindex < ch->size / 2 ? ch->first : ch->last);
    int step = (nindex < ch->size / 2) ? 1 : -1;

    for (size_t i = (nindex < ch->size / 2 ? 0 : ch->size - 1); 
         (step == 1) ? (i < nindex) : (i > nindex); 
         i += step
        )
    {
        cur = (step == 1) ? get_next(cur, ch->stride) : get_prev(cur, ch->stride);
    }

    va_list args;
    va_start(args, arg_count);
    
    if(__ctor((void**)cur, ch->stride, &args, arg_count) == -1) return -1;

    va_end(args);

    ch->size++;
    return 0;
}

int vchain_emplace_back(vchain* ch, size_t arg_count, ...)
{
    if(!ch || !arg_count) return -1;

    if (_vchain_resize(ch, ch->size + 1) == -1) return -1;

    vchain1_el_ctor __ctor = (ch->ver == VCHAIN_VER_0_0)? __def_vchain_ctor : ((_vchain1*)ch)->ctor;

    va_list args;
    va_start(args, arg_count);

    if(__ctor((void**)ch->last, ch->stride, &args, arg_count) == -1) return -1;

    va_end(args);

    ch->size++;
    return 0;
}

void * vchain_at(vchain* ch, VCHAIN_SSIZE_T index)
{
    if(!ch) return NULL;
    VCHAIN_SIZE_T nindex = (index < 0) ? (VCHAIN_SIZE_T)(ch->size + index) : (VCHAIN_SIZE_T)index;
    if (nindex >= ch->size) return NULL;

    vlink* cur = (nindex == 0) ? ch->first : (nindex < ch->size / 2 ? ch->first : ch->last);
    int step = (nindex < ch->size / 2) ? 1 : -1;

    for (size_t i = (nindex < ch->size / 2 ? 0 : ch->size - 1); 
         (step == 1) ? (i < nindex) : (i > nindex); 
         i += step
        )
    {
        cur = (step == 1) ? get_next(cur, ch->stride) : get_prev(cur, ch->stride);
    }

    return cur;
}

void vchain_pop_at(vchain* ch, VCHAIN_SSIZE_T index)
{
    if (!ch || ch->size == 0) return;

    VCHAIN_SIZE_T nindex = (index < 0) ? (VCHAIN_SIZE_T)(ch->size + index) : (VCHAIN_SIZE_T)index;
    if (nindex >= ch->size) return;

    vlink* cur = (nindex == 0) ? ch->first : (nindex < ch->size / 2 ? ch->first : ch->last);
    int step = (nindex < ch->size / 2) ? 1 : -1;

    for (size_t i = (nindex < ch->size / 2 ? 0 : ch->size - 1); 
         (step == 1) ? (i < nindex) : (i > nindex); 
         i += step
        )
    {
        cur = (step == 1) ? get_next(cur, ch->stride) : get_prev(cur, ch->stride);
    }

    vlink* nprev = get_prev(cur, ch->stride);
    vlink* nnext = get_next(cur, ch->stride);

    if (nprev) set_next(nprev, nnext, ch->stride);
    if (nnext) set_prev(nnext, nprev, ch->stride);

    if (cur == ch->first) ch->first = nnext;
    if (cur == ch->last) ch->last = nprev;

    free(cur);

    ch->size--;

}

void vchain_erase(vchain* ch, VCHAIN_SIZE_T first, VCHAIN_SIZE_T last)
{
    if (first >= last || !ch || ch->size == 0) return;

    vlink* start = vchain_at(ch, first);
    vlink* end = vchain_at(ch, last);
    
    if (!start || !end) return;

    vlink* start_prev = get_prev(start, ch->stride);
    vlink* end_next = get_next(end, ch->stride);

    if (start_prev) {
        set_next(start_prev, end_next, ch->stride);
    } else {
        ch->first = end_next;
    }

    if (end_next) {
        set_prev(end_next, start_prev, ch->stride);
    } else {
        ch->last = start_prev;
    }

    vlink* cur = start;
    while (cur != end_next) {
        vlink* next = get_next(cur, ch->stride);

        if (ch->ver == VCHAIN_VER_0_0) {
            __def_vchain_dtor((void**)cur, ch->stride);
        } else {
            ((_vchain1*)ch)->dtor((void**)cur, ch->stride);
        }

        free(cur);
        cur = next;
        ch->size--;
    }
}


void vchain_clear(vchain* ch)
{
    vchain_erase(ch, 0, ch->size - 1);
}

void vchain_swap(vchain* lhs, vchain* rhs)
{
    assert(lhs == rhs && "pointer are restricted from pointing to the same address");
    if(lhs->stride != rhs->stride) printf("vchain Warning: stride is not the same size for {lhs} and {rhs} in file: %s line: %d", __FILE__, __LINE__);
    //swaping things like meta data and the arrary data pointers
    if(lhs->ver == VCHAIN_VER_1_0 && rhs->ver == VCHAIN_VER_1_0){
        vchain1 temp = *lhs;
        *lhs = *rhs;
        *rhs = temp;
        return;
    }
    vchain temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

void vchain_destroy(vchain** ch)
{
    if (ch && *ch)
    {
        vchain_clear(*ch);
        free(*ch);
        *ch = NULL;
    }
}
