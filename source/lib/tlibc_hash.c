#include "lib/tlibc_hash.h"
#include <memory.h>

TLIBC_ERROR_CODE tlibc_hash_init(tlibc_hash_t *self, tlibc_hash_bucket_t *buckets, tuint32 size)
{
	tuint32 i;

	self->buckets = buckets;
	self->size = size;
	for(i = 0; i < self->size; ++i)
	{
		init_tlibc_list_head(&self->buckets[i].data_list);
	}

	init_tlibc_list_head(&self->all_data_list);
	self->all_data_list_num = 0;

	return E_TLIBC_NOERROR;
}

tuint32 tlibc_hash_key(const char* key, tuint32 key_size)
{
	tuint32 i, key_hash;
	key_hash = 0;
	for(i = 0; i < key_size; ++i)
	{
		key_hash = key_hash * 31 + key[i];
	}
	return key_hash;
}

void tlibc_hash_insert(tlibc_hash_t *self, const char* key, tuint32 key_size, tlibc_hash_head_t *val_head)
{
	tuint32 key_hash = tlibc_hash_key(key, key_size);
	tuint32 key_index = key_hash % self->size;
	tlibc_hash_bucket_t *bucket = &self->buckets[key_index];

	val_head->key = key;
	val_head->key_size = key_size;
	init_tlibc_list_head(&val_head->data_list);
	init_tlibc_list_head(&val_head->all_data_list);
	
	tlibc_list_add(&val_head->data_list, &bucket->data_list);
	tlibc_list_add(&val_head->all_data_list, &self->all_data_list);
	++self->all_data_list_num;
}

const tlibc_hash_head_t* tlibc_hash_find(const tlibc_hash_t *self, const char *key, tuint32 key_size)
{
	tuint32 key_hash = tlibc_hash_key(key, key_size);
	tuint32 key_index = key_hash % self->size;
	const tlibc_hash_bucket_t *bucket = &self->buckets[key_index];
	TLIBC_LIST_HEAD *iter;
	tuint32 i = 0;
	for(iter = bucket->data_list.next; iter != &bucket->data_list; iter = iter->next)
	{
		tlibc_hash_head_t *ele = TLIBC_CONTAINER_OF(iter, tlibc_hash_head_t, data_list);
		if(i >= self->all_data_list_num)
		{
			break;
		}
		if((ele->key_size == key_size ) && (memcmp(ele->key, key, key_size) == 0))
		{
			return ele;
		}
		++i;
	}
	return NULL;
}

void tlibc_hash_remove(tlibc_hash_t *self, tlibc_hash_head_t *ele)
{
	tlibc_list_del(&ele->all_data_list);
	tlibc_list_del(&ele->data_list);
	if(self->all_data_list_num > 0)
	{
		--self->all_data_list_num;
	}
}

void tlibc_hash_clear(tlibc_hash_t *self)
{
	TLIBC_LIST_HEAD *iter;
	for(iter = self->all_data_list.next; iter != &self->all_data_list; iter = iter->next)
	{
		tlibc_hash_head_t *ele = TLIBC_CONTAINER_OF(iter, tlibc_hash_head_t, all_data_list);
		tuint32 key_hash = tlibc_hash_key(ele->key, ele->key_size);
		tuint32 key_index = key_hash % self->size;
		tlibc_hash_bucket_t *bucket = &self->buckets[key_index];

		init_tlibc_list_head(&bucket->data_list);
	}
	init_tlibc_list_head(&self->all_data_list);
	self->all_data_list_num = 0;
}
