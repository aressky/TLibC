#ifndef _H_TLIBC_MEMPOOL_H
#define _H_TLIBC_MEMPOOL_H

#include "tlibc/platform/tlibc_platform.h"

typedef struct _tlibc_mempool_block_t
{
	int used;
	tuint32 code;
	int next;
	char data[1];
}tlibc_mempool_block_t;

typedef struct _tlibc_mempool_t
{
	tuint32 code;
	size_t unit_size;
	size_t unit_num;
	tuint32 free_head;
	char data[1];
}tlibc_mempool_t;

#define TLIBC_MEMPOOL_INVALID_INDEX TLIBC_UINT64_MAX

//如果错误返回-1, 否则返回可以容纳的元素个数
TLIBC_API int tlibc_mempool_init(tlibc_mempool_t* self, size_t size, size_t unit_size);

TLIBC_API tuint64 tlibc_mempool_alloc(tlibc_mempool_t* self);

TLIBC_API void tlibc_mempool_free(tlibc_mempool_t* self, tuint64 mid);

TLIBC_API void* tlibc_mempool_get(tlibc_mempool_t* self, tuint64 mid);

#endif//_H_TLIBC_MEMPOOL_H

