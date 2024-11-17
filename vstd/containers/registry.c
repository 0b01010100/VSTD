#include "registry.h"

#include "containers/darray.h"
#include "containers/registry.h"
#include "debug/vassert.h"
#include "defines.h"
#include "identifiers/vhandle.h"
#include "logger.h"
#include "memory/vmemory.h"

void vregistry_create(vregistry* out_registry) {
    if (!out_registry) {
        VERROR("vregistry_create requires a valid pointer to out_registry.");
        return;
    }

    out_registry->entries = darray_create(vregistry_entry);
}

void vregistry_destroy(vregistry* registry) {
    if (registry) {
        u32 entry_count = darray_length(registry->entries);
        for (u32 i = 0; i < entry_count; ++i) {
            vregistry_entry* entry = &registry->entries[i];

            // Destroy callbacks.
            if (entry->callbacks) {
                // NOTE: May want to notify of this perhaps, but not going to add this unless it's needed.
                darray_destroy(entry->callbacks);
                entry->callbacks = 0;
            }

            // Free resources
            if (entry->block) {
                vfree(entry->block, entry->block_size, MEMORY_TAG_REGISTRY);
                entry->block = 0;
                entry->block_size = 0;
            }
        }
        darray_destroy(registry->entries);
        registry->entries = 0;
    }
}

v_handle vregistry_add_entry(vregistry* registry, const void* block, u64 size, b8 auto_release) {
    if (!registry || !size) {
        VERROR("registry_add_entry requires a valid pointer to registry, and a nonzero size. Invalid handle will be returned.");
        return v_handle_invalid();
    }

    // Check that the block hasn't already been registered.
    u32 entry_count = darray_length(registry->entries);
    for (u32 i = 0; i < entry_count; ++i) {
        vregistry_entry* entry = &registry->entries[i];
        if (entry->block == block) {
            VWARN("Block of memory at address 0x%x has already been registered, and will not be re-registered. Returning its handle.");
            return v_handle_create_with_identifier(i, (identifier){entry->uniqueid});
        }
    }

    // If not, loop through existing array first and see if there is an open slot to use.
    for (u32 i = 0; i < entry_count; ++i) {
        vregistry_entry* entry = &registry->entries[i];
        if (entry->uniqueid == INVALID_ID_U64) {
            // Found an empty block, use it.
            v_handle new_handle = v_handle_create(i);
            entry->uniqueid = new_handle.unique_id.uniqueid;

            // Allocate a block of memory and copy the provided block to it.
            entry->block = vallocate(size, MEMORY_TAG_REGISTRY);
            entry->block_size = size;

            // Take a copy of the block of memory, if provided.
            if (block) {
                vcopy_memory(entry->block, block, entry->block_size);
            }

            entry->auto_release = auto_release;
            entry->reference_count = 0;
            entry->callbacks = 0; // Will create this on the fly if listened to.

            return new_handle;
        }
    }

    // If this pointe is reached, an entry doesn't exist and there was no free slot for one to be added to.
    // Create a new entry and push it to the array instead. Use the current entry_count as the index since it
    // will be added onto the end.
    {
        v_handle new_handle = v_handle_create(entry_count);

        vregistry_entry new_entry = {0};
        new_entry.uniqueid = new_handle.unique_id.uniqueid;

        // Allocate a block of memory and copy the provided block to it.
        new_entry.block = vallocate(size, MEMORY_TAG_REGISTRY);
        new_entry.block_size = size;

        // Take a copy of the block of memory, if provided.
        if (block) {
            vcopy_memory(new_entry.block, block, new_entry.block_size);
        }

        new_entry.auto_release = auto_release;
        new_entry.reference_count = 0;
        new_entry.callbacks = 0; // Will create this on the fly if listened to.

        // Now push the new entry into the array.
        darray_push(registry->entries, new_entry);

        return new_handle;
    }
}

