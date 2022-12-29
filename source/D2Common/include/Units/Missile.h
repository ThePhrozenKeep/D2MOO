#pragma once

#include "CommonDefinitions.h"
#include "Path/Path.h"

#pragma pack(1)
//TODO: Redo Header defs when .cpp is done

struct D2UnkMissileCalcStrc
{
	int32_t field_0;
	int32_t field_4;
	int32_t field_8;
	int32_t field_C;
	int32_t field_10;
	int32_t field_14;
	int32_t field_18;
	int32_t field_1C;
	D2SeedStrc pSeed;
};

struct D2MissileCalcStrc
{
	D2UnitStrc* pMissile;					//0x00
	D2UnitStrc* pOwner;						//0x04
	int32_t nMissileId;							//0x08
	int32_t nMissileLevel;						//0x0C
};

struct D2MissileDamageDataStrc
{
	int32_t nFlags;								//0x00
	int32_t nMinDamage;							//0x04
	int32_t nMaxDamage;							//0x08
	int32_t nFireMinDamage;						//0x0C
	int32_t nFireMaxDamage;						//0x10
	int32_t nFireLength;						//0x14
	int32_t nLightMinDamage;					//0x18
	int32_t nLightMaxDamage;					//0x1C
	int32_t nMagicMinDamage;					//0x20
	int32_t nMagicMaxDamage;					//0x24
	int32_t nColdMinDamage;						//0x28
	int32_t nColdMaxDamage;						//0x2C
	int32_t nColdLength;						//0x30
	int32_t nPoisonMinDamage;					//0x34
	int32_t nPoisonMaxDamage;					//0x38
	int32_t nPoisonLength;						//0x3C
	int32_t nPoisonCount;						//0x40
	int32_t nLifeDrainMinDamage;				//0x44
	int32_t nLifeDrainMaxDamage;				//0x48
	int32_t nManaDrainMinDamage;				//0x4C
	int32_t nManaDrainMaxDamage;				//0x50
	int32_t nStaminaDrainMinDamage;				//0x54
	int32_t nStaminaDrainMaxDamage;				//0x58
	int32_t nStunLength;						//0x5C
	int32_t nBurningMin;						//0x60
	int32_t nBurningMax;						//0x64
	int32_t nBurnLength;						//0x68
	int32_t nDemonDamagePercent;				//0x6C
	int32_t nUndeadDamagePercent;				//0x70
	int32_t nDamageTargetAC;					//0x74
	int32_t nDamagePercent;						//0x78
};

struct D2MissileDataStrc
{
	uint32_t unk0x00;							//0x00 - some type of coords, see D2Common.11128, D2Common.11131 - D2Common.11134
	int16_t nStreamMissile;					//0x04
	int16_t nStreamRange;						//0x06
	int16_t nActivateFrame;					//0x08
	int16_t nSkill;							//0x0A
	int16_t nLevel;							//0x0C
	int16_t nTotalFrames;						//0x0E
	int16_t nCurrentFrame;					//0x10
	uint16_t unk0x12;							//0x12
	uint32_t fFlags;							//0x14
	int32_t nOwnerType;							//0x18
	D2UnitGUID dwOwnerGUID;						//0x1C
	int32_t nHomeType;							//0x20
	D2UnitGUID dwHomeGUID;						//0x24
	union
	{
		struct
		{
			int32_t nStatus;					//0x28 - used only by heatseeking projectiles
			D2PathPointStrc pCoords;		//0x2C - same, these are not coords, they are missile streams etc, see D2Common.11213 & D2Common.11214
		};

		D2CoordStrc pTargetCoords;			//0x28
	};
	D2MissileStreamStrc* pStream;			//0x30
};

using MissileInitFunc = void(__fastcall*)(D2UnitStrc*, int32_t);
struct D2MissileStrc
{
	uint32_t dwFlags;							//0x00
	D2UnitStrc* pOwner;							//0x04
	D2UnitStrc* pOrigin;						//0x08
	D2UnitStrc* pTarget;						//0x0C
	int32_t nMissile;							//0x10
	int32_t nX;									//0x14
	int32_t nY;									//0x18
	int32_t nTargetX;							//0x1C
	int32_t nTargetY;							//0x20
	int32_t nGfxArg;							//0x24
	int32_t nVelocity;							//0x28
	int32_t nSkill;								//0x2C
	int32_t nSkillLevel;						//0x30
	int32_t nLoops;								//0x34
	uint32_t unk0x38;							//0x38
	uint32_t unk0x3C;							//0x3C
	int32_t nFrame;								//0x40
	int32_t nActivateFrame;						//0x44
	int32_t nAttBonus;							//0x48
	int32_t nRange;								//0x4C
	int32_t nLightRadius;						//0x50
	MissileInitFunc pInitFunc;					//0x54
	int32_t pInitArgs;							//0x58
};
#pragma pack()

