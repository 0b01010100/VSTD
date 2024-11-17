#include "memory/vmemory.h"

#include "logger.h"
#include "memory/allocators/dynamic_allocator.h"
#include "platform/platform.h"
#include "strings/vstring.h"
#include "threads/vmutex.h"

// TODO: Custom string lib
#include <stdio.h>
#include <string.h>

struct memory_stats {
    u64 total_allocated;
    u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
    u64 new_tagged_allocations[MEMORY_TAG_MAX_TAGS];
    u64 new_tagged_deallocations[MEMORY_TAG_MAX_TAGS];
};

static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN    ",
    "ARRAY      ",
    "LINEAR_ALLC",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "ENGINE     ",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "GAME       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      ",
    "RESOURCE   ",
    "VULKAN     ",
    "VULKAN_EXT ",
    "DIRECT3D   ",
    "OPENGL     ",
    "GPU_LOCAL  ",
    "BITMAP_FONT",
    "SYSTEM_FONT",
    "KEYMAP     ",
    "HASHTABLE  ",
    "UI         ",
    "AUDIO      ",
    "REGISTRY   ",
    "PLUGIN     "};

typedef struct memory_system_state {
    memory_system_configuration config;
    struct memory_stats stats;
    u64 alloc_count;
    u64 allocator_memory_requirement;
    dynamic_allocator allocator;
    void* allocator_block;
    // A mutex for allocations/frees
    vmutex allocation_mutex;
} memory_system_state;

// Pointer to system state.
static memory_system_state* state_ptr;

b8 memory_system_initialize(memory_system_configuration config) {
    // The amount needed by the system state.
    u64 state_memory_requirement = sizeof(memory_system_state);

    // Figure out how much space the dynamic allocator needs.
    u64 alloc_requirement = 0;
    dynamic_allocator_create(config.total_alloc_size, &alloc_requirement, 0, 0);

    // Call the platform allocator to get the memory for the whole system, including the state.
    // TODO: memory alignment
    void* block = platform_allocate(state_memory_requirement + alloc_requirement, true);
    if (!block) {
        VFATAL("Memory system allocation failed and the system cannot continue.");
        return false;
    }

    // The state is in the first part of the massive block of memory.
    state_ptr = (memory_system_state*)block;
    state_ptr->config = config;
    state_ptr->alloc_count = 0;
    state_ptr->allocator_memory_requirement = alloc_requirement;
    platform_zero_memory(&state_ptr->stats, sizeof(state_ptr->stats));
    // The allocator block is in the same block of memory, but after the state.
    state_ptr->allocator_block = ((char*)block + state_memory_requirement);

    if (!dynamic_allocator_create(
            config.total_alloc_size,
            &state_ptr->allocator_memory_requirement,
            state_ptr->allocator_block,
            &state_ptr->allocator)) {
        VFATAL("Memory system is unable to setup internal allocator. Application cannot continue.");
        return false;
    }

    // Create allocation mutex
    if (!vmutex_create(&state_ptr->allocation_mutex)) {
        VFATAL("Unable to create allocation mutex!");
        return false;
    }

    VDEBUG("Memory system successfully allocated %llu bytes.", config.total_alloc_size);
    return true;
}

void memory_system_shutdown(void) {
    if (state_ptr) {
        // Destroy allocation mutex
        vmutex_destroy(&state_ptr->allocation_mutex);

        dynamic_allocator_destroy(&state_ptr->allocator);
        // Free the entire block.
        platform_free(state_ptr, state_ptr->allocator_memory_requirement + sizeof(memory_system_state));
    }
    state_ptr = 0;
}

void* vallocate(u64 size, memory_tag tag) {
    return vallocate_aligned(size, 1, tag);
}

