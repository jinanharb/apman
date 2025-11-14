#ifndef datum_t
#error "Undefined datum type!"
#endif

#define T list
#include "generics.h"

typedef struct TYPE( link, datum_t ) *TYPE( T, datum_t );

TYPE( T, datum_t ) METHOD( new ) ( void );
int                METHOD( is_empty ) ( TYPE( T, datum_t ) list );
datum_t            METHOD( first ) ( TYPE( T, datum_t ) list );
TYPE( T, datum_t ) METHOD( next ) ( TYPE( T, datum_t ) list );
TYPE( T, datum_t ) METHOD( prepend ) ( datum_t value, TYPE( T, datum_t ) list );
TYPE( T, datum_t ) METHOD( delete ) ( TYPE( T, datum_t ) list, void (*destructor)( datum_t ) );

#undef T
#undef datum_t
#undef TYPENAME
#undef TYPE
#undef METHOD
