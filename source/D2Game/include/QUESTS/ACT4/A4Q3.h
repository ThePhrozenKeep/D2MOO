#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Hellforge
struct D2Act4Quest3Strc						//sizeof 0x20
{
	short nHellforgeObjectMode;				//0x00
	uint8_t bCainActivated;					//0x02
	uint8_t bRewardDropsPending;			//0x03
	uint8_t bSoulstoneSmashed;				//0x04
	uint8_t pad0x05[3];						//0x05
	int nGemDropTier;						//0x08
	int nHits;								//0x0C
	int nHellforgeHammersInGame;			//0x10
	int nPlayersInAct;						//0x14
	int unused0x18;							//0x18
	uint8_t bSoulstoneAcquired;				//0x1C
	uint8_t pad0x1D[3];						//0x1D
};

#pragma pack()



//D2Game.0x6FCAFF50
void __fastcall OBJECTS_InitFunction48_HellForge(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCAFFC0
int32_t __fastcall ACT4Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB0030
void __fastcall ACT4Q3_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB00E0
int32_t __fastcall OBJECTS_OperateFunction49_HellForge(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCB03A0
int32_t __fastcall ACT4Q3_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB0400
bool __fastcall ACT4Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCB0460
void __fastcall ACT4Q3_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB0550
void __fastcall ACT4Q3_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB05C0
bool __fastcall ACT4Q3_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB0650
void __fastcall ACT4Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB06D0
int32_t __fastcall ACT4Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB0760
void __fastcall ACT4Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB0920
void __fastcall ACT4Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB0A80
void __fastcall ACT4Q3_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB0AE0
void __fastcall ACT4Q3_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB0B00
void __fastcall ACT4Q3_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB0B20
void __fastcall ACT4Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB0C00
void __fastcall ACT4Q3_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB0C40
void __fastcall ACT4Q3_CreateReward(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit);
//
void __fastcall ACT4Q3_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
