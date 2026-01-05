#include "hash.h"
#include "fnv1a.h"

uint64_t
fnv1a_hash(const void* key, size_t key_size)
{
    const uint8_t* data = (const uint8_t*)key;
    uint64_t hash = FNV1A_64_OFFSET;

    for (size_t i = 0; i < key_size; i++) {
        hash ^= data[i];
        hash *= FNV1A_64_PRIME;
    }

    return hash;
}
