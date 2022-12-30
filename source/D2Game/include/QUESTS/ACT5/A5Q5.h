#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Ancients
struct D2Act5Quest5Strc					//sizeof 0x64
{
	uint8_t bAncientsDefeated;			//0x00
	uint8_t bQualKehkActivated;			//0x01
	uint8_t unused0x02[2];				//0x02
	uint8_t bTimerActive;				//0x04
	uint8_t unused0x05[7];				//0x05
	int nActivePortalCount;				//0x0C
	uint8_t unk0x10;					//0x10 TODO
	uint8_t bActivatable;				//0x11
	uint8_t pad0x12[2];					//0x12
	int nAncientStatueGUID[3];			//0x14
	int nAncientMonsterGUID[3];			//0x20
	uint8_t bAncientMonsterSpawned[3];	//0x2C
	uint8_t pad0x2F;					//0x2F
	int nAncientStatueObjectMode[3];	//0x30
	uint8_t bAncienStatueRespawned[3];	//0x3C
	uint8_t pad0x3F;					//0x3F
	int nAncientsSpawned;				//0x40
	int nAncientsAlive;					//0x44
	uint8_t bAncientsActivated;			//0x48
	uint8_t unused0x49;					//0x49
	uint8_t bQuestCompletedBefore;		//0x4A
	uint8_t pad0x4B;					//0x4B
	int nAltarObjectMode;				//0x4C
	int nAltarGUID;						//0x50
	int nLivingPlayersOnSummit;			//0x54
	uint8_t bSummitDoorInitialized;		//0x58
	uint8_t pad0x59[3];					//0x59
	int nSummitDoorGUID;				//0x5C
	int nSummitDoorObjectMode;			//0x60
};

#pragma pack()

//D2Game.0x6FCB6910
void __fastcall ACT5Q5_SpawnAncientMonsters(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit);
//D2Game.0x6FCB6A50
void __fastcall ACT5Q5_DeactivateAncientMonsters(D2GameStrc* pGame, D2QuestDataStrc* pQuestData, D2Act5Quest5Strc* pQuestDataEx);
//D2Game.0x6FCB6BA0
void __fastcall ACT5Q5_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB6C20
void __fastcall ACT5Q5_RewardPlayer(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCB6D30
bool __fastcall ACT5Q5_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCB6D60
void __fastcall ACT5Q5_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB6E30
void __fastcall ACT5Q5_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB6E70
int32_t __fastcall ACT5Q5_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB6EE0
void __fastcall ACT5Q5_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB7160
int32_t __fastcall ACT5Q5_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB71E0
void __fastcall ACT5Q5_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//
void __fastcall ACT5Q5_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB7370
void __fastcall ACT5Q5_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB7680
bool __fastcall ACT5Q5_Timer_RespawnAncientStatueObjects(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB7760
int32_t __fastcall ACT5Q5_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB77C0
int32_t __fastcall ACT5Q5_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB78A0
void __fastcall ACT5Q5_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB79F0
bool __fastcall ACT5Q5_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB7A60
void __fastcall ACT5Q5_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB7AF0
void __fastcall ACT5Q5_OnPortalOpened(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCB7B40
void __fastcall ACT5Q5_OnPortalClosed(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCB7B90
int32_t __fastcall ACT5Q5_IsNotActivatable(D2GameStrc* pGame);
//D2Game.0x6FCB7BB0
int32_t __fastcall ACT5Q5_GetPortalCount(D2GameStrc* pGame);
//D2Game.0x6FCB7BD0
int32_t __fastcall ACT5Q5_IsActivatable(D2GameStrc* pGame);
//D2Game.0x6FCB7C00
int32_t __fastcall ACT5Q5_IsArreatSummitClosed(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTargetLevel);
//D2Game.0x6FCB7C30
void __fastcall OBJECTS_InitFunction65_AncientStatue2(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB7CB0
void __fastcall OBJECTS_InitFunction63_AncientStatue3(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB7D30
void __fastcall OBJECTS_InitFunction64_AncientStatue1(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB7DB0
int32_t __fastcall OBJECTS_OperateFunction62_63_64_AncientStatue(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCB7DD0
void __fastcall OBJECTS_InitFunction72_AncientsAltar(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB7E10
void __fastcall OBJECTS_InitFunction73_AncientsDoor(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB7E50
int32_t __fastcall OBJECTS_OperateFunction65_AncientsAltar(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCB7F40
int32_t __fastcall ACT5Q5_UnitIterate_ClosePortals(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB7F80
int32_t __fastcall OBJECTS_OperateFunction66_AncientsDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCB80B0
void __fastcall ACT5Q5_OnPlayerDied(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCB8140
int32_t __fastcall ACT5Q5_UnitIterate_CountLivingPlayers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB8190
int32_t __fastcall OBJECTS_OperateFunction69_InvisibleAncient(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCB81F0
void __fastcall OBJECTS_InitFunction76_SummitDoor(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB8250
int32_t __fastcall OBJECTS_OperateFunction71_SummitDoor(D2ObjOperateFnStrc* pOp, int32_t nOperate);
