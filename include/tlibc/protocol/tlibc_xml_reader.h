#ifndef _H_TLIBC_XML_READER
#define _H_TLIBC_XML_READER

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

#include "tlibc/tdata/tdata_types.h"

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
	const tchar *yy_last;
	const tchar *yy_cursor;
	const tchar *yy_limit;	
	const tchar *yy_marker;
	const tchar *yy_start;

	const tchar *yy_text;
	tuint32 yy_leng;

	tuint32 yylineno;
	tuint32 yycolumn;

	TLIBC_XML_READER_YYLTYPE yylloc;

	char tag_name[TDATA_MAX_LENGTH_OF_IDENTIFIER];
	const tchar *content_begin;

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

	tuint32 struct_deep;
	int ignore_int32_once;
	int pre_read_uint32_field_once;
	tuint32 ui32;
}TLIBC_XML_READER;

TLIBC_API void tlibc_xml_reader_init(TLIBC_XML_READER *self);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_add_include(TLIBC_XML_READER *self, const char *path);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_reader_push_file(TLIBC_XML_READER *self, const char *file_name);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_reader_push_buff(TLIBC_XML_READER *self, const char *xml_start, const char* xml_limit);

TLIBC_API void tlibc_xml_reader_pop_file(TLIBC_XML_READER *self);

TLIBC_API void tlibc_xml_reader_pop_buff(TLIBC_XML_READER *self);

//获取实体引用表示的字符
TLIBC_API TLIBC_ERROR_CODE tlibc_xml_str2c(const char* curr, const char* limit, tchar *ch);


TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_struct_begin(TLIBC_ABSTRACT_READER *self, const char *struct_name);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_struct_end(TLIBC_ABSTRACT_READER *self, const char *struct_name);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_enum_begin(TLIBC_ABSTRACT_READER *self, const char *enum_name);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_vector_begin(TLIBC_ABSTRACT_READER *self);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_vector_end(TLIBC_ABSTRACT_READER *self);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_field_begin(TLIBC_ABSTRACT_READER *self, const char *var_name);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_field_end(TLIBC_ABSTRACT_READER *self, const char *var_name);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_vector_element_begin(TLIBC_ABSTRACT_READER *self, const char *var_name, tuint32 index);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_vector_element_end(TLIBC_ABSTRACT_READER *self, const char *var_name, tuint32 index);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tchar(TLIBC_ABSTRACT_READER *super, char *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tdouble(TLIBC_ABSTRACT_READER *super, double *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tint8(TLIBC_ABSTRACT_READER *super, tint8 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tint16(TLIBC_ABSTRACT_READER *super, tint16 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tint32(TLIBC_ABSTRACT_READER *super, tint32 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tint64(TLIBC_ABSTRACT_READER *super, tint64 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tuint8(TLIBC_ABSTRACT_READER *super, tuint8 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tuint16(TLIBC_ABSTRACT_READER *super, tuint16 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tuint32(TLIBC_ABSTRACT_READER *super, tuint32 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tuint64(TLIBC_ABSTRACT_READER *super, tuint64 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xml_read_tstring(TLIBC_ABSTRACT_READER *super, tchar *str, tuint32 str_len);

#endif

