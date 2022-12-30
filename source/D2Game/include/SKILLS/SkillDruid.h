#pragma once

#include "D2Structs.h"
#include <D2StatList.h>
#include <Units/Units.h>
#include <UNIT/SUnitDmg.h>


//D2Game.0x6FCFDCF0
int32_t __fastcall SKILLS_SrvDo114_Raven(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFDE90
int32_t __fastcall SKILLS_SrvDo115_Vines(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE030
void __fastcall sub_6FCFE030(D2UnitStrc* pUnit, int32_t nStateId, D2StatListStrc* pStatList);
//D2Game.0x6FCFE0E0
void __fastcall sub_6FCFE0E0(D2UnitStrc* pUnit, D2StatListStrc* pStatList, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE200
void __fastcall sub_6FCFE200(D2UnitStrc* pUnit, D2StatListStrc* pStatList, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE330
int32_t __fastcall SKILLS_SrvDo116_Wearwolf_Wearbear(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE4C0
int32_t __fastcall sub_6FCFE4C0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSubMissiles, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t a7);
//D2Game.0x6FCFE5F0
int32_t __fastcall SKILLS_SrvDo117_Firestorm(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE6A0
int32_t __fastcall SKILLS_SrvDo118_Twister_Tornado(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE750
int32_t __fastcall SKILLS_SrvDo119_DruidSummon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFE900
int32_t __fastcall SKILLS_SrvSt56_FeralRage_Maul(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFEA60
int32_t __fastcall SKILLS_SrvDo120_FeralRage_Maul(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFEC90
int32_t __fastcall SKILLS_SrvSt57_Rabies(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFEDD0
int32_t __fastcall sub_6FCFEDD0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nTimeout, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFEFD0
int32_t __fastcall SKILLS_SrvDo121_Rabies(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFF2E0
void __fastcall sub_6FCFF2E0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nRange, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFF3C0
int32_t __fastcall SKILLS_SrvSt58_FireClaws(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFF4E0
int32_t __fastcall SKILLS_SrvDo122_Hunger(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFF710
int32_t __fastcall SKILLS_SrvDo123_Volcano(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFF870
int32_t __fastcall SKILLS_SrvDo124_Armageddon_Hurricane(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFFAB0
int32_t __fastcall SKILLS_SrvDo145_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFFCA0
int32_t __fastcall SKILLS_SrvDo146_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00140
int32_t __fastcall D2GAME_EventFunc25_6FD00140(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00370
int32_t __fastcall sub_6FD00370(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillLevel);
//D2Game.0x6FD004E0
int32_t __fastcall SKILLS_SrvDo125_WakeOfDestruction(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00660
int32_t __fastcall SKILLS_SrvSt59_ImpInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00710
int32_t __fastcall SKILLS_SrvDo126_ImpInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD009F0
int32_t __fastcall SKILLS_SrvSt60_SuckBlood(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00BE0
int32_t __fastcall SKILLS_SrvDo127_SuckBlood(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00DC0
int32_t __fastcall SKILLS_SrvDo128_CryHelp(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD00EC0
void __fastcall sub_6FD00EC0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01010
void __fastcall sub_6FD01010(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pOwner, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01170
int32_t __fastcall SKILLS_SrvDo129_ImpTeleport(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01320
int32_t __fastcall SKILLS_SrvSt61_SelfResurrect(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01360
int32_t __fastcall SKILLS_SrvDo130_VineAttack(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD014A0
int32_t __fastcall SKILLS_SrvDo131_OverseerWhip(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD017F0
int32_t __fastcall sub_6FD017F0(int32_t nMonsterId, int32_t nNoInChain);
//D2Game.0x6FD01860
int32_t __fastcall SKILLS_SrvDo132_ImpFireMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01910
int32_t __fastcall sub_6FD01910(D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//D2Game.0x6FD019B0
int32_t __fastcall SKILLS_SrvDo133_Impregnate(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01B00
void __fastcall sub_6FD01B00(D2UnitStrc* pUnit, int32_t nStateId, D2StatListStrc* pStatList);
//D2Game.0x6FD01BB0
int32_t __fastcall SKILLS_SrvDo134_SiegeBeastStomp(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01CC0
int32_t __fastcall SKILLS_SrvSt62_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01D40
int32_t __fastcall SKILLS_SrvDo135_MinionSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD01F10
int32_t __fastcall SKILLS_SrvDo136_DeathMaul(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD02070
int32_t __fastcall SKILLS_SrvDo137_FenrisRage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD02140
int32_t __fastcall SKILLS_SrvDo138_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD02360
int32_t __fastcall SKILLS_SrvDo139_BaalColdMissiles(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD024A0
int32_t __fastcall SKILLS_SrvSt63_Corpse_VineCycler(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD025C0
int32_t __fastcall SKILLS_SrvDo147_Unused(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD025E0
int32_t __fastcall D2GAME_SKILLS_BloodMana_6FD025E0(D2UnitStrc* pUnit, int32_t nManaCost);
//D2Game.0x6FD026B0
int32_t __fastcall SKILLS_SrvDo140_BaalTentacle(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD02950
int32_t __fastcall SKILLS_SrvDo141_BaalCorpseExplode(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD02A50
int32_t __fastcall SKILLS_AreaEffect_BaalCorpseExplode(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillLevel, int32_t nUnused);
//D2Game.0x6FD02A80
D2UnitStrc* __fastcall SKILLS_FindUseableCorpse(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pOwner, int32_t nUnitSize);
