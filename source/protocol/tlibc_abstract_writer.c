#include "tlibc/protocol/tlibc_abstract_writer.h"
#include "tlibc/core/tlibc_error_code.h"

void tlibc_abstract_writer_init(TLIBC_ABSTRACT_WRITER *self)
{
	self->write_struct_begin = NULL;
	self->write_struct_end = NULL;
	self->write_union_begin = NULL;
	self->write_union_end = NULL;
	self->write_enum_begin = NULL;
	self->write_enum_end = NULL;
	self->write_vector_begin = NULL;
	self->write_vector_end = NULL;
	self->write_field_begin = NULL;
	self->write_field_end = NULL;
	self->write_vector_element_begin = NULL;
	self->write_vector_element_end = NULL;

	self->write_int8 = NULL;
	self->write_int16 = NULL;
	self->write_int32 = NULL;
	self->write_int64 = NULL;
	self->write_uint8 = NULL;
	self->write_uint16 = NULL;
	self->write_uint32 = NULL;
	self->write_uint64 = NULL;
	self->write_char = NULL;
	self->write_double = NULL;
	self->write_string = NULL;

}

TLIBC_ERROR_CODE tlibc_write_struct_begin(TLIBC_ABSTRACT_WRITER *self, const char *struct_name)
{
	if(self->write_struct_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_struct_begin(self, struct_name);
}

TLIBC_ERROR_CODE tlibc_write_struct_end(TLIBC_ABSTRACT_WRITER *self, const char *struct_name)
{
	if(self->write_struct_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_struct_end(self, struct_name);
}

TLIBC_ERROR_CODE tlibc_write_union_begin(TLIBC_ABSTRACT_WRITER *self, const char *union_name)
{
	if(self->write_union_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_union_begin(self, union_name);
}

TLIBC_ERROR_CODE tlibc_write_union_end(TLIBC_ABSTRACT_WRITER *self, const char *union_name)
{
	if(self->write_union_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_union_end(self, union_name);
}


TLIBC_ERROR_CODE tlibc_write_enum_begin(TLIBC_ABSTRACT_WRITER *self, const char *enum_name)
{
	if(self->write_enum_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_enum_begin(self, enum_name);
}

TLIBC_ERROR_CODE tlibc_write_enum_end(TLIBC_ABSTRACT_WRITER *self, const char *enum_name)
{
	if(self->write_enum_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_enum_end(self, enum_name);
}

TLIBC_ERROR_CODE tlibc_write_vector_begin(TLIBC_ABSTRACT_WRITER *self)
{
	if(self->write_vector_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_vector_begin(self);
}

TLIBC_ERROR_CODE tlibc_write_vector_end(TLIBC_ABSTRACT_WRITER *self)
{
	if(self->write_vector_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_vector_end(self);
}

TLIBC_ERROR_CODE tlibc_write_field_begin(TLIBC_ABSTRACT_WRITER *self, const char *var_name)
{
	if(self->write_field_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_field_begin(self, var_name);
}

TLIBC_ERROR_CODE tlibc_write_vector_element_begin(TLIBC_ABSTRACT_WRITER *self, const char *var_name, uint32_t index)
{
	if(self->write_vector_element_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_vector_element_begin(self, var_name, index);
}

TLIBC_ERROR_CODE tlibc_write_vector_element_end(TLIBC_ABSTRACT_WRITER *self, const char *var_name, uint32_t index)
{
	if(self->write_vector_element_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_vector_element_end(self, var_name, index);
}

TLIBC_ERROR_CODE tlibc_write_field_end(TLIBC_ABSTRACT_WRITER *self, const char *var_name)
{
	if(self->write_field_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_field_end(self, var_name);
}

TLIBC_ERROR_CODE tlibc_write_int8(TLIBC_ABSTRACT_WRITER *self, const int8_t *val)
{
	if(self->write_int8 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_int8(self, val);
}

TLIBC_ERROR_CODE tlibc_write_int16(TLIBC_ABSTRACT_WRITER *self, const int16_t *val)
{
	if(self->write_int16 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_int16(self, val);
}

TLIBC_ERROR_CODE tlibc_write_int32(TLIBC_ABSTRACT_WRITER *self, const int32_t *val)
{
	if(self->write_int32 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_int32(self, val);
}

TLIBC_ERROR_CODE tlibc_write_int64(TLIBC_ABSTRACT_WRITER *self, const int64_t *val)
{
	if(self->write_int64 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_int64(self, val);
}

TLIBC_ERROR_CODE tlibc_write_uint8(TLIBC_ABSTRACT_WRITER *self, const uint8_t *val)
{
	if(self->write_uint8 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_uint8(self, val);
}

TLIBC_ERROR_CODE tlibc_write_uint16(TLIBC_ABSTRACT_WRITER *self, const uint16_t *val)
{
	if(self->write_uint16 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_uint16(self, val);
}

TLIBC_ERROR_CODE tlibc_write_uint32(TLIBC_ABSTRACT_WRITER *self, const uint32_t *val)
{
	if(self->write_uint32 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_uint32(self, val);
}

TLIBC_ERROR_CODE tlibc_write_uint64(TLIBC_ABSTRACT_WRITER *self, const uint64_t *val)
{
	if(self->write_uint64 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_uint64(self, val);
}

TLIBC_ERROR_CODE tlibc_write_char(TLIBC_ABSTRACT_WRITER *self, const char *val)
{
	if(self->write_char == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_char(self, val);
}

TLIBC_ERROR_CODE tlibc_write_double(TLIBC_ABSTRACT_WRITER *self, const double *val)
{
	if(self->write_double == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_double(self, val);
}

TLIBC_ERROR_CODE tlibc_write_string(TLIBC_ABSTRACT_WRITER *self, const char* str, uint32_t str_length)
{
	if(self->write_string == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->write_string(self, str, str_length);
}
