#pragma once

#include <D2BitManip.h>

#include <Units/Units.h>
#include <GAME/Game.h>
#include <GAME/Clients.h>
#include <OBJECTS/Objects.h>
#include <OBJECTS/ObjMode.h>

#pragma pack(1)

enum D2QuestStateFlagIds
{
	QUESTSTATEFLAG_A1Q0 = 0,
	QUESTSTATEFLAG_A1Q1 = 1,
	QUESTSTATEFLAG_A1Q2 = 2,
	QUESTSTATEFLAG_A1Q3 = 3,
	QUESTSTATEFLAG_A1Q4 = 4,
	QUESTSTATEFLAG_A1Q5 = 5,
	QUESTSTATEFLAG_A1Q6 = 6,

	QUESTSTATEFLAG_A1COMPLETED = 7,

	QUESTSTATEFLAG_A2Q0 = 8,
	QUESTSTATEFLAG_A2Q1 = 9,
	QUESTSTATEFLAG_A2Q2 = 10,
	QUESTSTATEFLAG_A2Q3 = 11,
	QUESTSTATEFLAG_A2Q4 = 12,
	QUESTSTATEFLAG_A2Q5 = 13,
	QUESTSTATEFLAG_A2Q6 = 14,

	QUESTSTATEFLAG_A2COMPLETED = 15,

	QUESTSTATEFLAG_A3Q0 = 16,
	QUESTSTATEFLAG_A3Q1 = 17,
	QUESTSTATEFLAG_A3Q2 = 18,
	QUESTSTATEFLAG_A3Q3 = 19,
	QUESTSTATEFLAG_A3Q4 = 20,
	QUESTSTATEFLAG_A3Q5 = 21,
	QUESTSTATEFLAG_A3Q6 = 22,

	QUESTSTATEFLAG_A3COMPLETED = 23,

	QUESTSTATEFLAG_A4Q0 = 24,
	QUESTSTATEFLAG_A4Q1 = 25,
	QUESTSTATEFLAG_A4Q2 = 26,
	QUESTSTATEFLAG_A4Q3 = 27,

	QUESTSTATEFLAG_A4COMPLETED = 28,

	QUESTSTATEFLAG_A1Q7 = 29,

	QUESTSTATEFLAG_A2Q7 = 30,
	QUESTSTATEFLAG_A2Q8 = 31,

	QUESTSTATEFLAG_A3Q7 = 32,

	QUESTSTATEFLAG_A4Q4 = 33,

	//Unused: 34

	QUESTSTATEFLAG_A5Q1 = 35,
	QUESTSTATEFLAG_A5Q2 = 36,
	QUESTSTATEFLAG_A5Q3 = 37,
	QUESTSTATEFLAG_A5Q4 = 38,
	QUESTSTATEFLAG_A5Q5 = 39,
	QUESTSTATEFLAG_A5Q6 = 40,

	QUESTSTATEFLAG_A1INTRO = 41,
	QUESTSTATEFLAG_A2INTRO = 41,
	QUESTSTATEFLAG_A3INTRO = 41,
	QUESTSTATEFLAG_A5INTRO = 41,
};

enum D2Quests
{
	QUEST_A1Q0_WARRIVGOSSIP = 0,
	QUEST_A1Q1_DENOFEVIL = 1,
	QUEST_A1Q2_BLOODRAVEN = 2,
	QUEST_A1Q3_MALUS = 3,
	QUEST_A1Q4_CAIN = 4,
	QUEST_A1Q5_COUNTESS = 5,
	QUEST_A1Q6_ANDARIEL = 6,

	QUEST_A2Q0_JERHYNGOSSIP = 7,
	QUEST_A2Q1_RADAMENT = 8,
	QUEST_A2Q2_HORADRICSTAFF = 9,
	QUEST_A2Q3_TAINTEDSUN = 10,
	QUEST_A2Q4_HORAZONTOME = 11,
	QUEST_A2Q5_SUMMONER = 12,
	QUEST_A2Q6_DURIEL = 13,

	QUEST_A3Q0_HRATLIGOSSIP = 14,
	QUEST_A3Q1_LAMESENTOME = 15,
	QUEST_A3Q2_KHALIMFLAIL = 16,
	QUEST_A3Q3_GIDBINN = 17,
	QUEST_A3Q4_GOLDENBIRD = 18,
	QUEST_A3Q5_TRAVINCAL = 19,
	QUEST_A3Q6_MEPHISTO = 20,

