#pragma once


#include "D2Structs.h"


//D2Game.0x6FCA6420
void __fastcall ACT2Q7_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA64B0
void __fastcall ACT2Q7_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA6510
void __fastcall ACT2Q7_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA6540
void __fastcall ACT2Q7_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA6660
void __fastcall OBJECTS_InitFunction05_Door(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA6680
bool __fastcall ACT2Q7_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//
bool __fastcall ACT2Q7_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
