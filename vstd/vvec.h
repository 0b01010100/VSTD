#ifndef __vvec__
#define __vvec__
//@ref at: https://en.cppreference.com/w/cpp/container/vector
#include <stdint.h>
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#ifndef size_t
typedef SIZE_T size_t;
#endif
#endif
#include <stdarg.h> // <---- will be useful for the emplace functions

//Represents the front index of a vector
#define VVEC_FRONT (0)
//Represents the back index of a vector
#define VVEC_BACK (-1)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Version of the vvec type
*/
typedef enum VVEC_VER /* : int*/
{
    VVEC_VER_0_0 = 0x0,
    VVEC_VER_1_0 = 0x1
}VVEC_VER;

/**
 * @brief Enum for specifying which field to query from a vvec.
*/
typedef enum VVEC_FIELD /* : int*/
{

    VVEC_FIELD_VER             = 0,  /**< Version of the vvec structure */

    //vvec version 0.0

    VVEC_FIELD_STRIDE          = 1,  /**< Size of each element in the vector */
    VVEC_FIELD_LENGTH          = 2,  /**< Current number of elements in the vector */
    VVEC_FIELD_CAPACITY        = 3,  /**< Maximum number of elements the vector can hold */
    VVEC_FIELD_SCALE_PERCENT   = 4,  /**< Scale factor for dynamic resizing, in percentage */

    //vvec version 1.0

    VVEC1_FIELD_CTOR           = 5,  /**< Contructor function for each element */
    VVEC1_FIELD_CCTOR          = 6,  /**< Copy contructor function for each element */
    VVEC1_FIELD_DTOR           = 7,  /**< Destructor function for each element */
} VVEC_FIELD;

/**
 * @brief vector version 0.0 handle
*/
typedef struct vvec_t vvec_t;
/**
 * @brief vector version 1.0 handle
 * struct vvec1_t : vvec_t
*/
typedef vvec_t vvec1_t;

/**
 * @brief Represents a constructor function to be called for each element when the vector is destroyed,
*/
typedef int (*vvec1_ctor_t)(void * self, size_t size, va_list args, size_t count);

/**
 * @brief Represents a copy constructor function to be called for each element when the vector is destroyed,
*/
typedef int (*vvec1_cctor_t)(void * self, const void * original, size_t size);

/**
 * @brief Represents a destructor function to be called for each element when the vector is destroyed,
*/
typedef void (*vvec1_dtor_t)(void * self, size_t size);

/**
 * @brief Creates a vvec with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the vector.
 * @param initial_capacity Initial number of elements the vector can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vvec, or NULL if creation fails.
*/
vvec_t* _vvec_create(size_t stride, size_t initial_capacity, double scale_factor);

/**
 * @brief Creates a vvec with the specified element type, initial capacity, and scale factor.
 *
 * @param T Type of an element
 * @param initial_capacity Initial number of elements the vector can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vvec, or NULL if creation fails.
*/
#define vvec_create(T, initial_capacity, scale_factor) (vvec_t*)_vvec_create(sizeof(T), initial_capacity, scale_factor)

/**
 * @brief Creates a vvec with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the vector.
 * @param initial_capacity Initial number of elements the vector can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a vector is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a vector is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the vector is destroyed, or NULL if default.
 * @return Pointer to the newly created vvec, or NULL if creation fails.
*/
vvec1_t* _vvec1_create(size_t stride, size_t initial_capacity, double scale_factor, vvec1_ctor_t ctor, vvec1_cctor_t cctor, vvec1_dtor_t dtor);

/**
 * @brief Creates a vvec with the specified element stride, initial capacity, and scale factor.
 *
 * @param T type of an element
 * @param initial_capacity Initial number of elements the vector can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a vector is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a vector is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the vector is destroyed, or NULL if default.
 * @return Pointer to the newly created vvec, or NULL if creation fails.
*/
#define vvec1_create(T, initial_capacity, scale_factor, ctor, cctor, dtor) (vvec1_t*)_vvec1_create(sizeof(T), initial_capacity, scale_factor, ctor, cctor, dtor)

/**
 * @brief Retrieves a specific field's value from the vvec.
 *
 * @param vec Pointer to the vvec.
 * @param field Field to retrieve, specified by the VVEC_FIELD enum.
 * @return Value of the requested field.
*/
size_t vvec_get_field(vvec_t* vec, VVEC_FIELD field);

/**
 * @brief checks whether the container is empty.
 *
 * @param vec Pointer to the vvec.
 * @param field Field to retrieve, specified by the VVEC_FIELD enum.
 * @return 0 = FALSE, 1 = TRUE, -1 = ERROR
*/
#define vvec_empty(vec) (vvec_get_field(vec, VVEC_FIELD_LENGTH) == 0)

/**
 * @brief Retrieves a specific field's value from the vvec.
 *
 * @param vec Pointer to the vvec.
 * @param field Field to retrieve, specified by the VVEC_FIELD enum.
 * @param value New value of a field.
 * @note VVEC_FIELD_STRIDE is not setable.
 * @return Value of the requested field.
*/
void vvec_set_field(vvec_t* vec, VVEC_FIELD field, size_t value);