void* vallocate_aligned(u64 size, u16 alignment, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        VWARN("vallocate_aligned called using MEMORY_TAG_UNKNOWN. Re-class this allocation.");
    }

    // Either allocate from the system's allocator or the OS. The latter shouldn't ever
    // really happen.
    void* block = 0;
    if (state_ptr) {
        // Make sure multithreaded requests don't trample each other.
        if (!vmutex_lock(&state_ptr->allocation_mutex)) {
            VFATAL("Error obtaining mutex lock during allocation.");
            return 0;
        }

        // FIXME: Track aligned alloc offset as part of size.
        state_ptr->stats.total_allocated += size;
        state_ptr->stats.tagged_allocations[tag] += size;
        state_ptr->stats.new_tagged_allocations[tag] += size;
        state_ptr->alloc_count++;

        block = dynamic_allocator_allocate_aligned(&state_ptr->allocator, size, alignment);
        vmutex_unlock(&state_ptr->allocation_mutex);
    } else {
        // If the system is not up yet, warn about it but give memory for now.
        /* VTRACE("Warning: vallocate_aligned called before the memory system is initialized."); */
        // TODO: Memory alignment
        block = platform_allocate(size, false);
    }

    if (block) {
        platform_zero_memory(block, size);
        return block;
    }

    VFATAL("vallocate_aligned failed to allocate successfully.");
    return 0;
}

void vallocate_report(u64 size, memory_tag tag) {
    // Make sure multithreaded requests don't trample each other.
    if (!vmutex_lock(&state_ptr->allocation_mutex)) {
        VFATAL("Error obtaining mutex lock during allocation reporting.");
        return;
    }
    state_ptr->stats.total_allocated += size;
    state_ptr->stats.tagged_allocations[tag] += size;
    state_ptr->stats.new_tagged_allocations[tag] += size;
    state_ptr->alloc_count++;
    vmutex_unlock(&state_ptr->allocation_mutex);
}

void* vreallocate(void* block, u64 old_size, u64 new_size, memory_tag tag) {
    return vreallocate_aligned(block, old_size, new_size, 1, tag);
}

void* vreallocate_aligned(void* block, u64 old_size, u64 new_size, u16 alignment, memory_tag tag) {
    void* new_block = vallocate_aligned(new_size, alignment, tag);
    if (block && new_block) {
        vcopy_memory(new_block, block, old_size);
        vfree_aligned(block, old_size, alignment, tag);
    }
    return new_block;
}

void vreallocate_report(u64 old_size, u64 new_size, memory_tag tag) {
    vfree_report(old_size, tag);
    vallocate_report(new_size, tag);
}

void vfree(void* block, u64 size, memory_tag tag) {
    vfree_aligned(block, size, 1, tag);
}

void vfree_aligned(void* block, u64 size, u16 alignment, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        VWARN("vfree_aligned called using MEMORY_TAG_UNKNOWN. Re-class this allocation.");
    }
    if (state_ptr) {
        // Make sure multithreaded requests don't trample each other.
        if (!vmutex_lock(&state_ptr->allocation_mutex)) {
            VFATAL("Unable to obtain mutex lock for free operation. Heap corruption is likely.");
            return;
        }

        state_ptr->stats.total_allocated -= size;
        state_ptr->stats.tagged_allocations[tag] -= size;
        state_ptr->stats.new_tagged_deallocations[tag] += size;
        state_ptr->alloc_count--;
        b8 result = dynamic_allocator_free_aligned(&state_ptr->allocator, block);

        vmutex_unlock(&state_ptr->allocation_mutex);

        // If the free failed, it's possible this is because the allocation was made
        // before this system was started up. Since this absolutely should be an exception
        // to the rule, try freeing it on the platform level. If this fails, some other
        // brand of skulduggery is afoot, and we have bigger problems on our hands.
        if (!result) {
            // TODO: Memory alignment
            platform_free(block, false);
        }
    } else {
        // TODO: Memory alignment
        platform_free(block, false);
    }
}

