#include "tlibc/protocol/tlibc_xlsx_reader.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"
#include "tlibc/protocol/tlibc_xml_reader.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

const char* tlibc_xlsx_reader_workbook_rels_search_file(tlibc_xlsx_reader_t *self, const char* rid);
const char* tlibc_xlsx_reader_workbook_search_rid(tlibc_xlsx_reader_t *self, const char* sheet);
void tlibc_xlsx_reader_load_sharedstring(tlibc_xlsx_reader_t *self);
TLIBC_ERROR_CODE tlibc_xlsx_reader_loadsheet(tlibc_xlsx_reader_t *self, uint32_t bindinfo_row);

TLIBC_ERROR_CODE tlibc_xlsx_reader_init(tlibc_xlsx_reader_t *self, const char *file_name)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;

	ret = tlibc_unzip_init(&self->unzip, file_name);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}

	//load workbook
	ret = tlibc_unzip_locate(&self->unzip, "xl/workbook.xml");
	if(ret != E_TLIBC_NOERROR)
	{
		goto fini_unzip;
	}
	ret = tlibc_unzip_open_current_file(&self->unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto fini_unzip;
	}
	self->workbook_buff_size = self->unzip.cur_file_info.uncompressed_size;
	self->workbook_buff = (char*)malloc(self->workbook_buff_size);
	ret = tlibc_read_current_file(&self->unzip, self->workbook_buff, &self->workbook_buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		tlibc_unzip_close_current_file (&self->unzip);
		goto free_workbook;
	}
	ret = tlibc_unzip_close_current_file (&self->unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto free_workbook;
	}

	//load workbook rels
	ret = tlibc_unzip_locate(&self->unzip, "xl/_rels/workbook.xml.rels");
	if(ret != E_TLIBC_NOERROR)
	{
		goto free_workbook;
	}
	ret = tlibc_unzip_open_current_file(&self->unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto free_workbook;
	}
	self->workbook_rels_buff_size = self->unzip.cur_file_info.uncompressed_size;
	self->workbook_rels_buff = (char*)malloc(self->workbook_rels_buff_size);
	ret = tlibc_read_current_file(&self->unzip, self->workbook_rels_buff, &self->workbook_rels_buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		tlibc_unzip_close_current_file (&self->unzip);
		goto free_workbook_rels;
	}
	ret = tlibc_unzip_close_current_file (&self->unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto free_sharedstring;
	}


	//load shared string
	ret = tlibc_unzip_locate(&self->unzip, "xl/sharedStrings.xml");
	if(ret != E_TLIBC_NOERROR)
	{
		goto free_workbook_rels;
	}
	ret = tlibc_unzip_open_current_file(&self->unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto free_workbook_rels;
	}
	self->sharedstring_buff_size = self->unzip.cur_file_info.uncompressed_size;
	self->sharedstring_buff = (char*)malloc(self->sharedstring_buff_size);	
	ret = tlibc_read_current_file(&self->unzip, self->sharedstring_buff, &self->sharedstring_buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		tlibc_unzip_close_current_file (&self->unzip);
		goto free_sharedstring;
	}
	ret = tlibc_unzip_close_current_file (&self->unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto free_sharedstring;
	}
	tlibc_xlsx_reader_load_sharedstring(self);

	tlibc_abstract_reader_init(&self->super);

	self->super.read_vector_element_begin = tlibc_xlsx_read_vector_element_begin;
	self->super.read_vector_element_end = tlibc_xlsx_read_vector_element_end;
	self->super.read_field_begin = tlibc_xlsx_read_field_begin;	
	self->super.read_field_end = tlibc_xlsx_read_field_end;
	self->super.read_enum_begin = tlibc_xlsx_read_enum_begin;

	self->super.read_int8_t = tlibc_xlsx_read_int8_t;
	self->super.read_int16_t = tlibc_xlsx_read_int16_t;
	self->super.read_int32_t = tlibc_xlsx_read_int32_t;
	self->super.read_int64_t = tlibc_xlsx_read_int64_t;

	self->super.read_uint8_t = tlibc_xlsx_read_uint8_t;
	self->super.read_uint16_t = tlibc_xlsx_read_uint16_t;
	self->super.read_uint32_t = tlibc_xlsx_read_uint32_t;
	self->super.read_uint64_t = tlibc_xlsx_read_uint64_t;

	self->super.read_double = tlibc_xlsx_read_double;
	self->super.read_char = tlibc_xlsx_read_char;
	self->super.read_string = tlibc_xlsx_read_string;

	
	tlibc_hash_init(&self->name2index, self->hash_bucket, TLIBC_XLSX_HASH_BUCKET);
	

	return E_TLIBC_NOERROR;
free_sharedstring:
	free(self->sharedstring_buff);
free_workbook_rels:
	free(self->workbook_rels_buff);
free_workbook:
	free(self->workbook_buff);
fini_unzip:
	tlibc_unzip_fini(&self->unzip);
ERROR_RET:
	return E_TLIBC_ERROR;
}

