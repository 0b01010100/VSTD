#include <vll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
    +------------------+
    |  next (pointer)  |  // sizeof(vl_t*) bytes
    +------------------+
    |  prev (pointer)  |  // sizeof(vl_t*) bytes
    +------------------+
    |  user Data       |  // User-defined size (depends on stride var)
    +------------------+
    will be in the same data block
    vl ptrs will reference that block
    is unsigned char for to avoid compiling 
    errors when doing pointer arithmetic on 
    compilers like MSVS that don't like
    arithmetic with void ptrs
*/
typedef struct vl_t
{
   struct vl_t* next;
   struct vl_t* prev;
   char data[]; //generic
} vl_t;

// Creates a new link/node using the size of the user data + space for 'prev' and 'next' pointers.
#define _vl_create(user_data_size) (vl_t*)malloc(sizeof(vl_t) + user_data_size)
// Destroys a link/node.
#define _vl_destroy(_mem) free(_mem)

typedef struct vll_t
{
    size_t ver;             // Version of the list
    size_t stride;          // Size of eall item
    size_t size;            // Number of items
    vl_t* first;           // Pointer to the first link in the ll
    vl_t* last;            // Pointer to the last link in the ll
} vll_t;

typedef struct _vll1_t
{
    size_t ver;
    size_t stride;          // Size of eall item
    size_t size;            // Number of items
    vl_t* first;           // Pointer to the first link in the ll
    vl_t* last;            // Pointer to the last link in the ll

    // version 1.0 stuff

    vll1_ctor_t  ctor;  // Element Constructor
    vll1_cctor_t cctor; // Element Copy constructor
    vll1_dtor_t  dtor;  // Element Destructor
} _vll1_t;

/*@note memory is preallocated*/
int __def_vll_ctor(void *self, size_t size, va_list args, size_t count) 
{
    assert(count <= 1 && "for def ctor args count can't be grater than 1");
    if (args && count == 1) {
        memcpy(self, args, size);
    } else {
        memset(self, 0, size);
    }
    return 0;
}

/*@note memory is preallocated*/
int __def_vll_cctor(void *self, const void *original, size_t size)
{
    if(!self) return -1;
    memcpy(self, original, size);
    return 0;
}

void __def_vll_dtor(void *self, size_t size)
{
    //WHOLE BLOCK ALREADY _vl_destroyDED
}

vll_t* _vll_create(size_t stride, size_t initial_size)
{    
    vll_t* ll = (vll_t*)malloc(sizeof(vll_t));
    if (!ll) return NULL;

    ll->ver = 0;
    ll->stride = stride;
    ll->size = initial_size;
    ll->first = NULL;
    ll->last = NULL;
    
    if (initial_size > 0) {
        vl_t* prev = NULL;

        for (size_t i = 0; i < initial_size; i++) 
        {
            vl_t* next = _vl_create(stride);
            if (!next || (__def_vll_ctor(&next, stride, NULL, 0) == -1)) {
                while (ll->first) {
                    vl_t* temp = ll->first;
                    ll->first = temp->next;
                    _vl_destroy(temp);
                }
                free(ll);
                return NULL;
            }

            if (!ll->first) {
                ll->first = next; // first link
            } else {
                prev->next = next;
                next->prev = prev;
            }
            prev = next;
        }

        ll->last = prev; 
    }

    return ll;
}

vll1_t* _vll1_create(size_t stride, size_t initial_size, vll1_ctor_t ctor, vll1_cctor_t cctor, vll1_dtor_t dtor)
{    
    if(stride == 0) return NULL;
    _vll1_t* ll = (_vll1_t*)malloc(sizeof(_vll1_t));
    if (!ll) return NULL;

    ll->ver = 1;
    ll->stride = stride;
    ll->size = 0;
    ll->first = NULL;
    ll->last = NULL;
    ll->ctor = (ctor)? ctor : __def_vll_ctor;  
    ll->cctor = (cctor)? cctor : __def_vll_cctor;  
    ll->dtor = (dtor)? dtor : __def_vll_dtor;

    if (initial_size > 0) {
        vl_t* prev = NULL;

        for (size_t i = 0; i < initial_size; i++) 
        {
            vl_t* next = _vl_create(stride);
            if (!next || (__def_vll_ctor(&next, stride, NULL, 0) == -1)) {
                while (ll->first) {
                    vl_t* temp = ll->first;
                    ll->first = temp->next;
                    _vl_destroy(temp);
                }
                free(ll);
                return NULL;
            }

            if (!ll->first) {
                ll->first = next; // first link
            } else {
                prev->next = next;
                next->prev = prev;
            }
            prev = next;
        }

        ll->last = prev; 
    }

    return (vll1_t*)ll;
}

