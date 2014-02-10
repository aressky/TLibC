#include "tlibc/protocol/tlibc_xlsx_reader.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"


#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

const char* tlibc_xlsx_reader_workbook_rels_search_file(tlibc_xlsx_reader_t *self, const char* rid);
const char* tlibc_xlsx_reader_workbook_search_rid(tlibc_xlsx_reader_t *self, const char* sheet);
void tlibc_xlsx_reader_load_sharedstring(tlibc_xlsx_reader_t *self);
TLIBC_ERROR_CODE tlibc_xlsx_reader_loadsheet(tlibc_xlsx_reader_t *self, tuint32 bindinfo_row, tuint32 data_row);

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

	self->super.read_vector_begin = tlibc_xlsx_read_vector_begin;
	self->super.read_vector_end = tlibc_xlsx_read_vector_end;
	self->super.read_vector_element_begin = tlibc_xlsx_read_vector_element_begin;
	self->super.read_vector_element_end = tlibc_xlsx_read_vector_element_end;
	self->super.read_field_begin = tlibc_xlsx_read_field_begin;	
	self->super.read_field_end = tlibc_xlsx_read_field_end;

	self->super.read_tuint32 = tlibc_xlsx_read_tuint32;

	self->vector_level = 0;
	self->curr_name_ptr = self->curr_name;
	self->read_rowsize = FALSE;
	self->skip_a_field = FALSE;

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
TLIBC_API TLIBC_ERROR_CODE tlibc_xlsx_reader_open_sheet(tlibc_xlsx_reader_t *self, 
														const char* sheet, tuint32 bindinfo_row, tuint32 data_row)
{
	TLIBC_ERROR_CODE ret;
	const char *rid;
	const char *file;
	char sheet_file[TLIBC_MAX_PATH_LENGTH] = {XL_PREFIX};

	rid = tlibc_xlsx_reader_workbook_search_rid(self, sheet);
	if(rid == NULL)
	{
		goto ERROR_RET;
	}

	file = tlibc_xlsx_reader_workbook_rels_search_file(self, rid);
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
	if(tlibc_xlsx_reader_loadsheet(self, bindinfo_row, data_row) != E_TLIBC_NOERROR)
	{
		goto free_sheet;
	}

	return E_TLIBC_NOERROR;
free_sheet:
	free(self->sheet_buff);
ERROR_RET:
	return E_TLIBC_ERROR;
}

void tlibc_xlsx_reader_close_sheet(tlibc_xlsx_reader_t *self)
{
	free(self->sheet_buff);
}

void tlibc_xlsx_reader_fini(tlibc_xlsx_reader_t *self)
{
	free(self->workbook_buff);
	free(self->workbook_rels_buff);
	free(self->sharedstring_buff);
	
	tlibc_unzip_fini(&self->unzip);
}


TLIBC_ERROR_CODE tlibc_xlsx_read_vector_begin(TLIBC_ABSTRACT_READER *super)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	
	++self->vector_level;
	if(self->vector_level == 1)
	{
		self->skip_a_field = TRUE;
		self->read_rowsize = TRUE;
	}
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_vector_end(TLIBC_ABSTRACT_READER *super)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);	
	--self->vector_level;
	return E_TLIBC_NOERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_vector_element_begin(TLIBC_ABSTRACT_READER *super, const char* var_name, tuint32 index)
{
	int len;
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	if(self->vector_level == 1)
	{
		self->curr_row = self->data_row + index * self->cell_col_size;		
	}
	else
	{
		if(self->curr_name != self->curr_name_ptr)
		{
			if(self->curr_name_ptr - self->curr_name >= TLIBC_XLSX_READER_NAME_LENGTH)
			{
				goto ERROR_RET;
			}

			*self->curr_name_ptr = '.';
			++self->curr_name_ptr;
		}
		len = snprintf(self->curr_name_ptr, TLIBC_XLSX_READER_NAME_LENGTH - (self->curr_name_ptr - self->curr_name)
			, "%s[%u]", var_name, index);

		if(len < 0)
		{
			goto ERROR_RET;
		}
		self->curr_name_ptr += len;
		printf("%s\n", self->curr_name);
	}

	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_vector_element_end(TLIBC_ABSTRACT_READER *super, const char* var_name, tuint32 index)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	int len;
	char curr_name[TLIBC_XLSX_READER_NAME_LENGTH];
	
	if(self->vector_level != 1)
	{
		len = snprintf(curr_name, TLIBC_XLSX_READER_NAME_LENGTH, "%s[%u]", var_name, index);
		if(len < 0)
		{
			goto ERROR_RET;
		}
		self->curr_name_ptr -= len;
		if(self->curr_name != self->curr_name_ptr)
		{
			--self->curr_name_ptr;
		}
		*self->curr_name_ptr = 0;
	}
	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_field_begin(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	int len;
	if(self->skip_a_field)
	{
		goto done;
	}

	if(self->curr_name != self->curr_name_ptr)
	{
		if(self->curr_name_ptr - self->curr_name >= TLIBC_XLSX_READER_NAME_LENGTH)
		{
			goto ERROR_RET;
		}
		*self->curr_name_ptr = '.';
		++self->curr_name_ptr;
	}
	len = snprintf(self->curr_name_ptr, TLIBC_XLSX_READER_NAME_LENGTH - (self->curr_name_ptr - self->curr_name)
		, "%s", var_name);
	if(len < 0)
	{
		goto ERROR_RET;
	}
	self->curr_name_ptr += len;
	if(self->vector_level == 1)
		printf("%s\n", self->curr_name);

done:
	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_field_end(TLIBC_ABSTRACT_READER *super, const char *var_name)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	int len;
	char curr_name[TLIBC_XLSX_READER_NAME_LENGTH];	
	if(self->skip_a_field)
	{
		self->skip_a_field = FALSE;
		goto done;
	}

	len = snprintf(curr_name, TLIBC_XLSX_READER_NAME_LENGTH, "%s", var_name);
	if(len < 0)
	{
		goto ERROR_RET;
	}
	self->curr_name_ptr -= len;
	if(self->curr_name != self->curr_name_ptr)
	{
		--self->curr_name_ptr;
	}
	*self->curr_name_ptr = 0;

done:
	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}

TLIBC_ERROR_CODE tlibc_xlsx_read_tuint32(TLIBC_ABSTRACT_READER *super, tuint32 *val)
{
	tlibc_xlsx_reader_t *self = TLIBC_CONTAINER_OF(super, tlibc_xlsx_reader_t, super);
	if(self->read_rowsize)
	{
		self->read_rowsize = FALSE;
		*val = self->real_row_size - self->data_real_row_index;
		goto done;
	}

done:
	return E_TLIBC_NOERROR;
}
