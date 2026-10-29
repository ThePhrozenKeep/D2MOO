#pragma once

#include <Units/Units.h>
#include "SUnitProxy.h"

#pragma pack(1)

struct D2NpcTradeStrc
{
	struct
	{
		bool bVendorInit;		//0x00
		bool bHireInit;			//0x01
		uint8_t nAct;			//0x02
		bool bTrader;			//0x03
		bool bLevelRefresh;		//0x04
		bool bInited;			//0x05
		bool bForceVendor;		//0x06
		bool bRefreshInventory;	//0x07
	};

	uint32_t dwTicks;				//0x08
	D2UnitProxyStrc pProxy;			//0x0C
	uint32_t dwUnk;					//0x1C
	D2UnitGUID dwNPCGUID;			//0x20
};

struct D2NpcControlStrc
{
	int32_t nArraySize;							//0x00
	D2NpcRecordStrc* pFirstRecord;			//0x04
	D2SeedStrc pSeed;						//0x08
	int32_t unk0x10;							//0x10
};

struct D2NpcGambleStrc
{
	D2InventoryStrc* pInventory;			//0x00
	D2UnitGUID dwGUID;							//0x04
	D2NpcGambleStrc* pNext;					//0x08
};

struct D2NPCMessageListStrc
{
	uint16_t nMessageIndexes[8];				//0x00
};

struct D2NpcVendorChainStrc
{
	int dwGUID;
	uint8_t field_4;
	uint8_t unk0x05[3];
	D2NpcVendorChainStrc* pNext;
};

struct D2NpcEventStrc
{
	D2UnitStrc* pUnit;
	int32_t field_4;
	int32_t field_8;
	int32_t field_C;
	D2NpcEventStrc* pNext;
};

struct D2MercDataStrc
{
	int16_t nMercName;						//0x00
	int16_t nPad;								//0x02
	uint32_t dwSeed;							//0x04
	BOOL bHired;							//0x08
	BOOL bAvailable;						//0x0C
};

struct D2NpcRecordStrc
{
	int32_t nNPC;							//0x00
	D2InventoryStrc* pInventory;			//0x04
	D2NpcGambleStrc* pGamble;				//0x08
	BOOL bGambleInit;						//0x0C
	D2MercDataStrc* pMercData;				//0x10
	D2NpcEventStrc* pEvent;					//0x14
	D2NpcVendorChainStrc* pVendorChain;		//0x18
	BOOL bTrading;							//0x1C
	D2NpcTradeStrc npcTrade;				//0x20
};

#pragma pack()

