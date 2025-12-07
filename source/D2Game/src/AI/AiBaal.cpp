#include "AI/AiBaal.h"

#include <algorithm>

#include <D2Collision.h>
#include <D2Dungeon.h>
#include <DataTbls/LevelsIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/MonsterTbls.h>
#include <DataTbls/SkillsIds.h>
#include <DataTbls/SkillsTbls.h>
#include <D2Monsters.h>
#include <D2States.h>
#include <D2StatList.h>
#include <D2Skills.h>

#include "AI/AiGeneral.h"
#include "AI/AiTactics.h"
#include "AI/AiThink.h"
#include "AI/AiUtil.h"
#include "GAME/Clients.h"
#include "ITEMS/Items.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterSpawn.h"
#include "PLAYER/Player.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "SKILLS/SkillAss.h"


//D2Game.0x6FCCD450
void __fastcall AIBAAL_CountLivingMinions(D2UnitStrc* pUnit, void* ppUnitArg, void* pCounterArg)
{
	D2UnitStrc** ppUnit = (D2UnitStrc**)ppUnitArg;
	int32_t* pCounter = (int32_t*)pCounterArg;

	if (pUnit != *ppUnit && pUnit && pUnit->dwAnimMode != MONMODE_DEATH && pUnit->dwAnimMode != MONMODE_DEAD)
	{
		++*pCounter;
	}
}

//D2Game.0x6FCCD470
void __fastcall D2GAME_AI_Unk135_140_6FCCD470(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nParam1 = 0;
	int32_t nParam2 = 0;

	D2AiCmdStrc* pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	if (pAiCmd)
	{
		nParam1 = pAiCmd->nCmdParam[1];
		nParam2 = pAiCmd->nCmdParam[2];
	}

	if (STATES_CheckState(pUnit, STATE_INFERNO))
	{
		STATES_ToggleState(pUnit, STATE_INFERNO, 0);
	}

	AITHINK_ExecuteAiFn(pGame, pUnit, pAiTickParam->pAiControl, pAiTickParam->pAiControl->nAiSpecialState);

	if (!AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0) && nParam1 && nParam2)
	{
		D2AiCmdStrc aiCmd = {};
		aiCmd.nCmdParam[0] = 10;
		aiCmd.nCmdParam[1] = nParam1;
		aiCmd.nCmdParam[2] = nParam2;
		AIGENERAL_CopyAiCommand(pGame, pUnit, &aiCmd);
	}

	AITACTICS_IdleInNeutralMode(pGame, pUnit, 1);
}

//D2Game.0x6FCCD520
void __fastcall AITHINK_Fn135_BaalCrab(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	int32_t nCount = 0;
	int32_t nMax = 0;

	D2UnitStrc* pTarget = AIBAAL_GetTarget(pGame, pUnit, &nMax, &nCount, nullptr, AIBAAL_CullPotentialTargets);
	D2AiCmdStrc* pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	if (!pAiCmd)
	{
		D2AiCmdStrc aiCmd = {};
		aiCmd.nCmdParam[0] = 10;
		aiCmd.nCmdParam[1] = CLIENTS_GetUnitX(pUnit);
		aiCmd.nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
		AIGENERAL_CopyAiCommand(pGame, pUnit, &aiCmd);
		pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	}

	AIBAAL_MainSkillHandler(pGame, pUnit, pAiTickParam->pAiControl, pTarget, AIBAAL_RollRandomAiParam(pGame, pUnit, pAiTickParam->pAiControl, pTarget, nMax, nCount, pAiCmd), pAiCmd);
	AITACTICS_Idle(pGame, pUnit, 25);
}

