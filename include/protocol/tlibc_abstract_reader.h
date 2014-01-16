#ifndef _H_TLIBC_ABSTRACT_READR
#define _H_TLIBC_ABSTRACT_READR

#include "platform/tlibc_platform.h"

typedef struct _TLIBC_ABSTRACT_READER TLIBC_ABSTRACT_READER;
struct _TLIBC_ABSTRACT_READER
{
	tint32 (*read_struct_begin)(TLIBC_ABSTRACT_READER *self, const char *struct_name);
	tint32 (*read_struct_end)(TLIBC_ABSTRACT_READER *self, const char *struct_name);
	tint32 (*read_union_begin)(TLIBC_ABSTRACT_READER *self, const char *union_name);
	tint32 (*read_union_end)(TLIBC_ABSTRACT_READER *self, const char *union_name);
	tint32 (*read_enum_begin)(TLIBC_ABSTRACT_READER *self, const char *enum_name);
	tint32 (*read_enum_end)(TLIBC_ABSTRACT_READER *self, const char *enum_name);
	tint32 (*read_vector_begin)(TLIBC_ABSTRACT_READER *self);
	tint32 (*read_vector_end)(TLIBC_ABSTRACT_READER *self);
	tint32 (*read_field_begin)(TLIBC_ABSTRACT_READER *self, const char *var_name);
	tint32 (*read_field_end)(TLIBC_ABSTRACT_READER *self, const char *var_name);

	tint32 (*read_tint8)(TLIBC_ABSTRACT_READER *self, tint8 *val);
	tint32 (*read_tint16)(TLIBC_ABSTRACT_READER *self, tint16 *val);
	tint32 (*read_tint32)(TLIBC_ABSTRACT_READER *self, tint32 *val);
	tint32 (*read_tint64)(TLIBC_ABSTRACT_READER *self, tint64 *val);
	tint32 (*read_tuint8)(TLIBC_ABSTRACT_READER *self, tuint8 *val);
	tint32 (*read_tuint16)(TLIBC_ABSTRACT_READER *self, tuint16 *val);
	tint32 (*read_tuint32)(TLIBC_ABSTRACT_READER *self, tuint32 *val);
	tint32 (*read_tuint64)(TLIBC_ABSTRACT_READER *self, tuint64 *val);
	tint32 (*read_tchar)(TLIBC_ABSTRACT_READER *self, tchar *val);
	tint32 (*read_tdouble)(TLIBC_ABSTRACT_READER *self, tdouble *val);

	tint32 (*read_enum_name)(TLIBC_ABSTRACT_READER *self, tchar *enum_name, tuint32 enum_name_length);	
	tint32 (*read_string)(TLIBC_ABSTRACT_READER *self, tchar* str, tuint32 str_length);
};

void tlibc_abstract_reader_init(TLIBC_ABSTRACT_READER *self);

tint32 read_struct_begin(TLIBC_ABSTRACT_READER *self, const char *struct_name);
tint32 read_struct_end(TLIBC_ABSTRACT_READER *self, const char *struct_name);
tint32 read_union_begin(TLIBC_ABSTRACT_READER *self, const char *union_name);
tint32 read_union_end(TLIBC_ABSTRACT_READER *self, const char *union_name);
tint32 read_enum_begin(TLIBC_ABSTRACT_READER *self, const char *enum_name);
tint32 read_enum_end(TLIBC_ABSTRACT_READER *self, const char *enum_name);
tint32 read_vector_begin(TLIBC_ABSTRACT_READER *self);
tint32 read_vector_end(TLIBC_ABSTRACT_READER *self);
tint32 read_field_begin(TLIBC_ABSTRACT_READER *self, const char *var_name);
tint32 read_field_end(TLIBC_ABSTRACT_READER *self, const char *var_name);


tint32 read_tint8(TLIBC_ABSTRACT_READER *self, tint8 *val);
tint32 read_tint16(TLIBC_ABSTRACT_READER *self, tint16 *val);
tint32 read_tint32(TLIBC_ABSTRACT_READER *self, tint32 *val);
tint32 read_tint64(TLIBC_ABSTRACT_READER *self, tint64 *val);
tint32 read_tuint8(TLIBC_ABSTRACT_READER *self, tuint8 *val);
tint32 read_tuint16(TLIBC_ABSTRACT_READER *self, tuint16 *val);
tint32 read_tuint32(TLIBC_ABSTRACT_READER *self, tuint32 *val);
tint32 read_tuint64(TLIBC_ABSTRACT_READER *self, tuint64 *val);
tint32 read_tchar(TLIBC_ABSTRACT_READER *self, tchar *val);
tint32 read_tdouble(TLIBC_ABSTRACT_READER *self, tdouble *val);
tint32 read_tstring(TLIBC_ABSTRACT_READER *self, tchar* str, tuint32 str_length);

tint32 read_enum_name(TLIBC_ABSTRACT_READER *self, tchar *enum_name, tuint32 enum_name_length);


#endif

