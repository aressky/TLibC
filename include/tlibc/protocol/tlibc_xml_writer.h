#ifndef _H_TLIBC_XML_WRITER
#define _H_TLIBC_XML_WRITER

#include "tlibc/platform/tlibc_platform.h"
#include "tlibc/protocol/tlibc_abstract_writer.h"
#include "tlibc/core/tlibc_error_code.h"

#include <stdio.h>


typedef struct _TLIBC_XML_WRITER
{
	TLIBC_ABSTRACT_WRITER super;
	uint32_t count;
	FILE *f;
	int need_tab;

	int skip_uint32_field_once;

	int ignore_int32_once;
}TLIBC_XML_WRITER;

TLIBC_ERROR_CODE tlibc_xml_writer_init(TLIBC_XML_WRITER *self, const char *file_name);

void tlibc_xml_writer_fini(TLIBC_XML_WRITER *self);


TLIBC_ERROR_CODE tlibc_xml_write_vector_begin(TLIBC_ABSTRACT_WRITER *super);

TLIBC_ERROR_CODE tlibc_xml_write_vector_end(TLIBC_ABSTRACT_WRITER *super);

TLIBC_ERROR_CODE tlibc_xml_write_field_begin(TLIBC_ABSTRACT_WRITER *super, const char *var_name);

TLIBC_ERROR_CODE tlibc_xml_write_field_end(TLIBC_ABSTRACT_WRITER *super, const char *var_name);

TLIBC_ERROR_CODE tlibc_xml_write_vector_element_begin(TLIBC_ABSTRACT_WRITER *super, const char *var_name, uint32_t index);

TLIBC_ERROR_CODE tlibc_xml_write_vector_element_end(TLIBC_ABSTRACT_WRITER *super, const char *var_name, uint32_t index);

TLIBC_ERROR_CODE tlibc_xml_write_int8(TLIBC_ABSTRACT_WRITER *super, const int8_t *val);

TLIBC_ERROR_CODE tlibc_xml_write_int16(TLIBC_ABSTRACT_WRITER *super, const int16_t *val);

TLIBC_ERROR_CODE tlibc_xml_write_int32(TLIBC_ABSTRACT_WRITER *super, const int32_t *val);

TLIBC_ERROR_CODE tlibc_xml_write_int64(TLIBC_ABSTRACT_WRITER *super, const int64_t *val);

TLIBC_ERROR_CODE tlibc_xml_write_uint8(TLIBC_ABSTRACT_WRITER *super, const uint8_t *val);

TLIBC_ERROR_CODE tlibc_xml_write_uint16(TLIBC_ABSTRACT_WRITER *super, const uint16_t *val);

TLIBC_ERROR_CODE tlibc_xml_write_uint32(TLIBC_ABSTRACT_WRITER *super, const uint32_t *val);

TLIBC_ERROR_CODE tlibc_xml_write_uint64(TLIBC_ABSTRACT_WRITER *super, const uint64_t *val);

TLIBC_ERROR_CODE tlibc_xml_write_string(TLIBC_ABSTRACT_WRITER *super, const char* str, uint32_t str_length);

TLIBC_ERROR_CODE tlibc_xml_write_double(TLIBC_ABSTRACT_WRITER *super, const double *val);

TLIBC_ERROR_CODE tlibc_xml_write_char(TLIBC_ABSTRACT_WRITER *super, const char *val);

TLIBC_ERROR_CODE tlibc_xml_write_struct_begin(TLIBC_ABSTRACT_WRITER *super, const char *struct_name);

TLIBC_ERROR_CODE tlibc_xml_write_struct_end(TLIBC_ABSTRACT_WRITER *super, const char *struct_name);

TLIBC_ERROR_CODE tlibc_xml_write_enum_begin(TLIBC_ABSTRACT_WRITER *super, const char *enum_name);

#endif
