#include <libellul/memory.h>

#include <libellul/macros.h>

#ifndef NDEBUG
#include <libellul/logging.h>
#endif

void   *allocate__( size_t size, char *file, const char *func, int line ) {
  void *ptr = malloc( size );

#ifndef NDEBUG
  if ( NULL == ptr ) {
    FATAL( "Allocation failed at %s:%s:%d\n", file, func, line );
  }
#else
  UNUSED( file );
  UNUSED( func );
  UNUSED( line );
#endif
  assert( ptr );

#if !defined( NDEBUG ) && defined( MEM_TRACE )
  DEBUG( "%s:%d: Allocated %zu bytes at %p\n", func, line, size, ptr );
#endif

  return ptr;
}

void    dispose__( void **ptr, char *file, const char *func, int line ) {
  assert( ptr );

#if !defined( NDEBUG ) && defined( MEM_TRACE )
  DEBUG( "%s:%d: Freeing at %p\n", func, line, *ptr );
#else
  UNUSED( func );
  UNUSED( line );
#endif

  UNUSED( file );
  free( *ptr );
}
