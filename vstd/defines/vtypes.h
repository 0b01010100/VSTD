#include <stdint.h>
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#ifndef size_t
typedef SIZE_T size_t;
#endif
#endif

#define vtrue (1)
#define vfalse (0)

#define vyes vtrue
#define vno vfalse

/* 8-bit bool */
typedef union vb8 {
  uint8_t data;
  struct {
    uint8_t b0 : 1;  // bit 0
    uint8_t b1 : 1;  // bit 1
    uint8_t b2 : 1;  // bit 2
    uint8_t b3 : 1;  // bit 3
    uint8_t b4 : 1;  // bit 4
    uint8_t b5 : 1;  // bit 5
    uint8_t b6 : 1;  // bit 6
    uint8_t b7 : 1;  // bit 7
  };
} vb8;

/* 16-bit bool */
typedef union vb16 {
  uint16_t data;
  struct {
    uint16_t b0  : 1;  // bit 0
    uint16_t b1  : 1;  // bit 1
    uint16_t b2  : 1;  // bit 2
    uint16_t b3  : 1;  // bit 3
    uint16_t b4  : 1;  // bit 4
    uint16_t b5  : 1;  // bit 5
    uint16_t b6  : 1;  // bit 6
    uint16_t b7  : 1;  // bit 7
    uint16_t b8  : 1;  // bit 8
    uint16_t b9  : 1;  // bit 9
    uint16_t b10 : 1;  // bit 10
    uint16_t b11 : 1;  // bit 11
    uint16_t b12 : 1;  // bit 12
    uint16_t b13 : 1;  // bit 13
    uint16_t b14 : 1;  // bit 14
    uint16_t b15 : 1;  // bit 15
  };
} vb16;

/* 32-bit bool */
typedef union vb32 {
  uint32_t data;
  struct {
    uint32_t b0  : 1;  // bit 0
    uint32_t b1  : 1;  // bit 1
    uint32_t b2  : 1;  // bit 2
    uint32_t b3  : 1;  // bit 3
    uint32_t b4  : 1;  // bit 4
    uint32_t b5  : 1;  // bit 5
    uint32_t b6  : 1;  // bit 6
    uint32_t b7  : 1;  // bit 7
    uint32_t b8  : 1;  // bit 8
    uint32_t b9  : 1;  // bit 9
    uint32_t b10 : 1;  // bit 10
    uint32_t b11 : 1;  // bit 11
    uint32_t b12 : 1;  // bit 12
    uint32_t b13 : 1;  // bit 13
    uint32_t b14 : 1;  // bit 14
    uint32_t b15 : 1;  // bit 15
    uint32_t b16 : 1;  // bit 16
    uint32_t b17 : 1;  // bit 17
    uint32_t b18 : 1;  // bit 18
    uint32_t b19 : 1;  // bit 19
    uint32_t b20 : 1;  // bit 20
    uint32_t b21 : 1;  // bit 21
    uint32_t b22 : 1;  // bit 22
    uint32_t b23 : 1;  // bit 23
    uint32_t b24 : 1;  // bit 24
    uint32_t b25 : 1;  // bit 25
    uint32_t b26 : 1;  // bit 26
    uint32_t b27 : 1;  // bit 27
    uint32_t b28 : 1;  // bit 28
    uint32_t b29 : 1;  // bit 29
    uint32_t b30 : 1;  // bit 30
    uint32_t b31 : 1;  // bit 31
  };
} vb32;

