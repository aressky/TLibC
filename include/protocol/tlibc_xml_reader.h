#ifndef _H_TLIBC_XML_READER
#define _H_TLIBC_XML_READER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_reader.h"
#include "core/tlibc_error_code.h"

typedef struct _TLIBC_XML_READER_YYLTYPE
{
	char file_name[TLIBC_MAX_PATH_LENGTH];
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} TLIBC_XML_READER_YYLTYPE;

typedef struct _TLIBC_XML_READER_SCANNER_CONTEXT TLIBC_XML_READER_SCANNER_CONTEXT;
struct _TLIBC_XML_READER_SCANNER_CONTEXT
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

	TLIBC_XML_READER_YYLTYPE yylloc;

	char tag_name[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
	const char *content_begin;

	void *filecontent_ptr;
};

#define TLIBC_XML_MAX_INCLUDE 1024
#define TLIBC_XML_MAX_DEEP 1024
typedef struct _TLIBC_XML_READER
{
	TLIBC_ABSTRACT_READER super;

	TLIBC_XML_READER_SCANNER_CONTEXT scanner_context_stack[TLIBC_XML_MAX_DEEP];
	size_t scanner_context_stack_num;

	const char *include[TLIBC_XML_MAX_INCLUDE];
	size_t include_num;

	int struct_deep;
	int ignore_int32_once;
	int pre_read_uint32_field_once;
	uint32_t ui32;
}TLIBC_XML_READER;

 void tlibc_xml_reader_init(TLIBC_XML_READER *self);

 TLIBC_ERROR_CODE tlibc_xml_add_include(TLIBC_XML_READER *self, const char *path);

 TLIBC_ERROR_CODE tlibc_xml_reader_push_file(TLIBC_XML_READER *self, const char *file_name);

 TLIBC_ERROR_CODE tlibc_xml_reader_push_buff(TLIBC_XML_READER *self, const char *xml_start, const char* xml_limit);

 void tlibc_xml_reader_pop_file(TLIBC_XML_READER *self);

 void tlibc_xml_reader_pop_buff(TLIBC_XML_READER *self);

 const char* tlibc_xml_str2c(const char* curr, const char* limit, char *ch);

 const TLIBC_XML_READER_YYLTYPE* tlibc_xml_current_location(TLIBC_XML_READER *self);



 TLIBC_ERROR_CODE tlibc_xml_read_struct_begin(TLIBC_ABSTRACT_READER *self, const char *struct_name);

 TLIBC_ERROR_CODE tlibc_xml_read_struct_end(TLIBC_ABSTRACT_READER *self, const char *struct_name);

 TLIBC_ERROR_CODE tlibc_xml_read_enum_begin(TLIBC_ABSTRACT_READER *self, const char *enum_name);

 TLIBC_ERROR_CODE tlibc_xml_read_vector_begin(TLIBC_ABSTRACT_READER *self);

 TLIBC_ERROR_CODE tlibc_xml_read_vector_end(TLIBC_ABSTRACT_READER *self);

 TLIBC_ERROR_CODE tlibc_xml_read_field_begin(TLIBC_ABSTRACT_READER *self, const char *var_name);

 TLIBC_ERROR_CODE tlibc_xml_read_field_end(TLIBC_ABSTRACT_READER *self, const char *var_name);

 TLIBC_ERROR_CODE tlibc_xml_read_vector_element_begin(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index);

 TLIBC_ERROR_CODE tlibc_xml_read_vector_element_end(TLIBC_ABSTRACT_READER *self, const char *var_name, uint32_t index);

 TLIBC_ERROR_CODE tlibc_xml_read_char(TLIBC_ABSTRACT_READER *super, char *val);

 TLIBC_ERROR_CODE tlibc_xml_read_double(TLIBC_ABSTRACT_READER *super, double *val);

 TLIBC_ERROR_CODE tlibc_xml_read_int8(TLIBC_ABSTRACT_READER *super, int8_t *val);

 TLIBC_ERROR_CODE tlibc_xml_read_int16(TLIBC_ABSTRACT_READER *super, int16_t *val);

 TLIBC_ERROR_CODE tlibc_xml_read_int32(TLIBC_ABSTRACT_READER *super, int32_t *val);

 TLIBC_ERROR_CODE tlibc_xml_read_int64(TLIBC_ABSTRACT_READER *super, int64_t *val);

 TLIBC_ERROR_CODE tlibc_xml_read_uint8(TLIBC_ABSTRACT_READER *super, uint8_t *val);

 TLIBC_ERROR_CODE tlibc_xml_read_uint16(TLIBC_ABSTRACT_READER *super, uint16_t *val);

 TLIBC_ERROR_CODE tlibc_xml_read_uint32(TLIBC_ABSTRACT_READER *super, uint32_t *val);

 TLIBC_ERROR_CODE tlibc_xml_read_uint64(TLIBC_ABSTRACT_READER *super, uint64_t *val);

 TLIBC_ERROR_CODE tlibc_xml_read_string(TLIBC_ABSTRACT_READER *super, char *str, uint32_t str_len);

#endif

