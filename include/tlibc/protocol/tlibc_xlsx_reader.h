#ifndef _H_TLIBC_XLSX_READER
#define _H_TLIBC_XLSX_READER

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

#include "tlibc/tdata/tdata_types.h"

typedef struct _tlibc_xlsx_reader_scanner_t
{	
	const tchar *cursor;
	const tchar *limit;
}tlibc_xlsx_reader_scanner_t;

typedef struct _tlibc_xlsx_reader_t
{
	TLIBC_ABSTRACT_READER super;

	void *sharedstring_addr;
	const char *sharedstring_buff;
	tuint32 sharedstring_buff_size;

	tlibc_xlsx_reader_scanner_t scanner;
}tlibc_xlsx_reader_t;

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_reader_init(tlibc_xlsx_reader_t *self, const char *file_name);

#endif

