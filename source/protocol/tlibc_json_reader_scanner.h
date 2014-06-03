#ifndef _H_TLIBC_JSON_READER_SCANNER_H
#define _H_TLIBC_JSON_READER_SCANNER_H

#include "protocol/tlibc_json_reader.h"

#define YYSTYPE tlibc_json_reader_scanner_context_t
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


typedef enum tlibc_json_reader_token_e
{
	tok_end = 0,						//½âÎö½áÊø
	tok_error = 1,						//½âÎö´íÎó
	tok_tag_begin = 2,
	tok_tag_end = 3,
}tlibc_json_reader_token_t;

tlibc_json_reader_token_t tlibc_json_reader_scan(tlibc_json_reader_t *self);

void tlibc_json_reader_locate(tlibc_json_reader_t *self);

tlibc_json_reader_token_t tlibc_json_reader_get_token(tlibc_json_reader_t *self);

#endif//_H_TLIBC_JSON_READER_SCANNER_H

