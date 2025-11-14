
/* All map implementations must provide these functions: */
T_MAP_INTERFACE T      MAP_METHOD( new )( void );
T_MAP_INTERFACE size_t MAP_METHOD( length )( T map );
T_MAP_INTERFACE void   MAP_METHOD( delete )( T *map );
T_MAP_INTERFACE int    MAP_METHOD( contains )( T map, T_MAP_KEY key );
T_MAP_INTERFACE int    MAP_METHOD( remove )( T *map, T_MAP_KEY key );

#if defined( T_SET_ELEMENT )
T_MAP_INTERFACE int    MAP_METHOD( insert )( T *set, T_SET_ELEMENT element );
#define      T_HAS_COMPARATOR
T_MAP_INTERFACE int    MAP_METHOD( compare ) ( void *_set1, void *_set2 );
#else
T_MAP_INTERFACE int    MAP_METHOD( put )( T *map, T_MAP_KEY key, T_MAP_VALUE value );
T_MAP_INTERFACE int    MAP_METHOD( get )( T map, T_MAP_KEY key, T_MAP_VALUE *value );
#endif

/* Automatically constructed by type/map/epilogue.h: */
T_MAP_INTERFACE int    MAP_METHOD( is_empty )( T map );
