#ifndef __vvector__
#define __vvector__
//@ref at: https://en.cppreference.com/w/cpp/container/vector
#include <stdint.h>
//same as size_t
typedef uint64_t VVECTOR_SIZE_T;
//same as ssize_t
typedef int64_t VVECTOR_SSIZE_T;
#include <stdarg.h> // <---- will be useful for the emplace functions


#if defined(_MSC_VER)
    /*Making sure there is no overlapping memory regions.*/
    #define VVECTOR_FTN_RESTRICT __restrict
#elif defined(__GNUC__) || defined(__clang__)
    /*Making sure there is no overlapping memory regions.*/
    #define VVECTOR_FTN_RESTRICT __restrict__
#else
    /*Making sure there is no overlapping memory regions.*/
    #define VVECTOR_FTN_RESTRICT restrict
#endif

//Represents the front index of a vector
#define VVECTOR_FRONT (0)
//Represents the back index of a vector
#define VVECTOR_BACK (-1)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Version of the vvector type
*/
typedef enum VVECTOR_VER /* : int*/
{
    VVECTOR_VER_0_0 = 0x0,
    VVECTOR_VER_1_0 = 0x1
}VVECTOR_VER;

/**
 * @brief Enum for specifying which field to query from a vvector.
*/
typedef enum VVECTOR_FIELD /* : int*/
{

    VVECTOR_FIELD_VER             = 0,  /**< version of the vvector structure */

    //vvector version 0.0

    VVECTOR_FIELD_STRIDE          = 1,  /**< Size of each element in the vector */
    VVECTOR_FIELD_LENGTH          = 2,  /**< Current number of elements in the vector */
    VVECTOR_FIELD_CAPACITY        = 3,  /**< Maximum number of elements the vector can hold */
    VVECTOR_FIELD_SCALE_PERCENT   = 4,  /**< Scale factor for dynamic resizing, in percentage */

    //vvector version 1.0

    VVECTOR1_FIELD_CTOR           = 5,  /**< Contructor function for each element */
    VVECTOR1_FIELD_CCTOR          = 6,  /**< Copy contructor function for each element */
    VVECTOR1_FIELD_DTOR           = 7,  /**< Destructor function for each element */
} VVECTOR_FIELD;

/**
 * @brief vector version 0.0 handle
*/
typedef struct vvector vvector;
/**
 * @brief vector version 1.0 handle
*/
typedef vvector vvector1;

/**
 * @brief Represents a constructor function to be called for each element when the vector is destroyed,
*/
typedef int (*vvector1_el_ctor)(void *location, size_t size, va_list* args, size_t count);

/**
 * @brief Represents a copy constructor function to be called for each element when the vector is destroyed,
*/
typedef int (*vvector1_el_cctor)(void * location, const void * original, size_t size);

/**
 * @brief Represents a destructor function to be called for each element when the vector is destroyed,
*/
typedef void (*vvector1_el_dtor)(void  * location, size_t size);

/**
 * @brief Creates a vvector with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the vector.
 * @param initial_capacity Initial number of elements the vector can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vvector, or NULL if creation fails.
*/
vvector* _vvector_create(size_t stride, size_t initial_capacity, double scale_factor);

/**
 * @brief Creates a vvector with the specified element type, initial capacity, and scale factor.
 *
 * @param T Type of an element
 * @param initial_capacity Initial number of elements the vector can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vvector, or NULL if creation fails.
*/
#define vvector_create(T, initial_capacity, scale_factor) (vvector*)_vvector_create(sizeof(T), initial_capacity, scale_factor)

/**
 * @brief Creates a vvector with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the vector.
 * @param initial_capacity Initial number of elements the vector can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a vector is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a vector is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the vector is destroyed, or NULL if default.
 * @return Pointer to the newly created vvector, or NULL if creation fails.
*/
vvector1* _vvector1_create(size_t stride, size_t initial_capacity, double scale_factor, vvector1_el_ctor ctor, vvector1_el_cctor cctor, vvector1_el_dtor dtor);

/**
 * @brief Creates a vvector with the specified element stride, initial capacity, and scale factor.
 *
 * @param T type of an element
 * @param initial_capacity Initial number of elements the vector can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a vector is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a vector is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the vector is destroyed, or NULL if default.
 * @return Pointer to the newly created vvector, or NULL if creation fails.
*/
#define vvector1_create(T, initial_capacity, scale_factor, ctor, cctor, dtor) (vvector1*)_vvector1_create(sizeof(T), initial_capacity, scale_factor, ctor, cctor, dtor)

/**
 * @brief Retrieves a specific field's value from the vvector.
 *
 * @param vec Pointer to the vvector.
 * @param field Field to retrieve, specified by the VVECTOR_FIELD enum.
 * @return Value of the requested field.
*/
size_t vvector_get_field(vvector* vec, VVECTOR_FIELD field);

/**
 * @brief checks whether the container is empty.
 *
 * @param vec Pointer to the vvector.
 * @param field Field to retrieve, specified by the VVECTOR_FIELD enum.
 * @return 0 = FALSE, 1 = TRUE, -1 = ERROR
*/
#define vvector_empty(vec) (vvector_get_field(vec, VVECTOR_FIELD_LENGTH) == 0X0)

