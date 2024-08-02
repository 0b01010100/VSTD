#ifndef slib_H
#define slib_H

#ifdef __cplusplus
extern "C" {
#endif


// Pointer to a shared library function
typedef void* slib_pfn;

typedef const char* slib_pfnname;

// Path of the shard library file
typedef char* slib_path;

// Platform Specific handle to a Shared Library
typedef struct slib {
    void* handle;  
} slib;

// Load shared library
slib slib_Load(slib_path libraryName);

// Get a function pointer from the shared library
slib_pfn slib_Getpfn(slib* lib, slib_pfnname functionName);

// Unload a shared library
void slib_Unload(slib* lib);

#ifdef __cplusplus
}
#endif

#endif// slib_H