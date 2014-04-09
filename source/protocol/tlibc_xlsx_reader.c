#include "protocol/tlibc_xlsx_reader.h"
#include "core/tlibc_util.h"
#include "protocol/tlibc_abstract_reader.h"
#include "core/tlibc_error_code.h"
#include "protocol/tlibc_xml_reader.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

const char* tlibc_xlsx_reader_workbook_rels_search_file(tlibc_xlsx_reader_t *self, const char* rid);
const char* tlibc_xlsx_reader_workbook_search_rid(tlibc_xlsx_reader_t *self, const char* sheet);
void tlibc_xlsx_reader_load_sharedstring(tlibc_xlsx_reader_t *self);
tlibc_error_code_t tlibc_xlsx_reader_loadsheet(tlibc_xlsx_reader_t *self, uint32_t bindinfo_row);

tlibc_error_code_t tlibc_xlsx_reader_init(tlibc_xlsx_reader_t *self, const char *file_name)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;

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

	self->last_col = -1;
	self->last_col_str[0] = 0;

	self->super.read_vector_element_begin = tlibc_xlsx_read_vector_element_begin;
	self->super.read_field_begin = tlibc_xlsx_read_field_begin;	
	self->super.read_enum_begin = tlibc_xlsx_read_enum_begin;

	self->super.read_int8 = tlibc_xlsx_read_int8;
	self->super.read_int16 = tlibc_xlsx_read_int16;
	self->super.read_int32 = tlibc_xlsx_read_int32;
	self->super.read_int64 = tlibc_xlsx_read_int64;

	self->super.read_uint8 = tlibc_xlsx_read_uint8;
	self->super.read_uint16 = tlibc_xlsx_read_uint16;
	self->super.read_uint32 = tlibc_xlsx_read_uint32;
	self->super.read_uint64 = tlibc_xlsx_read_uint64;

	self->super.read_double = tlibc_xlsx_read_double;
	self->super.read_char = tlibc_xlsx_read_char;
	self->super.read_string = tlibc_xlsx_read_string;
	self->super.enable_name = TRUE;
	
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
 tlibc_error_code_t tlibc_xlsx_reader_open_sheet(tlibc_xlsx_reader_t *self, const char* sheet, uint32_t bindinfo_row)
{
	size_t i ;
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
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
	self->super.name_ptr = self->super.name;
	self->curr_cell = NULL;
	self->read_enum_name_once = FALSE;
	self->curr_row = self->cell_matrix + (offset - self->cell_min_pos.row) * self->cell_col_size;
}

void tlibc_xlsx_reader_close_sheet(tlibc_xlsx_reader_t *self)
{
	free(self->sheet_buff);
	tlibc_hash_clear(&self->name2index);
}

void tlibc_xlsx_reader_fini(tlibc_xlsx_reader_t *self)
{
	if(self->cell_matrix)
	{
		free(self->cell_matrix);
	}

	if(self->workbook_buff)
	{
		free(self->workbook_buff);
	}	

	if(self->workbook_rels_buff)
	{
		free(self->workbook_rels_buff);
	}
	
	if(self->sharedstring_buff)
	{
		free(self->sharedstring_buff);
	}
	
	if(self->sharedstring_begin_list)
	{
		free(self->sharedstring_begin_list);
	}

	if(self->sharedstring_end_list)
	{
		free(self->sharedstring_end_list);
	}
	
	tlibc_unzip_fini(&self->unzip);
}

static void tlibc_xlsx_locate(tlibc_xlsx_reader_t *self)
{
	tlibc_xlsx_cell_s *cell;
	tlibc_hash_head_t *head;

	self->curr_cell = NULL;
	if(self->super.name_ptr <= self->super.name)
	{
		goto done;
	}
	head = tlibc_hash_find(&self->name2index, self->super.name + 1, self->super.name_ptr - self->super.name - 1);
	if(head == NULL)
	{
		goto done;
	}
	cell = TLIBC_CONTAINER_OF(head, tlibc_xlsx_cell_s, name2index);
	self->curr_cell = self->curr_row + (cell - self->bindinfo_row);

	self->last_col = self->curr_cell - self->curr_row;

	

done:
	return;
}
tlibc_error_code_t tlibc_xlsx_read_vector_element_begin(tlibc_abstract_reader_t *super, const char* var_name, uint32_t index)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);	
	TLIBC_UNUSED(var_name);
	TLIBC_UNUSED(index);
	tlibc_xlsx_locate(self);

	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xlsx_read_field_begin(tlibc_abstract_reader_t *super, const char *var_name)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	TLIBC_UNUSED(var_name);
	tlibc_xlsx_locate(self);

	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xlsx_read_enum_begin(tlibc_abstract_reader_t *super, const char *enum_name)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	TLIBC_UNUSED(enum_name);

	self->read_enum_name_once = TRUE;

	return E_TLIBC_NOERROR;
}

