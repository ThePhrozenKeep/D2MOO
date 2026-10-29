#pragma once

#include <D2StatList.h>
#include <Units/Units.h>
#include <Units/UnitFinds.h>
#include <UNIT/SUnitDmg.h>
#include <UNIT/SUnitEvent.h>
#include <AI/AiStates.h>

struct D2ItemStatCostTxt;
struct D2MissilesTxt;
struct D2PetTypeTxt;
struct D2StatesTxt;

using CurseCallbackFunc = void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, int32_t);


using SkillStartFunc = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*, int32_t, int32_t);
using SkillDoFunc = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*, int32_t, int32_t);
extern const SkillDoFunc gpSkillSrvDoFnTable_6FD40A20[];
extern const int32_t gnSkillSrvDoFns;

#pragma pack(push, 1)
struct D2AuraCallbackStrc
{
	D2GameStrc* pGame;  //+00
	D2UnitStrc* pOwner; //+04
	int32_t nCounter;       //+08
	void* pArgs;       //+0C
};

struct D2StaticFieldCallbackArgStrc
{
	int32_t nMinDamage;       //+00
	int32_t nDamagePct;       //+04
	int32_t nStaticFieldMin;  //+08
	int32_t nElementalLength; //+0C
	int32_t nElementalType;   //+10
};

struct D2CurseAuraCallbackArgsStrc
{
	D2GameStrc* pGame;
	D2UnitStrc* pUnit;
	int32_t bApplyAiState;
	int32_t bUpdateAnimRate;
	int32_t nSkillId;
	int32_t nSkillLevel;
	int32_t nDuration;
	int32_t nAuraStat[6];
	int32_t nAuraStatCalc[6];
	int32_t nAuraTargetState;
	int32_t nAuraEvent[3];
	int32_t nAuraEventFunc[3];
};

struct D2AuraTargetSearchStrc
{
	D2UnitStrc* pTarget;
	int32_t nDistanceSquared;
	int32_t nX;
	int32_t nY;
	int32_t(__fastcall* pCallback)(D2GameStrc*, D2UnitStrc*);
};

struct D2AuraTargetCycleStrc
{
	D2UnitStrc* pNextTarget;
	int32_t nNextTargetGUID;
	D2UnitStrc* pFirstTarget;
	int32_t nFirstTargetGUID;
	int32_t nPreviousTargetGUID;
	int32_t nTargetCount;
};

struct D2EffectStrc
{
	uint32_t nFlags;
	D2UnitStrc* pUnit;
	int32_t nSkillId;
	int32_t nX;
	int32_t nY;
	int32_t nRadius;
	int32_t nSkillLevel;
	int32_t nCallbackParam;
	void* pfAreaEffect;
};

struct D2MonSkillInfoStrc
{
	int32_t nMinDamage;
	int32_t nMaxDamage;
	int32_t nToHit;
};

struct D2CurseStrc
{
	D2UnitStrc* pUnit;								//0x00
	D2UnitStrc* pTarget;							//0x04
	int32_t nSkill;									//0x08
	int32_t nSkillLevel;							//0x0C
	int32_t nDuration;								//0x10
	int32_t nStat;									//0x14
	int32_t nStatValue;								//0x18
	int32_t nState;									//0x1C
	StatListRemoveCallback pStateRemoveCallback;	//0x20
};

struct D2SummonArgStrc
{
	uint32_t dwFlags;						//0x00
	D2UnitStrc* pOwner;						//0x04
	int32_t nHcIdx;							//0x08
	D2C_AiSpecialState nAiSpecialState;		//0x0C
	int32_t nMonMode;						//0x10
	D2CoordStrc pPosition;					//0x14		
	int32_t nPetType;						//0x1C
	int32_t nPetMax;						//0x20
};

#pragma pack(pop)


