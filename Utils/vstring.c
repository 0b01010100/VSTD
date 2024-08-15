#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <vstring.h>

char * str_create_ex(const char * str, bool allocate)
{
    return strn_create_ex(str, strlen(str), allocate);
}

char * strn_create_ex(const char * str, size_t size, bool allocate)
{
    if (str == NULL) return NULL;

    if(size == 0) return NULL;
    //copy
    if(allocate == true)
    {
        char* copy = (char*)malloc((size + 1) * sizeof(char));
        if (copy == NULL) return NULL;
        strcpy(copy, str); 
        return copy;
    }
    //view
    else
    {
        return (char*)str;
    }
}

char* str_append(char* __stream, size_t __n, const char* format, ...)
{
    va_list args1, args2;
    va_start(args1, format);
    va_copy(args2, args1);

    //First, determine the required size
    int size = vsnprintf(NULL, 0, format, args1);
    va_end(args1);

    if (size < 0) {
        va_end(args2);
        return NULL;
    }

    //Alloc the required memory (plus 1 for null terminator)
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        va_end(args2);
        return NULL;
    }

    //Actually format the string
    vsnprintf(buffer, size + 1, format, args2);
    va_end(args2);
    //instert __stream string
    buffer = str_insert(buffer, 0, __stream);
    return buffer;
}

char * str_upper(char * str)
{
    if(str == NULL) return NULL;

    char* iter = str;

    while (*iter) {
        *iter = (*iter >= 'a' && *iter <= 'z') ? 'A' + (*iter - 'a') : *iter;
        iter+=1;
    }
    return str;
}

char * str_lower(char * str)
{
    if(str == NULL) return NULL;

    char* iter = str;

    while (*iter) {
        *iter = (*iter >= 'A' && *iter <= 'Z') ? 'a' + (*iter - 'A') : *iter;
        iter+=1;
    }
    return str;
}

char * str_rev(char * str)
{
    if(str == NULL) return NULL;

    char* iter = str;

    size_t start = 0;
    size_t end = strlen(str) - 1;
    char temp;

    while (start < end) {
        //Swap characters
        temp = iter[start];
        iter[start] = iter[end];
        iter[end] = temp;

        // Move towards the center
        start++;
        end--;
    }
    return str;
}

char * str_fill(char * str, const char val)
{
    if(str == NULL) return NULL;

    char* iter = str;

    size_t len = strlen(str);
    if(len == 0) return NULL;
    
    while(*iter)
    {
        *iter = val;
        iter++;
    }

    return str;
}
char str_get(const char * str, ssize_t index)
{
    if(str == NULL) return '\0';

    size_t l = strlen(str);

    if(l == 0) return '\0';

    //convert negative index to positive
    index = (index < 0) ? l - index : index;

    if(index >= 0 && index < l)
    {
        return str[index];
    } 
    return '\0';
}

char * str_set(char * str, ssize_t index, const char val)
{
    if(str == NULL) return NULL;

    char * ptr = str;
    
    size_t l = strlen(str);

    if(l == 0) return NULL;

    //convert negative index to positive
    index = (index < 0) ? l + index : index;
    //check valid index
    if(index >= 0 && index < l)
    {
        ptr[index] = val;
        return str;
    } 
    return NULL;
}

char * str_apn(char * str, const double number)
{
    if(str == NULL) return NULL;
    
    char * ptr = str;

    //cur len
    size_t str_len = strlen(str);
    
    //number to string
    char temp[31];
    snprintf(temp, 31, "%g", number);
    size_t temp_len = strlen(temp);

    //allocate space for the string-ifyed number
    char* result = (char*)realloc(ptr,  str_len + temp_len + 1);
    //append number to back of the string
    strcat(result, temp);
}

char* str_insert(char* dest, size_t index, const char* substr){
    return strn_insert(dest, strlen(dest), index, substr);
}

char* strn_insert(char* dest, size_t dest_size, size_t index, const char* substr)
{
    return strn_insert_s(dest, dest_size, index, substr, strlen(substr));
}

char* strn_insert_s(char* dest, size_t dest_size, size_t index, const char* substr, size_t ss_len)
{
    if (!dest || !substr) return NULL;

    size_t old_len = dest_size;

    char * dp = dest;

    size_t new_len = 0x0;
    //appending affter
    if(index >= old_len){
        new_len = old_len + (index - old_len) + ss_len + 1;

        dp = realloc(dp, new_len);
        if (!dp) return NULL;
        // Fill the gap with spaces
        memset(dp + old_len, ' ', index - old_len);
        // Copy substr into the created space
        memcpy(dp + index, substr, ss_len);
        strcpy(dp + index, substr);
        size_t d = strlen(dp);
        dp[new_len - 1] = '\0';
    }
    //inserting between existing characters
    else{
        char *a = strn_create_ex(dest + 0,index, false);
        char *b = str_create_ex(dest + index, true);

        size_t a_len = a ? strlen(a) : 0;
        size_t b_len = b ? strlen(b) : 0;
        size_t total_len = a_len + ss_len + b_len + 1;

        dp = realloc(dp, total_len);
        if (!dp) {
            free(a);
            free(b);
            return NULL;
        }

        if (a) {
            memmove(dp, a, a_len);
            free(a);
        }

        memmove(dp + index + ss_len, b, b_len + 1);
        memcpy(dp + index, substr, ss_len);

        if (b) {
            free(b);
        }
    }

    return dp;
}

