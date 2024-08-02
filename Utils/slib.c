#include "slib.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

slib slib_Load(slib_path libraryName) {
    slib lib = { 0 };

    if (!libraryName) {
        fprintf(stderr, "SharedLibrary.c: Failed to load shared library: libraryName is NULL\n");
        return lib;
    }

#ifdef _WIN32
    lib.handle = LoadLibrary(libraryName);
    if (!lib.handle) {
        fprintf(stderr, "SharedLibrary.c: Failed to load shared library: %s\n", libraryName);
    }
#else
    lib.handle = dlopen(libraryName, RTLD_LAZY);
    if (!lib.handle) {
        fprintf(stderr, "SharedLibrary.c: Failed to load shared library: %s\n", libraryName);
        fprintf(stderr, "Error: %s\n", dlerror());
    }
#endif

    return lib;
}

slib_pfn slib_Getpfn(slib* lib, slib_pfnname functionName){
    if (!lib || !lib->handle || !functionName) {
        fprintf(stderr, "SharedLibrary.c: Invalid arguments: lib or functionName is NULL\n");
        return NULL;
    }

#ifdef _WIN32
    void *funcPtr = (void *)GetProcAddress((HMODULE)lib->handle, functionName);
#else
    void *funcPtr = dlsym(lib->handle, functionName);
#endif

    if (!funcPtr) {
        fprintf(stderr, "SharedLibrary.c: Failed to load function pointer: %s\n", functionName);
    }

    return funcPtr;
}

void slib_Unload(slib *lib) {
    if (lib && lib->handle) {
#ifdef _WIN32
        if (!FreeLibrary((HMODULE)lib->handle)) {
            fprintf(stderr, "SharedLibrary.c: Failed to unload library: error code %lu\n", GetLastError());
        }
#else
        if (dlclose(lib->handle) != 0) {
            fprintf(stderr, "SharedLibrary.c: Failed to unload library: %s\n", dlerror());
        }
#endif
        lib->handle = NULL;
    }
}