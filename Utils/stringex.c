#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <stringex.h>

char * strmalloc_ex(size_t size)
{
    char* n = (char*)malloc((size + 1) * sizeof(char));
    if(n ==  NULL) return NULL;
    n[size] = '\0';
    return n;
}

char * strcalloc_ex(size_t size)
{
    char* n = (char*)calloc((size + 1) * sizeof(char), sizeof(char));
    if(n ==  NULL) return NULL;
    n[size] = '\0';//might be already set but just doing just in case 
    return n;
}

char * strfastinit_ex(const char * original)
{
    if (original == NULL) return NULL;
    size_t size = strlen(original);
    if(size == 0) return NULL;
    char* n = (char*)malloc((size + 1) * sizeof(char));
    if (n == NULL) return NULL;
    strcpy(n, original); // copy the original string
    return n;
}

char * strrealloc_ex(char * str, size_t newsize)
{
    if (str == NULL) return NULL;
    size_t size = strlen(str);
    if (size == 0) return NULL;

    char* newstr = realloc(str, newsize + /*'\0'*/1);
    if (newstr == NULL) return NULL;

    newstr[newsize] = '\0';
    return newstr;
}

char* strfastinit_printf_ex(const char* format, ...) 
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

    return buffer;
}

char * strsub_ex(const char * src, size_t sub_start, size_t sub_end)
{
    return strnsub_ex(src, strlen(src), sub_start, sub_end);
}

char * strnsub_ex(const char * src, size_t src_size, size_t sub_start, size_t sub_end)
{
    if(src == NULL || src_size == sub_start || sub_end == 0 || sub_start == src_size) return NULL;
   char * sub = calloc((sub_end - sub_start) + 1, 1);
   sub = strncpy(sub, src + sub_start, sub_end - sub_start);
}
char * strupper_ex(char * str)
{
    if(str == NULL) return NULL;

    char* iter = str;

    while (*iter) {
        *iter = (*iter >= 'a' && *iter <= 'z') ? 'A' + (*iter - 'a') : *iter;
        iter+=1;
    }
    return str;
}

char * strlower_ex(char * str)
{
    if(str == NULL) return NULL;

    char* iter = str;

    while (*iter) {
        *iter = (*iter >= 'A' && *iter <= 'Z') ? 'a' + (*iter - 'A') : *iter;
        iter+=1;
    }
    return str;
}

char * strrev_ex(char * str)
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

char * strfill_ex(char * str, const char val)
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
char strget_ex(const char * str, ssize_t index)
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

char * strset_ex(char * str, ssize_t index, const char val)
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

char * strapn_ex(char * str, const double number)
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

char * strapstr_ex(char * dest, const char * src)
{
   if(!dest || !src) return NULL;

    char * ptr = dest;

   //append string to back of the string
   size_t dest_len = strlen(dest);
   size_t src_len = strlen(src);

   //allocate space for the src string
   char* result = (char*)realloc(ptr, dest_len + src_len + 1);
   if(!result){
        return NULL;
   }

   strncpy(result, dest, dest_len);
   strncat(result, src, src_len);

   return result;
}

char* strinsert_ex(char* dest, size_t index, const char* substr){
    return strninsert_ex(dest, strlen(dest), index, substr);
}

char* strninsert_ex(char* dest, size_t dest_size, size_t index, const char* substr)
{
    return strninsert_s_ex(dest, dest_size, index, substr, strlen(substr));
}

char* strninsert_s_ex(char* dest, size_t dest_size, size_t index, const char* substr, size_t ss_len)
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
        char *a = strnsub_ex(dest, dest_size, 0, index);
        char *b = strfastinit_ex(dest + index);

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

        memmove(dp + index + ss_len, b, b_len + 1); // +1 to include the null terminator
        memcpy(dp + index, substr, ss_len);

        if (b) {
            free(b);
        }
    }

    return dp;
}

char* strfindl_ex(const char* str, const char* substr) {
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

char* strfindf_ex(const char* str, const char* substr) {
    if (!str || !substr) return NULL;
    
    size_t str_len = strlen(str);
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

char* strtrim_ex(char * str, const char * substr)
{
    if (str == NULL || substr == NULL) return NULL;

    size_t str_len = strlen(str);
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

char * strrem_ex(char * str, const char * substr)
{
    if (str == NULL || substr == NULL) return NULL;

    size_t str_len = strlen(str);
    size_t substr_len = strlen(substr);

    if (substr_len == 0) return str;
    if (substr_len > str_len) return str;

    char *result = calloc(str_len + 1, 1); // +1 for the null terminator
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

    str = memset(str, ' ', str_len);
    memcpy(str, result, 3);
    str[str_len - 1] = '\0';
    free(result);
    return str;
}

int strtoi_ex(const char * str)
{
    char *endptr;
    int num = strtol(str, &endptr, 10);
    // Check for possible errors
    if (endptr == str) {
        // No conversion was performed
        fprintf(stderr, "No digits were found in the input string.\n");
        return 0.0;
    } else if (errno == ERANGE) {
        // Out of range error
        fprintf(stderr, "The input value is out of range.\n");
        return num;
    } else if (*endptr != '\0') {
        // There are unprocessed characters left in the string
        fprintf(stderr, "The input string has extra characters: %s\n", endptr);
        return num;
    }
    return num;
}

double strtod_ex(const char *str)
{
    char *endptr;
    errno = 0; // To distinguish success/failure after call
    double result = strtod(str, &endptr);

    // Check for possible errors
    if (endptr == str) {
        // No conversion was performed
        fprintf(stderr, "No digits were found in the input string.\n");
        return 0.0;
    } else if (errno == ERANGE) {
        // Out of range error
        fprintf(stderr, "The input value is out of range.\n");
        return result;
    } else if (*endptr != '\0') {
        // There are unprocessed characters left in the string
        fprintf(stderr, "The input string has extra characters: %s\n", endptr);
        return result;
    }

    return result;
}

char ** strsplit_ex(const char * str, const char * delimiter, size_t * count) {
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

char * strjoin_ex(char ** str_array, size_t count, const char * delimiter)
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
    res[0] = '\0';

    for (size_t i = 0; i < count; ++i) {
        strcat(res, str_array[i]);
        if (i < count - 1) {
            strcat(res, delimiter);
        }
    }

    return res;
}

void strfree_ex(char * src)
{
    if(src != NULL) free(src);
}