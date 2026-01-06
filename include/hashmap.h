#pragma once
#ifndef __FELIX_HASHMAP_H__
#define __FELIX_HASHMAP_H__

#include <stddef.h>
#include <stdint.h>

typedef uint64_t (*hash_fn_t)(const void* key, size_t key_size);
typedef int (*cmp_fn_t)(const void* key1, const void* key2, size_t key_size);
typedef void* (*key_copy_fn_t)(const void* key, size_t key_size);
typedef void* (*value_copy_fn_t)(const void* value, size_t value_size);
typedef void (*key_free_fn_t)(void* key);
typedef void (*value_free_fn_t)(void* value);

typedef struct hashmap_t  hashmap_t;
typedef struct hashnode_t hashnode_t;

hashmap_t* hashmap_create(size_t key_size, size_t value_size, size_t capacity, float max_load_factor, hash_fn_t hash,
    cmp_fn_t cmp, key_copy_fn_t key_copy, value_copy_fn_t value_copy, key_free_fn_t key_free,
    value_free_fn_t value_free);

#define hashmap_create_default(key_size, item_size)                                                                    \
    hashmap_create(key_size, item_size, 0, 0.75f, NULL, NULL, NULL, NULL, NULL, NULL)

void* hashmap_get(hashmap_t* map, void* key);
int   hashmap_has(hashmap_t* map, void* key);
int   hashmap_set(hashmap_t* map, void* key, void* value);
int   hashmap_remove(hashmap_t* map, void* key);

void hashmap_destroy(hashmap_t* map);

#endif // __FELIX_HASHMAP_H__
