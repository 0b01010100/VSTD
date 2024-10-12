#include "vstr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <limits.h>

vstr _VSTR(const char * str)
{
    return (vstr){(char*)str, strlen(str), 0};
}

bool vstr_create_ex(vstr *ret, const char *str, size_t length, bool allocate) {
    if (!str || !ret) return false;

    if (allocate) {
        ret->str = malloc((sizeof(char) * (length + 1)));
        if (!ret->str) return false; 
        memcpy(ret->str, str, length);
        ret->str[length] = '\0';
        ret->len = length;
        ret->own = true;
    } else {
        ret->str = (char *)str;
        ret->len = length;
        ret->own = false;
    }

    return true;
}

bool vstr_upper(vstr *ret) {
    if (!ret || !ret->str) return false;
    if (!ret->own) return false;

    char *iter = ret->str;
    while (*iter) {
        *iter = (*iter >= 'a' && *iter <= 'z') ? 'A' + (*iter - 'a') : *iter;
        iter++;
    }

    return true;
}

bool vstr_lower(vstr *ret) {
    if (!ret || !ret->str) return false;
    if (!ret->own) return false;

    char *iter = ret->str;
    while (*iter) {
        *iter = (*iter >= 'A' && *iter <= 'Z') ? 'a' + (*iter - 'A') : *iter;
        iter++;
    }

    return true;
}

bool vstr_rev(vstr *ret) {
    if (!ret || !ret->str) return false;
    if (!ret->own) return false;

    char *iter = ret->str;
    size_t start = 0, end = ret->len - 1;
    char temp;

    while (start < end) {
        temp = iter[start];
        iter[start] = iter[end];
        iter[end] = temp;
        start++;
        end--;
    }

    return true;
}

char vstr_get(const vstr *str, ssize_t index) {
    if (!str || !str->str || str->len == 0) return '\0';

    if (index < 0) index += str->len;

    if (index >= 0 && (size_t)index < str->len) {
        return str->str[index];
    }

    return '\0';
}

bool vstr_set(vstr *ret, ssize_t index, char val) {
    if (!ret || !ret->str) return false;
    if (!ret->own) return false;

    if (index < 0) index += ret->len;

    if (index >= 0 && (size_t)index < ret->len) {
        ret->str[index] = val;
        return true;
    }

    return false;
}

vstr vstr_copy(const vstr *str, bool allocate) {
    vstr new_str = {0};
    if (str && str->str) {
        vstr_create_ex(&new_str, str->str, str->len, allocate);
    }
    return new_str;
}

bool vstr_concat(vstr *ret, const vstr *a, const vstr *b) {
    if (!a || !b || !ret) return false;

    size_t new_len = a->len + b->len;
    //if ret->str is already allocated resize it
    char *new_str;
    if (ret->str && ret->own) {
        new_str = realloc(ret->str, new_len + 1);
        if (!new_str) return false;
    } 
    //otherwise, allocate new memory
    else {
        new_str = malloc(new_len + 1);
        if (!new_str) return false;
        memcpy(new_str, a->str, a->len);
        ret->own = true;
    }

    memcpy(new_str + a->len, b->str, b->len);

    new_str[new_len] = '\0';

    ret->str = new_str;
    ret->len = new_len;
    

    return true;
}

bool vstr_slice(vstr *ret, const vstr *str, size_t start, size_t end) {
    if (!ret || !str || start >= end || start >= str->len || end > str->len) return false;

    return vstr_create_ex(ret, str->str + start, end - start, false);
}

bool vstr_slice_ex(vstr *ret, const vstr *str, size_t start, size_t end) {
    if (!str || start >= end || start >= str->len || end > str->len) return false;

    return vstr_create_ex(ret, str->str + start, end - start, true);
}

bool vstr_fill(vstr *ret, char val) {
    if (!ret || !ret->str) return false;
    if (!ret->own) return false;

    memset(ret->str, val, ret->len);
    ret->str[ret->len] = '\0';

    return true;
}

