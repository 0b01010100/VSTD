#ifndef __vstack__
#define __vstack__

#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum VSTACK_VER /* : int*/
{
    VSTACK_VER_0_0 = 0x0,
    VSTACK_VER_1_0 = 0x1
}VSTACK_VER;

/**
 * @brief Enum for specifying which field to query from a vstack.
 */
typedef enum VSTACK_FIELD
{

    VSTACK_FIELD_VER             = 0,  /**< version of the vstack structure */

    //vstack version 0.0

    VSTACK_FIELD_STRIDE          = 1,  /**< Size of each element in the stack */
    VSTACK_FIELD_LENGTH          = 2,  /**< Current number of elements in the stack */
    VSTACK_FIELD_CAPACITY        = 3,  /**< Maximum number of elements the stack can hold */
    VSTACK_FIELD_SCALE_PERCENT   = 4,  /**< Scale factor for dynamic resizing, in percentage */

    //vstack version 1.0

    VSTACK1_FIELD_CTOR           = 5,  /**< Contructor function for each element */
    VSTACK1_FIELD_CCTOR          = 6,  /**< Copy contructor function for each element */
    VSTACK1_FIELD_DTOR           = 7,  /**< Destructor function for each element */
} VSTACK_FIELD;

/**
 * @brief Stack version 0.0 handle
*/
typedef struct vstack vstack;
/**
 * @brief Stack version 1.0 handle
*/
typedef vstack vstack1;

/**
 * @brief Represents a constructor function to be called for each element when the stack is destroyed,
*/
typedef int (*vstack1_el_ctor)(void *location, size_t size, va_list* args, size_t count);

/**
 * @brief Represents a copy constructor function to be called for each element when the stack is destroyed,
*/
typedef int (*vstack1_el_cctor)(void * location, const void * original, size_t size);

/**
 * @brief Represents a destructor function to be called for each element when the stack is destroyed,
*/
typedef void (*vstack1_el_dtor)(void  * location, size_t size);

/**
 * @brief Creates a vstack with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the stack.
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vstack, or NULL if creation fails.
 */
vstack* _vstack_create(size_t stride, size_t initial_capacity, double scale_factor);

/**
 * @brief Creates a vstack with the specified element type, initial capacity, and scale factor.
 *
 * @param T Type of an element
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vstack, or NULL if creation fails.
 */
#define vstack_create(T, initial_capacity, scale_factor) (vstack*)_vstack_create(sizeof(T), initial_capacity, scale_factor)

/**
 * @brief Creates a vstack with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the stack.
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a stack is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a stack is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the stack is destroyed, or NULL if default.
 * @return Pointer to the newly created vstack, or NULL if creation fails.
 */
vstack1* _vstack1_create(size_t stride, size_t initial_capacity, double scale_factor, vstack1_el_ctor ctor, vstack1_el_cctor cctor, vstack1_el_dtor dtor);

/**
 * @brief Creates a vstack with the specified element stride, initial capacity, and scale factor.
 *
 * @param T type of an element
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a stack is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a stack is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the stack is destroyed, or NULL if default.
 * @return Pointer to the newly created vstack, or NULL if creation fails.
 */
#define vstack1_create(T, initial_capacity, scale_factor, ctor, cctor, dtor) (vstack1*)_vstack1_create(sizeof(T), initial_capacity, scale_factor, ctor, cctor, dtor)

/**
 * @brief Retrieves a specific field's value from the vstack.
 *
 * @param sk Pointer to the vstack.
 * @param field Field to retrieve, specified by the VSTACK_FIELD enum.
 * @return Value of the requested field.
 */
size_t vstack_get_field(vstack* sk, VSTACK_FIELD field);

/**
 * @brief Retrieves a specific field's value from the vstack.
 *
 * @param sk Pointer to the vstack.
 * @param field Field to retrieve, specified by the VSTACK_FIELD enum.
 * @param value New value of a field.
 * @note VSTACK_FIELD_STRIDE is not setable.
 * @return Value of the requested field.
 */
void vstack_set_field(vstack* sk, VSTACK_FIELD field, void* value);

/**
 * @brief Returns a pointer to the element at the specified index in the vstack.
 *
 * @param sk Pointer to the vstack.
 * @return Pointer to the top element of the stack.
 */
void * vstack_peek(vstack* sk);

/**
 * @brief Copies a object onto the stack.
 *
 * @param sk Pointer to the vstack.
 * @param item Pointer to the item to push onto the stack.
 * @return 0 on success, or -1 on failure.
 */
int vstack_push(vstack* sk, const void* item);

/**
 * @brief Constructs a new item in place at the top of the stack.
 *
 * @param sk Pointer to the vstack.
 * @param count Count of arguments to pass in
 * @param va Arguments to pass in
 * @return 0 on success, or -1 on failure.
 */
int vstack_emplace(vstack* sk, size_t count, ...);

/**
 * @brief Pops the item at the specified index from the vstack.
 *
 * @param sk Pointer to the vstack.
 */
void vstack_pop(vstack* sk);

/**
 * @brief Destroys the vstack and frees associated memory.
 *
 * @param sk Pointer to a pointer to the vstack. The pointer will be set to NULL after destruction.
 */
void vstack_destroy(vstack** sk);

#ifdef __cplusplus
}
#endif

#endif // __vstack__