//D2Game.0x6FCCD630
D2UnitStrc* __fastcall AIBAAL_GetTarget(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* pMax, int32_t* pCount, void* pArgs, int32_t(__fastcall* pfCull)(D2UnitStrc*, D2UnitStrc*))
{
	D2_ASSERT(pfCull);

	const int32_t nX = CLIENTS_GetUnitX(pUnit);
	const int32_t nY = CLIENTS_GetUnitY(pUnit);

	D2TargetNodeStrc* pTargetNode = nullptr;
	D2UnitStrc* pTarget = nullptr;
	int32_t nCurrent = 0;
	int32_t nCount = 0;
	int32_t nMax = 0;
	for (int32_t i = 0; i < 8; ++i)
	{
		pTargetNode = pGame->pTargetNodes[i];
		if (pTargetNode)
		{
			D2_ASSERT(pTargetNode->pUnit && pTargetNode->pUnit->dwUnitType == UNIT_PLAYER);

			if (pfCull(pUnit, pTargetNode->pUnit))
			{
				if (pTargetNode->pUnit->dwAnimMode == PLRMODE_DEAD || pTargetNode->pUnit->dwAnimMode == PLRMODE_DEATH)
				{
					nCurrent = 0;
				}
				else
				{
					nCurrent = AIBAAL_GetTargetScore(pUnit, pTargetNode->pUnit, pArgs);
				}

				while (1)
				{
					if (nMax < nCurrent)
					{
						nMax = nCurrent;
						pTarget = pTargetNode->pUnit;
					}

					pTargetNode = pTargetNode->pNext;
					++nCount;

					if (!pTargetNode)
					{
						break;
					}

					nCurrent = AIBAAL_GetTargetScore(pUnit, pTargetNode->pUnit, pArgs);
				}
			}
		}
	}

	while (pTargetNode)
	{
		if (pUnit->nAct == pTargetNode->pUnit->nAct)
		{
			nCurrent = AIBAAL_GetTargetScore(pUnit, pTargetNode->pUnit, pArgs);
			if (nMax < nCurrent)
			{
				nMax = nCurrent;
				pTarget = pTargetNode->pUnit;
			}
			++nCount;
		}

		pTargetNode = pTargetNode->pNext;
	}

	D2UnitStrc* pTemp = nullptr;
	int32_t nTemp = 0;

	pTargetNode = pGame->pTargetNodes[9];
	if (pTargetNode)
	{
		do
		{
			if (pUnit->nAct == pTargetNode->pUnit->nAct)
			{
				nCurrent = AIBAAL_GetTargetScore(pUnit, pTargetNode->pUnit, pArgs);
				if (nTemp < nCurrent)
				{
					nTemp = nCurrent;
					pTemp = pTargetNode->pUnit;
				}
			}

			pTargetNode = pTargetNode->pNext;
		}
		while (pTargetNode);

		if (pTemp && AIUTIL_GetDistanceToCoordinates_NoUnitSize(pTemp, nX, nY) < 5 && !UNITS_IsInMeleeRange(pUnit, pTarget, 0))
		{
			PATH_SetTargetUnit(pUnit->pDynamicPath, pTarget);
			PATH_SetType(pUnit->pDynamicPath, PATHTYPE_TOWARD);
			D2Common_10142(pUnit->pDynamicPath, pUnit, 0);

			if (!PATH_GetNumberOfPathPoints(pUnit->pDynamicPath))
			{
				nMax = nTemp;
				pTarget = pTemp;
			}
		}
	}

	*pCount = nCount;
	*pMax = nMax;

	return pTarget;
}

