#include "vstr.h"
#include <stdlib.h>
#include <string.h>

#define FIELD_SIZE (sizeof(bool)+sizeof(size_t))

void vstr_set_field(vstr *ret, size_t field, size_t val)
{
    if (ret == NULL) return;

    switch (field)
    {
        case VSTR_FIELD_IS_VIEW:
            {
                bool* f_is_view = (bool*)((unsigned char*)ret - FIELD_SIZE);
                *f_is_view = (bool)val;
                break;
            }

        case VSTR_FIELD_LENGTH:
            {
                size_t* f_length = (size_t*)((unsigned char*)ret - VSTR_FIELD_LENGTH);
                *f_length = val;
                break;
            }
    }
}

bool vstr_create_ex(vstr *ret, const char *str, size_t length, bool allocate) {
    if (str == NULL) return false;

    unsigned char* field_and_str;

    if (allocate) {
        // New string
        field_and_str = (unsigned char*)malloc(FIELD_SIZE + (sizeof(char) * (length + 1)));
        if (field_and_str == NULL) return false;
        char * vs = *ret = (field_and_str + FIELD_SIZE);
        (void)strncpy(vs, str, length);
        vs[length] = '\0';
    } else {
        // String view
        field_and_str = (unsigned char*)malloc(FIELD_SIZE + sizeof(char*));
        if (field_and_str == NULL) return false;
        *ret = (field_and_str + FIELD_SIZE);
        *ret = str;
    }

    vstr_set_field(ret, VSTR_FIELD_IS_VIEW, !allocate);
    vstr_set_field(ret, VSTR_FIELD_LENGTH, length);

    return true;
}

ssize_t vstr_get_field(const vstr *ret, VSTR_FIELD field)
{
    if (ret == NULL) return -1;

    switch (field)
    {
        case VSTR_FIELD_IS_VIEW:
            {
                bool* f_is_view = (bool*)((unsigned char*)ret - FIELD_SIZE);
                return *f_is_view;
                break;
            }

        case VSTR_FIELD_LENGTH:
            {
                size_t* f_length = (size_t*)((unsigned char*)ret - VSTR_FIELD_LENGTH);
                return *f_length;
                break;
            }
    }
    return -1;
}

bool vstr_upper(vstr * ret)
{
    if(ret == NULL) return NULL;

    bool is_view = (bool)vstr_get_field(ret, VSTR_FIELD_IS_VIEW);
    if(is_view) return false;

    char* iter = *ret;

    while (*iter) {
        *iter = (*iter >= 'a' && *iter <= 'z') ? 'A' + (*iter - 'a') : *iter;
        iter+=1;
    }
    return true;
}

bool vstr_lower(vstr * ret)
{
    if(ret == NULL) return NULL;

    bool is_view = (bool)vstr_get_field(ret, VSTR_FIELD_IS_VIEW);
    if(is_view) return false;

    char* iter = *ret;

    while (*iter) {
        *iter = (*iter >= 'A' && *iter <= 'Z') ? 'a' + (*iter - 'A') : *iter;
        iter+=1;
    }
    return true;
}

bool vstr_rev(vstr * ret)
{
    if(ret == NULL) return NULL;

    bool is_view = (bool)vstr_get_field(ret, VSTR_FIELD_IS_VIEW);
    if(is_view) return false;
    
    char* iter = *ret;

    size_t start = 0;
    size_t end = (size_t)vstr_get_field(ret, VSTR_FIELD_LENGTH) - 1;
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
    return true;
}

char vstr_get(vstr * str, ssize_t index)
{
    if(str == NULL) return '\0';

    size_t length = (size_t)vstr_get_field(str, VSTR_FIELD_LENGTH);

    if(length == 0) return '\0';

    char * iter = *str;
    //convert negative index to positive
    index = (index < 0) ? length - index : index;

    if(index >= 0 && index < length)
    {
        return iter[index];
    } 
    return '\0';
}

