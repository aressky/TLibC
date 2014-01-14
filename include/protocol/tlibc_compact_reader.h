#ifndef _H_TLIBC_COMPACT_READER
#define _H_TLIBC_COMPACT_READER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_reader.h"

typedef struct _TLIBC_COMPACT_READER
{
	TLIBC_ABSTRACT_READER super;

	const char *addr;
	tuint32 size;
	tuint32 offset;
}TLIBC_COMPACT_READER;

TLIBC_API void tlibc_compact_reader_init(TLIBC_COMPACT_READER *self, const void *addr, tuint32 size);

//virtual public functions
TLIBC_API tint32 tlibc_compact_read_tint8(TLIBC_ABSTRACT_READER *super, tint8 *val);

TLIBC_API tint32 tlibc_compact_read_tint16(TLIBC_ABSTRACT_READER *super, tint16 *val);

TLIBC_API tint32 tlibc_compact_read_tint32(TLIBC_ABSTRACT_READER *super, tint32 *val);

TLIBC_API tint32 tlibc_compact_read_tint64(TLIBC_ABSTRACT_READER *super, tint64 *val);

TLIBC_API tint32 tlibc_compact_read_tuint8(TLIBC_ABSTRACT_READER *super, tuint8 *val);

TLIBC_API tint32 tlibc_compact_read_tuint16(TLIBC_ABSTRACT_READER *super, tuint16 *val);

TLIBC_API tint32 tlibc_compact_read_tuint32(TLIBC_ABSTRACT_READER *super, tuint32 *val);

TLIBC_API tint32 tlibc_compact_read_tuint64(TLIBC_ABSTRACT_READER *super, tuint64 *val);

TLIBC_API tint32 tlibc_compact_read_tchar(TLIBC_ABSTRACT_READER *super, char *val);

TLIBC_API tint32 tlibc_compact_read_tdouble(TLIBC_ABSTRACT_READER *super, double *val);

TLIBC_API tint32 tlibc_compact_read_tstring(TLIBC_ABSTRACT_READER *super, tchar* str, tuint32 str_length);

TLIBC_API tint32 tlibc_compact_read_enum_number(TLIBC_ABSTRACT_READER *super, tint32 *val);

#endif
