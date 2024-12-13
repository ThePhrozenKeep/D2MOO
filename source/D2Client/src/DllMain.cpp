#include <Windows.h>
#include <D2CommonDefinitions.h>
#include <Fog.h>
#include <Core/WINMAIN.h>

#undef D2FUNC_DLL
#define D2FUNC_DLL D2FUNC_DLL_STUB
#include <D2Client.h>


//D2Client.0x6FB74108
D2ClientInterface gD2ClientInterface_6FB74108{ &Entrypoint };
//D2Client.0x6FB9A70C
HMODULE hD2ClientModule_6FB9A70C;

//D2Client.0x6FAA11D0
uint32_t __fastcall Entrypoint(D2ConfigStrc* pConfig)
{
	return D2ClientEntrypoint(hD2ClientModule_6FB9A70C, pConfig);
}


D2CLIENT_DLL_DECL D2ClientInterface* __cdecl QueryInterface()
{
	return &gD2ClientInterface_6FB74108;
}


// NOLINTBEGIN(bugprone-branch-clone)
//D2Client.0x6FAA11A0
BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, void* lpReserved)
{
	hD2ClientModule_6FB9A70C = hModule;

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
	default:
		break;
	}
	return TRUE;
}
// NOLINTEND(bugprone-branch-clone)
