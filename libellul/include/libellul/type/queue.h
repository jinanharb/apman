#ifndef _LIBELLUL_QUEUE_H_
#define _LIBELLUL_QUEUE_H_

#include <stddef.h>
#include <assert.h>

#include <libellul/generics.h>
#include <libellul/memory.h>
#include <libellul/type/deques.h>


#if !defined( T_QUEUE_TAG )
#error "Undefined T_QUEUE_TAG"
#endif

#if !defined( T_QUEUE_ELEMENT )
#error "Undefined T_QUEUE_ELEMENT"
#endif


#define QUEUE_TYPE      GEN_SYM( T_QUEUE_TAG, t )
#define QUEUE_METHOD(n) GEN_SYM( T_QUEUE_TAG, n )



typedef struct GEN_SYM( T_QUEUE_TAG, node ) {
    T_QUEUE_ELEMENT value;
    link_t          link;
} GEN_SYM( T_QUEUE_TAG, node );



typedef struct GEN_SYM( T_QUEUE_TAG, s ) {
    deque_t dq;
} *QUEUE_TYPE;


static inline QUEUE_TYPE
QUEUE_METHOD( new )( void )
{
    QUEUE_TYPE q = allocate( sizeof( *q ) );

    deque_init( &q->dq, GEN_SYM( T_QUEUE_TAG, node ), link );

    return q;
}


static inline void
QUEUE_METHOD( delete )( QUEUE_TYPE *q_ptr )
{
    if ( !q_ptr || !*q_ptr )
        return;

    QUEUE_TYPE q = *q_ptr;

    while ( q->dq.length > 0 ) {
        GEN_SYM( T_QUEUE_TAG, node ) *node =
            (GEN_SYM( T_QUEUE_TAG, node ) *)deque_first( &q->dq );

        deque_deletefront( &q->dq );
        dispose( (void **)&node );
    }

    dispose( (void **)q_ptr );
}



static inline size_t
QUEUE_METHOD( length )( QUEUE_TYPE q )
{
    return q ? (size_t)q->dq.length : 0;
}

static inline int
QUEUE_METHOD( is_empty )( QUEUE_TYPE q )
{
    return !q || (q->dq.length == 0);
}


static inline void
QUEUE_METHOD( push )( QUEUE_TYPE q, T_QUEUE_ELEMENT value )
{
    assert( q );

    GEN_SYM( T_QUEUE_TAG, node ) *node =
        allocate( sizeof( *node ) );

    node->value = value;
    link_init( &node->link );

    deque_pushback( &q->dq, node );
}



static inline int
QUEUE_METHOD( pop )( QUEUE_TYPE q, T_QUEUE_ELEMENT *out )
{
    if ( !q || q->dq.length == 0 )
        return 0;

    GEN_SYM( T_QUEUE_TAG, node ) *node =
        (GEN_SYM( T_QUEUE_TAG, node ) *)deque_first( &q->dq );

    if ( out )
        *out = node->value;

    deque_deletefront( &q->dq );
    dispose( (void **)&node );

    return 1;
}


static inline int
QUEUE_METHOD( front )( QUEUE_TYPE q, T_QUEUE_ELEMENT *out )
{
    if ( !q || q->dq.length == 0 )
        return 0;

    GEN_SYM( T_QUEUE_TAG, node ) *node =
        (GEN_SYM( T_QUEUE_TAG, node ) *)deque_first( &q->dq );

    if ( out )
        *out = node->value;

    return 1;
}

static inline int
QUEUE_METHOD( back )( QUEUE_TYPE q, T_QUEUE_ELEMENT *out )
{
    if ( !q || q->dq.length == 0 )
        return 0;

    GEN_SYM( T_QUEUE_TAG, node ) *node =
        (GEN_SYM( T_QUEUE_TAG, node ) *)deque_last( &q->dq );

    if ( out )
        *out = node->value;

    return 1;
}


#undef QUEUE_TYPE
#undef QUEUE_METHOD
#undef T_QUEUE_TAG
#undef T_QUEUE_ELEMENT

#endif /* _LIBELLUL_QUEUE_H_ */
