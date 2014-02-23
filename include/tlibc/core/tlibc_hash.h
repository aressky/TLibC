#ifndef _H_TLIBC_HASH
#define _H_TLIBC_HASH

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_list.h"
#include "tlibc/core/tlibc_error_code.h"
#include <stdint.h>
typedef struct _tlibc_hash_head_t tlibc_hash_head_t;
struct _tlibc_hash_head_t
{
	TLIBC_LIST_HEAD data_list;
	const char* key;
	uint32_t key_size;
	uint32_t key_index;
};

typedef struct _tlibc_hash_bucket_t tlibc_hash_bucket_t;
struct _tlibc_hash_bucket_t
{
	TLIBC_LIST_HEAD data_list;
	TLIBC_LIST_HEAD used_bucket_list;
	uint32_t data_list_num;
};

typedef struct _tlibc_hash_t tlibc_hash_t;
struct _tlibc_hash_t
{
	tlibc_hash_bucket_t		*buckets;
	uint32_t					size;

	TLIBC_LIST_HEAD			used_bucket_list;
	uint32_t					used_bucket_list_num;
};

 TLIBC_ERROR_CODE tlibc_hash_init(tlibc_hash_t *self, tlibc_hash_bucket_t *buckets, uint32_t size);

 uint32_t tlibc_hash_key(const char *key, uint32_t key_size);

 void tlibc_hash_insert(tlibc_hash_t *self, const char *key, uint32_t key_size, tlibc_hash_head_t *val);

 const tlibc_hash_head_t* tlibc_hash_find_const(const tlibc_hash_t *self, const char *key, uint32_t key_size);

 tlibc_hash_head_t* tlibc_hash_find(tlibc_hash_t *self, const char *key, uint32_t key_size);

 void tlibc_hash_remove(tlibc_hash_t *self, tlibc_hash_head_t *ele);

 void tlibc_hash_clear(tlibc_hash_t *self);

#endif//_H_TLIBC_HASH
