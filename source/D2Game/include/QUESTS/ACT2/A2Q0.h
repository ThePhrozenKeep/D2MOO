#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Jerhyn Gossip
struct D2Act2Quest0Strc						//sizeof 0x84
{
	D2QuestGUIDStrc tPlayerGUIDs;						//0x00
};

#pragma pack()



//D2Game.0x6FC9F350
//1.14d: 0x00598810
void __fastcall ACT2Q0_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9F3F0
//1.14d: 0x00598640
void __fastcall ACT2Q0_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9F460
//1.14d: 0x005986B0
void __fastcall ACT2Q0_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9F510
//1.14d: 0x00598780
bool __fastcall ACT2Q0_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FC9F540
//1.14d: 0x005987B0
void __fastcall ACT2Q0_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9F560
//1.14d: 0x005987D0
void __fastcall ACT2Q0_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//
//1.10f: D2Game.0x6FCA6670
//1.14d: 0x00598770
bool __fastcall ACT2Q0_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
