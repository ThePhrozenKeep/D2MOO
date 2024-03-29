#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Countess
struct D2Act1Quest5Strc						//sizeof 0x120
{
	int nUnitGUIDs1[12];					//0x00 TODO: Which UnitGUIDs?
	short nUnitCount1;						//0x30
	uint8_t pad0x32[2];						//0x32
	int nUnitGUIDs2[12];					//0x34 TODO: Which UnitGUIDs?
	short nUnitCount2;						//0x64
	uint8_t pad0x66[2];						//0x66
	int nObjectGUIDs[8];					//0x68 TODO: Which objects?
	uint16_t nObjectCount;					//0x88
	uint8_t pad0x8A[2];						//0x8A
	D2QuestGUIDStrc tPlayerGUIDs;					//0x8C
	D2CoordStrc pCoord;						//0x110
	uint8_t bCountessKilled;				//0x118
	uint8_t bDeathMissilesCreated;			//0x119
	uint8_t bTriggerSeqFilter;				//0x11A
	uint8_t bTowerTomeActivated;			//0x11B
	uint8_t unk0x11C;						//0x11C TODO
	uint8_t pad0x11D[3];					//0x11D
};

#pragma pack()



//D2Game.0x6FC9CD00
int32_t __fastcall OBJECTS_OperateFunction06_TowerTome(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC9CDA0
int32_t __fastcall ACT1Q5_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9CE00
int32_t __fastcall ACT1Q5_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9CED0
bool __fastcall ACT1Q5_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FC9CF20
void __fastcall ACT1Q5_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9CFD0
void __fastcall ACT1Q5_SpawnTowerChestMissiles(D2QuestDataStrc* pQuestData, D2Act1Quest5Strc* pQuestDataEx);
//D2Game.0x6FC9D2E0
void __fastcall ACT1Q5_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9D3B0
void __fastcall ACT1Q5_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9D5D0
int32_t __fastcall ACT1Q5_UpdateUnitGUIDLists(D2Act1Quest5Strc* pQuestDataEx, int32_t nUnitGUID, int32_t a3);
//D2Game.0x6FC9D650
void __fastcall ACT1Q5_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9D6A0
void __fastcall ACT1Q5_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9D830
void __fastcall ACT1Q5_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9D970
bool __fastcall ACT1Q5_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9D9F0
void __fastcall ACT1Q5_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9DBE0
int32_t __fastcall ACT1Q5_UnitIterate_SetRewardGranted(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9DC80
int32_t __fastcall ACT1Q5_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9DD40
int32_t __fastcall ACT1Q5_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9DD90
int32_t __fastcall ACT1Q5_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9DDE0
bool __fastcall ACT1Q5_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9DE10
void __fastcall ACT1Q5_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9DEE0
void __fastcall OBJECTS_InitFunction04_TowerTome(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9DF30
void __fastcall OBJECTS_InitFunction47_CountessChest(D2ObjInitFnStrc* pOp);
