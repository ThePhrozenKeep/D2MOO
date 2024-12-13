#pragma once

#include <D2Dll.h>
#include <D2CommonDefinitions.h>
#include <D2Config.h>

// 1.10f Base address: 0x6FAA0000

#ifdef D2_VERSION_110F
constexpr int D2ClientImageBase = 0x6FAA0000;

#define D2CLIENTSTUB(name, addr, ret, conv, params) \
    D2FUNC(D2CLIENT, name##_##addr, ret, conv, params, 0x##addr - D2ClientImageBase); auto name##_##addr = D2CLIENT_##name##_##addr;

#define D2CLIENTDWORDSTUB(addr) \
    D2VAR(D2CLIENT, pdword_##addr, DWORD, 0x##addr - D2ClientImageBase);
HMODULE delayedD2CLIENTDllBaseGet();
#endif

#pragma pack(1)

struct D2ClientInterface
{
	uint32_t(__fastcall* pEntrypoint)(D2ConfigStrc*);
};

#pragma pack()

D2FUNC_DLL(D2CLIENT, IsInitialized, BOOL, __stdcall, (DWORD), 0x8E00)									//D2CLIENT.#10001
D2FUNC_DLL(D2CLIENT, QUESTS_CheckQuestStatus, BYTE, __stdcall, (void* pHistory, int nQuest), 0x8EF0)	//D2CLIENT.#10002
D2FUNC_DLL(D2CLIENT, ShowWindow, DWORD, __cdecl, (), 0x1F50)											//D2CLIENT.#10003
//D2Client.0x0x11C0 (#10004)
D2CLIENT_DLL_DECL D2ClientInterface* __cdecl QueryInterface();

//D2Client.0x6FAA11D0
uint32_t __fastcall Entrypoint(D2ConfigStrc* pConfig);
