#include "tlibc/protocol/tlibc_xlsx_reader.h"
#include "tlibc/core/tlibc_util.h"
#include "tlibc/protocol/tlibc_abstract_reader.h"
#include "tlibc/core/tlibc_error_code.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

TLIBC_ERROR_CODE tlibc_xlsx_reader_init(tlibc_xlsx_reader_t *self, const char *file_name)
{
	TLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;	

	tlibc_abstract_reader_init(&self->super);


	return E_TLIBC_NOERROR;
ERROR_RET:
	return ret;
}
