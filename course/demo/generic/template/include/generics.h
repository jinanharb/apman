#ifndef T
#error "Undefined data structure!"
#endif

#ifndef datum_t
#error "Undefined datum type!"
#endif

#include "macros.h"

#define TYPENAME( T, datum_t ) CAT( T, CAT( _, datum_t ) )
#define TYPE( T, datum_t )     CAT( TYPENAME( T, datum_t ), _t )
#define METHOD( name )         CAT( TYPENAME( T, datum_t ), CAT( _, name ) )
