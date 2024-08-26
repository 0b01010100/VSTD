#ifndef __vdef__
#define __vdef__
#include <stddef.h>
#include <assert.h>

//bool
typedef int vbool;
#define vtrue (1)
#define vfalse (0)

//versions

#ifdef __STDC_VERSION__
    #if __STDC_VERSION__ >= 202311L
        //C23
        #define vC23 1
    #elif __STDC_VERSION__ >= 202000L
        //C20
        #define vC20 1
    #elif __STDC_VERSION__ >= 201710L
        //C17
        #define vC17 1
    #elif __STDC_VERSION__ >= 201112L
        //C11
        #define vC11 1
    #elif __STDC_VERSION__ >= 199901L
        //C98/C99
        #define vC99 1
    #endif
#endif


//generics

//oddly MSVC can supports __typeof__ but as an extenstion, so it is supported 
//in all C version even though the ideal of typeof was introduced in C23.
//define vForce to force the defining of __typeof__
#if defined(_MSC_VER) && defined(vC23) || defined(vForce)
    // Returns the type of an expression during compile time
    // https://learn.microsoft.com/en-us/cpp/c-language/typeof-c?view=msvc-170
    // @ note on MSVC intellisense will lie to you
        #define vtypeof __typeof__ 
#elif defined(vC23) && defined(__clang__) || defined(__GNUC__)
    // Returns the type of an expression during compile time
    // https://learn.microsoft.com/en-us/cpp/c-language/typeof-c?view=msvc-170
        #define vtypeof typeof
    #endif


#if defined(vC11)
    // Maps a type with a string
    // Like key value pairs(key = type, value = str)
    // https://learn.microsoft.com/en-us/cpp/c-language/generic-selection?view=msvc-170
    #define vTypeName _Generic 
#endif


/**
 * @brief Macro to mark TODO items in the code.
 * 
 * This macro is used to insert a compile-time assertion that will always fail, accompanied by
 * a custom TODO message. The assertion will trigger if the code is executed, making it clear
 * that there are parts of the code that need to be addressed or completed. This is useful for
 * marking incomplete features or reminders for future development.
 * 
 * @param msg The message to include with the TODO. This message will be concatenated with
 *            "TODO: " and included in the assertion failure message.
 * 
 * Example usage:
 * @code
 * vTODO("Implement error handling");
 * @endcode
 */
#define vTODO(msg) assert(false && "TODO: " #msg)


#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
  #ifdef _MSC_VER
    /**
     * @brief Marks a symbol for export from a DLL when using Microsoft Visual C++.
     * 
     * On Windows with MSVC, this macro uses `__declspec(dllexport)` to export the symbol
     * from a DLL. This ensures that the function or variable can be used by other modules or 
     * executables that link against the DLL.
     * 
     * Example usage:
     * @code
     * VEXPORT void my_function();
     * @endcode
     */
    #define VEXPORT __declspec(dllexport)
  #else//g++/gcc and other
    /**
     * @brief Marks a symbol for export from a DLL when using non-MSVC compilers on Windows.
     * 
     * On Windows with compilers other than MSVC, this macro uses `__attribute__((dllexport))` 
     * to export the symbol from a DLL. This ensures that the function or variable is visible 
     * to other modules or executables linking against the DLL.
     * 
     * Example usage:
     * @code
     * VEXPORT void my_function();
     * @endcode
     */
    #define VEXPORT __attribute__((dllexport))
  #endif
#elif defined(__GNUC__) || defined(__clang__)
  /**
   * @brief Marks a symbol for export from a shared library on UNIX-like systems.
   * 
   * On systems using GCC or Clang, this macro uses `__attribute__((visibility("default")))` 
   * to ensure the symbol is exported from a shared library. This allows the symbol to be 
   * accessed from other shared libraries or executables.
   * 
   * Example usage:
   * @code
   * VEXPORT void my_function();
   * @endcode
   */
  #define VEXPORT __attribute__((visibility("default")))
#else
  /**
   * @brief No specific export attributes are applied.
   * 
   * For compilers and platforms where no specific export attributes are needed, this macro 
   * expands to nothing. This allows the code to compile without any additional attributes 
   * being applied to the symbols.
   * 
   * Example usage:
   * @code
   * VEXPORT void my_function();
   * @endcode
   */
  #define VEXPORT
#endif

#if defined(__GNUC__) || defined(__clang__)
/**
 * @brief Stops name mangling by changing the symbol of the function to the specified one.
 * 
 * This macro can be used to ensure that a function has a specific symbol name in the compiled
 * object file, overriding the default name that would typically be generated (which might
 * include name mangling in C++). This is especially useful when interfacing with other languages
 * or assembly code that expects a particular symbol name.
 *
 * @param symbol The name that you want to replace the function's mangled name with.
 *
 * Example usage:
 * @code
 * template<typename T>
 * struct CustomType {
 *     T value;
 * };
 *
 * template<typename T>
 * CustomType<T> my_function() VSET_SYMBOL("my_function_alias");
 * @endcode
 */
#define VSET_SYMBOL(symbol) asm(symbol)
#else
/**
 * @brief Stops name mangling by changing the symbol of the function to the specified one.
 * 
 * This macro can be used to ensure that a function has a specific symbol name in the compiled
 * object file, overriding the default name that would typically be generated (which might
 * include name mangling in C++(, possibly beacuse of the function being templated)). This is especially useful when interfacing with other languages
 * or assembly code that expects a particular symbol name.   
 *
 * @param symbol The name that you want to replace the function's mangled name with.
 *
 * Example usage:
 * @code
 * template<typename T>
 * struct CustomType {
 *     T value;
 * };
 *
 * template<typename T>
 * CustomType<T> my_function() VSET_SYMBOL("my_function");
 * @endcode
 */
#define VSET_SYMBOL
#endif

#if defined(_MSC_VER)

// LIKE alignas(N) in C++
#define Valign(N) __declspec(align(N))

#else
// LIKE alignas(N) in C++
#define Valign(N) __attribute__((aligned(N)))

#endif

#ifdef __cplusplus
/**
 * @brief Ensures C linkage for the specified function to prevent name mangling in C++.
 * 
 * This macro is useful when you want to expose a C++ function with a C-compatible name,
 * which is necessary when interfacing with C code or when you need to avoid name mangling
 * that C++ applies to functions.
 *
 * Example usage:
 * @code
 * VEXTERN_C void my_function();
 * @endcode
 */
#define VEXTERN_C extern "C"
#else
#define VEXTERN_C
#endif

#endif // __vdef__
