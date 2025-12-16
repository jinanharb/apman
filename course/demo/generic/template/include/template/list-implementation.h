#ifndef datum_t
#error "Undefined datum type!"
#endif

#define T list
#include "generics.h"

#include <stdlib.h> /* malloc(3), NULL */
#include <assert.h>

struct TYPE( link, datum_t ) {
  struct TYPE( link, datum_t ) *next;
  datum_t                      datum;
};

#define NIL NULL
TYPE( T, datum_t ) METHOD( new ) ( void ) { return NIL; }
#undef NIL

int                METHOD( is_empty ) ( TYPE( T, datum_t ) list ) {
  return METHOD( new ) () == list;
}

datum_t            METHOD( first ) ( TYPE( T, datum_t ) list ) {
  assert( !METHOD( is_empty ) ( list ) && "List is empty!" );

  return list->datum;
}

TYPE( T, datum_t ) METHOD( next ) ( TYPE( T, datum_t ) list ) {
  assert( !METHOD( is_empty ) ( list ) && "List is empty!" );

  return list->next;
}

TYPE( T, datum_t ) METHOD( prepend ) ( datum_t value, TYPE( T, datum_t ) list ) {
  TYPE( T, datum_t ) new_head = malloc( sizeof *new_head );
  assert( new_head && "Allocation failed!" );

  new_head->datum = value;
  new_head->next  = list;

  return new_head;
}

TYPE( T, datum_t ) METHOD( delete ) ( TYPE( T, datum_t ) list, void (*destructor)( datum_t ) ) {
  while ( !METHOD( is_empty ) ( list ) ) {
    TYPE( T, datum_t ) next = METHOD( next ) ( list );
    if ( destructor ) destructor( METHOD( first ) ( list ) );
    free( list );
    list = next;
  }

  return list;
}

#undef T
#undef datum_t
#undef TYPENAME
#undef TYPE
#undef METHOD
