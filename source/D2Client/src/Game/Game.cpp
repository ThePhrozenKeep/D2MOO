#include <Windows.h>
#include <tracy/Tracy.hpp>

#include <D2Client.h>
// D2Game
#include <GAME/Game.h>
#include <GAME/Clients.h>
 // D2Client
#include <CGAME/Msg.h>
#include <CGAME/Game.h>
#include <CGAME/Record.h>
#include <CGAME/View.h>
#include <D2CommonDefinitions.h>
#include <D2Gfx.h>
#include <Window.h>
#include <D2Net.h>
#include <D2Dungeon.h>
#include <D2Environment.h>
#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgActivate.h>
#include <UI/UI.h>
#include <Sound/Sound.h>
#include <Client.h>
#include <D2WinPalette.h>
#include <D2WinMain.h>
#include <D2WinFont.h>
#include <DataTbls/LevelsTbls.h>
#include <D2Sound.h>
#include <BnClient.h>
#include <Engine/Cursor.h>
#include <Core/WNDPROC.h>
#include <D2StrTable.h>
#include <UI/CmdTbl.h>
#include <D2CMP.h>
#include <Core/WINMAIN.h>
#include <DataTbls/StringIds.h>
#include <Server.h>

#ifdef D2_VERSION_110F

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

D2CLIENTSTUB(GetMouseY, 6FB57BD0, int, __fastcall, ());
D2CLIENTSTUB(GetMouseX, 6FB57BC0, int, __fastcall, ());
D2CLIENTSTUB(PLAYER_unk, 6FB49920, int, __fastcall, (int a1, int nMouseX, int nMouseY, int a4));
D2CLIENTSTUB(NetUpdate, 6FAAD3B0, void, __cdecl, ());

D2CLIENTSTUB(sub, 6FB4EE70, void, __fastcall, (void*));
D2CLIENTSTUB(sub, 6FAA2040, void, __fastcall, (int a1));
D2CLIENTSTUB(D2Client_PACKETS_SendPlayerUpdates, 6FAB1CB0, void, __fastcall, (void* pPacketBuffer, int a2));
D2CLIENTSTUB(D2CLIENT_PACKETS_Handler, 6FAB50B0, void, __fastcall, (void* pPacketBuffer, int a2));
D2CLIENTSTUB(CHAT_Update, 6FAD2FB0, void, __cdecl, ());
D2CLIENTSTUB(CLIENT_SpawnMonsters, 6FB3EFB0, void, __thiscall, (D2ActiveRoomStrc* pRoom));
D2CLIENTSTUB(CLIENT_SpawnPresetUnits, 6FB27E70, void, __thiscall, (D2ActiveRoomStrc* pRoom));
D2CLIENTSTUB(CLIENT_QUEST_SpawnQuestsFX, 6FABA0D0, void, __thiscall, (D2ActiveRoomStrc* pRoom));
D2CLIENTSTUB(CLIENT_UI_AutomapUpdate, 6FACBAF0, D2ActiveRoomStrc*, __cdecl, ()); //1.00:0x10001FDC 1.13c:0x6FB12AF0

D2CLIENTSTUB(CLIENT_ProcessUnits, 6FB280F0, void, __cdecl, ());//1.00:0x10001BAE
D2CLIENTSTUB(CLIENT_QuestUpdate, 6FABA270, void, __cdecl, ());//1.00:0x10002D7E
D2CLIENTSTUB(CLIENT_EnvironmentUpdate, 6FB29390, void, __cdecl, ());//1.00:0x10002ED7
D2CLIENTSTUB(CLIENT_GfxProcess, 6FB5B210, DWORD, __fastcall, (BOOL));//1.00:0x10001343
D2CLIENTSTUB(sub, 6FB49910, void, __cdecl, ()); //1.13c:0x6FAC8550
D2CLIENTSTUB(CLIENT_UpdateUnitUnderMouse, 6FAB5D60, void, __cdecl, ());//1.13c:0x6FB01E80
D2CLIENTSTUB(UI_FreeLoadingScreenProgress, 6FB245C0, DWORD, __cdecl, ());

D2CLIENTSTUB(D2CLIENT_GAMEVIEW_GetViewRadius, 6FAB7180, void, __fastcall, (D2GameViewStrc* pGameView, int* a2, int* a3, int* a4, int* a5));
D2CLIENTSTUB(sub, 6FAB7C50, void, __fastcall, (int* a1, int* a2));

D2VAR(D2CLIENT, pgbAppliedConfiguration, BOOL, 0x6FB759E4 - D2ClientImageBase);
D2PTR(D2CLIENT, pgpConfigComInterface_6FBA7944, BnClientInterface*, 0x6FBA7944 - D2ClientImageBase);

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
D2CLIENTSTUB(ENV_DrawBackground, 6FAB7260, int, __fastcall, (D2GameViewStrc* pView));
D2CLIENTSTUB(D2CLIENT_UI_DrawUI, 6FB21B70, int, __fastcall, (D2GameViewStrc* pView));
void D2CLIENT_Return_6FAA1500(){}
D2CLIENTSTUB(sub, 6FB57AC0, void, __fastcall, ());
D2CLIENTSTUB(D2CLIENT_DrawFPSInfo, 6FAA28E0, void, __fastcall, ());

D2CLIENTDWORDSTUB(6FB75814);

//1.10f: D2Client.0x6FBBC200
//1.13c: D2Client.0x6FBCBBFC
//D2UnitStrc* gpPlayerUnit_6FBBC200;
D2VAR(D2CLIENT, pgpPlayerUnit_6FBBC200, D2UnitStrc*, 0x6FBBC200 - D2ClientImageBase);

//D2Client.0x6FBA7960
//eD2GameTypes *D2CLIENT_pgnGameType_6FBA7960;
D2VAR(D2CLIENT, pgnGameType_6FBA7960, eD2GameTypes, 0x6FBA7960 - D2ClientImageBase);


eD2GameTypes GAME_GetGameType() { return *D2CLIENT_pgnGameType_6FBA7960; }


using DrawFunc = BOOL(__fastcall*)(DWORD);


D2VAR(D2CLIENT, pgpCurrentAct_6FBA7984, D2DrlgActStrc*, 0x6FBA7984 - D2ClientImageBase); //1.13c:0x6FBCC3B8


D2CLIENTDWORDSTUB(6FBA79A8);
D2CLIENTDWORDSTUB(6FB7580C);

D2VAR(D2CLIENT, pgbIsInGame_6FBA796C, DWORD, 0x6FBA796C - D2ClientImageBase); // 1.13c:0x6FBCC3A0
D2CLIENTDWORDSTUB(6FBA79B0);
D2CLIENTDWORDSTUB(6FBA79C4);
D2CLIENTDWORDSTUB(6FBA79B4);
D2CLIENTDWORDSTUB(6FBA79B8);
D2VAR(D2CLIENT, pgbRequestedToStandStill_6FBA79BC, DWORD, 0x6FBA79BC - D2ClientImageBase);

D2CLIENTDWORDSTUB(6FBA79AC);
D2CLIENTDWORDSTUB(6FB75810);
D2CLIENTDWORDSTUB(6FBA79C8);
//1.10f: D2CLient.0x6FB75460
//1.13c: D2CLient.0x6FBA4318
static int gnTargetFrameDurationMs = 40; // Never modified by the game code. 25FPS.
D2CLIENTDWORDSTUB(6FB75460);

