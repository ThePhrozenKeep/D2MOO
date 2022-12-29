#pragma once


#include "D2Structs.h"


//D2Game.0x6FCA6E80
bool __fastcall ACT3Q1_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA6F10
int32_t __fastcall OBJECTS_OperateFunction28_LamEsenTome(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA7040
int32_t __fastcall ACT3Q1_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA70B0
void __fastcall ACT3Q1_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA7190
void __fastcall ACT3Q1_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA73B0
int32_t __fastcall ACT3Q1_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA7420
int32_t __fastcall ACT3Q1_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA74A0
int32_t __fastcall ACT3Q1_UnitIterate_AddStatPointReward(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA74F0
int32_t __fastcall ACT3Q1_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA7530
void __fastcall ACT3Q1_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA7580
void __fastcall ACT3Q1_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA7670
void __fastcall ACT3Q1_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA7740
void __fastcall ACT3Q1_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA77D0
void __fastcall ACT3Q1_Callback05_ItemDropped(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA7830
void __fastcall ACT3Q1_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA7870
void __fastcall ACT3Q1_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA78D0
void __fastcall ACT3Q1_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA79A0
bool __fastcall ACT3Q1_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA7A10
bool __fastcall ACT3Q1_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//D2Game.0x6FCA7B40
void __fastcall ACT3Q1_UnitIterate_CheckForTomeItem(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//
void __fastcall ACT3Q1_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
