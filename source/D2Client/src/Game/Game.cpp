#undef WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tracy/Tracy.hpp>

#include <Game/Game.h>
#include <D2Client.h>
#include <Game/Record.h>
#include <D2CommonDefinitions.h>
#include <D2Gfx.h>
#include <Window.h>
#include <D2Net.h>
#include <D2Dungeon.h>
#include <D2Environment.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgActivate.h>
#include <../../D2Game/include/GAME/Game.h>
#include <UI/UI.h>
#include <Sound/Sound.h>
#include <Client.h>
#include <D2WinPalette.h>
#include <D2WinMain.h>
#include <DataTbls/LevelsTbls.h>
#include <D2Sound.h>
#include <BnClient.h>
#include <Engine/Cursor.h>
#include <Core/WNDPROC.h>
#include <D2StrTable.h>
#include <D2CMP.h>

enum D2HotkeyIndices
{
    HOTKEY_STATSCREEN = 0x0,
    HOTKEY_INVENTORY = 0x1,
    HOTKEY_PARTY = 0x2,
    HOTKEY_MSGLOG = 0x3,
    HOTKEY_QUESTS = 0x4,
    HOTKEY_CHAT = 0x5,
    HOTKEY_HELP = 0x6,
    HOTKEY_AUTOMAP = 0x7,
    HOTKEY_MINIMAP_POSITION = 0x8,
    HOTKEY_FADEAUTOMAP = 0x9,
    HOTKEY_PARTYONMAP = 0xA,
    HOTKEY_NAMESONMAP = 0xB,
    HOTKEY_SKILLTREE = 0xC,
    HOTKEY_SKILLSELECT = 0xD,
    HOTKEY_SKILL1 = 0xE,
    HOTKEY_SKILL2 = 0xF,
    HOTKEY_SKILL3 = 0x10,
    HOTKEY_SKILL4 = 0x11,
    HOTKEY_SKILL5 = 0x12,
    HOTKEY_SKILL6 = 0x13,
    HOTKEY_SKILL7 = 0x14,
    HOTKEY_SKILL8 = 0x15,
    HOTKEY_SHOWBELT = 0x16,
    HOTKEY_POTION1 = 0x17,
    HOTKEY_POTION2 = 0x18,
    HOTKEY_POTION3 = 0x19,
    HOTKEY_POTION4 = 0x1A,
    HOTKEY_SPEECH1 = 0x1B,
    HOTKEY_SPEECH2 = 0x1C,
    HOTKEY_SPEECH3 = 0x1D,
    HOTKEY_SPEECH4 = 0x1E,
    HOTKEY_SPEECH5 = 0x1F,
    HOTKEY_SPEECH6 = 0x20,
    HOTKEY_SPEECH7 = 0x21,
    HOTKEY_RUN = 0x22,
    HOTKEY_RUNWALK = 0x23,
    HOTKEY_STANDSTILL = 0x24,
    HOTKEY_SHOWITEMS = 0x25,
    HOTKEY_CLEARSCREEN = 0x26,
    HOTKEY_PREVSKILL = 0x27,
    HOTKEY_NEXTSKILL = 0x28,
    HOTKEY_CLEARMESSAGES = 0x29,
    HOTKEY_SCREENSHOT = 0x2A,
    HOTKEY_PORTRAITS = 0x2B,
    HOTKEY_SWITCHWEAP = 0x2C,
    HOTKEY_MINIMAP = 0x2D,
    HOTKEY_SKILL9 = 0x2E,
    HOTKEY_SKILL10 = 0x2F,
    HOTKEY_SKILL11 = 0x30,
    HOTKEY_SKILL12 = 0x31,
    HOTKEY_SKILL13 = 0x32,
    HOTKEY_SKILL14 = 0x33,
    HOTKEY_SKILL15 = 0x34,
    HOTKEY_SKILL16 = 0x35,
    HOTKEY_MERCINV = 0x36,
    HOTKEY_SPEECH8 = 0x37,
    HOTKEY_ESCMENU = 0x38,
    HOTKEY_MAX_COUNT = 0x39,
};
enum D2VirtualKeys : SHORT
{
    D2VK_0 = 0x30,
    D2VK_1 = 0x31,
    D2VK_2 = 0x32,
    D2VK_3 = 0x33,
    D2VK_4 = 0x34,
    D2VK_5 = 0x35,
    D2VK_6 = 0x36,
    D2VK_7 = 0x37,
    D2VK_8 = 0x38,
    D2VK_9 = 0x39,
    D2VK_A = 0x41,
    D2VK_B = 0x42,
    D2VK_C = 0x43,
    D2VK_D = 0x44,
    D2VK_E = 0x45,
    D2VK_F = 0x46,
    D2VK_G = 0x47,
    D2VK_H = 0x48,
    D2VK_I = 0x49,
    D2VK_J = 0x4A,
    D2VK_K = 0x4B,
    D2VK_L = 0x4C,
    D2VK_M = 0x4D,
    D2VK_N = 0x4E,
    D2VK_O = 0x4F,
    D2VK_P = 0x50,
    D2VK_Q = 0x51,
    D2VK_R = 0x52,
    D2VK_S = 0x53,
    D2VK_T = 0x54,
    D2VK_U = 0x55,
    D2VK_V = 0x56,
    D2VK_W = 0x57,
    D2VK_X = 0x58,
    D2VK_Y = 0x59,
    D2VK_Z = 0x5A,
    D2VK_MBUTTON = 0x100,
    D2VK_MBUTTON4 = 0x101,
    D2VK_MBUTTON5 = 0x102,
    D2VK_WHEEL_UP = 0x103,
    D2VK_WHEEL_DOWN = 0x104,
    D2VK_INVALID = -1,
};

static const int D2MCPClientImageBase = 0x6F9F0000;

HMODULE delayedD2CLIENTDllBaseGet()
{
    static HMODULE DLLBASE_D2LANG = LoadLibraryA("D2Client.dll");
    return DLLBASE_D2LANG;
}
HMODULE delayedD2MCPClientDllBaseGet()
{
    static HMODULE DLLBASE_D2LANG = LoadLibraryA("D2MCPClient.dll");
    return DLLBASE_D2LANG;
}

HMODULE delayedD2SoundDllBaseGet()
{
    static HMODULE DLLBASE_D2LANG = LoadLibraryA("D2Sound.dll");
    return DLLBASE_D2LANG;
}

D2FUNC(D2MCPClient, 10003, void, , (), 0x6F9F1A20 - D2MCPClientImageBase);
D2FUNC(D2Sound, 10032, BOOL, , (), 0x6F984A00 - D2SoundImageBase);
D2FUNC(D2Sound, 10027, void, __fastcall, (int nVolume), 0x6F984630 - D2SoundImageBase);
D2FUNC(D2Sound, 10029, void, __fastcall, (int nDuration), 0x6F9843D0 - D2SoundImageBase);
D2FUNC(D2Sound, 10031, void, , (), 0x6F9849A0 - D2SoundImageBase);

