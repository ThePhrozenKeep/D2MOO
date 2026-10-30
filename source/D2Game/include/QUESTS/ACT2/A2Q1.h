#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Radament
struct D2Act2Quest1Strc						//sizeof 0x14
{
	uint8_t bRadamentKilled;				//0x00
	uint8_t pad0x01[3];						//0x01
	D2ActiveRoomStrc* pRoom;						//0x04
	uint8_t bAtmaActivated;					//0x08
	uint8_t bRadamentStatusIterated;		//0x09 TODO
	uint8_t bStatusTimerStarted;			//0x0A
	uint8_t pad0x0B;						//0x0B
	int nSkillBookDropCount;				//0x0C
	uint8_t bRewardPending;					//0x10
	uint8_t pad0x11[3];						//0x11
};

#pragma pack()



//D2Game.0x6FC9F5A0
//1.14d: 0x00598910
bool __fastcall ACT2Q1_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FC9F610
//1.14d: 0x00598D40
void __fastcall ACT2Q1_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9F690
//1.14d: 0x00599340
void __fastcall ACT2Q1_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9F770
//1.14d: 0x00598990
void __fastcall ACT2Q1_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9F7C0
//1.14d: 0x005988B0
int32_t __fastcall ACT2Q1_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9F830
//1.14d: 0x00598A70
void __fastcall ACT2Q1_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9FA20
//1.14d: 0x005989E0
int32_t __fastcall ACT2Q1_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9FAB0
//1.14d: 0x00598C40
void __fastcall ACT2Q1_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9FBB0
//1.14d: 0x00599020
void __fastcall ACT2Q1_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9FCB0
//1.14d: 0x00598DC0
int32_t __fastcall ACT2Q1_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9FD10
//1.14d: 0x00598E20
int32_t __fastcall ACT2Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9FE70
//1.14d: 0x00598F70
bool __fastcall ACT2Q1_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9FEA0
//1.14d: 0x00598FA0
int32_t __fastcall ACT2Q1_UnitIterate_DetermineSkillBookDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9FF20
//1.14d: 0x00599130
void __fastcall ACT2Q1_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9FFA0
//1.14d: 0x005991C0
bool __fastcall ACT2Q1_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA0010
//1.14d: 0x00599230
void __fastcall ACT2Q1_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA0120
//1.14d: 0x00599420
void __fastcall ACT2Q1_OnRadamentActivated(D2GameStrc* pGame, D2UnitStrc* pUnit);
//
//1.10f: D2Game.0x6FCA4D40
//1.14d: 0x00598980
void __fastcall ACT2Q1_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
