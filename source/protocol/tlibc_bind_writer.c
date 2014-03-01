#include "tlibc/protocol/tlibc_bind_writer.h"
#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_writer.h"
#include "tlibc/core/tlibc_error_code.h"

#include <string.h>
#include <assert.h>
#include <stdint.h>


void tlibc_bind_writer_init(tlibc_bind_writer_t *self, tlibc_bind_const_s *bind_vec, uint32_t bind_vec_num)
{
	tlibc_abstract_writer_init(&self->super);

	self->super.write_enum_begin = tlibc_bind_write_enum_begin;

	self->super.write_int8 = tlibc_bind_write_int8;
	self->super.write_int16 = tlibc_bind_write_int16;
	self->super.write_int32 = tlibc_bind_write_int32;
	self->super.write_int64 = tlibc_bind_write_int64;
	self->super.write_uint8 = tlibc_bind_write_uint8;
	self->super.write_uint16 = tlibc_bind_write_uint16;
	self->super.write_uint32 = tlibc_bind_write_uint32;
	self->super.write_uint64 = tlibc_bind_write_uint64;
	self->super.write_double = tlibc_bind_write_double;
	self->super.write_char = tlibc_bind_write_char;
	self->super.write_string = tlibc_bind_write_string;
	

	self->bind_vec = bind_vec;
	self->bind_vec_num = bind_vec_num;
	self->idx = 0;
	self->read_enum_name = FALSE;
}

TLIBC_ERROR_CODE tlibc_bind_write_enum_begin(TLIBC_ABSTRACT_WRITER *super, const char *enum_name)
{
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	TLIBC_UNUSED(enum_name);

	self->read_enum_name = TRUE;

	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_bind_write_int8(TLIBC_ABSTRACT_WRITER *super, const int8_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].type = e_tlibc_bind_int8;
	self->bind_vec[self->idx].buff = (const char*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_write_int16(TLIBC_ABSTRACT_WRITER *super, const int16_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].type = e_tlibc_bind_int16;
	self->bind_vec[self->idx].buff = (const char*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_write_int32(TLIBC_ABSTRACT_WRITER *super, const int32_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
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

	self->bind_vec[self->idx].type = e_tlibc_bind_int32;
	self->bind_vec[self->idx].buff = (const char*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_write_int64(TLIBC_ABSTRACT_WRITER *super, const int64_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].type = e_tlibc_bind_int64;
	self->bind_vec[self->idx].buff = (const char*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}


TLIBC_ERROR_CODE tlibc_bind_write_uint8(TLIBC_ABSTRACT_WRITER *super, const uint8_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].type = e_tlibc_bind_uint8;
	self->bind_vec[self->idx].buff = (const char*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_write_uint16(TLIBC_ABSTRACT_WRITER *super, const uint16_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].type = e_tlibc_bind_uint16;
	self->bind_vec[self->idx].buff = (const char*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_write_uint32(TLIBC_ABSTRACT_WRITER *super, const uint32_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].type = e_tlibc_bind_uint32;
	self->bind_vec[self->idx].buff = (const char*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_write_uint64(TLIBC_ABSTRACT_WRITER *super, const uint64_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].type = e_tlibc_bind_uint64;
	self->bind_vec[self->idx].buff = (const char*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_write_char(TLIBC_ABSTRACT_WRITER *super, const char *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].type = e_tlibc_bind_char;
	self->bind_vec[self->idx].buff = (const char*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_write_double(TLIBC_ABSTRACT_WRITER *super, const double *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].type = e_tlibc_bind_double;
	self->bind_vec[self->idx].buff = (const char*)val;
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_bind_write_string(TLIBC_ABSTRACT_WRITER *super, const char* str)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_bind_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_bind_writer_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].type = e_tlibc_bind_string;
	self->bind_vec[self->idx].buff = (const char*)str;
	self->bind_vec[self->idx].buff_size = strlen(str);
	++(self->idx);

	return E_TLIBC_NOERROR;
done:
	return ret;
}
