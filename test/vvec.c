
#include <vvec.h>
#include <stdio.h>
#include <stdlib.h>
#include <stringex.h>
#include <vtypes.h>

int str_ctor(void * self, size_t size, va_list args, size_t count)
{   
    char* arg = va_arg(args, char*);
    char** str_ptr = (char**)self;
    *str_ptr = malloc(sizeof(char) * (strlen(arg) + 1));
    if(!*str_ptr) return -1;
    strcpy(*str_ptr, arg);
    return 0;
}

int str_cctor(void * self, const void * original, size_t size)
{   
    char** str_ptr = (char**)self;
    const char* orig = *(const char**)original;
    *str_ptr = malloc(sizeof(char) * (strlen(orig) + 1));
    if(!*str_ptr) return -1;
    strcpy(*str_ptr, orig);
    return 0;
}

void str_dtor(void * self, size_t size)
{
    char** str_ptr = (char**)self;
    free(*str_ptr);
    *str_ptr = NULL;
}

int main() 
{   
    vvec1_t* v = vvec1_create(int, 2, 0.75, NULL, NULL, NULL);

    printf("is_empty = %d \n", vvec_empty(v));

    int a = 29;
    vvec_push_back(v, &a);
    int b = 12;
    vvec_push_back(v, &b);
    int c = 79;
    vvec_emplace_back(v, 1, &c);

    vvec_foreach(int, item, i, v, 
        printf("%d\n", *item);
    );

    printf("size is = %llu\n", vvec_get_field(v, VVEC_FIELD_LENGTH));

    vvec_erase(v, 0, 2);

    printf("size is = %llu\n", vvec_get_field(v, VVEC_FIELD_LENGTH));

    printf("is_empty = %d \n", vvec_empty(v));

    vvec_foreach(int, item, i, v, 
        printf("%d\n", *item);
    );

    vvec_clear(v);
    printf("is_empty = %d \n", vvec_empty(v));
    printf("size is = %llu\n", vvec_get_field(v, VVEC_FIELD_LENGTH));

    //vvec_insert(v, 0, &a); error
    vvec_foreach(int, item, i, v, 
        printf("%d\n", *item);
    );

    vvec_reserve(v, 2);

    vvec_push_back(v, &a);
    vvec_push_back(v, &b);
    vvec_push_back(v, &c);
    printf("a,b,c added. size is = %llu\n", vvec_get_field(v, VVEC_FIELD_LENGTH));

    vvec_pop_at(v, 2);
    vvec_foreach(int, item, i, v, 
        printf("%d\n", *item);
    );
    vvec_destroy(&v);

    
    

    vvec1_t* v0 = vvec1_create(vcp, 0, 0/*def*/, str_ctor, str_cctor, str_dtor);
    if (!v0) {
        fprintf(stderr, "Failed to create string vector\n");
        return 1;
    }

    vcp my = "my";
    vcp vec = "vec";
    if (vvec_push_back(v0, &my) != 0 || vvec_push_back(v0, &vec) != 0) {
        fprintf(stderr, "Failed to push strings to vector\n");
        vvec_destroy(&v0);
        return 1;
    }

    vvec_foreach(vcp, item, i, v0, 
        printf("%s\n", *item);
    );

    vcp* d = vvec_at(v0, 0);
    if (d && *d) {
        printf("%s\n", *d);
    } else {
        fprintf(stderr, "Failed to access string at index 0\n");
    }

    vvec_destroy(&v0);
    printf("is_empty = %d \n", vvec_empty(v0));
    return 0;
}