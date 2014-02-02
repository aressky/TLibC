#include "tlibc/protocol/tlibc_xlsx_reader.h"

void tlibc_xlsx_reader_sharedstring(tlibc_xlsx_reader_t *self)
{
/*!re2c
	re2c:define:YYCTYPE  = "char";
    re2c:define:YYCURSOR = self->scanner.cursor;
	re2c:define:YYLIMIT	 = self->scanner.limit;
    re2c:yyfill:enable   = 0;

    [0-9]+          {return;}
*/
}
