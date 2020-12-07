#pragma once

#include "CommonDefinitions.h"


//TODO: Redo Header defs when .cpp is done


//D2Common.0x6FDB9F30 (#11115)
void __stdcall MISSILE_AllocMissileData(D2UnitStrc* pMissile);
//D2Common.0x6FDB9F80 (#11116)
void __stdcall MISSILE_FreeMissileData(D2UnitStrc* pMissile);
//D2Common.0x6FDB9FC0 (#11117)
DWORD __stdcall MISSILE_GetFlags(D2UnitStrc* pMissile);
//D2Common.0x6FDB9FE0 (#11118)
void __stdcall MISSILE_SetFlags(D2UnitStrc* pMissile, DWORD dwFlags);
//D2Common.0x6FDBA000 (#11119)
void __stdcall MISSILE_SetLevel(D2UnitStrc* pMissile, WORD nLevel);
//D2Common.0x6FDBA020 (#11120)
DWORD __stdcall MISSILE_GetLevel(D2UnitStrc* pMissile);
//D2Common.0x6FDBA040 (#11126)
void __stdcall MISSILE_SetSkill(D2UnitStrc* pMissile, int nSkill);
//D2Common.0x6FDBA080 (#11127)
int __stdcall MISSILE_GetSkill(D2UnitStrc* pMissile);
//D2Common.0x6FDBA0A0 (#11121)
void __stdcall MISSILE_SetTotalFrames(D2UnitStrc* pMissile, int nTotalFrames);
//D2Common.0x6FDBA0E0 (#11122)
int __stdcall MISSILE_GetTotalFrames(D2UnitStrc* pMissile);
//D2Common.0x6FDBA100 (#11123)
void __stdcall MISSILE_SetCurrentFrame(D2UnitStrc* pMissile, int nCurrentFrame);
//D2Common.0x6FDBA140 (#11124)
int __stdcall MISSILE_GetCurrentFrame(D2UnitStrc* pMissile);
//D2Common.0x6FDBA160 (#11125)
int __stdcall MISSILE_GetRemainingFrames(D2UnitStrc* pMissile);
//D2Common.0x6FDBA190 (#11128)
int __stdcall MISSILE_GetClassId(D2UnitStrc* pMissile);
//D2Common.0x6FDBA1B0 (#11129)
void __stdcall MISSILE_SetOwner(D2UnitStrc* pMissile, D2UnitStrc* pOwner);
//D2Common.0x6FDBA230 (#11130)
BOOL __stdcall MISSILE_CheckUnitIfOwner(D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Common.0x6FDBA2B0 (#11131)
void __stdcall MISSILE_SetStreamMissile(D2UnitStrc* pMissile, WORD nStreamMissile);
//D2Common.0x6FDBA2D0 (#11132)
int __stdcall MISSILE_GetStreamMissile(D2UnitStrc* pMissile);
//D2Common.0x6FDBA300 (#11133)
void __stdcall MISSILE_SetStreamRange(D2UnitStrc* pMissile, short nStreamRange);
//D2Common.0x6FDBA320 (#11134)
int __stdcall MISSILE_GetStreamRange(D2UnitStrc* pMissile);
//D2Common.0x6FDBA340 (#11135)
int __stdcall MISSILE_GetHitClass(D2UnitStrc* pMissile);
//D2Common.0x6FDBA390 (#11136)
void __stdcall MISSILE_SetActivateFrame(D2UnitStrc* pMissile, int nActivateFrame);
//D2Common.0x6FDBA3D0 (#11137)
int __stdcall MISSILE_GetActivateFrame(D2UnitStrc* pMissile);
//D2Common.0x6FDBA3F0 (#11138)
int __stdcall MISSILE_GetAnimSpeed(D2UnitStrc* pMissile);
//D2Common.0x6FDBA410 (#11139)
void __stdcall MISSILE_SetAnimSpeed(D2UnitStrc* pMissile, int nAnimSpeed);
//D2Common.0x6FDBA450
void __fastcall MISSILE_SetStream(D2UnitStrc* pMissile, D2MissileStreamStrc* pStream);
//D2Common.0x6FDBA470
D2MissileStreamStrc* __fastcall MISSILE_GetStream(D2UnitStrc* pMissile);
//D2Common.0x6FDBA490 (#11140)
void __stdcall MISSILE_SetTargetX(D2UnitStrc* pMissile, int nTargetX);
//D2Common.0x6FDBA4B0 (#11141)
int __stdcall MISSILE_GetTargetX(D2UnitStrc* pMissile);
//D2Common.0x6FDBA4D0 (#11142)
void __stdcall MISSILE_SetTargetY(D2UnitStrc* pMissile, int nTargetY);
//D2Common.0x6FDBA4F0 (#11143)
int __stdcall MISSILE_GetTargetY(D2UnitStrc* pMissile);
//D2Common.0x6FDBA510 (#11144)
void __stdcall MISSILE_SetHomeType(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
//D2Common.0x6FDBA550 (#11145)
void __stdcall MISSILE_GetHomeType(D2UnitStrc* pMissile, int* nHomeType, int* nHomeGUID);
//D2Common.0x6FDBA5B0 (#11217)
void __stdcall MISSILE_CalculateDamageData(D2MissileDamageDataStrc* pMissileDamageData, D2UnitStrc* pOwner, D2UnitStrc* pOrigin, D2UnitStrc* pMissile, int nLevel);
//D2Common.0x6FDBADF0
BOOL __fastcall MISSILE_HasBonusStats(D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Common.0x6FDBAED0
void __fastcall MISSILE_AddStatsToDamage(D2MissileDamageDataStrc* pMissileDamageData, D2UnitStrc* pMissile, BYTE nShift);
//D2Common.0x6FDBB060
void __fastcall MISSILE_CalculateFinalDamage(D2MissileDamageDataStrc* pMissileDamageData, int nSrcDamage);
//D2Common.0x6FDBB1B0
int __fastcall MISSILE_CalculateMasteryBonus(D2UnitStrc* pUnit, int nElemType, int nSrcDamage);
//D2Common.0x6FDBB2E0 (#11218)
void __stdcall MISSILE_SetDamageStats(D2UnitStrc* pOwner, D2UnitStrc* pMissile, D2MissileDamageDataStrc* pMissileDamageData, int nLevel);
//D2Common.0x6FDBB5A0 (#11285)
int __stdcall MISSILE_GetMinDamage(D2UnitStrc* pMissile, D2UnitStrc* pOwner, int nMissileId, int nLevel);
//D2Common.0x6FDBB710 (#11286)
int __stdcall MISSILE_GetMaxDamage(D2UnitStrc* pMissile, D2UnitStrc* pOwner, int nMissileId, int nLevel);
//D2Common.0x6FDBB880 (#11289)
BYTE __stdcall MISSILE_GetElemTypeFromMissileId(int nMissileId);
//D2Common.0x6FDBB8C0 (#11287)
int __stdcall MISSILE_GetMinElemDamage(D2UnitStrc* pMissile, D2UnitStrc* pOwner, int nMissileId, int nLevel);
//D2Common.0x6FDBBA30 (#11288)
int __stdcall MISSILE_GetMaxElemDamage(D2UnitStrc* pMissile, D2UnitStrc* pOwner, int nMissileId, int nLevel);
//D2Common.0x6FDBBBA0 (#11221)
int __stdcall MISSILE_GetElementalLength(int nUnused, D2UnitStrc* pMissile, int nMissileId, int nLevel);
//D2Common.0x6FDBBC50 (#11290)
int __stdcall MISSILE_GetSpecialParamValue(D2UnitStrc* pMissile, D2UnitStrc* pOwner, BYTE nParamId, int nMissileId, int nLevel);
//D2Common.0x6FDBC060
int __fastcall MISSILE_GetCalcParamValue(BYTE nParamId, D2MissileCalcStrc* pMissileCalc);
//D2Common.0x6FDBC080
int __fastcall MISSILE_GetMinimum(int a1, int a2, int a3, int a4);
//D2Common.0x6FDBC090
int __fastcall MISSILE_GetMaximum(int a1, int a2, int a3, int a4);
//D2Common.0x6FDBC0A0
int __fastcall MISSILE_GetRandomNumberInRange(int nMin, int nMax, int nUnused, D2UnkMissileCalcStrc* pCalc);
//D2Common.0x6FDBC120
int __fastcall MISSILE_GetSpecialParamValueForSkillMissile(int nSkillId, BYTE nParamId, int nUnused, D2MissileCalcStrc* pMissileCalc);
//D2Common.0x6FDBC170 (#11284)
int __stdcall MISSILE_EvaluateMissileFormula(D2UnitStrc* pMissile, D2UnitStrc* pOwner, unsigned int nCalc, int nMissileId, int nLevel);
