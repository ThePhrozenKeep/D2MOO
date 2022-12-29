#pragma once


#include "D2Structs.h"


//D2Game.0x6FC9DFF0
void __fastcall ACT1Q6_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9E0E0
bool __fastcall ACT1Q6_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FC9E1B0
void __fastcall ACT1Q6_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9E2B0
void __fastcall ACT1Q6_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9E300
int32_t __fastcall ACT1Q6_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9E370
void __fastcall ACT1Q6_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9E510
int32_t __fastcall ACT1Q6_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9E5A0
void __fastcall ACT1Q6_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9E7A0
void __fastcall ACT1Q6_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9E900
void __fastcall ACT1Q6_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9EB50
int32_t __fastcall ACT1Q6_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9EBA0
void __fastcall ACT1Q6_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pArgs);
//D2Game.0x6FC9EBF0
int32_t __fastcall ACT1Q6_UnitIterate_SetRewardPending(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9ECD0
int32_t __fastcall ACT1Q6_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9ED30
int32_t __fastcall ACT1Q6_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9ED80
bool __fastcall ACT1Q6_Timer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuest);
//D2Game.0x6FC9EE00
int32_t __fastcall ACT1Q6_UnitIterate_CreatePortalToTown(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9EE70
bool __fastcall ACT1Q6_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9EE90
bool __fastcall ACT1Q6_Timer_StateDebug(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9EEB0
void __fastcall ACT1Q6_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