	QUEST_A4Q0_TYRAELGOSSIP = 21,
	QUEST_A4Q1_IZUAL = 22,
	QUEST_A4Q2_DIABLO = 23,
	QUEST_A4Q3_HELLFORGE = 24,

	QUEST_A1Q1EX_NAVI = 25,

	QUEST_A2Q7_UNUSED_GUARDGOSSIP = 26,
	QUEST_A2Q8_UNUSED_GUARDGOSSIP = 27,

	QUEST_A3Q7_DARKWANDERER = 28,

	QUEST_A4Q4_MALACHAI = 29,

	//TODO: 30 unused?

	QUEST_A5Q1_SHENK = 31,
	QUEST_A5Q2_RESCUESOLDIERS = 32,
	QUEST_A5Q3_PRISONOFICE = 33,
	QUEST_A5Q4_NIHLATHAK = 34,
	QUEST_A5Q5_ANCIENTS = 35,
	QUEST_A5Q6_BAAL = 36
};

// Original work from @Necrolis https://d2mods.info/forum/viewtopic.php?p=412899#p412899

struct D2QuestArgStrc
{
	D2GameStrc* pGame;							//0x00
	int32_t nEvent;								//0x04
	D2UnitStrc* pTarget;						//0x08
	D2UnitStrc* pPlayer;						//0x0C
	uint32_t unk0x10;							//0x10
	union
	{
		struct
		{
			D2TextHeaderStrc* pTextControl;		//0x14
			uint32_t dw18;						//0x18
		};
		struct
		{
			int16_t nNPCNo;						//0x14
			uint16_t unk0x16;					//0x16
			int16_t nMessageIndex;				//0x18
			uint16_t unk0x1A;					//0x1A
		};
		struct
		{
			int32_t nOldLevel;					//0x14
			int32_t nNewLevel;					//0x18
		};
	};
};

struct D2QuestChainStrc
{
	struct D2QuestDataStrc* pQuestData;				//0x00
	D2QuestChainStrc* pNext;					//0x04
};

typedef void(__fastcall* QUESTINIT)(D2QuestDataStrc*);
typedef void(__fastcall* QUESTCALLBACK)(D2QuestDataStrc*, D2QuestArgStrc*);
typedef bool(__fastcall* QUESTSTATUS)(D2QuestDataStrc*, D2UnitStrc*, D2BitBufferStrc*, D2BitBufferStrc*, uint8_t*);
typedef bool(__fastcall* QUESTUPDATE)(D2GameStrc*, D2QuestDataStrc*);
typedef bool(__fastcall* QUESTACTIVE)(D2QuestDataStrc*, int32_t, D2UnitStrc*, D2BitBufferStrc*, D2UnitStrc*);
typedef int32_t(__fastcall* QUESTSEQ)(D2QuestDataStrc*, D2UnitStrc*, D2BitBufferStrc*, D2BitBufferStrc*, uint8_t*);
typedef bool(__fastcall* QUESTSEQFILTER)(D2QuestDataStrc*);

struct D2QuestGUIDStrc						//sizeof 0x84
{
	uint32_t nPlayerGUIDs[32];				//0x00 - players that have entered the quest zone
	uint16_t nPlayerCount;					//0x80
	uint8_t pad0x82[2];						//0x82
};

struct D2NPCMessageStrc
{
	int32_t nNPCNo;							//0x00
	int16_t nStringIndex;					//0x04
	int16_t nPad;							//0x06
	BOOL nMenu;								//0x08
};

struct D2NPCMessageTableStrc
{
	D2NPCMessageStrc pMessages[16];			//0x00
	int32_t nMessages;						//0xC0
};

struct D2QuestDataStrc
{
	int32_t nQuestNo;						//0x00 - internal
	D2GameStrc* pGame;						//0x04
	char nActNo;							//0x08
	bool bNotIntro;							//0x09 - set to false for intro quests, it could also be for already completed...
	bool bActive;							//0x0A - confirmed
	uint8_t fLastState;						//0x0B - previous quest state
	uint8_t fState;							//0x0C - main quest state
	char nInitNo;							//0x0D
	uint16_t dw0E;							//0x0E 
	int32_t nSeqId;							//0x10 - nInitNo
	uint32_t dwFlags;						//0x14
	void* pQuestDataEx;						//0x18 - union of 0x29 structs
	D2QuestGUIDStrc tPlayerGUIDs;			//0x1C
	uint16_t dw9E;							//0x9E 
	QUESTCALLBACK pfCallback[15];			//0xA0
	D2NPCMessageTableStrc* pNPCMessages;	//0xDC
	int32_t nQuest;							//0xE0 - index in quest flag bit array
	QUESTSTATUS pfStatusFilter;				//0xE4
	QUESTACTIVE pfActiveFilter;				//0xE8
	QUESTSEQFILTER pfSeqFilter;				//0xEC
	D2QuestDataStrc* pPrev;					//0xF0
};

