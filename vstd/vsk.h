#ifndef __vsk__
#define __vsk__
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum VSK_VER /* : int*/
{
    VSK_VER_0_0 = 0x0,
    VSK_VER_1_0 = 0x1
}VSK_VER;

/**
 * @brief Enum for specifying which field to query from a vsk.
 */
typedef enum VSK_FIELD
{

    VSK_FIELD_VER             = 0,  /**< version of the vsk structure */

    //VSK version 0.0

    VSK_FIELD_STRIDE          = 1,  /**< Size of each element in the stack */
    VSK_FIELD_LENGTH          = 2,  /**< Current number of elements in the stack */
    VSK_FIELD_CAPACITY        = 3,  /**< Maximum number of elements the stack can hold */
    VSK_FIELD_SCALE_PERCENT   = 4,  /**< Scale factor for dynamic resizing, in percentage */

    //VSK version 1.0

    VSK1_FIELD_CTOR           = 5,  /**< Contructor function for each element */
    VSK1_FIELD_CCTOR          = 6,  /**< Copy contructor function for each element */
    VSK1_FIELD_DTOR           = 7,  /**< Destructor function for each element */
} VSK_FIELD;

/**
 * @brief Stack version 0.0 handle
*/
typedef struct vsk_t vsk_t;
/**
 * @brief Stack version 1.0 handle
*/
typedef vsk_t vsk1_t;

/**
 * @brief Represents a constructor function to be called for each element when the stack is destroyed,
*/
typedef int (*vsk1_ctor_t)(void * self, size_t size, va_list* args, size_t count);

/**
 * @brief Represents a copy constructor function to be called for each element when the stack is destroyed,
*/
typedef int (*vsk1_cctor_t)(void * self, const void * original, size_t size);

/**
 * @brief Represents a destructor function to be called for each element when the stack is destroyed,
*/
typedef void (*vsk1_dtor_t)(void * self, size_t size);

/**
 * @brief Creates a vsk with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the stack.
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vsk, or NULL if creation fails.
 */
vsk_t* _vsk_create(size_t stride, size_t initial_capacity, double scale_factor);

/**
 * @brief Creates a vsk with the specified element type, initial capacity, and scale factor.
 *
 * @param T Type of an element
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vsk, or NULL if creation fails.
 */
#define vsk_create(T, initial_capacity, scale_factor) (vsk_t*)_vsk_create(sizeof(T), initial_capacity, scale_factor)

/**
 * @brief Creates a vsk with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the stack.
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a stack is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a stack is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the stack is destroyed, or NULL if default.
 * @return Pointer to the newly created vsk, or NULL if creation fails.
 */
vsk1_t* _vsk1_create(size_t stride, size_t initial_capacity, double scale_factor, vsk1_ctor_t ctor, vsk1_cctor_t cctor, vsk1_dtor_t dtor);

/**
 * @brief Creates a vsk with the specified element stride, initial capacity, and scale factor.
 *
 * @param T type of an element
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a stack is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a stack is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the stack is destroyed, or NULL if default.
 * @return Pointer to the newly created vsk, or NULL if creation fails.
 */
#define vsk1_create(T, initial_capacity, scale_factor, ctor, cctor, dtor) (vsk1_t*)_vsk1_create(sizeof(T), initial_capacity, scale_factor, ctor, cctor, dtor)

/**
 * @brief Retrieves a specific field's value from the vsk.
 *
 * @param sk Pointer to the vsk.
 * @param field Field to retrieve, specified by the VSK_FIELD enum.
 * @return Value of the requested field.
 */
size_t VSK_get_field(vsk_t* sk, VSK_FIELD field);

/**
 * @brief Retrieves a specific field's value from the vsk.
 *
 * @param sk Pointer to the vsk.
 * @param field Field to retrieve, specified by the VSK_FIELD enum.
 * @param value New value of a field.
 * @note VSK_FIELD_STRIDE is not setable.
 * @return Value of the requested field.
 */
void VSK_set_field(vsk_t* sk, VSK_FIELD field, void* value);

/**
 * @brief checks whether the container is empty.
 *
 * @param sk Pointer to the vsk.
 * @return 0 = FALSE, 1 = TRUE, -1 = ERROR
*/
#define VSK_empty(sk) (VSK_get_field(sk, VSK_FIELD_LENGTH) == 0X0)

/**
 * @brief Direct access to the underlying contiguous storage
 *
 * @param sk Pointer to the vsk_t.
 * @return Pointer to the first element in the array
*/
#define vsk_data(sk) vsk_peek(sk)

/**
 * @brief Returns a pointer to the element at the specified index in the vsk_t.
 *
 * @param sk Pointer to the vsk_t.
 * @return Pointer to the top element of the stack.
 */
void * vsk_peek(vsk_t* sk);

/**
 * @brief Copies a object onto the stack.
 *
 * @param sk Pointer to the vsk_t.
 * @param item Pointer to the item to push onto the stack.
 * @return 0 on success, or -1 on failure.
 */
int vsk_push(vsk_t* sk, const void* item);

/**
 * @brief Constructs a new item in place at the top of the stack.
 *
 * @param sk Pointer to the vsk_t.
 * @param count Count of arguments to pass in
 * @param va Arguments to pass in
 * @return 0 on success, or -1 on failure.
 */
int vsk_emplace(vsk_t* sk, size_t count, ...);

/**
 * @brief Pops the item at the specified index from the vsk_t.
 *
 * @param sk Pointer to the vsk_t.
 */
void vsk_pop(vsk_t* sk);

/**
 * @brief Exchanges the contents and capacity of the container with those of `rhs`.
 * Does not invoke any move, copy, or swap operations on individual elements.
 * @param lhs Pointer ONE vsk_t.
 * @param rhs Pointer ANOTHER vsk_t.
*/
void vsk_swap(vsk_t* lhs, vsk_t* rhs);

/**
 * @brief Removes the elements in the range [`first`,`last`).
 *
 * @param sk Pointer to the vsktor.
 * @note Iterator start is the top of the stack
 * @param last Iterator end
*/
void vsktor_erase(vsk_t* sk, size_t last);

/**
 * @brief Erases all elements from the container.
 *
 * @param sk Pointer to the vsk_t.
*/
void vsk_clear(vsk_t* sk);

/**
 * @brief Destroys the vsk_t and frees associated memory.
 *
 * @param sk Pointer to a pointer to the vsk_t. The pointer will be set to NULL after destruction.
 */
void vsk_destroy(vsk_t** sk);

/**
 * Macro to iterate over a stack
 * @param T Type of the item to iterate over
 * @param item A variable of type T that will be assigned each element of the stack
 * @param sk The stack to iterate over
 * @param action The action to perform on each iteration(if any) -> Optional
*/
#define vsk_foreach(T, item, sk, action) {\
    size_t __len = vsk_get_field(sk, VSK_FIELD_LENGTH);\
    unsigned char* __data = vsk_data(sk);\
    size_t __stride = vsk_get_field(sk, VSK_FIELD_STRIDE);\
    for (size_t __i = 0; __i < __len; __i++) {\
        item = *(int *)(__data - __stride * __i);\
        action\
    }\
}
#ifdef __cplusplus
}
#endif

#endif // __vsk__
