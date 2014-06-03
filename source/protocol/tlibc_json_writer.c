#include "protocol/tlibc_json_writer.h"
#include "core/tlibc_error_code.h"
#include "core/tlibc_util.h"
#include "protocol/tlibc_abstract_writer.h"


#include <string.h>
#include <assert.h>
#include <stdio.h>


void tlibc_json_writer_init(tlibc_json_writer_t *self, char *start, char*limit)
{
	tlibc_abstract_writer_init(&self->super);


	self->cur = start;
	self->start = start;
	self->limit = limit;

	self->super.write_struct_begin = tlibc_json_write_struct_begin;
	self->super.write_struct_end = tlibc_json_write_struct_end;
	self->super.write_union_begin = tlibc_json_write_struct_begin;
	self->super.write_union_end = tlibc_json_write_struct_end;
	self->super.write_enum_begin = tlibc_json_write_enum_begin;

	self->super.write_vector_begin = tlibc_json_write_vector_begin;
	self->super.write_vector_end = tlibc_json_write_vector_end;
	self->super.write_field_begin = tlibc_json_write_field_begin;
	self->super.write_field_end = tlibc_json_write_field_end;
	self->super.write_vector_element_end = tlibc_json_write_vector_element_end;
	
	self->super.write_int8 = tlibc_json_write_int8;
	self->super.write_int16 = tlibc_json_write_int16;
	self->super.write_int32 = tlibc_json_write_int32;
	self->super.write_int64 = tlibc_json_write_int64;

	self->super.write_uint8 = tlibc_json_write_uint8;
	self->super.write_uint16 = tlibc_json_write_uint16;
	self->super.write_uint32 = tlibc_json_write_uint32;
	self->super.write_uint64 = tlibc_json_write_uint64;


	self->super.write_double = tlibc_json_write_double;
	self->super.write_string = tlibc_json_write_string;
	self->super.write_char = tlibc_json_write_char;


	self->skip_uint32_field_once = FALSE;
	self->ignore_int32_once = FALSE;
}

tlibc_error_code_t tlibc_json_write_struct_begin(tlibc_abstract_writer_t *super, const char *struct_name)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);	

	TLIBC_UNUSED(struct_name);
	if((size_t)(self->limit - self->cur) < 1)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	*(self->cur++) = '{';

done:
	return ret;
}

tlibc_error_code_t tlibc_json_write_struct_end(tlibc_abstract_writer_t *super, const char *struct_name)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);

	TLIBC_UNUSED(struct_name);
	if((self->cur > self->start) && (*(self->cur - 1) == ','))
	{
		--(self->cur);
	}

	if((size_t)(self->limit - self->cur) < 1)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	*(self->cur++) = '}';

done:
	return ret;
}

tlibc_error_code_t tlibc_json_write_enum_begin(tlibc_abstract_writer_t *super, const char *enum_name)
{
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);
	TLIBC_UNUSED(enum_name);
	self->ignore_int32_once = TRUE;
	
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_json_write_vector_begin(tlibc_abstract_writer_t *super, const char* vec_name)
{
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);	
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	ret = tlibc_json_write_field_begin(super, vec_name);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	
	self->skip_uint32_field_once = TRUE;

	TLIBC_UNUSED(vec_name);
	if((size_t)(self->limit - self->cur) < 1)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	*(self->cur++) = '[';
done:
	return ret;
}

tlibc_error_code_t tlibc_json_write_vector_end(tlibc_abstract_writer_t *super, const char* vec_name)
{
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);	
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	TLIBC_UNUSED(vec_name);

	if((self->cur > self->start) && (*(self->cur - 1) == ','))
	{
		--(self->cur);
	}

	if((size_t)(self->limit - self->cur) < 1)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	*(self->cur++) = ']';

	ret = tlibc_json_write_field_end(super, vec_name);
done:
	return ret;
}

tlibc_error_code_t tlibc_json_write_field_begin(tlibc_abstract_writer_t *super, const char *var_name)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);
	size_t len, slen;
	if(self->skip_uint32_field_once)
	{
		goto done;
	}

	slen = strlen(var_name);
	len = 1 + slen + 1 + 1;
	
	if((size_t)(self->limit - self->cur) < len)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	
	*(self->cur++) = '"';
	memcpy(self->cur, var_name, slen);
	self->cur+=slen;
	*(self->cur++) = '"';
	*(self->cur++) = ':';

done:
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_json_write_field_end(tlibc_abstract_writer_t *super, const char *var_name)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);	

	TLIBC_UNUSED(var_name);
	if(self->skip_uint32_field_once)
	{
		self->skip_uint32_field_once = FALSE;
		goto done;
	}
	
	if((size_t)(self->limit - self->cur) < 1)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	
	*(self->cur++) = ',';

done:
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_json_write_vector_element_end(tlibc_abstract_writer_t *super, const char *var_name, uint32_t index)
{
	TLIBC_UNUSED(index);
	return tlibc_json_write_field_end(super, var_name);
}

#define TLIBC_JSON_VALUE_LEN 128

