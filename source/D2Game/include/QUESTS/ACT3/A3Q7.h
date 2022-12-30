#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//DarkWanderer
struct D2Act3Quest7Strc						//sizeof 0x14
{
	uint8_t bDarkWandererInitialized;		//0x00
	uint8_t bPrimaryGoalOpen;				//0x01
	uint8_t bWandererCoordsCalculated;		//0x02
	uint8_t pad0x03;						//0x03
	int nDarkWandererX;						//0x04
	int nDarkWandererY;						//0x08
	uint8_t bVileDogsSpawned;				//0x0C
	uint8_t bVileDogSpawnTimerCreated;		//0x0D
	uint8_t pad0x0E[2];						//0x0E
	int nDarkWandererGUID;					//0x10
};

#pragma pack()



//D2Game.0x6FCAD210
int32_t __fastcall ACT3Q7_GetWandererCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pCoord);
//D2Game.0x6FCAD360
void __fastcall OBJECTS_InitFunction43_DarkWanderer(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCAD460
void __fastcall ACT3Q7_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCAD4F0
void __fastcall ACT3Q7_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAD530
void __fastcall ACT3Q7_CreateVileDogSpawnTimer(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCAD590
bool __fastcall ACT3Q7_SpawnVileDogs(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCAD690
int32_t __fastcall ACT3Q7_UnitIterate_SetRewardGranted(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//
bool __fastcall ACT3Q7_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//
bool __fastcall ACT3Q7_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