struct D2QuestInitTableStrc
{
	QUESTINIT pfInit;						//0x00
	uint8_t nAct;							//0x04
	uint8_t pad0x05[3];						//0x05
	uint32_t nVersion;						//0x08
	bool bNoSetState;						//0x0C - used by the sequences for quest init flags
	uint8_t pad0x0D[3];						//0x0D
	int32_t nChainNo;						//0x10 - quest data internal chain id
	uint32_t nQuestNo;						//0x14 - pQuestData flag no
};

struct D2QuestIntroTableStrc
{
	QUESTINIT pfInit;						//0x00
	uint8_t nAct;							//0x04
	uint8_t pad0x05[3];						//0x05
};

struct D2QuestTimerStrc
{
	QUESTUPDATE pfUpdate;					//0x00
	D2QuestDataStrc* pQuest;				//0x04
	uint32_t dwTicks;						//0x08
	uint32_t dwTimeout;						//0x0C
	D2QuestTimerStrc* pNext;				//0x10
};

struct D2QuestInfoStrc
{
	D2QuestDataStrc* pLastQuest;			//0x00
	BOOL bExecuting;						//0x04
	BOOL bPickedSet;						//0x08
	D2BitBufferStrc* pQuestFlags;			//0x0C
	D2QuestTimerStrc* pTimer;				//0x10
	uint32_t dwTick;						//0x14
	D2SeedStrc pSeed;						//0x18
	uint8_t unk0x20;						//0x20
	uint8_t unk0x21[3];						//0x21
};

#pragma pack()