#define D2CLIENTSTUB(name, addr, ret, conv, params) \
    D2FUNC(D2CLIENT, name##_##addr, ret, conv, params, 0x##addr - D2ClientImageBase); auto name##_##addr = D2CLIENT_##name##_##addr;

// TODO:
#define D2CLIENTDWORDSTUB(addr) \
    D2VAR(D2CLIENT, pdword_##addr, DWORD, 0x##addr - D2ClientImageBase);

D2CLIENTSTUB(GetMouseY, 6FB57BD0, int, __fastcall, ());
D2CLIENTSTUB(GetMouseX, 6FB57BC0, int, __fastcall, ());
D2CLIENTSTUB(PLAYER_unk, 6FB49920, int, __fastcall, (int a1, int nMouseX, int nMouseY, int a4));
D2CLIENTSTUB(COMMAND_FindVirtualKeyCode, 6FAD4B60, D2VirtualKeys, __fastcall, (int nHotkeyIdx, int bPrimaryHotkey));
D2CLIENTSTUB(NetUpdate, 6FAAD3B0, void, __cdecl, ());

D2CLIENTSTUB(HOTKEY_HasKeyMapping, 6FAD4F00, BOOL, __fastcall, (int nHotkeyIdx, BOOL bPrimaryHotkey));

D2CLIENTSTUB(sub, 6FB4EE70, void, __fastcall, (void*));
D2CLIENTSTUB(sub, 6FAA2040, void, __fastcall, (int a1));
D2CLIENTSTUB(D2Client_PACKETS_SendPlayerUpdates, 6FAB1CB0, void, __fastcall, (void* pPacketBuffer, int a2));
D2CLIENTSTUB(D2CLIENT_PACKETS_Handler, 6FAB50B0, void, __fastcall, (void* pPacketBuffer, int a2));
D2CLIENTSTUB(CHAT_Update, 6FAD2FB0, void, __cdecl, ());
D2CLIENTSTUB(CLIENT_SpawnMonsters, 6FB3EFB0, void, __thiscall, (D2ActiveRoomStrc* pRoom));
D2CLIENTSTUB(CLIENT_SpawnPresetUnits, 6FB27E70, void, __thiscall, (D2ActiveRoomStrc* pRoom));
D2CLIENTSTUB(CLIENT_QUEST_SpawnQuestsFX, 6FABA0D0, void, __thiscall, (D2ActiveRoomStrc* pRoom));
D2CLIENTSTUB(D2Client_UI_Automap_sub, 6FACBAF0, D2ActiveRoomStrc*, __cdecl, ()); //1.13c:0x6FB12AF0

D2CLIENTSTUB(CLIENT_UpdateUnits, 6FB280F0, void, __cdecl, ());
D2CLIENTSTUB(sub, 6FABA270, void, __cdecl, ());
D2CLIENTSTUB(sub, 6FB29390, void, __cdecl, ());
D2CLIENTSTUB(sub, 6FB5B210, DWORD, __fastcall, (BOOL));
D2CLIENTSTUB(sub, 6FB49910, void, __cdecl, ()); //1.13c:0x6FAC8550
D2CLIENTSTUB(CLIENT_UpdateUnitUnderMouse, 6FAB5D60, void, __cdecl, ());//1.13c:0x6FB01E80
D2CLIENTSTUB(UI_FreeLoadingScreenProgress, 6FB245C0, DWORD, __cdecl, ());

D2CLIENTSTUB(D2CLIENT_GAMEVIEW_GetViewRadius, 6FAB7180, void, __fastcall, (D2GameViewStrc* pGameView, int* a2, int* a3, int* a4, int* a5));
D2CLIENTSTUB(sub, 6FAB7C50, void, __fastcall, (int* a1, int* a2));

D2PTR(D2CLIENT, pgpConfigComInterface_6FBA7944, BnClientInterface*, 0x6FBA7944 - D2ClientImageBase);
D2PTR(D2CLIENT, pgpConfig_6FB9A948, D2ConfigStrc*, 0x6FB9A948 - D2ClientImageBase); //1.13c:0x6FBCB980
D2VAR(D2CLIENT, pgpView_6FBA7990, D2GameViewStrc*, 0x6FBA7990 - D2ClientImageBase);
D2CLIENTDWORDSTUB(6FB758D8);
D2CLIENTDWORDSTUB(6FB758DC);
D2CLIENTDWORDSTUB(6FBA79D8);
D2CLIENTDWORDSTUB(6FBA79DC);
void __fastcall sub_6FAB71B0(D2GameViewStrc* pView, int nOffsetX, int nOffsetY)
{
	if (pView->nOffsetX != nOffsetX || pView->nOffsetY != nOffsetY)
	{
		pView->nOffsetY = nOffsetY;
		pView->nOffsetX = nOffsetX;
		pView->dwFlags |= 1;
	}
}
D2CLIENTSTUB(sub, 6FAB7200, void, __fastcall, (D2GameViewStrc*, int a2));
D2CLIENTSTUB(ENV_Update, 6FAA7940, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAB5960, void, __fastcall, ());
D2CLIENTSTUB(D2CLIENT_GetGlobalUiStateNbPanels, 6FAB5750, int, __fastcall, ());
D2CLIENTSTUB(ENV_DrawBackground, 6FAB7260, int, __fastcall, (D2GameViewStrc* pView));
D2CLIENTSTUB(D2CLIENT_UI_DrawUI, 6FB21B70, int, __fastcall, (D2GameViewStrc* pView));
void D2CLIENT_Return_6FAA1500(){}
D2CLIENTSTUB(sub, 6FB57AC0, void, __fastcall, ());
D2CLIENTSTUB(D2CLIENT_DrawFPSInfo, 6FAA28E0, void, __fastcall, ());

D2CLIENTDWORDSTUB(6FB75814);

//1.10f:D2Client.0x6FBBC200
//1.13c:D2Client.0x6FBCBBFC
//D2UnitStrc* gpPlayerUnit_6FBBC200;
D2VAR(D2CLIENT, pgpPlayerUnit_6FBBC200, D2UnitStrc*, 0x6FBBC200 - D2ClientImageBase);

//D2Client.0x6FBA7960
//eD2GameTypes *D2CLIENT_pgnGameType_6FBA7960;
D2VAR(D2CLIENT, pgnGameType_6FBA7960, eD2GameTypes, 0x6FBA7960 - D2ClientImageBase);

using DrawFunc = BOOL(__fastcall*)(DWORD);
//D2Client.0x6FBA7754
//DrawFunc pfDraw_6FBA7754;
D2VAR(D2CLIENT, ppfDraw_6FBA7754, DrawFunc, 0x6FBA7754 - D2ClientImageBase);


D2VAR(D2CLIENT, pnNextNetUpdateTickCount_6FBA776C, DWORD, 0x6FBA776C - D2ClientImageBase);
D2VAR(D2CLIENT, pgnFpsSkipped_6FBA7780, DWORD, 0x6FBA7780 - D2ClientImageBase); //1.13c:0x6FBC9810
D2VAR(D2CLIENT, pgpCurrentAct_6FBA7984, D2DrlgActStrc*, 0x6FBA7984 - D2ClientImageBase); //1.13c:0x6FBCC3B8


D2VAR(D2CLIENT, pgnPreviousFrameStartTickCount_6FBA7760, DWORD, 0x6FBA7760 - D2ClientImageBase); // 1.13c:0x6FBC97F0
D2CLIENTDWORDSTUB(6FBA79A8);
D2CLIENTDWORDSTUB(6FB7580C);

D2VAR(D2CLIENT, pgbIsInGame_6FBA796C, DWORD, 0x6FBA796C - D2ClientImageBase); // 1.13c:0x6FBCC3A0
D2CLIENTDWORDSTUB(6FBA79B0);
D2CLIENTDWORDSTUB(6FBA79C4);
D2CLIENTDWORDSTUB(6FBA79B4);
D2CLIENTDWORDSTUB(6FBA79B8);
D2VAR(D2CLIENT, pgbRequestedToStandStill_6FBA79BC, DWORD, 0x6FBA79BC - D2ClientImageBase);
D2VAR(D2CLIENT, pgnFramesElapsed_6FBA7768, DWORD, 0x6FBA7768 - D2ClientImageBase);//1.13c:0x6FBC97F8
D2VAR(D2CLIENT, pgnFrameStartTickCount_6FBA775C, DWORD, 0x6FBA775C - D2ClientImageBase); //1.13c:0x6FBC97EC
D2VAR(D2CLIENT, pgnLastNetUpdateBeginTickCount_6FBA7770, DWORD, 0x6FBA7770 - D2ClientImageBase);

D2CLIENTDWORDSTUB(6FBA79AC);
D2CLIENTDWORDSTUB(6FB75810);
D2CLIENTDWORDSTUB(6FBA79C8);
D2CLIENTDWORDSTUB(6FBA7770);
D2CLIENTDWORDSTUB(6FBA7788);
//1.10f:D2CLient.0x6FB75460
//1.13c:D2CLient.0x6FBA4318
static int gnTargetFrameDurationMs = 40; // Never modified by the game code. 25FPS.
D2CLIENTDWORDSTUB(6FB75460);

D2VAR(D2CLIENT, pgnFPSRendered_6FBA7778, DWORD, 0x6FBA7778 - D2ClientImageBase); //1.13c:0x6FBC9808
D2VAR(D2CLIENT, pgnFramesSkippedSinceLastCheck_6FBA7784, DWORD, 0x6FBA7784 - D2ClientImageBase); //1.13c:0x6FBC980C
D2CLIENTDWORDSTUB(6FBA7780);
D2VAR(D2CLIENT, pgnRenderedFramesSinceLastCheck_6FBA777C, DWORD, 0x6FBA777C - D2ClientImageBase); //1.13c:0x6FBC980C
D2VAR(D2CLIENT, pgnFramesSinceLastUpdate_6FBA79E0, DWORD, 0x6FBA79E0 - D2ClientImageBase); //1.13c:0x6FBD3490
D2CLIENTDWORDSTUB(6FBA778C);
D2CLIENTDWORDSTUB(6FBA77EC);
D2CLIENTDWORDSTUB(6FBA7968);
D2CLIENTDWORDSTUB(6FBA7970);
D2CLIENTDWORDSTUB(6FBA799C);
D2CLIENTDWORDSTUB(6FBA7954);
D2CLIENTDWORDSTUB(6FBA7978);
D2CLIENTDWORDSTUB(6FBA797C);
D2CLIENTDWORDSTUB(6FBA77D0);
D2CLIENTDWORDSTUB(6FBA7974);
D2CLIENTDWORDSTUB(6FBA7948);
D2CLIENTDWORDSTUB(6FBA7764);
D2CLIENTDWORDSTUB(6FBAF978); // 1.13c: 0x6FBCC2E0
D2CLIENTDWORDSTUB(6FBA79A4);
D2VAR(D2CLIENT, pgnMaxPlayerCountForGameRefresh, DWORD, 0x6FB7591C - D2ClientImageBase); // Default value: 4, not modified anywhere. 1.13c:0x6FBA9F58
D2CLIENTDWORDSTUB(6FBA7998); //1.13c: 0x6FBCC3CC


D2VAR(D2CLIENT, pgnDifficulty_6FBA795C, uint8_t, 0x6FBA795C - D2ClientImageBase);
D2VAR(D2CLIENT, hAcc_6FB9A710, HACCEL, 0x6FB9A710 - D2ClientImageBase);
D2VAR(D2CLIENT, dword_6FB9A954, HACCEL, 0x6FB9A954 - D2ClientImageBase);
D2VAR(D2CLIENT, gpbWindowHasFocus, DWORD, 0x6FB747C0 - D2ClientImageBase);
D2VAR(D2CLIENT, gptOpenServerThreadLock_6FBA77D4, _RTL_CRITICAL_SECTION, 0x6FBA77D4 - D2ClientImageBase);// 1.13c: 0x6FBC97C8
D2VAR(D2CLIENT, gphOpenServerThread_6FBA7824, HANDLE, 0x6FBA7824 - D2ClientImageBase);
D2VAR(D2CLIENT, pgnOpenServerThreadId, DWORD, 0x6FBA774C - D2ClientImageBase);
D2VAR(D2CLIENT, psgbShutDownOpenServerThread_6FBA79A0, BOOL, 0x6FBA79A0 - D2ClientImageBase); //1.13c: 6FBCC3D4
D2VAR(D2CLIENT, pgtVersionInformation, _OSVERSIONINFOA, 0x6FBA78B0 - D2ClientImageBase); //1.13c:0x6FBC9888

D2CLIENTSTUB(D2Client_InitGouraudTables, 6FAB6AC0, void, __fastcall, ()); //1.13c:0x6FB3A600
D2CLIENTSTUB(D2Client_FreeGouraudTables, 6FAB6EB0, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAA1CB0, void, __fastcall, ()); //1.13c:Inlined
D2CLIENTSTUB(sub, 6FAA2840, void, __fastcall, ()); //1.13c:Inlined
D2CLIENTSTUB(sub, 6FB575F0, BOOL, __fastcall, (DWORD)); //1.13c:Inlined

D2CLIENTSTUB(sub, 6FAA8C10, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA8CC0, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA8D50, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA8E90, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA90E0, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA9090, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA8BC0, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA9100, void, __stdcall, (SMSGHANDLER_PARAMS*));

D2CLIENTSTUB(sub, 6FAA8F20, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA8F90, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA8FC0, void, __stdcall, (SMSGHANDLER_PARAMS*));

D2CLIENTSTUB(sub, 6FAA8FF0, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA9040, void, __stdcall, (SMSGHANDLER_PARAMS*));

D2CLIENTSTUB(sub, 6FAAA500, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAAA530, void, __stdcall, (SMSGHANDLER_PARAMS*));

D2CLIENTSTUB(sub, 6FAAA7D0, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAAA880, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAAA920, void, __stdcall, (SMSGHANDLER_PARAMS*));

D2CLIENTSTUB(sub, 6FAAC1B0, void, __fastcall, ());
D2CLIENTSTUB(NET_SendMessage_D2CLTSYS_JOINGAME, 6FAAD4B0, void, __fastcall, (int16_t wGameId, int nTokenId));

D2VAR(D2CLIENT, pgwJoinId, int16_t, 0x6FBA7748 - D2ClientImageBase);
D2VAR(D2CLIENT, pgnTokenId, uint32_t, 0x6FBA7890 - D2ClientImageBase);

D2CLIENTSTUB(sub, 6FAA69A0, uint32_t, __fastcall, ());
D2CLIENTSTUB(sub, 6FAA6A00, uint32_t, __fastcall, ());
D2CLIENTSTUB(CONFIG_GetCTemp, 6FAA69F0, uint32_t, __fastcall, ());
D2VAR(D2CLIENT, peConfigCharacterPlayerClass, uint32_t, 0x6FBA7894 - D2ClientImageBase);
D2VAR(D2CLIENT, pgszPlayerName, char, 0x6FBA7810 - D2ClientImageBase);//16 bytes
D2VAR(D2CLIENT, pgszGameName, char, 0x6FBA7898 - D2ClientImageBase);//24 bytes
D2CLIENTDWORDSTUB(6FBC1AF4);
D2CLIENTDWORDSTUB(6FBC1AFC);
#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 13
D2CLIENTSTUB(SoundInit, 6FB0EF90, void, __fastcall, (BOOL bSoundBackground));
#else
D2CLIENTSTUB(SoundInit, 6FB53D90, void, __fastcall, ());
#endif
D2CLIENTSTUB(D2Client_InitGame, 6FAA9450, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAAEAA0, BOOL, __fastcall, ());
D2CLIENTSTUB(sub, 6FAAEB00, BOOL, __fastcall, ());
D2CLIENTSTUB(sub, 6FAAEAF0, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAAEAD0, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FB54CB0, void, __fastcall, ());
D2CLIENTSTUB(DisplayErrorLoop, 6FAA98C0, BOOL, __stdcall, (int nIteration)); //1.13c:0x6FAF3A40
D2CLIENTSTUB(sub, 6FAAE810, void, __fastcall, ());
D2CLIENTDWORDSTUB(6FBA7950);
D2CLIENTSTUB(PlayVideo, 6FAA1680, void, __fastcall, (int nVideoId));
D2CLIENTSTUB(D2CLIENT_UI_ChangeResolution, 6FAA23B0, void, __fastcall, (int nResolutionMode));

D2CLIENTSTUB(Slideshow, 6FAAA560, BOOL, __stdcall, (int nIteration));
D2CLIENTSTUB(ShowEndGameMenu, 6FAAA980, BOOL, __stdcall, (int nIteration));
D2CLIENTSTUB(ENV_RainFreeGfx, 6FAA7440, void, __fastcall, ());
D2CLIENTSTUB(CLIENT_FreePalShifts, 6FAADFB0, void, __fastcall, ());
D2CLIENTSTUB(CLIENT_FreeUnits, 6FB28040, void, __fastcall, ());
D2CLIENTSTUB(CLIENT_DestroyCaches, 6FAA14E0, void, __fastcall, (BOOL bFreeLRU));
D2CLIENTSTUB(CLIENT_FreeGameView, 6FAB70D0, void, __fastcall, (D2GameViewStrc* pGameView));
D2CLIENTSTUB(CLIENT_DestroyUI, 6FB202C0, void, __fastcall, ());
D2CLIENTSTUB(CLIENT_FreeLightMap, 6FAA5DC0, void, __fastcall, ());
D2CLIENTSTUB(ENV_Shutdown, 6FAA6AB0, void, __fastcall, ());
D2VAR(D2CLIENT, pgpMemoryPoolSystem, void*, 0x6FBA7758 - D2ClientImageBase);
D2VAR(D2CLIENT, psgpGlobalSeed, D2SeedStrc*, 0x6FBA7988 - D2ClientImageBase);
D2CLIENTSTUB(D2Client_ShutdownSound, 6FB54860, void, __fastcall, ());
D2CLIENTSTUB(D2CLIENT_PushAppMode, 6FAA1D00, void, __fastcall, (uint32_t nAppMode));



#pragma pack(push, 1)
struct ClientGlobalData
{
	int field_0;
	BOOL(__fastcall* pfDraw_6FBA7754)(DWORD);
	int field_8;
	int gnFrameStartTickCount_6FBA775C;
	int gnPreviousFrameStartTickCount_6FBA7760;
	int field_14;
	DWORD gnFramesElapsed_6FBA7768;
	int nNextNetUpdateTickCount_6FBA776C;
	int gnLastNetUpdateBeginTickCount_6FBA7770;
	int gnPing_6FBA7774;
	int gnFPSRendered_6FBA7778;
	int gnRenderedFramesSinceLastCheck_6FBA777C;
	int gnFpsSkipped_6FBA7780;
	int gnSkippedFrameSinceLastCheck_6FBA7784;
	int field_38;
	int field_3C;
	BYTE gap40[4];
	int field_44;
	int field_48;
	int field_4C;
	int field_50;
	BYTE gap54[16];
	int field_64;
	BYTE gap68[12];
	int gnExpansionFlag_6FBA77C4;
	int gnLadderFlag_6FBA77C8;
	int field_7C;
};
#ifdef D2_VERSION_110F
static_assert(sizeof(ClientGlobalData) == 0x80, "ClientGlobalData must be of size 0x80");
#endif
#pragma pack(pop)
D2VAR(D2CLIENT, psgtGlobalData, ClientGlobalData, 0x6FBA7750 - D2ClientImageBase);


//1.00 :D2Client.0x
//1.10f:D2Client.0x6FAA25D0
//1.13c:D2Client.0x
int __fastcall ExecuteMessageLoop_6FAA25D0(int(__stdcall * pLoopBody)(int))
{
    int nIterationCount = 0;
    BOOL v3 = TRUE;
    do
    {
		struct tagMSG Msg;
		if (PeekMessageA(&Msg, 0, 0, 0, 0))
        {
            v3 = GetMessageA(&Msg, 0, 0, 0);
            if (v3)
            {
				HWND v4; // eax
				HACCEL v8;
				if (!*D2CLIENT_hAcc_6FB9A710 || (v8 = *D2CLIENT_hAcc_6FB9A710, v4 = WINDOW_GetWindow(), !TranslateAcceleratorA(v4, v8, &Msg)))
                {
                    if (*D2CLIENT_dword_6FB9A954)
                    {
						HACCEL v9 = *D2CLIENT_dword_6FB9A954;
                        TranslateAcceleratorA(WINDOW_GetWindow(), v9, &Msg);
                    }
                }
            }
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        }
        else
        {
            if (pLoopBody)
            {
				const int result = pLoopBody(nIterationCount++);
                if (result)
                    return result;
            }
#define FORCE_CPU_SLEEP 1
#if FORCE_CPU_SLEEP
			{
				// Original game will loop only to dequeue packets without ever spinning. To avoid burning the CPU, we insert 1ms sleeps. (which is not a good way to fix it, but better than nothing)
				ZoneScopedNC("Sleep", 0xFFFFFFFF);
				Sleep(1u);
			}
#else
            if (!*D2CLIENT_gpbWindowHasFocus
                && *D2CLIENT_pgnGameType_6FBA7960 != GAMETYPE_OBNET_HOST
                && *D2CLIENT_pgnGameType_6FBA7960 != GAMETYPE_LAN_HOST)
            {
				ZoneScopedNC("Sleep", 0xFFFFFFFF);
                Sleep(10u);
            }
#endif
        }
    } while (v3);
    return 1;
}
//1.00 :D2Client.0x10001CA3
//1.10f:D2Client.0x6FB283D0
D2UnitStrc* __fastcall D2CLIENT_GetPlayerUnit_6FB283D0()
{
    return *D2CLIENT_pgpPlayerUnit_6FBBC200;
}
//1.00 :D2Client.0x10002F13
//1.10f:D2Client.0x6FB29370
D2ActiveRoomStrc* __fastcall D2CLIENT_GetCurrentRoom_6FB29370()
{
    if (D2CLIENT_GetPlayerUnit_6FB283D0())
    {
        return UNITS_GetRoom(D2CLIENT_GetPlayerUnit_6FB283D0());
    }
    return nullptr;
}


//Helper function
bool HOTKEY_HasBeenPressed(int nHotkey, bool bPrimaryHotkey)
{
    if (HOTKEY_HasKeyMapping_6FAD4F00(nHotkey, bPrimaryHotkey))
    {
        const SHORT keyCode = COMMAND_FindVirtualKeyCode_6FAD4B60(nHotkey, bPrimaryHotkey);
        return keyCode != D2VK_INVALID && (GetAsyncKeyState(keyCode) & 0x8000);
    }
    return false;
}

//1.13c: D2Client.0x6FAF40C0
void D2CLIENT_DoRoomsSpawning()
{
    for (D2ActiveRoomStrc* pRoom = DUNGEON_GetRoomFromAct(*D2CLIENT_pgpCurrentAct_6FBA7984); 
        pRoom != nullptr; 
        pRoom = pRoom->pRoomNext)
    {
        if ((pRoom->dwFlags & 1) == 0)
        {
            CLIENT_SpawnMonsters_6FB3EFB0(pRoom);
            CLIENT_SpawnPresetUnits_6FB27E70(pRoom);
#ifdef D2_VERSION_113C
			// Optimization to avoid iterating rooms for noting.
			if (DUNGEON_GetLevelIdFromRoom(pRoom) == 8 && *D2CLIENT_pdword_6FBAF978)
#endif //D2_VERSION_113C
			{
				CLIENT_QUEST_SpawnQuestsFX_6FABA0D0(pRoom);
			}
            pRoom->dwFlags |= 1u;
        }
    }
}

void D2CLIENT_UpdatePlayerMousePosition(int nUpdateType)
{
    D2UnitStrc* pPlayerUnit = D2CLIENT_GetPlayerUnit_6FB283D0();
    uint32_t nFlags = 0;
    if (*D2CLIENT_pdword_6FBA79B4)
        nFlags = 8;
    if (*D2CLIENT_pdword_6FBA79B8)
        nFlags |= 8u;
    if (*D2CLIENT_pgbRequestedToStandStill_6FBA79BC)
        nFlags |= 4u;
    if (pPlayerUnit && pPlayerUnit->dwUnitType == UNIT_PLAYER)
    {
        const int nMouseX = D2CLIENT_GetMouseX_6FB57BC0();
        const int nMouseY = D2CLIENT_GetMouseY_6FB57BD0();
        D2CLIENT_PLAYER_unk_6FB49920(nUpdateType, nMouseX, nMouseY, nFlags);
    }
}
//1.10f: inlined
//1.13c: D2Client.0x6FAF4A80
void D2CLIENT_MainUpdate()
{
	ZoneScoped;
    *D2CLIENT_pgnFrameStartTickCount_6FBA775C = GetTickCount();

    D2CLIENT_DoRoomsSpawning();

	CLIENT_UpdateUnits_6FB280F0();
    DUNGEON_AnimateTiles(D2CLIENT_GetCurrentRoom_6FB29370());
    sub_6FABA270();
    sub_6FB5B210(1);
    {
		ZoneScopedN("Update env");
        if (ENVIRONMENT_UpdatePeriodOfDay(*D2CLIENT_pgpCurrentAct_6FBA7984, D2CLIENT_GetCurrentRoom_6FB29370()))
            sub_6FB29390();
    }
    D2DrlgStrc* v33 = DUNGEON_GetDrlgFromAct(*D2CLIENT_pgpCurrentAct_6FBA7984);

    {
		ZoneScopedN("DRLGACTIVATE_Update");
        DRLGACTIVATE_Update(v33);
    }
    D2Client_UI_Automap_sub_6FACBAF0();
    if (!(++ * D2CLIENT_pgnFramesElapsed_6FBA7768 % 13))
    {
		ZoneScopedN("DUNGEON_UpdateAndFreeInactiveRooms");
        DUNGEON_UpdateAndFreeInactiveRooms(*D2CLIENT_pgpCurrentAct_6FBA7984);
    }
}

static const char* sFrameName = nullptr;// "Main";
static const char* sFrameClientUpdateName = "ClientUpdate";

static bool sgbNewFrameMarkerNeeded = true;

// Scoped helper that will mark frame start/end only if notified
struct ScopedDiscontinuousFrameMarker {
	ScopedDiscontinuousFrameMarker()
	{
		if (sgbNewFrameMarkerNeeded)
		{
			if(sFrameName) FrameMarkStart(sFrameName);
		}
		sgbNewFrameMarkerNeeded = false;
	}
	~ScopedDiscontinuousFrameMarker()
	{
		if (sgbNewFrameMarkerNeeded)
		{
			if (sFrameName) FrameMarkEnd(sFrameName);
			else FrameMark;
		}
	}
};

//1.10f:Inlined
//1.13c:D2Client.0x6FAF4880
void D2Client_UpdateMouse1()
{
	*D2CLIENT_pdword_6FB7580C = 8;
	if (*D2CLIENT_pgbIsInGame_6FBA796C)
	{
		if (!*D2CLIENT_pdword_6FBA79B0)
		{
			*D2CLIENT_pdword_6FBA79C4 = TRUE;
			D2CLIENT_UpdatePlayerMousePosition(1);
		}
	}
}

//1.10f:Inlined
//1.13c:D2Client.0x6FAF46B0
void D2Client_UpdateMouse4()
{
	*D2CLIENT_pdword_6FB75810 = 8;
	if (*D2CLIENT_pgbIsInGame_6FBA796C)
	{
		*D2CLIENT_pdword_6FBA79C8 = TRUE;
		D2CLIENT_UpdatePlayerMousePosition(4);
	}
}

//1.10f:Inlined
//1.13c:D2Client.0x6FAF3310
void D2Client_UpdateStandStill()
{
	if (!HOTKEY_HasBeenPressed(HOTKEY_STANDSTILL, true))
	{
		if (!HOTKEY_HasBeenPressed(HOTKEY_STANDSTILL, false))
		{
			*D2CLIENT_pgbRequestedToStandStill_6FBA79BC = FALSE;
		}
	}
	else
	{
		*D2CLIENT_pgbRequestedToStandStill_6FBA79BC = TRUE;
	}
}

//1.10f:Inlined
//1.13c:D2Client.0x6FAF23A0
void D2Client_ComputeFPS()
{
	if ((*D2CLIENT_pgnFrameStartTickCount_6FBA775C - *D2CLIENT_pdword_6FBA7788) > (3 * 1000))
	{
		*D2CLIENT_pgnFPSRendered_6FBA7778 = *D2CLIENT_pgnRenderedFramesSinceLastCheck_6FBA777C / 3;
		*D2CLIENT_pgnFpsSkipped_6FBA7780 = *D2CLIENT_pgnFramesSkippedSinceLastCheck_6FBA7784 / 3;
		*D2CLIENT_pdword_6FBA7788 = *D2CLIENT_pgnFrameStartTickCount_6FBA775C;
		*D2CLIENT_pgnRenderedFramesSinceLastCheck_6FBA777C = 0;
		*D2CLIENT_pgnFramesSkippedSinceLastCheck_6FBA7784 = 0;
	}

}

//1.10f:D2Client.0x6FAA9850
//1.13c:D2Client.0x6FAF35D0
void D2Client_UpdateUIsIfNeeded(DWORD val)
{
	if (!*D2CLIENT_pdword_6FBA799C && !*D2CLIENT_pdword_6FBA7954 && !*D2CLIENT_pdword_6FBA797C)
	{
		if (val >= 29) *D2CLIENT_pdword_6FBA77EC = 9;
		else *D2CLIENT_pdword_6FBA77EC = val;

		CLIENT_UpdateUIs_6FB23800(1, 0);
		*D2CLIENT_pdword_6FBA799C = 600;
		*D2CLIENT_pdword_6FBA7970 = 1;
		*D2CLIENT_pgbIsInGame_6FBA796C = 0;
		*D2CLIENT_pdword_6FBA7968 = 0;
		sub_6FAA2040(0);
	}
}

//1.10f:Inlined
//1.13c:D2Client.0x6FAF35D0
void D2Client_MultiUpdateUIsIfNeeded()
{
	if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER || *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER_UNCAPPED)
		return;

	const DWORD nTickCount = GetTickCount();
	if (!D2NET_10017() && !*D2CLIENT_pdword_6FBA7970 && *D2CLIENT_pgbIsInGame_6FBA796C)
	{
		D2Client_UpdateUIsIfNeeded(7);
	}
	if (*D2CLIENT_pdword_6FBA7974 && (nTickCount - *D2CLIENT_pdword_6FBA77D0 > 180000))
	{
		D2Client_UpdateUIsIfNeeded(7);
	}
	if ((nTickCount - *D2CLIENT_pdword_6FBA7948) > 30000 && !*D2CLIENT_pgbIsInGame_6FBA796C && !*D2CLIENT_pdword_6FBA7970 && !*D2CLIENT_pdword_6FBA7974)
	{
		D2Client_UpdateUIsIfNeeded(6);
	}
	if (!*D2CLIENT_pdword_6FBA7968
		&& !*D2CLIENT_pgbIsInGame_6FBA796C
		&& !*D2CLIENT_pdword_6FBA7970
		&& !*D2CLIENT_pdword_6FBA7974
		&& (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_LAN_JOIN || *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_OBNET_JOIN)
		)
	{
		D2Client_UpdateUIsIfNeeded(6);
	}
}

//1.10f:Inlined
//1.13c:D2Client.0x6FAF2400
int __fastcall D2Client_FramesToSkipOrSomething(int nMissedFrames, int nRenderedFPS)
{
	int result = 0;
	if (nRenderedFPS < 7)
		++result;
	if (nMissedFrames > 1)
		++result;
	if (nMissedFrames > 9)
		++result;
	if (nMissedFrames > 19)
		++result;
	if (nMissedFrames > 29)
		++result;
	return result;
}

//1.00 :D2Client.0x10001B7C
//1.10f:D2Client.0x6FAA9AF0
//1.13c:D2Client.0x6FAF4B50
signed int __stdcall MainLoop_6FAA9AF0(int nIteration)
{
    BOOL bIsNewFrame = FALSE;
    BOOL bDidClientUpdate = FALSE;
	D2UnitStrc* pPlayerUnit = D2CLIENT_GetPlayerUnit_6FB283D0();
	
	ScopedDiscontinuousFrameMarker scopedDiscontinousMarker;

    if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER || *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER_UNCAPPED)
    {
        if (WINDOW_GetState())
        {
            *D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760 = GetTickCount();
            return 0;
        }
        if ((D2CLIENT_CheckUIState_6FB23860(9) || D2CLIENT_CheckUIState_6FB23860(11))
            && pPlayerUnit
            && D2CLIENT_GetCurrentRoom_6FB29370())
        {
            *D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760 = GetTickCount();
            (*D2CLIENT_ppfDraw_6FBA7754)(0);
            SOUND_UpdateEnvironmentSFX();
            return 0;
        }
    }

    RECORD_unk_6FAAEB40();
    if (*D2CLIENT_pdword_6FBA79A8)
    {
		D2Client_UpdateMouse1();
    }
    if (*D2CLIENT_pdword_6FBA79AC)
    {
		D2Client_UpdateMouse4();
    }

	// Network update
	{
		const DWORD nBeforeNetworkUpdateTickCount = GetTickCount();
		if (*D2CLIENT_pnNextNetUpdateTickCount_6FBA776C < nBeforeNetworkUpdateTickCount)
		{
			NetUpdate_6FAAD3B0();
			*D2CLIENT_pgnLastNetUpdateBeginTickCount_6FBA7770 = nBeforeNetworkUpdateTickCount;
			*D2CLIENT_pnNextNetUpdateTickCount_6FBA776C = nBeforeNetworkUpdateTickCount + 5000;
		}
	}

    const DWORD nFrameStartTickCount = GetTickCount();
    *D2CLIENT_pgnFrameStartTickCount_6FBA775C = nFrameStartTickCount;
    
	if (!*D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760) // Init
    {
        *D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760 = nFrameStartTickCount - gnTargetFrameDurationMs;
    }

	D2Client_ComputeFPS();

	unsigned int nTimeSincePreviousFrameStart = nTimeSincePreviousFrameStart = nFrameStartTickCount - *D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760;
    if ((nFrameStartTickCount - *D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760) >= gnTargetFrameDurationMs)
    {
        *D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760 = nFrameStartTickCount;
        bIsNewFrame = TRUE;
        if (nFrameStartTickCount < *D2CLIENT_pdword_6FBA778C)
        {
            int v18 = nTimeSincePreviousFrameStart - gnTargetFrameDurationMs;
            int v19 = gnTargetFrameDurationMs - 1;
            if (v18 < gnTargetFrameDurationMs - 1)
                v19 = v18;
            *D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 = 0;
            nTimeSincePreviousFrameStart = gnTargetFrameDurationMs;
            *D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760 = nFrameStartTickCount - v19;
            *D2CLIENT_pdword_6FBA7788 = nFrameStartTickCount;
            *D2CLIENT_pgnFPSRendered_6FBA7778 = 25;
            *D2CLIENT_pgnFpsSkipped_6FBA7780 = 0;
            *D2CLIENT_pgnRenderedFramesSinceLastCheck_6FBA777C = 0;
            *D2CLIENT_pgnFramesSkippedSinceLastCheck_6FBA7784 = 0;
        }
        if (*D2CLIENT_pgbIsInGame_6FBA796C)
            sub_6FB4EE70(pPlayerUnit);
    }
    int32_t dwGamesUpdateDuration = FALSE;
    
    if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER)
    {
        GAME_ProcessNetworkMessages();
        dwGamesUpdateDuration = GAME_UpdateGamesProgress(1);
        if (dwGamesUpdateDuration != 0)
            GAME_UpdateClients(TRUE, FALSE);
    }
    else if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER_UNCAPPED)
    {
        GAME_ProcessNetworkMessages();
        dwGamesUpdateDuration = GAME_UpdateGamesProgress(1);
        GAME_UpdateClients(FALSE, FALSE);
    }
    {
		ZoneScopedNC("DequeuePackets", 0xFF808000);
		uint8_t aPackatBuffer[512];
		int32_t nPacketSize;
        do
        {
			nPacketSize = CLIENT_DequeueSystemPacket(aPackatBuffer, sizeof(aPackatBuffer));
            D2Client_PACKETS_SendPlayerUpdates_6FAB1CB0(aPackatBuffer, nPacketSize);
            Sleep(0);
        } while (nPacketSize != -1);

        do
        {
			nPacketSize = CLIENT_DequeueGamePacket(aPackatBuffer, sizeof(aPackatBuffer));
            D2CLIENT_PACKETS_Handler_6FAB50B0(aPackatBuffer, nPacketSize);
            Sleep(0);
        } while (nPacketSize != -1);
    }
    CHAT_Update_6FAD2FB0();
    D2MCPClient_10003();
    D2Client_MultiUpdateUIsIfNeeded();
	if (!*D2CLIENT_pgbIsInGame_6FBA796C)
	{
		WINDOW_GetState();
	}
	else
	{
		if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER || *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER_UNCAPPED)
		{
			if (dwGamesUpdateDuration != 0)
			{
				// TODO: split main frame marker 
				sgbNewFrameMarkerNeeded = true;

				FrameMarkEnd(sFrameClientUpdateName);
				FrameMarkStart(sFrameClientUpdateName);

				D2CLIENT_MainUpdate();
				bDidClientUpdate = TRUE;

				if (nTimeSincePreviousFrameStart > gnTargetFrameDurationMs)
				{
					if (nTimeSincePreviousFrameStart >= 2 * gnTargetFrameDurationMs)
					{
						*D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 = *D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 <= 1;
						*D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760 -= (gnTargetFrameDurationMs - 1);
					}
					else
					{
						*D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 = 0;
						*D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760 -= (nTimeSincePreviousFrameStart - gnTargetFrameDurationMs);
					}
				}
				else
				{
					*D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 = 0;
				}
			}
		}
		else
		{
			if (bIsNewFrame)
			{
				//sgbNewFrameMarkerNeeded = true;

				FrameMarkEnd(sFrameClientUpdateName);
				FrameMarkStart(sFrameClientUpdateName);

				D2CLIENT_MainUpdate();
				bDidClientUpdate = TRUE;

				*D2CLIENT_pgnPreviousFrameStartTickCount_6FBA7760 += gnTargetFrameDurationMs - nTimeSincePreviousFrameStart;
				int v28 = D2Client_FramesToSkipOrSomething(nTimeSincePreviousFrameStart / gnTargetFrameDurationMs, *D2CLIENT_pgnFPSRendered_6FBA7778);
				if (*D2CLIENT_pgnFPSRendered_6FBA7778 > 28)
					v28 = 0;
				*D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 = *D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 <= v28;
			}
			else
			{
				*D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 = *D2CLIENT_pgnFPSRendered_6FBA7778 <= 22;
			}
		}
		if (*D2CLIENT_pgbIsInGame_6FBA796C)		
		{
			sub_6FB49910();
			CLIENT_UpdateUnitUnderMouse_6FAB5D60();
			if (D2Sound_10032())
			{
				ZoneScopedN("Update sound");
				D2Sound_10029(180);
				D2Sound_10031();
				D2Sound_10027(255);
			}
			if (*D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0)
			{
				// Drop this frame
				if (bIsNewFrame)
				{
					//sgbNewFrameMarkerNeeded = true;
					++*D2CLIENT_pgnFramesSkippedSinceLastCheck_6FBA7784;
				}
			}
			else if (D2CLIENT_GetCurrentRoom_6FB29370())
			{
				ZoneScopedN("Draw");
				(*D2CLIENT_ppfDraw_6FBA7754)(0);
				sgbNewFrameMarkerNeeded = true;

				++(*D2CLIENT_pdword_6FBA7764);
				++(*D2CLIENT_pgnRenderedFramesSinceLastCheck_6FBA777C);
			}
			if (bDidClientUpdate)
			{
				ZoneScopedN("SOUND_UpdateEnvironmentSFX");
				SOUND_UpdateEnvironmentSFX();
			}
			++ *D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0;
		}
	}

    if (!*D2CLIENT_pdword_6FBA7970)
        return 0;

