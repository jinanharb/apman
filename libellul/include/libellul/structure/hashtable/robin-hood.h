#define RH_INITIAL_CAPACITY 16
#define RH_EMPTY_KEY ((T_MAP_KEY)0)

#if defined( T_MAP_EXPORT_DEFS ) || !defined( T_MAP_EXPORT_CODE )

typedef struct T {
 
 size_t length;

 
 size_t capacity;

 
 T_MAP_KEY *bucket;

#if !defined( T_SET_ELEMENT )
 T_MAP_VALUE *value;
#endif

 
 size_t *dib;
} *T;

#endif /* export defs */

#ifndef deallocate
static inline void deallocate(void *ptr) { free(ptr); }
#endif


#include <libellul/type/map/interface.h>

#if !defined( T_MAP_EXPORT_DEFS )


#include <libellul/memory.h>
#include <libellul/type/array.h>


static inline size_t
rh_hash( T_MAP_KEY key, size_t cap )
{
 return (size_t)( T_MAP_HASHFUN( key ) % cap );
}


static inline int
rh_is_empty( T map, size_t idx )
{
 return ( map->bucket[ idx ] == RH_EMPTY_KEY );
}


static inline void
rh_clear_slot( T map, size_t idx )
{
 map->bucket[ idx ] = RH_EMPTY_KEY;
#if !defined( T_SET_ELEMENT )
 map->value[ idx ] = (T_MAP_VALUE)0;
#endif
 map->dib[ idx ] = 0;
}

/* -------------------------- rehash -------------------------- */
static void
rh_rehash( T map, size_t newcap )
{
 
 T_MAP_KEY *old_bucket = map->bucket;
#if !defined( T_SET_ELEMENT )
 T_MAP_VALUE *old_value = map->value;
#endif
 size_t *old_dib = map->dib;
 size_t oldcap = map->capacity;

 
 map->bucket = allocate( newcap * sizeof( T_MAP_KEY ) );
#if !defined( T_SET_ELEMENT )
 map->value = allocate( newcap * sizeof( T_MAP_VALUE ) );
#endif
 map->dib = allocate( newcap * sizeof( size_t ) );
 map->capacity = newcap;
 map->length = 0;

 
 for( size_t i = 0; i < newcap; ++i ) {
 map->bucket[ i ] = RH_EMPTY_KEY;
#if !defined( T_SET_ELEMENT )
 map->value[ i ] = (T_MAP_VALUE)0;
#endif
 map->dib[ i ] = 0;
 }

 
 for( size_t i = 0; i < oldcap; ++i ) {

 if( old_bucket[ i ] != RH_EMPTY_KEY ) {

 T_MAP_KEY k = old_bucket[ i ];
#if !defined( T_SET_ELEMENT )
 T_MAP_VALUE v = old_value[ i ];
#endif

 size_t h = rh_hash( k, newcap );
 size_t dist = 0;

 for( ;; ) {

 if( rh_is_empty( map, h ) ) {
 map->bucket[ h ] = k;
#if !defined( T_SET_ELEMENT )
 map->value[ h ] = v;
#endif
 map->dib[ h ] = dist;
 map->length++;
 break;
 }

 /* Robin Hood swap */
 if( dist > map->dib[ h ] ) {
 T_MAP_KEY tmpk = map->bucket[ h ];
#if !defined( T_SET_ELEMENT )
 T_MAP_VALUE tmpv = map->value[ h ];
#endif
 size_t tmpd = map->dib[ h ];

 map->bucket[ h ] = k;
#if !defined( T_SET_ELEMENT )
 map->value[ h ] = v;
#endif
 map->dib[ h ] = dist;

 k = tmpk;
#if !defined( T_SET_ELEMENT )
 v = tmpv;
#endif
 dist = tmpd;
 }

 h = ( h + 1 ) % newcap;
 dist = dist + 1;
 }
 }
 }

 
 deallocate( old_bucket );
#if !defined( T_SET_ELEMENT )
 deallocate( old_value );
#endif
 deallocate( old_dib );
}

/* --------------------------- new --------------------------- */

T_MAP_INTERFACE T
MAP_METHOD( new )( void )
{
 T map = allocate( sizeof *map );

 map->length = 0;
 map->capacity = RH_INITIAL_CAPACITY;

 map->bucket = allocate( map->capacity * sizeof( T_MAP_KEY ) );
#if !defined( T_SET_ELEMENT )
 map->value = allocate( map->capacity * sizeof( T_MAP_VALUE ) );
#endif
 map->dib = allocate( map->capacity * sizeof( size_t ) );

 for( size_t i = 0; i < map->capacity; ++i ) {
 map->bucket[ i ] = RH_EMPTY_KEY;
#if !defined( T_SET_ELEMENT )
 map->value[ i ] = (T_MAP_VALUE)0;
#endif
 map->dib[ i ] = 0;
 }

 return map;
}

/* -------------------------- length -------------------------- */

T_MAP_INTERFACE size_t
MAP_METHOD( length )( T map )
{
 return map ? map->length : 0;
}

