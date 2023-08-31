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
void __fastcall D2GAME_NPC_FirstFn_6FCC67D0(D2GameStrc* pGame, int32_t nVendorId, D2NpcRecordStrc* pNpcRecord);
//D2Game.0x6FCC6970
void __fastcall D2GAME_NPC_RepairItem_6FCC6970(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pPlayer);
//D2Game.0x6FCC6A60
D2UnitStrc* __fastcall D2GAME_NPC_GenerateStoreItem_6FCC6A60(D2UnitStrc* pNpc, int32_t szCode, D2GameStrc* pGame, int32_t a4, int32_t nQuality, int32_t nItemLevel, int32_t nPlayerLevel);
//D2Game.0x6FCC6F10
void __fastcall sub_6FCC6F10(D2UnitStrc* pNpc, D2UnitStrc* pItem, D2GameStrc* pGame, int32_t bInit);
//D2Game.0x6FCC6FF0
void __fastcall D2GAME_NPC_BuildHirelingList_6FCC6FF0(D2GameStrc* pGame, D2ClientStrc* pClient, D2UnitStrc* pUnit, int32_t a4);
//D2Game.0x6FCC7100
void __fastcall D2GAME_NPC_FillStoreInventory_6FCC7100(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, D2NpcTradeStrc* pTrade);
//D2Game.0x6FCC74F0
void __fastcall D2GAME_STORES_CreateVendorCache_6FCC74F0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNPC, int32_t a4, int32_t a5);
//D2Game.0x6FCC7680
int32_t __fastcall D2GAME_STORES_SellItem_6FCC7680(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcGUID, int32_t nItemGUID, int16_t nItemMode, int32_t a6);
//D2Game.0x6FCC7E20
void __fastcall sub_6FCC7E20(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pItem, D2UnitStrc* pUnit, int32_t a5);
//D2Game.0x6FCC7FA0
void __fastcall sub_6FCC7FA0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, uint16_t wName);
//D2Game.0x6FCC8430
void __fastcall sub_6FCC8430(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCC84D0
void __fastcall sub_6FCC84D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet);
//D2Game.0x6FCC8630
D2UnitStrc* __fastcall D2GAME_MERCS_Create_6FCC8630(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint16_t wName, uint32_t nSeed, int16_t wVersion, int32_t nBaseMonster, int32_t bDead);
//D2Game.0x6FCC87C0
D2UnitStrc* __fastcall sub_6FCC87C0(D2GameStrc* pPlayer, D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t* a4);
//D2Game.0x6FCC88B0
int32_t __fastcall sub_6FCC88B0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, int32_t nItemGUID, int32_t nItemMode, uint16_t nTransactionType, int32_t nCost, int32_t bFill);
//D2Game.0x6FCC92A0
int32_t __fastcall D2GAME_NPC_BuyItemHandler_6FCC92A0(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcUnitId, int32_t nItemId, int32_t nItemMode, uint16_t nTransactionType, int32_t nCost, int32_t bFill);
//D2Game.0x6FCC9350
void __fastcall D2GAME_NPC_ResurrectMerc_6FCC9350(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcUnitId);
//D2Game.0x6FCC9540
void __fastcall D2GAME_NPC_HireMerc_6FCC9540(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcUnitId, uint16_t a4);
//D2Game.0x6FCC95B0
int32_t __fastcall D2GAME_NPC_Repair_6FCC95B0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nNpcGUID, int32_t nItemGUID, int32_t nUnused, int32_t a6);
//D2Game.0x6FCC9C90
void __fastcall D2GAME_NPC_IdentifyAllItems_6FCC9C90(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nNpcGUID);
//D2Game.0x6FCC9F40
int32_t __fastcall NPC_HandleDialogMessage(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nType, int32_t nNpcGUID, int32_t nItemGUID);
//D2Game.0x6FCCA990
void __fastcall D2GAME_NPC_IdentifyBoughtItem_6FCCA990(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nItemGUID);
//D2Game.0x6FCCA9F0
void __fastcall D2GAME_STORES_FillGamble_6FCCA9F0(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pUnit, D2NpcRecordStrc* pNpcRecord);
//D2Game.0x6FCCAE20
void __fastcall D2GAME_STORES_CreateVendorCache_6FCCAE20(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, int32_t bGamble);
//D2Game.0x6FCCAF30
int32_t __fastcall D2GAME_NPC_RemoveStates_6FCCAF30(D2UnitStrc* pUnit);
//D2Game.0x6FCCAFA0
void __fastcall SUNITNPC_PetIterate_Heal(D2GameStrc* pGame, D2UnitStrc* a2, D2UnitStrc* pUnit, void* a4);
//D2Game.0x6FCCB080
void __fastcall D2GAME_NPC_HealPlayer_6FCCB080(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pNpc);
//D2Game.0x6FCCB220
void __fastcall D2GAME_NPC_Heal_6FCCB220(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pNpc);
//D2Game.0x6FCCB280
void __fastcall D2GAME_NPC_PurchaseHeal_6FCCB280(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nNpcGUID);
//D2Game.0x6FCCB4D0
void __fastcall D2GAME_NPC_ResetInteract_6FCCB4D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc);
//D2Game.0x6FCCB520
void __fastcall D2GAME_NPC_AssignMercenary_6FCCB520(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nMonster);
//D2Game.0x6FCCB7D0
int32_t __fastcall D2GAME_NPC_IsItemInNpcInventory_6FCCB7D0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, D2UnitStrc* pItem, int32_t a4);
