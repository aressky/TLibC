#include "platform/tlibc_platform.h"
#include "protocol/tlibc_xml_reader.h"


#include "tlibc_xml_reader_l.h"

		
tint32 tlibc_xml_scan(TLIBC_XML_READER *self, YYLTYPE *yylloc)
{
restart:
	if(YYCURSOR >= YYLIMIT)
	{
		return tok_end;
	}
	yylloc->first_line = self->scanner_context.yylineno;
	yylloc->first_column = self->scanner_context.yycolumn;
	yytext = YYCURSOR;
/*!re2c
re2c:yyfill:check = 0;

identifier		([a-zA-Z_][a-zA-Z_0-9]*)
comment			("<!--".*"-->")
anychar			([^])


<!*> := yyleng = YYCURSOR - yytext; xml_locate(self);

<*>{comment}							{	goto restart;																}

<INITIAL>'<' {identifier} '>'			{return tok_tag_begin;															}
<INITIAL>'<' '/' {identifier}'>'		{return tok_tag_end;															}

<*>{anychar}							{ goto restart;					 }	
*/	
}
