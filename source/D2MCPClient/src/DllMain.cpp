#include <Windows.h>

#include <cstdint>

//stubs
void __cdecl D2MCPClientCloseMCP(){}

// NOLINTBEGIN(bugprone-branch-clone)
BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
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