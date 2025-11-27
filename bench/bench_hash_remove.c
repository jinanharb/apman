#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <libellul.h>
#include "benchmark.h"
#include <string.h>

/* ============================================
   Closed entry struct
   ============================================ */
typedef struct {
    link_t link;
    int key;
    int value;
} closed_entry_t;

/* ============================================
   Hash + equality functions for closed
   ============================================ */
size_t closed_map_hash(const int *key) {
    return (uint64_t)(*key) * 11400714819323198485llu;
}

int closed_map_eq(const int *a, const int *b) {
    return *a == *b;
}

/* ============================================
   Define closed hash table
   ============================================ */
#define T_CLOSED_TAG closed_map
#define T_CLOSED_ENTRY closed_entry_t
#define T_CLOSED_KEY int
#define T_CLOSED_NBUCKETS 128
#include <libellul/structure/hashtable/closed.h>

/* ============================================
   Benchmark for Linear Remove
   ============================================ */
double benchmark_linear_remove(size_t N) {
    uint64_t t0 = get_time_nsec();
    for(int run = 0; run < RUNS; run++) {
        #define T_MAP_TAG dictlinear
        #define T_MAP_KEY int
        #define T_MAP_VALUE int
        #define T_MAP_HASHFUN(key) ((uint64_t)(key) * 11400714819323198485llu)
        #define T_IMPL_HASHTABLE_LINEAR
        #include <libellul/structure/hashtable/linear.h>
        
        T map = dictlinear_new();
        
        // Populate
        for (size_t key = 0; key < N; key++) {
            dictlinear_put(&map, (int)key, (int)key);
        }
        
        // Remove all (this is what we're timing)
        for (size_t key = 0; key < N; key++) {
            int removed = dictlinear_remove(&map, (int)key);
            assert(removed);
        }
        
        dictlinear_delete(&map);
        
        #undef T_MAP_TAG
        #undef T_MAP_KEY
        #undef T_MAP_VALUE
        #undef T_MAP_HASHFUN
        #undef T_IMPL_HASHTABLE_LINEAR
    }
    uint64_t t1 = get_time_nsec();
    return (double)(t1 - t0) / (RUNS * N);
}

/* ============================================
   Benchmark for Closed Remove
   ============================================ */
double benchmark_closed_remove(size_t N) {
    uint64_t t0 = get_time_nsec();
    for(int run = 0; run < RUNS; run++) {
        closed_map_t map = closed_map_new();
        
        // Populate
        for(size_t key = 0; key < N; key++) {
            closed_entry_t entry = { .key = (int)key, .value = (int)key };
            closed_map_insert(&map, &entry);
        }
        
        // Remove all (this is what we're timing)
        for(size_t key = 0; key < N; key++) {
            int k = (int)key;
            int removed = closed_map_remove(&map, &k);
            assert(removed == 0);
        }
        
        closed_map_delete(&map);
    }
    uint64_t t1 = get_time_nsec();
    return (double)(t1 - t0) / (RUNS * N);
}

/* ============================================
   Main
   ============================================ */
int main(void) {
    printf("log2(N),linear(ns/op),closed(ns/op)\n");
    for(size_t logN = LOG2_N_MIN; logN <= LOG2_N_MAX; logN++) {
        size_t N = (size_t)1 << logN;
        double t_linear = benchmark_linear_remove(N);
        double t_closed = benchmark_closed_remove(N);
        printf("%zu,%f,%f\n", logN, t_linear, t_closed);
    }
    return 0;
}