//D2Game.0x6FCCD8A0
int32_t __fastcall AIBAAL_GetTargetScore(D2UnitStrc* pUnit, D2UnitStrc* pTarget, void* pArgs)
{
	const int32_t bInMeleeRange = UNITS_IsInMeleeRange(pUnit, pTarget, 0);

	int32_t nDistanceWeight = 0;
	if (pArgs && AIUTIL_GetDistanceToCoordinates_HalfUnitSize(pUnit, *((int32_t*)pArgs + 3), *((int32_t*)pArgs + 4)) > 75 && AIUTIL_GetDistanceToCoordinates_HalfUnitSize(pTarget, *((int32_t*)pArgs + 3), *((int32_t*)pArgs + 4)) < 75)
	{
		nDistanceWeight = 100;
	}

	const int32_t nDamageResist = STATLIST_UnitGetStatValue(pTarget, STAT_DAMAGERESIST, 0);
	const int32_t nFireResist = STATLIST_UnitGetStatValue(pTarget, STAT_FIRERESIST, 0);
	const int32_t nColdResist = STATLIST_UnitGetStatValue(pTarget, STAT_COLDRESIST, 0);
	const int32_t nLightningResist = STATLIST_UnitGetStatValue(pTarget, STAT_LIGHTRESIST, 0);
	const int32_t nMagicResist = STATLIST_UnitGetStatValue(pTarget, STAT_MAGICRESIST, 0);
	const int32_t nResistWeight = (nFireResist + nColdResist + nLightningResist + 4 * (nDamageResist + 2 * nMagicResist)) / 15;

	const int64_t nMaxDamage = STATLIST_UnitGetStatValue(pTarget, STAT_MAXDAMAGE, 0);
	const int64_t nFireMaxDamage = STATLIST_UnitGetStatValue(pTarget, STAT_FIREMAXDAM, 0);
	const int64_t nLightningMaxDamage = STATLIST_UnitGetStatValue(pTarget, STAT_LIGHTMAXDAM, 0);
	const int64_t nMagicMaxDamage = STATLIST_UnitGetStatValue(pTarget, STAT_MAGICMAXDAM, 0);
	const int64_t nColdMaxDamage = STATLIST_UnitGetStatValue(pTarget, STAT_COLDMAXDAM, 0);
	const int64_t nPoisonMaxDamage = STATLIST_UnitGetStatValue(pTarget, STAT_POISONMAXDAM, 0) >> 8;
	const int32_t nDamageWeight = static_cast<int32_t>(nMaxDamage + nFireMaxDamage + nLightningMaxDamage + nMagicMaxDamage + nColdMaxDamage + nPoisonMaxDamage) / 2;

	const int32_t nColdStateWeight = STATES_CheckState(pTarget, STATE_COLD) ? 100 : 0;
	const int32_t nLifePercentageWeight = (UNITS_GetCurrentLifePercentage(pTarget) >= 20) ? 100 : 0;
	
	int32_t nLeftSkillLevel = 0;
	int32_t nLeftSkillAttackRank = 0;
	D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pTarget);
	if (pLeftSkill)
	{
		nLeftSkillLevel = SKILLS_GetSkillLevel(pTarget, pLeftSkill, 1);
		nLeftSkillAttackRank = SKILLS_GetSkillsTxtRecord(SKILLS_GetSkillIdFromSkill(pLeftSkill, __FILE__, __LINE__))->nAttackRank;
	}

	int32_t nRightSkillLevel = 0;
	int32_t nRightSkillAttackRank = 0;
	D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pTarget);
	if (pRightSkill)
	{
		nRightSkillLevel = SKILLS_GetSkillLevel(pTarget, pRightSkill, 1);
		nRightSkillAttackRank = SKILLS_GetSkillsTxtRecord(SKILLS_GetSkillIdFromSkill(pRightSkill, __FILE__, __LINE__))->nAttackRank;
	}

	const int32_t nAttackRankWeight = (nLeftSkillLevel * nLeftSkillAttackRank + nRightSkillLevel * nRightSkillAttackRank) / 4;

	int32_t nCollisionRangeWeight = 0;
	if (bInMeleeRange)
	{
		nCollisionRangeWeight = 100;
	}
	else if (!UNITS_TestCollisionWithUnit(pUnit, pTarget, COLLIDE_MISSILE_BARRIER))
	{
		nCollisionRangeWeight = 75;
	}

	if (pTarget && pTarget->dwUnitType == UNIT_MONSTER)
	{
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pTarget->dwClassId);
		if (!pMonStatsTxtRecord || pMonStatsTxtRecord->nThreat <= 1u)
		{
			return 0;
		}
	}

	int32_t nTotalScore = (nResistWeight + 5 * (nCollisionRangeWeight + nDistanceWeight) + 2 * (nDamageWeight + nAttackRankWeight + 2 * nColdStateWeight) + 3 * nLifePercentageWeight) / 22;
	if (!nTotalScore)
	{
		nTotalScore = 1;
	}

	return nTotalScore;
}

