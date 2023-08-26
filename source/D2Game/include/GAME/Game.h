#pragma once

#include <Units/Units.h>

#include "D2PacketDef.h"

struct D2ClientInfoStrc;
struct D2UnitInfoStrc;
struct D2PartyControlStrc;
struct D2ArenaStrc;
struct D2InactiveUnitListStrc;
struct D2MonsterRegionStrc;
struct D2NpcControlStrc;
struct D2QuestInfoStrc;
struct D2EventTimerQueueStrc;

enum D2PacketTypeAdmin
{
	PACKET_ADMIN_GETINFO = 0x01,
	PACKET_ADMIN_CONNECT = 0xFA,
	PACKET_ADMIN_DISCONNECT = 0xFB,
	PACKET_ADMIN_GETIP = 0xFC,
	PACKET_ADMIN_GETGAMEINFO = 0xFD
};

enum D2GameFlags
{
	GAMEFLAG_ARENA_MODE          = 0x00000002,
	GAMEFLAG_ARENA_UPDATECLIENTS = 0x00000004,
	GAMEFLAG_ARENA_TEMPLATE      = 0x00000020,
	GAMEFLAG_ARENA_UPDATE        = 0x00000400,
	GAMEFLAG_ARENA_HARDCORE      = 0x00000800,
	GAMEFLAG_ARENA_ACTIVE        = 0x00010000,
	GAMEFLAG_ARENA_EXPANSION     = 0x00100000,
	GAMEFLAG_ARENA_LADDER        = 0x00200000,

	GAMEFLAG_DIFFICULTY_BIT      = 12,
	GAMEFLAG_DIFFICULTY_MASK     = (0x7) << GAMEFLAG_DIFFICULTY_BIT,
};

using D2GameGUID = uint32_t;
constexpr D2GameGUID D2GameInvalidGUID = (D2GameGUID)-1;

using FnCloseGame = void(__fastcall*)(WORD nGameId, uint32_t nFlags, uint32_t nSpawnedPlayers, int32_t nFrame);
using FnLeaveGame = void(__fastcall*)(D2ClientInfoStrc** ppClientInfo, WORD nGameId, int32_t nClassId, int32_t nLevel, uint32_t nExperience, int32_t a6, uint32_t nFlags, const char* szCharName, const char* a9, int32_t bUnlockChar, int32_t nZero11, int32_t nZero12, const char* szAccountName, int32_t a14, void* a15); //TODO: Unknown args
using FnGetDatabaseCharacter = void(__fastcall*)(D2ClientInfoStrc** ppClientInfo, const char* szCharName, DWORD dwClientId, const char* szAccountName);
using FnSaveDatabaseCharacter = void(__fastcall*)(int32_t* pRealmId, const char* szCharName, const char* szAccountName, BYTE* pSaveData, uint32_t nSaveDataSize, int32_t nUnused);
using FnServerLogMessage = void(*)(int32_t nLogLevel, const char* szFormat, ...);
using FnEnterGame = void(__fastcall*)(WORD nGameId, const char* szCharName, int32_t nClassId, int32_t nLevel, uint32_t nFlags);
using FnFindPlayerToken = int32_t(__fastcall*)(const char* szCharName, int32_t nToken, WORD nGameId, const char* szAccountName, int32_t* a5, int32_t* a6, int32_t* a7); //TODO: Last 3 args
/*UNUSED*/	using FnSaveDatabaseGuild = int(__fastcall*)(const char*, char*, size_t);
using FnUnlockDatabaseCharacter = void(__fastcall*)(uint32_t* pGameData, const char* szCharName, const char* szAccountName);
/*UNUSED*/	using FnUnk0x24 = int(__fastcall*)(int, int);
using FnUpdateCharacterLadder = void(__fastcall*)(const char* szCharName, int32_t nClassId, int32_t nLevel, uint32_t nExperience, int32_t nZero, uint32_t nFlags, void* pUnknown);
using FnUpdateGameInformation = void(__fastcall*)(WORD nGameId, const char* szCharName, int32_t nClassId, int32_t nLevel);
using FnHandlePacket = void(__fastcall*)(void* pPacket, int32_t nPacketSize);
using FnSetGameData = uint32_t(__fastcall*)();
using FnRelockDatabaseCharacter = void(__fastcall*)(int32_t* pRealmId, const char* szCharName, const char* szAccountName);
/*UNUSED*/	using FnLoadComplete = int32_t(__stdcall*)(int32_t);

