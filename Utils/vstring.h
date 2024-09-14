/**
 * @brief Provides extended functionality for the standard `<string.h>`.
 *
 * This API extends `<string.h>` with additional methods for convenience.
 * @note that this is not part of the C standard library but a separate extension. Have Fun
*/
#ifndef __vstring__
#define __vstring__
#include <stdbool.h> // for bool type
#include <string.h>
#include <stdint.h> // for size_t and ssize_t(For Unix)
//WHY WINDOWS :=(
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <BaseTsd.h> //for size_t and ssize_t(For Windows)
typedef SSIZE_T ssize_t;
typedef SIZE_T size_t;
#endif

/**
 * @brief Represents the end of a string
 * @return -1
 */
#define strend (-1)

/**
 * @brief Represents the start of a string
 * @return 0
 */
#define strstart (0)

/**
 * @brief Represents a char * or string
 */
typedef char * str;

#ifdef __cplusplus
extern "C" {
#define STR_(string) string
#endif
/**
 * @brief Creates a string view
 * @param str The original string.
 * @return A pointer to the string or view
 */
#define str_create(str) str_create_ex(str, false)

/**
 * @brief Creates a string or view; allocation behavior controlled by the boolean parameter.
 * @param str The original string.
 * @param allocate If true, allocate new memory; otherwise, use existing memory.
 * @return A pointer to the initialized string.
 */
char * str_create_ex(const char * str, bool allocate);

/**
 * @brief Initialize a string with a copy of a portion of the original string, up to a specified size; allocation behavior controlled by the boolean parameter.
 * @param str The original string.
 * @param size The maximum number of characters to copy.
 * @param allocate If true, allocate new memory; otherwise, use existing memory.
 * @return A pointer to the initialized string.
 */
char * strn_create_ex(const char * str, size_t size, bool allocate);

/**
 * @brief Create a substring from the original string, from sub_start to sub_end (exclusive); allocates memory for the new substring.
 * @param src The original string.
 * @param sub_start The start index of the substring.
 * @param sub_end The end index of the substring (exclusive).
 * @param allocate Whether to create a view or a new string (true for new string, view otherwise).
 * @note sub_end will not stop at where you want it when creating a view (the end will be the same as the original string).
 * @return A pointer to the newly created substring.
 */
#define strn_sub_ex(src, sub_start, sub_end, allocate) \
    (char*)strn_create_ex(src, (sub_end - sub_start), allocate)

/**
 * @brief Create a substring from the original string, from sub_start to sub_end (exclusive); allocates memory for the new substring.
 * @param src The original string.
 * @param sub_start The start index of the substring.
 * @param sub_end The end index of the substring (exclusive).
 * @note sub_end will not stop at where you want it when creating a view (the end will be the same as the original string).
 * @return A pointer to the newly created substring.
 */
#define strn_sub(src, sub_start, sub_end) \
    (char*)strn_sub_ex(src, sub_start, sub_end, false)

/**
 * @brief Append a formatted string (using printf-style formatting) to the end of an existing string; allocates memory as needed.
 * @param __stream The existing string to which the formatted string will be appended.
 * @param __n The size of the existing string.
 * @param format The format string.
 * @param ... Additional arguments for formatting.
 * @return A pointer to the resulting string.
 */
char* str_append(char* __stream, size_t __n, const char* format, ...);

/**
 * @brief Insert a substring into a destination string at the specified index; does not check for destination size limits.
 * @param dest The destination string.
 * @param index The index at which to insert the substring.
 * @param substr The substring to insert.
 * @return A pointer to the resulting string.
 */
char* str_insert(char* dest, size_t index, const char* substr);

/**
 * @brief Insert a substring into a destination string at the specified index with a size limit for the destination string.
 * @param dest The destination string.
 * @param dest_size The size of the destination string.
 * @param index The index at which to insert the substring.
 * @param substr The substring to insert.
 * @return A pointer to the resulting string.
 */
char* strn_insert(char* dest, size_t dest_size, size_t index, const char* substr);

/**
 * @brief Insert a substring into a destination string at the specified index with a size limit for both the destination and the substring.
 * @param dest The destination string.
 * @param dest_size The size of the destination string.
 * @param index The index at which to insert the substring.
 * @param substr The substring to insert.
 * @param ss_len The length of the substring to insert.
 * @return A pointer to the resulting string.
 */
char* strn_insert_s(char* dest, size_t dest_size, size_t index, const char* substr, size_t ss_len);

/**
 * @brief Convert a string to uppercase; modifies the original string.
 * @param str The string to convert.
 * @return A pointer to the modified string.
 */
char * str_upper(char * str);

/**
 * @brief Convert a string to lowercase; modifies the original string.
 * @param str The string to convert.
 * @return A pointer to the modified string.
 */
char * str_lower(char * str);

/**
 * @brief Reverse the characters in a string; modifies the original string.
 * @param str The string to reverse.
 * @return A pointer to the modified string.
 */
char * str_rev(char * str);

/**
 * @brief Fill a string with a specified character; modifies the original string.
 * @param str The string to fill.
 * @param val The character to fill the string with.
 * @return A pointer to the modified string.
 */
char * str_fill(char * str, const char val);

/**
 * @brief Get the character at a specific index in a string.
 * @param str The string to search.
 * @param index The index of the character to retrieve.
 * @return The character at the specified index.
 */
char str_get(const char * str, ssize_t index);

/**
 * @brief Set the character at a specific index in a string to a new value; modifies the original string.
 * @param str The string to modify.
 * @param index The index of the character to set.
 * @param val The new character value.
 * @return A pointer to the modified string.
 */
char * str_set(char * str, ssize_t index, const char val);

/**
 * @brief Append a double number as a string to the end of an existing string; allocates memory as needed.
 * @param str The existing string.
 * @param number The double number to append.
 * @return A pointer to the resulting string.
 */
char * str_apn(char * str, const double number);

/**
 * @brief Find the last occurrence of a substring in a string.
 * @param str The string to search.
 * @param substr The substring to find.
 * @return A pointer to the last occurrence of the substring, or NULL if not found.
 */
char * str_findl(const char* str, const char* substr);

/**
 * @brief Find the first occurrence of a substring in a string.
 * @param str The string to search.
 * @param len The lenght of the string
 * @param substr The substring to find.
 * @return A pointer to the first occurrence of the substring, or NULL if not found.
 */
char* strn_findf(const char* str, size_t len, const char* substr);

/**
 * @brief A Macro to find the first occurrence of a substring in a string.
 * @param str The string to search.
 * @param substr The substring to find.
 * @return A pointer to the first occurrence of the substring, or NULL if not found.
 */
#define str_findf(str, substr) (char*)strn_findf(const char* str, strlen(str), const char* substr);

/**
 * @brief A Macro to find the first occurrence of a substring in a string. Same as str_findf
 * @param str The string to search.
 * @param substr The substring to find.
 * @return A pointer to the first occurrence of the substring, or NULL if not found.
 */
#define str_find str_findf

/**
 * @brief Macro to check if a substring is contained within a string.
 * @param str The string to search.
 * @param substr The substring to find.
 * @return True if the substring is contained within the string, false otherwise.
 */
#define str_contains(str, substr) (str_findf(str, substr) != NULL)

/**
 * @brief Split a string into an array of tokens based on a delimiter; sets the count of tokens.
 * @param str The string to split.
 * @param delimiter The delimiter used to split the string.
 * @param count Pointer to a size_t variable where the number of tokens will be stored.
 * @return An array of strings (tokens).
 */
char ** str_split(const char * str, const char * delimiter, size_t * count);

/**
 * @brief Join an array of strings into a single string, with a specified delimiter between each string.
 * @param str_array The array of strings to join.
 * @param count The number of strings in the array.
 * @param delimiter The delimiter to insert between each string.
 * @return A pointer to the joined string.
 */
char * str_join(char ** str_array, size_t count, const char * delimiter);

/**
 * @brief Trim occurrences of a substring from both ends of a string.
 * @param str The string to trim.
 * @param len The lenght of the string to trim.
 * @param substr The substring to remove.
 * @return A pointer to the trimmed string.
 */
char* strn_trim(char * str, size_t len, const char * substr);

/**
 * @brief A Macro to trim occurrences of a substring from both ends of a string.
 * @param str The string to trim.
 * @param len The lenght of the string to trim.
 * @param substr The substring to remove.
 * @return A pointer to the trimmed string.
 */
#define str_trim(str, substr) (char*)strn_trim(str, strlen(str), substr);

/**
 * @brief Removes a portion of the string starting from a specific position.
 *
 * @param str The main string from which a portion will be removed. The content will be modified.
 * @param len The lenght of the soruce string
 * @param pos The starting position in the main string to begin removing characters.
 * @param end The number of characters to remove starting from the position `pos`.
 * @param __realloc If true, the memory for the string will be reallocated to free unused space.
 * @return Returns the size of the string if the removal was successful, -1 otherwise.
 */
ssize_t strn_remove(char* str, size_t len, size_t pos, size_t end, bool __realloc);

/**
 * @brief A Macro that removes a portion of the string starting from a specific position.
 *
 * @param str The main string from which a portion will be removed. The content will be modified.
 * @param pos The starting position in the main string to begin removing characters.
 * @param end The number of characters to remove starting from the position `pos`.
 * @param __realloc If true, the memory for the string will be reallocated to free unused space.
 * @return Returns the size of the string if the removal was successful, -1 otherwise.
 */
#define str_remove(str, pos, end, __realloc) (ssize_t)strn_remove(str, strlen(str), old_substr, new_substr, force)

/**
 * @brief Replaces all occurrences of a substring within a string with another substring.
 * 
 * This function searches for all instances of `old_substr` within `str` and replaces them with `new_substr`. 
 * If `new_substr` is an empty string, the function will reallocate the memory for `str` to remove unused space if `force` is true.
 * 
 * @param str Pointer to the string where the replacement will occur. The content of this string will be modified.
 * @param len The length of the string `str`. This parameter allows specifying the maximum length to consider in the string.
 * @param old_substr The substring that will be replaced.
 * @param new_substr The substring that will replace `old_substr`. If this is an empty string, memory reallocation may occur based on `force`.
 * @param force If true, the memory for `str` will be reallocated to free any unused space. If false, the memory will not be reallocated.
 * 
 * @return Returns the size of the modified string if the operation was successful, or -1 if an error occurred.
 */
void strn_replace(char* str, size_t len, const char * old_substr, const char * new_substr, bool force);

/**
 * @brief Macro for replacing a portion of a string with another substring.
 * 
 * This macro calls `strn_replace` with the full length of the string `str`. It provides a convenient way to replace substrings 
 * without needing to specify the length of the string explicitly.
 * 
 * @param str The main string where replacements will be made. This string will be modified.
 * @param old_substr The substring to be replaced.
 * @param new_substr The substring that will replace `old_substr`. If this is an empty string, `force` determines if reallocation occurs.
 * @param force If true, the memory for `str` will be reallocated to remove any unused space after replacement.
 * 
 * @return The result of the `strn_replace` function call. Note that this macro does not return a value directly.
 */
#define str_replace(str, old_substr, new_substr, force) (void)strn_replace(str, strlen(str), old_substr, new_substr, force)

// This macro compares the first `maxCount` characters of two strings `str` and `str1`.
// It uses `strncmp`, which returns 0 if the first `maxCount` characters of both strings are equal.
// The macro expands to `strncmp(str, str1, maxCount)`, effectively performing the comparison.
#define strn_equal(str, str1, maxCount) strncmp(str, str1, maxCount)

// This macro compares two strings `str` and `str1` using `strcmp`.
// It checks if the entire strings are equal, returning 0 if they are.
// It should be `strcmp(str, str1)` without the `maxCount` parameter.
#define str_equal(str, str1) strcmp(str, str1)

/**
 * @brief Free memory allocated for a string.
 * @param src The string to free.
 */
void str_free(char * src);

/**
 * Macro to iterate over a arrary of chars 
 * @param item A variable of type T that will be assigned each element of the vector
 * @param str_ The string to iterate over
 * @param len The lenght of the string
 * @param action The action to perform on each iteration(if any) -> Optional
*/
#define strn_foreach(item, str_, len, action) { \
    size_t __len = len; \
    if (__len != 0) { \
        char* __data = str; \
        size_t __stride = sizeof(char);\
        for (size_t __i = 0; __i < __len; __i++) { \
            item = *(char*)(__data + __stride * __i);\
            action; \
        } \
    } \
}

/**
 * Macro to iterate over a arrary of chars 
 * @param item A copy of an item in array
 * @param str_ The string to iterate over
 * @param action The action to perform on each iteration(if any) -> Optional
*/
#define str_foreach(item, str_, len, action) \
    strn_foreach(item, str_, strlen(str_), action)


#ifdef __cplusplus
}
#endif
#endif