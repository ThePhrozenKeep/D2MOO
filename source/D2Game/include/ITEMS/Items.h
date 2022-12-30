#pragma once

#include "D2Structs.h"
#include <Units/Units.h>

#include <DataTbls/ItemsTbls.h>
#include <DataTbls/MonsterTbls.h>

#pragma pack(1)

struct D2ItemDropStrc
{
	D2UnitStrc* pUnit;						//0x00
	D2SeedStrc* pSeed;						//0x04
	D2GameStrc* pGame;						//0x08
	int32_t nItemLvl;						//0x0C
	uint32_t unk0x10;						//0x10
	int32_t nId;							//0x14
	int32_t nSpawnType;						//0x18 [3 for ground spawn, 4 for inv spawn]
	int32_t nX;								//0x1C
	int32_t nY;								//0x20
	D2RoomStrc* pRoom;						//0x24
	uint16_t wUnitInitFlags;				//0x28
	uint16_t wItemFormat;					//0x2A [ptGame0x0x78]
	BOOL bForce;							//0x2C
	int32_t nQuality;						//0x30
	int32_t nQuantity;						//0x34
	int32_t nMinDur;						//0x38
	int32_t nMaxDur;						//0x3C
	int32_t nItemIndex;						//0x40
	uint32_t dwFlags1;						//0x44 - itemflag override (used when force is true)
	uint32_t dwSeed;						//0x48 - overrides the seed, used when force is true
	uint32_t dwItemSeed;					//0x4C - overrides the item seed when force is true
	int32_t eEarLvl;						//0x50
	int32_t nQtyOverride;					//0x54
	char szName[16];						//0x58
	int32_t nPrefix[3];						//0x68
	int32_t nSuffix[3];						//0x74
	uint32_t dwFlags2;						//0x80
};

#pragma pack()

