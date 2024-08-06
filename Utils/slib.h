#ifndef slib_H
#define slib_H
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    #define slib_suffix ".dll"
#else
    #define slib_suffix ".so"
#endif

#if defined(__MINGW32__) || defined(__MINGW64__) || defined(__GNUC__) || defined(__GNUG__)
    #define slib_prefix "lib"
#else
    #define slib_prefix ""
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

// Load shared library
slib slib_Load(slib_path libraryName, bool add_prefix, bool add_suffix);

// Get a function pointer from the shared library
void* slib_Getpfn(slib* lib, slib_pfnname functionName);

// Unload a shared library
void slib_Unload(slib* lib);

#ifdef __cplusplus
}
#endif

#endif// slib_H