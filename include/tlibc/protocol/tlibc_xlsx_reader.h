#ifndef _H_TLIBC_XLSX_READER
#define _H_TLIBC_XLSX_READER

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"
#include "tlibc/core/tlibc_unzip.h"
#include "tlibc/tdata/tdata_types.h"
#include "tlibc/core/tlibc_hash.h"

typedef struct _tlibc_xlsx_reader_scanner_t
{
	int state;
	char *cursor;
	char *limit;
	char *marker;
}tlibc_xlsx_reader_scanner_t;

typedef struct _tlibc_xlsx_pos
{
	uint32_t row;
	uint32_t col;
}tlibc_xlsx_pos;

typedef struct _tlibc_xlsx_cell_s
{
	tlibc_hash_head_t name2index;
	int empty;
	const char* xpos;

	const char* val_begin;
	const char* val_end;
}tlibc_xlsx_cell_s;

#define TLIBC_XLSX_HASH_BUCKET 65536

typedef struct _tlibc_xlsx_reader_t
{
	TLIBC_ABSTRACT_READER super;

	tlibc_unzip_s unzip;

	char* workbook_rels_buff;
	uint32_t workbook_rels_buff_size;

	char* workbook_buff;
	uint32_t workbook_buff_size;

	char* sharedstring_buff;
	uint32_t sharedstring_buff_size;

	char* sheet_buff;
	uint32_t sheet_buff_size;

	tlibc_xlsx_reader_scanner_t scanner;

	char** sharedstring_begin_list;
	char** sharedstring_end_list;
	uint32_t sharedstring_list_num;

	tlibc_xlsx_pos cell_min_pos;
	tlibc_xlsx_pos cell_max_pos;
	uint32_t cell_row_size;
	uint32_t cell_col_size;
	tlibc_xlsx_cell_s *cell_matrix;
	tlibc_xlsx_cell_s *bindinfo_row;

	tlibc_xlsx_cell_s *curr_row;
	tlibc_xlsx_cell_s *curr_cell;	
	

	int ignore_int32_once;
	tlibc_hash_bucket_t hash_bucket[TLIBC_XLSX_HASH_BUCKET];
	tlibc_hash_t name2index;
}tlibc_xlsx_reader_t;

 TLIBC_ERROR_CODE tlibc_xlsx_reader_init(tlibc_xlsx_reader_t *self, const char *file_name);

 TLIBC_ERROR_CODE tlibc_xlsx_reader_open_sheet(tlibc_xlsx_reader_t *self, const char* sheet, uint32_t bindinfo_row);

 uint32_t tlibc_xlsx_reader_num_rows(tlibc_xlsx_reader_t *self);

 void tlibc_xlsx_reader_row_seek(tlibc_xlsx_reader_t *self, uint32_t offset);

 void tlibc_xlsx_reader_close_sheet(tlibc_xlsx_reader_t *self);

 void tlibc_xlsx_reader_fini(tlibc_xlsx_reader_t *self);

 int32_t tlibc_xlsx_current_location(tlibc_xlsx_reader_t *self);




 TLIBC_ERROR_CODE tlibc_xlsx_read_vector_element_begin(TLIBC_ABSTRACT_READER *super, const char* var_name, uint32_t index);

 TLIBC_ERROR_CODE tlibc_xlsx_read_field_begin(TLIBC_ABSTRACT_READER *super, const char *var_name);

 TLIBC_ERROR_CODE tlibc_xlsx_read_enum_begin(TLIBC_ABSTRACT_READER *super, const char *enum_name);

 TLIBC_ERROR_CODE tlibc_xlsx_read_char(TLIBC_ABSTRACT_READER *super, char *val);

 TLIBC_ERROR_CODE tlibc_xlsx_read_double(TLIBC_ABSTRACT_READER *super, double *val);

 TLIBC_ERROR_CODE tlibc_xlsx_read_int8(TLIBC_ABSTRACT_READER *super, int8_t *val);

 TLIBC_ERROR_CODE tlibc_xlsx_read_int16(TLIBC_ABSTRACT_READER *super, int16_t *val);

 TLIBC_ERROR_CODE tlibc_xlsx_read_int32(TLIBC_ABSTRACT_READER *super, int32_t *val);

 TLIBC_ERROR_CODE tlibc_xlsx_read_int64(TLIBC_ABSTRACT_READER *super, int64_t *val);

 TLIBC_ERROR_CODE tlibc_xlsx_read_uint8(TLIBC_ABSTRACT_READER *super, uint8_t *val);

 TLIBC_ERROR_CODE tlibc_xlsx_read_uint16(TLIBC_ABSTRACT_READER *super, uint16_t *val);

 TLIBC_ERROR_CODE tlibc_xlsx_read_uint32(TLIBC_ABSTRACT_READER *super, uint32_t *val);

 TLIBC_ERROR_CODE tlibc_xlsx_read_uint64(TLIBC_ABSTRACT_READER *super, uint64_t *val);

 TLIBC_ERROR_CODE tlibc_xlsx_read_string(TLIBC_ABSTRACT_READER *super, char *str, uint32_t str_len);

#endif
