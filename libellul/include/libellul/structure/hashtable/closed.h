/*
 * 'closed' hash table abstract data type.
 *
 * A closed addressing hash table using chaining with deques.
 *
 * Required template parameters:
 *
 * T_CLOSED_TAG: Prefix for generated type and function names
 * T_CLOSED_ENTRY: The entry type to store in the hash table
 * T_CLOSED_KEY: The key type within the entry
 * 
 * Required user-defined functions:
 * 
 * size_t T_CLOSED_TAG_hash(const T_CLOSED_KEY *key)
 * int T_CLOSED_TAG_eq(const T_CLOSED_KEY *a, const T_CLOSED_KEY *b)
 *
 * Optional parameters:
 *
 * T_CLOSED_NBUCKETS: Initial number of buckets (default: 16)
 *
 * Example usage:
 *
 * typedef struct {
 *     link_t link;  // Must be first member for deque
 *     int key;
 *     char *value;
 * } my_entry_t;
 *
 * #define T_CLOSED_TAG my_map
 * #define T_CLOSED_ENTRY my_entry_t
 * #define T_CLOSED_KEY int
 * #include <libellul/structure/hashtable/closed.h>
 *
 * // Then implement:
 * // size_t my_map_hash(const int *key) { return *key; }
 * // int my_map_eq(const int *a, const int *b) { return *a == *b; }
 */

#if !defined(T_CLOSED_TAG)
#error "Undefined T_CLOSED_TAG"
#endif
#if !defined(T_CLOSED_ENTRY)
#error "Undefined T_CLOSED_ENTRY"
#endif
#if !defined(T_CLOSED_KEY)
#error "Undefined T_CLOSED_KEY"
#endif

#include <libellul/type/prologue.h>

#if !defined(T_CLOSED_EXPORT_DEFS) && !defined(T_CLOSED_EXPORT_CODE)
#define T_CLOSED_INTERFACE static inline
#else
#define T_CLOSED_INTERFACE extern
#endif

#define T                      GEN_SYM(T_CLOSED_TAG, t)
#define CLOSED_METHOD(name)    GEN_SYM(T_CLOSED_TAG, name)

#if !defined(T_CLOSED_NBUCKETS)
#define T_CLOSED_NBUCKETS 16
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <libellul.h>
#include <libellul/type/deques.h>

/* Hash table structure */
typedef struct {
    deque_t *buckets;
    size_t nbuckets;
    size_t size;
} T;

/* Forward declare user-provided hash and equality functions */
extern size_t CLOSED_METHOD(hash)(const T_CLOSED_KEY *key);
extern int CLOSED_METHOD(eq)(const T_CLOSED_KEY *a, const T_CLOSED_KEY *b);

/* Interface functions */
T_CLOSED_INTERFACE T      CLOSED_METHOD(new)(void);
T_CLOSED_INTERFACE void   CLOSED_METHOD(delete)(T *ht);
T_CLOSED_INTERFACE int    CLOSED_METHOD(insert)(T *ht, T_CLOSED_ENTRY *entry);
T_CLOSED_INTERFACE T_CLOSED_ENTRY* CLOSED_METHOD(find)(T *ht, T_CLOSED_KEY *key);
T_CLOSED_INTERFACE int    CLOSED_METHOD(remove)(T *ht, T_CLOSED_KEY *key);
T_CLOSED_INTERFACE void   CLOSED_METHOD(foreach)(T *ht, void (*fn)(T_CLOSED_ENTRY *entry, void *ud), void *ud);
T_CLOSED_INTERFACE size_t CLOSED_METHOD(length)(T *ht);
T_CLOSED_INTERFACE size_t CLOSED_METHOD(bucket_count)(T *ht);
/* Implementation */
#if !defined(T_CLOSED_EXPORT_DEFS)

T_CLOSED_INTERFACE T CLOSED_METHOD(new)(void) {
    T ht;
    ht.nbuckets = T_CLOSED_NBUCKETS;
    ht.size = 0;
    ht.buckets = (deque_t*)calloc(ht.nbuckets, sizeof(deque_t));
    assert(ht.buckets);

    typedef struct { char dummy; } dummy_t;
    
    for (size_t i = 0; i < ht.nbuckets; ++i) {
        deque_init(&ht.buckets[i], T_CLOSED_ENTRY, link);
    }
    
    return ht;
}

T_CLOSED_INTERFACE void CLOSED_METHOD(delete)(T *ht) {
    if (!ht || !ht->buckets) return;

    for (size_t i = 0; i < ht->nbuckets; ++i) {
        deque_t *dq = &ht->buckets[i];
        deque_reset(dq);
    }

    free(ht->buckets);
    ht->buckets = NULL;
    ht->nbuckets = 0;
    ht->size = 0;
}

