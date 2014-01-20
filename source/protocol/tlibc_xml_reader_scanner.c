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

TLIBC_XML_READER_TOKEN tlibc_xml_reader_get_token(TLIBC_XML_READER_SCANNER_CONTEXT *self)
{
	TLIBC_XML_READER_TOKEN token = tok_end;

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
			if(self->yy_leng - 2 >= TDATA_MAX_LENGTH_OF_IDENTIFIER)
			{
				goto ERROR_RET;
			}
			memcpy(self->tag_name, self->yy_text + 1, self->yy_leng - 2);
			self->tag_name[self->yy_leng - 2] = 0;

			//记录值开始的位置
			self->content_begin = self->yy_cursor;			
			break;
		}
	case tok_tag_end:
		{
			if(self->yy_leng - 3 >= TDATA_MAX_LENGTH_OF_IDENTIFIER)
			{
				goto ERROR_RET;
			}
			memcpy(self->tag_name, self->yy_text + 2, self->yy_leng - 3);
			self->tag_name[self->yy_leng - 3] = 0;
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