//D2Game.0x6FCCDBB0
int32_t __fastcall AIBAAL_CullPotentialTargets(D2UnitStrc* pBaal, D2UnitStrc* pTarget)
{
	D2_ASSERT(pBaal);
	D2_ASSERT(pTarget);

	if (pBaal->nAct == pTarget->nAct)
	{
		return AIUTIL_GetDistanceToCoordinates_NoUnitSize(pTarget, CLIENTS_GetUnitX(pBaal), CLIENTS_GetUnitY(pBaal)) < 55;
	}

	return 0;
}

//D2Game.0x6FCCDC80
int32_t __fastcall AIBAAL_RollRandomAiParam(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, D2UnitStrc* pTarget, int32_t nMax, int32_t nCount, D2AiCmdStrc* pAiCmd)
{
	if (pAiControl->dwAiParam[0])
	{
		return pAiControl->dwAiParam[0];
	}

	if (!pTarget)
	{
		if (COLLISION_CheckAnyCollisionWithPattern(UNITS_GetRoom(pUnit), CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 2, COLLIDE_MISSILE))
		{
			return 4;
		}
		else
		{
			return 8 * ((ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < 8) + 1;
		}
	}

	if (!pGame)
	{
		pGame = pUnit->pGame;
	}

	int32_t bInCloseRange = 0;
	if (pTarget->dwUnitType == UNIT_PLAYER)
	{
		D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, PLAYER_GetUniqueIdFromPlayerData(pTarget));
		if (pObject && DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pObject)) == LEVEL_THEWORLDSTONECHAMBER)
		{
			if (!pAiCmd || UNITS_GetDistanceToCoordinates(pObject, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]) < 75)
			{
				bInCloseRange = 1;
			}
		}
	}

	int32_t bInMediumRange = 0;
	int32_t bInFarRange = 0;
	if (pAiCmd)
	{
		const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_HalfUnitSize(pTarget, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]);
		if (nDistance > 75)
		{
			bInMediumRange = 1;
		}

		if (nDistance > 100)
		{
			bInFarRange = 1;
		}
	}

	const int32_t bInMeleeRange = UNITS_IsInMeleeRange(pUnit, pTarget, 0);
	const int32_t bNotCollidingWithWall = UNITS_TestCollisionWithUnit(pUnit, pTarget, COLLIDE_MISSILE_BARRIER) == 0;
	if (bInMeleeRange)
	{
		int32_t aiParams[16] = { 0, 50, 0, 0, 0, 0, 0, 0, 20, 30, 150, 10, 10, 70, 40, 0 };

		if (!pGame->nDifficulty)
		{
			aiParams[1] = 75;
			aiParams[13] = 45;
		}

		if (UNITS_GetCurrentLifePercentage(pTarget) < 33)
		{
			aiParams[10] += 50;
		}

		aiParams[1] += 100 - UNITS_GetCurrentLifePercentage(pUnit);

		if (STATES_CheckState(pTarget, STATE_COLD))
		{
			aiParams[8] = 0;
			aiParams[13] = 0;
			aiParams[11] = 0;
		}

		if (!bNotCollidingWithWall)
		{
			aiParams[11] = 0;
			aiParams[13] = 0;
			aiParams[12] = 0;
		}

		return AI_GetRandomArrayIndex(aiParams, std::size(aiParams), pUnit, 1);
	}

	if (bNotCollidingWithWall)
	{
		return AIBAAL_RollRandomAiParamForNonCollidingUnit(pGame, pAiControl, pUnit, pTarget, nCount, bInMediumRange, bInFarRange, bInCloseRange, nMax);
	}

	int32_t aiParams[16] = { 0, 100, 20, 20, 20, 0, 20, 0, 80, 70, 0, 0, 0, 0, 0, 0 };
	if (!pGame->nDifficulty)
	{
		aiParams[1] = 125;
	}

	if (nCount < 2)
	{
		aiParams[2] = 45;
		aiParams[10] = 25;
	}

	aiParams[1] += 100 - UNITS_GetCurrentLifePercentage(pUnit);

	if (AI_CheckSpecialSkillsOnPrimeEvil(pTarget))
	{
		if (!bInMediumRange)
		{
			aiParams[14] += 50;
			aiParams[13] += 50;

			if (bInFarRange != 0)
			{
				aiParams[5] += 60;
			}

			return AI_GetRandomArrayIndex(aiParams, std::size(aiParams), pUnit, 1);
		}
	}
	else
	{
		if (!bInMediumRange)
		{
			if (bInFarRange)
			{
				aiParams[5] += 60;
			}

			return AI_GetRandomArrayIndex(aiParams, std::size(aiParams), pUnit, 1);
		}
	}

	aiParams[2] = 0;
	aiParams[6] = 0;
	aiParams[3] += 15;
	aiParams[11] += 25;
	aiParams[13] += 25;

	if (bInFarRange)
	{
		aiParams[5] += 60;
	}

	return AI_GetRandomArrayIndex(aiParams, std::size(aiParams), pUnit, 1);
}

