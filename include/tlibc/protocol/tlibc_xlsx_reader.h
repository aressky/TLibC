#ifndef _H_TLIBC_XLSX_READER
#define _H_TLIBC_XLSX_READER

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"
#include "tlibc/core/tlibc_unzip.h"
#include "tlibc/tdata/tdata_types.h"

typedef struct _tlibc_xlsx_reader_scanner_t
{
	int state;
	tchar *cursor;
	tchar *limit;
	tchar *marker;
}tlibc_xlsx_reader_scanner_t;

typedef struct _tlibc_xlsx_pos
{
	tuint32 row;
	tuint32 col;
}tlibc_xlsx_pos;

typedef struct _tlibc_xlsx_cell_s
{
	int empty;
	const char* xpos;

	const char* val_begin;
	const char* val_end;
}tlibc_xlsx_cell_s;

typedef struct _tlibc_xlsx_reader_t
{
	TLIBC_ABSTRACT_READER super;

	tlibc_unzip_s unzip;

	char* workbook_rels_buff;
	tuint32 workbook_rels_buff_size;

	char* workbook_buff;
	tuint32 workbook_buff_size;

	char* sharedstring_buff;
	tuint32 sharedstring_buff_size;

	char* sheet_buff;
	tuint32 sheet_buff_size;

	tlibc_xlsx_reader_scanner_t scanner;

	char** sharedstring_begin_list;
	char** sharedstring_end_list;
	tuint32 sharedstring_list_num;

	tlibc_xlsx_pos cell_min_pos;
	tlibc_xlsx_pos cell_max_pos;
	tuint32 cell_row_size;
	tuint32 cell_col_size;
	tlibc_xlsx_cell_s *cell_matrix;
	tuint32 real_row_size;


	tlibc_xlsx_cell_s *bindinfo_row;
	tuint32 data_real_row_index;
	tlibc_xlsx_cell_s *data_row;

	size_t vector_level;
}tlibc_xlsx_reader_t;

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_reader_init(tlibc_xlsx_reader_t *self, const char *file_name);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_reader_open_sheet(tlibc_xlsx_reader_t *self, 
														const char* sheet, tuint32 bindinfo_row, tuint32 data_row);

TLIBC_API void tlibc_xlsx_reader_close_sheet(tlibc_xlsx_reader_t *self);

TLIBC_API void tlibc_xlsx_reader_fini(tlibc_xlsx_reader_t *self);


TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_vector_begin(TLIBC_ABSTRACT_READER *super);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_vector_end(TLIBC_ABSTRACT_READER *super);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_field_begin(TLIBC_ABSTRACT_READER *super, const char *var_name);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_field_end(TLIBC_ABSTRACT_READER *super, const char *var_name);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tchar(TLIBC_ABSTRACT_READER *super, char *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tdouble(TLIBC_ABSTRACT_READER *super, double *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tint8(TLIBC_ABSTRACT_READER *super, tint8 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tint16(TLIBC_ABSTRACT_READER *super, tint16 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tint32(TLIBC_ABSTRACT_READER *super, tint32 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tint64(TLIBC_ABSTRACT_READER *super, tint64 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tuint8(TLIBC_ABSTRACT_READER *super, tuint8 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tuint16(TLIBC_ABSTRACT_READER *super, tuint16 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tuint32(TLIBC_ABSTRACT_READER *super, tuint32 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tuint64(TLIBC_ABSTRACT_READER *super, tuint64 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_read_tstring(TLIBC_ABSTRACT_READER *super, tchar *str, tuint32 str_len);

#endif
