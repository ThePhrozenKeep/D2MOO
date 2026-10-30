#pragma once

#include "D2PacketDef.h"
#include <Units/Units.h>
#include "Game.h"
#include "D2Packet.h"

struct D2SaveHeaderStrc;

#pragma pack(1)
enum D2SystemError
{
	SYSERROR_SUCCESS = 0,
	SYSERROR_BAD_INPUT = 1,
	SYSERROR_UNK_1 = 1,
	SYSERROR_UNK_2 = 2,
	SYSERROR_BAD_WAYPOINT = 3,
	SYSERROR_BAD_STATS = 4,
	SYSERROR_BAD_SKILLS = 5,
	SYSERROR_UNK_6 = 6,
	SYSERROR_UNK_7 = 7,
	SYSERROR_CORPSES = 8,
	SYSERROR_UNK_9 = 9,
	SYSERROR_UNK_10 = 10,
	SYSERROR_UNK_11 = 11,
	SYSERROR_UNK_12 = 12,
	SYSERROR_INVENTORY = 13,
	SYSERROR_UNK_14 = 14,
	SYSERROR_NIGHTMARE_NOT_UNLOCKED = 17,
	SYSERROR_HELL_NOT_UNLOCKED = 18,
	SYSERROR_SOFTCOREJOINHARDCORE = 19,
	SYSERROR_HARDCOREJOINSOFTCORE = 20,
	SYSERROR_DEADHARDCORE = 21,
	SYSERROR_EXPANSIONGAME = 23,
	SYSERROR_NOTEXPANSIONGAME = 24,
	SYSERROR_LADDERGAME = 25,
	SYSERROR_NOTLADDERGAME = 26,
};

enum D2ClientState : uint32_t
{
	CLIENTSTATE_JUST_CREATED = 0,
	CLIENTSTATE_GAME_INIT_SENT = 1,
	CLIENTSTATE_ACT_INIT_SENT = 2,
	CLIENTSTATE_PLAYER_SPAWNED = 3,
	CLIENTSTATE_INGAME = 4,
	CLIENTSTATE_CHANGING_ACT = 5,
};


// Character information flags
enum D2ClientSaveFlags
{
	CLIENTSAVEFLAG_INIT = 0x1, // Newbie save
	CLIENTSAVEFLAG_0x2 = 0x2, // Set at character creation for realm characters.
	CLIENTSAVEFLAG_HARDCORE = 0x4,
	CLIENTSAVEFLAG_DEAD = 0x8,
	CLIENTSAVEFLAG_0x10 = 0x10,
	CLIENTSAVEFLAG_EXPANSION = 0x20,
	CLIENTSAVEFLAG_LADDER = 0x40,
	CLIENTSAVEFLAG_0x80 = 0x80,
	CLIENTSAVEFLAG_WEAPON_SWITCH = 0x2000,

	// Encodes completed acts 
	// => 0 No act completed
	// => Acts 1-5 Normal (values 1-5), then 1-5 NM (values 6-10), then 1-5 Hell (values 11-15) for Expansion
	// => Acts 1-4 Normal (values 1-4), then 1-4 NM (values 5-8), then 1-4 Hell  (values 9-12) for Classic
	CLIENTSAVEFLAG_CHARACTER_PROGRESSION_BIT = 8,
	CLIENTSAVEFLAG_CHARACTER_PROGRESSION_MASK = (0x1F) << CLIENTSAVEFLAG_CHARACTER_PROGRESSION_BIT,
};