tlibc_error_code_t tlibc_xlsx_read_int8(tlibc_abstract_reader_t *super, int8_t *val)
{
	int64_t i64;
	tlibc_error_code_t ret = tlibc_xlsx_read_int64(super, &i64);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}

	*val = (int8_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tlibc_error_code_t tlibc_xlsx_read_int16(tlibc_abstract_reader_t *super, int16_t *val)
{
	int64_t i64;
	tlibc_error_code_t ret = tlibc_xlsx_read_int64(super, &i64);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}

	*val = (int16_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tlibc_error_code_t tlibc_xlsx_read_int32(tlibc_abstract_reader_t *super, int32_t *val)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	int64_t i64;
	tlibc_error_code_t ret;
	if(self->read_enum_name_once)
	{
		self->read_enum_name_once = FALSE;
		ret = E_TLIBC_PLEASE_READ_ENUM_NAME;
		goto done;
	}
	ret = tlibc_xlsx_read_int64(super, &i64);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}

	*val = (int32_t)i64;
	if(*val != i64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tlibc_error_code_t tlibc_xlsx_read_int64(tlibc_abstract_reader_t *super, int64_t *val)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	if(self->curr_cell == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		*val = 0;
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

tlibc_error_code_t tlibc_xlsx_read_uint8(tlibc_abstract_reader_t *super, uint8_t *val)
{
	uint64_t ui64;
	tlibc_error_code_t ret = tlibc_xlsx_read_uint64(super, &ui64);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}

	*val = (uint8_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tlibc_error_code_t tlibc_xlsx_read_uint16(tlibc_abstract_reader_t *super, uint16_t *val)
{
	uint64_t ui64;
	tlibc_error_code_t ret = tlibc_xlsx_read_uint64(super, &ui64);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	*val = (uint16_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tlibc_error_code_t tlibc_xlsx_read_uint32(tlibc_abstract_reader_t *super, uint32_t *val)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	uint64_t ui64;
	ret = tlibc_xlsx_read_uint64(super, &ui64);
	if(ret != E_TLIBC_NOERROR)
	{
		goto done;
	}
	*val = (uint32_t)ui64;
	if(*val != ui64)
	{
		return E_TLIBC_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tlibc_error_code_t tlibc_xlsx_read_uint64(tlibc_abstract_reader_t *super, uint64_t *val)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	if(self->curr_cell == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		*val = 0;
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

tlibc_error_code_t tlibc_xlsx_read_double(tlibc_abstract_reader_t *super, double *val)
{
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	if(self->curr_cell == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		*val = 0;		
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

tlibc_error_code_t tlibc_xlsx_read_char(tlibc_abstract_reader_t *super, char *val)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	const char* curr;
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	if(self->curr_cell == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		*val = 0;
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

tlibc_error_code_t tlibc_xlsx_read_string(tlibc_abstract_reader_t *super, char *str, uint32_t str_len)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	uint32_t len = 0;
	const char* curr;
	const char* limit;
	tlibc_error_code_t ret = E_TLIBC_NOERROR;
	if(self->curr_cell == NULL)
	{
		ret = E_TLIBC_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		if(str_len > 0)
		{
			*str = 0;
		}
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

const char* tlibc_xlsx_last_col(tlibc_xlsx_reader_t *self)
{
	if(self->last_col >= 0)
	{
		uint32_t col = self->last_col + 1;
		uint32_t i = 0;
		while(col > 0)
		{
			if(i >= TLIBC_XLSX_MAX_COL_STR)
			{
				break;
			}
			self->last_col_str[i] = col % 26 + 'A';
			++i;
			col /= 26;
		}
		if(i < TLIBC_XLSX_MAX_COL_STR)
		{
			self->last_col_str[i] = 0;
		}
		else
		{
			self->last_col_str[TLIBC_XLSX_MAX_COL_STR - 1] = 0;
		}
		
	}
	
	return self->last_col_str;
}
