#include <stdio.h>
#include <vsk.h>

int main() 
{   
    vsk1_t * sk = vsk1_create(int, 3, .75, NULL, NULL, NULL);
    int item = 8;
    int* d = NULL;
    vsk_emplace(sk, 1, &item);
    d = vsk_peek(sk);
    printf("top is %d\n", *d);
    item = 7;
    vsk_push(sk, &item);
    vsk_push(sk, &item);
    vsk_push(sk, &item);
    vsk_push(sk, &item);
    item = 6;
    vsk_push(sk, &item);
    d = vsk_peek(sk);
    printf("top is %d\n", *d);

    int it = 0x0;
    vsk_foreach(int, it, sk, 

        printf("cur index is: %d\n", it);
        
    );
    

    vstack_destroy(&sk);
    d = vstack_peek(sk);
    printf("top is %d\n", *d);
    return 0;
}