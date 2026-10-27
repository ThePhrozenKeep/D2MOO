#pragma once

#include <Units/Units.h>
#include <D2DataTbls.h>


//D2Game.0x6FC7B550
//1.14d: 0x005348C0
void __fastcall PLAYER_Create(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPlayerGUID);
//D2Game.0x6FC7B630
//1.14d: 0x005349D0
void __fastcall PLAYER_Destroy(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7B750
//1.14d: 0x00534A50
void __fastcall PLAYER_RemoveAllPlayers(D2GameStrc* pGame);
//D2Game.0x6FC7B7A0
//1.14d: 0x00534AD0
void __fastcall PLAYER_InitializePosition(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY);
//D2Game.0x6FC7B800
//1.14d: 0x00534D60
void __fastcall PLAYER_CreateStartItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2CharStatsTxt* pCharStatsTxt, int32_t nCount, D2CharItemStrc* pCharItem);
//D2Game.0x6FC7BB50
//1.14d: 0x00534B30
void __fastcall PLAYER_PlaceItemInInventory(D2UnitStrc* pItem, D2UnitStrc* pPlayer, D2GameStrc* pGame, int32_t a8);
//D2Game.0x6FC7BC30
//1.14d: 0x00534F10
void __fastcall PLAYER_CreateStartItemsFromCharStatsTxt(D2UnitStrc* pPlayer, D2GameStrc* pGame, void* pUnused);
//D2Game.0x6FC7BC90
//1.14d: 0x00534F80
void __fastcall PLAYER_SynchronizeItemsToClient(D2UnitStrc* pUnit, D2ClientStrc* pClient);
//D2Game.0x6FC7BD50
//1.14d: 0x00535060
int32_t __fastcall PLAYER_IsBusy(D2UnitStrc* pUnit);
//D2Game.0x6FC7BDB0
//1.14d: 0x005350B0
void __fastcall PLAYER_ResetBusyState(D2UnitStrc* pUnit);
//D2Game.0x6FC7BDF0
//1.14d: 0x005350F0
void __fastcall PLAYER_StopInteractions(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC7BEC0
//1.14d: 0x005351C0
void __fastcall PLAYER_CloseInteraction(D2GameStrc* pGame, D2UnitStrc* pUnit);
//1.10f: D2Game.0x6FC7BFC0
//1.13c: D2Game.0x6FC57B10
//1.14d: 0x005352C0
void __fastcall PLAYER_PlaceCorpse(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nPlayerGUID, D2CoordStrc* pCoord);
//D2Game.0x6FC7C0C0
//1.14d: 0x005353B0
void __fastcall PLAYER_SetUniqueIdInPlayerData(D2UnitStrc* pPlayer, int32_t nUnitGUID);
//D2Game.0x6FC7C120
//1.14d: 0x005353F0
int32_t __fastcall PLAYER_GetUniqueIdFromPlayerData(D2UnitStrc* pPlayer);
//D2Game.0x6FC7C170
//1.14d: 0x00535430
void __fastcall PLAYER_RemoveTownPortal(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7C260
//1.14d: 0x00535510
int32_t __fastcall PLAYER_DropGold(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUnitGUID, uint32_t nValue);
//D2Game.0x6FC7C3A0
//1.14d: 0x00535610
int32_t __fastcall PLAYER_CanModifyInventory(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem);
//D2Game.0x6FC7C450
//1.14d: 0x005356B0
int32_t __fastcall PLAYER_SetSkillHotKey(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nHotkeyId, int16_t nSkillId, char nHand, int32_t nFlags);
//D2Game.0x6FC7C490 (#10059)
//1.14d: 0x00535780
void __fastcall PLAYER_SetVirtualPlayerCount(int32_t nPlayers);
//D2Game.0x6FC7C4A0
//1.14d: 0x00535790
int32_t __fastcall PLAYER_GetPlayerCount(D2GameStrc* pGame);
//D2Game.0x6FC7C4E0
//1.14d: 0x00535760
void __fastcall PLAYER_CountLivingPlayers(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pLivingPlayers);
//D2Game.0x6FC7C500
//1.14d: 0x00535AB0
void __fastcall PLAYER_ApplyDeathPenalty(D2GameStrc* pGame, D2UnitStrc* pDefender, D2UnitStrc* pAttacker);
//D2Game.0x6FC7C750
//1.14d: 0x00535B10
int32_t __fastcall PLAYER_HasHostilePlayer(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC7C790
//1.14d: 0x00535AF0
void __fastcall PLAYER_CheckHostilityCallback(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArgs);
//D2Game.0x6FC7C7B0
//1.14d: 0x00535BC0
D2UnitStrc* __fastcall PLAYER_GetWeaponForSkill(D2UnitStrc* pUnit);
//D2Game.0x6FC7C900
//1.14d: 0x00535D10
void __fastcall PLAYER_ApplyWeaponStatsForSkill(D2UnitStrc* pAttacker, int32_t bTemporaryWeaponStats);
//D2Game.0x6FC7CA10
//1.14d: 0x00535E20
void __fastcall PLAYER_RestoreWeaponStats(D2UnitStrc* pTarget, int32_t bTemporaryWeaponStats);
//D2Game.0x6FC7CA70
//1.14d: 0x00530EA0
void __fastcall D2GAME_SetStatOrResetGold_6FC7CA70(D2UnitStrc* pUnit, int32_t nStat, int32_t nValue);
