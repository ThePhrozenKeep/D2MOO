#pragma once

#include <Units/Units.h>


#pragma pack(push, 1)
struct D2BaalThroneAiCallbackArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	int32_t unk0x08;
	int32_t nMaxDistance;
};

struct D2UnkAiCallbackArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	int32_t unk0x08;
	int32_t nMaxDistance;
	int32_t unk0x10;
	D2UnitStrc* pAlternativeTarget;
	int32_t nAlternativeDistance;
};

struct D2UnkAiCallbackArgStrc2
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	D2UnitStrc* pAlternativeTarget;
	int32_t nAlternativeDistance;
};

struct D2AiCallback7ArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	D2UnitStrc* unk0x08;
};

struct D2AiCallback11ArgStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistance;
	D2UnitStrc* unk0x08;
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

struct UnkAiStrc5
{
	int32_t nMaxDistance;
};
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
int32_t __fastcall sub_6FCF14D0(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2);
//D2Game.0x6FCF16D0
D2UnitStrc* __fastcall sub_6FCF16D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1740
D2UnitStrc* __fastcall sub_6FCF1740(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1780
D2UnitStrc* __fastcall sub_6FCF1780(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1980
D2UnitStrc* __fastcall sub_6FCF1980(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1A50
int32_t __fastcall sub_6FCF1A50(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//D2Game.0x6FCF1B30
D2UnitStrc* __fastcall sub_6FCF1B30(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1BD0
D2UnitStrc* __fastcall sub_6FCF1BD0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1CB0
D2UnitStrc* __fastcall sub_6FCF1CB0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1DC0
D2UnitStrc* __fastcall sub_6FCF1DC0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1E30
D2UnitStrc* __fastcall sub_6FCF1E30(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pCallbackArg);
//D2Game.0x6FCF1E80
D2UnitStrc* __fastcall sub_6FCF1E80(D2GameStrc* pGame, D2UnitStrc* pUnit, void* a3, D2UnitStrc* (__fastcall* a4)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*), int32_t nCallbackId);
//D2Game.0x6FCF20E0
void __fastcall sub_6FCF20E0(D2UnitStrc* pUnit, void* pArg, void* ppUnit);
//D2Game.0x6FCF2110
D2UnitStrc* __fastcall sub_6FCF2110(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, int32_t* pDistance, int32_t* pCombat);
//D2Game.0x6FCF27B0
int32_t __fastcall sub_6FCF27B0(D2UnitStrc* pUnit, D2UnitStrc** ppTarget, int32_t* pTargetDistance, D2UnitStrc* a4, int32_t a5);
//D2Game.0x6FCF2920
int32_t __fastcall sub_6FCF2920(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t a4, D2UnitStrc** ppUnit, int32_t* pDistance);
//D2Game.0x6FCF2B80
D2UnitStrc* __fastcall AIUTIL_FindTargetInAdjacentRooms(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg, D2UnitStrc* (__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*));
//D2Game.0x6FCF2C00
D2UnitStrc* __fastcall AIUTIL_FindTargetInAdjacentActiveRooms(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg, D2UnitStrc* (__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*));
//D2Game.0x6FCF2CC0
D2UnitStrc* __fastcall sub_6FCF2CC0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* pDistance, int32_t* pInMeleeRange);
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
int32_t __fastcall AIUTIL_CanUnitSwitchAi(D2UnitStrc* pUnit, int32_t nSpecialState);
