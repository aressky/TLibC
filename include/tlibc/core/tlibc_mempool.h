#ifndef _H_TLIBC_MEMPOOL_H
#define _H_TLIBC_MEMPOOL_H

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_list.h"
#include "tlibc_error_code.h"

typedef struct _tlibc_mempool_block_t
{
	TLIBC_LIST_HEAD used_list;
	TLIBC_LIST_HEAD unused_list;
	char data[1];
}tlibc_mempool_block_t;

typedef struct _tlibc_mempool_t
{
	size_t unit_size;
	size_t unit_num;

	TLIBC_LIST_HEAD used_list;
	size_t used_list_num;
	TLIBC_LIST_HEAD unused_list;

	char data[1];	
}tlibc_mempool_t;

#define TLIBC_MEMPOOL_SIZE(unit_size, unit_num) (TLIBC_OFFSET_OF(tlibc_mempool_t, data) + \
	(unit_size + TLIBC_OFFSET_OF(tlibc_mempool_block_t, data)) * unit_num)

#define TLIBC_MEMPOOL_UNIT_NUM(poll_size, unit_size) (poll_size - TLIBC_OFFSET_OF(tlibc_mempool_t, data))\
	/ (unit_size + TLIBC_OFFSET_OF(tlibc_mempool_block_t, data));

#define TLIBC_MEMPOOL_BLOCK_SIZE(self) (self->unit_size + TLIBC_OFFSET_OF(tlibc_mempool_block_t, data))

TLIBC_API TLIBC_ERROR_CODE tlibc_mempool_init(tlibc_mempool_t* self, size_t pool_size, size_t unit_size);

TLIBC_API void* tlibc_mempool_alloc(tlibc_mempool_t* self);

TLIBC_API void tlibc_mempool_free(tlibc_mempool_t* self, void* ptr);

//id2block有很基本的错误检查， id是可能重复使用的， 所以需要外部包装guid来确保内存不会被误用。
TLIBC_API tlibc_mempool_block_t* tlibc_mempool_id2block(tlibc_mempool_t *self, size_t id);

//block2id并没有做错误的检查， 因为使用指针直接访问的情况下都是很需要效率的。
TLIBC_API size_t tlibc_mempool_block2id(tlibc_mempool_t *self, void *ptr);

TLIBC_API void* tlibc_mempool_id2ptr(tlibc_mempool_t *self, size_t id);

TLIBC_API size_t tlibc_mempool_ptr2id(tlibc_mempool_t *self, void *ptr);

#endif//_H_TLIBC_MEMPOOL_H
