#include <stdint.h>

#define vtrue (1)
#define vfalse (0)

typedef int8_t vbool;
typedef int8_t vi8;
typedef uint8_t vui8;
typedef int16_t vi16;
typedef uint16_t vui16;
typedef int32_t vi32;
typedef uint32_t vui32;
typedef int64_t vi64;
typedef uint64_t vui64;


typedef char* vstr;              // Pointer to mutable string
typedef const char* vcstr;       // Pointer to immutable string

typedef wchar_t* vwstr;          // Pointer to mutable wide-string
typedef const wchar_t* vwcstr;   // Pointer to immutable wide-string

#ifdef vretro
// Legacy types
typedef unsigned char vbyte;        // Unsigned 8-bit integer
typedef unsigned short vdword;      // Unsigned 16-bit integer
typedef unsigned long long vqword;  // Unsigned 64-bit integer

typedef char vchar;                 // Signed 8-bit integer
typedef short vshort;               // Signed 16-bit integer
typedef long long vlong;            // Signed 64-bit integer
#endif