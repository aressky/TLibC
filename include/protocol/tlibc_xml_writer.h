#ifndef _H_TLIBC_XML_WRITER
#define _H_TLIBC_XML_WRITER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_writer.h"
#include "lib/tlibc_error_code.h"

#include <stdio.h>


#define MAX_VECTOR_DEEP 1024
typedef struct _TLIBC_XML_WRITER
{
	TLIBC_ABSTRACT_WRITER super;
	tuint32 count;
	FILE *f;
	int need_tab;

	int skip_uint16_field_once;

	int skip_int32_once;
}TLIBC_XML_WRITER;

TLIBC_ERROR_CODE xml_writer_init(TLIBC_XML_WRITER *self, const char *file_name);

TLIBC_API void xml_writer_fini(TLIBC_XML_WRITER *self);

//virtual functions
TLIBC_API tint32 xml_write_vector_begin(TLIBC_ABSTRACT_WRITER *super);

TLIBC_API tint32 xml_write_vector_end(TLIBC_ABSTRACT_WRITER *super);

TLIBC_API tint32 xml_write_field_begin(TLIBC_ABSTRACT_WRITER *super, const char *var_name);

TLIBC_API tint32 xml_write_field_end(TLIBC_ABSTRACT_WRITER *super, const char *var_name);

TLIBC_API tint32 xml_write_enum_name(TLIBC_ABSTRACT_WRITER *super, const tchar *enum_name);


TLIBC_API tint32 xml_write_tint8(TLIBC_ABSTRACT_WRITER *super, const tint8 *val);

TLIBC_API tint32 xml_write_tint16(TLIBC_ABSTRACT_WRITER *super, const tint16 *val);

TLIBC_API tint32 xml_write_tint32(TLIBC_ABSTRACT_WRITER *super, const tint32 *val);

TLIBC_API tint32 xml_write_tint64(TLIBC_ABSTRACT_WRITER *super, const tint64 *val);

TLIBC_API tint32 xml_write_tuint8(TLIBC_ABSTRACT_WRITER *super, const tuint8 *val);

TLIBC_API tint32 xml_write_tuint16(TLIBC_ABSTRACT_WRITER *super, const tuint16 *val);

TLIBC_API tint32 xml_write_tuint32(TLIBC_ABSTRACT_WRITER *super, const tuint32 *val);

TLIBC_API tint32 xml_write_tuint64(TLIBC_ABSTRACT_WRITER *super, const tuint64 *val);

TLIBC_API tint32 xml_write_tstring(TLIBC_ABSTRACT_WRITER *super, const tchar* str);

TLIBC_API tint32 xml_write_tdouble(TLIBC_ABSTRACT_WRITER *super, const double *val);

TLIBC_API tint32 xml_write_tchar(TLIBC_ABSTRACT_WRITER *super, const tchar *val);


TLIBC_API tint32 xml_write_vector_item_begin(TLIBC_ABSTRACT_WRITER *super, tuint32 index);

TLIBC_API tint32 xml_write_vector_item_end(TLIBC_ABSTRACT_WRITER *super, tuint32 index);

TLIBC_API tint32 xml_write_struct_begin(TLIBC_ABSTRACT_WRITER *super, const char *struct_name);

TLIBC_API tint32 xml_write_struct_end(TLIBC_ABSTRACT_WRITER *super, const char *struct_name);

TLIBC_API tint32 xml_write_enum_begin(TLIBC_ABSTRACT_WRITER *super, const char *enum_name);

TLIBC_API tint32 xml_write_enum_end(TLIBC_ABSTRACT_WRITER *super, const char *enum_name);

#endif