#pragma pack(push, 1)
struct D2ServerCallbackFunctions							// sizeof 0x40
{
	FnCloseGame pfCloseGame;								//0x00
	FnLeaveGame pfLeaveGame;								//0x04
	FnGetDatabaseCharacter pfGetDatabaseCharacter;			//0x08
	FnSaveDatabaseCharacter pfSaveDatabaseCharacter;		//0x0C
	FnServerLogMessage pfServerLogMessage;					//0x10
	FnEnterGame pfEnterGame;								//0x14
	FnFindPlayerToken pfFindPlayerToken;					//0x18
	FnSaveDatabaseGuild pfSaveDatabaseGuild;				//0x1C
	FnUnlockDatabaseCharacter pfUnlockDatabaseCharacter;	//0x20
	FnUnk0x24 unk0x24;										//0x24
	FnUpdateCharacterLadder pfUpdateCharacterLadder;		//0x28
	FnUpdateGameInformation pfUpdateGameInformation;		//0x2C
	FnHandlePacket pfHandlePacket;							//0x30
	FnSetGameData pfSetGameData;							//0x34
	FnRelockDatabaseCharacter pfRelockDatabaseCharacter;	//0x38
	FnLoadComplete pfLoadComplete;							//0x3C
};

struct D2GameHashTableStrc
{
	int32_t unk0x00;
	int32_t unk0x04;
	int32_t unk0x08;
	int32_t unk0x0C;
	int32_t unk0x10;
	int32_t unk0x14;
	int32_t unk0x18;
	int32_t unk0x1C;
	int32_t unk0x20;
	int32_t unk0x24;
};

struct D2GameManagerStrc
{
	D2GameHashTableStrc unk0x00;
	D2GameHashTableStrc pGamesEx;
	CRITICAL_SECTION pLock;
};

struct D2GameInfoStrc
{
	int32_t nServerToken;							// 0x00 nGameId
	uint32_t nInitSeed;								// 0x04
	int32_t nClients;								// 0x08
	int32_t nPlayers;								// 0x0C
	int32_t nMonsters;								// 0x10
	int32_t nObjects;								// 0x14
	int32_t nItems;									// 0x18
	int32_t nMissiles;								// 0x1C
	int32_t nUniqueItems;							// 0x20
	int32_t nNPCs;									// 0x24
	uint32_t dwLastUsedUnitGUID[UNIT_TYPES_COUNT];	// 0x3C Tile = Warp
	int32_t nPathTowardPct;							// 0x40
	int32_t nPathClockPct;							// 0x44
	int32_t nPathCounterPct;						// 0x48
	int32_t nPathFoWallPct;							// 0x4C
	int32_t nPathAStarPct;							// 0x50
	int32_t nPathTotalCalls;						// 0x54
	int32_t nFrames;								// 0x58 Frames / 100
	int32_t nTime;									// 0x5C Time (minutes) since game creation (bugged, actually returns machine uptime)
	int32_t nFrameRate;								// 0x60
	char szGameName[16];							// 0x64
	char szGamePassword[16];						// 0x74
	char szGameDescription[32];						// 0x84 Before 1.10f (unsure what version), struct used to have desc of 24 chars, and end here.
	uint8_t nArenaTemplate;							// 0xA4
	uint8_t unk0xA5;								// 0xA5
	uint8_t unk0xA6;								// 0xA6
	uint8_t unk0xA7;								// 0xA7
	uint32_t nArenaFlags;							// 0xA8
	void* pMemoryPool;								// 0xAC
};

struct D2TargetNodeStrc
{
	D2UnitStrc* pUnit;							//0x00
	int32_t unk0x04;							//0x04
	D2TargetNodeStrc* pNext;					//0x08
	D2TargetNodeStrc* unk0x0C;					//0x0C
};

