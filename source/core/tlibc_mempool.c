#include "tlibc/core/tlibc_mempool.h"
#include "tlibc/core/tlibc_error_code.h"
#include "tlibc/platform/tlibc_platform.h"

#include <stdlib.h>
#include <time.h>


#define GET_BLOCK(p, idx) ((tlibc_mempool_block_t*)((char*)p->data + \
	(p->unit_size + TLIBC_OFFSET_OF(tlibc_mempool_block_t, data)) * (idx)))


int tlibc_mempool_init(tlibc_mempool_t* self, size_t pool_size, size_t unit_size)
{
	tlibc_mempool_block_t *b = NULL;
	size_t i;
	if(pool_size < TLIBC_OFFSET_OF(tlibc_mempool_t, data))
	{
		goto ERROR_RET;
	}

	self->unit_size = unit_size;
	self->free_head = 0;
	self->unit_num = TLIBC_MEMPOOL_UNIT_NUM(pool_size, unit_size);

	if(self->unit_num > TLIBC_INT32_MAX)
	{
		goto ERROR_RET;
	}
	self->code |= (tuint32)(time(0) ^ rand());

	for(i = 0; i < self->unit_num; ++i)
	{
		b = GET_BLOCK(self, i);
		b->used = FALSE;
		b->next = i + 1;
		b->code = self->code;
	}
	return self->unit_num;
ERROR_RET:
	return -1;
}

#define MID_BUILD(code, index) (((tuint64)code << 32) | (index & TLIBC_INT32_MAX))
#define MID_GET_CODE(mid) (mid >> 32)
#define MID_GET_INDEX(mid) (mid & TLIBC_INT32_MAX)

tuint64 tlibc_mempool_alloc(tlibc_mempool_t* self)
{
	tlibc_mempool_block_t *b;
	tuint64 mid;

	if(self->free_head == self->unit_num)
	{
		goto ERROR_RET;
	}
	
	do 
	{
		++self->code;
		mid = MID_BUILD(self->code, self->free_head);
	}while(mid == TLIBC_MEMPOOL_INVALID_INDEX);

	b = GET_BLOCK(self, self->free_head);
	if(b->used)
	{
		goto ERROR_RET;
	}
	b->code = MID_GET_CODE(mid);
	b->used = TRUE;
	self->free_head = b->next;	
	
	return mid;
ERROR_RET:
	return TLIBC_MEMPOOL_INVALID_INDEX;
}

void tlibc_mempool_free(tlibc_mempool_t* self, tuint64 mid)
{
	tlibc_mempool_block_t *b;
	tuint32 code = MID_GET_CODE(mid);
	tuint32	index = MID_GET_INDEX(mid);

	if(index >= self->unit_num)
	{
		goto done;
	}

	b = GET_BLOCK(self, index);
	if(!b->used)
	{
		goto done;
	}
	if(b->code != code)
	{
		goto done;
	}
	b->next = self->free_head;
	b->used = FALSE;
	self->free_head = index;
done:
	return;
}

void* tlibc_mempool_get(tlibc_mempool_t* self, tuint64 mid)
{
	tlibc_mempool_block_t *b = NULL;
	tuint32 code = MID_GET_CODE(mid);
	tuint32	index = MID_GET_INDEX(mid);

	if(index >= self->unit_num)
	{
		return NULL;
	}

	b = GET_BLOCK(self, index);
	if(!b->used)
	{
		return NULL;
	}
	if(b->code != code)
	{
		return NULL;
	}

	return b->data;
}
