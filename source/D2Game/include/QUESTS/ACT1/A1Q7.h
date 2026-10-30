#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

#pragma pack()



//D2Game.0x6FC9EF40
//1.14d: 0x00596C40
void __fastcall ACT1Q7_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9EFA0
//1.14d: 0x00596A90
void __fastcall ACT1Q7_Callback00_NpcActivate(D2QuestDataStrc* pQuest, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9F0A0
//1.14d: 0x00596BC0
bool __fastcall ACT1Q7_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//
//1.10f: D2Game.0x6FCA6670
//1.14d: 0x00596BB0
bool __fastcall ACT1Q7_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
