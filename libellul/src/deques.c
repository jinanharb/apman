#include <libellul/type/deques.h>

#include <libellul/memory.h>

#include <string.h>
#include <stdio.h>




void link_init( link_t *link ) 
{
  assert( link && "Link is NULL!" );

  link->next = link;
  link->prev = link;
}

void deque_reset(deque_t *deque) {
    assert(deque);
    deque->head.next = &deque->head;
    deque->head.prev = &deque->head;
    deque->length = 0;
    deque->offset = 0;
}


void deque_addfront(deque_t *deque, link_t *link) {
    assert(deque && "deque is NULL!");
    assert(link && "Link is NULL!");
    
    link->next = deque->head.next;
    link->prev = &deque->head;
    deque->head.next->prev = link;
    deque->head.next = link;  
    
    deque->length++;
}

void deque_addback(deque_t *deque, link_t *link) {
    assert(deque && "deque is NULL!");
    assert(link && "Link is NULL!");
    
    link->next = &deque->head;         
    link->prev = deque->head.prev;    
    deque->head.prev->next = link;     
    deque->head.prev = link;           
    
    deque->length++;
}


void delete_front(deque_t *deque){
    assert (deque != NULL && deque->head.next != NULL);
       

    link_t* first = deque->head.next;

    if (first->next != NULL) {
        first->next->prev = &deque->head;
    }

    deque->head.next = first->next;

    deque->length--;


    return;
}

void delete_back(deque_t *deque){
    assert (deque != NULL && deque->head.next != NULL) ;

    link_t* last = deque->head.prev;

    if (last->prev != NULL) {
        last->prev->next = &deque->head;
    }

    deque->head.prev = last->prev;

    deque->length--;


    return;
}

void* deque_first(deque_t *deque){
    if(deque->length == 0){
        return NULL;
    }
    return (char*)deque->head.next - (deque)->offset;
}

void* deque_last(deque_t *deque){
    if(deque->length == 0){
        return NULL;
    }
    return (char*)deque->head.prev - (deque)->offset;
}                                                                                                                                                       