#pragma once
#include "defines.h"

/**
 * @brief Represents a basic clock, which can be used to track time
 * deltas in the system.
 */
typedef struct vclock {
    /** @brief The start time of the clock. If never started, this is 0. */
    f64 start_time;
    /** @brief The amount of time in seconds that have elapsed since this
     * clock was started. Only accurate after a call to clock_update. 
     */
    f64 elapsed;
} vclock;

/** 
 * @brief Updates the provided clock. Should be called just before checking elapsed time.
 * Has no effect on non-started clocks.
 * @param clock A pointer to the clock to be updated.
 */
VAPI void vclock_update(vclock* clock);

/** 
 * @brief Starts the provided clock. Resets elapsed time.
 * @param clock A pointer to the clock to be started.
 */
VAPI void vclock_start(vclock* clock);

/** 
 * @brief Stops the provided clock. Does not reset elapsed time.
 * @param clock A pointer to the clock to be stopped.
 */
VAPI void vclock_stop(vclock* clock);
