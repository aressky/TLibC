#ifndef _H_TLIBC_TIMER_H
#define _H_TLIBC_TIMER_H

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_list.h"
#include "tlibc/core/tlibc_error_code.h"

typedef struct _tlibc_timer_entry_t tlibc_timer_entry_t;
typedef void (*tlibc_timer_callback)(struct _tlibc_timer_entry_t*);
struct _tlibc_timer_entry_t
{
	TLIBC_LIST_HEAD entry;
	tuint64 expires;
	tlibc_timer_callback callback;
};

#define TIMER_ENTRY_BUILD(en, exp, c)\
	{\
		tlibc_list_init(&(en)->entry);\
		(en)->expires = exp;\
		(en)->callback = c;\
	}

#define TLIBC_TVN_BITS (6)
#define TLIBC_TVR_BITS (8)
#define TLIBC_TVN_SIZE (1 << TLIBC_TVN_BITS)
#define TLIBC_TVR_SIZE (1 << TLIBC_TVR_BITS)
#define TLIBC_TVN_MASK (TLIBC_TVN_SIZE - 1)
#define TLIBC_TVR_MASK (TLIBC_TVR_SIZE - 1)
#define TLIBC_MAX_TVAL ((unsigned long)((1ULL << (TLIBC_TVR_BITS + 4 * TLIBC_TVN_BITS)) - 1))

typedef struct _tlibc_timver_vec_t
{
	TLIBC_LIST_HEAD vec[TLIBC_TVN_SIZE];
}tlibc_timer_vec_t;

typedef struct _tlibc_timer_verc_root_t
{
	TLIBC_LIST_HEAD vec[TLIBC_TVR_SIZE];
}tlibc_timer_ver_root_t;

typedef struct _tlibc_timer_t
{
	tuint64 jiffies;
	tlibc_timer_ver_root_t tv1;
	tlibc_timer_vec_t tv2;
	tlibc_timer_vec_t tv3;
	tlibc_timer_vec_t tv4;
	tlibc_timer_vec_t tv5;
}tlibc_timer_t;


void tlibc_timer_init(tlibc_timer_t *base, tuint64 jiffies);


TLIBC_API void tlibc_timer_pop(tlibc_timer_entry_t *timer);


void tlibc_timer_push(tlibc_timer_t *self, tlibc_timer_entry_t *timer);


TLIBC_API TLIBC_ERROR_CODE tlibc_timer_tick(tlibc_timer_t *self, tuint64 jiffies);

#endif//_H_TLIBC_TIMER_H
