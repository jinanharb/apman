#include <libellul/type/array.h>

#include <libellul/memory.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

/*

   alloc_ptr
      v
      +---------+---------------+
      |  header |  data slots   |
      +---------+---------------+
                ^
            user_ptr

*/

typedef struct {
  size_t length;
  size_t allocd;
  size_t size;
  int    strict;
} array_header_t;

#define ARRAY_HEADER( array ) (((array_header_t*)(array))-1)

#if !defined( ARRAY_ALLOC_GEOM )
#define ARRAY_ALLOC_GEOM         1.3
#endif

#if !defined( ARRAY_ALLOC_MIN )
#define ARRAY_ALLOC_MIN          8
#endif

/*
 * Dynamic arrays by genericity with hidden metadata.
 *
 * An array contains 'length' elements of 'size' bytes,
 * with enough room for 'allocd' >= 'length' of them.
 * This is called amortized allocation.
 *
 * Amortized allocation will typically pre-allocate
 * ARRAY_ALLOC_MIN slots and the growth of 'allocd'
 * is a geometric progession of ratio ARRAY_ALLOC_GEOM.
 *
 * If 'strict' is non-zero, amortized allocation is
 * disabled and 'allocd' == 'length' at all times.
 *
 */

void*  array_new__( size_t nmemb, size_t size, int strict )
{
  array_header_t *array = malloc(sizeof(array_header_t) + 1.3 * (nmemb*size));
  array ->length = nmemb;
  array -> allocd = 1.3 * nmemb;
  array -> size = size;
  array -> strict = strict;
  return 1 + array;
}

/*
void    array_delete__( void **array_ptr )
{
  if (array_ptr && *array_ptr)
  {
    // step back to header
    array_header_t *hdr = ARRAY_HEADER(array_ptr);
    free(hdr);
    *array_ptr = NULL;
  }
  return;
}
*/

void array_delete__( void **array_ptr )
{
  if (array_ptr && *array_ptr)
  {
    /* step back to header */
    array_header_t *hdr = ARRAY_HEADER(*array_ptr);
    free(hdr);
    *array_ptr = NULL;
  }
  return;
}


size_t  array_length( void *array )
{
  return array ? ARRAY_HEADER( array )->length : 0;
}


size_t  array_resize__( void **array_ptr, size_t nmemb )
{
  array_header_t *header;

  assert( array_ptr && *array_ptr && "Array must be allocated first!" );

  header = ARRAY_HEADER( *array_ptr );

  /* TODO: Actually do the resize, update header and *array_ptr */
 
  //space allocated is already enough to add these new members
  if (header -> allocd >= nmemb)
  {
    header -> length = nmemb;
    return header->length;
  }

  //si c'est strict on ne peut pas changer
  if (header->strict)
  {
    printf("Array is strict!");
    return header -> length;
  }

  size_t total_size = sizeof(array_header_t) + 1.3 * (nmemb * header->size);

  array_header_t *new_header = realloc(header, total_size);

  assert(new_header != NULL);

  new_header->length = nmemb;

  new_header->allocd = nmemb * 1.3;

  *array_ptr = (void *)(new_header + 1);

  return new_header->length;
}

#undef ARRAY_HEADER
#undef ARRAY_ALLOC_GEOM
#undef ARRAY_ALLOC_MIN