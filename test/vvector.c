// #include <vvector.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdarg.h>
// #define _vvector_type  (0)
// #define _vvector1_type (1)
// typedef struct _vvector
// {
//     size_t type;         // The type of the vector
//     size_t stride;       // Size of each item
//     size_t size;         // Number of items
//     size_t capacity;     // Current capacity
//     double scale;        // Resize scale factor
//     unsigned char* data; // Stack data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)
// } _vvector;

// typedef struct _vvector1 
// {
//     size_t type;         // The type of the vector
//     size_t stride;          // Size of each item
//     size_t size;            // Number of items
//     size_t capacity;        // Current capacity
//     double scale;           // Resize scale factor
//     unsigned char* data;    // Stack data(has to be an uchar* becuase cl(msvc) is wierd with pointer math)
//     vvector1_el_ctor ctor;  //Constructor ftn
//     vvector1_el_stor stor;  //Sort ftn   
//     vvector1_el_dtor dtor;  //Destructor
// } _vvector1;

// void* vvector_defctor(void* _Dst, const void *_Src, size_t index_size) {
//     if (_Dst != NULL) {
//         memcpy(_Dst, _Src, index_size);
//     }
//     return _Dst;
// }

// int vvector_defstor(const void * a, const void * b, size_t index_size) {

//     size_t a_ = *((size_t*)a);
//     size_t b_ = *((size_t*)b);
//     return ((a_) < (b_));
// }

// void vvector_defdtor(void *self) {
//     if(self) free(self);
// }

// vvector* _vvector_create(size_t stride, size_t initial_capacity, double scale_factor)
// {
//     _vvector * vec = (_vvector*)malloc(sizeof(_vvector));
//     if (!vec) return NULL;
//     vec->stride = stride;
//     vec->size = 0;
//     vec->capacity = initial_capacity > 0 ? initial_capacity : 1;
//     vec->scale = scale_factor > 1.0 ? scale_factor : 1.5;
//     vec->data = (unsigned char*)malloc(vec->capacity * vec->stride);
//     if (!vec->data)
//     {
//         free(vec);
//         return NULL;
//     }

//     return (vvector*)vec;
// }

// vvector* _vvector1_create(size_t stride, size_t initial_capacity, double scale_factor, vvector1_el_ctor ctor, vvector1_el_stor stor, vvector1_el_dtor dtor)
// {
//     _vvector1 * vec = (_vvector1*)malloc(sizeof(_vvector1));
//     if (!vec) return NULL;
        
//     memset(vec, 0, sizeof(_vvector1));
//     vec->stride = stride;
//     vec->size = 0;
//     vec->capacity = initial_capacity > 0 ? initial_capacity : 1;
//     vec->scale = scale_factor > 1.0 ? scale_factor : 1.5;
//     vec->data = (unsigned char*)malloc(vec->capacity * vec->stride);
//     if (!vec->data) {free(vec); return NULL;}
//     vec->ctor = (ctor) ? ctor : vvector_defctor;
//     vec->stor = (stor) ? stor : vvector_defstor;
//     vec->dtor = (dtor) ? dtor : vvector_defdtor;

//     return (vvector1*)vec;
// }

// const void* vvector_get_field(vvector* vec, VVECTOR_FIELD field)
// {
//     if (!vec) return 0;

//     _vvector1* vec_ = (_vvector1*)vec;
//     switch (field)
//     {
//         case VVECTOR_FIELD_STRIDE:        return &vec_->stride;
//         case VVECTOR_FIELD_LENGTH:        return &vec_->size;
//         case VVECTOR_FIELD_CAPACITY:      return &vec_->capacity;
//         case VVECTOR_FIELD_SCALE_PERCENT: return &vec_->scale;
//         case VVECTOR1_FIELD_CTOR:         return &vec_->dtor;
//         case VVECTOR1_FIELD_STOR:         return &vec_->stor;
//         case VVECTOR1_FIELD_DTOR:         return &vec_->dtor;
//         default: return 0;
//     }
// }

// void vvector_set_field(vvector* vec, VVECTOR_FIELD field, void* value)
// {
//     if (!vec) return;

