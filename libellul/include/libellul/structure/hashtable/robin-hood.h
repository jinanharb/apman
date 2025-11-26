#ifndef ROBIN_HOOD_HASHTABLE_H
#define ROBIN_HOOD_HASHTABLE_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define INITIAL_CAPACITY 16
#define EMPTY_KEY 0

#if defined(T_MAP_EXPORT_DEFS) || !defined(T_MAP_EXPORT_CODE)

typedef struct {
 T_MAP_KEY key;
#ifndef T_SET_ELEMENT
 T_MAP_VALUE value;
#endif
 size_t dib; /* distance to initial bucket */
} RH_Slot;

typedef struct T {
 size_t length;
 size_t capacity;
 RH_Slot *table;
} *T;

#endif

#include <libellul/type/map/interface.h>

#if !defined(T_MAP_EXPORT_DEFS)

/* ----------------------------- Helpers ----------------------------- */

static inline size_t hfun(T_MAP_KEY key, size_t cap) {
 return (size_t)(T_MAP_HASHFUN(key) % cap);
}

static inline int is_empty(RH_Slot *s) {
 return (s->key == EMPTY_KEY);
}

static inline void slot_clear(RH_Slot *s) {
 s->key = EMPTY_KEY;
#ifndef T_SET_ELEMENT
 s->value = NULL;
#endif
 s->dib = 0;
}

/* ----------------------------- Rehash ----------------------------- */

static void rh_rehash(T map, size_t newcap)
{
 RH_Slot *old = map->table;
 size_t oldcap = map->capacity;

 map->table = calloc(newcap, sizeof(RH_Slot));
 map->capacity = newcap;
 map->length = 0;

 for (size_t i = 0; i < newcap; i++) slot_clear(&map->table[i]);

 for (size_t i = 0; i < oldcap; i++) {

 if (!is_empty(&old[i])) {

 T_MAP_KEY key = old[i].key;
#ifndef T_SET_ELEMENT
 T_MAP_VALUE val = old[i].value;
#endif
 size_t h = hfun(key, newcap);

 RH_Slot cur = old[i];
 cur.dib = 0;

 for (;;) {

 RH_Slot *s = &map->table[h];

 if (is_empty(s)) {
 *s = cur;
 map->length++;
 break;
 }

 if (cur.dib > s->dib) {
 RH_Slot tmp = *s;
 *s = cur;
 cur = tmp;
 }

 h = (h + 1) % newcap;
 cur.dib++;
 }
 }
 }

 free(old);
}

/* ----------------------------- new ----------------------------- */

T_MAP_INTERFACE
T MAP_METHOD(new)(void)
{
 T map = calloc(1, sizeof(*map));
 map->capacity = INITIAL_CAPACITY;
 map->length = 0;
 map->table = calloc(map->capacity, sizeof(RH_Slot));

 for (size_t i = 0; i < map->capacity; i++)
 slot_clear(&map->table[i]);

 return map;
}

/* ----------------------------- delete ----------------------------- */

T_MAP_INTERFACE
void MAP_METHOD(delete)(T *map)
{
 if (!map || !*map) return;

 free((*map)->table);
 free(*map);
 *map = NULL;
}

/* ----------------------------- length ----------------------------- */

T_MAP_INTERFACE
size_t MAP_METHOD(length)(T map) {
 return map ? map->length : 0;
}

/* ----------------------------- contains ----------------------------- */

T_MAP_INTERFACE
int MAP_METHOD(contains)(T map, T_MAP_KEY key)
{
 if (!map) return 0;

 size_t cap = map->capacity;
 size_t h = hfun(key, cap);
 size_t probe = 0;

 for (;;) {

 RH_Slot *s = &map->table[h];

 if (is_empty(s)) return 0;
 if (s->dib < probe) return 0;
 if (s->key == key) return 1;

 h = (h + 1) % cap;
 probe++;
 }
}

/* ----------------------------- get ----------------------------- */

T_MAP_INTERFACE
int MAP_METHOD(get)(T map, T_MAP_KEY key, T_MAP_VALUE *value)
{
 if (!map) return 0;

 size_t cap = map->capacity;
 size_t h = hfun(key, cap);
 size_t probe = 0;

 for (;;) {

 RH_Slot *s = &map->table[h];

 if (is_empty(s)) return 0;
 if (s->dib < probe) return 0;

 if (s->key == key) {
#ifndef T_SET_ELEMENT
 if (value) *value = s->value;
#endif
 return 1;
 }

 h = (h + 1) % cap;
 probe++;
 }
}

/* ----------------------------- put ----------------------------- */

T_MAP_INTERFACE
int MAP_METHOD(put)(T *map, T_MAP_KEY key, T_MAP_VALUE value)
{
 if (!*map) *map = MAP_METHOD(new)();
 T m = *map;

#ifndef T_SET_ELEMENT
 (void)value;
#endif

 if (m->length * 2 >= m->capacity)
 rh_rehash(m, m->capacity * 2);

 size_t cap = m->capacity;
 size_t h = hfun(key, cap);

 RH_Slot cur;
 cur.key = key;
#ifndef T_SET_ELEMENT
 cur.value = value;
#endif
 cur.dib = 0;

 for (;;) {

 RH_Slot *s = &m->table[h];

 if (is_empty(s)) {
 *s = cur;
 m->length++;
 return 1;
 }

 if (s->key == key) {
#ifndef T_SET_ELEMENT
 s->value = value;
#endif
 return 1;
 }

 if (cur.dib > s->dib) {
 RH_Slot tmp = *s;
 *s = cur;
 cur = tmp;
 }

 h = (h + 1) % cap;
 cur.dib++;
 }
}

/* ----------------------------- remove ----------------------------- */

T_MAP_INTERFACE
int MAP_METHOD(remove)(T *map, T_MAP_KEY key)
{
 if (!map || !*map) return 0;
 T m = *map;

 size_t cap = m->capacity;
 size_t h = hfun(key, cap);
 size_t probe = 0;

 for (;;) {

 RH_Slot *s = &m->table[h];

 if (is_empty(s)) return 0;
 if (s->dib < probe) return 0;

 if (s->key == key) {

 slot_clear(s);

 size_t next = (h + 1) % cap;

 while (!is_empty(&m->table[next]) &&
 m->table[next].dib > 0) {

 m->table[h] = m->table[next];
 m->table[h].dib--;

 slot_clear(&m->table[next]);

 h = next;
 next = (next + 1) % cap;
 }

 m->length--;
 return 1;
 }

 h = (h + 1) % cap;
 probe++;
 }
}

#endif
#endif






