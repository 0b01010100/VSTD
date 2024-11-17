#include "worker_thread.h"

#include "containers/queue.h"
#include "memory/vmemory.h"
#include "threads/vmutex.h"
#include "threads/vthread.h"
#include "logger.h"

typedef struct work {
    pfn_thread_start work_fn;
    void* params;
} work;

static u32 worker_thread_loop(void* params) {
    worker_thread* thread = params;

    while (true) {
        vmutex_lock(&thread->queue_mutex);
        if (thread->work_queue.element_count == 0) {
            vmutex_unlock(&thread->queue_mutex);
            break;
        }
        work w;
        if (!queue_pop(&thread->work_queue, &w)) {
            VERROR("Failed to pop work from work queue.");
            vmutex_unlock(&thread->queue_mutex);
            return 0;
        }
        vmutex_unlock(&thread->queue_mutex);
        w.work_fn(w.params);
    }

    VTRACE("Worker thread work complete.");

    return 1;
}

b8 worker_thread_create(worker_thread* out_thread) {
    if (!out_thread) {
        return false;
    }

    if (!queue_create(&out_thread->work_queue, sizeof(work))) {
        VERROR("Failed to create internal work queue for worker thread.");
        return false;
    }
    if (!vmutex_create(&out_thread->queue_mutex)) {
        VERROR("Failed to create internal work queue mutex for worker thread.");
        return false;
    }

    return true;
}

void worker_thread_destroy(worker_thread* thread) {
    if (!thread) {
        return;
    }

    queue_destroy(&thread->work_queue);
    vmutex_destroy(&thread->queue_mutex);
    vthread_destroy(&thread->thread);
    vzero_memory(thread, sizeof(worker_thread));
}

b8 worker_thread_add(worker_thread* thread, pfn_thread_start work_fn, void* work_params) {
    if (!thread || !work_fn) {
        VERROR("worker_thread_add requires valid pointers to a worker_thread and a work function pointer.");
        return false;
    }

    vmutex_lock(&thread->queue_mutex);
    work w;
    w.work_fn = work_fn;
    w.params = work_params;
    if (!queue_push(&thread->work_queue, &w)) {
        VERROR("Failed to push work into queue.");
        vmutex_unlock(&thread->queue_mutex);
        return false;
    }
    vmutex_unlock(&thread->queue_mutex);

    return true;
}

b8 worker_thread_start(worker_thread* thread) {
    if (!thread) {
        return false;
    }

    // Create the internal thread if need be.
    if (!vthread_create(worker_thread_loop, thread, false, &thread->thread)) {
        VERROR("Worker thread internal thread creation failed.");
        return false;
    }

    return true;
}

b8 worker_thread_wait(worker_thread* thread) {
    return vthread_wait(&thread->thread);
}
