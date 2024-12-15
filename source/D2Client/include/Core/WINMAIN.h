#pragma once  
#include <D2Config.h>

#pragma pack(1)

enum D2C_AppMode : uint32_t {
	APPMODE_CONTINUE = 0,
	APPMODE_SHOULDQUIT = 1,
	APPMODE_GAME = 2,
	APPMODE_STARTUP = 3,
	APPMODE_CLOSEWINDOW = 4,
	APPMODE_JOIN_GAME = 5,
	APPMODE_LEAVE_GAME = 6,
#ifdef NO_BUG_FIX
	APPMODE_TYLER = 7,
	APPMODE_PATH = 8,
	APPMODE_TEST = 9,
	APPMODE_COMPOSITE = 10,
	APPMODE_UNK = 11,

	APPMODE_LAST = 12,
#else
	APPMODE_LAST = 7,
#endif

	APPMODE_STACK_SIZE = 4, // Actually not a stack but a queue...
};

#pragma pack()

// Helper function
D2ConfigStrc* CONFIG_GetConfig();

//1.10f: D2Client.0x6FAA2050
int __stdcall D2ClientEntrypoint(HMODULE hModule, D2ConfigStrc* pConfig);

//1.10f: 6FAA1D00
void __fastcall APPMODE_Enqueue(D2C_AppMode nAppMode);
//1.10f: Inlined
D2C_AppMode __fastcall APPMODE_Dequeue();


//1.10f: Inlined in D2Client.0x6FAADD00 APPMODE_JoinGame
void CLIENT_InitGame();

//1.10f: D2Client.0x6FAADC70
BOOL __fastcall CLIENT_FreeGame();



