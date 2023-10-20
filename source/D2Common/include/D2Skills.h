#pragma once

#include "CommonDefinitions.h"

#include "D2Inventory.h"

struct D2CellFileStrc; // From D2CMP / D2Gfx
struct D2RoomStrc;
struct D2SkillsTxt;

#pragma pack(1)

enum D2C_SkillCastType
{
	SKCASTTYPE_SHIFTLEFT = 0xE5,
	SKCASTTYPE_TARGETLEFT = 0xC5,
	SKCASTTYPE_SHIFTRIGHT = 0x66,
	SKCASTTYPE_RIGHT = 0x46
};

enum D2C_AuraFilters
{
	AURAFILTER_FINDPLAYER = 0x01,			//Targets Players Units
	AURAFILTER_FINDMONSTER = 0x02,			//Targets Monster Units
	AURAFILTER_FINDUNDEAD = 0x04,			//Targets Undead Monsters Units
	AURAFILTER_FINDMISSILE = 0x08,			//Targets Missiles Units			
	AURAFILTER_FINDOBJECT = 0x10,			//Target Objects Units
	AURAFILTER_FINDITEM = 0x20,				//Targets Items Units
	AURAFILTER_FINDISATT = 0x80,			//Targets Units with IsAtt (Monstats2)
	AURAFILTER_IGNINTOWN = 0x100,			//Ignore Units in Town
	AURAFILTER_USELOS = 0x200,				//Filter Uses Line of Sight
	AURAFILTER_FINDISSEL = 0x400,			//Targets Selectable Units
	AURAFILTER_FINDCORPSE = 0x1000,			//Targets Units Corpses
	AURAFILTER_IGNINTOWN2 = 0x2000,			//Ignore Units in Town
	AURAFILTER_IGNBOSS = 0x4000,			//Ignore Special Units (boss in monstats)
	AURAFILTER_IGNALLY = 0x8000,			//Ignore Allied Units
	AURAFILTER_FINDALLY = 0x10000,			//Targets Allied Units
	AURAFILTER_IGNPRIME = 0x40000,			//Ignore Prime Evils
	AURAFILTER_IGNJUSTHIT = 0x80000			//Ignore Units with Justhit State
};

enum D2C_SkillFlags
{
	SKFLAG_PASSIVE = 0x00000001,
	SKFLAG_RSKILLONLY = 0x00000002,
	SKFLAG_TARGETABLE = 0x00000004,
	SKFLAG_NEEDCORPSE = 0x00000008,
	SKFLAG_INTOWN = 0x00000010,
	SKFLAG_PHYSICAL = 0x00000020,
	SKFLAG_MAGICAL = 0x00000040,
	SKFLAG_FIRE = 0x00000080,
	SKFLAG_COLD = 0x00000100,
	SKFLAG_LTNG = 0x00000200,
	SKFLAG_POIS = 0x00000400,
	SKFLAG_KNOCKBACK = 0x00000800,
	SKFLAG_STUN = 0x00001000,
	SKFLAG_RADIUSEFFECT = 0x00002000,
	SKFLAG_MELEE = 0x00004000,
	SKFLAG_SUMMON = 0x00008000,
	SKFLAG_PARTYCAST = 0x00010000,
	SKFLAG_GUIDED = 0x00020000
};

enum D2C_SkillUseStates
{
	SKILLUSTATE_USABLE = 0,
	SKILLUSTATE_NOMANA = 1,
	SKILLUSTATE_NOQUANTITY = 2,
	SKILLUSTATE_DISABLED = 3,
	SKILLUSTATE_SHAPERESTRICT = 4,
	SKILLUSTATE_PASSIVE = 5,
	SKILLUSTATE_AURA = 6,
	SKILLUSTATE_NOLEVEL = 7,
	SKILLUSTATE_BLOCKED = 8,
};

struct D2SkillCalcStrc
{
	D2UnitStrc* pUnit;						//0x00
	int32_t nSkillId;							//0x04
	int32_t nSkillLevel;						//0x08
};

struct D2SkillStrc
{

	D2SkillsTxt* pSkillsTxt;				//0x00
	D2SkillStrc* pNextSkill;				//0x04
	uint32_t dwSkillMode;						//0x08
	uint32_t dwFlags;							//0x0C D2C_SkillFlags
	uint32_t unk0x10[2];						//0x10 - not used?