size_t vll_get_field(vll_t* ll, VLL_FIELD field)
{       
    if (!ll) return 0;
    switch (field)
    {
        case VLL_FIELD_VER:          return ll->ver;
        case VLL_FIELD_STRIDE:       return ll->stride;
        case VLL_FIELD_LENGTH:       return ll->size;
        default:                     break;
    }

    if(ll->ver == VLL_VER_0_0) return 0;

    // version 1.0 stuff
    switch (field)
    {
        case VLL1_FIELD_CTOR:        return (size_t)((_vll1_t*)ll)->ctor;
        case VLL1_FIELD_CCTOR:       return (size_t)((_vll1_t*)ll)->cctor;
        case VLL1_FIELD_DTOR:        return (size_t)((_vll1_t*)ll)->dtor;
        default:                     return 0;
    }
}

// Internal

static int _vll_to_ver(vll_t* ll, size_t ver) /*no ptr check*/
{
    if(ver == VLL_VER_1_0){
        _vll1_t* new_data = (_vll1_t*)realloc(ll, sizeof(_vll1_t));
        new_data->ctor = __def_vll_ctor;
        new_data->cctor = __def_vll_cctor;
        new_data->dtor = __def_vll_dtor;
        new_data->ver = ver;
        if(!new_data) return -1;
    }else if(ver == VLL_VER_0_0){
        vll_t* new_data = (vll_t*)realloc(ll, sizeof(vll_t));
        new_data->ver = ver;
        if(!new_data) return -1;
    }else{
        return -1;
    }
    
    return 0; //Things went well
}

static vl_t * _vll_at(vll_t* ll, ssize_t index) /*no ptr check*/
{
    size_t nindex = (index < 0) ? (size_t)(ll->size + index) : (size_t)index;
    
    assert(nindex < ll->size && "index out-of-range");
    
    //Optimizes the traversal by starting from the end closest to the target index
    int step = (nindex <= ll->size >> 1) ? 1 : -1;
    vl_t* cur = (step == 1) ? ll->first : ll->last;

    size_t i = (step == 1) ? 0 : ll->size - 1;

    while (i != nindex) {
        cur = (step == 1) ? cur->next : cur->prev;
        i += step;
    }

    return cur;
}

static void _vll_erase(vll_t* ll, ssize_t start, ssize_t end) /*no ptr check*/
{
    if (start >= end || !ll || ll->size == 0 || start >= ll->size || end > ll->size){
        return;
    }
    size_t Ustart = (start < 0) ? (size_t)(ll->size + start) : (size_t)start;
    size_t Uend = (end < 0) ? (size_t)(ll->size + end) : (size_t)end;
    
    vl_t* first = _vll_at(ll, Ustart);
    vl_t* last = (Uend == ll->size) ? NULL : _vll_at(ll, Uend);

    vl_t* prev = (Ustart > 0) ? _vll_at(ll, Ustart - 1) : NULL;
    
    if (prev) {
        prev->next = last;
    } else {
        ll->first = last;
    }
    
    if (last) {
        last->prev = prev;
    } else {
        ll->last = prev;
    }

    vll1_dtor_t __dtor = (ll->ver == VLL_VER_0_0)? __def_vll_dtor : ((_vll1_t*)ll)->dtor;
    vl_t* current = first;
    while (current != last) {
        vl_t* next = current->next;
        __dtor(current->data, ll->stride);
        _vl_destroy(current);
        current = next;
    }

    ll->size -= (Uend - Ustart);

    if (ll->size == 0) {
        ll->first = NULL;
        ll->last = NULL;
    }
}

