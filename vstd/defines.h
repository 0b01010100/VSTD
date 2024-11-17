#pragma once

 // String types.

/** @brief string */
typedef char* s;

/** @brief const string */
typedef const char* cs;

// Unsigned int types.

/** @brief Unsigned 8-bit integer */
typedef unsigned char u8;

/** @brief Unsigned 16-bit integer */
typedef unsigned short u16;

/** @brief Unsigned 32-bit integer */
typedef unsigned int u32;

/** @brief Unsigned 64-bit integer */
typedef unsigned long long u64;

// Signed int types.

/** @brief Signed 8-bit integer */
typedef signed char i8;

/** @brief Signed 16-bit integer */
typedef signed short i16;

/** @brief Signed 32-bit integer */
typedef signed int i32;

/** @brief Signed 64-bit integer */
typedef signed long long i64;

// Floating point types

/** @brief 32-bit floating point number */
typedef float f32;

/** @brief 64-bit floating point number */
typedef double f64;

// Boolean types

/** @brief 32-bit boolean type, used for APIs which require it */
typedef int b32;

/** @brief 8-bit boolean type */
typedef _Bool b8;

/** @brief 8-bit bool */
typedef union vb8 {
  u8 data;
  struct {
    u8 b0 : 1;  // bit 0
    u8 b1 : 1;  // bit 1
    u8 b2 : 1;  // bit 2
    u8 b3 : 1;  // bit 3
    u8 b4 : 1;  // bit 4
    u8 b5 : 1;  // bit 5
    u8 b6 : 1;  // bit 6
    u8 b7 : 1;  // bit 7
  };
} vb8;

/** @brief 16-bit bool */
typedef union vb16 {
  u16 data;
  struct {
    u16 b0  : 1;  // bit 0
    u16 b1  : 1;  // bit 1
    u16 b2  : 1;  // bit 2
    u16 b3  : 1;  // bit 3
    u16 b4  : 1;  // bit 4
    u16 b5  : 1;  // bit 5
    u16 b6  : 1;  // bit 6
    u16 b7  : 1;  // bit 7
    u16 b8  : 1;  // bit 8
    u16 b9  : 1;  // bit 9
    u16 b10 : 1;  // bit 10
    u16 b11 : 1;  // bit 11
    u16 b12 : 1;  // bit 12
    u16 b13 : 1;  // bit 13
    u16 b14 : 1;  // bit 14
    u16 b15 : 1;  // bit 15
  };
} vb16;

/** @brief 32-bit bool */
typedef union vb32 {
  u32 data;
  struct {
    u32 b0  : 1;  // bit 0
    u32 b1  : 1;  // bit 1
    u32 b2  : 1;  // bit 2
    u32 b3  : 1;  // bit 3
    u32 b4  : 1;  // bit 4
    u32 b5  : 1;  // bit 5
    u32 b6  : 1;  // bit 6
    u32 b7  : 1;  // bit 7
    u32 b8  : 1;  // bit 8
    u32 b9  : 1;  // bit 9
    u32 b10 : 1;  // bit 10
    u32 b11 : 1;  // bit 11
    u32 b12 : 1;  // bit 12
    u32 b13 : 1;  // bit 13
    u32 b14 : 1;  // bit 14
    u32 b15 : 1;  // bit 15
    u32 b16 : 1;  // bit 16
    u32 b17 : 1;  // bit 17
    u32 b18 : 1;  // bit 18
    u32 b19 : 1;  // bit 19
    u32 b20 : 1;  // bit 20
    u32 b21 : 1;  // bit 21
    u32 b22 : 1;  // bit 22
    u32 b23 : 1;  // bit 23
    u32 b24 : 1;  // bit 24
    u32 b25 : 1;  // bit 25
    u32 b26 : 1;  // bit 26
    u32 b27 : 1;  // bit 27
    u32 b28 : 1;  // bit 28
    u32 b29 : 1;  // bit 29
    u32 b30 : 1;  // bit 30
    u32 b31 : 1;  // bit 31
  };
} vb32;

