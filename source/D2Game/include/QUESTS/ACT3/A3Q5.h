#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Council
struct D2Act3Quest5Strc						//sizeof 0x50
{
	int nLastKilledMonsterGUID;				//0x00
	uint8_t bOrmusActivated;				//0x04
	uint8_t bMonstersSpawned;				//0x05
	uint8_t pad0x06[2];						//0x06
	int bQuest1RewardGranted;				//0x08
	uint8_t bOrbSmashed;					//0x0C
	uint8_t bFlailDropped;					//0x0D
	uint8_t bCubeDropped;					//0x0E
	uint8_t pad0x0F;						//0x0F
	int nMonsterGUIDs[6];					//0x10
	uint8_t bCompellingOrbSpawned;			//0x28
	uint8_t pad0x29[3];						//0x29
	int nCompellingOrbGUID;					//0x2C
	int nSpawnCount;						//0x30
	int nMonstersLeftToKill;				//0x34
	int nHits;								//0x38
	int nFlailsMissing;						//0x3C
	int nHoradricCubesMissing;				//0x40
	uint8_t unused0x44[12];					//0x44
};

#pragma pack()



//D2Game.0x6FCAAFA0
bool __fastcall ACT3Q5_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCAB000
void __fastcall ACT3Q5_SpawnCouncil(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCAB0F0
int32_t __fastcall ACT3Q5_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAB160
int32_t __fastcall ACT3Q5_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAB1D0
void __fastcall ACT3Q5_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAB250
void __fastcall ACT3Q5_UnitIterate_DeleteKhalimItems(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAB380
int32_t __fastcall OBJECTS_OperateFunction53_CompellingOrb(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCAB580
void __fastcall OBJECTS_InitFunction53_StairsR(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCAB5B0
void __fastcall OBJECTS_InitFunction60_CompellingOrb(D2ObjInitFnStrc* pOp);

//D2Game.0x6FCAB620
void __fastcall ACT3Q5_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCAB6F0
void __fastcall ACT3Q5_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAB800
void __fastcall ACT3Q5_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAB900
void __fastcall ACT3Q5_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAB960
void __fastcall ACT3Q5_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCABB50
bool __fastcall ACT3Q5_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCABBC0
void __fastcall ACT3Q5_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCABCB0
void __fastcall ACT3Q5_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCABED0
int32_t __fastcall ACT3Q5_UnitIterate_UpdateQuestStateAfterMonsterKill(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAC020
int32_t __fastcall ACT3Q5_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAC070
int32_t __fastcall ACT3Q5_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAC0D0
int32_t __fastcall ACT3Q5_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAC110
int32_t __fastcall ACT3Q5_UnitIterate_DetermineFlailAndCubeDropCounts(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAC1B0
int32_t __fastcall ACT3Q5_IsDuranceOfHateClosed(D2GameStrc* pGame, D2UnitStrc* pUnit);
//
void __fastcall ACT3Q5_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
