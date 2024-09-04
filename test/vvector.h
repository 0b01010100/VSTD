// #ifndef __vvector__
// #define __vvector__
// //why not make this a littel like JS's array
// //ref at: https://www.w3schools.com/js/js_array_methods.asp
// #include <stdint.h>

// #ifdef __cplusplus
// extern "C" {
// #endif

// /**
//  * @brief Enum for specifying which field to query from a vvector.
//  */
// typedef enum VVECTOR_FIELD
// {
//     VVECTOR_FIELD_STRIDE          = 1,  /**< Size of each element in the stack */
//     VVECTOR_FIELD_LENGTH          = 2,  /**< Current number of elements in the stack */
//     VVECTOR_FIELD_CAPACITY        = 3,  /**< Maximum number of elements the stack can hold */
//     VVECTOR_FIELD_SCALE_PERCENT   = 4,   /**< Scale factor for dynamic resizing, in percentage */
//     VVECTOR1_FIELD_CTOR           = 5,   /**< Constructor function for each element */
//     VVECTOR1_FIELD_STOR           = 6,   /**< Sort function for each element */
//     VVECTOR1_FIELD_DTOR           = 7   /**< Destructor function for each element */
// } VVECTOR_FIELD;

// /**
//  * @brief vector handle
// */
// typedef unsigned char vvector;

// /**
//  * @brief vvector1 extends vvector, by allowing for custom memory management
//  * @note still will work on all the vector function
// */
// typedef unsigned char vvector1;

// /**
//  * @brief Represents a contructor and copy contructor function to be called for each element when the stack is destroyed,
// */
// typedef void* (*vvector1_el_ctor)(const void *_Src, size_t index_size);
// /**
//  * @brief Represents a sort function to be called for each element
// */
// typedef int (*vvector1_el_stor)(const void * a, const void * b, size_t index_size);
// /**
//  * @brief Represents a destructor function to be called for each element when the stack is destroyed,
// */
// typedef void (*vvector1_el_dtor)(void * self);

// /**
//  * @brief Creates a vvector with the specified element type, initial capacity, and scale factor.
//  *
//  * @param stride Size of each element in the stack.
//  * @param initial_capacity Initial number of elements the stack can hold.
//  * @param scale_factor Scale factor for dynamic resizing, in percentage.
//  * @return Pointer to the newly created vvector, or NULL if creation fails.
//  */
// #define vvector_create(T, initial_capacity, scale_factor) (vvector*)_vvector_create(sizeof(T), initial_capacity, scale_factor)

// /**
//  * @brief Creates a vvector with the specified element type, initial capacity, and scale factor.
//  *
//  * @param stride Size of each element in the stack.
//  * @param initial_capacity Initial number of elements the stack can hold.
//  * @param scale_factor Scale factor for dynamic resizing, in percentage.
//  * @param ctor Contructor function to be called for each element when the stack is destroyed, or NULL if not needed.
//  * @param dtor Destructor function to be called for each element when the stack is destroyed, or NULL if not needed.
//  * @return Pointer to the newly created vvector, or NULL if creation fails.
//  */
// #define vvector1_create(T, initial_capacity, scale_factor, ctor, stor, dtor) (vvector1*)_vvector1_create(sizeof(T), initial_capacity, scale_factor, ctor, stor, dtor)


// /**
//  * @brief Creates a vvector with the specified element stride, initial capacity, and scale factor.
//  *
//  * @param stride Size of each element in the stack.
//  * @param initial_capacity Initial number of elements the stack can hold.
//  * @param scale_factor Scale factor for dynamic resizing, in percentage.
//  * @return Pointer to the newly created vvector, or NULL if creation fails.
//  */
// vvector* _vvector_create(size_t stride, size_t initial_capacity, double scale_factor);

// /**
//  * @brief Creates a vvector with the specified element stride, initial capacity, and scale factor.
//  *
//  * @param stride Size of each element in the stack.
//  * @param initial_capacity Initial number of elements the stack can hold.
//  * @param scale_factor Scale factor for dynamic resizing, in percentage.
//  * @param ctor Constructor function to be called for each element when the vector element is created using push_ functions
//  * @param stor Sort function to be called for each element when sorting
//  * @param dtor Destructor function to be called for each element when the vector is destroyed, or NULL if not needed.
//  * @return Pointer to the newly created vvector, or NULL if creation fails.
//  */
// vvector* _vvector1_create(size_t stride, size_t initial_capacity, double scale_factor, vvector1_el_ctor ctor, vvector1_el_stor stor, vvector1_el_dtor dtor);

