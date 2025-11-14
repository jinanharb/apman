
#if defined( T_HAS_COMPARATOR )
T_INTERFACE int    METHOD( equals ) ( void *_set1, void *_set2 );
#endif

#if !defined( T_EXPORT_DEFS )

T_INTERFACE int    METHOD( is_empty )( T map ) {
  return !map || !METHOD( length ) ( map );
}

#if defined( T_HAS_COMPARATOR )
T_INTERFACE int    METHOD( equals ) ( void *_set1, void *_set2 ) {
  return !MAP_METHOD( compare ) ( _set1, _set2 );
}
#endif

#endif

#undef T_HAS_COMPARATOR
#undef T_INTERFACE
#undef T_EXPORT_DEFS
#undef METHOD
