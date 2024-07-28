#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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
    if (size == 0) return NULL;
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

char* strninsert_ex(char* dest, size_t dest_size, size_t index, const char* substr)
{
    if (!dest || !substr) return NULL;

    size_t old_len = strlen(dest);
    size_t ss_len = strlen(substr);

    char * dp = dest;

    size_t new_len = 0x0;
    if(index > old_len){
        new_len = old_len + (index - old_len) + ss_len;

        dp = realloc(dp, new_len);
        memset(dp + old_len, ' ', 11 - 4);
        strcpy(dp + old_len + (11 - 4), substr);
        dp[index + ss_len] = '\0';
        size_t d = strlen(dp);
        d = 0;
    }else{
        dp += dest_size - index;
        new_len = dp + ((size_t)dest + old_len - 1);
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
    printf("%zu", strlen(str));
    return str;
}

void strfree_ex(char * src)
{
    if(src != NULL) free(src);
}