void vfree_report(u64 size, memory_tag tag) {
    // Make sure multithreaded requests don't trample each other.
    if (!vmutex_lock(&state_ptr->allocation_mutex)) {
        VFATAL("Error obtaining mutex lock during allocation reporting.");
        return;
    }
    state_ptr->stats.total_allocated -= size;
    state_ptr->stats.tagged_allocations[tag] -= size;
    state_ptr->stats.new_tagged_deallocations[tag] += size;
    state_ptr->alloc_count--;
    vmutex_unlock(&state_ptr->allocation_mutex);
}

b8 vmemory_get_size_alignment(void* block, u64* out_size, u16* out_alignment) {
    if (!vmutex_lock(&state_ptr->allocation_mutex)) {
        VFATAL("Error obtaining mutex lock during vmemory_get_size_alignment.");
        return false;
    }
    b8 result = dynamic_allocator_get_size_alignment(block, out_size, out_alignment);
    vmutex_unlock(&state_ptr->allocation_mutex);
    return result;
}

void* vzero_memory(void* block, u64 size) {
    return platform_zero_memory(block, size);
}

void* vcopy_memory(void* dest, const void* source, u64 size) {
    return platform_copy_memory(dest, source, size);
}

void* vset_memory(void* dest, i32 value, u64 size) {
    return platform_set_memory(dest, value, size);
}

const char* get_unit_for_size(u64 size_bytes, f32* out_amount) {
    if (size_bytes >= GIBIBYTES(1)) {
        *out_amount = (f64)size_bytes / GIBIBYTES(1);
        return "GiB";
    } else if (size_bytes >= MEBIBYTES(1)) {
        *out_amount = (f64)size_bytes / MEBIBYTES(1);
        return "MiB";
    } else if (size_bytes >= KIBIBYTES(1)) {
        *out_amount = (f64)size_bytes / KIBIBYTES(1);
        return "KiB";
    } else {
        *out_amount = (f32)size_bytes;
        return "B";
    }
}

char* get_memory_usage_str(void) {
    char buffer[8000] = "System memory use (tagged):\n";
    u64 offset = strlen(buffer);
    for (u32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        f32 amounts[3] = {1.0f, 1.0f, 1.0f};
        const char* units[3] = {
            get_unit_for_size(state_ptr->stats.tagged_allocations[i], &amounts[0]),
            get_unit_for_size(state_ptr->stats.new_tagged_allocations[i], &amounts[1]),
            get_unit_for_size(state_ptr->stats.new_tagged_deallocations[i], &amounts[2])};

        i32 length = snprintf(buffer + offset, 8000, "  %s: %-7.2f %-3s [+ %-7.2f %-3s | - %-7.2f%-3s]\n",
                              memory_tag_strings[i],
                              amounts[0], units[0], amounts[1], units[1], amounts[2], units[2]);
        offset += length;
    }
    vzero_memory(&state_ptr->stats.new_tagged_allocations, sizeof(state_ptr->stats.new_tagged_allocations));
    vzero_memory(&state_ptr->stats.new_tagged_deallocations, sizeof(state_ptr->stats.new_tagged_deallocations));
    {
        // Compute total usage.
        u64 total_space = dynamic_allocator_total_space(&state_ptr->allocator);
        u64 free_space = dynamic_allocator_free_space(&state_ptr->allocator);
        u64 used_space = total_space - free_space;

        f32 used_amount = 1.0f;
        const char* used_unit = get_unit_for_size(used_space, &used_amount);

        f32 total_amount = 1.0f;
        const char* total_unit = get_unit_for_size(total_space, &total_amount);

        f64 percent_used = (f64)(used_space) / total_space;

        i32 length = snprintf(buffer + offset, 8000, "Total memory usage: %.2f%s of %.2f%s (%.2f%%)\n", used_amount, used_unit, total_amount, total_unit, percent_used);
        offset += length;
    }

    char* out_string = string_duplicate(buffer);
    return out_string;
}

u64 get_memory_alloc_count(void) {
    if (state_ptr) {
        return state_ptr->alloc_count;
    }
    return 0;
}