	union
	{
		uint32_t dwTargetInfo;					//0x18
		uint32_t nXpos;						//0x18
		uint32_t nPar1;						//0x18
	};
	union
	{
		uint32_t dwTargetType;					//0x1C
		uint32_t nYpos;						//0x1C
		uint32_t nPar2;						//0x1C
	};
	union
	{
		D2UnitGUID dwTargetGUID;					//0x20
		uint32_t nPar3;						//0x20
	};

	uint32_t nPar4;							//0x24
	int32_t nSkillLevel;						//0x28
	uint32_t nLevelBonus;						//0x2C
	int32_t nQuantity;							//0x30
	D2UnitGUID nOwnerGUID;						//0x34 -1 = Native Skill
	int32_t nCharges;							//0x38
};

struct D2SkillListStrc
{
	void* pMemPool;							//0x00
	D2SkillStrc* pFirstSkill;				//0x04
	D2SkillStrc* pLeftSkill;				//0x08
	D2SkillStrc* pRightSkill;				//0x0C
	D2SkillStrc* pUsedSkill;				//0x10
	uint32_t unk014;						//0x14
};

struct D2SkillTreeChartStrc
{
	D2CellFileStrc* pCellFile;				//0x00
	char* szFileName;						//0x04
	uint32_t unk0x08[6];						//0x08
	uint16_t unk0x20;							//0x20
};



#pragma pack()


