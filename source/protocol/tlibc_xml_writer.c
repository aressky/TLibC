#include "protocol/tlibc_xml_writer.h"
#include "core/tlibc_error_code.h"
#include "core/tlibc_util.h"
#include "protocol/tlibc_abstract_writer.h"


#include <string.h>
#include <assert.h>
#include <stdio.h>


tlibc_error_code_t tlibc_xml_writer_init(tlibc_xml_writer_t *self, const char *file_name)
{
	tlibc_error_code_t ret;

	tlibc_abstract_writer_init(&self->super);

	self->f = fopen(file_name, "wb");
	if(self->f == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto ERROR_RET;
	}

	self->count = 0;
	self->need_tab = FALSE;

	self->super.write_struct_begin = tlibc_xml_write_struct_begin;
	self->super.write_struct_end = tlibc_xml_write_struct_end;
	self->super.write_enum_begin = tlibc_xml_write_enum_begin;

	self->super.write_vector_begin = tlibc_xml_write_vector_begin;
	self->super.write_vector_end = tlibc_xml_write_vector_end;
	self->super.write_field_begin = tlibc_xml_write_field_begin;
	self->super.write_field_end = tlibc_xml_write_field_end;
	self->super.write_vector_element_begin = tlibc_xml_write_vector_element_begin;
	self->super.write_vector_element_end = tlibc_xml_write_vector_element_end;
	
	self->super.write_int8 = tlibc_xml_write_int8;
	self->super.write_int16 = tlibc_xml_write_int16;
	self->super.write_int32 = tlibc_xml_write_int32;
	self->super.write_int64 = tlibc_xml_write_int64;

	self->super.write_uint8 = tlibc_xml_write_uint8;
	self->super.write_uint16 = tlibc_xml_write_uint16;
	self->super.write_uint32 = tlibc_xml_write_uint32;
	self->super.write_uint64 = tlibc_xml_write_uint64;


	self->super.write_double = tlibc_xml_write_double;
	self->super.write_string = tlibc_xml_write_string;
	self->super.write_char = tlibc_xml_write_char;


	self->skip_uint32_field_once = FALSE;
	self->ignore_int32_once = FALSE;

	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;

}

 void tlibc_xml_writer_fini(tlibc_xml_writer_t *self)
{
	fclose(self->f);
}

static void printf_tab(tlibc_xml_writer_t *self)
{
	uint32_t i;
	for(i = 0;i < self->count; ++i)
	{
		fputc('\t', self->f);
	}
}

tlibc_error_code_t tlibc_xml_write_struct_begin(tlibc_abstract_writer_t *super, const char *struct_name)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);	
	uint32_t i;
	uint32_t len;

	if(self->count == 0)
	{
		len = strlen(struct_name);
		fputc('<', self->f);
		for(i = 0;i < len; ++i)
		{
			fputc(struct_name[i], self->f);
		}
		fputc('>', self->f);
	}
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xml_write_struct_end(tlibc_abstract_writer_t *super, const char *struct_name)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);	
	uint32_t i;
	uint32_t len = 0;
	if(self->count == 0)
	{
		len = strlen(struct_name);

		fputc('\n', self->f);
		fputc('<', self->f);
		fputc('/', self->f);
		for(i = 0;i < len; ++i)
		{
			fputc(struct_name[i], self->f);
		}
		fputc('>', self->f);
	}
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xml_write_enum_begin(tlibc_abstract_writer_t *super, const char *enum_name)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);
	TLIBC_UNUSED(enum_name);
	self->ignore_int32_once = TRUE;
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xml_write_vector_begin(tlibc_abstract_writer_t *super)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);	
	tlibc_error_code_t ret = tlibc_xml_write_field_begin(super, "vector");
	self->skip_uint32_field_once = TRUE;
	return ret;
}

tlibc_error_code_t tlibc_xml_write_vector_end(tlibc_abstract_writer_t *super)
{
	return tlibc_xml_write_field_end(super, "vector");
}

tlibc_error_code_t tlibc_xml_write_field_begin(tlibc_abstract_writer_t *super, const char *var_name)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);
	const char *i;
	if(self->skip_uint32_field_once)
	{
		goto done;
	}

	fputc('\n', self->f);	
	++(self->count);
	printf_tab(self);
	
	fputc('<', self->f);
	for(i = var_name;*i; ++i)
	{
		fputc(*i, self->f);
	}
	fputc('>', self->f);