b8 vregistry_entry_set(vregistry* registry, v_handle entry_handle, const void* block, u64 size, void* sender) {
    if (!registry || !block || !size) {
        VERROR("registry_entry_set requires a valid pointer to a registry and block, as well as have a nonzero size. Nothing was done.");
        return false;
    }

    if (v_handle_is_invalid(entry_handle)) {
        VERROR("registry_entry_set requires a valid handle, yet an invalid one was passed. Nothing was done.");
        return false;
    }

    u32 entry_count = darray_length(registry->entries);
    if (entry_handle.handle_index >= entry_count) {
        VERROR("The provided handle refers to an index which is out of range for the given registry. Nothing was done.");
        return false;
    }

    vregistry_entry* entry = &registry->entries[entry_handle.handle_index];

    // If block and size are set (they should be) release them first.
    VASSERT_MSG((entry->block && entry->block_size), "vregistry_entry_set called against an entry which somehow does not have a block and/or size. This means something is terribly wrong here.");
    vfree(entry->block, entry->block_size, MEMORY_TAG_REGISTRY);

    // Update the block and size.
    entry->block = vallocate(size, MEMORY_TAG_REGISTRY);
    entry->block_size = size;
    vcopy_memory(entry->block, block, entry->block_size);

    // Notify listeners that the block has changed if there are any.
    if (entry->callbacks) {
        u32 callback_count = darray_length(entry->callbacks);
        for (u32 i = 0; i < callback_count; ++i) {
            vregistry_entry_listener_callback* listener_callback = &entry->callbacks[i];
            if (listener_callback->callback) {
                listener_callback->callback(sender, entry->block, entry->block_size, V_REGISTRY_CHANGE_TYPE_BLOCK_CHANGED);
            }
        }
    }

    return true;
}

b8 vregistry_entry_update_callback_for_listener(vregistry* registry, v_handle entry_handle, void* listener, PFN_on_registry_entry_updated updated_callback) {
    if (!registry || !listener || !updated_callback) {
        VERROR("vregistry_entry_update_callback_for_listener requires a valid pointer to a registry, listener, and updated_callback. Nothing was done.");
        return false;
    }

    if (v_handle_is_invalid(entry_handle)) {
        VERROR("vregistry_entry_update_callback_for_listener requires a valid handle, yet an invalid one was passed. Nothing was done.");
        return false;
    }

    u32 entry_count = darray_length(registry->entries);
    if (entry_handle.handle_index >= entry_count) {
        VERROR("The provided handle refers to an index which is out of range for the given registry. Nothing was done.");
        return false;
    }

    vregistry_entry* entry = &registry->entries[entry_handle.handle_index];

    if (entry->callbacks) {
        u32 callback_count = darray_length(entry->callbacks);
        for (u32 i = 0; i < callback_count; ++i) {
            vregistry_entry_listener_callback* listener_callback = &entry->callbacks[i];
            if (listener_callback->listener == listener) {
                if (listener_callback->callback == updated_callback) {
                    // If both the listener and callback match, we are okay. Just warn about it.
                    VWARN("There is already a registered combination of this listener and callback. Nothing needs to be done.");
                } else {
                    // Update it.
                    listener_callback->callback = updated_callback;
                }
                // Either way, count this as a success.
                return true;
            }
        }
    }

    VERROR("No matching listener was found to update this callback for. Nothing was done.");
    return false;
}