#ifdef D2_VERSION_113C
	//TODO
	//WARDEN_unk_6FB374E0();
#endif //D2_VERSION_113C

	PALETTTE_LoadForAct(0);
    CLIENT_UpdateUIs_6FB23800(1, 0);

	eD2GameTypes nGameType = *D2CLIENT_pgnGameType_6FBA7960;
    if (nGameType == GAMETYPE_SINGLEPLAYER || nGameType == GAMETYPE_SINGLEPLAYER_UNCAPPED)
    {
		GAME_CloseAllGames();
        nGameType = *D2CLIENT_pgnGameType_6FBA7960; // GAME_CloseAllGames could change D2CLIENT_pgnGameType_6FBA7960 through messages ?
    }    
	
    if (nGameType == GAMETYPE_OBNET_HOST || nGameType == GAMETYPE_LAN_HOST)
    {
        EnterCriticalSection(D2CLIENT_gptOpenServerThreadLock_6FBA77D4);
        *D2CLIENT_psgbShutDownOpenServerThread_6FBA79A0 = 1;
        LeaveCriticalSection(D2CLIENT_gptOpenServerThreadLock_6FBA77D4);
        WaitForSingleObject(*D2CLIENT_gphOpenServerThread_6FBA7824, 10*1000);// 10s
        *D2CLIENT_gphOpenServerThread_6FBA7824 = NULL;
    }
    return 1;
}

