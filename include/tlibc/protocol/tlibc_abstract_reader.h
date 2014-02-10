#ifndef _H_TLIBC_ABSTRACT_READR
#define _H_TLIBC_ABSTRACT_READR

#include "tlibc/platform/tlibc_platform.h"

#include "tlibc/core/tlibc_error_code.h"

typedef struct _TLIBC_ABSTRACT_READER TLIBC_ABSTRACT_READER;
struct _TLIBC_ABSTRACT_READER
{
	TLIBC_ERROR_CODE (*read_struct_begin)(TLIBC_ABSTRACT_READER *self, const char *struct_name);
	TLIBC_ERROR_CODE (*read_struct_end)(TLIBC_ABSTRACT_READER *self, const char *struct_name);
	TLIBC_ERROR_CODE (*read_union_begin)(TLIBC_ABSTRACT_READER *self, const char *union_name);
	TLIBC_ERROR_CODE (*read_union_end)(TLIBC_ABSTRACT_READER *self, const char *union_name);
	TLIBC_ERROR_CODE (*read_enum_begin)(TLIBC_ABSTRACT_READER *self, const char *enum_name);
	TLIBC_ERROR_CODE (*read_enum_end)(TLIBC_ABSTRACT_READER *self, const char *enum_name);
	TLIBC_ERROR_CODE (*read_vector_begin)(TLIBC_ABSTRACT_READER *self);
	TLIBC_ERROR_CODE (*read_vector_end)(TLIBC_ABSTRACT_READER *self);
	TLIBC_ERROR_CODE (*read_vector_element_begin)(TLIBC_ABSTRACT_READER *self, const char *var_name, tuint32 index);
	TLIBC_ERROR_CODE (*read_vector_element_end)(TLIBC_ABSTRACT_READER *self, const char *var_name, tuint32 index);
	TLIBC_ERROR_CODE (*read_field_begin)(TLIBC_ABSTRACT_READER *self, const char *var_name);
	TLIBC_ERROR_CODE (*read_field_end)(TLIBC_ABSTRACT_READER *self, const char *var_name);

	TLIBC_ERROR_CODE (*read_tint8)(TLIBC_ABSTRACT_READER *self, tint8 *val);
	TLIBC_ERROR_CODE (*read_tint16)(TLIBC_ABSTRACT_READER *self, tint16 *val);
	TLIBC_ERROR_CODE (*read_tint32)(TLIBC_ABSTRACT_READER *self, tint32 *val);
	TLIBC_ERROR_CODE (*read_tint64)(TLIBC_ABSTRACT_READER *self, tint64 *val);
	TLIBC_ERROR_CODE (*read_tuint8)(TLIBC_ABSTRACT_READER *self, tuint8 *val);
	TLIBC_ERROR_CODE (*read_tuint16)(TLIBC_ABSTRACT_READER *self, tuint16 *val);
	TLIBC_ERROR_CODE (*read_tuint32)(TLIBC_ABSTRACT_READER *self, tuint32 *val);
	TLIBC_ERROR_CODE (*read_tuint64)(TLIBC_ABSTRACT_READER *self, tuint64 *val);
	TLIBC_ERROR_CODE (*read_tchar)(TLIBC_ABSTRACT_READER *self, tchar *val);
	TLIBC_ERROR_CODE (*read_tdouble)(TLIBC_ABSTRACT_READER *self, tdouble *val);
	TLIBC_ERROR_CODE (*read_tstring)(TLIBC_ABSTRACT_READER *self, tchar* str, tuint32 str_length);
};

TLIBC_API void tlibc_abstract_reader_init(TLIBC_ABSTRACT_READER *self);

TLIBC_API TLIBC_ERROR_CODE tlibc_read_struct_begin(TLIBC_ABSTRACT_READER *self, const char *struct_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_struct_end(TLIBC_ABSTRACT_READER *self, const char *struct_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_union_begin(TLIBC_ABSTRACT_READER *self, const char *union_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_union_end(TLIBC_ABSTRACT_READER *self, const char *union_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_enum_begin(TLIBC_ABSTRACT_READER *self, const char *enum_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_enum_end(TLIBC_ABSTRACT_READER *self, const char *enum_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_vector_begin(TLIBC_ABSTRACT_READER *self);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_vector_end(TLIBC_ABSTRACT_READER *self);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_field_begin(TLIBC_ABSTRACT_READER *self, const char *var_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_field_end(TLIBC_ABSTRACT_READER *self, const char *var_name);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_vector_element_begin(TLIBC_ABSTRACT_READER *self, const char *var_name, tuint32 index);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_vector_element_end(TLIBC_ABSTRACT_READER *self, const char *var_name, tuint32 index);

TLIBC_API TLIBC_ERROR_CODE tlibc_read_tint8(TLIBC_ABSTRACT_READER *self, tint8 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_tint16(TLIBC_ABSTRACT_READER *self, tint16 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_tint32(TLIBC_ABSTRACT_READER *self, tint32 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_tint64(TLIBC_ABSTRACT_READER *self, tint64 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_tuint8(TLIBC_ABSTRACT_READER *self, tuint8 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_tuint16(TLIBC_ABSTRACT_READER *self, tuint16 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_tuint32(TLIBC_ABSTRACT_READER *self, tuint32 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_tuint64(TLIBC_ABSTRACT_READER *self, tuint64 *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_tchar(TLIBC_ABSTRACT_READER *self, tchar *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_tdouble(TLIBC_ABSTRACT_READER *self, tdouble *val);
TLIBC_API TLIBC_ERROR_CODE tlibc_read_tstring(TLIBC_ABSTRACT_READER *self, tchar* str, tuint32 str_length);

#endif

