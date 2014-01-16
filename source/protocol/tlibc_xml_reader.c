#include "protocol/tlibc_xml_reader.h"
#include "lib/tlibc_code.h"
#include "protocol/tlibc_abstract_reader.h"
#include "lib/tlibc_error_code.h"
#include "tlibc_xml_reader_l.h"

#include "protocol/tlibc_xml_reader_scanner.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>




TLIBC_ERROR_CODE xml_reader_init(TLIBC_XML_READER *self, const char *file_name)
{
	FILE* fin;
	char c;
	TLIBC_XML_READER_SCANNER_CONTEXT *scanner = NULL;

	strncpy(self->scanner_context.yylloc.file_name, file_name, TLIBC_MAX_FILE_PATH_LENGTH);
	self->scanner_context.yylloc.file_name[TLIBC_MAX_FILE_PATH_LENGTH - 1] = 0;


	self->buff_size = 0;
	fin = fopen(file_name, "rb");
	while((c = (char)fgetc(fin)) != EOF)
	{
		if(self->buff_size >= TLIBC_LEX_LEX_BUFF_SIZE)
		{
			goto ERROR_RET;
		}
		self->buff[self->buff_size] = c;
		++self->buff_size;
	}

	scanner = &self->scanner_context;
	scanner->yy_state = yycINITIAL;
	scanner->yy_start = self->buff;
	scanner->yy_cursor = scanner->yy_start;
	scanner->yy_limit = scanner->yy_start + TLIBC_LEX_LEX_BUFF_SIZE;	
	scanner->yy_marker = scanner->yy_start;
	scanner->yy_last = scanner->yy_start;
	scanner->yylineno = 1;
	scanner->yycolumn = 1;

	self->struct_deep = 0;



	tlibc_abstract_reader_init(&self->super);


	self->super.read_enum_name = xml_read_enum_name;
	self->super.read_struct_begin = xml_read_struct_begin;
	self->super.read_struct_end = xml_read_struct_end;
	self->super.read_enum_begin = xml_read_enum_begin;
	self->super.read_enum_end = xml_read_enum_end;

	self->super.read_vector_begin = xml_read_vector_begin;
	self->super.read_vector_end = xml_read_vector_end;
	self->super.read_field_begin = xml_read_field_begin;
	self->super.read_field_end = xml_read_field_end;

	self->super.read_tint8 = xml_read_tint8;
	self->super.read_tint16 = xml_read_tint16;
	self->super.read_tint32 = xml_read_tint32;
	self->super.read_tint64 = xml_read_tint64;

	self->super.read_tuint8 = xml_read_tuint8;
	self->super.read_tuint16 = xml_read_tuint16;
	self->super.read_tuint32 = xml_read_tuint32;
	self->super.read_tuint64 = xml_read_tuint64;

	self->super.read_tdouble = xml_read_tdouble;
	self->super.read_string = xml_read_tstring;
	self->super.read_tchar = xml_read_tchar;
	self->pre_read_uint16_field_once = hpfalse;
	self->skip_int32_once = hpfalse;

	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}

tint32 xml_read_enum_name(TLIBC_ABSTRACT_READER *self, tchar *enum_name, tuint32 enum_name_length)
{
	xml_read_tstring(self, enum_name, enum_name_length);
	return E_TLIBC_NOERROR;
}

tint32 xml_read_struct_begin(TLIBC_ABSTRACT_READER *super, const char *struct_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	
	
	if(self->struct_deep == 0)
	{
		tlibc_xml_reader_get_token(&self->scanner_context);
	}
	
	++self->struct_deep;
	
	return E_TLIBC_NOERROR;
}

tint32 xml_read_struct_end(TLIBC_ABSTRACT_READER *super, const char *struct_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	
	if(self->struct_deep == 0)
	{
		tlibc_xml_reader_get_token(&self->scanner_context);
	}
	
	--self->struct_deep;
	return E_TLIBC_NOERROR;
}

tint32 xml_read_enum_begin(TLIBC_ABSTRACT_READER *super, const char *enum_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	self->skip_int32_once = hptrue;

	return E_TLIBC_NOERROR;
}

tint32 xml_read_enum_end(TLIBC_ABSTRACT_READER *super, const char *enum_name)
{
	return E_TLIBC_NOERROR;
}

