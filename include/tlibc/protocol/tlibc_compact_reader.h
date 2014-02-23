#ifndef _H_TLIBC_COMPACT_READER
#define _H_TLIBC_COMPACT_READER

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"

typedef struct _TLIBC_COMPACT_READER
{
	TLIBC_ABSTRACT_READER super;

	const char *addr;
	uint32_t size;
	uint32_t offset;
}TLIBC_COMPACT_READER;

 void tlibc_compact_reader_init(TLIBC_COMPACT_READER *self, const char *addr, uint32_t size);

 TLIBC_ERROR_CODE tlibc_compact_read_int8_t(TLIBC_ABSTRACT_READER *super, int8_t *val);

 TLIBC_ERROR_CODE tlibc_compact_read_int16_t(TLIBC_ABSTRACT_READER *super, int16_t *val);

 TLIBC_ERROR_CODE tlibc_compact_read_int32_t(TLIBC_ABSTRACT_READER *super, int32_t *val);

 TLIBC_ERROR_CODE tlibc_compact_read_int64_t(TLIBC_ABSTRACT_READER *super, int64_t *val);

 TLIBC_ERROR_CODE tlibc_compact_read_uint8_t(TLIBC_ABSTRACT_READER *super, uint8_t *val);

 TLIBC_ERROR_CODE tlibc_compact_read_uint16_t(TLIBC_ABSTRACT_READER *super, uint16_t *val);

 TLIBC_ERROR_CODE tlibc_compact_read_uint32_t(TLIBC_ABSTRACT_READER *super, uint32_t *val);

 TLIBC_ERROR_CODE tlibc_compact_read_uint64_t(TLIBC_ABSTRACT_READER *super, uint64_t *val);

 TLIBC_ERROR_CODE tlibc_compact_read_char(TLIBC_ABSTRACT_READER *super, char *val);

 TLIBC_ERROR_CODE tlibc_compact_read_double(TLIBC_ABSTRACT_READER *super, double *val);

 TLIBC_ERROR_CODE tlibc_compact_read_string(TLIBC_ABSTRACT_READER *super, char* str, uint32_t str_length);

#endif
