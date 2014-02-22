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
		tlibc_mempool_block_t *b = (tlibc_mempool_block_t*)(self->data + TLIBC_MEMPOOL_BLOCK_SIZE(self) * i);
		tlibc_list_init(&b->used_list);
		tlibc_list_init(&b->unused_list);

		tlibc_list_add_tail(&b->unused_list, &self->unused_list)		
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

	//这里做一个分配的标记
	tlibc_list_init(&b->unused_list);

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
	//这里做一个释放的标记
	tlibc_list_init(&b->used_list);

	tlibc_list_add(&b->unused_list, &self->unused_list);
	--self->used_list_num;
}

tlibc_mempool_block_t* tlibc_mempool_id2block(tlibc_mempool_t *self, size_t id)
{
	tlibc_mempool_block_t *ret = NULL;	
	if(id >= self->unit_num)
	{
		goto REUTRN_NULL;
	}
	ret = (tlibc_mempool_block_t*)(self->data + TLIBC_MEMPOOL_BLOCK_SIZE(self) * id);

	//如果已经被释放掉了， 那么就返回空
	if(tlibc_list_empty(&ret->used_list))
	{
		goto REUTRN_NULL;
	}

	//如果还没被分配， 那么返回空
	if(!tlibc_list_empty(&ret->unused_list))
	{
		goto REUTRN_NULL;
	}

	return ret;
REUTRN_NULL:
	return NULL;
}


size_t tlibc_mempool_block2id(tlibc_mempool_t *self, void *ptr)
{
	size_t block_size = self->unit_size + TLIBC_OFFSET_OF(tlibc_mempool_block_t, data);
	char *cptr = (char*)ptr;
	size_t offset = cptr - self->data;
	return offset / block_size;
}

void* tlibc_mempool_id2ptr(tlibc_mempool_t *self, size_t id)
{
	tlibc_mempool_block_t *ret = tlibc_mempool_id2block(self, id);
	if(ret == NULL)
	{
		goto RETURN_NULL;
	}

	return ret->data;
RETURN_NULL:
	return NULL;
}

size_t tlibc_mempool_ptr2id(tlibc_mempool_t *self, void *ptr)
{
	return tlibc_mempool_block2id(self, TLIBC_CONTAINER_OF(ptr, tlibc_mempool_block_t, data));
}
