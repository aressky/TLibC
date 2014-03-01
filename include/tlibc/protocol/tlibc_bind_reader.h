#ifndef _H_TLIBC_BIND_READER
#define _H_TLIBC_BIND_READER

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

typedef struct _tlibc_bind_reader_t
{
	TLIBC_ABSTRACT_READER		super;
	tlibc_bind_s				*bind_vec;
	uint32_t					bind_vec_num;
	uint32_t					idx;
	int							read_enum_name;
}tlibc_bind_reader_t;

void tlibc_bind_reader_init(tlibc_bind_reader_t *self, tlibc_bind_s *bind_vec, uint32_t bind_vec_num);

TLIBC_ERROR_CODE tlibc_bind_read_enum_begin(TLIBC_ABSTRACT_READER *super, const char *enum_name);

 TLIBC_ERROR_CODE tlibc_bind_read_int8(TLIBC_ABSTRACT_READER *super, int8_t *val);

 TLIBC_ERROR_CODE tlibc_bind_read_int16(TLIBC_ABSTRACT_READER *super, int16_t *val);

 TLIBC_ERROR_CODE tlibc_bind_read_int32(TLIBC_ABSTRACT_READER *super, int32_t *val);

 TLIBC_ERROR_CODE tlibc_bind_read_int64(TLIBC_ABSTRACT_READER *super, int64_t *val);

 TLIBC_ERROR_CODE tlibc_bind_read_uint8(TLIBC_ABSTRACT_READER *super, uint8_t *val);

 TLIBC_ERROR_CODE tlibc_bind_read_uint16(TLIBC_ABSTRACT_READER *super, uint16_t *val);

 TLIBC_ERROR_CODE tlibc_bind_read_uint32(TLIBC_ABSTRACT_READER *super, uint32_t *val);

 TLIBC_ERROR_CODE tlibc_bind_read_uint64(TLIBC_ABSTRACT_READER *super, uint64_t *val);

 TLIBC_ERROR_CODE tlibc_bind_read_double(TLIBC_ABSTRACT_READER *super, double *val);

 TLIBC_ERROR_CODE tlibc_bind_read_char(TLIBC_ABSTRACT_READER *super, char *val);

 TLIBC_ERROR_CODE tlibc_bind_read_string(TLIBC_ABSTRACT_READER *super, char *str, uint32_t str_len);

#endif

