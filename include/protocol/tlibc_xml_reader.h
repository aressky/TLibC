#ifndef _H_TLIBC_XML_READER
#define _H_TLIBC_XML_READER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_reader.h"
#include <stdio.h>
#include "lib/tlibc_error_code.h"
#include "tdata/tdata_types.h"

typedef struct _YYLTYPE
{
	char file_name[TLIBC_MAX_FILE_PATH_LENGTH];
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} YYLTYPE;

typedef union _XML_TOKEN_VALUE XML_TOKEN_VALUE;
union _XML_TOKEN_VALUE
{
	char tag_start[TLIBC_MAX_IDENTIFIER_LENGTH];
	char tag_end[TLIBC_MAX_IDENTIFIER_LENGTH];
};

#define YYSTYPE SCANNER_TOKEN_VALUE
#define YYLTYPE_IS_DECLARED
#define YYCTYPE   char
#define YYFILL(n) 
#define YYCURSOR  self->scanner_context.yy_cursor
#define YYLIMIT   self->scanner_context.yy_limit
#define YYMARKER self->scanner_context.yy_marker
#define YYGETCONDITION()  self->scanner_context.yy_state
#define YYSETCONDITION(s) self->scanner_context.yy_state = s
#define yytext self->scanner_context.yy_text
#define yyleng self->scanner_context.yy_leng

#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYSTATE      YYGETCONDITION()

typedef struct _SCANNER_CONTEXT SCANNER_CONTEXT;
struct _SCANNER_CONTEXT
{
	int yy_state;
	YYCTYPE *yy_last;
	YYCTYPE *yy_cursor;
	YYCTYPE *yy_limit;
	YYCTYPE *yy_text;
	YYCTYPE *yy_marker;
	YYCTYPE *yy_start;
	tuint32 yy_leng;

	tuint32 yylineno;
	tuint32 yycolumn;
};

enum EN_TOKEN
{
	tok_end = 0,						//½âÎö½áÊø
	tok_error = 0,						//½âÎö´íÎó
	tok_tag_begin = 1,
	tok_tag_end = 2,
};

#define MAX_LEX_BUFF_SIZE 10000000
#define MAX_SCANNER_STACK_SIZE 1024

typedef struct _TLIBC_XML_READER
{
	TLIBC_ABSTRACT_READER super;

	SCANNER_CONTEXT scanner_context;

	YYCTYPE *buff_limit;
	YYCTYPE *buff_curr;
	YYCTYPE buff[MAX_LEX_BUFF_SIZE];

	YYCTYPE *content_begin;
	YYCTYPE *content_end;

	YYLTYPE yylloc;

	tuint32 level;
}TLIBC_XML_READER;


TLIBC_ERROR_CODE xml_reader_init(TLIBC_XML_READER *self, const char *file_name);

//virtual public functions

TLIBC_API tint32 xml_read_struct_begin(TLIBC_ABSTRACT_READER *self, const char *struct_name);

TLIBC_API tint32 xml_read_struct_end(TLIBC_ABSTRACT_READER *self, const char *struct_name);

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

void xml_locate(TLIBC_XML_READER *self);

tint32 tlibc_xml_scan(TLIBC_XML_READER *self, YYLTYPE *yylloc);

int xml_scan(TLIBC_XML_READER *self, XML_TOKEN_VALUE *token_value);

#endif

