#pragma once

#include "AiGeneral.h"
#include <Units/Units.h>
#include "AiStates.h"

#pragma pack(push, 1)
struct D2BaalThroneAiCallbackArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	int32_t nMonsterCount;
	int32_t nMaxDistance;
};

struct D2AiTargetSearchArgsStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	int32_t bCheckTargetCollision;
	int32_t nMaxDistance;
	int32_t unk0x10;
	D2UnitStrc* pAlternativeTarget;
	int32_t nAlternativeDistance;
};

struct D2AiEligibleTargetSearchArgsStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	D2UnitStrc* pAlternativeTarget;
	int32_t nAlternativeDistance;
};

struct D2AiExcludedTargetSearchArgsStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	D2UnitStrc* pExcludedTarget;
};

struct D2AiLineTargetSearchArgsStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	D2UnitStrc* pExcludedTarget;
};

struct D2DoorObjectAiCallbackArgStrc
{
	D2UnitStrc* pDoor;
	int32_t nDistance;
};

struct D2FallenShamanAiCallbackArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nCounter;
	int32_t nDistance;
	int32_t nMaxDistance;
};

struct D2VileMotherAiCallbackArgStrc
{
	int32_t nLastInClass;
	int32_t nMaxDistance;
	int32_t nCounter;
};

struct D2AiDeathModeSearchArgsStrc
{
	int32_t nMaxDistance;
};

#ifdef D2_VERSION_HAS_UBERS
struct D2UbersAiCallbackArgStrc
{
	int32_t nUberMephisto; // BOOL
	int32_t nUberDiablo; // BOOL
	int32_t nUberBaal; // BOOL
	int32_t nDistance;
	int32_t nOtherMonsters;
	int32_t nDiabloSpawner;
	int32_t nBaalMinions;
	int32_t nDiabloMinions;
};
#endif
#pragma pack(pop)


//Inlined in D2Game.0x6FCF1210 and D2Game.0x6FCF1310
int32_t __fastcall AIUTIL_GetDistanceToCoordinatesWithSize(D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nSize);
//D2Game.0x6FCF1210
int32_t __fastcall AIUTIL_GetDistanceToCoordinates_FullUnitSize(D2UnitStrc* pTarget, D2UnitStrc* pSource);
//D2Game.0x6FCF1310
int32_t __fastcall AIUTIL_GetDistanceToCoordinates_HalfUnitSize(D2UnitStrc* pUnit, int32_t nX, int32_t nY);
//D2Game.0x6FCF13B0
int32_t __fastcall AIUTIL_GetDistanceToCoordinates_NoUnitSize(D2UnitStrc* pUnit, int32_t nX, int32_t nY);
//D2Game.0x6FCF1440
uint32_t __fastcall AIUTIL_GetDistanceToCoordinates(D2UnitStrc* pUnit, int32_t nX, int32_t nY);
//D2Game.0x6FCF14D0
int32_t __fastcall AIUTIL_HasClearApproachToTarget(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2);
//D2Game.0x6FCF16D0
D2UnitStrc* __fastcall AIUTIL_TargetCallback_CountAndFindFriendlyMonster(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1740
D2UnitStrc* __fastcall AIUTIL_TargetCallback_FindMonsterInDeathMode(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1780
D2UnitStrc* __fastcall AIUTIL_TargetCallback_SelectHostileTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1980
D2UnitStrc* __fastcall AIUTIL_TargetCallback_SelectEligibleHostileTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1A50
int32_t __fastcall AIUTIL_CanSelectHostileTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//D2Game.0x6FCF1B30
D2UnitStrc* __fastcall AIUTIL_TargetCallback_SelectHighThreatTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1BD0
D2UnitStrc* __fastcall AIUTIL_TargetCallback_SelectHighThreatTargetWithClearApproach(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1CB0
D2UnitStrc* __fastcall AIUTIL_TargetCallback_FindClosestClosedDoor(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1DC0
D2UnitStrc* __fastcall AIUTIL_TargetCallback_CountDeadMinions(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1E30
D2UnitStrc* __fastcall AIUTIL_TargetCallback_CountLivingMonstersOfClass(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1E80
D2UnitStrc* __fastcall AIUTIL_FindTargetWithCallback(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pCallbackArg, D2UnitStrc* (__fastcall* pfFallbackCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*), int32_t nCallbackId);
#ifdef D2_VERSION_HAS_UBERS
//1.14d: 0x005DD140
D2UnitStrc* __fastcall AIUTIL_TargetCallback_Ubers(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
#endif
//D2Game.0x6FCF20E0
void __fastcall AIUTIL_MinionCallback_FindTarget(D2UnitStrc* pUnit, void* pArg, void* ppUnit);
//D2Game.0x6FCF2110
D2UnitStrc* __fastcall AIUTIL_FindAiTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, int32_t* pDistance, int32_t* pCombat);
//D2Game.0x6FCF27B0
int32_t __fastcall AIUTIL_ShouldUseAlternativeTarget(D2UnitStrc* pUnit, D2UnitStrc** ppTarget, int32_t* pTargetDistance, D2UnitStrc* pAlternativeTarget, int32_t nAlternativeDistance);
//D2Game.0x6FCF2920
int32_t __fastcall AIUTIL_ResolveTargetOverride(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t bCheckCollision, int32_t bUseEligibilityCheck, D2UnitStrc** ppUnit, int32_t* pDistance);
//D2Game.0x6FCF2B80
D2UnitStrc* __fastcall AIUTIL_FindTargetInAdjacentRooms(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg, D2UnitStrc* (__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*));
//D2Game.0x6FCF2C00
D2UnitStrc* __fastcall AIUTIL_FindTargetInAdjacentActiveRooms(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg, D2UnitStrc* (__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*));
//D2Game.0x6FCF2CC0
D2UnitStrc* __fastcall AIUTIL_SelectAiTargetWithCollisionCheck(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* pDistance, int32_t* pInMeleeRange);
//D2Game.0x6FCF2DF0
int32_t __fastcall AIUTIL_CheckAiControlFlag(D2AiControlStrc* pAiControl, uint16_t nFlag);
//D2Game.0x6FCF2E00
void __fastcall AIUTIL_ToggleAiControlFlag(D2AiControlStrc* pAiControl, uint16_t nFlag, int32_t bSet);
//D2Game.0x6FCF2E20
void __fastcall AIUTIL_ToggleAiControlFlag0x20(D2UnitStrc* pUnit, int32_t bSet);
//D2Game.0x6FCF2E70
int32_t __fastcall sub_6FCF2E70(D2UnitStrc* pUnit);
//D2Game.0x6FCF2E90
int32_t __fastcall AIUTIL_CheckIfMonsterUsesSkill(D2UnitStrc* pUnit, int32_t nSkillId);
//D2Game.0x6FCF2EF0
void __fastcall AIUTIL_SetOwnerGUIDAndType(D2UnitStrc* pUnit, D2UnitStrc* pOwner);
//D2Game.0x6FCF2F30
void __fastcall AIUTIL_ApplyTerrorCurseState(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nParam1, int32_t nDuration);
//D2Game.0x6FCF3000
BOOL __fastcall AIUTIL_CanUnitSwitchAi(D2UnitStrc* pUnit, D2C_AiSpecialState nAiSpecialState);
