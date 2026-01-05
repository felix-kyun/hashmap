#pragma once
#ifndef __FELIX_HASHMAP_H__
#define __FELIX_HASHMAP_H__

#include <stddef.h>
#include <stdint.h>

typedef uint64_t (*hash_fn_t)(const void* key, size_t key_size);
typedef int (*hash_cmp_fn_t)(const void* key1, const void* key2, size_t key_size);
typedef void (*hash_key_free_fn_t)(void* key);
typedef void (*hash_value_free_fn_t)(void* value);

typedef struct hashmap_t {

    struct hashnode_t** buckets;

    size_t size;
    size_t capacity;
    size_t key_size;
    size_t item_size;
    float max_load_factor;

    hash_fn_t hash;
    hash_cmp_fn_t cmp;
    hash_key_free_fn_t key_free;
    hash_value_free_fn_t value_free;

} hashmap_t;

typedef struct hashnode_t {
    void* key;
    void* value;
    uint64_t hash;
    struct hashnode_t* next;
} hashnode_t;

hashmap_t* hashmap_create(
    size_t key_size,
    size_t item_size,
    size_t capacity,
    float max_load_factor,
    hash_fn_t hash,
    hash_cmp_fn_t cmp,
    /* Not Used */
    hash_key_free_fn_t key_free,
    hash_value_free_fn_t value_free);

#define hashmap_create_default(key_size, item_size) \
    hashmap_create(key_size, item_size, 0, 0.75f, NULL, NULL, NULL, NULL)

int hashmap_has(hashmap_t* map,  void* key);
void* hashmap_get(hashmap_t* map,  void* key);
int hashmap_set(hashmap_t* map,  void* key,  void* value);
int hashmap_remove(hashmap_t* map,  void* key);

void hashmap_destroy(hashmap_t* map);

#endif // __FELIX_HASHMAP_H__
