#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define T_MAP_TAG dict
#define T_MAP_KEY int
typedef char *string_t;
#define T_MAP_VALUE string_t
static inline unsigned hash_int(int x) { return (unsigned)x * 2654435761u; }
#define T_MAP_HASHFUN hash_int
#define T_IMPL_HASHTABLE_ROBINHOOD
#include <libellul/type/map.h>




/* ----------------------------------------------------------- */

int main(void) {

 printf("[1] Test dict_new...\n");
 dict_t m = dict_new();
 assert(m != NULL);

 printf("[2] Test length == 0...\n");
 assert(dict_length(m) == 0);

 printf("[3] Test contains absent...\n");
 assert(dict_contains(m, 42) == 0);

 printf("[4] Test put...\n");
 assert(dict_put(&m, 42, "hello") == 1);
 assert(dict_contains(m, 42) == 1);

 /*printf("[5] Test get...\n");
 string_t v = NULL;
 assert(dict_get(m, 42, &v) == 1);
 assert(strcmp(v, "hello") == 0);*/

 /*printf("[6] Test overwrite...\n");
 assert(dict_put(&m, 42, "world") == 1);
 assert(dict_get(m, 42, &v) == 1);
 assert(strcmp(v, "world") == 0);*/

 printf("[7] Test multiple insertions...\n");
 for (int i = 1; i <= 70; i++)
 assert(dict_put(&m, i, "x") == 1);

 printf("[8] Test contains(1..70)...\n");
 for (int i = 1; i <= 70; i++)
 assert(dict_contains(m, i) == 1);

 /*printf("[9] Test remove exist...\n");
 assert(dict_redict_getmove(&m, 10) == 1);
 assert(dict_contains(m, 10) == 0);*/

 printf("[10] Test remove absent...\n");
 assert(dict_remove(&m, 999) == 0);

 printf("[11] Reinsertion after deletion...\n");
 assert(dict_put(&m, 10, "again") == 1);
 assert(dict_contains(m, 10) == 1);

 printf("[12] Delete dictionary...\n");
 dict_delete(&m);
 assert(m == NULL);

 printf("Robin Hood: All the codes passed!\n");
 return 0;
}