#include <Windows.h>

#include <D2Dll.h>

#undef D2FUNC_DLL
#define D2FUNC_DLL D2FUNC_DLL_STUB
#undef D2FUNC_DLL_VOID
#define D2FUNC_DLL_VOID D2FUNC_DLL_VOID_STUB


#include <D2CMP.h>
