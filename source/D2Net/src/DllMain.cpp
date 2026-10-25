#include <Windows.h>

#include <cstdint>

extern int32_t gnConnectionMode;

// NOLINTBEGIN(bugprone-branch-clone)
//D2Net.0x6FC01B40
BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		gnConnectionMode = 0; // D2NET_CONNECTION_REMOTE
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
