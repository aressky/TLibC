#ifndef _H_TLIBC_XML_READER_SCANNER_H
#define _H_TLIBC_XML_READER_SCANNER_H

#include "protocol/tlibc_xml_reader.h"

#define YYSTYPE TLIBC_XML_READER_SCANNER_CONTEXT
#define YYLTYPE_IS_DECLARED
#define YYCTYPE   char
#define YYFILL(n) 
#define YYCURSOR  sp->yy_cursor
#define YYLIMIT   sp->yy_limit
#define YYMARKER sp->yy_marker
#define YYGETCONDITION()  sp->yy_state
#define YYSETCONDITION(s) sp->yy_state = s
#define yytext sp->yy_text
#define yyleng sp->yy_leng

#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYSTATE      YYGETCONDITION()


typedef enum _TLIBC_XML_READER_TOKEN
{
	tok_end = 0,						//½âÎö½áÊø
	tok_error = 1,						//½âÎö´íÎó
	tok_tag_begin = 2,
	tok_tag_end = 3,
}TLIBC_XML_READER_TOKEN;

TLIBC_XML_READER_TOKEN tlibc_xml_reader_scan(TLIBC_XML_READER *self);

void tlibc_xml_reader_locate(TLIBC_XML_READER *self);

TLIBC_XML_READER_TOKEN tlibc_xml_reader_get_token(TLIBC_XML_READER *self);

#endif//_H_TLIBC_XML_READER_SCANNER_H

