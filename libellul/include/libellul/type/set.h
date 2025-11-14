#if !defined( T_SET_TAG )
#error "Undefined T_SET_TAG"
#endif

#if !defined( T_SET_ELEMENT )
#error "Undefined T_SET_ELEMENT"
#endif

#define T_MAP_TAG T_SET_TAG
#define T_MAP_KEY T_SET_ELEMENT

#include <libellul/type/map.h>

#undef T_SET_ELEMENT
#undef T_SET_TAG
