#include "tlibc/protocol/tlibc_compact_writer.h"
#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_writer.h"
#include "tlibc/core/tlibc_error_code.h"

#include <string.h>
#include <assert.h>


static TLIBC_ERROR_CODE tlibc_compact_varint16_encode(tuint16 n, char *buff_ptr, tuint32 *buff_size)
{
	if(*buff_size < 1)
	{
		goto not_enough_byte_size;
	}
	buff_ptr[0] = (char)(n | 0x80);
	if (n >= (1 << 7))
	{
		if(*buff_size < 2)
		{
			goto not_enough_byte_size;
		}
		buff_ptr[1] = (char)((n >>  7) | 0x80);
		if (n >= (1 << 14))
		{
			if(*buff_size < 3)
			{
				goto not_enough_byte_size;
			}
			buff_ptr[2] = (char)(n >> 14);
			*buff_size = 3;
			goto done;
		}
		else
		{
			buff_ptr[1] &= 0x7F;
			*buff_size = 2;
			goto done;
		}
	}
	else
	{
		buff_ptr[0] &= 0x7F;
		*buff_size = 1;
		goto done;
	}

done:
	return E_TLIBC_NOERROR;

not_enough_byte_size:
	return E_TLIBC_OUT_OF_MEMORY;
}



static TLIBC_ERROR_CODE tlibc_compact_varint32_encode(tuint32 n, char *buff_ptr, tuint32 *buff_size)
{
	if(*buff_size < 1)
	{
		goto not_enough_byte_size;
	}
	buff_ptr[0] = (char)(n | 0x80);
	if (n >= (1 << 7))
	{
		if(*buff_size < 2)
		{
			goto not_enough_byte_size;
		}
		buff_ptr[1] = (char)((n >>  7) | 0x80);
		if (n >= (1 << 14))
		{
			if(*buff_size < 3)
			{
				goto not_enough_byte_size;
			}
			buff_ptr[2] = (char)((n >> 14) | 0x80);

			if (n >= (1 << 21))
			{
				if(*buff_size < 4)
				{
					goto not_enough_byte_size;
				}
				buff_ptr[3] = (char)((n >> 21) | 0x80);

				if (n >= (1 << 28))
				{
					if(*buff_size < 5)
					{
						goto not_enough_byte_size;
					}
					buff_ptr[4] = (char)(n >> 28);
					*buff_size = 5;
					goto done;
				}
				else
				{
					buff_ptr[3] &= 0x7F;
					*buff_size = 4;
					goto done;
				}
			}
			else
			{
				buff_ptr[2] &= 0x7F;
				*buff_size = 3;
				goto done;
			}
		}
		else
		{
			buff_ptr[1] &= 0x7F;
			*buff_size = 2;
			goto done;
		}
	}
	else
	{
		buff_ptr[0] &= 0x7F;
		*buff_size = 1;
		goto done;
	}

done:
	return E_TLIBC_NOERROR;

not_enough_byte_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

static TLIBC_ERROR_CODE tlibc_compact_varint64_encode(tuint64 n, char *buff_ptr, tuint32 *buff_size)
{
	tuint32 part0 = (tuint32)(n);
	tuint32 part1 = (tuint32)(n >> 28);
	tuint32 part2 = (tuint32)(n >> 56);

	tint32 size;

	if (part2 == 0)
	{
		if (part1 == 0)
		{
			if (part0 < (1 << 14))
			{
				if (part0 < (1 << 7))
				{
					size = 1;
					if(*buff_size < 1)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 1;

					goto size1;
				}
				else
				{
					size = 2;
					if(*buff_size < 2)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 2;

					goto size2;
				}
			}
			else
			{
				if (part0 < (1 << 21))
				{
					size = 3;
					if(*buff_size < 3)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 3;

					goto size3;
				}
				else
				{
					size = 4;
					if(*buff_size < 4)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 4;

					goto size4;
				}
			}
		}
		else
		{
			if (part1 < (1 << 14))
			{
				if (part1 < (1 << 7))
				{
					size = 5;
					if(*buff_size < 5)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 5;
					goto size5;
				}
				else
				{
					size = 6;
					if(*buff_size < 6)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 6;
					goto size6;
				}
			}
			else
			{
				if (part1 < (1 << 21))
				{
					size = 7;
					if(*buff_size < 7)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 7;
					goto size7;
				}
				else
				{
					size = 8;
					if(*buff_size < 8)
					{
						goto not_enough_byte_size;
					}
					*buff_size = 8;
					goto size8;
				}
			}
		}
	}
	else
	{
		if (part2 < (1 << 7))
		{
			size = 9;
			if(*buff_size < 9)
			{
				goto not_enough_byte_size;
			}
			*buff_size = 9;
			goto size9;
		}
		else
		{
			size = 10;
			if(*buff_size < 10)
			{
				goto not_enough_byte_size;
			}
			*buff_size = 10;
			goto size10;
		}
	}


size10:
	buff_ptr[9] = (char)((part2 >>  7) | 0x80);
size9:
	buff_ptr[8] = (char)((part2      ) | 0x80);
size8:
	buff_ptr[7] = (char)((part1 >> 21) | 0x80);
size7:
	buff_ptr[6] = (char)((part1 >> 14) | 0x80);
size6:
	buff_ptr[5] = (char)((part1 >>  7) | 0x80);
size5:
	buff_ptr[4] = (char)((part1      ) | 0x80);
size4:
	buff_ptr[3] = (char)((part0 >> 21) | 0x80);
size3:
	buff_ptr[2] = (char)((part0 >> 14) | 0x80);
size2:
	buff_ptr[1] = (char)((part0 >>  7) | 0x80);
size1:
	buff_ptr[0] = (char)((part0      ) | 0x80);

	buff_ptr[size-1] &= 0x7F;
	return E_TLIBC_NOERROR;

not_enough_byte_size:
	return E_TLIBC_OUT_OF_MEMORY;
}


void tlibc_compact_writer_init(TLIBC_COMPACT_WRITER *self, char *addr, tuint32 size)
{
	tlibc_abstract_writer_init(&self->super);

	self->super.write_tchar = tlibc_compact_write_tchar;
	self->super.write_tdouble = tlibc_compact_write_tdouble;
	self->super.write_tint8 = tlibc_compact_write_tint8;
	self->super.write_tint16 = tlibc_compact_write_tint16;
	self->super.write_tint32 = tlibc_compact_write_tint32;
	self->super.write_tint64 = tlibc_compact_write_tint64;
	self->super.write_tuint8 = tlibc_compact_write_tuint8;
	self->super.write_tuint16 = tlibc_compact_write_tuint16;
	self->super.write_tuint32 = tlibc_compact_write_tuint32;
	self->super.write_tuint64 = tlibc_compact_write_tuint64;
	self->super.write_tstring = tlibc_compact_write_tstring;

	self->addr = addr;
	self->size = size;
	self->offset = 0;
}

#define COMPACT_WRITER_CAPACITY(self) (self->size - self->offset)
#define COMPACT_WRITER_PTR(self) (self->addr + self->offset)

TLIBC_ERROR_CODE tlibc_compact_write_tint8(TLIBC_ABSTRACT_WRITER *super, const tint8 *val)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
	if(COMPACT_WRITER_CAPACITY(self) < sizeof(tint8))
	{
		goto not_enough_bytebuff_size;
	}
	*(tint8*)COMPACT_WRITER_PTR(self) = *val;
	self->offset += sizeof(tint8);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_compact_write_tint16(TLIBC_ABSTRACT_WRITER *super, const tint16 *val)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
	tuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	tint16 v = tlibc_zigzag_encode16(*val);
	TLIBC_ERROR_CODE ret;
	tlibc_host16_to_little(v);
	ret = tlibc_compact_varint16_encode(v, COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_write_tint32(TLIBC_ABSTRACT_WRITER *super, const tint32 *val)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
	tuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	tint32 v = tlibc_zigzag_encode16(*val);
	TLIBC_ERROR_CODE ret;
	tlibc_host32_to_little(v);
	ret = tlibc_compact_varint32_encode(v, COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_write_tint64(TLIBC_ABSTRACT_WRITER *super, const tint64 *val)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
	tuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	tint64 v = tlibc_zigzag_encode64(*val);
	TLIBC_ERROR_CODE ret;
	tlibc_host64_to_little(v);
	ret = tlibc_compact_varint64_encode(v, COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}


TLIBC_ERROR_CODE tlibc_compact_write_tuint8(TLIBC_ABSTRACT_WRITER *super, const tuint8 *val)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
	if(COMPACT_WRITER_CAPACITY(self) < sizeof(tuint8))
	{
		goto not_enough_bytebuff_size;
	}
	*(tuint8*)COMPACT_WRITER_PTR(self) = *val;
	self->offset += sizeof(tuint8);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_compact_write_tuint16(TLIBC_ABSTRACT_WRITER *super, const tuint16 *val)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
	tuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	tuint16 v = *val;
	TLIBC_ERROR_CODE ret;
	tlibc_host16_to_little(v);
	ret = tlibc_compact_varint16_encode(v, COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_write_tuint32(TLIBC_ABSTRACT_WRITER *super, const tuint32 *val)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
	tuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	tuint32 v = *val;
	TLIBC_ERROR_CODE ret;
	tlibc_host32_to_little(v);
	ret = tlibc_compact_varint32_encode(v, COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_write_tuint64(TLIBC_ABSTRACT_WRITER *super, const tuint64 *val)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
	tuint32 buff_size = COMPACT_WRITER_CAPACITY(self);
	tuint64 v = *val;
	TLIBC_ERROR_CODE ret;
	tlibc_host64_to_little(v);
	ret = tlibc_compact_varint64_encode(v, COMPACT_WRITER_PTR(self), &buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	self->offset += buff_size;

done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_compact_write_tchar(TLIBC_ABSTRACT_WRITER *super, const char *val)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
	if(COMPACT_WRITER_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*(char*)COMPACT_WRITER_PTR(self) = *val;
	self->offset += sizeof(char);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_compact_write_tdouble(TLIBC_ABSTRACT_WRITER *super, const double *val)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
	if(COMPACT_WRITER_CAPACITY(self) < sizeof(double))
	{
		goto not_enough_bytebuff_size;
	}
	*(double*)COMPACT_WRITER_PTR(self) = *val;
	self->offset += sizeof(double);

	return E_TLIBC_NOERROR;
not_enough_bytebuff_size:
	return E_TLIBC_OUT_OF_MEMORY;
}

TLIBC_ERROR_CODE tlibc_compact_write_tstring(TLIBC_ABSTRACT_WRITER *super, const tchar* str)
{
	TLIBC_COMPACT_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_COMPACT_WRITER, super);
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
