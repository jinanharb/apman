#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>
#include <time.h>

/* Number of times to run each benchmark */
#ifndef RUNS
#define RUNS 100
#endif

/* Range of N values to test: 2^LOG2_N_MIN to 2^LOG2_N_MAX */
#ifndef LOG2_N_MIN
#define LOG2_N_MIN 0
#endif

#ifndef LOG2_N_MAX
#define LOG2_N_MAX 19
#endif

/* Get elapsed time in nanoseconds */
static inline uint64_t get_time_nsec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

#endif /* BENCHMARK_H */