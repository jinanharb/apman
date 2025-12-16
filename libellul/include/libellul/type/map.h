/*
 * 'map' abstract data type.
 *
 * Available type implementations:
 *
 * T_IMPL_HASHTABLE (default)
 * T_IMPL_HASHTRIE
 * T_IMPL_TREAP
 * ...
 *
 * Type options:
 *
 * T_SET_ELEMENT <type>: The 'map' actually implements a 'set' of <type>'s.
 *
 */

#if !defined( T_MAP_TAG )
#error "Undefined T_MAP_TAG"
#endif

#if !defined(  T_MAP_KEY )
#error "Undefined T_MAP_KEY"
#endif

#include <libellul/type/prologue.h>

#if !defined( T_MAP_EXPORT_DEFS ) && !defined( T_MAP_EXPORT_CODE )
#define T_MAP_INTERFACE static inline
#else
#define T_MAP_INTERFACE extern
#endif

#define T                      GEN_SYM( T_MAP_TAG, t )
#define MAP_METHOD( name )     GEN_SYM( T_MAP_TAG, name )


#if defined( T_IMPL_HASHTABLE )
#include <libellul/structure/hashtable.h>
#elif defined( T_IMPL_HASHTRIE )
#include <libellul/structure/hashtrie.h>
#elif defined( T_IMPL_BINARY_SEARCH_MAP )
#include <libellul/structure/Binary_search_map.h>
#else /* Default data structure for a map is a hashtable */
#include <libellul/structure/hashtable.h>
#endif

#if defined( T_SET_ELEMENT )
T_MAP_INTERFACE T      MAP_METHOD( union ) ( T set1, T set2 );
T_MAP_INTERFACE T      MAP_METHOD( inter ) ( T set1, T set2 );
T_MAP_INTERFACE T      MAP_METHOD( diff ) ( T set1, T set2 );
T_MAP_INTERFACE T      MAP_METHOD( symdiff ) ( T set1, T set2 );

#if !defined( T_MAP_EXPORT_DEFS )
/* A 'set' is built on top of the 'map' interface */
T_MAP_INTERFACE T      MAP_METHOD( union ) ( T set1, T set2 ) {

}

T_MAP_INTERFACE T      MAP_METHOD( inter ) ( T set1, T set2 ) {

}

T_MAP_INTERFACE T      MAP_METHOD( diff ) ( T set1, T set2 ) {

}

T_MAP_INTERFACE T      MAP_METHOD( symdiff ) ( T set1, T set2 ) {

}

T_MAP_INTERFACE int    MAP_METHOD( compare ) ( void *_set1, void *_set2 ) {
  T *set1 = _set1;
  T *set2 = _set2;

  /* TODO: Use iterator to check every key of set2 in set1 or declare early -1 */

  return MAP_METHOD( length ) ( *set1 ) == MAP_METHOD( length ) ( *set2 ) ? 0 : 1;
}

#endif /* End of 'set' implementation */
#endif /* End of 'set' specialization  */


#define T_INTERFACE   T_MAP_INTERFACE
#define METHOD        MAP_METHOD
#if defined( T_MAP_EXPORT_DEFS )
#define T_EXPORT_DEFS
#endif
#include <libellul/type/epilogue.h>

#undef T_MAP_KEY
#undef T_MAP_VALUE
#undef T_MAP_TAG
#undef T_MAP_INTERFACE
#undef T_MAP_EXPORT_DEFS
#undef T_MAP_EXPORT_CODE
#undef T
#undef MAP_METHOD
