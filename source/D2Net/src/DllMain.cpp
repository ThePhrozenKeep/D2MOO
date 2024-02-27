#include <Windows.h>

#include <cstdint>

extern int32_t dword_6FC0B264;

// NOLINTBEGIN(bugprone-branch-clone)
//D2Net.0x6FC01B40
BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		dword_6FC0B264 = 0;
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}

	return TRUE;
}
// NOLINTEND(bugprone-branch-clone)
