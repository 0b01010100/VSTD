#ifndef __vll__
#define __vll__
//@ref at: https://en.cppreference.com/w/cpp/container/list
#include <stdint.h>
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#ifndef size_t
typedef SIZE_T size_t;
#endif
#endif
#include <stdarg.h> // <---- will be useful for the emplace functions

//Represents the front index of a LINKED LIST
#define VLL_FRONT (0)
//Represents the back index of a LINKED LIST
#define VLL_BACK (-1)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Version of the LINKED LIST type
*/
typedef enum vll_VER /* : int*/
{
    VLL_VER_0_0 = 0x0,
    VLL_VER_1_0 = 0x1
}vll_VER;

/**
 * @brief Enum for specifying which field to query from a vll.
*/
typedef enum VLL_FIELD /* : int*/
{

    VLL_FIELD_VER             = 0,  /**< Version of the vll structure */

    //LINKED LIST version 0.0

    VLL_FIELD_STRIDE          = 1, /**< Size of each element in the ll */
    VLL_FIELD_LENGTH          = 2,  /**< Current number of elements in the ll */

    //LINKED LIST version 1.0

    VLL1_FIELD_CTOR           = 5,  /**< Contructor function for each element */
    VLL1_FIELD_CCTOR          = 6,  /**< Copy contructor function for each element */
    VLL1_FIELD_DTOR           = 7,  /**< Destructor function for each element */
} VLL_FIELD;

/**
 * @brief ll version 0.0 handle
*/
typedef struct vll_t vll_t;
/**
 * @brief ll version 1.0 handle
*/
typedef vll_t vll1_t;

/**
 * @brief Represents a constructor function to be called for each element when the ll is destroyed,
 * @note `this` is preallocated
*/
typedef int (*vll1_ctor_t)(void * self, size_t size, va_list args, size_t count);

/**
 * @brief Represents a copy constructor function to be called for each element when the ll is destroyed,
 * @note `this` is preallocated
*/
typedef int (*vll1_cctor_t)(void * self, const void * original, size_t size);

/**
 * @brief Represents a destructor function to be called for each element when the ll is destroyed,
 * @note The main data block will be destroy auto matically, its up to you to destroy other data
*/
typedef void (*vll1_dtor_t)(void * self, size_t size);

/**
 * @brief Creates a vll with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the ll.
 * @param initial_size Initial number of elements the ll can hold.
 * @return Pointer to the newly created vll, or NULL if creation fails.
*/
vll_t* _vll_create(size_t stride, size_t initial_size);

/**
 * @brief Creates a vll with the specified element type, initial capacity, and scale factor.
 *
 * @param T Type of an element
 * @param initial_size Initial number of elements the ll can hold.
 * @return Pointer to the newly created vll, or NULL if creation fails.
*/
#define vll_create(T, initial_size) (vll_t*)_vll_create(sizeof(T), initial_size)

/**
 * @brief Creates a vll with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the ll.
 * @param initial_size Initial number of elements the ll can hold.
 * @param ctor Constructor function to be called for when a ll is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a ll is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the ll is destroyed, or NULL if default.
 * @return Pointer to the newly created vll, or NULL if creation fails.
*/
vll1_t* _vll1_create(size_t stride, size_t initial_size, vll1_ctor_t ctor, vll1_cctor_t cctor, vll1_dtor_t dtor);

/**
 * @brief Creates a vll with the specified element stride, initial capacity, and scale factor.
 *
 * @param T type of an element
 * @param initial_size Initial number of elements the ll can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a ll is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a ll is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the ll is destroyed, or NULL if default.
 * @return Pointer to the newly created vll, or NULL if creation fails.
*/
#define vll1_create(T, initial_size, scale_factor, ctor, cctor, dtor) (vll1*)_vll1_create(sizeof(T), initial_size, scale_factor, ctor, cctor, dtor)

/**
 * @brief Retrieves a specific field's value from the vll.
 *
 * @param ll Pointer to the vll.
 * @param field Field to retrieve, specified by the vll_FIELD enum.
 * @note `Returns` can hold func ptr addresses for 
 * (VLL1_FIELD_CTOR, VLL1_FIELD_CCTOR, VLL1_FIELD_DTOR)
 * @return Value of the requested field.
*/
size_t vll_get_field(vll_t* ll, VLL_FIELD field);

/**
 * @brief checks whether the container is empty.
 *
 * @param ll Pointer to the vll.
 * @param field Field to retrieve, specified by the vll_FIELD enum.
 * @return 0 = FALSE, 1 = TRUE, -1 = ERROR
*/
#define vll_empty(ll) ((ll)? (vll_get_field(ll, VLL_FIELD_LENGTH) == 0) : -1)

