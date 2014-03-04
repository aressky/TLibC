#ifndef _H_TLIBC_COMPACT_WRITER
#define _H_TLIBC_COMPACT_WRITER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_writer.h"

typedef struct _TLIBC_COMPACT_WRITER
{
	TLIBC_ABSTRACT_WRITER super;

	char *addr;
	uint32_t size;
	uint32_t offset;
}TLIBC_COMPACT_WRITER;

void tlibc_compact_writer_init(TLIBC_COMPACT_WRITER *self, char *addr, uint32_t size);

TLIBC_ERROR_CODE tlibc_compact_write_int8(TLIBC_ABSTRACT_WRITER *super, const int8_t *val);

TLIBC_ERROR_CODE tlibc_compact_write_int16(TLIBC_ABSTRACT_WRITER *super, const int16_t *val);

TLIBC_ERROR_CODE tlibc_compact_write_int32(TLIBC_ABSTRACT_WRITER *super, const int32_t *val);

TLIBC_ERROR_CODE tlibc_compact_write_int64(TLIBC_ABSTRACT_WRITER *super, const int64_t *val);

TLIBC_ERROR_CODE tlibc_compact_write_uint8(TLIBC_ABSTRACT_WRITER *super, const uint8_t *val);

TLIBC_ERROR_CODE tlibc_compact_write_uint16(TLIBC_ABSTRACT_WRITER *super, const uint16_t *val);

TLIBC_ERROR_CODE tlibc_compact_write_uint32(TLIBC_ABSTRACT_WRITER *super, const uint32_t *val);

TLIBC_ERROR_CODE tlibc_compact_write_uint64(TLIBC_ABSTRACT_WRITER *super, const uint64_t *val);

TLIBC_ERROR_CODE tlibc_compact_write_char(TLIBC_ABSTRACT_WRITER *super, const char *val);

TLIBC_ERROR_CODE tlibc_compact_write_double(TLIBC_ABSTRACT_WRITER *super, const double *val);

TLIBC_ERROR_CODE tlibc_compact_write_string(TLIBC_ABSTRACT_WRITER *super, const char* str, uint32_t str_length);

#endif
