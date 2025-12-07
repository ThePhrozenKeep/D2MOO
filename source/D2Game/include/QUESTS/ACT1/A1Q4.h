#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//RescueCain
struct D2Act1Quest4Strc								//sizeof 0x1BC
{
	uint16_t nStoneOrder[5];						//0x00
	uint16_t unused0x0A;							//0x0A
	uint16_t nCurrentMonolithNo;					//0x0C
	uint8_t pad0x0E[2];								//0x0E
	int nMonolithGUIDs[5];							//0x10
	int unused0x24;									//0x24
	int nInvisibleObjectGUID;						//0x28
	short nCairnStoneInteractions;					//0x2C
	uint8_t pad0x2E[2];								//0x2E
	int nInifussTreeGUID;							//0x30
	int nCainGibbetGUID;							//0x34
	int nDecipheredScrollGUID;						//0x38
	int nPlayerGUID;								//0x3C
	int nCairnStoneAlphaGUID;						//0x40
	uint8_t bOpenPortalTimerActive;					//0x44
	uint8_t bPortalToTristramOpened;				//0x45
	uint8_t bTimerActive;							//0x46
	uint8_t bInifussTreeInitialized;				//0x47
	uint8_t bCageModeChanged;						//0x48
	uint8_t bInvisbleObjectInitialized;				//0x49
	uint8_t bMonolithStonesOrdered;					//0x4A
	uint8_t unk0x4B;								//0x4B TODO
	uint8_t unk0x4C;								//0x4C TODO
	uint8_t bActivateCairnStones;					//0x4D
	uint8_t bDecipheredScrollAcquired;				//0x4E
	uint8_t bOperatedWithLastCairnStone;			//0x4F
	uint8_t unk0x50;								//0x50 TODO: Cain rescued?
	uint8_t bCainInTownSpawned;						//0x51
	uint8_t unk0x52;								//0x52 TODO: Cain not spawned?
	uint8_t bAkaraIntroActivated;					//0x53
	int nCainGibbetObjectMode;						//0x54
	int unk0x58;									//0x58 TODO
	uint8_t bCairnStonesActive[5];					//0x5C
	uint8_t bCainRemovedFromTristram;				//0x61
	uint8_t bCainNotCreated;						//0x62
	uint8_t bQuestCompleteBefore;					//0x63
	uint8_t bAkaraActivatedForScroll;				//0x64
	uint8_t unused0x65;								//0x65
	uint8_t bPortalFromTristramToTownSpawned;		//0x66
	uint8_t bCainPortalCoordsInitialized;			//0x67
	int nCainMonsterInTownGUID;						//0x68
	int nCainObjectGUID;							//0x6C
	uint8_t bCainObjectInitialized;					//0x70
	uint8_t pad0x71[3];								//0x71
	D2UnitStrc* pPlayerInTristram;					//0x74
	uint8_t bScrollAcquired;						//0x78
	uint8_t pad0x79[3];								//0x79
	int nScrollsInGame;								//0x7C
	int nObjectUpdateInvocations;					//0x80
	D2CoordStrc pCainStartPositionCoords;			//0x84
	int nGoldPilesToDropFromWirtsBody;				//0x8C
	uint8_t bWirtsBodyInitialized;					//0x90
	uint8_t bChangePortalOutsideTownToSpecialMode;	//0x91
	uint8_t bChangePortalInTownToSpecialMode;		//0x92
	uint8_t bCainInTristramDeactivated;				//0x93
	uint8_t unused0x94;								//0x94
	uint8_t bCainPortalOutsideTownSpawned;			//0x95
	uint8_t bCainPortalInTownSpawned;				//0x96
	uint8_t unused0x97;								//0x97
	int nCainPortalOutsideTownGUID;					//0x98
	D2CoordStrc pCainPortalOutsideTownCoords;		//0x9C
	int nCainPortalInTownGUID;						//0xA4
	D2CoordStrc pCainPortalInTownCoords;			//0xA8
	uint8_t unused0xB0[4];							//0xB0
	D2QuestGUIDStrc pQuestGUID;						//0xB4
	D2QuestGUIDStrc tPlayerGUIDs;							//0x138
};

#pragma pack()



