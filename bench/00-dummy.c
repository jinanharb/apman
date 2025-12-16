#include <libellul.h>

#include "benchmark.h"

#include <stdio.h>

static int  onheap( size_t N ) 
{
  int  sum = 0;
  int *vec = calloc( N, sizeof( *vec ) );
  assert( vec );

  for ( size_t i = 0 ; i < N ; i++ ) 
  {
    vec[ i ] = i;
  }

  for ( size_t i = 0 ; i < N ; i++ ) 
  {
    sum += vec[ i ];
  }

  free( vec );

  return sum;
}

static int  onstack( size_t N ) 
{
  int  sum = 0;
  int vec[ N ];

  for ( size_t i = 0 ; i < N ; i++ ) 
  {
    vec[ i ] = i;
  }

  for ( size_t i = 0 ; i < N ; i++ ) 
  {
    sum += vec[ i ];
  }

  return sum;
}

static void benchmark( size_t N, int runs ) 
{
  int s1, s2;

  /* Put dummy values in first two columns of CSV file */
  printf( "%10zu\t,", N );
  printf( "%10zu\t,", N/2 );

  elapsed_nsec(); /* Reset clock */

  for ( int run = 0 ; run < runs ; run++ )
    s1 = run + onheap( N ); /* Perform test for onheap column */

  printf( "%10.2g\t,", elapsed_nsec()/(runs*N) ); /* Normalize execution time by N  */

  elapsed_nsec();

  for ( int run = 0 ; run < runs ; run++ )
    s2 = run + onstack( N );

  printf( "%10.2g\t,", elapsed_nsec()/(runs*N) ); /* Normalize execution time by N  */

  /* Actually use the values so the compiler keeps all code */
  if ( s1 != s2 ) abort();
}

int main () {

  /* Warmup: Force dummy calls to all useful functions beforehand */
  fprintf( stderr, "%g\r", elapsed_nsec() );
  onstack( LOG2_N_MAX );

  /* Print CSV header */
  printf( " log2(N)   \t,  Op1      \t,  Op2      \t,  onheap     \t, onstack   \t\n" );

  for ( size_t log2_N = LOG2_N_MIN ; log2_N < LOG2_N_MAX+1 ; log2_N++ ) {
    fprintf( stderr, "N = %zu\r", (size_t)1 << log2_N ); /* User output on stderr */

    printf( "%10zu\t, ", log2_N ); /* CSV on standard output */

    benchmark( 1 << log2_N, RUNS );

    printf( "\n" ); /* End of CSV line  */
  }

  fprintf( stderr, "--END OF %s--\n", __FILE__ ); /* User output */

  exit( EXIT_SUCCESS );
}
