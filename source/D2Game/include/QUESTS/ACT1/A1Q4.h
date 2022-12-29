#pragma once


#include "D2Structs.h"


//D2Game.0x6FC99D00
void __fastcall ACT1Q4_UpdateObjects(D2QuestDataStrc* pQuestData, char a2, char a3);
//D2Game.0x6FC99DE0
int32_t __fastcall ACT1Q4_UnitIterate_RemoveCainFromTristram(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC99E80
void __fastcall ACT1Q4_SendPacket0x5DToClient(D2UnitStrc* pUnit);
//D2Game.0x6FC99EC0
void __fastcall ACT1Q4_SetMonolithOrder(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC99F90
void __fastcall ACT1Q4_SetInvisibleObjectGUID(D2GameStrc* pGame, D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit);
//D2Game.0x6FC99FC0
bool __fastcall ACT1Q4_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FC9A0E0
void __fastcall ACT1Q4_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A160
bool __fastcall ACT1Q4_Timer_RemoveCainFromTristram(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9A190
void __fastcall ACT1Q4_SpawnCainInTristram(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9A3D0
int32_t __fastcall ACT1Q4_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A440
int32_t __fastcall ACT1Q4_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A4D0
int32_t __fastcall ACT1Q4_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A530
int32_t __fastcall ACT1Q4_UnitIterate_FindPlayersInTristram(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A580
int32_t __fastcall OBJECTS_OperateFunction10_CainGibbet(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC9A6E0
void __fastcall OBJECTS_InitFunction06_CairnStone(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9A7C0
bool __fastcall ACT1Q4_OpenPortalToTristram(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9A870
void __fastcall ACT1Q4_UnitIterate_SetEnterAreaFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A8D0
int32_t __fastcall OBJECTS_OperateFunction09_Monolith(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC9ACD0
int32_t __fastcall OBJECTS_OperateFunction12_InifussTree(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC9AEA0
void __fastcall ACT1Q4_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9AF10
void __fastcall ACT1Q4_ResetInifussTree(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9AFD0
int32_t __fastcall ACT1Q4_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9B050
void __fastcall ACT1Q4_SendStoneOrderToClient(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9B0F0
void __fastcall ACT1Q4_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9B250
void __fastcall ACT1Q4_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9B2F0
void __fastcall ACT1Q4_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9B610
void __fastcall ACT1Q4_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9B930
void __fastcall ACT1Q4_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9B970
void __fastcall ACT1Q4_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9B9D0
void __fastcall ACT1Q4_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9BC80
void __fastcall ACT1Q4_SpawnCainInTown(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nX, int32_t nY);
//D2Game.0x6FC9BEF0
int32_t __fastcall ACT1Q4_UnitIterate_AddPlayerGUIDs(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9BF70
void __fastcall ACT1Q4_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9BFB0
void __fastcall ACT1Q4_Callback06_6FC9BFB0(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9BFF0
bool __fastcall ACT1Q4_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9C0A0
void __fastcall ACT1Q4_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9C220
void __fastcall ACT1Q4_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9C300
int32_t __fastcall ACT1Q4_UnitIterate_SetCustom6Flag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9C340
void __fastcall OBJECTS_InitFunction09_InifussTree(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9C3A0
void __fastcall OBJECTS_InitFunction13_InvisibleObject(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9C460
void __fastcall ACT1Q4_UpdateQuestStateOnActChange(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9C540
void __fastcall OBJECTS_InitFunction54_CainStartPosition(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9C5B0
int32_t __fastcall ACT1Q4_CreateCowPortal(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9C6E0
void __fastcall OBJECTS_InitFunction61_CainPortal(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9C710
void __fastcall ACT1Q4_UpdatePortalInTown(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit);
//D2Game.0x6FC9C7A0
int32_t __fastcall ACT1Q4_GetCainPortalInTownCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pCoord);
//D2Game.0x6FC9C7F0
int32_t __fastcall ACT1Q4_SpawnCainPortalOutsideTown(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9C930
int32_t __fastcall ACT1Q4_GetCainPortalOutsideTownCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pCoord);
//D2Game.0x6FC9C990
int32_t __fastcall ACT1Q4_UpdateCainPortalCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9C9D0
void __fastcall ACT1Q4_SpawnCainPortalInTown(D2GameStrc* pGame);
//D2Game.0x6FC9CBE0
void __fastcall ACT1Q4_OnCainInTownActivated(D2GameStrc* pGame);
//D2Game.0x6FC9CC00
int32_t __fastcall ACT1Q4_IsCainInTristramDeactivated(D2GameStrc* pGame);
