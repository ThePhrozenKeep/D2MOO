#include <Windows.h>

#include <D2Dll.h>


#undef D2FUNC_DLL_NP
#define D2FUNC_DLL_NP D2FUNC_DLL_NP_STUB

#include <Storm.h>

// NOLINTBEGIN(bugprone-branch-clone)
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
// NOLINTEND(bugprone-branch-clone)