void* vregistry_entry_acquire(vregistry* registry, v_handle entry_handle, void* listener, PFN_on_registry_entry_updated updated_callback) {
    if (!registry) {
        VERROR("registry_entry_acquire requires a valid pointer to a registry. 0/null will be returned.");
        return 0;
    }

    if (v_handle_is_invalid(entry_handle)) {
        VERROR("registry_entry_acquire requires a valid handle, yet an invalid one was passed. 0/null will be returned.");
        return 0;
    }

    u32 entry_count = darray_length(registry->entries);
    if (entry_handle.handle_index >= entry_count) {
        VERROR("The provided handle refers to an index which is out of range for the given registry. 0/null will be returned.");
        return 0;
    }

    vregistry_entry* entry = &registry->entries[entry_handle.handle_index];

    // Ensure the handle isn't stale.
    if (entry->uniqueid != entry_handle.unique_id.uniqueid) {
        VERROR("A stale handle was provided to registry_entry_acquire, thus selection cannot continue. 0/null will be returned.");
        return 0;
    }

    // Setup the listener/callback, if provided.
    if (updated_callback) {
        b8 skip_callback = false;
        if (entry->callbacks) {
            // If callbacks exists, check to make sure the listener doesn't already have an entry. If it does, acquisition will fail.
            u32 callback_count = darray_length(entry->callbacks);
            for (u32 i = 0; i < callback_count; ++i) {
                vregistry_entry_listener_callback* listener_callback = &entry->callbacks[i];
                if (listener_callback->listener == listener) {
                    if (listener_callback->callback == updated_callback) {
                        // If both the listener and callback match, we are okay. Just warn about it.
                        VWARN("Only one callback per listener can exist, and this listener is already registered. The good news is, so is the callback, so it's all good.");
                        skip_callback = true;
                        break;
                    } else {
                        VERROR("Only one callback per listener can exist, and this listener is already registered. The callback is different, so acquisition fails. 0/null will be returned.");
                        return 0;
                    }
                }
            }
        } else {
            // No callbacks are registered because the array doesn't even exist. Set it up.
            entry->callbacks = darray_create(vregistry_entry_listener_callback);
        }

        if (!skip_callback) {
            // Add it
            vregistry_entry_listener_callback listener_callback = {0};
            // If no listener was passed, assume the listener to be the block itself.
            listener_callback.listener = listener ? listener : entry->block;
            listener_callback.callback = updated_callback;
            darray_push(entry->callbacks, listener_callback);
        }
    }

    // Update the internal reference counter.
    entry->reference_count++;

    // Finally, return the block.
    return entry->block;
}

void vregistry_entry_release(vregistry* registry, v_handle entry_handle, void* listener) {
    if (!registry) {
        VERROR("registry_entry_release requires a valid pointer to a registry.");
        return;
    }

    if (v_handle_is_invalid(entry_handle)) {
        VERROR("registry_entry_release requires a valid entry_handle.");
        return;
    }

    u32 entry_count = darray_length(registry->entries);
    if (entry_handle.handle_index >= entry_count) {
        VERROR("The handle passed to registry_entry_release is outside the bounds of the entries for the provided registry.");
        return;
    }

    vregistry_entry* entry = &registry->entries[entry_handle.handle_index];
    if (entry->callbacks) {
        u32 callback_count = darray_length(entry->callbacks);
        for (u32 i = 0; i < callback_count; ++i) {
            vregistry_entry_listener_callback* listener_callback = &entry->callbacks[i];
            if (listener_callback->listener == listener) {
                // Remove it and throw the popped value away.
                darray_pop_at(entry->callbacks, i, 0);
                break;
            }
        }
    }

    // Decrement the reference counter.
    entry->reference_count--;

    // If setup to auto-release, do it if the counter reaches 0.
    if (entry->reference_count < 1 && entry->auto_release) {
        if (entry->block && entry->block_size) {
            vfree(entry->block, entry->block_size, MEMORY_TAG_REGISTRY);
            entry->block = 0;
            entry->block_size = 0;
        }

        if (entry->callbacks) {
            // Notify listeners. Theoretically there shouldn't be any, but...
            u32 callback_count = darray_length(entry->callbacks);
            for (u32 i = 0; i < callback_count; ++i) {
                vregistry_entry_listener_callback* listener_callback = &entry->callbacks[i];
                if (listener_callback->callback) {
                    listener_callback->callback(listener, entry->block, entry->block_size, V_REGISTRY_CHANGE_TYPE_DESTROYED);
                }
            }

            // Destroy all callbacks.
            darray_destroy(entry->callbacks);
            entry->callbacks = 0;
        }

        entry->auto_release = false;
        entry->reference_count = 0;

        // Invalidating the uniqueid marks this as an available "slot".
        entry->uniqueid = INVALID_ID_U64;
    }
}
