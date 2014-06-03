#ifndef _H_TLIBC_JSON_READER
#define _H_TLIBC_JSON_READER

/*
	非常抱歉， json_reader功能并没有开发完成， 请勿使用此文件。
	没有完成这个功能的原因是我发现可以使用PHP更加方便的处理JSON并把处理的结果存放到MySQL中， C语言直接读取MySQL即可。
*/

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_reader.h"
#include "core/tlibc_error_code.h"

typedef struct tlibc_json_reader_yyltype_s
{
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} tlibc_json_reader_yyltype_t;

typedef struct tlibc_json_reader_scanner_context_s tlibc_json_reader_scanner_context_t;
struct tlibc_json_reader_scanner_context_s
{
	int yy_state;
	const char *yy_last;
	const char *yy_cursor;
	const char *yy_limit;	
	const char *yy_marker;
	const char *yy_start;

	const char *yy_text;
	uint32_t yy_leng;

	uint32_t yylineno;
	uint32_t yycolumn;

	tlibc_json_reader_yyltype_t yylloc;

	char tag_name[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
	const char *content_begin;

	void *filecontent_ptr;
};

typedef struct tlibc_json_reader_s
{
	tlibc_abstract_reader_t super;

	tlibc_json_reader_scanner_context_t scanner_context_stack;

	int struct_deep;
	int ignore_int32_once;
	int pre_read_uint32_field_once;
	uint32_t ui32;
	tlibc_error_code_t error_code;
}tlibc_json_reader_t;

tlibc_error_code_t tlibc_json_reader_init(tlibc_json_reader_t *self, const char *json_start, const char* json_limit);

const char* tlibc_json_str2c(const char* curr, const char* limit, char *ch);

const tlibc_json_reader_yyltype_t* tlibc_json_current_location(tlibc_json_reader_t *self);

tlibc_error_code_t tlibc_json_read_struct_begin(tlibc_abstract_reader_t *self, const char *struct_name);

tlibc_error_code_t tlibc_json_read_struct_end(tlibc_abstract_reader_t *self, const char *struct_name);

tlibc_error_code_t tlibc_json_read_enum_begin(tlibc_abstract_reader_t *self, const char *enum_name);

tlibc_error_code_t tlibc_json_read_vector_begin(tlibc_abstract_reader_t *self, const char *vec_name);

tlibc_error_code_t tlibc_json_read_vector_end(tlibc_abstract_reader_t *self, const char *vec_name);

tlibc_error_code_t tlibc_json_read_field_begin(tlibc_abstract_reader_t *self, const char *var_name);

tlibc_error_code_t tlibc_json_read_field_end(tlibc_abstract_reader_t *self, const char *var_name);

tlibc_error_code_t tlibc_json_read_vector_element_begin(tlibc_abstract_reader_t *self, const char *var_name, uint32_t index);

tlibc_error_code_t tlibc_json_read_vector_element_end(tlibc_abstract_reader_t *self, const char *var_name, uint32_t index);

tlibc_error_code_t tlibc_json_read_char(tlibc_abstract_reader_t *super, char *val);

tlibc_error_code_t tlibc_json_read_double(tlibc_abstract_reader_t *super, double *val);

tlibc_error_code_t tlibc_json_read_int8(tlibc_abstract_reader_t *super, int8_t *val);

tlibc_error_code_t tlibc_json_read_int16(tlibc_abstract_reader_t *super, int16_t *val);

tlibc_error_code_t tlibc_json_read_int32(tlibc_abstract_reader_t *super, int32_t *val);

tlibc_error_code_t tlibc_json_read_int64(tlibc_abstract_reader_t *super, int64_t *val);

tlibc_error_code_t tlibc_json_read_uint8(tlibc_abstract_reader_t *super, uint8_t *val);

tlibc_error_code_t tlibc_json_read_uint16(tlibc_abstract_reader_t *super, uint16_t *val);

tlibc_error_code_t tlibc_json_read_uint32(tlibc_abstract_reader_t *super, uint32_t *val);

tlibc_error_code_t tlibc_json_read_uint64(tlibc_abstract_reader_t *super, uint64_t *val);

tlibc_error_code_t tlibc_json_read_string(tlibc_abstract_reader_t *super, char *str, uint32_t str_len);

#endif
