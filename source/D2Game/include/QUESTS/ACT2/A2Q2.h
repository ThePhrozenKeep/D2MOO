#pragma once


#include "D2Structs.h"


//D2Game.0x6FCA01F0
bool __fastcall ACT2Q2_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA0240
bool __fastcall ACT2Q2_CheckItemsAndState(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit, uint8_t* pData);
//D2Game.0x6FCA0370
void __fastcall ACT2Q2_UpdateHoradricItemCounts(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA0490
bool __fastcall ACT2Q2_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//D2Game.0x6FCA06C0
void __fastcall ACT2Q2_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA07A0
void __fastcall ACT2Q2_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA08C0
void __fastcall ACT2Q2_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA09C0
void __fastcall ACT2Q2_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA09D0
void __fastcall ACT2Q2_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA0CF0
int32_t __fastcall ACT2Q2_IsQuestDone(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA0D50
void __fastcall ACT2Q2_Callback05_ItemDropped(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA0DF0
void __fastcall ACT2Q2_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA0F10
void __fastcall ACT2Q2_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA0FC0
void __fastcall ACT2Q2_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA10B0
int32_t __fastcall OBJECTS_OperateFunction40_HoradricScrollChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA1180
int32_t __fastcall ACT2Q2_UnitIterate_DetermineHoradricScrollDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA11D0
int32_t __fastcall OBJECTS_OperateFunction41_StaffOfKingsChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA12C0
int32_t __fastcall ACT2Q2_UnitIterate_DetermineStaffDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA1330
int32_t __fastcall OBJECTS_OperateFunction39_HoradricCubeChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA1410
int32_t __fastcall ACT2Q2_UnitIterate_DetermineCubeDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//
void __fastcall ACT2Q2_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
