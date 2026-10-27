#pragma once

#include <D2StatList.h>
#include <Units/Units.h>
#include <UNIT/SUnitDmg.h>


//D2Game.0x6FCFDCF0
//1.14d: 0x005C6910
int32_t __fastcall SKILLS_SrvDo114_Raven(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFDE90
//1.14d: 0x005C6A80
int32_t __fastcall SKILLS_SrvDo115_Vines(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE030
//1.14d: 0x005C6C50
void __fastcall SKILLS_RemoveStateAndRestoreSkillMode(D2UnitStrc* pUnit, int32_t nStateId, D2StatListStrc* pStatList);
//D2Game.0x6FCFE0E0
//1.14d: 0x005C6CC0
void __fastcall SKILLS_ApplyAuraStatsToStatList(D2UnitStrc* pUnit, D2StatListStrc* pStatList, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE200
//1.14d: 0x005C6DC0
void __fastcall SKILLS_ApplyPassiveStatsToStatList(D2UnitStrc* pUnit, D2StatListStrc* pStatList, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE330
//1.14d: 0x005C6EC0
int32_t __fastcall SKILLS_SrvDo116_Wearwolf_Wearbear(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE4C0
//1.14d: 0x005C7040
int32_t __fastcall SKILLS_CreateMissileSpread(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSubMissiles, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t bCreateFirstMissile);
//D2Game.0x6FCFE5F0
//1.14d: 0x005C7160
int32_t __fastcall SKILLS_SrvDo117_Firestorm(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE6A0
//1.14d: 0x005C72F0
int32_t __fastcall SKILLS_SrvDo118_Twister_Tornado(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE750
//1.14d: 0x005C7390
int32_t __fastcall SKILLS_SrvDo119_DruidSummon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE900
//1.14d: 0x005C7690
int32_t __fastcall SKILLS_SrvSt56_FeralRage_Maul(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFEA60
//1.14d: 0x005C77C0
int32_t __fastcall SKILLS_SrvDo120_FeralRage_Maul(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFEC90
//1.14d: 0x005C79E0
int32_t __fastcall SKILLS_SrvSt57_Rabies(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFEDD0
//1.14d: 0x005C7DB0
int32_t __fastcall SKILLS_ApplyTargetAuraAndMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nTimeout, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFEFD0
//1.14d: 0x005C8AD0
int32_t __fastcall SKILLS_SrvDo121_Rabies(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFF2E0
//1.14d: 0x005C7CE0
void __fastcall SKILLS_CreateTargetAuraMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nRange, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFF3C0
//1.14d: 0x005C7E00
int32_t __fastcall SKILLS_SrvSt58_FireClaws(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFF4E0
//1.14d: 0x005C7F10
int32_t __fastcall SKILLS_SrvDo122_Hunger(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFF710
//1.14d: 0x005C8080
int32_t __fastcall SKILLS_SrvDo123_Volcano(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFF870
//1.14d: 0x005C8190
int32_t __fastcall SKILLS_SrvDo124_Armageddon_Hurricane(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFFAB0
//1.14d: 0x005C8380
int32_t __fastcall SKILLS_SrvDo145_PeriodicAreaDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFFCA0
//1.14d: 0x005C8520
int32_t __fastcall SKILLS_SrvDo146_PeriodicRandomMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00140
//1.14d: 0x005C87A0
int32_t __fastcall SKILLS_EventFunc25_AbsorbElementalDamage(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00370
//1.14d: 0x005C8980
int32_t __fastcall SKILLS_AllocTransformSkillCombat(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillLevel);
//D2Game.0x6FD004E0
//1.14d: 0x005D1170
int32_t __fastcall SKILLS_SrvDo125_WakeOfDestruction(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00660
//1.14d: 0x005D1280
int32_t __fastcall SKILLS_SrvSt59_ImpInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00710
//1.14d: 0x005D1350
int32_t __fastcall SKILLS_SrvDo126_ImpInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD009F0
//1.14d: 0x005D1570
int32_t __fastcall SKILLS_SrvSt60_SuckBlood(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00BE0
//1.14d: 0x005D16A0
int32_t __fastcall SKILLS_SrvDo127_SuckBlood(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00DC0
//1.14d: 0x005D1800
int32_t __fastcall SKILLS_SrvDo128_CryHelp(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00EC0
//1.14d: 0x005D18E0
void __fastcall SKILLS_AttachImpToMonster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01010
//1.14d: 0x005D19D0
void __fastcall SKILLS_DetachImpFromMonster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01170
//1.14d: 0x005D1AB0
int32_t __fastcall SKILLS_SrvDo129_ImpTeleport(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01320
//1.14d: 0x005D1BF0
int32_t __fastcall SKILLS_SrvSt61_SelfResurrect(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01360
//1.14d: 0x005D1CD0
int32_t __fastcall SKILLS_SrvDo130_VineAttack(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD014A0
//1.14d: 0x005D1F70
int32_t __fastcall SKILLS_SrvDo131_OverseerWhip(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD017F0
//1.14d: 0x0054DA60
int32_t __fastcall SKILLS_GetMonsterClassFromChain(int32_t nMonsterId, int32_t nNoInChain);
//D2Game.0x6FD01860
//1.14d: 0x005D2090
int32_t __fastcall SKILLS_SrvDo132_ImpFireMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01910
//1.14d: 0x005D2140
int32_t __fastcall SKILLS_CanImpregnateTarget(D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//D2Game.0x6FD019B0
//1.14d: 0x005D2250
int32_t __fastcall SKILLS_SrvDo133_Impregnate(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01B00
//1.14d: 0x005D21B0
void __fastcall SKILLS_RemovePregnantStateAndSpawnMonster(D2UnitStrc* pUnit, int32_t nStateId, D2StatListStrc* pStatList);
//D2Game.0x6FD01BB0
//1.14d: 0x005D2320
int32_t __fastcall SKILLS_SrvDo134_SiegeBeastStomp(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01CC0
//1.14d: 0x005D2420
int32_t __fastcall SKILLS_SrvSt62_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01D40
//1.14d: 0x005D2490
int32_t __fastcall SKILLS_SrvDo135_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01F10
//1.14d: 0x005D25B0
int32_t __fastcall SKILLS_SrvDo136_DeathMaul(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD02070
//1.14d: 0x005D26F0
int32_t __fastcall SKILLS_SrvDo137_FenrisRage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD02140
//1.14d: 0x005D27B0
int32_t __fastcall SKILLS_SrvDo138_CreatePerpendicularMissileLine(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD02360
//1.14d: 0x005D2940
int32_t __fastcall SKILLS_SrvDo139_BaalColdMissiles(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD024A0
//1.14d: 0x005D2A10
int32_t __fastcall SKILLS_SrvSt63_Corpse_VineCycler(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD025C0
//1.14d: 0x005D2B20
int32_t __fastcall SKILLS_SrvDo147_UpdateRiderPath(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD025E0
//1.14d: 0x005D2B60
int32_t __fastcall D2GAME_SKILLS_BloodMana_6FD025E0(D2UnitStrc* pUnit, int32_t nManaCost);
//D2Game.0x6FD026B0
//1.14d: 0x005D2C20
int32_t __fastcall SKILLS_SrvDo140_BaalTentacle(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD02950
//1.14d: 0x005D2E80
int32_t __fastcall SKILLS_SrvDo141_BaalCorpseExplode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD02A50
//1.14d: 0x005D2E50
int32_t __fastcall SKILLS_AreaEffect_BaalCorpseExplode(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillLevel, int32_t nUnused);
//D2Game.0x6FD02A80
//1.14d: 0x005D2F80
D2UnitStrc* __fastcall SKILLS_FindUseableCorpse(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pOwner, int32_t nUnitSize);
