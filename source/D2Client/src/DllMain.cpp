#include <Windows.h>
#include <D2CommonDefinitions.h>
#include <Fog.h>

#undef D2FUNC_DLL
#define D2FUNC_DLL D2FUNC_DLL_STUB
#include <D2Client.h>


//D2Client.0x6FB74108
D2ClientInterface gD2ClientInterface_6FB74108{ &Entrypoint };
//D2Client.0x6FB9A70C
HMODULE hD2ClientModule_6FB9A70C;

//D2Client.0x6FAA11D0
int __fastcall Entrypoint(int dwParam)
{
	return EntrypointMain(hD2ClientModule_6FB9A70C, dwParam);
}

//D2Client.0x6FAA2050
int __stdcall EntrypointMain(HMODULE hModule, int)
{
	UNIMPLEMENTED();
	return 0;
}

D2CLIENT_DLL_DECL D2ClientInterface* __cdecl QueryInterface()
{
	return &gD2ClientInterface_6FB74108;
}

//D2Client.6FAAB370
int __fastcall D2Client_Main_sub()
{
	UNIMPLEMENTED();
	return 0;
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
