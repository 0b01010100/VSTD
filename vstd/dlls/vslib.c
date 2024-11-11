#include "vslib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

vslib vslib_load(const char* libraryName, bool add_prefix, bool add_suffix)
{
    if(!libraryName) 
    {
        fprintf(stderr, "vslib.c: NameLess library: \n");
        return (vslib){0};
    }
    vslib lib = { 0 };
    char * temp = NULL;
    if(add_prefix || add_suffix){

        size_t temp_len = strlen(libraryName);
        size_t prefix_len = ((VSLIB_PREFIX == NULL)? 0 : strlen(VSLIB_PREFIX));
        size_t suffix_len = ((VSLIB_SUFFIX == NULL)? 0 : strlen(VSLIB_SUFFIX));

        temp = malloc((prefix_len + temp_len + suffix_len + 1) * sizeof(char));
        if(!temp) 
        {
            fprintf(stderr, "vslib.c: Failed malloc: \n");
            return lib;
        }
        temp[prefix_len + temp_len + suffix_len] = '\0';

        if(add_prefix){
            strncpy(temp, VSLIB_PREFIX, prefix_len);
        }

        strncpy(temp + prefix_len, libraryName, temp_len);

        if(add_suffix){
            strncpy(temp + prefix_len + temp_len, VSLIB_SUFFIX, suffix_len);
        }
        
        
    }else{
        temp = (char*)libraryName;
    }

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    lib.handle = LoadLibrary(temp);
    if (!lib.handle) {
        fprintf(stderr, "vslib.c: Failed to load shared library: %s\n", temp);
    }
#else
    lib.handle = dlopen(temp, RTLD_LAZY);
    if (!lib.handle) {
        fprintf(stderr, "vslib.c: Failed to load shared library: %s\n", temp);
        fprintf(stderr, "Error: %s\n", dlerror());
    }
#endif
    if(add_prefix || add_suffix) free(temp);

    return lib;
}

vslib_fn vslib_get_fn(vslib* lib, const char* fn_name){
    if (!lib || !lib->handle || !fn_name) {
        fprintf(stderr, "vslib.c: Invalid arguments: lib or function Name is NULL\n");
        return NULL;
    }

#ifdef _WIN32
    vslib_fn funcPtr = (vslib_fn)GetProcAddress((HMODULE)lib->handle, fn_name);
#else
    vslib_fn funcPtr = (vslib_fn)dlsym(lib->handle, fn_name);
#endif

    if (!funcPtr) {
        fprintf(stderr, "vslib.c: Failed to load function pointer: %s\n", fn_name);
    }

    return funcPtr;
}

void vslib_unload(vslib *lib) {
    if (lib && lib->handle) {
#ifdef _WIN32
        if (!FreeLibrary((HMODULE)lib->handle)) {
            fprintf(stderr, "vslib.c: Failed to unload library: error code %lu\n", GetLastError());
        }
#else
        if (dlclose(lib->handle) != 0) {
            fprintf(stderr, "vslib.c: Failed to unload library: %s\n", dlerror());
        }
#endif
        lib->handle = NULL;
    }
}