union D2PackedClientSaveFlags
{
	uint16_t nPackedValue;			// D2ClientSaveFlags
	struct {
		uint16_t bInit : 1;			// BIT(0)
		uint16_t bUnkFlag0x02 : 1;	// BIT(1)
		uint16_t bHardcore : 1;		// BIT(2)
		uint16_t bDead : 1;			// BIT(3)
		uint16_t bUnkFlag0x10 : 1;	// BIT(4)
		uint16_t bExpansion : 1;	// BIT(5)
		uint16_t bLadder : 1;		// BIT(6)
		uint16_t bUnkFlag0x80 : 1;	// BIT(7)
		// Encodes completed acts
		// => 0 No act completed
		// => Acts 1-5 Normal (values 1-5), then 1-5 NM (values 6-10), then 1-5 Hell (values 11-15) for Expansion
		// => Acts 1-4 Normal (values 1-4), then 1-4 NM (values 5-8), then 1-4 Hell  (values 9-12) for Classic
		uint16_t nProgression : 5;	// BIT(8-12)
		uint16_t bWeaponSwitch : 1;	// BIT(13)
		uint16_t nDONOTUSEBITS : 2; // BIT(14-15) Will not be propagated to character preview info string due to encoding with FOG_Encode14BitsToString
	};
};

// Internal management flags
enum D2ClientFlagsEx
{
	CLIENTFLAGEX_PLAYER_UNIT_ALIVE	= 0x01,
	CLIENTFLAGEX_ARENA_RELATED		= 0x04,
	CLIENTFLAGEX_HAS_SAVE_CHECKSUM	= 0x08,
	CLIENTFLAGEX_SAVE_LOADED		= 0x10,
	CLIENTFLAGEX_GAME_OWNER			= 0x20,
};

enum D2ClientsConstants
{
	CLIENTS_MAX_UPDATES = 55,
};

struct D2ClientInfoStrc
{
	D2ClientInfoStrc* pSelf;				//0x00
	int32_t dwClientId;							//0x04
	uint32_t nFlags;					//0x08
	const char* szKickMessage;				//0x0C
	int32_t unk0x10;							//0x10
	uint32_t dwLastPacketResetTick;		//0x14
	uint32_t nPacketsPerSecond;			//0x18
	uint32_t dwNewGameTick;				//0x1C
	uint32_t dwRemoveTick;				//0x20
	int32_t unk0x24;							//0x24
	int32_t unk0x28;							//0x28
	uint32_t dwHackDetectionPacketTick;	//0x2C
	uint32_t nACDataCount;				//0x30
	int32_t unk0x34;							//0x34
	int32_t unk0x38;							//0x38
	uint32_t unk0x3C;					//0x3C
};

struct D2ClientPlayerDataStrc
{
	uint32_t dwInactivityTime;					//0x00
	uint16_t nHitPoints;						//0x04
	uint16_t nManaPoints;						//0x06
	uint16_t nStaminaPoints;					//0x08
	uint8_t nPotionLifePercent;					//0x0A
	uint8_t nPotionManaPercent;					//0x0B
	uint16_t nPosX;								//0x0C
	uint16_t nPosY;								//0x0E
	uint16_t nTargetOffsetX;					//0x10
	uint16_t nTargetOffsetY;					//0x12
	uint32_t dwBeltGold;						//0x14
	uint32_t dwExperience;						//0x18
};

struct D2ClientUnitUpdateSortStrc
{
	D2UnitStrc* pUnit;							//0x00
	int32_t nDistance;							//0x04
	int32_t nNextIndex;							//0x08
};

struct D2ClientKeySkillStrc
{
	int16_t nSkill;								//0x00
	int16_t nHand;								//0x02
	uint32_t nItemGUID;							//0x04
};

struct D2GuildInformationStrc
{
	int16_t nGuildFlags;						//0x00
	uint32_t szGuildTag;						//0x02
	char szGuildName[28];						//0x06
	uint8_t nBackgroundColor;					//0x22
	uint8_t nForegroundColor;					//0x23
	uint8_t nEmblemType;						//0x24
};

