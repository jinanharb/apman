/**
 * @file test-robinhood.c
 * @brief Functional tests for the Robin-Hood hashtable.
 */

#include <libellul.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define T_MAP_KEY int
#define T_MAP_VALUE const char *
#define T_IMPL_HASHTABLE_ROBIN_HOOD


int main(int argc, char *argv[]) {

    unit_test(argc, argv);

    test_suite("Robin Hood Hashtable");

    
    T map = MAP_METHOD(new)();
    test_assert(map != NULL, "Map created successfully");
    test_assert(MAP_METHOD(length)(map) == 0, "Length should be zero on new map");

    
    test_assert(MAP_METHOD(put)(&map, 1, "one") == 1,
                "Inserting new key should return 1");
    test_assert(MAP_METHOD(length)(map) == 1,
                "After one insert, length should be 1");
    test_assert(MAP_METHOD(contains)(map, 1) == 1,
                "Key 1 must be contained");

    
    const char *val = NULL;
    test_assert(MAP_METHOD(get)(map, 1, &val) == 1,
                "get(1) should succeed");
    test_assert(strcmp(val, "one") == 0,
                "get(1) should return \"one\"");

    
    test_assert(MAP_METHOD(put)(&map, 1, "uno") == 0,
                "Inserting existing key returns 0");
    MAP_METHOD(get)(map, 1, &val);
    test_assert(strcmp(val, "uno") == 0,
                "Overwritten key should have new value");

    
    MAP_METHOD(put)(&map, 2, "two");
    MAP_METHOD(put)(&map, 3, "three");

    test_assert(MAP_METHOD(length)(map) == 3,
                "Length should be 3 after inserting 3 keys");

   
    test_assert(MAP_METHOD(remove)(&map, 2) == 1,
                "remove(2) should succeed");
    test_assert(MAP_METHOD(contains)(map, 2) == 0,
                "After removal, key 2 must not be contained");
    test_assert(MAP_METHOD(length)(map) == 2,
                "Length must decrease after remove");

   
    test_assert(MAP_METHOD(remove)(&map, 42) == 0,
                "Removing non-existing key returns 0");

   
    test_oracle_start(stdout);
    printf("Key 1 = %s\n", val);
    test_oracle_check("Oracle test", "Key 1 = uno");


    MAP_METHOD(delete)(&map);
    test_assert(map == NULL, "Map must be NULL after delete");

    exit(EXIT_SUCCESS);
}
