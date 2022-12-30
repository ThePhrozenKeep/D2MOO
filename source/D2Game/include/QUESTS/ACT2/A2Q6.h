#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//Duriel
struct D2Act2Quest6Strc							//sizeof 0x68
{
	uint8_t unk0x00;							//0x00 TODO
	uint8_t bDurielKilled;						//0x01
	uint8_t unk0x02;							//0x02 TOOO
	uint8_t bTimerActive;						//0x03
	uint8_t bHoradricOrificeInitialized;		//0x04
	uint8_t bTyraelsDoorInitialized;			//0x05
	uint8_t bUnusedObjectInitialized;			//0x06
	uint8_t unused0x07;							//0x07
	uint8_t bInitJerhynActivated;				//0x08
	uint8_t bTyraelActivated;					//0x09
	uint8_t bEndJerhynActivated;				//0x0A
	uint8_t bDurielLairTombOpen;				//0x0B
	uint8_t bDurielLairPortalNeedsToOpen;		//0x0C
	uint8_t bObjectsNeedUpdate;					//0x0D
	uint8_t bStaffItemsRemoved;					//0x0E
	uint8_t bPortalToLutGholeinOpened;			//0x0F
	uint8_t bHoradricStaffIsMissing;			//0x10
	uint8_t pad0x11[3];							//0x11
	int nQuestStatus;							//0x14
	int nTyraelsDoorObjectMode;					//0x18
	int unk0x1C;								//0x1C
	int nHoradricOrificeGUID;					//0x20
	uint8_t unused0x24[4];						//0x24
	int nUnusedObjectGUID;						//0x28
	int nTyraelsDoorGUID;						//0x2C
	uint8_t unused0x30[4];						//0x30
	int nStaffTombLevelId;						//0x34
	uint8_t bObjectIdsInitialized;				//0x38
	uint8_t pad0x39;							//0x39
	uint16_t nArcaneThings;						//0x3A
	uint8_t bPortalIsOpening;					//0x3C
	uint8_t bRewardedOrCompletedBefore;			//0x3D
	uint8_t pad0x3E[2];							//0x3E
	D2UnitStrc* pTyrael;						//0x40
	uint8_t bCloseToTyrael;						//0x44
	uint8_t pad0x45[3];							//0x45
	int nArcaneThingObjectIds[6];				//0x48
	D2RoomStrc* pDurielRoom;					//0x60
	int unused0x64;								//0x64
};

#pragma pack()



//D2Game.0x6FCA4650
void __fastcall ACT2Q6_AttachCompletionSoundToPlayers(D2GameStrc* pGame);
//D2Game.0x6FCA4660
int32_t __fastcall ACT2Q6_UnitIterate_AttachCompletionSound(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA46A0
void __fastcall ACT2Q6_UnitIterate_SetPrimaryGoalDone(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA4750
bool __fastcall ACT2Q6_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//D2Game.0x6FCA4820
void __fastcall ACT2Q6_UnitIterate_SetCustom1Flag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA48B0
bool __fastcall ACT2Q6_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA49C0
void __fastcall ACT2Q6_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA4AB0
void __fastcall ACT2Q6_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FC96C80
void __fastcall ACT2Q6_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA4D50
void __fastcall ACT2Q6_Callback02_NpcDeactivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0xFCA4E00
int32_t __fastcall ACT2Q6_UnitIterate_StatusCyclerEx(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA4E70
int32_t __fastcall ACT2Q6_UnitIterate_UpdateQuestStateFlags(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA4EC0
void __fastcall ACT2Q6_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA52D0
int32_t __fastcall ACT2Q6_UnitIterate_SetLeaveTownFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA53B0
int32_t __fastcall ACT2Q6_UnitIterate_SetPrimaryGoalDoneForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA5400
int32_t __fastcall ACT2Q6_UnitIterate_SetCompletionFlag(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA5470
void __fastcall ACT2Q6_Callback08_MonsterKilled(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA55E0
bool __fastcall ACT2Q6_TImer_StatusCycler(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA5620
int32_t __fastcall ACT2Q6_UnitIterate_SetCustom1FlagForPartyMembers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA56C0
void __fastcall ACT2Q6_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA5880
bool __fastcall ACT2Q6_SeqCallback(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA5900
void __fastcall ACT2Q6_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA5930
void __fastcall ACT2Q6_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA59F0
void __fastcall ACT2Q6_SendTombIdToClient(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA5B00
int32_t __fastcall ACT2Q6_GetObjectIdForArcaneThing(D2GameStrc* pGame);
//D2Game.0x6FCA5B20
void __fastcall D2GAME_WarpPlayerToTile_6FCA5B20(D2ObjOperateFnStrc* pOp);
//D2Game.0x6FCA5B60
void __fastcall OBJECTS_InitFunction37_Unused(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA5BE0
void __fastcall OBJECTS_InitFunction38_TyraelsDoor(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA5C30
int32_t __fastcall ACT2Q6_IsDurielsLairClosed(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA5C60
void __fastcall OBJECTS_InitFunction21_HoradricOrifice(D2ObjInitFnStrc* pOp);
//D2Game.0x6FCA5CE0
bool __fastcall ACT2Q6_Timer_UpdateObjects(D2GameStrc* pGame, D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA5F20
void __fastcall ACT2Q6_UnitIterate_DeleteHoradricItems(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA5FB0
int32_t __fastcall OBJECTS_OperateFunction25_StaffOrifice(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA60D0
void __fastcall ACT2Q6_DeleteAllHoradricItemsAndOpenTomb(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pObject);
//D2Game.0x6FCA6310
int32_t __fastcall ACT2Q6_IsInRangeOfTyrael(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA6370
void __fastcall ACT2Q6_UnitIterate_CheckDistanceToTyrael(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA6390
int32_t __fastcall ACT2Q6_ActivateTomb(D2GameStrc* pGame, D2CoordStrc* pCoord, D2RoomStrc** ppRoom);