/**
 * @brief Changes the number of elements stored
 *
 * @param vec Pointer to the vvector.
 * @param new_size New size
*/
inline void vvec_resize(vvec_t* vec, size_t new_size)
    {vvec_set_field(vec, VVEC_FIELD_LENGTH, new_size);}

/**
 * @brief Increase the capacity of the vector
 *
 * @param vec Pointer to the vvector.
 * @param reserves Amount of extra capacity
*/
int vvec_reserve(vvec_t* vec, size_t reserves);

/**
 * @brief Direct access to the underlying contiguous storage
 *
 * @param vec Pointer to the vvec.
 * @return Pointer to the first element in the array
*/
#define vvec_data(vec) vvec_at(vec, VVEC_FRONT)

/**
 * @brief Access the first element
 *
 * @param vec Pointer to the vvec.
 * @return Pointer to the front element.
*/
#define vvec_front(vec) vvec_at(vec, VVEC_FRONT)

/**
 * @brief Access the last element
 *
 * @param vec Pointer to the vvec.
 * @return Pointer to the last element.
*/
#define vvec_back(vec) vvec_at(vec, VVEC_BACK)

/**
 * @brief Copies a object onto the vector.
 *
 * @param vec Pointer to the vvec.
 * @param index index value. Can be negative of positive
 * @param original Pointer to the original to copy and push onto the vector.
 * @return 0 on success, or -1 on failure.
*/
int vvec_insert(vvec_t* vec, ssize_t index, const void* original);

/**
 * @brief Copies a object onto the vector.
 *
 * @param vec Pointer to the vvec.
 * @param item Pointer to the item to push onto the vector.
 * @return 0 on success, or -1 on failure.
*/
int vvec_push_back(vvec_t* vec, const void* original);

/**
 * @brief Constructs a new item in place at the top of the vector.
 *
 * @param vec Pointer to the vvec.
 * @param index Index value. Can be negative of positive
 * @param arg_count  Count of arguments to pass in for constructing an element
 * @param va Arguments to pass in for constructing an element
 * @return 0 on success, or -1 on failure.
*/
int vvec_emplace(vvec_t* vec, ssize_t index, size_t arg_count, ...);

/**
 * @brief Constructs a new item in place at the top of the vector.
 *
 * @param vec Pointer to the vvec.
 * @param arg_count Count of arguments to pass in for constructing an element
 * @param va Arguments to pass in for constructing an element
 * @return 0 on success, or -1 on failure.
*/
int vvec_emplace_back(vvec_t* vec, size_t arg_count, ...);

/**
 * @brief Returns a pointer to the element at the specified index in the vvec.
 *
 * @param vec Pointer to the vector.
 * @return Pointer to the top element of the vector.
 */
void * vvec_at(vvec_t* vec, ssize_t index);

/**
 * @brief Pops the item at the specified index from the vvec.
 *
 * @param vec Pointer to the vvec.
 * @param index index value. Can be negative of positive
*/
void vvec_pop_at(vvec_t* vec, ssize_t index);

/**
 * @brief Pops the last item of the vector.
 *
 * @param vec Pointer to the vvec.
*/
#define pop_back(vec) vvec_pop_at(vec, vvec_BACK)

/**
 * @brief Removes the elements in the range [`first`,`last`).
 *
 * @param vec Pointer to the vvec.
 * @param first Iterator start
 * @param last Iterator end
*/
void vvec_erase(vvec_t* vec, ssize_t first, ssize_t last);

/**
 * @brief Erases all elements from the container.
 *
 * @param vec Pointer to the vvec.
*/
void vvec_clear(vvec_t* vec);

/**
 * @brief Exchanges the contents and capacity of the container with those of `rhs`.
 * Does not invoke any move, copy, or swap operations on individual elements.
 * @param lhs Pointer ONE vvec.
 * @param rhs Pointer ANOTHER vvec.
*/
void vvec_swap(vvec_t* lhs, vvec_t* rhs);

/**
 * @brief Requests the removal of unused capacity.
 *
 * @param vec Pointer to the vvec.
*/
void vvec_shrink_to_fit(vvec_t* vec);

/**
 * @brief Destroys the vvec and frees associated memory.
 *
*/
void vvec_destroy(vvec_t** vec);

/**
 * Macro to iterate over a vvec
 * @param T Type of the item to iterate over
 * @param item A variable of type T that will be assigned each element of the vector
 * @param vec The vvec to iterate over
 * @param action The action to perform on each iteration(if any) -> Optional
*/
#define vvec_foreach(T, item, index, vec, action) do { \
    size_t __len = vvec_get_field(vec, VVEC_FIELD_LENGTH); \
    for (size_t index = 0; index < __len; index++) { \
        T* item = (T*)vvec_at(vec, index); \
        action \
    }\
} while(0)

#ifdef __cplusplus
}
#endif

#endif // __vvec__
