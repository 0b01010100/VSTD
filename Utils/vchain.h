#ifndef __vchain__
#define __vchain__
//@ref at: https://en.cppreference.com/w/cpp/container/list
#include <stdint.h>
//same as size_t
typedef uint64_t VCHAIN_SIZE_T;
//same as ssize_t
typedef int64_t VCHAIN_SSIZE_T;
#include <stdarg.h> // <---- will be useful for the emplace functions

//Represents the front index of a chain
#define VCHAIN_FRONT (0)
//Represents the back index of a chain
#define VCHAIN_BACK (-1)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Version of the vchain type
*/
typedef enum VCHAIN_VER /* : int*/
{
    VCHAIN_VER_0_0 = 0x0,
    VCHAIN_VER_1_0 = 0x1
}VCHAIN_VER;

/**
 * @brief Enum for specifying which field to query from a vchain.
*/
typedef enum VCHAIN_FIELD /* : int*/
{

    VCHAIN_FIELD_VER             = 0,  /**< Version of the vchain structure */

    //vchain version 0.0

    VCHAIN_FIELD_STRIDE          = 1, /**< Size of each element in the chain */
    VCHAIN_FIELD_LENGTH          = 2,  /**< Current number of elements in the chain */

    //vchain version 1.0

    VCHAIN1_FIELD_CTOR           = 5,  /**< Contructor function for each element */
    VCHAIN1_FIELD_CCTOR          = 6,  /**< Copy contructor function for each element */
    VCHAIN1_FIELD_DTOR           = 7,  /**< Destructor function for each element */
} VCHAIN_FIELD;

/**
 * @brief chain version 0.0 handle
*/
typedef struct vchain vchain;
/**
 * @brief chain version 1.0 handle
*/
typedef vchain vchain1;

/**
 * @brief Represents a constructor function to be called for each element when the chain is destroyed,
 * @note `this` is preallocated
*/
typedef int (*vchain1_el_ctor)(void ** this, size_t size, va_list* args, size_t count);

/**
 * @brief Represents a copy constructor function to be called for each element when the chain is destroyed,
 * @note `this` is preallocated
*/
typedef int (*vchain1_el_cctor)(void ** this, const void * original, size_t size);

/**
 * @brief Represents a destructor function to be called for each element when the chain is destroyed,
 * @note the main data(this) will be destroy auto matically it up to you to destroy other data
*/
typedef void (*vchain1_el_dtor)(void  ** this, size_t size);

/**
 * @brief Creates a vchain with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the chain.
 * @param initial_size Initial number of elements the chain can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vchain, or NULL if creation fails.
*/
vchain* _vchain_create(size_t stride, size_t initial_size);

/**
 * @brief Creates a vchain with the specified element type, initial capacity, and scale factor.
 *
 * @param T Type of an element
 * @param initial_size Initial number of elements the chain can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vchain, or NULL if creation fails.
*/
#define vchain_create(T, initial_size, scale_factor) (vchain*)_vchain_create(sizeof(T), initial_size, scale_factor)

/**
 * @brief Creates a vchain with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the chain.
 * @param initial_size Initial number of elements the chain can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a chain is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a chain is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the chain is destroyed, or NULL if default.
 * @return Pointer to the newly created vchain, or NULL if creation fails.
*/
vchain1* _vchain1_create(size_t stride, size_t initial_size, vchain1_el_ctor ctor, vchain1_el_cctor cctor, vchain1_el_dtor dtor);

/**
 * @brief Creates a vchain with the specified element stride, initial capacity, and scale factor.
 *
 * @param T type of an element
 * @param initial_size Initial number of elements the chain can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a chain is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a chain is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the chain is destroyed, or NULL if default.
 * @return Pointer to the newly created vchain, or NULL if creation fails.
*/
#define vchain1_create(T, initial_size, scale_factor, ctor, cctor, dtor) (vchain1*)_vchain1_create(sizeof(T), initial_size, scale_factor, ctor, cctor, dtor)

/**
 * @brief Retrieves a specific field's value from the vchain.
 *
 * @param ch Pointer to the vchain.
 * @param field Field to retrieve, specified by the VCHAIN_FIELD enum.
 * @return Value of the requested field.
*/
size_t vchain_get_field(vchain* ch, VCHAIN_FIELD field);

/**
 * @brief checks whether the container is empty.
 *
 * @param ch Pointer to the vchain.
 * @param field Field to retrieve, specified by the VCHAIN_FIELD enum.
 * @return 0 = FALSE, 1 = TRUE, -1 = ERROR
*/
#define vchain_empty(ch) ((ch)? (vchain_get_field(ch, VCHAIN_FIELD_LENGTH) == 0X0) : -1)

