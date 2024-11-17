#include "platform.h"

#if defined(VPLATFORM_LINUX) || defined(VPLATFORM_APPLE)

#    include "threads/vsemaphore.h"
#    include <dlfcn.h>
#    include <fcntl.h> // For O_* constants

// NOTE: Apple's include is on a different path.
#    if defined(VPLATFORM_APPLE)
#        include <sys/semaphore.h>
#    endif

// NOTE: Linux has its own path, plus needs a few more headers.
#    if defined(VPLATFORM_LINUX)
#        include <semaphore.h> // sudo apt install linux-headers
/* #include <sys/stat.h>   // For mode constants */
#    endif

#    include <errno.h> // For error reporting
#    include <pthread.h>
#    include <sys/shm.h>

#    include "containers/darray.h"
#    include "logger.h"
#    include "memory/vmemory.h"
#    include "strings/vstring.h"
#    include "threads/vmutex.h"
#    include "threads/vthread.h"

typedef struct nix_semaphore_internal {
    sem_t* semaphore;
    char* name;
} nix_semaphore_internal;

static u32 semaphore_id = 0;

// NOTE: Begin threads.

b8 vthread_create(pfn_thread_start start_function_ptr, void* params, b8 auto_detach, vthread* out_thread) {
    if (!start_function_ptr) {
        return false;
    }

    // pthread_create uses a function pointer that returns void*, so cold-cast to this type.
    i32 result = pthread_create((pthread_t*)&out_thread->thread_id, 0, (void* (*)(void*))start_function_ptr, params);
    if (result != 0) {
        switch (result) {
        case EAGAIN:
            VERROR("Failed to create thread: insufficient resources to create another thread.");
            return false;
        case EINVAL:
            VERROR("Failed to create thread: invalid settings were passed in attributes..");
            return false;
        default:
            VERROR("Failed to create thread: an unhandled error has occurred. errno=%i", result);
            return false;
        }
    }
    VDEBUG("Starting process on thread id: %#x", out_thread->thread_id);

    // Only save off the handle if not auto-detaching.
    if (!auto_detach) {
        out_thread->internal_data = platform_allocate(sizeof(u64), false);
        *(u64*)out_thread->internal_data = out_thread->thread_id;
    } else {
        // If immediately detaching, make sure the operation is a success.
        result = pthread_detach((pthread_t)out_thread->thread_id);
        if (result != 0) {
            switch (result) {
            case EINVAL:
                VERROR("Failed to detach newly-created thread: thread is not a joinable thread.");
                return false;
            case ESRCH:
                VERROR("Failed to detach newly-created thread: no thread with the id %#x could be found.", out_thread->thread_id);
                return false;
            default:
                VERROR("Failed to detach newly-created thread: an unknown error has occurred. errno=%i", result);
                return false;
            }
        }
    }

    return true;
}

void vthread_destroy(vthread* thread) {
    if (thread->internal_data) {
        vthread_cancel(thread);
    }
}

void vthread_detach(vthread* thread) {
    if (thread->internal_data) {
        i32 result = pthread_detach(*(pthread_t*)thread->internal_data);
        if (result != 0) {
            switch (result) {
            case EINVAL:
                VERROR("Failed to detach thread: thread is not a joinable thread.");
                break;
            case ESRCH:
                VERROR("Failed to detach thread: no thread with the id %#x could be found.", thread->thread_id);
                break;
            default:
                VERROR("Failed to detach thread: an unknown error has occurred. errno=%i", result);
                break;
            }
        }
        platform_free(thread->internal_data, false);
        thread->thread_id = 0;
        thread->internal_data = 0;
    }
}

void vthread_cancel(vthread* thread) {
    if (thread->internal_data) {
        i32 result = pthread_cancel(*(pthread_t*)thread->internal_data);
        if (result != 0) {
            switch (result) {
            case ESRCH:
                VERROR("Failed to cancel thread: no thread with the id %#x could be found.", thread->thread_id);
                break;
            default:
                VERROR("Failed to cancel thread: an unknown error has occurred. errno=%i", result);
                break;
            }
        }
        platform_free(thread->internal_data, false);
        thread->internal_data = 0;
        thread->thread_id = 0;
    }
}

b8 vthread_is_active(vthread* thread) {
    // TODO: Find a better way to verify this.
    return thread->internal_data != 0;
}

void vthread_sleep(vthread* thread, u64 ms) {
    platform_sleep(ms);
}

