#pragma once

#include <D2Dll.h>

#ifdef D2COMMON_IMPL
#define D2COMMON_DLL_DECL // We use .def files, not dllexport
#else
#define D2COMMON_DLL_DECL __declspec( dllimport )
#endif