bool vstr_set(vstr * ret, ssize_t index, const char val)
{
    if(ret == NULL) return NULL;

    bool is_view = (bool)vstr_get_field(ret, VSTR_FIELD_IS_VIEW);
    if(is_view) return false;

    char * iter = *ret;
    
    size_t length = (size_t)vstr_get_field(ret, VSTR_FIELD_LENGTH);

    if(length == 0) return false;

    //convert negative index to positive
    index = (index < 0) ? length + index : index;
    //check valid index
    if(index >= 0 && index < length)
    {
        iter[index] = val;
        return true;
    } 
    return false;
}

vstr vstr_copy(vstr* str)
{
    if (str == NULL) return NULL;

    size_t length = (size_t)vstr_get_field(str, VSTR_FIELD_LENGTH);
    bool is_view = (bool)vstr_get_field(str, VSTR_FIELD_IS_VIEW);

    //Make a copy
    vstr new_str = NULL;
    if (is_view) {
        vstr_create_ex(&new_str, *str, length, false);
    } else {
        vstr_create_ex(&new_str, *str, length, true);
    }

    return new_str;
}

bool vstr_concat(vstr *ret, vstr* a, vstr* b)
{
    if (a == NULL || b == NULL) return false;

    size_t length_a = (size_t)vstr_get_field(a, VSTR_FIELD_LENGTH);
    size_t length_b = (size_t)vstr_get_field(b, VSTR_FIELD_LENGTH);

    unsigned char* field_and_str = (unsigned char*)malloc(FIELD_SIZE + (length_a + length_b + 1) * sizeof(char));
    if (field_and_str == NULL) return false;
    char * vs = *ret = (field_and_str + FIELD_SIZE);
    strncpy(vs, *a, length_a);
    strncpy(vs + length_a, *b, length_b);
    
    (*ret)[length_a + length_b] = '\0';

    vstr_set_field(ret, VSTR_FIELD_IS_VIEW, false);
    vstr_set_field(ret, VSTR_FIELD_LENGTH, length_a + length_b);

    return true;
}

bool vstr_slice(vstr *ret, vstr str, size_t start, size_t end)
{
    if (str == NULL || start > end) return false;
    size_t length = (size_t)vstr_get_field(&str, VSTR_FIELD_LENGTH);

    if (start >= length || end > length) return false;
    size_t slice_length = end - start;
    char * orig_str = (char *)str;

    return vstr_create_ex(ret, orig_str + start, slice_length, false);
}

bool vstr_slice_ex(vstr *ret, vstr str, size_t start, size_t end)
{
    if (str == NULL || start > end) return false;

    size_t length = (size_t)vstr_get_field(&str, VSTR_FIELD_LENGTH);

    if (start >= length || end > length) return false;

    size_t slice_length = end - start;
    char *original_str = (char *)str;

    return vstr_create_ex(ret, original_str + start, slice_length, true);
}

bool vstr_fill(vstr *ret, const char val)
{
    if (ret == NULL)  return false;  
    bool is_view = (bool)vstr_get_field(ret, VSTR_FIELD_IS_VIEW);
    if(is_view) return false;

    char* iter = *ret;

    size_t length = (size_t)vstr_get_field(ret, VSTR_FIELD_LENGTH);
    if(length == 0) return false;
    
    while(*iter)
    {
        *iter = val;
        iter++;
    }
    return true;
}

// char* vstr_insert(vstr* dest, size_t index, vstr * substr)
// {
//     if (!dest || !substr) return NULL;

//     size_t old_len = (size_t)vstr_get_field(dest, VSTR_FIELD_LENGTH);
//     size_t ss_len = (size_t)vstr_get_field(substr, VSTR_FIELD_LENGTH);
//     char * dp = *dest;

//     size_t new_len = 0x0;
//     //appending affter
//     if(index >= old_len){
//         new_len = old_len + (index - old_len) + ss_len + 1;

