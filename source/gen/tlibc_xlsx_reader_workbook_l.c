/* Generated by re2c 0.13.5 on Wed Apr 09 15:25:09 2014 */
#line 1 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_workbook_l.re"
#include "protocol/tlibc_xlsx_reader.h"
#include "tlibc_xlsx_reader_workbook_l.h"
#include <string.h>

#define YYGETCONDITION()  self->scanner.state
#define YYSETCONDITION(s) self->scanner.state = s
#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYCURSOR self->scanner.cursor
#define YYLIMIT self->scanner.limit
#define YYMARKER self->scanner.marker
#define YYCTYPE char

const char* tlibc_xlsx_reader_workbook_search_rid(tlibc_xlsx_reader_t *self, const char* sheet)
{
	const char* rid_begin = NULL;
	const char* name_begin = NULL;

	self->scanner.cursor = self->workbook_buff;
	self->scanner.limit = self->workbook_buff + self->workbook_buff_size;
	self->scanner.marker = NULL;
	self->scanner.state = yycINITIAL;

restart:
	if(YYCURSOR >= YYLIMIT)
	{
		return NULL;
	}


#line 34 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_workbook_l.c"
{
	YYCTYPE yych;
	switch (YYGETCONDITION()) {
	case yycINITIAL: goto yyc_INITIAL;
	case yycIN_SHEETS: goto yyc_IN_SHEETS;
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
#line 65 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_workbook_l.re"
	{ goto restart;				}
#line 54 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_workbook_l.c"
yy4:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '/':	goto yy5;
	case 'S':
	case 's':	goto yy7;
	default:	goto yy3;
	}
yy5:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'S':
	case 's':	goto yy15;
	default:	goto yy6;
	}
yy6:
	YYCURSOR = YYMARKER;
	goto yy3;
yy7:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'H':
	case 'h':	goto yy8;
	default:	goto yy6;
	}
yy8:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy9;
	default:	goto yy6;
	}
yy9:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy10;
	default:	goto yy6;
	}
yy10:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'T':
	case 't':	goto yy11;
	default:	goto yy6;
	}
yy11:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'S':
	case 's':	goto yy12;
	default:	goto yy6;
	}
yy12:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy13;
	default:	goto yy6;
	}
yy13:
	++YYCURSOR;
#line 33 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_workbook_l.re"
	{ BEGIN(IN_SHEETS);goto restart;}
#line 118 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_workbook_l.c"
yy15:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'H':
	case 'h':	goto yy16;
	default:	goto yy6;
	}
yy16:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy17;
	default:	goto yy6;
	}
yy17:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy18;
	default:	goto yy6;
	}
yy18:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'T':
	case 't':	goto yy19;
	default:	goto yy6;
	}
yy19:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'S':
	case 's':	goto yy20;
	default:	goto yy6;
	}
yy20:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy21;
	default:	goto yy6;
	}
yy21:
	++YYCURSOR;
#line 61 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_workbook_l.re"
	{
	BEGIN(INITIAL);
	goto restart;
}
#line 167 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_workbook_l.c"
/* *********************************** */
yyc_IN_SHEETS:
	yych = *YYCURSOR;
	switch (yych) {
	case '"':	goto yy27;
	case 'N':
	case 'n':	goto yy28;
	case 'R':
	case 'r':	goto yy29;
	default:	goto yy25;
	}
yy25:
	++YYCURSOR;
yy26:
#line 65 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_workbook_l.re"
	{ goto restart;				}
#line 184 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_workbook_l.c"
yy27:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '/':	goto yy43;
	default:	goto yy26;
	}
yy28:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case 'A':
	case 'a':	goto yy37;
	default:	goto yy26;
	}
yy29:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case ':':	goto yy30;
	default:	goto yy26;
	}
yy30:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'I':
	case 'i':	goto yy32;
	default:	goto yy31;
	}
yy31:
	YYCURSOR = YYMARKER;
	goto yy26;
yy32:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'D':
	case 'd':	goto yy33;
	default:	goto yy31;
	}
yy33:
	yych = *++YYCURSOR;
	switch (yych) {
	case '=':	goto yy34;
	default:	goto yy31;
	}
yy34:
	yych = *++YYCURSOR;
	switch (yych) {
	case '"':	goto yy35;
	default:	goto yy31;
	}
yy35:
	++YYCURSOR;
#line 35 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_workbook_l.re"
	{
	rid_begin = YYCURSOR;
	while(*YYCURSOR != '\"')
		++YYCURSOR;
	*YYCURSOR = 0;
	++YYCURSOR;
	YYMARKER = YYCURSOR;
	goto restart;
}
#line 245 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_workbook_l.c"
yy37:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'M':
	case 'm':	goto yy38;
	default:	goto yy31;
	}
yy38:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'E':
	case 'e':	goto yy39;
	default:	goto yy31;
	}
yy39:
	yych = *++YYCURSOR;
	switch (yych) {
	case '=':	goto yy40;
	default:	goto yy31;
	}
yy40:
	yych = *++YYCURSOR;
	switch (yych) {
	case '"':	goto yy41;
	default:	goto yy31;
	}
yy41:
	++YYCURSOR;
#line 45 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_workbook_l.re"
	{
	name_begin = YYCURSOR;
	while(*YYCURSOR != '\"')
		++YYCURSOR;
	*YYCURSOR = 0;
	++YYCURSOR;
	YYMARKER = YYCURSOR;
	goto restart;
}
#line 284 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_workbook_l.c"
yy43:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy44;
	default:	goto yy31;
	}
yy44:
	++YYCURSOR;
#line 55 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_workbook_l.re"
	{
	if((sheet == NULL) || (strcmp(name_begin, sheet) == 0))
		return rid_begin;
	goto restart;
}
#line 299 "D:/GitHub/TLibC/source/gen/tlibc_xlsx_reader_workbook_l.c"
}
#line 66 "D:/GitHub/TLibC/source/protocol/tlibc_xlsx_reader_workbook_l.re"

}



