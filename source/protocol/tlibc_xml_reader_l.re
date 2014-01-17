#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_xml_reader.h"
#include "protocol/tlibc_xml_reader_scanner.h"
#include "tlibc_xml_reader_l.h"




		
TLIBC_XML_READER_TOKEN tlibc_xml_reader_scan(TLIBC_XML_READER_SCANNER_CONTEXT *self)
{
restart:
	if(YYCURSOR >= YYLIMIT)
	{
		return tok_end;
	}
	self->yylloc.first_line = self->yylineno;
	self->yylloc.first_column = self->yycolumn;
	yytext = YYCURSOR;
/*!re2c
re2c:yyfill:check = 0;

identifier		([a-zA-Z_][a-zA-Z_0-9]*)
comment			("<!--".*"-->")
anychar			([^])


<!*> := yyleng = YYCURSOR - yytext; tlibc_xml_reader_locate(self);

<*>{comment}							{	goto restart;																}

<INITIAL>'<' {identifier} '>'			{return tok_tag_begin;															}
<INITIAL>'<' '/' {identifier}'>'		{return tok_tag_end;															}

<*>{anychar}							{ goto restart;					 }	
*/	
}