//D2Game.0x6FCC67D0
//1.14d: 0x00576070
void __fastcall SUNITNPC_InitializeHirelingRoster(D2GameStrc* pGame, int32_t nVendorId, D2NpcRecordStrc* pNpcRecord);
//D2Game.0x6FCC6970
//1.14d: 0x005761C0
void __fastcall D2GAME_NPC_RepairItem_6FCC6970(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pPlayer);
//D2Game.0x6FCC6A60
//1.14d: 0x00576330
D2UnitStrc* __fastcall D2GAME_NPC_GenerateStoreItem_6FCC6A60(D2UnitStrc* pNpc, int32_t szCode, D2GameStrc* pGame, int32_t bVendorItem, int32_t nQuality, int32_t nItemLevel, int32_t nPlayerLevel);
//D2Game.0x6FCC6F10
//1.14d: 0x005766D0
void __fastcall SUNITNPC_RemoveStoreItem(D2UnitStrc* pNpc, D2UnitStrc* pItem, D2GameStrc* pGame, int32_t bInit);
//D2Game.0x6FCC6FF0
//1.14d: 0x00576770
void __fastcall D2GAME_NPC_BuildHirelingList_6FCC6FF0(D2GameStrc* pGame, D2ClientStrc* pClient, D2UnitStrc* pUnit, int32_t nUnusedArg);
//D2Game.0x6FCC7100
//1.14d: 0x00576980
void __fastcall D2GAME_NPC_FillStoreInventory_6FCC7100(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, D2NpcTradeStrc* pTrade);
//D2Game.0x6FCC74F0
//1.14d: 0x00579430
void __fastcall D2GAME_STORES_CreateVendorCache_6FCC74F0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNPC, int32_t bRefreshInventory, int32_t bGamble);
//D2Game.0x6FCC7680
//1.14d: 0x00579510
int32_t __fastcall D2GAME_STORES_SellItem_6FCC7680(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcGUID, int32_t nItemGUID, int16_t nItemMode, int32_t nUnusedArg);
//D2Game.0x6FCC7E20
//1.14d: 0x00576F50
void __fastcall SUNITNPC_RemovePurchasedItemFromStore(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pItem, D2UnitStrc* pPlayer, int32_t nTransactionType);
//D2Game.0x6FCC7FA0
//1.14d: 0x005770E0
void __fastcall SUNITNPC_HireMercenaryFromRoster(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, uint16_t wName);
//D2Game.0x6FCC8430
//1.14d: 0x005773D0
void __fastcall SUNITNPC_UpdateHirelingPosition(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCC84D0
//1.14d: 0x00579AA0
void __fastcall SUNITNPC_ResurrectHireling(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet);
//D2Game.0x6FCC8630
//1.14d: 0x005774F0
D2UnitStrc* __fastcall D2GAME_MERCS_Create_6FCC8630(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint16_t wName, uint32_t nSeed, int16_t wVersion, int32_t nBaseMonster, int32_t bDead);
//D2Game.0x6FCC87C0
//1.14d: 0x00577700
D2UnitStrc* __fastcall SUNITNPC_FindStackForStorePurchase(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem, int32_t* pFreeStackCount);
//D2Game.0x6FCC88B0
//1.14d: 0x00577830
int32_t __fastcall SUNITNPC_BuyItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, int32_t nItemGUID, int32_t nItemMode, uint16_t nTransactionType, int32_t nCost, int32_t bMultibuy);
//D2Game.0x6FCC92A0
//1.14d: 0x00577F30
int32_t __fastcall D2GAME_NPC_BuyItemHandler_6FCC92A0(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcUnitId, int32_t nItemId, int32_t nItemMode, uint16_t nTransactionType, int32_t nCost, int32_t bMultibuy);
//D2Game.0x6FCC9350
//1.14d: 0x00579C00
void __fastcall D2GAME_NPC_ResurrectMerc_6FCC9350(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcUnitId);
//D2Game.0x6FCC9540
//1.14d: 0x00577FE0
void __fastcall D2GAME_NPC_HireMerc_6FCC9540(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcUnitId, uint16_t nMercName);
//D2Game.0x6FCC95B0
//1.14d: 0x00578050
int32_t __fastcall D2GAME_NPC_Repair_6FCC95B0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nNpcGUID, int32_t nItemGUID, int32_t nUnused, int32_t nRepairFlags);
//D2Game.0x6FCC9C90
//1.14d: 0x00578460
void __fastcall D2GAME_NPC_IdentifyAllItems_6FCC9C90(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcGUID);
//D2Game.0x6FCC9F40
//1.14d: 0x00579D60
int32_t __fastcall NPC_HandleDialogMessage(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nType, int32_t nNpcGUID, int32_t nItemGUID);
//D2Game.0x6FCCA990
//1.14d: 0x00578640
void __fastcall D2GAME_NPC_IdentifyBoughtItem_6FCCA990(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nItemGUID);
//D2Game.0x6FCCA9F0
//1.14d: 0x00578790
void __fastcall D2GAME_STORES_FillGamble_6FCCA9F0(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pUnit, D2NpcRecordStrc* pNpcRecord);
//D2Game.0x6FCCAE20
//1.14d: 0x00578B30
void __fastcall D2GAME_STORES_CreateVendorCache_6FCCAE20(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, int32_t bGamble);
//D2Game.0x6FCCAF30
//1.14d: 0x00578C20
int32_t __fastcall D2GAME_NPC_RemoveStates_6FCCAF30(D2UnitStrc* pUnit);
//D2Game.0x6FCCAFA0
//1.14d: 0x00578CA0
void __fastcall SUNITNPC_PetIterate_Heal(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, void* pPlaySoundArg);
//D2Game.0x6FCCB080
//1.14d: 0x00578D30
void __fastcall D2GAME_NPC_HealPlayer_6FCCB080(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pNpc);
//D2Game.0x6FCCB220
//1.14d: 0x00578E70
void __fastcall D2GAME_NPC_Heal_6FCCB220(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pNpc);
//D2Game.0x6FCCB280
//1.14d: 0x00578ED0
void __fastcall D2GAME_NPC_PurchaseHeal_6FCCB280(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nNpcGUID);
//D2Game.0x6FCCB4D0
//1.14d: 0x00579130
void __fastcall D2GAME_NPC_ResetInteract_6FCCB4D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc);
//D2Game.0x6FCCB520
//1.14d: 0x00579180
void __fastcall D2GAME_NPC_AssignMercenary_6FCCB520(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMonster);
//D2Game.0x6FCCB7D0
//1.14d: 0x00579360
int32_t __fastcall D2GAME_NPC_IsItemInNpcInventory_6FCCB7D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, D2UnitStrc* pItem, int32_t bRemoveFromShop);
