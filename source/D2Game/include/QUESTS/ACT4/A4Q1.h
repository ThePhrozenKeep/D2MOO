#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Izual
struct D2Act4Quest1Strc				//sizeof 0x1C
{
	int unused0x00;					//0x00
	D2CoordStrc pCoords;			//0x04
	uint8_t unk0x0C;				//0x0C TODO
	uint8_t bIzualGhostIsSpawning;	//0x0D
	uint8_t unused0x0E;				//0x0E
	uint8_t unk0x0F;				//0x0F TODO
	uint8_t bNeedToSpawnIzualGhost;	//0x10
	uint8_t bTyraelActivated;		//0x11
	uint8_t bIzualGhostActivated;	//0x12
	uint8_t pad0x13;				//0x13
	D2UnitStrc* pTargetUnit;		//0x14
	uint8_t bCloseToIzualGhost;		//0x18
	uint8_t pad0x19[3];				//0x19
};

#pragma pack()



//D2Game.0x6FCAD830
bool __fastcall ACT4Q1_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCAD8B0
void __fastcall ACT4Q1_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAD930
void __fastcall ACT4Q1_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCADA00
bool __fastcall ACT4Q1_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCADA70
void __fastcall ACT4Q1_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCADC80
int32_t __fastcall ACT4Q1_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCADD00
int32_t __fastcall ACT4Q1_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pData);
//D2Game.0x6FCADD70
void __fastcall ACT4Q1_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCADEA0
void __fastcall ACT4Q1_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCADFC0
int32_t __fastcall ACT4Q1_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAE020
int32_t __fastcall ACT4Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAE070
bool __fastcall ACT4Q1_SpawnIzualGhost(D2GameStrc* pGame, D2QuestDataStrc* pQuest);
//D2Game.0x6FCAE140
int32_t __fastcall ACT4Q1_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAE250
void __fastcall ACT4Q1_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAE2E0
void __fastcall ACT4Q1_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAE330
void __fastcall ACT4Q1_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAE3C0
void __fastcall ACT4Q1_OnIzualActivated(D2GameStrc* pGame);
//D2Game.0x6FCAE400
int32_t __fastcall ACT4Q1_IsAnyPlayerInRangeOfIzualGhost(D2GameStrc* pGame, D2UnitStrc* pTargetUnit);
//D2Game.0x6FCAE450
void __fastcall ACT4Q1_UnitIterate_CheckDistanceToIzualGhost(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAE470
void __fastcall ACT4Q1_AttachCompletionSoundToPlayers(D2GameStrc* pGame);
//D2Game.0x6FCAE480
int32_t __fastcall ACT4Q1_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//
void __fastcall ACT4Q1_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
