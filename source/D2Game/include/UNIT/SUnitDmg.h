#pragma once

#include <Units/Units.h>

#include <D2DataTbls.h>

enum D2DamageResultFlags
{
	DAMAGERESULTFLAG_SUCCESSFULHIT = 0x00000001,
	DAMAGERESULTFLAG_WILLDIE = 0x00000002,
	DAMAGERESULTFLAG_GETHIT = 0x00000004,
	DAMAGERESULTFLAG_KNOCKBACK = 0x00000008,
	DAMAGERESULTFLAG_BLOCK = 0x00000010,
	DAMAGERESULTFLAG_32 = 0x00000020,//TODO: Name
	//DAMAGERESULTFLAG_WILLDIE7 = 0x00000040,
	DAMAGERESULTFLAG_DODGE = 0x00000080,
	DAMAGERESULTFLAG_AVOID = 0x00000100,
	DAMAGERESULTFLAG_EVADE = 0x00000200,
	//DAMAGERESULTFLAG_WILLDIE11 = 0x00000400,
	//DAMAGERESULTFLAG_WILLDIE12 = 0x00000800,
	//DAMAGERESULTFLAG_WILLDIE13 = 0x00001000,
	DAMAGERESULTFLAG_CRITICALSTRIKE = 0x00002000,
	DAMAGERESULTFLAG_SOFTHIT = 0x00004000,
	DAMAGERESULTFLAG_WEAPONBLOCK = 0x00008000,
};

enum D2DamageHitFlags
{
	DAMAGEHITFLAG_1 = 0x00000001,//TODO: Name
	DAMAGEHITFLAG_2 = 0x00000002,//TODO: Name
	DAMAGEHITFLAG_LIFEDRAIN = 0x00000004,
	DAMAGEHITFLAG_MANADRAIN = 0x00000008,
	DAMAGEHITFLAG_STAMINADRAIN = 0x00000010,
	DAMAGEHITFLAG_32 = 0x00000020,//TODO: Name
	//DAMAGEHITFLAG_64 = 0x00000040,
	DAMAGEHITFLAG_128 = 0x00000080,//TODO: Name
	DAMAGEHITFLAG_BYPASSUNDEAD = 0x00000100,
	DAMAGEHITFLAG_BYPASSDEMON = 0x00000200,
	DAMAGEHITFLAG_BYPASSBEASTS = 0x00000400,
	DAMAGEHITFLAG_2048 = 0x00000800,//TODO: Name
	DAMAGEHITFLAG_4096 = 0x00001000,
};

enum D2BlockFlags
{
	BLOCKFLAG_NONE = 0x00000000,
	BLOCKFLAG_BLOCK = 0x00000001,
	BLOCKFLAG_AVOID = 0x00000002,
	BLOCKFLAG_DODGE = 0x00000004,
	BLOCKFLAG_EVADE = 0x00000008,
	BLOCKFLAG_WEAPONBLOCK = 0x00000010,
	BLOCKFLAG_ALL = BLOCKFLAG_BLOCK | BLOCKFLAG_AVOID | BLOCKFLAG_DODGE | BLOCKFLAG_EVADE | BLOCKFLAG_WEAPONBLOCK,
};

enum D2DamageReductionType : uint32_t
{
	DAMAGE_REDUCTION_NONE = 0,
	DAMAGE_REDUCTION_PHYSICAL = 1,
	DAMAGE_REDUCTION_MAGICAL = 2,
	DAMAGE_REDUCTION_TYPES_COUNT
};

#pragma pack(push, 1)

struct D2DamageStrc
{
	uint32_t dwHitFlags;					//0x00
	uint16_t wResultFlags;					//0x04
	uint16_t wExtra;						//0x06
	int32_t dwPhysDamage;					//0x08
	uint32_t dwEnDmgPct;					//0x0C
	int32_t dwFireDamage;					//0x10
	int32_t dwBurnDamage;					//0x14
	uint32_t dwBurnLen;						//0x18
	int32_t dwLtngDamage;					//0x1C
	int32_t dwMagDamage;					//0x20
	int32_t dwColdDamage;					//0x24
	int32_t dwPoisDamage;					//0x28
	uint32_t dwPoisLen;						//0x2C
	uint32_t dwColdLen;						//0x30
	uint32_t dwFrzLen;						//0x34
	int32_t dwLifeLeech;					//0x38
	int32_t dwManaLeech;					//0x3C
	int32_t dwStamLeech;					//0x40
	uint32_t dwStunLen;						//0x44
	int32_t dwAbsLife;						//0x48
	uint32_t dwDmgTotal;					//0x4C
	uint32_t unk0x50;						//0x50
	uint32_t dwPiercePct;					//0x54
	uint32_t dwDamageRate;					//0x58
	uint32_t unk0x5C;						//0x5C
	uint32_t dwHitClass;					//0x60
	uint8_t nHitClassActiveSet;				//0x64
	char nConvType;							//0x65
	uint8_t unk0x66[2];						//0x66
	int32_t dwConvPct;						//0x68
	int32_t nOverlay;						//0x6C
};

