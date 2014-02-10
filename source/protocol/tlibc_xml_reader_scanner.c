#include "tlibc_xml_reader_scanner.h"

#include <string.h>

void tlibc_xml_reader_locate(TLIBC_XML_READER *self)
{
	const char *i;
	TLIBC_XML_READER_SCANNER_CONTEXT *sp = &self->scanner_context_stack[self->scanner_context_stack_num - 1];
	for(i = sp->yy_last; i < sp->yy_cursor;++i)
	{
		if(*i == '\n')
		{
			++(sp->yylineno);
			sp->yycolumn = 1;
		}		
		else if(*i == '\r')
		{
			sp->yycolumn = 1;		
		}
		else
		{
			++(sp->yycolumn);
		}
	}
	sp->yy_last = sp->yy_cursor;
}

TLIBC_XML_READER_TOKEN tlibc_xml_reader_get_token(TLIBC_XML_READER *self)
{
	TLIBC_XML_READER_TOKEN token = tok_end;
	TLIBC_XML_READER_SCANNER_CONTEXT *sp = NULL;

	token = tlibc_xml_reader_scan(self);
	if(token == tok_end)
	{
		goto done;
	}
	sp = &self->scanner_context_stack[self->scanner_context_stack_num - 1];
	sp->yylloc.last_line = sp->yylineno;
	sp->yylloc.last_column = sp->yycolumn;

	switch(token)
	{
	case tok_tag_begin:
		{
			if(sp->yy_leng - 2 >= TDATA_MAX_LENGTH_OF_IDENTIFIER)
			{
				goto ERROR_RET;
			}
			memcpy(sp->tag_name, sp->yy_text + 1, sp->yy_leng - 2);
			sp->tag_name[sp->yy_leng - 2] = 0;

			//记录值开始的位置
			sp->content_begin = sp->yy_cursor;			
			break;
		}
	case tok_tag_end:
		{
			if(sp->yy_leng - 3 >= TDATA_MAX_LENGTH_OF_IDENTIFIER)
			{
				goto ERROR_RET;
			}
			memcpy(sp->tag_name, sp->yy_text + 2, sp->yy_leng - 3);
			sp->tag_name[sp->yy_leng - 3] = 0;
			break;
		}
	default:
		goto ERROR_RET;
	}
done:
	return token;

ERROR_RET:
	return tok_error;
}
