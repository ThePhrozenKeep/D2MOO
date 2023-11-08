#pragma once

#include <Units/Units.h>

#pragma pack(1)

struct D2MinionListStrc
{
	D2UnitGUID dwMinionGUID;						//0x00
	D2MinionListStrc* pNext;				//0x04
};

struct D2InactiveItemNodeStrc
{
	D2InactiveItemNodeStrc* pNext;			//0x00
	int32_t nFrame;								//0x04
	int32_t nOwnerId;							//0x08
	uint16_t nBitstreamSize;					//0x0C
	uint8_t pBitstream;						//0x0E - dynamic size dependent on item serialization
};

struct D2InactiveMonsterNodeStrc
{
	int32_t nX;									//0x00
	int32_t nY;									//0x04
	int32_t nClassId;							//0x08
	int32_t nUnitId;							//0x0C
	int32_t nUnitFlags;							//0x10
	int32_t nUnitFlagsEx;						//0x14
	int32_t nTypeFlags;							//0x18
	int32_t unk0x1C;							//0x1C
	D2MinionListStrc* pMinionList;			//0x20
	D2MapAIStrc* pMapAI;					//0x24
	int32_t nSpecialAiState;					//0x28
	int32_t nLevelId;							//0x2C
	uint16_t nNameSeed;							//0x30
	uint8_t nMonUMods[9];						//0x32
	uint8_t unk0x3B;							//0x3B
	uint16_t nBossHcIdx;						//0x3C
	uint16_t unk0x3E;							//0x3E
	int32_t nExperience;						//0x40
	int32_t nMaxHitpoints;						//0x44
	int32_t nHitpoints;							//0x48
	int32_t nCmdParam1;							//0x4C
	int32_t nCmdParam2;							//0x50
	int32_t nGameFrame;							//0x54
	D2InactiveMonsterNodeStrc* pNext;		//0x58
};

struct D2InactiveUnitNodeStrc
{
	int32_t nX;									//0x00
	int32_t nY;									//0x04
	int32_t nUnitType;							//0x08
	int32_t nClassId;							//0x0C
	int32_t nAnimMode;							//0x10
	int32_t nGameFrame;							//0x14
	int32_t nUnitFlags;							//0x18
	int32_t nUnitFlagsEx;						//0x1C
	int32_t nUnitId;							//0x20
	int32_t nFrame;								//0x24
	uint8_t nInteractType;						//0x28
	uint8_t unk0x29;							//0x29
	uint16_t unk0x2A;							//0x2A
	int32_t nDropItemCode;						//0x2C
	D2InactiveUnitNodeStrc* pNext;			//0x30
};

struct D2InactiveUnitListStrc
{
	int32_t nX;											//0x00
	int32_t nY;											//0x04
	D2InactiveItemNodeStrc* pInactiveItems;			//0x08
	D2InactiveMonsterNodeStrc* pInactiveMonsters;	//0x0C
	D2InactiveUnitNodeStrc* pInactiveUnits;			//0x10
	D2InactiveUnitListStrc* pNext;					//0x14
};

#pragma pack()

//D2Game.0x6FCC3850
void __fastcall SUNITINACTIVE_RestoreInactiveUnits(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FCC40D0
D2AiControlStrc* __fastcall AIGENERAL_GetAiControlFromUnit(D2UnitStrc* pUnit);
//D2Game.0x6FCC40F0
void __fastcall SUNITINACTIVE_FreeInactiveMonsterNode(D2GameStrc* pGame, D2InactiveMonsterNodeStrc* pInactiveMonsterNode);
//D2Game.0x6FCC4120
D2UnitStrc* __fastcall SUNITINACTIVE_RestoreInactiveItem(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2InactiveItemNodeStrc* pInactiveItemNode);
//D2Game.0x6FCC4270
void __fastcall SUNITINACTIVE_FreeInactiveUnitLists(D2GameStrc* pGame);
//D2Game.0x6FCC4370
void __fastcall SUNITINACTIVE_CompressUnitIfNeeded(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCC4650
void __fastcall SUNITINACTIVE_CompressInactiveUnit(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCC4C90
void __fastcall SUNITINACTIVE_DeleteSingleListNode(D2GameStrc* pGame, uint16_t nUnitType, uint16_t nClassId, uint8_t nAct);
//D2Game.0x6FCC4DC0
D2InactiveUnitListStrc* __fastcall SUNITINACTIVE_GetListNodeFromActAndCoordinates(D2GameStrc* pGame, int32_t nAct, int32_t nX, int32_t nY, int32_t bAllocNewNode);
//D2Game.0x6FCC4E80
D2InactiveUnitListStrc* __fastcall SUNITINACTIVE_GetListNodeFromRoom(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t bAllocNewNode);
//D2Game.0x6FCC4ED0
void __fastcall SUNITINACTIVE_RestoreSpecialMonsterParameters(D2GameStrc* pGame, D2UnitStrc* pUnit, D2InactiveMonsterNodeStrc* pInactiveMonsterNode);
//D2Game.0x6FCC50B0
void __fastcall SUNITINACTIVE_SaveSpecialMonsterParameters(D2GameStrc* pGame, D2UnitStrc* pUnit, D2InactiveMonsterNodeStrc* pInactiveMonsterNode);
//D2Game.0x6FCC52C0
int32_t __fastcall SUNITINACTIVE_IsUnitInsideRoom(D2GameStrc* pGame, D2ActiveRoomStrc* pRoomNear, int32_t nGameX, int32_t nGameY, int32_t nClassId);
//D2Game.0x6FCC5490
void __fastcall SUNITINACTIVE_DeleteExpiredItemNodes(D2GameStrc* pGame, int32_t nAct);
//D2Game.0x6FCC54F0
void __fastcall SUNITINACTIVE_SetUnitFlagEx(D2UnitStrc* pUnit, uint32_t nFlag, int32_t bSet);
