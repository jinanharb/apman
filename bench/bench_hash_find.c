#include <libellul.h>
#include "benchmark.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* ============================================
   LINEAR hashtable (T is already a pointer!)
   ============================================ */
#define T_MAP_TAG dictlinear
#define T_MAP_KEY int
#define T_MAP_VALUE int
#define T_MAP_HASHFUN(key) ((uint64_t)(key) * 11400714819323198485llu)
#define T_IMPL_HASHTABLE_LINEAR
#include <libellul/structure/hashtable/linear.h>

/* ============================================
   CLOSED hashtable
   ============================================ */
typedef struct {
    link_t link;
    int key;
    int value;
} closed_entry_t;

size_t closed_map_hash(const int *key) {
    return (uint64_t)(*key) * 11400714819323198485llu;
}

int closed_map_eq(const int *a, const int *b) {
    return *a == *b;
}

#define T_CLOSED_TAG closed_map
#define T_CLOSED_ENTRY closed_entry_t
#define T_CLOSED_KEY int
#define T_CLOSED_NBUCKETS 128
#include <libellul/structure/hashtable/closed.h>

/* ============================================
   Benchmarks
   ============================================ */
static int bench_linear(size_t N) {
    dictlinear_t map = dictlinear_new();
    for (size_t i = 0; i < N; i++) {
        dictlinear_put(&map, (int)i, (int)i);
    }
    
    int found = 0;
    for (size_t i = 0; i < N; i++) {
        found += dictlinear_contains(map, (int)i);
    }
    
    dictlinear_delete(&map);
    return found;
}

static int bench_closed(size_t N) {
    closed_entry_t *entries = malloc(N * sizeof(closed_entry_t));
    closed_map_t map = closed_map_new();
    
    for (size_t i = 0; i < N; i++) {
        entries[i].key = (int)i;
        entries[i].value = (int)i;
        closed_map_insert(&map, &entries[i]);
    }
    
    int found = 0;
    for (size_t i = 0; i < N; i++) {
        int k = (int)i;
        if (closed_map_find(&map, &k))
            found++;
    }
    
    closed_map_delete(&map);
    free(entries);
    return found;
}

static void benchmark(size_t N, int runs) {
    int s1, s2;

    printf("%10zu\t,", N);
    printf("%10zu\t,", N/2);

    elapsed_nsec();
    for (int run = 0; run < runs; run++)
        s1 = bench_linear(N);
    printf("%10.2g\t,", elapsed_nsec()/(runs*N));

    elapsed_nsec();
    for (int run = 0; run < runs; run++)
        s2 = bench_closed(N);
    printf("%10.2g\t,", elapsed_nsec()/(runs*N));

    if (s1 != (int)N || s2 != (int)N) abort();
}

int main() {
    fprintf(stderr, "FIND: Linear vs Closed\n");
    bench_linear(10);
    bench_closed(10);

    printf(" log2(N)   \t,  Op1      \t,  Op2      \t,  linear     \t, closed    \t\n");

    for (size_t log2_N = LOG2_N_MIN; log2_N < LOG2_N_MAX + 1; log2_N++) {
        size_t N = (size_t)1 << log2_N;
        fprintf(stderr, "N = %zu\n", N);

        printf("%10zu\t, ", log2_N);
        benchmark(N, RUNS);
        printf("\n");
    }

    fprintf(stderr, "--END--\n");
    exit(EXIT_SUCCESS);
}