struct D2CombatStrc
{
	D2GameStrc* pGame;						//0x00
	uint32_t dwAttackerType;				//0x04
	uint32_t dwAttackerId;					//0x08
	uint32_t dwDefenderType;				//0x0C
	uint32_t dwDefenderId;					//0x10
	D2DamageStrc tDamage;					//0x14
	D2CombatStrc* pNext;					//0x84
};

struct D2DamageInfoStrc
{
	D2GameStrc* pGame;
	D2DifficultyLevelsTxt* pDifficultyLevelsTxt;
	D2UnitStrc* pAttacker;
	D2UnitStrc* pDefender;
	int32_t bAttackerIsMonster;
	int32_t bDefenderIsMonster;
	D2DamageStrc* pDamage;
	int32_t nDamageReduction[DAMAGE_REDUCTION_TYPES_COUNT];
};

struct D2DamageStatTableStrc
{
	int32_t nOffsetInDamageStrc;
	int32_t nResStatId;
	int32_t nMaxResStatId;
	int32_t nPierceStatId;
	int32_t nAbsorbPctStatId;
	int32_t nAbsorbStatId;
	D2DamageReductionType nDamageReductionType;
	int32_t unk0x1C;
	int32_t unk0x20;
	const char* szName;
	uint8_t unk0x28;
	uint8_t pad0x29;
	uint8_t pad0x2A;
	uint8_t pad0x2B;
};
#pragma pack(pop)



