#include <Utils/inc/ht.h>
typedef char* cstring;
// decl_ht_t(cstring, int);
// def_ht_t(cstring, int);
// decl_ht_t_create(cstring, int);
#define cstring_ht_dtor(cp) free(cp)
#define cstring_ht_equal(cp1, cp2) strcmp(cp1, cp2) == 0
char* cstring_copy(char* cp) {
    // Allocate memory for the copy
    char* ret = malloc(strlen(cp) + 1);
    memset(ret, 0, strlen(cp) + 1);
    if (!ret) {
        return NULL; // Handle memory allocation failure
    }

    char* temp = ret;
    while (*cp) {
        *temp++ = *cp++;
    }
    *temp = '\0'; // Null-terminate the copied string
    return ret;
}

size_t cstring_ht_hash(char* cp) {
    size_t ret = 0;
    const size_t len = strlen(cp);
    __HASHFUNC_FNV1A(ret, cp, len);
    return ret;
}

typedef struct ___ht_entry_cstring_cstring {
    cstring key; cstring val; _Bool is_set;
} ___ht_entry_cstring_cstring; typedef struct __ht_cstring_cstring {
    ___ht_entry_cstring_cstring* entries; size_t size; size_t length; double max_load_factor;
} cstring_cstring_ht_t; cstring_cstring_ht_t cstring_cstring_ht_create(); cstring_cstring_ht_t cstring_cstring_ht_create_ex(size_t ht_arr_capacity, double max_load_factor); void cstring_cstring_ht_destroy(cstring_cstring_ht_t* ht); void cstring_cstring_ht_set(cstring_cstring_ht_t* ht, cstring key, cstring val); cstring cstring_cstring_ht_get(cstring_cstring_ht_t* ht, cstring key); _Bool cstring_cstring_ht_exists(cstring_cstring_ht_t* ht, cstring key); void cstring_cstring_ht_remove(cstring_cstring_ht_t* ht, cstring key); void cstring_cstring_ht_resize(cstring_cstring_ht_t* ht); void cstring_cstring_ht_clear(cstring_cstring_ht_t* ht); cstring_cstring_ht_t cstring_cstring_ht_merge(cstring_cstring_ht_t* ht1, cstring_cstring_ht_t* ht2);;
cstring_cstring_ht_t cstring_cstring_ht_create() {
    return cstring_cstring_ht_create_ex(10000, 0.75);
} cstring_cstring_ht_t cstring_cstring_ht_create_ex(size_t ht_arr_capacity, double max_load_factor) {
    cstring_cstring_ht_t ht = { 0 }; ht.entries = (___ht_entry_cstring_cstring*)malloc(sizeof(___ht_entry_cstring_cstring) * ht_arr_capacity); memset(ht.entries, 0, sizeof(___ht_entry_cstring_cstring) * ht_arr_capacity); ht.size = ht_arr_capacity; ht.max_load_factor = max_load_factor; return ht;
} void cstring_cstring_ht_destroy(cstring_cstring_ht_t* ht) {
    for (int i = 0; i < ht->size; i++) {
        ___ht_entry_cstring_cstring cur = ht->entries[i]; if (!cur.is_set) {
            continue;
        } free(cur.key); free(cur.val); ht->entries[i] = (___ht_entry_cstring_cstring){ 0 };
    } free(ht->entries); ht->entries = ((void*)0); ht->size = 0; ht->length = 0; ht->max_load_factor = 0;
} void cstring_cstring_ht_set(cstring_cstring_ht_t* ht, cstring key, cstring val) {
    const uint64_t powers_of_2[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296 }; 
    size_t index = cstring_ht_hash(key) % ht->size; 
    ___ht_entry_cstring_cstring* current_entry = &ht->entries[index]; 
    uint32_t power_of_two = 0; 
    while (current_entry->is_set) {
        _Bool equal = strcmp(current_entry->key, key) == 0; 
        if (equal) {
            free(current_entry->val); current_entry->val = cstring_copy(val); break;
        }
        else {
            if (power_of_two == 33) {
                power_of_two = 0;
            } index = (index + powers_of_2[power_of_two++]) % ht->size; current_entry = &ht->entries[index];
        }
    } if (!current_entry->is_set) {
        ht->length++; current_entry->is_set = 1; 
        current_entry->key = cstring_copy(key); 
        current_entry->val = cstring_copy(val);
    } if ((double)ht->length / ht->size > ht->max_load_factor) {
        cstring_cstring_ht_resize(ht);
    }
} cstring cstring_cstring_ht_get(cstring_cstring_ht_t* ht, cstring key) {
    const uint64_t powers_of_2[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296 }; size_t index = cstring_ht_hash(key) % ht->size; ___ht_entry_cstring_cstring* current_entry = &ht->entries[index]; uint32_t power_of_two = 0; while (current_entry->is_set) {
        _Bool equal = strcmp(current_entry->key, key) == 0; if (equal) {
            return cstring_copy(current_entry->val);
        }
        else {
            if (power_of_two == 33) {
                power_of_two = 0;
            } index = (index + powers_of_2[power_of_two++]) % ht->size; current_entry = &ht->entries[index];
        }
    } (void)((!!(0 && "key not found")) || (_wassert(L"false && \"key not found\"", L"C:\\Users\\josdd\\repos\\p\\VUtilities\\test\\main.c", (unsigned)(31)), 0));
} _Bool cstring_cstring_ht_exists(cstring_cstring_ht_t* ht, cstring key) {
    const uint64_t powers_of_2[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296 }; size_t index = cstring_ht_hash(key) % ht->size; ___ht_entry_cstring_cstring* current_entry = &ht->entries[index]; uint32_t power_of_two = 0; while (current_entry->is_set) {
        _Bool equal = strcmp(current_entry->key, key) == 0; if (equal) {
            return 1;
        }
        else {
            if (power_of_two == 33) {
                power_of_two = 0;
            } index = (index + powers_of_2[power_of_two++]) % ht->size; current_entry = &ht->entries[index];
        }
    } return 0;
} void cstring_cstring_ht_remove(cstring_cstring_ht_t* ht, cstring key) {
    const uint64_t powers_of_2[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296 }; size_t index = cstring_ht_hash(key) % ht->size; ___ht_entry_cstring_cstring* current_entry = &ht->entries[index]; uint32_t power_of_two = 0; while (current_entry->is_set) {
        _Bool equal = strcmp(current_entry->key, key) == 0; if (equal) {
            free(current_entry->val); current_entry->is_set = 0; ht->length--; break;
        }
        else {
            if (power_of_two == 33) {
                power_of_two = 0;
            } index = (index + powers_of_2[power_of_two++]) % ht->size; current_entry = &ht->entries[index];
        }
    } if ((double)ht->length / ht->size > ht->max_load_factor) {
        cstring_cstring_ht_resize(ht);
    }
} void cstring_cstring_ht_resize(cstring_cstring_ht_t* ht) {
    const uint64_t powers_of_2[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296 }; uint32_t power_of_two = 0; cstring_cstring_ht_t new_ht = cstring_cstring_ht_create_ex(ht->size * 2, ht->max_load_factor); for (size_t i = 0; i < ht->size; i++) {
        if (!ht->entries[i].is_set) {
            continue;
        } ___ht_entry_cstring_cstring cur = ht->entries[i]; size_t index = cstring_ht_hash(cur.key) % new_ht.size; ___ht_entry_cstring_cstring* current_entry = &new_ht.entries[index]; uint32_t power_of_two = 0; while (current_entry->is_set) {
            if (power_of_two == 33) {
                power_of_two = 0;
            } index = (index + powers_of_2[power_of_two++]) % new_ht.size; current_entry = &new_ht.entries[index];
        } current_entry->is_set = 1; current_entry->key = cur.key; current_entry->val = cur.val; new_ht.length++;
    } free(ht->entries); *ht = new_ht;
} void cstring_cstring_ht_clear(cstring_cstring_ht_t* ht) {
    for (int i = 0; i < ht->size; i++) {
        ___ht_entry_cstring_cstring cur = ht->entries[i]; if (!cur.is_set) {
            continue;
        } free(cur.key); free(cur.val); ht->entries[i] = (___ht_entry_cstring_cstring){ 0 };
    } ht->length = 0;
} cstring_cstring_ht_t cstring_cstring_ht_merge(cstring_cstring_ht_t* ht1, cstring_cstring_ht_t* ht2) {
    const uint64_t powers_of_2[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296 }; 
    cstring_cstring_ht_t new_ht = { 0 }; 
    const cstring_cstring_ht_t biggest_ht = ht1->size > ht2->size ? *ht1 : *ht2; 
    const _Bool length_not_fits_load = (ht1->length + ht2->length) / biggest_ht.size > biggest_ht.max_load_factor; 
    if (length_not_fits_load) {
        new_ht = cstring_cstring_ht_create_ex(biggest_ht.size, biggest_ht.max_load_factor);
        for (size_t i = 0; i < biggest_ht.size; i++) {
            ___ht_entry_cstring_cstring cur = biggest_ht.entries[i]; if (!cur.is_set) {
                continue;
            } size_t index = cstring_ht_hash(cur.key) % new_ht.size; ___ht_entry_cstring_cstring* current_entry = &new_ht.entries[index]; uint32_t power_of_two = 0; while (current_entry->is_set) {
                if (power_of_two == 33) {
                    power_of_two = 0;
                } index = (index + powers_of_2[power_of_two++]) % new_ht.size; 
                current_entry = &new_ht.entries[index];
            } current_entry->is_set = 1; 
            current_entry->key = cur.key; 
            current_entry->val = (cur.val);
            new_ht.length++;
        }
    }
    else {
        new_ht = ht1->size > ht2->size ? *ht1 : *ht2;
    } 
    const cstring_cstring_ht_t* smallest_ht = ht1->size > ht2->size ? ht2 : ht1; 
    for (size_t i = 0; i < smallest_ht->size; i++) {
        ___ht_entry_cstring_cstring* cur = &smallest_ht->entries[i]; 
        if (!cur->is_set) {
            continue;
        } 
        size_t index = cstring_ht_hash(cur->key) % new_ht.size; 
        ___ht_entry_cstring_cstring* current_entry = &new_ht.entries[index]; 
        uint32_t power_of_two = 0; 
        while (current_entry->is_set) {
            _Bool equal = strcmp(cur->key, current_entry->key) == 0; 
            if (equal) {
                free(current_entry->key); 
                free(current_entry->val); 
                current_entry->key = cur->key; 
                current_entry->val = cur->val; 
                break;
            } if (power_of_two == 33) {
                power_of_two = 0;
            } index = (index + powers_of_2[power_of_two++]) % new_ht.size; 
            current_entry = &new_ht.entries[index];
        } if (!current_entry->is_set) {
            current_entry->is_set = 1; 
            current_entry->key = cur->key; 
            current_entry->val = cur->val; 
            new_ht.length++;
        }
    } if (length_not_fits_load) {
        free(ht1->entries); free(ht2->entries);
    }
    else {
        free(smallest_ht->entries);
    } return new_ht;
}

