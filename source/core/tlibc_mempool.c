#include "tlibc/core/tlibc_mempool.h"
#include "tlibc/core/tlibc_error_code.h"
#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_list.h"


TLIBC_ERROR_CODE tlibc_mempool_init(tlibc_mempool_t* self, size_t pool_size, size_t unit_size)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	size_t i;
	if(pool_size < TLIBC_OFFSET_OF(tlibc_mempool_t, data))
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->unit_size = unit_size;	
	self->unit_num = TLIBC_MEMPOOL_UNIT_NUM(pool_size, unit_size);
	tlibc_list_init(&self->used_list);
	tlibc_list_init(&self->unused_list);

	for(i = 0; i < self->unit_num; ++i)
	{
		tlibc_mempool_block_t *b = (tlibc_mempool_block_t*)tlibc_mempool_id2block(self, i);
		tlibc_list_add_tail(&b->unused_list, &self->unused_list)
		tlibc_list_init(&b->used_list);
	}
	self->used_list_num = 0;

done:
	return ret;
}

void* tlibc_mempool_alloc(tlibc_mempool_t* self)
{
	tlibc_mempool_block_t *b;

	if(tlibc_list_empty(&self->unused_list))
	{
		goto ERROR_RET;
	}
	b = TLIBC_CONTAINER_OF(self->unused_list.next, tlibc_mempool_block_t, unused_list);

	tlibc_list_del(&b->unused_list);

	tlibc_list_add_tail(&b->used_list, &self->used_list);
	++self->used_list_num;
	
	return b->data;
ERROR_RET:
	return NULL;
}

void tlibc_mempool_free(tlibc_mempool_t* self, void* ptr)
{
	tlibc_mempool_block_t *b = TLIBC_CONTAINER_OF(ptr, tlibc_mempool_block_t, data);

	tlibc_list_del(&b->used_list);
	tlibc_list_add(&b->unused_list, &self->unused_list);
	--self->used_list_num;
}
