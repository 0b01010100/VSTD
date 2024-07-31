#include "SharedLibrary.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

SharedLibrary SharedLibrary_Load(SharedLibrary_Path libraryName) {
    SharedLibrary lib = { 0 };

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

SharedLibrary_Function SharedLibrary_GetFunctionPtr(SharedLibrary *lib, const SharedLibrary_Function_Name functionName) {
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

void SharedLibrary_Unload(SharedLibrary *lib) {
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