#ifndef _H_TLIBC_PLATFORM
#define _H_TLIBC_PLATFORM

#include "tlibc/platform/tlibc_platform_common.h"

#ifndef _WIN32
	#include "tlibc/platform/tlibc_platform_linux.h"
#else
	#include "tlibc/platform/tlibc_platform_windows.h"
#endif

#endif
