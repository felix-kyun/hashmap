#pragma once
#ifndef __FELIX_HASHMAP_STRING_H__
#define __FELIX_HASHMAP_STRING_H__

#include <stddef.h>
#include <stdint.h>

/**
 * Hash function for strings using FNV-1a algorithm.
 * The key_size is ignored.
 */
uint64_t fnv1a_hash_string(const void* key, size_t key_size);

/**
 * Function to compare two strings.
 * The size param is ignored.
 */
int hashmap_cmp_string(const void* key1, const void* key2, size_t size);

/**
 * Function to copy a string key
 * The size param is ignored.
 */
void* hashmap_key_copy_string(const void* key, size_t size);

/**
 * Function to copy a string value
 * The size param is ignored.
 */
void* hashmap_value_copy_string(const void* value, size_t size);

/**
 * Function to free a string key
 */
void hashmap_key_free_string(void* key);

/**
 * Function to free a string value
 */
void hashmap_value_free_string(void* value);

#define hashmap_create_string(item_size)                                                                               \
    hashmap_create(0, item_size, 0, 0.75f, fnv1a_hash_string, hashmap_cmp_string, hashmap_key_copy_string, NULL,       \
        hashmap_key_free_string, NULL)

#endif // __FELIX_HASHMAP_STRING_H__
