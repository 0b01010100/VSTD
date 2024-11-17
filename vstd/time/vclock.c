#include "time/vclock.h"

#include "platform/platform.h"

void vclock_update(vclock* clock) {
    if (clock->start_time != 0) {
        clock->elapsed = platform_get_absolute_time() - clock->start_time;
    }
}

void vclock_start(vclock* clock) {
    clock->start_time = platform_get_absolute_time();
    clock->elapsed = 0;
}

void vclock_stop(vclock* clock) {
    clock->start_time = 0;
}