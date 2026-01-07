#pragma once
#ifndef __FELIX_HASHMAP_HELPERS_H__
#define __FELIX_HASHMAP_HELPERS_H__

#include "hashmap.h" // IWYU pragma: keep
#include <stdlib.h>  // IWYU pragma: keep

#define HASHMAP_KEY_REF(type, value)       (&(type) { value })
#define HASHMAP_VALUE_REF(type, value)     (&(type) { value })
#define HASHMAP_TYPE(key_name, value_name) hashmap_##key_name##_##value_name##_t

#define HASHMAP_CREATE(key_type, key_name, value_type, value_name)                                                     \
    static inline HASHMAP_TYPE(key_name, value_name) * hashmap_create_##key_name##_##value_name(void)                  \
    {                                                                                                                  \
        HASHMAP_TYPE(key_name, value_name)* wrapper = malloc(sizeof(HASHMAP_TYPE(key_name, value_name)));              \
        if (!wrapper)                                                                                                  \
            return NULL;                                                                                               \
        wrapper->__internal = (hashmap_t*)hashmap_create_default(sizeof(key_type), sizeof(value_type));                \
        if (!wrapper->__internal) {                                                                                    \
            free(wrapper);                                                                                             \
            return NULL;                                                                                               \
        }                                                                                                              \
        return wrapper;                                                                                                \
    }

#define HASHMAP_SET(key_type, key_name, value_type, value_name)                                                        \
    static inline int hashmap_set_##key_name##_##value_name(                                                           \
        HASHMAP_TYPE(key_name, value_name) * map, key_type key, value_type value)                                      \
    {                                                                                                                  \
        if (!map || !map->__internal)                                                                                  \
            return -1;                                                                                                 \
        return hashmap_set(map->__internal, HASHMAP_KEY_REF(key_type, key), HASHMAP_VALUE_REF(value_type, value));     \
    }

#define HASHMAP_GET(key_type, key_name, value_type, value_name)                                                        \
    static inline value_type hashmap_get_##key_name##_##value_name(                                                    \
        HASHMAP_TYPE(key_name, value_name) * map, key_type key)                                                        \
    {                                                                                                                  \
        if (!map || !map->__internal)                                                                                  \
            return (value_type) { 0 };                                                                                 \
        value_type* value_ptr = (value_type*)hashmap_get(map->__internal, HASHMAP_KEY_REF(key_type, key));             \
        return value_ptr ? *value_ptr : (value_type) { 0 };                                                            \
    }

#define HASHMAP_GETP(key_type, key_name, value_type, value_name)                                                       \
    static inline value_type* hashmap_getp_##key_name##_##value_name(                                                  \
        HASHMAP_TYPE(key_name, value_name) * map, key_type key)                                                        \
    {                                                                                                                  \
        if (!map || !map->__internal)                                                                                  \
            return NULL;                                                                                               \
        return (value_type*)hashmap_get(map->__internal, HASHMAP_KEY_REF(key_type, key));                              \
    }

#define HASHMAP_HAS(key_type, key_name, value_type, value_name)                                                        \
    static inline int hashmap_has_##key_name##_##value_name(HASHMAP_TYPE(key_name, value_name) * map, key_type key)    \
    {                                                                                                                  \
        if (!map || !map->__internal)                                                                                  \
            return -1;                                                                                                 \
        return hashmap_has(map->__internal, HASHMAP_KEY_REF(key_type, key));                                           \
    }

#define HASHMAP_REMOVE(key_type, key_name, value_type, value_name)                                                     \
    static inline int hashmap_remove_##key_name##_##value_name(HASHMAP_TYPE(key_name, value_name) * map, key_type key) \
    {                                                                                                                  \
        if (!map || !map->__internal)                                                                                  \
            return -1;                                                                                                 \
        return hashmap_remove(map->__internal, HASHMAP_KEY_REF(key_type, key));                                        \
    }

#define HASHMAP_DESTROY(key_type, key_name, value_type, value_name)                                                    \
    static inline void hashmap_destroy_##key_name##_##value_name(HASHMAP_TYPE(key_name, value_name) * map)             \
    {                                                                                                                  \
        hashmap_destroy(map->__internal);                                                                              \
        if (map)                                                                                                       \
            free(map);                                                                                                 \
    }

#define HASHMAP_DEFINE_TYPE(key_type, key_name, value_type, value_name)                                                \
    typedef struct HASHMAP_TYPE(key_name, value_name) {                                                                \
        hashmap_t* __internal;                                                                                         \
    } HASHMAP_TYPE(key_name, value_name);                                                                              \
    HASHMAP_CREATE(key_type, key_name, value_type, value_name)                                                         \
    HASHMAP_SET(key_type, key_name, value_type, value_name)                                                            \
    HASHMAP_GET(key_type, key_name, value_type, value_name)                                                            \
    HASHMAP_GETP(key_type, key_name, value_type, value_name)                                                           \
    HASHMAP_HAS(key_type, key_name, value_type, value_name)                                                            \
    HASHMAP_REMOVE(key_type, key_name, value_type, value_name)                                                         \
    HASHMAP_DESTROY(key_type, key_name, value_type, value_name)

#endif