D2VAR(D2CLIENT, pgnFramesSinceLastUpdate_6FBA79E0, DWORD, 0x6FBA79E0 - D2ClientImageBase); //1.13c:0x6FBD3490
D2CLIENTDWORDSTUB(6FBA7968);
D2CLIENTDWORDSTUB(6FBA7970);
//1.10f:0x6FBA799C
uint32_t gnLoopIterationsRemaining = 0;
D2CLIENTDWORDSTUB(6FBA7954);
D2CLIENTDWORDSTUB(6FBA7958);
D2CLIENTDWORDSTUB(6FBA7978);
D2CLIENTDWORDSTUB(6FBA797C);
D2CLIENTDWORDSTUB(6FBA77D0);
D2CLIENTDWORDSTUB(6FBA7974);
D2CLIENTDWORDSTUB(6FBA7948);
D2CLIENTDWORDSTUB(6FBAF978); // 1.13c: 0x6FBCC2E0
D2CLIENTDWORDSTUB(6FBA79A4);
D2CLIENTDWORDSTUB(6FBA7BFC);
D2CLIENTDWORDSTUB(6FBA79D4);
D2VAR(D2CLIENT, pgnMaxPlayerCountForGameRefresh, DWORD, 0x6FB7591C - D2ClientImageBase); // Default value: 4, not modified anywhere. 1.13c:0x6FBA9F58

D2VAR(D2CLIENT, pgnDrawMaxWidth, int32_t, 0x6FB9A704 - D2ClientImageBase);

D2VAR(D2CLIENT, pgnDifficulty_6FBA795C, uint8_t, 0x6FBA795C - D2ClientImageBase);
D2VAR(D2CLIENT, gptOpenServerThreadLock_6FBA77D4, _RTL_CRITICAL_SECTION, 0x6FBA77D4 - D2ClientImageBase);// 1.13c: 0x6FBC97C8
D2VAR(D2CLIENT, gphOpenServerThread_6FBA7824, HANDLE, 0x6FBA7824 - D2ClientImageBase);
D2VAR(D2CLIENT, pgnOpenServerThreadId, DWORD, 0x6FBA774C - D2ClientImageBase);
D2VAR(D2CLIENT, psgbShutDownOpenServerThread_6FBA79A0, BOOL, 0x6FBA79A0 - D2ClientImageBase); //1.13c: 6FBCC3D4

D2CLIENTSTUB(D2Client_InitGouraudTables, 6FAB6AC0, void, __fastcall, ()); //1.13c:0x6FB3A600
D2CLIENTSTUB(D2Client_FreeGouraudTables, 6FAB6EB0, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAAE910, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAAF4E0, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAB14C0, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAA2850, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAA1CB0, void, __fastcall, ()); //1.13c:Inlined
D2CLIENTSTUB(sub, 6FAA2840, void, __fastcall, ()); //1.13c:Inlined
D2CLIENTSTUB(sub, 6FB575F0, BOOL, __fastcall, (DWORD)); //1.13c:Inlined

D2CLIENTSTUB(sub, 6FAA8C10, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA8CC0, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA8D50, void, __stdcall, (SMSGHANDLER_PARAMS*));
D2CLIENTSTUB(sub, 6FAA8E90, void, __stdcall, (SMSGHANDLER_PARAMS*));
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

D2CLIENTSTUB(NET_SendMessage_D2CLTSYS_JOINGAME, 6FAAD4B0, void, __fastcall, (int16_t wGameId, int nTokenId));

D2VAR(D2CLIENT, pgwJoinId, int16_t, 0x6FBA7748 - D2ClientImageBase);
D2VAR(D2CLIENT, pgnTokenId, uint32_t, 0x6FBA7890 - D2ClientImageBase);

D2CLIENTSTUB(sub, 6FAA69A0, uint32_t, __fastcall, ());
D2VAR(D2CLIENT, pgszGameName, char, 0x6FBA7898 - D2ClientImageBase);//24 bytes
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
D2CLIENTSTUB(sub, 6FAAE810, void, __fastcall, ());
D2CLIENTDWORDSTUB(6FBA7950);
D2CLIENTSTUB(PlayVideo, 6FAA1680, void, __fastcall, (int nVideoId));

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
D2VAR(D2CLIENT, psgpGlobalSeed, D2SeedStrc*, 0x6FBA7988 - D2ClientImageBase);
D2VAR(D2CLIENT, pgbDataLocalColorUseGreenBlood, BOOL, 0x6FBA794C - D2ClientImageBase);

D2CLIENTSTUB(D2Client_ShutdownSound, 6FB54860, void, __fastcall, ());
D2CLIENTSTUB(CLIENT_SetPlayerUnit, 6FB28390, void, __fastcall, (D2UnitStrc* pPlayerUnit));
D2CLIENTSTUB(D2CLIENT_OpenFile, 6FB6B91C, BOOL, __fastcall, (int a1, const char* pzFileName, HANDLE* pOutHandle, int a4));
D2CLIENTSTUB(UI_LoadingScreenInit, 6FB241A0, void, __fastcall, ());
D2CLIENTSTUB(UI_UpdateLoadingScreen, 6FB242E0, void, __fastcall, (BOOL bIncrementLoadingScreenFrame));
D2CLIENTSTUB(sub, 6FB6BB88, void*, __fastcall, (void*));
D2CLIENTSTUB(sub, 6FAA9420, void, __fastcall, ());
D2CLIENTSTUB(D2Client_PalshiftInit, 6FAADEA0, void, __fastcall, ());
D2CLIENTSTUB(D2Client_InitAllCaches, 6FAA13A0, void, __fastcall, (void* pMemPoolSystem, int bInitTileCache));
D2CLIENTSTUB(sub, 6FB3F330, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FABA580, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FB28020, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAA46A0, void, __fastcall, ());
D2CLIENTSTUB(InitEnvironment, 6FAA6A10, void, __fastcall, ());
D2CLIENTSTUB(sub, 6FAD2F60, void, __fastcall, ());


//1.10f: D2Client.0x6FB758E0
uint32_t /*D2C_StringIndices*/ aErrorStrIndices[] = {
	STR_IDX_5365_Unable_to_enter_game_Bad_character_versi,
	STR_IDX_5366_Unable_to_enter_game_Bad_character_quest,
	STR_IDX_5367_Unable_to_enter_game_Bad_character_waypo,
	STR_IDX_5368_Unable_to_enter_game_Bad_character_stats,
	STR_IDX_5369_Unable_to_enter_game_Bad_character_skill,
	STR_IDX_5371_Unable_to_enter_game,
	STR_IDX_5370_failed_to_join_game,
	STR_IDX_5357_Your_connection_has_been_interrupted,
	STR_IDX_5358_The_Host_of_this_game_has_left,
	STR_IDX_5372_unknown_failure,
	STR_IDX_5373_Unable_to_enter_game_Bad_inventory_data,
	STR_IDX_5374_Unable_to_enter_game_bad_dead_bodies,
	STR_IDX_5375_Unable_to_enter_game_bad_header,
	STR_IDX_5376_Unable_to_enter_game_bad_hireables,
	STR_IDX_5377_Unable_to_enter_game_bad_intro_data,
	STR_IDX_5378_Unable_to_enter_game_bad_item,
	STR_IDX_5379_Unable_to_enter_game_bad_dead_body_item,
	STR_IDX_5380_Unable_to_enter_game_generic_bad_file,
	STR_IDX_5381_Game_is_full,
	STR_IDX_5360_Versions_do_not_match_Please_log_onto_ba,
	STR_IDX_5364_Unable_to_enter_game_Your_character_must,
	STR_IDX_5363_Unable_to_enter_game_Your_character_must,
	STR_IDX_5362_Unable_to_enter_game_A_normal_character_,
	STR_IDX_5361_Unable_to_enter_game_A_hardcore_characte,
	STR_IDX_5359_A_dead_hardcore_character_cannot_join_or,
	STR_IDX_10101_x_NORMAL_CANT_JOIN_EXP_GAME,
	STR_IDX_10102_x_EXP_CANT_JOIN_NORMAL_GAME,
	STR_IDX_5370_failed_to_join_game,
	STR_IDX_5371_Unable_to_enter_game,
};
#ifdef D2_VERSION_110F
static_assert(D2JOINERR_COUNT == 29 && ARRAY_SIZE(aErrorStrIndices) == D2JOINERR_COUNT, "There are 29 error types in the original game");
#endif//D2_VERSION_110F
//1.10f: D2Client.0x6FBA77EC
D2C_JoinErrors gdwErrorMessageIndex;

