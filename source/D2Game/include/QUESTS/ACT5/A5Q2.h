#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//RescueBarbarians
struct D2Act5Quest2Strc						//sizeof 0x160
{
	D2QuestGUIDStrc tPlayerGUIDs;					//0x00
	uint8_t bQualKehkActivated;				//0x84
	uint8_t unused0x85;						//0x85
	uint8_t bWussiesSpawned[3];				//0x86
	uint8_t pad0x89[3];						//0x89
	D2CoordStrc pWussieCoords[3];			//0x8C
	int nSpawnedWussies;					//0xA4
	int nKilledWussies;						//0xA8
	int nFreedWussies;						//0xAC
	int unk0xB0[15];						//0xB0 TODO: Some UnitGUIDs
	int nPortalGUIDs[3];					//0xEC
	uint8_t bPortalSpawned[3];				//0xF8
	uint8_t pad0xFB;						//0xFB
	int nPortalUpdateInvocations[3];		//0xFC
	uint8_t unk0x108[3];					//0x108 TODO: Some boolean
	uint8_t bChangeToSpecialObjectMode[3];	//0x10C
	uint8_t unk0x10E[3];					//0x10E TODO: Some boolean
	uint8_t pad0x111[3];					//0x111
	int unk0x114[3];						//0x114 TODO: Some Counter
	int nFreedWussieUnitGUIDs[15];			//0x120
	int nWussiesInRangeToDoor;				//0x15C
};

#pragma pack()



//D2Game.0x6FCB1F30
void __fastcall ACT5Q2_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB1FF0
bool __fastcall ACT5Q2_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCB2080
void __fastcall ACT5Q2_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB2160
void __fastcall ACT5Q2_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB21C0
int32_t __fastcall ACT5Q2_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB2230
void __fastcall ACT5Q2_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB2460
int32_t __fastcall ACT5Q2_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB24D0
void __fastcall ACT5Q2_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB2610
void __fastcall ACT5Q2_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB2650
void __fastcall ACT5Q2_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB2860
int32_t __fastcall ACT5Q2_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB28D0
void __fastcall ACT5Q2_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB2A20
bool __fastcall ACT5Q2_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB2A90
void __fastcall ACT5Q2_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB2AC0
void __fastcall ACT5Q2_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB2B60
int32_t __fastcall ACT5Q2_SpawnCagedWussies(D2QuestDataStrc* pQuestData, D2ObjInitFnStrc* pOp, int32_t nCount);
//D2Game.0x6FCB2C00
void __fastcall OBJECTS_InitFunction62_CagedWussie(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB2CF0
int32_t __fastcall ACT5Q2_CheckWussieCounters(D2GameStrc* pGame, D2UnitStrc* pWussie);
//D2Game.0x6FCB2D60
void __fastcall ACT5Q2_UpdateWussieCounters(D2GameStrc* pGame, int32_t nUnused, D2UnitStrc* pWussie);
//D2Game.0x6FCB2DE0
void __fastcall ACT5Q2_UpdateQuestState(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pWussie);
//D2Game.0x6FCB3240
int32_t __fastcall ACT5Q2_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB3290
int32_t __fastcall ACT5Q2_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB32D0
int32_t __fastcall ACT5Q2_GetBarbsToBeRescued(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB3310
void __fastcall ACT5Q2_UpdatePortalMode(D2QuestDataStrc* pQuestData, D2UnitStrc* pPortal);
//D2Game.0x6FCB33B0
int32_t __fastcall ACT5Q2_FindPortal(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc** ppPortal);
//D2Game.0x6FCB3440
void __fastcall ACT5Q2_OnWussieActivated(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCB3490
int32_t __fastcall ACT5Q2_IsPrisonDoorDead(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pWussie);
