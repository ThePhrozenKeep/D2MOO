#pragma once


#include "D2Structs.h"


//D2Game.0x6FCAD6F0
void __fastcall ACT4Q0_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCAD750
void __fastcall ACT4Q0_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAD790
void __fastcall ACT4Q0_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAD800
bool __fastcall ACT4Q0_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//
bool __fastcall ACT4Q0_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