//         dp = realloc(dp, new_len);
//         if (!dp) return NULL;
//         // Fill the gap with spaces
//         memset(dp + old_len, ' ', index - old_len);
//         // Copy substr into the created space
//         memcpy(dp + index, substr, ss_len);
//         strcpy(dp + index, substr);
//         size_t d = strlen(dp);
//         dp[new_len - 1] = '\0';
//         vstr_set_field(dest, VSTR_FIELD_LENGTH, new_len - 1);
//     }
//     //inserting between existing characters
//     else{
//         vstr a, b;
//         vstr_slice(&a, *dest, 0, index);
//         vstr_slice(&b, *dest, index, );
//         strfastinit_ex(dest + index);

//         size_t a_len = a ? strlen(a) : 0;
//         size_t b_len = b ? strlen(b) : 0;
//         size_t total_len = a_len + ss_len + b_len + 1;

//         dp = realloc(dp, total_len);
//         if (!dp) {
//             free(a);
//             free(b);
//             return NULL;
//         }

//         if (a) {
//             memmove(dp, a, a_len);
//             free(a);
//         }

//         memmove(dp + index + ss_len, b, b_len + 1); // +1 to include the null terminator
//         memcpy(dp + index, substr, ss_len);

//         if (b) {
//             free(b);
//         }
//         vstr_set_field(dest, VSTR_FIELD_LENGTH, total_len - 1);
//     }

//     return dp;
// }

vstr* vstr_findl(const vstr* src, const vstr* substr) {
    if (!src || !substr) return NULL;
    
    size_t str_len = (size_t)vstr_get_field(src, VSTR_FIELD_LENGTH);
    size_t substr_len = (size_t)vstr_get_field(substr, VSTR_FIELD_LENGTH);
    
    if (substr_len == 0) return NULL;
    if (substr_len > str_len) return NULL;

    const char* p = (*src) + str_len - substr_len;
    
    while (p >= (*src)) {
        if (strncmp(p, *substr, substr_len) == 0) {
            vstr * view;
            if(!vstr_create(view, p, substr_len)){
                return NULL;
            }
            return view;
        }
        p--;
    }
    
    return NULL;
}

vstr* vstr_findf(const vstr* src, const vstr* substr) {
    if (!src || !substr) return NULL;
    
    size_t str_len = (size_t)vstr_get_field(src, VSTR_FIELD_LENGTH);
    size_t substr_len = (size_t)vstr_get_field(substr, VSTR_FIELD_LENGTH);
    
    if (substr_len == 0) return NULL;
    if (substr_len > str_len) return NULL;

    const char* p = *src;
    
    while (p < (*src) + str_len - 1) {
        if (strncmp(p, *substr, substr_len) == 0) {
            vstr * view;
            if(!vstr_create(view, p, substr_len)){
                return NULL;
            }
            return view;
        }
        p++;
    }
    
    return NULL;
}

// char* strtrim_ex(char * str, const char * substr)
// {
//     if (str == NULL || substr == NULL) return NULL;

//     size_t str_len = strlen(str);
//     size_t substr_len = strlen(substr);

//     if (substr_len == 0) return str;
//     if (substr_len > str_len) return str;

//     char *result = malloc(str_len + 1);
//     if (!result) return NULL;

//     const char *p = str;
//     char *r = result;

//     while (*p)
//     {
//         if (strncmp(p, substr, substr_len) == 0)
//         {
//             p += substr_len;
//         }
//         else
//         {
//             *r++ = *p++;
//         }
//     }
//     *r = '\0';

//     size_t result_len = strlen(result);
//     strcpy(str, result);
//     free(result);
//     str = realloc(str, result_len);

//     return str;
// }


void vstr_destroy(vstr str)
{
    if (str == NULL) return;

    free((unsigned char*)str - FIELD_SIZE);
}