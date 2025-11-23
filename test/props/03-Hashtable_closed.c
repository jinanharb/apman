#include <libellul.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* --- Example entry type --- */
typedef struct {
int key;
int value;
link_t link; // used by deque internally
} int_entry_t;

/* Hash function */
static size_t int_hash(const void *k) {
const int *key = k;
return (size_t)(*key);
}

/* Equality function */
static int int_eq(const void *a, const void *b) {
return (*(const int*)a == *(const int*)b);
}

/* Generate random entries */
static int_entry_t* rand_entries(size_t n) 
{
int_entry_t *arr = calloc(n, sizeof(*arr)); // Dynamically allocate memory for the array
assert(arr); // Ensure memory allocation was successful
for (size_t i = 0; i < n; i++) {
arr[i].key = rand() % 100; // Random key
arr[i].value = rand() % 500; // Random value
}
return arr; // Return the dynamically allocated array
}


/* Test inserting, finding, and removing entries */
static void hashtable_basic_test(size_t n) {
int_entry_t *entries = rand_entries(n);
closed_t ht;

test_suite("Hashtable closed-addressing basic operations");

/* Initialize hashtable with link offset for deque */
closed_init(&ht, 7, sizeof(int_entry_t), offsetof(int_entry_t, link));
test_assert(ht.size == 0, "New hashtable is empty");
fflush(stdout);

/* Insert entries using key_offset for hashing/equality */
for (size_t i = 0; i < n; i++) {
int ret = closed_insert(&ht, &entries[i],
offsetof(int_entry_t, key), // key offset for hash/eq
int_hash, int_eq);
test_assert(ret == 0, "Insert returns 0");
fflush(stdout);
}
test_assert(ht.size == n, "All entries inserted");
fflush(stdout);

/* Find entries */
for (size_t i = 0; i < n; i++) 
{
int_entry_t *found = closed_find(&ht,
&entries[i].key,
offsetof(int_entry_t, key),
int_hash, int_eq);
test_assert(found != NULL, "Entry found");
fflush(stdout);
test_assert(found->value == entries[i].value, "Value matches inserted");
fflush(stdout);
}

/* Destroy table */
closed_destroy(&ht, sizeof(int_entry_t), offsetof(int_entry_t, link));
test_assert(ht.buckets == NULL && ht.size == 0, "Hashtable destroyed");
fflush(stdout);

free(entries);
}

/* Main test entry */
int main(int argc, char *argv[]) {
unit_test(argc, argv);

hashtable_basic_test(3);

exit(EXIT_SUCCESS);
}