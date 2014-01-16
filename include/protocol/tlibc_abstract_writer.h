#ifndef _H_TLIBC_ABSTRACT_WRITER
#define _H_TLIBC_ABSTRACT_WRITER

#include "platform/tlibc_platform.h"
#include "core/tlibc_error_code.h"

typedef struct _TLIBC_ABSTRACT_WRITER TLIBC_ABSTRACT_WRITER;
struct _TLIBC_ABSTRACT_WRITER
{
	TLIBC_ERROR_CODE (*write_struct_begin)(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);
	TLIBC_ERROR_CODE (*write_struct_end)(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);
	TLIBC_ERROR_CODE (*write_union_begin)(TLIBC_ABSTRACT_WRITER *self, const char *union_name);
	TLIBC_ERROR_CODE (*write_union_end)(TLIBC_ABSTRACT_WRITER *self, const char *union_name);
	TLIBC_ERROR_CODE (*write_enum_begin)(TLIBC_ABSTRACT_WRITER *self, const char *enum_name);
	TLIBC_ERROR_CODE (*write_enum_end)(TLIBC_ABSTRACT_WRITER *self, const char *enum_name);
	TLIBC_ERROR_CODE (*write_vector_begin)(TLIBC_ABSTRACT_WRITER *self);
	TLIBC_ERROR_CODE (*write_vector_end)(TLIBC_ABSTRACT_WRITER *self);
	TLIBC_ERROR_CODE (*write_field_begin)(TLIBC_ABSTRACT_WRITER *self, const char *var_name);
	TLIBC_ERROR_CODE (*write_field_end)(TLIBC_ABSTRACT_WRITER *self, const char *var_name);	

	TLIBC_ERROR_CODE (*write_tint8)(TLIBC_ABSTRACT_WRITER *self, const tint8 *val);
	TLIBC_ERROR_CODE (*write_tint16)(TLIBC_ABSTRACT_WRITER *self, const tint16 *val);
	TLIBC_ERROR_CODE (*write_tint32)(TLIBC_ABSTRACT_WRITER *self, const tint32 *val);
	TLIBC_ERROR_CODE (*write_tint64)(TLIBC_ABSTRACT_WRITER *self, const tint64 *val);
	TLIBC_ERROR_CODE (*write_tuint8)(TLIBC_ABSTRACT_WRITER *self, const tuint8 *val);
	TLIBC_ERROR_CODE (*write_tuint16)(TLIBC_ABSTRACT_WRITER *self, const tuint16 *val);
	TLIBC_ERROR_CODE (*write_tuint32)(TLIBC_ABSTRACT_WRITER *self, const tuint32 *val);
	TLIBC_ERROR_CODE (*write_tuint64)(TLIBC_ABSTRACT_WRITER *self, const tuint64 *val);
	TLIBC_ERROR_CODE (*write_tchar)(TLIBC_ABSTRACT_WRITER *self, const tchar *val);	
	TLIBC_ERROR_CODE (*write_tdouble)(TLIBC_ABSTRACT_WRITER *self, const tdouble *val);
	TLIBC_ERROR_CODE (*write_tstring)(TLIBC_ABSTRACT_WRITER *self, const tchar* str);
};

TLIBC_API void tlibc_abstract_writer_init(TLIBC_ABSTRACT_WRITER *self);

TLIBC_API TLIBC_ERROR_CODE tlibc_write_struct_begin(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_struct_end(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_union_begin(TLIBC_ABSTRACT_WRITER *self, const char *union_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_union_end(TLIBC_ABSTRACT_WRITER *self, const char *union_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_enum_begin(TLIBC_ABSTRACT_WRITER *self, const char *enum_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_enum_end(TLIBC_ABSTRACT_WRITER *self, const char *enum_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_vector_begin(TLIBC_ABSTRACT_WRITER *self);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_vector_end(TLIBC_ABSTRACT_WRITER *self);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_field_begin(TLIBC_ABSTRACT_WRITER *self, const char *var_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_field_end(TLIBC_ABSTRACT_WRITER *self, const char *var_name);

TLIBC_API TLIBC_ERROR_CODE tlibc_write_tint8(TLIBC_ABSTRACT_WRITER *self, const tint8 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_tint16(TLIBC_ABSTRACT_WRITER *self, const tint16 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_tint32(TLIBC_ABSTRACT_WRITER *self, const tint32 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_tint64(TLIBC_ABSTRACT_WRITER *self, const tint64 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_tuint8(TLIBC_ABSTRACT_WRITER *self, const tuint8 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_tuint16(TLIBC_ABSTRACT_WRITER *self, const tuint16 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_tuint32(TLIBC_ABSTRACT_WRITER *self, const tuint32 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_tuint64(TLIBC_ABSTRACT_WRITER *self, const tuint64 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_tchar(TLIBC_ABSTRACT_WRITER *self, const tchar *val);	
TLIBC_API TLIBC_ERROR_CODE tlibc_write_tdouble(TLIBC_ABSTRACT_WRITER *self, const tdouble *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_write_tstring(TLIBC_ABSTRACT_WRITER *self, const tchar *str);


#endif

