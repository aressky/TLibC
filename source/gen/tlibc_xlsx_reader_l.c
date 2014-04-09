/* Generated by re2c 0.13.5 on Wed Apr 09 15:25:09 2014 */
#line 1 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
#include "protocol/tlibc_xlsx_reader.h"
#include "tlibc_xlsx_reader_l.h"
#include <errno.h>

#define YYGETCONDITION()  self->scanner.state
#define YYSETCONDITION(s) self->scanner.state = s
#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYCURSOR self->scanner.cursor
#define YYLIMIT self->scanner.limit
#define YYMARKER self->scanner.marker
#define YYCTYPE char
#line 15 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"


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

tlibc_error_code_t tlibc_xlsx_reader_loadsheet(tlibc_xlsx_reader_t *self, uint32_t bindinfo_row)
{
	tlibc_xlsx_cell_s *cell = NULL;
	int is_sharedstring = FALSE;
	tlibc_xlsx_cell_s *current_row = NULL;

	self->scanner.cursor = self->sheet_buff;
	self->scanner.limit = self->sheet_buff + self->sheet_buff_size;
	self->scanner.marker = self->scanner.cursor;
	self->scanner.state = yycINITIAL;
	self->cell_matrix = NULL;
	self->bindinfo_row = NULL;

restart:
	if(self->scanner.cursor >= self->scanner.limit)
	{
		if(self->bindinfo_row == NULL)
		{
			goto ERROR_RET;
		}
		return E_TLIBC_NOERROR;
	}

#line 61 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
{
	YYCTYPE yych;
	switch (YYGETCONDITION()) {
	case yycINITIAL: goto yyc_INITIAL;
	case yycIN_COL: goto yyc_IN_COL;
	case yycIN_ROW: goto yyc_IN_ROW;
	case yycIN_SHEETDATA: goto yyc_IN_SHEETDATA;
	}
/* *********************************** */
yyc_INITIAL:

	yych = *YYCURSOR;
	switch (yych) {
	case '<':	goto yy4;
	default:	goto yy2;
	}
yy2:
	++YYCURSOR;
yy3:
#line 204 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{ goto restart;}
#line 83 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy4:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '/':	goto yy5;
	case 'D':
	case 'd':	goto yy8;
	case 'S':
	case 's':	goto yy7;
	default:	goto yy3;
	}
yy5:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'S':
	case 's':	goto yy34;
	default:	goto yy6;
	}
yy6:
	YYCURSOR = YYMARKER;
	goto yy3;
yy7:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'H':
	case 'h':	goto yy24;
	default:	goto yy6;
	}
yy8:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'I':
	case 'i':	goto yy9;
	default:	goto yy6;
	}
yy9:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'M':
	case 'm':	goto yy10;
	default:	goto yy6;
	}
yy10:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy11;
	default:	goto yy6;
	}
yy11:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'N':
	case 'n':	goto yy12;
	default:	goto yy6;
	}
yy12:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'S':
	case 's':	goto yy13;
	default:	goto yy6;
	}
yy13:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'I':
	case 'i':	goto yy14;
	default:	goto yy6;
	}
yy14:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'O':
	case 'o':	goto yy15;
	default:	goto yy6;
	}
yy15:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'N':
	case 'n':	goto yy16;
	default:	goto yy6;
	}
yy16:
	++YYCURSOR;
	yych = *YYCURSOR;
	switch (yych) {
	case '\t':
	case ' ':	goto yy16;
	case 'R':
	case 'r':	goto yy18;
	default:	goto yy6;
	}
yy18:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy19;
	default:	goto yy6;
	}
yy19:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'F':
	case 'f':	goto yy20;
	default:	goto yy6;
	}
yy20:
	yych = *++YYCURSOR;
	switch (yych) {
	case '=':	goto yy21;
	default:	goto yy6;
	}
yy21:
	yych = *++YYCURSOR;
	switch (yych) {
	case '"':	goto yy22;
	default:	goto yy6;
	}
yy22:
	++YYCURSOR;
#line 60 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{
	uint32_t i;
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
		goto ERROR_RET;
	}
	for(i = 0; i < self->cell_row_size * self->cell_col_size; ++i)
	{
		self->cell_matrix[i].empty = TRUE;
	}
	goto restart;
}
#line 243 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy24:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy25;
	default:	goto yy6;
	}
yy25:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy26;
	default:	goto yy6;
	}