done:
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xml_write_field_end(tlibc_abstract_writer_t *super, const char *var_name)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);	
	const char *i;
	if(self->skip_uint32_field_once)
	{
		self->skip_uint32_field_once = FALSE;
		goto done;
	}

	if(self->need_tab)
	{
		fputc('\n', self->f);
		printf_tab(self);
	}
	--(self->count);
	self->need_tab = TRUE;
	fputc('<', self->f);
	fputc('/', self->f);
	for(i = var_name; *i; ++i)
	{
		fputc(*i, self->f);
	}
	fputc('>', self->f);

done:
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xml_write_vector_element_begin(tlibc_abstract_writer_t *super, const char *var_name, uint32_t index)
{
	TLIBC_UNUSED(index);
	return tlibc_xml_write_field_begin(super, var_name);
}

tlibc_error_code_t tlibc_xml_write_vector_element_end(tlibc_abstract_writer_t *super, const char *var_name, uint32_t index)
{
	TLIBC_UNUSED(index);
	return tlibc_xml_write_field_end(super, var_name);
}

tlibc_error_code_t tlibc_xml_write_double(tlibc_abstract_writer_t *super, const double *val)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);
	fprintf(self->f, "%lf", *val);
	self->need_tab = FALSE;
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xml_write_int8(tlibc_abstract_writer_t *super, const int8_t *val)
{
	int64_t v = *val;
	return tlibc_xml_write_int64(super, &v);
}

tlibc_error_code_t tlibc_xml_write_int16(tlibc_abstract_writer_t *super, const int16_t *val)
{
	int64_t v = *val;
	return tlibc_xml_write_int64(super, &v);
}

tlibc_error_code_t tlibc_xml_write_int32(tlibc_abstract_writer_t *super, const int32_t *val)
{
	int64_t v;
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);
	if(self->ignore_int32_once)
	{
		self->ignore_int32_once = FALSE;
		ret = E_TLIBC_PLEASE_READ_ENUM_NAME;
		goto done;
	}
	v = *val;
	ret = tlibc_xml_write_int64(super, &v);
done:
	return ret;
}

tlibc_error_code_t tlibc_xml_write_int64(tlibc_abstract_writer_t *super, const int64_t *val)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);
	fprintf(self->f, "%"PRIi64, *val);
	self->need_tab = FALSE;
	return E_TLIBC_NOERROR;
}


tlibc_error_code_t tlibc_xml_write_uint8(tlibc_abstract_writer_t *super, const uint8_t *val)
{
	uint64_t v = *val;
	return tlibc_xml_write_uint64(super, &v);
}

tlibc_error_code_t tlibc_xml_write_uint16(tlibc_abstract_writer_t *super, const uint16_t *val)
{
	uint64_t v = *val;
	
	return tlibc_xml_write_uint64(super, &v);
}

tlibc_error_code_t tlibc_xml_write_uint32(tlibc_abstract_writer_t *super, const uint32_t *val)
{
	uint64_t v = *val;
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);
	if(self->skip_uint32_field_once)
	{
		goto done;
	}
	return tlibc_xml_write_uint64(super, &v);
done:
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xml_write_uint64(tlibc_abstract_writer_t *super, const uint64_t *val)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);
	fprintf(self->f, "%"PRIu64, *val);
	self->need_tab = FALSE;
	return E_TLIBC_NOERROR;
}

static void write_char(FILE* fout, char c)
{
	switch (c)
	{
	case '<':
		fprintf(fout, "&lt");
		break;
	case '>':
		fprintf(fout, "&gt");
		break;
	case '&':
		fprintf(fout, "&amp");
		break;
	case '\'':
		fprintf(fout, "&apos");
		break;
	case '\"':
		fprintf(fout, "&quot");
		break;
	default:
		{
			fputc(c, fout);
		}
	}
}

tlibc_error_code_t tlibc_xml_write_string(tlibc_abstract_writer_t *super, const char* str, uint32_t str_length)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);
	const char *i;
	TLIBC_UNUSED(str_length);

	self->need_tab = FALSE;
	for(i = str; *i ; ++i)
	{
		write_char(self->f, *i);
	}

	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xml_write_char(tlibc_abstract_writer_t *super, const char *val)
{
	tlibc_xml_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_xml_writer_t, super);

	write_char(self->f, *val);
	return E_TLIBC_NOERROR;
}
