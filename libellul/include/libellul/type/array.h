#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stddef.h>

void   *array_new__( size_t nmemb, size_t size, int strict );
#define array_strict( N, T ) array_new__( (N), sizeof( T ), 1 )
#define array_new( N, T )    array_new__( (N), sizeof( T ), 0 )
#define array( T )           array_new( 0, T )

void    array_delete__( void **array_ptr );
#define array_delete( array_ptr ) array_delete__( (void**)(array_ptr) )

size_t  array_length( void *array );
#define array_is_empty( array ) ( 0 == array_length( array ) )

size_t  array_resize__( void **array_ptr, size_t nmemb );
#define array_resize( array_ptr, nmemb ) array_resize__( (void**)(array_ptr), nmemb )

#define array_push( array_ptr, element ) ( array_resize( (array_ptr), array_length( *(array_ptr) ) + 1 ) , (*(array_ptr))[ array_length( *(array_ptr) ) - 1 ] = element )

#define array_pop( array_ptr, element ) ( *(element) = *(array_ptr) ? (*(array_ptr))[ array_length( *(array_ptr) ) - 1 ] : *(element) , array_resize( (array_ptr), array_length( *(array_ptr) ) - 1 ) )

#endif
