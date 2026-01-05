#define _GNU_SOURCE
#include "hashmap_string.h"
#include "fnv1a.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint64_t
fnv1a_hash_string(const void *_key, size_t key_size)
{
    (void)key_size;
    uint64_t hash = FNV1A_64_OFFSET;

    const uint8_t* key = (const uint8_t*)_key;
    while (*key) {
        hash ^= *key++;
        hash *= FNV1A_64_PRIME;
    }

    return hash;
}

int
hashmap_cmp_string(const void* key1, const void* key2, size_t size)
{
    (void)size;
    const char* str1 = (const char*)key1;
    const char* str2 = (const char*)key2;

    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return (*str1 < *str2) ? -1 : 1;
        }
        str1++;
        str2++;
    }

    if (*str1 == *str2) {
        return 0;
    } else {
        return (*str1 < *str2) ? -1 : 1;
    }
}

void*
hashmap_key_copy_string(const void* key, size_t size)
{
    (void)size;
    return strdup(key);
}

void
hashmap_key_free_string(void* key)
{
    free(key);
}
