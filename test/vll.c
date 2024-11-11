#include <vll.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    
    vll1_t* ll = vll_create(size_t, 0);
    // emplace_back
    vll_emplace_back(ll, 1, 1);
    vll_emplace_back(ll, 1, 3);
    vll_emplace_back(ll, 1, 2);
    // push_back
    size_t 
    t = 4;
    vll_push_back(ll, &t);
    t = 5;
    //vll_emplace
    vll_emplace(ll, -1, 1, 999);
    t = 6;
    vll_insert(ll, 0, &t);
    //VLL_FIELD_LENGTH
    printf("len = %llu\n", vll_get_field(ll, VLL_FIELD_LENGTH));
    printf("[4] = %llu\n", *(size_t*)vll_at(ll, 4));
    vll_foreach(size_t, item, i,ll, printf("%llu\n", *item););

    //vll_erase
    vll_clear(ll);

    vll_foreach(size_t, item, i,ll, printf("%llu\n", *item););
    
    for (char i = 0; i < 100; i++)
    {
        vll_emplace_back(ll, 1, i);
    }
       
    printf("___________\n");
    vll_reverse(ll);
    vll_foreach(size_t, item, i, ll, printf("%llu\n", *item););
    printf("___________\n");
    vll_reverse(ll);
    vll_foreach(size_t, item, i, ll, printf("%llu\n", *item););
    printf("___________\n");
    vll_pop_at(ll, 99);
    vll_pop_at(ll, 0);
    vll_foreach(size_t, item, i,ll, printf("%llu\n", *item););

    vll_clear(ll);
    //vll_pop_at(ll, -1); // tested error
    vll_foreach(size_t, item, i,ll, printf("%llu\n", *item););
    vll_resize(ll, 2);
    vll_foreach(size_t, item, i,ll, printf("%llu\n", *item););
    vll_destroy(&ll);
    return -1;
}