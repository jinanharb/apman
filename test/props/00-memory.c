#include <libellul.h>

static void memory( void ) {
  void *ptr;

  test_suite( "Testing allocation wrappers" );

  ptr = allocate( 42 );

  test_assert( NULL != ptr, "Can allocate memory with allocate()" );

  dispose( &ptr );

  test_assert( NULL == ptr, "Pointer is NULL after dispose()" );
}

int main ( int argc, char *argv[] ) {

  unit_test( argc, argv );

  memory();

  exit( EXIT_SUCCESS );
}
