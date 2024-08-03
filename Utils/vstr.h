#include <stdbool.h>
#include <string.h>
#include <stdint.h>
//WHY WINDOWS :=(
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
typedef SIZE_T size_t;
#endif

#define vstr_end (-1)
#define vstr_start (0)

typedef struct vstr {
    char* str;
    size_t len;
    bool own;
}vstr;

#ifdef __cplusplus
/**
 * Creates a vstr object from a C-string.
 * 
 * @param str The C-string to initialize the vstr object with.
 * @return A vstr object initialized with the given C-string, its length, and a default value of 0, meaning it is a string_view.
 */

#define VSTR(str) _VSTR(str)
/**
 * Creates a vstr object from a C-string.
 * 
 * @param str The C-string to initialize the vstr object with.
 * @return A vstr object initialized with the given C-string, its length, and a default value of 0, meaning it is a string_view.
 */
extern "C" {
vstr _VSTR(const char * str);
}
#else
#define VSTR(str) (vstr){(char*)str, strlen(str), 0}
#endif
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a string view from a given C-string and length.
 * This macro is a shorthand for vstr_create_ex with allocate set to false.
 * @param ret Pointer to the vstr structure to initialize.
 * @param str C-string to create the view or copy from.
 * @param length Length of the string.
 */
#define vstr_create(ret, str, length) vstr_create_ex(ret, str, length, false)

/**
 * Creates a vstr object. It can either create a view of an existing string or allocate a new string.
 * @param ret Pointer to the vstr structure to initialize.
 * @param str C-string to create the view or copy from.
 * @param length Length of the string.
 * @param allocate If true, allocates memory for a new string; if false, creates a view.
 * @return True on success, false on failure.
 */
bool vstr_create_ex(vstr* ret, const char* str, size_t length, bool allocate);

/**
 * Converts all characters in a vstr object to uppercase.
 * @param ret Pointer to the vstr object to convert.
 * @return True on success, false on failure.
 */
bool vstr_upper(vstr * ret);

/**
 * @brief Macro to check if a string does own the memory.
 * @param str The string struct.
 * @return True if the string does own the memory, false otherwise.
 */
#define vstr_owns(str) (str.own)

/**
 * @brief Macro to get the length of a string.
 * @param str The string struct.
 * @return The length of the string.
 */
#define vstr_len(str) (str.len)

/**
 * Converts all characters in a vstr object to lowercase.
 * @param ret Pointer to the vstr object to convert.
 * @return True on success, false on failure.
 */
bool vstr_lower(vstr *ret);

/**
 * Reverses the characters in a vstr object.
 * @param ret Pointer to the vstr object to reverse.
 * @return True on success, false on failure.
 */
bool vstr_rev(vstr * ret);

/**
 * Gets the character at the specified index in a vstr object.
 * @param str Pointer to the vstr object.
 * @param index Index of the character to retrieve (negative index counts from the end).
 * @return The character at the specified index, or '\0' if out of bounds.
 */
char vstr_get(const vstr *str, ssize_t index);

/**
 * Sets the character at the specified index in a vstr object.
 * @param ret Pointer to the vstr object.
 * @param index Index of the character to set (negative index counts from the end).
 * @param val The character to set at the specified index.
 * @return True on success, false on failure.
 */
bool vstr_set(vstr * ret, ssize_t index, const char val);

/**
 * Creates a copy of a vstr object.
 * @param str Pointer to the vstr object to copy.
 * @param allocate If true, allocates memory for a new string; if false, creates a view.
 * @return The copied vstr object.
 */
vstr vstr_copy(const vstr *str, bool allocate);

/**
 * Concatenates two vstr objects into a new vstr object.
 * @param ret Pointer to the vstr object to store the result.
 * @param a Pointer to the first vstr object.
 * @param b Pointer to the second vstr object.
 * @return True on success, false on failure.
 */
bool vstr_concat(vstr *ret, const vstr *a, const vstr *b);

/**
 * Creates a substring view from a vstr object.
 * @param ret Pointer to the vstr object to store the result.
 * @param str Pointer to the original vstr object.
 * @param start Start index of the substring.
 * @param end End index of the substring.
 * @return True on success, false on failure.
 */
bool vstr_slice(vstr *ret, const vstr *str, size_t start, size_t end);

/**
 * Creates a new vstr object from a substring of an existing vstr object.
 * @param ret Pointer to the vstr object to store the result.
 * @param str Pointer to the original vstr object.
 * @param start Start index of the substring.
 * @param end End index of the substring.
 * @return True on success, false on failure.
 */