//D2Game.0x6FCCE040
int32_t __fastcall AI_CheckSpecialSkillsOnPrimeEvil(D2UnitStrc* pUnit)
{
	D2SkillStrc* pSkill = UNITS_GetRightSkill(pUnit);
	if (pSkill)
	{
		const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
		if (nSkillId == SKILL_BLIZZARD || nSkillId == SKILL_METEOR)
		{
			return 1;
		}

		if (nSkillId == SKILL_FIREWALL)
		{
			if (SKILLS_GetSkillLevel(pUnit, pSkill, 1) > 3)
			{
				return 1;
			}
		}
		else if (nSkillId == SKILL_IMMOLATIONARROW)
		{
			if (SKILLS_GetSkillLevel(pUnit, pSkill, 1) > 7)
			{
				return 1;
			}
		}
	}

	pSkill = UNITS_GetLeftSkill(pUnit);
	if (pSkill)
	{
		const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
		if (nSkillId == SKILL_BLIZZARD || nSkillId == SKILL_METEOR)
		{
			return 1;
		}

		if (nSkillId == SKILL_FIREWALL)
		{
			if (SKILLS_GetSkillLevel(pUnit, pSkill, 1) > 3)
			{
				return 1;
			}
		}
		else if (nSkillId == SKILL_IMMOLATIONARROW)
		{
			if (SKILLS_GetSkillLevel(pUnit, pSkill, 1) > 7)
			{
				return 1;
			}
		}
	}

	return 0;
}

//D2Game.0x6FCCE100
int32_t __fastcall AI_GetRandomArrayIndex(int32_t* pArray, int32_t nArraySize, D2UnitStrc* pUnit, int32_t nDefaultValue)
{
	int32_t nSum = 0;
	for (int32_t i = 0; i < nArraySize; ++i)
	{
		nSum += pArray[i];
	}

	const int32_t nRand = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nSum);

	int32_t nThreshold = 0;
	for (int32_t i = 0; i < nArraySize; ++i)
	{
		nThreshold += pArray[i];
		if (nRand < nThreshold)
		{
			return i;
		}
	}

	return nDefaultValue;
}

