#include "tlibc/protocol/tlibc_bind_reader.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"


#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>


void tlibc_bind_reader_init(tlibc_bind_reader_t *self, tlibc_bind_s *bind_vec, uint32_t bind_vec_num)
{
	tlibc_abstract_reader_init(&self->super);

	self->super.read_enum_begin = tlibc_bind_read_enum_begin;

	self->super.read_int8 = tlibc_bind_read_int8;
	self->super.read_int16 = tlibc_bind_read_int16;
	self->super.read_int32 = tlibc_bind_read_int32;
	self->super.read_int64 = tlibc_bind_read_int64;

	self->super.read_uint8 = tlibc_bind_read_uint8;
	self->super.read_uint16 = tlibc_bind_read_uint16;
	self->super.read_uint32 = tlibc_bind_read_uint32;
	self->super.read_uint64 = tlibc_bind_read_uint64;

	self->super.read_double = tlibc_bind_read_double;
	self->super.read_char = tlibc_bind_read_char;
	self->super.read_string = tlibc_bind_read_string;

	self->bind_vec = bind_vec;
	self->bind_vec_num = bind_vec_num;
	self->idx = 0;
	self->read_enum_name = FALSE;
}

TLIBC_ERROR_CODE tlibc_bind_read_enum_begin(TLIBC_ABSTRACT_READER *super, const char *enum_name)
{
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	TLIBC_UNUSED(enum_name);

	self->read_enum_name = TRUE;

	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_bind_read_int8(TLIBC_ABSTRACT_READER *super, int8_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_int8)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	*val = *(int8_t*)self->bind_vec[self->idx].buff;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_int16(TLIBC_ABSTRACT_READER *super, int16_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_int16)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	*val = *(int16_t*)self->bind_vec[self->idx].buff;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_int32(TLIBC_ABSTRACT_READER *super, int32_t *val)
{	
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);

	if(self->read_enum_name)
	{
		ret = E_TLIBC_PLEASE_READ_ENUM_NAME;
		goto done;
	}

	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_int32)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	*val = *(int32_t*)self->bind_vec[self->idx].buff;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_int64(TLIBC_ABSTRACT_READER *super, int64_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_int64)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	*val = *(int64_t*)self->bind_vec[self->idx].buff;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_uint8(TLIBC_ABSTRACT_READER *super, uint8_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_uint8)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	*val = *(uint8_t*)self->bind_vec[self->idx].buff;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_uint16(TLIBC_ABSTRACT_READER *super, uint16_t *val)
{	
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_uint16)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	*val = *(uint16_t*)self->bind_vec[self->idx].buff;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_uint32(TLIBC_ABSTRACT_READER *super, uint32_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_uint32)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	*val = *(uint32_t*)self->bind_vec[self->idx].buff;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_uint64(TLIBC_ABSTRACT_READER *super, uint64_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_uint64)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	*val = *(uint64_t*)self->bind_vec[self->idx].buff;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}


TLIBC_ERROR_CODE tlibc_bind_read_double(TLIBC_ABSTRACT_READER *super, double *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_double)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	*val = *(double*)self->bind_vec[self->idx].buff;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_char(TLIBC_ABSTRACT_READER *super, char *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_char)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	*val = *(char*)self->bind_vec[self->idx].buff;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_string(TLIBC_ABSTRACT_READER *super, char *str, uint32_t str_len)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	if(self->bind_vec[self->idx].type != e_tlibc_bind_string)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}
	if(self->bind_vec[self->idx].buff_size <= str_len)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	memcpy(str, self->bind_vec[self->idx].buff, self->bind_vec[self->idx].buff_size);
	str[self->bind_vec[self->idx].buff_size] = 0;

	return E_TLIBC_NOERROR;
done:
	return ret;
}
