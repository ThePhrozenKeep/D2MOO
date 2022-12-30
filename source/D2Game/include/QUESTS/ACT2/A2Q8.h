#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Greiz Gossip
struct D2Act2Quest8Strc						//sizeof 0x02
{
	uint8_t unk0x00[2];						//0x00
};

#pragma pack()



//D2Game.0x6FCA66F0
void __fastcall ACT2Q8_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA6770
void __fastcall ACT2Q8_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA67B0
void __fastcall ACT2Q8_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA6850
bool __fastcall ACT2Q8_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//
bool __fastcall ACT2Q8_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