//D2Game.0x6FCCE1A0
int32_t __fastcall AIBAAL_RollRandomAiParamForNonCollidingUnit(D2GameStrc* pGame, D2AiControlStrc* pAiControl, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nCount, int32_t bInMediumRange, int32_t bInFarRange, int32_t bInCloseRange, int32_t nMax)
{
	int32_t aiParams[16] = { 0, 0, 5, 5, 5, 0, 5, 0, 40, 40, 0, 70, 80, 60, 20, 0};
	int32_t nBonus = 100;
	if (!pGame->nDifficulty)
	{
		nBonus = 125;
		aiParams[13] = 40;
	}

	aiParams[15] = 10 * (2 - pAiControl->dwAiParam[1]);
	if (aiParams[15] < 0)
	{
		aiParams[15] = 0;
	}

	const int32_t nDistance = AIUTIL_GetDistanceToCoordinates_FullUnitSize(pUnit, pTarget);
	if (nDistance > 35)
	{
		aiParams[6] = 15;
		aiParams[13] = 0;
	}

	aiParams[1] = 100 - UNITS_GetCurrentLifePercentage(pUnit) + nBonus;
	if (nDistance > 25)
	{
		aiParams[14] = 30;
		aiParams[11] = 0;
	}

	if (nCount < 2)
	{
		aiParams[11] -= 10;
	}

	if (nCount > 3)
	{
		aiParams[13] += 25;
	}

	if (nMax > 60)
	{
		aiParams[8] = 70;
	}

	D2PlayerCountBonusStrc playerCountBonus = {};
	MONSTER_GetPlayerCountBonus(pGame, &playerCountBonus, UNITS_GetRoom(pUnit), pUnit);

	if (nCount < 2 && playerCountBonus.nDifficulty < 2)
	{
		aiParams[8] = 0;
	}

	if (AI_CheckSpecialSkillsOnPrimeEvil(pTarget))
	{
		if (!bInMediumRange)
		{
			aiParams[6] += 30;
			aiParams[11] += 10;
			aiParams[14] += 15;
		}
		else
		{
			aiParams[3] = 25;
			aiParams[2] = 0;
			aiParams[6] += 25;
			aiParams[14] += 35;
		}
	}
	else
	{
		if (bInMediumRange)
		{
			aiParams[3] = 25;
			aiParams[2] = 0;
			aiParams[6] += 25;
			aiParams[14] += 35;
		}
	}

	if (bInFarRange)
	{
		aiParams[5] = 60;
	}

	if (bInCloseRange)
	{
		aiParams[7] = 0;
	}

	return AI_GetRandomArrayIndex(aiParams, std::size(aiParams), pUnit, 1);
}

