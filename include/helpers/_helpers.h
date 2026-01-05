#pragma once
#ifndef __FELIX_HASHMAP_HELPERS_H__
#define __FELIX_HASHMAP_HELPERS_H__

#include "hashmap.h"
#include <stdlib.h>

#define hashmap_create_default(key_size, item_size) \
    hashmap_create(key_size, item_size, 0, 0.75f, NULL, NULL, NULL, NULL, NULL, NULL)

#define HASHMAP_KEY_REF(type, value) (&(type) { value })
#define HASHMAP_VALUE_REF(type, value) (&(type) { value })

#define HASHMAP_DEFINE_TYPE(key_type, key_name, value_type, value_name)                                            \
    typedef struct hashmap_##key_name##_##value_name##_t {                                                         \
        hashmap_t* __internal;                                                                                     \
    } hashmap_##key_name##_##value_name##_t;                                                                       \
    static inline hashmap_##key_name##_##value_name##_t* hashmap_create_##key_name##_##value_name(void)            \
    {                                                                                                              \
        hashmap_##key_name##_##value_name##_t* wrapper = malloc(sizeof(hashmap_##key_name##_##value_name##_t));    \
        wrapper->__internal = (hashmap_t*)hashmap_create_default(sizeof(key_type), sizeof(value_type));            \
        return wrapper;                                                                                            \
    }                                                                                                              \
    static inline int hashmap_set_##key_name##_##value_name(                                                       \
        hashmap_##key_name##_##value_name##_t* map, key_type key, value_type value)                                \
    {                                                                                                              \
        return hashmap_set(map->__internal, HASHMAP_KEY_REF(key_type, key), HASHMAP_VALUE_REF(value_type, value)); \
    }                                                                                                              \
    static inline value_type hashmap_get_##key_name##_##value_name(                                                \
        hashmap_##key_name##_##value_name##_t* map, key_type key)                                                  \
    {                                                                                                              \
        value_type* value_ptr = (value_type*)hashmap_get(map->__internal, HASHMAP_KEY_REF(key_type, key));         \
        return value_ptr ? *value_ptr : (value_type) { 0 };                                                        \
    }                                                                                                              \
    static inline value_type* hashmap_getp_##key_name##_##value_name(                                              \
        hashmap_##key_name##_##value_name##_t* map, key_type key)                                                  \
    {                                                                                                              \
        return (value_type*)hashmap_get(map->__internal, HASHMAP_KEY_REF(key_type, key));                          \
    }                                                                                                              \
    static inline int hashmap_has_##key_name##_##value_name(                                                       \
        hashmap_##key_name##_##value_name##_t* map, key_type key)                                                  \
    {                                                                                                              \
        return hashmap_has(map->__internal, HASHMAP_KEY_REF(key_type, key));                                       \
    }                                                                                                              \
    static inline int hashmap_remove_##key_name##_##value_name(                                                    \
        hashmap_##key_name##_##value_name##_t* map, key_type key)                                                  \
    {                                                                                                              \
        return hashmap_remove(map->__internal, HASHMAP_KEY_REF(key_type, key));                                    \
    }                                                                                                              \
    static inline void hashmap_destroy_##key_name##_##value_name(                                                  \
        hashmap_##key_name##_##value_name##_t* map)                                                                \
    {                                                                                                              \
        hashmap_destroy(map->__internal);                                                                          \
        free(map);                                                                                                 \
    }

#endif
