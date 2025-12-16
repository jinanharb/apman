#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* ==== DEFINITIONS FOR THE MAP ==== */

#define T_MAP_TAG bmap
typedef int key_t;
#define T_MAP_KEY key_t

typedef const char * value_t;
#define T_MAP_VALUE value_t

// Hash function is just identity for integers
static inline int hash_id(key_t key) {
    return key;
}

#define T_MAP_HASHFUN hash_id

static inline int equals(key_t k1, key_t k2) {
    return k1 == k2;
}
#define T_IMPL_BINARY_SEARCH_MAP
// Include your generic binary search map implementation
#include <libellul/type/map.h>

int main(void) {

    printf("[1] Test bmap_new...\n");
    bmap_t map = bmap_new();
    assert(map != NULL);

    printf("[2] Test bmap_length == 0...\n");
    assert(bmap_length(map) == 0);

    printf("[3] Test bmap_contains on missing key...\n");
    assert(bmap_contains(map, 10) == 0);

    printf("[4] Test bmap_put (insert)...\n");
    assert(bmap_put(&map, 10, "hello") == 1);
    assert(bmap_length(map) == 1);

    printf("[5] Test bmap_contains on existing key...\n");
    assert(bmap_contains(map, 10) == 1);

    /* ==== Test bmap_get ==== */
    value_t val;
    int ok;

    ok = bmap_get(map, 10, &val);
    assert(ok == 1);
    assert(strcmp(val, "hello") == 0);

    ok = bmap_get(map, 99999, &val);
    assert(ok == 0); 

    printf("[6] Test bmap_put overwrite...\n");
    assert(bmap_put(&map, 10, "world") == 1);
    assert(bmap_contains(map, 10) == 1);

    printf("[7] Test multiple insertions (1..10)...\n");
    for (int i = 1; i <= 10; i++)
        assert(bmap_put(&map, i, "x") == 1);

    printf("[8] Verify contains for 1..10...\n");
    for (int i = 1; i <= 10; i++)
        assert(bmap_contains(map, i) == 1);

    printf("[9] Test remove on existing key...\n");
    assert(bmap_remove(&map, 10) == 1);
    assert(bmap_contains(map, 10) == 0);

    printf("[10] Test remove on missing key...\n");
    assert(bmap_remove(&map, 99999) == 0);

    printf("[11] Test re-insertion after remove...\n");
    assert(bmap_put(&map, 10, "again") == 1);
    assert(bmap_contains(map, 10) == 1);

    printf("[12] Test bmap_delete...\n");
    bmap_delete(&map);
    assert(map == NULL);

    printf("All binary search map tests PASSED!\n");
    return 0;
}