T_CLOSED_INTERFACE int CLOSED_METHOD(insert)(T *ht, T_CLOSED_ENTRY *entry) {
    if (!ht || !ht->buckets || !entry) {
        fprintf(stderr, "Error: Invalid arguments in insert\n");
        return -1;
    }

    /* Get the key from the entry - offsetof(T_CLOSED_ENTRY, key) */
    T_CLOSED_KEY *key = &((T_CLOSED_ENTRY*)entry)->key;
    size_t h = CLOSED_METHOD(hash)(key) % ht->nbuckets;
    deque_t *dq = &ht->buckets[h];

    if (!dq) {
        fprintf(stderr, "Error: Invalid deque at bucket %zu\n", h);
        return -1;
    }

    /* Check if key already exists */
    deque_foreach(dq, el) {
        T_CLOSED_KEY *ekey = &((T_CLOSED_ENTRY*)el)->key;
        if (CLOSED_METHOD(eq)(ekey, key)) {
            T_CLOSED_ENTRY *existing = (T_CLOSED_ENTRY*)el;
            existing->value = entry->value;  // update other fields only

            return 0;
        }
    }

    /* Initialize link and add new entry */
    link_t *lnk = (link_t*)entry;
    link_init(lnk);
    deque_pushback(dq, entry);
    ht->size++;

    return 0;
}

T_CLOSED_INTERFACE T_CLOSED_ENTRY* CLOSED_METHOD(find)(T *ht, T_CLOSED_KEY *key) {
    if (!ht || !ht->buckets) return NULL;
    
    size_t h = CLOSED_METHOD(hash)(key) % ht->nbuckets;
    deque_t *dq = &ht->buckets[h];

    deque_foreach(dq, el) {
        T_CLOSED_KEY *ekey = &((T_CLOSED_ENTRY*)el)->key;
        if (CLOSED_METHOD(eq)(ekey, key)) {
            return (T_CLOSED_ENTRY*)el;
        }
    }
    
    return NULL;
}

T_CLOSED_INTERFACE int CLOSED_METHOD(remove)(T *ht, T_CLOSED_KEY *key) {
    if (!ht || !ht->buckets) return -1;

    size_t h = CLOSED_METHOD(hash)(key) % ht->nbuckets;
    deque_t *dq = &ht->buckets[h];

    /* Find the element */
    void *to_remove = NULL;
    deque_foreach(dq, el) {
        T_CLOSED_KEY *ekey = &((T_CLOSED_ENTRY*)el)->key;
        if (CLOSED_METHOD(eq)(ekey, key)) {
            to_remove = el;
            break;
        }
    }

    if (to_remove) {
    /* Get the link from the entry using the offset */
    link_t *lnk = (link_t*)((char*)to_remove + dq->offset);
    
    /* Remove from deque using link operations */
    lnk->prev->next = lnk->next;
    lnk->next->prev = lnk->prev;
    lnk->prev = lnk->next = lnk;
    
    ht->size--;
    return 0;
}

    return -1;
}

T_CLOSED_INTERFACE void CLOSED_METHOD(foreach)(T *ht, void (*fn)(T_CLOSED_ENTRY *entry, void *ud), void *ud) {
    if (!ht || !ht->buckets || !fn) return;

    for (size_t i = 0; i < ht->nbuckets; ++i) {
        deque_t *dq = &ht->buckets[i];
        deque_foreach(dq, el) {
            fn((T_CLOSED_ENTRY*)el, ud);
        }
    }
}

T_CLOSED_INTERFACE size_t CLOSED_METHOD(length)(T *ht) {
    return ht ? ht->size : 0;
}

T_CLOSED_INTERFACE size_t CLOSED_METHOD(bucket_count)(T *ht) {
    return ht ? ht->nbuckets : 0;
}

#endif /* T_CLOSED_EXPORT_DEFS */

/* Define is_empty - compatible with our pointer-based API */
#if !defined(T_CLOSED_EXPORT_DEFS)
T_CLOSED_INTERFACE int CLOSED_METHOD(is_empty)(T *ht) {
    return CLOSED_METHOD(length)(ht) == 0;
}
#endif

/* Cleanup */
#undef T_CLOSED_TAG
#undef T_CLOSED_ENTRY
#undef T_CLOSED_KEY
#undef T_CLOSED_NBUCKETS
#undef T_CLOSED_INTERFACE
#undef T_CLOSED_EXPORT_DEFS
#undef T_CLOSED_EXPORT_CODE
#undef T
#undef CLOSED_METHOD