bool vstr_slice_ex(vstr *ret, const vstr *str, size_t start, size_t end);

/**
 * Fills a vstr object with a specified character.
 * @param ret Pointer to the vstr object to fill.
 * @param val The character to fill with.
 * @return True on success, false on failure.
 */
bool vstr_fill(vstr *ret, const char val);

/**
 * Inserts a substring into a vstr object at the specified index.
 * @param dest Pointer to the vstr object to modify.
 * @param index Index where the substring will be inserted.
 * @param substr Pointer to the vstr object containing the substring.
 * @return True on success, false on failure.
 */
bool vstr_insert(vstr *dest, size_t index, const vstr *substr);

/**
 * Finds the last occurrence of a substring within a vstr object.
 * @param ret_view Pointer to the vstr object to store the view of the found substring (can be NULL).
 * @param src Pointer to the source vstr object.
 * @param substr Pointer to the vstr object containing the substring to find.
 * @return The index of the last occurrence, or -1 if not found.
 */
int vstr_findl(vstr* ret_view ,const vstr* src, const vstr* substr);

/**
 * Finds the first occurrence of a substring within a vstr object.
 * @param ret_view Pointer to the vstr object to store the view of the found substring (can be NULL).
 * @param src Pointer to the source vstr object.
 * @param substr Pointer to the vstr object containing the substring to find.
 * @return The index of the first occurrence, or -1 if not found.
 */
int vstr_findf(vstr* ret_view ,const vstr* src, const vstr* substr);

/**
 * Macro to check if a substring is contained within a string.
 * @param src Pointer to the source vstr object.
 * @param substr Pointer to the vstr object containing the substring to find.
 * @return True if the substring is found, false otherwise.
 */
#define strcontains(src, substr) (vstr_findf(NULL, src, substr) == 1)

/**
 * Checks if two vstr objects are equal.
 * @param a Pointer to the first vstr object.
 * @param b Pointer to the second vstr object.
 * @return True if the strings are equal, false otherwise.
 */
bool str_equals(const vstr* a, const vstr* b);

/**
 * Converts a vstr object to an integer.
 * @param str Pointer to the vstr object containing the string representation of the number.
 * @param ret Pointer to an int64_t variable to store the result.
 * @return True on success, false on failure.
 */
bool vstr_toint(const vstr *str, int64_t *ret);

/**
 * Converts an integer to a vstr object.
 * @param ret Pointer to the vstr object to store the result.
 * @param i The integer to convert.
 * @return True on success, false on failure.
 */
bool vstr_fromint(vstr *ret, int64_t i);

/**
 * Converts a vstr object to a floating-point number.
 * @param str Pointer to the vstr object containing the string representation of the number.
 * @param ret Pointer to a double variable to store the result.
 * @return True on success, false on failure.
 */
bool vstr_tofloat(const vstr *str, double* ret);

/**
 * Converts a floating-point number to a vstr object.
 * @param ret Pointer to the vstr object to store the result.
 * @param f The floating-point number to convert.
 * @return True on success, false on failure.
 */
bool vstr_fromfloat(vstr *ret, double f);

/**
 * Splits a given vstr string into an array of vstr strings using a specified delimiter.
 * 
 * @param str The input vstr string to be split.
 * @param delimiter The vstr delimiter used to split the input string.
 * @param count A pointer to a size_t variable where the function will store the number of tokens found.
 * 
 * @return An array of vstr pointers, each pointing to a token found in the input string.
 *         The caller is responsible for freeing the allocated memory for each token and the array.
 *         Returns NULL in case of an error or if the input string is empty.
 */
vstr ** vstr_split(vstr * str, vstr * delimiter, size_t * count);

/**
 * Joins multiple vstr structures into a single vstr, separated by a delimiter.
 *
 * @param delimiter A vstr struct containing the delimiter string.
 * @param count The number of vstr structures to join.
 * @param ... Variadic arguments representing the vstr structures to join.
 * @note VSTR() macro might be useful
 * @return A vstr struct containing the joined string.
 */
vstr vstr_join(vstr *delimiter, size_t count, ...);

void vstr_trim(vstr * str, const vstr * substr);

//TODO MYBE ADD THIS LATER 
//void vstr_rep(vstr* str, const vstr* old_substr, const vstr* new_substr);

/**
 * Destroys a vstr object, freeing any allocated memory for its string data.
 * Can be used to destroy both new strings and string views.
 * @param str Pointer to the vstr object to destroy. If the pointer is NULL, the function does nothing.
 */
void vstr_destroy(vstr* str);

#ifdef __cplusplus
}
#endif
