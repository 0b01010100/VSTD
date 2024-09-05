
#include <vvector.h>

int main() 
{   
    vvector1* d = vvector1_create(int, 2, 0.75, NULL, NULL, NULL);
    int a = 29;
    vvector_push_back(d, &a);
    const int* t = vvector_at(d, 0);
    printf("%i", *t);
    return 0;
}