/** @brief 64-bit bool */
typedef union vb64 {
    u64 data;
    struct {
        u64 b0  : 1;  // bit 0
        u64 b1  : 1;  // bit 1
        u64 b2  : 1;  // bit 2
        u64 b3  : 1;  // bit 3
        u64 b4  : 1;  // bit 4
        u64 b5  : 1;  // bit 5
        u64 b6  : 1;  // bit 6
        u64 b7  : 1;  // bit 7
        u64 b8  : 1;  // bit 8
        u64 b9  : 1;  // bit 9
        u64 b10 : 1;  // bit 10
        u64 b11 : 1;  // bit 11
        u64 b12 : 1;  // bit 12
        u64 b13 : 1;  // bit 13
        u64 b14 : 1;  // bit 14
        u64 b15 : 1;  // bit 15
        u64 b16 : 1;  // bit 16
        u64 b17 : 1;  // bit 17
        u64 b18 : 1;  // bit 18
        u64 b19 : 1;  // bit 19
        u64 b20 : 1;  // bit 20
        u64 b21 : 1;  // bit 21
        u64 b22 : 1;  // bit 22
        u64 b23 : 1;  // bit 23
        u64 b24 : 1;  // bit 24
        u64 b25 : 1;  // bit 25
        u64 b26 : 1;  // bit 26
        u64 b27 : 1;  // bit 27
        u64 b28 : 1;  // bit 28
        u64 b29 : 1;  // bit 29
        u64 b30 : 1;  // bit 30
        u64 b31 : 1;  // bit 31
        u64 b32 : 1;  // bit 32
        u64 b33 : 1;  // bit 33
        u64 b34 : 1;  // bit 34
        u64 b35 : 1;  // bit 35
        u64 b36 : 1;  // bit 36
        u64 b37 : 1;  // bit 37
        u64 b38 : 1;  // bit 38
        u64 b39 : 1;  // bit 39
        u64 b40 : 1;  // bit 40
        u64 b41 : 1;  // bit 41
        u64 b42 : 1;  // bit 42
        u64 b43 : 1;  // bit 43
        u64 b44 : 1;  // bit 44
        u64 b45 : 1;  // bit 45
        u64 b46 : 1;  // bit 46
        u64 b47 : 1;  // bit 47
        u64 b48 : 1;  // bit 48
        u64 b49 : 1;  // bit 49
        u64 b50 : 1;  // bit 50
        u64 b51 : 1;  // bit 51
        u64 b52 : 1;  // bit 52
        u64 b53 : 1;  // bit 53
        u64 b54 : 1;  // bit 54
        u64 b55 : 1;  // bit 55
        u64 b56 : 1;  // bit 56
        u64 b57 : 1;  // bit 57
        u64 b58 : 1;  // bit 58
        u64 b59 : 1;  // bit 59
        u64 b60 : 1;  // bit 60
        u64 b61 : 1;  // bit 61
        u64 b62 : 1;  // bit 62
        u64 b63 : 1;  // bit 63
    };
} vb64;

/** @brief A range, typically of memory */
typedef struct range {
    /** @brief The offset in bytes. */
    u64 offset;
    /** @brief The size in bytes. */
    u64 size;
} range;

/** @brief A range, typically of memory */
typedef struct range32 {
    /** @brief The offset in bytes. */
    i32 offset;
    /** @brief The size in bytes. */
    i32 size;
} range32;
// Properly define static assertions.
#if defined(__clang__) || defined(__GNUC__)
/** @brief Static assertion */
#define STATIC_ASSERT _Static_assert
#else

/** @brief Static assertion */
#define STATIC_ASSERT static_assert
#endif

// Ensure all types are of the correct size.

/** @brief Assert u8 to be 1 byte.*/
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");

/** @brief Assert u16 to be 2 bytes.*/
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");

/** @brief Assert u32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");

/** @brief Assert u64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

/** @brief Assert i8 to be 1 byte.*/
STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");

/** @brief Assert i16 to be 2 bytes.*/
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");

/** @brief Assert i32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");

/** @brief Assert i64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

/** @brief Assert f32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");

/** @brief Assert f64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

/** @brief True.*/
#define true 1

/** @brief False. */
#define false 0

/**
 * @brief Any id set to this should be considered invalid,
 * and not actually pointing to a real object.
 */
#define INVALID_ID_U64 18446744073709551615UL
#define INVALID_ID 4294967295U
#define INVALID_ID_U16 65535U
#define INVALID_ID_U8 255U

