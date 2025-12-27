#pragma once
#ifndef __FELIX_HASHMAP_HASH_H__
#define __FELIX_HASHMAP_HASH_H__

#include <stddef.h>
#include <stdint.h>

uint64_t fnv1a_hash(const void* key, size_t key_size);

#endif // __FELIX_HASHMAP_HASH_H__
