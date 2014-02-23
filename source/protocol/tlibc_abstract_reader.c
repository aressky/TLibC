#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

#include <stdint.h>

void tlibc_abstract_reader_init(TLIBC_ABSTRACT_READER *self)
{
	self->read_struct_begin = NULL;
	self->read_struct_end = NULL;
	self->read_enum_begin = NULL;
	self->read_enum_end = NULL;
	self->read_union_begin = NULL;
	self->read_union_end = NULL;
	self->read_vector_begin = NULL;
	self->read_vector_end = NULL;
	self->read_field_begin = NULL;
	self->read_field_end = NULL;
	self->read_vector_element_begin = NULL;
	self->read_vector_element_end = NULL;

	self->read_int8_t = NULL;
	self->read_int16_t = NULL;
	self->read_int32_t = NULL;
	self->read_int64_t = NULL;
	self->read_uint8_t = NULL;
	self->read_uint16_t = NULL;
	self->read_uint32_t = NULL;
	self->read_uint64_t = NULL;
	self->read_char = NULL;
	self->read_double = NULL;
	self->read_string = NULL;
}

TLIBC_ERROR_CODE tlibc_read_struct_begin(TLIBC_ABSTRACT_READER *self, const char *struct_name)
{
	if(self->read_struct_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_struct_begin(self, struct_name);
}

TLIBC_ERROR_CODE tlibc_read_struct_end(TLIBC_ABSTRACT_READER *self, const char *struct_name)
{
	if(self->read_struct_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_struct_end(self, struct_name);
}


TLIBC_ERROR_CODE tlibc_read_union_begin(TLIBC_ABSTRACT_READER *self, const char *union_name)
{
	if(self->read_union_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_union_begin(self, union_name);
}

TLIBC_ERROR_CODE tlibc_read_union_end(TLIBC_ABSTRACT_READER *self, const char *union_name)
{
	if(self->read_union_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_union_end(self, union_name);
}


TLIBC_ERROR_CODE tlibc_read_enum_begin(TLIBC_ABSTRACT_READER *self, const char *enum_name)
{
	if(self->read_enum_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_enum_begin(self, enum_name);
}

TLIBC_ERROR_CODE tlibc_read_enum_end(TLIBC_ABSTRACT_READER *self, const char *enum_name)
{
	if(self->read_enum_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_enum_end(self, enum_name);
}



TLIBC_ERROR_CODE tlibc_read_vector_begin(TLIBC_ABSTRACT_READER *self)
{
	if(self->read_vector_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_vector_begin(self);
}

TLIBC_ERROR_CODE tlibc_read_vector_end(TLIBC_ABSTRACT_READER *self)
{
	if(self->read_vector_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_vector_end(self);
}

TLIBC_ERROR_CODE tlibc_read_field_begin(TLIBC_ABSTRACT_READER *self, const char *var_name)
{
	if(self->read_field_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_field_begin(self, var_name);
}

TLIBC_ERROR_CODE tlibc_read_field_end(TLIBC_ABSTRACT_READER *self, const char *var_name)
{
	if(self->read_field_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_field_end(self, var_name);
}

TLIBC_ERROR_CODE tlibc_read_vector_element_begin(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index)
{
	if(self->read_vector_element_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_vector_element_begin(self, var_name, index);
}

TLIBC_ERROR_CODE tlibc_read_vector_element_end(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index)
{
	if(self->read_vector_element_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_vector_element_end(self, var_name, index);
}

TLIBC_ERROR_CODE tlibc_read_int8_t(TLIBC_ABSTRACT_READER *self, int8_t *val)
{
	if(self->read_int8_t == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_int8_t(self, val);
}

TLIBC_ERROR_CODE tlibc_read_int16_t(TLIBC_ABSTRACT_READER *self, int16_t *val)
{
	if(self->read_int16_t == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_int16_t(self, val);
}

TLIBC_ERROR_CODE tlibc_read_int32_t(TLIBC_ABSTRACT_READER *self, int32_t *val)
{
	if(self->read_int32_t == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_int32_t(self, val);
}

TLIBC_ERROR_CODE tlibc_read_int64_t(TLIBC_ABSTRACT_READER *self, int64_t *val)
{
	if(self->read_int64_t == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_int64_t(self, val);
}

TLIBC_ERROR_CODE tlibc_read_uint8_t(TLIBC_ABSTRACT_READER *self, uint8_t *val)
{
	if(self->read_uint8_t == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_uint8_t(self, val);
}

TLIBC_ERROR_CODE tlibc_read_uint16_t(TLIBC_ABSTRACT_READER *self, uint16_t *val)
{
	if(self->read_uint16_t == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_uint16_t(self, val);
}

TLIBC_ERROR_CODE tlibc_read_uint32_t(TLIBC_ABSTRACT_READER *self, uint32_t *val)
{
	if(self->read_uint32_t == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_uint32_t(self, val);
}

TLIBC_ERROR_CODE tlibc_read_uint64_t(TLIBC_ABSTRACT_READER *self, uint64_t *val)
{
	if(self->read_uint64_t == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_uint64_t(self, val);
}

TLIBC_ERROR_CODE tlibc_read_char(TLIBC_ABSTRACT_READER *self, char *val)
{
	if(self->read_char == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_char(self, val);
}

TLIBC_ERROR_CODE tlibc_read_double(TLIBC_ABSTRACT_READER *self, double *val)
{
	if(self->read_double == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_double(self, val);
}

TLIBC_ERROR_CODE tlibc_read_string(TLIBC_ABSTRACT_READER *self, char* str, uint32_t str_length)
{
	if(self->read_string == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_string(self, str, str_length);
}
