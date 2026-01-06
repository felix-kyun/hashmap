#include "hashmap.h"
#include "hash.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct hashmap_t {

    struct hashnode_t** buckets;

    size_t size;
    size_t capacity;
    size_t key_size;
    size_t value_size;
    float  max_load_factor;

    hash_fn_t       hash;
    cmp_fn_t        cmp;
    key_copy_fn_t   key_copy;
    value_copy_fn_t value_copy;
    key_free_fn_t   key_free;
    value_free_fn_t value_free;

} hashmap_t;

typedef struct hashnode_t {
    void*              key;
    void*              value;
    uint64_t           hash;
    struct hashnode_t* next;
} hashnode_t;

static inline int
hashmap_default_cmp(const void* key1, const void* key2, size_t size)
{
    return memcmp(key1, key2, size);
}

static inline void*
hashmap_default_key_copy(const void* key, size_t size)
{
    void* new_key = malloc(size);
    if (!new_key)
        return NULL;

    memcpy(new_key, key, size);
    return new_key;
}

static inline void*
hashmap_default_value_copy(const void* value, size_t size)
{
    void* new_value = malloc(size);
    if (!new_value)
        return NULL;

    memcpy(new_value, value, size);
    return new_value;
}

static inline void
hashmap_default_key_free(void* key)
{
    free(key);
}

static inline void
hashmap_default_value_free(void* value)
{
    free(value);
}

static inline hashnode_t*
create_new_node(hashmap_t* map, void* key, void* value, uint64_t hash)
{
    hashnode_t* node = (hashnode_t*)malloc(sizeof(hashnode_t));
    if (!node)
        return NULL;

    node->hash = hash;
    node->next = NULL;
    node->key  = map->key_copy(key, map->key_size);
    if (!node->key) {
        free(node);
        return NULL;
    }
    node->value = map->value_copy(value, map->value_size);
    if (!node->value) {
        free(node->key);
        free(node);
        return NULL;
    }

    return node;
}

static inline hashnode_t*
hashmap_get_node(hashmap_t* map, void* key)
{
    if (!map || !key)
        return NULL;

    uint64_t hash  = map->hash(key, map->key_size);
    size_t   index = hash % map->capacity;

    hashnode_t* current = map->buckets[index];
    while (current) {
        if (current->hash == hash && map->cmp(key, current->key, map->key_size) == 0)
            return current;
        current = current->next;
    }

    return NULL;
}

static inline void
hashmap_set_existing_node(hashmap_t* map, hashnode_t* node)
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

static int
hashmap_resize(hashmap_t* map)
{
    size_t       old_capacity = map->capacity;
    size_t       old_size     = map->size;
    hashnode_t** old_buckets  = map->buckets;
    map->capacity             = map->capacity * 2;
    map->size                 = 0;
    map->buckets              = (hashnode_t**)calloc(map->capacity, sizeof(hashnode_t*));

    if (!map->buckets) {
        map->capacity = old_capacity;
        map->size     = old_size;
        map->buckets  = old_buckets;
        return -1;
    }

    for (size_t i = 0; i < old_capacity; i++) {
        hashnode_t* current = old_buckets[i];
        while (current) {
            hashnode_t* next = current->next;
            current->next    = NULL;
            hashmap_set_existing_node(map, current);
            current = next;
        }
    }
    free(old_buckets);
    return 0;
}

hashmap_t*
hashmap_create(size_t key_size, size_t value_size, size_t capacity, float max_load_factor, hash_fn_t hash, cmp_fn_t cmp,
    key_copy_fn_t key_copy, value_copy_fn_t value_copy, key_free_fn_t key_free, value_free_fn_t value_free)
{
    hashmap_t* map = (hashmap_t*)malloc(sizeof(hashmap_t));
    if (!map)
        return NULL;

    if (capacity < 16)
        capacity = 16;
    size_t cap = 1;
    while (cap < capacity)
        cap <<= 1;

    map->size     = 0;
    map->capacity = cap;

    map->key_size        = key_size;
    map->value_size      = value_size;
    map->max_load_factor = max_load_factor ? max_load_factor : 0.75f;

    map->buckets = (hashnode_t**)calloc(cap, sizeof(hashnode_t*));
    if (!map->buckets) {
        free(map);
        return NULL;
    }

    map->hash       = hash ? hash : fnv1a_hash;
    map->cmp        = cmp ? cmp : hashmap_default_cmp;
    map->key_copy   = key_copy ? key_copy : hashmap_default_key_copy;
    map->value_copy = value_copy ? value_copy : hashmap_default_value_copy;
    map->key_free   = key_free ? key_free : hashmap_default_key_free;
    map->value_free = value_free ? value_free : hashmap_default_value_free;

    return map;
}

int
hashmap_set(hashmap_t* map, void* key, void* value)
{
    if (!map || !key || !value)
        return -1;

    uint64_t hash = map->hash(key, map->key_size);

    hashnode_t* existing = hashmap_get_node(map, key);
    if (existing) {
        void* old_value = existing->value;
        void* new_value = map->value_copy(value, map->value_size);
        if (!new_value)
            return -1;
        map->value_free(old_value);
        existing->value = new_value;
    } else {
        hashnode_t* new_node = create_new_node(map, key, value, hash);
        if (!new_node)
            return -1;

        hashmap_set_existing_node(map, new_node);
    }

    if (hashmap_needs_resize(map))
        return hashmap_resize(map);

    return 0;
}

void*
hashmap_get(hashmap_t* map, void* key)
{
    hashnode_t* existing = hashmap_get_node(map, key);
    return existing ? existing->value : NULL;
}

int
hashmap_has(hashmap_t* map, void* key)
{
    return hashmap_get_node(map, key) != NULL;
}

int
hashmap_remove(hashmap_t* map, void* key)
{
    if (!map || !key)
        return -1;

    uint64_t hash  = map->hash(key, map->key_size);
    size_t   index = hash % map->capacity;

    hashnode_t* current = map->buckets[index];
    hashnode_t* prev    = NULL;
    while (current) {
        if (current->hash == hash && map->cmp(key, current->key, map->key_size) == 0) {

            if (prev)
                prev->next = current->next;
            else
                map->buckets[index] = current->next;

            map->size--;

            map->key_free(current->key);
            map->value_free(current->value);
            free(current);
            return 0;
        }
        prev    = current;
        current = current->next;
    }

    return -1;
}

void
hashmap_destroy(hashmap_t* map)
{
    if (!map)
        return;

    for (size_t i = 0; i < map->capacity; i++) {
        hashnode_t* current = map->buckets[i];

        while (current) {
            hashnode_t* next = current->next;

            map->key_free(current->key);
            map->value_free(current->value);
            free(current);
            current = next;
        }
    }

    free(map->buckets);
    free(map);
}