/* -------------------------- delete -------------------------- */

T_MAP_INTERFACE void
MAP_METHOD( delete )( T *map )
{
 if( !map || !*map ) return;

 deallocate( (*map)->bucket );
#if !defined( T_SET_ELEMENT )
 deallocate( (*map)->value );
#endif
 deallocate( (*map)->dib );

 deallocate( *map );
 *map = NULL;
}

/* ------------------------- contains ------------------------- */

T_MAP_INTERFACE int
MAP_METHOD( contains )( T map, T_MAP_KEY key )
{
 if( !map ) return 0;

 size_t cap = map->capacity;
 size_t h = rh_hash( key, cap );
 size_t dist = 0;

 for( ;; ) {

 if( rh_is_empty( map, h ) )
 return 0;

 if( map->dib[ h ] < dist )
 return 0;

 if( map->bucket[ h ] == key )
 return 1;

 h = ( h + 1 ) % cap;
 dist = dist + 1;
 }
}

/* ---------------------------- get ---------------------------- */

#if !defined( T_SET_ELEMENT )
T_MAP_INTERFACE int
MAP_METHOD( get )( T map, T_MAP_KEY key, T_MAP_VALUE *value )
{
 if( !map ) return 0;

 size_t cap = map->capacity;
 size_t h = rh_hash( key, cap );
 size_t dist = 0;

 for( ;; ) {

 if( rh_is_empty( map, h ) )
 return 0;

 if( map->dib[ h ] < dist )
 return 0;

 if( map->bucket[ h ] == key ) {
 if( value ) {
 *value = map->value[ h ];
 }
 return 1;
 }

 h = ( h + 1 ) % cap;
 dist = dist + 1;
 }
}
#endif /* !T_SET_ELEMENT */

/* ---------------------------- put ---------------------------- */

T_MAP_INTERFACE int
MAP_METHOD( put )( T *map_ptr, T_MAP_KEY key, T_MAP_VALUE value )
{
 if( !*map_ptr )
 *map_ptr = MAP_METHOD( new )();

 T map = *map_ptr;

 /* rehash se load factor >= 0.5 */
 if( map->length * 2 >= map->capacity )
 rh_rehash( map, map->capacity * 2 );

 size_t cap = map->capacity;
 size_t h = rh_hash( key, cap );
 size_t dist = 0;

 for( ;; ) {

 if( rh_is_empty( map, h ) ) {
 map->bucket[ h ] = key;
#if !defined( T_SET_ELEMENT )
 map->value[ h ] = value;
#endif
 map->dib[ h ] = dist;
 map->length++;
 return 1; /* nova inserção */
 }

 if( map->bucket[ h ] == key ) {
#if !defined( T_SET_ELEMENT )
 map->value[ h ] = value;
#endif
 return 0; /* overwrite */
 }

 if( dist > map->dib[ h ] ) {
 /* swap Robin Hood */
 T_MAP_KEY tmpk = map->bucket[ h ];
#if !defined( T_SET_ELEMENT )
 T_MAP_VALUE tmpv = map->value[ h ];
#endif
 size_t tmpd = map->dib[ h ];

 map->bucket[ h ] = key;
#if !defined( T_SET_ELEMENT )
 map->value[ h ] = value;
#endif
 map->dib[ h ] = dist;

 key = tmpk;
#if !defined( T_SET_ELEMENT )
 value = tmpv;
#endif
 dist = tmpd;
 }

 h = ( h + 1 ) % cap;
 dist = dist + 1;
 }
}

/* --------------------------- remove --------------------------- */

T_MAP_INTERFACE int
MAP_METHOD( remove )( T *map_ptr, T_MAP_KEY key )
{
 if( !map_ptr || !*map_ptr )
 return 0;

 T map = *map_ptr;
 size_t cap = map->capacity;
 size_t h = rh_hash( key, cap );
 size_t dist = 0;

 for( ;; ) {

 if( rh_is_empty( map, h ) )
 return 0;

 if( map->dib[ h ] < dist )
 return 0;

 if( map->bucket[ h ] == key ) {

 /* limpa slot atual */
 rh_clear_slot( map, h );

 /* backward-shift do cluster seguinte */
 size_t next = ( h + 1 ) % cap;

 while( !rh_is_empty( map, next ) && map->dib[ next ] > 0 ) {

 map->bucket[ h ] = map->bucket[ next ];
#if !defined( T_SET_ELEMENT )
 map->value[ h ] = map->value[ next ];
#endif
 map->dib[ h ] = map->dib[ next ] - 1;

 rh_clear_slot( map, next );

 h = next;
 next = ( next + 1 ) % cap;
 }

 map->length--;
 return 1;
 }

 h = ( h + 1 ) % cap;
 dist = dist + 1;
 }
}

#endif /* !T_MAP_EXPORT_DEFS */

#undef RH_INITIAL_CAPACITY
#undef RH_EMPTY_KEY
#undef T_IMPL_HASHTABLE_ROBINHOOD