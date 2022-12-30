#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//GoldenBird
struct D2Act3Quest4Strc						//sizeof 0x1C
{
	uint8_t bGoldenBirdBroughtToAlkor;		//0x00
	uint8_t unk0x01;						//0x01 TODO
	uint8_t unk0x02;						//0x02 TODO
	uint8_t pad0x03;						//0x03
	int nBossGUID;							//0x04
	uint8_t bTalkedToAlkor;					//0x08
	uint8_t bTalkedToCainOnce;				//0x09
	uint8_t bTalkedToCainTwice;				//0x0A
	uint8_t bTalkedToMeshif;				//0x0B
	uint8_t unk0x0C;						//0x0C TODO
	uint8_t pad0x0D[3];						//0x0D
	int nGoldenBirdsInGame;					//0x10
	uint8_t bJadeFigurineDropped;			//0x14
	uint8_t bPlayerDroppedWithItem;			//0x15
	uint8_t pad0x16[2];						//0x16
	int nCurrentQuestFlag;					//0x18
};

#pragma pack()



//D2Game.0x6FCA9FF0
bool __fastcall ACT3Q4_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCAA0F0
bool __fastcall ACT3Q4_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//D2Game.0x6FCAA210
void __fastcall ACT3Q4_UnitIterate_UpdateCurrentQuestState(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAA270
void __fastcall ACT3Q4_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCAA360
void __fastcall ACT3Q4_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAA510
void __fastcall ACT3Q4_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAA520
void __fastcall ACT3Q4_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAA8D0
void __fastcall ACT3Q4_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAA990
int32_t __fastcall ACT3Q4_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAAA00
void __fastcall ACT3Q4_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAAB10
bool __fastcall ACT3Q4_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCAABE0
void __fastcall ACT3Q4_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAACE0
void __fastcall ACT3Q4_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAAD20
void __fastcall ACT3Q4_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAADA0
int32_t __fastcall ACT3Q4_UnitIterate_SetGoldenBirdBoss(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit);
//D2Game.0x6FCAAE20
void __fastcall ACT3Q4_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAAF60
bool __fastcall ACT3Q4_GoldenBirdBroughtToAlkor(D2GameStrc* pGame);
//D2Game.0x6FCAAF80
void __fastcall ACT3Q4_ResetAlkor(D2GameStrc* pGame);
//
void __fastcall ACT3Q4_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
