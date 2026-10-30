#pragma once

#include <D2StatList.h>
#include <Units/Units.h>
#include <UNIT/SUnitDmg.h>


struct D2AuraCallbackStrc;


//D2Game.0x6FD18330
//1.14d: 0x005CE790
int32_t __fastcall SKILLS_SrvSt29_Sacrifice(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD184B0
//1.14d: 0x005CE8E0
int32_t __fastcall SKILLS_SrvDo064_Sacrifice(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD18650
//1.14d: 0x005CE9F0
int32_t __fastcall SKILLS_SrvDo150_Smite(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD18900
//1.14d: 0x005CF010
int32_t __fastcall SKILLS_SrvDo065_BasicAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD18BC0
//1.14d: 0x005CEDC0
int32_t __fastcall SKILLS_AuraCallback_BasicAura(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FD18FE0
//1.14d: 0x005CEC50
void __fastcall SKILLS_CurseStateCallback_BasicAura(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList);
//D2Game.0x6FD19020
//1.14d: 0x005CF3A0
int32_t __fastcall SKILLS_SrvDo066_HolyFire_HolyShock_Sanctuary_Conviction(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD19390
//1.14d: 0x005CF2A0
int32_t __fastcall SKILLS_AuraCallback_HolyFire_HolyShock_Sanctuary_Conviction(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FD19460
//1.14d: 0x005CF6B0
int32_t __fastcall SKILLS_SrvSt31_Charge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD19780
//1.14d: 0x005CF900
int32_t __fastcall SKILLS_SrvDo067_Charge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD19C80
//1.14d: 0x005CFE10
int32_t __fastcall SKILLS_SrvSt35_Vengeance(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD1A200
//1.14d: 0x005D0040
int32_t __fastcall SKILLS_SrvDo073_BlessedHammer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD1A480
//1.14d: 0x005D0180
int32_t __fastcall SKILLS_SrvSt36_HolyShield(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD1A4A0
//1.14d: 0x005D0350
int32_t __fastcall SKILLS_SrvDo079_Conversion(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD1A900
//1.14d: 0x005D01A0
void __fastcall SKILLS_StatRemoveCallback_Conversion(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList);
//D2Game.0x6FD1AA50
//1.14d: 0x005D0670
int32_t __fastcall SKILLS_SrvDo080_FistOfTheHeavens(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD1ABC0
//1.14d: 0x005D0920
int32_t __fastcall SKILLS_SrvDo081_HolyFreeze(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD1AF40
//1.14d: 0x005D0770
void __fastcall SKILLS_CurseStateCallback_HolyFreeze(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList);
//D2Game.0x6FD1AF90
//1.14d: 0x005D07C0
int32_t __fastcall SKILLS_AuraCallback_HolyFreeze(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FD1B100
//1.14d: 0x005D0C40
int32_t __fastcall SKILLS_ApplyRedemptionEffect(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pCorpse, int32_t nSkillId, int32_t nSkillLevel, int32_t bSkipChanceRoll);
//D2Game.0x6FD1B260
//1.14d: 0x005D0E90
int32_t __fastcall SKILLS_SrvDo082_Redemption(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD1B490
//1.14d: 0x005D0D70
int32_t __fastcall SKILLS_AuraCallback_Redemption(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FD1B4C0
//1.14d: 0x005D10C0
void __fastcall SKILLS_ApplyThornsDamage(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage);
