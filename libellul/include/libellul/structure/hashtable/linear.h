/*
 * Linear addressing hashtable.
 *
 * Variants:
 * T_IMPL_HASHTABLE_LINEAR_NO_TOMBSTONES: Use backward-shifting deletion.
 *
 */
#define TABLE_SIZE 128
#define EMPTY_KEY 0
#define INITIAL_CAPACITY 16
#define TOMBSTONE_KEY ((T_MAP_KEY) -1)



#if defined( T_MAP_EXPORT_DEFS ) || !defined( T_MAP_EXPORT_CODE )

typedef struct T {
 size_t length;
 T_MAP_KEY *bucket;

#if !defined ( T_SET_ELEMENT )
 T_MAP_VALUE *value;
#endif
} *T;

#endif

#include <libellul/type/map/interface.h>

#if !defined( T_MAP_EXPORT_DEFS )
/* Code templating for the actual implementation starts here */

#include <libellul/memory.h>
#include <libellul/type/array.h>


/* Of course we are entitled to functions that are always local: */
static inline int MAP_METHOD( foo_helper ) ( T map ) {
 /* Dummy example! Mostly useful to factorize code in the implementation! */
 return printf( "Called %s::%s on %sempty map at %p\n",
 __FILE__, __func__, !MAP_METHOD( is_empty ) ( map ) ? "non-" : "", map );
}


T_MAP_INTERFACE T MAP_METHOD( new )( void ) {
 T map = calloc(1, sizeof(*map));

 map->length = 0;
 map->bucket = calloc(TABLE_SIZE, sizeof(T_MAP_KEY));

#if !defined(T_SET_ELEMENT)
 map->value = calloc(TABLE_SIZE, sizeof(T_MAP_VALUE));
#endif

 return map;
}


T_MAP_INTERFACE size_t MAP_METHOD( length )( T map ) {
 return map->length;
}

T_MAP_INTERFACE void MAP_METHOD( delete )( T *map ) {
 if (!map || !*map) return;

 free((*map)->bucket);
#if !defined(T_SET_ELEMENT)
 free((*map)->value);
#endif

 free(*map);
 *map = NULL;

}

T_MAP_INTERFACE int MAP_METHOD( contains )( T map, T_MAP_KEY key ) {
 size_t idx = T_MAP_HASHFUN(key) % TABLE_SIZE;

 for (size_t i = 0; i < TABLE_SIZE; i++) {
 if (map->bucket[idx] == EMPTY_KEY)
 return 0; // key never inserted
 if (map->bucket[idx] == key)
 return 1;

 idx = (idx + 1) % TABLE_SIZE;
 }

 return 0;
}

T_MAP_INTERFACE int MAP_METHOD( remove )( T *map, T_MAP_KEY key ) {
 size_t idx = T_MAP_HASHFUN(key) % TABLE_SIZE;

 for (size_t i = 0; i < TABLE_SIZE; i++) {
 if ((*map)->bucket[idx] == EMPTY_KEY)
 return 0;
 if ((*map)->bucket[idx] == key) {
 (*map)->bucket[idx] = EMPTY_KEY;
#if !defined(T_SET_ELEMENT)
 (*map)->value[idx] = 0;
#endif
 (*map)->length--;
 return 1;
 }
 idx = (idx + 1) % TABLE_SIZE;
 }

 return 0;
}


#if defined( T_SET_ELEMENT )
T_MAP_INTERFACE int MAP_METHOD( insert )( T *map, T_SET_ELEMENT element ) {
 size_t idx = T_MAP_HASHFUN(element) % TABLE_SIZE;

 for (size_t i = 0; i < TABLE_SIZE; i++) {
 if ((*map)->bucket[idx] == EMPTY_KEY) {
 (*map)->bucket[idx] = element;
 (*map)->length++;
 return 1;
 }
 idx = (idx + 1) % TABLE_SIZE;
 }

 return 0; // table full
}

#else
T_MAP_INTERFACE int MAP_METHOD( put )( T *map, T_MAP_KEY key, T_MAP_VALUE value ) {

 size_t idx = T_MAP_HASHFUN(key) % TABLE_SIZE;

 for (size_t i = 0; i < TABLE_SIZE; i++) {

 // Insert in empty slot
 if ((*map)->bucket[idx] == EMPTY_KEY) {
 (*map)->bucket[idx] = key;
 (*map)->value[idx] = value;
 (*map)->length++;
 return 1;
 }

 // Update existing
 if ((*map)->bucket[idx] == key) {
 (*map)->value[idx] = value;
 return 1;
 }

 idx = (idx + 1) % TABLE_SIZE;
 }

 return 0; // full
}

T_MAP_INTERFACE int MAP_METHOD(get)(T map, T_MAP_KEY key, T_MAP_VALUE *value) {
 if (!map || !map->bucket
#ifndef T_SET_ELEMENT
 || !map->value
#endif
 )
 return 0;

 size_t idx = T_MAP_HASHFUN(key) % TABLE_SIZE;

 for (size_t i = 0; i < TABLE_SIZE; i++) {
 if (map->bucket[idx] == EMPTY_KEY)
 return 0; 

 if (map->bucket[idx] == key) {
#ifndef T_SET_ELEMENT
 *value = map->value[idx];
#endif
 return 1;
 }

 idx = (idx + 1) % TABLE_SIZE;
 }

 return 0; 
}


 
#endif

#endif

#undef T_IMPL_HASHTABLE_LINEAR
#undef T_IMPL_HASHTABLE_LINEAR_NO_TOMBSTONES