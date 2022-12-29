#pragma once


#include "D2Structs.h"


//D2Game.0x6FCA4650
void __fastcall ACT2Q6_AttachCompletionSoundToPlayers(D2GameStrc* pGame);
//D2Game.0x6FCA4660
int32_t __fastcall ACT2Q6_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA46A0
void __fastcall ACT2Q6_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA4750
bool __fastcall ACT2Q6_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//D2Game.0x6FCA4820
void __fastcall ACT2Q6_UnitIterate_SetCustom1Flag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA48B0
bool __fastcall ACT2Q6_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA49C0
void __fastcall ACT2Q6_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA4AB0
void __fastcall ACT2Q6_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC96C80
void __fastcall ACT2Q6_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA4D50
void __fastcall ACT2Q6_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0xFCA4E00
int32_t __fastcall ACT2Q6_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA4E70
int32_t __fastcall ACT2Q6_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA4EC0
void __fastcall ACT2Q6_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA52D0
int32_t __fastcall ACT2Q6_UnitIterate_SetLeaveTownFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA53B0
int32_t __fastcall ACT2Q6_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA5400
int32_t __fastcall ACT2Q6_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA5470
void __fastcall ACT2Q6_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA55E0
bool __fastcall ACT2Q6_TImer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA5620
int32_t __fastcall ACT2Q6_UnitIterate_SetCustom1FlagForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA56C0
void __fastcall ACT2Q6_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA5880
bool __fastcall ACT2Q6_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA5900
void __fastcall ACT2Q6_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA5930
void __fastcall ACT2Q6_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA59F0
void __fastcall ACT2Q6_SendTombIdToClient(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA5B00
int32_t __fastcall ACT2Q6_GetObjectIdForArcaneThing(D2GameStrc* pGame);
//D2Game.0x6FCA5B20
void __fastcall D2GAME_WarpPlayerToTile_6FCA5B20(D2ObjOperateFnStrc* pOp);
//D2Game.0x6FCA5B60
void __fastcall OBJECTS_InitFunction37_Unused(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA5BE0
void __fastcall OBJECTS_InitFunction38_TyraelsDoor(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA5C30
int32_t __fastcall ACT2Q6_IsDurielsLairClosed(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA5C60
void __fastcall OBJECTS_InitFunction21_HoradricOrifice(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA5CE0
bool __fastcall ACT2Q6_Timer_UpdateObjects(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA5F20
void __fastcall ACT2Q6_UnitIterate_DeleteHoradricItems(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA5FB0
int32_t __fastcall OBJECTS_OperateFunction25_StaffOrifice(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA60D0
void __fastcall ACT2Q6_DeleteAllHoradricItemsAndOpenTomb(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pObject);
//D2Game.0x6FCA6310
int32_t __fastcall ACT2Q6_IsInRangeOfTyrael(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA6370
void __fastcall ACT2Q6_UnitIterate_CheckDistanceToTyrael(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA6390
int32_t __fastcall ACT2Q6_ActivateTomb(D2GameStrc* pGame, D2CoordStrc* pCoord, D2RoomStrc** ppRoom);
