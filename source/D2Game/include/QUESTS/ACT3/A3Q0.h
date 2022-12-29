#pragma once


#include "D2Structs.h"


//D2Game.0x6FCA6B40
void __fastcall ACT3Q0_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA6BD0
void __fastcall ACT3Q0_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA6C30
void __fastcall ACT3Q0_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA6CB0
bool __fastcall ACT3Q0_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA6CE0
void __fastcall ACT3Q0_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA6D20
void __fastcall OBJECTS_InitFunction49_HratliStart(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA6DA0
void __fastcall OBJECTS_InitFunction50_HratliEnd(D2ObjInitFnStrc* pOp);
//
bool __fastcall ACT3Q0_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