static int _vll_resize(vll_t* ll, size_t new_size) /*no ptr check*/
{
    size_t current_size = ll->size;

    if (new_size == current_size) return 0;

    if (new_size > current_size) 
    {
        vll1_ctor_t __cctor = (ll->ver == VLL_VER_0_0) ? __def_vll_ctor : ((_vll1_t*)ll)->ctor;
        
        for (size_t i = current_size; i < new_size; ++i) 
        {
            vl_t* vl = _vl_create(ll->stride);
            if (!vl)
            {
                // Rollback: remove the nodes we've added
                _vll_erase(ll, current_size, ll->size);
                return -2;
            }
            vl->next = NULL;
            if(ll->size == 0){
                ll->first = vl;
                vl->prev = NULL;
            }else{ 
                vl->prev = ll->last;
                ll->last->next = vl;
            }
            ll->last = vl;
            __cctor(vl->data, ll->stride, NULL, 1);
            ll->size++;
        }
    } 
    else  // new_size < current_size
    {
        _vll_erase(ll, new_size, current_size);
    }

    return 0;
}

// Public

void vll_set_field(vll_t* ll, VLL_FIELD field, size_t value)
{
    if (!ll) return;

    switch (field)
    {
        case VLL_FIELD_VER:           {_vll_to_ver(ll, value); return;}
        case VLL_FIELD_LENGTH:        {_vll_resize(ll, value); return;}
        default: break;
    }
    if(ll->ver == VLL_VER_0_0) return;

    // version 1.0 stuff

    switch (field)
    {
        case VLL1_FIELD_CTOR:        {((_vll1_t*)ll)->ctor = (vll1_ctor_t)value; return;}
        case VLL1_FIELD_CCTOR:       {((_vll1_t*)ll)->cctor = (vll1_cctor_t)value; return;}
        case VLL1_FIELD_DTOR:        {((_vll1_t*)ll)->dtor = (vll1_dtor_t)value; return;}
        default:                        return;
    }
}

int vll_push_back(vll_t* ll, const void* original)
{
    if (!ll || !original) return -1;

    vl_t* vl = _vl_create(ll->stride);
    if(!vl) return -1;
    vl->next = NULL;
    if(ll->size == 0){
        ll->first = (ll->last = vl);
        vl->prev = NULL;
    }else{ 
        /*const*/ vl_t* last = ll->last;
        last->next = vl;
        vl->prev = last;
        ll->last = vl;
    }

    vll1_cctor_t __cctor = (ll->ver == VLL_VER_0_0)? __def_vll_cctor : ((_vll1_t*)ll)->cctor;

    if(__cctor(vl->data, original, ll->stride) == -1) return -1;

    ll->size++;
    return 0;
}

int vll_emplace_back(vll_t* ll, size_t arg_count, ...)
{
    if(!ll || !arg_count) return -1;

    vl_t* vl = _vl_create(ll->stride);
    if(!vl) return -1;
    vl->next = NULL;
    if(ll->size == 0){
        ll->first = (ll->last = vl);
        vl->prev = NULL;
    }else{ 
        /*const*/ vl_t* last = ll->last;
        last->next = vl;
        vl->prev = last;
        ll->last = vl;
    }

    vll1_ctor_t __ctor = (ll->ver == VLL_VER_0_0)? __def_vll_ctor : ((_vll1_t*)ll)->ctor;

    va_list args;
    va_start(args, arg_count);

    if(__ctor(vl->data, ll->stride, args, arg_count) == -1) return -1;

    va_end(args);

    ll->size++;
    return 0;
}

