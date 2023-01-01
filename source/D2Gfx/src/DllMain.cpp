#include <Windows.h>


extern HWND ghWnd;


//D2Gfx.0x6FA73C70
BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, void* lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		ghWnd = nullptr;
	}

	return TRUE;
}
