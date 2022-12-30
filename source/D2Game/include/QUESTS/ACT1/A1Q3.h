#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Malus
struct D2Act1Quest3Strc						//sizeof 0xA4
{
	uint8_t unk0x00;						//0x00 TODO
	uint8_t bMalusInitialized;				//0x01
	uint8_t bCharsiIntroActivated;			//0x02
	uint8_t bCharsiEndActivated;			//0x03
	int nMalusGUID;							//0x04
	uint8_t unk0x08;						//0x08 TODO
	uint8_t unused0x09[9];					//0x09
	uint16_t unk0x12;						//0x12
	D2QuestGUIDStrc tPlayerGUIDs;					//0x14
	int nMalusObjectMode;					//0x98
	int nMalusItemsInGame;					//0x9C
	uint8_t bMalusAcquired;					//0xA0
	uint8_t bStartingPlayerHasMalus;		//0xA1
	uint8_t pad0xA2[2];						//0xA2
};

#pragma pack()



//D2Game.0x6FC98EF0
void __fastcall ACT1Q3_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC98F50
int32_t __fastcall ACT1Q3_SetRewardGranted(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC98FD0
int32_t __fastcall OBJECTS_OperateFunction21_HoradrimMalus(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC99150
int32_t __fastcall ACT1Q3_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC991C0
int32_t __fastcall ACT1Q3_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC99240
bool __fastcall ACT1Q3_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FC992C0
void __fastcall ACT1Q3_UnitIterate_CheckForMalus(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC992F0
bool __fastcall ACT1Q3_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//D2Game.0x6FC99430
void __fastcall ACT1Q3_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC99540
void __fastcall ACT1Q3_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC995C0
void __fastcall ACT1Q3_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC997E0
void __fastcall ACT1Q3_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC998D0
void __fastcall ACT1Q3_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC99990
void __fastcall ACT1Q3_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC999F0
void __fastcall ACT1Q3_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC99A50
void __fastcall ACT1Q3_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC99AA0
void __fastcall ACT1Q3_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC99AD0
void __fastcall ACT1Q3_Callback06(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC99B80
bool __fastcall ACT1Q3_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC99C10
void __fastcall ACT1Q3_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
