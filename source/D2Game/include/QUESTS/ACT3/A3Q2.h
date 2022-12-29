#pragma once


#include "D2Structs.h"


//D2Game.0x6FCA7B70
bool __fastcall ACT3Q2_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA7CD0
void __fastcall ACT3Q2_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA7DA0
void __fastcall ACT3Q2_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA7FB0
void __fastcall ACT3Q2_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA7FE0
bool __fastcall ACT3Q2_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//D2Game.0x6FCA81E0
void __fastcall ACT3Q2_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA82D0
void __fastcall ACT3Q2_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA8310
int32_t __fastcall ACT3Q2_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA8360
bool __fastcall ACT3Q2_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA83D0
void __fastcall ACT3Q2_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA8780
void __fastcall ACT3Q2_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA87F0
int32_t __fastcall OBJECTS_OperateFunction44_SewerStairs(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA8810
int32_t __fastcall OBJECTS_OperateFunction45_SewerLever(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA8940
void __fastcall OBJECTS_InitFunction41_SewerStairs(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA8990
void __fastcall OBJECTS_InitFunction42_SewerLever(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA89C0
void __fastcall ACT3Q2_UpdateKhalimItemCounts(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA8A30
int32_t __fastcall OBJECTS_OperateFunction57_KhalimChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA8B10
int32_t __fastcall ACT3Q2_UnitIterate_DetermineKhalimHeartDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA8B60
int32_t __fastcall OBJECTS_OperateFunction58_KhalimChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA8C40
int32_t __fastcall ACT3Q2_UnitIterate_DetermineKhalimEyeDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA8C90
int32_t __fastcall OBJECTS_OperateFunction59_KhalimChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA8D70
int32_t __fastcall ACT3Q2_UnitIterate_DetermineKhalimBrainDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