struct D2ClientStrc
{
	uint32_t dwClientId;						//0x000
	D2ClientState dwClientState;				//0x004
	uint8_t nClassId;							//0x008
	uint8_t unk0x09;							//0x009
	D2PackedClientSaveFlags tSaveFlags;			//0x00A
	uint8_t nCharTemplate;						//0x00C
	char szName[16];							//0x00D
	char szAccount[16];							//0x01D
	uint8_t unk0x2D[51];						//0x02D
	int32_t nCharSaveTransactionToken;			//0x060
	int32_t unk0x64;							//0x064
	D2ClientInfoStrc* pClientInfo;				//0x068
	D2CharacterPreviewInfoStrc tCharacterInfo;	//0x06C
	uint8_t unk0x92[218];						//0x092
	uint32_t dwUnitType;						//0x16C
	D2UnitGUID dwUnitGUID;						//0x170
	D2UnitStrc* pPlayer;						//0x174
	uint32_t bUnlockCharacter;					//0x178
	D2SaveHeaderStrc* pSaveHeader;				//0x17C
	int32_t nSaveHeaderSize;					//0x180
	uint32_t nSaveHeaderSendDelay;				//0x184
	uint32_t nSaveHeaderDataSentBytes;			//0x188
	DWORD nSaveChecksum;						//0x18C
	FILETIME nSaveCreationTimestamp;			//0x190
	uint32_t dwDeathUnitType;					//0x198
	D2UnitGUID dwDeathUnitGUID;					//0x19C
	DWORD unk0x1A0[2];							//0x1A0
	D2GameStrc* pGame;							//0x1A8
	uint8_t nAct;								//0x1AC
	uint8_t pad0x1AD[3];						//0x1AD
	uint32_t nPlayerSyncCount;					//0x1B0
	D2ActiveRoomStrc* pRoom;					//0x1B4
	struct PacketList {
		D2PacketDataStrc* pHead;				//0x1B8
		D2PacketDataStrc* pTail;				//0x1BC
		D2PacketDataStrc* pPacketDataPool;		//0x1C0
	} tPacketDataList;
	uint32_t nSaveHeaderSendFailures;			//0x1C4
	uint32_t unk0x1C8;							//0x1C8
	D2ClientUnitUpdateStrc unitUpdate[CLIENTS_MAX_UPDATES]; //0x1CC
	uint8_t pad0x3BB;							//0x3BB
	uint32_t nUnitUpdateIndex;					//0x3BC
	int32_t aLastWarpAttemptsFrame[5];			//0x3C0
	uint32_t dwFlags;							//0x3D4 D2ClientFlagsEx
	uint32_t dwLastPacketTick;					//0x3D8
	D2ClientKeySkillStrc HotkeySkills[16];		//0x3DC
	uint8_t bSwitchWeapon;						//0x45C
	uint8_t padding0x45D;						//0x45D
	D2GuildInformationStrc tGuildInfo;			//0x45E
	uint8_t padding0x483;						//0x483
	int32_t nIronGolemItemGUID;					//0x484
	uint32_t nCreateTime;						//0x488
	D2ClientPlayerDataStrc clientPlayerData;	//0x48C
	D2ClientStrc* pNext;						//0x4A8
	D2ClientStrc* pServerNext;					//0x4AC
	D2ClientStrc* pServerNextByName;			//0x4B0
	uint32_t aPingHistory[17];					//0x4B4
	uint64_t nPingMovingAverage;				//0x4F8
	uint32_t dwPingsCount;						//0x500
#ifdef D2_VERSION_113C
	uint32_t unk0x504;							//0x504
#elif D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 14
	uint32_t bDisconnectRequested;				//0x504 set by the client's disconnect request; the client timeout check then disconnects the client
#endif
	int32_t nExpLoss;							//0x504 (0x508 in 1.13c / 1.14d)
	uint32_t dwLocale;							//0x508 (0x50C in 1.13c / 1.14d)
	uint32_t dwLangId;							//0x50C (0x510 in 1.13c / 1.14d)
#if defined(D2_VERSION_113C) || (D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 14)
	uint32_t pad0x514;							//0x514
#endif
};

#pragma pack()

