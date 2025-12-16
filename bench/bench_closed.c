#include <libellul.h>
#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct {
  link_t link;
  uint64_t key;
  uint64_t value;
} entry_t;

#define T_MAP_TAG dict
#define T_MAP_KEY uint64_t
#define T_MAP_ENTRY entry_t

#define T_IMPL_HASHTABLE_CLOSED
#include <libellul/structure/hashtable.h>

size_t dict_hash(const uint64_t *key) {
  uint64_t z = *key;
  z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
  z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
  z = z ^ (z >> 31);
  return (size_t)z;
}

int dict_eq(const uint64_t *a, const uint64_t *b) {
  return *a == *b;
}

static uint64_t rand_uint64(void) {
  uint64_t r = 0;
  for (int i = 0; i < 64; i += 15) {
    r = (r << 15) | (rand() & 0x7FFF);
  }
  return r;
}

static void benchmark_insert(size_t N, int runs) {
  double total_time = 0;

  uint64_t *keys = malloc(N * sizeof(uint64_t));
  uint64_t *values = malloc(N * sizeof(uint64_t));
  for (size_t i = 0; i < N; i++) {
    keys[i] = rand_uint64();
    values[i] = rand_uint64();
  }
  
  for (int run = 0; run < runs; run++) {
    T ht = dict_new();
    entry_t *entries = malloc(N * sizeof(entry_t));
    
    elapsed_nsec();
    
    for (size_t i = 0; i < N; i++) {
      entries[i].key = keys[i];
      entries[i].value = values[i];
      link_init(&entries[i].link);
      dict_insert(ht, &entries[i]);
    }
    
    total_time += elapsed_nsec();
    
    dict_delete(ht);
    free(entries);
  }
  
  free(keys);
  free(values);
  printf("%10.2g\t,", total_time / (runs * N));
}

static void benchmark_find(size_t N, int runs) {
  double total_time = 0;

  uint64_t *keys = malloc(N * sizeof(uint64_t));
  uint64_t *values = malloc(N * sizeof(uint64_t));
  for (size_t i = 0; i < N; i++) {
    keys[i] = rand_uint64();
    values[i] = rand_uint64();
  }

  T ht = dict_new();
  entry_t *entries = malloc(N * sizeof(entry_t));
  for (size_t i = 0; i < N; i++) {
    entries[i].key = keys[i];
    entries[i].value = values[i];
    link_init(&entries[i].link);
    dict_insert(ht, &entries[i]);
  }
  
  for (int run = 0; run < runs; run++) {
    elapsed_nsec(); 
    
    for (size_t i = 0; i < N; i++) {
      dict_find(ht, &keys[i]);
    }
    
    total_time += elapsed_nsec();
  }
  
  printf("%10.2g\t,", total_time / (runs * N));
  
  dict_delete(ht);
  free(entries);
  free(keys);
  free(values);
}

static void benchmark_remove(size_t N, int runs) {
  double total_time = 0;

  uint64_t *keys = malloc(N * sizeof(uint64_t));
  uint64_t *values = malloc(N * sizeof(uint64_t));
  for (size_t i = 0; i < N; i++) {
    keys[i] = rand_uint64();
    values[i] = rand_uint64();
  }
  
  for (int run = 0; run < runs; run++) {
    T ht = dict_new();
    entry_t *entries = malloc(N * sizeof(entry_t));
    
    for (size_t i = 0; i < N; i++) {
      entries[i].key = keys[i];
      entries[i].value = values[i];
      link_init(&entries[i].link);
      dict_insert(ht, &entries[i]);
    }
    
    elapsed_nsec(); 
    
    for (size_t i = 0; i < N; i++) {
      dict_remove(ht, &keys[i]);
    }
    
    total_time += elapsed_nsec();
    
    dict_delete(ht);
    free(entries);
  }
  
  free(keys);
  free(values);
  printf("%10.2g\t,", total_time / (runs * N));
}

int main() {

  srand(42);
  
  fprintf(stderr, "%g\r", elapsed_nsec());
  
  printf("log2(N)\t,N\t,insert\t,find\t,remove\t\n");
  
  for (size_t log2_N = LOG2_N_MIN; log2_N < LOG2_N_MAX + 1; log2_N++) {
    size_t N = 1 << log2_N;
    fprintf(stderr, "Closed N = %zu\r", N);
    
    printf("%10zu\t,%10zu\t,", log2_N, N);
    
    benchmark_insert(N, RUNS);
    benchmark_find(N, RUNS);
    benchmark_remove(N, RUNS);
    
    printf("\n");
  }
  
  fprintf(stderr, "--END OF %s--\n", __FILE__);
  
  exit(EXIT_SUCCESS);
}