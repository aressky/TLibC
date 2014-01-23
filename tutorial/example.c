#include "tlibc/core/tlibc_hash.h"
#include "tlibc/core/tlibc_timer.h"
#include "tlibc/core/tlibc_mempool.h"


#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>


#define HASH_KEY_LENGTH 128
typedef struct _test_hash_data_t
{
	tlibc_hash_head_t hash_head;
	char key[HASH_KEY_LENGTH];
	int data;
}test_hash_data_t;

#define hash_bucket_size 1024

void test_hash()
{
	tlibc_hash_bucket_t buckets[hash_bucket_size];//定义桶的大小
	tlibc_hash_t hash_table;//定义hash表
	test_hash_data_t d0, d1, d2;
	tlibc_hash_head_t *pos;
	const tlibc_hash_head_t *pos_const;
	const test_hash_data_t *data;
	const TLIBC_LIST_HEAD *iter;
	const TLIBC_LIST_HEAD *iter_bucket;

	//准备数据
	d0.data = 0;
	snprintf(d0.key, HASH_KEY_LENGTH, "d0");

	d1.data = 10;
	snprintf(d1.key, HASH_KEY_LENGTH, "d1");

	d2.data = 200;
	snprintf(d2.key, HASH_KEY_LENGTH, "d2");

	//buckets内存不可释放掉
	tlibc_hash_init(&hash_table, buckets, hash_bucket_size);

	//insert
	//插入数据之后, key指向的内存不可以释放掉
	tlibc_hash_insert(&hash_table, d0.key, strlen(d0.key), &d0.hash_head);
	tlibc_hash_insert(&hash_table, d1.key, strlen(d1.key), &d1.hash_head);
	tlibc_hash_insert(&hash_table, d2.key, strlen(d2.key), &d2.hash_head);

	pos_const = tlibc_hash_find_const(&hash_table, "d0", 2);
	if(pos_const != NULL)
	{
		data = TLIBC_CONTAINER_OF(pos_const, const test_hash_data_t, hash_head);
		printf("find d0 data: %d\n", data->data);
	}
	

	//遍历所有数据
	printf("\n");
	for(iter_bucket = hash_table.used_bucket_list.next; iter_bucket != &hash_table.used_bucket_list; iter_bucket = iter_bucket->next)
	{
		const tlibc_hash_bucket_t *bucket = TLIBC_CONTAINER_OF(iter_bucket, const tlibc_hash_bucket_t, used_bucket_list);
		for(iter = bucket->data_list.next; iter != &bucket->data_list; iter = iter->next)
		{
			data = TLIBC_CONTAINER_OF(iter, const test_hash_data_t, hash_head);
			printf("key %s, data %d\n", data->key, data->data);
		}
	}
	
	
	//删除
	printf("\n");
	pos = tlibc_hash_find(&hash_table, "d2", 2);
	if(pos != NULL)
	{
		data = TLIBC_CONTAINER_OF(pos, const test_hash_data_t, hash_head);
		printf("find d2 data: %d\n", data->data);


		tlibc_hash_remove(&hash_table, pos);
		pos_const = tlibc_hash_find_const(&hash_table, "d2", 2);
		if(pos_const == NULL)
		{
			data = TLIBC_CONTAINER_OF(pos_const, const test_hash_data_t, hash_head);
			printf("can not find d2\n");
		}
	}

	//清空hash表
	printf("\n");
	tlibc_hash_clear(&hash_table);
	pos_const = tlibc_hash_find_const(&hash_table, "d0", 2);
	if(pos_const == NULL)
	{
		printf("can not find d0\n");
	}
}

typedef struct _timer_data_t
{
	tlibc_timer_entry_t timer_entry;

	int data;
}timer_data_t;

timer_data_t timer_db;
tlibc_timer_t timer;

#define TIMER_INTERVAL_MS 1000
#ifdef _WIN32
#include <winsock2.h>
time_t get_current_ms()
{
	time_t tv_usec;
	time_t tv_sec;
	union {
		long long ns100;
		FILETIME ft;
	} now;

	GetSystemTimeAsFileTime (&now.ft);
	tv_usec = (long) ((now.ns100 / 10LL) % 1000000LL);
	tv_sec = (long) ((now.ns100 - 116444736000000000LL) / 10000000LL);

	return tv_sec*1000 + tv_usec/1000;
}
#else
time_t get_current_ms()
{
	struct timeval tv;	
	gettimeofday(&tv, NULL);

	return tv.tv_sec*1000 + tv.tv_usec/1000;
}
#endif/*_WIN32*/


void timer_callback(const tlibc_timer_entry_t* header)
{
	timer_data_t *self = TLIBC_CONTAINER_OF(header, timer_data_t, timer_entry);
	printf("hello world %d!\n", self->data);
	timer_db.timer_entry.expires = timer.jiffies + TIMER_INTERVAL_MS;
	tlibc_timer_push(&timer, &timer_db.timer_entry);
}
time_t start_ms;
void test_timer()
{	
	tuint32 i = 0;
	tuint32 count = 0;
	tuint64 start_ms = get_current_ms();

	tlibc_timer_init(&timer, 0);
	timer_db.data = 123456;

	TIMER_ENTRY_BUILD(&timer_db.timer_entry, timer.jiffies + TIMER_INTERVAL_MS, timer_callback);
	

	
	for(;;)
	{
		TLIBC_ERROR_CODE ret = tlibc_timer_tick(&timer, get_current_ms() - start_ms);
		if(ret == E_TLIBC_AGAIN )
		{
			++count;
			if(count > 50)
			{
				Sleep(0);
			}
		}
	}
}

typedef struct _unit_t
{
	int data;
}unit_t;

#define MAX_UNIT_NUM 1024
char mem[TLIBC_MEMPOOL_SIZE(sizeof(unit_t), MAX_UNIT_NUM)];

void test_mempool()
{
	tlibc_mempool_t *mp = (tlibc_mempool_t*)mem;
	int unit_size = tlibc_mempool_init(mp, sizeof(mem), sizeof(unit_t));
	tuint64 mid, mid_last;
	void *addr;
	int i;

	assert(unit_size == MAX_UNIT_NUM);
	for(i = 0; i < unit_size; ++i)
	{		
		mid = tlibc_mempool_alloc(mp);
		assert(mid != TLIBC_MEMPOOL_INVALID_INDEX);
		addr = tlibc_mempool_get(mp, mid);
		assert(addr != NULL);
		mid_last = mid;
	}
	mid = tlibc_mempool_alloc(mp);
	assert(mid == TLIBC_MEMPOOL_INVALID_INDEX);
	addr = tlibc_mempool_get(mp, mid);
	assert(addr == NULL);

	tlibc_mempool_free(mp, mid_last);
	//重复删除不会出错
	tlibc_mempool_free(mp, mid_last);
	mid = tlibc_mempool_alloc(mp);
	//mid不重用
	assert(mid != mid_last);
	assert(mid != TLIBC_MEMPOOL_INVALID_INDEX);
	addr = tlibc_mempool_get(mp, mid);
	assert(addr != NULL);

	tlibc_mempool_free(mp, mid_last + 1);
	mid = tlibc_mempool_alloc(mp);
	assert(mid == TLIBC_MEMPOOL_INVALID_INDEX);
	addr = tlibc_mempool_get(mp, mid);
	assert(addr == NULL);

	printf("%d\n", unit_size);
}

int main()
{
	//test_hash();

	//test_timer();

	test_mempool();

	return 0;
}
