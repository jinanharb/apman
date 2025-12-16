#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libellul.h>
#include <libellul/type/deques.h>



// user defined structure
typedef struct {
    link_t link;    
    int key;          
    const char *value;
} my_entry_t;

#define T_MAP_TAG dict
#define T_MAP_KEY int
#define T_MAP_ENTRY my_entry_t

// Specifying closed implementation before including map.h
#define T_IMPL_HASHTABLE_CLOSED

// map.h includes the right file: closed.h
#include <libellul/type/map.h>

size_t dict_hash(const int *key) {
    return (size_t)(*key);
}

int dict_eq(const int *a, const int *b) {
    return *a == *b;
}

static void count_entries(my_entry_t *entry, void *ud) {
    (void)entry;
    int *counter = (int*)ud;
    (*counter)++;
}


int main(void) {
    printf("[1] Test dict_new...\n");
    dict_t table = dict_new();
    assert(table->buckets != NULL);
    assert(dict_length(table) == 0);
    
    printf("[2] Test dict_length == 0...\n");
    assert(dict_length(table) == 0);
    assert(dict_is_empty(table) == 1);
    
    printf("[3] Test dict_find sur clé absente...\n");
    int key_test = 10;
    assert(dict_find(table, &key_test) == NULL);
    
    printf("[4] Test dict_insert (insertion)...\n");
    my_entry_t entry1 = {.key = 10, .value = "hello"};
    assert(dict_insert(table, &entry1) == 0);
    assert(dict_length(table) == 1);
    assert(dict_is_empty(table) == 0);
    
    printf("[5] Test dict_find (clé présente)...\n");
    my_entry_t *found = dict_find(table, &key_test);
    assert(found != NULL);
    assert(found->key == 10);
    assert(strcmp(found->value, "hello") == 0);
    
    printf("[6] Test dict_insert (overwrite)...\n");
    my_entry_t entry2 = {.key = 10, .value = "world"};
    assert(dict_insert(table, &entry2) == 0);
    found = dict_find(table, &key_test);
    assert(found != NULL);
    assert(strcmp(found->value, "world") == 0);
    assert(dict_length(table) == 1); 
    
    printf("[7] Test insertion multiple (1..50)...\n");
    static my_entry_t entries[50]; 
    for (int i = 1; i <= 50; i++) {
        entries[i-1].key = i;
        entries[i-1].value = "x";
        assert(dict_insert(table, &entries[i-1]) == 0);
    }
    assert(dict_length(table) == 50); 
    
    printf("[8] Vérification find pour 1..50...\n");
    for (int i = 1; i <= 50; i++) {
        int k = i;
        my_entry_t *e = dict_find(table, &k);
        assert(e != NULL);
        assert(e->key == i);
    }
  
    printf("[9] Test remove sur clé existante...\n");
    assert(dict_remove(table, &key_test) == 0);
    assert(dict_find(table, &key_test) == NULL);
    assert(dict_length(table) == 49);
    
    printf("[10] Test remove sur clé absente...\n");
    int key_absent = 99999;
    assert(dict_remove(table, &key_absent) == -1);
    assert(dict_length(table) == 49);
    
    printf("[11] Test réinsertion après remove...\n");
    static my_entry_t entry3 = {.key = 10, .value = "again"};  
    assert(dict_insert(table, &entry3) == 0);
    found = dict_find(table, &key_test);
    assert(found != NULL);
    assert(strcmp(found->value, "again") == 0);
    assert(dict_length(table) == 50);
    
    printf("[12] Test dict_foreach...\n");
    int count = 0;
    dict_foreach(table, count_entries, &count);
    assert(count == 50);
    
    printf("[13] Test bucket_count...\n");
    size_t nbuckets = dict_bucket_count(table);
    printf("    Nombre de buckets: %zu\n", nbuckets);
    
    printf("[14] Test dict_delete...\n");
    dict_delete(table);
    table = NULL;
    
    printf("\n✓ Tous les tests de closed sont PASSÉS sans erreur !\n");
    return 0;
}