//D2Game.0x6FD0F8B0
//1.14d: 0x005C6870
int32_t __fastcall SKILLS_SharePotionLifeWithBloodGolem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nLife);
//D2Game.0x6FD0FA00
//1.14d: 0x0056B3E0
int32_t __fastcall SKILLS_CheckAuraTarget(D2UnitStrc* pSource, D2UnitStrc* pTarget, uint32_t nAuraFilter);
//D2Game.0x6FD0FDD0
//1.14d: 0x0056B6C0
void __fastcall SKILLS_ClampVitalsAndUpdateAnimRate(D2UnitStrc* pUnit);
//D2Game.0x6FD0FE50
//1.14d: 0x0056B740
void __fastcall SKILLS_ApplyAuraToSelf(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t(__fastcall* pCallback)(D2AuraCallbackStrc*, D2UnitStrc*), void* pArgs);
//D2Game.0x6FD0FE80
//1.14d: 0x0056B7E0
void __fastcall SKILLS_IterateAuraTargets(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nAuraRange, uint32_t nAuraFilter, int32_t(__fastcall* pCallback)(D2AuraCallbackStrc*, D2UnitStrc*), void* pCallbackArgs, int32_t bCheckMonAuraFlag, const char* szFile, int32_t nLine);
//D2Game.0x6FD10140
//1.14d: 0x0056B9C0
void __fastcall SKILLS_ApplyDamageToTarget(D2GameStrc* pGame, D2UnitStrc* pAttackerArg, D2UnitStrc* pTarget, D2DamageStrc* pDamage, int32_t nUnused);
//D2Game.0x6FD10200
//1.14d: 0x0056BAD0
int32_t __fastcall SKILLS_ApplyAreaDamage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nAuraRange, D2DamageStrc* pDamage, uint32_t nAuraFilter);
//D2Game.0x6FD10250
//1.14d: 0x00570590
void __fastcall SKILLS_ApplyTimedState(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nState, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD10360
//1.14d: 0x0056E780
int32_t __fastcall SKILLS_IterateTargetsAroundTarget(D2GameStrc* pGame, uint32_t nAuraFilter, D2UnitStrc* pUnit, int32_t nAuraRange, int32_t(__fastcall* pCallback)(D2UnitStrc*, void*), void* pArgs);
//D2Game.0x6FD10630
//1.14d: 0x0056BBC0
D2UnitStrc* __fastcall SKILLS_FindAuraTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nAuraRange, int32_t(__fastcall* pCallback)(D2GameStrc*, D2UnitStrc*));
//D2Game.0x6FD106F0
//1.14d: 0x0056BB20
int32_t __fastcall SKILLS_AuraCallback_FindClosestTarget(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FD10790
//1.14d: 0x0056BC80
int32_t __fastcall SKILLS_CountAuraTargets(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nAuraRange, uint32_t nAuraFilter);
//D2Game.0x6FD107E0
//1.14d: 0x0056BC70
int32_t __fastcall SKILLS_AuraCallback_CountTargets(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);
//D2Game.0x6FD107F0
//1.14d: 0x0056BD10
D2UnitStrc* __fastcall SKILLS_FindNextAuraTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nAuraRange, uint32_t nAuraFilter, int32_t nPreviousTargetGUID, int32_t* pTargetCount);
//D2Game.0x6FD10880
//1.14d: 0x0056BCD0
int32_t __fastcall SKILLS_AuraCallback_FindNextTarget(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit);