b8 vthread_wait(vthread* thread) {
    if (thread && thread->internal_data) {
        i32 result = pthread_join(*(pthread_t*)thread->internal_data, 0);
        // When a thread is joined, its lifecycle ends.
        platform_free(thread->internal_data, false);
        thread->internal_data = 0;
        thread->thread_id = 0;
        if (result == 0) {
            return true;
        }
    }
    return false;
}

b8 vthread_wait_timeout(vthread* thread, u64 wait_ms) {
    if (thread && thread->internal_data) {
        VWARN("vthread_wait_timeout - timeout not supported on this platform.");
        // LEFTOFF: Need a wait/notify loop to support timeout.
        i32 result = pthread_join(*(pthread_t*)thread->internal_data, 0);
        // When a thread is joined, its lifecycle ends.
        platform_free(thread->internal_data, false);
        thread->internal_data = 0;
        thread->thread_id = 0;
        if (result == 0) {
            return true;
        }
    }
    return false;
}

u64 platform_current_thread_id(void) {
    return (u64)pthread_self();
}
// NOTE: End threads.

// NOTE: Begin mutexes
b8 vmutex_create(vmutex* out_mutex) {
    if (!out_mutex) {
        return false;
    }

    // Initialize
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_t mutex;
    i32 result = pthread_mutex_init(&mutex, &mutex_attr);
    if (result != 0) {
        VERROR("Mutex creation failure!");
        return false;
    }

    // Save off the mutex handle.
    out_mutex->internal_data = platform_allocate(sizeof(pthread_mutex_t), false);
    *(pthread_mutex_t*)out_mutex->internal_data = mutex;

    return true;
}

void vmutex_destroy(vmutex* mutex) {
    if (mutex) {
        i32 result = pthread_mutex_destroy((pthread_mutex_t*)mutex->internal_data);
        switch (result) {
        case 0:
            // VTRACE("Mutex destroyed.");
            break;
        case EBUSY:
            VERROR("Unable to destroy mutex: mutex is locked or referenced.");
            break;
        case EINVAL:
            VERROR("Unable to destroy mutex: the value specified by mutex is invalid.");
            break;
        default:
            VERROR("An handled error has occurred while destroy a mutex: errno=%i", result);
            break;
        }

        platform_free(mutex->internal_data, false);
        mutex->internal_data = 0;
    }
}

b8 vmutex_lock(vmutex* mutex) {
    if (!mutex) {
        return false;
    }
    // Lock
    i32 result = pthread_mutex_lock((pthread_mutex_t*)mutex->internal_data);
    switch (result) {
    case 0:
        // Success, everything else is a failure.
        // VTRACE("Obtained mutex lock.");
        return true;
    case EOWNERDEAD:
        VERROR("Owning thread terminated while mutex still active.");
        return false;
    case EAGAIN:
        VERROR("Unable to obtain mutex lock: the maximum number of recursive mutex locks has been reached.");
        return false;
    case EBUSY:
        VERROR("Unable to obtain mutex lock: a mutex lock already exists.");
        return false;
    case EDEADLK:
        VERROR("Unable to obtain mutex lock: a mutex deadlock was detected.");
        return false;
    default:
        VERROR("An handled error has occurred while obtaining a mutex lock: errno=%i", result);
        return false;
    }
}

b8 vmutex_unlock(vmutex* mutex) {
    if (!mutex) {
        return false;
    }
    if (mutex->internal_data) {
        i32 result = pthread_mutex_unlock((pthread_mutex_t*)mutex->internal_data);
        switch (result) {
        case 0:
            // VTRACE("Freed mutex lock.");
            return true;
        case EOWNERDEAD:
            VERROR("Unable to unlock mutex: owning thread terminated while mutex still active.");
            return false;
        case EPERM:
            VERROR("Unable to unlock mutex: mutex not owned by current thread.");
            return false;
        default:
            VERROR("An handled error has occurred while unlocking a mutex lock: errno=%i", result);
            return false;
        }
    }

    return false;
}
// NOTE: End mutexes

b8 vsemaphore_create(vsemaphore* out_semaphore, u32 max_count, u32 start_count) {
    if (!out_semaphore) {
        return false;
    }

    char name_buf[20] = {0};
    string_format_unsafe(name_buf, "/VIRTUNITY_job_sem_%u", semaphore_id);
    semaphore_id++;

    out_semaphore->internal_data = vallocate(sizeof(nix_semaphore_internal), MEMORY_TAG_ENGINE);
    nix_semaphore_internal* internal = out_semaphore->internal_data;

    if ((internal->semaphore = sem_open(name_buf, O_CREAT, 0664, 1)) == SEM_FAILED) {
        VERROR("Failed to open semaphore");
        return false;
    }
    internal->name = string_duplicate(name_buf);

    return true;
}

