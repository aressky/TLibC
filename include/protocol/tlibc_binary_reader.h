#ifndef _H_TLIBC_BINARY_READER
#define _H_TLIBC_BINARY_READER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_reader.h"

typedef struct _TLIBC_BINARY_READER
{
	TLIBC_ABSTRACT_READER super;

	const char *addr;
	uint32_t size;
	uint32_t offset;
}TLIBC_BINARY_READER;

 void tlibc_binary_reader_init(TLIBC_BINARY_READER *self, const char *addr, uint32_t size);

 TLIBC_ERROR_CODE tlibc_binary_read_int8(TLIBC_ABSTRACT_READER *super, int8_t *val);

 TLIBC_ERROR_CODE tlibc_binary_read_int16(TLIBC_ABSTRACT_READER *super, int16_t *val);

 TLIBC_ERROR_CODE tlibc_binary_read_int32(TLIBC_ABSTRACT_READER *super, int32_t *val);

 TLIBC_ERROR_CODE tlibc_binary_read_int64(TLIBC_ABSTRACT_READER *super, int64_t *val);

 TLIBC_ERROR_CODE tlibc_binary_read_uint8(TLIBC_ABSTRACT_READER *super, uint8_t *val);

 TLIBC_ERROR_CODE tlibc_binary_read_uint16(TLIBC_ABSTRACT_READER *super, uint16_t *val);

 TLIBC_ERROR_CODE tlibc_binary_read_uint32(TLIBC_ABSTRACT_READER *super, uint32_t *val);

 TLIBC_ERROR_CODE tlibc_binary_read_uint64(TLIBC_ABSTRACT_READER *super, uint64_t *val);

 TLIBC_ERROR_CODE tlibc_binary_read_char(TLIBC_ABSTRACT_READER *super, char *val);

 TLIBC_ERROR_CODE tlibc_binary_read_double(TLIBC_ABSTRACT_READER *super, double *val);

 TLIBC_ERROR_CODE tlibc_binary_read_string(TLIBC_ABSTRACT_READER *super, char* str, uint32_t str_length);

#endif
