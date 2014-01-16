#ifndef _H_TLIBC_ERROR_CODE
#define _H_TLIBC_ERROR_CODE

#include "platform/tlibc_platform.h"

typedef enum _TLIBC_ERROR_CODE
{
    E_TLIBC_NOERROR = 0,
    E_TLIBC_ERROR = -1,
	E_TLIBC_IGNORE = -2,
    E_TLIBC_OUT_OF_MEMORY = -3,    
}TLIBC_ERROR_CODE;

#define TLIBC_ERROR_CODE_NUM 15

#endif
