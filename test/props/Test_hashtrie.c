#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
/* ==== DÉFINITIONS ATTENDUES PAR TA LIBRAIRIE ==== */

#define T_MAP_TAG dict
// Clé = int
typedef int key_t;
#define T_MAP_KEY key_t

typedef const char * value_t;
#define T_MAP_VALUE value_t

// Hash simple
static inline uint64_t hash_id(key_t key) {
    return (uint64_t)key;
}
#define T_MAP_HASHFUN hash_id

static inline int equals(key_t key1, key_t key2) {
    return key1 == key2;
}

#define T_IMPL_HASHTRIE
#include <libellul/type/map.h>

int main(void) {
    printf("[1] Test dict_new...\n");
    dict_t map = dict_new();
    assert(map != NULL);

    printf("[2] Test dict_length == 0...\n");
    assert(dict_length(map) == 0);

    printf("[3] Test dict_contains sur clé absente...\n");
    assert(dict_contains(map, 10) == 0);

    printf("[4] Test dict_put (insertion)...\n");
    assert(dict_put(&map, 10, "hello") == 1);
    assert(dict_length(map) == 1);

    printf("[5] Test dict_contains (clé présente)...\n");
    assert(dict_contains(map, 10) == 1);

    /* ==== Test dict_get ==== */
    value_t val;
    int ok;

    ok = dict_get(map, 10, &val);
    assert(ok == 1);
    assert(strcmp(val, "hello") == 0);

    ok = dict_get(map, 99999, &val);
    assert(ok == 0); 

    printf("[6] Test dict_put (overwrite)...\n");
    assert(dict_put(&map, 10, "world") == 1);
    ok = dict_get(map, 10, &val);
    assert(ok == 1);
    assert(strcmp(val, "world") == 0);

    printf("[7] Test insertion multiple (1..10)...\n");
    for(int i=1; i<=10; i++)
        assert(dict_put(&map, i, "x") == 1);

    printf("[8] Vérification contains pour 1..10...\n");
    for(int i=1; i<=10; i++)
        assert(dict_contains(map, i) == 1);

    printf("[9] Test dict_remove sur clé existante...\n");
    assert(dict_remove(&map, 10) == 1);
    assert(dict_contains(map, 10) == 0);

    printf("[10] Test dict_remove sur clé absente...\n");
    assert(dict_remove(&map, 99999) == 0);

    printf("[11] Test réinsertion après remove...\n");
    assert(dict_put(&map, 10, "again") == 1);
    assert(dict_contains(map, 10) == 1);
    ok = dict_get(map, 10, &val);
    assert(ok == 1);
    assert(strcmp(val, "again") == 0);

    printf("[12] Test dict_delete...\n");
    dict_delete(&map);
    assert(map == NULL);

    printf("Tous les tests de hashtrie sont PASSÉS sans erreur !\n");
    return 0;
}