//D2Common.0x6FDB9F30 (#11115)
D2COMMON_DLL_DECL void __stdcall MISSILE_AllocMissileData(D2UnitStrc* pMissile);
//D2Common.0x6FDB9F80 (#11116)
D2COMMON_DLL_DECL void __stdcall MISSILE_FreeMissileData(D2UnitStrc* pMissile);
//D2Common.0x6FDB9FC0 (#11117)
D2COMMON_DLL_DECL uint32_t __stdcall MISSILE_GetFlags(D2UnitStrc* pMissile);
//D2Common.0x6FDB9FE0 (#11118)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetFlags(D2UnitStrc* pMissile, uint32_t dwFlags);
//D2Common.0x6FDBA000 (#11119)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetLevel(D2UnitStrc* pMissile, uint16_t nLevel);
//D2Common.0x6FDBA020 (#11120)
D2COMMON_DLL_DECL uint32_t __stdcall MISSILE_GetLevel(D2UnitStrc* pMissile);
//D2Common.0x6FDBA040 (#11126)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetSkill(D2UnitStrc* pMissile, int nSkill);
//D2Common.0x6FDBA080 (#11127)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetSkill(D2UnitStrc* pMissile);
//D2Common.0x6FDBA0A0 (#11121)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetTotalFrames(D2UnitStrc* pMissile, int nTotalFrames);
//D2Common.0x6FDBA0E0 (#11122)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetTotalFrames(D2UnitStrc* pMissile);
//D2Common.0x6FDBA100 (#11123)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetCurrentFrame(D2UnitStrc* pMissile, int nCurrentFrame);
//D2Common.0x6FDBA140 (#11124)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetCurrentFrame(D2UnitStrc* pMissile);
//D2Common.0x6FDBA160 (#11125)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetRemainingFrames(D2UnitStrc* pMissile);
//D2Common.0x6FDBA190 (#11128)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetClassId(D2UnitStrc* pMissile);
//D2Common.0x6FDBA1B0 (#11129)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetOwner(D2UnitStrc* pMissile, D2UnitStrc* pOwner);
//D2Common.0x6FDBA230 (#11130)
D2COMMON_DLL_DECL BOOL __stdcall MISSILE_CheckUnitIfOwner(D2UnitStrc* pMissile, D2UnitStrc* pUnit);
//D2Common.0x6FDBA2B0 (#11131)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetStreamMissile(D2UnitStrc* pMissile, uint16_t nStreamMissile);
//D2Common.0x6FDBA2D0 (#11132)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetStreamMissile(D2UnitStrc* pMissile);
//D2Common.0x6FDBA300 (#11133)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetStreamRange(D2UnitStrc* pMissile, short nStreamRange);
//D2Common.0x6FDBA320 (#11134)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetStreamRange(D2UnitStrc* pMissile);
//D2Common.0x6FDBA340 (#11135)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetHitClass(D2UnitStrc* pMissile);
//D2Common.0x6FDBA390 (#11136)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetActivateFrame(D2UnitStrc* pMissile, int nActivateFrame);
//D2Common.0x6FDBA3D0 (#11137)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetActivateFrame(D2UnitStrc* pMissile);
//D2Common.0x6FDBA3F0 (#11138)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetAnimSpeed(D2UnitStrc* pMissile);
//D2Common.0x6FDBA410 (#11139)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetAnimSpeed(D2UnitStrc* pMissile, int nAnimSpeed);
//D2Common.0x6FDBA450
void __fastcall MISSILE_SetStream(D2UnitStrc* pMissile, D2MissileStreamStrc* pStream);
//D2Common.0x6FDBA470
D2MissileStreamStrc* __fastcall MISSILE_GetStream(D2UnitStrc* pMissile);
//D2Common.0x6FDBA490 (#11140)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetTargetX(D2UnitStrc* pMissile, int nTargetX);
//D2Common.0x6FDBA4B0 (#11141)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetTargetX(D2UnitStrc* pMissile);
//D2Common.0x6FDBA4D0 (#11142)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetTargetY(D2UnitStrc* pMissile, int nTargetY);
//D2Common.0x6FDBA4F0 (#11143)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetTargetY(D2UnitStrc* pMissile);
//D2Common.0x6FDBA510 (#11144)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetHomeType(D2UnitStrc* pMissile, D2UnitStrc* pTarget);
//D2Common.0x6FDBA550 (#11145)
D2COMMON_DLL_DECL void __stdcall MISSILE_GetHomeType(D2UnitStrc* pMissile, int* nHomeType, D2UnitGUID* nHomeGUID);
//D2Common.0x6FDBA5B0 (#11217)
D2COMMON_DLL_DECL void __stdcall MISSILE_CalculateDamageData(D2MissileDamageDataStrc* pMissileDamageData, D2UnitStrc* pOwner, D2UnitStrc* pOrigin, D2UnitStrc* pMissile, int nLevel);
//D2Common.0x6FDBADF0
BOOL __fastcall MISSILE_HasBonusStats(D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Common.0x6FDBAED0
void __fastcall MISSILE_AddStatsToDamage(D2MissileDamageDataStrc* pMissileDamageData, D2UnitStrc* pMissile, uint8_t nShift);
//D2Common.0x6FDBB060
void __fastcall MISSILE_CalculateFinalDamage(D2MissileDamageDataStrc* pMissileDamageData, int nSrcDamage);
//D2Common.0x6FDBB1B0
int __fastcall MISSILE_CalculateMasteryBonus(D2UnitStrc* pUnit, int nElemType, int nSrcDamage);
//D2Common.0x6FDBB2E0 (#11218)
D2COMMON_DLL_DECL void __stdcall MISSILE_SetDamageStats(D2UnitStrc* pOwner, D2UnitStrc* pMissile, D2MissileDamageDataStrc* pMissileDamageData, int nLevel);
//D2Common.0x6FDBB5A0 (#11285)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetMinDamage(D2UnitStrc* pMissile, D2UnitStrc* pOwner, int nMissileId, int nLevel);
//D2Common.0x6FDBB710 (#11286)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetMaxDamage(D2UnitStrc* pMissile, D2UnitStrc* pOwner, int nMissileId, int nLevel);
//D2Common.0x6FDBB880 (#11289)
D2COMMON_DLL_DECL uint8_t __stdcall MISSILE_GetElemTypeFromMissileId(int nMissileId);
//D2Common.0x6FDBB8C0 (#11287)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetMinElemDamage(D2UnitStrc* pMissile, D2UnitStrc* pOwner, int nMissileId, int nLevel);
//D2Common.0x6FDBBA30 (#11288)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetMaxElemDamage(D2UnitStrc* pMissile, D2UnitStrc* pOwner, int nMissileId, int nLevel);
//D2Common.0x6FDBBBA0 (#11221)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetElementalLength(int nUnused, D2UnitStrc* pMissile, int nMissileId, int nLevel);
//D2Common.0x6FDBBC50 (#11290)
D2COMMON_DLL_DECL int __stdcall MISSILE_GetSpecialParamValue(D2UnitStrc* pMissile, D2UnitStrc* pOwner, uint8_t nParamId, int nMissileId, int nLevel);
//D2Common.0x6FDBC060
int __fastcall MISSILE_GetCalcParamValue(uint8_t nParamId, D2MissileCalcStrc* pMissileCalc);
//D2Common.0x6FDBC080
int __fastcall MISSILE_GetMinimum(int a1, int a2, int a3, int a4);
//D2Common.0x6FDBC090
int __fastcall MISSILE_GetMaximum(int a1, int a2, int a3, int a4);
//D2Common.0x6FDBC0A0
int __fastcall MISSILE_GetRandomNumberInRange(int nMin, int nMax, int nUnused, D2UnkMissileCalcStrc* pCalc);
//D2Common.0x6FDBC120
int __fastcall MISSILE_GetSpecialParamValueForSkillMissile(int nSkillId, uint8_t nParamId, int nUnused, D2MissileCalcStrc* pMissileCalc);
//D2Common.0x6FDBC170 (#11284)
D2COMMON_DLL_DECL int __stdcall MISSILE_EvaluateMissileFormula(D2UnitStrc* pMissile, D2UnitStrc* pOwner, unsigned int nCalc, int nMissileId, int nLevel);
