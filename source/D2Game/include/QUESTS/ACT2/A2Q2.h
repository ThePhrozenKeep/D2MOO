#pragma once

#include <QUESTS/Quests.h>

#pragma pack(1)

//HoradricStaff
struct D2Act2Quest2Strc						//sizeof 0x30
{
	int unused0x00[5];						//0x00
	int nItemDropCount;						//0x14
	int nBaseStaffs;						//0x18
	int nCubedStaffs;						//0x1C
	int nHoradricCubes;						//0x20
	int nAmulets;							//0x24
	uint8_t bBaseStaffDropped;				//0x28
	uint8_t bCubeDropped;					//0x29
	uint8_t bStaffCubed;					//0x2A
	uint8_t bStaffMissingDetected;			//0x2B TODO
	int nPlayerGUID;						//0x2C
};

#pragma pack()



//D2Game.0x6FCA01F0
bool __fastcall ACT2Q2_ActiveFilterCallback(D2QuestDataStrc* pQuest, int32_t nNpcId, D2UnitStrc* pPlayer, D2BitBufferStrc* pQuestFlags, D2UnitStrc* pNPC);
//D2Game.0x6FCA0240
bool __fastcall ACT2Q2_CheckItemsAndState(D2QuestDataStrc* pQuestData, D2UnitStrc* pUnit, uint8_t* pData);
//D2Game.0x6FCA0370
//1.14d: 0x0059E5C0
void __fastcall ACT2Q2_UpdateHoradricItemCounts(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA0490
//1.14d: 0x0059E630
bool __fastcall ACT2Q2_StatusFilterCallback(D2QuestDataStrc* pQuest, D2UnitStrc* pPlayer, D2BitBufferStrc* pGlobalFlags, D2BitBufferStrc* pFlags, uint8_t* pStatus);
//D2Game.0x6FCA06C0
//1.14d: 0x0059EB00
void __fastcall ACT2Q2_InitQuestData(D2QuestDataStrc* pQuestData);
//D2Game.0x6FCA07A0
//1.14d: 0x005997F0
void __fastcall ACT2Q2_Callback11_ScrollMessage(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA08C0
//1.14d: 0x00599910
void __fastcall ACT2Q2_Callback00_NpcActivate(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA09C0
//1.14d: 0x00599A00
void __fastcall ACT2Q2_Callback03_ChangedLevel(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA09D0
//1.14d: 0x00599A30
void __fastcall ACT2Q2_Callback04_ItemPickedUp(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA0CF0
//1.14d: 0x00599510
int32_t __fastcall ACT2Q2_ShouldUpdateQuestStatus(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCA0D50
//1.14d: 0x00599B30
void __fastcall ACT2Q2_Callback05_ItemDropped(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA0DF0
//1.14d: 0x0059E850
void __fastcall ACT2Q2_Callback13_PlayerStartedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA0F10
//1.14d: 0x0059E970
void __fastcall ACT2Q2_Callback14_PlayerJoinedGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA0FC0
//1.14d: 0x0059EA20
void __fastcall ACT2Q2_Callback09_PlayerDroppedWithQuestItem(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
//D2Game.0x6FCA10B0
//1.14d: 0x00599C10
int32_t __fastcall OBJECTS_OperateFunction40_HoradricScrollChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA1180
//1.14d: 0x00599700
int32_t __fastcall ACT2Q2_UnitIterate_DetermineHoradricScrollDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA11D0
//1.14d: 0x00599CF0
int32_t __fastcall OBJECTS_OperateFunction41_StaffOfKingsChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA12C0
//1.14d: 0x00599750
int32_t __fastcall ACT2Q2_UnitIterate_DetermineStaffDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//D2Game.0x6FCA1330
//1.14d: 0x00599DF0
int32_t __fastcall OBJECTS_OperateFunction39_HoradricCubeChest(D2ObjOperateFnStrc* pOp, int32_t nOperate);
//D2Game.0x6FCA1410
//1.14d: 0x005997C0
int32_t __fastcall ACT2Q2_UnitIterate_DetermineCubeDropCount(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pData);
//
//1.10f: D2Game.0x6FCA4D40
//1.14d: 0x00599A10
void __fastcall ACT2Q2_Callback10_PlayerLeavesGame(D2QuestDataStrc* pQuestData, D2QuestArgStrc* pQuestArg);
