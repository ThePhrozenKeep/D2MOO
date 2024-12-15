#pragma once  

#include <D2BasicTypes.h>
#include <D2Config.h>
#include <Units/Units.h>
#include <Core/WINMAIN.h>

#pragma pack(push, 1)
enum eD2GameTypes : uint32_t
{
	GAMETYPE_SINGLEPLAYER = 0x0, // Original game uses GT_SINGLE_PLAYER. See 1.00 D2Client.0x1009E930
	GAMETYPE_SINGLEPLAYER_UNCAPPED = 0x1,
	GAMETYPE_BNET_BETA = 0x2,
	GAMETYPE_BNET = 0x3,
	GAMETYPE_BNET_INTERNAL = 0x4,
	GAMETYPE_BNET_UNUSED = 0x5,
	GAMETYPE_OBNET_HOST = 0x6,
	GAMETYPE_OBNET_JOIN = 0x7,
	GAMETYPE_LAN_HOST = 0x8,
	GAMETYPE_LAN_JOIN = 0x9,
	GAMETYPE_COUNT = 0xA,
};

enum D2C_JoinErrors {
	D2JOINERR_Bad_character_version,
	D2JOINERR_Bad_character_quest,
	D2JOINERR_Bad_character_waypoint,
	D2JOINERR_Bad_character_stats,
	D2JOINERR_Bad_character_skill,
	D2JOINERR_Unable_to_enter_game_generic,
	D2JOINERR_failed_to_join_game_6,
	D2JOINERR_Your_connection_has_been_interrupted,
	D2JOINERR_The_Host_of_this_game_has_left,
	D2JOINERR_unknown_failure,
	D2JOINERR_Bad_inventory_data,
	D2JOINERR_bad_dead_bodies,
	D2JOINERR_bad_header,
	D2JOINERR_bad_hireables,
	D2JOINERR_bad_intro_data,
	D2JOINERR_bad_item,
	D2JOINERR_bad_dead_body_item,
	D2JOINERR_generic_bad_file,
	D2JOINERR_Game_is_full,
	D2JOINERR_Versions_do_not_match,
	D2JOINERR_NightmareNotUnlocked,
	D2JOINERR_HellNotUnlocked,
	D2JOINERR_A_normal_character_cannot_join_hardcore,
	D2JOINERR_A_hardcore_character_cannot_join_normal,
	D2JOINERR_dead_hardcore_character,
	D2JOINERR_NORMAL_CANT_JOIN_EXP_GAME,
	D2JOINERR_EXP_CANT_JOIN_NORMAL_GAME,
	D2JOINERR_failed_to_join_game_27,
	D2JOINERR_Unable_to_enter_game,
	D2JOINERR_COUNT
};

#pragma pack(pop)

//1.10f: D2Client.0x6FAA9450
void __fastcall D2Client_InitGame();

//1.10f: D2Client.0x6FAA9640
void __fastcall D2CLIENT_DrawGameScene(DWORD a1);

//1.00 : D2Client.0x10001B7C
//1.10f: D2Client.0x6FAA9AF0
//1.13c: D2Client.0x6FAF4B50
signed int __stdcall ClientGameLoop(int nIteration);

//1.10f: D2Client.0x6FAABEE0
BOOL __fastcall ApplyConfiguration(D2ConfigStrc* pConfig);

//1.00 : D2Client.0x10001F7D (thunk) D2Client.0x10010B90 (impl)
//1.10f: D2Client.0x6FAAA4E0
D2C_AppMode __fastcall APPMODE_Startup();

//1.00 : D2Client.0x1000219E (thunk) D2Client.0x10012360 (impl)
//1.10f: D2Client.0x6FAABB40
D2C_AppMode __fastcall APPMODE_CloseWindow();

//1.00 : D2Client.0x10015F60 (thunk) D2Client.0x10015F60 (impl)
//1.10f: D2Client.0x6FAADD00
D2C_AppMode __fastcall APPMODE_JoinGame();

//1.00 : D2Client.0x10002EEB (thunk) D2Client.0x10016120 (impl)
//1.10f: D2Client.0x6FAADE60
D2C_AppMode __fastcall APPMODE_LeaveGame();

//1.00 : D2Client.0x10002B80 (thunk) D2Client.0x10011B60 (impl)
//1.10f: D2Client.0x6FAAB370
//1.13c: D2Client.0x6FAF4F40
D2C_AppMode __fastcall APPMODE_Game();

//1.00 : D2Client.0x10001CA3
//1.10f: D2Client.0x6FB283D0
D2UnitStrc* __fastcall D2CLIENT_GetPlayerUnit_6FB283D0();


// Helper function
eD2GameTypes GAME_GetGameType();
//1.10f: D2Client.0x6FAAB250
BOOL __fastcall sub_6FAAB250();
//1.10f: D2Client.0x6FAAB290
BOOL __fastcall sub_6FAAB290();
//1.10f: D2Client.0x6FAAB2E0
BOOL __fastcall sub_6FAAB2E0();
//1.10f: D2Client.0x6FAAB320
BOOL __fastcall sub_6FAAB320();