yy26:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'T':
	case 't':	goto yy27;
	default:	goto yy6;
	}
yy27:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'D':
	case 'd':	goto yy28;
	default:	goto yy6;
	}
yy28:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'A':
	case 'a':	goto yy29;
	default:	goto yy6;
	}
yy29:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'T':
	case 't':	goto yy30;
	default:	goto yy6;
	}
yy30:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'A':
	case 'a':	goto yy31;
	default:	goto yy6;
	}
yy31:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy32;
	default:	goto yy6;
	}
yy32:
	++YYCURSOR;
#line 97 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{ BEGIN(IN_SHEETDATA);goto restart;	}
#line 303 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy34:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'H':
	case 'h':	goto yy35;
	default:	goto yy6;
	}
yy35:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy36;
	default:	goto yy6;
	}
yy36:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy37;
	default:	goto yy6;
	}
yy37:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'T':
	case 't':	goto yy38;
	default:	goto yy6;
	}
yy38:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'D':
	case 'd':	goto yy39;
	default:	goto yy6;
	}
yy39:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'A':
	case 'a':	goto yy40;
	default:	goto yy6;
	}
yy40:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'T':
	case 't':	goto yy41;
	default:	goto yy6;
	}
yy41:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'A':
	case 'a':	goto yy42;
	default:	goto yy6;
	}
yy42:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy43;
	default:	goto yy6;
	}
yy43:
	++YYCURSOR;
#line 203 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{ BEGIN(INITIAL);goto restart;		}
#line 370 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
/* *********************************** */
yyc_IN_COL:
	yych = *YYCURSOR;
	switch (yych) {
	case '/':	goto yy49;
	case '<':	goto yy50;
	case 'R':
	case 'r':	goto yy51;
	case 'T':
	case 't':	goto yy52;
	default:	goto yy47;
	}
yy47:
	++YYCURSOR;
yy48:
#line 204 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{ goto restart;}
#line 388 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy49:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy70;
	default:	goto yy48;
	}
yy50:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '/':	goto yy60;
	case 'V':
	case 'v':	goto yy61;
	default:	goto yy48;
	}
yy51:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '=':	goto yy57;
	default:	goto yy48;
	}
yy52:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '=':	goto yy53;
	default:	goto yy48;
	}
yy53:
	yych = *++YYCURSOR;
	switch (yych) {
	case '"':	goto yy55;
	default:	goto yy54;
	}
yy54:
	YYCURSOR = YYMARKER;
	goto yy48;
yy55:
	++YYCURSOR;
#line 165 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{
	if((*YYCURSOR == 's') && (*(YYCURSOR + 1) == '"'))
	{
		is_sharedstring = TRUE;
	}	
	
	while(*YYCURSOR != '>')
	{
		++YYCURSOR;
	}
	++YYCURSOR;
}
#line 439 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy57:
	yych = *++YYCURSOR;
	switch (yych) {
	case '"':	goto yy58;
	default:	goto yy54;
	}
yy58:
	++YYCURSOR;
#line 147 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
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
	cell = current_row + (pos.col - self->cell_min_pos.col);
	cell->empty = FALSE;
	cell->xpos = xpos;
		
	goto restart;
}
#line 466 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy60:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'C':
	case 'c':	goto yy64;
	case 'V':
	case 'v':	goto yy65;
	default:	goto yy54;
	}
yy61:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy62;
	default:	goto yy54;
	}
yy62:
	++YYCURSOR;
#line 199 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{ cell->val_begin = YYCURSOR; goto restart;}
#line 486 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy64:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy68;
	default:	goto yy54;
	}
yy65:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy66;
	default:	goto yy54;
	}
yy66:
	++YYCURSOR;
#line 200 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{ cell->val_end = YYCURSOR - 4; *(YYCURSOR - 4)= 0; goto restart;}
#line 503 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy68:
	++YYCURSOR;
#line 183 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{
	if(is_sharedstring)
	{
		uint32_t string_index;
		errno = 0;
		string_index = strtoul(cell->val_begin, NULL, 10);
		if(errno != 0)
		{
			goto ERROR_RET;
		}
		cell->val_begin = self->sharedstring_begin_list[string_index];
		cell->val_end = self->sharedstring_end_list[string_index];
	}
	BEGIN(IN_ROW);
	goto restart;
}
#line 523 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy70:
	++YYCURSOR;
