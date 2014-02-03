#include "tlibc/protocol/tlibc_xlsx_reader.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"
#include "tlibc/core/tlibc_unzip.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

const char* tlibc_xlsx_reader_workbook_rels_search_file(tlibc_xlsx_reader_t *self, const char* rid);
const char* tlibc_xlsx_reader_workbook_search_rid(tlibc_xlsx_reader_t *self, const char* sheet);
void tlibc_xlsx_reader_load_sharedstring(tlibc_xlsx_reader_t *self);
void tlibc_xlsx_reader_loadsheet(tlibc_xlsx_reader_t *self);

#define XL_PREFIX "xl/"
TLIBC_ERROR_CODE tlibc_xlsx_reader_init(tlibc_xlsx_reader_t *self, const char *file_name, const char* sheet)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;
	tlibc_unzip_s unzip;
	const char *rid;
	const char *file;
	char sheet_file[TLIBC_MAX_PATH_LENGTH] = {XL_PREFIX};


	ret = tlibc_unzip_init(&unzip, file_name);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}

	//load workbook
	ret = tlibc_unzip_locate(&unzip, "xl/workbook.xml");
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	ret = tlibc_unzip_open_current_file(&unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	self->workbook_buff_size = unzip.cur_file_info.uncompressed_size;
	self->workbook_buff = (char*)malloc(self->workbook_buff_size);
	ret = tlibc_read_current_file(&unzip, self->workbook_buff, &self->workbook_buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	ret = tlibc_unzip_close_current_file (&unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}	
	rid = tlibc_xlsx_reader_workbook_search_rid(self, sheet);
	if(rid == NULL)
	{
		goto ERROR_RET;
	}



	//load workbook rels
	ret = tlibc_unzip_locate(&unzip, "xl/_rels/workbook.xml.rels");
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	ret = tlibc_unzip_open_current_file(&unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	self->workbook_rels_buff_size = unzip.cur_file_info.uncompressed_size;
	self->workbook_rels_buff = (char*)malloc(self->workbook_rels_buff_size);
	ret = tlibc_read_current_file(&unzip, self->workbook_rels_buff, &self->workbook_rels_buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	ret = tlibc_unzip_close_current_file (&unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	file = tlibc_xlsx_reader_workbook_rels_search_file(self, rid);
	strncpy(sheet_file + strlen(XL_PREFIX), file, TLIBC_MAX_PATH_LENGTH - strlen(XL_PREFIX) - 1);



	//load shared string
	ret = tlibc_unzip_locate(&unzip, "xl/sharedStrings.xml");
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	ret = tlibc_unzip_open_current_file(&unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	self->sharedstring_buff_size = unzip.cur_file_info.uncompressed_size;
	self->sharedstring_buff = (char*)malloc(self->sharedstring_buff_size);	
	ret = tlibc_read_current_file(&unzip, self->sharedstring_buff, &self->sharedstring_buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	ret = tlibc_unzip_close_current_file (&unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	tlibc_xlsx_reader_load_sharedstring(self);


	//load sheet
	ret = tlibc_unzip_locate(&unzip, sheet_file);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	ret = tlibc_unzip_open_current_file(&unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	self->sheet_buff_size = unzip.cur_file_info.uncompressed_size;
	self->sheet_buff = (char*)malloc(self->sheet_buff_size);	
	ret = tlibc_read_current_file(&unzip, self->sheet_buff, &self->sheet_buff_size);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	ret = tlibc_unzip_close_current_file (&unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}
	tlibc_xlsx_reader_loadsheet(self);



	ret = tlibc_unzip_fini(&unzip);
	if(ret != E_TLIBC_NOERROR)
	{
		goto ERROR_RET;
	}


	tlibc_abstract_reader_init(&self->super);


	return E_TLIBC_NOERROR;
ERROR_RET:
	return E_TLIBC_ERROR;
}
