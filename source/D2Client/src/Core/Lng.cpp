#include <Core/Lng.h>
#include <D2CommonDefinitions.h>
#include <Storm.h>
#include <Fog.h>
#define XFAULT_IMPL
#define XFAULT_MODULE_NAME "D2Client.dll"
#include <Fault.h>

//1.10f: D2Client.0x6FB9A718
CHAR gszLNGDllName[D2_MAX_PATH];
//1.10f: D2Client.0x6FB9A718
HMODULE sghLNGInstance = NULL;
//1.10f: D2Client.0x6FB9A710
HACCEL sghLNGAccTable = NULL;

HACCEL LNG_GetAccelerationTable() { return sghLNGAccTable; }

//1.10f: D2Client.0x6FAA11E0
BOOL __fastcall LNG_RegisterCmdLineArguments()
{
	SCmdRegisterArgument(0x20000, -1, "lng", gszLNGDllName, D2_MAX_PATH, 0, 0, 0);
	return 1;
}

//1.10f: D2Client.0x6FB6BC27
BOOL __fastcall PromptErrorOrCancel(LPCSTR lpText)
{
	return MessageBoxA(0, lpText, XFAULT_MODULE_NAME, (MB_OKCANCEL | MB_ICONERROR | MB_TASKMODAL)) == IDCANCEL;
}

//D2Client.0x6FBC345C
static BOOL sgbLNGInitFailed = FALSE;

//1.10f: D2Client.0x6FAA1210
BOOL __cdecl LNG_Initialize()
{
	if (gszLNGDllName[0])
	{
		sghLNGInstance = LoadLibraryExA(gszLNGDllName, 0, LOAD_LIBRARY_AS_DATAFILE);
		SRegSaveString("Diablo II", "LNG File", 0, gszLNGDllName);
	}
	if (!sghLNGInstance && SRegLoadString("Diablo II", "LNG File", (uint32_t)sghLNGInstance, gszLNGDllName, 260))
	{
		sghLNGInstance = LoadLibraryExA(gszLNGDllName, 0, LOAD_LIBRARY_AS_DATAFILE);
	}
	if (!sghLNGInstance)
	{
		sghLNGInstance = LoadLibraryExA("D2.LNG", 0, LOAD_LIBRARY_AS_DATAFILE);
	}

	if (sghLNGInstance)
	{
		sghLNGAccTable = LoadAcceleratorsA(sghLNGInstance, MAKEINTRESOURCEA(100));
		SErrRegisterMessageSource(4095, (uint32_t)sghLNGInstance, 0);
		FaultRegisterMessageSource(LNG_MessageSource);
		return TRUE;
	}
	else
	{
		if (PromptErrorOrCancel("Unable to start LNG manager."))
		{
			sgbLNGInitFailed = 1;
			__debugbreak();
			sgbLNGInitFailed = 0;
		}
		return FALSE;
	}
}

//1.10f: D2Client.0x6FAA1300
void __fastcall LNG_Shutdown()
{
	if (sghLNGInstance)
		FreeLibrary(sghLNGInstance);
	sghLNGInstance = NULL;
	sghLNGAccTable = NULL;
}

//1.10f: D2Client.0x6FAA1330
BOOL __stdcall LNG_MessageSource(DWORD dwMessageId, char* buffer, unsigned int iMaxLength)
{
	D2_ASSERT(sghLNGInstance);
	if (SErrGetErrorStr(dwMessageId, buffer, iMaxLength))
		return TRUE;
	lstrcpynA(buffer, "Stupid Lng.cpp error.", iMaxLength);
	return FALSE;
}

