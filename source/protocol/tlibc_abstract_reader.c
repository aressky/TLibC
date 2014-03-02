#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/core/tlibc_error_code.h"

#include <stdint.h>
#include <stdio.h>

void tlibc_abstract_reader_init(TLIBC_ABSTRACT_READER *self)
{
	self->enable_name = FALSE;
	self->name[0] = 0;
	self->name_ptr = self->name;

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

	self->read_int8 = NULL;
	self->read_int16 = NULL;
	self->read_int32 = NULL;
	self->read_int64 = NULL;
	self->read_uint8 = NULL;
	self->read_uint16 = NULL;
	self->read_uint32 = NULL;
	self->read_uint64 = NULL;
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
	if(self->enable_name)
	{
		int len = snprintf(self->name_ptr, TLIBC_READER_NAME_LENGTH - (self->name_ptr - self->name)
				, ".%s", var_name);

		if(len < 0)
		{
			return E_TLIBC_ERROR;
		}
		
		self->name_ptr += len;
	}

	if(self->read_field_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_field_begin(self, var_name);
}

TLIBC_ERROR_CODE tlibc_read_field_end(TLIBC_ABSTRACT_READER *self, const char *var_name)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;

	if(self->read_field_end != NULL)
	{
		ret = self->read_field_end(self, var_name);
	}

	if(self->enable_name)
	{	
		char curr_name[TLIBC_READER_NAME_LENGTH];	
		int len = snprintf(curr_name, TLIBC_READER_NAME_LENGTH - (self->name_ptr - self->name)
				, ".%s", var_name);

		if(len < 0)
		{
			return E_TLIBC_ERROR;
		}
		self->name_ptr -= len;
	}

	return ret;
}

TLIBC_ERROR_CODE tlibc_read_vector_element_begin(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index)
{
	if(self->enable_name)
	{
		int len = snprintf(self->name_ptr, TLIBC_READER_NAME_LENGTH - (self->name_ptr - self->name)
				, ".%s[%u]", var_name, index);

		if(len < 0)
		{
			return E_TLIBC_ERROR;
		}
		self->name_ptr += len;
	}
	
	if(self->read_vector_element_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_vector_element_begin(self, var_name, index);
}

TLIBC_ERROR_CODE tlibc_read_vector_element_end(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;

	if(self->read_vector_element_end != NULL)
	{
		ret = self->read_vector_element_end(self, var_name, index);
	}	

	if(self->enable_name)
	{		
		char curr_name[TLIBC_READER_NAME_LENGTH];
		int len = snprintf(curr_name, TLIBC_READER_NAME_LENGTH, ".%s[%u]", var_name, index);
		
		if(len < 0)
		{
			return E_TLIBC_ERROR;
		}
		self->name_ptr -= len;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_read_int8(TLIBC_ABSTRACT_READER *self, int8_t *val)
{
	if(self->read_int8 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_int8(self, val);
}

TLIBC_ERROR_CODE tlibc_read_int16(TLIBC_ABSTRACT_READER *self, int16_t *val)
{
	if(self->read_int16 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_int16(self, val);
}

TLIBC_ERROR_CODE tlibc_read_int32(TLIBC_ABSTRACT_READER *self, int32_t *val)
{
	if(self->read_int32 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_int32(self, val);
}

TLIBC_ERROR_CODE tlibc_read_int64(TLIBC_ABSTRACT_READER *self, int64_t *val)
{
	if(self->read_int64 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_int64(self, val);
}

TLIBC_ERROR_CODE tlibc_read_uint8(TLIBC_ABSTRACT_READER *self, uint8_t *val)
{
	if(self->read_uint8 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_uint8(self, val);
}

TLIBC_ERROR_CODE tlibc_read_uint16(TLIBC_ABSTRACT_READER *self, uint16_t *val)
{
	if(self->read_uint16 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_uint16(self, val);
}

TLIBC_ERROR_CODE tlibc_read_uint32(TLIBC_ABSTRACT_READER *self, uint32_t *val)
{
	if(self->read_uint32 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_uint32(self, val);
}

TLIBC_ERROR_CODE tlibc_read_uint64(TLIBC_ABSTRACT_READER *self, uint64_t *val)
{
	if(self->read_uint64 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_uint64(self, val);
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

