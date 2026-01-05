#include "hashmap.h"
#include "hash.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int
hashmap_default_cmp(const void* key1, const void* key2, size_t size)
{
    return memcmp(key1, key2, size);
}

static hashnode_t*
create_new_node(void* key, void* value, uint64_t hash)
{
    hashnode_t* node = (hashnode_t*)malloc(sizeof(hashnode_t));
    if (!node)
        return NULL;

    // TODO: copy key and add key_free
    node->key = key;
    node->value = value;
    node->hash = hash;
    node->next = NULL;

    return node;
}

static inline void
hashmap_set_exsisting_node(hashmap_t* map, hashnode_t* node)
{
    size_t index = node->hash % map->capacity;
    if (!map->buckets[index]) {
        map->buckets[index] = node;
    } else {
        hashnode_t* current = map->buckets[index];
        while (current->next) {
            current = current->next;
        }
        current->next = node;
    }

    map->size++;
}

static inline int
hashmap_needs_resize(hashmap_t* map)
{
    return map->size > map->capacity * map->max_load_factor;
}

static void
hashmap_resize(hashmap_t* map)
{
    size_t old_capacity = map->capacity;
    hashnode_t** old_buckets = map->buckets;
    map->capacity = map->capacity * 2;
    map->buckets = (hashnode_t**)calloc(map->capacity, sizeof(hashnode_t*));
    map->size = 0;

    for (size_t i = 0; i < old_capacity; i++) {
        hashnode_t* current = old_buckets[i];
        while (current) {
            hashnode_t* next = current->next;
            current->next = NULL;
            hashmap_set_exsisting_node(map, current);
            current = next;
        }
    }
    free(old_buckets);
}

hashmap_t*
hashmap_create(
    size_t key_size,
    size_t item_size,
    size_t capacity,
    float max_load_factor,
    hash_fn_t hash,
    hash_cmp_fn_t cmp,
    hash_key_free_fn_t key_free,
    hash_value_free_fn_t value_free)
{
    hashmap_t* map = (hashmap_t*)malloc(sizeof(hashmap_t));
    if (!map)
        return NULL;

    if (capacity < 16)
        capacity = 16;
    size_t cap = 1;
    while (cap < capacity)
        cap <<= 1;

    map->size = 0;
    map->capacity = cap;

    map->key_size = key_size;
    map->item_size = item_size;
    map->max_load_factor = max_load_factor ? max_load_factor : 0.75f;

    map->buckets = (hashnode_t**)calloc(cap, sizeof(hashnode_t*));
    if (!map->buckets) {
        free(map);
        return NULL;
    }

    map->hash = hash ? hash : fnv1a_hash;
    map->cmp = cmp ? cmp : hashmap_default_cmp;
    map->key_free = key_free;
    map->value_free = value_free;

    return map;
}

int
hashmap_set(hashmap_t *map, void *key, void *value)
{
    if (!map || !key || !value)
        return -1;

    uint64_t hash = map->hash(key, map->key_size);
    hashnode_t* new_node = create_new_node(key, value, hash);

    hashmap_set_exsisting_node(map, new_node);

    if (hashmap_needs_resize(map))
        hashmap_resize(map);

    return 0;
}

void*
hashmap_get(hashmap_t* map,  void* key)
{
    if (!map || !key)
        return NULL;

    uint64_t hash = map->hash(key, map->key_size);
    size_t index = hash % map->capacity;

    hashnode_t* current = map->buckets[index];
    while (current) {
        if (current->hash == hash && map->cmp(key, current->key, map->key_size) == 0)
            return current->value;
        current = current->next;
    }

    return NULL;
}

int
hashmap_has(hashmap_t* map, void* key)
{
    return hashmap_get(map, key) != NULL;
}

int
hashmap_remove(hashmap_t* map, void* key)
{
    if (!map || !key)
        return -1;

    uint64_t hash = map->hash(key, map->key_size);
    size_t index = hash % map->capacity;

    hashnode_t* current = map->buckets[index];
    hashnode_t* prev = NULL;
    while (current) {
        if (current->hash == hash && map->cmp(key, current->key, map->key_size) == 0) {

            if (prev)
                prev->next = current->next;
            else
                map->buckets[index] = current->next;

            map->size--;

            if (map->key_free)
                map->key_free(current->key);
            if (map->value_free)
                map->value_free(current->value);
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }

    return -1;
}

void
hashmap_destroy(hashmap_t* map)
{
    if(!map)
        return;

    for (size_t i = 0; i < map->capacity; i++) {
        hashnode_t* current = map->buckets[i];

        while (current) {
            hashnode_t* next = current->next;

            if (map->key_free)
                map->key_free(current->key);
            if (map->value_free)
                map->value_free(current->value);
            free(current);
            current = next;
        }
    }

    free(map->buckets);
    free(map);
}
