#ifndef _H_TLIBC_COMPACT_WRITER
#define _H_TLIBC_COMPACT_WRITER

#include "platform/tlibc_platform.h"
#include "protocol/tlibc_abstract_writer.h"

typedef struct _TLIBC_COMPACT_WRITER
{
	TLIBC_ABSTRACT_WRITER super;

	char *addr;
	tuint32 size;
	tuint32 offset;
}TLIBC_COMPACT_WRITER;

TLIBC_API void tlibc_compact_writer_init(TLIBC_COMPACT_WRITER *self, void *addr, tuint32 size);

//virtual functions
TLIBC_API tint32 tlibc_compact_write_tint8(TLIBC_ABSTRACT_WRITER *super, const tint8 val);

TLIBC_API tint32 tlibc_compact_write_tint16(TLIBC_ABSTRACT_WRITER *super, const tint16 val);

TLIBC_API tint32 tlibc_compact_write_tint32(TLIBC_ABSTRACT_WRITER *super, const tint32 val);

TLIBC_API tint32 tlibc_compact_write_tint64(TLIBC_ABSTRACT_WRITER *super, const tint64 val);

TLIBC_API tint32 tlibc_compact_write_tuint8(TLIBC_ABSTRACT_WRITER *super, const tuint8 val);

TLIBC_API tint32 tlibc_compact_write_tuint16(TLIBC_ABSTRACT_WRITER *super, const tuint16 val);

TLIBC_API tint32 tlibc_compact_write_tuint32(TLIBC_ABSTRACT_WRITER *super, const tuint32 val);

TLIBC_API tint32 tlibc_compact_write_tuint64(TLIBC_ABSTRACT_WRITER *super, const tuint64 val);

TLIBC_API tint32 tlibc_compact_write_tchar(TLIBC_ABSTRACT_WRITER *super, const char val);

TLIBC_API tint32 tlibc_compact_write_tdouble(TLIBC_ABSTRACT_WRITER *super, const double val);

TLIBC_API tint32 tlibc_compact_write_string(TLIBC_ABSTRACT_WRITER *super, const tchar* str);

TLIBC_API tint32 tlibc_compact_write_enum_number(TLIBC_ABSTRACT_WRITER *super, const tint32 val);

#endif
