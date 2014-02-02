#include "tlibc/protocol/tlibc_binary_reader.h"
#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

#include <string.h>
#include <assert.h>


void tlibc_binary_reader_init(TLIBC_BINARY_READER *self, const char *addr, tuint32 size)
{
	tlibc_abstract_reader_init(&self->super);

	self->super.read_tchar = tlibc_binary_read_tchar;
	self->super.read_tdouble = tlibc_binary_read_tdouble;
	self->super.read_tint8 = tlibc_binary_read_tint8;
	self->super.read_tint16 = tlibc_binary_read_tint16;
	self->super.read_tint32 = tlibc_binary_read_tint32;
	self->super.read_tint64 = tlibc_binary_read_tint64;
	self->super.read_tuint8 = tlibc_binary_read_tuint8;
	self->super.read_tuint16 = tlibc_binary_read_tuint16;
	self->super.read_tuint32 = tlibc_binary_read_tuint32;
	self->super.read_tuint64 = tlibc_binary_read_tuint64;
	self->super.read_tstring = tlibc_binary_read_tstring;



	self->addr = addr;
	self->size = size;
	self->offset = 0;
}

#define READER_CAPACITY(self) (self->size - self->offset)
#define READER_PTR(self) (self->addr + self->offset)


TLIBC_ERROR_CODE tlibc_binary_read_tint8(TLIBC_ABSTRACT_READER *super, tint8 *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(tint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(tint8*)READER_PTR(self);
	self->offset += sizeof(tint8);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_tint16(TLIBC_ABSTRACT_READER *super, tint16 *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(tint16))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(tint16*)READER_PTR(self);
	tlibc_little_to_host16(*val);
	self->offset += sizeof(tint16);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_tint32(TLIBC_ABSTRACT_READER *super, tint32 *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(tint32))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(tint32*)READER_PTR(self);
	tlibc_little_to_host32(*val);
	self->offset += sizeof(tint32);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_tint64(TLIBC_ABSTRACT_READER *super, tint64 *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(tint64))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(tint64*)READER_PTR(self);
	tlibc_little_to_host64(*val)
	self->offset += sizeof(tint64);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_tuint8(TLIBC_ABSTRACT_READER *super, tuint8 *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(tuint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(tuint8*)READER_PTR(self);
	self->offset += sizeof(tuint8);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_tuint16(TLIBC_ABSTRACT_READER *super, tuint16 *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(tuint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(tuint8*)READER_PTR(self);
	tlibc_little_to_host16(*val);
	self->offset += sizeof(tuint8);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_tuint32(TLIBC_ABSTRACT_READER *super, tuint32 *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(tuint32))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(tuint32*)READER_PTR(self);
	tlibc_little_to_host32(*val);
	self->offset += sizeof(tuint32);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_tuint64(TLIBC_ABSTRACT_READER *super, tuint64 *val)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	if(READER_CAPACITY(self) < sizeof(tuint64))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(tuint64*)READER_PTR(self);
	tlibc_little_to_host64(*val);
	self->offset += sizeof(tuint64);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_read_tchar(TLIBC_ABSTRACT_READER *super, char *val)
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


TLIBC_ERROR_CODE tlibc_binary_read_tdouble(TLIBC_ABSTRACT_READER *super, double *val)
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


TLIBC_ERROR_CODE tlibc_binary_read_tstring(TLIBC_ABSTRACT_READER *super, tchar* str, tuint32 str_length)
{
	TLIBC_BINARY_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_READER, super);
	tuint32 str_len = 0;
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;

	for(; self->offset < self->size; ++(self->offset))
	{
		if(str_len >= str_length)
		{
			ret = E_TLIBC_OUT_OF_MEMORY;
			goto done;
		}
		str[str_len++] = self->addr[self->offset];

		if(self->addr[self->offset] == 0)
		{
			goto done;
		}
	}
	ret = E_TLIBC_OUT_OF_MEMORY;
done:
	return ret;
}

