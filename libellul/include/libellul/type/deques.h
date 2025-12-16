#ifndef _deque_H_
#define _deque_H_

#include <stddef.h> /* ptrdiff_t */
#include <assert.h>

typedef struct link_t {
  struct link_t *next;
  struct link_t *prev;
} link_t;

typedef struct {
  link_t      head;
  ptrdiff_t length;
  ptrdiff_t offset;
} deque_t;

//prototypes
void      link_init( link_t *deque );
void      deque_reset( deque_t *deque_head );
void      deque_reset( deque_t *deque );
void      deque_addfront( deque_t *deque, link_t *link );
void      deque_addback( deque_t *deque, link_t *link );
void delete_front(deque_t *deque);
void delete_back(deque_t *deque);
void* deque_first(deque_t *deque);
void* deque_last(deque_t *deque);

#define deque_init(deque_ptr, type, field) do {     \
    link_init(&(deque_ptr)->head);                   \
    (deque_ptr)->head.next = &(deque_ptr)->head;    \
    (deque_ptr)->head.prev = &(deque_ptr)->head;    \
    (deque_ptr)->offset = offsetof(type, field);    \
    (deque_ptr)->length = 0;                        \
} while (0)


#define   deque_destroy( deque_ptr, destructor ) do \
{            \
    deque_foreach( deque_ptr, element ) destructor( element );  \
    deque_reset( deque_ptr );                               \
  } while ( 0 ) 


#define deque_foreach(deque_head_ptr, element) \
for ( link_t *link = (deque_head_ptr)->head.next, *next; \
      link != &(deque_head_ptr)->head && (next = link->next, 1); \
      link = next ) \
for ( void *element = (char*)link - (deque_head_ptr)->offset; element; element = NULL )
        
#define   deque_pushfront( deque_ptr, object  )    \
  deque_addfront( deque_ptr, (link_t*)( (char*)(object) + (deque_ptr)->offset ) )

#define   deque_pushback( deque_ptr, object  )    \
  deque_addback( deque_ptr, (link_t*)( (char*)(object) + (deque_ptr)->offset ) )

#define deque_deletefront(deque_ptr) delete_front(deque_ptr)

#define deque_deleteback(deque_ptr) delete_back(deque_ptr)

#endif