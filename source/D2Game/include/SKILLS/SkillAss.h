#pragma once

#include "D2Structs.h"
#include <D2StatList.h>
#include <Units/Units.h>
#include <Units/UnitFinds.h>
#include <UNIT/SUnitDmg.h>


struct D2AuraCallbackStrc;


//D2Game.0x6FCF5090
int32_t __fastcall SKILLS_SrvSt22_PsychicHammer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF50E0
int32_t __fastcall SKILLS_SrvDo033_PsychicHammer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF52C0
int32_t __fastcall SKILLS_SrvSt23_AssasinChargeStrikes(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF52E0
void __fastcall SKILLS_StatRemoveCallback_ProgressiveStrike(D2UnitStrc* pUnit, int32_t nState, int32_t nUnused);
//D2Game.0x6FCF52F0
int32_t __fastcall SKILLS_SrvDo034_TigerStrike_CobraStrike_RoyalStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF55B0
int32_t __fastcall SKILLS_SrvDo035_FistsOfFire_ClawsOfThunder_BladesOfIce(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF5680
void __fastcall sub_6FCF5680(D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FCF5870
void __fastcall sub_6FCF5870(D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FCF5B90
D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId);
//D2Game.0x6FCF5BC0
void __fastcall sub_6FCF5BC0(D2UnitStrc* pUnit, D2StatListStrc* pStatList, D2DamageStrc* pDamage);
//D2Game.0x6FCF5DE0
int32_t __fastcall sub_6FCF5DE0(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FCF5E20
int32_t __fastcall SKILLS_GetProgressiveSkillMissileId(D2UnitStrc* pUnit, int32_t nSkillId);
//D2Game.0x6FCF5EE0
int32_t __fastcall SKILLS_EvaluateProgressiveSkillCalc(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6000
int32_t __fastcall SKILLS_SrvDo038_FistsOfFire_BladesOfIce_ProgressiveFn2(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6140
int32_t __fastcall SKILLS_SrvDo039_FistsOfFire_BladesOfIce_ProgressiveFn3(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6420
int32_t __fastcall SKILLS_SrvDo036_ClawsOfThunder_ProgressiveFn2(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6500
int32_t __fastcall SKILLS_SrvDo037_ClawsOfThunder_ProgressiveFn3(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6600
void __fastcall sub_6FCF6600(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nLowSeed, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep);
//D2Game.0x6FCF6C10
void __fastcall sub_6FCF6C10(D2UnitStrc* pMissile, int32_t nLowSeed);
//D2Game.0x6FCF6C70
int32_t __fastcall SKILLS_SrvDo143_FistsOfFire_RoyalStrike_ProgressiveFn(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6D70
void __fastcall sub_6FCF6D70(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nLowSeed, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep);
//D2Game.0x6FCF7390
void __fastcall sub_6FCF7390(D2UnitStrc* pMissile, int32_t nInitSeed);
//D2Game.0x6FCF7590
int32_t __fastcall SKILLS_SrvDo040_RoyalStrike_ProgressiveFn1(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF7610
int32_t __fastcall SKILLS_SrvDo041_RoyalStrike_ProgressiveFn3(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF77E0
void __fastcall sub_6FCF77E0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FCF7AD0
int32_t __fastcall SKILLS_SrvSt24_DragonTalon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF7BC0
void __fastcall sub_6FCF7BC0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2SkillsTxt* pSkillsTxtRecordArg, int32_t nSkillId, int32_t nSkillLevel, int32_t bKnockback);;
//D2Game.0x6FCF7CE0
void __fastcall sub_6FCF7CE0(D2GameStrc* pGame, D2DamageStrc* pDamage, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8110
int32_t __fastcall SKILLS_SrvDo042_DragonTalon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8330
int32_t __fastcall sub_6FCF8330(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSubMissiles, int32_t nRange, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8550
int32_t __fastcall SKILLS_SrvDo043_ShockField(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8610
D2UnitStrc* __fastcall sub_6FCF8610(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF88E0
int32_t __fastcall SKILLS_SrvDo044_BladeSentinel(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8B00
int32_t __fastcall SKILLS_SrvDo045_Sentry(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8BA0
int32_t __fastcall SKILLS_SrvDo046_DragonClaw(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8C70
void __fastcall sub_6FCF8C70(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8DD0
void __fastcall sub_6FCF8DD0(D2UnitStrc* pDefender);
//D2Game.0x6FCF8ED0
int32_t __fastcall SKILLS_SrvDo047_CloakOfShadows(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF9160
int32_t __fastcall SKILLS_AuraCallback_CloakOfShadows(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FCF9260
int32_t __fastcall SKILLS_SrvDo048_BladeFury(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF93B0
int32_t __fastcall SKILLS_SrvSt26_BladeFury(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF9550
void __fastcall SKILLS_StatRemoveCallback_RemoveState(D2UnitStrc* pUnit, int32_t nState, int32_t nUnused);
//D2Game.0x6FCF9580
void __fastcall sub_6FCF9580(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nSkillId, int32_t nSkillLevel, int32_t nItemLevel, int32_t bCheckOnInit);
//D2Game.0x6FCF9750
int32_t __fastcall SKILLS_SrvDo049_ShadowWarrior_Master(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF9B50
int32_t __fastcall SKILLS_SrvSt27_DragonTail(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF9C70
int32_t __fastcall SKILLS_SrvDo050_DragonTail(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF9EA0
void __fastcall SKILLS_StatRemoveCallback_MindBlast(D2UnitStrc* pItem, int32_t nState, int32_t nUnused);
//D2Game.0x6FCF9FF0
int32_t __fastcall SKILLS_AuraCallback_MindBlast(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FCFA350
int32_t __fastcall SKILLS_SrvDo051_MindBlast(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFA4F0
int32_t __fastcall SKILLS_SrvDo052_DragonFlight(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFA720
int32_t __fastcall SKILLS_SrvSt28_BladeShield(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFA880
int32_t __fastcall SKILLS_SrvDo053_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFA980
int32_t __fastcall SKILLS_AuraCallback_SrvDo142(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FCFAA20
int32_t __fastcall SKILLS_SrvDo142_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFAB60
int32_t __fastcall SKILLS_SrvDo054_BladeShield(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