//     switch (field)
//     {
//         case VVECTOR_FIELD_SCALE_PERCENT: ((_vvector*)vec)->scale = *((double*)value);
//         case VVECTOR1_FIELD_CTOR:         ((_vvector1*)vec)->ctor = value;
//         case VVECTOR1_FIELD_STOR:         ((_vvector1*)vec)->stor = value;
//         case VVECTOR1_FIELD_DTOR:         ((_vvector1*)vec)->dtor = value;
//         default: return;
//     }
// }

// int vvector_push(vvector* vec, const void* original)
// {
//     //by defualt this will do the same thing as vvector_emplace_back
//     _vvector* v = (_vvector*)vec;
//     void * new_item = (v->type == _vvector1_type)? ((_vvector1*)v)->ctor(original, v->stride) : vvector_defctor(original, v->stride);
//     if (!new_item) return -1;
    
//     if (v->size + 1 > v->capacity) {
//         size_t new_capacity = v->capacity * v->scale;
//         void *new_data = realloc(v->data, new_capacity);
//         if (!new_data) {
//             free(new_item);
//             return -1;
//         }
//         v->data = (unsigned char*)new_data;
//         v->capacity = new_capacity;
//     }

//     void* dest = (char*)v->data + v->size * v->element_size;
//     if (v->copy) {
//         v->copy(dest, element);
//     } else {
//         memcpy(dest, element, v->element_size);
//     }

//     v->size++;
//     return 0;
// }

// int vvector_push_front(vvector* vec, const void* item)
// {
//     //by defualt this will do the same thing as vvector_emplace_front
//     _vvector* v = (_vvector*)vec;
//     void * new_item = (v->type == _vvector1_type) ? 
//                       ((_vvector1*)v)->ctor(item, v->stride) : 
//                       vvector_defctor(item, v->stride);
    
//     if (!new_item) return -1;
    
//     if (v->size + 1 > v->capacity) {
//         size_t new_capacity = v->capacity * v->scale;
//         void *new_data = realloc(v->data, new_capacity * v->stride);
//         if (!new_data) {
//             free(new_item);
//             return -1;
//         }
//         v->data = (unsigned char*)new_data;
//         v->capacity = new_capacity;
//     }

//     memmove(v->data + v->stride, v->data, v->size * v->stride);

//     memcpy(v->data, new_item, v->stride);
//     free(new_item);

//     v->size++;
//     return 0;
// }

// int vvector_emplace_back(vvector* vec, const void* item)
// {
//     _vvector* v = (_vvector*)vec;
//     void * new_item = (v->type == _vvector1_type)? ((_vvector1*)v)->ctor(item, v->stride) : vvector_defctor(item, v->stride);
//     if (!new_item) return -1;
    
//     if (v->size + 1 > v->capacity) {
//         size_t new_capacity = v->capacity * v->scale;
//         void *new_data = realloc(v->data, new_capacity);
//         if (!new_data) {
//             free(new_item);
//             return -1;
//         }
//         v->data = (unsigned char*)new_data;
//         v->capacity = new_capacity;
//     }

//     memmove(v->data + (1 * v->stride), v->data, (v->size * v->stride));
//     memmove(v->data, new_item, 1 * v->stride);
//     free(new_item);

//     v->size++;
//     return 0;
// }

// int vvector_emplace_front(vvector* vec, const void* item)
// {
//     _vvector* v = (_vvector*)vec;
//     void * new_item = vvector_defctor(item, v->stride);
//     if (!new_item) return -1;
    
//     if (v->size + 1 > v->capacity) {
//         size_t new_capacity = v->capacity * v->scale;
//         void *new_data = realloc(v->data, new_capacity);
//         if (!new_data) {
//             free(new_item);
//             return -1;
//         }
//         v->data = (unsigned char*)new_data;
//         v->capacity = new_capacity;
//     }

//     memmove(v->data + (1 * v->stride), v->data, (v->size * v->stride));
//     memmove(v->data, new_item, 1 * v->stride);
//     free(new_item);

//     v->size++;
//     return 0;
// }

// void vvector_remove(vvector* vec, size_t index)
// {
//     _vvector* v = (_vvector*)vec;
//     if (index >= v->size) return;

//     if (v->type == _vvector1_type)
//         ((_vvector1*)v)->dtor(v->data + (v->stride * index));
//     else
//         vvector_defdtor(v->data + (v->stride * index));

