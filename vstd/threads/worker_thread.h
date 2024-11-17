#pragma once

#include "containers/queue.h"
#include "threads/vmutex.h"
#include "threads/vsemaphore.h"
#include "threads/vthread.h"
#include "defines.h"

typedef struct worker_thread {
    vthread thread;
    queue work_queue;
    vmutex queue_mutex;
} worker_thread;

VAPI b8 worker_thread_create(worker_thread* out_thread);

VAPI void worker_thread_destroy(worker_thread* thread);

VAPI b8 worker_thread_add(worker_thread* thread, pfn_thread_start work_fn, void* params);

VAPI b8 worker_thread_start(worker_thread* thread);

VAPI b8 worker_thread_wait(worker_thread* thread);
