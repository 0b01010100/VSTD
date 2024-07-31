#pragma once

// TODO file the damn ERROR HANDLING FOR THE STRING FUNCTIONS THAT DO ALLOCATIONS
// I forgot to do error checking for when our string function calls realloc for strings that are on the stack

//Ah fuck relloc throws for strings on the stack
    //result = strapn_ex("ddd", 2);

#include <stdbool.h> // for bool type

// for size_t and ssize_t
#ifdef U_WINDOWS
#include <baseTsd.h>
typedef SIZE_T size_t;
typedef SSIZE_T ssize_t;
#else
#include <stdint.h> 
#endif
#include <string.h>

#define strend_ex (-1)
#define strstart_ex (0)

// Type alias for C-style strings
typedef char * cstring;

//----Memory allocation functions

// Allocate memory for a string of given size
char * strmalloc_ex(size_t size);

// Allocate and zero-initialize memory for a string of given size
char * strcalloc_ex(size_t size);

// Initialize a string with a copy of the original string
char * strfastinit_ex(const char * original);

// Initialize a string using a formatted string (like printf, but does memory allocations)
char* strfastinit_snprintf_ex(char* __stream, size_t __n, const char* format, ...);

// Create a substring from src between sub_start and sub_end (exclusive)
char * strsub_ex(const char * src, size_t sub_start, size_t sub_end);

// Create a substring from src with given size between sub_start and sub_end (exclusive)
char * strnsub_ex(const char * src, size_t src_size, size_t sub_start, size_t sub_end);

//----///////////////////////////

// Insert substr into dest at given index
char* strinsert_ex(char* dest, size_t index, const char* substr);

//Gets the length of a string.
#define strlength_ex(str) strlen(str)

// Compare two strings
#define strcmp_ex(a, b) strcmp(a, b)

// Concatenate two strings
#define strcat_ex(dest, src) strcat(dest, src);

// Copy a string onto a buffer
#define strcpy_ex(dest, src) strcpy(dest, src);

// Insert substr into dest at given index with destination size limit
char* strninsert_ex(char* dest, size_t dest_size, size_t index, const char* substr);

// Insert substr into dest at given index with size limit for substr
char* strninsert_s_ex(char* dest, size_t dest_size, size_t index, const char* substr, size_t ss_len);

// Convert a string to uppercase
char * strupper_ex(char * str);

// Convert a string to lowercase
char * strlower_ex(char * str);

// Reverse a string
char * strrev_ex(char * str);

// Fill a string with a given character
char * strfill_ex(char * str, const char val);

// Get the character at the given index in a string
char strget_ex(const char * str, ssize_t index);

// Set the character at the given index in a string to a given value
char * strset_ex(char * str, ssize_t index, const char val);

// Append a double number as a string to the end of str
char * strapn_ex(char * str, const double number);

// Append src string to the end of dest string
char * strapstr_ex(char * dest, const char * src);

// Insert substr into dest at given index
char* strinsert_ex(char* dest, size_t index, const char* substr);

// Insert substr into dest at given index with destination size limit
char * strninsert_ex(char * dest, size_t dest_size, size_t index, const char * substr);

// Find the last occurrence of substr in str
char * strfindl_ex(const char* str, const char* substr);

// Find the first occurrence of substr in str
char * strfindf_ex(const char* str, const char* substr);

// Macro to check if substr is contained within str
#define strcontains_ex(str, substr) (strfindf(str, substr) != NULL)

// Trim occurrences of substr from both ends of str
char * strtrim_ex(char * str, const char * substr);

// Remove occurrences of substr from str
char * strrem_ex(char * str, const char * substr);

//Convert a string to an integer
int strtoi_ex(const char * str);

//convert string to double
double strtod_ex(const char *str);

// Split a string into tokens based on delimiter
char ** strsplit_ex(const char * str, const char * delimiter, size_t * count);

// Join an array of strings into one string with a delimiter
char * strjoin_ex(char ** str_array, size_t count, const char * delimiter);

// Free memory allocated for a string
void strfree_ex(char * src);