//D2Game.0x6FCCE450
void __fastcall AIBAAL_MainSkillHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiControlStrc* pAiControl, D2UnitStrc* pTarget, int32_t nParam, D2AiCmdStrc* pAiCmd)
{
	if (!pUnit)
	{
		return;
	}

	D2MonStatsTxt* pMonstatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
	if (!pMonstatsTxtRecord)
	{
		return;
	}

	switch (nParam)
	{
	case 1:
	{
		if (pGame->nDifficulty == 0)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 35);
		}
		else if (pGame->nDifficulty == 1)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
		}
		else if (pGame->nDifficulty == 2)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
		}
		return;
	}
	case 2:
	case 6:
	{
		AITACTICS_WalkInRadiusToTarget(pGame, pUnit, pTarget, 12, 0);
		return;
	}
	case 3:
	{
		sub_6FCD0E80(pGame, pUnit, pTarget, 6u, 1);
		return;
	}
	case 4:
	{
		AITACTICS_WalkCloseToUnit(pGame, pUnit, 16);
		return;
	}
	case 5:
	{
		if (!pAiCmd)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
			return;
		}

		AITACTICS_WalkToTargetCoordinates(pGame, pUnit, pAiCmd->nCmdParam[1], pAiCmd->nCmdParam[2]);
		return;
	}
	case 8:
	{
		if (!pTarget)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
			return;
		}

		if (pMonstatsTxtRecord->nSkill[5] > 0 && ((STATLIST_GetMaxLifeFromUnit(pTarget) > STATLIST_GetMaxManaFromUnit(pTarget)) || pTarget->dwUnitType != UNIT_PLAYER))
		{
			AITACTICS_UseSkill(pGame, pUnit, pMonstatsTxtRecord->nSkillMode[5], pMonstatsTxtRecord->nSkill[5], pTarget, 0, 0);
		}
		else if (pMonstatsTxtRecord->nSkill[6] > 0 && pTarget->dwUnitType == UNIT_PLAYER)
		{
			AITACTICS_UseSkill(pGame, pUnit, pMonstatsTxtRecord->nSkillMode[6], pMonstatsTxtRecord->nSkill[6], pTarget, 0, 0);
		}
		return;
	}
	case 9:
	{
		AITACTICS_UseSkill(pGame, pUnit, MONMODE_SKILL2, SKILL_BAALTENTACLE, pTarget, 0, 0);
		return;
	}
	case 10:
	{
		AITACTICS_ChangeModeAndTargetUnit(pGame, pUnit, MONMODE_ATTACK2, pTarget);
		return;
	}
	case 11:
	{
		SKILLS_AssignSkill(pUnit, SKILL_BAALNOVA, 1, 0, __FILE__, __LINE__);
		AITACTICS_UseSkill(pGame, pUnit, MONMODE_SKILL3, SKILL_BAALNOVA, pTarget, 0, 0);
		return;
	}
	case 12:
	{
		AITACTICS_UseSkill(pGame, pUnit, MONMODE_SKILL1, SKILL_BAALINFERNO, pTarget, 0, 0);
		return;
	}
	case 13:
	{
		AITACTICS_UseSkill(pGame, pUnit, MONMODE_ATTACK1, SKILL_BAALCOLDMISSILES, pTarget, 0, 0);
		return;
	}
	case 14:
	{
		if (!pTarget)
		{
			AIBAAL_MainSkillHandler(pGame, pUnit, pAiControl, 0, 0, pAiCmd);
			return;
		}

		const int32_t nX = CLIENTS_GetUnitX(pUnit);
		const int32_t nY = CLIENTS_GetUnitY(pUnit);
		const int32_t nTargetX = CLIENTS_GetUnitX(pTarget);
		const int32_t nTargetY = CLIENTS_GetUnitY(pTarget);
		const int32_t nDistance = std::max(UNITS_GetDistanceToOtherUnit(pUnit, pTarget), 1);
		int32_t nFinalX = nX + (25 * (nX - nTargetX)) / nDistance;
		int32_t nFinalY = nY + (25 * (nY - nTargetY)) / nDistance;

		if (sub_6FC66260(pGame, UNITS_GetRoom(pUnit), 0, pUnit->dwClassId, &nFinalX, &nFinalY, 0))
		{
			AITACTICS_UseSkill(pGame, pUnit, pMonstatsTxtRecord->nSkillMode[4], pMonstatsTxtRecord->nSkill[4], 0, nFinalX, nFinalY);
		}
		else
		{
			AIBAAL_MainSkillHandler(pGame, pUnit, pAiControl, pTarget, 0, pAiCmd);
		}
		return;
	}
	case 15:
	{
		D2UnitStrc* pTemp = pUnit;
		int32_t nLivingMinions = 0;
		AIGENERAL_ExecuteCallbackOnMinions(pUnit, &pTemp, &nLivingMinions, AIBAAL_CountLivingMinions);

		if (nLivingMinions)
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
			return;
		}

		D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pUnit);
		if (pOwner && !SUNIT_IsDead(pOwner))
		{
			AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
			return;
		}

		int32_t nMonsterId = MONSTER_BAALCLONE;
		int32_t nUnused1 = 0;
		int32_t nUnused2 = 0;
		int32_t nMode = MONMODE_NEUTRAL;
		MONSTERS_GetMinionSpawnInfo(pUnit, &nMonsterId, &nUnused1, &nUnused2, &nMode, pGame->nDifficulty, 0);

		int32_t nX = ITEMS_RollRandomNumber(&pUnit->pSeed) % 24 - 12;
		int32_t nY = ITEMS_RollRandomNumber(&pUnit->pSeed) % 24 - 12;

		D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
		if (pTargetUnit)
		{
			nX += CLIENTS_GetUnitX(pTargetUnit);
			nY += CLIENTS_GetUnitY(pTargetUnit);
		}
		else
		{
			nX += CLIENTS_GetUnitX(pUnit);
			nY += CLIENTS_GetUnitY(pUnit);
		}

		D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
		if (pRoom)
		{
			D2UnitStrc* pBaalClone = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, nMode, -1, 0);
			if (pBaalClone)
			{
				pBaalClone->dwFlags |= UNITFLAG_NOXP | UNITFLAG_NOTC;
				AITACTICS_Idle(pGame, pBaalClone, 15);
				AIGENERAL_SetOwnerData(pGame, pUnit, pUnit->dwUnitId, pUnit->dwUnitType, 1, 0);
				AIGENERAL_AllocMinionList(pGame, pUnit, pBaalClone);
				AIGENERAL_SetOwnerData(pGame, pBaalClone, pUnit->dwUnitId, pUnit->dwUnitType, 1, 0);
				STATLIST_SetUnitStat(pBaalClone, STAT_MAXHP, STATLIST_GetMaxLifeFromUnit(pUnit) / 3, 0);
				STATLIST_SetUnitStat(pBaalClone, STAT_HITPOINTS, STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) / 3, 0);
				STATLIST_SetUnitStat(pBaalClone, STAT_HPREGEN, 0, 0);
				UNITS_StoreOwner(pBaalClone, pUnit);
				UNITS_StoreOwner(pUnit, pBaalClone);
				SUNIT_AttachSound(pUnit, 16, 0);
				++pAiControl->dwAiParam[1];
			}
		}

		AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
		return;
	}
	default:
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 5);
		return;
	}
	}
}