#define U64_MAX 18446744073709551615UL
#define U32_MAX 4294967295U
#define U16_MAX 65535U
#define U8_MAX 255U
#define U64_MIN 0UL
#define U32_MIN 0U
#define U16_MIN 0U
#define U8_MIN 0U

#define I8_MAX 127
#define I16_MAX 32767
#define I32_MAX 2147483647
#define I64_MAX 9223372036854775807L
#define I8_MIN (-I8_MAX - 1)
#define I16_MIN (-I16_MAX - 1)
#define I32_MIN (-I32_MAX - 1)
#define I64_MIN (-I64_MAX - 1)

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define VPLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define VPLATFORM_LINUX 1
#if defined(__ANDROID__)
#define VPLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
// Catch anything not caught by the above.
#define VPLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
// Posix
#define VPLATFORM_POSIX 1
#elif __APPLE__
// Apple platforms
#define VPLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#define VPLATFORM_IOS 1
#define VPLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define VPLATFORM_IOS 1
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif

#ifdef VEXPORT
// Exports
#ifdef _MSC_VER
#define VAPI __declspec(dllexport)
#else
#define VAPI __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
/** @brief Import/export qualifier */
#define VAPI __declspec(dllimport)
#else
/** @brief Import/export qualifier */
#define VAPI
#endif
#endif

#ifdef _DEBUG
#define V_DEBUG
#else
#define V_RELEASE
#endif

/**
 * @brief Clamps value to a range of min and max (inclusive).
 * @param value The value to be clamped.
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @returns The clamped value.
 */
#define VCLAMP(value, min, max) ((value <= min) ? min : (value >= max) ? max \
                                                                       : value)

// Inlining
#if defined(__clang__) || defined(__gcc__)
/** @brief Inline qualifier */
#define VINLINE __attribute__((always_inline)) inline

/** @brief No-inline qualifier */
#define VNOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)

/** @brief Inline qualifier */
#define VINLINE __forceinline

/** @brief No-inline qualifier */
#define VNOINLINE __declspec(noinline)
#else

/** @brief Inline qualifier */
#define VINLINE static inline

/** @brief No-inline qualifier */
#define VNOINLINE
#endif

// Deprecation
#if defined(__clang__) || defined(__gcc__)
/** @brief Mark something (i.e. a function) as deprecated. */
#define VDEPRECATED(message) __attribute__((deprecated(message)))
#elif defined(_MSC_VER)
/** @brief Mark something (i.e. a function) as deprecated. */
#define VDEPRECATED(message) __declspec(deprecated(message))
#else
#error "Unsupported compiler - don't know how to define deprecations!"
#endif

/** @brief Gets the number of bytes from amount of gibibytes (GiB) (1024*1024*1024) */
#define GIBIBYTES(amount) ((amount) * 1024ULL * 1024ULL * 1024ULL)
/** @brief Gets the number of bytes from amount of mebibytes (MiB) (1024*1024) */
#define MEBIBYTES(amount) ((amount) * 1024ULL * 1024ULL)
/** @brief Gets the number of bytes from amount of kibibytes (KiB) (1024) */
#define KIBIBYTES(amount) ((amount) * 1024ULL)

/** @brief Gets the number of bytes from amount of gigabytes (GB) (1000*1000*1000) */
#define GIGABYTES(amount) ((amount) * 1000ULL * 1000ULL * 1000ULL)
/** @brief Gets the number of bytes from amount of megabytes (MB) (1000*1000) */
#define MEGABYTES(amount) ((amount) * 1000ULL * 1000ULL)
/** @brief Gets the number of bytes from amount of kilobytes (KB) (1000) */
#define KILOBYTES(amount) ((amount) * 1000ULL)

VINLINE u64 get_aligned(u64 operand, u64 granularity) {
    return ((operand + (granularity - 1)) & ~(granularity - 1));
}

VINLINE range get_aligned_range(u64 offset, u64 size, u64 granularity) {
    return (range){get_aligned(offset, granularity), get_aligned(size, granularity)};
}

#define VMIN(x, y) (x < y ? x : y)
#define VMAX(x, y) (x > y ? x : y)