#pragma pack(push, 1)
struct ClientGlobalData								//Size:0x80
{
	int field_0;									//0x00
	void(__fastcall* pfDraw)(DWORD);				//0x04
	void* pMemPoolSystem;							//0x08
	int gnFrameStartTickCount_6FBA775C;				//0x0C
	int gnPreviousFrameStartTickCount_6FBA7760;		//0x10
	int field_14;									//0x14
	DWORD gnFramesElapsed_6FBA7768;					//0x18
	int nNextNetUpdateTickCount_6FBA776C;			//0x1C
	int gnLastNetUpdateBeginTickCount_6FBA7770;		//0x20
	int gnPing_6FBA7774;							//0x24
	int gnFPSRendered_6FBA7778;						//0x28
	int gnRenderedFramesSinceLastCheck_6FBA777C;	//0x2C
	int gnFpsSkipped_6FBA7780;						//0x30
	int gnFramesSkippedSinceLastCheck_6FBA7784;		//0x34
	int nUnkFameStartTickCount;						//0x38
	int field_3C;									//0x3C
	BYTE gap40[4];									//0x40
	int field_44;									//0x44
	int field_48;									//0x48
	int field_4C;									//0x4C
	int field_50;									//0x50
	BYTE gap54[16];									//0x54
	int field_64;									//0x64
	BYTE gap68[12];									//0x68
	int gnExpansionFlag_6FBA77C4;					//0x74
	int gnLadderFlag_6FBA77C8;						//0x78
	int field_7C;									//0x7C
};
#ifdef D2_VERSION_110F
static_assert(sizeof(ClientGlobalData) == 0x80, "ClientGlobalData must be of size 0x80");
#endif
#pragma pack(pop)
D2VAR(D2CLIENT, psgtGlobalData, ClientGlobalData, 0x6FBA7750 - D2ClientImageBase);


//1.00 : D2Client.0x10001CA3
//1.10f: D2Client.0x6FB283D0
D2UnitStrc* __fastcall D2CLIENT_GetPlayerUnit_6FB283D0()
{
    return *D2CLIENT_pgpPlayerUnit_6FBBC200;
}
//1.00 : D2Client.0x10002F13
//1.10f: D2Client.0x6FB29370
D2ActiveRoomStrc* __fastcall D2CLIENT_GetCurrentRoom_6FB29370()
{
    if (D2CLIENT_GetPlayerUnit_6FB283D0())
    {
        return UNITS_GetRoom(D2CLIENT_GetPlayerUnit_6FB283D0());
    }
    return nullptr;
}

//1.10f: D2Client.0x6FB09C90
void __fastcall D2CLIENT_InitResolution()
{
	BOOL b800x600 = FALSE;
	SRegLoadValue("Diablo II", "Resolution", 0, (DWORD*)&b800x600);
	D2CLIENT_UI_ChangeResolution(b800x600 ? D2GAMERES_800x600 : D2GAMERES_640x480);
}

int __fastcall sub_6FAADC90()
{
	return *D2CLIENT_pdword_6FBA7BFC;
}

//1.10f: D2Client.0x6FAA9450
void __fastcall D2Client_InitGame()
{
	memset(D2CLIENT_psgtGlobalData, 0, sizeof(*D2CLIENT_psgtGlobalData));
	gnLoopIterationsRemaining = 0;
	*D2CLIENT_pdword_6FBA797C = 0;
	*D2CLIENT_pdword_6FBA7978 = 0;
	*D2CLIENT_pdword_6FBA7958 = 0;
	*D2CLIENT_pdword_6FBA7974 = 0;
	*D2CLIENT_pgbRequestedToStandStill_6FBA79BC = FALSE;
	FOG_CreateNewPoolSystem(&D2CLIENT_psgtGlobalData->pMemPoolSystem, "Client Pool System", 0x40000, 0x1000);
	CLIENT_SetPlayerUnit_6FB28390(nullptr);
	if (!*D2CLIENT_pgbDataLocalColorUseGreenBlood)
	{
		HSFILE hColorTxtFile;
		if (ARCHIVE_OpenFile(0, "Data\\Local\\Color.txt", &hColorTxtFile, TRUE))
		{
			uint8_t bColorFromFile;
			ARCHIVE_ReadFileToBuffer(0, hColorTxtFile, &bColorFromFile, 1);
			ARCHIVE_CloseFile(0, hColorTxtFile);
			if (bColorFromFile == '1')
				*D2CLIENT_pgbDataLocalColorUseGreenBlood = TRUE;
		}
	}
	if (!(*D2CLIENT_pgpView_6FBA7990))
		(*D2CLIENT_pgpView_6FBA7990) = GAMEVIEW_AllocateGameView();
	D2CLIENT_InitResolution();
	UI_LoadingScreenInit_6FB241A0();
	UI_UpdateLoadingScreen_6FB242E0(TRUE);
	if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_OBNET_JOIN
		|| *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_LAN_JOIN
		|| *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_OBNET_HOST
		|| *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_LAN_HOST)
	{
		MSG_Send_D2CLTSYS_OPENCHAR(TRUE);
	}
	sub_6FB6BB88(sub_6FAA9420);
	D2Client_PalshiftInit_6FAADEA0();
	D2CMP_CreateItemPaletteCache();
	D2Client_InitAllCaches_6FAA13A0(D2CLIENT_psgtGlobalData->pMemPoolSystem, TRUE);
	*D2CLIENT_pdword_6FBA7970 = sub_6FAADC90() != 0;
	*D2CLIENT_pdword_6FBA77D0 = GetTickCount();
	*D2CLIENT_pdword_6FBA7948 = GetTickCount();
	D2CLIENT_psgtGlobalData->pfDraw = D2CLIENT_DrawGameScene;
	D2CLIENT_psgtGlobalData->gnFramesElapsed_6FBA7768 = 0;
	D2CLIENT_psgtGlobalData->field_14 = 0;
	sub_6FAB71B0(*D2CLIENT_pgpView_6FBA7990, 0, 0);
	UI_AdjustViewMatrix(0);
	D2CLIENT_psgtGlobalData->field_7C = 0;
	sub_6FB3F330();
	D2CLIENT_Return_6FAA1500();
	sub_6FABA580();
	
	*D2CLIENT_psgpGlobalSeed = D2_ALLOC_STRC(D2SeedStrc);
	sub_6FB28020();
	sub_6FAA46A0();
	InitEnvironment_6FAA6A10();
	sub_6FAD2F60();
	*D2CLIENT_pdword_6FBA79D4 = 0;
	*D2CLIENT_pgbIsInGame_6FBA796C = 0;
}

