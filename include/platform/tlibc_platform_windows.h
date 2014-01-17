#ifndef _H_TLIBC_PLATFORM_WINDOWS
#define _H_TLIBC_PLATFORM_WINDOWS

#ifndef _WIN32
#error "This is a Windows header only"
#endif

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

#endif
