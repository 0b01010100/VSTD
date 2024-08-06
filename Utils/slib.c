#include "slib.h"
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

slib slib_Load(slib_path libraryName, bool add_prefix, bool add_suffix)
{
    if(!libraryName) 
    {
        fprintf(stderr, "slib.c: NameLess library: \n");
        
    }
    slib lib = { 0 };
    char * temp = NULL;
    if(add_prefix || add_suffix){

        size_t temp_len = strlen(libraryName);
        size_t prefix_len = ((slib_prefix == NULL)? 0 : strlen(slib_prefix));
        size_t suffix_len = ((slib_suffix == NULL)? 0 : strlen(slib_suffix));

        temp = malloc((prefix_len + temp_len + suffix_len + 1) * sizeof(char));
        if(!temp) 
        {
            fprintf(stderr, "slib.c: Failed malloc: \n");
            return lib;
        }
        temp[prefix_len + temp_len + suffix_len] = '\0';

        if(add_prefix){
            strncpy(temp, slib_prefix, prefix_len);
        }

        strncpy(temp + prefix_len, libraryName, temp_len);

        if(add_suffix){
            strncpy(temp + prefix_len + temp_len, slib_suffix, suffix_len);
        }
        
        
    }else{
        temp = (char*)libraryName;
    }

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    lib.handle = LoadLibrary(temp);
    if (!lib.handle) {
        fprintf(stderr, "slib.c: Failed to load shared library: %s\n", temp);
    }
#else
    lib.handle = dlopen(temp, RTLD_LAZY);
    if (!lib.handle) {
        fprintf(stderr, "slib.c: Failed to load shared library: %s\n", temp);
        fprintf(stderr, "Error: %s\n", dlerror());
    }
#endif
    if(add_prefix || add_suffix) free(temp);

    return lib;
}

void* slib_Getpfn(slib* lib, slib_pfnname functionName){
    if (!lib || !lib->handle || !functionName) {
        fprintf(stderr, "slib.c: Invalid arguments: lib or functionName is NULL\n");
        return NULL;
    }

#ifdef _WIN32
    void *funcPtr = (void *)GetProcAddress((HMODULE)lib->handle, functionName);
#else
    void *funcPtr = dlsym(lib->handle, functionName);
#endif

    if (!funcPtr) {
        fprintf(stderr, "slib.c: Failed to load function pointer: %s\n", functionName);
    }

    return funcPtr;
}

void slib_Unload(slib *lib) {
    if (lib && lib->handle) {
#ifdef _WIN32
        if (!FreeLibrary((HMODULE)lib->handle)) {
            fprintf(stderr, "slib.c: Failed to unload library: error code %lu\n", GetLastError());
        }
#else
        if (dlclose(lib->handle) != 0) {
            fprintf(stderr, "slib.c: Failed to unload library: %s\n", dlerror());
        }
#endif
        lib->handle = NULL;
    }
}