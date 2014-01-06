#ifndef _H_TLIBC_PLATFORM_COMMON
#define _H_TLIBC_PLATFORM_COMMON

typedef signed char tint8;
typedef signed short int tint16;
typedef signed int tint32;
typedef signed long long tint64;
typedef unsigned char tuint8;
typedef unsigned short int tuint16;
typedef unsigned int tuint32;
typedef unsigned long long tuint64;
typedef char tchar;
typedef double tdouble;
typedef struct _tbytes
{
	char *ptr;
	tuint32 len;
}tbytes;

typedef tchar* tstring;
typedef tint32 tbool;

#ifndef NULL
#define NULL				0
#endif


#define TLIBC_UINT64_MAX ((((tuint64)0xffffffffUL) << 32) | 0xffffffffUL)
#define TLIBC_INT64_MAX  ((((tint64) 0x7fffffffL) << 32) | 0xffffffffL)
#define TLIBC_INT64_MIN  ((-TLIBC_INT64_MAX) - 1)
#define TLIBC_UINT32_MAX ((tuint32)0xffffffffUL)
#define TLIBC_INT32_MAX  ((tint32) 0x7fffffffL)
#define TLIBC_INT32_MIN  ((-TLIBC_INT32_MAX) - 1)
#define TLIBC_UINT16_MAX ((tuint16)0xffffUL)
#define TLIBC_INT16_MAX  ((tint16) 0x7fffL)
#define TLIBC_INT16_MIN  ((-TLIBC_INT16_MAX) - 1)
#define TLIBC_UINT8_MAX  255
#define TLIBC_INT8_MAX   127
#define TLIBC_INT8_MIN   ((-TLIBC_INT8_MAX) - 1)

#define TLIBC_BYTE_ORDER (0x1234 & 0xff)
#define TLIBC_LITTLE_ENDIAN (0x34)
#define TLIBC_BIG_ENDIAN (0x12)


#define TLIBC_MAX_ENUM_LENGTH 512
#define TLIBC_MAX_FILE_PATH_LENGTH 1024

#define TLIBC_OFFSET_OF(type, member) ((size_t) &((type *)0)->member)

#define TLIBC_CONTAINER_OF(ptr, type, member) ((type *)((size_t)ptr - TLIBC_OFFSET_OF(type, member)))


#ifndef TLIBC_IN
#define TLIBC_IN
#endif

#ifndef TLIBC_OUT
#define TLIBC_OUT
#endif

#ifndef TLIBC_INOUT
#define TLIBC_INOUT
#endif

#if (defined (SHARED_EXPORTS))
#define TLIBC_API __declspec(dllexport)
#else
#define TLIBC_API extern
#endif


#define TLIBC_UNUSED(x) (void)(x)

#define hptrue 1
#define hpfalse 0
typedef enum _HPType
{
	E_TLIBC_INT8 = 0,
	E_TLIBC_INT16 = 1,
	E_TLIBC_INT32 = 2,
	E_TLIBC_INT64 = 3,
	E_TLIBC_UINT8 = 4,
	E_TLIBC_UINT16 = 5,
	E_TLIBC_UINT32 = 6,
	E_TLIBC_UINT64 = 7,
	E_TLIBC_CHAR = 8,
	E_TLIBC_BYTES = 9,
	E_TLIBC_STRING = 10,
	E_TLIBC_DOUBLE = 11,
	E_TLIBC_BOOL = 12,
	E_TLIBC_NULL = 13,
	E_TLIBC_VECTOR = 14,
	E_TLIBC_OBJECT = 15,
}HPType;

#endif