/* 64-bit bool */
typedef union vb64 {
    uint64_t data;
    struct {
        uint64_t b0  : 1;  // bit 0
        uint64_t b1  : 1;  // bit 1
        uint64_t b2  : 1;  // bit 2
        uint64_t b3  : 1;  // bit 3
        uint64_t b4  : 1;  // bit 4
        uint64_t b5  : 1;  // bit 5
        uint64_t b6  : 1;  // bit 6
        uint64_t b7  : 1;  // bit 7
        uint64_t b8  : 1;  // bit 8
        uint64_t b9  : 1;  // bit 9
        uint64_t b10 : 1;  // bit 10
        uint64_t b11 : 1;  // bit 11
        uint64_t b12 : 1;  // bit 12
        uint64_t b13 : 1;  // bit 13
        uint64_t b14 : 1;  // bit 14
        uint64_t b15 : 1;  // bit 15
        uint64_t b16 : 1;  // bit 16
        uint64_t b17 : 1;  // bit 17
        uint64_t b18 : 1;  // bit 18
        uint64_t b19 : 1;  // bit 19
        uint64_t b20 : 1;  // bit 20
        uint64_t b21 : 1;  // bit 21
        uint64_t b22 : 1;  // bit 22
        uint64_t b23 : 1;  // bit 23
        uint64_t b24 : 1;  // bit 24
        uint64_t b25 : 1;  // bit 25
        uint64_t b26 : 1;  // bit 26
        uint64_t b27 : 1;  // bit 27
        uint64_t b28 : 1;  // bit 28
        uint64_t b29 : 1;  // bit 29
        uint64_t b30 : 1;  // bit 30
        uint64_t b31 : 1;  // bit 31
        uint64_t b32 : 1;  // bit 32
        uint64_t b33 : 1;  // bit 33
        uint64_t b34 : 1;  // bit 34
        uint64_t b35 : 1;  // bit 35
        uint64_t b36 : 1;  // bit 36
        uint64_t b37 : 1;  // bit 37
        uint64_t b38 : 1;  // bit 38
        uint64_t b39 : 1;  // bit 39
        uint64_t b40 : 1;  // bit 40
        uint64_t b41 : 1;  // bit 41
        uint64_t b42 : 1;  // bit 42
        uint64_t b43 : 1;  // bit 43
        uint64_t b44 : 1;  // bit 44
        uint64_t b45 : 1;  // bit 45
        uint64_t b46 : 1;  // bit 46
        uint64_t b47 : 1;  // bit 47
        uint64_t b48 : 1;  // bit 48
        uint64_t b49 : 1;  // bit 49
        uint64_t b50 : 1;  // bit 50
        uint64_t b51 : 1;  // bit 51
        uint64_t b52 : 1;  // bit 52
        uint64_t b53 : 1;  // bit 53
        uint64_t b54 : 1;  // bit 54
        uint64_t b55 : 1;  // bit 55
        uint64_t b56 : 1;  // bit 56
        uint64_t b57 : 1;  // bit 57
        uint64_t b58 : 1;  // bit 58
        uint64_t b59 : 1;  // bit 59
        uint64_t b60 : 1;  // bit 60
        uint64_t b61 : 1;  // bit 61
        uint64_t b62 : 1;  // bit 62
        uint64_t b63 : 1;  // bit 63
    };
} vb64;