/**
 * @brief Retrieves a specific field's value from the vchain.
 *
 * @param ch Pointer to the vchain.
 * @param field Field to retrieve, specified by the VCHAIN_FIELD enum.
 * @param value New value of a field.
 * @note VCHAIN_FIELD_STRIDE is not setable.
 * @return Value of the requested field.
*/
void vchain_set_field(vchain* ch, VCHAIN_FIELD field, void* value);

/**
 * @brief Changes the number of elements stored
 *
 * @param ch Pointer to the vchain.
 * @param new_size New size
*/
#define vchain_resize(ch, new_size) vchain_set_field(ch, VCHAIN_FIELD_LENGTH, &new_size)

/**
 * @brief Access the first element
 *
 * @param ch Pointer to the vchain.
 * @return Pointer to the front element.
*/
#define vchain_front(ch) vchain_at(ch, VCHAIN_FRONT)

/**
 * @brief Access the last element
 *
 * @param ch Pointer to the vchain.
 * @return Pointer to the last element.
*/
#define vchain_back(ch) vchain_at(ch, VCHAIN_BACK)

/**
 * @brief Copies a object onto the chain.
 *
 * @param ch Pointer to the vchain.
 * @param index index value. Can be negative of positive
 * @param original Pointer to the original to copy and push onto the chain.
 * @return 0 on success, or -1 on failure.
*/
int vchain_insert(vchain* ch, VCHAIN_SSIZE_T index, const void* original);

/**
 * @brief Copies a object onto the chain.
 *
 * @param ch Pointer to the vchain.
 * @param item Pointer to the item to push onto the chain.
 * @return 0 on success, or -1 on failure.
*/
int vchain_push_back(vchain* ch, const void* original);

/**
 * @brief Constructs a new item in place at the top of the chain.
 *
 * @param ch Pointer to the vchain.
 * @param index Index value. Can be negative of positive
 * @param arg_count  Count of arguments to pass in for constructing an element
 * @param va Arguments to pass in for constructing an element
 * @return 0 on success, or -1 on failure.
*/
int vchain_emplace(vchain* ch, VCHAIN_SSIZE_T index, size_t arg_count, ...);

/**
 * @brief Constructs a new item in place at the top of the chain.
 *
 * @param ch Pointer to the vchain.
 * @param arg_count Count of arguments to pass in for constructing an element
 * @param va Arguments to pass in for constructing an element
 * @return 0 on success, or -1 on failure.
*/
int vchain_emplace_back(vchain* ch, VCHAIN_SIZE_T arg_count, ...);

/**
 * @brief Returns a pointer to the element at the specified index in the vchain.
 *
 * @param ch Pointer to the chain.
 * @return Pointer to the top element of the chain.
 */
void * vchain_at(vchain* ch, VCHAIN_SSIZE_T index);

/**
 * @brief Pops the item at the specified index from the vchain.
 *
 * @param ch Pointer to the vchain.
 * @param index index value. Can be negative of positive
*/
void vchain_pop_at(vchain* ch, VCHAIN_SSIZE_T index);

/**
 * @brief Pops the last item of the chain.
 *
 * @param ch Pointer to the vchain.
*/
#define pop_back(ch) vchain_pop_at(ch, VCHAIN_BACK)

/**
 * @brief Removes the elements in the range [`first`,`last`).
 *
 * @param ch Pointer to the vchain.
 * @param first Iterator start
 * @param last Iterator end
*/
void vchain_erase(vchain* ch, VCHAIN_SIZE_T first, VCHAIN_SIZE_T last);

/**
 * @brief Erases all elements from the container.
 *
 * @param ch Pointer to the vchain.
*/
void vchain_clear(vchain* ch);

/**
 * @brief Exchanges the contents of the container with those of `rhs`.
 * Does not invoke any move, copy, or swap operations on individual elements.
 * @param lhs Pointer ONE vchain.
 * @param rhs Pointer ANOTHER vchain.
*/
void vchain_swap(vchain* lhs, vchain* rhs);

/**
 * @brief Destroys the vchain and frees associated memory.
 *
*/
void vchain_destroy(vchain** ch);

/**
 * Macro to iterate over a vchain
 * @param T Type of the item to iterate over
 * @param item A variable of type T that will be assigned each element of the chain
 * @param ch The vchain to iterate over
 * @param action The action to perform on each iteration(if any) -> Optional
*/
#define vchain_foreach(T, item, ch, action) do { \
    size_t __len = vchain_get_field(ch, VCHAIN_FIELD_LENGTH); \
    size_t __stride = vchain_get_field(ch, VCHAIN_FIELD_STRIDE); \
    for (size_t __i = 0; __i < __len; __i++) { \
        T* item = (T*)vchain_at(ch, __i); \
        action \
    } \
} while(0)



#ifdef __cplusplus
}
#endif

#endif // __vchain__
