#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Diablo
struct D2Act4Quest2Strc						//sizeof 0x4C
{
	uint8_t bTalkedToTyrael;				//0x00
	uint8_t bTimerCreated;					//0x01
	uint8_t bNeedToSpawnDiablo;				//0x02
	uint8_t unk0x03;						//0x03 TODO
	uint8_t bNeedToEndGame;					//0x04
	uint8_t bNeedToWarpPlayers;				//0x05
	uint8_t bDiabloStartPointInitialized;	//0x06
	uint8_t pad0x07;						//0x07
	int nDiabloStartPointObjectGUID;		//0x08
	uint8_t bSealActivated[5];				//0x0C
	uint8_t bDiabloSpawned;					//0x11
	uint8_t unused0x12;						//0x12
	uint8_t bSanctumCleared;				//0x13
	uint8_t bDiabloKilled;					//0x14
	uint8_t bClientsSaved;					//0x15
	uint8_t pad0x16[2];						//0x16
	uint32_t dwTickCount;					//0x18 TODO: What TickCount?
	int nPlayerCount;						//0x1C
	D2UnitStrc* unk0x20;					//0x20 TODO
	D2CoordStrc pSealCoords[3];				//0x24
	D2RoomStrc* pRoom;						//0x3C
	int nLastLastPortalObjectMode;			//0x40
	uint8_t unk0x44;						//0x44 TODO
	uint8_t bPortalToAct5Spawned;			//0x45
	uint8_t pad0x46[2];						//0x46
	int nBossesKilled;						//0x48
};

#pragma pack()



//D2Game.0x6FCAE4C0
bool __fastcall ACT4Q2_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCAE5B0
void __fastcall ACT4Q2_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAE670
int32_t __fastcall ACT4Q2_IsChaosSanctumCleared(D2GameStrc* pGame);
//D2Game.0x6FCAE690
void __fastcall ACT4Q2_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCAE750
bool __fastcall ACT4Q2_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCAE780
void __fastcall ACT4Q2_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAE950
int32_t __fastcall ACT4Q2_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAE9D0
void __fastcall ACT4Q2_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAEBD0
void __fastcall ACT4Q2_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAECC0
int32_t __fastcall ACT4Q2_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAED30
void __fastcall ACT4Q2_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAEE30
void __fastcall ACT4Q2_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAEE80
void __fastcall ACT4Q2_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAEF40
void __fastcall ACT4Q2_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCAF110
int32_t __fastcall ACT4Q2_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAF160
int32_t __fastcall ACT4Q2_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAF1E0
bool __fastcall ACT4Q2_SpawnDiablo(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCAF3D0
int32_t __fastcall ACT4Q2_UnitIterate_WarpToTownEndGame(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAF4A0
int32_t __fastcall ACT4Q2_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAF4F0
int32_t __fastcall ACT4Q2_UnitIterate_UpdatePlayerState(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCAF630
void __fastcall ACT4Q2_KillAllMonstersInCS(D2GameStrc* pGame, D2Act4Quest2Strc* pQuestDataEx);
//D2Game.0x6FCAF6D0
void __fastcall OBJECTS_InitFunction55_DiabloStartPoint(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCAF760
int32_t __fastcall OBJECTS_OperateFunction54_DiabloSeal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCAF8D0
int32_t __fastcall OBJECTS_OperateFunction55_DiabloSeal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCAFA40
int32_t __fastcall OBJECTS_OperateFunction56_DiabloSeal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCAFBB0
int32_t __fastcall OBJECTS_OperateFunction52_DiabloSeal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCAFCB0
void __fastcall ACT4Q2_SpawnSealBoss(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCAFD80
int32_t __fastcall ACT4Q2_HasDiabloBeenKilled(D2GameStrc* pGame);
//D2Game.0x6FCAFDB0
void __fastcall OBJECTS_InitFunction78_LastLastPortal(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCAFDF0
int32_t __fastcall OBJECTS_OperateFunction73_LastLastPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
