#include "platform/tlibc_platform.h"
#include "protocol/tlibc_json_reader.h"
#include "protocol/tlibc_json_reader_scanner.h"
#include "tlibc_json_reader_l.h"
#include <string.h>




		
tlibc_json_reader_token_t tlibc_json_reader_scan(tlibc_json_reader_t *self)
{
	tlibc_json_reader_scanner_context_t *sp = NULL;
	self->error_code = E_TLIBC_NOERROR;
restart:
	sp = &self->scanner_context_stack;
	if(YYCURSOR >= YYLIMIT)
	{
		return tok_end;
	}
	sp->yylloc.first_line = sp->yylineno;
	sp->yylloc.first_column = sp->yycolumn;
	yytext = YYCURSOR;
/*!re2c
re2c:yyfill:check = 0;

identifier		([a-zA-Z_][a-zA-Z_0-9]*)
comment			("<!--")
anychar			([^])



<!*> := yyleng = YYCURSOR - yytext; tlibc_json_reader_locate(self);

<*>{comment}
{
	const char * comment_begin = NULL;
	for(comment_begin = YYCURSOR; YYCURSOR != YYLIMIT; ++YYCURSOR)
	{
		if(YYCURSOR - comment_begin >= 2)
		{
			if((*(YYCURSOR - 2) == '-') && (*(YYCURSOR - 1) == '-') && (*(YYCURSOR - 0) == '>'))
			{
				goto restart;
			}
		}
	}
	goto restart;
}

<INITIAL>'<' {identifier} '>'			{return tok_tag_begin;															}
<INITIAL>'<' '/' {identifier}'>'		{return tok_tag_end;															}

<*>{anychar}							{ goto restart;					 }	
*/	
}
