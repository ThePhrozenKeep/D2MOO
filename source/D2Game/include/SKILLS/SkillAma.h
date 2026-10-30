#pragma once

#include <Units/Units.h>


struct D2AuraCallbackStrc;


// Same as #10049
int32_t __stdcall D2Game_10048_Return1(int32_t nUnusedParam1, int32_t nUnusedParam2, int32_t nUnusedParam3, int32_t nUnusedParam4, int32_t nUnusedParam5, int32_t nUnusedParam6);
//D2Game.0x6FCF30D0
int32_t __stdcall D2Game_10049_Return1(int32_t nUnusedParam1, int32_t nUnusedParam2, int32_t nUnusedParam3, int32_t nUnusedParam4, int32_t nUnusedParam5, int32_t nUnusedParam6);
//D2Game.0x6FCF30E0
//1.14d: 0x005DA8B0
int32_t __fastcall SKILLS_SrvSt04_Arrow_Bolt(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF3120
//1.14d: 0x005DA8F0
int32_t __fastcall SKILLS_SrvSt05_Jab(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF3170
//1.14d: 0x005DA940
int32_t __fastcall SKILLS_SrvSt06_PowerStrike_ChargedStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF3280
//1.14d: 0x005DAB40
int32_t __fastcall SKILLS_SrvSt07_Impale(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF3510
//1.14d: 0x005DACD0
int32_t __fastcall SKILLS_SrvSt08_Strafe(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF3690
//1.14d: 0x005DAE30
int32_t __fastcall SKILLS_SrvSt09_Fend(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF37B0
//1.14d: 0x005DAF40
int32_t __fastcall SKILLS_SrvSt37_Zeal_Fury_BloodLordFrenzy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF38C0
//1.14d: 0x005DB020
int32_t __fastcall SKILLS_SrvSt10_LightningStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF3A70
//1.14d: 0x005DB1C0
int32_t __fastcall SKILLS_SrvDo006_InnerSight_SlowMissiles(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF3B90
//1.14d: 0x005DB150
int32_t __fastcall SKILLS_AuraCallback_InnerSight_SlowMissiles(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FCF3C00
//1.14d: 0x005DB2D0
int32_t __fastcall SKILLS_SrvDo007_Jab(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF3D70
//1.14d: 0x005DB410
int32_t __fastcall SKILLS_SrvDo008_MultipleShot_Teeth_ShockWave(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF4080
//1.14d: 0x005DB6D0
int32_t __fastcall SKILLS_SrvDo010_GuidedArrow_BoneSpirit(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF4280
//1.14d: 0x005DB6A0
void __fastcall SKILLS_AddDamagePercentBonus(D2UnitStrc* pUnit, int32_t nBonus);
//D2Game.0x6FCF42B0
//1.14d: 0x005DB850
int32_t __fastcall SKILLS_SrvDo011_ChargedStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF44D0
//1.14d: 0x005DBA40
int32_t __fastcall SKILLS_SrvDo012_Strafe(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF47D0
//1.14d: 0x005DBC60
int32_t __fastcall SKILLS_SrvDo013_Fend_Zeal_Fury(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF4A20
//1.14d: 0x005DBE50
int32_t __fastcall SKILLS_SrvDo014_LightningStrike(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF4C50
//1.14d: 0x005DC000
int32_t __fastcall SKILLS_SrvDo015_Dopplezon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCF4EE0
//1.14d: 0x005DC1E0
int32_t __fastcall SKILLS_SrvDo016_Valkyrie(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
