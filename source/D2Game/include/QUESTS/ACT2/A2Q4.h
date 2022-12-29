#pragma once


#include "D2Structs.h"


//D2Game.0x6FCA25C0
bool __fastcall ACT2Q4_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA2660
int32_t __fastcall ACT2Q4_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA26D0
int32_t __fastcall ACT2Q4_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA2780
int32_t __fastcall ACT2Q4_IsHaremBlockerObjectOpened(D2GameStrc* pGame);
//D2Game.0x6FCA27B0
void __fastcall ACT2Q4_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA2840
void __fastcall ACT2Q4_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA2920
void __fastcall ACT2Q4_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA2980
void __fastcall ACT2Q4_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA2C50
void __fastcall ACT2Q4_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA2D90
void __fastcall ACT2Q4_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA2F60
void __fastcall ACT2Q4_InitializeJerhynMonster(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit, D2RoomStrc* pRoom, D2CoordStrc* pCoord);
//D2Game.0x6FCA3160
bool __fastcall ACT2Q4_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA31E0
void __fastcall ACT2Q4_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA3320
void __fastcall ACT2Q4_InitializeJerhynStartObject(D2QuestDataStrc* pQuestData, D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA33F0
void __fastcall ACT2Q4_InitializeJerhynPalaceObject(D2QuestDataStrc* pQuestData, D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA34D0
int32_t __fastcall ACT2Q4_IsGuardMoving(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA3500
void __fastcall ACT2Q4_InitializeHaremBlockerObject(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit);
//D2Game.0x6FCA35D0
void __fastcall OBJECTS_InitFunction30_HaremBlocker(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA3620
int32_t __fastcall ACT2Q4_IsJerhynPalaceActivated(D2GameStrc* pGame);
//D2Game.0x6FCA36A0
int32_t __fastcall ACT2Q4_HasGuardMovedToEndPosition(D2GameStrc* pGame);
//D2Game.0x6FCA36E0
int32_t __fastcall ACT2Q4_GetGuardCoordinates(D2GameStrc* pGame, D2CoordStrc* pCoord);
//D2Game.0x6FCA3740
void __fastcall ACT2Q4_GetAndUpdatePalaceNpcState(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* a3, int32_t* pIdle);
//D2Game.0x6FCA3A10
int32_t __fastcall ACT2Q4_UnitIterate_CheckDistanceToHaremBlocker(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA3AA0
int32_t __fastcall OBJECTS_OperateFunction42_SanctuaryTome(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA3B80
int32_t __fastcall ACT2Q4_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA3BC0
int32_t __fastcall ACT2Q4_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA3C70
int32_t __fastcall ACT2Q4_UnitIterate_UselessGoalCheck(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA3CA0
void __fastcall OBJECTS_InitFunction29_ArcaneSanctuaryPortal(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA3D60
void __fastcall ACT2Q4_SetPortalMode(D2GameStrc* pGame, int32_t nLevelId);
//
void __fastcall ACT2Q4_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
