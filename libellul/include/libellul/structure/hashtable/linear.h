#ifndef LINEAR_HASHTABLE_H
#define LINEAR_HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>

typedef int T_MAP_KEY;
typedef const char* T_MAP_VALUE;

#define HASH_FUN(key) ((unsigned long)(key) * 2654435761u % 4294967296u)
#define EQ_FUN(a,b) ((a) == (b))

#ifndef T_SET_ELEMENT
#define T_SET_ELEMENT 0
#endif

typedef struct T {
    size_t length;
    T_MAP_KEY *bucket;
#if !T_SET_ELEMENT
    T_MAP_VALUE *value;
#endif
} *T;

#define T_MAP_INTERFACE
#define MAP_METHOD(name) name##_linear

/* ==================== Fonctions ==================== */

/* Création */
T_MAP_INTERFACE T MAP_METHOD(new)(void) {
    T map = (T) malloc(sizeof(*map));
    map->length = 0;

    size_t cap = 16;
    map->bucket = (T_MAP_KEY*) malloc(sizeof(T_MAP_KEY) * cap);
    for (size_t i = 0; i < cap; ++i) map->bucket[i] = -1;

#if !T_SET_ELEMENT
    map->value = (T_MAP_VALUE*) malloc(sizeof(T_MAP_VALUE) * cap);
#endif

    return map;
}

/* Longueur */
T_MAP_INTERFACE size_t MAP_METHOD(length)(T map) {
    return map ? map->length : 0;
}

/* Supprimer */
T_MAP_INTERFACE void MAP_METHOD(delete)(T *map) {
    if (!map || !*map) return;
    T m = *map;

    if (m->bucket) free(m->bucket);
#if !T_SET_ELEMENT
    if (m->value) free(m->value);
#endif
    free(m);
    *map = NULL;
}

/* Contient */
T_MAP_INTERFACE int MAP_METHOD(contains)(T map, T_MAP_KEY key) {
    if (!map) return 0;
    size_t cap = 16;
    size_t h = HASH_FUN(key) % cap;
    for (size_t i = 0; i < cap; ++i) {
        size_t idx = (h + i) % cap;
        T_MAP_KEY k = map->bucket[idx];
        if (k == -1) return 0;
        if (k == key) return 1;
    }
    return 0;
}

/* Ajouter / mettre */
T_MAP_INTERFACE int MAP_METHOD(put)(T *map, T_MAP_KEY key, T_MAP_VALUE value) {
    if (!map) return 0;
    if (!*map) *map = MAP_METHOD(new)();
    T m = *map;

#if T_SET_ELEMENT
    (void)value; // éviter le warning si on n’utilise pas value
#endif

    size_t cap = 16;
    size_t h = HASH_FUN(key) % cap;
    for (size_t i = 0; i < cap; ++i) {
        size_t idx = (h + i) % cap;
        T_MAP_KEY k = m->bucket[idx];
        if (k == -1) {
            m->bucket[idx] = key;
#if !T_SET_ELEMENT
            m->value[idx] = value;
#endif
            m->length++;
            return 1;
        }
        if (k == key) {
#if !T_SET_ELEMENT
            m->value[idx] = value;
#endif
            return 0;
        }
    }
    return 0;
}

/* Obtenir / get */
T_MAP_INTERFACE int MAP_METHOD(get)(T map, T_MAP_KEY key, T_MAP_VALUE *value) {
#if T_SET_ELEMENT
    (void)value; // valeur inutilisée pour un set
#endif

    if (!map) return 0;
    size_t cap = 16;
    size_t h = HASH_FUN(key) % cap;
    for (size_t i = 0; i < cap; ++i) {
        size_t idx = (h + i) % cap;
        T_MAP_KEY k = map->bucket[idx];
        if (k == -1) return 0;
        if (k == key) {
#if !T_SET_ELEMENT
            if (value) *value = map->value[idx];
#endif
            return 1;
        }
    }
    return 0;
}

/* Supprimer clé */
T_MAP_INTERFACE int MAP_METHOD(remove)(T *map, T_MAP_KEY key) {
    if (!map || !*map) return 0;
    T m = *map;
    size_t cap = 16;
    size_t h = HASH_FUN(key) % cap;
    for (size_t i = 0; i < cap; ++i) {
        size_t idx = (h + i) % cap;
        T_MAP_KEY k = m->bucket[idx];
        if (k == -1) return 0;
        if (k == key) {
            m->bucket[idx] = -1;
#if !T_SET_ELEMENT
            m->value[idx] = NULL;
#endif
            if (m->length > 0) m->length--;
            return 1;
        }
    }
    return 0;
}

#endif /* LINEAR_HASHTABLE_H */
