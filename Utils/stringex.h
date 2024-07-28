#pragma once
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

//INIT FUCTIONS

char * strmalloc_ex(size_t size);

char * strcalloc_ex(size_t size);

char * strfastinit_ex(const char * original);

char * strfastinit_printf_ex(const char* format, ...);

char * strsub_ex(const char * src, size_t sub_start, size_t sub_end);

char * strnsub_ex(const char * src, size_t src_size, size_t sub_start, size_t sub_end);

///////////////////////////////

char* strinsert_ex(char* dest, size_t index, const char* substr);

char* strninsert_ex(char* dest, size_t dest_size, size_t index, const char* substr);

char* strninsert_s_ex(char* dest, size_t dest_size, size_t index, const char* substr, size_t ss_len);

char * strupper_ex(char * str);

char * strlower_ex(char * str);

char * strrev_ex(char * str);

char * strfill_ex(char * str, const char val);

char strget_ex(const char * str, ssize_t index);

char * strset_ex(char * str, ssize_t index, const char val);

char * strapn_ex(char * str, const double number);

char * strapstr_ex(char * dest, const char * src);

char* strinsert_ex(char* dest, size_t index, const char* substr);

char * strninsert_ex(char * dest, size_t dest_size, size_t index, const char * substr);

char * strfindl_ex(const char* str, const char* substr);

char * strfindf_ex(const char* str, const char* substr);

#define strcontains_ex(str, substr) (strfindf(str, substr) != NULL)

char * strtrim_ex(char * str, const char * substr);

char * strrem_ex(char * str, const char * substr);

void strfree_ex(char * src);
