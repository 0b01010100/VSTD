#include "containers/darray.h"

#include "logger.h"
#include "memory/vmemory.h"

void* _darray_create(u64 length, u64 stride, frame_allocator_int* allocator) {
    u64 header_size = sizeof(darray_header);
    u64 array_size = length * stride;
    void* new_array = 0;
    if (allocator) {
        new_array = allocator->allocate(header_size + array_size);
    } else {
        new_array = vallocate(header_size + array_size, MEMORY_TAG_DARRAY);
    }
    vset_memory(new_array, 0, header_size + array_size);
    if (length == 0) {
        VFATAL("_darray_create called with length of 0");
    }
    darray_header* header = new_array;
    header->capacity = length;
    header->length = 0;
    header->stride = stride;
    header->allocator = allocator;

    return (void*)((u8*)new_array + header_size);
}

void darray_destroy(void* array) {
    if (array) {
        u64 header_size = sizeof(darray_header);
        darray_header* header = (darray_header*)((u8*)array - header_size);
        u64 total_size = header_size + header->capacity * header->stride;
        if (header->allocator) {
            header->allocator->free(header, total_size);
        } else {
            vfree(header, total_size, MEMORY_TAG_DARRAY);
        }
    }
}

void* _darray_resize(void* array) {
    u64 header_size = sizeof(darray_header);
    darray_header* header = (darray_header*)((u8*)array - header_size);
    if (header->capacity == 0) {
        VFATAL("_darray_resize called on an array with 0 capacity. This should not be possible.");
        return 0;
    }
    void* temp = _darray_create((DARRAY_RESIZE_FACTOR * header->capacity), header->stride, header->allocator);

    darray_header* new_header = (darray_header*)((u8*)temp - header_size);
    new_header->length = header->length;

    vcopy_memory(temp, array, header->length * header->stride);

    darray_destroy(array);
    return temp;
}

void* _darray_push(void* array, const void* value_ptr) {
    u64 header_size = sizeof(darray_header);
    darray_header* header = (darray_header*)((u8*)array - header_size);
    if (header->length >= header->capacity) {
        array = _darray_resize(array);
        header = (darray_header*)((u8*)array - header_size);
    }

    u64 addr = (u64)array;
    addr += (header->length * header->stride);
    vcopy_memory((void*)addr, value_ptr, header->stride);
    darray_length_set(array, header->length + 1);
    return array;
}

void _darray_pop(void* array, void* dest) {
    u64 length = darray_length(array);
    u64 stride = darray_stride(array);
    if (length < 1) {
        VERROR("darray_pop called on an empty darray. Nothing to be done.");
        return;
    }
    u64 addr = (u64)array;
    addr += ((length - 1) * stride);
    if (dest) {
        vcopy_memory(dest, (void*)addr, stride);
    }
    darray_length_set(array, length - 1);
}

void* darray_pop_at(void* array, u64 index, void* dest) {
    u64 length = darray_length(array);
    u64 stride = darray_stride(array);
    if (index >= length) {
        VERROR("Index outside the bounds of this array! Length: %i, index: %index", length, index);
        return array;
    }

    u64 addr = (u64)array;
    if (dest) {
        vcopy_memory(dest, (void*)(addr + (index * stride)), stride);
    }

    // If not on the last element, snip out the entry and copy the rest inward.
    if (index != length - 1) {
        vcopy_memory(
            (void*)(addr + (index * stride)),
            (void*)(addr + ((index + 1) * stride)),
            stride * (length - (index - 1)));
    }

    darray_length_set(array, length - 1);
    return array;
}

void* _darray_insert_at(void* array, u64 index, void* value_ptr) {
    u64 length = darray_length(array);
    u64 stride = darray_stride(array);
    if (index >= length) {
        VERROR("Index outside the bounds of this array! Length: %i, index: %index", length, index);
        return array;
    }
    if (length >= darray_capacity(array)) {
        array = _darray_resize(array);
    }

    u64 addr = (u64)array;

    // Push element(s) from index forward out by one. This should
    // even happen if inserted at the last index.
    vcopy_memory(
        (void*)(addr + ((index + 1) * stride)),
        (void*)(addr + (index * stride)),
        stride * (length - index));

    // Set the value at the index
    vcopy_memory((void*)(addr + (index * stride)), value_ptr, stride);

    darray_length_set(array, length + 1);
    return array;
}

void darray_clear(void* array) {
    darray_length_set(array, 0);
}

u64 darray_capacity(void* array) {
    u64 header_size = sizeof(darray_header);
    darray_header* header = (darray_header*)((u8*)array - header_size);
    return header->capacity;
}

u64 darray_length(void* array) {
    u64 header_size = sizeof(darray_header);
    darray_header* header = (darray_header*)((u8*)array - header_size);
    return header->length;
}

u64 darray_stride(void* array) {
    u64 header_size = sizeof(darray_header);
    darray_header* header = (darray_header*)((u8*)array - header_size);
    return header->stride;
}

void darray_length_set(void* array, u64 value) {
    u64 header_size = sizeof(darray_header);
    darray_header* header = (darray_header*)((u8*)array - header_size);
    header->length = value;
}
