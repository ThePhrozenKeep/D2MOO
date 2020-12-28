#include <Windows.h>

template <typename T>
constexpr auto default_or_void() -> T { return {}; }
template <>
constexpr auto default_or_void<void>() -> void {}

#define D2FUNC_DLL(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	FOG_DLL_DECL RETURN CONV DLL##_##NAME## ARGS {  	  \
		OutputDebugStringA(#NAME);						  \
		return default_or_void<RETURN>();				  \
	}													   

#include <Fog.h>

BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, void* lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
