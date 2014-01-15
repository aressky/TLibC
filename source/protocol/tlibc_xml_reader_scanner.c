#include "tlibc_xml_reader_scanner.h"

#include <string.h>

void tlibc_xml_reader_locate(TLIBC_XML_READER_SCANNER_CONTEXT *self)
{
	const char *i;
	YYSTYPE *sp = self;
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

int tlibc_xml_reader_get_token(TLIBC_XML_READER_SCANNER_CONTEXT *self)
{
	tchar *iter;
	int token = tok_end;

	token = tlibc_xml_reader_scan(self);
	if(token == tok_end)
	{
		goto done;
	}
	self->yylloc.last_line = self->yylineno;
	self->yylloc.last_column = self->yycolumn;

	switch(token)
	{
	case tok_tag_begin:
		{
			if(self->yy_leng - 2 >= TLIBC_MAX_IDENTIFIER_LENGTH)
			{
				goto ERROR_RET;
			}
			memcpy(self->tag_name, self->yy_text + 1, self->yy_leng - 2);
			self->tag_name[self->yy_leng - 2] = 0;

			//¼ÇÂ¼content
			self->content_begin = self->yy_cursor;
			for(iter = self->content_begin; iter < self->yy_limit; ++iter)
			{
				if(*iter == '<')
				{
					self->content_end = iter;
					break;
				}
			}
			if(*iter != '<')
			{
				goto ERROR_RET;
			}
		}
	case tok_tag_end:
		{
			if(self->yy_leng - 3 >= TLIBC_MAX_IDENTIFIER_LENGTH)
			{
				goto ERROR_RET;
			}
			memcpy(self->tag_name, self->yy_text + 2, self->yy_leng - 3);
			self->tag_name[self->yy_leng - 3] = 0;
			break;
		}
	}
done:
	return token;

ERROR_RET:
	return tok_error;
}