tlibc_error_code_t tlibc_json_write_double(tlibc_abstract_writer_t *super, const double *val)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);
	char str[TLIBC_JSON_VALUE_LEN];
	size_t len;
	len = snprintf(str, TLIBC_JSON_VALUE_LEN, "%lf", *val);
	if((size_t)(self->limit - self->cur) < len)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	memcpy(self->cur, str, len);
	self->cur += len;
done:
	return ret;
}

tlibc_error_code_t tlibc_json_write_int8(tlibc_abstract_writer_t *super, const int8_t *val)
{
	int64_t v = *val;
	return tlibc_json_write_int64(super, &v);
}

tlibc_error_code_t tlibc_json_write_int16(tlibc_abstract_writer_t *super, const int16_t *val)
{
	int64_t v = *val;
	return tlibc_json_write_int64(super, &v);
}

tlibc_error_code_t tlibc_json_write_int32(tlibc_abstract_writer_t *super, const int32_t *val)
{
	int64_t v;
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);
	if(self->ignore_int32_once)
	{
		self->ignore_int32_once = FALSE;
		ret = E_TLIBC_PLEASE_READ_ENUM_NAME;
		goto done;
	}
	v = *val;
	ret = tlibc_json_write_int64(super, &v);
done:
	return ret;
}

tlibc_error_code_t tlibc_json_write_int64(tlibc_abstract_writer_t *super, const int64_t *val)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);
	char str[TLIBC_JSON_VALUE_LEN];
	size_t len;
	len = snprintf(str, TLIBC_JSON_VALUE_LEN, "%"PRIi64, *val);
	if((size_t)(self->limit - self->cur) < len)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	memcpy(self->cur, str, len);
	self->cur += len;
done:
	return ret;
}


tlibc_error_code_t tlibc_json_write_uint8(tlibc_abstract_writer_t *super, const uint8_t *val)
{
	uint64_t v = *val;
	return tlibc_json_write_uint64(super, &v);
}

tlibc_error_code_t tlibc_json_write_uint16(tlibc_abstract_writer_t *super, const uint16_t *val)
{
	uint64_t v = *val;
	
	return tlibc_json_write_uint64(super, &v);
}

tlibc_error_code_t tlibc_json_write_uint32(tlibc_abstract_writer_t *super, const uint32_t *val)
{
	uint64_t v = *val;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);
	if(self->skip_uint32_field_once)
	{
		goto done;
	}
	return tlibc_json_write_uint64(super, &v);
done:
	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_json_write_uint64(tlibc_abstract_writer_t *super, const uint64_t *val)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);
	char str[TLIBC_JSON_VALUE_LEN];
	size_t len;
	len = snprintf(str, TLIBC_JSON_VALUE_LEN, "%"PRIu64, *val);
	if((size_t)(self->limit - self->cur) < len)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	memcpy(self->cur, str, len);
	self->cur += len;
done:
	return ret;
}

static char* write_char(char *cur, char *limit, char c)
{
	char carr[2];
	const char *str = NULL;
	size_t len = 0;
	carr[0] = c;
	carr[1] = 0;
	switch (c)
	{
	case '\"':
		str = "\\\"";
		break;
	case '\\':
		str = "\\\\";
		break;
	case '/':
		str = "\\/";
		break;
	case '\b':
		str = "\\b";
		break;
	case '\f':
		str = "\\f";
		break;
	case '\n':
		str = "\\n";
		break;
	case '\r':
		str = "\\r";
		break;
	case '\t':
		str = "\\t";
		break;
	default:
		str = carr;
		break;
	}
	len = strlen(str);
	if((size_t)(limit - cur) < len)
	{
		return NULL;
	}
	memcpy(cur, str, len);
	return cur + len;
}

tlibc_error_code_t tlibc_json_write_string(tlibc_abstract_writer_t *super, const char* str, uint32_t str_length)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);
	const char *i;	
	TLIBC_UNUSED(str_length);

	if((size_t)(self->limit - self->cur) < 1)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	*(self->cur++) = '"';

	for(i = str; *i ; ++i)
	{
		char *next = write_char(self->cur, self->limit, *i);
		if(next == NULL)
		{
			ret = E_TLIBC_OUT_OF_MEMORY;
			goto done;
		}
		self->cur = next;
	}

	if((size_t)(self->limit - self->cur) < 1)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	*(self->cur++) = '"';
done:
	return ret;
}

tlibc_error_code_t tlibc_json_write_char(tlibc_abstract_writer_t *super, const char *val)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_json_writer_t *self = TLIBC_CONTAINER_OF(super, tlibc_json_writer_t, super);
	char *next = NULL;

	if((size_t)(self->limit - self->cur) < 1)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	*(self->cur++) = '"';

	next = write_char(self->cur, self->limit, *val);
	if(next == NULL)
	{
		return E_TLIBC_OUT_OF_MEMORY;
	}
	self->cur = next;

	if((size_t)(self->limit - self->cur) < 1)
	{
		ret = E_TLIBC_OUT_OF_MEMORY;
		goto done;
	}
	*(self->cur++) = '"';

done:
	return ret;
}
