#include <libellul.h>
#include <stdint.h>


static inline unsigned int_hash(int key) {
    return (unsigned)key;
}

#define T_SET_TAG      int_set
#define T_SET_ELEMENT  int
#define T_IMPL_HASHTABLE_LINEAR
#define T_MAP_HASHFUN  int_hash
#define T_MAP_EXPORT_DEFS  

#include <libellul/type/set.h>