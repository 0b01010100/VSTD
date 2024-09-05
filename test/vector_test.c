
#include <vvector.h>

int main() 
{   
    vvector1* v = vvector1_create(int, 2, 0.75, NULL, NULL, NULL);

    printf("is_empty = %d \n", vvector_empty(v));

    int a = 29;
    vvector_push_back(v, &a);
    int b = 12;
    vvector_push_back(v, &b);
    int c = 79;
    vvector_emplace_back(v, 1, &a);

    int d = 792;
    vvector_insert(v, 0, &d);
    const int* t = vvector_at(v, 0);
    printf("%i\n", *t); 
    vvector_emplace(v, 2, 1, &d);
    t = vvector_at(v, 2);
    printf("%i\n", *t); 

    printf("is_empty = %d \n", vvector_empty(v));
    
    vvector_clear(v);
    printf("is_empty = %d \n", vvector_empty(v));

    vvector_push_back(v, &a);
    vvector_push_back(v, &a);
    vvector_push_back(v, &a);
    vvector_push_back(v, &a);
    vvector_erase(v, 0, 2);

    printf("size is = %d\n", vvector_get_field(v, VVECTOR_FIELD_LENGTH));
    vvector_reserve(v, 2);

    printf("is_empty = %d \n", vvector_empty(v));
    return 0;
}