#line 178 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{
	BEGIN(IN_ROW);
	goto restart;
}
#line 531 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
/* *********************************** */
yyc_IN_ROW:
	yych = *YYCURSOR;
	switch (yych) {
	case '<':	goto yy76;
	default:	goto yy74;
	}
yy74:
	++YYCURSOR;
yy75:
#line 204 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{ goto restart;}
#line 544 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy76:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '/':	goto yy77;
	case 'C':
	case 'c':	goto yy79;
	default:	goto yy75;
	}
yy77:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'R':
	case 'r':	goto yy81;
	default:	goto yy78;
	}
yy78:
	YYCURSOR = YYMARKER;
	goto yy75;
yy79:
	++YYCURSOR;
#line 139 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{
	cell = NULL;
	is_sharedstring = FALSE;

	BEGIN(IN_COL);	
	goto restart;
}
#line 573 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy81:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'O':
	case 'o':	goto yy82;
	default:	goto yy78;
	}
yy82:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'W':
	case 'w':	goto yy83;
	default:	goto yy78;
	}
yy83:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy84;
	default:	goto yy78;
	}
yy84:
	++YYCURSOR;
#line 201 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{ BEGIN(IN_SHEETDATA); goto restart; }
#line 598 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
/* *********************************** */
yyc_IN_SHEETDATA:
	yych = *YYCURSOR;
	switch (yych) {
	case '<':	goto yy90;
	default:	goto yy88;
	}
yy88:
	++YYCURSOR;
yy89:
#line 204 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{ goto restart;}
#line 611 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy90:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '/':	goto yy91;
	case 'R':
	case 'r':	goto yy93;
	default:	goto yy89;
	}
yy91:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'S':
	case 's':	goto yy101;
	default:	goto yy92;
	}
yy92:
	YYCURSOR = YYMARKER;
	goto yy89;
yy93:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'O':
	case 'o':	goto yy94;
	default:	goto yy92;
	}
yy94:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'W':
	case 'w':	goto yy95;
	default:	goto yy92;
	}
yy95:
	++YYCURSOR;
	yych = *YYCURSOR;
	switch (yych) {
	case '\t':
	case ' ':	goto yy95;
	case 'R':
	case 'r':	goto yy97;
	default:	goto yy92;
	}
yy97:
	yych = *++YYCURSOR;
	switch (yych) {
	case '=':	goto yy98;
	default:	goto yy92;
	}
yy98:
	yych = *++YYCURSOR;
	switch (yych) {
	case '"':	goto yy99;
	default:	goto yy92;
	}
yy99:
	++YYCURSOR;
#line 99 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{
	const char *r = YYCURSOR;
	uint32_t row;
	int is_single = FALSE;
	while(*YYCURSOR != '"')
	{
		++YYCURSOR;
	}
	*YYCURSOR = 0;
	errno = 0;
	row = strtoul(r, NULL, 10);
	if(errno != 0)
	{
		goto ERROR_RET;
	}
	current_row = self->cell_matrix + (row - self->cell_min_pos.row) * self->cell_col_size;

	while(*YYCURSOR != '>')
	{
		if(*YYCURSOR == '/')
		{
			is_single = TRUE;
		}
		++YYCURSOR;
	}
	++YYCURSOR;
	
	if(is_single)
	{
		goto restart;
	}

	if(row == bindinfo_row)
	{
		self->bindinfo_row = current_row;
	}
	BEGIN(IN_ROW);
	goto restart;
}
#line 708 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
yy101:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'H':
	case 'h':	goto yy102;
	default:	goto yy92;
	}
yy102:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy103;
	default:	goto yy92;
	}
yy103:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy104;
	default:	goto yy92;
	}
yy104:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'T':
	case 't':	goto yy105;
	default:	goto yy92;
	}
yy105:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'D':
	case 'd':	goto yy106;
	default:	goto yy92;
	}
yy106:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'A':
	case 'a':	goto yy107;
	default:	goto yy92;
	}
yy107:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'T':
	case 't':	goto yy108;
	default:	goto yy92;
	}
yy108:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'A':
	case 'a':	goto yy109;
	default:	goto yy92;
	}
yy109:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy110;
	default:	goto yy92;
	}
yy110:
	++YYCURSOR;
#line 202 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"
	{ BEGIN(INITIAL); goto restart;		}
#line 775 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_l.c"
}
#line 205 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_l.re"

ERROR_RET:
	if(self->cell_matrix != NULL)
	{
		free(self->cell_matrix);
	}
	return E_TLIBC_ERROR;
}
