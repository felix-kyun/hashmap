#pragma once
#ifndef __FELIX_HASHMAP_STRING_H__
#define __FELIX_HASHMAP_STRING_H__

#include <stddef.h>
#include <stdint.h>

/**
 * Hash function for strings using FNV-1a algorithm.
 * The key_size is ignored.
 */
uint64_t fnv1a_string_hash(const void* key, size_t key_size);

/**
 * Function to compare two strings.
 * The size param is ignored.
 */
int hashmap_string_cmp(const void* key1, const void* key2, size_t size);

#define hashmap_create_string(item_size) \
    hashmap_create(0, item_size, 0, 0.75f, fnv1a_string_hash, hashmap_string_cmp, NULL, NULL)

#endif // __FELIX_HASHMAP_STRING_H__
