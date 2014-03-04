#ifndef _H_TLIBC_PLATFORM
#define _H_TLIBC_PLATFORM

#include "platform/tlibc_platform_common.h"

#ifndef _WIN32
	#include "platform/tlibc_platform_linux.h"
#else
	#include "platform/tlibc_platform_windows.h"
#endif

#endif
