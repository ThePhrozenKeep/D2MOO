#pragma once

#include <Units/Units.h>
#include <UNIT/SUnitNpc.h>

#pragma pack(push, 1)
struct D2MonsterEquipmentStrc
{
    int32_t nItemCode;
    uint8_t nBodyLoc;
    uint8_t unk0x05;
    uint8_t unk0x06;
    uint8_t unk0x07;
};
#pragma pack(pop)


//D2Game.0x6FC61190
//1.14d: 0x00573B20
void __fastcall MONSTERAI_CreateAndEquipItem(D2GameStrc* pGame, D2UnitStrc* pMonster, D2MonsterEquipmentStrc* pItemCode, DWORD dwILvl, DWORD dwQuality);
//D2Game.0x6FC61270
//1.14d: 0x00573270
void __fastcall MONSTERAI_AssignHireling(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pUnit, int16_t nId, D2MercDataStrc* pMercData, int32_t bDead);
//D2Game.0x6FC61490
//1.14d: 0x005726C0
void __fastcall MONSTERAI_SendMercStats(D2GameStrc* pGame, D2UnitStrc* pPlayer, DWORD dwZero);
//D2Game.0x6FC61610
//1.14d: 0x00572840
void __fastcall MONSTERAI_UpdateMercStatsAndSkills(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pMerc, int32_t nLevel);
//D2Game.0x6FC61980
//1.14d: 0x00572BA0
D2MonsterInteractStrc* __fastcall MONSTERAI_AllocMonsterInteract(D2GameStrc* pGame);
//D2Game.0x6FC619A0
//1.14d: 0x00572BC0
void __fastcall MONSTERAI_FreeMonsterInteract(D2GameStrc* pGame, D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC619F0
//1.14d: 0x00572DA0
int32_t __fastcall MONSTERAI_GetInteractUnitCount(D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC61A10
//1.14d: 0x00572DC0
int32_t __fastcall MONSTERAI_HasInteractUnit(D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC61A30
//1.14d: 0x00572DE0
int32_t __fastcall MONSTERAI_IsInteractingWith(D2MonsterInteractStrc* pMonInteract, D2UnitStrc* pUnit);
//D2Game.0x6FC61A50
//1.14d: 0x00572E00
void __fastcall MONSTERAI_RemoveInteractInfoFor(D2GameStrc* pGame, D2UnitStrc* pMonster, D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC61AB0
//1.14d: 0x00572E60
void __fastcall MONSTERAI_ActivateNpcInteraction(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pNpc, D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC61AF0
//1.14d: 0x00572EA0
void __fastcall D2GAME_NPCS_SetInteractTrading_6FC61AF0(D2UnitStrc* pNPC, D2UnitStrc* pPlayer);
//D2Game.0x6FC61B30
//1.14d: 0x00572EE0
int32_t __fastcall MONSTERAI_IsTrading(D2UnitStrc* pMonster, D2UnitStrc* pPlayer);
//D2Game.0x6FC61B70
//1.14d: 0x00572F20
void __fastcall MONSTERAI_EndNpcInteraction(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pNpc, D2MonsterInteractStrc* pMonInteract);
//D2Game.0x6FC61C70
//1.14d: 0x00572C10
void __fastcall MONSTERAI_StartNpcInteraction(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2MonsterInteractStrc* pMonInteract, char nQuestFlags);
//D2Game.0x6FC61E30
//1.14d: 0x00573090
int32_t __fastcall MONSTERAI_SetTargetOverride(D2UnitStrc* pUnit, int32_t nTargetType, int32_t nTargetGUID);
//D2Game.0x6FC61EC0
//1.14d: 0x005730E0
int32_t __fastcall MONSTERAI_GetTargetOverrideType(D2UnitStrc* pMonster);
//D2Game.0x6FC61EE0
//1.14d: 0x00573100
int32_t __fastcall MONSTERAI_GetTargetOverrideGUID(D2UnitStrc* pMonster);
//D2Game.0x6FC61F00
//1.14d: 0x00573120
void __fastcall MONSTERAI_ClearTargetOverride(D2UnitStrc* pMonster);
//D2Game.0xFC61F20
//1.10f: D2Game.0x6FC61F20
//1.14d: 0x00573140
int32_t __fastcall MONSTERAI_IsInteractionReady(D2UnitStrc* pMonster, D2UnitStrc* pUnit);
//D2Game.0x6FC61F70
void __fastcall MONSTERAI_CloseNpcInteractions(D2GameStrc* pGame, D2UnitStrc* pMonster, void(__fastcall* pCallback)(D2UnitStrc*));