#define XL_PREFIX "xl/"
 TLIBC_ERROR_CODE tlibc_xlsx_reader_open_sheet(tlibc_xlsx_reader_t *self, const char* sheet, uint32_t bindinfo_row)
{
	size_t i ;
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	const char *rid;
	const char *file;
	char sheet_file[TLIBC_MAX_PATH_LENGTH] = {XL_PREFIX};

	rid = tlibc_xlsx_reader_workbook_search_rid(self, sheet);
	if(rid == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto ERROR_RET;
	}

	file = tlibc_xlsx_reader_workbook_rels_search_file(self, rid);
	if(file == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto ERROR_RET;
	}
	strncpy(sheet_file + strlen(XL_PREFIX), file, TLIBC_MAX_PATH_LENGTH - strlen(XL_PREFIX) - 1);

	ret = tlibc_unzip_locate(&self->unzip, sheet_file);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}

	ret = tlibc_unzip_open_current_file(&self->unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	self->sheet_buff_size = self->unzip.cur_file_info.uncompressed_size;
	self->sheet_buff = (char*)malloc(self->sheet_buff_size);	
	ret = tlibc_read_current_file(&self->unzip, self->sheet_buff, &self->sheet_buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		tlibc_unzip_close_current_file (&self->unzip);
		goto free_sheet;
	}
	ret = tlibc_unzip_close_current_file (&self->unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto free_sheet;
	}
	if(tlibc_xlsx_reader_loadsheet(self, bindinfo_row) != E_TLIBC_NOERROR)
	{
		goto free_sheet;
	}	

	for(i = 0; i < self->cell_col_size; ++i)
	{
		if(!self->bindinfo_row[i].empty)
		{
			tlibc_hash_insert(&self->name2index, self->bindinfo_row[i].val_begin
				, self->bindinfo_row[i].val_end - self->bindinfo_row[i].val_begin, &self->bindinfo_row[i].name2index);
		}
	}

	return E_TLIBC_NOERROR;
free_sheet:
	free(self->sheet_buff);
ERROR_RET:
	return E_TLIBC_ERROR;
}

uint32_t tlibc_xlsx_reader_num_rows(tlibc_xlsx_reader_t *self)
{
	return self->cell_row_size;
}

void tlibc_xlsx_reader_row_seek(tlibc_xlsx_reader_t *self, uint32_t offset)
{
	self->curr_name_ptr = self->curr_name;
	self->curr_cell = NULL;
	self->ignore_int32_once = FALSE;
	self->curr_row = self->cell_matrix + (offset - self->cell_min_pos.row) * self->cell_col_size;
}

void tlibc_xlsx_reader_close_sheet(tlibc_xlsx_reader_t *self)
{
	free(self->sheet_buff);
	tlibc_hash_clear(&self->name2index);
}

void tlibc_xlsx_reader_fini(tlibc_xlsx_reader_t *self)
{
	free(self->workbook_buff);
	free(self->workbook_rels_buff);
	free(self->sharedstring_buff);
	
	tlibc_unzip_fini(&self->unzip);
}

