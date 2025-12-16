#include <libellul.h>
#include "benchmark.h"
#include <libellul/type/array.h>
#include <libellul/clock.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Return average TOTAL TIME per run (not per element!) */
static void benchmark_new(size_t N, int runs)
{
  double total_time = 0;

  for(int run = 0; run < runs; run++) {
    elapsed_nsec();
    int *arr = array_new(N, int);
    total_time += elapsed_nsec();
    array_delete(&arr);
  }

  printf("%10.3g\t,", total_time / runs);
}

static void benchmark_push(size_t N, int runs)
{
  double total_time = 0;

  for(int run = 0; run < runs; run++) {
    int *arr = array_new(0, int);

    elapsed_nsec();
    for(size_t i = 0; i < N; i++)
      array_push(&arr, (int)i);
    total_time += elapsed_nsec();

    array_delete(&arr);
  }

  printf("%10.3g\t,", total_time / runs);
}

static void benchmark_read(size_t N, int runs)
{
  double total_time = 0;

  int *arr = array_new(N, int);
  for(size_t i = 0; i < N; i++)
    arr[i] = (int)i;

  volatile int sink = 0;

  for(int run = 0; run < runs; run++) {
    elapsed_nsec();
    for(size_t i = 0; i < N; i++)
      sink ^= arr[rand() % N];
    total_time += elapsed_nsec();
  }

  array_delete(&arr);

  printf("%10.3g\t,", total_time / runs);
}

static void benchmark_write(size_t N, int runs)
{
  double total_time = 0;

  int *arr = array_new(N, int);

  for(int run = 0; run < runs; run++) {
    elapsed_nsec();
    for(size_t i = 0; i < N; i++)
      arr[rand() % N] = (int)i;
    total_time += elapsed_nsec();
  }

  array_delete(&arr);

  printf("%10.3g\t,", total_time / runs);
}

static void benchmark_pop(size_t N, int runs)
{
  double total_time = 0;

  for(int run = 0; run < runs; run++) {

    int *arr = array_new(N, int);
    for(size_t i = 0; i < N; i++)
      arr[i] = (int)i;

    int x = 0;

    elapsed_nsec();
    for(size_t i = 0; i < N; i++)
      array_pop(&arr, &x);
    total_time += elapsed_nsec();

    array_delete(&arr);
  }

  printf("%10.3g\t,", total_time / runs);
}

int main(void)
{
  srand(42);

  fprintf(stderr, "%g\r", elapsed_nsec());
  printf("log2(N)\t,N\t,new\t,push\t,read\t,write\t,pop\t\n");

  for(size_t log2_N = LOG2_N_MIN; log2_N <= LOG2_N_MAX; log2_N++) {
    size_t N = 1 << log2_N;

    fprintf(stderr, "Array N = %zu\r", N);
    printf("%10zu\t,%10zu\t,", log2_N, N);

    benchmark_new(N, RUNS);
    benchmark_push(N, RUNS);
    benchmark_read(N, RUNS);
    benchmark_write(N, RUNS);
    benchmark_pop(N, RUNS);

    printf("\n");
  }

  fprintf(stderr, "--END OF %s--\n", __FILE__);
  return 0;
}
