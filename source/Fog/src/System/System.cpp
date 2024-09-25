#include <Fog.h>
#include <System/System.h>
#include <Windows.h>

// 1.10f: 0x6FF5A860 (#10176)
BOOL __stdcall FOG_IsWindowsNT()
{
	OSVERSIONINFOA VersionInformation{};
	VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	return GetVersionExA(&VersionInformation) && VersionInformation.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

