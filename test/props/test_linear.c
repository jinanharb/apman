#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* ==== DÉFINITIONS ATTENDUES PAR TA LIBRAIRIE ==== */

#define T_MAP_TAG dict
// La clé est un int
#define T_MAP_KEY int

typedef char *string_t;
#define T_MAP_VALUE string_t


// Préfixe pour générer map_new, map_put, etc.
static inline int hash_id( int key ) {
 return key;
}

#define T_MAP_HASHFUN hash_id

static inline int equals( int key1, int key2 ) {
 return key1 == key2;
}

#define T_MAP_HASHFUN hash_id
// Pas de mot-clé particulier

// Charge ton code générique (le .h après expansion des macros)
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

 printf("[6] Test dict_put (overwrite)...\n");
 assert(dict_put(&map, 10, "world") == 1);
 assert(dict_contains(map, 10) == 1);

 printf("[7] Test insertion multiple (1..50)...\n");
 for (int i = 1; i <= 50; i++)
 assert(dict_put(&map, i, "x") == 1);

 printf("[8] Vérification contains pour 1..50...\n");
 for (int i = 1; i <= 50; i++)
 assert(dict_contains(map, i) == 1);

 printf("[9] Test remove sur clé existante...\n");
 assert(dict_remove(&map, 10) == 1);
 assert(dict_contains(map, 10) == 0);

 printf("[10] Test remove sur clé absente...\n");
 assert(dict_remove(&map, 99999) == 0);

 printf("[11] Test réinsertion après tombstone...\n");
 assert(dict_put(&map, 10, "again") == 1);
 assert(dict_contains(map, 10) == 1);

 printf("[12] Test dict_delete...\n");
 dict_delete(&map);
 assert(map == NULL);

 printf("Tous les tests sont PASSÉS sans erreur !\n");
 return 0;
}