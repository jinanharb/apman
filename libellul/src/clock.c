#include <libellul/clock.h>

struct timespec time_point;

#define CLOCK_TYPE  CLOCK_MONOTONIC

double  elapsed_nsec( void ) {
  size_t           ns;
  struct timespec now;

  clock_gettime( CLOCK_TYPE, &now );

  ns  = 1e9 * ( now.tv_sec - time_point.tv_sec );
  ns += now.tv_nsec - time_point.tv_nsec;

  time_point = now;

  return ns;
}

double  elapsed_usec( void ) {
  return elapsed_nsec() / 1e3;
}

double  elapsed_msec( void ) {
  return elapsed_nsec() / 1e6;
}


void    tick( struct timespec *ts ) {
  clock_gettime( CLOCK_TYPE, ts );
}

double  tock_nsec( struct timespec *ts ) {
  size_t           ns;
  struct timespec now;

  clock_gettime( CLOCK_TYPE, &now );

  ns  = 1e9 * ( now.tv_sec - ts->tv_sec );
  ns += now.tv_nsec - ts->tv_nsec;

  *ts = now;

  return ns;
}

double  tock_usec( struct timespec *ts ) {
  return tock_nsec( ts ) / 1e3;
}

double  tock_msec( struct timespec *ts ) {
  return tock_nsec( ts ) / 1e6;
}

#undef CLOCK_TYPE
