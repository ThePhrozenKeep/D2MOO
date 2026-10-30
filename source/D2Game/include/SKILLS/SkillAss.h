#pragma once

#include <D2StatList.h>
#include <Units/Units.h>
#include <Units/UnitFinds.h>
#include <UNIT/SUnitDmg.h>


struct D2AuraCallbackStrc;


//D2Game.0x6FCF5090
//1.14d: 0x005D30F0
int32_t __fastcall SKILLS_SrvSt22_PsychicHammer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF50E0
//1.14d: 0x005D3140
int32_t __fastcall SKILLS_SrvDo033_PsychicHammer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF52C0
//1.14d: 0x005D32F0
int32_t __fastcall SKILLS_SrvSt23_AssasinChargeStrikes(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF52E0
//1.14d: 0x005D3310
void __fastcall SKILLS_StatRemoveCallback_ProgressiveStrike(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList);
//D2Game.0x6FCF52F0
//1.14d: 0x005D3490
int32_t __fastcall SKILLS_SrvDo034_TigerStrike_CobraStrike_RoyalStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF55B0
//1.14d: 0x005D35D0
int32_t __fastcall SKILLS_SrvDo035_FistsOfFire_ClawsOfThunder_BladesOfIce(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF5680
//1.14d: 0x005D3AC0
void __fastcall SKILLS_ApplyProgressivePhysicalDamageBonus(D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FCF5870
//1.14d: 0x005D3BA0
void __fastcall SKILLS_ApplyProgressiveLeechAndElementalDamage(D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FCF5B90
D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId);
//D2Game.0x6FCF5BC0
//1.14d: 0x005D3970
void __fastcall SKILLS_ApplyProgressiveElementalConversion(D2UnitStrc* pUnit, D2StatListStrc* pStatList, D2DamageStrc* pDamage);
//D2Game.0x6FCF5DE0
//1.14d: 0x005D3CB0
int32_t __fastcall SKILLS_AuraCallback_ApplyCopiedDamage(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FCF5E20
//1.14d: 0x005D3CF0
int32_t __fastcall SKILLS_GetProgressiveSkillMissileId(D2UnitStrc* pUnit, int32_t nSkillId);
//D2Game.0x6FCF5EE0
//1.14d: 0x005D3DA0
int32_t __fastcall SKILLS_EvaluateProgressiveSkillCalc(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6000
//1.14d: 0x005D3E80
int32_t __fastcall SKILLS_SrvDo038_FistsOfFire_BladesOfIce_ProgressiveFn2(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6140
//1.14d: 0x005D3F90
int32_t __fastcall SKILLS_SrvDo039_FistsOfFire_BladesOfIce_ProgressiveFn3(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6420
//1.14d: 0x005D4DB0
int32_t __fastcall SKILLS_SrvDo036_ClawsOfThunder_ProgressiveFn2(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6500
//1.14d: 0x005D4E70
int32_t __fastcall SKILLS_SrvDo037_ClawsOfThunder_ProgressiveFn3(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6600
//1.14d: 0x005D4150
void __fastcall SKILLS_CreateChargedBoltMissileRing(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nLowSeed, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep);
//D2Game.0x6FCF6C10
//1.14d: 0x005D40F0
void __fastcall SKILLS_MissileInit_SeededChargedBolt(D2UnitStrc* pMissile, int32_t nLowSeed);
//D2Game.0x6FCF6C70
//1.14d: 0x005D4F40
int32_t __fastcall SKILLS_SrvDo143_FistsOfFire_RoyalStrike_ProgressiveFn(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF6D70
//1.14d: 0x005D4870
void __fastcall SKILLS_CreateCurvingMissileRing(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nLowSeed, int32_t nMissileId, int32_t nX, int32_t nY, int32_t nSkillId, int32_t nSkillLevel, int32_t nStep);
//D2Game.0x6FCF7390
//1.14d: 0x005D4680
void __fastcall SKILLS_MissileInit_CurvingPath(D2UnitStrc* pMissile, int32_t nInitSeed);
//D2Game.0x6FCF7590
//1.14d: 0x005D5010
int32_t __fastcall SKILLS_SrvDo040_RoyalStrike_ProgressiveFn1(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF7610
//1.14d: 0x005D5080
int32_t __fastcall SKILLS_SrvDo041_RoyalStrike_ProgressiveFn3(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF77E0
//1.14d: 0x005D5220
void __fastcall SKILLS_ReleaseProgressiveCharges(D2GameStrc* pGame, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FCF7AD0
//1.14d: 0x005D5970
int32_t __fastcall SKILLS_SrvSt24_DragonTalon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF7BC0
//1.14d: 0x005D5880
void __fastcall SKILLS_AllocDragonTalonCombat(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2SkillsTxt* pSkillsTxtRecordArg, int32_t nSkillId, int32_t nSkillLevel, int32_t bKnockback);;
//D2Game.0x6FCF7CE0
//1.14d: 0x005D54B0
void __fastcall SKILLS_FillKickDamage(D2GameStrc* pGame, D2DamageStrc* pDamage, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8110
//1.14d: 0x005D5A30
int32_t __fastcall SKILLS_SrvDo042_DragonTalon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8330
//1.14d: 0x005D5BF0
int32_t __fastcall SKILLS_CreateMissilesAroundTarget(D2GameStrc* pGame, D2UnitStrc* pMissile, D2UnitStrc* pUnit, int32_t nMissileId, int32_t nSubMissiles, int32_t nRange, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8550
//1.14d: 0x005D5D70
int32_t __fastcall SKILLS_SrvDo043_ShockField(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8610
//1.14d: 0x005D5E10
D2UnitStrc* __fastcall SKILLS_SummonSkillMonster(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF88E0
//1.14d: 0x005D6020
int32_t __fastcall SKILLS_SrvDo044_BladeSentinel(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8B00
//1.14d: 0x005D6170
int32_t __fastcall SKILLS_SrvDo045_Sentry(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8BA0
//1.14d: 0x005D6340
int32_t __fastcall SKILLS_SrvDo046_DragonClaw(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8C70
//1.14d: 0x005D6200
void __fastcall SKILLS_AllocDragonClawCombat(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF8DD0
//1.14d: 0x005D6410
void __fastcall SKILLS_RemoveStatesOnHit(D2UnitStrc* pDefender);
//D2Game.0x6FCF8ED0
//1.14d: 0x005D6630
int32_t __fastcall SKILLS_SrvDo047_CloakOfShadows(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF9160
//1.14d: 0x005D6520
int32_t __fastcall SKILLS_AuraCallback_CloakOfShadows(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FCF9260
//1.14d: 0x005D68A0
int32_t __fastcall SKILLS_SrvDo048_BladeFury(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF93B0
//1.14d: 0x005D69D0
int32_t __fastcall SKILLS_SrvSt26_BladeFury(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF9550
//1.14d: 0x005D69B0
void __fastcall SKILLS_StatRemoveCallback_RemoveState(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList);
//D2Game.0x6FCF9580
//1.14d: 0x005D6B60
void __fastcall SKILLS_EquipMonsterFromTable(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nSkillId, int32_t nSkillLevel, int32_t nItemLevel, int32_t bCheckOnInit);
//D2Game.0x6FCF9750
//1.14d: 0x005D6E70
int32_t __fastcall SKILLS_SrvDo049_ShadowWarrior_Master(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF9B50
//1.14d: 0x005D7090
int32_t __fastcall SKILLS_SrvSt27_DragonTail(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF9C70
//1.14d: 0x005D7180
int32_t __fastcall SKILLS_SrvDo050_DragonTail(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF9EA0
//1.14d: 0x005D7310
void __fastcall SKILLS_StatRemoveCallback_MindBlast(D2UnitStrc* pItem, int32_t nState, D2StatListStrc* pStatList);
//D2Game.0x6FCF9FF0
//1.14d: 0x005D7680
int32_t __fastcall SKILLS_AuraCallback_MindBlast(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FCFA350
//1.14d: 0x005D76E0
int32_t __fastcall SKILLS_SrvDo051_MindBlast(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFA4F0
//1.14d: 0x005D7850
int32_t __fastcall SKILLS_SrvDo052_DragonFlight(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFA720
//1.14d: 0x005D7A00
int32_t __fastcall SKILLS_SrvSt28_BladeShield(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFA880
//1.14d: 0x005D7B60
int32_t __fastcall SKILLS_SrvDo053_ApplyAreaDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFA980
int32_t __fastcall SKILLS_AuraCallback_AllocSkillCombat(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FCFAA20
//1.14d: 0x005D7CE0
int32_t __fastcall SKILLS_SrvDo142_ApplyAreaCombat(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFAB60
//1.14d: 0x005D7E10
int32_t __fastcall SKILLS_SrvDo054_BladeShield(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