//1.10f:D2Client.0x6FAA9640
void __fastcall D2CLIENT_DrawGameScene(DWORD a1)
{
	UI_FreeLoadingScreenProgress_6FB245C0();

	if (WINDOW_GetState() != 0)
	{
		return;
	}

	int nViewLeft;
	int nViewTop;
	int nViewRight;
	int nViewBottom;
	D2CLIENT_GAMEVIEW_GetViewRadius_6FAB7180(*D2CLIENT_pgpView_6FBA7990, &nViewLeft, &nViewTop, &nViewRight, &nViewBottom);
	D2UnitStrc* ptPlayer = D2CLIENT_GetPlayerUnit_6FB283D0();
	const int nTargetX = UNITS_GetClientCoordX(ptPlayer);
	const int nTargetY = UNITS_GetClientCoordY(ptPlayer);
	int nOffsetX = nTargetX - (nViewRight - nViewLeft) / 2;
	int nOffsetY = nTargetY - (nViewBottom - nViewTop) / 2;
	sub_6FAB7C50(&nOffsetX, &nOffsetY);
	if (*D2CLIENT_pdword_6FB758D8 != -1 || *D2CLIENT_pdword_6FB758DC != -1)
	{
		*D2CLIENT_pdword_6FBA79D8 = nOffsetX - *D2CLIENT_pdword_6FB758D8;
		*D2CLIENT_pdword_6FBA79DC = nOffsetY - *D2CLIENT_pdword_6FB758DC;
	}
	*D2CLIENT_pdword_6FB758D8 = nOffsetX;
	*D2CLIENT_pdword_6FB758DC = nOffsetY;
	sub_6FAB71B0(*D2CLIENT_pgpView_6FBA7990, nOffsetX, nOffsetY);
	sub_6FAB7200(*D2CLIENT_pgpView_6FBA7990, 0);
	{
		ZoneScopedN("Env and light update");
		ENV_Update_6FAA7940();
		BYTE pBlue[4]; // [esp+18h] [ebp-1Ch] BYREF
		BYTE pGreen[4]; // [esp+1Ch] [ebp-18h] BYREF
		BYTE pRed[4]; // [esp+20h] [ebp-14h] BYREF
		ENVIRONMENT_GetLightColorFromAct(*D2CLIENT_pgpCurrentAct_6FBA7984, pRed, pGreen, pBlue);
		D2GFX_SetAmbientColor(pRed[0], pGreen[0], pBlue[0]);
	}
	D2ActiveRoomStrc* ptRoom = D2CLIENT_GetCurrentRoom_6FB29370();
	if (ptRoom)
	{
		int nLevelId = DUNGEON_GetLevelIdFromRoom(ptRoom);

		D2LevelsTxt* ptLevelData = DATATBLS_GetLevelsTxtRecord(nLevelId);
		D2_ASSERT(ptLevelData);
		ptRoom = (D2ActiveRoomStrc*)ptLevelData->dwBlankScreen;
	}
	BOOL bShouldDraw;
	{
		ZoneScopedN("D2GFX_StartDraw");
		bShouldDraw = D2GFX_StartDraw((BOOL)(ptRoom != nullptr), 0, 0, 0);
	}
	if (bShouldDraw)
	{
		D2Win_10179();
		sub_6FAB5960();
		if (D2CLIENT_GetGlobalUiStateNbPanels_6FAB5750() != 3)
		{
			ZoneScopedN("ENV_DrawBackground");
			ENV_DrawBackground_6FAB7260(*D2CLIENT_pgpView_6FBA7990);
		}
		{
			ZoneScopedN("DrawUI");
			D2CLIENT_UI_DrawUI_6FB21B70(*D2CLIENT_pgpView_6FBA7990);
		}
		D2Win_10019();
		D2CLIENT_Return_6FAA1500();
		if (!a1)
			sub_6FB57AC0();
		D2CLIENT_DrawFPSInfo_6FAA28E0();
		if (*D2CLIENT_pdword_6FB75814)
		{
			D2GFX_ClearScreen(0);
			if (--*D2CLIENT_pdword_6FB75814 < 0)
			{
				*D2CLIENT_pdword_6FB75814 = 0;
				FOG_Trace(
					"global static overwrite error at line %d of file %s\n",
					1713,
					"C:\\projects\\D2\\head\\Diablo2\\Source\\D2Client\\GAME\\Game.cpp");
			}
		}
		ZoneScopedN("D2GFX_EndScene");
		D2GFX_EndScene();
	}
}

