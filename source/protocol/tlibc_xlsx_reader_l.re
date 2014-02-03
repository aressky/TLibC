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

void xpos2pos(tlibc_xlsx_pos *self, const char* xpos)
{
	self->col = 0;
	while(*xpos >='A')
	{
		self->col *= 26;
		self->col += *xpos - 'A';
		++xpos;
	}

	self->row = 0;
	while(*xpos != 0)
	{
		self->row *= 10;
		self->row += *xpos - '0';
		++xpos;
	}
}

TLIBC_ERROR_CODE tlibc_xlsx_reader_loadsheet(tlibc_xlsx_reader_t *self)
{
	tlibc_xlsx_cell_s *cell = NULL;

	self->scanner.cursor = self->sheet_buff;
	self->scanner.limit = self->sheet_buff + self->sheet_buff_size;
	self->scanner.marker = self->scanner.cursor;
	self->scanner.state = yycINITIAL;
	self->cell_matrix = NULL;

restart:
	if(self->scanner.cursor >= self->scanner.limit)
	{
		return E_TLIBC_NOERROR;
	}
/*!re2c
<INITIAL>"<dimension ref=\""
{
	char *size_min = YYCURSOR;
	char *size_max = NULL;

	
	while(*YYCURSOR != '"')
	{
		if(*YYCURSOR == ':')
		{
			*YYCURSOR = 0;
			++YYCURSOR;
			size_max = YYCURSOR;
		}
		else
		{
			++YYCURSOR;
		}
	}
	*YYCURSOR = 0;
	++YYCURSOR;

	xpos2pos(&self->cell_min_pos, size_min);
	xpos2pos(&self->cell_max_pos, size_max);
	self->cell_row_size = (self->cell_max_pos.row - self->cell_min_pos.row + 1);
	self->cell_col_size = (self->cell_max_pos.col - self->cell_min_pos.col + 1);
	self->cell_matrix = malloc(sizeof(tlibc_xlsx_cell_s) * self->cell_row_size * self->cell_col_size);
	if(self->cell_matrix == NULL)
	{
		return E_TLIBC_ERROR;
	}
	goto restart;
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
<IN_ROW>"<c r=\""
{
	const char* xpos = YYCURSOR;
	tlibc_xlsx_pos pos;
	while(*YYCURSOR != '"')
	{
		++YYCURSOR;
	}
	*YYCURSOR = 0;
	++YYCURSOR;

	xpos2pos(&pos, xpos);
	cell = self->cell_matrix + (pos.row - self->cell_min_pos.row) * self->cell_col_size + pos.col;
	cell->xpos = xpos;

	while(*YYCURSOR != '>')
	{
		++YYCURSOR;
	}
	++YYCURSOR;
	goto restart;
}
<IN_ROW>"<v>"						{ cell->val_begin = YYCURSOR; goto restart;}
<IN_ROW>"</v>"						{ cell->val_end = YYCURSOR - 4; goto restart;}
<IN_ROW>"</row>"					{ BEGIN(IN_SHEETDATA); goto restart; }
<IN_SHEETDATA>"</sheetData>"		{ BEGIN(INITIAL); goto restart;		}
<INITIAL>"</sheetData>"				{ BEGIN(INITIAL);goto restart;		}
<*>[^]								{ goto restart;}
*/
}
