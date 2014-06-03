#include "protocol/tlibc_json_reader.h"
#include "core/tlibc_util.h"
#include "protocol/tlibc_abstract_reader.h"
#include "core/tlibc_error_code.h"
#include "tlibc_json_reader_l.h"

#include "tlibc_json_reader_scanner.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

static tlibc_error_code_t tlibc_json_parser(tlibc_json_reader_t *self)
{
	tlibc_json_reader_token_t token;

	for(;;)
	{
		token = tlibc_json_reader_get_token(self);
		if(token == tok_end)
		{
			break;
		}
		else if(token == tok_error)
		{
			break;
		}
	}

	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_json_reader_init(tlibc_json_reader_t *self, const char *json_start, const char* json_limit)
{
	tlibc_json_reader_scanner_context_t *scanner = &self->scanner_context_stack;
	

	tlibc_abstract_reader_init(&self->super);

	self->super.read_struct_begin = tlibc_json_read_struct_begin;
	self->super.read_struct_end = tlibc_json_read_struct_end;
	self->super.read_union_begin = tlibc_json_read_struct_begin;
	self->super.read_union_end = tlibc_json_read_struct_end;
	self->super.read_enum_begin = tlibc_json_read_enum_begin;

	self->super.read_vector_begin = tlibc_json_read_vector_begin;
	self->super.read_vector_end = tlibc_json_read_vector_end;
	self->super.read_field_begin = tlibc_json_read_field_begin;
	self->super.read_field_end = tlibc_json_read_field_end;
	self->super.read_vector_element_begin = tlibc_json_read_vector_element_begin;
	self->super.read_vector_element_end = tlibc_json_read_vector_element_end;

	self->super.read_int8 = tlibc_json_read_int8;
	self->super.read_int16 = tlibc_json_read_int16;
	self->super.read_int32 = tlibc_json_read_int32;
	self->super.read_int64 = tlibc_json_read_int64;

	self->super.read_uint8 = tlibc_json_read_uint8;
	self->super.read_uint16 = tlibc_json_read_uint16;
	self->super.read_uint32 = tlibc_json_read_uint32;
	self->super.read_uint64 = tlibc_json_read_uint64;

	self->super.read_double = tlibc_json_read_double;
	self->super.read_string = tlibc_json_read_string;
	self->super.read_char = tlibc_json_read_char;

	self->pre_read_uint32_field_once = FALSE;
	self->ignore_int32_once = FALSE;
	self->struct_deep = 0;


	scanner->yy_state = yycINITIAL;
	scanner->yy_start = json_start;
	scanner->yy_limit = json_limit;
	scanner->yy_cursor = scanner->yy_start;	
	scanner->yy_marker = scanner->yy_start;
	scanner->yy_last = scanner->yy_start;
	scanner->filecontent_ptr = NULL;
	scanner->yylineno = 1;
	scanner->yycolumn = 1;	


	return tlibc_json_parser(self);
}

tlibc_error_code_t tlibc_json_read_struct_begin(tlibc_abstract_reader_t *super, const char *struct_name)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);	
	tlibc_json_reader_token_t token;

	if(self->struct_deep != 0)
	{
		++self->struct_deep;
		goto done;
	}

	token = tlibc_json_reader_get_token(self);
	if(token != tok_tag_begin)
	{
		if(self->error_code == E_TLIBC_NOERROR)
		{
			ret = E_TLIBC_BAD_FILE;
		}
		else
		{
			ret = self->error_code;
		}
		goto done;
	}

	if(strcmp(struct_name, self->scanner_context_stack.tag_name) != 0)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}
	++self->struct_deep;
done:	
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_json_read_struct_end(tlibc_abstract_reader_t *super, const char *struct_name)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);
	tlibc_json_reader_token_t token;

	if(self->struct_deep != 0)
	{
		--self->struct_deep;
		goto done;		
	}

	token = tlibc_json_reader_get_token(self);
	if(token != tok_tag_end)
	{
		if(self->error_code == E_TLIBC_NOERROR)
		{
			ret = E_TLIBC_BAD_FILE;
		}
		else
		{
			ret = self->error_code;
		}
		goto done;
	}
	if(strcmp(struct_name, self->scanner_context_stack.tag_name) != 0)
	{
		ret = E_TLIBC_MISMATCH;
		goto done;
	}

	--self->struct_deep;	