struct D2GameStrc
{
	uint32_t nHashKey;								//0x00
	uint32_t __0004[3];								//0x04
	D2GameStrc* pNext;								//0x10
	uint32_t __0014;								//0x14
	LPCRITICAL_SECTION lpCriticalSection;			//0x18
	void* pMemoryPool;								//0x1C
	uint32_t nGameData;								//0x20
	uint32_t unk0x24;								//0x24
	uint16_t nServerToken;							//0x28
	char szGameName[16];							//0x2A
	char szGamePassword[16];						//0x3A
	char szGameDesc[32];							//0x4A
	uint8_t nGameType;								//0x6A
	uint8_t nArenaTemplate;							//0x6B
	uint8_t unk0x6C;								//0x6C
	uint8_t nDifficulty;							//0x6D
	uint8_t unk0x6E;								//0x6E
	uint8_t unk0x6F;								//0x6F
	BOOL bExpansion;								//0x70
	uint32_t dwGameType;							//0x74 1=Ladder
	uint16_t wItemFormat;							//0x78
	uint16_t unk0x7A;								//0x7A
	uint32_t dwInitSeed;							//0x7C
	uint32_t dwObjSeed;								//0x80
	uint32_t InitSeed;								//0x84
	D2ClientStrc* pClientList;						//0x88
	uint32_t nClients;								//0x8C
	uint32_t dwLastUsedUnitGUID[UNIT_TYPES_COUNT];	//0x90
	int32_t dwGameFrame;							//0xA8
	uint32_t nFrameRate;							//0xAC
	uint32_t nFramesSinceLastFrameRateUpdate;		//0xB0
	uint32_t nPreviousUpdateTickCount;				//0xB4
	D2EventTimerQueueStrc* pTimerQueue;				//0xB8
	D2DrlgActStrc* pAct[5];							//0xBC
	D2SeedStrc pGameSeed;							//0xD0
	D2InactiveUnitListStrc* pInactiveUnitList[5];	//0xD8
	uint32_t dwMonSeed;								//0xEC
	D2MonsterRegionStrc* pMonReg[1024];				//0xF0
	D2ObjectControlStrc* pObjectControl;			//0x10F0
	D2QuestInfoStrc* pQuestControl;					//0x10F4
	D2TargetNodeStrc* pTargetNodes[10];				//0x10F8
	D2UnitStrc* pUnitList[5][128];					//0x1120 Note: This is not indexed by UnitType!! UNIT_MISSILE is index 4 and UNIT_ITEM is index 3
	D2UnitStrc* pTileList;							//0x1B20
	uint32_t dwUniqueFlags[128];					//0x1B24
	D2NpcControlStrc* pNpcControl;					//0x1D24
	D2ArenaStrc* pArenaCtrl;						//0x1D28
	D2PartyControlStrc* pPartyControl;				//0x1D2C
	uint8_t nBossFlagList[64];						//0x1D30
	uint32_t dwMonModeData[17];						//0x1D70
	uint32_t nMonModeData;							//0x1DB4
	uint32_t nLastUpdateSystemTimeMs;				//0x1DB8
	uint32_t nCreationTimeMs_Or_CPUTargetRatioFP10;	//0x1DBC Used to be the creation time, but now represents ratio of the budget used for last frame, in FP10 represenation (x/1024)
	uint32_t nTickCountSinceNoClients;				//0x1DC0
	uint32_t nSyncTimer;							//0x1DC4

	uint32_t unk0x1DC8;								//0x1DC8
	uint32_t unk0x1DCC;								//0x1DCC
	uint32_t unk0x1DD0;								//0x1DD0
	uint32_t unk0x1DD4;								//0x1DD4
	uint32_t unk0x1DD8;								//0x1DD8
	uint32_t unk0x1DDC;								//0x1DDC
};

struct D2GameStatisticsStrc
{
	DWORD nGamesCount;
	DWORD nActiveMonsters;
	DWORD nInactiveMonsters;
	DWORD nPlayersCount;
};

#pragma pack(pop)

extern D2ServerCallbackFunctions* gpD2ServerCallbackFunctions_6FD45830;