//D2Game.0x6FC31CD0
//1.14d: 0x005376C0
void __fastcall CLIENTS_Initialize();
//D2Game.0x6FC31D50
//1.14d: 0x00537730
void __fastcall CLIENTS_Release();
//D2Game.0x6FC31D80
//1.14d: 0x005377B0
int32_t __stdcall CLIENTS_GetExpansionClientCount();
//D2Game.0x6FC31DE0
//1.14d: 0x00537810
D2ClientStrc* __fastcall CLIENTS_GetClientFromClientId(D2GameStrc* pGame, int32_t nClientId);
//D2Game.0x6FC31E20
//1.14d: 0x00537860
D2UnitStrc* __stdcall CLIENTS_GetPlayerFromClient(D2ClientStrc* pClient, BOOL bIgnoreDeath);
//D2Game.0x6FC31EA0
//1.14d: 0x005378E0
void __fastcall CLIENTS_SetPlayerInClient(D2ClientStrc* pClient, D2UnitStrc* pUnit);
//D2Game.0x6FC31EF0
//1.14d: 0x005394A0
void __fastcall CLIENTS_SpawnPlayer(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2GameStrc* pGame, D2ActiveRoomStrc* pRoomArg, int32_t nXArg, int32_t nYArg);
//D2Game.0x6FC32220
//1.14d: 0x00537970
void __fastcall CLIENTS_SetPlayerUnitAlive(D2ClientStrc* pClient);
//D2Game.0x6FC32260
//1.14d: 0x00539760
int32_t __fastcall CLIENTS_AddPlayerToGame(D2ClientStrc* pClient, D2GameStrc* pGame, int32_t bLoadSave, D2ActiveRoomStrc* pRoomArg, int32_t nXArg, int32_t nYArg);
//1.10f: D2Game.0x6FC325E0
//1.13c: D2Game.0x6FC6A9B0
//1.14d: 0x00539A30
D2ClientStrc* __fastcall CLIENTS_AddToGame(D2GameStrc* pGame, int32_t nClientId, uint8_t nClassIdOrCharTemplate, const char* szClientName, const char* szAccount, int32_t nCharSaveTransactionToken, uint32_t nLocale, int32_t nUnused1, int32_t nUnused2);
//D2Game.0x6FC327E0
//1.14d: 0x005379A0
void __fastcall CLIENTS_SetGameData(D2GameStrc* pGame);
//D2Game.0x6FC32810
//1.14d: 0x00539BE0
void __fastcall CLIENTS_FillCharacterPreviewInfo(D2ClientStrc* pClient, D2CharacterPreviewInfoStrc* pCharacterPreviewInfo);
//D2Game.0x6FC32A30
//1.14d: 0x00539DA0
void __fastcall CLIENTS_RemoveClientFromGame(D2GameStrc* pGame, int32_t nClientIdToRemove, BOOL bTriggerSave);
//D2Game.0x6FC32FE0
//1.14d: 0x00537AC0
void __fastcall CLIENTS_FreeClientsFromGame(D2GameStrc* pGame);
//D2Game.0x6FC33020
//1.14d: 0x00537B50
void __fastcall CLIENTS_ChangeRoom(D2ClientStrc* pClient, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC33210
//1.14d: 0x00537FD0
void __fastcall CLIENTS_RefreshUnitsUpdateList(D2ClientStrc* pClient, uint32_t nUpdateSize);
//D2Game.0x6FC33510
//1.14d: 0x0045ADF0
int32_t __fastcall CLIENTS_GetUnitX(D2UnitStrc* pUnit);
//D2Game.0x6FC33540
//1.14d: 0x0045AE20
int32_t __fastcall CLIENTS_GetUnitY(D2UnitStrc* pUnit);
//D2Game.0x6FC33570
//1.14d: 0x00537EE0
void __fastcall CLIENTS_SetUnitsUpdateList(D2ClientStrc* pClient, D2ClientUnitUpdateSortStrc* pSort);
//D2Game.0x6FC33670
//1.14d: 0x005380D0
void __fastcall CLIENTS_SynchronizePlayer(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC337B0
//1.14d: 0x005381C0
int32_t __fastcall CLIENTS_IsInGame(D2GameStrc* pGame, int32_t nClientId);
//D2Game.0x6FC337E0
//1.14d: 0x005381F0
void __fastcall CLIENTS_SetRoomInClient(D2ClientStrc* pClient, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC33830
//1.14d: 0x00538260
void __fastcall D2GAME_SetClientDead_6FC33830(D2ClientStrc* pClient, D2UnitStrc* pUnit);
//D2Game.0x6FC33890
//1.14d: 0x005382B0
uint8_t __fastcall CLIENTS_GetActNo(D2ClientStrc* pClient);
//D2Game.0x6FC338C0
//1.14d: 0x005382E0
void __fastcall CLIENTS_SetActNo(D2ClientStrc* pClient, uint8_t nAct);
//D2Game.0x6FC33910
//1.14d: 0x00538320
void __fastcall CLIENTS_EnableCharacterUnlock(D2ClientStrc* pClient);
//D2Game.0x6FC33940
//1.14d: 0x00538350
uint32_t __fastcall D2GAME_GetSaveChecksumFromClient_6FC33940(D2ClientStrc* pClient);
//D2Game.0x6FC33970
//1.14d: 0x00538380
void __fastcall D2GAME_SetSaveFileChecksum_6FC33970(D2ClientStrc* pClient, uint32_t nSaveChecksum);
//D2Game.0x6FC339A0
//1.14d: 0x00538590
int32_t __fastcall CLIENTS_GetClientId(D2ClientStrc* pClient);
//D2Game.0x6FC339E0
//1.14d: 0x005385F0
int32_t __fastcall CLIENTS_GetClassId(D2ClientStrc* pClient);
//D2Game.0x6FC33A10
//1.14d: 0x00538620
void __fastcall CLIENTS_SetClassId(D2ClientStrc* pClient, int32_t nClass);
//D2Game.0x6FC33A20
//1.14d: 0x00538630
void __fastcall CLIENTS_SetFlags(D2ClientStrc* pClient, int32_t nFlags);
//D2Game.0x6FC33A30
//1.14d: 0x00538640
int32_t __fastcall CLIENTS_GetFlags(D2ClientStrc* pClient);
//D2Game.0x6FC33A40
//1.14d: 0x00538650
void __fastcall CLIENTS_ToggleFlag(D2ClientStrc* pClient, uint16_t nFlag, int32_t bSet);
//D2Game.0x6FC33A60
//1.14d: 0x00538670
int32_t __fastcall CLIENTS_CheckFlag(D2ClientStrc* pClient, uint16_t nFlag);
//D2Game.0x6FC33A70
//1.14d: 0x00538680
void __fastcall CLIENTS_UpdateCharacterProgression(D2ClientStrc* pClient, uint16_t nAct, uint16_t nDifficulty);
//D2Game.0x6FC33AC0
//1.14d: 0x005386D0
void __fastcall CLIENTS_SetClientState(D2ClientStrc* pClient, D2ClientState nClientState);
//D2Game.0x6FC33AF0
//1.14d: 0x00538700
void __fastcall CLIENTS_SetIronGolemItemGUID(D2ClientStrc* pClient, int32_t nIronGolemItemGUID);
//D2Game.0x6FC33B20
//1.14d: 0x00538760
void __fastcall CLIENTS_SetCreateTime(D2ClientStrc* pClient, DWORD dwCreateTime);
//D2Game.0x6FC33B50
//1.14d: 0x00538790
int32_t __fastcall CLIENTS_GetCreateTime(D2ClientStrc* pClient);
//D2Game.0x6FC33B80
//1.14d: 0x005387C0
D2GameStrc* __fastcall CLIENTS_GetGame(D2ClientStrc* pClient);
//D2Game.0x6FC33BB0
//1.14d: 0x005387F0
int32_t __fastcall CLIENTS_IsInUnitsRoom(D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FC33BE0
//1.14d: 0x00538830
char* __fastcall CLIENTS_GetName(D2ClientStrc* pClient);
//D2Game.0x6FC33C10
//1.14d: 0x005388C0
uint32_t __fastcall CLIENTS_HasArenaFlag(D2ClientStrc* pClient);
//D2Game.0x6FC33C50
//1.14d: 0x00538930
BOOL __fastcall CLIENTS_CheckState(int32_t nClientId, D2ClientState nExpectedClientState);
//D2Game.0x6FC33CD0
//1.14d: 0x005389A0
void __fastcall CLIENTS_UpdatePing(int32_t nClientId, int32_t nClientTick, int32_t nTickAdjustment);
//D2Game.0x6FC33EA0
//1.14d: 0x00538B70
int32_t __fastcall CLIENTS_GetNameById(int32_t nClientId, char* szName);
//D2Game.0x6FC33F20
//1.14d: 0x00538BF0
int32_t __fastcall CLIENTS_GetClientIdByName(const char* szName);
//D2Game.0x6FC33F90
//1.14d: 0x00538C60
int32_t __fastcall CLIENTS_GetCanonicalName(const char* szName, char* szCanonicalName);
//D2Game.0x6FC34020
//1.14d: 0x00538CE0
int32_t __fastcall CLIENTS_AttachSaveFile(int32_t nClientId, const void* pSaveData, int32_t nSize, int32_t nTotalSize, BOOL bUnlockCharacter, int32_t nUnused1, int32_t nUnused2);
//D2Game.0x6FC34170
//1.14d: 0x00538E00
D2SaveHeaderStrc* __fastcall CLIENTS_GetSaveHeader(D2ClientStrc* pClient);
//D2Game.0x6FC341A0
//1.14d: 0x00538E30
uint32_t __fastcall CLIENTS_GetSaveHeaderSize(D2ClientStrc* pClient);
//D2Game.0x6FC341D0
//1.14d: 0x00538E60
void __fastcall CLIENTS_CopySaveDataToClient(D2ClientStrc* pClient, const void* pData, uint32_t nSize);
//D2Game.0x6FC34280
//1.14d: 0x00538F10
void __fastcall CLIENTS_FreeSaveHeader(D2ClientStrc* pClient);
//D2Game.0x6FC34300
//1.14d: 0x00538F80
void __fastcall D2GAME_SetSaveLoadComplete_6FC34300(D2ClientStrc* pClient);
//D2Game.0x6FC34350
//1.14d: 0x00538FC0
int32_t __fastcall CLIENTS_HasLoadedSave(D2ClientStrc* pClient);
//D2Game.0x6FC34390
//1.14d: 0x00539000
void __fastcall CLIENT_SetGameOwner(D2ClientStrc* pClient);
//D2Game.0x6FC343D0
//1.14d: 0x00539030
uint32_t __fastcall CLIENT_IsGameOwner(D2ClientStrc* pClient);
//D2Game.0x6FC34420
//1.14d: 0x00539080
D2ClientStrc* __fastcall CLIENTS_GetNext(D2ClientStrc* pClient);
//D2Game.0x6FC34430
//1.14d: 0x005390A0
void __fastcall CLIENTS_SetSkillHotKey(D2ClientStrc* pClient, int32_t nHotkeyId, int16_t nSkillId, uint8_t nHand, int32_t nItemGUID);
//D2Game.0x6FC34460
//1.14d: 0x005390D0
void __fastcall CLIENTS_GetSkillHotKey(D2ClientStrc* pClient, int32_t nId, int32_t* pSkillId, int32_t* nHand, int32_t* nItemGUID);
//D2Game.0x6FC344A0
//1.14d: 0x00539110
int16_t __fastcall CLIENTS_GetGuildFlags(D2ClientStrc* pClient);
//D2Game.0x6FC344B0
//1.14d: 0x00539120
void __fastcall CLIENTS_SetGuildFlags(D2ClientStrc* pClient, int16_t nFlags);
//D2Game.0x6FC344C0
//1.14d: 0x00539130
void __fastcall CLIENTS_GetGuildTag(D2ClientStrc* pClient, int32_t* pTag);
//D2Game.0x6FC344D0
//1.14d: 0x00539140
void __fastcall CLIENTS_SetGuildTag(D2ClientStrc* pClient, int32_t* pTag);
//D2Game.0x6FC344E0
//1.14d: 0x00539150
void __fastcall CLIENTS_GetGuildName(D2ClientStrc* pClient, char* szName);
//D2Game.0x6FC34500
//1.14d: 0x00539170
void __fastcall CLIENTS_SetGuildName(D2ClientStrc* pClient, char* szName);
//D2Game.0x6FC34520
//1.14d: 0x00539190
void __fastcall CLIENTS_GetGuildEmblem(D2ClientStrc* pClient, uint8_t* pBackgroundColor, uint8_t* pForegroundColor, uint8_t* pEmblemType);
//D2Game.0x6FC34550
//1.14d: 0x005391C0
void __fastcall CLIENTS_SetGuildEmblem(D2ClientStrc* pClient, uint8_t nBackgroundColor, uint8_t nForegroundColor, uint8_t nEmblemType);
//D2Game.0x6FC34570
//1.14d: 0x005391E0
void __fastcall CLIENTS_SetExperienceLoss(D2ClientStrc* pClient, int32_t nExpLoss);
//D2Game.0x6FC345A0
//1.14d: 0x00539210
int32_t __fastcall CLIENTS_GetExperienceLoss(D2ClientStrc* pClient);
//D2Game.0x6FC345B0
//1.14d: 0x00539220
uint8_t __fastcall CLIENTS_GetWeaponSwitch(D2ClientStrc* pClient);
//D2Game.0x6FC345C0
//1.14d: 0x00539230
void __fastcall CLIENTS_SetWeaponSwitch(D2ClientStrc* pClient, uint8_t bSwitchWeapon);
//D2Game.0x6FC345D0
//1.14d: 0x00539240
void __fastcall CLIENTS_PacketDataList_Append(D2ClientStrc* pClient, D2PacketDataStrc* pPacketData);
//D2Game.0x6FC34600
//1.14d: 0x00539270
void __fastcall CLIENTS_PacketDataList_Reset(D2ClientStrc* pClient, D2PacketDataStrc* pSinglePacketData);
//D2Game.0x6FC34630
//1.14d: 0x005392A0
D2PacketDataStrc* __fastcall CLIENTS_PacketDataList_PopHead(D2ClientStrc* pClient);
//D2Game.0x6FC34670
//1.14d: 0x005392D0
D2PacketDataStrc* __fastcall CLIENTS_PacketDataList_GetTail(D2ClientStrc* pClient);
//D2Game.0x6FC34680
//1.14d: 0x005392E0
D2PacketDataStrc* __fastcall CLIENTS_PacketDataList_GetHead(D2ClientStrc* pClient);
//D2Game.0x6FC34690
//1.14d: 0x005392F0
void __fastcall CLIENTS_CopyAccountNameToBuffer(D2ClientStrc* pClient, char* szAccount);
//D2Game.0x6FC346A0
//1.14d: 0x00539300
void __fastcall D2GAME_GetCharSaveTransactionToken_6FC346A0(D2ClientStrc* pClient, int32_t* pCharSaveTransactionToken);
//D2Game.0x6FC346B0
//1.14d: 0x00539310
void __fastcall CLIENTS_GetClientInfo(D2ClientStrc* pClient, D2ClientInfoStrc** ppClientInfo);
//D2Game.0x6FC346C0
//1.14d: 0x00539320
void __fastcall CLIENTS_SetClientInfo(D2ClientStrc* pClient, D2ClientInfoStrc* pClientInfo);
//D2Game.0x6FC346D0
//1.14d: 0x00539330
D2ClientPlayerDataStrc* __fastcall CLIENTS_GetClientPlayerData(D2ClientStrc* pClient);
//D2Game.0x6FC34700
//1.14d: 0x00539360
void __fastcall CLIENTS_NotifyWarpAttempt(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC347A0
//1.14d: 0x005393F0
BOOL __fastcall CLIENTS_ShouldDelayWarpAttempt(D2GameStrc* pGame, D2UnitStrc* pUnit);