//D2Game.0x6FC4D470
D2UniqueItemsTxt* __fastcall ITEMS_GetUniqueItemsTxtRecord(int32_t nUniqueItemId);
//D2Game.0x6FC4D4A0
uint32_t __fastcall ITEMS_HasUniqueBeenDroppedAlready(D2GameStrc* pGame, int32_t nUniqueItemId);
//D2Game.0x6FC4D4E0
int32_t __fastcall ITEMS_CanUniqueItemBeDropped(D2GameStrc* pGame, D2UnitStrc* pItem);
//D2Game.0x6FC4D5E0
int32_t __fastcall sub_6FC4D5E0(D2UnitStrc** ppItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC4D6B0
void __fastcall sub_6FC4D6B0(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC4D800
void __fastcall ITEMS_MakeEthereal(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC4D900
void __fastcall sub_6FC4D900(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC4DA10
int32_t __fastcall ITEMS_AssignCharmAffixes(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC4DB60
void __fastcall D2GAME_ITEMS_AssignSpecial_6FC4DB60(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC4DC20
void __fastcall sub_6FC4DC20(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC4DE00
int32_t __fastcall ITEMS_RollItemQuality(D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC4E1A0
void __fastcall sub_6FC4E1A0(D2UnitStrc** ppItem);
//D2Game.0x6FC4E1F0
void __fastcall sub_6FC4E1F0(D2GameStrc* pGame, D2ItemDropStrc* pItemDrop, D2UnitStrc** ppItem, int32_t nLowSeed);
//D2Game.0x6FC4E430
int32_t __fastcall sub_6FC4E430(D2GameStrc* pGame, D2ItemDropStrc* pItemDrop, D2UnitStrc** ppItem, uint32_t* pLowSeed);
//D2Game.0x6FC4E4D0
uint32_t __fastcall ITEMS_RollLimitedRandomNumber(D2SeedStrc* pSeed, int32_t nMax);
//D2Game.0x6FC4E520
int32_t __fastcall D2GAME_InitItemStats_6FC4E520(D2GameStrc* pGame, D2UnitStrc** ppUnit, D2ItemDropStrc* pItemDrop, int32_t bQuestItem);
//D2Game.0x6FC4EBF0
uint32_t __fastcall ITEMS_RollRandomNumber(D2SeedStrc* pSeed);
//D2Game.0x6FC4EC10
D2UnitStrc* __fastcall sub_6FC4EC10(D2GameStrc* pGame, D2RoomStrc* pRoom, BYTE* pBitstream, uint32_t nBufferSize, int32_t bCheckForHeader, D2ItemSaveStrc* pItemSave, uint32_t* pSize, uint32_t dwVersion);
//D2Game.0x6FC4ED80
D2UnitStrc* __fastcall D2GAME_CreateItemEx_6FC4ED80(D2GameStrc* pGame, D2ItemDropStrc* pItemDrop, int32_t a3);
//D2Game.0x6FC4F290
D2UnitStrc* __fastcall sub_6FC4F290(D2GameStrc* pGame, D2RoomStrc* pRoom, D2CoordStrc* pCoords, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC4F4A0
int32_t __fastcall sub_6FC4F4A0(D2GameStrc* pGame, D2SeedStrc* pSeed, int32_t nLevelId, int32_t a4, int32_t a5, int32_t a6);
//D2Game.0x6FC4F640
void __fastcall sub_6FC4F640(D2GameStrc* pGame, D2RoomStrc* pRoom, D2CoordStrc* pCoord);
//D2Game.0x6FC4F830
D2UnitStrc* __fastcall D2GAME_DropArmor_6FC4F830(D2GameStrc* pGame, D2RoomStrc* pRoom, D2CoordStrc* pCoords, int32_t a4, int32_t a5, D2UnitStrc* pUnit);
//D2Game.0x6FC4FA50
D2UnitStrc* __fastcall D2GAME_DropWeapon_6FC4FA50(D2GameStrc* pGame, D2RoomStrc* pRoom, D2CoordStrc* pCoords, int32_t a4, int32_t a5, D2UnitStrc* pUnit);
//D2Game.0x6FC4FCA0
D2UnitStrc* __fastcall sub_6FC4FCA0(D2GameStrc* pGame, D2RoomStrc* pRoom, D2CoordStrc* pCoord, int32_t a4, int32_t a5, D2UnitStrc* pUnit);
//D2Game.0x6FC4FEC0
D2UnitStrc* __fastcall D2GAME_DropItemAtUnit_6FC4FEC0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nQuality, int32_t* pItemLevel, D2ItemDropStrc* pItemDrop, int32_t a6, int32_t a7);
//D2Game.0x6FC501A0
D2UnitStrc* __fastcall D2GAME_CreateItemUnit_6FC501A0(D2UnitStrc* pPlayer, int32_t nItemId, D2GameStrc* pGame, int32_t nSpawnTarget, int32_t nQuality, int32_t us1, int32_t alw1, int32_t nItemLevel, int32_t us0, int32_t a1, int32_t alw0);
//D2Game.0x6FC502B0
void __fastcall sub_6FC502B0(D2GameStrc* pGame, D2UnitStrc* pItem);
//D2Game.0x6FC502E0
void __fastcall ITEMS_RemoveAll(D2GameStrc* pGame);
//D2Game.0x6FC50320
void __fastcall ITEMS_RemoveFromAllPlayers(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC50340
void __fastcall ITEMS_PlayerIterateCallback_RemoveItem(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg);
//D2Game.0x6FC503A0
int32_t __fastcall ITEMS_IsInPlayersInventory(D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2InventoryStrc* pInventory);
//D2Game.0x6FC50410
D2UnitStrc* __fastcall ITEMS_FindQuestItem(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t dwItemCode);
//D2Game.0x6FC504F0
int32_t __fastcall ITEMS_GetItemLevelForNewItem(D2UnitStrc* pUnit, int32_t nLevelId);
//D2Game.0x6FC50560
void __fastcall ITEMS_UpdateDurability(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC50820
void __fastcall ITEMS_FillItemDrop(D2GameStrc* pGame, D2ItemDropStrc* pItemDrop, D2UnitStrc* pItem);
//D2Game.0x6FC509F0
void __fastcall ITEMS_DropGoldPile(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t nValue);
//D2Game.0x6FC50C50
void __fastcall ITEMS_HandleGoldTransaction(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t nValue);
//D2Game.0x6FC50D80
void __fastcall ITEMS_DropPlayerEar(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC50F70
int32_t __fastcall ITEMS_GetHealthPotionDropCode(D2UnitStrc* pUnit);
//D2Game.0x6FC50FF0
int32_t __fastcall ITEMS_GetManaPotionDropCode(D2UnitStrc* pUnit);
//D2Game.0x6FC51070
D2UnitStrc* __fastcall ITEMS_Duplicate(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pOwner, int32_t bDuplicateSocketFillers);
//D2Game.0x6FC512C0
void __fastcall sub_6FC512C0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC51310
void __fastcall sub_6FC51310(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC51360
void __fastcall D2GAME_DropTC_6FC51360(D2GameStrc* pGame, D2UnitStrc* pMonster, D2UnitStrc* pPlayer, D2TCExShortStrc* pTCTxtRecord, int32_t nQuality, int32_t nLvl, int32_t a7, D2UnitStrc** ppItems, int32_t* pnItemsDropped, int32_t nMaxItems);
//D2Game.0x6FC52070
D2RoomStrc* __fastcall D2GAME_GetRoom_6FC52070(D2RoomStrc* pRoom, int32_t nSubtileX, int32_t nSubtileY);
//D2Game.0x6FC52110
void __fastcall sub_6FC52110(D2GameStrc* pGame, D2UnitStrc* pMonster, D2UnitStrc* pPlayer, int32_t nTCId, int32_t nQuality, int32_t nItemLevel, int32_t a7, D2UnitStrc** ppItems, int32_t* pnItemsDropped, int32_t nMaxItems);
//D2Game.0x6FC521D0
int32_t __fastcall ITEMS_GetGroundRemovalTime(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC52260
void __fastcall D2GAME_DropItem_6FC52260(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2RoomStrc* pRoom, int32_t nX, int32_t nY);
//D2Game.0x6FC52300
void __fastcall ITEMS_DeleteInactiveItems(D2GameStrc* pGame, int32_t nAct);
//D2Game.0x6FC523B0
void __fastcall ITEMS_DestroyRunewordStatList(D2GameStrc* pGame, D2UnitStrc* pItem);
//D2Game.0x6FC52410
void __fastcall sub_6FC52410(D2UnitStrc* pUnit, D2ItemDropStrc* pItemDrop);
//D2Game.0x6FC52650
void __fastcall sub_6FC52650(D2UnitStrc* pUnit, int32_t nItemLvl, int32_t nClassFirstSkillId, int32_t nSkillCount, int32_t nItemDropLvl);
