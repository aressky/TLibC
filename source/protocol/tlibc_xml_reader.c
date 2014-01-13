#include "protocol/tlibc_xml_reader.h"
#include "lib/tlibc_code.h"
#include "protocol/tlibc_abstract_reader.h"
#include "lib/tlibc_error_code.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>


void xml_reader_init(TLIBC_XML_READER *self, FILE *f)
{
	tlibc_abstract_reader_init(&self->super);

	self->f = f;
	self->count = 0;
	self->need_tab = hpfalse;
	

	self->super.read_enum_name = xml_read_enum_name;
	self->super.read_struct_begin = xml_read_struct_begin;
	self->super.read_struct_end = xml_read_struct_end;

	self->super.read_vector_begin = xml_read_vector_begin;
	self->super.read_vector_end = xml_read_vector_end;
	self->super.read_field_begin = xml_read_field_begin;
	self->super.read_field_end = xml_read_field_end;
	self->super.read_vector_item_begin = xml_read_vector_item_begin;
	self->super.read_vector_item_end= xml_read_vector_item_end;

	self->super.read_tint8 = xml_read_tint8;
	self->super.read_tint16 = xml_read_tint16;
	self->super.read_tint32 = xml_read_tint32;
	self->super.read_tint64 = xml_read_tint64;

	self->super.read_tuint8 = xml_read_tuint8;
	self->super.read_tuint16 = xml_read_tuint16;
	self->super.read_tuint32 = xml_read_tuint32;
	self->super.read_tuint64 = xml_read_tuint64;

	self->super.read_tdouble = xml_read_tdouble;
	self->super.read_string = xml_read_string;
	self->super.read_tchar = xml_read_tchar;
	self->super.read_counter = xml_read_counter;
}

static void skip_tab(TLIBC_XML_READER *self)
{
	for(;;)
	{
		int c = fgetc(self->f);
		if((c != '\t') && (c != '\n') && (c != '\r') && (c != ' '))
		{
			ungetc(c, self->f);
			break;
		}
	}
}

tint32 xml_read_enum_name(TLIBC_ABSTRACT_READER *self, tchar *enum_name, tuint32 enum_name_length)
{
	xml_read_string(self, enum_name, enum_name_length);
	return E_TLIBC_NOERROR;
}

tint32 xml_read_struct_begin(TLIBC_ABSTRACT_READER *super, const char *struct_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	
	tuint32 i;
	tuint32 len;

	if(self->count == 0)
	{
		len = strlen(struct_name);
		fgetc(self->f);
		for(i = 0;i < len; ++i)
		{
			fgetc(self->f);
		}
		fgetc(self->f);
	}
	return E_TLIBC_NOERROR;
}

tint32 xml_read_struct_end(TLIBC_ABSTRACT_READER *super, const char *struct_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	
	tuint32 i;
	tuint32 len;

	if(self->count == 0)
	{
		len = strlen(struct_name);
		fgetc(self->f);
		for(i = 0;i < len; ++i)
		{
			fgetc(self->f);
		}
		fgetc(self->f);
	}
	return E_TLIBC_NOERROR;
}

tint32 xml_read_vector_begin(TLIBC_ABSTRACT_READER *super)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	
	++self->count;
	return E_TLIBC_NOERROR;
}

tint32 xml_read_vector_end(TLIBC_ABSTRACT_READER *super)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	--self->count;

	return E_TLIBC_NOERROR;
}

tint32 xml_read_field_begin(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	const char *i;

	fgetc(self->f);
	++(self->count);
	skip_tab(self);	
	fgetc(self->f);
	for(i = var_name; *i; ++i)
	{
		fgetc(self->f);
	}
	fgetc(self->f);

	return E_TLIBC_NOERROR;
}

tint32 xml_read_field_end(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	
	const char *i;
	
	if(self->need_tab)
	{
		fgetc(self->f);
		skip_tab(self);
	}
	--(self->count);
	self->need_tab = hptrue;
	fgetc(self->f);
	fgetc(self->f);
	for(i = var_name; *i; ++i)
	{
		fgetc(self->f);
	}
	fgetc(self->f);
	return E_TLIBC_NOERROR;
}