char* str_findl(const char* str, const char* substr) {
    if (!str || !substr) return NULL;
    
    size_t str_len = strlen(str);
    size_t substr_len = strlen(substr);
    
    if (substr_len == 0) return (char*)str + str_len;
    if (substr_len > str_len) return NULL;

    const char* p = str + str_len - substr_len;
    
    while (p >= str) {
        if (strncmp(p, substr, substr_len) == 0) {
            return (char*)p;
        }
        p--;
    }
    
    return NULL;
}

char* strn_findf(const char* str, size_t len, const char* substr) 
{
    if (!str || len == 0 || !substr) return NULL;
    
    size_t str_len = len;
    size_t substr_len = strlen(substr);
    
    if (substr_len == 0) return (char*)str + str_len;
    if (substr_len > str_len) return NULL;

    const char* p = str + 0;
    
    while (p < str + str_len - 1) {
        if (strncmp(p, substr, substr_len) == 0) {
            return (char*)p;
        }
        p++;
    }
    
    return NULL;
}

char ** str_split(const char * str, const char * delimiter, size_t * count) {
    char * str_copy = strdup(str);
    char * token;
    size_t capacity = 10;
    char ** tokens = malloc(capacity * sizeof(char *));
    size_t token_count = 0;

    token = strtok(str_copy, delimiter);
    while (token != NULL) {
        if (token_count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(char *));
        }
        tokens[token_count++] = strdup(token);
        token = strtok(NULL, delimiter);
    }

    *count = token_count;
    free(str_copy);
    return tokens;
}

char * str_join(char ** str_array, size_t count, const char * delimiter)
{
    size_t total_len = 0;
    size_t delimiter_len = strlen(delimiter);
    for (size_t i = 0; i < count; ++i) {
        total_len += strlen(str_array[i]);
        if (i < count - 1) {
            total_len += delimiter_len;
        }
    }

    char * res = calloc(total_len + 1, 1);
    
    for (size_t i = 0; i < count; ++i) {
        strcat(res, str_array[i]);
        if (i < count - 1) {
            strcat(res, delimiter);
        }
    }

    res[total_len] = '\0';

    return res;
}

char* strn_trim(char * str, size_t len, const char * substr)
{
    if (str == NULL || len == 0 || substr == NULL) return NULL;

    size_t str_len = len;
    size_t substr_len = strlen(substr);

    if (substr_len == 0) return str;
    if (substr_len > str_len) return str;

    char *result = malloc(str_len + 1);
    if (!result) return NULL;

    const char *p = str;
    char *r = result;

    while (*p)
    {
        if (strncmp(p, substr, substr_len) == 0)
        {
            p += substr_len;
        }
        else
        {
            *r++ = *p++;
        }
    }
    *r = '\0';

    size_t result_len = strlen(result);
    strcpy(str, result);
    free(result);
    str = realloc(str, result_len);

    return str;
}

ssize_t strn_remove(char* str, size_t len, size_t pos, size_t end, bool __realloc)
{
    if (!str || !str || pos >= len || end == 0) return -1;

    if (pos + end > len) return -1;

    memmove(str + pos, str + pos + end, len - (pos + end) + 1);
    
    //if true realloc to close gaps. Ex: ("\b" or " " or '32') to ("")
    if (__realloc) {
        //checks if the data need to be reallocated for me
        char * __new = (char*)realloc(str, (len + 1) * sizeof(char));
        if (!__new) return -1;

        str = __new;
        len -= end; 
        str[len] = '\0';
        return len;
    }

    return len;
}

void strn_replace(char* str, size_t len, const char * old_substr, const char * new_substr, bool force)
{
    if(!str || !old_substr || !new_substr || strcmp(str, new_substr) == 0) return;

    size_t str_len = len;
    size_t old_substr_len = strlen(old_substr); 
    size_t new_substr_len = strlen(new_substr); 

    //if false we will realloc and instert even though new_substr_len > old_substr_len 
    if(new_substr_len > old_substr_len && !force) return;
    
    char *found_ptr = NULL;
    while ((found_ptr = strn_findf(str, len, old_substr)) != NULL) {
        
        // pointer to index
        size_t pos = found_ptr - str;

        //remove old substring
        strn_remove(str, len, pos, old_substr_len, true);
        len -= old_substr_len;

        //insert new substring(even if the substring is too big)
        str_insert(str, pos, new_substr);
        
        //adjust position to continue searching
        pos += new_substr_len;

        //update the length after insertion
        len += new_substr_len;
    }
}

void str_free(char * src)
{
    if(src != NULL) free(src);
}