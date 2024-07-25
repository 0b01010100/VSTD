#pragma once
// TODO: ADD: lower, upper, compare_ignore_case, ends_with, starts_with, find_last, replace, join, split
#include <stdbool.h> // for bool type
#include <stdint.h> // for size_t

//string struct
typedef struct string_t string_t;

//cstring
typedef char* cstring_t;

/*
    Allocates a string_t
    @param size -> amount of indices for each char to allocate
    @return pointer to heap-allocated string_t
*/
string_t* string_t_create(const size_t size);

/*
    Creates a string_t using cstring_t (or char *) as initial data 
    @param str -> source string
    @return pointer to heap-allocated string_t
*/
string_t* string_t_create_from_cstring_t(const cstring_t cs);

/*
    Creates a string_t using char* (or cstring_t) as initial data 
    @param cp -> source string
    @return pointer to heap-allocated string_t
*/
#define string_t_create_from_char_pointer(cp) string_t_create_from_cstring_t(cp)

/*
    Gets the value of an index of the string
    @param index -> index for value(CAN BE NEGATIVE OR POSITIVE)
    @return value of char at index
*/
char string_t_get(const string_t * str, ssize_t index);

/*
    Sets the index of the string to a value
    @param str -> source string
    @param index -> index for value(CAN BE NEGATIVE OR POSITIVE)
    @param value -> value to set at the index
    @return -1 if index no in bonds
*/
int string_t_set(const string_t * str, ssize_t index, const char value);

/*
    Sets all the indices of the string to a value
    @param str -> source string
    @param value -> value to set at each index
    @return none
*/
void string_t_fill(string_t* str, const char value);

/*
    Gets the length of the string
    @param src -> source string
    @return size_t length of the string
*/
size_t string_t_length(const string_t* src);

/*
    Concatenates two strings. out = src0 + src1
    @param src0 -> first string
    @param src1 -> second string
    @param out -> output string
    @return size_t length of the concatenated string
*/
size_t string_t_concat(const string_t* src0, const string_t* src1, string_t* out);

/* 
    Concatenates a substring and a string. out = (in_start..in_end) + src1
    @param in_start -> start address of substring
    @param in_end -> end address of substring
    @param out -> output string
    @return size_t length of the concatenated string
*/
size_t string_t_concat_sub_string_t_a(const string_t* in_start, const string_t* in_end, string_t* out);

/*
    Concatenates a substring and a string using index. out = (in[in_start..in_end]) + src1
    @param in -> source string
    @param in_start -> start index of substring
    @param in_end -> end index of substring
    @param out -> output string
    @return size_t length of the concatenated string
*/
size_t string_t_concat_sub_string_t_i(const string_t* in, const size_t in_start, const size_t in_end, string_t* out);

/*
    Appends a string to another string
    @param out -> string to append data to
    @param in -> string to append
    @return size_t length of the resulting string
*/
size_t string_t_append_string_t(string_t* out, const string_t* in);

/*
    Appends a number to a string
    @param out -> output string
    @param number -> number to append
    @return size_t length of the resulting string
*/
size_t string_t_append_number(string_t* out, const double number);

/*
    Appends a substring to a string. out = (in_start..in_end) + out
    @param out -> output string
    @param in_start -> start address of substring
    @param in_end -> end address of substring
    @return size_t length of the resulting string
*/
size_t string_t_append_sub_string_t_a(string_t* out, const string_t* in_start, const string_t* in_end);

/*
    Appends a substring to a string using index. out = in[in_start..in_end] + out
    @param out -> output string
    @param in -> source string
    @param in_start -> start index of substring
    @param in_end -> end index of substring
    @return size_t length of the resulting string
*/
size_t string_t_append_sub_string_t_i(string_t* out, const string_t* in, const size_t in_start, const size_t in_end);

/*
    Copies the contents of one string to another
    @param src -> original string
    @param dest -> copy destination
    @return size_t length of the copied string
*/
size_t string_t_copy(const string_t* src, string_t* dest);

/*
    Creates a substring and copies it to the output
    @param src_start -> start address of the source string
    @param src_end -> end address of the source string
    @param out -> output substring
    @return size_t length of the new substring
*/
size_t string_t_substr_a(const char* src_start, const char* src_end, string_t* out);

/*
    Creates a substring using index and copies it to the output
    @param in -> source string
    @param in_start -> start index of substring
    @param in_end -> end index of substring
    @param out -> output substring
    @return size_t length of the new substring
*/
size_t string_t_substr_i(const string_t* in, const size_t in_start, const size_t in_end, string_t* out);

/*
    Checks if the string is a number
    @param src -> source string
    @return true if the string is a number
*/
bool string_t_is_number_t(const string_t* src);

/*
    Returns a pointer to the first occurrence of a character or string in the source string
    @param src -> source string
    @param contents -> contents to look for
    @return pointer to the first occurrence of the character or string, or 0 if not found
*/
string_t* string_t_contains(const string_t* src, const string_t* contents);

/*
    Returns a pointer to the first occurrence of a character or string in the source string
    @param src -> source string
    @param target -> contents to look for
    @return pointer to the first occurrence of the character or string, or 0 if not found
*/
#define string_t_find(src, target) string_t_contains(src, target)

/*
    Compares two strings
    @param src0 -> first string to compare
    @param src1 -> second string to compare
    @return -1 on error, 0 if they don't match, 1 if they match
*/
int string_t_compare(const string_t* src0, const string_t* src1);

/*
    Removes all white spaces from the string
    @param str_to_trim -> source string to remove white spaces from
    @return size_t length of the trimmed string
*/
size_t string_t_trim_whitespace(const string_t* str_to_trim);

/*
    Removes all occurrences of bias from the source string
    @param src -> source string
    @param bias -> characters to remove from the string
    @return size_t length of the resulting string
*/
size_t string_t_remove_all(const string_t* src, const string_t* bias);

/*
    Reverses the order of characters in the string
    @param src -> source string
    @return none
*/
void string_t_reverse(string_t* src);

/*
    Converts string to cstring_t (or char*)
    Note: malloc is called in here for cstring_t (or char*)
    @param src -> source string
    @return cstring_t
*/
cstring_t string_t_to_cstring_t(const string_t* src);

/*
    Converts string_t to char*
    Note: malloc is called in here for char* (or cstring_t)
    @param src -> source string
    @return char*
*/ 
#define string_t_to_char_pointer(src) string_t_to_cstring_t(src)

/*
    Converts string to char* then frees string_t structure
    Note: malloc is call in here for cstring_t (or char*)
    @param src -> source string
    @return cstring_t
*/
cstring_t string_t_to_cstring_t_then_free(string_t* src);

/*
    Converts string to char* then frees string_t structure
    Note: malloc is call in here for char* (or cstring_t)
    @param src -> source string
    @return char*
*/
#define string_t_to_char_pointer_then_free(src) string_t_to_cstring_t_then_free(src)

/*
    Prints the string to the console
    @param src -> source string
*/
void string_t_printf(const string_t* src);

/*
    Clears the char array of the allocated memory
    @param src -> source string
*/
void string_t_clear(string_t* src);

/*
    Frees the allocated memory
    @param src -> source string
*/
void string_t_free(string_t* src);