#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

#pragma pack()



//D2Game.0x6FC9F120
//1.14d: 0x005985D0
void __fastcall ACT2Intro_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9F180
//1.14d: 0x005983E0
void __fastcall ACT2Intro_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9F260
//1.14d: 0x005984C0
void __fastcall ACT2Intro_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//
//1.10f: D2Game.0x6FCA6B00
//1.14d: 0x005985B0
bool __fastcall ACT2Intro_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//
//1.10f: D2Game.0x6FCA6670
//1.14d: 0x005985C0
bool __fastcall ACT2Intro_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
