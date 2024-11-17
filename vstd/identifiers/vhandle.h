#ifndef _v_handle_H_
#define _v_handle_H_

#include "identifiers/identifier.h"
#include "defines.h"

#define INVALID_v_handle INVALID_ID_U64

/**
 * @brief A handle is a unique identifier used a system in the engine to
 * avoid using raw pointers where possible.
 */
typedef struct v_handle {
    /** @brief Index into a resource table. Considered null if == INVALID_ID */
    u32 handle_index;
    /** @brief A globally unique identifier */
    identifier unique_id;
} v_handle;

/**
 * @brief Creates and returns a handle with the given handle index. Also creates a new unique identifier.
 */
VAPI v_handle v_handle_create(u32 handle_index);

/** @brief Creates and returns a handle based on the handle index and identifier provided. */
VAPI v_handle v_handle_create_with_identifier(u32 handle_index, identifier id);

/** @brief Creates and returns an invalid handle. */
VAPI v_handle v_handle_invalid(void);

/** @brief Indicates if the provided handle is invalid. */
VAPI b8 v_handle_is_invalid(v_handle handle);

/** @brief Invalidates the provided handle. */
VAPI void v_handle_invalidate(v_handle* handle);

#endif
