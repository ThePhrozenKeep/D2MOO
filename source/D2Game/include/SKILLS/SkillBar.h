#pragma once

#include <Units/Units.h>


//D2Game.0x6FCFABF0
//1.14d: 0x005D7EA0
int32_t __fastcall SKILLS_SrvSt32_Conversion_Bash_Stun_Concentrate_BearSmite(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFAE50
//1.14d: 0x005D80C0
int32_t __fastcall SKILLS_SrvSt33_FindPotion_GrimWard(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFAE90
//1.14d: 0x005D81C0
int32_t __fastcall SKILLS_SrvDo069_FindPotion(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFB070
//1.14d: 0x005D8290
int32_t __fastcall SKILLS_ApplyWarcryStats(D2GameStrc* pGame, D2UnitStrc* pTarget, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFB1D0
//1.14d: 0x005D83E0
int32_t __fastcall SKILLS_SrvDo068_BasicShout(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFB270
//1.14d: 0x005D8470
int32_t __fastcall SKILLS_SrvDo070_DoubleSwing(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFB320
//1.14d: 0x005D8510
int32_t __fastcall SKILLS_Callback_FindTargetForTaunt(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCFB390
//1.14d: 0x005D8570
int32_t __fastcall SKILLS_SrvDo071_Taunt(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFB610
//1.14d: 0x005D8760
int32_t __fastcall SKILLS_SrvSt34_FindItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFB630
//1.14d: 0x005D8780
int32_t __fastcall SKILLS_SrvDo072_FindItem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFB7A0
//1.14d: 0x005D88B0
int32_t __fastcall SKILLS_SrvDo074_DoubleThrow(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFB8A0
//1.14d: 0x005D89B0
int32_t __fastcall SKILLS_SrvDo075_GrimWard(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFBB40
//1.14d: 0x005D8B10
int32_t __fastcall SKILLS_RollFrenzyDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFBCE0
//1.14d: 0x005D8C70
void __fastcall SKILLS_ApplyFrenzyStats(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFBE80
//1.14d: 0x005D8E00
int32_t __fastcall SKILLS_SrvDo009_Frenzy(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFBF80
//1.14d: 0x005D8EF0
void __fastcall SKILLS_CurseStateCallback_Whirlwind(D2UnitStrc* pUnit, int32_t nState, struct D2StatListStrc* pStatList);
//D2Game.0x6FCFBFE0
//1.14d: 0x005D8F50
int32_t __fastcall SKILLS_SrvSt38_Whirlwind(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFC3A0
//1.14d: 0x005D9460
int32_t __fastcall SKILLS_RemoveWhirlwindStats(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nSkillId, int32_t nX, int32_t nY);
//D2Game.0x6FCFC4C0
//1.14d: 0x005D9580
int32_t __fastcall SKILLS_SrvDo076_Whirlwind(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFC8E0
//1.14d: 0x005D97F0
int32_t __fastcall SKILLS_SrvSt39_Berserk(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFCB30
//1.14d: 0x005D9D50
int32_t __fastcall SKILLS_SrvSt40_Leap(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFCF60
//1.14d: 0x005D9A10
int32_t __fastcall SKILLS_FindLeapTargetPosition(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t* pX, int32_t* pY);
//D2Game.0x6FCFD280
//1.14d: 0x005DA370
int32_t __fastcall SKILLS_SrvDo077_Leap(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFD3C0
//1.14d: 0x005D9F70
int32_t __fastcall SKILLS_SetVelocityForLeap(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Game.0x6FCFD4C0
//1.14d: 0x005DA120
int32_t __fastcall SKILLS_Leap(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Game.0x6FCFD880
//1.14d: 0x005DA540
int32_t __fastcall SKILLS_SrvSt41_LeapAttack(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFD9B0
//1.14d: 0x005DA7E0
int32_t __fastcall SKILLS_SrvDo078_LeapAttack(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FCFDC40
//1.14d: 0x005DA490
D2UnitStrc* __fastcall SKILLS_FindLeapAttackTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SkillStrc* pSkill);
