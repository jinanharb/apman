
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


#define BTREE_BITS 4
#define RADIX (1 << BTREE_BITS)
#define LEVEL_MASK (RADIX-1)
#define MAX_LEVELS ((sizeof(uint64_t)*8 + BTREE_BITS -1)/BTREE_BITS)

#if defined(T_MAP_EXPORT_DEFS) || !defined(T_MAP_EXPORT_CODE)

typedef struct ht_entry {
    T_MAP_KEY key;
#if !defined(T_SET_ELEMENT)
    T_MAP_VALUE value;
#endif
    struct ht_entry *next;
} ht_entry_t;

typedef struct ht_node {
    int is_leaf;
    union {
        struct ht_node *children[RADIX];
        struct {
            ht_entry_t *entries;
            size_t count;
        } leaf;
    };
} ht_node_t;

typedef struct T {
    ht_node_t *root;
    size_t length;
} *T;

#endif


#include <libellul/type/map/interface.h>

#if !defined( T_MAP_EXPORT_DEFS )
/* Code templating for the actual implementation starts here */

#include <libellul/memory.h>
#include <libellul/type/array.h>
/* --- Utils --- */
static uint32_t ht_get_slice(uint64_t hash, unsigned level) {
    return (hash >> (level * BTREE_BITS)) & LEVEL_MASK;
}

/* --- Node creation --- */
static ht_node_t *ht_new_leaf(void) {
    ht_node_t *n = calloc(1,sizeof(*n));
    if(!n) return NULL;
    n->is_leaf = 1;
    n->leaf.entries = NULL;
    n->leaf.count = 0;
    return n;
}


/* --- Free nodes recursively --- */
static void ht_free(ht_node_t *n) {
    if(!n) return;
    if(n->is_leaf) {
        ht_entry_t *e = n->leaf.entries;
        while(e) {
            ht_entry_t *tmp = e->next;
            free(e);
            e = tmp;
        }
    } else {
        for(int i=0;i<RADIX;i++) {
            if(n->children[i]) ht_free(n->children[i]);
        }
    }
    free(n);
}

/* --- API functions --- */
T_MAP_INTERFACE T MAP_METHOD(new)(void) {
    T map = calloc(1,sizeof(*map));
    if(!map) return NULL;
    map->root = ht_new_leaf();
    map->length = 0;
    return map;
}

T_MAP_INTERFACE void MAP_METHOD(delete)(T *map_ptr) {
    if(!map_ptr || !*map_ptr) return;
    ht_free((*map_ptr)->root);
    free(*map_ptr);
    *map_ptr = NULL;
}

T_MAP_INTERFACE size_t MAP_METHOD(length)(T map) {
    return map ? map->length : 0;
}


#if defined(T_SET_ELEMENT)
T_MAP_INTERFACE int MAP_METHOD(insert)(T *map_ptr, T_SET_ELEMENT element) {
    if(!map_ptr || !*map_ptr) return 0;
    int r = ht_insert_recursive(&(*map_ptr)->root, element, 0);
    if(r) (*map_ptr)->length++;
    return r;
}
#else
T_MAP_INTERFACE int MAP_METHOD(put)(T *map_ptr, T_MAP_KEY key, T_MAP_VALUE value) {
    if (!map_ptr || !*map_ptr) return 0;
    T map = *map_ptr;

    if (!map->root) map->root = ht_new_leaf();
    ht_node_t *node = map->root;
    uint64_t hash = (uint64_t) T_MAP_HASHFUN(key);
    unsigned level = 0;

    while (1) {
        if (node->is_leaf) {
            // Chercher si la clé existe déjà
            ht_entry_t *e = node->leaf.entries;
            while (e) {
                if (e->key == key) {
                    e->value = value; // update
                    return 1;
                }
                e = e->next;
            }
            // Ajouter nouvelle entrée
            ht_entry_t *ne = calloc(1, sizeof(*ne));
            if (!ne) return 0;
            ne->key = key;
            ne->value = value;
            ne->next = node->leaf.entries;
            node->leaf.entries = ne;
            node->leaf.count++;
            map->length++;
            return 1;
        } else {
            // Passer au bon enfant
            uint32_t idx = (hash >> (level * BTREE_BITS)) & LEVEL_MASK;
            if (!node->children[idx]) node->children[idx] = ht_new_leaf();
            node = node->children[idx];
            level++;
            if (level >= MAX_LEVELS) node->is_leaf = 1; // sécurité si trop profond
        }
    }
}

/* --- Contains / get --- */
static ht_entry_t *ht_find(ht_node_t *n, T_MAP_KEY key, unsigned level) {
    if(!n) return NULL;
    if(n->is_leaf) {
        ht_entry_t *e = n->leaf.entries;
        while(e) {
            if(e->key == key) return e;
            e = e->next;
        }
        return NULL;
    } else {
        uint64_t h = (uint64_t) T_MAP_HASHFUN(key);
        uint32_t idx = ht_get_slice(h, level);
        return ht_find(n->children[idx], key, level+1);
    }
}

T_MAP_INTERFACE int MAP_METHOD(contains)(T map, T_MAP_KEY key) {
    return ht_find(map->root, key, 0) != NULL;
}

#if !defined(T_SET_ELEMENT)
T_MAP_INTERFACE int MAP_METHOD(get)(T map, T_MAP_KEY key, T_MAP_VALUE *value) {
    ht_entry_t *e = ht_find(map->root, key, 0);
    if(!e) return 0;
    *value = e->value;
    return 1;
}
#endif

/* --- Remove --- */
static int ht_remove_recursive(ht_node_t **node_ptr, T_MAP_KEY key, unsigned level) {
    ht_node_t *n = *node_ptr;
    if(!n) return 0;
    if(n->is_leaf) {
        ht_entry_t *prev = NULL;
        ht_entry_t *cur = n->leaf.entries;
        while(cur) {
            if(cur->key == key) {
                if(prev) prev->next = cur->next;
                else n->leaf.entries = cur->next;
                free(cur);
                n->leaf.count--;
                return 1;
            }
            prev = cur;
            cur = cur->next;
        }
        return 0;
    } else {
        uint64_t h = (uint64_t) T_MAP_HASHFUN(key);
        uint32_t idx = ht_get_slice(h, level);
        return ht_remove_recursive(&n->children[idx], key, level+1);
    }
}

T_MAP_INTERFACE int MAP_METHOD(remove)(T *map_ptr, T_MAP_KEY key) {
    if(!map_ptr || !*map_ptr) return 0;
    int r = ht_remove_recursive(&(*map_ptr)->root, key, 0);
    if(r) (*map_ptr)->length--;
    return r;
}

#endif

#endif

#undef T_IMPL_HASHTRIE
