#pragma once

#include <Units/Units.h>

#include "D2Config.h"
#include "D2PacketDef.h"

#pragma warning(push)
#pragma warning(disable: 4820)
#include <storm/hash/TSHashObject.hpp>
#include <storm/hash/Hashkey.hpp>
#include <storm/hash/TSExportTableSyncReuse.hpp>
#include <storm/thread/CCritSect.hpp>
#pragma warning(pop)

struct D2ClientInfoStrc;
struct D2UnitInfoStrc;
struct D2UnitDescriptionListStrc;
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
	GAMEFLAG_ARENA_MODE					= 0x00000002,
	GAMEFLAG_ARENA_UPDATECLIENTS		= 0x00000004,
	GAMEFLAG_ARENA_TEMPLATE				= 0x00000020,
	GAMEFLAG_ARENA_CLASS_OR_TEMPLATE_ID = 0x00000100,
	GAMEFLAG_ARENA_UPDATE				= 0x00000400,
	GAMEFLAG_ARENA_HARDCORE				= 0x00000800,
	GAMEFLAG_ARENA_ACTIVE				= 0x00010000,
	GAMEFLAG_ARENA_EXPANSION			= 0x00100000,
	GAMEFLAG_ARENA_LADDER				= 0x00200000,

	GAMEFLAG_DIFFICULTY_BIT				= 12,
	GAMEFLAG_DIFFICULTY_MASK			= (0x7) << GAMEFLAG_DIFFICULTY_BIT,
};

using D2GameGUID = uint32_t;
constexpr D2GameGUID D2GameInvalidGUID = (D2GameGUID)-1;
DECLARE_STRICT_HANDLE(HGAMEDATA);
DECLARE_STRICT_HANDLE(GAMEDATALOCKEDHANDLE);

// TODO: get rid of this and use handles properly where needed
inline D2GameGUID GetHashValueFromGameHandle(HGAMEDATA hGame) { return (uint32_t)(uintptr_t)hGame; }
inline HGAMEDATA GetGameHandleFromHashValue(D2GameGUID nGameGUID) { return (HGAMEDATA)(uintptr_t)nGameGUID; }
static const HGAMEDATA D2GameReservedSlotHandle = GetGameHandleFromHashValue(D2GameInvalidGUID);


using FnCloseGame = void(__fastcall*)(WORD nGameId, uint32_t nProductCode, uint32_t nSpawnedPlayers, int32_t nFrame);
using FnLeaveGame = void(__fastcall*)(D2ClientInfoStrc** ppClientInfo, WORD nGameId, int32_t nClassId, int32_t nLevel, uint64_t nExperience, uint32_t nFlags, const char* szCharName, const char* pPreviewInfoSerialized, int32_t bUnlockChar, int32_t nZero11, int32_t nZero12, const char* szAccountName, int32_t nCharSaveTransactionToken, FILETIME* pSaveCreationTimestamp);
using FnGetDatabaseCharacter = void(__fastcall*)(D2ClientInfoStrc** ppClientInfo, const char* szCharName, DWORD dwClientId, const char* szAccountName);
using FnSaveDatabaseCharacter = void(__fastcall*)(D2ClientInfoStrc** ppClientInfo, const char* szCharName, const char* szAccountName, BYTE* pSaveData, uint32_t nSaveDataSize, int32_t nCharSaveTransactionToken);
using FnServerLogMessage = void(*)(int32_t nLogLevel, const char* szFormat, ...);
using FnEnterGame = void(__fastcall*)(WORD nGameId, const char* szCharName, int32_t nClassId, int32_t nLevel, uint32_t nFlags);
using FnFindPlayerToken = int32_t(__fastcall*)(const char* szCharName, int32_t nTokenId, WORD nGameId, char* pszOutAccountName, int32_t* pOutCharSaveTransactionToken, int32_t* a6, int32_t* a7); //TODO: Last 2 args
/*UNUSED*/	using FnSaveDatabaseGuild = int(__fastcall*)(const char*, char*, size_t);
using FnUnlockDatabaseCharacter = void(__fastcall*)(uint32_t* pGameData, const char* szCharName, const char* szAccountName);
/*UNUSED*/	using FnUnk0x24 = int(__fastcall*)(int, int);
using FnUpdateCharacterLadder = void(__fastcall*)(const char* szCharName, int32_t nClassId, int32_t nLevel, uint32_t nExperience, int32_t nZero, uint32_t nFlags, FILETIME* pSaveCreationTimestamp);
using FnUpdateGameInformation = void(__fastcall*)(WORD nGameId, const char* szCharName, int32_t nClassId, int32_t nLevel);
using FnHandlePacket = void(__fastcall*)(void* pPacket, int32_t nPacketSize);
using FnSetGameData = uint32_t(__fastcall*)();
using FnRelockDatabaseCharacter = void(__fastcall*)(D2ClientInfoStrc** ppClientInfo, const char* szCharName, const char* szAccountName);
/*UNUSED*/	using FnLoadComplete = int32_t(__stdcall*)(int32_t);

