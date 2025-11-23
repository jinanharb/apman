#ifndef CLOSED_H
#define CLOSED_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h> // needed because your .c uses printf
#include <libellul.h>

/* Forward declaration of the hashtable structure */
typedef struct closed_entry_t closed_entry_t;

typedef struct 
{
deque_t *buckets;
size_t nbuckets;
size_t size;
} closed_t;

/* User must provide hash and equality functions */
typedef size_t (*closed_hash_fn)(const void *key);
typedef int (*closed_eq_fn)(const void *a, const void *b);

/* Functions */
void closed_init(closed_t *ht, size_t nbuckets, size_t entry_size, size_t key_offset);
void closed_destroy(closed_t *ht, size_t entry_size, size_t key_offset);
int closed_insert(closed_t *ht, void *entry, size_t key_offset, closed_hash_fn hash, closed_eq_fn eq);
void* closed_find(closed_t *ht, void *key, size_t key_offset, closed_hash_fn hash, closed_eq_fn eq);
void closed_foreach(closed_t *ht, size_t key_offset, void (*fn)(void *entry, void *ud), void *ud);

static inline size_t closed_size(closed_t *ht) { return ht ? ht->size : 0; }
static inline size_t closed_bucket_count(closed_t *ht) { return ht ? ht->nbuckets : 0; }

/* ------------------------- IMPLEMENTATION BELOW ------------------------- */

/* Init hashtable with number of buckets and entry offset for deque */
void closed_init(closed_t *ht, size_t nbuckets, size_t entry_size, size_t key_offset) {
assert(ht);
ht->nbuckets = nbuckets ? nbuckets : 16;
ht->size = 0;
ht->buckets = (deque_t*)calloc(ht->nbuckets, sizeof(deque_t));
assert(ht->buckets);

typedef struct { char dummy; } dummy_t; // dummy struct for offset

for (size_t i = 0; i < ht->nbuckets; ++i) {
deque_init(&ht->buckets[i], dummy_t, dummy); // use dummy
}
}

void closed_destroy(closed_t *ht, size_t entry_size, size_t key_offset) {
if (!ht || !ht->buckets) return;

// Iterate through all the buckets in the hashtable
for (size_t i = 0; i < ht->nbuckets; ++i) {
deque_t *dq = &ht->buckets[i];

// Reset the deque's internal state (but don't free the elements)
deque_reset(dq); // Don't free elements, just reset the deque
}

// After all elements are not freed, free the bucket array itself
free(ht->buckets);
ht->buckets = NULL;
ht->nbuckets = 0;
ht->size = 0; // Reset the size of the hashtable
}


int closed_insert(closed_t *ht, void *entry, size_t key_offset, closed_hash_fn hash, closed_eq_fn eq) {
if (!ht || !ht->buckets || !entry) {
fprintf(stderr, "Error: Invalid arguments in closed_insert\n");
return -1;
}

// Get the key from the entry
void *key = (char*)entry + key_offset;
size_t h = hash(key) % ht->nbuckets;
deque_t *dq = &ht->buckets[h];

if (!dq) {
fprintf(stderr, "Error: Invalid deque at bucket %zu\n", h);
return -1;
}

// Iterate through the deque to check if the key already exists
deque_foreach(dq, el) {
void *ekey = (char*)el + key_offset;
if (eq(ekey, key)) {
memcpy(el, entry, sizeof(*entry)); // Replace the existing entry
return 0; // Key found, replaced the entry
}
}

link_t *lnk = (link_t*)entry;
link_init(lnk); // Initialize the link if needed
deque_pushback(dq, entry); // Add the new entry to the deque
ht->size++; // Increment the size of the hashtable

return 0;
}


/* Find entry by key */
void* closed_find(closed_t *ht, void *key, size_t key_offset, closed_hash_fn hash, closed_eq_fn eq) {
if (!ht || !ht->buckets) return NULL;
size_t h = hash(key) % ht->nbuckets;
deque_t *dq = &ht->buckets[h];

deque_foreach(dq, el) {
void *ekey = (char*)el + key_offset;
if (eq(ekey, key)) return el;
}
return NULL;
}


/* Apply callback to every entry */
void closed_foreach(closed_t *ht, size_t key_offset, void (*fn)(void *entry, void *ud), void *ud) {
if (!ht || !ht->buckets || !fn) return;

for (size_t i = 0; i < ht->nbuckets; ++i) {
deque_t *dq = &ht->buckets[i];
deque_foreach(dq, el) {
fn(el, ud);
}
}
}

#endif /* CLOSED_H */