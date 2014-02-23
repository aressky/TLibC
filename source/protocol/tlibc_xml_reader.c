#include "tlibc/protocol/tlibc_xml_reader.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"
#include "tlibc_xml_reader_l.h"

#include "tlibc_xml_reader_scanner.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

TLIBC_ERROR_CODE tlibc_xml_reader_push_file(TLIBC_XML_READER *self, const char *file_name)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	FILE* fin;
	size_t file_size;
	char c;
	char *start, *curr, *limit;

	fin = fopen(file_name, "rb");
	if(fin == NULL)
	{
		size_t i;
		char fn[TLIBC_MAX_PATH_LENGTH];
		for(i = 0; i < self->include_num; ++i)
		{
			snprintf(fn, TLIBC_MAX_PATH_LENGTH, "%s/%s", self->include[i], file_name);
			fin = fopen(fn, "rb");
			if(fin != NULL)
			{
				break;
			}
		}		
	}
	if(fin == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto ERROR_RET;
	}	

	fseek(fin, 0, SEEK_END);
	file_size = ftell(fin);
	fseek(fin, 0, SEEK_SET);
	start = (char*)malloc(file_size);
	if(start == NULL)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;		
		goto free_file;
	}
	curr = start;
	limit = start + file_size;

	while((c = (char)fgetc(fin)) != EOF)
	{
		if(curr == limit)
		{
			ret = E_TLIBC_OUT_OF_MEMORY;
			goto free_buff;
		}
		*curr = c;
		++curr;
	}
	
	ret = tlibc_xml_reader_push_buff(self, start, limit);
	if(ret != E_TLIBC_NOERROR)
	{
		goto free_buff;
	}
	fclose(fin);

	strncpy(self->scanner_context_stack[self->scanner_context_stack_num - 1].yylloc.file_name, file_name, TLIBC_MAX_PATH_LENGTH - 1);
	self->scanner_context_stack[self->scanner_context_stack_num - 1].filecontent_ptr = start;
	
	return E_TLIBC_NOERROR;
free_buff:
	free(start);
free_file:
	fclose(fin);
ERROR_RET:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_reader_push_buff(TLIBC_XML_READER *self, const char *xml_start, const char* xml_limit)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	TLIBC_XML_READER_SCANNER_CONTEXT *scanner = NULL;
	if(self->scanner_context_stack_num >= TLIBC_XML_MAX_DEEP)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto ERROR_RET;
	}
	scanner = &self->scanner_context_stack[self->scanner_context_stack_num];
	scanner->yy_state = yycINITIAL;
	scanner->yy_start = xml_start;
	scanner->yy_limit = xml_limit;
	scanner->yy_cursor = scanner->yy_start;	
	scanner->yy_marker = scanner->yy_start;
	scanner->yy_last = scanner->yy_start;
	scanner->filecontent_ptr = NULL;
	scanner->yylineno = 1;
	scanner->yycolumn = 1;	
	++self->scanner_context_stack_num;
	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}

void tlibc_xml_reader_pop_file(TLIBC_XML_READER *self)
{
	tlibc_xml_reader_pop_buff(self);
	free(self->scanner_context_stack[self->scanner_context_stack_num].filecontent_ptr);
}

void tlibc_xml_reader_pop_buff(TLIBC_XML_READER *self)
{
	--self->scanner_context_stack_num;
}

void tlibc_xml_reader_init(TLIBC_XML_READER *self)
{
	tlibc_abstract_reader_init(&self->super);

	self->super.read_struct_begin = tlibc_xml_read_struct_begin;
	self->super.read_struct_end = tlibc_xml_read_struct_end;
	self->super.read_enum_begin = tlibc_xml_read_enum_begin;

	self->super.read_vector_begin = tlibc_xml_read_vector_begin;
	self->super.read_vector_end = tlibc_xml_read_vector_end;
	self->super.read_field_begin = tlibc_xml_read_field_begin;
	self->super.read_field_end = tlibc_xml_read_field_end;
	self->super.read_vector_element_begin = tlibc_xml_read_vector_element_begin;
	self->super.read_vector_element_end = tlibc_xml_read_vector_element_end;

	self->super.read_int8_t = tlibc_xml_read_int8_t;
	self->super.read_int16_t = tlibc_xml_read_int16_t;
	self->super.read_int32_t = tlibc_xml_read_int32_t;
	self->super.read_int64_t = tlibc_xml_read_int64_t;

	self->super.read_uint8_t = tlibc_xml_read_uint8_t;
	self->super.read_uint16_t = tlibc_xml_read_uint16_t;
	self->super.read_uint32_t = tlibc_xml_read_uint32_t;
	self->super.read_uint64_t = tlibc_xml_read_uint64_t;

	self->super.read_double = tlibc_xml_read_double;
	self->super.read_string = tlibc_xml_read_string;
	self->super.read_char = tlibc_xml_read_char;

	self->pre_read_uint32_t_field_once = FALSE;
	self->ignore_int32_once = FALSE;
	self->scanner_context_stack_num = 0;
	self->include_num = 0;
	self->struct_deep = 0;
}

 TLIBC_ERROR_CODE tlibc_xml_add_include(TLIBC_XML_READER *self, const char *path)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	if(self->include_num >= TLIBC_XML_MAX_INCLUDE)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	self->include[self->include_num] = path;
	++self->include_num;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_struct_begin(TLIBC_ABSTRACT_READER *super, const char *struct_name)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	
	TLIBC_XML_READER_TOKEN token;

	if(self->struct_deep != 0)
	{
		++self->struct_deep;
		goto done;
	}

	token = tlibc_xml_reader_get_token(self);
	if(token != tok_tag_begin)
	{
		ret = E_TLIBC_SYNTAX;
		goto done;
	}

	if(strcmp(struct_name, self->scanner_context_stack[self->scanner_context_stack_num - 1].tag_name) != 0)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}
	++self->struct_deep;