extern char gszEmptyString_6FD447EC[8];

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


struct D2GameInfoStrc
{
	int32_t nGameId;								// 0x00
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
	int32_t nPathAStarPct;							// 0x4C Note: Inverted with FoWall in D2Server
	int32_t nPathFoWallPct;							// 0x50
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
	uint8_t padding0xA7;							// 0xA7
	uint32_t nArenaFlags;							// 0xA8 D2GameFlags
	void* pMemoryPool;								// 0xAC
};

struct D2TargetNodeStrc
{
	D2UnitStrc* pUnit;							//0x00
	int32_t unk0x04;							//0x04
	D2TargetNodeStrc* pNext;					//0x08
	D2TargetNodeStrc* unk0x0C;					//0x0C
};

struct D2GameStrc : TSHashObject<D2GameStrc, HASHKEY_NONE> // called SGAMEDATA in original game
{
	LPCRITICAL_SECTION lpCriticalSection;			//0x18
	void* pMemoryPool;								//0x1C
	uint32_t nGameData;								//0x20
	uint32_t unk0x24;								//0x24
	uint16_t nGameId;								//0x28
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
	D2UnitStrc* pUnitList[5][128];					//0x1120 Note: This is not indexed by UnitType! See GAME_RemapUnitTypeToListIndex
	D2UnitStrc* pTileList;							//0x1B20
	uint32_t dwUniqueFlags[128];					//0x1B24
	D2NpcControlStrc* pNpcControl;					//0x1D24
	D2ArenaStrc* pArenaCtrl;						//0x1D28
	D2PartyControlStrc* pPartyControl;				//0x1D2C
	uint8_t nBossFlagList[64];						//0x1D30
	uint32_t dwPathTypesCount[17];					//0x1D70 Note: 17 entries only, means PATHTYPE_UNKNOWN_17 is unused ? Stats only
	uint32_t nTotalPathTypesCount;					//0x1DB4
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

struct D2GameDataTableStrc
{
	TSExportTableSyncReuse<D2GameStrc, HGAMEDATA, GAMEDATALOCKEDHANDLE, CCritSect> tHashTable;
}; // Size = 0x68

struct D2GameStatisticsStrc
{
	DWORD nGamesCount;
	DWORD nActiveMonsters;
	DWORD nInactiveMonsters;
	DWORD nPlayersCount;
};

#pragma pack(pop)

extern D2ServerCallbackFunctions* gpD2EventCallbackTable_6FD45830;

// Helper function: Missile and items are inverted on purpose here, this is like this in the game
inline int GAME_RemapUnitTypeToListIndex(const D2C_UnitTypes nUnitType)
{
	switch (nUnitType)
	{
	case UNIT_MISSILE: return 4;
	case UNIT_ITEM: return 3;
	default: return (int)nUnitType;
	}
}

//D2Game.0x6FC356D0
int32_t __stdcall sub_6FC356D0(int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5);
//D2Game.0x6FC35700
int32_t __fastcall sub_6FC35700();
//D2Game.0x6FC35720
void* __fastcall sub_6FC35720();
//D2Game.0x6FC35740
void GAME_LogMessage(int32_t a1, const char* szFormat, ...);
//D2Game.0x6FC357C0
D2GAME_DLL_DECL int32_t __stdcall D2Game_10046();
//D2Game.0x6FC35810
int32_t __stdcall D2Game_10050();
//D2Game.0x6FC35840
D2GameGUID __fastcall GAME_GetGameGUIDFromGameId(uint16_t nGameId);
//D2Game.0x6FC35880
void __stdcall GAME_InitGameDataTable(D2GameDataTableStrc* pGameDataTbl, void* pGameList);
//D2Game.0x6FC358E0
void __stdcall GAME_SetServerCallbackFunctions(D2ServerCallbackFunctions* pD2ServerCallbackFunctions);
//D2Game.0x6FC35920 (#10010)
D2GAME_DLL_DECL void __stdcall GAME_SetInitSeed(int32_t nInitSeed);
//D2Game.0x6FC35930
D2GAME_DLL_DECL uint32_t __fastcall D2Game_10011();
//D2Game.0x6FC35A10
void __fastcall GAME_ResolveGameNameConflict(D2GameStrc* pGameToSanitize, char* szGameName, int32_t a3);
//D2Game.0x6FC35CB0
BOOL __fastcall GAME_VerifyCreateNewGame(int32_t nClientId, D2GSPacketClt66* pPacket);
//D2Game.0x6FC35E50
D2GAME_DLL_DECL void __stdcall D2Game_10056(int32_t a1);
//D2Game.0x6FC35E70 (#10047)
D2GAME_DLL_DECL BOOL __stdcall GAME_CreateNewEmptyGame(char* szGameName, const char* szPassword, const char* szGameDescription, uint32_t nFlags, uint8_t nArenaTemplate, uint8_t a6, uint8_t a7, uint16_t* pGameId);
//D2Game.0x6FC36280 (#10007)
D2GAME_DLL_DECL int32_t __stdcall GAME_ReceiveDatabaseCharacter(int32_t nClientId, const uint8_t* pSaveData, uint16_t nSaveSize, uint16_t nTotalSize, int32_t a5, int32_t a6, FILETIME* pSaveCreationTimestamp, int32_t nCharSaveTransactionToken);
//D2Game.0x6FC36570
void __fastcall GAME_SendGameInit(int32_t nClientId, char* szGameName, uint8_t nGameType, uint8_t nCharTemplate, const char* szClientName, int32_t nArena, uint32_t nFlags, int32_t nArenaTemplate, int32_t a9, int32_t a10, uint8_t nDifficulty, uint8_t nLocale, int32_t a13, int32_t a14);
//D2Game.0x6FC369C0
void __fastcall sub_6FC369C0(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC36AA0
void __fastcall GAME_SendPacket0x5CToAllConnectedClients(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC36AE0
BOOL __fastcall GAME_VerifyJoinAct(int32_t nClientId);
//D2Game.0x6FC36B20
void __stdcall sub_6FC36B20(int32_t nClientId, const char* szFile, int32_t nLine);
//D2Game.0x6FC36C20
void __fastcall sub_6FC36C20(D2GameStrc* pGame, int32_t nClientId, const char* szFile, int32_t nLine);
//D2Game.0x6FC36C60
void __fastcall GAME_SendActInit(int32_t nClientId);
//D2Game.0x6FC36DF0
BOOL __fastcall GAME_VerifyJoinGame(int32_t nClientId, uint16_t nGameId, uint8_t nPlayerClass, const char* szClientName, int32_t nTokenId, char* pszInOutAccountName, int32_t* pOutCharSaveTransactionToken, uint8_t nLocale, int32_t* a9, int32_t* a10);
//D2Game.0x6FC37150
void __fastcall GAME_JoinGame(int32_t dwClientId, uint16_t nGameId, int32_t nClass, char* szClientName, char* szAccountName, int32_t nCharSaveTransactionToken, int32_t nLocale, int32_t a8, int32_t a9);
//D2Game.0x6FC37450
_Requires_lock_held_(*pGame->lpCriticalSection)
_Releases_lock_(*pGame->lpCriticalSection)
void __fastcall GAME_FreeGame(D2GameGUID nGameGUID, D2GameStrc* pGame);
//D2Game.0x6FC37560
BOOL __fastcall GAME_VerifyEndGame(int32_t nClientId);
//D2Game.0x6FC375A0
void __fastcall GAME_TriggerClientSave(D2ClientStrc* pClient, D2GameStrc* pGame);
//D2Game.0x6FC37690
void __fastcall GAME_DisconnectClient(D2GameStrc* pGame, D2ClientStrc* pClient, D2C_SRV2CLT5A_TYPES nEventType);
//D2Game.0x6FC37750
void __fastcall GAME_DisconnectClientById(int32_t nClientId, D2C_SRV2CLT5A_TYPES nEventType);
//D2Game.0x6FC37880
BOOL __stdcall GAME_DisconnectClientByName(const char* szClientName, D2C_SRV2CLT5A_TYPES nEventType);
//D2Game.0x6FC379C0
void __stdcall D2Game_10024_RemoveClientFromGame(int32_t nClientId);
//D2Game.0x6FC37B10
void __fastcall sub_6FC37B10(D2GameStrc* pGame);
//D2Game.0x6FC37B90
void __fastcall CLIENT_SendSaveHeaderPart_6FC37B90(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC37CE0
void __fastcall GAME_EndGame(int32_t nClientId, int32_t a2);
//D2Game.0x6FC37FB0
void __fastcall sub_6FC37FB0(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC380D0
void __fastcall sub_6FC380D0(D2ClientStrc* pClient, void* pArg);
//D2Game.0x6FC380F0
void __fastcall sub_6FC380F0(D2ClientStrc* pClient, void* pArg);
//D2Game.0x6FC38100
BOOL __fastcall GAME_VerifyDisconnect(int32_t nClientId);
//D2Game.0x6FC38140
void __fastcall sub_6FC38140(void* a1, int32_t a2);
//D2Game.0x6FC38530 (#10003)
D2GAME_DLL_DECL void __fastcall GAME_ProcessNetworkMessages();
//D2Game.0x6FC385A0
void __fastcall sub_6FC385A0(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC38610
void __fastcall GAME_UpdateEnvironment(D2GameStrc* pGame);
//1.10f: D2Game.0x6FC386D0
//1.14d: Game.0x52D870
void __fastcall GAME_UpdateProgress(D2GameStrc* pGame);
//D2Game.0x6FC38990
void __fastcall GAME_PopulateRoom(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC389C0
void __fastcall D2GAME_UpdateAllClients_6FC389C0(D2GameStrc* pGame);
//D2Game.0x6FC38E00
void __fastcall sub_6FC38E00();
//1.10f:D2Game.0x6FC38E20 (#10004)
//1.13c:D2Game.0x6FC4E1E0 (#10008)
D2GAME_DLL_DECL int32_t __stdcall GAME_UpdateGamesProgress(int32_t a1);
//D2Game.0x6FC39030
void __fastcall sub_6FC39030(D2GameStrc* pGame, D2ClientStrc* pClient, int32_t a3, int32_t nUnused);
//D2Game.0x6FC39270
void __fastcall sub_6FC39270(D2GameStrc* pGame, int32_t a2);
//D2Game.0x6FC392A0 (#10005)
D2GAME_DLL_DECL void __stdcall GAME_UpdateClients(int32_t a1, int32_t a2);
//D2Game.0x6FC394E0
D2GameStrc* __fastcall GAME_GetGameByClientId(int32_t nClientId);
//D2Game.0x6FC395B0
void __fastcall GAME_LeaveGamesCriticalSection(D2GameStrc* pGame);
//D2Game.0x6FC39600
void __fastcall GAME_CloseGame(D2GameGUID nGameGUID);
//D2Game.0x6FC397A0
_Acquires_lock_(*(return->lpCriticalSection))
_Acquires_lock_(return->lpCriticalSection)
D2GameStrc* __fastcall GAME_LockGame(D2GameGUID nGameGUID);
//D2Game.0x6FC39870
void __fastcall sub_6FC39870(int32_t nClientId);
//D2Game.0x6FC399A0
int32_t __fastcall sub_6FC399A0(int32_t nClientId, D2GSPacketClt6B* pPacket);
//D2Game.0x6FC399C0
D2GAME_DLL_DECL void __stdcall GAME_CloseAllGames();
//D2Game.0x6FC39B50 (#10012)
D2GAME_DLL_DECL int32_t __fastcall GAME_GetGamesCount();
//D2Game.0x6FC39B70 (#10057)
D2GAME_DLL_DECL int32_t __stdcall D2Game_10057();
//D2Game.0x6FC39C70
D2GAME_DLL_DECL void __stdcall D2Game_10053(int16_t* pCount, int32_t nArraySize);
//D2Game.0x6FC39D80
D2GAME_DLL_DECL void __stdcall D2Game_10054(uint16_t* a1, int32_t nMaxCount);
//D2Game.0x6FC39EC0
D2GAME_DLL_DECL int32_t __fastcall D2Game_10051();
//D2Game.0x6FC39ED0
D2GAME_DLL_DECL void __stdcall GAME_GetMemoryUsage(int* pCurrentMemoryUsage, int* pPeakMemoryUsageInLast10s);
//D2Game.0x6FC39EF0
D2GAME_DLL_DECL int32_t __stdcall D2Game_10013(uint16_t nGameId);
//D2Game.0x6FC39FF0 (#10014)
D2GAME_DLL_DECL BOOL __stdcall GAME_GetGameInformation(uint16_t nGameId, D2GameInfoStrc* pGameInfo);
//D2Game.0x6FC3A390 (#10015)
D2GAME_DLL_DECL int32_t __stdcall GAME_GetGameServerGameIds(uint16_t* pServerToken, int32_t nMaxCount);
//D2Game.0x6FC3A490 (#10016)
D2GAME_DLL_DECL int32_t __stdcall GAME_GetPlayerUnitsCount(uint16_t nGameId);
//D2Game.0x6FC3A5A0 (#10017)
D2GAME_DLL_DECL int32_t __stdcall GAME_GetPlayerUnitsInfo(uint16_t nGameId, D2UnitInfoStrc* pUnitInfo, int32_t nMaxCount);
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
//D2Game.0x6FC3A8B0 (#10019)
D2GAME_DLL_DECL void __stdcall GAME_GetUnitsDescriptions(uint16_t nGameId, D2UnitDescriptionListStrc* pUnitDescriptionsList, uint32_t eType);
//D2Game.0x6FC3AB20 (#10018)
D2GAME_DLL_DECL int32_t __stdcall D2Game_10018(uint16_t nGameId, int32_t nMaxCount, D2UnitInfoStrc* pUnitInfo, int32_t* pUnitType, int32_t* pUnitGUID);
//D2Game.0x6FC3ACD0 (#10020)
D2GAME_DLL_DECL void __stdcall GAME_GetStatistics(D2GameStatisticsStrc* pStats);
//D2Game.0x6FC3AE10 (#10021)
D2GAME_DLL_DECL void __stdcall GAME_SendMessageToAllClients(int32_t a1, int32_t nPacketParam, const char* szMessage);
//D2Game.0x6FC3AFB0 (#10022)
D2GAME_DLL_DECL void __stdcall GAME_SendMessageToGameClients(uint16_t a1, const char* Str2);
using GAME_ForEachIngameClientCallbackPtr = void(__fastcall*)(D2ClientStrc*, void*);
//D2Game.0x6FC3B0E0
void __fastcall GAME_ForEachIngameClient(D2GameStrc* pGame, GAME_ForEachIngameClientCallbackPtr pFn, void* pContext);
//D2Game.0x6FC3B160
D2GameStrc* __fastcall sub_6FC3B160();
//D2Game.0x6FC3B220
void __fastcall GAME_LeaveGlobalGamesCriticalSection();
//D2Game.0x6FC3B280
void __stdcall D2Game_10008(int32_t a1);
//D2Game.0x6FC3B2A0 (#10009)
D2GAME_DLL_DECL void __stdcall GAME_SetGlobalAct(int32_t nAct);
//D2Game.0x6FC3B2B0
void __fastcall sub_6FC3B2B0(D2UnitStrc* pUnit, D2GameStrc* pGame);
//D2Game.0x6FC3B3D0
void __fastcall sub_6FC3B3D0(D2ClientStrc* pClient, D2UnitStrc* pUnit);
//D2Game.0x6FC3B480) --------------------------------------------------------
D2GameStrc* __fastcall D2GameDataTable_Lock(D2GameDataTableStrc* pGameDataTable, int32_t nUnused, HGAMEDATA hGame, GAMEDATALOCKEDHANDLE* pLockedHandle, int32_t forWriting);
//D2Game.0x6FC3B510
void __fastcall D2GameDataTable_SyncEnterLock(D2GameDataTableStrc* pGameDataTable, int32_t nUnused, int32_t* pLockHandle, int32_t bForWriting);
//D2Game.0x6FC3B540
void __fastcall D2GameDataTable_SyncLeaveLock(D2GameDataTableStrc* pGameDataTable, int32_t nUnused, int32_t tLockHandle);
//D2Game.0x6FC3B560
TSLink<D2GameStrc>* __fastcall D2GameDataTable_TSGetExplicitLink_Link(STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)* pNode, int32_t nUnused, int nLinkOffset);
//D2Game.0x6FC3B590
D2GameStrc* __fastcall D2GameDataTable_New(D2GameDataTableStrc* pGameDataTable, int32_t nUnused, D2GameGUID nGameGUID, HASHKEY_NONE* pKey, int32_t extrabytes, int32_t flags);
//D2Game.0x6FC3B6A0
D2GameStrc* __fastcall D2GameDataTable_Ptr(D2GameDataTableStrc* pGameDataTable, int32_t nUnused, D2GameGUID nGameGUID, const HASHKEY_NONE* pHashKey);
//D2Game.0x6FC3B6F0
TSLink<D2GameStrc>* __fastcall D2GameDataTable_TSExplicitList_Link(STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)* pNode, int32_t nUnused, D2GameStrc* ptr);
//D2Game.0x6FC3B710
//void __thiscall TSHashTable_SGAMEDATA::Grow_6FC3B710(D2GameDataTableStrc pGameDataTable)
//D2Game.0x6FC3B8E0
void __fastcall D2GameDataTable_TSExplicitList_UnlinkAll_AndUninitTerminator(STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)* pList);
//D2Game.0x6FC3B960
void __fastcall D2GameDataTable_TSLink_Unlink(TSLink<D2GameStrc>* pLink);
//D2Game.0x6FC3B9A0
void __fastcall D2GameDataTable_GrowableArray_TSExplicitList_SetCount(TSGrowableArray<STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)>* pArray, int32_t nUnused, uint32_t nCount);
//D2Game.0x6FC3BB10
void __fastcall D2GameDataTable_TSExplicitList_Destroy(STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)* pNode, int32_t nUnused, char bShouldFree);
//D2Game.0x6FC3BB40
void __fastcall D2GameDataTable_TSExplicitList_InitializeTerminator(STORM_EXPLICIT_LIST(D2GameStrc, m_linktoslot)* pList);
//D2Game.0x6FC3BB50
void __fastcall D2GameDataTable_TSExplicitList_InplaceNew(void* pMemory);
//D2Game.0x6FC3BB80
void __fastcall D2GameDataTable_TSExplicitList_InplaceNew_WithList(void* pMemory, int32_t nUnused, TSExplicitList<D2GameStrc, 0xDDDDDDDD>* pList);