int vll_insert(vll_t* ll, ssize_t index, const void* original)
{
    if (!ll || !original) return -1;

    size_t nindex = (index < 0) ? (size_t)(ll->size + index) : (size_t)index;
    assert(!(nindex > ll->size) && "index out-of range");

    vl_t* vl = _vl_create(ll->stride);
    if(!vl) return -1;

    vl_t* l = _vll_at(ll, index);
    vl->next = NULL;
    if(ll->size == 0){
        ll->first = (l = vl);
        vl->prev = NULL;
    }else{ 

        if(index > 0)
        {
            vl->next = l;
            vl->prev = l->prev;
            l->prev->next = vl;
            l->prev = vl;
        }else{
            vl->next = ll->first;
            vl->prev = NULL;
            ll->first->prev = vl;
            ll->first = vl;
        }

    }    

    vll1_cctor_t __cctor = (ll->ver == VLL_VER_0_0)? __def_vll_cctor : ((_vll1_t*)ll)->cctor;
    if (__cctor(vl->data, original, ll->stride) == -1) return -1;
    ll->size++;
    return 0;
}

int vll_emplace(vll_t* ll, ssize_t index, size_t arg_count, ...)
{
    if(!ll || !arg_count) return -1;

    size_t nindex = (index < 0) ? (size_t)(ll->size + index) : (size_t)index;
    assert(!(nindex > ll->size) && "index out-of range");

    vl_t* vl = _vl_create(ll->stride);
    if(!vl) return -1;
    vl_t* l = _vll_at(ll, index);
    vl->next = NULL;
    if(ll->size == 0){
        ll->first = (l = vl);
        vl->prev = NULL;
    }else{ 

        if(index > 0)
        {
            vl->next = l;
            vl->prev = l->prev;
            l->prev->next = vl;
            l->prev = vl;
        }else{
            vl->next = ll->first;
            vl->prev = NULL;
            ll->first->prev = vl;
            ll->first = vl;
        }

    }   

    vll1_ctor_t __ctor = (ll->ver == VLL_VER_0_0)? __def_vll_ctor : ((_vll1_t*)ll)->ctor;
    
    va_list args;
    va_start(args, arg_count);
    
    if(__ctor(vl->data, ll->stride, args, arg_count) == -1) return -1;

    va_end(args);

    ll->size++;
    return 0;
}

void *vll_at(vll_t* ll, ssize_t index) {
    if (!ll || ll->size == 0) return NULL;
    return _vll_at(ll, index)->data;
}

void vll_pop_at(vll_t* ll, ssize_t index)
{
    if (!ll) return;
    
    vl_t* l = _vll_at(ll, index);
    
    if (l == ll->first) {
        ll->first = l->next;
        if (ll->first) ll->first->prev = NULL;
        else ll->last = NULL;
    } else if (l == ll->last) {
        ll->last = l->prev;
        ll->last->next = NULL;
    } else {
        l->prev->next = l->next;
        l->next->prev = l->prev;
    }
    
    vll1_dtor_t __dtor = (ll->ver == VLL_VER_0_0) ? __def_vll_dtor : ((_vll1_t*)ll)->dtor;
    __dtor(l->data, ll->stride);
    _vl_destroy(l);
    ll->size--;
}

void vll_erase(vll_t* ll, ssize_t start, ssize_t end)
{
    _vll_erase(ll, start, end);
}

void vll_clear(vll_t* ll)
{
    vll_erase(ll, 0, ll->size);
}

void vll_swap(vll_t* lhs, vll_t* rhs)
{
    assert(lhs == rhs && "pointer are restricted from pointing to the same address");
    if(lhs->stride != rhs->stride) printf("vll Warning: stride is not the same size for {lhs} and {rhs} in file: %s line: %d", __FILE__, __LINE__);
    //swaping things like meta data and the arrary data pointers
    if(lhs->ver == VLL_VER_1_0 && rhs->ver == VLL_VER_1_0){
        vll1_t temp = *lhs;
        *lhs = *rhs;
        *rhs = temp;
        return;
    }
    vll_t temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

void vll_reverse(vll_t* ll)
{
    if (ll->size == 0 || ll->first == NULL) {
        return;
    }

    vl_t* prev = NULL;
    vl_t* current = ll->first;
    vl_t* next = NULL;
    
    ll->last = ll->first;
    
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        current->prev = next;
        
        prev = current;
        current = next;
    }
    
    ll->first = prev;
    ll->first->prev = NULL;
    ll->last->next = NULL;
}

void vll_destroy(vll_t** ll)
{
    if (ll && *ll)
    {
        vll_clear(*ll);
        free(*ll);
        *ll = NULL;
    }
}