/* 128-bit bool */
typedef union vb128 {
    char data[16];    
    struct {
        uint64_t b0  : 1;  // bit 0
        uint64_t b1  : 1;  // bit 1
        uint64_t b2  : 1;  // bit 2
        uint64_t b3  : 1;  // bit 3
        uint64_t b4  : 1;  // bit 4
        uint64_t b5  : 1;  // bit 5
        uint64_t b6  : 1;  // bit 6
        uint64_t b7  : 1;  // bit 7
        uint64_t b8  : 1;  // bit 8
        uint64_t b9  : 1;  // bit 9
        uint64_t b10 : 1;  // bit 10
        uint64_t b11 : 1;  // bit 11
        uint64_t b12 : 1;  // bit 12
        uint64_t b13 : 1;  // bit 13
        uint64_t b14 : 1;  // bit 14
        uint64_t b15 : 1;  // bit 15
        uint64_t b16 : 1;  // bit 16
        uint64_t b17 : 1;  // bit 17
        uint64_t b18 : 1;  // bit 18
        uint64_t b19 : 1;  // bit 19
        uint64_t b20 : 1;  // bit 20
        uint64_t b21 : 1;  // bit 21
        uint64_t b22 : 1;  // bit 22
        uint64_t b23 : 1;  // bit 23
        uint64_t b24 : 1;  // bit 24
        uint64_t b25 : 1;  // bit 25
        uint64_t b26 : 1;  // bit 26
        uint64_t b27 : 1;  // bit 27
        uint64_t b28 : 1;  // bit 28
        uint64_t b29 : 1;  // bit 29
        uint64_t b30 : 1;  // bit 30
        uint64_t b31 : 1;  // bit 31
        uint64_t b32 : 1;  // bit 32
        uint64_t b33 : 1;  // bit 33
        uint64_t b34 : 1;  // bit 34
        uint64_t b35 : 1;  // bit 35
        uint64_t b36 : 1;  // bit 36
        uint64_t b37 : 1;  // bit 37
        uint64_t b38 : 1;  // bit 38
        uint64_t b39 : 1;  // bit 39
        uint64_t b40 : 1;  // bit 40
        uint64_t b41 : 1;  // bit 41
        uint64_t b42 : 1;  // bit 42
        uint64_t b43 : 1;  // bit 43
        uint64_t b44 : 1;  // bit 44
        uint64_t b45 : 1;  // bit 45
        uint64_t b46 : 1;  // bit 46
        uint64_t b47 : 1;  // bit 47
        uint64_t b48 : 1;  // bit 48
        uint64_t b49 : 1;  // bit 49
        uint64_t b50 : 1;  // bit 50
        uint64_t b51 : 1;  // bit 51
        uint64_t b52 : 1;  // bit 52
        uint64_t b53 : 1;  // bit 53
        uint64_t b54 : 1;  // bit 54
        uint64_t b55 : 1;  // bit 55
        uint64_t b56 : 1;  // bit 56
        uint64_t b57 : 1;  // bit 57
        uint64_t b58 : 1;  // bit 58
        uint64_t b59 : 1;  // bit 59
        uint64_t b60 : 1;  // bit 60
        uint64_t b61 : 1;  // bit 61
        uint64_t b62 : 1;  // bit 62
        uint64_t b63 : 1;  // bit 63
        uint64_t b64  : 1;  // bit 64
        uint64_t b65  : 1;  // bit 65
        uint64_t b66  : 1;  // bit 66
        uint64_t b67  : 1;  // bit 67
        uint64_t b68  : 1;  // bit 68
        uint64_t b69  : 1;  // bit 69
        uint64_t b70  : 1;  // bit 70
        uint64_t b71  : 1;  // bit 71
        uint64_t b72  : 1;  // bit 72
        uint64_t b73  : 1;  // bit 73
        uint64_t b74  : 1;  // bit 74
        uint64_t b75  : 1;  // bit 75
        uint64_t b76  : 1;  // bit 76
        uint64_t b77  : 1;  // bit 77
        uint64_t b78  : 1;  // bit 78
        uint64_t b79  : 1;  // bit 79
        uint64_t b80  : 1;  // bit 80
        uint64_t b81  : 1;  // bit 81
        uint64_t b82  : 1;  // bit 82
        uint64_t b83  : 1;  // bit 83
        uint64_t b84  : 1;  // bit 84
        uint64_t b85  : 1;  // bit 85
        uint64_t b86  : 1;  // bit 86
        uint64_t b87  : 1;  // bit 87
        uint64_t b88  : 1;  // bit 88
        uint64_t b89  : 1;  // bit 89
        uint64_t b90  : 1;  // bit 90
        uint64_t b91  : 1;  // bit 91
        uint64_t b92  : 1;  // bit 92
        uint64_t b93  : 1;  // bit 93
        uint64_t b94  : 1;  // bit 94
        uint64_t b95  : 1;  // bit 95
        uint64_t b96  : 1;  // bit 96
        uint64_t b97  : 1;  // bit 97
        uint64_t b98  : 1;  // bit 98
        uint64_t b99  : 1;  // bit 99
        uint64_t b100  : 1;  // bit 100
        uint64_t b101  : 1;  // bit 101
        uint64_t b102  : 1;  // bit 102
        uint64_t b103  : 1;  // bit 103
        uint64_t b104  : 1;  // bit 104
        uint64_t b105  : 1;  // bit 105
        uint64_t b106  : 1;  // bit 106
        uint64_t b107  : 1;  // bit 107
        uint64_t b108  : 1;  // bit 108
        uint64_t b109  : 1;  // bit 109
        uint64_t b110  : 1;  // bit 110
        uint64_t b111  : 1;  // bit 111
        uint64_t b112  : 1;  // bit 112
        uint64_t b113  : 1;  // bit 113
        uint64_t b114  : 1;  // bit 114
        uint64_t b115  : 1;  // bit 115
        uint64_t b116  : 1;  // bit 116
        uint64_t b117  : 1;  // bit 117
        uint64_t b118  : 1;  // bit 118
        uint64_t b119  : 1;  // bit 119
        uint64_t b120  : 1;  // bit 120
        uint64_t b121  : 1;  // bit 121
        uint64_t b122  : 1;  // bit 122
        uint64_t b123  : 1;  // bit 123
        uint64_t b124  : 1;  // bit 124
        uint64_t b125  : 1;  // bit 125
        uint64_t b126  : 1;  // bit 126
        uint64_t b127  : 1;  // bit 127
  };
}vb128;

typedef int8_t vi8;
typedef uint8_t vui8;
typedef int16_t vi16;
typedef uint16_t vui16;
typedef int32_t vi32;
typedef uint32_t vui32;
typedef int64_t vi64;
typedef uint64_t vui64;
typedef char vui128[16];

typedef char* vcp;              // Pointer to mutable string
typedef const char* vccp;       // Pointer to immutable string

typedef wchar_t* vwcp;          // Pointer to mutable wide-string
typedef const wchar_t* vwccp;   // Pointer to immutable wide-string

typedef float vf32;
typedef double vf64, vnum, vnumber;

typedef size_t vs; 
typedef ssize_t vss; 

typedef void* vvp; 

#ifdef vretro
// Legacy types
typedef unsigned char vbyte;        // Unsigned 8-bit integer
typedef unsigned short vdword;      // Unsigned 16-bit integer
typedef unsigned long long vqword;  // Unsigned 64-bit integer

typedef char vchar;                 // Signed 8-bit integer
typedef wchar_t vwchar;             // Signed 16-bit integer
typedef short vshort;               // Signed 16-bit integer
typedef long long vlong;            // Signed 64-bit integer
#endif