static void tlibc_xlsx_locate(tlibc_xlsx_reader_t *self)
{
	tlibc_xlsx_cell_s *cell;
	tlibc_hash_head_t *head;

	self->curr_cell = NULL;
	if(self->curr_name_ptr <= self->curr_name)
	{		
		goto done;
	}
	head = tlibc_hash_find(&self->name2index, self->curr_name + 1, self->curr_name_ptr - self->curr_name - 1);
	if(head == NULL)
	{
		goto done;
	}
	cell = TLIBC_CONTAINER_OF(head, tlibc_xlsx_cell_s, name2index);
	self->curr_cell = self->curr_row + (cell - self->bindinfo_row);

done:
	return;
}
TLIBC_ERROR_CODE tlibc_xlsx_read_vector_element_begin(TLIBC_ABSTRACT_READER *super, const char* var_name, uint32_t index)
{
	int len;
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	
	len = snprintf(self->curr_name_ptr, TLIBC_XLSX_READER_NAME_LENGTH - (self->curr_name_ptr - self->curr_name)
		, ".%s[%u]", var_name, index);

	if(len < 0)
	{
		goto ERROR_RET;
	}
	self->curr_name_ptr += len;

	tlibc_xlsx_locate(self);

	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_vector_element_end(TLIBC_ABSTRACT_READER *super, const char* var_name, uint32_t index)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	int len;
	char curr_name[TLIBC_XLSX_READER_NAME_LENGTH];
	
	len = snprintf(curr_name, TLIBC_XLSX_READER_NAME_LENGTH, ".%s[%u]", var_name, index);
	if(len < 0)
	{
		goto ERROR_RET;
	}
	self->curr_name_ptr -= len;

	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_field_begin(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	int len;

	len = snprintf(self->curr_name_ptr, TLIBC_XLSX_READER_NAME_LENGTH - (self->curr_name_ptr - self->curr_name)
		, ".%s", var_name);
	if(len < 0)
	{
		goto ERROR_RET;
	}
	self->curr_name_ptr += len;

	tlibc_xlsx_locate(self);

	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_field_end(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	int len;
	char curr_name[TLIBC_XLSX_READER_NAME_LENGTH];	

	len = snprintf(curr_name, TLIBC_XLSX_READER_NAME_LENGTH, ".%s", var_name);
	if(len < 0)
	{
		goto ERROR_RET;
	}
	self->curr_name_ptr -= len;

	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_enum_begin(TLIBC_ABSTRACT_READER *super, const char *enum_name)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	TLIBC_UNUSED(enum_name);

	self->ignore_int32_once = TRUE;

	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_int8_t(TLIBC_ABSTRACT_READER *super, int8_t *val)
{
	int64_t i64;
	TLIBC_ERROR_CODE ret = tlibc_xlsx_read_int64_t(super, &i64);
	*val = (int8_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_int16_t(TLIBC_ABSTRACT_READER *super, int16_t *val)
{
	int64_t i64;
	TLIBC_ERROR_CODE ret = tlibc_xlsx_read_int64_t(super, &i64);
	*val = (int16_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_int32_t(TLIBC_ABSTRACT_READER *super, int32_t *val)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	int64_t i64;
	TLIBC_ERROR_CODE ret;
	if(self->ignore_int32_once)
	{
		self->ignore_int32_once = FALSE;
		ret = E_TLIBC_PLEASE_READ_ENUM_NAME;
		goto done;
	}
	ret = tlibc_xlsx_read_int64_t(super, &i64);
	*val = (int32_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_int64_t(TLIBC_ABSTRACT_READER *super, int64_t *val)
{
	TLIBC_ERROR_CODE ret;
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	if(self->curr_cell == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		ret = E_TLIBC_EMPTY;
		goto done;
	}

	errno = 0;
	*val = strtoll(self->curr_cell->val_begin, NULL, 10);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto done;
	}

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_uint8_t(TLIBC_ABSTRACT_READER *super, uint8_t *val)
{
	uint64_t ui64;
	TLIBC_ERROR_CODE ret = tlibc_xlsx_read_uint64_t(super, &ui64);
	*val = (uint8_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_uint16_t(TLIBC_ABSTRACT_READER *super, uint16_t *val)
{
	uint64_t ui64;
	TLIBC_ERROR_CODE ret = tlibc_xlsx_read_uint64_t(super, &ui64);
	*val = (uint16_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_uint32_t(TLIBC_ABSTRACT_READER *super, uint32_t *val)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	uint64_t ui64;
	ret = tlibc_xlsx_read_uint64_t(super, &ui64);
	*val = (uint32_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
	return ret;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_uint64_t(TLIBC_ABSTRACT_READER *super, uint64_t *val)
{
	TLIBC_ERROR_CODE ret;
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	if(self->curr_cell == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		ret = E_TLIBC_EMPTY;
		goto done;
	}

	errno = 0;
	*val = strtoull(self->curr_cell->val_begin, NULL, 10);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto done;
	}

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_double(TLIBC_ABSTRACT_READER *super, double *val)
{
	TLIBC_ERROR_CODE ret;
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	if(self->curr_cell == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		ret = E_TLIBC_EMPTY;
		goto done;
	}

	errno = 0;
	*val = strtod(self->curr_cell->val_begin, NULL);
	if(errno != 0)
	{
		ret = E_TLIBC_ERRNO;
		goto done;
	}

	return E_TLIBC_NOERROR;
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_char(TLIBC_ABSTRACT_READER *super, char *val)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	const char* curr;
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	if(self->curr_cell == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		ret = E_TLIBC_EMPTY;
		goto done;
	}

	curr = tlibc_xml_str2c(self->curr_cell->val_begin
		, self->curr_cell->val_end, val);
	if(curr == NULL)
	{
		return E_TLIBC_OUT_OF_MEMORY;
	}
done:
	return ret;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_string(TLIBC_ABSTRACT_READER *super, char *str, uint32_t str_len)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	uint32_t len = 0;
	const char* curr;
	const char* limit;
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	if(self->curr_cell == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		ret = E_TLIBC_EMPTY;
		goto done;
	}
	curr = self->curr_cell->val_begin;
	limit = self->curr_cell->val_end;
	while(curr < limit)
	{
		char c;
		curr = tlibc_xml_str2c(curr, limit, &c);
		if(curr == NULL)
		{
			ret = E_TLIBC_OUT_OF_MEMORY;
			goto done;
		}

		if(c == '<')
		{
			break;
		}
		if(len >= str_len)
		{
			ret = E_TLIBC_OUT_OF_MEMORY;
			goto done;
		}
		str[len++] = c;		
	}
	str[len] = 0;
done:
	return ret;
}

int32_t tlibc_xlsx_current_location(tlibc_xlsx_reader_t *self)
{
	if(self->curr_cell == NULL)
	{
		return -1;
	}
	return self->curr_cell - self->curr_row;
}