//1.10f:D2Client.0x6FAA1F80
BnClientInterface* __cdecl GetConfigComInt_6FAA1F80()
{
	return (*D2CLIENT_pgpConfig_6FB9A948)->pComInterface;
}

//1.10f:Inlined
//1.13c:Inlined
int GetGameTypeFromConfig()
{
	int nGameType = 0;
	if ((*D2CLIENT_pgpConfig_6FB9A948)->nArenaDifficulty == 1)
		nGameType = 1;
	if ((*D2CLIENT_pgpConfig_6FB9A948)->nArenaDifficulty == 2)
		nGameType = nGameType | 2;
	uint16_t v9 = HIWORD((*D2CLIENT_pgpConfig_6FB9A948)->dwCTemp);
	if ((v9 & 4) != 0)
		nGameType = nGameType | 4;
	if ((v9 & 0x20) != 0)
		nGameType = nGameType | 8;
	return nGameType;
}

//1.10f:D2Client.0x6FAAA3B0
//1.13c:D2Client.0x6FAF31B0
signed int __stdcall OpenServerThreadProc(int a1)
{
	int nPreviousPlayerCount = 0;
	int32_t nLastGameUpdateDuration = 0;
	while (1)
	{
		EnterCriticalSection(D2CLIENT_gptOpenServerThreadLock_6FBA77D4);
		BOOL bShutDownOpenServerThread = *D2CLIENT_psgbShutDownOpenServerThread_6FBA79A0;
		BOOL bDoUpdate = *D2CLIENT_pdword_6FBA79A4;
		LeaveCriticalSection(D2CLIENT_gptOpenServerThreadLock_6FBA77D4);
		if (bShutDownOpenServerThread)
			break;
		if (bDoUpdate)
		{
			GAME_ProcessNetworkMessages();
			nLastGameUpdateDuration = GAME_UpdateGamesProgress(1);
			if (nLastGameUpdateDuration) GAME_UpdateClients(0, 0);
		}
		if (!nLastGameUpdateDuration)
		{
			Sleep(10u);
		}
		else
		{
			Sleep(30u);
			if (BnClientInterface* pBnClient = GetConfigComInt_6FAA1F80())
			{
				const int nCurrentPlayerCount = GAME_GetPlayerUnitsCount(1);
				if (nCurrentPlayerCount != nPreviousPlayerCount)
				{
					if (nPreviousPlayerCount >= *D2CLIENT_pgnMaxPlayerCountForGameRefresh)
					{
						if (nCurrentPlayerCount < *D2CLIENT_pgnMaxPlayerCountForGameRefresh)
						{
							const int nGameType = GetGameTypeFromConfig();
							pBnClient->RefreshGame(
								(*D2CLIENT_pgpConfig_6FB9A948)->szGameName,
								(*D2CLIENT_pgpConfig_6FB9A948)->szGamePassword,
								(*D2CLIENT_pgpConfig_6FB9A948)->szGameStatstring,
								nGameType);
						}
						nPreviousPlayerCount = nCurrentPlayerCount;
					}
					else
					{
						if (nCurrentPlayerCount >= *D2CLIENT_pgnMaxPlayerCountForGameRefresh)
							pBnClient->SendKeepAlivePacket();
						nPreviousPlayerCount = nCurrentPlayerCount;
					}
				}
			}
		}
	}
	GAME_CloseAllGames();
	GAME_UpdateClients(0, 1);
	DeleteCriticalSection(D2CLIENT_gptOpenServerThreadLock_6FBA77D4);
	OutputDebugStringA("Open server thread shut down\n");
	return 1;
}