//     // move the elements after the index to the left to fill the gap
//     if (v->size - 1 != index) {
//         memmove(v->data + index * v->stride, 
//                 v->data + (index + 1) * v->stride, 
//                 (v->size - index - 1) * v->stride);
//     }

//     // shrink the data array
//     v->size--;
//     v->data = realloc(v->data, v->size * v->stride);
// }

// const void* vvector_at(vvector* vec, size_t index)
// {
//     _vvector* v = (_vvector*)vec;
//     return (index >= v->size) ? NULL : (void*)(v->data + index * v->stride);
// }

// void vvector_set(vvector* vec, size_t index, void * value)
// {
//     _vvector* v = (_vvector*)vec;
//     if(index >= v->size) return;
//     memcpy((void*)(v->data + index * v->stride), value, v->stride);
// }

// void vvector_sort(vvector* vec)
// {
//     _vvector* v = (_vvector*)vec;
//     if (v->size <= 1) return; 

//     size_t count = v->size;
//     size_t byte_stride = v->stride;
//     unsigned char* arr = v->data;
//     vvector1_el_stor sorter = (v->type == _vvector1_type) ? ((_vvector1*)v)->stor : vvector_defstor;

//     for (size_t i = 0; i < count - 1; i++) { 
//         for (size_t j = i + 1; j < count; j++) { 
//             if (sorter(arr + i * byte_stride, arr + j * byte_stride, byte_stride)) { 
//                 unsigned char* temp = malloc(byte_stride);
//                 if (temp == NULL) {
                    
//                     return;
//                 }
                
//                 // swap the elements
//                 memcpy(temp, arr + i * byte_stride, byte_stride);
//                 memcpy(arr + i * byte_stride, arr + j * byte_stride, byte_stride);
//                 memcpy(arr + j * byte_stride, temp, byte_stride);

//                 free(temp);
//             } 
//         } 
//     } 
// }

// /*
// void vvector_splice(vvector* vec, size_t start, size_t end, ...)
// {
//     _vvector* v = (_vvector*)vec;
//     if (start >= v->size) return;

//     size_t to_del = end - start;



// }
// */
// void vvector_destroy_ex(_vvector** vec, char hard)
// {
//     //no checking here, beacuse it is an internal/private ftn
//     //if (!vec || !*vec) return;

//     _vvector* v = *vec;

//     if (hard)
//     {
//         if (v->type == _vvector1_type)
//         {
//             _vvector1* v1 = (_vvector1*)v;
//             for (size_t i = 0; i < v->size; i++)
//             {
//                 v1->dtor(v->data + i * v->stride);
//             }
//         }
//         else
//         {
//             for (size_t i = 0; i < v->size; i++)
//             {
//                 vvector_defdtor(v->data + i * v->stride);
//             }
//         }
//     }

//     free(v->data);
//     free(v);

//     *vec = NULL;
// }

// vvector* vvector_concat(vvector* a, vvector* b, int del)
// {
//     if(!a || !b) return NULL;
//     _vvector* v0 = (_vvector*)a;
//     _vvector* v1 = (_vvector*)b;

//     if(v0->stride != v1->stride) return NULL;

//     if(v1->capacity >= v0->capacity){
//         _vvector* temp = v0;
//         v0 = v1;
//         v1 = temp;
//     }
     
//     size_t new_size = v0->size + v1->size;
//     if (v0->capacity < new_size)
//     {
//         size_t new_capacity = new_size * v0->scale;
//         void* new_data = realloc(v0->data, new_capacity * v0->stride);
//         if (!new_data) return NULL;

//         v0->data = (unsigned char*)new_data;
//         v0->capacity = new_capacity;
//     }

//     memcpy(v0->data + v0->size * v0->stride, v1->data, v1->size * v0->stride);
//     v0->size = new_size;

//     #define soft 0x0
//     if (del & 0x02) vvector_destroy_ex(&v1, soft);

//     return a;
// }

// void vvector_destroy(vvector** vec)
// {
//     if (!vec || !*vec) return;
//     #define hard 0x0
//     vvector_destroy_ex((_vvector1*)vec, hard);
// }