//1.00 : Inlined
//1.10f: Inlined
//1.13c: D2Client.0x6FAF40C0
void D2CLIENT_PopulateRooms()
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


//1.00 : D2Client.0x100105D0
//1.10f: inlined
//1.13c: D2Client.0x6FAF4A80
// Note: 1.00 Actually has some kind of profiling builtin which could dump timings to a file called `ClientLoopTimes.txt`. See D2Client.0x10007EC0
void D2CLIENT_MainUpdate()
{
	ZoneScoped;
	D2CLIENT_psgtGlobalData->gnFrameStartTickCount_6FBA775C = GetTickCount();

	{
		ZoneScopedN("Populate");
		D2CLIENT_PopulateRooms();
	}
	{
		ZoneScopedN("Process");
		CLIENT_ProcessUnits_6FB280F0();
	}
	{
		ZoneScopedN("Animate");
		DUNGEON_AnimateTiles(D2CLIENT_GetCurrentRoom_6FB29370());
	}
	{
		ZoneScopedN("QuestUpdate");
		CLIENT_QuestUpdate_6FABA270();
	}
	{
		ZoneScopedN("GfxProcess");
		CLIENT_GfxProcess_6FB5B210(1);
	}
    {
		ZoneScopedN("Update env");
        if (ENVIRONMENT_UpdatePeriodOfDay(*D2CLIENT_pgpCurrentAct_6FBA7984, D2CLIENT_GetCurrentRoom_6FB29370()))
			CLIENT_EnvironmentUpdate_6FB29390();
    }
    {
		ZoneScopedN("DungeonUpdate");
        DRLGACTIVATE_Update(DUNGEON_GetDrlgFromAct(*D2CLIENT_pgpCurrentAct_6FBA7984));
    }
	{
		ZoneScopedN("AutomapUpdate");
		CLIENT_UI_AutomapUpdate_6FACBAF0();
	}
    if (!(++D2CLIENT_psgtGlobalData->gnFramesElapsed_6FBA7768 % 13))
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
//1.13c: D2Client.0x6FAF4880
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
//1.13c: D2Client.0x6FAF46B0
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
//1.13c: D2Client.0x6FAF3310
void D2Client_UpdateStandStill()
{
	if (!COMMAND_KeyHasBeenPressed(HOTKEY_STANDSTILL, true))
	{
		if (!COMMAND_KeyHasBeenPressed(HOTKEY_STANDSTILL, false))
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
//1.13c: D2Client.0x6FAF23A0
void D2Client_ComputeFPS()
{
	if ((D2CLIENT_psgtGlobalData->gnFrameStartTickCount_6FBA775C - D2CLIENT_psgtGlobalData->nUnkFameStartTickCount) > (3 * 1000))
	{
		D2CLIENT_psgtGlobalData->gnFPSRendered_6FBA7778 = D2CLIENT_psgtGlobalData->gnRenderedFramesSinceLastCheck_6FBA777C / 3;
		D2CLIENT_psgtGlobalData->gnFpsSkipped_6FBA7780 = D2CLIENT_psgtGlobalData->gnFramesSkippedSinceLastCheck_6FBA7784 / 3;
		D2CLIENT_psgtGlobalData->nUnkFameStartTickCount = D2CLIENT_psgtGlobalData->gnFrameStartTickCount_6FBA775C;
		D2CLIENT_psgtGlobalData->gnRenderedFramesSinceLastCheck_6FBA777C = 0;
		D2CLIENT_psgtGlobalData->gnFramesSkippedSinceLastCheck_6FBA7784 = 0;
	}

}

//1.10f: D2Client.0x6FAA9850
//1.13c: D2Client.0x6FAF35D0
void D2Client_MoveToErrorScreen(D2C_JoinErrors nError)
{
	if (!gnLoopIterationsRemaining && !*D2CLIENT_pdword_6FBA7954 && !*D2CLIENT_pdword_6FBA797C)
	{
		if (nError >= D2JOINERR_COUNT) gdwErrorMessageIndex = D2JOINERR_unknown_failure;
		else gdwErrorMessageIndex = nError;

		CLIENT_UpdateUIs_6FB23800(1, 0);
		gnLoopIterationsRemaining = 600;
		*D2CLIENT_pdword_6FBA7970 = 1;
		*D2CLIENT_pgbIsInGame_6FBA796C = 0;
		*D2CLIENT_pdword_6FBA7968 = 0;
		sub_6FAA2040(0);
	}
}

//1.10f: D2Client.0x6FAA98C0
//1.13c: D2Client.0x6FAF3A40
signed int __stdcall DisplayErrorLoop(int nIteration)
{
	if (!gnLoopIterationsRemaining)
		return FALSE;
	if (!WINDOW_GetState() && D2GFX_StartDraw(1, 0, 0, 0))
	{
		D2GFX_ClearScreen(0);
		D2Win_10127_SetFont(D2FONT_FONT42);
		const Unicode* szErrorString = D2LANG_GetStringFromTblIndex(aErrorStrIndices[gdwErrorMessageIndex]);
		if ((CONFIG_GetConfig()->unpackedCTemp.nCharacterSaveFlags & CLIENTSAVEFLAG_EXPANSION) != 0)
		{
			if (gdwErrorMessageIndex == D2JOINERR_NightmareNotUnlocked)
			{
				szErrorString = D2LANG_GetStringFromTblIndex(STR_IDX_21794_x);
			}
			else if (gdwErrorMessageIndex == D2JOINERR_HellNotUnlocked)
			{
				szErrorString = D2LANG_GetStringFromTblIndex(STR_IDX_21793_x);
			}
		}

		int nTextWidth = D2Win_10121_GetTextWidth(szErrorString);
		if (nTextWidth >= (*D2CLIENT_pgnDrawMaxWidth) - 40)
		{
			D2Win_10127_SetFont(D2FONT_FONT30);
			int nLines;
			D2SplittedTextStrc* pSplittedText = D2Win_10199(szErrorString, &nLines, 600);
			D2SplittedTextStrc* pCurrentTextLine = pSplittedText;
			for (int nLineIdx = 0; nLineIdx < nLines; ++nLineIdx)
			{
				int nTextX = ((*D2CLIENT_pgnDrawMaxWidth) - D2Win_10121_GetTextWidth(pCurrentTextLine->wszLine)) / 2;
				D2Win_10117_DrawText(pCurrentTextLine->wszLine, nTextX, 2 * nLineIdx * D2Win_10125() + 200, 0, 0);
				pCurrentTextLine = pCurrentTextLine->pNextLine;
			}
			D2Win_10200(pSplittedText);
		}
		else
		{
			D2Win_10117_DrawText(szErrorString, ((*D2CLIENT_pgnDrawMaxWidth) - nTextWidth) / 2, 200, 0, 0);
		}
		D2Win_10019();
		D2GFX_EndDraw();
	}
	gnLoopIterationsRemaining--;
	if (gnLoopIterationsRemaining == 0)
	{
		UI_FreeLoadingScreenProgress_6FB245C0();
		return TRUE;
	}
	return FALSE;
}

//1.10f:Inlined
//1.13c: D2Client.0x6FAF35D0
void D2Client_MultiUpdateUIsIfNeeded()
{
	if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER || *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER_UNCAPPED)
		return;

	const DWORD nTickCount = GetTickCount();
	if (!D2NET_10017() && !*D2CLIENT_pdword_6FBA7970 && *D2CLIENT_pgbIsInGame_6FBA796C)
	{
		D2Client_MoveToErrorScreen(D2JOINERR_Your_connection_has_been_interrupted);
	}
	if (*D2CLIENT_pdword_6FBA7974 && (nTickCount - *D2CLIENT_pdword_6FBA77D0 > 180000))
	{
		D2Client_MoveToErrorScreen(D2JOINERR_Your_connection_has_been_interrupted);
	}
	if ((nTickCount - *D2CLIENT_pdword_6FBA7948) > 30000 && !*D2CLIENT_pgbIsInGame_6FBA796C && !*D2CLIENT_pdword_6FBA7970 && !*D2CLIENT_pdword_6FBA7974)
	{
		D2Client_MoveToErrorScreen(D2JOINERR_failed_to_join_game_6);
	}
	if (!*D2CLIENT_pdword_6FBA7968
		&& !*D2CLIENT_pgbIsInGame_6FBA796C
		&& !*D2CLIENT_pdword_6FBA7970
		&& !*D2CLIENT_pdword_6FBA7974
		&& (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_LAN_JOIN || *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_OBNET_JOIN)
		)
	{
		D2Client_MoveToErrorScreen(D2JOINERR_failed_to_join_game_6);
	}
}

//1.00 :Inlined
//1.10f:Inlined
//1.13c: D2Client.0x6FAF2400
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

//1.00 : D2Client.0x10001B7C (thunk) D2Client.0x1000FC40 (impl)
//1.10f: D2Client.0x6FAA9AF0
//1.13c: D2Client.0x6FAF4B50
signed int __stdcall ClientGameLoop(int nIteration)
{
    BOOL bIsNewFrame = FALSE;
    BOOL bDidClientUpdate = FALSE;
	D2UnitStrc* pPlayerUnit = D2CLIENT_GetPlayerUnit_6FB283D0();
	
	ScopedDiscontinuousFrameMarker scopedDiscontinousMarker;

    if (*D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER || *D2CLIENT_pgnGameType_6FBA7960 == GAMETYPE_SINGLEPLAYER_UNCAPPED)
    {
        if (WINDOW_GetState())
        {
			D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760 = GetTickCount();
            return 0;
        }
        if ((D2CLIENT_CheckUIState_6FB23860(9) || D2CLIENT_CheckUIState_6FB23860(11))
            && pPlayerUnit
            && D2CLIENT_GetCurrentRoom_6FB29370())
        {
			D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760 = GetTickCount();
			D2CLIENT_psgtGlobalData->pfDraw(0);
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
		if (D2CLIENT_psgtGlobalData->nNextNetUpdateTickCount_6FBA776C < nBeforeNetworkUpdateTickCount)
		{
			NetUpdate_6FAAD3B0();
			D2CLIENT_psgtGlobalData->gnLastNetUpdateBeginTickCount_6FBA7770 = nBeforeNetworkUpdateTickCount;
			D2CLIENT_psgtGlobalData->nNextNetUpdateTickCount_6FBA776C = nBeforeNetworkUpdateTickCount + 5000;
		}
	}

    const DWORD nFrameStartTickCount = GetTickCount();
    D2CLIENT_psgtGlobalData->gnFrameStartTickCount_6FBA775C = nFrameStartTickCount;
    
	if (!D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760) // Init
    {
		D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760 = nFrameStartTickCount - gnTargetFrameDurationMs;
    }

	D2Client_ComputeFPS();

	unsigned int nTimeSincePreviousFrameStart = nTimeSincePreviousFrameStart = nFrameStartTickCount - D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760;
    if ((nFrameStartTickCount - D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760) >= gnTargetFrameDurationMs)
    {
		D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760 = nFrameStartTickCount;
        bIsNewFrame = TRUE;
        if (nFrameStartTickCount < D2CLIENT_psgtGlobalData->field_3C)
        {
            int v18 = nTimeSincePreviousFrameStart - gnTargetFrameDurationMs;
            int v19 = gnTargetFrameDurationMs - 1;
            if (v18 < gnTargetFrameDurationMs - 1)
                v19 = v18;
            *D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 = 0;
            nTimeSincePreviousFrameStart = gnTargetFrameDurationMs;
			D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760 = nFrameStartTickCount - v19;
            D2CLIENT_psgtGlobalData->nUnkFameStartTickCount = nFrameStartTickCount;
            D2CLIENT_psgtGlobalData->gnFPSRendered_6FBA7778 = 25;
            D2CLIENT_psgtGlobalData->gnFpsSkipped_6FBA7780 = 0;
            D2CLIENT_psgtGlobalData->gnRenderedFramesSinceLastCheck_6FBA777C = 0;
            D2CLIENT_psgtGlobalData->gnFramesSkippedSinceLastCheck_6FBA7784 = 0;
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
						D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760 -= (gnTargetFrameDurationMs - 1);
					}
					else
					{
						*D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 = 0;
						D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760 -= (nTimeSincePreviousFrameStart - gnTargetFrameDurationMs);
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

				D2CLIENT_psgtGlobalData->gnPreviousFrameStartTickCount_6FBA7760 += gnTargetFrameDurationMs - nTimeSincePreviousFrameStart;
				int nFramesToSkip = D2Client_FramesToSkipOrSomething(nTimeSincePreviousFrameStart / gnTargetFrameDurationMs, D2CLIENT_psgtGlobalData->gnFPSRendered_6FBA7778);
				if (D2CLIENT_psgtGlobalData->gnFPSRendered_6FBA7778 > 28)
					nFramesToSkip = 0;
				*D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 = *D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 <= nFramesToSkip;
			}
			else
			{
				*D2CLIENT_pgnFramesSinceLastUpdate_6FBA79E0 = D2CLIENT_psgtGlobalData->gnFPSRendered_6FBA7778 <= 22;
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
					++D2CLIENT_psgtGlobalData->gnFramesSkippedSinceLastCheck_6FBA7784;
				}
			}
			else if (D2CLIENT_GetCurrentRoom_6FB29370())
			{
				ZoneScopedN("Draw");
				D2CLIENT_psgtGlobalData->pfDraw(0);
				sgbNewFrameMarkerNeeded = true;

				D2CLIENT_psgtGlobalData->field_14++;
				D2CLIENT_psgtGlobalData->gnRenderedFramesSinceLastCheck_6FBA777C++;
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

//1.10f: D2Client.0x6FAA9640
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
		if (D2CLIENT_GetGlobalUiStateNbPanels() != 3)
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
		ZoneScopedN("D2GFX_EndDraw");
		D2GFX_EndDraw();
	}
}

//1.10f: D2Client.0x6FAA1F80
BnClientInterface* __cdecl GetConfigComInt_6FAA1F80()
{
	return CONFIG_GetConfig()->pComInterface;
}

//1.10f:Inlined
//1.13c:Inlined
int GetGameTypeFromConfig()
{
	const D2ConfigStrc* pCfg = CONFIG_GetConfig();
	int nGameType = 0;
	if (pCfg->nArenaDifficulty == 1)
		nGameType = 1;
	if (pCfg->nArenaDifficulty == 2)
		nGameType = nGameType | 2;
	uint16_t v9 = HIWORD(pCfg->dwCTemp);
	if ((v9 & 4) != 0)
		nGameType = nGameType | 4;
	if ((v9 & 0x20) != 0)
		nGameType = nGameType | 8;
	return nGameType;
}

//1.10f: D2Client.0x6FAAA3B0
//1.13c: D2Client.0x6FAF31B0
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
							D2ConfigStrc* pCfg = CONFIG_GetConfig();
							pBnClient->RefreshGame(
								pCfg->szGameName,
								pCfg->szGamePassword,
								pCfg->szGameStatstring,
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

//1.10f: D2Client.0x6FAA90E0
void __stdcall WNDPROC_BreakHandlerChainCallback(SMSGHANDLER_PARAMS* pMsg)
{
	pMsg->bUseResult = TRUE;
	pMsg->lResult = 0;
	SMsgBreakHandlerChain(pMsg);
}

//1.10f: 0x6FB75818
//1.13c: 0x6FBA46D0
D2WindowProcCallbackStrc gatGlobalWindowCallbacks_6FB75818[] = {
	{ D2_WINPROC_MESSAGE, WM_LBUTTONDOWN,	sub_6FAA8C10},
	{ D2_WINPROC_MESSAGE, WM_LBUTTONUP,		sub_6FAA8CC0},
	{ D2_WINPROC_MESSAGE, WM_RBUTTONDOWN,	sub_6FAA8D50},
	{ D2_WINPROC_MESSAGE, WM_RBUTTONUP,		sub_6FAA8E90},
	{ D2_WINPROC_KEYDOWN, VK_LWIN,			WNDPROC_BreakHandlerChainCallback},
	{ D2_WINPROC_KEYDOWN, VK_RWIN,			WNDPROC_BreakHandlerChainCallback},
	{ D2_WINPROC_KEYDOWN, VK_APPS,			WNDPROC_BreakHandlerChainCallback},
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
//1.10f: D2Client.0x6FB75920
D2WindowProcCallbackStrc aUnkCallbacks_6FB75920[] = {
	{ D2_WINPROC_MESSAGE, WM_LBUTTONDOWN,	sub_6FAAA500 },
	{ D2_WINPROC_MESSAGE, WM_RBUTTONDOWN,	sub_6FAAA500 },
	{ D2_WINPROC_MESSAGE, WM_KEYFIRST,		sub_6FAAA530 },
	{ D2_WINPROC_MESSAGE, WM_ACTIVATEAPP,	sub_6FAA9100 },
	{ D2_WINPROC_KEYDOWN, VK_LWIN,			WNDPROC_BreakHandlerChainCallback },
	{ D2_WINPROC_KEYDOWN, VK_RWIN,			WNDPROC_BreakHandlerChainCallback },
	{ D2_WINPROC_KEYDOWN, WM_ACTIVATEAPP,	WNDPROC_BreakHandlerChainCallback },
};

//1.10f: D2Client.0x6FB75920
D2WindowProcCallbackStrc aUnkCallbacks_6FB75978[] = {
	{ D2_WINPROC_MESSAGE, WM_LBUTTONDOWN, sub_6FAAA7D0 },
	{ D2_WINPROC_MESSAGE, WM_RBUTTONDOWN, sub_6FAAA7D0 },
	{ D2_WINPROC_MESSAGE, WM_LBUTTONUP, sub_6FAAA880 },
	{ D2_WINPROC_MESSAGE, WM_RBUTTONUP, sub_6FAAA880 },
	{ D2_WINPROC_MESSAGE, WM_KEYFIRST, sub_6FAAA920 },
	{ D2_WINPROC_MESSAGE, WM_ACTIVATEAPP, sub_6FAA9100 },
	{ D2_WINPROC_KEYDOWN, VK_LWIN, WNDPROC_BreakHandlerChainCallback },
	{ D2_WINPROC_KEYDOWN, VK_RWIN, WNDPROC_BreakHandlerChainCallback },
	{ D2_WINPROC_KEYDOWN, WM_ACTIVATEAPP, WNDPROC_BreakHandlerChainCallback },
};


//1.10f: D2Client.0x6FBA7998
//1.13c: D2Client.0x6FBCC3CC
D2VAR(D2CLIENT, pgbHasOsVersion, BOOL, 0x6FBA7998 - D2ClientImageBase);
//1.10f: D2Client.0x6FBA78B0
//1.13c: D2Client.0x6FBC9888
D2VAR(D2CLIENT, pgtVersionInformation, _OSVERSIONINFOA, 0x6FBA78B0 - D2ClientImageBase);

//1.10c:D2Client.0x6FB758D0
D2CLIENTDWORDSTUB(6FB758D0)

void D2CLIENT_InitOSVersion()
{
	D2CLIENT_pgtVersionInformation->dwOSVersionInfoSize = sizeof(_OSVERSIONINFOA);
	if (GetVersionExA(D2CLIENT_pgtVersionInformation))
		*D2CLIENT_pgbHasOsVersion = 1;
}

//1.10f: D2Client.0x6FAAB250
BOOL __fastcall sub_6FAAB250()
{
	return GetSystemMetrics(75)
		&& *D2CLIENT_pgbHasOsVersion
		&& D2CLIENT_pgtVersionInformation->dwPlatformId == 1
		&& D2CLIENT_pgtVersionInformation->dwMinorVersion == 0;
}
//1.10f: D2Client.0x6FAAB290
BOOL __fastcall sub_6FAAB290()
{
	_OSVERSIONINFOA& verInfo = *D2CLIENT_pgtVersionInformation;
	return GetSystemMetrics(75)
		&& *D2CLIENT_pgbHasOsVersion
		&& (
			(verInfo.dwPlatformId == 1 && verInfo.dwMinorVersion != 0)
			|| (verInfo.dwPlatformId == 2 && (verInfo.dwMajorVersion == 4 || verInfo.dwMajorVersion == 5))
			);
}

//1.10f: D2Client.0x6FAAB2E0
BOOL __fastcall sub_6FAAB2E0()
{
	if (!*D2CLIENT_pgbHasOsVersion) return FALSE;
	_OSVERSIONINFOA& verInfo = *D2CLIENT_pgtVersionInformation;
	return verInfo.dwPlatformId == 1
		|| verInfo.dwPlatformId == 2 && (verInfo.dwMajorVersion == 4 || verInfo.dwMajorVersion == 5);
}

//1.10f: D2Client.0x6FAAB320
BOOL __fastcall sub_6FAAB320()
{
	return *D2CLIENT_pgbHasOsVersion
		&& D2CLIENT_pgtVersionInformation->dwPlatformId != 1
		&& D2CLIENT_pgtVersionInformation->dwPlatformId == 2
		&& D2CLIENT_pgtVersionInformation->dwMajorVersion != 4
		&& D2CLIENT_pgtVersionInformation->dwMajorVersion == 5;
}


//1.10f: D2Client.0x6FB75570
char gszBattleNetIP[D2_MAX_PATH] = "207.82.87.139";

//1.10f: D2Client.0x6FB75468
char gszServerIP[D2_MAX_PATH] = "207.82.87.243";

//1.10f: D2Client.0x6FAABBF0
void __fastcall CONFIG_ApplyGameInformation(D2ConfigStrc* pConfig)
{
	if (strcmp(pConfig->szServerIP, "0"))
		lstrcpyA(gszServerIP, pConfig->szServerIP);
	lstrcpyA(D2CLIENT_pgszGameName, pConfig->szGameName);
	*D2CLIENT_pgnGameType_6FBA7960 = eD2GameTypes(pConfig->dwGameType);
	*D2CLIENT_pgwJoinId = pConfig->wJoinID;
	*D2CLIENT_pgnTokenId = pConfig->nTokenId;
}

//1.10f: D2Client.0x6FAABC50
void __fastcall CONFIG_Apply_BattleNetIP(D2ConfigStrc* pConfig)
{
	if (strcmp(pConfig->szBattleNetIP, "0"))
		lstrcpyA(gszBattleNetIP, pConfig->szServerIP);
}

using ConfigSetupFunction = BOOL(__fastcall*)(D2ConfigStrc* pConfig);
D2VAR(D2CLIENT, paConfigSetupFunctions, ConfigSetupFunction, 0x6FB759E8 - D2ClientImageBase);
//1.10f: D2Client.0x6FAABEE0
BOOL __fastcall ApplyConfiguration(D2ConfigStrc* pConfig)
{
	*D2CLIENT_pgbAppliedConfiguration = TRUE;
	for (int i = 0; i < 48; ++i)
	{
		if (const auto setupFunc = D2CLIENT_paConfigSetupFunctions[i])
			setupFunc(pConfig);
	}
	return *D2CLIENT_pgbAppliedConfiguration;
}

//1.10f:Inlined
void __fastcall WNDPROC_RegisterGlobalCallbacks()
{
	D2CLIENT_INPUT_RegisterCallbacks(WINDOW_GetWindow(), gatGlobalWindowCallbacks_6FB75818, ARRAY_SIZE(gatGlobalWindowCallbacks_6FB75818));
}
//1.10f:Inlined
void __fastcall WNDPROC_UnregisterGlobalCallbacks()
{
	D2CLIENT_INPUT_UnregisterCallbacks(WINDOW_GetWindow(), gatGlobalWindowCallbacks_6FB75818, ARRAY_SIZE(gatGlobalWindowCallbacks_6FB75818));
}

//1.10f: D2Client.0x6FAAC1B0
//1.13c: D2Client.0x6FAF2880
void __fastcall WNDPROC_RegisterMouseWheelCallbacks()
{
	if (sub_6FAAB290())
	{
		D2CLIENT_INPUT_RegisterCallbacks(WINDOW_GetWindow(), &stru_6FB75890, 1u);
	}
	if (sub_6FAAB2E0())
	{
		D2CLIENT_INPUT_RegisterCallbacks(WINDOW_GetWindow(), stru_6FB7589C, ARRAY_SIZE(stru_6FB7589C));
	}
	if (sub_6FAAB320())
	{
		D2CLIENT_INPUT_RegisterCallbacks(WINDOW_GetWindow(), stru_6FB758B4, ARRAY_SIZE(stru_6FB758B4));
	}
	if (sub_6FAAB250())
	{
		UINT v3 = RegisterWindowMessageA("MSH_WHEELSUPPORT_MSG");
		if (HWND WindowA = FindWindowA("MouseZ", "Magellan MSWHEEL"))
		{
			if (v3 && SendMessageA(WindowA, v3, 0, 0))
			{
				*D2CLIENT_pdword_6FB758D0 = v3;
				D2CLIENT_INPUT_RegisterCallbacks(WINDOW_GetWindow(), &stru_6FB758CC, 1u);
			}
		}
	}
}

//1.10f: D2Client.0x6FAAC0A0 and Inlined
//1.13c: D2Client.0x6FAF29C0
void __fastcall WNDPROC_UnregisterMouseWheelCallbacks()
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

//1.00 : D2Client.0x100F67C4
//1.10f: D2Client.0x6FB7545C
//Note: Unused except in APPMODE_Startup. Leftover of the -mode commandline ?
static D2C_AppMode gnAppModeAfterStartup = APPMODE_JOIN_GAME;

//1.00 : D2Client.0x10001F7D (thunk) D2Client.0x10010B90 (impl)
//1.10f: D2Client.0x6FAAA4E0
D2C_AppMode __fastcall APPMODE_Startup()
{
	if (gnAppModeAfterStartup == APPMODE_JOIN_GAME)
	{
		APPMODE_Enqueue(APPMODE_GAME);
	}
	return gnAppModeAfterStartup;
}

//1.00 : D2Client.0x1000219E (thunk) D2Client.0x10012360 (impl)
//1.10f: D2Client.0x6FAABB40
D2C_AppMode __fastcall APPMODE_CloseWindow()
{
	D2Client_FreeGouraudTables_6FAB6EB0();
	CLIENT_UnloadCursors();
	sub_6FAAE910();
	sub_6FAAF4E0();
	sub_6FAB14C0();
	sub_6FAA2850();
	return APPMODE_SHOULDQUIT;
}

//1.10f: D2Client.0x6FAADCA0
BOOL __stdcall CLIENT_NetworkConnectionWaitMessageLoop(int a1)
{
	if ((GAME_GetGameType() == GAMETYPE_SINGLEPLAYER || GAME_GetGameType() == GAMETYPE_SINGLEPLAYER_UNCAPPED)
		&& WINDOW_GetState())
	{
		return FALSE;
	}
	int32_t v1 = D2NET_10025();
	if (v1)
	{
		return v1 == 2;
	}
	else
	{
		D2Client_MoveToErrorScreen(D2JOINERR_failed_to_join_game_6);
		*D2CLIENT_pdword_6FBA7BFC = 1;
		*D2CLIENT_pdword_6FBA7968 = 0;
		return TRUE;
	}
}

//1.10f: D2Client.0x6FBA7C00
DWORD gnGameJoinStartTickCount;

//1.10f: D2Client.0x6FAADBC0
BOOL __stdcall CLIENT_JoinGameMessageLoop(int a1)
{
	if (! ((GAME_GetGameType() == GAMETYPE_SINGLEPLAYER || GAME_GetGameType() == GAMETYPE_SINGLEPLAYER_UNCAPPED) && WINDOW_GetState()) )
	{
		if (GetTickCount() - gnGameJoinStartTickCount > 10000)
		{
			D2Client_MoveToErrorScreen(D2JOINERR_failed_to_join_game_6);
			*D2CLIENT_pdword_6FBA7BFC = 1;
			*D2CLIENT_pdword_6FBA7968 = 0;
			return TRUE;
		}
		if (!D2NET_10017())
			return TRUE;
	}
	uint8_t aPacketBuffer[512];
	int32_t nReceivedPacketsSize = CLIENT_DequeueSystemPacket(aPacketBuffer, ARRAY_SIZE(aPacketBuffer));
	D2Client_PACKETS_SendPlayerUpdates_6FAB1CB0(aPacketBuffer, nReceivedPacketsSize);
	return *D2CLIENT_pdword_6FBA7968 != 0;
}

//1.00 : D2Client.0x10015F60 (thunk) D2Client.0x10015F60 (impl)
//1.10f: D2Client.0x6FAADD00
D2C_AppMode __fastcall APPMODE_JoinGame()
{
	switch (GAME_GetGameType())
	{
	case GAMETYPE_SINGLEPLAYER:
		SERVER_Initialize(1, 1);
		CLIENT_Initialize(1, nullptr);
		ExecuteMessageLoop_6FAA25D0(CLIENT_NetworkConnectionWaitMessageLoop);
		SERVER_SetHackListEnabled(0);
		CLIENT_InitGame();
		break;
	case GAMETYPE_SINGLEPLAYER_UNCAPPED:
		SERVER_Initialize(2, 1);
		CLIENT_Initialize(2, nullptr);
		ExecuteMessageLoop_6FAA25D0(CLIENT_NetworkConnectionWaitMessageLoop);
		SERVER_SetHackListEnabled(0);
		CLIENT_InitGame();
		break;
	case GAMETYPE_BNET_INTERNAL:
		CLIENT_Initialize(0, gszBattleNetIP);
		ExecuteMessageLoop_6FAA25D0(CLIENT_NetworkConnectionWaitMessageLoop);
		break;
	case GAMETYPE_OBNET_HOST: // FALLTHROUGH
	case GAMETYPE_LAN_HOST:
		SERVER_Initialize(0, 1);
		SERVER_SetMaxClientsPerGame(8);
		SERVER_SetHackListEnabled(0);
		CLIENT_Initialize(1, gszServerIP);
		ExecuteMessageLoop_6FAA25D0(CLIENT_NetworkConnectionWaitMessageLoop);
		CLIENT_InitGame();
		break;

	case GAMETYPE_BNET_BETA:
	case GAMETYPE_BNET:
	case GAMETYPE_BNET_UNUSED:
	case GAMETYPE_OBNET_JOIN:
	case GAMETYPE_LAN_JOIN:
	default:
		CLIENT_Initialize(0, gszServerIP);
		ExecuteMessageLoop_6FAA25D0(CLIENT_NetworkConnectionWaitMessageLoop);
		break;
	}

	gnGameJoinStartTickCount = GetTickCount();
	*D2CLIENT_pdword_6FBA7BFC = 0;
	ExecuteMessageLoop_6FAA25D0(CLIENT_JoinGameMessageLoop);
	return APPMODE_Dequeue();
}

//1.10f: D2Client.0x6FBA7C04
DWORD gnGameLeaveStartTickCount;

//1.10f: D2Client.0x6FAADDF0
BOOL __stdcall CLIENT_LeaveGameMessageLoop(int a1)
{
	if (GAME_GetGameType() == GAMETYPE_SINGLEPLAYER || GAME_GetGameType() == GAMETYPE_SINGLEPLAYER_UNCAPPED)
		GAME_ProcessNetworkMessages();	

	uint8_t aPacketBuffer[512];
	int32_t nReceivedPacketsSize = CLIENT_DequeueSystemPacket(aPacketBuffer, ARRAY_SIZE(aPacketBuffer));
	D2Client_PACKETS_SendPlayerUpdates_6FAB1CB0(aPacketBuffer, nReceivedPacketsSize);

	return (GetTickCount() - gnGameLeaveStartTickCount) > 10000 || *D2CLIENT_pdword_6FBA7968 == 0;
}

//1.00 : D2Client.0x10002EEB (thunk) D2Client.0x10016120 (impl)
//1.10f: D2Client.0x6FAADE60
D2C_AppMode __fastcall APPMODE_LeaveGame()
{
	gnGameLeaveStartTickCount = GetTickCount();
	ExecuteMessageLoop_6FAA25D0(CLIENT_LeaveGameMessageLoop);
	CLIENT_Release();
	if (GAME_GetGameType() == GAMETYPE_SINGLEPLAYER
		|| GAME_GetGameType() == GAMETYPE_SINGLEPLAYER_UNCAPPED
		|| GAME_GetGameType() == GAMETYPE_OBNET_HOST
		|| GAME_GetGameType() == GAMETYPE_LAN_HOST)
	{
		SERVER_Release();
	}
	return APPMODE_Dequeue();
}

//
//1.10f: D2Client.0x6FAAB370
//1.13c: D2Client.0x6FAF4F40
D2C_AppMode __fastcall APPMODE_Game()
{
	BOOL(__stdcall * v0)(LPMSG, HWND, UINT, UINT, UINT); // esi
	struct tagMSG Msg; // [esp+14h] [ebp-1Ch] BYREF

	if (!gnLoopIterationsRemaining)
		sub_6FAA2040(1);
	DWORD v1 = GetTickCount();
	FOG_Trace("[D2CLIENT]  Start entering at %d\n", v1);

	D2CLIENT_InitOSVersion();

	D2Client_InitGouraudTables_6FAB6AC0();
	sub_6FAA1CB0();
	sub_6FAA2840();
	CLIENT_LoadCursors(0);
	WNDPROC_RegisterGlobalCallbacks();
	WNDPROC_RegisterMouseWheelCallbacks();
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
	ExecuteMessageLoop_6FAA25D0(ClientGameLoop);// This is the main loop
	DWORD nBeginTickCount = GetTickCount();
	FOG_Trace("[D2CLIENT]  Start closing at %d.\n", nBeginTickCount);
	D2Sound_10029(180);
	sub_6FB54CB0();
	if (gnLoopIterationsRemaining && !*D2CLIENT_pdword_6FBA7954 && !*D2CLIENT_pdword_6FBA797C)
		ExecuteMessageLoop_6FAA25D0(DisplayErrorLoop);
	if (sub_6FAAEAA0())
		sub_6FAAE810();
	if (*D2CLIENT_pdword_6FBA7950)
	{
		DWORD v4 = GetTickCount();
		FOG_Trace("slide show start %d\n", v4);
		WNDPROC_UnregisterGlobalCallbacks();
		WNDPROC_UnregisterMouseWheelCallbacks();
		D2CLIENT_INPUT_RegisterCallbacks(WINDOW_GetWindow(), aUnkCallbacks_6FB75920, ARRAY_SIZE(aUnkCallbacks_6FB75920));
		ExecuteMessageLoop_6FAA25D0(Slideshow_6FAAA560);
	}
	else if (*D2CLIENT_pdword_6FBA7954)
	{
		PlayVideo_6FAA1680(5);
		WNDPROC_UnregisterGlobalCallbacks();
		WNDPROC_UnregisterMouseWheelCallbacks();
		D2CLIENT_UI_ChangeResolution(D2GAMERES_640x480);
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
		int v18 = HIBYTE(CONFIG_GetConfig()->dwCTemp) & 0x1F;
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
		CONFIG_GetConfig()->unpackedCTemp.nCharacterSaveFlags =
			(CONFIG_GetConfig()->unpackedCTemp.nCharacterSaveFlags & 0xE0FF)
			| (((*D2CLIENT_pgnDifficulty_6FBA795C) << 10)
			+ 1280);
		if (v17) {
			WNDPROC_UnregisterGlobalCallbacks();
			WNDPROC_UnregisterMouseWheelCallbacks();
			D2CLIENT_UI_ChangeResolution(D2GAMERES_800x600);
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
	FOG_DestroyMemoryPoolSystem(D2CLIENT_psgtGlobalData->pMemPoolSystem);
	memset(D2CLIENT_psgtGlobalData, 0, sizeof(ClientGlobalData));
	D2_FREE(*D2CLIENT_psgpGlobalSeed);
	DWORD nGameCloseEndTickCount = GetTickCount();
	FOG_Trace("game close end %d\n", nGameCloseEndTickCount);
	D2Client_ShutdownSound_6FB54860();
	D2Client_FreeGouraudTables_6FAB6EB0();
	CLIENT_UnloadCursors();
	D2Win_10006_ClearDrawCaches();
	APPMODE_Enqueue(APPMODE_CLOSEWINDOW);
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
		WNDPROC_UnregisterGlobalCallbacks();
		WNDPROC_UnregisterMouseWheelCallbacks();
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
	return APPMODE_LEAVE_GAME;
}

#endif //D2_VERSION_110F
