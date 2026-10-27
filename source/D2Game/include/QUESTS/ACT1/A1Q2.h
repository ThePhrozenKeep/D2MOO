#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//BloodRaven
struct D2Act1Quest2Strc						//sizeof 0x0C
{
	uint8_t unk0x00;								//0x00 TODO
	uint8_t unk0x01;								//0x01 TODO
	uint8_t bBloodravenKilled;						//0x02
	uint8_t bKashyaActivated;						//0x03
	int unk0x04;								//0x04 TODO
	int nBloodravenGUID;						//0x08
};

#pragma pack()

//D2Game.0x6FC98450
//1.14d: 0x00590D60
void __fastcall ACT1Q2_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC984C0
//1.14d: 0x00591080
bool __fastcall ACT1Q2_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FC98530
//1.14d: 0x00591210
void __fastcall ACT1Q2_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC98600
//1.14d: 0x00590920
void __fastcall ACT1Q2_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC98660
//1.14d: 0x00590830
int32_t __fastcall ACT1Q2_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC986D0
//1.14d: 0x00590890
int32_t __fastcall ACT1Q2_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC98760
//1.14d: 0x00590980
void __fastcall ACT1Q2_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC988F0
//1.14d: 0x00590B10
void __fastcall ACT1Q2_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC989D0
//1.14d: 0x00590EC0
void __fastcall ACT1Q2_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC98AB0
//1.14d: 0x00590BF0
bool __fastcall ACT1Q2_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FC98AD0
//1.14d: 0x00590C40
int32_t __fastcall ACT1Q2_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC98C00
//1.14d: 0x00590DD0
int32_t __fastcall ACT1Q2_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC98C60
//1.14d: 0x00590E30
int32_t __fastcall ACT1Q2_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC98CA0
//1.14d: 0x00590E70
int32_t __fastcall ACT1Q2_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC98CF0
//1.14d: 0x00590FA0
void __fastcall ACT1Q2_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC98DD0
//1.14d: 0x005910F0
bool __fastcall ACT1Q2_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC98E60
//1.14d: 0x00591180
void __fastcall ACT1Q2_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//
void __fastcall ACT1Q2_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
