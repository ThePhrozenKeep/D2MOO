#pragma once


#include "D2Structs.h"


//D2Game.0x6FCB0FD0
void __fastcall ACT5Intro_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB1030
void __fastcall ACT5Intro_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB1100
void __fastcall ACT5Intro_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB11D0
bool __fastcall ACT5Intro_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//
bool __fastcall ACT5Intro_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