// /**
//  * @brief Retrieves a specific field's value from the vvector.
//  *
//  * @param vec Pointer to the vvector.
//  * @param field Field to retrieve, specified by the VVECTOR_FIELD enum.
//  * @return Value of the requested field.
//  */
// const void* vvector_get_field(vvector* vec, VVECTOR_FIELD field);

// /**
//  * @brief Retrieves a specific field's value from the vvector.
//  *
//  * @param vec Pointer to the vvector.
//  * @param field Field to retrieve, specified by the VVECTOR_FIELD enum.
//  * @note only VVECTOR_FIELD_SCALE_PERCENT && VVECTOR_FIELD_DTOR are set able
//  * @return Value of the requested field.
//  */
// void vvector_set_field(vvector* vec, VVECTOR_FIELD field, void* value);

// /**
//  * @brief Pushes a new item into the vvector back of the vector.
//  * @note uses a constructor, unlike emplace
//  * @param vec Pointer to the vvector.
//  * @param item Pointer to the item to push into the stack.
//  * @return 0 on success, or -1 on failure.
//  */
// int vvector_push_back(vvector* vec, const void* item);

// /**
//  * @brief Pushes a new item into the vvector front of the vector.
//  * @note uses a constructor, unlike emplace
//  * @param vec Pointer to the vvector.
//  * @param item Pointer to the item to push into the stack.
//  * @return 0 on success, or -1 on failure.
//  */
// int vvector_push_front(vvector* vec, const void* item);

// /**
//  * @brief Emplaces a new item into the vector.
//  * 
//  * @param vec Pointer to the vvector.
//  * @param item Pointer to the item to emplace back into the vector.
//  * @return 0 on success, or -1 on failure.
//  */
// int vvector_emplace_back(vvector* vec, const void* item);

// /**
//  * @brief Pushes a new item into the vvector.
//  *
//  * @param vec Pointer to the vvector.
//  * @param item Pointer to the item to emplace forward into the vector.
//  * @return 0 on success, or -1 on failure.
//  */
// int vvector_emplace_front(vvector* vec, const void* item);

// /**
//  * @brief Pops the item at the specified index from the vvector.
//  *
//  * @param vec Pointer to the vvector.
//  */
// void vvector_remove(vvector* vec, size_t index);

// /**
//  * @brief Pops the item at the specified index from the vvector.
//  *
//  * @param vec Pointer to the vvector.
//  * @param index Index of the vector.
//  * @return ptr to the index value, NULL if out of range
//  */
// const void* vvector_at(vvector* vec, size_t index);

// /**
//  * @brief Pops the item at the specified index from the vvector.
//  *
//  * @param vec Pointer to the vvector.
//  * @param index Index of the vector.
//  * @param value New value of the index.
//  */
// void vvector_set(vvector* vec, size_t index, void * value);

// /**
//  * @brief Pops the item at the specified index from the vvector.
//  *
//  * @param vec Pointer to the vvector.
//  */
// void vvector_sort(vvector* vec);

// // /**
// //  * @brief Pops the item at the specified index from the vvector.
// //  *
// //  * @param vec Pointer to the vvector.
// //  */
// // void vvector_splice(vvector* vec, size_t start, size_t* end, ...);

// /**
//  * @brief Destroys the vvector and frees associated memory.
//  *
//  * @param vec Pointer to a pointer to the vvector. The pointer will be set to NULL after destruction.
//  */
// vvector* vvector_concat(vvector* a, vvector* b, int del_ab);

// /**
//  * @brief Destroys the vvector and frees associated memory.
//  *
//  * @param vec Pointer to a pointer to the vvector. The pointer will be set to NULL after destruction.
//  */
// void vvector_destroy(vvector** vec);
// #ifdef __cplusplus
// }
// #endif

// #endif // __vvector__