//D2Game.0x6FCCEB70
void __fastcall AITHINK_Fn140_BaalCrabClone(D2GameStrc* pGame, D2UnitStrc* pUnit, D2AiTickParamStrc* pAiTickParam)
{
	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (pOwner && SUNIT_IsDead(pOwner))
	{
		SUNITDMG_KillMonster(pGame, pUnit, nullptr, 1);
		return;
	}

	pOwner = SUNIT_GetOwner(pGame, pUnit);
	if (!pOwner || SUNIT_IsDead(pOwner))
	{
		SUNITDMG_KillMonster(pGame, pUnit, nullptr, 1);
		return;
	}

	int32_t nCount = 0;
	int32_t nMax = 0;
	D2UnitStrc* pTarget = AIBAAL_GetTarget(pGame, pUnit, &nMax, &nCount, nullptr, AIBAAL_CullPotentialTargets);

	D2AiCmdStrc* pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	if (!pAiCmd)
	{
		D2AiCmdStrc aiCmd = {};
		aiCmd.nCmdParam[0] = 10;
		aiCmd.nCmdParam[1] = CLIENTS_GetUnitX(pUnit);
		aiCmd.nCmdParam[2] = CLIENTS_GetUnitY(pUnit);
		AIGENERAL_CopyAiCommand(pGame, pUnit, &aiCmd);
		pAiCmd = AIGENERAL_GetAiCommandFromParam(pUnit, 10, 0);
	}

	int32_t nParam = AIBAAL_RollRandomAiParam(pGame, pUnit, pAiTickParam->pAiControl, pTarget, nMax, nCount, pAiCmd);
	if (nParam == 7 || nParam == 9 || nParam == 14 || nParam == 15)
	{
		nParam = 2;
	}

	if (pTarget)
	{
		AIBAAL_MainSkillHandler(pGame, pUnit, pAiTickParam->pAiControl, pTarget, nParam, pAiCmd);
		AITACTICS_Idle(pGame, pUnit, 25);
	}
	else
	{
		AITACTICS_IdleInNeutralMode(pGame, pUnit, 15);
	}
}
