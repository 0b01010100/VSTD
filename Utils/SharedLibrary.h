#ifndef SharedLibrary_H
#define SharedLibrary_H

#ifdef __cplusplus
extern "C" {
#endif


// Pointer to a shared library function
typedef void* SharedLibrary_Function;

typedef char* SharedLibrary_Function_Name;

// Path of the shard library file
typedef char * SharedLibrary_Path;

// Platform Specific handle to a Shared Library
typedef struct SharedLibrary {
    void* handle;  
} SharedLibrary;



// Load shared library
SharedLibrary SharedLibrary_Load(SharedLibrary_Path libraryName);

// Unload a shared library
void SharedLibrary_Unload(SharedLibrary* lib);

// Get a function pointer from the shared library
SharedLibrary_Function SharedLibrary_GetFunctionPtr(SharedLibrary* lib, const SharedLibrary_Function_Name functionName);

#ifdef __cplusplus
}
#endif

#endif// SharedLibrary_H