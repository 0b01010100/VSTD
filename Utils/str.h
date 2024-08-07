/**
 * @brief Provides extended functionality for the standard `<string.h>`.
 *
 * This API extends `<string.h>` with additional methods for convenience.
 * @note that this is not part of the C standard library but a separate extension. Have Fun
*/

#pragma once
#include <stdbool.h> // for bool type
#include <string.h>
#include <stdint.h>
//WHY WINDOWS :=(
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
typedef SIZE_T size_t;
#endif

#define strend (-1)
#define strstart (0)

typedef char * str;

// Create a string with a copy of the original string, without allocating new memory
#define str_create(str) str_create_ex(str, false)

/**
 * @brief Initialize a string with a copy of the original string; allocation behavior controlled by the boolean parameter.
 * @param str The original string.
 * @param allocate If true, allocate new memory; otherwise, use existing memory.
 * @return A pointer to the initialized string.
 */
const char * str_create_ex(const char * str, bool allocate);

/**
 * @brief Initialize a string with a copy of a portion of the original string, up to a specified size; allocation behavior controlled by the boolean parameter.
 * @param str The original string.
 * @param size The maximum number of characters to copy.
 * @param allocate If true, allocate new memory; otherwise, use existing memory.
 * @return A pointer to the initialized string.
 */
const char * strn_create_ex(const char * str, size_t size, bool allocate);

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
 * @brief Create a substring from the original string, from sub_start to sub_end (exclusive); allocates memory for the new substring.
 * @param src The original string.
 * @param sub_start The start index of the substring.
 * @param sub_end The end index of the substring (exclusive).
 * @return A pointer to the newly created substring.
 */
char * str_sub(const char * src, size_t sub_start, size_t sub_end);

/**
 * @brief Create a substring from the original string with a given size, from sub_start to sub_end (exclusive); allocates memory for the new substring.
 * @param src The original string.
 * @param src_size The size of the original string.
 * @param sub_start The start index of the substring.
 * @param sub_end The end index of the substring (exclusive).
 * @return A pointer to the newly created substring.
 */
char * str_nsub(const char * src, size_t src_size, size_t sub_start, size_t sub_end);

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
char* str_ninsert(char* dest, size_t dest_size, size_t index, const char* substr);

/**
 * @brief Insert a substring into a destination string at the specified index with a size limit for both the destination and the substring.
 * @param dest The destination string.
 * @param dest_size The size of the destination string.
 * @param index The index at which to insert the substring.
 * @param substr The substring to insert.
 * @param ss_len The length of the substring to insert.
 * @return A pointer to the resulting string.
 */
char* str_ninsert_s(char* dest, size_t dest_size, size_t index, const char* substr, size_t ss_len);

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
 * @brief Insert a substring into a destination string at the specified index; does not check for destination size limits (duplicate of the earlier str_insert function).
 * @param dest The destination string.
 * @param index The index at which to insert the substring.
 * @param substr The substring to insert.
 * @return A pointer to the resulting string.
 */
char* str_insert(char* dest, size_t index, const char* substr);

/**
 * @brief Insert a substring into a destination string at the specified index with a size limit for the destination string; (duplicate of the earlier str_ninsert function).
 * @param dest The destination string.
 * @param dest_size The size of the destination string.
 * @param index The index at which to insert the substring.
 * @param substr The substring to insert.
 * @return A pointer to the resulting string.
 */
char * str_ninsert(char * dest, size_t dest_size, size_t index, const char * substr);

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
 * @param substr The substring to find.
 * @return A pointer to the first occurrence of the substring, or NULL if not found.
 */
char * str_findf(const char* str, const char* substr);

/**
 * @brief Macro to check if a substring is contained within a string.
 * @param str The string to search.
 * @param substr The substring to find.
 * @return True if the substring is contained within the string, false otherwise.
 */
#define str_contains(str, substr) (str_findf(str, substr) != NULL)

/**
 * @brief Trim occurrences of a substring from both ends of a string.
 * @param str The string to trim.
 * @param substr The substring to remove.
 * @return A pointer to the trimmed string.
 */
char * str_trim(char * str, const char * substr);

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
 * @brief Free memory allocated for a string.
 * @param src The string to free.
 */
void str_free(char * src);