//1.10f:D2Client.0x6FAAD260
void __fastcall SendNewGamePacket(const char* szGameName)
{
	D2GSPacketClt66 tPacketClt66;
	tPacketClt66.nHeader = 0x66;
	SStrCopy(tPacketClt66.szGameName, szGameName, STORM_MAX_STR);
	switch (*D2CLIENT_pgnGameType_6FBA7960)
	{
	case GAMETYPE_SINGLEPLAYER:
		tPacketClt66.nGameType = 3;
		break;
	case GAMETYPE_OBNET_HOST:
		tPacketClt66.nGameType = 1;
		break;
	case GAMETYPE_LAN_HOST:
		tPacketClt66.nGameType = 2;
		break;
	default:
		tPacketClt66.nGameType = 0;
		break;

	}
	if (sub_6FAA6A00())
		tPacketClt66.nPlayerClass = CONFIG_GetCTemp_6FAA69F0();
	else
		tPacketClt66.nPlayerClass = *D2CLIENT_peConfigCharacterPlayerClass;
	tPacketClt66.nLocale = STRTABLE_GetLanguage();
	D2ConfigStrc* pCfg = (*D2CLIENT_pgpConfig_6FB9A948);
	if (!pCfg->nArenaFlags)
	{
		pCfg->nArenaFlags = 4;
		pCfg->nArenaFlags |= 0x100000u;
	}
	tPacketClt66.wArena = pCfg->wArena;
	tPacketClt66.nGameFlags = pCfg->nArenaFlags;
	tPacketClt66.nTemplate = pCfg->nArenaTemplate;
	tPacketClt66.unk0x2B = pCfg->_01E9[0];
	tPacketClt66.unk0x2C = pCfg->_01E9[1];
	tPacketClt66.nDifficulty = pCfg->nArenaDifficulty;
	SStrCopy(tPacketClt66.szClientName, D2CLIENT_pgszPlayerName, STORM_MAX_STR);
	CLIENT_Send(0, (const uint8_t*) &tPacketClt66, sizeof(D2GSPacketClt66));
	*D2CLIENT_pdword_6FBC1AF4 += sizeof(D2GSPacketClt66);
	++(*D2CLIENT_pdword_6FBC1AFC);
}
//1.10f: 0x6FB75818
//1.13c: 0x6FBA46D0
D2WindowProcCallbackStrc gatGlobalWindowCallbacks_6FB75818[] = {
	{ D2_WINPROC_MESSAGE, WM_LBUTTONDOWN,	sub_6FAA8C10},
	{ D2_WINPROC_MESSAGE, WM_LBUTTONUP,		sub_6FAA8CC0},
	{ D2_WINPROC_MESSAGE, WM_RBUTTONDOWN,	sub_6FAA8D50},
	{ D2_WINPROC_MESSAGE, WM_RBUTTONUP,		sub_6FAA8E90},
	{ D2_WINPROC_KEYDOWN, VK_LWIN,			sub_6FAA90E0},
	{ D2_WINPROC_KEYDOWN, VK_RWIN,			sub_6FAA90E0},
	{ D2_WINPROC_KEYDOWN, VK_APPS,			sub_6FAA90E0},
	{ D2_WINPROC_MESSAGE, WM_SYSCOMMAND,	sub_6FAA9090},
	{ D2_WINPROC_MESSAGE, WM_CLOSE,			sub_6FAA8BC0},
	{ D2_WINPROC_MESSAGE, WM_ACTIVATEAPP,	sub_6FAA9100},
};

