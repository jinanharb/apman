#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdlib.h>
#include <assert.h>

void   *allocate__( size_t size, char *file, const char *func, int line );
#define allocate( size ) allocate__( (size), __FILE__, __func__, __LINE__ )

void    dispose__( void **ptr, char *file, const char *func, int line );
#define dispose( ptr_address ) dispose__( (void**)(ptr_address), __FILE__, __func__, __LINE__ ) , *(ptr_address) = NULL

#endif
