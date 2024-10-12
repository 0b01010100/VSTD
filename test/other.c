#include <vdef.h>

#define vtypeof(x) _Generic((x), \
        _Bool: "_Bool", \
        char: "char", \
        unsigned char: "unsigned char", \
        signed char: "char", \
        short: "short", \
        unsigned short: "unsigned short", \
        int: "int", \
        unsigned int: "unsigned int", \
        long: "long", \
        unsigned long: "unsigned long", \
        long long: "long long", \
        unsigned long long: "unsigned long long", \
        float: "float", \
        double: "double", \
        long double: "long double", \
        _Bool *: "_Bool*", \
        char *: "char*", \
        signed char *: "char*", \
        unsigned char *: "unsigned char*", \
        short *: "short*", \
        unsigned short *: "unsigned short*", \
        int *: "int*", \
        unsigned int *: "unsigned int*", \
        long *: "long*", \
        unsigned long *: "unsigned long*", \
        float *: "float*", \
        double *: "double*", \
        long double *: "long double*", \
        void *: "void*", \
        default: "unknown")

int main(){
    int X = 0;
    char * g = vtypeof(X);
}