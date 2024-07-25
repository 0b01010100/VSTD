#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string_t.h>

typedef char* cstring_t;

typedef struct string_t {
    cstring_t array;
    size_t size;
}string_t;

// Function to create a string_t with a given size
string_t* string_t_create(const size_t size) {
    // Allocate memory for the string_t structure
    string_t* s = (string_t*)calloc(1, sizeof(string_t));
    if (!s) return NULL;

    // Allocate memory for the char array, including space for the null terminator
    s->array = (char*)calloc((size + 1), sizeof(char));
    if (!s->array) {
        free(s);
        return NULL;
    }

    // Ensure the string array is null-terminated
    s->array[size] = '\0';

    // Set the size of the string
    s->size = size;

    return s;
}

// Function to create a string_t from a C-string
string_t* string_t_create_from_cstring_t(const cstring_t cs) {
    // Allocate memory for the string_t structure
    string_t* s = (string_t*)calloc(1, sizeof(string_t));
    if (!s) return NULL;

    // Calculate the length of the C-string
    size_t size = strlen(cs);

    // Allocate memory for the char array, including space for the null terminator
    s->array = (char*)calloc((size + 1), sizeof(char));
    if (!s->array) {
        free(s);
        return NULL;
    }

    // Copy the C-string to the string array
    strcpy(s->array, cs);

    // Set the size of the string
    s->size = size;

    return s;
}

// Function to get a character from a string_t
char string_t_get(const string_t* str, ssize_t index) {
    assert(str != NULL);

    // If negative, convert negative index to positive equivalent
    if (index < 0) index = str->size + index; 

    // Check if the index is within bounds
    if (index >= 0 && index < str->size) {
        return str->array[index];
    } else {
        return '\0'; // Return null character if the index is out of bounds
    }
}

// Function to set a character in a string_t
int string_t_set(string_t* str, ssize_t index, const char value) {
    assert(str != NULL);

    // If negative, convert negative index to positive equivalent
    if (index < 0) index = str->size + index; 

    // Check if the index is within bounds
    if (index >= 0 && index < str->size) {
        str->array[index] = value;
        return 0;
    }
    return -1;
}

// Function to fill a string_t with a specified character
void string_t_fill(string_t* str, const char value) {
    assert(str != NULL);

    // Set all indices to the same value
    for (size_t i = 0; i < str->size; i++) {
        str->array[i] = value;
    }
}

// Function to get the length of a string_t
size_t string_t_length(const string_t* src) {
    assert(src != NULL);
    return src->size;
}

// Function to concatenate two string_t objects
size_t string_t_concat(const string_t* src0, const string_t* src1, string_t* out) {
    assert(src0 != NULL && src1 != NULL);

    // Allocate memory for the concatenated string_t
    out = (string_t*)calloc(1, sizeof(string_t));
    if (!out) return 0;

    // Allocate memory for the concatenated char array, including space for the null terminator
    out->array = (char*)calloc(src0->size + src1->size + 1, sizeof(char));
    if (!out->array) {
        free(out);
        return 0;
    }

    // Copy src0 to out->array
    memcpy(out->array, src0->array, src0->size);

    // Concatenate src1 to out->array
    memcpy(out->array + src0->size, src1->array, src1->size);

    // Null-terminate the concatenated string
    out->array[src0->size + src1->size] = '\0';

    // Set the size of the concatenated string
    out->size = src0->size + src1->size;

    return out->size;
}

// Function to compare two string_t objects
int string_t_compare(const string_t* src0, const string_t* src1) {
    if (!src0 || !src1 || !src0->array || !src1->array) return -1;
    return strcmp(src0->array, src1->array);
}

// Function to reverse a string_t
void string_t_reverse(string_t* src) {
    assert(src != NULL);

    if (src->size == 0) return;

    size_t start = 0;
    size_t end = src->size - 1;
    char temp;

    while (start < end) {
        // Swap characters
        temp = src->array[start];
        src->array[start] = src->array[end];
        src->array[end] = temp;

        // Move towards the center
        start++;
        end--;
    }
}

// Function to convert a string_t to a C-string
cstring_t string_t_to_cstring_t(const string_t* src) {
    assert(src != NULL);

    // Allocate memory for cstring_t
    cstring_t cs = (cstring_t)calloc(src->size + 1, sizeof(char));
    if (!cs) return NULL;

    // Copy data from src->array to cs
    memcpy(cs, src->array, src->size);
    cs[src->size] = '\0';

    return cs;
}

// Function to convert a string_t to a C-string and free the string_t
cstring_t string_t_to_cstring_t_then_free(string_t* src) {    
    assert(src != NULL);

    // Make cs pointer own char array now
    cstring_t cs = src->array;
    src->array = NULL;

    // Free the string_t
    string_t_free(src);

    // Return the cstring_t
    return cs;
}

// Function to print a string_t
void string_t_printf(const string_t* src) {
    assert(src != NULL);

    // Return out if no memory was allocated
    if (src->size == 0) return;

    // Display char array to console
    printf("%s", src->array);
}

// Function to clear a string_t by setting all characters to '\0'
void string_t_clear(string_t* src) {
    assert(src != NULL);

    // Return out if no memory was allocated
    if (src->size == 0) return;

    // Zero memory
    memset(src->array, 0, src->size);
}

// Function to free a string_t
void string_t_free(string_t* src) {
    if (src) {
        if (src->array) free(src->array);
        free(src);
    }
}
