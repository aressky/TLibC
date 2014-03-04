#ifndef _H_TLIBC_BIND_WRITER
#define _H_TLIBC_BIND_WRITER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_writer.h"
#include "core/tlibc_error_code.h"

#include <stdio.h>
#include <stdint.h>


#include "mysql.h"

typedef struct _tlibc_bind_writer_t
{
	TLIBC_ABSTRACT_WRITER super;
	MYSQL_BIND *bind_vec;
	uint32_t bind_vec_num;
	uint32_t idx;
}tlibc_bind_writer_t;

void tlibc_bind_writer_init(tlibc_bind_writer_t *self, MYSQL_BIND *bind_vec, uint32_t bind_vec_num);

 TLIBC_ERROR_CODE tlibc_bind_write_int8(TLIBC_ABSTRACT_WRITER *super, const int8_t *val);

 TLIBC_ERROR_CODE tlibc_bind_write_int16(TLIBC_ABSTRACT_WRITER *super, const int16_t *val);

 TLIBC_ERROR_CODE tlibc_bind_write_int32(TLIBC_ABSTRACT_WRITER *super, const int32_t *val);

 TLIBC_ERROR_CODE tlibc_bind_write_int64(TLIBC_ABSTRACT_WRITER *super, const int64_t *val);

 TLIBC_ERROR_CODE tlibc_bind_write_uint8(TLIBC_ABSTRACT_WRITER *super, const uint8_t *val);

 TLIBC_ERROR_CODE tlibc_bind_write_uint16(TLIBC_ABSTRACT_WRITER *super, const uint16_t *val);

 TLIBC_ERROR_CODE tlibc_bind_write_uint32(TLIBC_ABSTRACT_WRITER *super, const uint32_t *val);

 TLIBC_ERROR_CODE tlibc_bind_write_uint64(TLIBC_ABSTRACT_WRITER *super, const uint64_t *val);

 TLIBC_ERROR_CODE tlibc_bind_write_double(TLIBC_ABSTRACT_WRITER *super, const double *val);

 TLIBC_ERROR_CODE tlibc_bind_write_char(TLIBC_ABSTRACT_WRITER *super, const char *val);

 TLIBC_ERROR_CODE tlibc_bind_write_string(TLIBC_ABSTRACT_WRITER *super, const char* str, uint32_t str_length);

#endif
