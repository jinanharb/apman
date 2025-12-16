#if !defined(T_MAP_TAG)
#error "Undefined T_MAP_TAG"
#endif
#if !defined(T_MAP_ENTRY)
#error "Undefined T_MAP_ENTRY"
#endif
#if !defined(T_MAP_KEY)
#error "Undefined T_MAP_KEY"
#endif

#include <libellul/type/prologue.h>

#if !defined(T_MAP_EXPORT_DEFS) && !defined(T_MAP_EXPORT_CODE)
#define T_MAP_INTERFACE static inline
#else
#define T_MAP_INTERFACE extern
#endif

#define CLOSED_METHOD(name)    GEN_SYM(T_MAP_TAG, name)

#if !defined(T_MAP_NBUCKETS)
#define T_MAP_NBUCKETS 1024
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <libellul.h>
#include <libellul/type/deques.h>

// Hash table structure 
typedef struct {
    deque_t *buckets;
    size_t nbuckets;
    size_t size;
} *T;

// user should provide these
extern size_t CLOSED_METHOD(hash)(const T_MAP_KEY *key);
extern int CLOSED_METHOD(eq)(const T_MAP_KEY *a, const T_MAP_KEY *b);


T_MAP_INTERFACE T      CLOSED_METHOD(new)(void);
T_MAP_INTERFACE void   CLOSED_METHOD(delete)(T ht);
T_MAP_INTERFACE int    CLOSED_METHOD(insert)(T ht, T_MAP_ENTRY *entry);
T_MAP_INTERFACE T_MAP_ENTRY* CLOSED_METHOD(find)(T ht, T_MAP_KEY *key);
T_MAP_INTERFACE int    CLOSED_METHOD(remove)(T ht, T_MAP_KEY *key);
T_MAP_INTERFACE void   CLOSED_METHOD(foreach)(T ht, void (*fn)(T_MAP_ENTRY *entry, void *ud), void *ud);
T_MAP_INTERFACE size_t CLOSED_METHOD(length)(T ht);
T_MAP_INTERFACE size_t CLOSED_METHOD(bucket_count)(T ht);

#if !defined(T_MAP_EXPORT_DEFS)

T_MAP_INTERFACE T CLOSED_METHOD(new)(void) {
    T ht = calloc( 1, sizeof( *ht ) );
    ht->nbuckets = T_MAP_NBUCKETS;
    ht->size = 0;
    ht->buckets = (deque_t*)calloc(ht->nbuckets, sizeof(deque_t));
    assert(ht->buckets);
    
    for (size_t i = 0; i < ht->nbuckets; ++i) {
        deque_init(&ht->buckets[i], T_MAP_ENTRY, link);
    }
    
    return ht;
}

T_MAP_INTERFACE void CLOSED_METHOD(delete)(T ht) 
{
    if (!ht || !ht->buckets) return;

    for (size_t i = 0; i < ht->nbuckets; ++i) {
        deque_t *dq = &ht->buckets[i];
        deque_reset(dq);
    }

    free(ht->buckets);
    ht->buckets = NULL;
    ht->nbuckets = 0;
    ht->size = 0;

    free(ht);
}

T_MAP_INTERFACE int CLOSED_METHOD(insert)(T ht, T_MAP_ENTRY *entry) {
    if (!ht || !ht->buckets || !entry) {
        fprintf(stderr, "Error: Invalid arguments in insert\n");
        return -1;
    }

    T_MAP_KEY *key = &entry->key;
    size_t h = CLOSED_METHOD(hash)(key) % ht->nbuckets;
    deque_t *dq = &ht->buckets[h];

    if (!dq) {
        fprintf(stderr, "Error: Invalid deque at bucket %zu\n", h);
        return -1;
    }

    deque_foreach(dq, el) {
        T_MAP_ENTRY *existing = (T_MAP_ENTRY*)el;
        T_MAP_KEY *ekey = &existing->key;
        if (CLOSED_METHOD(eq)(ekey, key)) {
    
            existing->value = entry->value;
            return 0;
        }
    }

    link_t *lnk = (link_t*)entry;
    link_init(lnk);

    deque_pushback(dq, entry);
    ht->size++;

    return 0;
}

T_MAP_INTERFACE T_MAP_ENTRY* CLOSED_METHOD(find)(T ht, T_MAP_KEY *key) {
    if (!ht || !ht->buckets) return NULL;
    
    size_t h = CLOSED_METHOD(hash)(key) % ht->nbuckets;
    deque_t *dq = &ht->buckets[h];

    deque_foreach(dq, el) {
        T_MAP_KEY *ekey = &((T_MAP_ENTRY*)el)->key;
        if (CLOSED_METHOD(eq)(ekey, key)) {
            return (T_MAP_ENTRY*)el;
        }
    }
    
    return NULL;
}

T_MAP_INTERFACE int CLOSED_METHOD(remove)(T ht, T_MAP_KEY *key) {
    if (!ht || !ht->buckets) return -1;

    size_t h = CLOSED_METHOD(hash)(key) % ht->nbuckets;
    deque_t *dq = &ht->buckets[h];

    void *to_remove = NULL;
    deque_foreach(dq, el) {
        T_MAP_KEY *ekey = &((T_MAP_ENTRY*)el)->key;
        if (CLOSED_METHOD(eq)(ekey, key)) {
            to_remove = el;
            break;
        }
    }

    if (to_remove) {
    link_t *lnk = (link_t*)((char*)to_remove + dq->offset);
    
    lnk->prev->next = lnk->next;
    lnk->next->prev = lnk->prev;
    lnk->prev = lnk->next = lnk;
    
    ht->size--;
    return 0;
}

    return -1;
}

T_MAP_INTERFACE void CLOSED_METHOD(foreach)(T ht, void (*fn)(T_MAP_ENTRY *entry, void *ud), void *ud) {
    if (!ht || !ht->buckets || !fn) return;

    for (size_t i = 0; i < ht->nbuckets; ++i) {
        deque_t *dq = &ht->buckets[i];
        deque_foreach(dq, el) {
            fn((T_MAP_ENTRY*)el, ud);
        }
    }
}

T_MAP_INTERFACE size_t CLOSED_METHOD(length)(T ht) {
    return ht ? ht->size : 0;
}

T_MAP_INTERFACE size_t CLOSED_METHOD(bucket_count)(T ht) {
    return ht ? ht->nbuckets : 0;
}

#endif 



//undefining the local variables that will not be used elsewhere
#undef T_MAP_ENTRY
#undef T_MAP_NBUCKETS
#undef T_MAP_EXPORT_DEFS
#undef T_MAP_EXPORT_CODE
#undef CLOSED_METHOD