tint32 xml_read_vector_begin(TLIBC_ABSTRACT_READER *super)
{
	int i;
	tuint32 level;
	tuint16 count;
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	
	TLIBC_XML_READER_SCANNER_CONTEXT scanner_context = self->scanner_context;
	count = 0;
	level = 0;
	do
	{
		TLIBC_XML_READER_TOKEN token = (TLIBC_XML_READER_TOKEN)tlibc_xml_reader_get_token(&scanner_context);
		switch(token)
		{
			case tok_tag_begin:				
				if(level == 1)
				{
					++count;
				}
				++level;
				break;
			case tok_tag_end:
				--level;
				break;
		}
		
	}while(level != 0);

	xml_read_field_begin(super, "vector");

	self->pre_read_uint16_field_once = hptrue;
	self->ui16 = count;
	
	return E_TLIBC_NOERROR;
}

tint32 xml_read_vector_end(TLIBC_ABSTRACT_READER *super)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	xml_read_field_begin(super, "vector");

	return E_TLIBC_NOERROR;
}

tint32 xml_read_field_begin(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);

	if(self->pre_read_uint16_field_once)
	{
		goto done;
	}
	tlibc_xml_reader_get_token(&self->scanner_context);
done:
	return E_TLIBC_NOERROR;
}

tint32 xml_read_field_end(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	

	if(self->pre_read_uint16_field_once)
	{
		self->pre_read_uint16_field_once = hpfalse;
		goto done;
	}
	tlibc_xml_reader_get_token(&self->scanner_context);
done:
	return E_TLIBC_NOERROR;
}

TLIBC_API tint32 xml_read_vector_item_begin(TLIBC_ABSTRACT_READER *super, tuint32 index)
{	
	/*
	char str[1024];
	snprintf(str, 1024, "[%d]", index);
	return xml_read_field_begin(super, str);
	*/
	return E_TLIBC_NOERROR;
}

TLIBC_API tint32 xml_read_vector_item_end(TLIBC_ABSTRACT_READER *super, tuint32 index)
{
	/*
	char str[1024];
	snprintf(str, 1024, "[%d]", index);
	return xml_read_field_end(super, str);
	*/
	return E_TLIBC_NOERROR;
}

tint32 xml_read_tdouble(TLIBC_ABSTRACT_READER *super, double *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	
	tlibc_xml_reader_get_content(&self->scanner_context);

	*self->scanner_context.content_end = 0;
	sscanf(self->scanner_context.content_begin, "%lf", val);
	*self->scanner_context.content_end = '<';

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
	tint32 ret = E_TLIBC_NOERROR;
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	if(self->skip_int32_once)
	{
		self->skip_int32_once = hpfalse;
		goto done;
	}
	
	ret = xml_read_tint64(super, &i64);
	*val = (tint32)i64;
done:
	return ret;
}

tint32 xml_read_tint64(TLIBC_ABSTRACT_READER *super, tint64 *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	tlibc_xml_reader_get_content(&self->scanner_context);

	*self->scanner_context.content_end = 0;
	sscanf(self->scanner_context.content_begin, "%lld", val);
	*self->scanner_context.content_end = '<';
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
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	tuint64 ui64;
	tint32 ret;
	if(self->pre_read_uint16_field_once)
	{
		ui64 = self->ui16;
		ret = E_TLIBC_NOERROR;
	}
	else
	{
		ret = xml_read_tuint64(super, &ui64);
	}
	
	*val = (tuint16)ui64;
	return ret;
}

tint32 xml_read_tuint64(TLIBC_ABSTRACT_READER *super, tuint64 *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	tlibc_xml_reader_get_content(&self->scanner_context);

	*self->scanner_context.content_end = 0;
	sscanf(self->scanner_context.content_begin, "%llu", val);
	*self->scanner_context.content_end = '<';
	return E_TLIBC_NOERROR;
}


static void read_char(TLIBC_XML_READER* self, tchar *ch)
{
	char c = *self->scanner_context.content_begin++;
	
	if(c == '&')
	{		
		char c2 = *self->scanner_context.content_begin++;
		if(c2 == 'l')
		{
			//&lt
			*ch = '<';
		}
		else if(c2 == 'g')
		{
			//&gt
			*ch = '>';
		}
		else
		{
			char c3 = *self->scanner_context.content_begin++;
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

	tlibc_xml_reader_get_content(&self->scanner_context);
	read_char(self, val);

	return E_TLIBC_NOERROR;
}

tint32 xml_read_tstring(TLIBC_ABSTRACT_READER *super, tchar *str, tuint32 str_len)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	tuint32 len = 0;

	tlibc_xml_reader_get_content(&self->scanner_context);

	while(self->scanner_context.content_begin < self->scanner_context.content_end)
	{
		char c;
		read_char(self, &c);
		str[len++] = c;
	}
	str[len] = 0;

	return E_TLIBC_NOERROR;
}


