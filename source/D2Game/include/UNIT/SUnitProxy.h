#pragma once

#include <Units/Units.h>
#include "GAME/Game.h"

#pragma pack(1)

struct D2NpcRecordStrc;
struct D2NpcVendorChainStrc;

struct D2ItemCacheStrc
{
	uint8_t nMin;								//0x00
	uint8_t nMax;								//0x01
	uint8_t nMagicMin;							//0x02
	uint8_t nMagicMax;							//0x03
	uint32_t dwCode;							//0x04
	int32_t nMagicLevel;						//0x08
};

struct D2UnitProxyStrc
{
	D2ItemCacheStrc* pItemCache;			//0x00
	int32_t nItems;								//0x04
	uint32_t* pPermCache;						//0x08
	int32_t nPerms;								//0x0C
};

#pragma pack()


//D2Game.0x6FCCB8A0
//1.14d: 0x00535EA0
D2NpcRecordStrc* __fastcall SUNITPROXY_GetNpcRecordFromClassId(D2GameStrc* pGame, int32_t nNpcClassId, int32_t* pIndex);
//D2Game.0x6FCCB910
//1.14d: 0x00535F10
D2NpcRecordStrc* __fastcall SUNITPROXY_GetNpcRecordFromUnit(D2GameStrc* pGame, D2UnitStrc* pNpc, int32_t* pIndex);
//D2Game.0x6FCCB980
//1.14d: 0x00535F30
void __fastcall SUNITPROXY_AllocNpcEvent(D2UnitStrc* pNpc, D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nEventParam);
//D2Game.0x6FCCBA30
//1.14d: 0x00536070
void __fastcall SUNITPROXY_InitializeNpcControl(D2GameStrc* pGame);
//D2Game.0x6FCCBF50
//1.14d: 0x00535FB0
void __fastcall SUNITPROXY_InitializeItemCache(D2GameStrc* pGame, D2UnitProxyStrc* pUnitProxy, void* pUnused, int32_t nNpcInventoryId);
//D2Game.0x6FCCC030
//1.14d: 0x00536640
void __fastcall SUNITPROXY_FreeNpcControl(D2GameStrc* pGame);
//D2Game.0x6FCCC140
//1.14d: 0x00536580
void __fastcall SUNITPROXY_ClearNpcRecordData(D2GameStrc* pGame, D2NpcRecordStrc* pNpcRecord);
//D2Game.0x6FCCC2E0
//1.14d: 0x00537340
void __fastcall SUNITPROXY_UpdateNpcsOnActChange(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCurrentLevelId, int32_t nDestLevelId);
//D2Game.0x6FCCC540
//1.14d: 0x00537230
void __fastcall SUNITPROXY_UpdateVendorInventory(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nAct, int32_t bNoMorePlayersInLevel);
//D2Game.0x6FCCC690
//1.14d: 0x00536820
void __fastcall SUNITPROXY_CountPlayersInLevel(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg);
//D2Game.0x6FCCC6B0
//1.14d: 0x00537580
void __fastcall SUNITPROXY_OnClientRemovedFromGame(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCCC7C0
//1.14d: 0x00536850
void __fastcall SUNITPROXY_InitializeNpcEventChain(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCCC860
//1.14d: 0x00536B20
D2InventoryStrc* __fastcall SUNITPROXY_GetNpcInventory(D2GameStrc* pGame, int32_t nNpc);
//D2Game.0x6FCCC8B0
//1.14d: 0x00536B40
D2SeedStrc* __fastcall SUNITPROXY_GetSeedFromNpcControl(D2GameStrc* pGame);
//D2Game.0x6FCCC8C0
//1.14d: 0x00536B50
void __fastcall SUNITPROXY_FreeVendorChain(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc);
//D2Game.0x6FCCCA70
//1.14d: 0x00537680
void __fastcall SUNITPROXY_UpdateGambleInventory(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pPlayer, D2ClientStrc* pClient);
//D2Game.0x6FCCCB20
//1.14d: 0x00536C50
D2InventoryStrc* __fastcall SUNITPROXY_GetGambleInventory(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc);
//D2Game.0x6FCCCBB0
//1.14d: 0x00536CB0
D2NpcVendorChainStrc* __fastcall SUNITPROXY_GetVendorChain(D2GameStrc* pGame, D2NpcRecordStrc* pNpcRecord, D2UnitStrc* pNpc);
//D2Game.0x6FCCCC00
//1.14d: 0x00536D10
void __fastcall SUNITPROXY_AllocNpcInventory(D2GameStrc* pGame, D2NpcRecordStrc* pNpcRecord, D2UnitStrc* pNPC);
//D2Game.0x6FCCCC40
//1.14d: 0x00536F80
void __fastcall SUNITPROXY_FillGlobalItemCache();
//D2Game.0x6FCCCED0
//1.14d: 0x00536D50
void __fastcall SUNITPROXY_FillIGlobaltemCacheRecordForNpc(int32_t nNpcId);
//D2Game.0x6FCCD120
//1.14d: 0x00537120
void __fastcall SUNITPROXY_ClearGlobalItemCache();
//D2Game.0x6FCCD190
//1.14d: 0x00537190
void __fastcall SUNITPROXY_FreeNpcGamble(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pPlayer);