done:	
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_xml_read_struct_end(TLIBC_ABSTRACT_READER *super, const char *struct_name)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	TLIBC_XML_READER_TOKEN token;

	if(self->struct_deep != 0)
	{
		--self->struct_deep;
		goto done;		
	}

	token = tlibc_xml_reader_get_token(self);
	if(token != tok_tag_end)
	{
		ret = E_TLIBC_SYNTAX;
		goto done;
	}
	if(strcmp(struct_name, self->scanner_context_stack[self->scanner_context_stack_num - 1].tag_name) != 0)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	--self->struct_deep;	
done:	
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_enum_begin(TLIBC_ABSTRACT_READER *super, const char *enum_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	TLIBC_UNUSED(enum_name);

	self->ignore_int32_once = TRUE;

	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_xml_read_vector_begin(TLIBC_ABSTRACT_READER *super)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	uint32_t level;
	uint32_t count;
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	TLIBC_XML_READER self_copy = *self;
	count = 0;
	level = 0;
	do
	{
		TLIBC_XML_READER_TOKEN token = tlibc_xml_reader_get_token(&self_copy);
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
			default:
				ret = E_TLIBC_SYNTAX;
				goto ERROR_RET;
		}
	}while(level != 0);

	ret = tlibc_xml_read_field_begin(super, "vector");
	self->pre_read_uint32_t_field_once = TRUE;
	self->ui32 = count;

	return ret;
ERROR_RET:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_vector_end(TLIBC_ABSTRACT_READER *super)
{
	return tlibc_xml_read_field_end(super, "vector");
}