//D2Game.0x6FC99D00
void __fastcall ACT1Q4_UpdateObjects(D2QuestDataStrc* pQuestData, char a2, char a3);
//D2Game.0x6FC99DE0
int32_t __fastcall ACT1Q4_UnitIterate_RemoveCainFromTristram(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC99E80
void __fastcall ACT1Q4_SendPacket0x5DToClient(D2UnitStrc* pUnit);
//D2Game.0x6FC99EC0
void __fastcall ACT1Q4_SetMonolithOrder(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC99F90
void __fastcall ACT1Q4_SetInvisibleObjectGUID(D2GameStrc* pGame, D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit);
//D2Game.0x6FC99FC0
bool __fastcall ACT1Q4_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FC9A0E0
void __fastcall ACT1Q4_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A160
bool __fastcall ACT1Q4_Timer_RemoveCainFromTristram(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9A190
void __fastcall ACT1Q4_SpawnCainInTristram(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9A3D0
int32_t __fastcall ACT1Q4_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A440
int32_t __fastcall ACT1Q4_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A4D0
int32_t __fastcall ACT1Q4_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A530
int32_t __fastcall ACT1Q4_UnitIterate_FindPlayersInTristram(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A580
int32_t __fastcall OBJECTS_OperateFunction10_CainGibbet(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC9A6E0
void __fastcall OBJECTS_InitFunction06_CairnStone(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9A7C0
bool __fastcall ACT1Q4_OpenPortalToTristram(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9A870
void __fastcall ACT1Q4_UnitIterate_SetEnterAreaFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9A8D0
int32_t __fastcall OBJECTS_OperateFunction09_Monolith(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC9ACD0
int32_t __fastcall OBJECTS_OperateFunction12_InifussTree(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FC9AEA0
void __fastcall ACT1Q4_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9AF10
void __fastcall ACT1Q4_ResetInifussTree(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9AFD0
int32_t __fastcall ACT1Q4_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9B050
void __fastcall ACT1Q4_SendStoneOrderToClient(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9B0F0
void __fastcall ACT1Q4_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9B250
void __fastcall ACT1Q4_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9B2F0
void __fastcall ACT1Q4_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9B610
void __fastcall ACT1Q4_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9B930
void __fastcall ACT1Q4_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9B970
void __fastcall ACT1Q4_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9B9D0
void __fastcall ACT1Q4_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9BC80
void __fastcall ACT1Q4_SpawnCainInTown(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY);
//D2Game.0x6FC9BEF0
int32_t __fastcall ACT1Q4_UnitIterate_AddPlayerGUIDs(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9BF70
void __fastcall ACT1Q4_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9BFB0
void __fastcall ACT1Q4_Callback06_6FC9BFB0(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9BFF0
bool __fastcall ACT1Q4_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FC9C0A0
void __fastcall ACT1Q4_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9C220
void __fastcall ACT1Q4_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC9C300
int32_t __fastcall ACT1Q4_UnitIterate_SetCustom6Flag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FC9C340
void __fastcall OBJECTS_InitFunction09_InifussTree(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9C3A0
void __fastcall OBJECTS_InitFunction13_InvisibleObject(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9C460
void __fastcall ACT1Q4_UpdateQuestStateOnActChange(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9C540
void __fastcall OBJECTS_InitFunction54_CainStartPosition(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9C5B0
int32_t __fastcall ACT1Q4_CreateCowPortal(D2GameStrc* pGame, D2UnitStrc* pUnit);
#ifdef D2_VERSION_HAS_UBERS
//1.14d: 0x00594270
int32_t __fastcall ACT1Q4_CreateUberRunPortal(D2GameStrc* pGame, D2UnitStrc* pUnit);
//1.14d: 0x00594280
int32_t __fastcall ACT1Q4_CreateUberFinalePortal(D2GameStrc* pGame, D2UnitStrc* pUnit);
#endif
//D2Game.0x6FC9C6E0
void __fastcall OBJECTS_InitFunction61_CainPortal(D2ObjInitFnStrc* pOp);
//D2Game.0x6FC9C710
void __fastcall ACT1Q4_UpdatePortalInTown(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit);
//D2Game.0x6FC9C7A0
int32_t __fastcall ACT1Q4_GetCainPortalInTownCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pCoord);
//D2Game.0x6FC9C7F0
int32_t __fastcall ACT1Q4_SpawnCainPortalOutsideTown(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9C930
int32_t __fastcall ACT1Q4_GetCainPortalOutsideTownCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, D2CoordStrc* pCoord);
//D2Game.0x6FC9C990
int32_t __fastcall ACT1Q4_UpdateCainPortalCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC9C9D0
void __fastcall ACT1Q4_SpawnCainPortalInTown(D2GameStrc* pGame);
//D2Game.0x6FC9CBE0
void __fastcall ACT1Q4_OnCainInTownActivated(D2GameStrc* pGame);
//D2Game.0x6FC9CC00
int32_t __fastcall ACT1Q4_IsCainInTristramDeactivated(D2GameStrc* pGame);
