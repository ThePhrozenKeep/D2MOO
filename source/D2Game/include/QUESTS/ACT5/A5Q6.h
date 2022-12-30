#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Baal
struct D2Act5Quest6Strc						//sizeof 0xA8
{
	D2QuestGUIDStrc tPlayerGUIDs;					//0x00
	uint8_t unused0x84;						//0x84
	uint8_t unused0x85;						//0x85
	uint8_t bWorldStoneChamberOpen;			//0x86
	uint8_t pad0x87;						//0x87
	int nPlayersInWorldStoneChamber;		//0x88
	D2RoomStrc* pRoom;						//0x8C
	uint8_t bQuestStarted;					//0x90
	uint8_t pad0x91[3];						//0x91
	int nBaalPortalObjectMode;				//0x94
	uint8_t bLastPortalOpened;				//0x98
	uint8_t pad0x99[3];						//0x99
	int nLastPortalObjectMode;				//0x9C
	uint8_t bZooInitialized;				//0xA0
	uint8_t pad0xA1[3];						//0xA1
	uint16_t nZooMonsterId;					//0xA4
	uint8_t pad0xA6[2];						//0xA6
};

#pragma pack()



//D2Game.0x6FCB8320
void __fastcall ACT5Q6_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB83B0
bool __fastcall ACT5Q6_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCB8490
void __fastcall ACT5Q6_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB8580
void __fastcall ACT5Q6_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB85C0
int32_t __fastcall ACT5Q6_UnitIterate_CreateLastPortal(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB8660
void __fastcall ACT5Q6_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB8710
void __fastcall ACT5Q6_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB8910
void __fastcall ACT5Q6_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB8B80
int32_t __fastcall ACT5Q6_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB8BF0
int32_t __fastcall ACT5Q6_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB8C40
int32_t __fastcall ACT5Q6_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB8C90
int32_t __fastcall ACT5Q6_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB8D10
int32_t __fastcall ACT5Q6_UnitIterate_SetRewardGranted(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB8DE0
void __fastcall ACT5Q6_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB8F30
int32_t __fastcall ACT5Q6_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB8FD0
bool __fastcall ACT5Q6_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCB9030
void __fastcall ACT5Q6_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB9070
void __fastcall ACT5Q6_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCB9150
void __fastcall ACT5Q6_OpenWorldStoneChamber(D2GameStrc* pGame);
//D2Game.0x6FCB9190
int32_t __fastcall ACT5Q6_IsWorldStoneChamberClosed(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCB91C0
void __fastcall OBJECTS_InitFunction75_BaalsPortal(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB91F0
int32_t __fastcall OBJECTS_OperateFunction70_BaalPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCB9260
void __fastcall OBJECTS_InitFunction77_LastPortal(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB9290
int32_t __fastcall OBJECTS_OperateFunction72_LastPortal(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCB9370
void __fastcall OBJECTS_InitFunction79_Zoo(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCB94C0
int32_t __fastcall ACT5Q6_UnitIterate_SendZooMonsterIdToClient(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCB9520
void __fastcall ACT5Q6_SpawnTyrael(D2GameStrc* pGame, D2RoomStrc* pRoom, D2UnitStrc* pUnit);
//
void __fastcall ACT5Q6_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
