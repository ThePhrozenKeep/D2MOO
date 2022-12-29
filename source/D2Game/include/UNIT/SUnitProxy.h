#pragma once

#include "D2Structs.h"
#include <Units/Units.h>
#include "GAME/Game.h"


//D2Game.0x6FCCB8A0
D2NpcRecordStrc* __fastcall SUNITPROXY_GetNpcRecordFromClassId(D2GameStrc* pGame, int32_t nNpcClassId, int32_t* pIndex);
//D2Game.0x6FCCB910
D2NpcRecordStrc* __fastcall SUNITPROXY_GetNpcRecordFromUnit(D2GameStrc* pGame, D2UnitStrc* pNpc, int32_t* pIndex);
//D2Game.0x6FCCB980
void __fastcall SUNITPROXY_AllocNpcEvent(D2UnitStrc* pNpc, D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t a4);
//D2Game.0x6FCCBA30
void __fastcall SUNITPROXY_InitializeNpcControl(D2GameStrc* pGame);
//D2Game.0x6FCCBF50
void __fastcall SUNITPROXY_InitializeItemCache(D2GameStrc* pGame, D2UnitProxyStrc* pUnitProxy, void* pUnused, int32_t nNpcInventoryId);
//D2Game.0x6FCCC030
void __fastcall SUNITPROXY_FreeNpcControl(D2GameStrc* pGame);
//D2Game.0x6FCCC140
void __fastcall SUNITPROXY_ClearNpcRecordData(D2GameStrc* pGame, D2NpcRecordStrc* pNpcRecord);
//D2Game.0x6FCCC2E0
void __fastcall SUNITPROXY_UpdateNpcsOnActChange(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCurrentLevelId, int32_t nDestLevelId);
//D2Game.0x6FCCC540
void __fastcall SUNITPROXY_UpdateVendorInventory(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nAct, int32_t bNoMorePlayersInLevel);
//D2Game.0x6FCCC690
void __fastcall SUNITPROXY_CountPlayersInLevel(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg);
//D2Game.0x6FCCC6B0
void __fastcall SUNITPROXY_OnClientRemovedFromGame(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCCC7C0
void __fastcall SUNITPROXY_InitializeNpcEventChain(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCCC860
D2InventoryStrc* __fastcall SUNITPROXY_GetNpcInventory(D2GameStrc* pGame, int32_t nNpc);
//D2Game.0x6FCCC8B0
D2SeedStrc* __fastcall SUNITPROXY_GetSeedFromNpcControl(D2GameStrc* pGame);
//D2Game.0x6FCCC8C0
void __fastcall SUNITPROXY_FreeVendorChain(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc);
//D2Game.0x6FCCCA70
void __fastcall SUNITPROXY_UpdateGambleInventory(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pPlayer, D2ClientStrc* pClient);
//D2Game.0x6FCCCB20
D2InventoryStrc* __fastcall SUNITPROXY_GetGambleInventory(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc);
//D2Game.0x6FCCCBB0
D2NpcVendorChainStrc* __fastcall SUNITPROXY_GetVendorChain(D2GameStrc* pGame, D2NpcRecordStrc* pNpcRecord, D2UnitStrc* pNpc);
//D2Game.0x6FCCCC00
void __fastcall SUNITPROXY_AllocNpcInventory(D2GameStrc* pGame, D2NpcRecordStrc* pNpcRecord, D2UnitStrc* pNPC);
//D2Game.0x6FCCCC40
void __fastcall SUNITPROXY_FillGlobalItemCache();
//D2Game.0x6FCCCED0
void __fastcall SUNITPROXY_FillIGlobaltemCacheRecordForNpc(int32_t nNpcId);
//D2Game.0x6FCCD120
void __fastcall SUNITPROXY_ClearGlobalItemCache();
//D2Game.0x6FCCD190
void __fastcall SUNITPROXY_FreeNpcGamble(D2GameStrc* pGame, D2UnitStrc* pNpc, D2UnitStrc* pPlayer);
