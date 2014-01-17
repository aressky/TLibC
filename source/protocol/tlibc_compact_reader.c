#include "tlibc/protocol/tlibc_compact_reader.h"
#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

#include <string.h>
#include <assert.h>

static TLIBC_ERROR_CODE tlibc_compact_varint16_decode(const char *buff_ptr, tuint32 *buff_size, tuint16 *result)
{
	tuint8 b;

	if(*buff_size < 1)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 0);
	*result = (b & 0x7F);
	if (!(b & 0x80))
	{
		*buff_size = 1;
		goto done;
	}

	if(*buff_size < 2)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 1);
	*result |= (b & 0x7F) << 7;
	if (!(b & 0x80))
	{
		*buff_size = 2;
		goto done;
	}

	if(*buff_size < 3)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 2);
	*result |= (b & 0x7F) << 14;
	if (!(b & 0x80))
	{
		*buff_size = 3;
		goto done;
	}

	return E_TLIBC_ERROR;
done:
	return E_TLIBC_NOERROR;

not_enough_byte_size:
	return E_TLIBC_OUT_OF_MEMORY;
}


static TLIBC_ERROR_CODE tlibc_compact_varint32_decode(const char *buff_ptr, tuint32 *buff_size, tuint32 *result)
{
	tuint8 b;

	if(*buff_size < 1)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 0);
	*result = (b & 0x7F);
	if (!(b & 0x80))
	{
		*buff_size = 1;
		goto done;
	}

	if(*buff_size < 2)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 1);
	*result |= (b & 0x7F) << 7;
	if (!(b & 0x80))
	{
		*buff_size = 2;
		goto done;
	}

	if(*buff_size < 3)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 2);
	*result |= (b & 0x7F) << 14;
	if (!(b & 0x80))
	{
		*buff_size = 3;
		goto done;
	}

	if(*buff_size < 4)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 3);
	*result |= (b & 0x7F) << 21;
	if (!(b & 0x80))
	{
		*buff_size = 4;
		goto done;
	}

	if(*buff_size < 5)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 4);
	*result |= (b & 0x7F) << 28;
	if (!(b & 0x80))
	{
		*buff_size = 5;
		goto done;
	}	

	return E_TLIBC_ERROR;
done:
	return E_TLIBC_NOERROR;

not_enough_byte_size:
	return E_TLIBC_OUT_OF_MEMORY;
}


static TLIBC_ERROR_CODE tlibc_compact_varint64_decode(const char *buff_ptr, tuint32 *buff_size, tuint64 *result)
{
	tuint8 b;

	tuint32 par0 = 0;
	tuint32 par1 = 0;
	tuint32 par2 = 0;

	//par0
	if(*buff_size < 1)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 0);
	par0 = (b & 0x7F);
	if (!(b & 0x80))
	{
		*buff_size = 1;
		goto done;
	}

	if(*buff_size < 2)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 1);
	par0 |= (b & 0x7F) << 7;
	if (!(b & 0x80))
	{
		*buff_size = 2;
		goto done;
	}

	if(*buff_size < 3)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 2);
	par0 |= (b & 0x7F) << 14;
	if (!(b & 0x80))
	{
		*buff_size = 3;
		goto done;
	}

	if(*buff_size < 4)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 3);
	par0 |= (b & 0x7F) << 21;
	if (!(b & 0x80))
	{
		*buff_size = 4;
		goto done;
	}

	//par1
	if(*buff_size < 5)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 4);
	par1 = (b & 0x7F);
	if (!(b & 0x80))
	{
		*buff_size = 5;
		goto done;
	}

	if(*buff_size < 6)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 5);
	par1 |= (b & 0x7F) << 7;
	if (!(b & 0x80))
	{
		*buff_size = 6;
		goto done;
	}

	if(*buff_size < 7)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 6);
	par1 |= (b & 0x7F) << 14;
	if (!(b & 0x80))
	{
		*buff_size = 7;
		goto done;
	}

	if(*buff_size < 8)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 7);
	par1 |= (b & 0x7F) << 21;
	if (!(b & 0x80))
	{
		*buff_size = 8;
		goto done;
	}

	//par2
	if(*buff_size < 9)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 8);
	par2 = (b & 0x7F);
	if (!(b & 0x80))
	{
		*buff_size = 9;
		goto done;
	}

	if(*buff_size < 10)
	{
		goto not_enough_byte_size;
	}
	b = *(tuint8*)(buff_ptr + 9);
	par2 |= (b & 0x7F) << 7;
	if (!(b & 0x80))
	{
		*buff_size = 10;
		goto done;
	}
	return E_TLIBC_ERROR;
done:
	*result = ((tuint64)par0) | ((tuint64)par1 << 28 )| ((tuint64)par2 << 56);
	return E_TLIBC_NOERROR;

