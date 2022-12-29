#pragma once


#include "D2Structs.h"


//D2Game.0x6FCB3530
void __fastcall ACT5Q3_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB35A0
void __fastcall ACT5Q3_RemoveNihlathakFromTown(D2GameStrc* pGame);
//D2Game.0x6FCB3670
bool __fastcall ACT5Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCB3760
void __fastcall ACT5Q3_UpdateResistances(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FCB3810
void __fastcall ACT5Q3_ApplyResistanceReward(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCB3880
void __fastcall ACT5Q3_UnitIterate_CheckForDefrostPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB38B0
void __fastcall ACT5Q3_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB39A0
void __fastcall ACT5Q3_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB3A20
int32_t __fastcall ACT5Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB3A90
bool __fastcall ACT5Q3_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB3B00
void __fastcall ACT5Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB4040
int32_t __fastcall ACT5Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB40B0
int32_t __fastcall ACT5Q3_RemoveDrehyaIced(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB4150
void __fastcall ACT5Q3_SendPacket0x5DToClient(D2UnitStrc* pUnit);
//D2Game.0x6FCB4190
void __fastcall ACT5Q3_SpawnDrehyaInTown(D2QuestDataStrc* pQuestData, D2Act5Quest3Strc* pQuestDataEx);
//D2Game.0x6FCB4400
//TODO: Find a name
void __fastcall sub_6FCB4400(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB45E0
void __fastcall ACT5Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB4810
void __fastcall ACT5Q3_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB4870
void __fastcall ACT5Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB4A10
int32_t __fastcall ACT5Q3_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB4A80
void __fastcall ACT5Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB4B50
void __fastcall ACT5Q3_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB4BF0
bool __fastcall ACT5Q3_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//D2Game.0x6FCB4D30
void __fastcall OBJECTS_InitFunction66_DrehyaStartInTown(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB4EA0
void __fastcall ACT5Q3_SpawnFrozenDrehya(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCB4FB0
void __fastcall OBJECTS_InitFunction67_DrehyaStartOutsideTown(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB5010
void __fastcall OBJECTS_InitFunction68_NihlathakStartInTown(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB50C0
void __fastcall OBJECTS_InitFunction69_NihlathakStartOutsideTown(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB5130
void __fastcall ACT5Q3_UpdateDrehyaPortalMode(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit);
//D2Game.0x6FCB51C0
int32_t __fastcall ACT5Q3_SpawnDrehyaPortalOutsideTown(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCB5320
int32_t __fastcall ACT5Q3_GetDrehyaPortalCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pCoord);
//D2Game.0x6FCB5390
int32_t __fastcall ACT5Q3_InitializeDrehyaPortalCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCB53D0
//TODO: Find a name
void __fastcall sub_6FCB53D0(D2GameStrc* pGame);
//D2Game.0x6FCB5400
//TODO: Find a name
int32_t __fastcall sub_6FCB5400(D2GameStrc* pGame);
//D2Game.0x6FCB5430
//TODO: Find a name
void __fastcall sub_6FCB5430(D2GameStrc* pGame);
//D2Game.0x6FCB5470
void __fastcall OBJECTS_InitFunction74_FrozenAnya(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB54D0
int32_t __fastcall OBJECTS_OperateFunction67_FrozenAnya(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCB56D0
int32_t __fastcall ACT5Q3_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB5720
bool __fastcall ACT5Q3_SpawnDrehyaIcedMonsterOutsideTown(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB5890
void __fastcall ACT5Q3_ChangeDrehyaMapAI(D2GameStrc* pGame, D2UnitStrc* pUnit, D2MapAIStrc* pMapAi);
//D2Game.0x6FCB5920
void __fastcall ACT5Q3_ChangeNihlathakMapAI(D2GameStrc* pGame, D2UnitStrc* pUnit, D2MapAIStrc* pMapAi);