D2WindowProcCallbackStrc stru_6FB75890{ D2_WINPROC_MESSAGE, WM_MOUSEWHEEL, sub_6FAA8F20 };
D2WindowProcCallbackStrc stru_6FB7589C[2] = {
	{ D2_WINPROC_MESSAGE, WM_MBUTTONUP,		sub_6FAA8F90 },
	{ D2_WINPROC_MESSAGE, WM_MBUTTONDOWN,	sub_6FAA8FC0 },
};
D2WindowProcCallbackStrc stru_6FB758B4[2] = {
	{ D2_WINPROC_MESSAGE, WM_XBUTTONDOWN,	sub_6FAA8FF0 },
	{ D2_WINPROC_MESSAGE, WM_XBUTTONUP,		sub_6FAA9040 },
};

D2WindowProcCallbackStrc stru_6FB758CC{ D2_WINPROC_MESSAGE, WM_NULL, sub_6FAA8F20 };

//1.10f:D2Client.0x6FB75920
D2WindowProcCallbackStrc aUnkCallbacks_6FB75920[] = {
	{ D2_WINPROC_MESSAGE, WM_LBUTTONDOWN,	sub_6FAAA500 },
	{ D2_WINPROC_MESSAGE, WM_RBUTTONDOWN,	sub_6FAAA500 },
	{ D2_WINPROC_MESSAGE, WM_KEYFIRST,		sub_6FAAA530 },
	{ D2_WINPROC_MESSAGE, WM_ACTIVATEAPP,	sub_6FAA9100 },
	{ D2_WINPROC_KEYDOWN, VK_LWIN,			sub_6FAA90E0 },
	{ D2_WINPROC_KEYDOWN, VK_RWIN,			sub_6FAA90E0 },
	{ D2_WINPROC_KEYDOWN, WM_ACTIVATEAPP,	sub_6FAA90E0 },
};

//1.10f:D2Client.0x6FB75920
D2WindowProcCallbackStrc aUnkCallbacks_6FB75978[] = {
	{ D2_WINPROC_MESSAGE, WM_LBUTTONDOWN, sub_6FAAA7D0 },
	{ D2_WINPROC_MESSAGE, WM_RBUTTONDOWN, sub_6FAAA7D0 },
	{ D2_WINPROC_MESSAGE, WM_LBUTTONUP, sub_6FAAA880 },
	{ D2_WINPROC_MESSAGE, WM_RBUTTONUP, sub_6FAAA880 },
	{ D2_WINPROC_MESSAGE, WM_KEYFIRST, sub_6FAAA920 },
	{ D2_WINPROC_MESSAGE, WM_ACTIVATEAPP, sub_6FAA9100 },
	{ D2_WINPROC_KEYDOWN, VK_LWIN, sub_6FAA90E0 },
	{ D2_WINPROC_KEYDOWN, VK_RWIN, sub_6FAA90E0 },
	{ D2_WINPROC_KEYDOWN, WM_ACTIVATEAPP, sub_6FAA90E0 },
};


//1.10f:D2Client.0x6FAAB250
BOOL __cdecl sub_6FAAB250()
{
	return GetSystemMetrics(75)
		&& *D2CLIENT_pdword_6FBA7998
		&& D2CLIENT_pgtVersionInformation->dwPlatformId == 1
		&& D2CLIENT_pgtVersionInformation->dwMinorVersion == 0;
}
//1.10f:D2Client.0x6FAAB290
BOOL __cdecl sub_6FAAB290()
{
	_OSVERSIONINFOA& verInfo = *D2CLIENT_pgtVersionInformation;
	return GetSystemMetrics(75) 
		&& *D2CLIENT_pdword_6FBA7998
		&& (
			(verInfo.dwPlatformId == 1 && verInfo.dwMinorVersion != 0)
		|| (verInfo.dwPlatformId == 2 && (verInfo.dwMajorVersion == 4 || verInfo.dwMajorVersion == 5))
		);
}

//1.10f:D2Client.0x6FAAB2E0
signed int __cdecl sub_6FAAB2E0()
{
	if (!*D2CLIENT_pdword_6FBA7998) return FALSE;
	_OSVERSIONINFOA& verInfo = *D2CLIENT_pgtVersionInformation;
	return verInfo.dwPlatformId == 1
		|| verInfo.dwPlatformId == 2 && (verInfo.dwMajorVersion == 4 || verInfo.dwMajorVersion == 5);
}

//1.10f:D2Client.0x6FAAB320
BOOL __cdecl sub_6FAAB320()
{
	return *D2CLIENT_pdword_6FBA7998
		&& D2CLIENT_pgtVersionInformation->dwPlatformId != 1
		&& D2CLIENT_pgtVersionInformation->dwPlatformId == 2
		&& D2CLIENT_pgtVersionInformation->dwMajorVersion != 4
		&& D2CLIENT_pgtVersionInformation->dwMajorVersion == 5;
}

//1.10f:D2Client.0x6FAAC0A0 and Inlined
//1.13c:D2Client.0x6FAF29C0
void __cdecl sub_6FAAC0A0()
{
	if (sub_6FAAB290())
	{
		D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), &stru_6FB75890, 1u);
	}
	if (sub_6FAAB2E0())
	{
		D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), stru_6FB7589C, ARRAY_SIZE(stru_6FB7589C));
	}
	if (sub_6FAAB320())
	{
		D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), stru_6FB758B4, ARRAY_SIZE(stru_6FB758B4));
	}
	if (sub_6FAAB250())
	{
		if (gatGlobalWindowCallbacks_6FB75818[0].uMessage)
		{
			D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), &stru_6FB758CC, 1u);
		}
	}
}


