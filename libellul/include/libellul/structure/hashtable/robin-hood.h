#ifndef T_IMPL_HASHTABLE_ROBIN_HOOD
#error "Define T_IMPL_HASHTABLE_ROBIN_HOOD before including robin-hood.h"
#endif



T_MAP_INTERFACE T MAP_METHOD(new)( void ) {
    T map = mem_alloc( sizeof(*map) );
    assert( map && "Allocation failure" );

    map->length = 0;

    map->bucket =
        mem_calloc( 16, sizeof(T_MAP_KEY) );
    assert( map->bucket );

#if !defined(T_SET_ELEMENT)
    map->value =
        mem_calloc( 16, sizeof(T_MAP_VALUE) );
    assert( map->value );
#endif

    return map;
}

T_MAP_INTERFACE size_t MAP_METHOD(length)( T map ) {
    return map->length;
}

T_MAP_INTERFACE void MAP_METHOD(delete)( T *map ) {
    assert( map && *map );

    mem_free( (*map)->bucket );

#if !defined(T_SET_ELEMENT)
    mem_free( (*map)->value );
#endif

    mem_free( *map );
    *map = NULL;
}

T_MAP_INTERFACE int MAP_METHOD(contains)( T map, T_MAP_KEY key ) {
    size_t h = (size_t)key & 15; 

    for( size_t i = 0; i < 16; i++ ) {
        size_t pos = (h + i) & 15;

        if( map->bucket[pos] == 0 )
            return 0;

        if( map->bucket[pos] == key )
            return 1;
    }

    return 0;
}

T_MAP_INTERFACE int MAP_METHOD(remove)( T *map, T_MAP_KEY key ) {
    assert( map && *map );

    size_t h = (size_t)key & 15;

    for( size_t i = 0; i < 16; i++ ) {
        size_t pos = (h + i) & 15;

        if( (*map)->bucket[pos] == key ) {
            (*map)->bucket[pos] = 0;
#if !defined(T_SET_ELEMENT)
            (*map)->value[pos] = 0;
#endif
            (*map)->length--;
            return 1;
        }

        if( (*map)->bucket[pos] == 0 )
            return 0;
    }

    return 0;
}

#if defined(T_SET_ELEMENT)



T_MAP_INTERFACE int MAP_METHOD(insert)( T *set, T_SET_ELEMENT el ) {
    T map = *set;

    if( MAP_METHOD(contains)( map, el ) )
        return 0;

    size_t h = (size_t)el & 15;

    for( size_t i = 0; i < 16; i++ ) {
        size_t pos = (h + i) & 15;

        if( map->bucket[pos] == 0 ) {
            map->bucket[pos] = el;
            map->length++;
            return 1;
        }
    }
    return 0; 
}

#else



T_MAP_INTERFACE int MAP_METHOD(put)( T *map_ptr, T_MAP_KEY key, T_MAP_VALUE val ) {
    T map = *map_ptr;
    size_t h = (size_t)key & 15;

    
    for( size_t i = 0; i < 16; i++ ) {
        size_t pos = (h + i) & 15;

        if( map->bucket[pos] == key ) {
#if !defined(T_SET_ELEMENT)
            map->value[pos] = val;
#endif
            return 0;
        }

        if( map->bucket[pos] == 0 )
            break;
    }


    for( size_t i = 0; i < 16; i++ ) {
        size_t pos = (h + i) & 15;

        if( map->bucket[pos] == 0 ) {
            map->bucket[pos] = key;

#if !defined(T_SET_ELEMENT)
            map->value[pos] = val;
#endif

            map->length++;
            return 1; 
        }
    }

    return 0; 
}

T_MAP_INTERFACE int MAP_METHOD(get)( T map, T_MAP_KEY key, T_MAP_VALUE *val ) {
    size_t h = (size_t)key & 15;

    for( size_t i = 0; i < 16; i++ ) {
        size_t pos = (h + i) & 15;

        if( map->bucket[pos] == key ) {
#if !defined(T_SET_ELEMENT)
            *val = map->value[pos];
#endif
            return 1;
        }

        if( map->bucket[pos] == 0 )
            return 0;
    }

    return 0;
}

#endif 


