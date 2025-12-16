#include <stddef.h>
#include <assert.h>

#include <libellul/generics.h>
#include <libellul/memory.h>
#include <libellul/type/array.h>
#include <libellul/type/deques.h>

/*
 * Generic stack type.
 *
 * Uso típico:
 *
 *   // implementação via array dinâmico
 *   #define T_STACK_TAG     int_stack_array
 *   #define T_STACK_ELEMENT int
 *   #define T_IMPL_STACK_ARRAY
 *   #include <libellul/type/stack.h>
 *
 *   // implementação via deque
 *   #define T_STACK_TAG     int_stack_deque
 *   #define T_STACK_ELEMENT int
 *   #define T_IMPL_STACK_DEQUE
 *   #include <libellul/type/stack.h>
 *
 * Gera:
 *   typedef ... T_STACK_TAG_t;
 *   T_STACK_TAG_t T_STACK_TAG_new(void);
 *   void          T_STACK_TAG_delete(T_STACK_TAG_t *s);
 *   size_t        T_STACK_TAG_length(T_STACK_TAG_t s);
 *   int           T_STACK_TAG_is_empty(T_STACK_TAG_t s);
 *   void          T_STACK_TAG_push(T_STACK_TAG_t *s, T_STACK_ELEMENT v);
 *   int           T_STACK_TAG_pop(T_STACK_TAG_t *s, T_STACK_ELEMENT *out);
 *   int           T_STACK_TAG_top(T_STACK_TAG_t s, T_STACK_ELEMENT *out);
 */

#if !defined( T_STACK_TAG )
#error "Undefined T_STACK_TAG"
#endif

#if !defined( T_STACK_ELEMENT )
#error "Undefined T_STACK_ELEMENT"
#endif

/* Segue o padrão de map.h:
 *
 *   STACK_TYPE      = GEN_SYM( TAG, t )    -> int_stack_array_t
 *   STACK_METHOD(n) = GEN_SYM( TAG, n )    -> int_stack_array_new, ...
 */

#define STACK_TYPE      GEN_SYM( T_STACK_TAG, t )
#define STACK_METHOD(n) GEN_SYM( T_STACK_TAG, n )

/* =========================================================
 * Implementação via ARRAY (padrão se não tiver DEQUE)
 * ========================================================= */

#if !defined( T_IMPL_STACK_DEQUE )

typedef T_STACK_ELEMENT *STACK_TYPE;

static inline STACK_TYPE
STACK_METHOD( new )( void )
{
    /* array(T) começa vazio */
    return array( T_STACK_ELEMENT );
}

static inline void
STACK_METHOD( delete )( STACK_TYPE *stack_ptr )
{
    /* array_delete já cuida de NULL e zera o ponteiro */
    array_delete( stack_ptr );
}

static inline size_t
STACK_METHOD( length )( STACK_TYPE stack )
{
    return array_length( stack );
}

static inline int
STACK_METHOD( is_empty )( STACK_TYPE stack )
{
    return array_is_empty( stack );
}

static inline void
STACK_METHOD( push )( STACK_TYPE *stack_ptr,
                      T_STACK_ELEMENT value )
{
    array_push( stack_ptr, value );
}

static inline int
STACK_METHOD( pop )( STACK_TYPE *stack_ptr,
                     T_STACK_ELEMENT *out )
{
    if ( !stack_ptr || !*stack_ptr || array_is_empty( *stack_ptr ) )
        return 0;

    array_pop( stack_ptr, out );
    return 1;
}

static inline int
STACK_METHOD( top )( STACK_TYPE stack,
                     T_STACK_ELEMENT *out )
{
    if ( !stack || array_is_empty( stack ) )
        return 0;

    if ( out ) {
        size_t n = array_length( stack );
        *out = stack[n - 1];
    }
    return 1;
}

/* =========================================================
 * Implementação via DEQUE (LIFO usando o "back")
 * ========================================================= */

#else /* T_IMPL_STACK_DEQUE definido */

typedef struct GEN_SYM( T_STACK_TAG, node ) {
    T_STACK_ELEMENT value;
    link_t          link;
} GEN_SYM( T_STACK_TAG, node );

typedef struct GEN_SYM( T_STACK_TAG, s ) {
    deque_t dq;
} *STACK_TYPE;

static inline STACK_TYPE
STACK_METHOD( new )( void )
{
    STACK_TYPE s = allocate( sizeof( *s ) );

    /* cada nó tem campo 'link' usado pelo deque */
    deque_init( &s->dq, GEN_SYM( T_STACK_TAG, node ), link );

    return s;
}

static inline void
STACK_METHOD( delete )( STACK_TYPE *stack_ptr )
{
    if ( !stack_ptr || !*stack_ptr )
        return;

    STACK_TYPE s = *stack_ptr;

    /* esvazia pilha liberando cada nó */
    while ( s->dq.length > 0 ) {
        GEN_SYM( T_STACK_TAG, node ) *node =
            (GEN_SYM( T_STACK_TAG, node ) *)deque_last( &s->dq );

        deque_deleteback( &s->dq );
        dispose( (void **)&node );
    }

    dispose( (void **)stack_ptr ); /* também zera o ponteiro */
}

static inline size_t
STACK_METHOD( length )( STACK_TYPE stack )
{
    return stack ? (size_t)stack->dq.length : 0;
}

static inline int
STACK_METHOD( is_empty )( STACK_TYPE stack )
{
    return !stack || (stack->dq.length == 0);
}

static inline void
STACK_METHOD( push )( STACK_TYPE *stack_ptr,
                      T_STACK_ELEMENT value )
{
    assert( stack_ptr && *stack_ptr );

    STACK_TYPE s = *stack_ptr;

    GEN_SYM( T_STACK_TAG, node ) *node =
        allocate( sizeof( *node ) );

    node->value = value;
    link_init( &node->link );

    /* topo da pilha = back do deque */
    deque_pushback( &s->dq, node );
}

static inline int
STACK_METHOD( pop )( STACK_TYPE *stack_ptr,
                     T_STACK_ELEMENT *out )
{
    if ( !stack_ptr || !*stack_ptr )
        return 0;

    STACK_TYPE s = *stack_ptr;

    if ( s->dq.length == 0 )
        return 0;

    GEN_SYM( T_STACK_TAG, node ) *node =
        (GEN_SYM( T_STACK_TAG, node ) *)deque_last( &s->dq );

    if ( out )
        *out = node->value;

    deque_deleteback( &s->dq );
    dispose( (void **)&node );

    return 1;
}

static inline int
STACK_METHOD( top )( STACK_TYPE stack,
                     T_STACK_ELEMENT *out )
{
    if ( !stack || stack->dq.length == 0 )
        return 0;

    GEN_SYM( T_STACK_TAG, node ) *node =
        (GEN_SYM( T_STACK_TAG, node ) *)deque_last( &stack->dq );

    if ( out )
        *out = node->value;

    return 1;
}

#endif /* escolha de implementação */

/* Limpa macros locais para poder re-instanciar no mesmo .c */

#undef STACK_TYPE
#undef STACK_METHOD
#undef T_STACK_TAG
#undef T_STACK_ELEMENT
#undef T_IMPL_STACK_ARRAY
#undef T_IMPL_STACK_DEQUE
