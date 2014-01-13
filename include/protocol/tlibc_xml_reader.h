#ifndef _H_TLIBC_XML_READER
#define _H_TLIBC_XML_READER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_reader.h"
#include <stdio.h>

typedef struct _TLIBC_XML_READER
{
	TLIBC_ABSTRACT_READER super;

	tuint32 count;
	FILE *f;
	int need_tab;
}TLIBC_XML_READER;


TLIBC_API void xml_reader_init(TLIBC_XML_READER *self, FILE *f);

//virtual public functions

TLIBC_API tint32 xml_read_struct_begin(TLIBC_ABSTRACT_READER *self, const char *struct_name);

TLIBC_API tint32 xml_read_struct_end(TLIBC_ABSTRACT_READER *self, const char *struct_name);

TLIBC_API tint32 xml_read_vector_begin(TLIBC_ABSTRACT_READER *self);

TLIBC_API tint32 xml_read_vector_end(TLIBC_ABSTRACT_READER *self);

TLIBC_API tint32 xml_read_field_begin(TLIBC_ABSTRACT_READER *self, const char *var_name);

TLIBC_API tint32 xml_read_field_end(TLIBC_ABSTRACT_READER *self, const char *var_name);

TLIBC_API tint32 xml_read_enum_name(TLIBC_ABSTRACT_READER *super, tchar *enum_name, tuint32 enum_name_length);

TLIBC_API tint32 xml_read_tchar(TLIBC_ABSTRACT_READER *super, char *val);

TLIBC_API tint32 xml_read_tdouble(TLIBC_ABSTRACT_READER *super, double *val);

TLIBC_API tint32 xml_read_tint8(TLIBC_ABSTRACT_READER *super, tint8 *val);

TLIBC_API tint32 xml_read_tint16(TLIBC_ABSTRACT_READER *super, tint16 *val);

TLIBC_API tint32 xml_read_tint32(TLIBC_ABSTRACT_READER *super, tint32 *val);

TLIBC_API tint32 xml_read_tint64(TLIBC_ABSTRACT_READER *super, tint64 *val);

TLIBC_API tint32 xml_read_tuint8(TLIBC_ABSTRACT_READER *super, tuint8 *val);

TLIBC_API tint32 xml_read_tuint16(TLIBC_ABSTRACT_READER *super, tuint16 *val);

TLIBC_API tint32 xml_read_tuint32(TLIBC_ABSTRACT_READER *super, tuint32 *val);

TLIBC_API tint32 xml_read_tuint64(TLIBC_ABSTRACT_READER *super, tuint64 *val);


TLIBC_API tint32 xml_read_vector_item_end(TLIBC_ABSTRACT_READER *super, tuint32 index);

TLIBC_API tint32 xml_read_vector_item_begin(TLIBC_ABSTRACT_READER *super, tuint32 index);

TLIBC_API tint32 xml_read_string(TLIBC_ABSTRACT_READER *super, tchar *str, tuint32 str_len);

TLIBC_API tint32 xml_read_counter(TLIBC_ABSTRACT_READER *super, const tchar *name, tuint32 *val);

#endif
