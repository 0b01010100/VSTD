#ifndef __vstack__
#define __vstack__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enum for specifying which field to query from a vstack.
 */
typedef enum VSTACK_FIELD
{
    VSTACK_FIELD_STRIDE          = 1,  /**< Size of each element in the stack */
    VSTACK_FIELD_LENGTH          = 2,  /**< Current number of elements in the stack */
    VSTACK_FIELD_CAPACITY        = 3,  /**< Maximum number of elements the stack can hold */
    VSTACK_FIELD_SCALE_PERCENT   = 4   /**< Scale factor for dynamic resizing, in percentage */
} VSTACK_FIELD;

/**
 * @brief Stack handle
*/
typedef struct vstack vstack;
/**
 * @brief Represents a destructor function to be called for each element when the stack is destroyed,
*/
typedef void (*vstack_el_dtor)(void * __date, size_t size);

/**
 * @brief Creates a vstack with the specified element type, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the stack.
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param dtor Destructor function to be called for each element when the stack is destroyed, or NULL if not needed.
 * @return Pointer to the newly created vstack, or NULL if creation fails.
 */
#define vstack_create(T, initial_capacity, scale_factor, dtor) (vstack*)_vstack_create(sizeof(T), initial_capacity, scale_factor, dtor)

/**
 * @brief Creates a vstack with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the stack.
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param dtor Destructor function to be called for each element when the stack is destroyed, or NULL if not needed.
 * @return Pointer to the newly created vstack, or NULL if creation fails.
 */
vstack* _vstack_create(size_t stride, size_t initial_capacity, double scale_factor, vstack_el_dtor dtor);

/**
 * @brief Retrieves a specific field's value from the vstack.
 *
 * @param sk Pointer to the vstack.
 * @param field Field to retrieve, specified by the VSTACK_FIELD enum.
 * @return Value of the requested field.
 */
size_t vstack_get_field(vstack* sk, VSTACK_FIELD field);

/**
 * @brief Resizes the vstack to a new capacity.
 *
 * @param sk Pointer to the vstack.
 * @param capacity New capacity for the stack.
 * @return 0 on success, or -1 on failure.
 */
int vstack_resize(vstack* sk, size_t capacity);

/**
 * @brief Returns a pointer to the raw data in the vstack.
 *
 * @param sk Pointer to the vstack.
 * @return Pointer to the raw data.
 */
const void * vstack_data(vstack* sk);

/**
 * @brief Returns a pointer to the element at the specified index in the vstack.
 *
 * @param sk Pointer to the vstack.
 * @param index Index of the element to retrieve.
 * @return Pointer to the element at the specified index, or NULL if the index is out of range.
 */
void * vstack_peek(vstack* sk, size_t index);

/**
 * @brief Pushes a new item onto the vstack.
 *
 * @param sk Pointer to the vstack.
 * @param item Pointer to the item to push onto the stack.
 * @return 0 on success, or -1 on failure.
 */
int vstack_push(vstack* sk, const void* item);

/**
 * @brief Pops the item at the specified index from the vstack.
 *
 * @param sk Pointer to the vstack.
 * @param index Index of the item to pop.
 */
void vstack_pop(vstack* sk, size_t index);

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
