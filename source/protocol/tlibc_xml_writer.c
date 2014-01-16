#include "protocol/tlibc_xml_writer.h"
#include "lib/tlibc_code.h"
#include "protocol/tlibc_abstract_writer.h"
#include "lib/tlibc_error_code.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

TLIBC_ERROR_CODE xml_writer_init(TLIBC_XML_WRITER *self, const char *file_name)
{
	tlibc_abstract_writer_init(&self->super);

	self->f = fopen(file_name, "wb");
	if(self->f == NULL)
	{
		goto ERROR_RET;
	}

	self->count = 0;
	self->need_tab = hpfalse;

	self->super.write_struct_begin = xml_write_struct_begin;
	self->super.write_struct_end = xml_write_struct_end;
	self->super.write_enum_begin = xml_write_enum_begin;
	self->super.write_enum_end = xml_write_enum_end;

	self->super.write_vector_begin = xml_write_vector_begin;
	self->super.write_vector_end = xml_write_vector_end;
	self->super.write_field_begin = xml_write_field_begin;
	self->super.write_field_end = xml_write_field_end;
	
	self->super.write_tint8 = xml_write_tint8;
	self->super.write_tint16 = xml_write_tint16;
	self->super.write_tint32 = xml_write_tint32;
	self->super.write_tint64 = xml_write_tint64;

	self->super.write_tuint8 = xml_write_tuint8;
	self->super.write_tuint16 = xml_write_tuint16;
	self->super.write_tuint32 = xml_write_tuint32;
	self->super.write_tuint64 = xml_write_tuint64;


	self->super.write_tdouble = xml_write_tdouble;
	self->super.write_string = xml_write_tstring;
	self->super.write_tchar = xml_write_tchar;

	self->super.write_enum_name = xml_write_enum_name;

	self->skip_a_uint16_field = hpfalse;

	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;

}

TLIBC_API void xml_writer_fini(TLIBC_XML_WRITER *self)
{
	fclose(self->f);
}

static void printf_tab(TLIBC_XML_WRITER *self)
{
	tuint32 i;
	for(i = 0;i < self->count; ++i)
	{
		fputc('\t', self->f);
	}
}

tint32 xml_write_struct_begin(TLIBC_ABSTRACT_WRITER *super, const char *struct_name)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);	
	tuint32 i;
	tuint32 len;

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

tint32 xml_write_struct_end(TLIBC_ABSTRACT_WRITER *super, const char *struct_name)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);	
	tuint32 i;
	tuint32 len = 0;
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

tint32 xml_write_enum_begin(TLIBC_ABSTRACT_WRITER *super, const char *enum_name)
{
	return E_TLIBC_NOERROR;
}

tint32 xml_write_enum_end(TLIBC_ABSTRACT_WRITER *super, const char *enum_name)
{
	return E_TLIBC_NOERROR;
}

tint32 xml_write_vector_begin(TLIBC_ABSTRACT_WRITER *super)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);	
	xml_write_field_begin(super, "vector");
	++self->count;
	self->skip_a_uint16_field = hptrue;
	return E_TLIBC_NOERROR;
}

tint32 xml_write_vector_end(TLIBC_ABSTRACT_WRITER *super)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	--self->count;
	xml_write_field_end(super, "vector");
	return E_TLIBC_NOERROR;
}

tint32 xml_write_field_begin(TLIBC_ABSTRACT_WRITER *super, const char *var_name)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	const char *i;
	if(self->skip_a_uint16_field)
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

tint32 xml_write_field_end(TLIBC_ABSTRACT_WRITER *super, const char *var_name)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);	
	const char *i;
	if(self->skip_a_uint16_field)
	{
		self->skip_a_uint16_field = hpfalse;
		goto done;
	}

	if(self->need_tab)
	{
		fputc('\n', self->f);
		printf_tab(self);
	}
	--(self->count);
	self->need_tab = hptrue;
	fputc('<', self->f);
	fputc('/', self->f);
	for(i = var_name; *i; ++i)
	{
		fputc(*i, self->f);
	}
	fputc('>', self->f);
	//fputc('\n', self->f);
done:
	return E_TLIBC_NOERROR;
}

TLIBC_API tint32 xml_write_vector_item_begin(TLIBC_ABSTRACT_WRITER *super, tuint32 index)
{
	char str[1024];
	snprintf(str, 1024, "index_%d", index);
	return xml_write_field_begin(super, str);
}

TLIBC_API tint32 xml_write_vector_item_end(TLIBC_ABSTRACT_WRITER *super, tuint32 index)
{
	char str[1024];
	snprintf(str, 1024, "index_%d", index);
	return xml_write_field_end(super, str);
}

tint32 xml_write_tdouble(TLIBC_ABSTRACT_WRITER *super, const double *val)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	fprintf(self->f, "%lf", val);
	self->need_tab = hpfalse;
	return E_TLIBC_NOERROR;
}

tint32 xml_write_tint8(TLIBC_ABSTRACT_WRITER *super, const tint8 *val)
{
	tint64 v = *val;
	return xml_write_tint64(super, &v);
}

tint32 xml_write_tint16(TLIBC_ABSTRACT_WRITER *super, const tint16 *val)
{
	tint64 v = *val;
	return xml_write_tint64(super, &v);
}

tint32 xml_write_tint32(TLIBC_ABSTRACT_WRITER *super, const tint32 *val)
{
	tint64 v = *val;
	return xml_write_tint64(super, &v);
}

tint32 xml_write_tint64(TLIBC_ABSTRACT_WRITER *super, const tint64 *val)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	fprintf(self->f, "%lld", *val);
	self->need_tab = hpfalse;
	return E_TLIBC_NOERROR;
}


tint32 xml_write_tuint8(TLIBC_ABSTRACT_WRITER *super, const tuint8 *val)
{
	tuint64 v = *val;
	return xml_write_tuint64(super, &v);
}

tint32 xml_write_tuint16(TLIBC_ABSTRACT_WRITER *super, const tuint16 *val)
{
	tuint64 v = *val;
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);

	if(self->skip_a_uint16_field)
	{
		goto done;
	}

	return xml_write_tuint64(super, &v);
done:
	return E_TLIBC_NOERROR;
}

tint32 xml_write_tuint32(TLIBC_ABSTRACT_WRITER *super, const tuint32 *val)
{
	tuint64 v = *val;
	return xml_write_tuint64(super, &v);
}

tint32 xml_write_tuint64(TLIBC_ABSTRACT_WRITER *super, const tuint64 *val)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	fprintf(self->f, "%llu", *val);
	self->need_tab = hpfalse;
	return E_TLIBC_NOERROR;
}

static void write_char(FILE* fout, tchar c)
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

tint32 xml_write_tstring(TLIBC_ABSTRACT_WRITER *super, const tchar* str)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	const tchar *i;
	self->need_tab = hpfalse;
	for(i = str; *i ; ++i)
	{
		write_char(self->f, *i);
	}

	return E_TLIBC_NOERROR;
}
tint32 xml_write_enum_name(TLIBC_ABSTRACT_WRITER *self, const tchar *enum_name)
{
	return xml_write_tstring(self, enum_name);
}

tint32 xml_write_tchar(TLIBC_ABSTRACT_WRITER *super, const tchar *val)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);

	write_char(self->f, *val);
	return E_TLIBC_NOERROR;
}
