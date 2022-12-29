#pragma once


#include "D2Structs.h"


//D2Game.0x6FCA8DC0
bool __fastcall ACT3Q3_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//D2Game.0x6FCA8F10
bool __fastcall ACT3Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA8FC0
void __fastcall ACT3Q3_UnitIterate_SetGidbinnBroughtBackQuestState(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA9050
void __fastcall ACT3Q3_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA9130
void __fastcall ACT3Q3_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA91A0
int32_t __fastcall ACT3Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA9260
int32_t __fastcall ACT3Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA92D0
void __fastcall ACT3Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA9490
void __fastcall ACT3Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA9540
void __fastcall ACT3Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA9820
int32_t __fastcall ACT3Q3_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA98A0
void __fastcall ACT3Q3_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA9990
bool __fastcall ACT3Q3_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA9A00
void __fastcall ACT3Q3_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA9A30
void __fastcall ACT3Q3_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA9A70
void __fastcall ACT3Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA9BA0
void __fastcall OBJECTS_InitFunction25_Gidbinn(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA9C30
int32_t __fastcall OBJECTS_OperateFunction31_GidbinnDecoy(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA9D50
bool __fastcall ACT3Q3_SpawnGidbinnBoss(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA9E40
void __fastcall ACT3Q3_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA9EF0
int32_t __fastcall ACT3Q3_GetAltarCoordinates(D2GameStrc* pGame, D2CoordStrc* pCoord);
//D2Game.0x6FCA9F30
void __fastcall ACT3Q3_SetAltarMode(D2GameStrc* pGame);
//D2Game.0x6FCA9FA0
void __fastcall OBJECTS_InitFunction39_GidbinnAltar(D2ObjInitFnStrc* pOp);