TLIBC_ERROR_CODE tlibc_xml_read_field_begin(TLIBC_ABSTRACT_READER *super, const char *var_name)
{	
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	TLIBC_ERROR_CODE ret;
	TLIBC_XML_READER_TOKEN token;	

	if(self->pre_read_uint32_t_field_once)
	{
		goto done;
	}
	token = tlibc_xml_reader_get_token(self);
	if(token != tok_tag_begin)
	{
		ret = E_TLIBC_SYNTAX;
		goto ERROR_RET;
	}
	if(strcmp(self->scanner_context_stack[self->scanner_context_stack_num - 1].tag_name, var_name) != 0)
	{
		ret = E_TLIBC_MISMATCH;
		goto ERROR_RET;
	}

done:
	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_field_end(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);	
	TLIBC_ERROR_CODE ret;
	TLIBC_XML_READER_TOKEN token;

	if(self->pre_read_uint32_t_field_once)
	{
		self->pre_read_uint32_t_field_once = FALSE;
		goto done;
	}
	token = tlibc_xml_reader_get_token(self);
	if(token != tok_tag_end)
	{
		ret = E_TLIBC_SYNTAX;
		goto ERROR_RET;
	}
	if(strcmp(self->scanner_context_stack[self->scanner_context_stack_num - 1].tag_name, var_name) != 0)
	{
		ret = E_TLIBC_MISMATCH;
		goto ERROR_RET;
	}

done:
	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_vector_element_begin(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index)
{
	TLIBC_UNUSED(index);
	return tlibc_xml_read_field_begin(self, var_name);
}

TLIBC_ERROR_CODE tlibc_xml_read_vector_element_end(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index)
{
	TLIBC_UNUSED(index);
	return tlibc_xml_read_field_end(self, var_name);
}

TLIBC_ERROR_CODE tlibc_xml_read_double(TLIBC_ABSTRACT_READER *super, double *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	TLIBC_ERROR_CODE ret;
	
	errno = 0;
	*val = strtod(self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin, NULL);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto ERROR_RET;
	}

	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_int8_t(TLIBC_ABSTRACT_READER *super, int8_t *val)
{
	int64_t i64;
	TLIBC_ERROR_CODE ret = tlibc_xml_read_int64_t(super, &i64);
	*val = (int8_t)i64;
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_int16_t(TLIBC_ABSTRACT_READER *super, int16_t *val)
{
	int64_t i64;
	TLIBC_ERROR_CODE ret = tlibc_xml_read_int64_t(super, &i64);
	*val = (int16_t)i64;
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_int32_t(TLIBC_ABSTRACT_READER *super, int32_t *val)
{
	int64_t i64;
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	if(self->ignore_int32_once)
	{
		self->ignore_int32_once = FALSE;
		ret = E_TLIBC_PLEASE_READ_ENUM_NAME;
		goto done;
	}
	
	ret = tlibc_xml_read_int64_t(super, &i64);
	*val = (int32_t)i64;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_int64_t(TLIBC_ABSTRACT_READER *super, int64_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);

	errno = 0;
	*val = strtoll(self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin, NULL, 10);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto ERROR_RET;
	}

	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_uint8_t(TLIBC_ABSTRACT_READER *super, uint8_t *val)
{
	uint64_t ui64;
	TLIBC_ERROR_CODE ret = tlibc_xml_read_uint64_t(super, &ui64);
	*val = (uint8_t)ui64;
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_uint32_t(TLIBC_ABSTRACT_READER *super, uint32_t *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	uint64_t ui64;
	TLIBC_ERROR_CODE ret;

	if(self->pre_read_uint32_t_field_once)
	{
		*val = self->ui32;
		ret = E_TLIBC_NOERROR;
	}
	else
	{
		ret = tlibc_xml_read_uint64_t(super, &ui64);
		*val = (uint32_t)ui64;
	}	
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_uint16_t(TLIBC_ABSTRACT_READER *super, uint16_t *val)
{	
	uint64_t ui64;
	TLIBC_ERROR_CODE ret;
	
	ret = tlibc_xml_read_uint64_t(super, &ui64);
		
	*val = (uint16_t)ui64;
	return ret;
}

TLIBC_ERROR_CODE tlibc_xml_read_uint64_t(TLIBC_ABSTRACT_READER *super, uint64_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	errno = 0;
	*val = strtoull(self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin, NULL, 10);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto ERROR_RET;
	}

	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}


const char* tlibc_xml_str2c(const char* curr, const char* limit, char *ch)
{
	char c;
	if(curr >= limit)
	{
		goto ERROR_RET;
	}
	c = *curr++;
	
	if(c == '&')
	{
		char c2;
		if(curr >= limit)
		{
			goto ERROR_RET;
		}
		c2 = *curr++;
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
			char c3;
			if(curr >= limit)
			{
				goto ERROR_RET;
			}
			c3 = *curr++;
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

	return curr;
ERROR_RET:
	return NULL;
}

TLIBC_ERROR_CODE tlibc_xml_read_char(TLIBC_ABSTRACT_READER *super, char *val)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	const char* ret = tlibc_xml_str2c(self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin
		, self->scanner_context_stack[self->scanner_context_stack_num - 1].yy_limit, val);
	self->scanner_context_stack[self->scanner_context_stack_num - 1].yy_cursor = ret;
	if(ret == NULL)
	{
		return E_TLIBC_OUT_OF_MEMORY;
	}
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_xml_read_string(TLIBC_ABSTRACT_READER *super, char *str, uint32_t str_len)
{
	TLIBC_XML_READER *self = TLIBC_CONTAINER_OF(super, TLIBC_XML_READER, super);
	uint32_t len = 0;
	TLIBC_ERROR_CODE ret;
	const char* curr = self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin;
	const char* limit = self->scanner_context_stack[self->scanner_context_stack_num - 1].yy_limit;
	while(curr < limit)
	{
		char c;
		curr = tlibc_xml_str2c(curr, limit, &c);
		if(curr == NULL)
		{
			ret = E_TLIBC_OUT_OF_MEMORY;
			goto ERROR_RET;
		}

		if(c == '<')
		{
			self->scanner_context_stack[self->scanner_context_stack_num - 1].yy_cursor = curr - 1;
			break;
		}
		if(len >= str_len)
		{
			ret = E_TLIBC_OUT_OF_MEMORY;
			goto ERROR_RET;
		}
		str[len++] = c;		
	}
	str[len] = 0;
	

	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}

const TLIBC_XML_READER_YYLTYPE* tlibc_xml_current_location(TLIBC_XML_READER *self)
{
	if(self->scanner_context_stack_num > 0)
	{
		return &self->scanner_context_stack[self->scanner_context_stack_num - 1].yylloc;
	}
	else
	{
		return NULL;
	}
}
