#ifndef _H_TLIBC_XML_READER
#define _H_TLIBC_XML_READER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_reader.h"
#include "core/tlibc_error_code.h"

#include "tdata/tdata_types.h"

typedef struct _TLIBC_XML_READER_YYLTYPE
{
	char file_name[TLIBC_MAX_FILE_PATH_LENGTH];
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} TLIBC_XML_READER_YYLTYPE;

typedef struct _TLIBC_XML_READER_SCANNER_CONTEXT TLIBC_XML_READER_SCANNER_CONTEXT;
struct _TLIBC_XML_READER_SCANNER_CONTEXT
{
	int yy_state;
	tchar *yy_last;
	tchar *yy_cursor;
	tchar *yy_limit;
	tchar *yy_text;
	tchar *yy_marker;
	tchar *yy_start;
	tuint32 yy_leng;

	tuint32 yylineno;
	tuint32 yycolumn;

	

	TLIBC_XML_READER_YYLTYPE yylloc;

	char tag_name[TDATA_MAX_LENGTH_OF_IDENTIFIER];
	tchar *content_begin;
	tchar *content_end;
};


#define TLIBC_LEX_LEX_BUFF_SIZE 10000000

typedef struct _TLIBC_XML_READER
{
	TLIBC_ABSTRACT_READER super;

	TLIBC_XML_READER_SCANNER_CONTEXT scanner_context;

	tchar buff[TLIBC_LEX_LEX_BUFF_SIZE];
	tuint32 buff_size;

	tuint32 struct_deep;

	int ignore_int32_once;

	int pre_read_uint16_field_once;
	tuint16 ui16;
}TLIBC_XML_READER;


TLIBC_ERROR_CODE xml_reader_init(TLIBC_XML_READER *self, const char *file_name);

//virtual public functions

TLIBC_API tint32 xml_read_struct_begin(TLIBC_ABSTRACT_READER *self, const char *struct_name);

TLIBC_API tint32 xml_read_struct_end(TLIBC_ABSTRACT_READER *self, const char *struct_name);

TLIBC_API tint32 xml_read_enum_begin(TLIBC_ABSTRACT_READER *self, const char *enum_name);

TLIBC_API tint32 xml_read_enum_end(TLIBC_ABSTRACT_READER *self, const char *enum_name);

TLIBC_API tint32 xml_read_vector_begin(TLIBC_ABSTRACT_READER *self);

TLIBC_API tint32 xml_read_vector_end(TLIBC_ABSTRACT_READER *self);

TLIBC_API tint32 xml_read_field_begin(TLIBC_ABSTRACT_READER *self, const char *var_name);

TLIBC_API tint32 xml_read_field_end(TLIBC_ABSTRACT_READER *self, const char *var_name);

TLIBC_API tint32 xml_read_enum_name(TLIBC_ABSTRACT_READER *super, tchar *enum_name, tuint32 enum_name_length);

TLIBC_API tint32 xml_read_tchar(TLIBC_ABSTRACT_READER *super, char *val);

TLIBC_API tint32 xml_read_tdouble(TLIBC_ABSTRACT_READER *super, double *val);

TLIBC_API tint32 xml_read_tint8(TLIBC_ABSTRACT_READER *super, tint8 *val);

TLIBC_API tint32 xml_read_tint16(TLIBC_ABSTRACT_READER *super, tint16 *val);

TLIBC_API tint32 xml_read_tint32(TLIBC_ABSTRACT_READER *super, tint32 *val);

TLIBC_API tint32 xml_read_tint64(TLIBC_ABSTRACT_READER *super, tint64 *val);

TLIBC_API tint32 xml_read_tuint8(TLIBC_ABSTRACT_READER *super, tuint8 *val);

TLIBC_API tint32 xml_read_tuint16(TLIBC_ABSTRACT_READER *super, tuint16 *val);

TLIBC_API tint32 xml_read_tuint32(TLIBC_ABSTRACT_READER *super, tuint32 *val);

TLIBC_API tint32 xml_read_tuint64(TLIBC_ABSTRACT_READER *super, tuint64 *val);

TLIBC_API tint32 xml_read_tstring(TLIBC_ABSTRACT_READER *super, tchar *str, tuint32 str_len);

TLIBC_API tint32 xml_read_vector_item_end(TLIBC_ABSTRACT_READER *super, tuint32 index);

TLIBC_API tint32 xml_read_vector_item_begin(TLIBC_ABSTRACT_READER *super, tuint32 index);

#endif