void vsemaphore_destroy(vsemaphore* semaphore) {
    if (!semaphore) {
        return;
    }

    nix_semaphore_internal* internal = semaphore->internal_data;
    if (sem_close(internal->semaphore) == -1) {
        VERROR("Failed to close semaphore.");
    }

    if (sem_unlink(internal->name) == -1) {
        VERROR("Failed to unlink semaphore");
    }

    string_free(internal->name);
    vfree(semaphore->internal_data, sizeof(nix_semaphore_internal), MEMORY_TAG_ENGINE);
    semaphore->internal_data = 0;
}

b8 vsemaphore_signal(vsemaphore* semaphore) {
    if (!semaphore || !semaphore->internal_data) {
        return false;
    }

    nix_semaphore_internal* internal = semaphore->internal_data;
    if (sem_post(internal->semaphore) != 0) {
        VERROR("Semaphore failed to post!");
        return false;
    }

    return true;
}

/**
 * Decreases the semaphore count by 1. If the count reaches 0, the
 * semaphore is considered unsignaled and this call blocks until the
 * semaphore is signaled by vsemaphore_signal.
 */
b8 vsemaphore_wait(vsemaphore* semaphore, u64 timeout_ms) {
    if (!semaphore || !semaphore->internal_data) {
        return false;
    }

    nix_semaphore_internal* internal = semaphore->internal_data;
    // TODO: handle timeout value using sem_timedwait()
    if (sem_wait(internal->semaphore) != 0) {
        VERROR("Semaphore failed to wait!");
        return false;
    }

    return true;
}

b8 platform_dynamic_library_load(const char* name, dynamic_library* out_library) {
    if (!out_library) {
        return false;
    }
    vzero_memory(out_library, sizeof(dynamic_library));
    if (!name) {
        return false;
    }

    char filename[260]; // NOTE: same as Windows, for now.
    vzero_memory(filename, sizeof(char) * 260);

    const char* extension = platform_dynamic_library_extension();
    const char* prefix = platform_dynamic_library_prefix();

    string_format_unsafe(filename, "%s%s%s", prefix, name, extension);

    void* library = dlopen(filename, RTLD_NOW); // "libtestbed_lib_loaded.dylib"
    if (!library) {
        VERROR("Error opening library: %s", dlerror());
        return false;
    }

    out_library->name = string_duplicate(name);
    out_library->filename = string_duplicate(filename);

    out_library->internal_data_size = 8;
    out_library->internal_data = library;

    out_library->functions = darray_create(dynamic_library_function);

    return true;
}

b8 platform_dynamic_library_unload(dynamic_library* library) {
    if (!library) {
        return false;
    }

    if (!library->internal_data) {
        return false;
    }

    i32 result = dlclose(library->internal_data);
    if (result != 0) { // Opposite of Windows, 0 means success.
        return false;
    }
    library->internal_data = 0;

    if (library->name) {
        u64 length = string_length(library->name);
        vfree((void*)library->name, sizeof(char) * (length + 1), MEMORY_TAG_STRING);
    }

    if (library->filename) {
        u64 length = string_length(library->filename);
        vfree((void*)library->filename, sizeof(char) * (length + 1), MEMORY_TAG_STRING);
    }

    if (library->functions) {
        u32 count = darray_length(library->functions);
        for (u32 i = 0; i < count; ++i) {
            dynamic_library_function* f = &library->functions[i];
            if (f->name) {
                u64 length = string_length(f->name);
                vfree((void*)f->name, sizeof(char) * (length + 1), MEMORY_TAG_STRING);
            }
        }

        darray_destroy(library->functions);
        library->functions = 0;
    }

    vzero_memory(library, sizeof(dynamic_library));

    return true;
}

void* platform_dynamic_library_load_function(const char* name, dynamic_library* library) {
    if (!name || !library) {
        return false;
    }

    if (!library->internal_data) {
        return false;
    }

    void* f_addr = dlsym(library->internal_data, name);
    if (!f_addr) {
        return false;
    }

    dynamic_library_function f = {0};
    f.pfn = f_addr;
    f.name = string_duplicate(name);
    darray_push(library->functions, f);

    return f_addr;
}

#endif
