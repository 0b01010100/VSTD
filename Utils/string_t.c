#include <string_t.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
struct string_t
{
    cstring_t array;
    size_t size;
};

string_t* string_t_create(const size_t size)
{
    // Allocate memory for the string_t structure
    string_t* s = (string_t*)calloc(1, sizeof(string_t));
    if (!s) return NULL;

    // Allocate memory for the char array, including space for the null terminator
    s->array = (char*)calloc((size + /*'\0'*/1), sizeof(char));
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

string_t* string_t_create_from_cstring_t(const cstring_t cs)
{
    // Allocate memory for the string_t structure
    string_t* s = (string_t*)calloc(1, sizeof(string_t));
    if (!s) return NULL;

    // Calculate the length of the C-string
    size_t size = strlen(cs);

    // Allocate memory for the char array, including space for the null terminator
    s->array = (char*)calloc((size + /*'\0'*/1), sizeof(char));
    if (!s->array) {
        free(s);
        return NULL;
    }

    // Copy the C-string to the string array
    strncpy(s->array, cs, size);

    // Ensure the string array is null-terminated
    s->array[size] = '\0';

    // Set the size of the string
    s->size = size;

    return s;
}

char string_t_get(const string_t * str, ssize_t index)
{
    assert(str != NULL || str->size == 0);

    // If negative, Convert negative index to positive equivalent
    if (index < 0) index = str->size + index; 

    // Check if the index is within bounds
    if (index >= 0 && index < str->size) {
        return str->array[index];
    } else {
        return '\0'; // Return null character if the index is out of bounds
    }
}

int string_t_set(const string_t * str, ssize_t index, const char value)
{
    assert(str != NULL || str->size == 0);
    // If negative, Convert negative index to positive equivalent
    if (index < 0) index = str->size + index; 

    // Check if the index is within bounds
    if (index >= 0 && index < str->size) {
        str->array[index] = value;
        return 0;
    }
    return -1;
}

void string_t_fill(string_t* str, const char value)
{
    assert(str != NULL || str->size == 0);
    //set all indies to the same value
    size_t i = 0;
    for (;i < str->size; i++)
    {
        str->array[i] = value;
    }
}

size_t string_t_length(const string_t* src)
{
    assert(src != NULL);
    //return string size
    return src->size;
}

size_t string_t_concat(const string_t* src0, const string_t* src1, string_t* out)
{
    assert(src0 != NULL || src1 != NULL);

    //If size of both is zero just return out
    if(src0->array == 0 && src1->array == 0) return 0;

    // Allocate memory for the output string_t struct
    out = (string_t*)calloc(1, sizeof(string_t));
    if(!out) return 0;

    //Create char array to hold the concatinated strings from src0 and src1
    out->array = (cstring_t)calloc(src0->size + src1->size + /*'\0'*/1, sizeof(char));
    if(!out->array) return 0;

    //Copy src0 to out->array
    memcpy(out->array, src0->array, src0->size);

    //Concatenate src1 to out->array
    memcpy(out->array + src0->size, src1->array, src1->size);

    //Null terminate the concatenated string
    out->array[src0->size + src1->size] = '\0';

    //Set the size of the concatenated string
    out->size = src0->size + src1->size;
}

void string_t_reverse(string_t* src)
{
    assert(src != NULL);

    if(src->size == 0) return;

    int length = src->size;
    int start = 0;
    int end = length - 1;
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

cstring_t string_t_to_cstring_t(const string_t* src)
{
    assert(src != NULL || src->size == 0);
    
    //allocate memory for cstring_t
    cstring_t cs = (cstring_t)calloc(src->size, sizeof(char));
    //copy data from src->array to cs
    memmove(cs, src->array, src->size);
    return cs;
}

cstring_t string_t_to_cstring_t_then_free(string_t* src)
{    
    assert(src != NULL|| src->size == 0);

    //make cs pointer own char array now
    cstring_t cs = src->array;
    src->array = NULL;

    //destroy string_t
    string_t_free(src);

    // ret the cstring_t
    return cs;
}

void string_t_printf(const string_t* src)
{
    assert(src != NULL);
    //return out if no memory was allocated
    if(src->size == 0) return;
    //display char array to console
    printf(src->array);
}

void string_t_clear(string_t* src)
{
    assert(src != NULL);

    //return out if no memory was allocated
    if(src->size == 0) return;

    //zero memoery
    memset(src->array, 0, src->size);
}

void string_t_free(string_t* src)
{
    if (src) {
        if(src->array) free(src->array);
        free(src);
    }
}