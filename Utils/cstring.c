#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <cstring.h>


size_t cstring_t_length(const cstring str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

size_t cstring_concat(cstring_in src0, cstring_in src1, cstring_out out) {
    size_t len0 = cstring_length(src0);
    size_t len1 = cstring_length(src1);
    for (size_t i = 0; i < len0; i++) {
        out[i] = src0[i];
    }
    for (size_t i = 0; i < len1; i++) {
        out[len0 + i] = src1[i];
    }
    out[len0 + len1] = '\0';
    return len0 + len1;
}

size_t cstring_append_s(cstring_in src, size_t len, cstring_out in) {
    size_t in_len = cstring_length(in);
    for (size_t i = 0; i < in_len; i++) {
        src[len + i] = in[i];
    }
    src[len + in_len] = '\0';
    return len + in_len;
}

size_t cstring_append_n(double number, cstring_out out) {
    size_t len = cstring_length(out);
    len += sprintf(out + len, "%g", number);
    return len;
}

size_t cstring_copy(cstring_in in, cstring_out out) {
    size_t len = cstring_length(in);
    for (size_t i = 0; i < len; i++) {
        out[i] = in[i];
    }
    out[len] = '\0';
    return len;
}

size_t cstring_substr_a(cstring_in src_start, cstring_in src_end, cstring_out out) {
    size_t len = 0;
    size_t address = src_start;
    while (src_start != src_end) {
        out[len++] = address++;
    }
    out[len] = '\0';
    return len;
}

bool cstring_is_number(cstring_in src) {
    
}