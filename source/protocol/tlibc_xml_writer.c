#include "protocol/tlibc_xml_writer.h"
#include "core/tlibc_util.h"
#include "protocol/tlibc_abstract_writer.h"
#include "core/tlibc_error_code.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

TLIBC_ERROR_CODE tlibc_xml_writer_init(TLIBC_XML_WRITER *self, const char *file_name)
{
	TLIBC_ERROR_CODE ret;

	tlibc_abstract_writer_init(&self->super);

	self->f = fopen(file_name, "wb");
	if(self->f == NULL)
	{
		ret = E_TLIBC_CAN_NOT_OPEN_FILE;
		goto ERROR_RET;
	}

	self->count = 0;
	self->need_tab = hpfalse;

	self->super.write_struct_begin = tlibc_xml_write_struct_begin;
	self->super.write_struct_end = tlibc_xml_write_struct_end;
	self->super.write_enum_begin = tlibc_xml_write_enum_begin;

	self->super.write_vector_begin = tlibc_xml_write_vector_begin;
	self->super.write_vector_end = tlibc_xml_write_vector_end;
	self->super.write_field_begin = tlibc_xml_write_field_begin;
	self->super.write_field_end = tlibc_xml_write_field_end;
	
	self->super.write_tint8 = tlibc_xml_write_tint8;
	self->super.write_tint16 = tlibc_xml_write_tint16;
	self->super.write_tint32 = tlibc_xml_write_tint32;
	self->super.write_tint64 = tlibc_xml_write_tint64;

	self->super.write_tuint8 = tlibc_xml_write_tuint8;
	self->super.write_tuint16 = tlibc_xml_write_tuint16;
	self->super.write_tuint32 = tlibc_xml_write_tuint32;
	self->super.write_tuint64 = tlibc_xml_write_tuint64;


	self->super.write_tdouble = tlibc_xml_write_tdouble;
	self->super.write_tstring = tlibc_xml_write_tstring;
	self->super.write_tchar = tlibc_xml_write_tchar;


	self->skip_uint16_field_once = hpfalse;
	self->ignore_int32_once = hpfalse;

	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;

}

TLIBC_API void tlibc_xml_writer_fini(TLIBC_XML_WRITER *self)
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

TLIBC_ERROR_CODE tlibc_xml_write_struct_begin(TLIBC_ABSTRACT_WRITER *super, const char *struct_name)
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

TLIBC_ERROR_CODE tlibc_xml_write_struct_end(TLIBC_ABSTRACT_WRITER *super, const char *struct_name)
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

TLIBC_ERROR_CODE tlibc_xml_write_enum_begin(TLIBC_ABSTRACT_WRITER *super, const char *enum_name)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	TLIBC_UNUSED(enum_name);
	self->ignore_int32_once = hptrue;
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_xml_write_vector_begin(TLIBC_ABSTRACT_WRITER *super)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);	
	TLIBC_ERROR_CODE ret = tlibc_xml_write_field_begin(super, "vector");
	self->skip_uint16_field_once = hptrue;
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_write_vector_end(TLIBC_ABSTRACT_WRITER *super)
{
	return tlibc_xml_write_field_end(super, "vector");
}

TLIBC_ERROR_CODE tlibc_xml_write_field_begin(TLIBC_ABSTRACT_WRITER *super, const char *var_name)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	const char *i;
	if(self->skip_uint16_field_once)
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

TLIBC_ERROR_CODE tlibc_xml_write_field_end(TLIBC_ABSTRACT_WRITER *super, const char *var_name)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);	
	const char *i;
	if(self->skip_uint16_field_once)
	{
		self->skip_uint16_field_once = hpfalse;
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

done:
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_xml_write_tdouble(TLIBC_ABSTRACT_WRITER *super, const double *val)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	fprintf(self->f, "%lf", *val);
	self->need_tab = hpfalse;
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_xml_write_tint8(TLIBC_ABSTRACT_WRITER *super, const tint8 *val)
{
	tint64 v = *val;
	return tlibc_xml_write_tint64(super, &v);
}

TLIBC_ERROR_CODE tlibc_xml_write_tint16(TLIBC_ABSTRACT_WRITER *super, const tint16 *val)
{
	tint64 v = *val;
	return tlibc_xml_write_tint64(super, &v);
}

TLIBC_ERROR_CODE tlibc_xml_write_tint32(TLIBC_ABSTRACT_WRITER *super, const tint32 *val)
{
	tint64 v;
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	if(self->ignore_int32_once)
	{
		self->ignore_int32_once = hpfalse;
		ret = E_TLIBC_IGNORE;
		goto done;
	}
	v = *val;
	ret = tlibc_xml_write_tint64(super, &v);
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_write_tint64(TLIBC_ABSTRACT_WRITER *super, const tint64 *val)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);
	fprintf(self->f, "%lld", *val);
	self->need_tab = hpfalse;
	return E_TLIBC_NOERROR;
}


TLIBC_ERROR_CODE tlibc_xml_write_tuint8(TLIBC_ABSTRACT_WRITER *super, const tuint8 *val)
{
	tuint64 v = *val;
	return tlibc_xml_write_tuint64(super, &v);
}

TLIBC_ERROR_CODE tlibc_xml_write_tuint16(TLIBC_ABSTRACT_WRITER *super, const tuint16 *val)
{
	tuint64 v = *val;
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);

	if(self->skip_uint16_field_once)
	{
		goto done;
	}

	return tlibc_xml_write_tuint64(super, &v);
done:
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_xml_write_tuint32(TLIBC_ABSTRACT_WRITER *super, const tuint32 *val)
{
	tuint64 v = *val;
	return tlibc_xml_write_tuint64(super, &v);
}

TLIBC_ERROR_CODE tlibc_xml_write_tuint64(TLIBC_ABSTRACT_WRITER *super, const tuint64 *val)
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

TLIBC_ERROR_CODE tlibc_xml_write_tstring(TLIBC_ABSTRACT_WRITER *super, const tchar* str)
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

TLIBC_ERROR_CODE tlibc_xml_write_tchar(TLIBC_ABSTRACT_WRITER *super, const tchar *val)
{
	TLIBC_XML_WRITER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_WRITER, super);

	write_char(self->f, *val);
	return E_TLIBC_NOERROR;
}
