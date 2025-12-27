#include "hash.h"

uint64_t
fnv1a_hash(const void* key, size_t key_size)
{
    const uint8_t* data = (const uint8_t*)key;
    uint64_t hash = 0xcbf29ce484222325ULL;

    for (size_t i = 0; i < key_size; i++) {
        hash ^= data[i];
        hash *= 0x00000100000001b3ULL;
    }

    return hash;
}
