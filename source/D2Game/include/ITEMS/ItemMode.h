#pragma once

#include <Units/Units.h>

#include <DataTbls/ItemsTbls.h>
#include "Items.h"
#include <GAME/Event.h>

#pragma pack(push, 1)
struct D2EquipmentRefreshStateStrc
{
	int32_t nHitpoints;
	int32_t nMana;
	int32_t nStamina;
	int32_t nRightSkillId;
	int32_t nLeftSkillId;
	int32_t nRightSkillOwnerGUID;
	int32_t nLeftSkillOwnerGUID;
};

struct D2WeaponSwitchItemStrc
{
	int32_t nBodyLoc;
	D2UnitStrc* pItem;
	int32_t nSwitchBodyLoc;
	int32_t nSocketArg;
};

struct D2EquipmentTraitsStrc
{
    int32_t bIsBow;
    int32_t bIsCrossBow;
    int32_t bIsBowQuiver;
    int32_t bIsCrossBowQuiver;
    int32_t bIsShield;
    int32_t bIsWeapon;
    int32_t bIsTwoHanded;
    int32_t bCanUse2Weapons;
    int32_t bIsThrowable;
    int32_t bIsRing;
};
#pragma pack(pop)


//D2Game.0x6FC41900
//1.14d: 0x00564CA0
int32_t __fastcall D2GAME_Return1_6FC41900();
//D2Game.0x6FC41910
//1.14d: 0x0055B800
void __fastcall D2GAME_ITEMMODE_ServerStatlistCallback_6FC41910(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pOther, int32_t nLayer_StatId, int32_t nPreviousValue, int32_t nNewValue);
//D2Game.0x6FC42050
//1.14d: 0x0055BED0
void __fastcall ITEMMODE_SendGroundItemToClient(D2UnitStrc* pItem, D2ClientStrc* pClient);
//D2Game.0x6FC420B0
//1.14d: 0x0055BF30
void __fastcall ITEMMODE_SendUpdatedGroundItemToClient(D2UnitStrc* pItem, D2ClientStrc* pClient);
//D2Game.0x6FC42120
//1.14d: 0x0055C110
int32_t __fastcall ITEMMODE_UpdateBookScrollSkillQuantity(D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t bAddQuantity);
//D2Game.0x6FC42310
//1.14d: 0x0055C2C0
void __fastcall D2GAME_ITEMS_UpdateItemStatlist_6FC42310(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pUnit, int32_t bAllowBrokenItemStats);
//D2Game.0x6FC424E0
//1.14d: 0x0055C730
void __fastcall D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t bUpdateStatList, int32_t bUpdateVitals);
//D2Game.0x6FC425F0
//1.14d: 0x0055CC90
int32_t __fastcall ITEMMODE_CanPickUpItem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC428F0
//1.14d: 0x0055CA40
int32_t __fastcall ITEMMODE_HasConflictingQuestOrUniqueItem(D2UnitStrc* pItem, D2ItemsTxt* pItemsTxtRecord, D2UnitStrc* pPreviousItem);
//D2Game.0x6FC42B80
//1.14d: 0x0055CF50
void __fastcall D2GAME_PickupItemEx_6FC42B80(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t* pItemStatus);
//D2Game.0x6FC42DD0
//1.14d: 0x0055C850
void __fastcall D2GAME_PickupGold_6FC42DD0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pGoldPile);
//D2Game.0x6FC42F20
//1.14d: 0x0055D710
int32_t __fastcall ITEMMODE_FindFreeBodyLocation(D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t* pBodyLoc, int32_t bSkipRequirementCheck);
//D2Game.0x6FC43160
//1.14d: 0x0055D560
void __fastcall ITEMMODE_BuildEquipmentTraits(D2UnitStrc* pUnit, D2UnitStrc* pItem, D2EquipmentTraitsStrc* pTraits);
//D2Game.0x6FC43280
//1.14d: 0x0055D670
int32_t __fastcall ITEMMODE_AreEquipmentTraitsCompatible(D2EquipmentTraitsStrc* pTraits1, D2EquipmentTraitsStrc* pTraits2);
//D2Game.0x6FC43340
//1.14d: 0x00563560
int32_t __fastcall D2GAME_PickupItem_6FC43340(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t* pItemStatus);
//D2Game.0x6FC437F0
//1.14d: 0x0055D0D0
int32_t __fastcall ITEMMODE_AutoStackPickup(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC43AF0
//1.14d: 0x0055C070
void __fastcall ITEMMODE_AddBookScrollSkillQuantity(D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t nQuantityBonus);
//D2Game.0x6FC43BF0
//1.14d: 0x0055D370
int32_t __fastcall ITEMMODE_TryMergeBookQuantity(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC43E60
//1.14d: 0x005600A0
int32_t __fastcall ITEMMODE_PlaceItemAtFreeInventoryPosition(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t bRemove, int32_t nPage, const char* szFile, int32_t nLine);
//D2Game.0x6FC44030
//1.14d: 0x00563C00
void __fastcall ITEMMODE_DropCursorItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID);
//D2Game.0x6FC44410
//1.14d: 0x00560200
int32_t __fastcall D2GAME_PlaceItem_6FC44410(const char* szFile, int32_t nLine, D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nUnitId, int32_t nX, int32_t nY, int32_t bFindFreePosition, int32_t bSendUpdate, D2InventoryStrc* pInventory);
//D2Game.0x6FC446B0
//1.14d: 0x00560420
int32_t __fastcall ITEMMODE_RemoveStoredItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t* pItemStatus, int32_t nUpdateMode, int32_t bSetQuantityFlag, D2InventoryStrc* pInventoryArg, int32_t bAllowOccupiedCursor);
//D2Game.0x6FC44A90
//1.14d: 0x0055DBC0
void __fastcall D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t bRefresh);
//D2Game.0x6FC44F00
void __fastcall ITEMMODE_SaveEquipmentRefreshState(D2UnitStrc* pUnit, D2EquipmentRefreshStateStrc* pRefreshState);
//D2Game.0x6FC44FB0
//1.14d: 0x0055DA70
void __fastcall ITEMMODE_RestoreSelectedSkills(D2UnitStrc* pUnit, D2EquipmentRefreshStateStrc* pRefreshState);
//D2Game.0x6FC45050
//1.14d: 0x0055DF00
void __fastcall D2GAME_ITEMS_UpdateInventoryItems_6FC45050(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUnused, int32_t bRefresh);
//D2Game.0x6FC45060
//1.14d: 0x005606B0
int32_t __fastcall ITEMMODE_EquipCursorItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nItemGUID, uint8_t nBodyLoc, int32_t bSkipRequirementCheck, int32_t* pItemStatus);
//D2Game.0x6FC45300
//1.14d: 0x0055C5C0
void __fastcall ITEMMODE_RefreshSkillsForEquippedWeapons(D2UnitStrc* pUnit);
//D2Game.0x6FC45550
//1.14d: 0x00563D20
int32_t __fastcall ITEMMODE_ReplaceOppositeHandItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, uint8_t nBodyLoc, int32_t bSkipRequirementCheck, int32_t* pItemStatus);
//D2Game.0x6FC45930
//1.14d: 0x005608C0
void __fastcall ITEMMODE_MoveExcessBeltItems(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC45B30
//1.14d: 0x00560CD0
int32_t __fastcall ITEMMODE_UnequipBodyItem(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nBodyLoc, int32_t bSetQuantityFlag, int32_t* pItemStatus);
//D2Game.0x6FC45E60
//1.14d: 0x00560F00
int32_t __fastcall ITEMMODE_SwapCursorAndBodyItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, uint8_t nBodyLoc, int32_t bSkipRequirementCheck, int32_t* pItemStatus);
//D2Game.0x6FC46270
//1.14d: 0x00561220
int32_t __fastcall ITEMMODE_ReplaceHandItemsWithCursorItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, uint8_t nBodyLoc, int32_t* pItemStatus);
//D2Game.0x6FC46840
//1.14d: 0x005616A0
int32_t __fastcall ITEMMODE_SwapWeaponSets(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* pItemStatus);
//D2Game.0x6FC46D40
//1.14d: 0x00561B00
int32_t __fastcall ITEMMODE_SwapCursorAndStoredItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCursorItemGUID, int32_t nGridItemGUID, int32_t nX, int32_t nY, int32_t* pItemStatus);
//D2Game.0x6FC471F0
//1.14d: 0x0055DF10
void __fastcall D2GAME_RemoveItem_6FC471F0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t bSendPacket);
//D2Game.0x6FC47380
//1.14d: 0x00561E70
void __fastcall ITEMMODE_RemoveBeltItem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC47470
//1.14d: 0x00561ED0
int32_t __fastcall ITEMMODE_UseItemOnItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTargetGUID, int32_t nUseItemGUID, int32_t* pItemStatus);
//D2Game.0x6FC47C90
//1.14d: 0x0055E0D0
void __fastcall ITEMMODE_DecrementBookScrollSkillQuantity(D2UnitStrc* pUnit, int32_t nSkillId);
//D2Game.0x6FC47D30
//1.14d: 0x0055E170
int32_t __fastcall ITEMMODE_UseStoredItemAtCoordinates(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t nX, int32_t nY, int32_t* pItemStatus);
//D2Game.0x6FC484E0
//1.14d: 0x0055E7C0
int32_t __fastcall ITEMMODE_MergeItemStacks(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID1, int32_t nItemGUID2, int32_t* pItemStatus);
//D2Game.0x6FC48930
//1.14d: 0x0055E9A0
int32_t __stdcall D2GAME_Return0_6FC48930(int32_t nUnused, int32_t* pUnused);
//D2Game.0x6FC48940
//1.14d: 0x0055E9B0
int32_t __fastcall ITEMMODE_PlaceCursorItemInBelt(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t nBeltSlot, int32_t bFindFreeSlot, int32_t* pItemStatus);
//D2Game.0x6FC48B40
//1.14d: 0x00562250
int32_t __fastcall ITEMMODE_RemoveBeltItemToCursor(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t* pItemStatus);
//D2Game.0x6FC48D50
//1.14d: 0x0055EB30
int32_t __fastcall ITEMMODE_SwapCursorAndBeltItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCursorItemGUID, int32_t nBeltItemGUID, int32_t* pItemStatus);
//D2Game.0x6FC49090
//1.14d: 0x0055ED30
void __fastcall ITEMMODE_RemoveInventoryItemFromAllPlayers(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC49140
//1.14d: 0x0055EDC0
void __fastcall ITEMMODE_CompactBeltColumn(D2GameStrc* pGame, D2UnitStrc* pUnit, int8_t nRemovedSlot);
//D2Game.0x6FC49220
//1.14d: 0x00562390
int32_t __fastcall ITEMMODE_UseBeltItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t nX, int32_t nY, int32_t* pItemStatus, int32_t bUseOnMerc);
//D2Game.0x6FC49670
//1.14d: 0x00562590
void __fastcall D2GAME_ITEMS_Identify_6FC49670(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC49760
//1.14d: 0x0055EEA0
int32_t __fastcall D2GAME_RemoveItemIfOnCursor_6FC49760(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC497E0
int32_t __fastcall D2GAME_ITEMSOCKET_PlaceItem_6FC497E0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSocketFillerGUID, int32_t nItemGUID, int32_t* bPlaced, int32_t bRefresh, int32_t bResetCursorItem, int32_t bCheckMode);
//D2Game.0x6FC49AE0
//1.14d: 0x0055EF20
int32_t __fastcall ITEMMODE_AddScrollToBook(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nScrollGUID, int32_t nBookGUID, int32_t* pItemStatus);
//D2Game.0x6FC49DC0
//1.14d: 0x005628C0
int32_t __fastcall ITEMMODE_PlaceItemInCube(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t nCubeGUID, int32_t* pItemStatus);
//D2Game.0x6FC49F80
//1.14d: 0x00562A30
int32_t __fastcall ITEMMODE_EquipStoredItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, uint8_t nBodyLoc);
//D2Game.0x6FC4A2E0
//1.14d: 0x00562D30
void __fastcall ITEMMODE_ReplenishItem(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC4A350
//1.14d: 0x00562C40
int32_t __fastcall ITEMMODE_ReplenishItemStat(D2GameStrc* pGame, D2UnitStrc* pItem, int32_t nLengthStatId, int32_t nValueStatId, int32_t nMaxValue);
//D2Game.0x6FC4A450
//1.14d: 0x0055F130
void __fastcall ITEMMODE_UpdateItemStatListExpiration(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC4A460
//1.14d: 0x00562DA0
void __fastcall D2GAME_Items_EventsHandler_6FC4A460(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType);
//D2Game.0x6FC4A4B0
//1.14d: 0x0055F140
int32_t __fastcall D2GAME_DoKeyCheck_6FC4A4B0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC4A660
//1.14d: 0x00563FE0
int32_t __fastcall D2GAME_Transmogrify_6FC4A660(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC4A9B0
//1.14d: 0x00562E00
int32_t __fastcall ITEMMODE_EquipItemAtResolvedBodyLocation(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, uint8_t bSkipRequirementCheck);
//D2Game.0x6FC4AB10
//1.14d: 0x0055F2D0
int32_t __fastcall ITEMMODE_CheckAndResolveBodySlotPair(D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2UnitStrc** ppBodyItem, D2UnitStrc** ppOtherBodyItem, int32_t* pBodyLoc);
//D2Game.0x6FC4AD80
//1.14d: 0x00562F30
int32_t __fastcall ITEMMODE_RecoverCorpseItems(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pCorpse);
//D2Game.0x6FC4B240
//1.14d: 0x0055F500
void __fastcall D2GAME_UpdatePlayerVitals_6FC4B240(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC4B2D0
//1.14d: 0x0055F590
int32_t __fastcall D2GAME_CheckHasFilledSockets_6FC4B2D0(D2UnitStrc* pItem, const char* szFile, int32_t nLine);
//D2Game.0x6FC4B310
//1.14d: 0x00563350
int32_t __fastcall ITEMMODE_DuplicateSocketFillers(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pSocketable, int32_t bRefresh);
//D2Game.0x6FC4B430
//1.14d: 0x0055F640
D2UnitStrc* __fastcall ITEMMODE_FindMatchingBookWithSpace(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t* pFreeQuantity);
//D2Game.0x6FC4B520
//1.14d: 0x0055F6E0
void __fastcall ITEMMODE_AddBookQuantity(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t nValue);
//D2Game.0x6FC4B580
//1.14d: 0x0055F850
void __fastcall ITEMMODE_BreakItem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC4B630
//1.14d: 0x0055F900
void __fastcall D2GAME_RepairBrokenItem_6FC4B630(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC4B740
//1.14d: 0x0055F9C0
void __fastcall ITEMMODE_ResetBookScrollSkillQuantity(D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC4B830
//1.14d: 0x0055FA40
void __fastcall ITEMMODE_RebuildBookScrollSkillQuantities(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC4B9D0
//1.14d: 0x0055FB10
int32_t __fastcall ITEMMODE_MarkItemOnCursor(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC4BA50
//1.14d: 0x0055FCC0
void __fastcall ITEMMODE_RemoveInventoryItemProperties(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC4BB90
//1.14d: 0x0055FDE0
void __fastcall D2GAME_UpdatePlayerItems_6FC4BB90(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t bRefresh);
//D2Game.0x6FC4BBB0
//1.14d: 0x0055FE00
void __fastcall ITEMMODE_IdentifyItemAndApplyCharmProperties(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pPlayer);
//D2Game.0x6FC4BC00
//1.14d: 0x0055FE60
void __fastcall D2GAME_ITEMS_UpdateStatList_6FC4BC00(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pUnit);
//D2Game.0x6FC4BC10
//1.14d: 0x00563470
void __fastcall ITEMMODE_DetachItemFromOwner(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC4BD50
//1.14d: 0x0055FE80
int32_t __fastcall D2GAME_RechargeItem_6FC4BD50(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FC4BE80
//1.14d: 0x00555D20
void __fastcall ITEMMODE_InitializeItemUnit(D2UnitStrc* pUnit, D2GameStrc* pGame, int32_t nUnitGUID, D2ActiveRoomStrc* pRoom);
//D2Game.0x6FC4BF00
//1.14d: 0x00555DA0
D2ActiveRoomStrc* __fastcall D2GAME_GetFreeSpaceEx_6FC4BF00(D2ActiveRoomStrc* pRoom, D2CoordStrc* pCoords, D2CoordStrc* pReturnCoords, int32_t nUnitSize);
//D2Game.0x6FC4BFF0
//1.14d: 0x00555E70
int32_t __fastcall ITEMMODE_SelectRandomArmorItemId(D2GameStrc* pGame, D2SeedStrc* pSeed, int32_t nLevelId, int32_t nItemType, int32_t bIgnoreRarity);
//D2Game.0x6FC4C1E0
//1.14d: 0x00555FB0
int32_t __fastcall ITEMMODE_SelectRandomWeaponItemId(D2GameStrc* pGame, D2SeedStrc* pSeed, int32_t nLevelId, int32_t nItemType, int32_t bIgnoreRarity);
//D2Game.0x6FC4C3D0
//1.14d: 0x005560F0
int32_t __fastcall ITEMMODE_SelectRandomMiscItemId(D2GameStrc* pGame, D2SeedStrc* pSeed, int32_t nLevelId, int32_t nItemType, int32_t bIgnoreRarity, int32_t bAllowBodyParts);
//D2Game.0x6FC4C5F0
//1.14d: 0x00557450
int32_t __fastcall ITEMMODE_AssignItemQualityAndProperties(D2GameStrc* pGame, D2UnitStrc** ppItem, D2ItemDropStrc* pItemDrop);
