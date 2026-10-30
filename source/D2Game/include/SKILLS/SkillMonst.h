#pragma once

#include <Units/Units.h>
#include <DataTbls/SkillsTbls.h>

//D2Game.0x6FD061C0
//1.14d: 0x005CAE40
int32_t __fastcall SKILLS_SrvSt42_FireHit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD062E0
//1.14d: 0x005CAF30
int32_t __fastcall SKILLS_SrvDo083_FireHit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06330
//1.14d: 0x005CAF80
int32_t __fastcall SKILLS_SrvSt43_MaggotEgg(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06360
//1.14d: 0x005CAFA0
int32_t __fastcall SKILLS_SrvDo084_MaggotEgg(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD064D0
//1.14d: 0x005CB0C0
int32_t __fastcall SKILLS_SrvDo085_UnholyBolt_ShamanFire(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06590
//1.14d: 0x005CB170
int32_t __fastcall SKILLS_SrvSt44_MaggotUp(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD066A0
//1.14d: 0x005CB270
int32_t __fastcall SKILLS_SrvSt45_MaggotDown(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06730
//1.14d: 0x005CB300
int32_t __fastcall SKILLS_SrvDo086_MaggotDown(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06880
//1.14d: 0x005CB3C0
int32_t __fastcall SKILLS_SrvDo087_MaggotLay(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06A60
//1.14d: 0x005CB4D0
int32_t __fastcall SKILLS_SrvSt46_AndrialSpray(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06B20
//1.14d: 0x005CB580
int32_t __fastcall SKILLS_SrvDo088_AndrialSpray(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD06D60
//1.14d: 0x005CB730
int32_t __fastcall SKILLS_SrvSt47_Jump(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD070F0
//1.14d: 0x005CB940
int32_t __fastcall SKILLS_SrvDo089_Jump(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD074B0
//1.14d: 0x005CBBF0
int32_t __fastcall SKILLS_SrvSt48_SwarmMove(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07540
//1.14d: 0x005CBC80
int32_t __fastcall SKILLS_SrvDo090_SwarmMove(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD075E0
//1.14d: 0x005CBD10
int32_t __fastcall SKILLS_SrvSt49_Nest_EvilHutSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07770
//1.14d: 0x005CBE00
int32_t __fastcall SKILLS_SrvDo091_Nest_EvilHutSpawner(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07920
//1.14d: 0x005CBF30
int32_t __fastcall SKILLS_SrvSt50_QuickStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07970
//1.14d: 0x005CBF90
int32_t __fastcall SKILLS_SrvDo092_QuickStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07A30
//1.14d: 0x005CC050
int32_t __fastcall SKILLS_SrvDo093_GargoyleTrap(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07C50
//1.14d: 0x005CC1D0
int32_t __fastcall SKILLS_SrvSt51_Submerge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07C70
//1.14d: 0x005CC1F0
int32_t __fastcall SKILLS_SrvDo094_Submerge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07CB0
//1.14d: 0x005CC220
int32_t __fastcall SKILLS_SrvSt52_Emerge(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07CD0
//1.14d: 0x005CC240
int32_t __fastcall SKILLS_SrvSt53_MonInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD07D80
//1.14d: 0x005CC440
void __fastcall SKILLS_SetInfernoFrame(D2SkillsTxt* pSkillsTxtRecord, D2UnitStrc* pUnit);
//D2Game.0x6FD07E30
//1.14d: 0x005CC4E0
int32_t __fastcall SKILLS_SrvDo095_MonInferno(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD081E0
//1.14d: 0x005CC2E0
int32_t __fastcall SKILLS_UpdateInfernoAnimationParameters(D2UnitStrc* pUnit, D2UnitStrc* pMissile, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD082D0
//1.14d: 0x005CC690
int32_t __fastcall SKILLS_SrvDo152_DiabLight(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD08680
//1.14d: 0x005CC840
int32_t __fastcall SKILLS_SrvDo096_ZakarumHeal_Bestow(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD08850
//1.14d: 0x005CC960
int32_t __fastcall SKILLS_ResurrectUnit(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FD089E0
//1.14d: 0x005CCB10
int32_t __fastcall SKILLS_SrvDo097_Resurrect(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD08BB0
//1.14d: 0x005CCC80
int32_t __fastcall SKILLS_SrvDo098_MonTeleport(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD08CD0
//1.14d: 0x005CCD10
int32_t __fastcall SKILLS_SrvDo099_PrimePoisonNova(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD08EB0
//1.14d: 0x005CCE80
int32_t __fastcall SKILLS_SrvDo100_DiabCold(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09000
//1.14d: 0x005CCFA0
int32_t __fastcall SKILLS_SrvDo101_FingerMageSpider(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD092B0
//1.14d: 0x005CD1C0
int32_t __fastcall SKILLS_SrvDo102_DiabWall(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09410
//1.14d: 0x005CD110
void __fastcall SKILLS_MissileInit_DiabWall(D2UnitStrc* pMissile, int32_t nInitSeed);
//D2Game.0x6FD094B0
//1.14d: 0x005CD2C0
int32_t __fastcall SKILLS_SrvSt54_DiabRun(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09570
//1.14d: 0x005CD380
int32_t __fastcall SKILLS_SrvDo103_DiabRun(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09890
//1.14d: 0x005CD5D0
int32_t __fastcall SKILLS_SrvDo104_DiabPrison(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09970
//1.14d: 0x005CD6A0
int32_t __fastcall SKILLS_SrvDo105_DesertTurret(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09BF0
//1.14d: 0x005CD870
int32_t __fastcall SKILLS_SrvDo106_ArcaneTower(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09C90
//1.14d: 0x005CD910
int32_t __fastcall SKILLS_SrvSt55_Mosquito(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD09DE0
//1.14d: 0x005CDA00
int32_t __fastcall SKILLS_SrvDo107_Mosquito(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A190
//1.14d: 0x005CDC10
int32_t __fastcall SKILLS_SrvDo108_RegurgitatorEat(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A340
//1.14d: 0x005CDD30
int32_t __fastcall SKILLS_GetMonFrenzySequenceFrame(D2UnitStrc* pUnit);
//D2Game.0x6FD0A3D0
//1.14d: 0x005CDDD0
int32_t __fastcall SKILLS_RollMonFrenzyDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A520
int32_t __fastcall SKILLS_SrvSt25_64_DragonClaw_MonFrenzy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A530
//1.14d: 0x005CDF10
int32_t __fastcall SKILLS_SrvDo109_MonFrenzy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A5E0
//1.14d: 0x005CDFB0
int32_t __fastcall SKILLS_SrvDo148_DoomKnightMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A720
//1.14d: 0x005CE0B0
int32_t __fastcall SKILLS_SrvDo149_NecromageMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A860
//1.14d: 0x005CE1B0
int32_t __fastcall SKILLS_SrvDo110_Hireable_RogueMissile(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0A9B0
//1.14d: 0x005CE2B0
int32_t __fastcall SKILLS_SrvDo112_MonCurseCast(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD0AC20
//1.14d: 0x005CE4F0
D2UnitStrc* __fastcall SKILLS_CreateSpiderLayMissile(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FD0AE10
//1.14d: 0x005CE670
int32_t __fastcall SKILLS_SrvDo111_FetishAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
