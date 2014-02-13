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
	int unit_num;
	int free_head;
	int used_head;
	char data[1];
}tlibc_mempool_t;

//计算元素需要多大的内存池
#define TLIBC_MEMPOOL_SIZE(unit_size, unit_num) (TLIBC_OFFSET_OF(tlibc_mempool_t, data) + \
	(unit_size + TLIBC_OFFSET_OF(tlibc_mempool_block_t, data)) * unit_num)

//计算内存池可以容纳多少元素
#define TLIBC_MEMPOOL_UNIT_NUM(poll_size, unit_size) (poll_size - TLIBC_OFFSET_OF(tlibc_mempool_t, data))\
	/ (unit_size + TLIBC_OFFSET_OF(tlibc_mempool_block_t, data));

#define TLIBC_MEMPOOL_GET_BLOCK(p, idx) ((tlibc_mempool_block_t*)((char*)p->data + \
	(p->unit_size + TLIBC_OFFSET_OF(tlibc_mempool_block_t, data)) * (idx)))

#define TLIBC_MEMPOOL_INVALID_INDEX TLIBC_UINT64_MAX

//如果错误返回-1, 否则返回可以容纳的元素个数
TLIBC_API int tlibc_mempool_init(tlibc_mempool_t* self, size_t pool_size, size_t unit_size);

TLIBC_API tuint64 tlibc_mempool_alloc(tlibc_mempool_t* self);

TLIBC_API void tlibc_mempool_free(tlibc_mempool_t* self, tuint64 mid);

TLIBC_API void* tlibc_mempool_get(tlibc_mempool_t* self, tuint64 mid);

#endif//_H_TLIBC_MEMPOOL_H