TLIBC_API tint32 xml_read_vector_item_begin(TLIBC_ABSTRACT_READER *super, tuint32 index)
{	
	char str[1024];
	snprintf(str, 1024, "[%d]", index);
	return xml_read_field_begin(super, str);
}

TLIBC_API tint32 xml_read_vector_item_end(TLIBC_ABSTRACT_READER *super, tuint32 index)
{
	char str[1024];
	snprintf(str, 1024, "[%d]", index);
	return xml_read_field_end(super, str);
}

tint32 xml_read_tdouble(TLIBC_ABSTRACT_READER *super, double *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	fscanf(self->f, "%lf", val);
	self->need_tab = hpfalse;
	return E_TLIBC_NOERROR;
}

tint32 xml_read_tint8(TLIBC_ABSTRACT_READER *super, tint8 *val)
{
	tint64 i64;
	tint32 ret = xml_read_tint64(super, &i64);
	*val = (tint8)i64;
	return ret;
}

tint32 xml_read_tint16(TLIBC_ABSTRACT_READER *super, tint16 *val)
{
	tint64 i64;
	tint32 ret = xml_read_tint64(super, &i64);
	*val = (tint16)i64;
	return ret;
}

tint32 xml_read_tint32(TLIBC_ABSTRACT_READER *super, tint32 *val)
{
	tint64 i64;
	tint32 ret = xml_read_tint64(super, &i64);
	*val = (tint32)i64;
	return ret;
}

tint32 xml_read_tint64(TLIBC_ABSTRACT_READER *super, tint64 *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	fscanf(self->f, "%lld", val);
	self->need_tab = hpfalse;
	return E_TLIBC_NOERROR;
}

tint32 xml_read_tuint8(TLIBC_ABSTRACT_READER *super, tuint8 *val)
{
	tuint64 ui64;
	tint32 ret = xml_read_tuint64(super, &ui64);
	*val = (tuint8)ui64;
	return ret;
}

tint32 xml_read_tuint32(TLIBC_ABSTRACT_READER *super, tuint32 *val)
{
	tuint64 ui64;
	tint32 ret = xml_read_tuint64(super, &ui64);
	*val = (tuint32)ui64;
	return ret;
}

tint32 xml_read_tuint16(TLIBC_ABSTRACT_READER *super, tuint16 *val)
{
	tuint64 ui64;
	tint32 ret = xml_read_tuint64(super, &ui64);
	*val = (tuint16)ui64;
	return ret;
}

tint32 xml_read_tuint64(TLIBC_ABSTRACT_READER *super, tuint64 *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	fscanf(self->f, "%llu", val);
	self->need_tab = hpfalse;
	return E_TLIBC_NOERROR;
}


static void read_char(FILE* fin, tchar *ch)
{
	char c = fgetc(fin);
	
	if(c == '&')
	{
		char c2 = fgetc(fin);
		if(c2 == 'l')
		{
			//&lt
			fgetc(fin);
			*ch = '<';
		}
		else if(c2 == 'g')
		{
			//&gt
			*ch = '>';
		}
		else
		{
			char c3 = fgetc(fin);
			if(c3 == 'm')
			{
				//&amp
				*ch = '&';
			}
			else if(c3 == 'p')
			{
				//&apos
				*ch = '\'';
			}
			else if(c3 == 'u')
			{
				//&auot
				*ch = '\"';
			}
		}
	}
	else
	{
		*ch = c;
	}
}

tint32 xml_read_tchar(TLIBC_ABSTRACT_READER *super, char *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	read_char(self->f, val);

	return E_TLIBC_NOERROR;
}

tint32 xml_read_string(TLIBC_ABSTRACT_READER *super, tchar *str, tuint32 str_len)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	tuint32 len = 0;

	self->need_tab = hpfalse;
	for(;;)
	{
		char c;
		read_char(self->f, &c);
		if(c == '<')
		{
			ungetc('<', self->f);
			str[len++] = 0;
			break;
		}
		else
		{
			str[len++] = c;
		}
	}

	return E_TLIBC_NOERROR;
}

TLIBC_API tint32 xml_read_counter(TLIBC_ABSTRACT_READER *super, const tchar *name, tuint32 *val)
{
	xml_read_field_begin(super, name);
	xml_read_tuint32(super, val);
	xml_read_field_end(super, name);

	return E_TLIBC_NOERROR;
}
