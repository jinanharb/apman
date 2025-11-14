#include <libellul.h>

static void array_NULL( void ) {
  int  value = 42;
  int *array = NULL;

  test_suite( "NULL as an array" );

  test_assert( 0 == array_length( NULL ), "NULL is an array of length zero" );
  test_assert( array_is_empty( NULL ), "NULL is an the empty array" );

  test_abort( array_resize( NULL, 42 ), "Cannot resize NULL" );

  array_delete( NULL );
  test_assert( 1, "Can delete NULL as an array" );


  test_suite( "NULL variable as an array" );

  test_assert( 0 == array_length( array ), "A NULL array has length zero" );
  test_assert( array_is_empty( array ), "A NULL array is empty" );

  array_delete( &array );
  test_assert( NULL == array, "Can delete a NULL array" );

  test_abort( array_push( &array, 3 ), "Cannot push to NULL array" );
  test_abort( array_pop( &array, &value ), "Cannot pop from NULL array" );

  test_abort( array_resize( &array, 42 ), "Cannot resize a NULL array" );
}

int *rand_ints( size_t n ) {
  int *vec = calloc( n, sizeof( *vec ) );
  assert( vec );

  for ( size_t i = 0 ; i < n ; i++ ) vec[ i ] = rand() % 50;

  return vec;
}

void array_stack( size_t n ) {
  int *truth = rand_ints( n );

  int *array = array( int );

  test_assert( array_is_empty( array ), "A new array is empty" );

  for ( size_t i = 0 ; i < n ; i++ ) {
    array_push( &array, truth[ i ] );
    test_assert( truth[ i ] == array[ i ], "Can push to array" );
  }

  for ( size_t i = 0 ; i < n ; i++ ) {
    int popped = -1;
    array_pop( &array, &popped );
    test_assert( truth[ n-1-i ] == popped, "Can pop from array" );
  }

  test_assert( array && array_is_empty( array ), "Empty, non-NULL array at the end" );

  array_delete( &array );
  test_assert( NULL == array, "Array is NULL after delete" );

  free( truth );
}

int main ( int argc, char *argv[] ) {

  unit_test( argc, argv );

  array_NULL();
  /* array_stack( 13 ); */

  exit( EXIT_SUCCESS );
}