/**
 * @brief Retrieves a specific field's value from the vvector.
 *
 * @param vec Pointer to the vvector.
 * @param field Field to retrieve, specified by the VVECTOR_FIELD enum.
 * @param value New value of a field.
 * @note VVECTOR_FIELD_STRIDE is not setable.
 * @return Value of the requested field.
*/
void vvector_set_field(vvector* vec, VVECTOR_FIELD field, void* value);

/**
 * @brief Changes the number of elements stored
 *
 * @param vec Pointer to the vvector.
 * @param new_size New size
*/
#define vvector_resize(vec, new_size) vvector_set_field(vec, VVECTOR_FIELD_LENGTH, (int**)new_size)

/**
 * @brief Increase the capacity of the vector
 *
 * @param vec Pointer to the vvector.
 * @param reserves Amount of extra capacity
*/
void vvector_reserve(vvector* vec, VVECTOR_SIZE_T reserves);

/**
 * @brief Direct access to the underlying contiguous storage
 *
 * @param vec Pointer to the vvector.
 * @return Pointer to the first element in the array
*/
#define vvector_data(vec) vvector_at(vec, VVECTOR_FRONT);

/**
 * @brief Access the first element
 *
 * @param vec Pointer to the vvector.
 * @return Pointer to the front element.
*/
#define vvector_front(vec) vvector_at(vec, VVECTOR_FRONT);

/**
 * @brief Access the last element
 *
 * @param vec Pointer to the vvector.
 * @return Pointer to the last element.
*/
#define vvector_back(vec) vvector_at(vec, VVECTOR_BACK);

/**
 * @brief Copies a object onto the vector.
 *
 * @param vec Pointer to the vvector.
 * @param index index value. Can be negative of positive
 * @param original Pointer to the original to copy and push onto the vector.
 * @return 0 on success, or -1 on failure.
*/
int vvector_insert(vvector* vec, VVECTOR_SSIZE_T index, const void* original);

/**
 * @brief Copies a object onto the vector.
 *
 * @param vec Pointer to the vvector.
 * @param item Pointer to the item to push onto the vector.
 * @return 0 on success, or -1 on failure.
*/
int vvector_push_back(vvector* vec, const void* original);

/**
 * @brief Constructs a new item in place at the top of the vector.
 *
 * @param vec Pointer to the vvector.
 * @param index Index value. Can be negative of positive
 * @param arg_count  Count of arguments to pass in for constructing an element
 * @param va Arguments to pass in for constructing an element
 * @return 0 on success, or -1 on failure.
*/
int vvector_emplace(vvector* vec, VVECTOR_SSIZE_T index, size_t arg_count, ...);

/**
 * @brief Constructs a new item in place at the top of the vector.
 *
 * @param vec Pointer to the vvector.
 * @param arg_count Count of arguments to pass in for constructing an element
 * @param va Arguments to pass in for constructing an element
 * @return 0 on success, or -1 on failure.
*/
int vvector_emplace_back(vvector* vec, size_t arg_count, ...);

/**
 * @brief Returns a pointer to the element at the specified index in the vvector.
 *
 * @param vec Pointer to the vector.
 * @return Pointer to the top element of the vector.
 */
void * vvector_at(vvector* vec, VVECTOR_SSIZE_T index);

/**
 * @brief Pops the item at the specified index from the vvector.
 *
 * @param vec Pointer to the vvector.
 * @param index index value. Can be negative of positive
*/
void vvector_pop_at(vvector* vec, VVECTOR_SSIZE_T index);

/**
 * @brief Pops the last item of the vector.
 *
 * @param vec Pointer to the vvector.
*/
#define pop_back(vec) vvector_pop_at(vec, VVECTOR_BACK)

/**
 * @brief Removes the elements in the range [`first`,`last`).
 *
 * @param vec Pointer to the vvector.
 * @param first Iterator start
 * @param last Iterator end
*/
void vvector_erase(vvector* vec, VVECTOR_SIZE_T first, VVECTOR_SIZE_T last);

/**
 * @brief Erases all elements from the container.
 *
 * @param vec Pointer to the vvector.
*/
void vvector_clear(vvector* vec);

/**
 * @brief Exchanges the contents and capacity of the container with those of `rhs`.
 * Does not invoke any move, copy, or swap operations on individual elements.
 * @param lhs Pointer ONE vvector.
 * @param rhs Pointer ANOTHER vvector.
*/
void vvector_swap(vvector* VVECTOR_FTN_RESTRICT lhs, vvector* VVECTOR_FTN_RESTRICT rhs);

/**
 * @brief Requests the removal of unused capacity.
 *
 * @param vec Pointer to the vvector.
*/
void vvector_shrink_to_fit(vvector* vec);

/**
 * @brief Destroys the vvector and frees associated memory.
 *
*/
void vvector_destroy(vvector** vec);

#define vvector_foreach(vec, it, func) \
    while (vec)\
    {\
       \
    }\
    

#ifdef __cplusplus
}
#endif

#endif // __vvector__
