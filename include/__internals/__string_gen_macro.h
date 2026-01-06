#pragma once
#ifndef __FELIX_HASHMAP_HELPERS_STRING_H__
#define __FELIX_HASHMAP_HELPERS_STRING_H__

#include "hashmap.h"        // IWYU pragma: keep
#include "hashmap_string.h" // IWYU pragma: keep
#include <stdlib.h>         // IWYU pragma: keep
//
#define HASHMAP_VALUE_REF(type, value) (&(type) { value })

#define HASHMAP_DEFINE_STRING_TYPE(value_type, value_name)                                                             \
    typedef struct hashmap_string_##value_name##_t {                                                                   \
        hashmap_t* __internal;                                                                                         \
    } hashmap_string_##value_name##_t;                                                                                 \
    static inline hashmap_string_##value_name##_t* hashmap_create_string_##value_name(void)                            \
    {                                                                                                                  \
        hashmap_string_##value_name##_t* wrapper = malloc(sizeof(hashmap_string_##value_name##_t));                    \
        wrapper->__internal                      = (hashmap_t*)hashmap_create_string(sizeof(value_type));              \
        return wrapper;                                                                                                \
    }                                                                                                                  \
    static inline int hashmap_set_string_##value_name(                                                                 \
        hashmap_string_##value_name##_t* map, char* key, value_type value)                                             \
    {                                                                                                                  \
        return hashmap_set(map->__internal, key, HASHMAP_VALUE_REF(value_type, value));                                \
    }                                                                                                                  \
    static inline value_type hashmap_get_string_##value_name(hashmap_string_##value_name##_t* map, char* key)          \
    {                                                                                                                  \
        value_type* value_ptr = (value_type*)hashmap_get(map->__internal, key);                                        \
        return value_ptr ? *value_ptr : (value_type) { 0 };                                                            \
    }                                                                                                                  \
    static inline value_type* hashmap_getp_string_##value_name(hashmap_string_##value_name##_t* map, char* key)        \
    {                                                                                                                  \
        return (value_type*)hashmap_get(map->__internal, key);                                                         \
    }                                                                                                                  \
    static inline int hashmap_has_string_##value_name(hashmap_string_##value_name##_t* map, char* key)                 \
    {                                                                                                                  \
        return hashmap_has(map->__internal, key);                                                                      \
    }                                                                                                                  \
    static inline int hashmap_remove_string_##value_name(hashmap_string_##value_name##_t* map, char* key)              \
    {                                                                                                                  \
        return hashmap_remove(map->__internal, key);                                                                   \
    }                                                                                                                  \
    static inline void hashmap_destroy_string_##value_name(hashmap_string_##value_name##_t* map)                       \
    {                                                                                                                  \
        hashmap_destroy(map->__internal);                                                                              \
        free(map);                                                                                                     \
    }

#endif // __FELIX_HASHMAP_HELPERS_STRING_H__
