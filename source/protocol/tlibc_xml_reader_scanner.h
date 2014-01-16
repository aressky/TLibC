#ifndef _H_TLIBC_XML_READER_SCANNER_H
#define _H_TLIBC_XML_READER_SCANNER_H

#include "protocol/tlibc_xml_reader.h"

#define YYSTYPE TLIBC_XML_READER_SCANNER_CONTEXT
#define YYLTYPE_IS_DECLARED
#define YYCTYPE   char
#define YYFILL(n) 
#define YYCURSOR  self->yy_cursor
#define YYLIMIT   self->yy_limit
#define YYMARKER self->yy_marker
#define YYGETCONDITION()  self->yy_state
#define YYSETCONDITION(s) self->yy_state = s
#define yytext self->yy_text
#define yyleng self->yy_leng

#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYSTATE      YYGETCONDITION()


typedef enum _TLIBC_XML_READER_TOKEN
{
	tok_end = 0,						//½âÎö½áÊø
	tok_error = 0,						//½âÎö´íÎó
	tok_tag_begin = 1,
	tok_tag_end = 2,
}TLIBC_XML_READER_TOKEN;

TLIBC_XML_READER_TOKEN tlibc_xml_reader_scan(TLIBC_XML_READER_SCANNER_CONTEXT *self);

void tlibc_xml_reader_locate(TLIBC_XML_READER_SCANNER_CONTEXT *self);

int tlibc_xml_reader_get_token(TLIBC_XML_READER_SCANNER_CONTEXT *self);

TLIBC_ERROR_CODE tlibc_xml_reader_get_content(TLIBC_XML_READER_SCANNER_CONTEXT *self);

#endif//_H_TLIBC_XML_READER_SCANNER_H

