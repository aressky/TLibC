#ifndef _H_TLIBC_HASH
#define _H_TLIBC_HASH

#include "platform/tlibc_platform.h"
#include "lib/tlibc_list.h"
#include "lib/tlibc_error_code.h"

typedef struct _tlibc_hash_head_t tlibc_hash_head_t;
struct _tlibc_hash_head_t
{
	TLIBC_LIST_HEAD data_list;
	TLIBC_LIST_HEAD all_data_list;
	const char* key;
	tuint32 key_size;
};

typedef struct _tlibc_hash_bucket_t tlibc_hash_bucket_t;
struct _tlibc_hash_bucket_t
{
	TLIBC_LIST_HEAD data_list;
};

typedef struct _tlibc_hash_t tlibc_hash_t;
struct _tlibc_hash_t
{
	tlibc_hash_bucket_t		*buckets;
	tuint32					size;

	TLIBC_LIST_HEAD			all_data_list;
};

TLIBC_ERROR_CODE tlibc_hash_init(tlibc_hash_t *self, tlibc_hash_bucket_t *buckets, tuint32 size);

tuint32 tlibc_hash_key(const char *key, tuint32 key_size);

void tlibc_hash_insert(tlibc_hash_t *self, const char *key, tuint32 key_size, tlibc_hash_head_t *val);

const tlibc_hash_head_t* tlibc_hash_find(const tlibc_hash_t *self, const char *key, tuint32 key_size);

void tlibc_hash_remove(tlibc_hash_t *self, tlibc_hash_head_t *ele);

void tlibc_hash_clear(tlibc_hash_t *self);

#endif//_H_TLIBC_HASH
