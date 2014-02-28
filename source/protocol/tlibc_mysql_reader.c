#include "tlibc/protocol/tlibc_mysql_reader.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"


#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>



void tlibc_mysql_reader_init(tlibc_mysql_reader_t *self, const MYSQL_FIELD *mysql_field_vec, uint32_t mysql_field_vec_num)
{
	size_t i;

	tlibc_abstract_reader_init(&self->super);

	self->super.read_field_begin = tlibc_mysql_read_field_begin;
	self->super.read_vector_element_begin = tlibc_mysql_read_vector_element_begin;
	self->super.read_enum_begin = tlibc_mysql_read_enum_begin;

	self->super.read_int8 = tlibc_mysql_read_int8;
	self->super.read_int16 = tlibc_mysql_read_int16;
	self->super.read_int32 = tlibc_mysql_read_int32;
	self->super.read_int64 = tlibc_mysql_read_int64;

	self->super.read_uint8 = tlibc_mysql_read_uint8;
	self->super.read_uint16 = tlibc_mysql_read_uint16;
	self->super.read_uint32 = tlibc_mysql_read_uint32;
	self->super.read_uint64 = tlibc_mysql_read_uint64;

	self->super.read_double = tlibc_mysql_read_double;
	self->super.read_string = tlibc_mysql_read_string;
	self->super.read_char = tlibc_mysql_read_char;

	

	self->read_enum_name_once = FALSE;
	self->super.enable_name = TRUE;
	self->mysql_field_vec = mysql_field_vec;
	self->mysql_field_vec_num = mysql_field_vec_num;
	tlibc_hash_init(&self->name2field, self->hash_bucket, TLIBC_MYSQL_HASH_BUCKET);
	for(i = 0; i < self->mysql_field_vec_num; ++i)
	{
		self->field_vec[i].myfield = &self->mysql_field_vec[i];
		tlibc_hash_insert(&self->name2field, self->mysql_field_vec[i].name, self->mysql_field_vec[i].name_length
			, &self->field_vec[i].name2field);
	}
	self->cur_myfield = NULL;
}

void tlibc_mysql_reader_fetch(tlibc_mysql_reader_t *self, MYSQL_ROW row
										  , const unsigned long *length_vec)
{
	self->row = row;
	self->length_vec = length_vec;
	self->super.name[0] = 0;
	self->super.name_ptr = self->super.name;
}

TLIBC_ERROR_CODE tlibc_mysql_read_field_begin(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);
	const tlibc_hash_head_t* h = NULL;
	tlibc_mysql_field_t *field = NULL;
	size_t				idx;


	if(super->name_ptr <= super->name)
	{
		ret = E_TLIBC_ERROR;
		goto done;
	}
	h = tlibc_hash_find_const(&self->name2field, super->name + 1, super->name_ptr - super->name - 1);
	TLIBC_UNUSED(var_name);
	if(h == NULL)
	{
		ret = E_TLIBC_IGNORE;
		self->cur_myfield = NULL;
		goto done;
	}
	field = TLIBC_CONTAINER_OF(h, tlibc_mysql_field_t, name2field);

	self->cur_myfield = field->myfield;
	idx = field - self->field_vec;
	if(idx >= self->mysql_field_vec_num)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}

	self->cur_col = self->row[field - self->field_vec];
	self->cur_col_length = self->length_vec[field - self->field_vec];
	
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_vector_element_begin(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index)
{
	TLIBC_UNUSED(index);
	return tlibc_mysql_read_field_begin(self, var_name);
}

TLIBC_ERROR_CODE tlibc_mysql_read_enum_begin(TLIBC_ABSTRACT_READER *super, const char *enum_name)
{
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);
	TLIBC_UNUSED(enum_name);

	self->read_enum_name_once = TRUE;

	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_mysql_read_double(TLIBC_ABSTRACT_READER *super, double *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);

	if(self->cur_col == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}

	errno = 0;
	*val = strtod(self->cur_col, NULL);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto done;
	}
	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_int8(TLIBC_ABSTRACT_READER *super, int8_t *val)
{
	int64_t i64;
	TLIBC_ERROR_CODE ret = tlibc_mysql_read_int64(super, &i64);
	*val = (int8_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_int16(TLIBC_ABSTRACT_READER *super, int16_t *val)
{
	int64_t i64;
	TLIBC_ERROR_CODE ret = tlibc_mysql_read_int64(super, &i64);
	*val = (int16_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_int32(TLIBC_ABSTRACT_READER *super, int32_t *val)
{	
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);
	int64_t i64;

	if(self->read_enum_name_once)
	{
		self->read_enum_name_once = FALSE;
		ret = E_TLIBC_PLEASE_READ_ENUM_NAME;
		goto done;
	}

	ret = tlibc_mysql_read_int64(super, &i64);
	*val = (int32_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_int64(TLIBC_ABSTRACT_READER *super, int64_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);
	if(self->cur_col == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}

	errno = 0;
	*val = strtoll(self->cur_col, NULL, 10);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto done;
	}
	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_uint8(TLIBC_ABSTRACT_READER *super, uint8_t *val)
{
	uint64_t ui64;
	TLIBC_ERROR_CODE ret = tlibc_mysql_read_uint64(super, &ui64);
	*val = (uint8_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_uint16(TLIBC_ABSTRACT_READER *super, uint16_t *val)
{	
	uint64_t ui64;
	TLIBC_ERROR_CODE ret = tlibc_mysql_read_uint64(super, &ui64);
	*val = (uint16_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_uint32(TLIBC_ABSTRACT_READER *super, uint32_t *val)
{
	uint64_t ui64;
	TLIBC_ERROR_CODE ret = tlibc_mysql_read_uint64(super, &ui64);
	*val = (uint32_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_uint64(TLIBC_ABSTRACT_READER *super, uint64_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);
	if(self->cur_col == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}

	errno = 0;
	*val = strtoull(self->cur_col, NULL, 10);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto done;
	}
	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_char(TLIBC_ABSTRACT_READER *super, char *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);
	if(self->cur_col == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}

	if(self->cur_col_length <= 0)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	*val = *self->cur_col;
	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_string(TLIBC_ABSTRACT_READER *super, char *str, uint32_t str_len)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);
	if(self->cur_col == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}

	if(self->cur_col_length >= str_len)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	memcpy(str, self->cur_col, self->cur_col_length);
	str[self->cur_col_length] = 0;
	return E_TLIBC_NOERROR;
done:
	return ret;
}