/**
 * @brief Retrieves a specific field's value from the vll.
 *
 * @param ll Pointer to the vll.
 * @param field Field to retrieve, specified by the vll_FIELD enum.
 * @param value New value of a field.
 * @note vll_FIELD_STRIDE is not setable. 
 * @note `value` can also hold func ptr addresses for 
 * (VLL1_FIELD_CTOR, VLL1_FIELD_CCTOR, VLL1_FIELD_DTOR)
 * @return Value of the requested field.
*/
void vll_set_field(vll_t* ll, VLL_FIELD field, size_t value);

/**
 * @brief Changes the number of elements stored
 *
 * @param ll Pointer to the vll.
 * @param new_size New size
*/
#define vll_resize(ll, new_size) vll_set_field(ll, VLL_FIELD_LENGTH, new_size)

/**
 * @brief Access the first element
 *
 * @param ll Pointer to the vll.
 * @return Pointer to the front element.
*/
#define vll_front(ll) vll_at(ll, VLL_FRONT)

/**
 * @brief Access the last element
 *
 * @param ll Pointer to the vll.
 * @return Pointer to the last element.
*/
#define vll_back(ll) vll_at(ll, VLL_BACK)

/**
 * @brief Copies a object onto the ll.
 *
 * @param ll Pointer to the vll.
 * @param index index value. Can be negative of positive
 * @param original Pointer to the original to copy and push onto the ll.
 * @return 0 on success, or -1 on failure.
*/
int vll_insert(vll_t* ll, ssize_t index, const void* original);

/**
 * @brief Copies a object onto the ll.
 *
 * @param ll Pointer to the vll.
 * @param item Pointer to the item to push onto the ll.
 * @return 0 on success, or -1 on failure.
*/
int vll_push_back(vll_t* ll, const void* original);

/**
 * @brief Constructs a new item in place at the top of the ll.
 *
 * @param ll Pointer to the vll.
 * @param index Index value. Can be negative of positive
 * @param arg_count  Count of arguments to pass in for constructing an element
 * @param va Arguments to pass in for constructing an element
 * @return 0 on success, or -1 on failure.
*/
int vll_emplace(vll_t* ll, ssize_t index, size_t arg_count, ...);

/**
 * @brief Constructs a new item in place at the top of the ll.
 *
 * @param ll Pointer to the vll.
 * @param arg_count Count of arguments to pass in for constructing an element
 * @param va Arguments to pass in for constructing an element
 * @return 0 on success, or -1 on failure.
*/
int vll_emplace_back(vll_t* ll, size_t arg_count, ...);

/**
 * @brief Returns a pointer to the element at the specified index in the vll.
 *
 * @param ll Pointer to the ll.
 * @return Pointer to the top element of the ll.
 */
void * vll_at(vll_t* ll, ssize_t index);

/**
 * @brief Pops the item at the specified index from the vll.
 *
 * @param ll Pointer to the vll.
 * @param index index value. Can be negative of positive
*/
void vll_pop_at(vll_t* ll, ssize_t index);

/**
 * @brief Pops the last item of the ll.
 *
 * @param ll Pointer to the vll.
*/
#define pop_back(ll) vll_pop_at(ll, VLL_BACK)

/**
 * @brief Removes the elements in the range [`first`,`last`).
 *
 * @param ll Pointer to the vll.
 * @param first Iterator start
 * @param last Iterator end
*/
void vll_erase(vll_t* ll, ssize_t start, ssize_t end);

/**
 * @brief Erases all elements from the container.
 *
 * @param ll Pointer to the vll.
*/
void vll_clear(vll_t* ll);

/**
 * @brief Exchanges the contents of the container with those of `rhs`.
 * Does not invoke any move, copy, or swap operations on individual elements.
 * @param lhs Pointer ONE vll.
 * @param rhs Pointer ANOTHER vll.
*/
void vll_swap(vll_t* lhs, vll_t* rhs);

/**
 * @brief Reverses the linked list.
 *
 * This function reverses the order of elements in the 
 * linked list provided.
 *
 * @param ll A pointer to the linked list to be reversed.
 *           The function modifies the list in place.
 */
void vll_reverse(vll_t* ll);

/**
 * @brief Destroys the vll and frees associated memory.
 *
*/
void vll_destroy(vll_t** ll);

/**
 * Macro to iterate over a vll
 * @param T Type of the item to iterate over
 * @param item A variable of type T that will be assigned each element of the ll
 * @param index The name of the indexer(normally name i)
 * @param ll The vll to iterate over
 * @param action The action to perform on each iteration(if any) -> Optional
*/
#define vll_foreach(T, item, index, ll, action) do { \
    size_t __len = vll_get_field(ll, VLL_FIELD_LENGTH); \
    for (size_t index = 0; index < __len; index++) { \
        T* item = (T*)vll_at(ll, index); \
        action \
    } \
} while(0)

#ifdef __cplusplus
}
#endif

#endif // __vll__