bool vstr_insert(vstr *dest, size_t index, const vstr *substr) {
    if (!dest || !substr || !dest->str || !substr->str || !dest->own) return false;

    size_t dest_len = dest->len;
    size_t substr_len = substr->len;

    if (index > dest_len) index = dest_len;

    char *new_str = realloc(dest->str, dest_len + substr_len + 1);
    if (!new_str) return false;

    memmove(new_str + index + substr_len, new_str + index, dest_len - index);
    memcpy(new_str + index, substr->str, substr_len);
    new_str[dest_len + substr_len] = '\0';

    dest->str = new_str;
    dest->len = dest_len + substr_len;

    return true;
}

int vstr_findf(vstr* ret_view ,const vstr* src, const vstr* substr) {
    if (!src || !substr) return -1;

    size_t str_len = src->len;
    size_t substr_len = substr->len;

    if (substr_len == 0 || substr_len > str_len) return 0;

    const char *p = src->str;
    while (p < src->str + str_len) {
        if (strncmp(p, substr->str, substr_len) == 0) {
            vstr_create_ex(ret_view, p, substr_len, false);
            return 1; // true
        }
        p++;
    }
    return 0;
}

int vstr_findl(vstr* ret_view ,const vstr* src, const vstr* substr) {
    if (!src || !substr) return -1;

    size_t str_len = src->len;
    size_t substr_len = substr->len;

    if (substr_len == 0 || substr_len > str_len) return 0;

    const char *p = src->str + str_len - substr_len;
    while (p >= src->str) {
        if (strncmp(p, substr->str, substr_len) == 0) {
            vstr_create_ex(ret_view, p, substr_len, false);
            return 1; // true
        }
        p--;
    }
    return 0;
}

bool str_equals(const vstr* a, const vstr* b) {
    if (a->len != b->len) {
        return false;
    }
    
    return strcmp(a->str, b->str) == 0;
}

bool vstr_toint(const vstr *str, int64_t *ret) {
    if(ret == NULL) return false;
    assert(ret != NULL && "vstr_toint: ret cannot be null");
    if(sscanf(str->str, "%lld", ret) != 1) {
        return false;
    }
    return true;
}

bool vstr_fromint(vstr *ret, int64_t i)
{
    if(ret == NULL) return false;
    char buf[21];
    snprintf(buf, 64, "%lld", i);
    size_t len = strlen(buf);
    return vstr_create_ex(ret, buf, len, true);
}

bool vstr_tofloat(const vstr *str, double* ret) {
    if(ret == NULL) return false;
    assert(ret != NULL && "vstr_toint: ret cannot be null");
    if(sscanf(str->str, "%lf", ret) != 1) {
        return false;
    }
    return true;
}

bool vstr_fromfloat(vstr *ret, double f) {
    if(ret == NULL) return false;
    char buf[32];
    snprintf(buf, 64, "%lf", f);
    size_t len = strlen(buf);
    return vstr_create_ex(ret, buf, len, true);
}

vstr **vstr_split(vstr *str, vstr *delimiter, size_t *count) {
    if (!str || !delimiter || !count || !str->str || !delimiter->str || !str->own) {
        return NULL;
    }
    vstr str_cpy = {0};
    vstr_create_ex(&str_cpy, str->str, str->len, true);
    if (!str_cpy.str) {
        *count = 0;
        return NULL;
    }

    //Tokenize the string using the delimiter
    char *token;
    size_t capacity = 10;
    vstr **tokens = malloc(capacity * sizeof(vstr *));
    if (!tokens) {
        free(str_cpy.str);
        *count = 0;
        return NULL; 
    }
    size_t token_count = 0;

    token = strtok(str_cpy.str, delimiter->str);
    while (token != NULL) {
        if (token_count >= capacity) {
            capacity *= 2;
            vstr **new_tokens = realloc(tokens, capacity * sizeof(vstr *));
            if (!new_tokens) {
                for (size_t i = 0; i < token_count; ++i) {
                    free(tokens[i]->str);
                    free(tokens[i]);
                }
                free(tokens);
                free(str_cpy.str);
                *count = 0;
                return NULL; 
            }
            tokens = new_tokens;
        }
        tokens[token_count] = malloc(sizeof(vstr));
        if (!tokens[token_count]) {
            for (size_t i = 0; i < token_count; ++i) {
                free(tokens[i]->str);
                free(tokens[i]);
            }
            free(tokens);
            free(str_cpy.str);
            *count = 0;
            return NULL; 
        }
        size_t token_len = strlen(token);
        char * token_cpy = malloc((token_len + 1) * sizeof(char));
        token_cpy = memcpy(token_cpy, token, token_len  + 1);

        tokens[token_count]->str = token_cpy;
        tokens[token_count]->len = token_len;
        ++token_count;
        token = strtok(NULL, delimiter->str);
    }

    free(str_cpy.str);
    *count = token_count;
    return tokens;
}

