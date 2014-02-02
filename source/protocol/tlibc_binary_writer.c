#include "tlibc/protocol/tlibc_binary_writer.h"
#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_writer.h"
#include "tlibc/core/tlibc_error_code.h"

#include <string.h>
#include <assert.h>


void tlibc_binary_writer_init(TLIBC_BINARY_WRITER *self, char *addr, tuint32 size)
{
	tlibc_abstract_writer_init(&self->super);

	self->super.write_tchar = tlibc_binary_write_tchar;
	self->super.write_tdouble = tlibc_binary_write_tdouble;
	self->super.write_tint8 = tlibc_binary_write_tint8;
	self->super.write_tint16 = tlibc_binary_write_tint16;
	self->super.write_tint32 = tlibc_binary_write_tint32;
	self->super.write_tint64 = tlibc_binary_write_tint64;
	self->super.write_tuint8 = tlibc_binary_write_tuint8;
	self->super.write_tuint16 = tlibc_binary_write_tuint16;
	self->super.write_tuint32 = tlibc_binary_write_tuint32;
	self->super.write_tuint64 = tlibc_binary_write_tuint64;
	self->super.write_tstring = tlibc_binary_write_tstring;

	self->addr = addr;
	self->size = size;
	self->offset = 0;
}

#define WRITER_CAPACITY(self) (self->size - self->offset)
#define WRITER_PTR(self) (self->addr + self->offset)

TLIBC_ERROR_CODE tlibc_binary_write_tint8(TLIBC_ABSTRACT_WRITER *super, const tint8 *val)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	if(WRITER_CAPACITY(self) < sizeof(tint8))
	{
		goto not_enough_bytebuff_size;
	}
	*(tint8*)WRITER_PTR(self) = *val;
	self->offset += sizeof(tint8);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_write_tint16(TLIBC_ABSTRACT_WRITER *super, const tint16 *val)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	if(WRITER_CAPACITY(self) < sizeof(tint16))
	{
		goto not_enough_bytebuff_size;
	}
	*(tint16*)WRITER_PTR(self) = *val;
	tlibc_host16_to_little(*(tint16*)WRITER_PTR(self));
	self->offset += sizeof(tint16);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_write_tint32(TLIBC_ABSTRACT_WRITER *super, const tint32 *val)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	if(WRITER_CAPACITY(self) < sizeof(tint32))
	{
		goto not_enough_bytebuff_size;
	}
	*(tint32*)WRITER_PTR(self) = *val;
	tlibc_host32_to_little(*(tint32*)WRITER_PTR(self));
	self->offset += sizeof(tint32);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_write_tint64(TLIBC_ABSTRACT_WRITER *super, const tint64 *val)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	if(WRITER_CAPACITY(self) < sizeof(tint64))
	{
		goto not_enough_bytebuff_size;
	}
	*(tint64*)WRITER_PTR(self) = *val;
	tlibc_host64_to_little(*(tint64*)WRITER_PTR(self));
	self->offset += sizeof(tint64);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}


TLIBC_ERROR_CODE tlibc_binary_write_tuint8(TLIBC_ABSTRACT_WRITER *super, const tuint8 *val)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	if(WRITER_CAPACITY(self) < sizeof(tuint8))
	{
		goto not_enough_bytebuff_size;
	}
	*(tuint8*)WRITER_PTR(self) = *val;
	self->offset += sizeof(tuint8);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_write_tuint16(TLIBC_ABSTRACT_WRITER *super, const tuint16 *val)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	if(WRITER_CAPACITY(self) < sizeof(tuint16))
	{
		goto not_enough_bytebuff_size;
	}
	*(tuint16*)WRITER_PTR(self) = *val;
	tlibc_host16_to_little(*(tuint16*)WRITER_PTR(self));
	self->offset += sizeof(tuint16);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_write_tuint32(TLIBC_ABSTRACT_WRITER *super, const tuint32 *val)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	if(WRITER_CAPACITY(self) < sizeof(tuint32))
	{
		goto not_enough_bytebuff_size;
	}
	*(tuint32*)WRITER_PTR(self) = *val;
	tlibc_host32_to_little(*(tuint32*)WRITER_PTR(self));
	self->offset += sizeof(tuint32);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_write_tuint64(TLIBC_ABSTRACT_WRITER *super, const tuint64 *val)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	if(WRITER_CAPACITY(self) < sizeof(tuint64))
	{
		goto not_enough_bytebuff_size;
	}
	*(tuint64*)WRITER_PTR(self) = *val;
	tlibc_host64_to_little(*(tuint64*)WRITER_PTR(self));
	self->offset += sizeof(tuint64);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;

}

TLIBC_ERROR_CODE tlibc_binary_write_tchar(TLIBC_ABSTRACT_WRITER *super, const char *val)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	if(WRITER_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*(char*)WRITER_PTR(self) = *val;
	self->offset += sizeof(char);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_write_tdouble(TLIBC_ABSTRACT_WRITER *super, const double *val)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	if(WRITER_CAPACITY(self) < sizeof(double))
	{
		goto not_enough_bytebuff_size;
	}
	*(double*)WRITER_PTR(self) = *val;
	self->offset += sizeof(double);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_binary_write_tstring(TLIBC_ABSTRACT_WRITER *super, const tchar* str)
{
	TLIBC_BINARY_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_BINARY_WRITER, super);
	tuint32 str_len = 0;
	TLIBC_ERROR_CODE ret= E_TLIBC_NOERROR;

	for(; self->offset < self->size; ++(self->offset))
	{
		self->addr[self->offset] = str[str_len++];

		if(self->addr[self->offset] == 0)
		{
			goto done;
		}
	}
	ret = E_TLIBC_OUT_OF_MEMORY;
done:
	return ret;
}
