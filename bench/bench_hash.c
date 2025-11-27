#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <libellul.h>
#include "benchmark.h"
#include <string.h>

/* ============================================
   Hash + equality functions
   ============================================ */
#define HASH_FUN(key) ((uint64_t)(key) * 11400714819323198485llu)
#define EQ_FUN(a,b) ((a) == (b))

/* ============================================
   Closed entry struct
   ============================================ */
typedef struct {
    int key;
    int value;
    link_t link; // required for closed deque
} closed_entry_t;

/* ============================================
   Benchmark for Linear
   ============================================ */
double benchmark_linear(size_t N) {
    uint64_t t0 = elapsed_nsec();
    for(int run = 0; run < RUNS; run++) {
        #define T_MAP_TAG dictlinear
        #define T_MAP_KEY int
        #define T_MAP_VALUE int
        #define T_IMPL_HASHTABLE_LINEAR
        #include <libellul/structure/hashtable/linear.h>

        T map = MAP_METHOD(new)();
        for (int key = 0; key < N; key++) {
            MAP_METHOD(put)(&map, key, key);
        }
        MAP_METHOD(delete)(&map);

        #undef T_MAP_TAG
        #undef T_MAP_KEY
        #undef T_MAP_VALUE
        #undef T_IMPL_HASHTABLE_LINEAR
    }
    uint64_t t1 = elapsed_nsec();
    return (double)(t1 - t0) / (RUNS * N);
}

/* ============================================
   Benchmark for Closed
   ============================================ */
double benchmark_closed(size_t N) {
    uint64_t t0 = elapsed_nsec();
    for(int run = 0; run < RUNS; run++) {
        closed_t map;
        closed_init(&map, N, sizeof(closed_entry_t), offsetof(closed_entry_t,key));

        for(int key = 0; key < N; key++) {
            closed_entry_t entry = { .key = key, .value = key };
            closed_insert(&map, &entry, offsetof(closed_entry_t,key), HASH_FUN, EQ_FUN);
        }

        closed_destroy(&map, sizeof(closed_entry_t), offsetof(closed_entry_t,key));
    }
    uint64_t t1 = elapsed_nsec();
    return (double)(t1 - t0) / (RUNS * N);
}

/* ============================================
   Main
   ============================================ */
int main(void) {
    printf("log2(N),linear(ns/op),closed(ns/op)\n");

    for(size_t logN = LOG2_N_MIN; logN <= LOG2_N_MAX; logN++) {
        size_t N = (size_t)1 << logN;

        double t_linear = benchmark_linear(N);
        double t_closed = benchmark_closed(N);

        printf("%zu,%f,%f\n", logN, t_linear, t_closed);
    }

    return 0;
}
