#pragma once
#ifndef __FELIX_HASHMAP_HELPERS_H__
#define __FELIX_HASHMAP_HELPERS_H__

#include "hashmap.h" // IWYU pragma: keep
#include <stdlib.h>  // IWYU pragma: keep

#define HASHMAP_KEY_REF(type, value)   (&(type) { value })
#define HASHMAP_VALUE_REF(type, value) (&(type) { value })

#define HASHMAP_TYPE(key_name, value_name) hashmap_##key_name##_##value_name##_t
#define HASHMAP_DEFINE_TYPE(key_type, key_name, value_type, value_name)                                                \
    typedef struct HASHMAP_TYPE(key_name, value_name) {                                                                \
        hashmap_t* __internal;                                                                                         \
    } HASHMAP_TYPE(key_name, value_name);                                                                              \
    static inline HASHMAP_TYPE(key_name, value_name) * hashmap_create_##key_name##_##value_name(void)                  \
    {                                                                                                                  \
        HASHMAP_TYPE(key_name, value_name)* wrapper = malloc(sizeof(HASHMAP_TYPE(key_name, value_name)));              \
        wrapper->__internal = (hashmap_t*)hashmap_create_default(sizeof(key_type), sizeof(value_type));                \
        return wrapper;                                                                                                \
    }                                                                                                                  \
    static inline int hashmap_set_##key_name##_##value_name(                                                           \
        HASHMAP_TYPE(key_name, value_name) * map, key_type key, value_type value)                                      \
    {                                                                                                                  \
        return hashmap_set(map->__internal, HASHMAP_KEY_REF(key_type, key), HASHMAP_VALUE_REF(value_type, value));     \
    }                                                                                                                  \
    static inline value_type hashmap_get_##key_name##_##value_name(                                                    \
        HASHMAP_TYPE(key_name, value_name) * map, key_type key)                                                        \
    {                                                                                                                  \
        value_type* value_ptr = (value_type*)hashmap_get(map->__internal, HASHMAP_KEY_REF(key_type, key));             \
        return value_ptr ? *value_ptr : (value_type) { 0 };                                                            \
    }                                                                                                                  \
    static inline value_type* hashmap_getp_##key_name##_##value_name(                                                  \
        HASHMAP_TYPE(key_name, value_name) * map, key_type key)                                                        \
    {                                                                                                                  \
        return (value_type*)hashmap_get(map->__internal, HASHMAP_KEY_REF(key_type, key));                              \
    }                                                                                                                  \
    static inline int hashmap_has_##key_name##_##value_name(HASHMAP_TYPE(key_name, value_name) * map, key_type key)    \
    {                                                                                                                  \
        return hashmap_has(map->__internal, HASHMAP_KEY_REF(key_type, key));                                           \
    }                                                                                                                  \
    static inline int hashmap_remove_##key_name##_##value_name(HASHMAP_TYPE(key_name, value_name) * map, key_type key) \
    {                                                                                                                  \
        return hashmap_remove(map->__internal, HASHMAP_KEY_REF(key_type, key));                                        \
    }                                                                                                                  \
    static inline void hashmap_destroy_##key_name##_##value_name(HASHMAP_TYPE(key_name, value_name) * map)             \
    {                                                                                                                  \
        hashmap_destroy(map->__internal);                                                                              \
        free(map);                                                                                                     \
    }

#endif
