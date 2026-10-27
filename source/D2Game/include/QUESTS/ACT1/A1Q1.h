#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//DenOfEvil
struct D2Act1Quest1Strc						//sizeof 0x8C
{
	D2QuestGUIDStrc pQuestGUID;					//0x00
	uint8_t bFinished;								//0x84
	uint8_t bEnteredDen;							//0x85
	uint8_t bAkaraActivated;						//0x86
	uint8_t bTimerActive;							//0x87
	int nMonstersLeft;							//0x88
};

#pragma pack()



//D2Game.0x6FC97920
//1.14d: 0x00590120
void __fastcall ACT1Q1_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC97990
//1.14d: 0x005901E0
int32_t __fastcall ACT1Q1_GetMonstersToBeKilled(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC979A0
//1.14d: 0x005905B0
bool __fastcall ACT1Q1_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FC97A10
//1.14d: 0x00590720
void __fastcall ACT1Q1_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC97AE0
//1.14d: 0x0058FC40
void __fastcall ACT1Q1_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC97B30
//1.14d: 0x0058FBE0
int32_t __fastcall ACT1Q1_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC97BA0
//1.14d: 0x0058FDD0
void __fastcall ACT1Q1_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC97D30
//1.14d: 0x0058FC90
int32_t __fastcall ACT1Q1_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC97DC0
//1.14d: 0x0058FF90
void __fastcall ACT1Q1_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC97ED0
//1.14d: 0x00590260
void __fastcall ACT1Q1_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC980C0
//1.14d: 0x00590080
int32_t __fastcall ACT1Q1_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC98120
//1.14d: 0x005900E0
int32_t __fastcall ACT1Q1_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC98160
//1.14d: 0x00590190
int32_t __fastcall ACT1Q1_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC981B0
//1.14d: 0x00590230
bool __fastcall ACT1Q1_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FC981E0
//1.14d: 0x00590470
void __fastcall ACT1Q1_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC98330
//1.14d: 0x00590620
bool __fastcall ACT1Q1_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC983A0
//1.14d: 0x00590690
void __fastcall ACT1Q1_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC98430
//1.14d: 0x00590810
bool __fastcall ACT1Q1_CanClientFXBeTriggered(D2QuestDataStrc* pQuestData);
//
//1.10f: D2Game.0x6FCB2610
//1.14d: 0x005901F0
void __fastcall ACT1Q1_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
