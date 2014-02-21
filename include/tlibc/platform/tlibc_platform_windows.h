#ifndef _H_TLIBC_PLATFORM_WINDOWS
#define _H_TLIBC_PLATFORM_WINDOWS

#include <stddef.h>
//如果不包含stdlib.h strtoll会出错
#include <stdlib.h>
#include <direct.h>

#define strtoll(begin_ptr, end_ptr, radix) _strtoi64(begin_ptr, end_ptr, radix)
#define strtoull(begin_ptr, end_ptr, radix) _strtoui64(begin_ptr, end_ptr, radix)
#define snprintf _snprintf
#define snwprintf _snwprintf

#define PRIu64 "I64u"
#define PRIi64 "I64d"

#define tlibc_mkdir(path, mode) mkdir(path)

#ifdef _WIN32
#define TLIBC_WORDSIZE 32
#elif _WIN64
#define TLIBC_WORDSIZE 64
#else
#error "unknow wordsize"
#endif


//屏蔽广告
#pragma warning (disable: 4996)

#endif
