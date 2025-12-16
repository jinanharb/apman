#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <time.h>

extern struct timespec time_point;

double  elapsed_nsec( void );
double  elapsed_usec( void );
double  elapsed_msec( void );

void    tick( struct timespec *ts );
double  tock_nsec( struct timespec *ts );
double  tock_usec( struct timespec *ts );
double  tock_msec( struct timespec *ts );

#endif /* _CLOCK_H_ */
