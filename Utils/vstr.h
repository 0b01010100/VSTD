#include <stdbool.h>
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
typedef SIZE_T size_t;
#else
#include <stdint.h>
#endif


typedef char* vstr;

typedef enum VSTR_FIELD
{
    VSTR_FIELD_IS_VIEW = 0,
    VSTR_FIELD_LENGTH = sizeof(size_t)
}VSTR_FIELD;

#define VSTR_CSTR(vstr) (const char*)vstr.str

//creates a string view
#define vstr_create(ret, str, length) vstr_create_ex(ret, str, length, false)

//can create a string view or a new string
bool vstr_create_ex(vstr* ret, const char* str, size_t length, bool allocate);

//allows use to access fields like VSTR_FIELD_IS_VIEW and VSTR_FIELD_LENGTH
ssize_t vstr_get_field(const vstr *ret, VSTR_FIELD field);

// Convert a string to uppercase
bool vstr_upper(vstr *ret);

// Convert a string to lowercase
bool vstr_lower(vstr *ret);

// Reverse a string
bool vstr_rev(vstr * ret);

// Get the character at the given index in a string
char vstr_get(vstr * str, ssize_t index);

// Set the character at the given index in a string to a given value
bool vstr_set(vstr * ret, ssize_t index, const char val);

//make a copy of the origanal string
vstr vstr_copy(vstr* str);

//makes a new string that has a's abd b's data on the buffer
bool vstr_concat(vstr *ret, vstr* a, vstr* b);

//for making a string view that will point to a substring 
bool vstr_slice(vstr* ret, vstr str, size_t start, size_t end);

//for making an instance of the substring
bool vstr_slice_ex(vstr* ret, vstr str, size_t start, size_t end);

// Fill a string with a given character
bool vstr_fill(vstr *ret, const char val);

// Finds the last Occurance of the substring the src string
vstr* vstr_findl(const vstr* src, const vstr* substr);

// Finds the first Occurance of the substring the src string
vstr* vstr_findf(const vstr* src, const vstr* substr);

//char* vstr_insert(vstr* dest, size_t index, vstr * substr);

//can destroy a new string or string view
void vstr_destroy(vstr str);

