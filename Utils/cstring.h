#pragma once
#include <stdbool.h> // for bool type
#include <stdint.h> // for size_t -> long long
//char *
typedef char* cstring;

//input cstring 
#define cstring_in const cstring
//opur cstring 
#define cstring_out cstring

//gets lenght of the cstring
size_t cstring_length(cstring_in str);
/*
    concats two cstrings.
    @param src0 -> first cstring 
    @param src1 -> second cstring 
    @param src1_len -> lenght of first cstring 
    @param out -> output cstring 
*/
size_t cstring_concat(cstring_in src0, cstring_in src1, size_t src1_len, cstring_out out);
/*
    concats two cstrings.
    @param src -> cstring to append data to
    @param len -> length of second string 
    @param in -> cstring to append
    @return size_t of new the cstring
*/
size_t cstring_append_s(cstring_in src, size_t len, cstring_out in);
/*
    concats two cstrings.
    @param number -> number to append data to
    @param out -> output cstring
    @return size_t of new the cstring
*/
size_t cstring_append_n(double number, cstring_out out);
/*
    copies the contents of a cstring
    @param number -> in
    @param out -> output cstring
    @return size_t of new the cstring
*/
size_t cstring_copy(cstring_in in, cstring_out out);
/*
    copies the contents of a cstring from start pointer to end pointer
    @param src_start -> start pointer(where to start copying in the cstring)
    @param src_end -> end pointer(where to end copying in the cstring)
    @param out -> output cstring
    @return size_t of new the cstring
*/
size_t cstring_substr_a(cstring_in src_start, cstring_in src_end, cstring_out out);
/*
    copies the contents of a cstring from start index to end index
    @param src -> source cstring
    @param index_start -> end cstring index(where to end copying in the cstring)
    @param index_end -> end cstring index(where to end copying in the cstring)
    @param out -> output cstring
    @return size_t of new the cstring
*/
size_t cstring_substr_i(cstring_in src, size_t index_start, size_t index_end, cstring_out out);
/*
    checks if the cstring is a number
    @param src -> source cstring
    @return true = is number
*/
bool cstring_is_number(cstring_in src);
/*
    Returns a pointer to the first occurrence of character and or cstring in the cstring str.
    @param src -> source string
    @param contents -> contents to check for
    @return pointer to the first occurrence of character and or cstring in the cstring str.
*/
cstring_out cstring_contians(cstring_in src, cstring_in contents);
/*
    compares two cstrings
    src0 -> first to compare to
    src1 -> second to compare to
    return -1 = error | 0 = don't match | 1 = match
*/
int cstring_compare(cstring_in src0, cstring_in src1);
/*
    removes all white spaces from the cstring
    str_to_trim -> src cstring to remove white spaces from
    return size_t of the cstring now
*/
size_t trim(cstring_out str_to_trim);
/*
    removes all occources of ${str} from the cstring
    src -> src cstring to remove all ${str} from
    str -> charcter[s] to remove from the cstring
    return size_t of the cstring now
*/
size_t remove_all(cstring_out src, cstring_in str);