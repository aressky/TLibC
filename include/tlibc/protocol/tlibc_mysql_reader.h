#ifndef _H_TLIBC_MYSQL_READER
#define _H_TLIBC_MYSQL_READER

#include "mysql.h"

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

#include "tlibc/core/tlibc_hash.h"

typedef struct _tlibc_mysql_field_t
{
	tlibc_hash_head_t		name2field;
	const MYSQL_FIELD		*myfield;
}tlibc_mysql_field_t;

#define TLIBC_MYSQL_FIELD_VEC_NUM 65536
#define TLIBC_MYSQL_HASH_BUCKET 65536
typedef struct _tlibc_mysql_reader_t
{
	TLIBC_ABSTRACT_READER	super;
	const MYSQL_FIELD		*mysql_field_vec;
	uint32_t				mysql_field_vec_num;
	const MYSQL_FIELD		*cur_myfield;
	char*					cur_col;
	unsigned long			cur_col_length;

	tlibc_hash_bucket_t		hash_bucket[TLIBC_MYSQL_HASH_BUCKET];
	tlibc_hash_t			name2field;


	tlibc_mysql_field_t		field_vec[TLIBC_MYSQL_FIELD_VEC_NUM];
	MYSQL_ROW				row;
	const unsigned long		*length_vec;
}tlibc_mysql_reader_t;

void tlibc_mysql_reader_init(tlibc_mysql_reader_t *self, const MYSQL_FIELD *mysql_field_vec, uint32_t mysql_field_vec_num);

void tlibc_mysql_reader_fetch(tlibc_mysql_reader_t *self, MYSQL_ROW row
										  , const unsigned long *length_vec);


 TLIBC_ERROR_CODE tlibc_mysql_read_field_begin(TLIBC_ABSTRACT_READER *self, const char *var_name);

 TLIBC_ERROR_CODE tlibc_mysql_read_vector_element_begin(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index);

 TLIBC_ERROR_CODE tlibc_mysql_read_char(TLIBC_ABSTRACT_READER *super, char *val);

 TLIBC_ERROR_CODE tlibc_mysql_read_double(TLIBC_ABSTRACT_READER *super, double *val);

 TLIBC_ERROR_CODE tlibc_mysql_read_int8(TLIBC_ABSTRACT_READER *super, int8_t *val);

 TLIBC_ERROR_CODE tlibc_mysql_read_int16(TLIBC_ABSTRACT_READER *super, int16_t *val);

 TLIBC_ERROR_CODE tlibc_mysql_read_int32(TLIBC_ABSTRACT_READER *super, int32_t *val);

 TLIBC_ERROR_CODE tlibc_mysql_read_int64(TLIBC_ABSTRACT_READER *super, int64_t *val);

 TLIBC_ERROR_CODE tlibc_mysql_read_uint8(TLIBC_ABSTRACT_READER *super, uint8_t *val);

 TLIBC_ERROR_CODE tlibc_mysql_read_uint16(TLIBC_ABSTRACT_READER *super, uint16_t *val);

 TLIBC_ERROR_CODE tlibc_mysql_read_uint32(TLIBC_ABSTRACT_READER *super, uint32_t *val);

 TLIBC_ERROR_CODE tlibc_mysql_read_uint64(TLIBC_ABSTRACT_READER *super, uint64_t *val);

 TLIBC_ERROR_CODE tlibc_mysql_read_string(TLIBC_ABSTRACT_READER *super, char *str, uint32_t str_len);

#endif

