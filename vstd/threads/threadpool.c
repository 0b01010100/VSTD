#include "threadpool.h"

#include "logger.h"
#include "memory/vmemory.h"
#include "worker_thread.h"

b8 threadpool_create(u32 thread_count, threadpool* out_pool) {
    if (!thread_count || !out_pool) {
        VERROR("threadpool_create requires at least 1 thread and a valid pointer to hold the created pool.");
        return false;
    }

    out_pool->thread_count = thread_count;
    out_pool->threads = vallocate(sizeof(worker_thread) * thread_count, MEMORY_TAG_ARRAY);
    for (u32 i = 0; i < thread_count; ++i) {
        if (!worker_thread_create(&out_pool->threads[i])) {
            VERROR("Error creating worker thread. threadpool_create failed.");
            return false;
        }
    }

    return true;
}

void threadpool_destroy(threadpool* pool) {
    if (pool) {
        if (pool->threads) {
            for (u32 i = 0; i < pool->thread_count; ++i) {
                worker_thread_destroy(&pool->threads[i]);
            }

            vfree(pool->threads, sizeof(worker_thread) * pool->thread_count, MEMORY_TAG_ARRAY);
            pool->threads = 0;
        }
        pool->thread_count = 0;
    }
}

b8 threadpool_wait(threadpool* pool) {
    if (!pool) {
        VERROR("threadpool_wait requires a valid pointer to a thread pool.");
        return false;
    }

    b8 success = true;
    for (u32 i = 0; i < pool->thread_count; ++i) {
        if (!worker_thread_wait(&pool->threads[i])) {
            VERROR("Failed to wait for worker thread in thread pool. See logs for details.");
            success = false;
        }
        VTRACE("Worker thread wait complete.");
    }

    if (!success) {
        VERROR("There was an error waiting for the threadpool. See logs for details.");
    }

    VTRACE("Done waiting on all threads");

    return success;
}
