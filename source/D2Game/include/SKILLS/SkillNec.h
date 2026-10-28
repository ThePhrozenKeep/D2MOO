#pragma once

#include <Units/Units.h>
#include <UNIT/SUnitDmg.h>


struct D2AuraCallbackStrc;


//D2Game.0x6FD0AF30
//1.14d: 0x005C3070
int32_t __fastcall SKILLS_SrvSt15_RaiseSkeleton_Mage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0AF70
//1.14d: 0x005C30A0
int32_t __fastcall SKILLS_SrvSt16_PoisonDagger(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0B0B0
//1.14d: 0x005C31C0
int32_t __fastcall SKILLS_SrvSt17_Poison_CorpseExplosion(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0B0F0
//1.14d: 0x005C3270
int32_t __fastcall SKILLS_SrvSt19_BonePrison(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0B120
//1.14d: 0x005C32A0
int32_t __fastcall SKILLS_SrvSt20_IronGolem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0B190
//1.14d: 0x005C3350
int32_t __fastcall SKILLS_SrvSt21_Revive(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0B250
//1.14d: 0x005C3370
void __fastcall SKILLS_CurseStateCallback_ResetAi(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList);
//D2Game.0x6FD0B2B0
//1.14d: 0x005C3420
int32_t __fastcall SKILLS_CanApplyCurseToTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//D2Game.0x6FD0B3D0
//1.14d: 0x005C3540
int32_t __fastcall SKILLS_ApplyResistanceImmunityPenalty(D2UnitStrc* pUnit, int32_t nStatId, int32_t nResist);
//D2Game.0x6FD0B450
//1.14d: 0x005C35C0
int32_t __fastcall SKILLS_AuraCallback_ApplyCurse(D2UnitStrc* pUnit, void* pArgs);
//D2Game.0x6FD0B790
//1.14d: 0x005C37C0
int32_t __fastcall SKILLS_SrvDo030_Curse(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0B9F0
//1.14d: 0x005C39C0
void __fastcall SKILLS_ApplyDimVisionToTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillLevel);
//D2Game.0x6FD0BB60
//1.14d: 0x005C3B90
int32_t __fastcall SKILLS_SrvDo059_Attract(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0BDA0
//1.14d: 0x005C3B30
int32_t __fastcall SKILLS_AuraCallback_SetAttractTarget(D2UnitStrc* pUnit, void* pArg);
//D2Game.0x6FD0BE50
//1.14d: 0x005C3F20
int32_t __fastcall SKILLS_SrvDo061_Confuse(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0C060
//1.14d: 0x005C3DE0
int32_t __fastcall SKILLS_AuraCallback_ApplyConfuse(D2UnitStrc* pUnit, void* pArg);
//D2Game.0x6FD0C2B0
//1.14d: 0x005C3DB0
void __fastcall SKILLS_CurseStateCallback_ResetAlignment(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList);
//D2Game.0x6FD0C2E0
//1.14d: 0x005C40D0
void __fastcall D2GAME_SetSummonResistance_6FD0C2E0(D2UnitStrc* pUnit, D2UnitStrc* pPet);
//D2Game.0x6FD0C3A0
//1.14d: 0x005C4180
void __fastcall D2GAME_SetUnitComponent_6FD0C3A0(D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nLevel, int32_t bHasShield, int32_t bSpecial);
//D2Game.0x6FD0C500
//1.14d: 0x00541830
void __fastcall SKILLS_SetMonsterComponent(D2UnitStrc* pMonster, uint8_t nIndex, uint8_t nComponent);
//D2Game.0x6FD0C530
//1.14d: 0x005C4470
int32_t __fastcall D2GAME_SetSummonPassiveStats_6FD0C530(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nSkillId, int32_t nSkillLevel, int32_t nItemLevel);
//D2Game.0x6FD0CB10
//1.14d: 0x005C49E0
int32_t __fastcall D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nPetLevelArg, int32_t nSkillLevel);
//D2Game.0x6FD0CC10
//1.14d: 0x005C4B00
int32_t __fastcall SKILLS_SrvDo031_RaiseSkeleton_Mage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0CFC0
//1.14d: 0x005C4CD0
int32_t __fastcall SKILLS_SrvDo032_PoisonDagger(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0D000
//1.14d: 0x005C4D10
int32_t __fastcall SKILLS_AuraCallback_ApplyCorpseExplosionDamage(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FD0D0D0
//1.14d: 0x005C4DF0
int32_t __fastcall SKILLS_SrvDo055_CorpseExplosion(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0D620
//1.14d: 0x005C5100
int32_t __fastcall SKILLS_SrvDo056_Golem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0D7B0
//1.14d: 0x005C5250
int32_t __fastcall SKILLS_SrvDo057_IronGolem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0DAC0
//1.14d: 0x005C56C0
int32_t __fastcall SKILLS_SrvDo058_Revive(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0DF40
//1.14d: 0x005C55C0
void __fastcall SKILLS_InitializeRevivedMonster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pMonster, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0E050
//1.14d: 0x005C58B0
int32_t __fastcall SKILLS_SrvDo060_BoneWall(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0E4C0
//1.14d: 0x005C5D00
int32_t __fastcall SKILLS_SrvDo062_BonePrison(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0E790
//1.14d: 0x005C5E60
int32_t __fastcall SKILLS_SrvDo063_PoisonExplosion(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0E840
//1.14d: 0x005C5F50
int32_t __fastcall SKILLS_EventFunc04_ReflectPhysicalDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0EDE0
//1.14d: 0x005C61E0
int32_t __fastcall D2GAME_EventFunc05_6FD0EDE0(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0F000
//1.14d: 0x005C6370
int32_t __fastcall SKILLS_EventFunc22_AbsorbPhysicalDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0F1F0
//1.14d: 0x005C6530
int32_t __fastcall D2GAME_EventFunc23_6FD0F1F0(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0F590
//1.14d: 0x005C5F10
int32_t __fastcall SKILLS_AddLifeAndGetGain(D2UnitStrc* pUnit, int32_t nValue);
//D2Game.0x6FD0F5E0
//1.14d: 0x005C6690
int32_t __fastcall D2GAME_EventFunc26_6FD0F5E0(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0F7A0
//1.14d: 0x005C6760
int32_t __fastcall D2GAME_EventFunc27_6FD0F7A0(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