//1.14d: 0x00478A20
D2PetTypeTxt* __fastcall DATATBLS_GetPetTypeTxtRecord(int32_t nPetType);
//D2Game.0x6FD108D0
//1.14d: 0x0056BD90
void __fastcall SKILLS_UpdatePetLimitForSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId);
//D2Game.0x6FD10A50
//1.14d: 0x0056BFE0
int32_t __fastcall D2GAME_SKILLMANA_Consume_6FD10A50(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD10C90
//1.14d: 0x0056C110
int32_t __fastcall D2GAME_SKILLMANA_AuraConsume_6FD10C90(D2UnitStrc* pUnit, int32_t nManaCost);
//D2Game.0x6FD10CE0
//1.14d: 0x0056C160
int32_t __fastcall SKILLS_HasManaOrChargesForUsedSkill(D2UnitStrc* pUnit);
//D2Game.0x6FD10D90
//1.14d: 0x0056C210
D2UnitFindDataStrc* __fastcall SKILLS_AllocUnitSearch(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSize, int32_t nX, int32_t nY);
//D2Game.0x6FD10E20
//1.14d: 0x0056C2A0
void __fastcall SKILLS_FreeUnitSearch(D2GameStrc* pGame, D2UnitFindDataStrc* pUnitFindData);
//D2Game.0x6FD10E50
//1.14d: 0x0056E900
void __fastcall SKILLS_RemoveStateAndRefreshPassives(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList);
//D2Game.0x6FD10EC0
//1.14d: 0x0056E970
D2StatListStrc* __fastcall SKILLS_ApplyStateEffect(D2CurseStrc* pCurse);
//D2Game.0x6FD11260
//1.14d: 0x0056EBE0
D2StatListStrc* __fastcall SKILLS_ApplyStateFromSkillParams(D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel, int32_t nStatId, int32_t nState, StatListRemoveCallback pfRemoveStatCallback);
//D2Game.0x6FD11340
//1.14d: 0x0056C310
int32_t __fastcall SKILLS_ConsumeItemQuantity(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FD11420
//1.14d: 0x0056ECB0
D2UnitStrc* __fastcall SKILLS_CreateMissileAtOffset(D2GameStrc* pGame, int32_t nMissileId, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t bCheckItemQuantity);
//D2Game.0x6FD115E0
//1.14d: 0x0056EDE0
D2UnitStrc* __fastcall D2GAME_CreateMissile_6FD115E0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nMissileId, int32_t nX, int32_t nY);
//D2Game.0x6FD11710
//1.14d: 0x0056EE90
D2UnitStrc* __fastcall SKILLS_CreateMissileFromOrigin(D2GameStrc* pGame, int32_t nMissileId, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t nX, int32_t nY, int32_t nTargetX, int32_t nTargetY, int32_t bConsumeQuantity);
//D2Game.0x6FD118C0
//1.14d: 0x0056C3F0
int32_t __fastcall SKILLS_ConsumeWeaponOrAmmoQuantity(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FD119C0
//1.14d: 0x0056C4E0
int32_t __fastcall SKILLS_CanUseWeaponAmmo(D2UnitStrc* pUnit);
//D2Game.0x6FD11AE0
//1.14d: 0x0056C6A0
void __fastcall SKILLS_HandleTransmogrify(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem);
//D2Game.0x6FD11B50
//1.14d: 0x0056C700
int32_t __fastcall D2GAME_CheckSkillIfUnitsClassSkill_6FD11B50(D2UnitStrc* pUnit, int32_t nSkillId);
//D2Game.0x6FD11BA0
//1.14d: 0x0056F020
void __fastcall SKILLS_ApplySkillDelay(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD11C00
//1.14d: 0x0056EF90
void __fastcall D2GAME_SKILLS_SetDelay_6FD11C00(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nDelay);
//D2Game.0x6FD11C90
//1.14d: 0x0056C740
int32_t __fastcall SKILLS_RemoveStatesInGroup(D2UnitStrc* pUnit, int32_t nStateId, int32_t bIncludeState);
//D2Game.0x6FD11D90
//1.14d: 0x0056C840
void __fastcall SKILLS_SetElementalDurationFromSkill(D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nLength, int32_t nSkillId);
//D2Game.0x6FD11E40
//1.14d: 0x0056C8E0
void __fastcall SKILLS_AddElementalDamage(D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nElementalType, int32_t nDamage, int32_t nLength, int32_t* pResistStatId, int32_t* pElementalType);
//D2Game.0x6FD11FF0
//1.14d: 0x0056CA40
int32_t __fastcall SKILLS_SrvSt01_Attack_LeftHandSwing(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD12060
//1.14d: 0x0056CAB0
int32_t __fastcall SKILLS_SrvSt65_Throw_LeftHandThrow(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD120A0
//1.14d: 0x0056CAF0
int32_t __fastcall SKILLS_SrvSt02_Kick(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD12150
//1.14d: 0x0056CBA0
int32_t __fastcall SKILLS_SrvSt03_Unsummon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD12210
//1.14d: 0x0056F070
int32_t __fastcall SKILLS_SrvDo001_Attack_LeftHandSwing(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD123D0
//1.14d: 0x0056F1F0
int32_t __fastcall SKILLS_SrvDo002_Kick_PowerStrike_MonIceSpear_Impale_Bash_Stun_Concentrate_BearSmite_Vengeance_Berserk_FireClaws(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD12670
//1.14d: 0x0056CC20
int32_t __fastcall SKILLS_SrvDo004_Unsummon(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD126B0
//1.14d: 0x0056F460
int32_t __fastcall SKILLS_SrvDo003_Throw(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD12800
//1.14d: 0x0056F550
int32_t __fastcall SKILLS_SrvDo005_LeftHandThrow(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD12950
//1.14d: 0x0056F640
int32_t __fastcall SKILLS_StartSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t bSkipCost);
//D2Game.0x6FD12BA0
//1.14d: 0x0056F7F0
int32_t __fastcall SKILLS_ExecuteSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t bApplyCosts, int32_t bSkipSkillCheck, int32_t bItemEffect);
//D2Game.0x6FD12FD0
//1.14d: 0x0056FAF0
int32_t __fastcall SKILLS_StartUsedSkill(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FD13220
//1.14d: 0x0056CD50
int32_t __fastcall SKILL_ComputePeriodicRate(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD13260
//1.14d: 0x0056CDA0
int32_t __fastcall SKILLS_SchedulePeriodicSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t bUseRightSkill);
//D2Game.0x6FD13330
//1.14d: 0x0056CE70
int32_t __fastcall SKILLS_SchedulePeriodicStat(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventKey, int32_t nSkillId, int32_t nSkillLevel, int32_t bUseRightSkill);
//D2Game.0x6FD13410
//1.14d: 0x0056FC50
void __fastcall SKILLS_ExecuteUsedSkill(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FD13470
//1.14d: 0x0056FCB0
void __fastcall SKILLS_EventCallback_PeriodicSkillAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillIdArg, int32_t nUnused);
//D2Game.0x6FD13610
//1.14d: 0x0056FE40
void __fastcall SKILLS_EventCallback_ItemAura(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventCustomId, int32_t nSkillId);
//D2Game.0x6FD136E0
//1.14d: 0x00570080
int32_t __fastcall SKILLS_TryIncreaseSkillLevel(D2UnitStrc* pUnit, int32_t nSkillId);
//D2Game.0x6FD13800
//1.14d: 0x005701B0
void __fastcall D2GAME_AssignSkill_6FD13800(D2UnitStrc* pUnit, int32_t bLeftSkill, int32_t nSkillId, int32_t nOwnerGUID);
//D2Game.0x6FD13B20
//1.14d: 0x0056CF40
D2UnitStrc* __fastcall D2GAME_CreateLinkPortal_6FD13B20(D2GameStrc* pGame, D2UnitStrc* pPortal, int32_t nDestLevel, int32_t nSourceLevel);
//D2Game.0x6FD13DF0
//1.14d: 0x0056D130
int32_t __fastcall D2GAME_CreatePortalObject_6FD13DF0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nDestLevel, D2UnitStrc** ppSourceUnit, int32_t nObjectId, int32_t bPerm);
//D2Game.0x6FD14020
//1.14d: 0x0056D2C0
int32_t __fastcall D2GAME_GetXAndYFromTargetUnit_6FD14020(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* pX, int32_t* pY);
//D2Game.0x6FD140D0
//1.14d: 0x0056D370
void __fastcall SKILLS_EnlargeVector(int32_t* pX, int32_t* pY);
//D2Game.0x6FD14120
//1.14d: 0x0056D3B0
void __fastcall SKILLS_MakeShortPerpendicularVector(int32_t* pX, int32_t* pY);
//D2Game.0x6FD14170
//1.14d: 0x0056D400
void __fastcall SKILLS_CreateMissileRing(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pOrigin, int32_t nMissileId, int32_t nSkillId, int32_t nSkillLevel, int32_t nVelocity);
//D2Game.0x6FD14260
//1.14d: 0x0056D4E0
void __fastcall SKILLS_CreateMissilePattern(D2GameStrc* pGame, int32_t nMissiles, D2MissileStrc* missileParams);
//D2Game.0x6FD14370
//1.14d: 0x0056D790
void __fastcall SKILLS_EventCallback_ActiveState(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD14400
//1.14d: 0x0056D810
void __fastcall SKILLS_ExecuteServerDoFunction(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nDoFunc, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD14430
//1.14d: 0x0056D940
D2UnitStrc* __fastcall D2GAME_SummonPet_6FD14430(D2GameStrc* pGame, D2SummonArgStrc* pSummonArg);
//D2Game.0x6FD146D0
//1.14d: 0x0056D840
void __fastcall SKILLS_RemoveTrackedSummon(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FD14770
//1.14d: 0x0056DBC0
void __fastcall SKILLS_ApplyCurseAroundTarget(D2GameStrc* pGame, int32_t nFlags, D2UnitStrc* pUnit, int32_t nSkillLevel, int32_t nRange, CurseCallbackFunc pfCurse);
//D2Game.0x6FD149B0
//1.14d: 0x0056DCC0
void __fastcall SKILLS_ApplyAreaEffect(D2GameStrc* pGame, D2EffectStrc* pEffect);
//D2Game.0x6FD14BD0
//1.14d: 0x0056DDE0
void __fastcall SKILLS_ApplyAreaEffectAtCoordinates(D2GameStrc* pGame, int32_t nFlags, int32_t nX, int32_t nY, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, void* pfAreaEffect);
//D2Game.0x6FD14C30
//1.14d: 0x0056DE40
void __fastcall SKILLS_RefreshPassivesAndPetLimits(D2UnitStrc* pUnit);
//D2Game.0x6FD14C60
//1.14d: 0x0056DEB0
void __fastcall D2GAME_SetSkills_6FD14C60(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, int32_t bRemove);
//D2Game.0x6FD14CC0
//1.14d: 0x0056DFA0
void __fastcall D2GAME_RefreshPassiveSkills_6FD14CC0(D2UnitStrc* pUnit);
//D2Game.0x6FD14D20
//1.14d: 0x0056E050
void __fastcall SKILLS_FillMinionSkillDamage(D2UnitStrc* pUnit, int32_t nUnused, D2MonSkillInfoStrc* pMonSkillInfo);
//D2Game.0x6FD14DD0
//1.14d: 0x0056E0C0
int32_t __fastcall D2GAME_RollElementalDamage_6FD14DD0(D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD14EC0
//1.14d: 0x0056E170
void __fastcall D2GAME_RollPhysicalDamage_6FD14EC0(D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD14F70
//1.14d: 0x00570420
void __fastcall SKILLS_StunMonster(D2GameStrc* pGame, D2UnitStrc* pUnused, D2UnitStrc* pUnit, int32_t nDelay);
//D2Game.0x6FD15080
//1.14d: 0x0056E210
void __fastcall SKILLS_RewindUnitAnimation(D2UnitStrc* pUnit, int32_t nRewindPercentage);
//D2Game.0x6FD150A0
//1.14d: 0x0056E230
int32_t __fastcall SKILLS_StartBasicAttack(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget);
//D2Game.0x6FD15190
//1.14d: 0x0056E2F0
int32_t __fastcall SKILLS_CanSwitchAi(D2UnitStrc* pUnit, D2C_AiSpecialState nAiSpecialState);
//D2Game.0x6FD15210
//1.14d: 0x0056E390
D2UnitStrc* __fastcall SKILLS_FindCorpseNearTarget(D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FD15320
//1.14d: 0x0056E370
int32_t __fastcall SKILLS_UnitFindTest_Corpse(D2UnitStrc* pUnit, D2UnitFindArgStrc* pArg);
//D2Game.0x6FD15340
//1.14d: 0x0056E450
int32_t __fastcall SKILLS_IsMissileTargetPositionClear(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMissileId);
//D2Game.0x6FD15460
//1.14d: 0x0056E510
int32_t __fastcall SKILLS_GetMeleeSearchRange(D2UnitStrc* pUnit);
//D2Game.0x6FD15470
//1.14d: 0x0056E520
void __fastcall SKILLS_ApplyAttackRateModifier(D2UnitStrc* pUnit, int32_t nAttackRate);
//D2Game.0x6FD154D0
//1.14d: 0x0056E580
void __fastcall SKILLS_ClearMonsterMinionOwnership(D2UnitStrc* pUnit);
//D2Game.0x6FD15500
//1.14d: 0x0056E5B0
int32_t __fastcall SKILLS_GetBaseWalkVelocity(D2UnitStrc* pUnit);
//D2Game.0x6FD15580
//1.14d: 0x0056E620
int32_t __fastcall D2GAME_GetSummonIdFromSkill_6FD15580(D2UnitStrc* pUnit, int32_t bFromMonster, int32_t nSkillId, int32_t nSkillLevel, int32_t* pSpawnMode, int32_t* pX, int32_t* pY);
//D2Game.0x6FD155E0
//1.14d: 0x0056E680
void __fastcall SKILLS_SetDamageResultFlagsFromSkill(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, D2SkillsTxt* pSkillsTxtRecord, int32_t nSkillId, int32_t nSkillLevel, D2DamageStrc* pDamage, int32_t nRangeOffset);
//D2Game.0x6FD15650
//1.14d: 0x0056E6F0
void __fastcall SKILLS_SetDamageResultFlags(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pUnit, uint16_t wResultFlags, int32_t nToHitFactor, D2DamageStrc* pDamage, int32_t nRangeOffset);
//D2Game.0x6FD156A0
//1.14d: 0x0056E740
D2UnitEventStrc* __fastcall SKILLS_RegisterUnitEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_UnitEventTypes nUnitEventType, int32_t nParam1, int32_t nParam2, int32_t nEventFunc, int32_t nEventGroup, int32_t nEventKey);

//D2Game.0x6FC808D0
//1.14d: 0x005C3260
int32_t __fastcall SKILLS_SrvSt18_Attract(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel);

D2StatesTxt* __fastcall SKILLS_GetStatesTxtRecord(int32_t nStateId);
D2ItemStatCostTxt* __fastcall SKILLS_GetItemStatCostTxtRecord(int32_t nStatId);
D2MissilesTxt* __fastcall SKILLS_GetMissilesTxtRecord(int32_t nMissileId);
