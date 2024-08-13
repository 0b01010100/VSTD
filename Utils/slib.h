/**
 * @file slib.h
 * @brief A library for loading and managing shared libraries (dynamic link libraries).
 * 
 * This header provides functionality for loading shared libraries, retrieving function pointers from them, and unloading them.
 * It is designed to work across different platforms with appropriate suffix and prefix handling.
 */
#ifndef slib_H
#define slib_H
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

// Define shared library suffix based on the platform
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    #define SLIB_SUFFIX ".dll"
#elif defined(__APPLE__)
    #define SLIB_SUFFIX ".dylib"
#elif defined(__unix__) || defined(__unix)
    #define SLIB_SUFFIX ".so"
#else
    #error "Unsupported platform"
#endif

// Define shared library prefix based on the compiler
#if defined(__MINGW32__) || defined(__MINGW64__) || defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    #define SLIB_PREFIX "lib"
#else//others
    #define SLIB_PREFIX ""
#endif

// Pointer to a shared library function
#define slib_pfn void*

typedef const char* slib_pfnname;

// Path of the shard library file
typedef const char* slib_path;

// Platform Specific handle to a Shared Library
typedef struct slib {
    void* handle;  
} slib;

/**
 * @brief Load a shared library.
 * 
 * @param libraryName The name of the library to load.
 * @param add_prefix Whether to add a prefix to the library name.
 * @param add_suffix Whether to add a suffix to the library name.
 * @return An `slib` structure with the handle to the loaded library.
 */
slib slib_Load(slib_path libraryName, bool add_prefix, bool add_suffix);

/**
 * @brief Get a function pointer from the shared library.
 * 
 * @param lib Pointer to the `slib` structure representing the library.
 * @param functionName The name of the function to retrieve.
 * @return A function pointer to the requested function.
 */
slib_pfn slib_Getpfn(slib* lib, slib_pfnname functionName);

/**
 * @brief Unload a shared library.
 * 
 * @param lib Pointer to the `slib` structure representing the library to unload.
 */
void slib_Unload(slib* lib);

#ifdef __cplusplus
}
#endif

#endif// slib_H