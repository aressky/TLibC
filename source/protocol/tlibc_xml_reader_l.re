#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_xml_reader.h"
#include "protocol/tlibc_xml_reader_scanner.h"
#include "tlibc_xml_reader_l.h"
#include <string.h>




		
TLIBC_XML_READER_TOKEN tlibc_xml_reader_scan(TLIBC_XML_READER *self)
{
	TLIBC_XML_READER_SCANNER_CONTEXT *sp = NULL;
restart:
	sp = &self->scanner_context_stack[self->scanner_context_stack_num - 1];
	if(YYCURSOR >= YYLIMIT)
	{
		if(self->scanner_context_stack_num > 0)
		{
			--self->scanner_context_stack_num;
			goto restart;
		}
		return tok_end;
	}
	sp->yylloc.first_line = sp->yylineno;
	sp->yylloc.first_column = sp->yycolumn;
	yytext = YYCURSOR;
/*!re2c
re2c:yyfill:check = 0;

identifier		([a-zA-Z_][a-zA-Z_0-9]*)
comment			("<!--".*"-->")
anychar			([^])


<!*> := yyleng = YYCURSOR - yytext; tlibc_xml_reader_locate(self);

<*>{comment}							{	goto restart;																}
<INITIAL>'<!include'
{
	const char* file_begin;
	const char* file_end;
	size_t file_len;
	char file[TLIBC_MAX_PATH_LENGTH];

	for(file_begin = NULL; YYCURSOR != YYLIMIT; ++YYCURSOR)
	{
		if(*YYCURSOR == '"')
		{
			++YYCURSOR;
			file_begin = YYCURSOR;
			break;
		}
	}
	if(file_begin == NULL)
	{
		return tok_error;
	}
	for(file_end = NULL; YYCURSOR != YYLIMIT; ++YYCURSOR)
	{
		if(*YYCURSOR == '"')
		{			
			file_end = YYCURSOR;
			++YYCURSOR;
			break;
		}		
	}
	if(file_end == NULL)
	{
		return tok_error;
	}
	for(;YYCURSOR != YYLIMIT; ++YYCURSOR)
	{
		if(*YYCURSOR == '>')
		{
			++YYCURSOR;
			break;
		}
	}
	file_len = file_end - file_begin;
	if(file_len >= TLIBC_MAX_PATH_LENGTH)
	{
		return tok_error;
	}
	memcpy(file, file_begin, file_len);
	file[file_len] = 0;

	if(tlibc_xml_reader_push_file(self, file) != E_TLIBC_NOERROR)
	{
		return tok_error;
	}
	goto restart;
}
<INITIAL>'<' {identifier} '>'			{return tok_tag_begin;															}
<INITIAL>'<' '/' {identifier}'>'		{return tok_tag_end;															}

<*>{anychar}							{ goto restart;					 }	
*/	
}
