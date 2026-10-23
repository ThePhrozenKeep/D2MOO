#pragma once

#if defined(_MSC_VER) && _MSC_VER < 1600
// VC6 doesnt have <stdint.h>
#include <stddef.h>

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef int intptr_t;
typedef unsigned int uintptr_t;
#else
#include <stdint.h>
#endif
