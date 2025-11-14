#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <libellul.h>

#define HASH_FUN(key) ((unsigned long)(key) * 2654435761u % 4294967296u)
#define EQ_FUN(a, b) ((a) == (b))

typedef int T_MAP_KEY;
typedef const char* T_MAP_VALUE;

int main(void) {
    printf("=== TESTS HASHTABLE LINEAR ===\n");

    // Création
    T map = MAP_METHOD(new)();
    assert(map != NULL);
    printf("Création OK\n");

    // Vérifier que la table est vide
    assert(MAP_METHOD(length)(map) == 0);
    assert(MAP_METHOD(contains)(map, 42) == 0);
    printf("Table vide OK\n");

    // Insertion d'un élément
    int success = MAP_METHOD(put)(&map, 42, "hello");
    assert(success == 1);
    assert(MAP_METHOD(length)(map) == 1);
    printf("Insertion OK\n");

    // Lecture d'un élément
    T_MAP_VALUE value = NULL;
    int found = MAP_METHOD(get)(map, 42, &value);
    assert(found == 1);
    assert(strcmp(value, "hello") == 0);
    printf("Lecture OK: %s\n", value);

    // Vérification contains
    assert(MAP_METHOD(contains)(map, 42) == 1);
    printf("Contains OK\n");

    // Suppression
    int removed = MAP_METHOD(remove)(&map, 42);
    assert(removed == 1);
    assert(MAP_METHOD(length)(map) == 0);
    assert(MAP_METHOD(contains)(map, 42) == 0);
    printf("Suppression OK\n");

    // Suppression clé absente
    removed = MAP_METHOD(remove)(&map, 99);
    assert(removed == 0);
    printf("Suppression inexistante OK\n");

    // Libération mémoire
    MAP_METHOD(delete)(&map);
    assert(map == NULL);
    printf("Suppression de la table OK\n");

    printf("Tous les tests sont passés.\n");

    return 0;
}
