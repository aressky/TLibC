#include "tlibc/protocol/tlibc_bind_reader.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"


#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>


void tlibc_bind_reader_init(tlibc_bind_reader_t *self, MYSQL_BIND *bind_vec, uint32_t bind_vec_num)
{
	tlibc_abstract_reader_init(&self->super);

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

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_TINY;
	self->bind_vec[self->idx].buffer = (void*)val;
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

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_SHORT;
	self->bind_vec[self->idx].buffer = (void*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_int32(TLIBC_ABSTRACT_READER *super, int32_t *val)
{	
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);

	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_LONG;
	self->bind_vec[self->idx].buffer = (void*)val;
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

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_LONGLONG;
	self->bind_vec[self->idx].buffer = (void*)val;

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
	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_TINY;
	self->bind_vec[self->idx].buffer = (void*)val;
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

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_SHORT;
	self->bind_vec[self->idx].buffer = (void*)val;
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

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_LONG;
	self->bind_vec[self->idx].buffer = (void*)val;
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

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_LONGLONG;
	self->bind_vec[self->idx].buffer = (void*)val;
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

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_DOUBLE;
	self->bind_vec[self->idx].buffer = (void*)val;
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

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_STRING;
	self->bind_vec[self->idx].buffer = (void*)val;
	self->bind_vec[self->idx].buffer_length = 1;

	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_read_string(TLIBC_ABSTRACT_READER *super, char *str, uint32_t str_len)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_reader_t, super);
	TLIBC_UNUSED(str_len);

	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}


	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_STRING;
	self->bind_vec[self->idx].buffer = (void*)str;	
	self->bind_vec[self->idx].buffer_length = strlen(str) + 1;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}