done:	
	return ret;
}

tlibc_error_code_t tlibc_json_read_enum_begin(tlibc_abstract_reader_t *super, const char *enum_name)
{
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);
	TLIBC_UNUSED(enum_name);

	self->ignore_int32_once = TRUE;

	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_json_read_vector_begin(tlibc_abstract_reader_t *super, const char *vec_name)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	uint32_t level;
	uint32_t count;
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);
	//copy一份是因为统计数组的长度。
	tlibc_json_reader_t self_copy = *self;
	TLIBC_UNUSED(vec_name);
	count = 0;
	level = 0;
	do
	{
		tlibc_json_reader_token_t token = tlibc_json_reader_get_token(&self_copy);
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
				if(self->error_code == E_TLIBC_NOERROR)
				{
					ret = E_TLIBC_BAD_FILE;
				}
				else
				{
					ret = self->error_code;
				}
				goto ERROR_RET;
		}
	}while(level != 0);

	ret = tlibc_json_read_field_begin(super, vec_name);
	self->pre_read_uint32_field_once = TRUE;
	self->ui32 = count;

	return ret;
ERROR_RET:
	return ret;
}

tlibc_error_code_t tlibc_json_read_vector_end(tlibc_abstract_reader_t *super, const char *vec_name)
{
	return tlibc_json_read_field_end(super, vec_name);
}

tlibc_error_code_t tlibc_json_read_field_begin(tlibc_abstract_reader_t *super, const char *var_name)
{	
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);
	tlibc_error_code_t ret;
	tlibc_json_reader_token_t token;	

	if(self->pre_read_uint32_field_once)
	{
		goto done;
	}
	token = tlibc_json_reader_get_token(self);
	if(token != tok_tag_begin)
	{
		if(self->error_code == E_TLIBC_NOERROR)
		{
			ret = E_TLIBC_BAD_FILE;
		}
		else
		{
			ret = self->error_code;
		}
		goto ERROR_RET;
	}
	if(strcmp(self->scanner_context_stack.tag_name, var_name) != 0)
	{
		ret = E_TLIBC_MISMATCH;
		goto ERROR_RET;
	}

done:
	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}

tlibc_error_code_t tlibc_json_read_field_end(tlibc_abstract_reader_t *super, const char *var_name)
{
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);	
	tlibc_error_code_t ret;
	tlibc_json_reader_token_t token;

	if(self->pre_read_uint32_field_once)
	{
		self->pre_read_uint32_field_once = FALSE;
		goto done;
	}
	token = tlibc_json_reader_get_token(self);
	if(token != tok_tag_end)
	{
		if(self->error_code == E_TLIBC_NOERROR)
		{
			ret = E_TLIBC_BAD_FILE;
		}
		else
		{
			ret = self->error_code;
		}
		goto ERROR_RET;
	}
	if(strcmp(self->scanner_context_stack.tag_name, var_name) != 0)
	{
		ret = E_TLIBC_MISMATCH;
		goto ERROR_RET;
	}

done:
	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}

tlibc_error_code_t tlibc_json_read_vector_element_begin(tlibc_abstract_reader_t *self, const char *var_name, uint32_t index)
{
	TLIBC_UNUSED(var_name);
	TLIBC_UNUSED(index);
	return tlibc_json_read_field_begin(self, "element");
}

tlibc_error_code_t tlibc_json_read_vector_element_end(tlibc_abstract_reader_t *self, const char *var_name, uint32_t index)
{
	TLIBC_UNUSED(var_name);
	TLIBC_UNUSED(index);
	return tlibc_json_read_field_end(self, "element");
}

tlibc_error_code_t tlibc_json_read_double(tlibc_abstract_reader_t *super, double *val)
{
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);
	tlibc_error_code_t ret;
	
	errno = 0;
	*val = strtod(self->scanner_context_stack.content_begin, NULL);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto ERROR_RET;
	}

	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}