//D2Game.0x6FC93B60
void __fastcall QUESTS_FreeChainRecord(D2GameStrc* pGame, D2QuestChainStrc* pRecord);
//D2Game.0x6FC93B90
D2QuestDataStrc* __fastcall QUESTS_GetQuestData(D2GameStrc* pGame, int32_t nId);
//D2Game.0x6FC93BD0
void __fastcall QUESTS_AttachLevelChainRecord(D2GameStrc* pGame, D2UnitStrc* pUnit, D2RoomStrc* pRoom, int32_t bDebug);
//D2Game.0x6FC93C40
int32_t __fastcall QUESTS_CreateChainRecord(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nQuestId);
//D2Game.0x6FC93D60
void __fastcall QUESTS_InitScrollTextChain(D2QuestDataStrc* pQuest, D2TextHeaderStrc* pTextControl, int32_t nNPCID, int32_t nIndex);
//D2Game.0x6FC93DC0
void __fastcall QUESTS_QuestInit(D2GameStrc* pGame);
//D2Game.0x6FC93FD0
void __fastcall QUESTS_QuestFree(D2GameStrc* pGame);
//D2Game.0x6FC94080
D2SeedStrc* __fastcall QUESTS_GetGlobalSeed(D2GameStrc* pGame);
//D2Game.0x6FC940B0
void __fastcall QUESTS_ParseKill(D2GameStrc* pGame, D2UnitStrc* pDefender, D2UnitStrc* pAttacker);
//D2Game.0x6FC94210
void __fastcall QUESTS_StatusCycler(D2QuestArgStrc* pArgs, bool bForceActive);
//D2Game.0x6FC94290
void __fastcall QUESTS_ChangeLevel(D2GameStrc* pGame, int32_t nOldLevelId, int32_t nTargetLevelId, D2UnitStrc* pUnit);
//D2Game.0x6FC942D0
void __fastcall QUESTS_EventCallback(D2QuestArgStrc* pArgs, bool bCheckActive, bool bCheckAct);
//D2Game.0x6FC94390
void __fastcall QUESTS_PlayerDroppedWithQuestItem(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC944B0
void __fastcall QUESTS_NPCActivate(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pTarget, D2TextHeaderStrc* pTextControl);
//D2Game.0x6FC944F0
void __fastcall QUESTS_NPCDeactivate(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNPC);
//D2Game.0x6FC94520
void __fastcall QUESTS_ItemPickedUp(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC94550
void __fastcall QUESTS_ItemDropped(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC94580
void __fastcall QUESTS_QuestUpdater(D2GameStrc* pGame);
//D2Game.0x6FC94690
void __fastcall QUESTS_CreateTimer(D2QuestDataStrc* pQuest, QUESTUPDATE pfnCallback, int32_t nTicks);
//D2Game.0x6FC94710
void __fastcall QUESTS_RefreshStatus(D2QuestDataStrc* pQuest, uint8_t* pQuestList, D2BitBufferStrc* pQuestFlags);
//D2Game.0x6FC947F0
void __fastcall QUESTS_StatusCallback(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC94A30
int32_t __fastcall QUESTS_DebugOutput(D2GameStrc* pGame, const char* szMessage, const char* szFile, int32_t nLine);
//D2Game.0x6FC94A50
void __fastcall QUESTS_DeleteItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint32_t dwItemCode);
//D2Game.0x6FC94B20
void __fastcall QUESTS_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t nQuestId);
//D2Game.0x6FC94CA0
void __fastcall QUESTS_UnitIterate(D2QuestDataStrc* pQuest, int32_t nIterateState, D2UnitStrc* pUnit, int32_t(__fastcall* pfIterate)(D2GameStrc*, D2UnitStrc*, void*), bool bIterate);
//D2Game.0x6FC94CF0
int32_t __fastcall QUESTS_StateDebug(D2QuestDataStrc* pQuestData, int32_t nState, const char* szFile, int32_t nLine);
//D2Game.0x6FC94D40
void __fastcall QUESTS_NPCMessage(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t dwNPCGUID, uint16_t nMessage);
//D2Game.0x6FC94DB0
void __fastcall QUESTS_SequenceCycler(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t bGameEnter);
//D2Game.0x6FC95360 (#10037)
int32_t __stdcall QUESTS_CheckFirstPickedSet(D2GameStrc* pGame);
//D2Game.0x6FC953A0 (#10038)
int32_t __stdcall QUESTS_CheckNotIntroQuest(D2GameStrc* pGame, int32_t nQuestId);
//D2Game.0x6FC95430
void __fastcall QUESTS_SendCurrentFlags(D2GameStrc* pGame, D2ClientStrc* pClient);
//D2Game.0x6FC95490
int32_t __fastcall QUESTS_ActiveCycler(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNPC);
//D2Game.0x6FC956C0
void __fastcall QUESTS_SetGlobalState(D2GameStrc* pGame, int32_t nQuestId, int32_t nState);
//D2Game.0x6FC95700
int32_t __fastcall QUESTS_GetGlobalState(D2GameStrc* pGame, int32_t nQuestId, int32_t nState);
//D2Game.0x6FC95740
void __fastcall QUESTS_WarrivSpawn(D2GameStrc* pGame, D2UnitStrc* pWarriv, int32_t nXpos, int32_t nYpos, int32_t nWarrivID);
//D2Game.0x6FC95790
void __fastcall QUESTS_UseClueItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUnitId);
//D2Game.0x6FC95800
void __fastcall OBJECTS_InitFunction18_JerhynPosition(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC95830
void __fastcall OBJECTS_InitFunction19_JerhynPositionEx(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC95860
void __fastcall OBJECTS_InitFunction20_TaintedAltar(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC958A0
void __fastcall OBJECTS_InitFunction15_MalusStand(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC958E0
void __fastcall OBJECTS_InitFunction07_CainGibbet(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC95920 (#10035)
void __fastcall QUESTS_DisableSequences();
//D2Game.0x6FC95930
void __fastcall QUESTS_ObjectEvents(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC95CC0
void __fastcall OBJECTS_InitFunction23_LamEsenTomeStand(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC95D10
void __fastcall QUESTS_SetJadeFigurineBoss(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC95DB0
void __fastcall QUESTS_SetGoldenBirdSpawn(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC95DF0
D2UnitStrc* __fastcall QUESTS_CreateItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint32_t dwCode, int32_t nLevel, uint8_t nQuality, int32_t bDroppable);
//D2Game.0x6FC95F30
void __fastcall QUESTS_UpdateNPCIntros(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nAct);
//D2Game.0x6FC960C0
void __fastcall QUESTS_ActChange_HirelingChangeAct(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pVendor, uint32_t dwDestLvl, uint32_t unk);
//D2Game.0x6FC964B0
void __fastcall QUESTS_DeleteItemEx(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Game.0x6FC96580
void __fastcall QUESTS_HellPortalActChange(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nLevel, D2UnitStrc* pObject);
//D2Game.0x6FC967D0
void __fastcall QUESTS_SetIntroFlags(D2GameStrc* pGame, D2UnitStrc* pPlayer, const int32_t* pNPCs, int32_t nNPCs);
//D2Game.0x6FC96810
void __fastcall QUESTS_AddPlayerGUID(D2QuestGUIDStrc* pQuestGUID, int32_t dwGUID);
//D2Game.0x6FC96840
int32_t __fastcall QUESTS_FastRemovePlayerGUID(D2QuestGUIDStrc* pGUIDs, int32_t dwGUID);
//D2Game.0x6FC96880
int32_t __fastcall QUESTS_QuickCheckPlayerGUID(D2QuestGUIDStrc* pGUIDs, int32_t dwGUID);
//D2Game.0x6FC968B0
int32_t __fastcall QUESTS_CheckPlayerGUID(D2QuestDataStrc* pQuest, int32_t dwGUID);
//D2Game.0x6FC968E0
void __fastcall QUESTS_ResetPlayerGUIDCount(D2QuestGUIDStrc* pGUIDs);
//D2Game.0x6FC968F0
void __fastcall QUESTS_QuickRemovePlayerGUID(D2QuestDataStrc* pQuest, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC96940
void __fastcall QUESTS_GetFreePosition(D2RoomStrc* pRoom, D2CoordStrc* pCoord, uint32_t nSize, uint16_t fCollision, D2RoomStrc** ppRoom, int32_t nRadius);
//D2Game.0x6FC96C80
void __fastcall QUESTS_RemovePlayerGUID(D2QuestDataStrc* pQuest, D2QuestArgStrc* pArgs);
//D2Game.0x6FC96D20
void __fastcall QUESTS_UpdatePlayerFlags(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC96D60
void __fastcall QUESTS_GUIDUpdate(D2GameStrc* pGame, D2QuestGUIDStrc* pQuestGUID, int32_t nQuest, uint16_t wSoundCMD);
//D2Game.0x6FC96E20
void __fastcall QUESTS_SendScrollMessage(D2UnitStrc* pPlayer, D2UnitStrc* pUnit, uint16_t nMessage);
//D2Game.0x6FC96E80
void __fastcall QUESTS_TriggerFX(D2GameStrc* pGame, uint8_t a2);
//D2Game.0x6FC96EA0
int32_t __fastcall QUESTS_FXIterate(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg);
//D2Game.0x6FC96F10
void __fastcall QUESTS_NPCActivateSpeeches(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pTarget);
//D2Game.0x6FC97020
int32_t __fastcall QUESTS_PortalCheck(D2GameStrc* pGame, D2CoordStrc* pCoord, int32_t nLevel, D2RoomStrc** ppRoom);
//D2Game.0x6FC97040
int32_t __fastcall QUESTS_SetObjectSelection(D2ObjOperateFnStrc* pOp);
//D2Game.0x6FC970E0
void __fastcall QUESTS_SendLogUpdate(D2UnitStrc* pUnit, uint8_t nQuestId);
//D2Game.0x6FC97120
void __fastcall QUESTS_SendLogUpdateEx(D2UnitStrc* pPlayer, uint8_t nQuestId, uint8_t nAct);
//D2Game.0x6FC97190
D2UnitStrc* __fastcall QUESTS_SpawnCriticalMonster(D2GameStrc* pGame, int32_t nXpos, int32_t nYpos, D2RoomStrc* pRoom, int32_t bSpecialInit, int32_t nMonster);
//D2Game.0x6FC973D0
void __fastcall QUESTS_MonsterSpawn(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC97400
int32_t __fastcall QUESTS_LevelWarpCheck(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nSourceLevel, int32_t nTargetLevel);
//D2Game.0x6FC974B0
D2UnitStrc* __fastcall QUESTS_SpawnMonster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pPosition, int32_t nType, int32_t nIndex);
//D2Game.0x6FC975A0
void __fastcall QUESTS_MapPresetSpawn(D2GameStrc* pGame, D2UnitStrc* pUnit, D2MapAIStrc* pMapAI);
