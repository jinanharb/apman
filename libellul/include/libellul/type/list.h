#ifndef _LIBELLUL_LIST_H_
#define _LIBELLUL_LIST_H_

#include <libellul/type/deques.h>



typedef deque_t list_t;


#define list_reset(list_ptr) \
    deque_reset((list_ptr))

#define list_init(list_ptr, type, link_member) \
    deque_init((list_ptr), type, link_member)


#define list_length(list_ptr) \
    ((list_ptr)->length)

#define list_is_empty(list_ptr) \
    ((list_ptr)->length == 0)


#define list_push_front(list_ptr, obj) \
    deque_pushfront((list_ptr), (obj))

#define list_push_back(list_ptr, obj) \
    deque_pushback((list_ptr), (obj))

#define list_pop_front(list_ptr) \
    deque_deletefront((list_ptr))

#define list_pop_back(list_ptr) \
    deque_deleteback((list_ptr))


#define list_first(list_ptr) \
    deque_first((list_ptr))

#define list_last(list_ptr) \
    deque_last((list_ptr))


#define list_foreach(list_ptr, element) \
    deque_foreach((list_ptr), element)

#endif /* _LIBELLUL_LIST_H_ */