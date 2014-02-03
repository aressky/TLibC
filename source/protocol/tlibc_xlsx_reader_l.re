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

workbook_rels_token tlibc_xlsx_reader_workbook_rels(tlibc_xlsx_reader_t *self)
{
restart:
	if(self->scanner.cursor >= self->scanner.limit)
	{
		return e_wr_relationships_eof;
	}
/*!re2c
<INITIAL>"Relationships"          { BEGIN(IN_RELATIONSHIPS);	}
<IN_RELATIONSHIPS>"Target=\""	  { return e_wr_target;			}
<IN_RELATIONSHIPS>"Id=\""		  { return e_wr_id;				}
<INITIAL>"/Relationships"         { BEGIN(INITIAL);				}
<*>[^]						{ goto restart;						}
*/
}



