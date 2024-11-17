#include "defines.h"

#define STATE_VECTOR_LENGTH 624
// NOTE: Changes to STATE_VECTOR_LENGTH requires a change to this as well.
#define STATE_VECTOR_M 397

typedef struct mtrand_state {
    u64 mt[STATE_VECTOR_LENGTH];
    i32 index;
} mtrand_state;

/**
 * @brief Creates a new Mersenne Twister random number generator using the provided seed.
 * @param The seed to use for the RNG.
 * @returns The state for the MT RNG.
 */
VAPI mtrand_state mtrand_create(u64 seed);

/**
 * @brief Generates a new random 64-bit unsigned integer from the given generator.
 * @param generator A pointer to the random number generator to be used.
 * @returns A newly generated 64-bit unsigned integer.
 */
VAPI u64 mtrand_generate(mtrand_state* generator);

/**
 * @brief Generates a new random 64-bit floating-point number from the given generator.
 * @param generator A pointer to the random number generator to be used.
 * @returns A newly generated 64-bit floating-point number.
 */
VAPI f64 mtrand_generate_d(mtrand_state* generator);
