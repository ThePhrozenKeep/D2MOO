#pragma once

#include <GAME/Game.h>
#include <Units/Units.h>
#include <UNIT/SUnitDmg.h>
#include <GAME/Event.h>

#pragma pack(push, 1)

struct D2ModeChangeStrc
{
	int32_t nMode;								//0x00
	D2UnitStrc* pUnit;						//0x04
	D2UnitStrc* pTargetUnit;				//0x08
	int32_t nX;									//0x0C
	int32_t nY;									//0x10
	uint8_t unk0x14[4];						//0x14
	int32_t unk0x18;							//0x18
	uint8_t unk0x1C;							//0x1C
	uint8_t unk0x1D[3];							//0x1D
};

// TODO: Better names
using MonModeStartFn = int32_t(__fastcall*)(D2GameStrc*, D2ModeChangeStrc*);
using MonModeUpdateFn = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*);
using MonModeEndFn = void(__fastcall*)(D2GameStrc*, D2UnitStrc*);
struct D2MonModeCallbackTableStrc
{
	MonModeStartFn pfStart;
	MonModeUpdateFn pfUpdate;
	MonModeEndFn pfEnd;
	int32_t bScheduleEvents;
};

#pragma pack(pop)


//D2Game.0x6FC62770
int32_t __fastcall D2GAME_IsMonster_6FC62770(D2UnitStrc* pUnit);
//D2Game.0x6FC62780
void __fastcall MONSTERMODE_DistributeExperienceIfEligible(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2GameStrc* pGame);
//D2Game.0x6FC627B0
void __fastcall MONSTERMODE_UpdateModeAttackStats(D2UnitStrc* pUnit, int32_t nMode);
//D2Game.0x6FC62D90
void __fastcall MONSTERMODE_AllocateTargetCombat(D2UnitStrc* pUnit, D2GameStrc* pGame);
//D2Game.0x6FC62DF0
void __stdcall MONSTERMODE_AddSandLeaperKnockback(D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FC62E70
void __fastcall D2GAME_MONSTER_ApplyCriticalDamage_6FC62E70(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage);
//D2Game.0x6FC62F50
uint8_t __fastcall MONSTERMODE_GetHpPercent128(D2UnitStrc* pUnit);
//D2Game.0x6FC62F90
void __fastcall D2GAME_MONSTERMODE_ResetVelocityParams_6FC62F90(D2UnitStrc* pUnit);
//D2Game.0x6FC62FC0
void __fastcall D2GAME_MONSTERMODE_SetVelocityParams_6FC62FC0(D2AiParamStrc* pAiParam, int32_t nPathType, int32_t nVel, uint8_t nNewDistance);
//D2Game.0x6FC62FF0
D2AiParamStrc* __fastcall D2GAME_MONSTERMODE_AllocParams_6FC62FF0(D2GameStrc* pGame);
//D2Game.0x6FC63020
void __fastcall D2GAME_MONSTERMODE_FreeParams_6FC63020(D2GameStrc* pGame, D2AiParamStrc* pAiParam);
//D2Game.0x6FC63040
void __fastcall MONSTERMODE_TryDropTreasure(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange, int32_t nUnused);
//D2Game.0x6FC631B0
void __fastcall MONSTERMODE_DropTreasure(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t bIgnoreNoDrop, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC63440
void __fastcall D2GAME_MONSTER_ApplyStatRegen_6FC63440(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nUnused1, int32_t nUnused2);
//D2Game.0x6FC63650
D2MonStatsTxt* __fastcall MONSTERMODE_GetMonStatsTxtRecord(int32_t nMonsterId);
//D2Game.0x6FC63680
void __fastcall MONSTERMODE_HandleEndAnimation(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC63940
void __fastcall MONSTERMODE_EndTrappedSoulAttack(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC63A30
void __fastcall MONSTERMODE_EndTrappedSoulSkill(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC63B20
int32_t __fastcall D2GAME_RemoveModeChangeEventCallback_6FC63B20(D2GameStrc* pGame, D2UnitStrc* pMonster);
//D2Game.0x6FC63B30
int32_t __fastcall MONSTERMODE_StartDeath(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC63E80
void __fastcall MONSTERMODE_CleanupDeath(D2GameStrc* pGame, D2UnitStrc* pUnit, DWORD dwDir);
//D2Game.0x6FC63FD0
void __fastcall MONSTERMODE_ApplyDeathAreaDamage(D2GameStrc* pGame, D2UnitStrc* pAttacker);
//D2Game.0x6FC64090
void __fastcall MONSTERMODE_HandleSiegeBeastDeath(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC641D0
void __fastcall MONSTERMODE_EndDeath(D2GameStrc* pGame, D2UnitStrc* pAttacker);
//D2Game.0x6FC64280
int32_t __fastcall MONSTERMODE_UpdateDeath(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC642C0
int32_t __fastcall MONSTERMODE_StartDead(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC64310
int32_t __fastcall MONSTERMODE_StartNeutral(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC643D0
int32_t __fastcall MONSTERMODE_StartSkill3(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC643E0
int32_t __fastcall MONSTERMODE_UpdateSkill3(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC64410
int32_t __fastcall MONSTERMODE_StartSkill4(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC64450
int32_t __fastcall MONSTERMODE_StartWalk(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC64480
int32_t __fastcall MONSTERMODE_UpdateWalk(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC644E0
int32_t __fastcall MONSTERMODE_StartRun(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC64510
int32_t __fastcall MONSTERMODE_UpdateRun(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC64540
int32_t __fastcall MONSTERMODE_StartGetHit(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC645E0
int32_t __fastcall MONSTERMODE_StartAttackOrSkill(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC64790
int32_t __fastcall MONSTERMODE_UpdateAttackOrSkill(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC64B10
int32_t __fastcall D2GAME_GetMonsterBaseId_6FC64B10(D2UnitStrc* pUnit);
//D2Game.0x6FC64B50
int32_t __fastcall MONSTERMODE_StartBlock(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC64B60
int32_t __fastcall MONSTERMODE_StartKnockback(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC64CD0
void __fastcall MONSTERMODE_EndKnockback(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC64E20
int32_t __fastcall MONSTERMODE_UpdateKnockback(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC64E60
int32_t __fastcall MONSTERMODE_StartSequence(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC64E90
int32_t __fastcall MONSTERMODE_UpdateSequence(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC64F50
const D2MonModeCallbackTableStrc* __fastcall MONSTERMODE_GetCallbackTableRecord(D2UnitStrc* pUnit, int32_t nMode);
//D2Game.0x6FC65080
void __fastcall MONSTERMODE_EventModeChange(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nEventCustomParam);
//D2Game.0x6FC65150
void __fastcall MONSTERMODE_EventEndAnimation(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nEventCustomParam);
//D2Game.0x6FC65220
int32_t __stdcall D2GAME_ModeChange_6FC65220(D2GameStrc* pGame, D2ModeChangeStrc* pModeChange, int32_t bClearUsedSkill);
//D2Game.0x6FC65680
int32_t __fastcall MONSTERMODE_ComputePath(D2UnitStrc* pUnit, int32_t nPathType, D2AiParamStrc* pAiParam, int32_t dwNewDist);
//D2Game.0x6FC65780
void __stdcall MONSTERMODE_GetModeChangeInfo(D2UnitStrc* pUnit, int32_t nMode, D2ModeChangeStrc* pModeChange);
//D2Game.0x6FC65890
void __fastcall MONSTERMODE_EventUpdateStatListExpirations(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nEventCustomParam);
//D2Game.0x6FC658B0
void __fastcall MONSTERMODE_EventFreeHoverText(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nEventCustomParam);
//D2Game.0x6FC65920
void __fastcall MONSTERMODE_EventClearTargetOverride(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nEventCustomParam);
//D2Game.0x6FC65930
void __fastcall MONSTERMODE_EventHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEvent, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FC659B0
void __fastcall MONSTERMODE_DropTreasureForFindItem(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t a4);
