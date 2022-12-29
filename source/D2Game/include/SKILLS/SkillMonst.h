#pragma once

#include "D2Structs.h"
#include <Units/Units.h>


//D2Game.0x6FD061C0
int32_t __fastcall SKILLS_SrvSt42_FireHit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD062E0
int32_t __fastcall SKILLS_SrvDo083_FireHit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06330
int32_t __fastcall SKILLS_SrvSt43_MaggotEgg(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06360
int32_t __fastcall SKILLS_SrvDo084_MaggotEgg(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD064D0
int32_t __fastcall SKILLS_SrvDo085_UnholyBolt_ShamanFire(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06590
int32_t __fastcall SKILLS_SrvSt44_MaggotUp(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD066A0
int32_t __fastcall SKILLS_SrvSt45_MaggotDown(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06730
int32_t __fastcall SKILLS_SrvDo086_MaggotDown(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06880
int32_t __fastcall SKILLS_SrvDo087_MaggotLay(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06A60
int32_t __fastcall SKILLS_SrvSt46_AndrialSpray(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06B20
int32_t __fastcall SKILLS_SrvDo088_AndrialSpray(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06D60
int32_t __fastcall SKILLS_SrvSt47_Jump(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD070F0
int32_t __fastcall SKILLS_SrvDo089_Jump(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD074B0
int32_t __fastcall SKILLS_SrvSt48_SwarmMove(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07540
int32_t __fastcall SKILLS_SrvDo090_SwarmMove(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD075E0
int32_t __fastcall SKILLS_SrvSt49_Nest_EvilHutSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07770
int32_t __fastcall SKILLS_SrvDo091_Nest_EvilHutSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07920
int32_t __fastcall SKILLS_SrvSt50_QuickStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07970
int32_t __fastcall SKILLS_SrvDo092_QuickStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07A30
int32_t __fastcall SKILLS_SrvDo093_GargoyleTrap(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07C50
int32_t __fastcall SKILLS_SrvSt51_Submerge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07C70
int32_t __fastcall SKILLS_SrvDo094_Submerge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07CB0
int32_t __fastcall SKILLS_SrvSt52_Emerge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07CD0
int32_t __fastcall SKILLS_SrvSt53_MonInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07D80
void __fastcall SKILLS_SetInfernoFrame(D2SkillsTxt* pSkillsTxtRecord, D2UnitStrc* pUnit);
//D2Game.0x6FD07E30
int32_t __fastcall SKILLS_SrvDo095_MonInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD081E0
int32_t __fastcall SKILLS_UpdateInfernoAnimationParameters(D2UnitStrc* pUnit, D2UnitStrc* pMissile, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD082D0
int32_t __fastcall SKILLS_SrvDo152_DiabLight(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD08680
int32_t __fastcall SKILLS_SrvDo096_ZakarumHeal_Bestow(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD08850
int32_t __fastcall SKILLS_ResurrectUnit(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FD089E0
int32_t __fastcall SKILLS_SrvDo097_Resurrect(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD08BB0
int32_t __fastcall SKILLS_SrvDo098_MonTeleport(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD08CD0
int32_t __fastcall SKILLS_SrvDo099_PrimePoisonNova(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD08EB0
int32_t __fastcall SKILLS_SrvDo100_DiabCold(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09000
int32_t __fastcall SKILLS_SrvDo101_FingerMageSpider(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD092B0
int32_t __fastcall SKILLS_SrvDo102_DiabWall(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09410
void __fastcall SKILLS_MissileInit_DiabWall(D2UnitStrc* pMissile, int32_t nInitSeed);
//D2Game.0x6FD094B0
int32_t __fastcall SKILLS_SrvSt54_DiabRun(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09570
int32_t __fastcall SKILLS_SrvDo103_DiabRun(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09890
int32_t __fastcall SKILLS_SrvDo104_DiabPrison(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09970
int32_t __fastcall SKILLS_SrvDo105_DesertTurret(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09BF0
int32_t __fastcall SKILLS_SrvDo106_ArcaneTower(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09C90
int32_t __fastcall SKILLS_SrvSt55_Mosquito(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09DE0
int32_t __fastcall SKILLS_SrvDo107_Mosquito(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A190
int32_t __fastcall SKILLS_SrvDo108_RegurgitatorEat(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A340
int32_t __fastcall SKILLS_GetMonFrenzySequenceFrame(D2UnitStrc* pUnit);
//D2Game.0x6FD0A3D0
int32_t __fastcall SKILLS_RollMonFrenzyDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A520
int32_t __fastcall SKILLS_SrvSt25_64_DragonClaw_MonFrenzy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A530
int32_t __fastcall SKILLS_SrvDo109_MonFrenzy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A5E0
int32_t __fastcall SKILLS_SrvDo148_DoomKnightMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A720
int32_t __fastcall SKILLS_SrvDo149_NecromageMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A860
int32_t __fastcall SKILLS_SrvDo110_Hireable_RogueMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A9B0
int32_t __fastcall SKILLS_SrvDo112_MonCurseCast(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0AC20
D2UnitStrc* __fastcall SKILLS_CreateSpiderLayMissile(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FD0AE10
int32_t __fastcall SKILLS_SrvDo111_FetishAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
