#ifndef _GENERICS_H_
#define _GENERICS_H_

#define GEN_CONCAT( foo, bar ) foo ## bar
#define GEN_CAT( foo, bar )    GEN_CONCAT( foo, bar )

#define GEN_SYM( foo, bar )    GEN_CAT( foo, GEN_CAT( _, bar ) )

#endif
