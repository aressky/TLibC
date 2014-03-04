#ifndef _H_TLIBC_ABSTRACT_WRITER
#define _H_TLIBC_ABSTRACT_WRITER

#include "platform/tlibc_platform.h"
#include "core/tlibc_error_code.h"
#include <stdint.h>

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
	TLIBC_ERROR_CODE (*write_vector_element_begin)(TLIBC_ABSTRACT_WRITER *self, const char *var_name, uint32_t index);
	TLIBC_ERROR_CODE (*write_vector_element_end)(TLIBC_ABSTRACT_WRITER *self, const char *var_name, uint32_t index);

	TLIBC_ERROR_CODE (*write_int8)(TLIBC_ABSTRACT_WRITER *self, const int8_t *val);
	TLIBC_ERROR_CODE (*write_int16)(TLIBC_ABSTRACT_WRITER *self, const int16_t *val);
	TLIBC_ERROR_CODE (*write_int32)(TLIBC_ABSTRACT_WRITER *self, const int32_t *val);
	TLIBC_ERROR_CODE (*write_int64)(TLIBC_ABSTRACT_WRITER *self, const int64_t *val);
	TLIBC_ERROR_CODE (*write_uint8)(TLIBC_ABSTRACT_WRITER *self, const uint8_t *val);
	TLIBC_ERROR_CODE (*write_uint16)(TLIBC_ABSTRACT_WRITER *self, const uint16_t *val);
	TLIBC_ERROR_CODE (*write_uint32)(TLIBC_ABSTRACT_WRITER *self, const uint32_t *val);
	TLIBC_ERROR_CODE (*write_uint64)(TLIBC_ABSTRACT_WRITER *self, const uint64_t *val);
	TLIBC_ERROR_CODE (*write_char)(TLIBC_ABSTRACT_WRITER *self, const char *val);
	TLIBC_ERROR_CODE (*write_double)(TLIBC_ABSTRACT_WRITER *self, const double *val);
	TLIBC_ERROR_CODE (*write_string)(TLIBC_ABSTRACT_WRITER *self, const char* str, uint32_t str_length);
};

 void tlibc_abstract_writer_init(TLIBC_ABSTRACT_WRITER *self);

 TLIBC_ERROR_CODE tlibc_write_struct_begin(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);
 TLIBC_ERROR_CODE tlibc_write_struct_end(TLIBC_ABSTRACT_WRITER *self, const char *struct_name);
 TLIBC_ERROR_CODE tlibc_write_union_begin(TLIBC_ABSTRACT_WRITER *self, const char *union_name);
 TLIBC_ERROR_CODE tlibc_write_union_end(TLIBC_ABSTRACT_WRITER *self, const char *union_name);
 TLIBC_ERROR_CODE tlibc_write_enum_begin(TLIBC_ABSTRACT_WRITER *self, const char *enum_name);
 TLIBC_ERROR_CODE tlibc_write_enum_end(TLIBC_ABSTRACT_WRITER *self, const char *enum_name);
 TLIBC_ERROR_CODE tlibc_write_vector_begin(TLIBC_ABSTRACT_WRITER *self);
 TLIBC_ERROR_CODE tlibc_write_vector_end(TLIBC_ABSTRACT_WRITER *self);
 TLIBC_ERROR_CODE tlibc_write_field_begin(TLIBC_ABSTRACT_WRITER *self, const char *var_name);
 TLIBC_ERROR_CODE tlibc_write_field_end(TLIBC_ABSTRACT_WRITER *self, const char *var_name);
 TLIBC_ERROR_CODE tlibc_write_vector_element_begin(TLIBC_ABSTRACT_WRITER *self, const char *var_name, uint32_t index);
 TLIBC_ERROR_CODE tlibc_write_vector_element_end(TLIBC_ABSTRACT_WRITER *self, const char *var_name, uint32_t index);

 TLIBC_ERROR_CODE tlibc_write_int8(TLIBC_ABSTRACT_WRITER *self, const int8_t *val);
 TLIBC_ERROR_CODE tlibc_write_int16(TLIBC_ABSTRACT_WRITER *self, const int16_t *val);
 TLIBC_ERROR_CODE tlibc_write_int32(TLIBC_ABSTRACT_WRITER *self, const int32_t *val);
 TLIBC_ERROR_CODE tlibc_write_int64(TLIBC_ABSTRACT_WRITER *self, const int64_t *val);
 TLIBC_ERROR_CODE tlibc_write_uint8(TLIBC_ABSTRACT_WRITER *self, const uint8_t *val);
 TLIBC_ERROR_CODE tlibc_write_uint16(TLIBC_ABSTRACT_WRITER *self, const uint16_t *val);
 TLIBC_ERROR_CODE tlibc_write_uint32(TLIBC_ABSTRACT_WRITER *self, const uint32_t *val);
 TLIBC_ERROR_CODE tlibc_write_uint64(TLIBC_ABSTRACT_WRITER *self, const uint64_t *val);
 TLIBC_ERROR_CODE tlibc_write_char(TLIBC_ABSTRACT_WRITER *self, const char *val);	
 TLIBC_ERROR_CODE tlibc_write_double(TLIBC_ABSTRACT_WRITER *self, const double *val);
 TLIBC_ERROR_CODE tlibc_write_string(TLIBC_ABSTRACT_WRITER *self, const char *str, uint32_t str_length);


#endif

