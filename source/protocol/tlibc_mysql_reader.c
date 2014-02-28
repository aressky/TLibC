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

	self->super.read_struct_begin = tlibc_mysql_read_struct_begin;
	self->super.read_struct_end = tlibc_mysql_read_struct_end;
	self->super.read_enum_begin = tlibc_mysql_read_enum_begin;

	self->super.read_vector_begin = tlibc_mysql_read_vector_begin;
	self->super.read_vector_end = tlibc_mysql_read_vector_end;
	self->super.read_field_begin = tlibc_mysql_read_field_begin;
	self->super.read_field_end = tlibc_mysql_read_field_end;
	self->super.read_vector_element_begin = tlibc_mysql_read_vector_element_begin;
	self->super.read_vector_element_end = tlibc_mysql_read_vector_element_end;

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

	self->mysql_field_vec = mysql_field_vec;
	self->mysql_field_vec_num = mysql_field_vec_num;
	tlibc_hash_init(&self->name2field, self->hash_bucket, TLIBC_MYSQL_HASH_BUCKET);
	for(i = 0; i < self->mysql_field_vec_num; ++i)
	{
		tlibc_hash_insert(&self->name2field, self->mysql_field_vec[i].org_name, self->mysql_field_vec[i].org_name_length
			, &self->field_vec[i].name2field);
	}
}

void tlibc_mysql_reader_fetch(tlibc_mysql_reader_t *self, MYSQL_ROW row
										  , const unsigned long *length_vec)
{
	self->row = row;
	self->length_vec = length_vec;
}

TLIBC_ERROR_CODE tlibc_mysql_read_struct_begin(TLIBC_ABSTRACT_READER *super, const char *struct_name)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_struct_end(TLIBC_ABSTRACT_READER *super, const char *struct_name)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_enum_begin(TLIBC_ABSTRACT_READER *super, const char *enum_name)
{
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);

	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_mysql_read_vector_begin(TLIBC_ABSTRACT_READER *super)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);
	
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_vector_end(TLIBC_ABSTRACT_READER *super)
{
	return tlibc_mysql_read_field_end(super, "vector");
}

TLIBC_ERROR_CODE tlibc_mysql_read_field_begin(TLIBC_ABSTRACT_READER *super, const char *var_name)
{	
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);

	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_field_end(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);	

	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_vector_element_begin(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index)
{
	TLIBC_UNUSED(index);
	return tlibc_mysql_read_field_begin(self, var_name);
}

TLIBC_ERROR_CODE tlibc_mysql_read_vector_element_end(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index)
{
	TLIBC_UNUSED(index);
	return tlibc_mysql_read_field_end(self, var_name);
}

TLIBC_ERROR_CODE tlibc_mysql_read_double(TLIBC_ABSTRACT_READER *super, double *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);

	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_int8(TLIBC_ABSTRACT_READER *super, int8_t *val)
{	
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_mysql_read_int16(TLIBC_ABSTRACT_READER *super, int16_t *val)
{
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_mysql_read_int32(TLIBC_ABSTRACT_READER *super, int32_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);

	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_int64(TLIBC_ABSTRACT_READER *super, int64_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);

	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_uint8(TLIBC_ABSTRACT_READER *super, uint8_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_uint16(TLIBC_ABSTRACT_READER *super, uint16_t *val)
{	
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_uint32(TLIBC_ABSTRACT_READER *super, uint32_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_uint64(TLIBC_ABSTRACT_READER *super, uint64_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	return ret;
}

TLIBC_ERROR_CODE tlibc_mysql_read_char(TLIBC_ABSTRACT_READER *super, char *val)
{
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);
	
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_mysql_read_string(TLIBC_ABSTRACT_READER *super, char *str, uint32_t str_len)
{
	tlibc_mysql_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_mysql_reader_t, super);
	
	return E_TLIBC_NOERROR;
}