//1.10f:D2Client.0x6FAAB370
//1.13c:D2Client.0x6FAF4F40
int __fastcall D2Client_Main_sub_6FAAB370()
{
	BOOL(__stdcall * v0)(LPMSG, HWND, UINT, UINT, UINT); // esi
	struct tagMSG Msg; // [esp+14h] [ebp-1Ch] BYREF

	if (!*D2CLIENT_pdword_6FBA799C)
		sub_6FAA2040(1);
	DWORD v1 = GetTickCount();
	FOG_Trace("[D2CLIENT]  Start entering at %d\n", v1);
	D2CLIENT_pgtVersionInformation->dwOSVersionInfoSize = sizeof(_OSVERSIONINFOA);
	if (GetVersionExA(D2CLIENT_pgtVersionInformation))
		*D2CLIENT_pdword_6FBA7998 = 1;
	D2Client_InitGouraudTables_6FAB6AC0();
	sub_6FAA1CB0();
	sub_6FAA2840();
	CLIENT_LoadCursors(0);
	HWND v2 = WINDOW_GetWindow();
	D2CLIENT_INPUT_RegisterCallbacks(v2, gatGlobalWindowCallbacks_6FB75818, ARRAY_SIZE(gatGlobalWindowCallbacks_6FB75818));
	sub_6FAAC1B0();
	if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_BNET
		|| *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_OBNET_JOIN
		|| *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_LAN_JOIN)
	{
		NET_SendMessage_D2CLTSYS_JOINGAME_6FAAD4B0(*D2CLIENT_pgwJoinId, *D2CLIENT_pgnTokenId);
	}
	else
	{
		sub_6FAA69A0();
		SendNewGamePacket(D2CLIENT_pgszGameName);
		if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_OBNET_HOST || *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_LAN_HOST)
		{
			*D2CLIENT_psgbShutDownOpenServerThread_6FBA79A0 = 0;
			*D2CLIENT_pdword_6FBA79A4 = 0;
			InitializeCriticalSection(D2CLIENT_gptOpenServerThreadLock_6FBA77D4);
			*D2CLIENT_gphOpenServerThread_6FBA7824 = CreateThread(
				0,
				0,
				(LPTHREAD_START_ROUTINE)OpenServerThreadProc,
				0,
				0,
				D2CLIENT_pgnOpenServerThreadId);
		}
	}
#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 13
	D2CLIENT_SoundInit_6FB0EF90((*D2CLIENT_pgpConfig_6FB9A948)->bSoundBackground);
#else
	D2CLIENT_SoundInit_6FB53D90();
#endif
	D2Client_InitGame_6FAA9450();
	if (sub_6FAAEAA0() || sub_6FAAEB00())
	{
		v0 = PeekMessageA;
		while (PeekMessageA(&Msg, 0, 0, 0, 1u))
			;
		if (sub_6FAAEB00())
		{
			sub_6FAAEAF0();
		}
		else if (sub_6FAAEAA0())
		{
			sub_6FAAEAD0();
		}
	}
	FOG_SetMemoryPoolSystemMode(nullptr, 0);
	if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_OBNET_HOST || *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_LAN_HOST)
	{
		EnterCriticalSection(D2CLIENT_gptOpenServerThreadLock_6FBA77D4);
		*D2CLIENT_pdword_6FBA79A4 = 1;
		LeaveCriticalSection(D2CLIENT_gptOpenServerThreadLock_6FBA77D4);
	}
	DWORD v37 = GetTickCount() - v1;
	DWORD v3 = GetTickCount();
	FOG_Trace("[D2CLIENT]  Finish entering at %d.  Elapsed: %d\n", v3, v37);
	ExecuteMessageLoop_6FAA25D0(MainLoop_6FAA9AF0);// This is the main loop
	DWORD nBeginTickCount = GetTickCount();
	FOG_Trace("[D2CLIENT]  Start closing at %d.\n", nBeginTickCount);
	D2Sound_10029(180);
	sub_6FB54CB0();
	if (*D2CLIENT_pdword_6FBA799C && !*D2CLIENT_pdword_6FBA7954 && !*D2CLIENT_pdword_6FBA797C)
		ExecuteMessageLoop_6FAA25D0(DisplayErrorLoop_6FAA98C0);
	if (sub_6FAAEAA0())
		sub_6FAAE810();
	if (*D2CLIENT_pdword_6FBA7950)
	{
		DWORD v4 = GetTickCount();
		FOG_Trace("slide show start %d\n", v4);
		D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), gatGlobalWindowCallbacks_6FB75818, ARRAY_SIZE(gatGlobalWindowCallbacks_6FB75818));
		sub_6FAAC0A0();
		D2CLIENT_INPUT_RegisterCallbacks(WINDOW_GetWindow(), aUnkCallbacks_6FB75920, ARRAY_SIZE(aUnkCallbacks_6FB75920));
		ExecuteMessageLoop_6FAA25D0(Slideshow_6FAAA560);
	}
	else if (*D2CLIENT_pdword_6FBA7954)
	{
		PlayVideo_6FAA1680(5);
		D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), gatGlobalWindowCallbacks_6FB75818, ARRAY_SIZE(gatGlobalWindowCallbacks_6FB75818));
		sub_6FAAC0A0();
		D2CLIENT_UI_ChangeResolution_6FAA23B0(0);
		D2CLIENT_INPUT_RegisterCallbacks(WINDOW_GetWindow(), aUnkCallbacks_6FB75978, ARRAY_SIZE(aUnkCallbacks_6FB75978));
		ExecuteMessageLoop_6FAA25D0(ShowEndGameMenu_6FAAA980);
	}
	else if (*D2CLIENT_pdword_6FBA7978)
	{
		PlayVideo_6FAA1680(7);
		*D2CLIENT_pdword_6FBA797C = 1;
	}

	BOOL v17 = 1;
	if (*D2CLIENT_pdword_6FBA797C)
	{
		int v18 = HIBYTE((*D2CLIENT_pgpConfig_6FB9A948)->dwCTemp) & 0x1F;
		if ((*D2CLIENT_pgnDifficulty_6FBA795C))
		{
			if ((*D2CLIENT_pgnDifficulty_6FBA795C) != 1)
				goto LABEL_74;
		LABEL_72:
			if (v18 != 12)
				goto LABEL_74;
			goto LABEL_73;
		}
		if (v18 != 8)
			goto LABEL_72;
	LABEL_73:
		v17 = 0;
	LABEL_74:
		(*D2CLIENT_pgpConfig_6FB9A948)->unpackedCTemp.nCharacterSaveFlags = 
			((*D2CLIENT_pgpConfig_6FB9A948)->unpackedCTemp.nCharacterSaveFlags & 0xE0FF) 
			| (((*D2CLIENT_pgnDifficulty_6FBA795C) << 10)
			+ 1280);
		if (v17) {
			D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), gatGlobalWindowCallbacks_6FB75818, ARRAY_SIZE(gatGlobalWindowCallbacks_6FB75818));
			sub_6FAAC0A0();
			D2CLIENT_UI_ChangeResolution_6FAA23B0(2);
			D2CLIENT_INPUT_RegisterCallbacks(WINDOW_GetWindow(), aUnkCallbacks_6FB75978, ARRAY_SIZE(aUnkCallbacks_6FB75978));
			ExecuteMessageLoop_6FAA25D0(ShowEndGameMenu_6FAAA980);
		}
	}
	DWORD v25 = GetTickCount();
	FOG_Trace("game close start %d\n", v25);
	ENV_RainFreeGfx_6FAA7440();
	CLIENT_FreePalShifts_6FAADFB0();
	CLIENT_FreeUnits_6FB28040();
	if (*D2CLIENT_pgpCurrentAct_6FBA7984)
		DUNGEON_FreeAct(*D2CLIENT_pgpCurrentAct_6FBA7984);
	*D2CLIENT_pgpCurrentAct_6FBA7984 = nullptr;
	D2CMP_SpriteFreeAsyncLoads();
	D2CMP_TileFreeAsyncLoads();
	CLIENT_DestroyCaches_6FAA14E0(FALSE);
	CLIENT_FreeGameView_6FAB70D0(*D2CLIENT_pgpView_6FBA7990);
	*D2CLIENT_pgpView_6FBA7990 = nullptr;
	CLIENT_DestroyUI_6FB202C0();
	D2CLIENT_Return_6FAA1500();
	CLIENT_FreeLightMap_6FAA5DC0();
	ENV_Shutdown_6FAA6AB0();
	FOG_DestroyMemoryPoolSystem(*D2CLIENT_pgpMemoryPoolSystem);
	memset(D2CLIENT_psgtGlobalData, 0, sizeof(ClientGlobalData));
	D2_FREE(*D2CLIENT_psgpGlobalSeed);
	DWORD nGameCloseEndTickCount = GetTickCount();
	FOG_Trace("game close end %d\n", nGameCloseEndTickCount);
	D2Client_ShutdownSound_6FB54860();
	D2Client_FreeGouraudTables_6FAB6EB0();
	CLIENT_UnloadCursors();
	D2Win_10006_ClearDrawCaches();
	D2CLIENT_PushAppMode_6FAA1D00(4);
	if (*D2CLIENT_pdword_6FBA7950)
	{
		D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), aUnkCallbacks_6FB75920, ARRAY_SIZE(aUnkCallbacks_6FB75920));
		*D2CLIENT_pdword_6FBA7950 = 0;
	}
	else if (*D2CLIENT_pdword_6FBA7954)
	{
		D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), aUnkCallbacks_6FB75978, ARRAY_SIZE(aUnkCallbacks_6FB75978));
		*D2CLIENT_pdword_6FBA7954 = 0;
	}
	else if (v17 && *D2CLIENT_pdword_6FBA797C)
	{
		D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), aUnkCallbacks_6FB75978, ARRAY_SIZE(aUnkCallbacks_6FB75978));
		*D2CLIENT_pdword_6FBA797C = 0;
	}
	else {
		D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), gatGlobalWindowCallbacks_6FB75818, ARRAY_SIZE(gatGlobalWindowCallbacks_6FB75818));
		sub_6FAAC0A0();
	}
	const DWORD nElapsedTickCount = GetTickCount() - nBeginTickCount;
	const DWORD nEndTickCount = GetTickCount();
	FOG_Trace("[D2CLIENT]  Finish closing at %d.  Elapsed: %d\n", nEndTickCount, nElapsedTickCount);
	if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_BNET_BETA
		|| *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_OBNET_HOST
		|| *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_BNET
		|| *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_OBNET_JOIN)
	{
		if (*D2CLIENT_pgpConfigComInterface_6FBA7944)
			(*D2CLIENT_pgpConfigComInterface_6FBA7944)->LeaveGame();
	}
	return 6;
}