//D2Game.0x6FC356D0
int32_t __stdcall sub_6FC356D0(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5);
//D2Game.0x6FC35700
int32_t __fastcall sub_6FC35700();
//D2Game.0x6FC35720
void* __fastcall sub_6FC35720();
//D2Game.0x6FC35740
void GAME_LogMessage(int32_t a1, const char* szFormat, ...);
//D2Game.0x6FC357C0
int32_t __stdcall D2Game_10046();
//D2Game.0x6FC35810
int32_t __stdcall D2Game_10050();
//D2Game.0x6FC35840
int32_t __fastcall sub_6FC35840(uint16_t nGameId);
//D2Game.0x6FC35880
void __stdcall D2Game_10002(D2GameManagerStrc* pGameDataTbl, void* pGameList);
//D2Game.0x6FC358E0
void __stdcall GAME_SetServerCallbackFunctions(D2ServerCallbackFunctions* pD2ServerCallbackFunctions);
//D2Game.0x6FC35920 (#10010)
void __stdcall GAME_SetInitSeed(int32_t nInitSeed);
//D2Game.0x6FC35930
uint32_t __fastcall D2Game_10011();
//D2Game.0x6FC35A10
void __fastcall GAME_ResolveGameNameConflict(D2GameStrc* pGameToSanitize, char* szGameName, int32_t a3);
//D2Game.0x6FC35CB0
int32_t __fastcall GAME_VerifyCreateNewGame(int32_t nClientId, D2GSPacketClt66* pPacket);
//D2Game.0x6FC35E50
void __stdcall D2Game_10056(int32_t a1);
//D2Game.0x6FC35E70
int32_t __stdcall GAME_CreateNewEmptyGame(char* szGameName, const char* szPassword, const char* szGameDescription, uint32_t nFlags, uint8_t a5, uint8_t a6, uint8_t a7, uint16_t* pGameId);
//D2Game.0x6FC36280 (#10007)
int32_t __stdcall GAME_ReceiveDatabaseCharacter(int32_t nClientId, const uint8_t* pSaveData, uint16_t nSaveSize, uint16_t nTotalSize, int32_t a5, int32_t a6, int32_t a7, int32_t a8);
//D2Game.0x6FC36570
void __fastcall GAME_SendGameInit(int32_t nClientId, char* szGameName, uint8_t nGameType, uint8_t nCharTemplate, const char* szClientName, int32_t a2, uint32_t nFlags, int32_t nArenaTemplate, int32_t a9, int32_t a10, uint8_t nDifficulty, uint8_t nExpLost, int32_t a13, int32_t a14);
//D2Game.0x6FC369C0
void __fastcall sub_6FC369C0(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC36AA0
void __fastcall GAME_SendPacket0x5CToAllConnectedClients(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC36AE0
int32_t __fastcall GAME_VerifyJoinAct(int32_t nClientId);
//D2Game.0x6FC36B20
void __stdcall sub_6FC36B20(int32_t nClientId, const char* szFile, int32_t nLine);
//D2Game.0x6FC36C20
void __fastcall sub_6FC36C20(D2GameStrc* pGame, int32_t nClientId, const char* szFile, int32_t nLine);
//D2Game.0x6FC36C60
void __fastcall GAME_SendActInit(int32_t nClientId);
//D2Game.0x6FC36DF0
int32_t __fastcall GAME_VerifyJoinGme(int32_t nClientId, uint16_t nGameId, uint8_t nPlayerClass, const char* szClientName, int32_t nToken, const char* szAccountName, int32_t* a7, uint8_t nLocale, int32_t* a9, int32_t* a10);
//D2Game.0x6FC37150
void __fastcall GAME_JoinGame(int32_t dwClientId, uint16_t nGameId, int32_t a3, char* szClientName, char* szAccountName, int32_t a6, int32_t a7, int32_t a8, int32_t a9);
//D2Game.0x6FC37450
void __fastcall GAME_FreeGame(int32_t a1, D2GameStrc* pGame);
//D2Game.0x6FC37560
int32_t __fastcall GAME_VerifyEndGame(int32_t nClientId);
//D2Game.0x6FC375A0
void __fastcall GAME_TriggerClientSave(D2ClientStrc* pClient, D2GameStrc* pGame);
//D2Game.0x6FC37690
void __fastcall GAME_DisconnectClient(D2GameStrc* pGame, D2ClientStrc* pClient, uint8_t a3);
//D2Game.0x6FC37750
void __fastcall GAME_DisconnectClientById(int32_t nClientId, int32_t a2);
//D2Game.0x6FC37880
int32_t __stdcall GAME_DisconnectClientByName(const char* szClientName, char a3);
//D2Game.0x6FC379C0
void __stdcall D2Game_10024_RemoveClientFromGame(int32_t nClientId);
//D2Game.0x6FC37B10
void __fastcall sub_6FC37B10(D2GameStrc* pGame);
//D2Game.0x6FC37B90
void __fastcall sub_6FC37B90(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC37CE0
void __fastcall GAME_EndGame(int32_t nClientId, int32_t a2);
//D2Game.0x6FC37FB0
void __fastcall sub_6FC37FB0(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC380D0
void __fastcall sub_6FC380D0(D2ClientStrc* pClient, void* pArg);
//D2Game.0x6FC380F0
void __fastcall sub_6FC380F0(D2ClientStrc* pClient, void* pArg);
//D2Game.0x6FC38100
int32_t __fastcall sub_6FC38100(int32_t nClientId);
//D2Game.0x6FC38140
void __fastcall sub_6FC38140(void* a1, int32_t a2);
//D2Game.0x6FC38530 (#10003)
void __fastcall GAME_ProcessNetworkMessages();
//D2Game.0x6FC385A0
void __fastcall sub_6FC385A0(D2GameStrc* pGame, D2RoomStrc* pRoom);
//D2Game.0x6FC38610
void __fastcall GAME_UpdateEnvironment(D2GameStrc* pGame);
//D2Game.0x6FC386D0
void __fastcall GAME_UpdateProgress(D2GameStrc* pGame);
//D2Game.0x6FC38990
void __fastcall GAME_PopulateRoom(D2GameStrc* pGame, D2RoomStrc* pRoom);
//D2Game.0x6FC389C0
void __fastcall D2GAME_UpdateAllClients_6FC389C0(D2GameStrc* pGame);
//D2Game.0x6FC38E00
void __fastcall sub_6FC38E00();
//D2Game.0x6FC38E20 (#10004)
int32_t __stdcall GAME_UpdateGamesProgress(int32_t a1);
//D2Game.0x6FC39030
void __fastcall sub_6FC39030(D2GameStrc* pGame, D2ClientStrc* pClient, int32_t a3, int32_t nUnused);
//D2Game.0x6FC39270
void __fastcall sub_6FC39270(D2GameStrc* pGame, int32_t a2);
//D2Game.0x6FC392A0 (#10005)
void __stdcall GAME_UpdateClients(int32_t a1, int32_t a2);
//D2Game.0x6FC394E0
D2GameStrc* __fastcall GAME_GetGameByClientId(int32_t nClientId);
//D2Game.0x6FC395B0
void __fastcall GAME_LeaveGamesCriticalSection(D2GameStrc* pGame);
//D2Game.0x6FC39600
void __fastcall GAME_CloseGame(int32_t a1);
//D2Game.0x6FC397A0
D2GameStrc* __fastcall TASK_GetGame_6FC397A0(int32_t nGameHashKey);
//D2Game.0x6FC39870
void __fastcall sub_6FC39870(int32_t nClientId);
//D2Game.0x6FC399A0
int32_t __fastcall sub_6FC399A0(int32_t nClientId, D2GSPacketClt6B* pPacket);
//D2Game.0x6FC399C0
void __stdcall D2Game_10006();
//D2Game.0x6FC39B50 (#10012)
int32_t __fastcall GAME_GetGamesCount();
//D2Game.0x6FC39B70
int32_t __stdcall D2Game_10057();
//D2Game.0x6FC39C70
void __stdcall D2Game_10053(int16_t* pCount, int32_t nArraySize);
//D2Game.0x6FC39D80
void __stdcall D2Game_10054(uint16_t* a1, int32_t nMaxCount);
//D2Game.0x6FC39EC0
int32_t __fastcall D2Game_10051();
//D2Game.0x6FC39ED0
void __stdcall GAME_GetMemoryUsage(int* pCurrentMemoryUsage, int* pPeakMemoryUsageInLast10s);
//D2Game.0x6FC39EF0
int32_t __stdcall D2Game_10013(uint16_t nGameId);
//D2Game.0x6FC39FF0
int32_t __stdcall D2Game_10014(uint16_t nGameId, D2GameInfoStrc* pGameInfo);
//D2Game.0x6FC3A390 (#10015)
int32_t __stdcall GAME_GetGameServerTokens(uint16_t* pServerToken, int32_t nMaxCount);
//D2Game.0x6FC3A490
int32_t __stdcall D2Game_10016(uint16_t a1);
//D2Game.0x6FC3A5A0
int32_t __stdcall D2Game_10017(uint16_t nGameId, D2UnitInfoStrc* pUnitInfo, int32_t nMaxCount);
//D2Game.0x6FC3A6F0
int32_t __stdcall GAME_ReturnArgument(int32_t a1);
//D2Game.0x6FC3A700
void __fastcall GAME_GetPlayerDescription(char* szDescription, int32_t nClassId);
//D2Game.0x6FC3A7C0
void __fastcall GAME_GetMonsterDescription(char* szDescription, int32_t nClassId);
//D2Game.0x6FC3A820
void __fastcall GAME_GetObjectDescription(char* szDescription, int32_t nClassId);
//D2Game.0x6FC3A860
void __fastcall GAME_GetItemDescription(char* szDescription, int32_t nClassId);
//D2Game.0x6FC3A8A0
void __fastcall GAME_GetMissileDescription(char* szDescription, int32_t nClassId);
//D2Game.0x6FC3A8B0
void __stdcall D2Game_10019(uint16_t a1, int32_t a2, int32_t nUnitType);
//D2Game.0x6FC3AB20
int32_t __stdcall D2Game_10018(uint16_t nGameId, int32_t nMaxCount, D2UnitInfoStrc* pUnitInfo, int32_t* pUnitType, int32_t* pUnitGUID);
//D2Game.0x6FC3ACD0
void __stdcall GAME_GetStatistics(D2GameStatisticsStrc* pStats);
//D2Game.0x6FC3AE10
void __stdcall D2Game_10021(int32_t a1, int32_t nPacketParam, const char* szMessage);
//D2Game.0x6FC3AFB0
void __stdcall D2Game_10022(uint16_t a1, const char* Str2);
//D2Game.0x6FC3B0E0
void __fastcall GAME_SendPacketToAllConnectedClients(D2GameStrc* pGame, void(__fastcall* pFn)(D2ClientStrc*, void*), void* pPacket);
//D2Game.0x6FC3B160
D2GameStrc* __fastcall sub_6FC3B160();
//D2Game.0x6FC3B220
void __fastcall GAME_LeaveGlobalGamesCriticalSection();
//D2Game.0x6FC3B280
void __stdcall D2Game_10008(int32_t a1);
//D2Game.0x6FC3B2A0 (#10009)
void __stdcall GAME_SetGlobalAct(int32_t nAct);
//D2Game.0x6FC3B2B0
void __fastcall sub_6FC3B2B0(D2UnitStrc* pUnit, D2GameStrc* pGame);
//D2Game.0x6FC3B3D0
void __fastcall sub_6FC3B3D0(D2ClientStrc* pClient, D2UnitStrc* pUnit);
//D2Game.0x6FC3B480) --------------------------------------------------------
D2GameStrc* __fastcall D2GAME_FindAndLockGameByGUID__6FC3B480(D2GameManagerStrc* pGameManager, int32_t nUnused, int32_t a2, void* a3, int32_t a4);
//D2Game.0x6FC3B510
void __fastcall D2GameManagerStrc_Lock_6FC3B510(D2GameManagerStrc* pGameManager, int32_t nUnused, int32_t* a2, int32_t a3);
//D2Game.0x6FC3B540
void __fastcall D2GameManagerStrc_Unlock_6FC3B540(D2GameManagerStrc* pGameManager, int32_t nUnused, int32_t bLeaveCriticalSection);
////D2Game.0x6FC3B560) --------------------------------------------------------
//int32_t __thiscall sub_6FC3B560(void* this, int32_t a2);
//D2Game.0x6FC3B590) --------------------------------------------------------
D2GameStrc* __fastcall sub_6FC3B590(D2GameManagerStrc* a1, int32_t nUnused, int32_t a2, int32_t* a3, int32_t a4, int32_t a5);
//D2Game.0x6FC3B6A0) --------------------------------------------------------
D2GameStrc* __fastcall D2GAME_FindGameByGUID_6FC3B6A0(D2GameManagerStrc* a1, int32_t nUnused, D2GameGUID nGameGUID, void* a4);
////D2Game.0x6FC3B6F0) --------------------------------------------------------
//int32_t __thiscall sub_6FC3B6F0(void* this, int32_t a2);
////D2Game.0x6FC3B710) --------------------------------------------------------
//void __thiscall sub_6FC3B710(int32_t this);
////D2Game.0x6FC3B8E0) --------------------------------------------------------
//void __thiscall sub_6FC3B8E0(int32_t this);
////D2Game.0x6FC3B960) --------------------------------------------------------
//void __thiscall sub_6FC3B960(void* this);
////D2Game.0x6FC3B9A0) --------------------------------------------------------
//int32_t __thiscall sub_6FC3B9A0(int32_t this, uint32_t a2);
////D2Game.0x6FC3BB10) --------------------------------------------------------
//int32_t __thiscall sub_6FC3BB10(int32_t this, char a2);
////D2Game.0x6FC3BB40) --------------------------------------------------------
//int32_t __thiscall sub_6FC3BB40(int32_t this);
////D2Game.0x6FC3BB50) --------------------------------------------------------
//int32_t __thiscall sub_6FC3BB50(int32_t this);
//D2Game.0x6FC3BB80
void __fastcall sub_6FC3BB80(int32_t a1, int32_t a2);
