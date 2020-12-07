#pragma once

#include "CommonDefinitions.h"


//TODO: Redo Header defs when .cpp is done


//D2Common.0x6FDA4C10 (#11082)
int __fastcall MONSTERS_HirelingInit(BOOL bExpansion, D2UnitStrc* pMonster, int nLowSeed, int nAct, int nDifficulty, D2HirelingInitStrc* pHirelingInit);
//D2Common.0x6FDA4E20 (#11081)
BOOL __fastcall D2Common_11081(int nLowSeed, D2HirelingInitStrc* pHirelingInit, BYTE a3);
//D2Common.0x6FDA51A0 (#11085)
wchar_t* __fastcall MONSTERS_GetHirelingDescString(int nId);
//D2Common.0x6FDA51C0 (#11086)
int __fastcall MONSTERS_GetActFromHirelingTxt(BOOL bExpansion, int nClassId, WORD nNameId);
//D2Common.0x6FDA5200 (#11084)
int __stdcall MONSTERS_GetHirelingExpForNextLevel(int nLevel, int nExpPerLevel);
//D2Common.0x6FDA5220 (#11083)
int __stdcall MONSTERS_GetHirelingResurrectionCost(D2UnitStrc* pHireling);
//D2Common.0x6FDA5270 (#11068)
int __stdcall D2COMMON_11068_GetCompInfo(D2UnitStrc* pMonster, int nComponent);
//D2Common.0x6FDA52F0 (#11069)
DWORD __stdcall D2Common_11069(D2UnitStrc* pMonster, unsigned int nIndex, unsigned int nComponent);
//D2Common.0x6FDA5450 (#11070)
int __stdcall D2Common_11070(int nMonsterId, unsigned int nComponent, unsigned int a3);
//D2Common.0x6FDA54E0 (#11050)
int __stdcall D2Common_11050(D2UnitStrc* pUnit, int a2);
//D2Common.0x6FDA55E0 (#11052)
char __stdcall D2Common_11052(BYTE a1);
//D2Common.0x6FDA5600 (#11053)
char __stdcall D2Common_11053(BYTE a1);
//D2Common.0x6FDA5620 (#11054)
char __stdcall D2Common_11054(BYTE a1);
//D2Common.0x6FDA5640 (#11055)
void __stdcall D2Common_11055(BYTE a1, int* a2, int* a3);
//D2Common.0x6FDA5670 (#11297)
void __fastcall MONSTERS_SetMonsterNameInMonsterData(D2UnitStrc* pMonster, wchar_t* wszName);
//D2Common.0x6FDA56C0
BOOL __fastcall MONSTERS_CanBeInTown(D2UnitStrc* pMonster);
//D2Common.0x6FDA5750 (#11057)
BOOL __stdcall MONSTESR_IsSandLeaper(D2UnitStrc* pMonster, BOOL bAlwaysReturnFalse);
//D2Common.0x6FDA57D0 (#11058)
BOOL __stdcall MONSTERS_IsDemon(D2UnitStrc* pMonster);
//D2Common.0x6FDA5830 (#11059)
BOOL __stdcall MONSTERS_IsUndead(D2UnitStrc* pMonster);
//D2Common.0x6FDA58A0 (#11060)
BOOL __stdcall MONSTERS_IsBoss(D2MonStatsTxt* pMonStatsTxtRecord, D2UnitStrc* pMonster);
//D2Common.0x6FDA5900 (#11064)
BOOL __stdcall MONSTERS_IsDead(D2UnitStrc* pMonster);
//D2Common.0x6FDA5930 (#11280)
int __stdcall MONSTERS_GetSpawnMode_XY(D2UnitStrc* pMonster, BOOL bFromMonster, int nSkillId, int nUnused, int* pSpawnMode, int* pX, int* pY);
//D2Common.0x6FDA5B30 (#11061)
void __stdcall MONSTERS_GetMinionSpawnInfo(D2UnitStrc* pMonster, int* pId, int* pX, int* pY, int* pSpawnMode, int nDifficulty, int(__fastcall* pfSpawnClassCallback)(D2UnitStrc*));
//D2Common.0x6FDA6410 (#11051)
BYTE __stdcall MONSTERS_GetMaximalLightRadius(D2UnitStrc* pMonster);
//D2Common.0x6FDA64B0 (#11063)
int __stdcall D2Common_11063(D2RoomStrc* pRoom, int nMonsterId);
//D2Common.0x6FDA6620 (#11065)
BOOL __stdcall MONSTERS_IsPrimeEvil(D2UnitStrc* pMonster);
//D2Common.0x6FDA6680 (#11066)
void __stdcall D2Common_11066(D2UnitStrc* pMonster, int* pDirectionX, int* pDirectionY);
//D2Common.0x6FDA6730 (#11067)
int __stdcall MONSTERS_GetHirelingTypeId(D2UnitStrc* pHireling);
//D2Common.0x6FDA6790 (#11246)
void __stdcall D2Common_11246(D2UnitStrc* pMonster, int a2, BYTE a3);
//D2Common.0x6FDA6920
int __fastcall MONSTERS_GetBaseIdFromMonsterId(int nMonsterId);
//D2Common.0x6FDA6950
int __fastcall MONSTERS_GetClassIdFromMonsterChain(int nMonsterId, int nChainId);
//D2Common.0x6FDA69C0
int __fastcall MONSTERS_ValidateMonsterId(int nMonsterId);
