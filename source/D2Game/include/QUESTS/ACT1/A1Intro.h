#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

#pragma pack()



//D2Game.0x6FC975E0
//1.14d: 0x0058FA20
void __fastcall ACT1Intro_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC97640
//1.14d: 0x0058F870
void __fastcall ACT1Intro_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC976C0
//1.14d: 0x0058F8F0
void __fastcall ACT1Intro_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC97770
//1.14d: 0x0058F9C0
bool __fastcall ACT1Intro_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//
bool __fastcall ACT1Intro_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
