#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

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

	self->read_tint8 = NULL;
	self->read_tint16 = NULL;
	self->read_tint32 = NULL;
	self->read_tint64 = NULL;
	self->read_tuint8 = NULL;
	self->read_tuint16 = NULL;
	self->read_tuint32 = NULL;
	self->read_tuint64 = NULL;
	self->read_tchar = NULL;
	self->read_tdouble = NULL;
	self->read_tstring = NULL;

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

TLIBC_ERROR_CODE tlibc_read_vector_element_begin(TLIBC_ABSTRACT_READER *self, tuint32 index)
{
	if(self->read_vector_element_begin == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_vector_element_begin(self, index);
}

TLIBC_ERROR_CODE tlibc_read_vector_element_end(TLIBC_ABSTRACT_READER *self, tuint32 index)
{
	if(self->read_vector_element_end == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_vector_element_end(self, index);
}

TLIBC_ERROR_CODE tlibc_read_tint8(TLIBC_ABSTRACT_READER *self, tint8 *val)
{
	if(self->read_tint8 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tint8(self, val);
}

TLIBC_ERROR_CODE tlibc_read_tint16(TLIBC_ABSTRACT_READER *self, tint16 *val)
{
	if(self->read_tint16 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tint16(self, val);
}

TLIBC_ERROR_CODE tlibc_read_tint32(TLIBC_ABSTRACT_READER *self, tint32 *val)
{
	if(self->read_tint32 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tint32(self, val);
}

TLIBC_ERROR_CODE tlibc_read_tint64(TLIBC_ABSTRACT_READER *self, tint64 *val)
{
	if(self->read_tint64 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tint64(self, val);
}

TLIBC_ERROR_CODE tlibc_read_tuint8(TLIBC_ABSTRACT_READER *self, tuint8 *val)
{
	if(self->read_tuint8 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tuint8(self, val);
}

TLIBC_ERROR_CODE tlibc_read_tuint16(TLIBC_ABSTRACT_READER *self, tuint16 *val)
{
	if(self->read_tuint16 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tuint16(self, val);
}

TLIBC_ERROR_CODE tlibc_read_tuint32(TLIBC_ABSTRACT_READER *self, tuint32 *val)
{
	if(self->read_tuint32 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tuint32(self, val);
}

TLIBC_ERROR_CODE tlibc_read_tuint64(TLIBC_ABSTRACT_READER *self, tuint64 *val)
{
	if(self->read_tuint64 == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tuint64(self, val);
}

TLIBC_ERROR_CODE tlibc_read_tchar(TLIBC_ABSTRACT_READER *self, tchar *val)
{
	if(self->read_tchar == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tchar(self, val);
}

TLIBC_ERROR_CODE tlibc_read_tdouble(TLIBC_ABSTRACT_READER *self, tdouble *val)
{
	if(self->read_tdouble == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tdouble(self, val);
}

TLIBC_ERROR_CODE tlibc_read_tstring(TLIBC_ABSTRACT_READER *self, tchar* str, tuint32 str_length)
{
	if(self->read_tstring == NULL)
	{
		return E_TLIBC_NOERROR;
	}
	return self->read_tstring(self, str, str_length);
}
