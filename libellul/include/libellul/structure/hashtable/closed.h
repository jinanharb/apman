/*
 * closed addressing hashtable.
 *
 */

#if defined( T_MAP_EXPORT_DEFS ) || !defined( T_MAP_EXPORT_CODE )

typedef struct T {
  size_t       length;
  T_MAP_KEY   *bucket;

#if !defined ( T_SET_ELEMENT )
  T_MAP_VALUE *value;
#endif
} *T;

#endif

#include <libellul/type/map/interface.h>

#if !defined( T_MAP_EXPORT_DEFS )
/* Code templating for the actual implementation starts here */

#include <libellul/memory.h>
#include <libellul/type/array.h>

/* Of course we are entitled to functions that are always local: */
static inline int MAP_METHOD( foo_helper ) ( T map ) {
  /* Dummy example! Mostly useful to factorize code in the implementation! */
  return printf( "Called %s::%s on %sempty map at %p\n",
                 __FILE__, __func__, !MAP_METHOD( is_empty ) ( map ) ? "non-" : "", map );
}


T_MAP_INTERFACE T MAP_METHOD( new )( void ) {
  T map = NULL;

  /* In the implementation, use any local or interface function like: */
  MAP_METHOD( foo_helper ) ( map );

  /* TODO: Finish init */

  return map;
}

/* TODO: Implement other functions in the interface */

T_MAP_INTERFACE size_t MAP_METHOD( length )( T map ) {
  return 0 == map->length;
}

T_MAP_INTERFACE void   MAP_METHOD( delete )( T *map ) {

}

T_MAP_INTERFACE int    MAP_METHOD( contains )( T map, T_MAP_KEY key ) {

}

T_MAP_INTERFACE int    MAP_METHOD( remove )( T *map, T_MAP_KEY key ) {

}

#if defined( T_SET_ELEMENT )
T_MAP_INTERFACE int    MAP_METHOD( insert )( T *set, T_SET_ELEMENT element ) {

}
#else
T_MAP_INTERFACE int    MAP_METHOD( put )( T *map, T_MAP_KEY key, T_MAP_VALUE value ) {

}

T_MAP_INTERFACE int    MAP_METHOD( get )( T map, T_MAP_KEY key, T_MAP_VALUE *value ) {

}
#endif

#endif

#undef T_IMPL_HASHTABLE_LINEAR
#undef T_IMPL_HASHTABLE_LINEAR_NO_TOMBSTONES
/*
 * closed addressing hashtable using Libellul deques
 */

#include <libellul/type/deques.h>
#include <libellul/memory.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

/* === Map types === */
typedef int T_MAP_KEY;
typedef const char* T_MAP_VALUE;

#ifndef T_SET_ELEMENT
#define T_SET_ELEMENT 0
#endif

/* === Entry in a deque === */
typedef struct Entry {
    link_t link;
    T_MAP_KEY key;
#if !T_SET_ELEMENT
    T_MAP_VALUE value;
#endif
} Entry;

/* === Hashtable type === */
typedef struct T {
    size_t length;      // total number of elements
    size_t capacity;    // number of buckets
    deque_t *buckets;   // array of deques
} *T;

/* === Hash function === */
#define HASH_FUN(key) ((unsigned long)(key) * 2654435761u % 4294967296u)

/* === Interface macros === */
#define T_MAP_INTERFACE
#define MAP_METHOD(name) name##_closed

/* ==================== Functions ==================== */

/* Create a new hashtable */
T_MAP_INTERFACE T MAP_METHOD(new)(void) {
    size_t initial_capacity = 16;
    T map = malloc(sizeof(*map));
    map->length = 0;
    map->capacity = initial_capacity;

    map->buckets = malloc(sizeof(deque_t) * initial_capacity);
    for (size_t i = 0; i < initial_capacity; i++) {
        deque_reset(&map->buckets[i]);
    }
    return map;
}

/* Insert / put a key-value pair */
T_MAP_INTERFACE int MAP_METHOD(put)(T *map_ptr, T_MAP_KEY key, T_MAP_VALUE value) {
    if (!map_ptr) return 0;
    if (!*map_ptr) *map_ptr = MAP_METHOD(new)();
    T map = *map_ptr;

    size_t h = HASH_FUN(key) % map->capacity;
    deque_t *bucket = &map->buckets[h];

    /* Check if key exists */
    link_t *link = bucket->head.next;
    while (link) {
        Entry *entry = (Entry*)((char*)link - offsetof(Entry, link));
        if (entry->key == key) {
#if !T_SET_ELEMENT
            entry->value = value;
#endif
            return 0; // updated
        }
        link = link->next;
    }

    /* Insert new entry */
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->key = key;
#if !T_SET_ELEMENT
    new_entry->value = value;
#endif
    link_init(&new_entry->link);
    deque_addback(bucket, &new_entry->link);

    map->length++;
    return 1; // inserted
}

/* Insert for set elements (no value) */
#if T_SET_ELEMENT
T_MAP_INTERFACE int MAP_METHOD(insert)(T *set_ptr, T_SET_ELEMENT element) {
    /* treat element as key */
    return MAP_METHOD(put)((T*)set_ptr, element, 0);
}
#endif

/* Get value for a key */
T_MAP_INTERFACE int MAP_METHOD(get)(T map, T_MAP_KEY key, T_MAP_VALUE *value) {
    if (!map) return 0;
    size_t h = HASH_FUN(key) % map->capacity;
    deque_t *bucket = &map->buckets[h];

    link_t *link = bucket->head.next;
    while (link) {
        Entry *entry = (Entry*)((char*)link - offsetof(Entry, link));
        if (entry->key == key) {
#if !T_SET_ELEMENT
            if (value) *value = entry->value;
#endif
            return 1;
        }
        link = link->next;
    }
    return 0;
}

/* Remove a key */
T_MAP_INTERFACE int MAP_METHOD(remove)(T *map_ptr, T_MAP_KEY key) {
    if (!map_ptr || !*map_ptr) return 0;
    T map = *map_ptr;

    size_t h = HASH_FUN(key) % map->capacity;
    deque_t *bucket = &map->buckets[h];

    link_t *link = bucket->head.next;
    while (link) {
        Entry *entry = (Entry*)((char*)link - offsetof(Entry, link));
        if (entry->key == key) {
            /* Remove from deque */
            if (link->prev) link->prev->next = link->next;
            if (link->next) link->next->prev = link->prev;
            if (bucket->head.next == link) bucket->head.next = link->next;
            if (bucket->head.prev == link) bucket->head.prev = link->prev;

            free(entry);
            map->length--;
            bucket->length--;
            return 1;
        }
        link = link->next;
    }
    return 0;
}

/* Check if key exists */
T_MAP_INTERFACE int MAP_METHOD(contains)(T map, T_MAP_KEY key) {
    return MAP_METHOD(get)(map, key, NULL);
}

/* Return total number of elements */
T_MAP_INTERFACE size_t MAP_METHOD(length)(T map) {
    return map ? map->length : 0;
}

/* Delete the hashtable */
T_MAP_INTERFACE void MAP_METHOD(delete)(T *map_ptr) {
    if (!map_ptr || !*map_ptr) return;
    T map = *map_ptr;

    for (size_t i = 0; i < map->capacity; i++) {
        link_t *link = map->buckets[i].head.next;
        while (link) {
            Entry *entry = (Entry*)((char*)link - offsetof(Entry, link));
            link_t *next = link->next;
            free(entry);
            link = next;
        }
    }

    free(map->buckets);
    free(map);
    *map_ptr = NULL;
}