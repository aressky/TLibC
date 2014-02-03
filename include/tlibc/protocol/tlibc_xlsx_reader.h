#ifndef _H_TLIBC_XLSX_READER
#define _H_TLIBC_XLSX_READER

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

#include "tlibc/tdata/tdata_types.h"

typedef enum _workbook_rels_token
{
	e_wr_relationships_eof = 0,
	e_wr_target = 1,
	e_wr_id = 1,
}workbook_rels_token;

typedef struct _tlibc_xlsx_reader_scanner_t
{
	int state;
	tchar *cursor;
	tchar *limit;
	tchar *marker;
}tlibc_xlsx_reader_scanner_t;

typedef struct _tlibc_xlsx_reader_t
{
	TLIBC_ABSTRACT_READER super;

	char* workbook_rels_buff;
	tuint32 workbook_rels_buff_size;

	char* workbook_buff;
	tuint32 workbook_buff_size;

	char* sharedstring_buff;
	tuint32 sharedstring_buff_size;

	char* sheet_buff;
	tuint32 sheet_buff_size;

	tlibc_xlsx_reader_scanner_t scanner;
}tlibc_xlsx_reader_t;

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_reader_init(tlibc_xlsx_reader_t *self, const char *file_name, const char* sheet);

#endif

