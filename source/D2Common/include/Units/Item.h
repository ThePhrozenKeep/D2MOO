#pragma once

#include <D2Inventory.h>

#pragma pack(1)

struct D2ItemDataStrc
{
	uint32_t dwQualityNo;						//0x00
	D2SeedStrc pSeed;						//0x04
	uint32_t dwOwnerGUID;						//0x0C
	uint32_t dwInitSeed;						//0x10
	uint32_t dwCommandFlags;					//0x14
	uint32_t dwItemFlags;						//0x18
	uint32_t dwRealmData[2];					//0x1C
	uint32_t dwActionStamp;					//0x24
	int32_t dwFileIndex;						//0x28
	uint32_t dwItemLevel;						//0x2C
	uint16_t wItemFormat;						//0x30
	uint16_t wRarePrefix;						//0x32
	uint16_t wRareSuffix;						//0x34
	uint16_t wAutoAffix;						//0x36
	uint16_t wMagicPrefix[3];					//0x38
	uint16_t wMagicSuffix[3];					//0x3E
	uint8_t nBodyLoc;							//0x44					// location on body = xpos of item (only if on body) (spread)
	uint8_t nInvPage;							//0x45					// 0 = inventory,  3= cube, 4=stash (spread)
	uint8_t nCellOverlap;						//0x46
	uint8_t nItemCell;							//0x47
	uint8_t nEarLvl;							//0x48
	uint8_t nInvGfxIdx;						//0x49
	char szPlayerName[16];					//0x4A
	uint8_t unk0x5A[2];						//0x5A
	D2ItemExtraDataStrc pExtraData;			//0x5C
	//D2InventoryStrc* pParentInv;			//0x5C
	//uint32_t unk0x60;							//0x60
	//D2UnitStrc* pNextItem;					//0x64
	//uint8_t nNodePos;							//0x68
	//uint8_t nNodePosOther;						//0x69
};

#pragma pack()
