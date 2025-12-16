/*
 * Binary Search Map (sorted array)
 *
 * Same style as linear.h:
 *  - T_MAP_INTERFACE + MAP_METHOD
 *  - Keys are kept sorted in an array
 *  - Uses binary search for lookups
 *
 * Note: insertion and removal shift elements to maintain order.
 */

#define INITIAL_CAPACITY 16
#define EMPTY_KEY 0

#if defined(T_MAP_EXPORT_DEFS) || !defined(T_MAP_EXPORT_CODE)

typedef struct T {
    size_t length;
    size_t capacity;
    T_MAP_KEY *keys;
#if !defined(T_SET_ELEMENT)
    T_MAP_VALUE *values;
#endif
} *T;

#endif

#include <libellul/type/map/interface.h>
#include <stdlib.h>
#include <stdio.h>

/* --- Internal helper: binary search --- */
static ssize_t bs_find(T map, T_MAP_KEY key) {
    size_t left = 0;
    size_t right = map->length;
    while (left < right) {
        size_t mid = left + (right - left)/2;
        if (map->keys[mid] == key) return mid;
        if (map->keys[mid] < key) left = mid + 1;
        else right = mid;
    }
    return -1; // not found
}

static size_t bs_find_insert_pos(T map, T_MAP_KEY key) {
    size_t left = 0;
    size_t right = map->length;
    while (left < right) {
        size_t mid = left + (right - left)/2;
        if (map->keys[mid] < key) left = mid + 1;
        else right = mid;
    }
    return left;
}

/* --- API --- */
T_MAP_INTERFACE T MAP_METHOD(new)(void) {
    T map = calloc(1,sizeof(*map));
    if (!map) return NULL;
    map->length = 0;
    map->capacity = INITIAL_CAPACITY;
    map->keys = calloc(map->capacity,sizeof(T_MAP_KEY));
#if !defined(T_SET_ELEMENT)
    map->values = calloc(map->capacity,sizeof(T_MAP_VALUE));
#endif
    return map;
}

T_MAP_INTERFACE void MAP_METHOD(delete)(T *map_ptr) {
    if (!map_ptr || !*map_ptr) return;
    free((*map_ptr)->keys);
#if !defined(T_SET_ELEMENT)
    free((*map_ptr)->values);
#endif
    free(*map_ptr);
    *map_ptr = NULL;
}

T_MAP_INTERFACE size_t MAP_METHOD(length)(T map) {
    return map ? map->length : 0;
}

T_MAP_INTERFACE int MAP_METHOD(contains)(T map, T_MAP_KEY key) {
    return bs_find(map,key) >= 0;
}

#if defined(T_SET_ELEMENT)
T_MAP_INTERFACE int MAP_METHOD(insert)(T *map_ptr, T_SET_ELEMENT element) {
    if (!map_ptr || !*map_ptr) return 0;
    T map = *map_ptr;
    // grow if needed
    if (map->length >= map->capacity) {
        map->capacity *= 2;
        map->keys = realloc(map->keys,map->capacity*sizeof(T_MAP_KEY));
    }
    size_t pos = bs_find_insert_pos(map,element);
    // shift elements to make room
    for (size_t i=map->length;i>pos;i--) map->keys[i] = map->keys[i-1];
    map->keys[pos] = element;
    map->length++;
    return 1;
}
#else
T_MAP_INTERFACE int MAP_METHOD(put)(T *map_ptr, T_MAP_KEY key, T_MAP_VALUE value) {
    if (!map_ptr || !*map_ptr) return 0;
    T map = *map_ptr;
    ssize_t idx = bs_find(map,key);
    if (idx >= 0) {
        map->values[idx] = value; // overwrite
        return 1;
    }
    // grow if needed
    if (map->length >= map->capacity) {
        map->capacity *= 2;
        map->keys = realloc(map->keys,map->capacity*sizeof(T_MAP_KEY));
        map->values = realloc(map->values,map->capacity*sizeof(T_MAP_VALUE));
    }
    size_t pos = bs_find_insert_pos(map,key);
    // shift elements to make room
    for (size_t i=map->length;i>pos;i--) {
        map->keys[i] = map->keys[i-1];
        map->values[i] = map->values[i-1];
    }
    map->keys[pos] = key;
    map->values[pos] = value;
    map->length++;
    return 1;
}
#endif

#if !defined(T_SET_ELEMENT)
T_MAP_INTERFACE int MAP_METHOD(get)(T map, T_MAP_KEY key, T_MAP_VALUE *value) {
    ssize_t idx = bs_find(map,key);
    if (idx < 0) return 0;
    *value = map->values[idx];
    return 1;
}
#endif

T_MAP_INTERFACE int MAP_METHOD(remove)(T *map_ptr, T_MAP_KEY key) {
    if (!map_ptr || !*map_ptr) return 0;
    T map = *map_ptr;
    ssize_t idx = bs_find(map,key);
    if (idx < 0) return 0;
    for (size_t i=idx;i<map->length-1;i++) {
        map->keys[i] = map->keys[i+1];
#if !defined(T_SET_ELEMENT)
        map->values[i] = map->values[i+1];
#endif
    }
    map->length--;
    return 1;
}


#undef T_IMPL_BINARY_SEARCH_MAP