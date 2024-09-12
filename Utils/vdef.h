#ifndef __vdef__
#define __vdef__

#include <stddef.h>
#include <assert.h>

// Compiler-specific macros
#if defined(__GNUC__)
    #define VCOMPILER_GNU 1
#elif defined(_MSC_VER)
    #define VCOMPILER_MSVC 1
#elif defined(__clang__)
    #define VCOMPILER_CLANG 1
#elif defined(__INTEL_COMPILER) || defined(__ICL)
    #define VCOMPILER_INTEL 1
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
    #define VCOMPILER_SUN 1
#elif defined(__PGI) || defined(__NVCOMPILER)
    #define VCOMPILER_PGI 1
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
    #define VCOMPILER_ARM 1
#elif defined(__ibmxl__) || defined(__xlC__)
    #define VCOMPILER_IBM_XL 1
#elif defined(__HP_cc) || defined(__HP_aCC)
    #define VCOMPILER_HP_ACC 1
#elif defined(__DMC__)
    #define VCOMPILER_DMC 1
#elif defined(__POCC__)
    #define VCOMPILER_PELLES 1
#elif defined(__TINYC__)
    #define VCOMPILER_TINYC 1
#elif defined(__BORLANDC__) || defined(__CODEGEARC__)
    #define VCOMPILER_BORLAND 1
#elif defined(__KEIL__)
    #define VCOMPILER_KEIL 1
#elif defined(__IAR_SYSTEMS_ICC__)
    #define VCOMPILER_IAR 1
#elif defined(__ZIG__)
    #define VCOMPILER_ZIG 1
#elif defined(__ghs__)
    #define VCOMPILER_GHS 1
#elif defined(__MVS__) || defined(__HOS_MVS__)
    #define VCOMPILER_IBM_Z 1
#else
    #define VCOMPILER_UNKNOWN 1
#endif

// C standard versions
#ifdef __STDC_VERSION__
    #if __STDC_VERSION__ >= 202311L
        #define vC23 1
    #elif __STDC_VERSION__ >= 202000L
        #define vC20 1
    #elif __STDC_VERSION__ >= 201710L
        #define vC17 1
    #elif __STDC_VERSION__ >= 201112L
        #define vC11 1
    #elif __STDC_VERSION__ >= 199901L
        #define vC99 1
    #elif __STDC_VERSION__ >= 199409L
        #define vC95 1
    #endif
#else
    #define vC89 1
#endif

// Compiler attributes
#if defined(VCOMPILER_GNU) || defined(VCOMPILER_CLANG)
    #define VSET_SYMBOL(symbol) asm(symbol)
    #define vpacked __attribute__((packed))
    #define valign(x) __attribute__((aligned(x)))
    #define vinline __inline__
    #define vnoreturn __attribute__((noreturn))
    #define vrestrict __restrict
    #if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
        #define vexport __attribute__((dllexport))
    #else
        #define vexport __attribute__((visibility("default")))
    #endif

#elif defined(VCOMPILER_MSVC)
    #define VSET_SYMBOL(symbol)
    #define vpacked
    #define valign(x) __declspec(align(x))
    #define vinline __inline
    #define vnoreturn __declspec(noreturn)
    #define vrestrict __restrict
    #define vexport __declspec(dllexport)

#elif defined(VCOMPILER_INTEL)
    #define VSET_SYMBOL(symbol)
    #define vpacked __attribute__((packed))
    #define valign(x) __attribute__((aligned(x)))
    #define vinline __inline__
    #define vnoreturn __attribute__((noreturn))
    #define vrestrict __restrict
    #define vexport __declspec(dllexport)

#elif defined(VCOMPILER_SUN) || defined(VCOMPILER_PGI) || defined(VCOMPILER_ARM) || defined(VCOMPILER_IBM_XL)
    #define VSET_SYMBOL(symbol)
    #define vpacked __attribute__((packed))
    #define valign(x) __attribute__((aligned(x)))
    #define vinline __inline__
    #define vnoreturn __attribute__((noreturn))
    #define vrestrict __restrict
    #define vexport __attribute__((visibility("default")))

#elif defined(VCOMPILER_KEIL)
    #define VSET_SYMBOL(symbol)
    #define vpacked __packed
    #define valign(x) __align(x)
    #define vinline __inline
    #define vnoreturn __declspec(noreturn)
    #define vrestrict __restrict
    #define vexport

#elif defined(VCOMPILER_IAR)
    #define VSET_SYMBOL(symbol)
    #define vpacked __packed
    #define valign(x) _Pragma("data_alignment=" #x)
    #define vinline __inline
    #define vnoreturn __noreturn
    #define vrestrict __restrict
    #define vexport

/*
#elif defined(VCOMPILER_TINYC) || 
      defined(VCOMPILER_DMC) || 
      defined(VCOMPILER_PELLES) || 
      defined(VCOMPILER_BORLAND) || 
      defined(VCOMPILER_ZIG) || 
      defined(VCOMPILER_GHS) || 
      defined(VCOMPILER_IBM_Z)
    #define VSET_SYMBOL(symbol)
    #define packed
    #define align(x)
    #define inline
    #define noreturn
    #define restrict
    #define vexport
*/
#else
    #define VSET_SYMBOL(symbol)
    #define vpacked
    #define valign(x)
    #define vinline
    #define vnoreturn
    #define vrestrict
    #define vexport
#endif

//might not work in msvc
#if defined(vC11) || defined(vC17) || defined(vC20) || defined(vC23)
    #define vtypename _Generic
    #define vtypename_(x) vtypename((x), \
        _Bool: "_Bool", \
        char: "char", \
        unsigned char: "unsigned char", \
        signed char: "char", \
        short: "short", \
        unsigned short: "unsigned short", \
        int: "int", \
        unsigned int: "unsigned int", \
        long: "long", \
        unsigned long: "unsigned long", \
        long long: "long long", \
        unsigned long long: "unsigned long long", \
        float: "float", \
        double: "double", \
        long double: "long double", \
        _Bool *: "_Bool*", \
        char *: "char*", \
        signed char *: "char*", \
        unsigned char *: "unsigned char*", \
        short *: "short*", \
        unsigned short *: "unsigned short*", \
        int *: "int*", \
        unsigned int *: "unsigned int*", \
        long *: "long*", \
        unsigned long *: "unsigned long*", \
        float *: "float*", \
        double *: "double*", \
        long double *: "long double*", \
        void *: "void*", \
        default: "unknown")
#endif

//cross compiler assert
#define VSTATIC_ASSERT(cond, msg) typedef char vstatic_assert_##msg[(cond) ? 1 : -1]

//Gets elment count of a static array
#define VARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

//Marks sections of the code that need to be implemented (TODO). Will assert.
#define vTODO(msg) assert(false && "TODO: " #msg)

#ifdef __cplusplus
    #define vextern_c extern "C"
    #define vextern_cpp extern "C++"
#else
    #define vextern_c
#endif

#endif // __vdef__
