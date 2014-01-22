#ifndef _H_TLIBC_WRITER_WRITER
#define _H_TLIBC_WRITER_WRITER

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_abstract_writer.h"

typedef struct _TLIBC_BINARY_WRITER
{
	TLIBC_ABSTRACT_WRITER super;

	char *addr;
	tuint32 size;
	tuint32 offset;
}TLIBC_BINARY_WRITER;

TLIBC_API void tlibc_binary_writer_init(TLIBC_BINARY_WRITER *self, char *addr, tuint32 size);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tint8(TLIBC_ABSTRACT_WRITER *super, const tint8 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tint16(TLIBC_ABSTRACT_WRITER *super, const tint16 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tint32(TLIBC_ABSTRACT_WRITER *super, const tint32 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tint64(TLIBC_ABSTRACT_WRITER *super, const tint64 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tuint8(TLIBC_ABSTRACT_WRITER *super, const tuint8 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tuint16(TLIBC_ABSTRACT_WRITER *super, const tuint16 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tuint32(TLIBC_ABSTRACT_WRITER *super, const tuint32 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tuint64(TLIBC_ABSTRACT_WRITER *super, const tuint64 *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tchar(TLIBC_ABSTRACT_WRITER *super, const char *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tdouble(TLIBC_ABSTRACT_WRITER *super, const double *val);

TLIBC_API TLIBC_ERROR_CODE tlibc_binary_write_tstring(TLIBC_ABSTRACT_WRITER *super, const tchar* str);

#endif
