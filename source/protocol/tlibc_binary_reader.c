#include "tlibc/protocol/tlibc_binary_reader.h"
#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

#include <string.h>
#include <assert.h>


void tlibc_binary_reader_init(TLIBC_BINARY_READER *self, const char *addr, uint32_t size)
{
	tlibc_abstract_reader_init(&self->super);

	self->super.read_char = tlibc_binary_read_char;
	self->super.read_double = tlibc_binary_read_double;
	self->super.read_int8 = tlibc_binary_read_int8;
	self->super.read_int16 = tlibc_binary_read_int16;
	self->super.read_int32 = tlibc_binary_read_int32;
	self->super.read_int64 = tlibc_binary_read_int64;
	self->super.read_uint8 = tlibc_binary_read_uint8;
	self->super.read_uint16 = tlibc_binary_read_uint16;
	self->super.read_uint32 = tlibc_binary_read_uint32;
	self->super.read_uint64 = tlibc_binary_read_uint64;
	self->super.read_string = tlibc_binary_read_string;



	self->addr = addr;
	self->size = size;
	self->offset = 0;
}

#define READER_CAPACITY(self) (self->size - self->offset)
#define READER_PTR(self) (self->addr + self->offset)


TLIBC_ERROR_CODE tlibc_binary_read_int8(TLIBC_ABSTRACT_READER *super, int8_t *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(int8_t))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(int8_t*)READER_PTR(self);
	self->offset += sizeof(int8_t);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_int16(TLIBC_ABSTRACT_READER *super, int16_t *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(int16_t))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(int16_t*)READER_PTR(self);
	tlibc_little_to_host16(*val);
	self->offset += sizeof(int16_t);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_int32(TLIBC_ABSTRACT_READER *super, int32_t *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(int32_t))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(int32_t*)READER_PTR(self);
	tlibc_little_to_host32(*val);
	self->offset += sizeof(int32_t);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_int64(TLIBC_ABSTRACT_READER *super, int64_t *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(int64_t))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(int64_t*)READER_PTR(self);
	tlibc_little_to_host64(*val)
	self->offset += sizeof(int64_t);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_uint8(TLIBC_ABSTRACT_READER *super, uint8_t *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(uint8_t))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(uint8_t*)READER_PTR(self);
	self->offset += sizeof(uint8_t);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_uint16(TLIBC_ABSTRACT_READER *super, uint16_t *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(uint8_t))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(uint8_t*)READER_PTR(self);
	tlibc_little_to_host16(*val);
	self->offset += sizeof(uint16_t);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_uint32(TLIBC_ABSTRACT_READER *super, uint32_t *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(uint32_t))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(uint32_t*)READER_PTR(self);
	tlibc_little_to_host32(*val);
	self->offset += sizeof(uint32_t);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_uint64(TLIBC_ABSTRACT_READER *super, uint64_t *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(uint64_t))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(uint64_t*)READER_PTR(self);
	tlibc_little_to_host64(*val);
	self->offset += sizeof(uint64_t);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_char(TLIBC_ABSTRACT_READER *super, char *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(char*)READER_PTR(self);
	self->offset += sizeof(char);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}


TLIBC_ERROR_CODE tlibc_binary_read_double(TLIBC_ABSTRACT_READER *super, double *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(double))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(double*)READER_PTR(self);
	self->offset += sizeof(double);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}


TLIBC_ERROR_CODE tlibc_binary_read_string(TLIBC_ABSTRACT_READER *super, char* str, uint32_t str_length)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	uint32_t str_len = 0;
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;

	for(; self->offset < self->size; )
	{
		char c;
		if(str_len >= str_length)
		{
			ret = E_TLIBC_OUT_OF_MEMORY;
			goto done;
		}
		c = (str[str_len++] = self->addr[self->offset++]);

		if(c == 0)
		{
			goto done;
		}
	}

	ret = E_TLIBC_OUT_OF_MEMORY;
done:
	return ret;
}