//D2Common.0x6FDAEB10 (#10938)
D2COMMON_DLL_DECL short __stdcall SKILLS_GetPassiveState(int nSkillId);
//D2Common.0x6FDAEB60 (#11271)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetSpecialParamValue(D2UnitStrc* pUnit, uint8_t nParamId, int nSkillId, int nSkillLevel);
//D2Common.0x6FDAF6A0
int __fastcall sub_6FDAF6A0(int32_t nParamId, void* pUserData);
//D2Common.0x6FDAF6C0
int __fastcall sub_6FDAF6C0(int nSkillId, int nParamId, int nUnused, void* pUserData);
//D2Common.0x6FDAF780
int __fastcall sub_6FDAF780(int nMissileId, int nParamId, int nUnused, void* pUserData);
//D2Common.0x6FDAF7A0
int __fastcall sub_6FDAF7A0(int nSkillId, int a2, int a3 , void* pUserData);
//D2Common.0x6FDAF7E0 (#11276)
D2COMMON_DLL_DECL int __stdcall SKILLS_EvaluateSkillFormula(D2UnitStrc* pUnit, unsigned int nCalc, int nSkillId, int nSkillLevel);
//D2Common.0x6FDAF850 (#11302)
D2COMMON_DLL_DECL int __stdcall SKILLS_EvaluateSkillDescFormula(D2UnitStrc* pUnit, unsigned int nCalc, int nSkillId, int nSkillLevel);
//D2Common.0x6FDAF8C0 (#10940)
D2COMMON_DLL_DECL void __stdcall SKILLS_RefreshSkill(D2UnitStrc* pUnit, int nSkillId);
//D2Common.0x6FDAFB40 (#10941)
D2COMMON_DLL_DECL void __stdcall SKILLS_RefreshPassiveSkills(D2UnitStrc* pUnit);
//D2Common.0x6FDAFC30
D2SkillStrc* __fastcall SKILLS_GetHighestLevelSkillFromSkillId(D2UnitStrc* pUnit, int nSkillId);
//D2Common.0x6FDAFC80 (#10942)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetSkillModeFromUnit(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDAFCA0 (#11049)
D2COMMON_DLL_DECL int __stdcall SKILLS_Return1(int a1);
//D2Common.0x6FDAFCB0 (#10944)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetRange(D2SkillStrc* pSkill);
//D2Common.0x6FDAFCD0 (#10945)
D2COMMON_DLL_DECL D2SkillListStrc* __stdcall SKILLS_AllocSkillList(void* pMemPool);
//D2Common.0x6FDAFD10 (#10946)
D2COMMON_DLL_DECL void __stdcall SKILLS_InitSkillList(D2UnitStrc* pUnit);
//D2Common.0x6FDAFEA0 (#10947)
D2COMMON_DLL_DECL D2SkillStrc* __stdcall SKILLS_GetNextSkill(D2SkillStrc* pSkill);
//D2Common.0x6FDAFEA0 (#10948)
D2COMMON_DLL_DECL D2SkillStrc* __stdcall SKILLS_GetFirstSkillFromSkillList(D2SkillListStrc* pSkillList);
//D2Common.0x6FDAFEC0
D2SkillStrc* __fastcall SKILLS_GetLeftSkillFromSkillList(D2SkillListStrc* pSkillList);
//D2Common.0x6FDAFEF0
D2SkillStrc* __fastcall SKILLS_GetRightSkillFromSkillList(D2SkillListStrc* pSkillList);
//D2Common.0x6FDAFF20
void __fastcall SKILLS_SetUsedSkillInSkillList(D2SkillListStrc* pSkillList, D2SkillStrc* pUsedSkill);
//D2Common.0x6FDAFF30
D2SkillStrc* __fastcall SKILLS_GetUsedSkillFromSkillList(D2SkillListStrc* pSkillList);
//D2Common.0x6FDAFF40 (#10949)
D2COMMON_DLL_DECL D2SkillStrc* __fastcall SKILLS_GetSkillById(D2UnitStrc* pUnit, int nSkillId, D2UnitGUID nOwnerGUID);
//D2Common.0x6FDAFF80 (#10950)
D2COMMON_DLL_DECL D2SkillStrc* __fastcall SKILLS_GetHighestLevelSkillFromUnitAndId(D2UnitStrc* pUnit, int nSkillId);
//D2Common.0x6FDAFFD0 (#10951)
D2COMMON_DLL_DECL void __stdcall SKILLS_RemoveSkill(D2UnitStrc* pUnit, int nSkillId, const char* szFile, int nLine);
//D2Common.0x6FDAFFF0
void __fastcall D2COMMON_SKILLS_RemoveSkill_6FDAFFF0(D2UnitStrc* pUnit, int nSkillId, int bDecrementAndCheckSkillLevel, const char* szFile, int nLine);
//D2Common.0x6FDB0270 (#10958)
D2COMMON_DLL_DECL void* __stdcall D2Common_10958(D2UnitStrc* pUnit, void* a2);
//D2Common.0x6FDB02A0 (#10959)
D2COMMON_DLL_DECL void* __stdcall D2Common_10959(D2UnitStrc* pUnit);
//D2Common.0x6FDB02C0 (#10960)
D2COMMON_DLL_DECL void __stdcall SKILLS_FreeSkillList(D2UnitStrc* pUnit);
//D2Common.0x6FDB0320 (#10952)
D2COMMON_DLL_DECL D2SkillStrc* __stdcall SKILLS_AddSkill(D2UnitStrc* pUnit, int nSkillId);
//D2Common.0x6FDB04D0 (#10953)
D2COMMON_DLL_DECL void __stdcall SKILLS_AssignSkill(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL bRemove, const char* szFile, int nLine);
//D2Common.0x6FDB05E0 (#10954)
D2COMMON_DLL_DECL void __stdcall D2Common_10954(D2UnitStrc* pUnit, D2UnitGUID nOwnerGUID, int nSkillId, int nSkillLevel, int nCharges, BOOL bFreeMemory);
//D2Common.0x6FDB08C0 (#10957)
D2COMMON_DLL_DECL D2UnitGUID __stdcall SKILLS_GetOwnerGUIDFromSkill(D2SkillStrc* pSkill);
//D2Common.0x6FDB08F0 (#10955)
D2COMMON_DLL_DECL BOOL __stdcall SKILLS_GetSkillInfo(D2SkillStrc* pSkill, D2UnitGUID* pOwnerGUID, int* pSkillId, int* pSkillLevel, int* pCharges);
//D2Common.0x6FDB0960 (#10956)
D2COMMON_DLL_DECL BOOL __stdcall SKILLS_SetCharges(D2SkillStrc* pSkill, int nCharges);
//D2Common.0x6FDB09A0 (#10961)
D2COMMON_DLL_DECL void __stdcall SKILLS_SetLeftActiveSkill(D2UnitStrc* pUnit, int nSkillId, D2UnitGUID nOwnerGUID);
//D2Common.0x6FDB0A30 (#10962)
D2COMMON_DLL_DECL void __stdcall SKILLS_SetRightActiveSkill(D2UnitStrc* pUnit, int nSkillId, D2UnitGUID nOwnerGUID);
//D2Common.0x6FDB0AC0 (#10963)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetSkillIdFromSkill(D2SkillStrc* pSkill, const char* szFile, int nLine);
//D2Common.0x6FDB0AF0 (#10965)
D2COMMON_DLL_DECL int __fastcall SKILLS_GetSeqNumFromSkill(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDB0B70 (#10964)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetUseState(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDB0F50
BOOL __fastcall D2Common_SKILLMANA_CheckStat_6FDB0F50(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDB1050
D2UnitStrc* __fastcall sub_6FDB1050(D2InventoryStrc* pInventory, int nBodyLoc);
//D2Common.0x6FDB1070
BOOL __fastcall sub_6FDB1070(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDB1130
BOOL __fastcall sub_6FDB1130(D2UnitStrc* pItem, D2UnitStrc* pUnit, D2SkillsTxt* pSkillsTxtRecord, int nType);
//D2Common.0x6FDB1380
BOOL __fastcall D2Common_SKILLS_CheckShapeRestriction_6FDB1380(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDB1400
BOOL __fastcall D2Common_SKILLMANA_CheckStartStat_6FDB1400(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDB1450
BOOL __fastcall SKILLS_CheckSkillDelay(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDB1540 (#10966)
D2COMMON_DLL_DECL D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecordFromSkill(D2SkillStrc* pSkill);
//D2Common.0x6FDB1550 (#10967)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetShrineSkillLevelBonus(D2UnitStrc* pUnit);
//D2Common.0x6FDB1580
int __fastcall SKILLS_GetBonusSkillLevel(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDB1700 (#10968)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetSkillLevel(D2UnitStrc* pUnit, D2SkillStrc* pSkill, BOOL bBonus);
//D2Common.0x6FDB1750 (#11029)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetBonusSkillLevelFromSkillId(D2UnitStrc* pUnit, int nSkillId);
//D2Common.0x6FDB17A0 (#11030)
D2COMMON_DLL_DECL void __stdcall D2Common_11030(D2UnitStrc* pUnit, int nSkillId, int a3);
//D2Common.0x6FDB1820 (#11031)
D2COMMON_DLL_DECL void __stdcall D2Common_11031(D2UnitStrc* pUnit, int nSkillId, int a3);
//D2Common.0x6FDB18B0 (#10974)
D2COMMON_DLL_DECL void __stdcall SKILLS_SetSkillMode(D2SkillStrc* pSkill, int nSkillMode);
//D2Common.0x6FDB18D0 (#10975)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetSkillMode(D2SkillStrc* pSkill);
//D2Common.0x6FDB18F0 (#10969)
D2COMMON_DLL_DECL int __stdcall D2Common_10969(D2SkillStrc* pSkill);
//D2Common.0x6FDB1920 (#10970)
D2COMMON_DLL_DECL void __stdcall D2Common_10970(D2SkillStrc* pSkill, int nUnknown);
//D2Common.0x6FDB1950 (#10971)
D2COMMON_DLL_DECL int __stdcall D2Common_10971(D2SkillStrc* pSkill);
//D2Common.0x6FDB1980 (#10972)
D2COMMON_DLL_DECL void __stdcall D2Common_10972(D2SkillStrc* pSkill, int nUnknown);
//D2Common.0x6FDB19B0 (#10973)
D2COMMON_DLL_DECL void __stdcall D2Common_10973(D2SkillStrc* pSkill);
//D2Common.0x6FDB19F0 (#10976)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetParam1(D2SkillStrc* pSkill);
//D2Common.0x6FDB1A20 (#10977)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetParam2(D2SkillStrc* pSkill);
//D2Common.0x6FDB1A50 (#10978)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetParam3(D2SkillStrc* pSkill);
//D2Common.0x6FDB1A80 (#10979)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetParam4(D2SkillStrc* pSkill);
//D2Common.0x6FDB1AB0 (#10980)
D2COMMON_DLL_DECL void __stdcall SKILLS_SetParam1(D2SkillStrc* pSkill, int nPar1);
//D2Common.0x6FDB1AE0 (#10981)
D2COMMON_DLL_DECL void __stdcall SKILLS_SetParam2(D2SkillStrc* pSkill, int nPar2);
//D2Common.0x6FDB1B10 (#10982)
D2COMMON_DLL_DECL void __stdcall SKILLS_SetParam3(D2SkillStrc* pSkill, int nPar3);
//D2Common.0x6FDB1B40 (#10983)
D2COMMON_DLL_DECL void __stdcall SKILLS_SetParam4(D2SkillStrc* pSkill, int nPar4);
//D2Common.0x6FDB1B70 (#10984)
D2COMMON_DLL_DECL void __stdcall SKILLS_SetFlags(D2SkillStrc* pSkill, int nFlags);
//D2Common.0x6FDB1BA0 (#10985)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetFlags(D2SkillStrc* pSkill);
//D2Common.0x6FDB1BC0 (#10986)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetRequiredLevel(int nSkillId);
//D2Common.0x6FDB1C00 (#10987)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetRequiredLevelBasedOnCurrent(D2UnitStrc* pUnit, int nSkillId);
//D2Common.0x6FDB1C80 (#10988)
D2COMMON_DLL_DECL BOOL __stdcall SKILLS_CheckRequiredSkills(D2UnitStrc* pUnit, int nSkillId);
//D2Common.0x6FDB1F80
D2SkillStrc* __fastcall SKILLS_GetSkill(D2UnitStrc* pUnit, int nSkillId, D2UnitGUID nOwnerGUID);
//D2Common.0x6FDB1FC0 (#10989)
D2COMMON_DLL_DECL BOOL __stdcall SKILLS_CheckRequiredAttributes(D2UnitStrc* pUnit, int nSkillId);
//D2Common.0x6FDB2110 (#10999)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetToHitFactor(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel);
//D2Common.0x6FDB21E0 (#11000)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetHighestSkillLevelById(D2UnitStrc* pUnit, int nSkillId);
//D2Common.0x6FDB2280 (#11001)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetManaCosts(int nSkillId, int nSkillLevel);
//D2Common.0x6FDB22E0
int __fastcall SKILLS_CalculateDamageBonusByLevel(int nLevel, int* pLevelDamage);
//D2Common.0x6FDB2390 (#11002)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetMinPhysDamage(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL a4);
//D2Common.0x6FDB25D0 (#11003)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetMaxPhysDamage(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL a4);
//D2Common.0x6FDB2810 (#11004)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetMinElemDamage(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL a4);
//D2Common.0x6FDB29D0
int __fastcall SKILLS_CalculateMasteryBonus(D2UnitStrc* pUnit, int nElemType, int nSrcDamage);
//D2Common.0x6FDB2B00 (#11005)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetMaxElemDamage(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL a4);
//D2Common.0x6FDB2CA0 (#11006)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetElementalLength(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, BOOL bUnused);
//D2Common.0x6FDB2E70 (#11239)
D2COMMON_DLL_DECL int __stdcall SKILLS_Return0(int arg);
//D2Common.0x6FDB2E80 (#11008)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetClassIdFromSkillId(int nSkillId);
//D2Common.0x6FDB2EC0 (#11010)
D2COMMON_DLL_DECL BOOL __stdcall SKILLS_IsPlayerClassSkill(int nSkillId, int* pPlayerClass);
//D2Common.0x6FDB2F40 (#11011)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetQuantity(D2SkillStrc* pSkill);
//D2Common.0x6FDB2F70 (#11012)
D2COMMON_DLL_DECL void __stdcall SKILLS_SetQuantity(D2SkillStrc* pSkill, int nQuantity);
//D2Common.0x6FDB2FA0 (#11014)
D2COMMON_DLL_DECL int __stdcall D2Common_11014_ConvertShapeShiftedMode(int nArrayIndex, int nMonsterId);
//D2Common.0x6FDB30A0 (#11013)
D2COMMON_DLL_DECL void __stdcall D2COMMON_11013_ConvertMode(D2UnitStrc* pUnit, int* pType, int* pClass, int* pMode, const char* szFile, int nLine);
//D2Common.0x6FDB3290 (#11015)
D2COMMON_DLL_DECL void __stdcall D2Common_11015(D2UnitStrc* pUnit, int a2, int nSkillId);
//D2Common.0x6FDB3340 (#11016)
D2COMMON_DLL_DECL int __stdcall D2Common_11016(D2UnitStrc* pUnit, D2SkillStrc* pSkill);
//D2Common.0x6FDB33A0 (#11017)
D2COMMON_DLL_DECL BOOL __stdcall D2COMMON_11017_CheckUnitIfConsumeable(D2UnitStrc* pUnit, int a2);
//D2Common.0x6FDB3480 (#11020)
D2COMMON_DLL_DECL BOOL __stdcall D2Common_11020(D2UnitStrc* pUnit);
//D2Common.0x6FDB3520 (#11022)
D2COMMON_DLL_DECL BOOL __stdcall SKILLS_CanUnitCorpseBeSelected(D2UnitStrc* pUnit);
//D2Common.0x6FDB35B0 (#11024)
D2COMMON_DLL_DECL int __stdcall D2Common_11024(D2UnitStrc* pUnit, D2UnitStrc* pItem, D2SkillStrc* pSkill, int nType, BOOL* pHasThrowBonus);
//D2Common.0x6FDB36D0 (#11023)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetWeaponMasteryBonus(D2UnitStrc* pUnit, D2UnitStrc* pItem, D2SkillStrc* pSkill, int nType);
//D2Common.0x6FDB37B0 (#11032)
D2COMMON_DLL_DECL int __stdcall D2Common_11032(D2UnitStrc* pUnit, int nSkillId, int nSkillLevel, int nType);
//D2Common.0x6FDB3910 (#11025)
D2COMMON_DLL_DECL BOOL __stdcall D2Common_11025(int nX1, int nY1, int nX2, int nY2, D2RoomStrc* pRoom, int a6);
//D2Common.0x6FDB3960 (#11026)
D2COMMON_DLL_DECL BOOL __stdcall D2Common_11026(int nX, int nY, D2UnitStrc* pUnit, int a4);
//D2Common.0x6FDB3A10 (#11027)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetShiftedManaCosts(int nSkillId, int nLevel);
//D2Common.0x6FDB3A90 (#11028)
D2COMMON_DLL_DECL int __stdcall D2Common_11028(int a1);
//D2Common.0x6FDB3AB0 (#11033)
D2COMMON_DLL_DECL int __stdcall D2Common_11033(int nLevel, int nParam, int nMax);
//D2Common.0x6FDB3B00 (#11034)
D2COMMON_DLL_DECL int __stdcall D2Common_11034(int nLevel, int nSkillId);
//D2Common.0x6FDB3B90 (#11035)
D2COMMON_DLL_DECL int __stdcall D2Common_11035(int nLevel, int nSkillId);
//D2Common.0x6FDB3C20 (#11036)
D2COMMON_DLL_DECL int __stdcall D2COMMON_11036_GetMonCurseResistanceSubtraction(int nLevel, int nSkillId);
//D2Common.0x6FDB3CB0 (#11037)
D2COMMON_DLL_DECL BOOL __stdcall SKILLS_CheckIfCanLeapTo(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int* pX, int* pY);
//D2Common.0x6FDB3F60 (#11039)
D2COMMON_DLL_DECL int __stdcall D2COMMON_11039_CheckWeaponIsMissileBased(D2UnitStrc* pUnit, int* pValue);
//D2Common.0x6FDB4020 (#11040)
D2COMMON_DLL_DECL BOOL __stdcall SKILLS_IsEnhanceable(int nSkillId);
//D2Common.0x6FDB4070 (#11230)
D2COMMON_DLL_DECL BOOL __stdcall SKILLS_RemoveTransformStatesFromShapeshiftedUnit(D2UnitStrc* pUnit);
//D2Common.0x6FDB4100 (#11041)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetClassSkillId(int nClassId, int nPosition);
//D2Common.0x6FDB4150 (#11042)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetPlayerSkillCount(int nClassId);
//D2Common.0x6FDB4180 (#11043)
D2COMMON_DLL_DECL int __stdcall D2Common_11043(D2UnitStrc* pUnit);
//D2Common.0x6FDB41D0 (#11047)
D2COMMON_DLL_DECL int __stdcall SKILLS_GetConcentrationDamageBonus(D2UnitStrc* pUnit, int nSkillId);
//D2Common.0x6FDB4260 (#11283)
D2COMMON_DLL_DECL void __stdcall SKILLS_CalculateKickDamage(D2UnitStrc* pUnit, int* pMinDamage, int* pMaxDamage, int* pDamagePercent);
