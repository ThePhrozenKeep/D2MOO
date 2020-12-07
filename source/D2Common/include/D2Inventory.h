#pragma once

#include "CommonDefinitions.h"


//D2Common.0x6FD8E210
BOOL __fastcall INVENTORY_RemoveItem(D2UnitStrc* pItem);
//D2Common.0x6FD8E4A0
D2ItemExtraDataStrc* __fastcall INVENTORY_GetItemExtraDataFromItem(D2UnitStrc* pItem);
//D2Common.0x6FD8E4C0 (#10240)
D2InventoryStrc* __stdcall INVENTORY_AllocInventory(void* pMemPool, D2UnitStrc* pOwner);
//D2Common.0x6FD8E520 (#10241)
void __stdcall INVENTORY_FreeInventory(D2InventoryStrc* pInventory);
//D2Common.0x6FD8E620 (#10244)
BOOL __stdcall INVENTORY_CompareWithItemsParentInventory(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD8E660 (#10243)
D2UnitStrc* __stdcall INVENTORY_RemoveItemFromInventory(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD8E6A0 (#10242)
BOOL __stdcall INVENTORY_PlaceItemInSocket(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nUnused);
//D2Common.0x6FD8E7A0 (#10277)
D2UnitStrc* __stdcall INVENTORY_GetFirstItem(D2InventoryStrc* pInventory);
//D2Common.0x6FD8E7C0 (#10278)
D2UnitStrc* __stdcall INVENTORY_GetLastItem(D2InventoryStrc* pInventory);
//D2Common.0x6FD8E7E0 (#10245)
BOOL __stdcall INVENTORY_GetFreePosition(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nInventoryRecordId, int* pFreeX, int* pFreeY, BYTE nPage);
//D2Common.0x6FD8EAF0
D2InventoryGridStrc* __fastcall INVENTORY_GetGrid(D2InventoryStrc* pInventory, int nInventoryGrid, D2InventoryGridInfoStrc* pInventoryGridInfo);
//D2Common.0x6FD8EC70
BOOL __fastcall INVENTORY_CanItemBePlacedAtPos(D2InventoryGridStrc* pInventoryGrid, int nX, int nY, BYTE nItemWidth, BYTE nItemHeight);
//D2Common.0x6FD8ECF0
BOOL __fastcall sub_6FD8ECF0(D2InventoryGridStrc* pInventoryGrid, int* pFreeX, int* pFreeY, BYTE nItemWidth, BYTE nItemHeight);
//D2Common.0x6FD8EE20
int __fastcall sub_6FD8EE20(D2InventoryGridStrc* pInventoryGrid, int nXPos, int nYPos, BYTE nItemWidth, BYTE nItemHeight);
//D2Common.0x6FD8EFB0
BOOL __fastcall sub_6FD8EFB0(D2InventoryGridStrc* pInventoryGrid, int* pFreeX, int* pFreeY, BYTE nItemWidth, BYTE nItemHeight);
//D2Common.0x6FD8F0E0
BOOL __fastcall sub_6FD8F0E0(D2InventoryGridStrc* pInventoryGrid, int* pFreeX, int* pFreeY, BYTE nItemWidth, BYTE nItemHeight);
//D2Common.0x6FD8F1E0 (#10246)
BOOL __stdcall INVENTORY_PlaceItemAtFreePosition(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nInventoryRecordId, BOOL bUnused, BYTE nPage, char* szFile, int nLine);
//D2Common.0x6FD8F250
BOOL __fastcall INVENTORY_PlaceItemInGrid(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nXPos, int nYPos, int nInventoryGrid, int nInventoryRecordId, BOOL bUnused);
//D2Common.0x6FD8F600 (#10247)
BOOL __stdcall INVENTORY_CanItemBePlaced(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nXPos, int nYPos, int nInventoryRecordId, D2UnitStrc** ppExchangeItem, unsigned int* pHoveredItems, BYTE nPage);
//D2Common.0x6FD8F780 (#10248)
BOOL __stdcall INVENTORY_CanItemsBeExchanged(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nXPos, int nYPos, int nInventoryRecordId, D2UnitStrc** ppItem, BYTE nPage, BOOL bCheckIfCube);
//D2Common.0x6FD8F930 (#10249)
BOOL __stdcall INVENTORY_PlaceItemAtInventoryPage(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nXPos, int nYPos, int nInventoryRecordId, BOOL bUnused, BYTE nPage);
//D2Common.0x6FD8F970 (#10250)
void __stdcall INVENTORY_Return(char* szFile, int nLine, D2InventoryStrc* pInventory, int nX, int nY, int nInventoryRecordId, BOOL bClient, BYTE nPage);
//D2Common.0x6FD8F980 (#10252)
D2UnitStrc* __stdcall INVENTORY_GetItemFromInventoryPage(D2InventoryStrc* pInventory, int nX, int nY, int* pX, int* pY, int nInventoryRecordId, BYTE nPage);
//D2Common.0x6FD8FAB0 (#10253)
BOOL __stdcall INVENTORY_PlaceItemInBodyLoc(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nBodyLoc);
//D2Common.0x6FD8FAE0 (#10257)
D2UnitStrc* __stdcall INVENTORY_GetItemFromBodyLoc(D2InventoryStrc* pInventory, int nBodyLoc);
//D2Common.0x6FD8FB20 (#10255)
void __stdcall INVENTORY_GetSecondWieldingWeapon(D2UnitStrc* pPlayer, D2InventoryStrc* pInventory, D2UnitStrc** ppItem, int nBodyLoc);
//D2Common.0x6FD8FBB0 (#10256)
BOOL __stdcall INVENTORY_CheckEquipmentForWeaponByClass(D2InventoryStrc* pInventory, int nWeaponClass);
//D2Common.0x6FD8FC60 (#10258)
D2UnitStrc* __stdcall INVENTORY_GetLeftHandWeapon(D2InventoryStrc* pInventory);
//D2Common.0x6FD8FD10 (#11301)
D2UnitStrc* __stdcall INVENTORY_GetSecondaryWeapon(D2InventoryStrc* pInventory);
//D2Common.0x6FD8FDD0 (#10259)
D2UnitStrc* __stdcall INVENTORY_GetCompositItem(D2InventoryStrc* pInventory, int nComponent);
//D2Common.0x6FD8FE80 (#10260)
int __stdcall INVENTORY_GetBodyLocFromEquippedItem(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD8FED0 (#11278)
int __stdcall INVENTORY_GetItemsXPosition(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD8FF20 (#10261)
void __stdcall INVENTORY_SetCursorItem(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD8FF80 (#10262)
D2UnitStrc* __stdcall INVENTORY_GetCursorItem(D2InventoryStrc* pInventory);
//D2Common.0x6FD8FFA0 (#10263)
D2UnitStrc* __stdcall INVENTORY_FindBackPackItemForStack(D2InventoryStrc* pInventory, D2UnitStrc* a2, D2UnitStrc* a3);
//D2Common.0x6FD90080 (#10264)
D2UnitStrc* __stdcall INVENTORY_FindEquippedItemForStack(D2InventoryStrc* pInventory, D2UnitStrc* a2, D2UnitStrc* a3);
//D2Common.0x6FD90130 (#10265)
D2UnitStrc* __stdcall INVENTORY_FindFillableBook(D2InventoryStrc* pInventory, D2UnitStrc* pScrolls, D2UnitStrc* a3);
//D2Common.0x6FD90230 (#10266)
BOOL __stdcall INVENTORY_PlaceItemInBeltSlot(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nSlot);
//D2Common.0x6FD902B0 (#10268)
BOOL __stdcall INVENTORY_HasSimilarPotionInBelt(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD90340 (#10269)
BOOL __stdcall INVENTORY_GetFreeBeltSlot(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int* pFreeSlotId);
//D2Common.0x6FD904F0 (#10270)
BOOL __stdcall INVENTORY_PlaceItemInFreeBeltSlot(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD90550 (#10271)
D2UnitStrc* __stdcall INVENTORY_GetItemFromBeltSlot(D2InventoryStrc* pInventory, int nSlotId);
//D2Common.0x6FD90590 (#10272)
BOOL __stdcall INVENTORY_GetUseableItemFromBeltSlot(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nSlotId, D2UnitStrc** ppItem);
//D2Common.0x6FD90690 (#10273)
BOOL __stdcall INVENTORY_GetEquippedShield(D2InventoryStrc* pInventory, D2UnitStrc** ppItem);
//D2Common.0x6FD90760 (#10274)
BOOL __stdcall INVENTORY_GetEquippedWeapon(D2InventoryStrc* pInventory, D2UnitStrc** ppItem, int* pBodyLoc, BOOL* pIsLeftHandItem);
//D2Common.0x6FD90850 (#10275)
BOOL __stdcall INVENTORY_HasBodyArmorEquipped(D2InventoryStrc* pInventory);
//D2Common.0x6FD908A0 (#10276)
BOOL __stdcall INVENTORY_IsItemBodyLocFree(D2InventoryStrc* pInventory, D2UnitStrc* pItem, int nBodyLoc, int nInventoryRecordId);
//D2Common.0x6FD90910 (#10279)
void __stdcall INVENTORY_RemoveInventoryItems(D2InventoryStrc* pInventory);
//D2Common.0x6FD90940 (#10280)
D2InventoryNodeStrc* __stdcall INVENTORY_GetTradeInventory(D2InventoryStrc* pInventory);
//D2Common.0x6FD90960 (#10281)
void __stdcall INVENTORY_FreeTradeInventory(D2InventoryStrc* pInventory);
//D2Common.0x6FD909B0 (#10282)
BOOL __stdcall INVENTORY_CheckForItemInTradeInventory(D2InventoryStrc* pInventory, int nItemId);
//D2Common.0x6FD909F0 (#10283)
void __stdcall INVENTORY_AddItemToTradeInventory(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD90AB0 (#10316)
int __stdcall D2Common_10316(D2CorpseStrc* pCorpse);
//D2Common.0x6FD90AC0 (#10284)
int __stdcall INVENTORY_GetItemCount(D2InventoryStrc* pInventory);
//D2Common.0x6FD90AE0 (#10285)
D2UnitStrc* __stdcall INVENTORY_GetBackPackItemByType(D2InventoryStrc* pInventory, int nItemType, D2UnitStrc* a3);
//D2Common.0x6FD90BC0 (#10286)
D2UnitStrc* __stdcall INVENTORY_GetEquippedItemByType(D2InventoryStrc* pInventory, int nItemType, D2UnitStrc* a3);
//D2Common.0x6FD90C80 (#10287)
D2UnitStrc* __stdcall INVENTORY_GetEquippedItemByCode(D2InventoryStrc* pInventory, int nItemCode, D2UnitStrc* a3);
//D2Common.0x6FD90D50 (#11306)
D2UnitStrc* __stdcall INVENTORY_GetBackPackItemByCode(D2InventoryStrc* pInventory, int nItemCode, D2UnitStrc* a3);
//D2Common.0x6FD90E20 (#10288)
int __stdcall INVENTORY_GetSetItemEquipCountByFileIndex(D2InventoryStrc* pInventory, int nItemFileIndex);
//D2Common.0x6FD90ED0 (#10289)
void __stdcall INVENTORY_UpdateWeaponGUIDOnInsert(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD90F80 (#10290)
void __stdcall INVENTORY_UpdateWeaponGUIDOnRemoval(D2InventoryStrc* pInventory, D2UnitStrc* pItem);
//D2Common.0x6FD91050 (#10291)
int __stdcall INVENTORY_GetWieldType(D2UnitStrc* pPlayer, D2InventoryStrc* pInventory);
//D2Common.0x6FD91140 (#10292)
void __stdcall INVENTORY_SetOwnerId(D2InventoryStrc* pInventory, int nOwnerId);
//D2Common.0x6FD91160 (#10293)
int __stdcall INVENTORY_GetOwnerId(D2InventoryStrc* pInventory);
//D2Common.0x6FD91190 (#10294)
void __stdcall INVENTORY_CreateCorpseForPlayer(D2InventoryStrc* pInventory, int nUnitId, int a3, int a4);
//D2Common.0x6FD91210 (#10295)
BOOL __stdcall INVENTORY_FreeCorpse(D2InventoryStrc* pInventory, int nUnitId, int a3);
//D2Common.0x6FD91290 (#10296)
D2CorpseStrc* __stdcall INVENTORY_GetFirstCorpse(D2InventoryStrc* pInventory);
//D2Common.0x6FD912B0 (#10297)
int __stdcall INVENTORY_GetCorpseCount(D2InventoryStrc* pInventory);
//D2Common.0x6FD912D0 (#10313)
D2CorpseStrc* __stdcall INVENTORY_GetNextCorpse(D2CorpseStrc* pCorpse);
//D2Common.0x6FDAFEA0 (#10314)
int __stdcall INVENTORY_GetUnitGUIDFromCorpse(D2CorpseStrc* pCorpse);
//D2Common.0x6FDB18D0 (#10315)
int __stdcall D2Common_10315(D2CorpseStrc* pCorpse);
//D2Common.0x6FD912F0 (#10298)
void __stdcall INVENTORY_GetItemSaveGfxInfo(D2UnitStrc* pPlayer, BYTE* a2, BYTE* pColor);
//D2Common.0x6FD915C0
void __fastcall sub_6FD915C0();
//D2Common.0x6FD917B0
void __fastcall sub_6FD917B0(D2UnitStrc* pUnit, BYTE* a2, BYTE* pColor, D2UnitStrc* pItem);
//D2Common.0x6FD91B60 (#10299)
int __stdcall D2Common_10299(D2UnitStrc* pUnit, int nBodyLoc, D2UnitStrc* pItem, BOOL bDontCheckReqs);
//D2Common.0x6FD91D50
int __fastcall sub_6FD91D50(D2UnitStrc* pPlayer, int a2, int nBodyLoc, D2UnitStrc* a3, D2UnitStrc* a4, D2UnitStrc* pItem, int nUnused);
//D2Common.0x6FD91E80
BOOL __fastcall sub_6FD91E80(D2UnitStrc* pUnit, D2UnitStrc* pItem1, D2UnitStrc* pItem2);
//D2Common.0x6FD92080 (#10304)
D2UnitStrc* __stdcall INVENTORY_GetNextItem(D2UnitStrc* pItem);
//Inlined at various places
D2UnitStrc* __stdcall INVENTORY_GetUnknownItem(D2UnitStrc* pItem);
//D2Common.0x6FD920C0 (#10305)
D2UnitStrc* __stdcall INVENTORY_UnitIsItem(D2UnitStrc* pItem);
//D2Common.0x6FD920E0 (#10306)
int __stdcall INVENTORY_GetItemGUID(D2UnitStrc* pItem);
//D2Common.0x6FD92100 (#10307)
int __stdcall INVENTORY_GetItemNodePage(D2UnitStrc* pItem);
//D2Common.0x6FD92140 (#10310)
D2UnitStrc* __stdcall INVENTORY_IsItemInInventory(D2InventoryStrc* pParentInventory, D2UnitStrc* pItem);
//D2Common.0x6FDAFEA0 (#10311)
D2InventoryNodeStrc* __stdcall INVENTORY_GetNextNode(D2InventoryNodeStrc* pNode);
//D2Common.0x6FD90AB0 (#10312)
int __stdcall INVENTORY_GetItemGUIDFromNode(D2InventoryNodeStrc* pNode);
//D2Common.0x6FD92180 (#10300)
BOOL __stdcall INVENTORY_RemoveAllItems(D2InventoryStrc* pInventory);
//D2Common.0x6FD921D0 (#10302)
BOOL __stdcall INVENTORY_CanItemsBeTraded(void* pMemPool, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2, BOOL* a4);
//D2Common.0x6FD923C0
BOOL __fastcall INVENTORY_CopyUnitItemsToTradeInventory(D2InventoryStrc* pInventory, D2UnitStrc* pUnit);
//D2Common.0x6FD92490
BOOL __fastcall INVENTORY_CanItemBePlacedInInventory(D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2InventoryStrc* pInventory);
//D2Common.0x6FD925E0
int __fastcall UNITS_GetXPosition(D2UnitStrc* pUnit);
//D2Common.0x6FD92610
int __fastcall UNITS_GetYPosition(D2UnitStrc* pUnit);
