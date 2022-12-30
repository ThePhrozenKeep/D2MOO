#pragma once

#include <Units/Units.h>
#include <UNIT/SUnitNpc.h>

#pragma pack(push, 1)
struct Unk
{
    int32_t nItemCode;
    uint8_t nBodyLoc;
    uint8_t unk0x05;
    uint8_t unk0x06;
    uint8_t unk0x07;
};
#pragma pack(pop)


//D2Game.0x6FC61190
void __fastcall sub_6FC61190(D2GameStrc* pGame, D2UnitStrc* pMonster, Unk* pItemCode, DWORD dwILvl, DWORD dwQuality);
//D2Game.0x6FC61270
void __fastcall sub_6FC61270(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pUnit, int16_t nId, D2MercDataStrc* pMercData, int32_t bDead);
//D2Game.0x6FC61490
void __fastcall MONSTERAI_SendMercStats(D2GameStrc* pGame, D2UnitStrc* pPlayer, DWORD dwZero);
//D2Game.0x6FC61610
void __fastcall MONSTERAI_UpdateMercStatsAndSkills(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pMerc, int32_t nLevel);
//D2Game.0x6FC61980
D2MonsterInteractStrc* __fastcall MONSTERAI_AllocMonsterInteract(D2GameStrc* pGame);
//D2Game.0x6FC619A0
void __fastcall MONSTERAI_FreeMonsterInteract(D2GameStrc* pGame, D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC619F0
int32_t __fastcall MONSTERAI_GetInteractUnitCount(D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC61A10
int32_t __fastcall MONSTERAI_HasInteractUnit(D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC61A30
int32_t __fastcall MONSTERAI_IsInteractingWith(D2MonsterInteractStrc* pMonInteract, D2UnitStrc* pUnit);
//D2Game.0x6FC61A50
void __fastcall MONSTERAI_RemoveInteractInfoFor(D2GameStrc* pGame, D2UnitStrc* pMonster, D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC61AB0
void __fastcall sub_6FC61AB0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pNpc, D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC61AF0
void __fastcall D2GAME_NPCS_SetInteractTrading_6FC61AF0(D2UnitStrc* pNPC, D2UnitStrc* pPlayer);
//D2Game.0x6FC61B30
int32_t __fastcall sub_6FC61B30(D2UnitStrc* pMonster, D2UnitStrc* pPlayer);
//D2Game.0x6FC61B70
void __fastcall sub_6FC61B70(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC61C70
void __fastcall sub_6FC61C70(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2MonsterInteractStrc* pMonInteract, char a5);
//D2Game.0x6FC61E30
int32_t __fastcall sub_6FC61E30(D2UnitStrc* pUnit, int32_t a2, int32_t a3);
//D2Game.0x6FC61EC0
int32_t __fastcall sub_6FC61EC0(D2UnitStrc* pMonster);
//D2Game.0x6FC61EE0
int32_t __fastcall sub_6FC61EE0(D2UnitStrc* pMonster);
//D2Game.0x6FC61F00
void __fastcall sub_6FC61F00(D2UnitStrc* pMonster);
//D2Game.0xFC61F20
int32_t __fastcall sub_6FC61F20(D2UnitStrc* pMonster, D2UnitStrc* pUnit);
//D2Game.0x6FC61F70
void __fastcall D2GAME_MONSTERAI_Last_6FC61F70(D2GameStrc* pGame, D2UnitStrc* pMonster, void(__fastcall* pCallback)(D2UnitStrc*));
