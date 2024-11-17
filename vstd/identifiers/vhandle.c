#include "identifiers/vhandle.h"

#include "identifiers/identifier.h"
#include "defines.h"

v_handle v_handle_create(u32 handle_index) {
    v_handle out_handle = {0};
    out_handle.handle_index = handle_index;
    out_handle.unique_id = identifier_create();
    return out_handle;
}

v_handle v_handle_create_with_identifier(u32 handle_index, identifier id) {
    v_handle out_handle = {0};
    out_handle.handle_index = handle_index;
    out_handle.unique_id = id;
    return out_handle;
}

v_handle v_handle_invalid(void) {
    v_handle out_handle = {0};
    out_handle.handle_index = INVALID_ID;
    out_handle.unique_id.uniqueid = INVALID_ID_U64;
    return out_handle;
}

b8 v_handle_is_invalid(v_handle handle) {
    return handle.handle_index == INVALID_ID || handle.unique_id.uniqueid == INVALID_ID_U64;
}

void v_handle_invalidate(v_handle* handle) {
    if (handle) {
        handle->handle_index = INVALID_ID;
        handle->unique_id.uniqueid = INVALID_ID_U64;
    }
}