not_enough_byte_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

void tlibc_compact_reader_init(TLIBC_COMPACT_READER *self, const char *addr, tuint32 size)
{
	tlibc_abstract_reader_init(&self->super);

	self->super.read_tchar = tlibc_compact_read_tchar;
	self->super.read_tdouble = tlibc_compact_read_tdouble;
	self->super.read_tint8 = tlibc_compact_read_tint8;
	self->super.read_tint16 = tlibc_compact_read_tint16;
	self->super.read_tint32 = tlibc_compact_read_tint32;
	self->super.read_tint64 = tlibc_compact_read_tint64;
	self->super.read_tuint8 = tlibc_compact_read_tuint8;
	self->super.read_tuint16 = tlibc_compact_read_tuint16;
	self->super.read_tuint32 = tlibc_compact_read_tuint32;
	self->super.read_tuint64 = tlibc_compact_read_tuint64;
	self->super.read_tstring = tlibc_compact_read_tstring;



	self->addr = addr;
	self->size = size;
	self->offset = 0;
}

#define COMPACT_READER_CAPACITY(self) (self->size - self->offset)
#define COMPACT_READER_PTR(self) (self->addr + self->offset)


TLIBC_ERROR_CODE tlibc_compact_read_tint8(TLIBC_ABSTRACT_READER *super, tint8 *val)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
	if(COMPACT_READER_CAPACITY(self) < sizeof(tint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(tint8*)COMPACT_READER_PTR(self);
	self->offset += sizeof(tint8);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_compact_read_tint16(TLIBC_ABSTRACT_READER *super, tint16 *val)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
	tuint16 res;
	tuint32 buff_size = COMPACT_READER_CAPACITY(self);
	TLIBC_ERROR_CODE ret = tlibc_compact_varint16_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	*val = tlibc_zigzag_decode16(tlibc_little_to_host16(res));
	self->offset +=buff_size;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_read_tint32(TLIBC_ABSTRACT_READER *super, tint32 *val)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
	tuint32 res;
	tuint32 buff_size = COMPACT_READER_CAPACITY(self);
	TLIBC_ERROR_CODE ret = tlibc_compact_varint32_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	*val = tlibc_zigzag_decode32(tlibc_little_to_host32(res));
	self->offset +=buff_size;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_read_tint64(TLIBC_ABSTRACT_READER *super, tint64 *val)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
	tuint64 res;
	tuint32 buff_size = COMPACT_READER_CAPACITY(self);
	TLIBC_ERROR_CODE ret = tlibc_compact_varint64_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	*val = tlibc_zigzag_decode64(tlibc_little_to_host64(res));
	self->offset +=buff_size;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_read_tuint8(TLIBC_ABSTRACT_READER *super, tuint8 *val)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
	if(COMPACT_READER_CAPACITY(self) < sizeof(tuint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(tuint8*)COMPACT_READER_PTR(self);
	self->offset += sizeof(tuint8);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_compact_read_tuint16(TLIBC_ABSTRACT_READER *super, tuint16 *val)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
	tuint16 res;
	tuint32 buff_size = COMPACT_READER_CAPACITY(self);
	TLIBC_ERROR_CODE ret = tlibc_compact_varint16_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	*val = tlibc_little_to_host16(res);
	self->offset += buff_size;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_read_tuint32(TLIBC_ABSTRACT_READER *super, tuint32 *val)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
	tuint32 res;
	tuint32 buff_size = COMPACT_READER_CAPACITY(self);
	TLIBC_ERROR_CODE ret = tlibc_compact_varint32_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	*val = tlibc_little_to_host32(res);
	self->offset += buff_size;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_read_tuint64(TLIBC_ABSTRACT_READER *super, tuint64 *val)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
	tuint64 res;
	tuint32 buff_size = COMPACT_READER_CAPACITY(self);
	TLIBC_ERROR_CODE ret = tlibc_compact_varint64_decode(COMPACT_READER_PTR(self), &buff_size, &res);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	*val = tlibc_little_to_host64(res);
	self->offset += buff_size;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_read_tchar(TLIBC_ABSTRACT_READER *super, char *val)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
	if(COMPACT_READER_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(char*)COMPACT_READER_PTR(self);
	self->offset += sizeof(char);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}


TLIBC_ERROR_CODE tlibc_compact_read_tdouble(TLIBC_ABSTRACT_READER *super, double *val)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
	if(COMPACT_READER_CAPACITY(self) < sizeof(double))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(double*)COMPACT_READER_PTR(self);
	self->offset += sizeof(double);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}


TLIBC_ERROR_CODE tlibc_compact_read_tstring(TLIBC_ABSTRACT_READER *super, tchar* str, tuint32 str_length)
{
	TLIBC_COMPACT_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_READER, super);
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