//D2Game.0x6FCBE2F0
int32_t __fastcall SUNITDMG_SetHitClass(D2DamageStrc* pDamage, uint32_t nHitClass);
//D2Game.0x6FCBE310
int32_t __fastcall SUNITDMG_GetColdEffect(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBE360
void __fastcall SUNITDMG_RemoveFreezeState(D2UnitStrc* pUnit, int32_t nState, int32_t a3);
//D2Game.0x6FCBE420
int32_t __fastcall SUNITDMG_ApplyDamageBonuses(D2UnitStrc* pUnit, int32_t bGetStats, D2UnitStrc* pItem, int32_t nMinDmg, int32_t nMaxDmg, int32_t nDamagePercent, int32_t nDamage, uint8_t nSrcDam);
//D2Game.0x6FCBE7E0
void __fastcall SUNITDMG_FillDamageValues(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage, int32_t a5, uint8_t nSrcDam);
//D2Game.0x6FCBF400
int32_t __fastcall SUNITDMG_CheckMonType(int32_t nMonType1, int32_t nMonType2);
//D2Game.0x6FCBF450
int32_t __fastcall SUNITDMG_RollDamageValueInRange(D2UnitStrc* pUnit, int32_t nMinDamage, int32_t nMaxDamage, int32_t nMinDamageBonusPct, int32_t nMaxDamageBonusPct, int32_t nDamage);
//D2Game.0x6FCBF620
void __fastcall SUNITDMG_CalculateTotalDamage(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage);
//D2Game.0x6FCBFB40
void __fastcall SUNITDMG_ApplyResistancesAndAbsorb(D2DamageInfoStrc* pDamageInfo, const D2DamageStatTableStrc* pDamageStatTableRecord, int32_t bDontAbsorb);
//D2Game.0x6FCBFE90
void __fastcall SUNITDMG_ExecuteEvents(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t bMissile, D2DamageStrc* pDamage);
//D2Game.0x6FCC05D0
D2MonStatsTxt* __fastcall SUNITDMG_GetMonStatsTxtRecordFromUnit(D2UnitStrc* pUnit);
//D2Game.0x6FCC05F0
int32_t __fastcall SUNITDMG_AddLeechedLife(D2UnitStrc* pUnit, int32_t nLifeLeeched);
//D2Game.0x6FCC0660
int32_t __fastcall SUNITDMG_AddLeechedMana(D2UnitStrc* pUnit, int32_t nManaLeeched);
//D2Game.0x6FCC06C0
void __fastcall SUNITDMG_ApplyPoisonDamage(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nPoisonDamage, int32_t nPoisonLength);
//D2Game.0x6FCC0800
void __fastcall SUNITDMG_ApplyBurnDamage(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nBurnDamage, int32_t nBurnLength);
//D2Game.0x6FCC0940
void __fastcall SUNITDMG_ApplyColdState(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nColdLength);
//D2Game.0x6FCC0B90
void __fastcall SUNITDMG_RemoveShatterState(D2UnitStrc* pUnit, int32_t nState, int32_t a3);
//D2Game.0x6FCC0BE0
void __fastcall SUNITDMG_ApplyFreezeState(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nFreezeLength);
//D2Game.0x6FCC0E20
void __fastcall SUNITDMG_FreeAttackerCombatList(D2GameStrc* pGame, D2UnitStrc* pAttacker);
//D2Game.0x6FCC0E90
void __fastcall SUNITDMG_FreeAttackerDefenderCombatList(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender);
//D2Game.0x6FCC0F10
void __fastcall SUNITDMG_KillMonster(D2GameStrc* pGame, D2UnitStrc* pDefender, D2UnitStrc* pAttacker, int32_t bPetKill);
//D2Game.0x6FCC1260
void __fastcall SUNITDMG_ExecuteMissileDamage(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
//D2Game.0x6FCC1870
int32_t __fastcall sub_6FCC1870(D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nHitClass);
//D2Game.0x6FCC1A50
int32_t __fastcall SUNITDMG_GetHitClass(D2DamageStrc* pDamage, uint32_t nBaseHitClass);
//D2Game.0x6FCC1AC0
void __fastcall SUNITDMG_DrainItemDurability(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nUnused);
//D2Game.0x6FCC1D70
D2DamageStrc* __fastcall SUNITDMG_GetDamageFromUnits(D2UnitStrc* pAttacker, D2UnitStrc* pDefender);
//D2Game.0x6FCC1DC0
bool __stdcall D2Game_10033(D2UnitStrc* pUnit, int32_t* a2, int32_t* a3);
//D2Game.0x6FCC1E70
int32_t __fastcall SUNITDMG_IsHitSuccessful(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nStatValue, int32_t bMissile);
//D2Game.0x6FCC2300
uint16_t __fastcall SUNITDMG_GetResultFlags(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nStatValue, int32_t nRangeOffset);
//D2Game.0x6FCC2420
void __fastcall SUNITDMG_AllocCombat(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage, uint8_t nSrcDam);
//D2Game.0x6FCC2530
int32_t __fastcall SUNITDMG_ApplyBlockOrDodge(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t bAvoid, int32_t bBlock);
//D2Game.0x6FCC2610
int32_t __fastcall SUNITDMG_ApplyDodge(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t bAvoid);
//D2Game.0x6FCC2850
int32_t __fastcall SUNITDMG_GetWeaponBlock(D2UnitStrc* pUnit);
//D2Game.0x6FCC2910
int32_t __fastcall SUNITDMG_SetMissileDamageFlagsForNearbyUnits(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nSize, D2DamageStrc* pDamage, int32_t a7, int32_t a8, int32_t(__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*), int32_t a10);
//D2Game.0x6FCC2BC0
void __fastcall SUNITDMG_RollDamage(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, D2DamageStrc* pDamage);
//D2Game.0x6FCC2BF0
void __fastcall SUNITDMG_RollSuckBloodDamage(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nSkillId, int32_t nSkillLevel, D2DamageStrc* pDamage);
//D2Game.0x6FCC2C70
void __fastcall SUNITDMG_DistributeExperience(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender);
//D2Game.0x6FCC2EC0
uint32_t __fastcall SUNITDMG_ComputeExperienceGain(D2GameStrc* pGame, D2UnitStrc* pAttacker, uint32_t nAttackerLevel, uint32_t nDefenderLevel, uint32_t nDefenderExperience);
//D2Game.0x6FCC3170
void __fastcall SUNITDMG_AddExperienceForPlayer(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t nOldLevel, uint32_t nExperienceGained);
//D2Game.0x6FCC3200
void __fastcall SUNITDMG_PartyCallback_ComputePartyExperience(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg);
//D2Game.0x6FCC3360
void __fastcall SUNITDMG_AddExperienceForHireling(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pHireling, uint32_t nLevel, uint32_t nExperienceBonus);
//D2Game.0x6FCC34A0
void __fastcall SUNITDMG_AddExperience(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t nExperienceBonus);
//D2Game.0x6FCC3510
void __fastcall SUNITDMG_SetExperienceForTargetLevel(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t nTargetLevel);
