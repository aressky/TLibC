#include "tlibc/protocol/tlibc_xlsx_reader.h"
#include "tlibc_xlsx_reader_l.h"

#define YYGETCONDITION()  self->scanner.state
#define YYSETCONDITION(s) self->scanner.state = s
#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYCURSOR self->scanner.cursor
#define YYLIMIT self->scanner.limit
#define YYMARKER self->scanner.marker
#define YYCTYPE char
/*!re2c
re2c:yyfill:enable   = 0;
*/

void tlibc_xlsx_reader_loadsheet(tlibc_xlsx_reader_t *self)
{
	self->scanner.cursor = self->sheet_buff;
	self->scanner.limit = self->sheet_buff + self->sheet_buff_size;
	self->scanner.marker = self->scanner.cursor;
	self->scanner.state = yycINITIAL;

restart:
	if(self->scanner.cursor >= self->scanner.limit)
	{
		return;
	}
/*!re2c
<INITIAL>"<dimension ref=\""
{
	char *size = YYCURSOR;
	while(*YYCURSOR != '"')
	{
		++YYCURSOR;
	}
	*YYCURSOR = 0;
	++YYCURSOR;
}
<INITIAL>"<sheetData>"				{ BEGIN(IN_SHEETDATA);goto restart;	}
<IN_SHEETDATA>"<row"
{
	int single_field = FALSE;
	while(*YYCURSOR != '>')
	{
		if(*YYCURSOR == '/')
		{
			single_field = TRUE;
		}
		++YYCURSOR;
	}
	++YYCURSOR;
	YYMARKER = YYCURSOR;
	if(single_field)
	{
		goto restart;
	}

	BEGIN(IN_ROW);
	goto restart;
}
<IN_ROW>"</row>"					{ BEGIN(IN_SHEETDATA); goto restart; }
<IN_SHEETDATA>"</sheetData>"		{ BEGIN(INITIAL); goto restart;		}
<INITIAL>"</sheetData>"				{ BEGIN(INITIAL);goto restart;		}
<*>[^]								{ goto restart;}
*/
}