tlibc_error_code_t tlibc_json_read_int8(tlibc_abstract_reader_t *super, int8_t *val)
{
	int64_t i64;
	tlibc_error_code_t ret = tlibc_json_read_int64(super, &i64);
	*val = (int8_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

tlibc_error_code_t tlibc_json_read_int16(tlibc_abstract_reader_t *super, int16_t *val)
{
	int64_t i64;
	tlibc_error_code_t ret = tlibc_json_read_int64(super, &i64);
	*val = (int16_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

tlibc_error_code_t tlibc_json_read_int32(tlibc_abstract_reader_t *super, int32_t *val)
{
	int64_t i64;
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);
	if(self->ignore_int32_once)
	{
		self->ignore_int32_once = FALSE;
		ret = E_TLIBC_PLEASE_READ_ENUM_NAME;
		goto done;
	}
	
	ret = tlibc_json_read_int64(super, &i64);
	*val = (int32_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tlibc_error_code_t tlibc_json_read_int64(tlibc_abstract_reader_t *super, int64_t *val)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);

	errno = 0;
	*val = strtoll(self->scanner_context_stack.content_begin, NULL, 10);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto ERROR_RET;
	}

	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}

tlibc_error_code_t tlibc_json_read_uint8(tlibc_abstract_reader_t *super, uint8_t *val)
{
	uint64_t ui64;
	tlibc_error_code_t ret = tlibc_json_read_uint64(super, &ui64);
	*val = (uint8_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

tlibc_error_code_t tlibc_json_read_uint16(tlibc_abstract_reader_t *super, uint16_t *val)
{	
	uint64_t ui64;
	tlibc_error_code_t ret;

	ret = tlibc_json_read_uint64(super, &ui64);

	*val = (uint16_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

tlibc_error_code_t tlibc_json_read_uint32(tlibc_abstract_reader_t *super, uint32_t *val)
{
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);
	uint64_t ui64;
	tlibc_error_code_t ret;

	if(self->pre_read_uint32_field_once)
	{
		*val = self->ui32;
		ret = E_TLIBC_NOERROR;
	}
	else
	{
		ret = tlibc_json_read_uint64(super, &ui64);
		*val = (uint32_t)ui64;
		if(*val != ui64)
		{
			return E_TLIBC_INTEGER_OVERFLOW;
		}
	}	
	return ret;
}

tlibc_error_code_t tlibc_json_read_uint64(tlibc_abstract_reader_t *super, uint64_t *val)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);
	errno = 0;
	*val = strtoull(self->scanner_context_stack.content_begin, NULL, 10);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto ERROR_RET;
	}

	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}


const char* tlibc_json_str2c(const char* curr, const char* limit, char *ch)
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
			curr += 2;
			if(curr >= limit)
			{
				goto ERROR_RET;
			}
		}
		else if(c2 == 'g')
		{
			//&gt
			*ch = '>';
			curr += 2;
			if(curr >= limit)
			{
				goto ERROR_RET;
			}
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
				curr += 2;
				if(curr >= limit)
				{
					goto ERROR_RET;
				}
			}
			else if(c3 == 'p')
			{
				//&apos
				*ch = '\'';
				curr += 3;
				if(curr >= limit)
				{
					goto ERROR_RET;
				}
			}
			else if(c3 == 'u')
			{
				//&auot
				*ch = '\"';
				curr += 3;
				if(curr >= limit)
				{
					goto ERROR_RET;
				}
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

tlibc_error_code_t tlibc_json_read_char(tlibc_abstract_reader_t *super, char *val)
{
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);
	const char* ret = tlibc_json_str2c(self->scanner_context_stack.content_begin
		, self->scanner_context_stack.yy_limit, val);
	self->scanner_context_stack.yy_cursor = ret;
	if(ret == NULL)
	{
		return E_TLIBC_OUT_OF_MEMORY;
	}
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_json_read_string(tlibc_abstract_reader_t *super, char *str, uint32_t str_len)
{
	tlibc_json_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_reader_t, super);
	uint32_t len = 0;
	tlibc_error_code_t ret;
	const char* curr = self->scanner_context_stack.content_begin;
	const char* limit = self->scanner_context_stack.yy_limit;
	while(curr < limit)
	{
		char c = 0;
		if(*curr == '<')
		{
			self->scanner_context_stack.yy_cursor = curr - 1;
			break;
		}
		curr = tlibc_json_str2c(curr, limit, &c);
		if(curr == NULL)
		{
			ret = E_TLIBC_OUT_OF_MEMORY;
			goto ERROR_RET;
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

const tlibc_json_reader_yyltype_t* tlibc_json_current_location(tlibc_json_reader_t *self)
{
	return &self->scanner_context_stack.yylloc;
}