vstr vstr_join(vstr *delimiter, size_t count, ...) {
    size_t total_len = 0;
    size_t delimiter_len = delimiter->len;

    //find needed length 
    va_list args;
    va_start(args, count);
    for (size_t i = 0; i < count; ++i) {
        vstr *v = va_arg(args, vstr *);
        total_len += v->len;
        if (i < count - 1) {
            total_len += delimiter_len;
        }
    }
    va_end(args);

    vstr res;
    res.str = (char *)calloc(total_len + 1, sizeof(char));
    if (!res.str) {
        return (vstr){NULL, 0};
    }
    res.len = total_len;

    va_start(args, count);
    for (size_t i = 0; i < count; ++i) {
        vstr *v = va_arg(args, vstr *);
        strcat(res.str, v->str);
        if (i < count - 1) {
            strcat(res.str, delimiter->str);
        }
    }
    va_end(args);
    res.own = true;
    return res;
}

void vstr_trim(vstr* str, const vstr* substr) {
    if (str == NULL || !str->own) return;
    
    size_t str_len = str->len;
    size_t substr_len = substr->len;

    if (substr_len == 0) return;
    if (substr_len > str_len) return;

    char *result = malloc(str_len + 1);
    if (!result) return;

    char *p = str->str;
    char *r = result;

    while (*p)
    {
        if (strncmp(p, substr->str, substr_len) == 0)
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
    strcpy(str->str, result);
    free(result);
    str = realloc(str->str, result_len);
}

bool vstr_remove(vstr* str, size_t pos, size_t len, bool __realloc)
{
    if (!str || pos >= str->len || len == 0 || !str->own) return false;

    if (pos + len > str->len) len = str->len - pos;

    memmove(str->str + pos, str->str + pos + len, str->len - (pos + len));

    if(__realloc){

        char * __new = (char*)realloc(str->str, (len + 1) * sizeof(char));
        if(!__new) return false;

        str->len -= len;

        str->str[str->len] = '\0';
    }
    return true;
}

void vstr_replace(vstr* str, const vstr* old_substr, const vstr* new_substr, bool force)
{
    if(!str || !old_substr || !new_substr || !str->own || strcmp(str->str, new_substr->str) == 0) return;

    size_t str_len = str->len;
    size_t old_substr_len = old_substr->len;
    size_t new_substr_len = new_substr->len;

    //if false we will realloc and instert even though new_substr_len > old_substr_len 
    if(new_substr_len > old_substr_len && !force) return;


    vstr view;
    
    size_t pos = 0;
    while ((pos = vstr_findf(&view, str, old_substr)) != -1) {
        
        //remove old substring
        vstr_remove(str, pos, old_substr_len, true);

        //insert new substring
        vstr_insert(str, pos, new_substr);
        
        //adjust position to continue searching
        pos += new_substr_len;
    }
}

void vstr_destroy(vstr *str) {
    if (str && str->own) {
        free(str->str);
        str->str = NULL;
        str->len = 0;
        str->own = false;
    }
}