int main()
{
    printf("\nh\n\n\n");
    cstring_cstring_ht_t ht = cstring_cstring_ht_create(); 
    cstring_cstring_ht_set(&ht, "one", "1");
    cstring_cstring_ht_set(&ht, "two", "2");
    cstring_cstring_ht_set(&ht, "three", "3");
    cstring_cstring_ht_set(&ht, "four", "4");
    for (int i = 0; i < ht.size; i++)
    {
        if (ht.entries[i].is_set) 
        {
            printf("key = %s; val = %s \n", ht.entries[i].key, ht.entries[i].val); 
        }
    }
    printf("\nh0\n\n\n");
    cstring_cstring_ht_t ht0 = cstring_cstring_ht_create();
    cstring_cstring_ht_set(&ht0, "four", " ");
    cstring_cstring_ht_set(&ht0, "four", " ");
    cstring_cstring_ht_set(&ht0, "five", "5");
    cstring_cstring_ht_set(&ht0, "six", "6");
    cstring_cstring_ht_set(&ht0, "seven", "7");
    for (int i = 0; i < ht0.size; i++)
    {
        if (ht0.entries[i].is_set){
            printf("key = %s; val = %s \n", ht0.entries[i].key, ht0.entries[i].val);
        }
    }
    printf("\nh1\n\n\n");
    cstring_cstring_ht_t h1 = cstring_cstring_ht_merge(&ht, &ht0);
    for (int i = 0; i < h1.size; i++)
    {
        if (h1.entries[i].is_set) 
            printf("key = %s; val = %s \n", h1.entries[i].key, h1.entries[i].val); 
    } 


    while (1)
    {}
    

    return -1;
}