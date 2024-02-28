#include "UNIT/SUnitDmg.h"

#include <algorithm>

#include <D2BitManip.h>
#include <D2Math.h>

#include <D2Combat.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Items.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <UselessOrdinals.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <Units/UnitFinds.h>
#include <Units/UnitRoom.h>


#include "AI/AiGeneral.h"
#include "GAME/Arena.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "ITEMS/Items.h"
#include "MISSILES/MissMode.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterUnique.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlayerStats.h"
#include "PLAYER/PlrModes.h"
#include "QUESTS/Quests.h"
#include "SKILLS/Skills.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillPal.h"
#include "UNIT/Party.h"
#include "UNIT/SUnitEvent.h"
#include "UNIT/SUnit.h"


#pragma pack(push, 1)
struct D2DamageHitClassMappingStrc
{
	int32_t nDamageStrcOffset;
	uint32_t nHitClass;
};

struct D2DurabilityLossStrc
{
	int32_t nBodyLoc;
	int32_t nWeight;
};

struct D2PartyExpStrc
{
	D2UnitStrc* pDefenderUnit;
	D2UnitStrc* pMembers[8];
	int32_t nMemberLevels[8];
	int32_t nMembers;
	int32_t nLevelSum;
};
#pragma pack(pop)


//D2Game.0x6FCBE2F0
int32_t __fastcall SUNITDMG_SetHitClass(D2DamageStrc* pDamage, uint32_t nHitClass)
{
    if (!(pDamage->dwHitClass & 0xF0))
    {
        pDamage->dwHitClass |= nHitClass;
        return 1;
    }

    return 0;
}

//D2Game.0x6FCBE310
int32_t __fastcall SUNITDMG_GetColdEffect(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
	{
		return -50;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
	if (!pMonStatsTxtRecord)
	{
		return -50;
	}

	return pMonStatsTxtRecord->nColdEffect[pGame->nDifficulty];
}

//D2Game.0x6FCBE360
void __fastcall SUNITDMG_RemoveFreezeState(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
	D2_MAYBE_UNUSED(pStatList);
	if (SUNIT_IsDead(pUnit) && STATES_CheckStateMaskStayDeathOnUnitByStateId(pUnit, nState))
	{
		return;
	}

	STATES_ToggleState(pUnit, nState, 0);

	D2GameStrc* pGame = pUnit->pGame;

	SUNIT_SetCombatMode(pGame, pUnit, 1);

	if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || !pUnit->pMonsterData)
	{
		return;
	}

	D2MonStatsTxt* pMonStatsTxtRecord = pUnit->pMonsterData->pMonstatsTxt;
	if (!pMonStatsTxtRecord)
	{
		return;
	}

	int32_t nAIDelay = 0;
	if (pGame->nGameType || pGame->dwGameType)
	{
		nAIDelay = pMonStatsTxtRecord->nAIdel[pGame->nDifficulty];
	}
	else
	{
		nAIDelay = pMonStatsTxtRecord->nAIdel[0];
	}

	if (nAIDelay <= 0)
	{
		nAIDelay = 15;
	}

	D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, 0);
	EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_AITHINK, nAIDelay + pGame->dwGameFrame, 0, 0);
}

//D2Game.0x6FCBE420
int32_t __fastcall SUNITDMG_ApplyDamageBonuses(D2UnitStrc* pUnit, int32_t bGetStats, D2UnitStrc* pItem, int32_t nMinDmg, int32_t nMaxDmg, int32_t nDamagePercent, int32_t nDamage, uint8_t nSrcDam)
{
    int32_t nMinDamage = 0;
    int32_t nMaxDamage = 0;

    if (bGetStats)
    {
        if (!pItem)
        {
            pItem = sub_6FC7C7B0(pUnit);
        }

        if (pItem)
        {
            if (INVENTORY_GetWieldType(pUnit, pUnit->pInventory) == 2)
            {
                nMinDamage = STATLIST_UnitGetStatValue(pUnit, STAT_SECONDARY_MINDAMAGE, 0) << 8;
                nMaxDamage = STATLIST_UnitGetStatValue(pUnit, STAT_SECONDARY_MAXDAMAGE, 0) << 8;
            }
            else
            {
                nMinDamage = STATLIST_UnitGetStatValue(pUnit, STAT_MINDAMAGE, 0) << 8;
                nMaxDamage = STATLIST_UnitGetStatValue(pUnit, STAT_MAXDAMAGE, 0) << 8;
            }
        }
        else
        {
            nMinDamage = std::max(STATLIST_UnitGetStatValue(pUnit, STAT_MINDAMAGE, 0), 1);
            nMaxDamage = std::max(STATLIST_UnitGetStatValue(pUnit, STAT_MAXDAMAGE, 0), 2);

            nMinDamage <<= 8;
            nMaxDamage <<= 8;
        }
    }
    else
    {
        nMinDamage = nMinDmg;
        nMaxDamage = nMaxDmg;
    }

    const int32_t nNormalDamage = STATLIST_UnitGetItemStatOrSkillStatValue(pUnit, STAT_ITEM_NORMALDAMAGE, 0) << 8;
    int32_t nTotalMinDamage = nNormalDamage + nMinDamage;
    int32_t nTotalMaxDamage = nNormalDamage + nMaxDamage;

    if (nTotalMinDamage < 1)
    {
        nTotalMinDamage = 256;
    }

    if (nTotalMaxDamage <= nTotalMinDamage)
    {
        nTotalMaxDamage = nTotalMinDamage + 256;
    }

    int32_t nDamagePct = nDamagePercent + STATLIST_UnitGetStatValue(pUnit, STAT_DAMAGEPERCENT, 0);

    if (pItem)
    {
        const int32_t nStrengthBonus = ITEMS_GetStrengthBonus(pItem);
        if (nStrengthBonus)
        {
            nDamagePct += nStrengthBonus * STATLIST_UnitGetStatValue(pUnit, STAT_STRENGTH, 0) / 100;
        }

        const int32_t nDexterityBonus = ITEMS_GetDexBonus(pItem);
        if (nDexterityBonus)
        {
            nDamagePct += nDexterityBonus * STATLIST_UnitGetStatValue(pUnit, STAT_DEXTERITY, 0) / 100;
        }

        nDamagePct += SKILLS_GetWeaponMasteryBonus(pUnit, pItem, 0, 1);
    }
    else if (bGetStats)
    {
        nDamagePct += STATLIST_UnitGetStatValue(pUnit, STAT_STRENGTH, 0);
    }

	nDamagePct = std::max(nDamagePct, -90);

    if (nTotalMaxDamage > 0)
    {
        nTotalMinDamage += MONSTERUNIQUE_CalculatePercentage(nTotalMinDamage, nDamagePct + STATLIST_UnitGetStatValue(pUnit, STAT_ITEM_MINDAMAGE_PERCENT, 0), 100);
        nTotalMaxDamage += MONSTERUNIQUE_CalculatePercentage(nTotalMaxDamage, nDamagePct + STATLIST_UnitGetStatValue(pUnit, STAT_ITEM_MAXDAMAGE_PERCENT, 0), 100);

        nDamage += nTotalMinDamage;
        if (nTotalMaxDamage > nTotalMinDamage)
        {
            nDamage += ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nTotalMaxDamage - nTotalMinDamage);
        }
    }

	nDamage = std::max(nDamage, 0);

    if (nSrcDam == 128)
    {
        return nDamage;
    }

    return MONSTERUNIQUE_CalculatePercentage(nDamage, nSrcDam, 128);
}

//D2Game.0x6FCBE7E0
void __fastcall SUNITDMG_FillDamageValues(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage, int32_t a5, uint8_t nSrcDam)
{
	if (!pAttacker || !pDefender)
	{
		return;
	}

	if (!nSrcDam)
	{
		nSrcDam = 128;
	}

	sub_6FC7C900(pAttacker, a5);

	pDamage->dwHitFlags |= DAMAGEHITFLAG_32;

	if ((pAttacker->dwUnitType == UNIT_PLAYER || STATLIST_GetUnitAlignment(pAttacker) == UNIT_ALIGNMENT_GOOD) && pDefender->dwUnitType == UNIT_MONSTER)
	{
		const int32_t nDamageTargetAc = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_DAMAGETARGETAC, 0);
		if (nDamageTargetAc)
		{
			const int32_t nArmorClass = std::max(nDamageTargetAc + (int32_t)STATLIST_UnitGetStatValue(pDefender, STAT_ARMORCLASS, 0), 0);
			STATLIST_SetUnitStat(pDefender, STAT_ARMORCLASS, nArmorClass, 0);
		}

		if (MONSTERS_IsDemon(pDefender))
		{
			const int32_t nDemonDamagePct = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_DEMONDAMAGE_PERCENT, 0);
			if (nDemonDamagePct > 0)
			{
				pDamage->dwEnDmgPct += nDemonDamagePct;
			}
		}

		if (MONSTERS_IsUndead(pDefender))
		{
			int32_t nUndeadDamagePct = 0;
			D2UnitStrc* pWeapon = sub_6FC7C7B0(pAttacker);
			if (pWeapon && ITEMS_CheckItemTypeId(pWeapon, ITEMTYPE_BLUNT))
			{
				nUndeadDamagePct = 50;
			}

			nUndeadDamagePct += STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_UNDEADDAMAGE_PERCENT, 0);
			if (nUndeadDamagePct > 0)
			{
				pDamage->dwEnDmgPct += nUndeadDamagePct;
			}
		}

		if (pDefender->dwUnitType == UNIT_MONSTER)
		{
			D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pDefender->dwClassId);
			if (pMonStatsTxtRecord && pMonStatsTxtRecord->wMonType > 0)
			{
				D2StatStrc stats[128] = {};
				const int32_t nStats = STATLIST_CopyStats(pAttacker, STAT_DAMAGE_VS_MONTYPE, stats, std::size(stats));
				for (int32_t i = 0; i < nStats; ++i)
				{
					if (SUNITDMG_CheckMonType(stats[i].nLayer, pMonStatsTxtRecord->wMonType))
					{
						pDamage->dwEnDmgPct += stats[i].nValue;
					}
				}
			}
		}
	}

	if (!(pDamage->dwHitFlags & DAMAGEHITFLAG_1))
	{
		pDamage->dwPhysDamage = SUNITDMG_ApplyDamageBonuses(pAttacker, 1, nullptr, 0, 0, pDamage->dwEnDmgPct, pDamage->dwPhysDamage, nSrcDam);

		int32_t nWeaponMastery = 0;

		if (!a5 
			&& (nWeaponMastery = SKILLS_GetWeaponMasteryBonus(pAttacker, sub_6FC7C7B0(pAttacker), 0, 2), nWeaponMastery > 0) // NOLINT(bugprone-assignment-in-if-condition)
			&& (int32_t)(ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100) < nWeaponMastery
			)
		{
			pDamage->wResultFlags |= DAMAGERESULTFLAG_CRITICALSTRIKE;
			pDamage->dwPhysDamage *= 2;
		}
		else
		{
			const int32_t nCriticalStrike = STATLIST_UnitGetStatValue(pAttacker, STAT_PASSIVE_CRITICAL_STRIKE, 0);
			if (nCriticalStrike > 0 && (ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100) < nCriticalStrike)
			{
				pDamage->wResultFlags |= DAMAGERESULTFLAG_CRITICALSTRIKE;
				pDamage->dwPhysDamage *= 2;
			}
			else
			{
				const int32_t nDeadlyStrike = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_DEADLYSTRIKE, 0);
				if (nDeadlyStrike > 0 && (ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100) < nDeadlyStrike)
				{
					pDamage->wResultFlags |= DAMAGERESULTFLAG_CRITICALSTRIKE;
					pDamage->dwPhysDamage *= 2;
				}
			}
		}
	}

	if (nSrcDam == 128)
	{
		int32_t nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_FIREMAXDAM, 0) << 8;
		if (nMaxDamage >= 8)
		{
			const int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_FIREMINDAM, 0) << 8;
			const int32_t nMastery = STATLIST_UnitGetStatValue(pAttacker, STAT_PASSIVE_FIRE_MASTERY, 0);
			pDamage->dwFireDamage = SUNITDMG_RollDamageValueInRange(pAttacker, nMinDamage, nMaxDamage, nMastery, nMastery, pDamage->dwFireDamage);
		}
		else
		{
			pDamage->dwFireDamage = std::max(pDamage->dwFireDamage, 0);
		}

		nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_LIGHTMAXDAM, 0) << 8;
		if (nMaxDamage >= 8)
		{
			const int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_LIGHTMINDAM, 0) << 8;
			const int32_t nMastery = STATLIST_UnitGetStatValue(pAttacker, STAT_PASSIVE_LTNG_MASTERY, 0);
			pDamage->dwLtngDamage = SUNITDMG_RollDamageValueInRange(pAttacker, nMinDamage, nMaxDamage, nMastery, nMastery, pDamage->dwLtngDamage);
		}
		else
		{
			pDamage->dwLtngDamage = std::max(pDamage->dwLtngDamage, 0);
		}

		nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_COLDMAXDAM, 0) << 8;
		if (nMaxDamage >= 8)
		{
			const int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_COLDMINDAM, 0) << 8;
			const int32_t nMastery = STATLIST_UnitGetStatValue(pAttacker, STAT_PASSIVE_COLD_MASTERY, 0);
			pDamage->dwColdDamage = SUNITDMG_RollDamageValueInRange(pAttacker, nMinDamage, nMaxDamage, nMastery, nMastery, pDamage->dwColdDamage);
		}
		else
		{
			pDamage->dwColdDamage = std::max(pDamage->dwColdDamage, 0);
		}

		nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_MAGICMAXDAM, 0) << 8;
		if (nMaxDamage >= 8)
		{
			const int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_MAGICMINDAM, 0) << 8;
			const int32_t nMastery = STATLIST_UnitGetStatValue(pAttacker, STAT_PASSIVE_MAG_MASTERY, 0);
			pDamage->dwMagDamage = SUNITDMG_RollDamageValueInRange(pAttacker, nMinDamage, nMaxDamage, nMastery, nMastery, pDamage->dwMagDamage);
		}
		else
		{
			pDamage->dwMagDamage = std::max(pDamage->dwMagDamage, 0);
		}
	}
	else
	{
		int32_t nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_FIREMAXDAM, 0) << 8;
		if (nMaxDamage >= 8)
		{
			int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_FIREMINDAM, 0) << 8;
			const int32_t nMastery = STATLIST_UnitGetStatValue(pAttacker, STAT_PASSIVE_FIRE_MASTERY, 0);
			if (nMaxDamage > 0)
			{
				nMinDamage += MONSTERUNIQUE_CalculatePercentage(nMinDamage, nMastery, 100);
				nMaxDamage += MONSTERUNIQUE_CalculatePercentage(nMaxDamage, nMastery, 100);

				pDamage->dwFireDamage += nMinDamage;
				if (nMaxDamage > nMinDamage)
				{
					pDamage->dwFireDamage += ITEMS_RollLimitedRandomNumber(&pAttacker->pSeed, nMaxDamage - nMinDamage);
				}
			}
		}

		pDamage->dwFireDamage = std::max(pDamage->dwFireDamage, 0) * nSrcDam / 128;

		nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_LIGHTMAXDAM, 0) << 8;
		if (nMaxDamage >= 8)
		{
			int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_LIGHTMINDAM, 0) << 8;
			const int32_t nMastery = STATLIST_UnitGetStatValue(pAttacker, STAT_PASSIVE_LTNG_MASTERY, 0);
			if (nMaxDamage > 0)
			{
				nMinDamage += MONSTERUNIQUE_CalculatePercentage(nMinDamage, nMastery, 100);
				nMaxDamage += MONSTERUNIQUE_CalculatePercentage(nMaxDamage, nMastery, 100);

				pDamage->dwLtngDamage += nMinDamage;
				if (nMaxDamage > nMinDamage)
				{
					pDamage->dwLtngDamage += ITEMS_RollLimitedRandomNumber(&pAttacker->pSeed, nMaxDamage - nMinDamage);
				}
			}
		}

		pDamage->dwLtngDamage = std::max(pDamage->dwLtngDamage, 0) * nSrcDam / 128;

		nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_COLDMAXDAM, 0) << 8;
		if (nMaxDamage >= 8)
		{
			int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_COLDMINDAM, 0) << 8;
			const int32_t nMastery = STATLIST_UnitGetStatValue(pAttacker, STAT_PASSIVE_COLD_MASTERY, 0);
			if (nMaxDamage > 0)
			{
				nMinDamage += MONSTERUNIQUE_CalculatePercentage(nMinDamage, nMastery, 100);
				nMaxDamage += MONSTERUNIQUE_CalculatePercentage(nMaxDamage, nMastery, 100);

				pDamage->dwColdDamage += nMinDamage;
				if (nMaxDamage > nMinDamage)
				{
					pDamage->dwColdDamage += ITEMS_RollLimitedRandomNumber(&pAttacker->pSeed, nMaxDamage - nMinDamage);
				}
			}
		}

		pDamage->dwColdDamage = std::max(pDamage->dwColdDamage, 0) * nSrcDam / 128;

		nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_MAGICMAXDAM, 0) << 8;
		if (nMaxDamage >= 8)
		{
			int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_MAGICMINDAM, 0) << 8;
			const int32_t nMastery = STATLIST_UnitGetStatValue(pAttacker, STAT_PASSIVE_MAG_MASTERY, 0);
			if (nMaxDamage > 0)
			{
				nMinDamage += MONSTERUNIQUE_CalculatePercentage(nMinDamage, nMastery, 100);
				nMaxDamage += MONSTERUNIQUE_CalculatePercentage(nMaxDamage, nMastery, 100);

				pDamage->dwMagDamage += nMinDamage;
				if (nMaxDamage > nMinDamage)
				{
					pDamage->dwMagDamage += ITEMS_RollLimitedRandomNumber(&pAttacker->pSeed, nMaxDamage - nMinDamage);
				}
			}
		}

		pDamage->dwMagDamage = std::max(pDamage->dwMagDamage, 0) * nSrcDam / 128;
	}

	const int32_t nHirelingTypeId = MONSTERS_GetHirelingTypeId(pAttacker);
	if (pAttacker->dwUnitType == UNIT_MONSTER)
	{
		if (!nHirelingTypeId)
		{
			if (!(pDamage->dwHitFlags & DAMAGEHITFLAG_LIFEDRAIN))
			{
				const int32_t nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_LIFEDRAINMAXDAM, 0) << 8;
				if (nMaxDamage >= 8)
				{
					const int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_LIFEDRAINMINDAM, 0) << 8;
					pDamage->dwLifeLeech = SUNITDMG_RollDamageValueInRange(pAttacker, nMinDamage, nMaxDamage, 0, 0, pDamage->dwLifeLeech);
				}
				else
				{
					pDamage->dwLifeLeech = std::max(pDamage->dwLifeLeech, 0);
				}
				pDamage->dwLifeLeech += pDamage->dwLifeLeech * nSrcDam / 128;
			}

			if (!(pDamage->dwHitFlags & DAMAGEHITFLAG_MANADRAIN))
			{
				const int32_t nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_MANADRAINMAXDAM, 0) << 8;
				if (nMaxDamage >= 8)
				{
					const int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_MANADRAINMINDAM, 0) << 8;
					pDamage->dwManaLeech = SUNITDMG_RollDamageValueInRange(pAttacker, nMinDamage, nMaxDamage, 0, 0, pDamage->dwManaLeech);
				}
				else
				{
					pDamage->dwManaLeech = std::max(pDamage->dwManaLeech, 0);
				}
				pDamage->dwManaLeech += pDamage->dwManaLeech * nSrcDam / 128;
			}

			if (!(pDamage->dwHitFlags & DAMAGEHITFLAG_STAMINADRAIN))
			{
				const int32_t nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_STAMDRAINMAXDAM, 0) << 8;
				if (nMaxDamage >= 8)
				{
					const int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_STAMDRAINMINDAM, 0) << 8;
					pDamage->dwStamLeech = SUNITDMG_RollDamageValueInRange(pAttacker, nMinDamage, nMaxDamage, 0, 0, pDamage->dwStamLeech);
				}
				else
				{
					pDamage->dwStamLeech = std::max(pDamage->dwStamLeech, 0);
				}
				pDamage->dwStamLeech += pDamage->dwStamLeech * nSrcDam / 128;
			}
		}
		else
		{
			if (!(pDamage->dwHitFlags & DAMAGEHITFLAG_LIFEDRAIN))
			{
				pDamage->dwLifeLeech += STATLIST_UnitGetStatValue(pAttacker, STAT_LIFEDRAINMINDAM, 0);
			}

			if (!(pDamage->dwHitFlags & DAMAGEHITFLAG_MANADRAIN))
			{
				pDamage->dwManaLeech += STATLIST_UnitGetStatValue(pAttacker, STAT_MANADRAINMINDAM, 0);
			}

			if (STATLIST_UnitGetStatValue(pAttacker, STAT_SKILL_BYPASS_UNDEAD, 0))
			{
				pDamage->dwHitFlags |= DAMAGEHITFLAG_BYPASSUNDEAD;
			}

			if (STATLIST_UnitGetStatValue(pAttacker, STAT_SKILL_BYPASS_DEMONS, 0))
			{
				pDamage->dwHitFlags |= DAMAGEHITFLAG_BYPASSDEMON;
			}

			if (STATLIST_UnitGetStatValue(pAttacker, STAT_SKILL_BYPASS_BEASTS, 0))
			{
				pDamage->dwHitFlags |= DAMAGEHITFLAG_BYPASSBEASTS;
			}
		}
	}
	else
	{
		if (pAttacker->dwUnitType == UNIT_PLAYER || nHirelingTypeId)
		{
			if (!(pDamage->dwHitFlags & DAMAGEHITFLAG_LIFEDRAIN))
			{
				pDamage->dwLifeLeech += STATLIST_UnitGetStatValue(pAttacker, STAT_LIFEDRAINMINDAM, 0);
			}

			if (!(pDamage->dwHitFlags & DAMAGEHITFLAG_MANADRAIN))
			{
				pDamage->dwManaLeech += STATLIST_UnitGetStatValue(pAttacker, STAT_MANADRAINMINDAM, 0);
			}

			if (STATLIST_UnitGetStatValue(pAttacker, STAT_SKILL_BYPASS_UNDEAD, 0))
			{
				pDamage->dwHitFlags |= DAMAGEHITFLAG_BYPASSUNDEAD;
			}

			if (STATLIST_UnitGetStatValue(pAttacker, STAT_SKILL_BYPASS_DEMONS, 0))
			{
				pDamage->dwHitFlags |= DAMAGEHITFLAG_BYPASSDEMON;
			}

			if (STATLIST_UnitGetStatValue(pAttacker, STAT_SKILL_BYPASS_BEASTS, 0))
			{
				pDamage->dwHitFlags |= DAMAGEHITFLAG_BYPASSBEASTS;
			}
		}
	}

	int32_t nMinDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_POISONMINDAM, 0);
	int32_t nMaxDamage = STATLIST_UnitGetStatValue(pAttacker, STAT_POISONMAXDAM, 0);

	if (nMaxDamage > 0)
	{
		const int32_t nMastery = STATLIST_UnitGetStatValue(pAttacker, STAT_PASSIVE_POIS_MASTERY, 0);

		nMinDamage += MONSTERUNIQUE_CalculatePercentage(nMinDamage, nMastery, 100);
		nMaxDamage += MONSTERUNIQUE_CalculatePercentage(nMaxDamage, nMastery, 100);

		pDamage->dwPoisDamage += nMinDamage;

		if (nMaxDamage > nMinDamage)
		{
			pDamage->dwPoisDamage += ITEMS_RollLimitedRandomNumber(&pAttacker->pSeed, nMaxDamage - nMinDamage);
		}
	}

	pDamage->dwPoisDamage = std::max(pDamage->dwPoisDamage, 0) * nSrcDam / 128;

	if (pDamage->dwPoisDamage)
	{
		const int32_t nPoisonLength = STATLIST_UnitGetStatValue(pAttacker, STAT_SKILL_POISON_OVERRIDE_LENGTH, 0);
		if (nPoisonLength <= 0)
		{
			pDamage->dwPoisLen += STATLIST_UnitGetStatValue(pAttacker, STAT_POISONLENGTH, 0);
			const int32_t nPoisonCount = STATLIST_UnitGetStatValue(pAttacker, STAT_POISON_COUNT, 0);
			if (nPoisonCount > 1)
			{
				pDamage->dwPoisLen /= nPoisonCount;
			}
		}
		else
		{
			pDamage->dwPoisLen += nPoisonLength;
		}
	}

	if (pDamage->dwColdDamage > 0)
	{
		pDamage->dwColdLen += nSrcDam * STATLIST_UnitGetStatValue(pAttacker, STAT_COLDLENGTH, 0) / 128;
	}

	if (!pDamage->dwStunLen)
	{
		pDamage->dwStunLen += nSrcDam * STATLIST_UnitGetStatValue(pAttacker, STAT_STUNLENGTH, 0) / 128;
	}

	//TODO: Burn damage

	//v90 = nSrcDam * pDamage->dwBurnDamage;
	//v91 = DAMAGE_CalculatePercentageValue(316, 0) + 316;
	//v92 = DAMAGE_CalculatePercentageValue(317, 0) + 317;
	//v93 = v91 + v90;
	//if (v92 > v91)
	//{
	//	v93 += ITEMS_RollLimitedRandomNumber(&pAttacker->pSeed, v92 - v91);
	//}

	//v94 = v93 & ((v93 < 0) - 1);
	//v95 = (((BYTE4(v94) & 0x7F) + (int32_t)v94) >> 7) + pDamage->dwBurnDamage;
	//LODWORD(v94) = pDamage->dwBurnLen;
	//pDamage->dwBurnDamage = v95;
	//if ((_DWORD)v94)
	//{
	//	pDamage->dwBurnLen += nSrcDam * STATLIST_UnitGetStatValue(pAttacker, STAT_FIRELENGTH, 0) / 128;
	//}

	if ((pAttacker->dwUnitType == UNIT_PLAYER || nHirelingTypeId) && !(pDamage->wResultFlags & DAMAGERESULTFLAG_32))
	{
		SUNITEVENT_EventFunc_Handler(pGame, EVENT_ATTACKEDINMELEE, pDefender, pAttacker, pDamage);
	}

	if (pDamage->nConvType)
	{
		const int32_t nConvertedDamage = MONSTERUNIQUE_CalculatePercentage(pDamage->dwPhysDamage, pDamage->dwConvPct, 100);

		pDamage->dwPhysDamage -= nConvertedDamage;

		pDamage->dwPhysDamage = std::max(pDamage->dwPhysDamage, 0);

		int32_t nElemType = pDamage->nConvType;
		if (nElemType == ELEMTYPE_RAND)
		{
			nElemType = ITEMS_RollRandomNumber(&pAttacker->pSeed) % 5 + 1;
		}

		switch (nElemType)
		{
		case ELEMTYPE_FIRE:
			pDamage->dwFireDamage += nConvertedDamage;
			break;

		case ELEMTYPE_LTNG:
			pDamage->dwLtngDamage += nConvertedDamage;
			break;

		case ELEMTYPE_MAGIC:
			pDamage->dwMagDamage += nConvertedDamage;
			break;

		case ELEMTYPE_COLD:
			pDamage->dwColdDamage += nConvertedDamage;
			if (pDamage->dwColdLen < 50)
			{
				pDamage->dwColdLen = 50;
			}
			break;

		case ELEMTYPE_POIS:
			pDamage->dwPoisDamage += nConvertedDamage / 8;
			pDamage->dwPoisLen = std::max(pDamage->dwPoisLen, 50);
			break;

		case ELEMTYPE_BURN:
			pDamage->dwFireDamage += nConvertedDamage;
			pDamage->dwBurnLen = std::max(pDamage->dwBurnLen, 50);
			break;

		case ELEMTYPE_FREEZE:
			pDamage->dwColdDamage += nConvertedDamage;
			pDamage->dwFrzLen = std::max(pDamage->dwFrzLen, 50);
			break;

		default:
			break;
		}
	}

	D2GAME_MONSTER_ApplyCriticalDamage_6FC62E70(pAttacker, pDefender, pDamage);
	sub_6FC7CA10(pAttacker, a5);
}

//D2Game.0x6FCBF400
int32_t __fastcall SUNITDMG_CheckMonType(int32_t nMonType1, int32_t nMonType2)
{
	if (nMonType1 > 0 && nMonType1 < sgptDataTables->nMonTypeTxtRecordCount && nMonType2 > 0 && nMonType2 < sgptDataTables->nMonTypeTxtRecordCount)
	{
		return gdwBitMasks[nMonType1 & 31] & sgptDataTables->pMonTypeNest[(nMonType1 >> 5) + nMonType2 * sgptDataTables->nMonTypeIndex];
	}

	return 0;
}

//D2Game.0x6FCBF450
int32_t __fastcall SUNITDMG_RollDamageValueInRange(D2UnitStrc* pUnit, int32_t nMinDamage, int32_t nMaxDamage, int32_t nMinDamageBonusPct, int32_t nMaxDamageBonusPct, int32_t nDamage)
{
	if (nMaxDamage > 0)
	{
		nMinDamage += MONSTERUNIQUE_CalculatePercentage(nMinDamage, nMinDamageBonusPct, 100);

		nMaxDamage += MONSTERUNIQUE_CalculatePercentage(nMaxDamage, nMaxDamageBonusPct, 100);

		nDamage += nMinDamage;

		if (nMaxDamage > nMinDamage)
		{
			nDamage += ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nMaxDamage - nMinDamage);
		}
	}

	return std::max(nDamage, 0);
}

//D2Game.0x6FCBF620
void __fastcall SUNITDMG_CalculateTotalDamage(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage)
{
	//D2Game.0x6FD3F090
	constexpr D2DamageStatTableStrc sgDamageStatTable[12] =
	{
		{ offsetof(D2DamageStrc, dwPhysDamage),	STAT_DAMAGERESIST,				-1,						-1,							-1,								-1,						DAMAGE_REDUCTION_PHYSICAL,	0, 1, "Dam", 8, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwFireDamage),	STAT_FIRERESIST,				STAT_MAXFIRERESIST,		STAT_PASSIVE_FIRE_PIERCE,	STAT_ITEM_ABSORBFIRE_PERCENT,	STAT_ITEM_ABSORBFIRE,	DAMAGE_REDUCTION_MAGICAL,	0, 1, "Fire", 8, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwLtngDamage),	STAT_LIGHTRESIST,				STAT_MAXLIGHTRESIST,	STAT_PASSIVE_LTNG_PIERCE,	STAT_ITEM_ABSORBLIGHT_PERCENT,	STAT_ITEM_ABSORBLIGHT,	DAMAGE_REDUCTION_MAGICAL,	0, 1, "Ligt", 8, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwColdDamage),	STAT_COLDRESIST,				STAT_MAXCOLDRESIST,		STAT_PASSIVE_COLD_PIERCE,	STAT_ITEM_ABSORBCOLD_PERCENT,	STAT_ITEM_ABSORBCOLD,	DAMAGE_REDUCTION_MAGICAL,	0, 1, "Cold", 8, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwMagDamage),	STAT_MAGICRESIST,				STAT_MAXMAGICRESIST,	-1,							STAT_ITEM_ABSORBMAGIC_PERCENT,	STAT_ITEM_ABSORBMAGIC,	DAMAGE_REDUCTION_MAGICAL,	0, 1, "Magc", 8, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwColdLen),	STAT_COLDRESIST,				STAT_MAXCOLDRESIST,		STAT_PASSIVE_COLD_PIERCE,	-1,								-1,						DAMAGE_REDUCTION_NONE,		0, 0, "Clen", 0, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwFrzLen),		STAT_COLDRESIST,				STAT_MAXCOLDRESIST,		STAT_PASSIVE_COLD_PIERCE,	-1,								-1,						DAMAGE_REDUCTION_NONE,		0, 0, "Flen", 0, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwPoisLen),	STAT_ITEM_POISONLENGTHRESIST,	-1,						STAT_PASSIVE_POIS_PIERCE,	-1,								-1,						DAMAGE_REDUCTION_NONE,		0, 0, "Plen", 0, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwPoisDamage),	STAT_POISONRESIST,				STAT_MAXPOISONRESIST,	STAT_PASSIVE_POIS_PIERCE,	-1,								-1,						DAMAGE_REDUCTION_NONE,		0, 1, "Pois", 0, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwLifeLeech),	-1,								-1,						-1,							-1,								-1,						DAMAGE_REDUCTION_NONE,		1, 1, "Life", 8, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwManaLeech),	-1,								-1,						-1,							-1,								-1,						DAMAGE_REDUCTION_NONE,		1, 1, "Mana", 8, 0, 0, 0 },
		{ offsetof(D2DamageStrc, dwStamLeech),	-1,								-1,						-1,							-1,								-1,						DAMAGE_REDUCTION_NONE,		1, 1, "Stam", 8, 0, 0, 0 },
	};

	D2DamageInfoStrc damageInfo = {};
	damageInfo.pGame = pGame;
	damageInfo.pAttacker = pAttacker;
	damageInfo.pDifficultyLevelsTxt = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
	damageInfo.pDefender = pDefender;
	damageInfo.pDamage = pDamage;


	damageInfo.bAttackerIsMonster = 0;
	if (damageInfo.pAttacker && pAttacker->dwUnitType == UNIT_MONSTER && !MONSTERS_GetHirelingTypeId(pAttacker))
	{
		damageInfo.bAttackerIsMonster = 1;
	}

	damageInfo.bDefenderIsMonster = 0;
	if (pDefender && pDefender->dwUnitType == UNIT_MONSTER && !MONSTERS_GetHirelingTypeId(pDefender))
	{
		damageInfo.bDefenderIsMonster = 1;
	}

	damageInfo.nDamageReduction[0] = 0;

	int32_t nDamageReduction = STATLIST_UnitGetStatValue(pDefender, STAT_NORMAL_DAMAGE_REDUCTION, 0) << 8;
	if (nDamageReduction > 0 && pDamage->dwPiercePct > 0)
	{
		nDamageReduction = MONSTERUNIQUE_CalculatePercentage(nDamageReduction, pDamage->dwPiercePct, 1024);
	}
	damageInfo.nDamageReduction[1] = nDamageReduction;

	int32_t nMagicDamageReduction = STATLIST_UnitGetStatValue(pDefender, STAT_MAGIC_DAMAGE_REDUCTION, 0) << 8;
	if (nMagicDamageReduction > 0 && pDamage->dwPiercePct > 0)
	{
		nMagicDamageReduction = MONSTERUNIQUE_CalculatePercentage(nMagicDamageReduction, pDamage->dwPiercePct, 1024);
	}
	damageInfo.nDamageReduction[2] = nMagicDamageReduction;

	if (damageInfo.pAttacker && damageInfo.pDefender && damageInfo.pAttacker != damageInfo.pDefender)
	{
		int32_t nDamagePercent = 100;

		if (damageInfo.pDefender->dwUnitType == UNIT_PLAYER)
		{
			if (damageInfo.pAttacker->dwUnitType == UNIT_PLAYER || MONSTERS_GetHirelingTypeId(damageInfo.pAttacker) || damageInfo.pAttacker->dwFlags & UNITFLAG_ISREVIVE)
			{
				nDamagePercent = 17;
			}
		}
		else if (MONSTERS_GetHirelingTypeId(damageInfo.pDefender) && MONSTERS_GetHirelingTypeId(damageInfo.pAttacker))
		{
			nDamagePercent = 25;
		}
		else if (MONSTERS_GetHirelingTypeId(damageInfo.pAttacker) && MONSTERS_IsBoss(nullptr, damageInfo.pDefender))
		{
			if (damageInfo.pDifficultyLevelsTxt)
			{
				nDamagePercent = damageInfo.pDifficultyLevelsTxt->dwHireableBossDmgPercent;
			}
		}
		else if (MONSTERS_IsPrimeEvil(damageInfo.pAttacker) && damageInfo.pDefender && damageInfo.pDefender->dwFlags & UNITFLAG_ISREVIVE)
		{
			if (MONSTERS_GetHirelingTypeId(damageInfo.pDefender))
			{
				nDamagePercent = 200;
			}
			else
			{
				nDamagePercent = 400;
			}
		}

		if (nDamagePercent != 100)
		{
			for (int32_t i = 0; i < std::size(sgDamageStatTable); ++i)
			{
				const D2DamageStatTableStrc* pDamageStatTableRecord = &sgDamageStatTable[i];
				if (pDamageStatTableRecord->unk0x20)
				{
					int32_t* pDamageValue = (int32_t*)((char*)damageInfo.pDamage + pDamageStatTableRecord->nOffsetInDamageStrc);
					if (*pDamageValue > 0)
					{
						*pDamageValue = MONSTERUNIQUE_CalculatePercentage(*pDamageValue, nDamagePercent, 100);
					}
				}
			}
		}
	}

	SUNITEVENT_EventFunc_Handler(pGame, EVENT_ABSORBDAMAGE, pDefender, pAttacker, pDamage);

	if (damageInfo.pDamage->dwColdLen > 0 || damageInfo.pDamage->dwFrzLen > 0)
	{
		if (STATLIST_UnitGetItemStatOrSkillStatValue(damageInfo.pDefender, STAT_ITEM_CANNOTBEFROZEN, 0))
		{
			damageInfo.pDamage->dwColdLen = 0;
			damageInfo.pDamage->dwFrzLen = 0;
		}
		else if (STATLIST_UnitGetItemStatOrSkillStatValue(damageInfo.pDefender, STAT_ITEM_HALFFREEZEDURATION, 0))
		{
			damageInfo.pDamage->dwColdLen /= 2;
			damageInfo.pDamage->dwFrzLen /= 2;
		}
	}

	if (damageInfo.pDamage->dwPoisLen > 0 && STATES_CheckState(damageInfo.pDefender, STATE_SHRINE_RESIST_POISON))
	{
		damageInfo.pDamage->dwPoisLen = 0;
	}

	if (damageInfo.pDamage->dwBurnLen > 0 && STATES_CheckState(damageInfo.pDefender, STATE_SHRINE_RESIST_FIRE))
	{
		damageInfo.pDamage->dwBurnLen = 0;
	}

	int32_t bDontAbsorb = 0;
	if (pDefender && pDefender->dwUnitType == UNIT_MONSTER)
	{
		const int32_t bIsUndead = MONSTERS_IsUndead(pDefender);
		const int32_t bIsDemon = MONSTERS_IsDemon(pDefender);
		if (bIsUndead && pDamage->dwHitFlags & DAMAGEHITFLAG_BYPASSUNDEAD)
		{
			bDontAbsorb = 1;
		}

		if (bIsDemon && pDamage->dwHitFlags & DAMAGEHITFLAG_BYPASSDEMON)
		{
			bDontAbsorb = 1;
		}

		if (!bIsUndead && !bIsDemon && pDamage->dwHitFlags & DAMAGEHITFLAG_BYPASSBEASTS)
		{
			bDontAbsorb = 1;
		}
	}
	else
	{
		if (pDamage->dwHitFlags & DAMAGEHITFLAG_BYPASSBEASTS)
		{
			bDontAbsorb = 1;
		}
	}

	for (int32_t i = 0; i < std::size(sgDamageStatTable); ++i)
	{
		const D2DamageStatTableStrc* pDamageStatTableRecord = &sgDamageStatTable[i];
		if (pDamageStatTableRecord->unk0x1C && !damageInfo.bAttackerIsMonster)
		{
			break;
		}

		SUNITDMG_ApplyResistancesAndAbsorb(&damageInfo, pDamageStatTableRecord, bDontAbsorb);
	}

	pDamage->dwDmgTotal = pDamage->dwPhysDamage + pDamage->dwFireDamage + pDamage->dwLtngDamage + pDamage->dwMagDamage + pDamage->dwColdDamage + pDamage->dwPoisDamage;

	if (damageInfo.bAttackerIsMonster)
	{
		pDamage->dwDmgTotal += pDamage->dwLifeLeech;
	}
}

//D2Game.0x6FCBFB40
void __fastcall SUNITDMG_ApplyResistancesAndAbsorb(D2DamageInfoStrc* pDamageInfo, const D2DamageStatTableStrc* pDamageStatTableRecord, int32_t bDontAbsorb)
{
	int32_t* pValue = (int32_t*)((char*)pDamageInfo->pDamage + pDamageStatTableRecord->nOffsetInDamageStrc);
	int32_t nValue = *pValue;
	int32_t nPreviousValue = *pValue;

	*pValue = std::max(*pValue, 0);

	int32_t nResValue = 0;
	if (pDamageStatTableRecord->nResStatId != -1)
	{
		nResValue = STATLIST_UnitGetStatValue(pDamageInfo->pDefender, pDamageStatTableRecord->nResStatId, 0);
	}

	if (pDamageStatTableRecord->nPierceStatId != -1 && (nResValue < 100 || !pDamageInfo->bDefenderIsMonster))
	{
		const int32_t nPierceValue = STATLIST_UnitGetStatValue(pDamageInfo->pAttacker, pDamageStatTableRecord->nPierceStatId, 0);
		if (nPierceValue)
		{
			nResValue -= nPierceValue;
		}
	}

	if (!pDamageInfo->bDefenderIsMonster && pDamageStatTableRecord->nResStatId != STAT_DAMAGERESIST && pDamageStatTableRecord->nResStatId != STAT_MAGICRESIST)
	{
		if (pDamageInfo->pGame->bExpansion)
		{
			nResValue += pDamageInfo->pDifficultyLevelsTxt->dwResistPenalty;
		}
		else if (pDamageInfo->pGame->nDifficulty == DIFFMODE_NIGHTMARE)
		{
			nResValue -= 20;
		}
		else if (pDamageInfo->pGame->nDifficulty == DIFFMODE_HELL)
		{
			nResValue -= 50;
		}
	}

	if (nResValue > 0)
	{
		if (!pDamageInfo->bDefenderIsMonster)
		{
			int32_t nMaxResValue = 75;
			if (pDamageStatTableRecord->nMaxResStatId == -1)
			{
				if (pDamageStatTableRecord->nResStatId == STAT_DAMAGERESIST)
				{
					nMaxResValue = 50;
				}
			}
			else
			{
				nMaxResValue = std::min(STATLIST_UnitGetStatValue(pDamageInfo->pDefender, pDamageStatTableRecord->nMaxResStatId, pDamageInfo->bDefenderIsMonster) + 75, 95);
			}

			nResValue = D2Clamp(nResValue, -100, nMaxResValue);
		}

		if (pDamageStatTableRecord->nResStatId == STAT_DAMAGERESIST && STATES_CheckState(pDamageInfo->pAttacker, STATE_SANCTUARY) && MONSTERS_IsUndead(pDamageInfo->pDefender))
		{
			nResValue = 0;
		}
	}
	else
	{
		nResValue = std::max(nResValue, -100);
	}

	if (bDontAbsorb)
	{
		nResValue = std::min(nResValue, 0);
	}
	else
	{
		nValue = nPreviousValue - pDamageInfo->nDamageReduction[pDamageStatTableRecord->nDamageReductionType];
	}

	if (nValue > 0 && nResValue)
	{
		nResValue = std::min(nResValue, 100);
		nValue = MONSTERUNIQUE_CalculatePercentage(nValue, 100 - nResValue, 100);
	}

	//D2GAME_Return_6FCCD440(pDamageInfo->pDefender, nPreviousValue, nResValue, pDamageInfo->nDamageReduction[pDamageStatTableRecord->nDamageReductionType], nValue, pDamageStatTableRecord->szName, pDamageStatTableRecord->field_28);

	if (bDontAbsorb)
	{
		*pValue = nValue;
		return;
	}

	nPreviousValue = nValue;
	if (pDamageStatTableRecord->nAbsorbPctStatId != -1)
	{
		int32_t nAbsorbPctValue = STATLIST_UnitGetStatValue(pDamageInfo->pDefender, pDamageStatTableRecord->nAbsorbPctStatId, 0);

		if (nAbsorbPctValue > 0)
		{
			nAbsorbPctValue = std::min(nAbsorbPctValue, 40);
			const int32_t nDamageAbsorbed = MONSTERUNIQUE_CalculatePercentage(nValue, nAbsorbPctValue, 100);

			nValue -= nDamageAbsorbed;
			pDamageInfo->pDamage->dwAbsLife += nDamageAbsorbed;
		}

		int32_t nAbsorbValue = STATLIST_UnitGetStatValue(pDamageInfo->pDefender, pDamageStatTableRecord->nAbsorbStatId, 0) << 8;
		if (nAbsorbValue > 0)
		{
			nAbsorbValue = std::min(nAbsorbValue, nValue);
			nValue -= nAbsorbValue;

			pDamageInfo->pDamage->dwAbsLife += nAbsorbValue;
		}
	}

	//D2GAME_Return_6FCCD420(pDamageInfo->pDefender, nPreviousValue - nValue, pDamageStatTableRecord->szName);

	*pValue = nValue;
}

//D2Game.0x6FCBFE90
void __fastcall SUNITDMG_ExecuteEvents(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t bMissile, D2DamageStrc* pDamage)
{
	if (!sub_6FCBD900(pGame, pAttacker, pDefender) && !(pDamage->dwHitFlags & DAMAGEHITFLAG_4096))
	{
		pDamage->wResultFlags &= (uint16_t)(~(DAMAGERESULTFLAG_SUCCESSFULHIT | DAMAGERESULTFLAG_WILLDIE));
		return;
	}

	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pDefender);
	if (!pRoom)
	{
		pDamage->wResultFlags &= (uint16_t)(~(DAMAGERESULTFLAG_SUCCESSFULHIT | DAMAGERESULTFLAG_WILLDIE));
		return;
	}

	if (DUNGEON_IsRoomInTown(pRoom))
	{
		if (!pAttacker || pAttacker->dwUnitType != UNIT_MONSTER)
		{
			pDamage->wResultFlags &= (uint16_t)(~DAMAGERESULTFLAG_WILLDIE);
			return;
		}

		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pAttacker->dwClassId);
		if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTOWN]))
		{
			pDamage->wResultFlags &= (uint16_t)(~DAMAGERESULTFLAG_WILLDIE);
			return;
		}
	}

	int32_t nAttackerUnitType = 6;
	if (pAttacker)
	{
		nAttackerUnitType = pAttacker->dwUnitType;
	}

	int32_t nDefenderUnitType = 6;
	if (pDefender)
	{
		nDefenderUnitType = pDefender->dwUnitType;

		if (pDefender->dwUnitType == UNIT_MONSTER)
		{
			D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pDefender->dwClassId);
			if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_KILLABLE]))
			{
				return;
			}
		}
	}

	if (SUNIT_IsDead(pDefender))
	{
		return;
	}

	UNITS_StoreLastAttacker(pDefender, pAttacker);

	if (bMissile)
	{
		SUNITDMG_CalculateTotalDamage(pGame, pAttacker, pDefender, pDamage);
	}

	if (!(pDamage->wResultFlags & DAMAGERESULTFLAG_32) && pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
	{
		if (bMissile)
		{
			if (pDamage->dwHitFlags & DAMAGEHITFLAG_32)
			{
				if (!(pDamage->dwHitFlags & DAMAGEHITFLAG_128))
				{
					SUNITEVENT_EventFunc_Handler(pGame, EVENT_DOMISSILEDMG, pAttacker, pDefender, pDamage);
				}
				SUNITEVENT_EventFunc_Handler(pGame, EVENT_DAMAGEDBYMISSILE, pDefender, pAttacker, pDamage);
			}
		}
		else
		{
			SUNITEVENT_EventFunc_Handler(pGame, EVENT_DOMELEEDMG, pAttacker, pDefender, pDamage);
			SUNITEVENT_EventFunc_Handler(pGame, EVENT_DAMAGEDINMELEE, pDefender, pAttacker, pDamage);
		}
	}

	const int32_t nHp = STATLIST_UnitGetStatValue(pDefender, STAT_HITPOINTS, 0);
	pDamage->dwPhysDamage = std::min(pDamage->dwPhysDamage, nHp);

	if (pDamage->dwLifeLeech || pDamage->dwManaLeech)
	{
		int32_t nDrain = 0;
		if (pDefender && pDefender->dwUnitType == UNIT_MONSTER)
		{
			D2MonStatsTxt* pMonStatsTxtRecord = SUNITDMG_GetMonStatsTxtRecordFromUnit(pDefender);
			if (pMonStatsTxtRecord)
			{
				nDrain = pMonStatsTxtRecord->nDrain[pGame->nDifficulty];
			}
		}
		else
		{
			nDrain = 100;
		}

		if (nDrain > 0)
		{
			pDamage->dwLifeLeech <<= 6;
			pDamage->dwManaLeech <<= 6;

			int32_t nUnitType = 0;
			if (pAttacker)
			{
				if (pAttacker->dwUnitType == UNIT_PLAYER)
				{
					D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pAttacker->pGame->nDifficulty);
					if (pDifficultyLevelsTxtRecord->dwLifeStealDiv)
					{
						pDamage->dwLifeLeech /= pDifficultyLevelsTxtRecord->dwLifeStealDiv;
					}

					if (pDifficultyLevelsTxtRecord->dwManaStealDiv)
					{
						pDamage->dwManaLeech /= pDifficultyLevelsTxtRecord->dwManaStealDiv;
					}
				}
				else
				{
					if (pAttacker->dwUnitType != UNIT_MONSTER || !MONSTERS_GetHirelingTypeId(pAttacker))
					{
						nUnitType = pAttacker->dwUnitType;
						int32_t nClassId = pAttacker->dwClassId;
						int32_t nAnimMode = pAttacker->dwAnimMode;
						D2COMMON_11013_ConvertMode(pAttacker, &nUnitType, &nClassId, &nAnimMode, __FILE__, __LINE__);
					}
				}
			}
			else
			{
				nUnitType = 6;
				int32_t nClassId = -1;
				int32_t nAnimMode = 0;
				D2COMMON_11013_ConvertMode(pAttacker, &nUnitType, &nClassId, &nAnimMode, __FILE__, __LINE__);
			}

			if (!nUnitType)
			{
				if (pDamage->dwPhysDamage > 0)
				{
					if (pDamage->dwManaLeech)
					{
						int32_t nLeechedMana = MONSTERUNIQUE_CalculatePercentage(pDamage->dwPhysDamage, pDamage->dwManaLeech, 100);
						if (nDrain != 100)
						{
							nLeechedMana = MONSTERUNIQUE_CalculatePercentage(nLeechedMana, nDrain, 100);
						}

						SUNITDMG_AddLeechedMana(pAttacker, nLeechedMana / 64);
					}

					if (pDamage->dwLifeLeech)
					{
						int32_t nLeechedHp = MONSTERUNIQUE_CalculatePercentage(pDamage->dwPhysDamage, pDamage->dwLifeLeech, 100);
						if (nDrain != 100)
						{
							nLeechedHp = MONSTERUNIQUE_CalculatePercentage(nLeechedHp, nDrain, 100);
						}

						SUNITDMG_AddLeechedLife(pAttacker, nLeechedHp / 64);

						if (pDamage->dwManaLeech)
						{
							if (!ITEMS_RollLimitedRandomNumber(&pAttacker->pSeed, 2))
							{
								UNITS_SetOverlay(pAttacker, 151, 0);
							}
							else
							{
								UNITS_SetOverlay(pAttacker, 152, 0);
							}
						}
						else
						{
							UNITS_SetOverlay(pAttacker, 151, 0);
						}
					}
					else if (pDamage->dwManaLeech)
					{
						UNITS_SetOverlay(pAttacker, 152, 0);
					}
				}
			}
			else
			{
				const int32_t nMana = STATLIST_UnitGetStatValue(pDefender, STAT_MANA, 0);
				const int32_t nStamina = STATLIST_UnitGetStatValue(pDefender, STAT_STAMINA, 0);

				pDamage->dwLifeLeech <<= 6;
				pDamage->dwManaLeech <<= 6;

				const int32_t nLifeLeech = std::min(pDamage->dwLifeLeech, pDamage->dwPhysDamage);
				const int32_t nManaLeech = std::min(pDamage->dwManaLeech, nMana);
				const int32_t nStaminaLeech = std::min(pDamage->dwStamLeech, nStamina);
				int32_t nTotalLeech = nStaminaLeech + nManaLeech + nLifeLeech;
				if (nTotalLeech)
				{
					if (nDrain != 100)
					{
						nTotalLeech = MONSTERUNIQUE_CalculatePercentage(nTotalLeech, nDrain, 100);
					}

					const int32_t nMissingHp = STATLIST_GetMaxLifeFromUnit(pAttacker) - STATLIST_UnitGetStatValue(pAttacker, STAT_HITPOINTS, 0);
					nTotalLeech = std::min(nTotalLeech, nMissingHp);

					if (nTotalLeech > 0)
					{
						STATLIST_AddUnitStat(pAttacker, STAT_HITPOINTS, nTotalLeech, 0);
						UNITS_SetOverlay(pAttacker, 151, 0);
					}
				}
			}
		}
	}

	if (nAttackerUnitType == UNIT_MONSTER && pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
	{
		sub_6FC6E890(pGame, pAttacker);
	}

	if (pDamage->dwAbsLife > 0 && !SUNIT_IsDead(pDefender) && !STATES_CheckState(pDefender, STATE_DEATH_DELAY))
	{
		const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pDefender);
		const int32_t nNewHp = std::min(pDamage->dwAbsLife + (int32_t)STATLIST_UnitGetStatValue(pDefender, STAT_HITPOINTS, 0), nMaxHp);
		STATLIST_SetUnitStat(pDefender, STAT_HITPOINTS, nNewHp, 0);
	}

	if (pDamage->dwDmgTotal > 0)
	{
		int32_t nNewHp = STATLIST_UnitGetStatValue(pDefender, STAT_HITPOINTS, 0) - pDamage->dwDmgTotal;
		if (nNewHp < 256)
		{
			nNewHp = 0;
		}
		STATLIST_SetUnitStat(pDefender, STAT_HITPOINTS, nNewHp, 0);
	}

	if (pDamage->dwManaLeech > 0)
	{
		int32_t nNewMana = STATLIST_UnitGetStatValue(pDefender, STAT_MANA, 0) - pDamage->dwManaLeech;
		if (nNewMana < 256)
		{
			nNewMana = 0;
		}
		STATLIST_SetUnitStat(pDefender, STAT_MANA, nNewMana, 0);
	}

	if (pDamage->dwStamLeech > 0)
	{
		int32_t nNewStamina = STATLIST_UnitGetStatValue(pDefender, STAT_STAMINA, 0) - pDamage->dwStamLeech;
		if (nNewStamina < 256)
		{
			nNewStamina = 0;
		}
		STATLIST_SetUnitStat(pDefender, STAT_STAMINA, nNewStamina, 0);
	}

	int32_t nStunLength = pDamage->dwStunLen;
	int32_t bApplyStun = 0;
	if (nStunLength > 0)
	{
		if (!pDefender || pDefender->dwUnitType != UNIT_MONSTER)
		{
			nStunLength = std::min(nStunLength, 250);
			bApplyStun = 1;
		}

		if ((!MONSTERUNIQUE_CheckMonTypeFlag(pDefender, 8u) || ITEMS_RollLimitedRandomNumber(&pAttacker->pSeed, 100) >= 90) && !MONSTERS_IsBoss(0, pDefender) && MONSTERMODE_GetMonStatsTxtRecord(pDefender->dwClassId)->nVelocity)
		{
			if (MONSTERS_GetHirelingTypeId(pDefender) && nStunLength >= 13)
			{
				nStunLength = 13;
			}
			else if (nStunLength > 250)
			{
				nStunLength = 250;
			}

			bApplyStun = 1;
		}

		if (bApplyStun)
		{
			D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pDefender, STATE_STUNNED);
			const int32_t nStunEndFrame = nStunLength + pAttacker->pGame->dwGameFrame;
			if (pStatList)
			{
				STATLIST_SetExpireFrame(pStatList, nStunEndFrame);
				EVENT_SetEvent(pAttacker->pGame, pDefender, UNITEVENTCALLBACK_REMOVESTATE, nStunEndFrame, 0, 0);
			}
			else
			{
				D2StatListStrc* pNewStatList = STATLIST_AllocStatList(pAttacker->pGame->pMemoryPool, 2u, nStunEndFrame, pAttacker->dwUnitType, pAttacker->dwUnitId);
				EVENT_SetEvent(pAttacker->pGame, pDefender, UNITEVENTCALLBACK_REMOVESTATE, nStunEndFrame, 0, 0);
				STATLIST_SetState(pNewStatList, STATE_STUNNED);
				STATLIST_SetStatRemoveCallback(pNewStatList, sub_6FD10E50);
				D2COMMON_10475_PostStatToStatList(pDefender, pNewStatList, 1);
				STATES_ToggleState(pDefender, STATE_STUNNED, 1);
			}
		}
	}

	SUNITDMG_ApplyColdState(pAttacker, pDefender, pDamage->dwColdLen);
	SUNITDMG_ApplyFreezeState(pAttacker, pDefender, pDamage->dwFrzLen);
	SUNITDMG_ApplyPoisonDamage(pAttacker, pDefender, pDamage->dwPoisDamage, pDamage->dwPoisLen);
	SUNITDMG_ApplyBurnDamage(pAttacker, pDefender, pDamage->dwBurnDamage, pDamage->dwBurnLen);

	if (STATLIST_UnitGetStatValue(pDefender, STAT_HITPOINTS, 0) <= 0)
	{
		pDamage->wResultFlags |= DAMAGERESULTFLAG_WILLDIE;
	}
	else
	{
		pDamage->wResultFlags &= (uint16_t)(~DAMAGERESULTFLAG_WILLDIE);
		if (pDamage->dwDmgTotal && nDefenderUnitType == UNIT_MONSTER)
		{
			if (STATLIST_UnitGetStatValue(pDefender, STAT_HPREGEN, 0))
			{
				D2GAME_EVENTS_Delete_6FC34840(pGame, pDefender, UNITEVENTCALLBACK_STATREGEN, 0);
				EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_STATREGEN, pGame->dwGameFrame + 1, 0, 0);
			}
			sub_6FCF8DD0(pDefender);
		}
	}

	if (!(pDamage->wResultFlags & DAMAGERESULTFLAG_32) && pDamage->wResultFlags & DAMAGERESULTFLAG_WILLDIE)
	{
		SUNITEVENT_EventFunc_Handler(pGame, EVENT_KILLED, pDefender, pAttacker, pDamage);
		SUNITEVENT_EventFunc_Handler(pGame, EVENT_KILL, pAttacker, pDefender, pDamage);
	}
}

//D2Game.0x6FCC05D0
D2MonStatsTxt* __fastcall SUNITDMG_GetMonStatsTxtRecordFromUnit(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        return pUnit->pMonsterData->pMonstatsTxt;
    }

    return nullptr;
}

//D2Game.0x6FCC05F0
int32_t __fastcall SUNITDMG_AddLeechedLife(D2UnitStrc* pUnit, int32_t nLifeLeeched)
{
	if (nLifeLeeched <= 0 || SUNIT_IsDead(pUnit) || STATES_CheckState(pUnit, STATE_DEATH_DELAY))
	{
		return 0;
	}

	const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
	const int32_t nHp = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
	const int32_t nNewHp = std::min(nLifeLeeched + nHp, nMaxHp);
	STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);

	if (nNewHp == nMaxHp)
	{
		return nMaxHp - nHp;
	}

	return nLifeLeeched;
}

//D2Game.0x6FCC0660
int32_t __fastcall SUNITDMG_AddLeechedMana(D2UnitStrc* pUnit, int32_t nManaLeeched)
{
	if (nManaLeeched <= 0)
	{
		return 0;
	}

	const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pUnit);
	const int32_t nMana = STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0);
	const int32_t nNewMana = std::min(nManaLeeched + nMana, nMaxMana);
	STATLIST_SetUnitStat(pUnit, STAT_MANA, nNewMana, 0);

	if (nNewMana == nMaxMana)
	{
		return nMaxMana - nMana;
	}

	return nManaLeeched;
}

//D2Game.0x6FCC06C0
void __fastcall SUNITDMG_ApplyPoisonDamage(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nPoisonDamage, int32_t nPoisonLength)
{
	if (nPoisonLength <= 0 || nPoisonDamage <= 0)
	{
		return;
	}

	D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pDefender, STATE_POISON);
	D2GameStrc* pGame = pAttacker->pGame;
	if (pDefender && pDefender->dwUnitType == UNIT_MONSTER)
	{
		D2GAME_EVENTS_Delete_6FC34840(pGame, pDefender, UNITEVENTCALLBACK_STATREGEN, 0);
		EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_STATREGEN, pGame->dwGameFrame + 1, 0, 0);
	}

	const int32_t nExpireFrame = nPoisonLength + pGame->dwGameFrame;
	if (pStatList)
	{
		if (-STATLIST_GetStatValue(pStatList, STAT_HPREGEN, 0) <= nPoisonDamage)
		{
			D2COMMON_10476(pStatList, nExpireFrame);
			EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);
			STATLIST_SetStatIfListIsValid(pStatList, STAT_HPREGEN, -nPoisonDamage, 0);
		}
		return;
	}

	STATES_ToggleState(pDefender, STATE_POISON, 1);

	int32_t nAttackerGUID = -1;
	int32_t nAttackerType = 6;
	if (pAttacker)
	{
		nAttackerGUID = pAttacker->dwUnitId;
		nAttackerType = pAttacker->dwUnitType;
	}

	D2StatListStrc* pNewStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nExpireFrame, nAttackerType, nAttackerGUID);
	EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);

	STATLIST_SetStatIfListIsValid(pNewStatList, STAT_HPREGEN, -nPoisonDamage, 0);
	STATLIST_SetState(pNewStatList, STATE_POISON);
	STATLIST_SetStatRemoveCallback(pNewStatList, sub_6FD10E50);
	D2COMMON_10475_PostStatToStatList(pDefender, pNewStatList, 1);
}

//D2Game.0x6FCC0800
void __fastcall SUNITDMG_ApplyBurnDamage(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nBurnDamage, int32_t nBurnLength)
{
	if (nBurnLength <= 0 || nBurnDamage <= 0)
	{
		return;
	}

	D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pDefender, STATE_BURNING);
	D2GameStrc* pGame = pAttacker->pGame;
	if (pDefender && pDefender->dwUnitType == UNIT_MONSTER)
	{
		D2GAME_EVENTS_Delete_6FC34840(pGame, pDefender, UNITEVENTCALLBACK_STATREGEN, 0);
		EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_STATREGEN, pGame->dwGameFrame + 1, 0, 0);
	}

	const int32_t nExpireFrame = nBurnLength + pGame->dwGameFrame;
	if (pStatList)
	{
		if (-STATLIST_GetStatValue(pStatList, STAT_HPREGEN, 0) <= nBurnDamage)
		{
			D2COMMON_10476(pStatList, nExpireFrame);
			EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);
			STATLIST_SetStatIfListIsValid(pStatList, STAT_HPREGEN, -nBurnDamage, 0);
		}
		return;
	}

	STATES_ToggleState(pDefender, STATE_BURNING, 1);

	int32_t nAttackerGUID = -1;
	int32_t nAttackerType = 6;
	if (pAttacker)
	{
		nAttackerGUID = pAttacker->dwUnitId;
		nAttackerType = pAttacker->dwUnitType;
	}

	D2StatListStrc* pNewStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nExpireFrame, nAttackerType, nAttackerGUID);
	EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);

	STATLIST_SetStatIfListIsValid(pNewStatList, STAT_HPREGEN, -nBurnDamage, 0);
	STATLIST_SetState(pNewStatList, STATE_BURNING);
	STATLIST_SetStatRemoveCallback(pNewStatList, sub_6FD10E50);
	D2COMMON_10475_PostStatToStatList(pDefender, pNewStatList, 1);
}

//D2Game.0x6FCC0940
void __fastcall SUNITDMG_ApplyColdState(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nColdLength)
{
	if (nColdLength <= 0)
	{
		return;
	}

	D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pDefender, STATE_COLD);
	D2GameStrc* pGame = pAttacker->pGame;

	const int32_t nColdEffect = SUNITDMG_GetColdEffect(pGame, pDefender);
	if (!nColdEffect)
	{
		return;
	}

	if (pDefender && pDefender->dwUnitType == UNIT_MONSTER && nColdEffect < 0)
	{
		const int32_t nColdDivisor = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty)->dwMonsterColdDiv;
		if (nColdDivisor)
		{
			nColdLength /= nColdDivisor;
		}
	}

	nColdLength = std::max(nColdLength, 1);

	const int32_t nExpireFrame = nColdLength + pGame->dwGameFrame;
	if (pStatList)
	{
		if (STATLIST_GetExpireFrame(pStatList) < nExpireFrame)
		{
			STATLIST_SetExpireFrame(pStatList, nExpireFrame);
			EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);
		}
	}
	else
	{
		STATES_ToggleState(pDefender, STATE_COLD, 1);

		int32_t nAttackerGUID = -1;
		int32_t nAttackerType = 6;
		if (pAttacker)
		{
			nAttackerGUID = pAttacker->dwUnitId;
			nAttackerType = pAttacker->dwUnitType;
		}

		D2StatListStrc* pNewStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nExpireFrame, nAttackerType, nAttackerGUID);
		EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);

		STATLIST_SetState(pNewStatList, STATE_COLD);
		STATLIST_SetStatRemoveCallback(pNewStatList, SUNITDMG_RemoveShatterState);
		D2COMMON_10475_PostStatToStatList(pDefender, pNewStatList, 1);
		STATLIST_SetStatIfListIsValid(pNewStatList, STAT_VELOCITYPERCENT, nColdEffect, 0);
		STATLIST_SetStatIfListIsValid(pNewStatList, STAT_ATTACKRATE, nColdEffect, 0);
		STATLIST_SetStatIfListIsValid(pNewStatList, STAT_OTHER_ANIMRATE, nColdEffect, 0);
		UNITS_UpdateAnimRateAndVelocity(pDefender, __FILE__, __LINE__);
	}

	if ((ITEMS_RollRandomNumber(&pDefender->pSeed) % 100) >= 20 && pDefender && pDefender->dwUnitType == UNIT_MONSTER)
	{
		D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pDefender->dwClassId);

		if (pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_DEADCOL])
		{
			STATES_ToggleState(pDefender, STATE_SHATTER, 1);
			return;
		}
	}

	STATES_ToggleState(pDefender, STATE_SHATTER, 0);
}

//D2Game.0x6FCC0B90
void __fastcall SUNITDMG_RemoveShatterState(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
	D2_MAYBE_UNUSED(pStatList);
	if (SUNIT_IsDead(pUnit) && STATES_CheckStateMaskStayDeathOnUnitByStateId(pUnit, nState))
	{
		return;
	}

	STATES_ToggleState(pUnit, nState, 0);
	if (!SUNIT_IsDead(pUnit))
	{
		STATES_ToggleState(pUnit, STATE_SHATTER, 0);
	}

	UNITS_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
}

//D2Game.0x6FCC0BE0
void __fastcall SUNITDMG_ApplyFreezeState(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nFreezeLength)
{
	if (nFreezeLength <= 0)
	{
		return;
	}

	int32_t nDefenderType = 6;
	if (pDefender)
	{
		nDefenderType = pDefender->dwUnitType;
	}

	if (nDefenderType == UNIT_PLAYER)
	{
		SUNITDMG_ApplyColdState(pAttacker, pDefender, nFreezeLength);
		return;
	}
	else if (nDefenderType == UNIT_MONSTER)
	{
		if (STATES_CheckState(pDefender, STATE_UNINTERRUPTABLE))
		{
			return;
		}

		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pDefender->dwClassId);
		if (MONSTERS_IsBoss(pMonStatsTxtRecord, pDefender) || MONSTERUNIQUE_CheckMonTypeFlag(pDefender, 8u) || MONSTERS_GetHirelingTypeId(pDefender))
		{
			SUNITDMG_ApplyColdState(pAttacker, pDefender, nFreezeLength);
			return;
		}
	}

	D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pDefender, STATE_FREEZE);
	D2GameStrc* pGame = pAttacker->pGame;
	if (pDefender && pDefender->dwUnitType == UNIT_MONSTER)
	{
		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pDefender->dwClassId);
		if (pMonStatsTxtRecord && pMonStatsTxtRecord->nColdEffect[pGame->nDifficulty] >= 0)
		{
			return;
		}

		const int32_t nFreezeDivisor = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty)->dwMonsterFreezeDiv;
		D2_ASSERT(nFreezeDivisor);
		nFreezeLength /= nFreezeDivisor;
	}

	int32_t nExpireFrame = nFreezeLength + pGame->dwGameFrame;
	if (pStatList)
	{
		const int32_t nStatListExpireFrame = STATLIST_GetExpireFrame(pStatList);
		if (nStatListExpireFrame >= nExpireFrame)
		{
			nExpireFrame = nStatListExpireFrame;
		}
		else
		{
			STATLIST_SetExpireFrame(pStatList, nExpireFrame);
		}
	}
	else
	{
		STATES_ToggleState(pDefender, STATE_FREEZE, 1);

		int32_t nAttackerGUID = -1;
		int32_t nAttackerType = 6;
		if (pAttacker)
		{
			nAttackerGUID = pAttacker->dwUnitId;
			nAttackerType = pAttacker->dwUnitType;
		}

		D2StatListStrc* pNewStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, nExpireFrame, nAttackerType, nAttackerGUID);
		STATLIST_SetState(pNewStatList, 1u);
		STATLIST_SetStatRemoveCallback(pNewStatList, SUNITDMG_RemoveFreezeState);
		D2COMMON_10475_PostStatToStatList(pDefender, pNewStatList, 1);
	}

	EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_REMOVESTATE, nExpireFrame, 0, 0);

	if (!pDefender || pDefender->dwUnitType != UNIT_MONSTER)
	{
		FOG_DisplayWarning("UnitGetType (hTarget) == UNIT_MONSTER", __FILE__, __LINE__);
	}

	D2GAME_EVENTS_Delete_6FC34840(pGame, pDefender, UNITEVENTCALLBACK_AITHINK, 0);
	EVENT_SetEvent(pGame, pDefender, UNITEVENTCALLBACK_AITHINK, pGame->dwGameFrame + nFreezeLength + 1, 0, 0);
}

//D2Game.0x6FCC0E20
void __fastcall SUNITDMG_FreeAttackerCombatList(D2GameStrc* pGame, D2UnitStrc* pAttacker)
{
	D2CombatStrc* pPrevious = nullptr;
	D2CombatStrc* pNext = nullptr;

	for (D2CombatStrc* pCombat = pAttacker->pCombat; pCombat; pCombat = pNext)
	{
		pNext = pCombat->pNext;

		if (pCombat->dwAttackerType == pAttacker->dwUnitType && pCombat->dwAttackerId == pAttacker->dwUnitId)
		{
			if (pPrevious)
			{
				pPrevious->pNext = pNext;
			}
			else
			{
				pAttacker->pCombat = pNext;
			}

			D2_FREE_POOL(pGame->pMemoryPool, pCombat);
		}
		else
		{
			pPrevious = pCombat;
		}
	}
}

//D2Game.0x6FCC0E90
void __fastcall SUNITDMG_FreeAttackerDefenderCombatList(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender)
{
	if (!pAttacker || !pDefender)
	{
		return;
	}

	D2CombatStrc* pPrevious = nullptr;
	D2CombatStrc* pNext = nullptr;

	for (D2CombatStrc* pCombat = pAttacker->pCombat; pCombat; pCombat = pNext)
	{
		pNext = pCombat->pNext;

		if (pCombat->dwAttackerType == pAttacker->dwUnitType && pCombat->dwAttackerId == pAttacker->dwUnitId && pCombat->dwDefenderType == pDefender->dwUnitType && pCombat->dwDefenderId == pDefender->dwUnitId)
		{
			if (pPrevious)
			{
				pPrevious->pNext = pNext;
			}
			else
			{
				pAttacker->pCombat = pNext;
			}

			D2_FREE_POOL(pGame->pMemoryPool, pCombat);
		}
		else
		{
			pPrevious = pCombat;
		}
	}
}

//D2Game.0x6FCC0F10
void __fastcall SUNITDMG_KillMonster(D2GameStrc* pGame, D2UnitStrc* pDefender, D2UnitStrc* pAttacker, int32_t bPetKill)
{
	if (!pDefender)
	{
		return;
	}

	switch (pDefender->dwUnitType)
	{
	case UNIT_PLAYER:
	{
		if (pDefender->dwAnimMode == PLRMODE_DEATH || pDefender->dwAnimMode == PLRMODE_DEAD)
		{
			return;
		}
		break;
	}
	case UNIT_MONSTER:
	{
		if (pDefender->dwAnimMode == MONMODE_DEATH || pDefender->dwAnimMode == MONMODE_DEAD)
		{
			return;
		}

		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pDefender->dwClassId);
		if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_KILLABLE]))
		{
			return;
		}

		D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pDefender);
		if (bPetKill && pOwner && pOwner->dwUnitType == UNIT_PLAYER)
		{
			sub_6FC7D470(pGame, pOwner, pDefender);
		}
		break;
	}
	default:
		return;
	}

	if (pAttacker)
	{
		sub_6FC62780(pAttacker, pDefender, pGame);
		ARENA_ProcessKillEvent(pGame, pAttacker, pDefender);
	}

	if (pDefender->dwUnitType == UNIT_MONSTER)
	{
		D2ModeChangeStrc modeChange = {};
		MONSTERMODE_GetModeChangeInfo(pDefender, MONMODE_DEATH, &modeChange);

		if (pAttacker)
		{
			modeChange.unk0x14[0] = UNITS_GetDirectionToCoords(pDefender, CLIENTS_GetUnitX(pAttacker), CLIENTS_GetUnitY(pAttacker));
		}
		else
		{
			modeChange.unk0x14[0] = PATH_GetDirection(pDefender->pDynamicPath);
		}

		modeChange.pTargetUnit = pAttacker;
		D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);

		if (!(pDefender->dwFlags & UNITFLAG_ISREVIVE))
		{
			QUESTS_ParseKill(pGame, pDefender, pAttacker);
		}

		D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pDefender->dwClassId);
		if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_OBJCOL])
		{
			switch (pDefender->dwClassId)
			{
			case MONSTER_BARRICADEDOOR1:
			{
				D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pDefender);
				if (pRoom)
				{
					for (D2UnitStrc* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pRoomNext)
					{
						if (pUnit->dwUnitType == UNIT_OBJECT && pUnit->dwClassId == 571)
						{
							D2CoordStrc pCoords = {};
							UNITS_GetCoords(pUnit, &pCoords);
							D2CoordStrc pDefenderCoords = {};
							UNITS_GetCoords(pDefender, &pDefenderCoords);

							if (pCoords.nX == pDefenderCoords.nX && pCoords.nY == pDefenderCoords.nY)
							{
								UNITS_ChangeAnimMode(pUnit, OBJMODE_OPENED);
								UNITS_FreeCollisionPath(pUnit);
								return;
							}
						}
					}
				}
				break;
			}
			case MONSTER_BARRICADEDOOR2:
			{
				D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pDefender);
				if (pRoom)
				{
					for (D2UnitStrc* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pRoomNext)
					{
						if (pUnit->dwUnitType == UNIT_OBJECT && pUnit->dwClassId == 572)
						{
							D2CoordStrc pCoords = {};
							UNITS_GetCoords(pUnit, &pCoords);
							D2CoordStrc pDefenderCoords = {};
							UNITS_GetCoords(pDefender, &pDefenderCoords);

							if (pCoords.nX == pDefenderCoords.nX && pCoords.nY == pDefenderCoords.nY)
							{
								UNITS_ChangeAnimMode(pUnit, OBJMODE_OPENED);
								UNITS_FreeCollisionPath(pUnit);
								return;
							}
						}
					}
				}
				break;
			}
			}
		}
	}
}

//D2Game.0x6FCC1260
void __fastcall SUNITDMG_ExecuteMissileDamage(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
	D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
	if (pRoom && DUNGEON_IsRoomInTown(pRoom))
	{
		if (!pAttacker || pAttacker->dwUnitType != UNIT_MONSTER)
		{
			return;
		}

		D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pAttacker->dwClassId);
		if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTOWN]))
		{
			return;
		}
	}

	if (pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
	{
		if (!pDamage->nHitClassActiveSet && !(pDamage->dwHitClass & 0xF0))
		{
			pDamage->dwHitClass = SUNITDMG_GetHitClass(pDamage, pDamage->dwHitClass);
		}
		pUnit->dwLastHitClass = pDamage->dwHitClass;
	}

	if (STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
	{
		if (pDamage->wResultFlags & DAMAGERESULTFLAG_WILLDIE)
		{
			STATES_ToggleState(pUnit, STATE_DEATH_DELAY, 1);
		}
		return;
	}

	if (!pUnit || !pAttacker)
	{
		return;
	}

	if (pUnit->dwUnitType == UNIT_PLAYER)
	{
		int32_t nState = 0;
		if (pDamage->wResultFlags & DAMAGERESULTFLAG_DODGE)
		{
			nState = STATE_DODGE;
		}
		else if (pDamage->wResultFlags & DAMAGERESULTFLAG_AVOID)
		{
			nState = STATE_AVOID;
		}
		else if (pDamage->wResultFlags & DAMAGERESULTFLAG_EVADE)
		{
			D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_EVADE);
			if (pStatList)
			{
				const int32_t nSkillId = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);

				if (SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId))
				{
					D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
					if (pSkillsTxtRecord && pSkillsTxtRecord->wStSound > 0)
					{
						SUNIT_AttachSound(pUnit, 12, pUnit);
					}
				}
			}
			return;
		}
		else if (pDamage->wResultFlags & (DAMAGERESULTFLAG_BLOCK | DAMAGERESULTFLAG_WEAPONBLOCK))
		{
			if (!(pDamage->wResultFlags & DAMAGERESULTFLAG_SOFTHIT) && (int32_t)(pGame->dwGameFrame - STATLIST_UnitGetStatValue(pUnit, STAT_LASTBLOCKFRAME, 0)) > STATLIST_UnitGetStatValue(pUnit, STAT_ITEM_FASTERBLOCKRATE, 0) / 8 + 15)
			{
				sub_6FC817D0(pGame, pUnit, 0, 9, 0, 0, 0);
				STATLIST_SetUnitStat(pUnit, STAT_LASTBLOCKFRAME, pGame->dwGameFrame, 0);
			}
			return;
		}
		else if (pDamage->wResultFlags & DAMAGERESULTFLAG_WILLDIE)
		{
			if (pUnit && pUnit->dwAnimMode != PLRMODE_DEATH && pUnit->dwAnimMode != PLRMODE_DEAD)
			{
				D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pUnit, 0, 0, pAttacker->dwUnitType, pAttacker->dwUnitId, 0);
			}
			return;
		}
		else if (pDamage->wResultFlags & DAMAGERESULTFLAG_KNOCKBACK)
		{
			D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pUnit, 0, 19, pAttacker->dwUnitType, pAttacker->dwUnitId, 0);
			return;
		}
		else
		{
			if (pDamage->wResultFlags & DAMAGERESULTFLAG_GETHIT)
			{
				if (STATLIST_GetStatListFromUnitAndState(pUnit, STATE_STUNNED) || !sub_6FCC1870(pUnit, pDamage, pDamage->dwHitClass))
				{
					sub_6FC817D0(pGame, pUnit, nullptr, 4, pDamage->dwDmgTotal, 0, 0);
					return;
				}
			}
			else
			{
				if (!(pDamage->wResultFlags & DAMAGERESULTFLAG_SOFTHIT))
				{
					return;
				}
			}

			UNITROOM_RefreshUnit(pUnit);
			if (pUnit)
			{
				pUnit->dwFlags |= UNITFLAG_UPGRLIFENHITCLASS;
			}
			return;
		}

		D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nState);
		if (pStatList)
		{
			D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0));
			if (pSkill)
			{
				SKILLS_SetFlags(pSkill, SKILLS_GetFlags(pSkill) | 4);
				D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pUnit, pSkill, 13u, pAttacker->dwUnitType, pAttacker->dwUnitId, 0);
			}
		}
		return;
	}
	else if (pUnit->dwUnitType == UNIT_MONSTER)
	{
		if (pUnit->dwAnimMode == MONMODE_DEATH || pUnit->dwAnimMode == MONMODE_DEAD)
		{
			return;
		}

		if (pDamage->wResultFlags & DAMAGERESULTFLAG_KNOCKBACK)
		{
			D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
			if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_KNOCKBACK]))
			{
				pDamage->wResultFlags = pDamage->wResultFlags & (uint16_t)(~DAMAGERESULTFLAG_KNOCKBACK) | DAMAGERESULTFLAG_GETHIT;
			}
		}

		sub_6FC62DF0(pUnit, pDamage);

		if (pDamage->wResultFlags & DAMAGERESULTFLAG_WILLDIE)
		{
			SUNITDMG_KillMonster(pGame, pUnit, pAttacker, 1);
			return;
		}

		if (pDamage->wResultFlags & DAMAGERESULTFLAG_KNOCKBACK)
		{
			D2ModeChangeStrc modeChange = {};
			MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_KNOCKBACK, &modeChange);
			modeChange.pTargetUnit = pAttacker;
			D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
			return;
		}

		if (pDamage->wResultFlags & DAMAGERESULTFLAG_BLOCK)
		{
			if (!(pDamage->wResultFlags & DAMAGERESULTFLAG_SOFTHIT))
			{
				if (pUnit->dwClassId != MONSTER_DIABLO && pUnit->dwClassId != MONSTER_DIABLOCLONE)
				{
					D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
					if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_BLOCK])
					{
						D2ModeChangeStrc modeChange = {};
						MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_BLOCK, &modeChange);
						D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
						return;
					}
				}
			}

			MONSTER_SetAiState(pUnit, 19u);
			return;
		}

		if (pDamage->wResultFlags & DAMAGERESULTFLAG_GETHIT)
		{
			if (STATLIST_GetStatListFromUnitAndState(pUnit, STATE_STUNNED) || !sub_6FCC1870(pUnit, pDamage, pDamage->dwHitClass))
			{
				D2ModeChangeStrc modeChange = {};
				MONSTERMODE_GetModeChangeInfo(pUnit, MONMODE_GETHIT, &modeChange);
				D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);
				sub_6FC6E8A0(pGame, pUnit);
				return;
			}

			UNITROOM_RefreshUnit(pUnit);
			pUnit->dwFlags |= UNITFLAG_UPGRLIFENHITCLASS;
			MONSTER_SetAiState(pUnit, 19u);
			sub_6FC6E8A0(pGame, pUnit);
			return;
		}

		if (pDamage->wResultFlags & DAMAGERESULTFLAG_SOFTHIT)
		{
			UNITROOM_RefreshUnit(pUnit);
			pUnit->dwFlags |= UNITFLAG_UPGRLIFENHITCLASS;
			MONSTER_SetAiState(pUnit, 19u);
			sub_6FC6E8A0(pGame, pUnit);
			return;
		}

		if (pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT && pDamage->dwDmgTotal > 0)
		{
			const int32_t nHpDiff = std::abs((int32_t)STATLIST_UnitGetStatValue(pUnit, STAT_LAST_SENT_HP_PCT, 0) - sub_6FC62F50(pUnit));
			if (nHpDiff > 4)
			{
				UNITROOM_RefreshUnit(pUnit);
				pUnit->dwFlags |= UNITFLAG_UPGRLIFENHITCLASS;
				return;
			}
		}
	}
}

//D2Game.0x6FCC1870
int32_t __fastcall sub_6FCC1870(D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nHitClass)
{
	if (STATES_CheckState(pUnit, STATE_FREEZE))
	{
		return 1;
	}

	if (pDamage->dwPoisDamage && pDamage->dwPoisDamage == pDamage->dwDmgTotal)
	{
		return 1;
	}

	if (pDamage->dwDmgTotal < 256)
	{
		return 1;
	}

	const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);

	int32_t nDivisor = 0;
	switch (nHitClass)
	{
	case 2:
	case 6:
	case 10:
	case 11:
		nDivisor = 8;
		break;

	case 5:
		nDivisor = 64;
		break;

	case 4:
	case 8:
		nDivisor = 32;
		break;

	default:
		nDivisor = 16;
		break;
	}

	if (pDamage->dwDmgTotal < nMaxHp / nDivisor)
	{
		return 1;
	}

	if (pDamage->dwDmgTotal < nMaxHp / (nDivisor / 2) && !(ITEMS_RollRandomNumber(&pUnit->pSeed) & 1))
	{
		return 1;
	}

	if (pDamage->dwDmgTotal < nMaxHp / (nDivisor / 4) && !(ITEMS_RollRandomNumber(&pUnit->pSeed) & 3))
	{
		return 1;
	}

	if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
	{
		return 1;
	}

	D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
	if (!pMonStats2TxtRecord)
	{
		return 1;
	}

	return !(pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[MONMODE_GETHIT]);
}

//D2Game.0x6FCC1A50
int32_t __fastcall SUNITDMG_GetHitClass(D2DamageStrc* pDamage, uint32_t nBaseHitClass)
{
	constexpr D2DamageHitClassMappingStrc sgDamageHitClassMapping[] =
	{
		{ offsetof(D2DamageStrc, dwColdDamage), 0x30 },
		{ offsetof(D2DamageStrc, dwFireDamage), 0x20 },
		{ offsetof(D2DamageStrc, dwLtngDamage), 0x40 },
		{ offsetof(D2DamageStrc, dwPoisDamage), 0x50 },
		{ offsetof(D2DamageStrc, dwColdDamage), 0x30 },
		{ offsetof(D2DamageStrc, dwFireDamage), 0x20 },
		{ offsetof(D2DamageStrc, dwLtngDamage), 0x40 },
		{ offsetof(D2DamageStrc, dwPoisDamage), 0x50 },
	};

	static int32_t dword_6FD4DC44;

	int32_t nIndex = dword_6FD4DC44 % 4;
	const int32_t nEnd = nIndex + 4;

	++dword_6FD4DC44;

	uint32_t nHitClass = 0;
	while (nIndex < nEnd)
	{
		if (*(int32_t*)((char*)pDamage + sgDamageHitClassMapping[nIndex].nDamageStrcOffset) > 0)
		{
			nHitClass = sgDamageHitClassMapping[nIndex].nHitClass;
			break;
		}

		++nIndex;
	}

	if (!nHitClass)
	{
		if (pDamage->wResultFlags & DAMAGERESULTFLAG_CRITICALSTRIKE)
		{
			nHitClass = 0x10u;
		}
	}

	if (!nBaseHitClass)
	{
		nBaseHitClass = 0xDu;
	}

	return nBaseHitClass | nHitClass;
}

//D2Game.0x6FCC1AC0
void __fastcall SUNITDMG_DrainItemDurability(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nUnused)
{
	constexpr D2DurabilityLossStrc sgDurabilityLossWeights[7] =
	{
		{ BODYLOC_HEAD, 3 },
		{ BODYLOC_TORSO, 5 },
		{ BODYLOC_RARM, 4 },
		{ BODYLOC_LARM, 4 },
		{ BODYLOC_BELT, 2 },
		{ BODYLOC_FEET, 2 },
		{ BODYLOC_GLOVES, 2 },
	};

	D2DamageStrc* pDamage = SUNITDMG_GetDamageFromUnits(pAttacker, pDefender);

	if (!pDamage)
	{
		return;
	}

	D2DamageStrc damageCopy = {};
	memcpy(&damageCopy, pDamage, sizeof(damageCopy));

	if (!UNITS_IsInMeleeRange(pAttacker, pDefender, 2 * (pAttacker->dwUnitType == UNIT_PLAYER) + 1))
	{
		SUNITDMG_FreeAttackerDefenderCombatList(pGame, pAttacker, pDefender);
		return;
	}

	if (damageCopy.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
	{
		if (pAttacker->dwUnitType != UNIT_OBJECT && (pAttacker->dwUnitType == UNIT_PLAYER || pAttacker->dwUnitType == UNIT_MONSTER) && !pAttacker->dwAnimMode)
		{
			return;
		}

		damageCopy.dwHitFlags = DAMAGEHITFLAG_32;
		SUNITDMG_ExecuteEvents(pGame, pAttacker, pDefender, 0, &damageCopy);

		if (damageCopy.nOverlay > 0)
		{
			UNITS_SetOverlay(pDefender, damageCopy.nOverlay, 0);
		}

		if (!damageCopy.nHitClassActiveSet && !(damageCopy.dwHitClass & 0xF))
		{
			damageCopy.dwHitClass |= UNITS_GetHitClass(pAttacker);
		}

		if (pAttacker->dwUnitType == UNIT_PLAYER)
		{
			D2UnitStrc* pItem = sub_6FC7C7B0(pAttacker);
			if (pItem)
			{
				ITEMS_UpdateDurability(pGame, pAttacker, pItem);
			}
		}

		if (pDefender->dwUnitType == UNIT_PLAYER && pDefender->pInventory)
		{
			D2UnitStrc* pItems[7] = {};
			int32_t nTotalWeight = 0;
			for (int32_t i = 0; i < std::size(sgDurabilityLossWeights); ++i)
			{
				D2UnitStrc* pItem = INVENTORY_GetItemFromBodyLoc(pDefender->pInventory, sgDurabilityLossWeights[i].nBodyLoc);
				if (pItem && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_ARMOR))
				{
					pItems[i] = pItem;
					nTotalWeight += sgDurabilityLossWeights[i].nWeight;
				}
			}

			if (nTotalWeight > 0)
			{
				int32_t nIndex = ITEMS_RollLimitedRandomNumber(&pDefender->pSeed, std::size(sgDurabilityLossWeights));
				int32_t nWeight = ITEMS_RollLimitedRandomNumber(&pDefender->pSeed, nTotalWeight);
				do
				{
					if (pItems[nIndex])
					{
						if (nWeight < sgDurabilityLossWeights[nIndex].nWeight)
						{
							ITEMS_UpdateDurability(pGame, pDefender, pItems[nIndex]);
							break;
						}
						nWeight -= sgDurabilityLossWeights[nIndex].nWeight;
					}
					nIndex = (nIndex + 1) % std::size(sgDurabilityLossWeights);
				}
				while (nWeight >= 0);
			}
		}
	}

	SUNITEVENT_EventFunc_Handler(pGame, EVENT_DOMELEEATTACK, pAttacker, pDefender, &damageCopy);
	SUNITEVENT_EventFunc_Handler(pGame, EVENT_ATTACKEDINMELEE, pDefender, pAttacker, &damageCopy);

	if (damageCopy.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
	{
		if (pAttacker->dwUnitType == UNIT_OBJECT || pAttacker->dwUnitType != UNIT_PLAYER && pAttacker->dwUnitType != UNIT_MONSTER || pAttacker->dwAnimMode)
		{
			SKILLS_ApplyThornsDamage(pGame, pAttacker, pDefender, &damageCopy);
		}
	}

	SUNITDMG_ExecuteMissileDamage(pGame, pAttacker, pDefender, &damageCopy);
	SUNITDMG_FreeAttackerDefenderCombatList(pGame, pAttacker, pDefender);
}

//D2Game.0x6FCC1D70
D2DamageStrc* __fastcall SUNITDMG_GetDamageFromUnits(D2UnitStrc* pAttacker, D2UnitStrc* pDefender)
{
	if (!pAttacker || !pDefender)
	{
		return nullptr;
	}

	for (D2CombatStrc* pCombat = pAttacker->pCombat; pCombat; pCombat = pCombat->pNext)
	{
		if (pCombat->dwAttackerType == pAttacker->dwUnitType && pCombat->dwAttackerId == pAttacker->dwUnitId && pCombat->dwDefenderType == pDefender->dwUnitType && pCombat->dwDefenderId == pDefender->dwUnitId)
		{
			return &pCombat->tDamage;
		}
	}

	return nullptr;
}

//D2Game.0x6FCC1DC0
bool __stdcall D2Game_10033(D2UnitStrc* pUnit, int32_t* a2, int32_t* a3)
{
	*a2 = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, *a2);
	*a3 = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, *a3);

	return *a2 >= *a3;
}

//D2Game.0x6FCC1E70
int32_t __fastcall SUNITDMG_IsHitSuccessful(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nStatValue, int32_t bMissile)
{
	if (!pDefender || !pAttacker)
	{
		return 0;
	}

	int32_t nArmorClassStatId = 0;
	if (bMissile)
	{
		nArmorClassStatId = STAT_ARMORCLASS_VS_MISSILE;
	}
	else
	{
		nArmorClassStatId = STAT_ARMORCLASS_VS_HTH;
	}

	int32_t nTotalDefense = STATLIST_UnitGetStatValue(pDefender, nArmorClassStatId, 0) + UNITS_GetDefense(pDefender);

	const int32_t nDefenderLevel = STATLIST_UnitGetStatValue(pDefender, STAT_LEVEL, 0);
	const int32_t nAttackerLevel = STATLIST_UnitGetStatValue(pAttacker, STAT_LEVEL, 0);

	int32_t nAttackRate = 0;
	int32_t nToHitPercent = 0;
	if (pAttacker->dwUnitType != UNIT_PLAYER)
	{
		nAttackRate = nStatValue + 5 * STATLIST_UnitGetStatValue(pAttacker, STAT_DEXTERITY, 0) + STATLIST_UnitGetStatValue(pAttacker, STAT_TOHIT, 0);
		nToHitPercent = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_TOHIT_PERCENT, 0);
	}
	else
	{
		nAttackRate = UNITS_GetAttackRate(pAttacker);
		if (STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_IGNORETARGETAC, 0) && pDefender->dwUnitType == UNIT_MONSTER && !MONSTERUNIQUE_CheckMonTypeFlag(pDefender, MONTYPEFLAG_UNIQUE | MONTYPEFLAG_SUPERUNIQUE) && !MONSTERS_IsBoss(nullptr, pDefender) && !MONSTERS_GetHirelingTypeId(pDefender))
		{
			nTotalDefense = 0;
		}

		int32_t nFractionalTargetAc = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_FRACTIONALTARGETAC, 0);
		if (nFractionalTargetAc > 0)
		{
			if (pDefender->dwUnitType == UNIT_PLAYER || pDefender->dwUnitType == UNIT_MONSTER && (MONSTERS_IsBoss(0, pDefender) || MONSTERUNIQUE_CheckMonTypeFlag(pDefender, MONTYPEFLAG_SUPERUNIQUE) || MONSTERS_GetHirelingTypeId(pDefender)))
			{
				nFractionalTargetAc /= 2;
			}

			nFractionalTargetAc = D2Clamp(nFractionalTargetAc, 0, 100);
			nTotalDefense -= MONSTERUNIQUE_CalculatePercentage(nTotalDefense, nFractionalTargetAc, 100);
		}

		const int32_t nDemonToHit = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_DEMON_TOHIT, 0);
		if (nDemonToHit && MONSTERS_IsDemon(pDefender))
		{
			nAttackRate += nDemonToHit;
		}

		const int32_t nUndeadToHit = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_UNDEAD_TOHIT, 0);
		if (nUndeadToHit && MONSTERS_IsUndead(pDefender))
		{
			nAttackRate += nUndeadToHit;
		}

		if (!bMissile)
		{
			D2UnitStrc* pWeapon = sub_6FC7C7B0(pAttacker);
			if (pWeapon)
			{
				nToHitPercent = SKILLS_GetWeaponMasteryBonus(pAttacker, pWeapon, 0, 0);
			}
		}

		nToHitPercent += nStatValue + STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_TOHIT_PERCENT, 0);

		if (pDefender->dwUnitType == UNIT_MONSTER)
		{
			D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pDefender->dwClassId);
			if (pMonStatsTxtRecord)
			{
				const int32_t nMonType = pMonStatsTxtRecord->wMonType;
				if (nMonType > 0)
				{
					D2StatStrc stats[128] = {};
					const int32_t nStats = STATLIST_CopyStats(pAttacker, STAT_ATTACK_VS_MONTYPE, stats, std::size(stats));
					for (int32_t i = 0; i < nStats; ++i)
					{
						if (SUNITDMG_CheckMonType(stats[i].nLayer, nMonType))
						{
							nToHitPercent += stats[i].nValue;
						}
					}
				}
			}
		}
	}

	int32_t nDefense = nTotalDefense;
	int32_t nToHit = nAttackRate + MONSTERUNIQUE_CalculatePercentage(nAttackRate, nToHitPercent, 100);
	if (nTotalDefense < 0)
	{
		nToHit -= nTotalDefense;
		nDefense = 0;
	}

	if (nToHit < 0)
	{
		nDefense -= nToHit;
		nToHit = 0;
	}

	nDefense = std::max(nDefense, 0);

	const int32_t nDivisor = nToHit + nDefense;
	int32_t nToHitFactor = 100;
	if (nDivisor)
	{
		nToHitFactor = 100 * nToHit / nDivisor;
	}

	const int32_t nChanceToHit = D2Clamp(2 * nAttackerLevel * nToHitFactor / (nDefenderLevel + nAttackerLevel), 5, 95);
	const int32_t nRand = ITEMS_RollRandomNumber(&pAttacker->pSeed) % 100;
	//SUNIT_GetGameFromUnit(pAttacker);
	//D2GAME_Return_6FCCD420(pDefender, nChanceToHit, nRand);
	if (nRand >= nChanceToHit)
	{
		return 0;
	}

	if (pAttacker->dwUnitType == UNIT_PLAYER && pDefender->dwUnitType == UNIT_MONSTER && STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_PREVENTHEAL, 0))
	{
		D2CurseStrc curse = {};
		curse.pUnit = pAttacker;
		curse.pTarget = pDefender;
		curse.nSkill = 0;
		curse.nSkillLevel = 1;
		curse.nDuration = 120000;
		curse.nStat = STAT_ARMORCLASS;
		curse.nStatValue = 0;
		curse.nState = STATE_PREVENTHEAL;
		sub_6FD10EC0(&curse);
	}

	return 1;
}

//D2Game.0x6FCC2300
uint16_t __fastcall SUNITDMG_GetResultFlags(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nStatValue, int32_t nRangeOffset)
{
	if (!pAttacker || !pDefender || pAttacker->dwUnitType != UNIT_PLAYER && pAttacker->dwUnitType != UNIT_MONSTER || pDefender->dwUnitType != UNIT_PLAYER && pDefender->dwUnitType != UNIT_MONSTER || !sub_6FCBD900(pGame, pAttacker, pDefender))
	{
		return 0;
	}

	uint16_t wResultFlags = 0;
	if (pDefender->dwUnitType == UNIT_PLAYER)
	{
		if (pDefender->dwAnimMode == PLRMODE_RUN)
		{
			wResultFlags = DAMAGERESULTFLAG_SUCCESSFULHIT;
		}
	}
	//else
	//{
	//	if (pDefender->dwUnitType == UNIT_MONSTER && !D2GAME_IsMonster_6FC62770(pDefender)) // TODO: This check can never succeed
	//	{
	//		return 0;
	//	}
	//}

	int32_t nRange = 2 * (pAttacker->dwUnitType == UNIT_MONSTER) + 1;
	if (nRangeOffset)
	{
		nRange -= nRangeOffset + UNITS_GetMeleeRange(pAttacker);
	}

	if (!UNITS_IsInMeleeRange(pAttacker, pDefender, nRange))
	{
		return 0;
	}

	if (!(wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT) && SUNITDMG_IsHitSuccessful(pAttacker, pDefender, nStatValue, 0))
	{
		wResultFlags |= DAMAGERESULTFLAG_SUCCESSFULHIT;
	}

	if (wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
	{
		const int32_t nActionFlags = SUNITDMG_ApplyBlockOrDodge(pGame, pAttacker, pDefender, 0, 1);
		if (nActionFlags & BLOCKFLAG_AVOID)
		{
			wResultFlags |= DAMAGERESULTFLAG_AVOID;
		}

		if (nActionFlags & BLOCKFLAG_DODGE)
		{
			wResultFlags |= DAMAGERESULTFLAG_DODGE;
		}

		if (nActionFlags & BLOCKFLAG_WEAPONBLOCK)
		{
			wResultFlags |= DAMAGERESULTFLAG_WEAPONBLOCK;
		}

		if (nActionFlags & BLOCKFLAG_BLOCK)
		{
			wResultFlags |= DAMAGERESULTFLAG_BLOCK;
		}

		if (nActionFlags)
		{
			wResultFlags &= (uint16_t)(~DAMAGERESULTFLAG_SUCCESSFULHIT);
		}
	}

	if (wResultFlags & DAMAGERESULTFLAG_BLOCK)
	{
		wResultFlags &= (uint16_t)(~DAMAGERESULTFLAG_SUCCESSFULHIT);
	}

	if (wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT && !STATES_CheckState(pDefender, STATE_UNINTERRUPTABLE))
	{
		wResultFlags |= DAMAGERESULTFLAG_GETHIT;
	}

	return wResultFlags;
}

//D2Game.0x6FCC2420
void __fastcall SUNITDMG_AllocCombat(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, D2DamageStrc* pDamage, uint8_t nSrcDam)
{
	if (!pAttacker || !pDefender)
	{
		return;
	}

	if (!(pDamage->wResultFlags & (DAMAGERESULTFLAG_DODGE | DAMAGERESULTFLAG_AVOID | DAMAGERESULTFLAG_EVADE | DAMAGERESULTFLAG_WEAPONBLOCK)) && pDamage->wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
	{
		if (!(pDamage->dwHitFlags & DAMAGEHITFLAG_2))
		{
			SUNITDMG_FillDamageValues(pGame, pAttacker, pDefender, pDamage, 0, nSrcDam);
		}

		SUNITDMG_CalculateTotalDamage(pGame, pAttacker, pDefender, pDamage);

		int32_t nTotalDamage = pDamage->dwPhysDamage + pDamage->dwFireDamage + pDamage->dwLtngDamage + pDamage->dwMagDamage + pDamage->dwColdDamage + pDamage->dwPoisDamage;
		if (pAttacker->dwUnitType == UNIT_MONSTER)
		{
			nTotalDamage += pDamage->dwLifeLeech;
		}

		const int32_t nHitpoints = STATLIST_UnitGetStatValue(pDefender, STAT_HITPOINTS, 0);

		if ((nTotalDamage & 0xFFFFFF00) > (nHitpoints & 0xFFFFFF00))
		{
			pDamage->wResultFlags |= DAMAGERESULTFLAG_WILLDIE;
		}
	}

	if (pDefender->dwUnitType != UNIT_MONSTER && pDefender->dwUnitType != UNIT_PLAYER)
	{
		return;
	}

	D2CombatStrc* pCombat = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2CombatStrc);
	pCombat->pGame = pGame;
	pCombat->dwAttackerType = pAttacker->dwUnitType;
	pCombat->dwAttackerId = pAttacker->dwUnitId;
	pCombat->dwDefenderType = pDefender->dwUnitType;
	pCombat->dwDefenderId = pDefender->dwUnitId;
	memcpy(&pCombat->tDamage, pDamage, sizeof(pCombat->tDamage));
	pCombat->pNext = pAttacker->pCombat;
	pAttacker->pCombat = pCombat;
}

//D2Game.0x6FCC2530
int32_t __fastcall SUNITDMG_ApplyBlockOrDodge(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t bAvoid, int32_t bBlock)
{
	if (!bBlock)
	{
		return SUNITDMG_ApplyDodge(pAttacker, pDefender, bAvoid);
	}

	int32_t nBlockChance = UNITS_GetBlockRate(pDefender, pGame->bExpansion);
	if (nBlockChance <= 0)
	{
		return SUNITDMG_ApplyDodge(pAttacker, pDefender, bAvoid);
	}

	if (pDefender && pDefender->dwUnitType == UNIT_PLAYER && UNITS_IsInMovingMode(pDefender) && pDefender->dwAnimMode != PLRMODE_WALK)
	{
		nBlockChance /= 3;
	}

	const int32_t nRand = ITEMS_RollRandomNumber(&pDefender->pSeed) % 100;
	//SUNIT_GetGameFromUnit(pAttacker);
	//D2GAME_Return_6FCCD430(pDefender, nBlockChance, nRand, "blocks");
	if (nRand >= nBlockChance)
	{
		return SUNITDMG_ApplyDodge(pAttacker, pDefender, bAvoid);
	}

	return BLOCKFLAG_BLOCK;
}

//D2Game.0x6FCC2610
int32_t __fastcall SUNITDMG_ApplyDodge(D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t bAvoid)
{
	if (pDefender)
	{
		if (pDefender->dwUnitType == UNIT_PLAYER && (pDefender->dwAnimMode == PLRMODE_RUN || pDefender->dwAnimMode == PLRMODE_WALK)
			|| pDefender->dwUnitType == UNIT_MONSTER && (pDefender->dwAnimMode == MONMODE_RUN || pDefender->dwAnimMode == MONMODE_WALK))
		{
			const int32_t nEvade = STATLIST_UnitGetStatValue(pDefender, STAT_PASSIVE_EVADE, 0);
			if (nEvade > 0)
			{
				const int32_t nRand = ITEMS_RollRandomNumber(&pDefender->pSeed) % 100;
				//SUNIT_GetGameFromUnit(pAttacker);
				//D2GAME_Return_6FCCD430(pDefender, nEvade, nRand, "evades");
				if (nRand < nEvade)
				{
					return BLOCKFLAG_EVADE;
				}
			}

			return BLOCKFLAG_NONE;
		}
	}

	const int32_t nWeaponBlock = SUNITDMG_GetWeaponBlock(pDefender);
	if (nWeaponBlock > 0)
	{
		int32_t nWeaponClassId = 0;
		COMPOSIT_GetWeaponClassId(pDefender, pDefender->pInventory, &nWeaponClassId, -1, 1);
		if (nWeaponClassId == 13)
		{
			const int32_t nRand = ITEMS_RollRandomNumber(&pDefender->pSeed) % 100;
			//SUNIT_GetGameFromUnit(pAttacker);
			//D2GAME_Return_6FCCD430(pDefender, nWeaponBlock, nRand, "block");
			if (nRand < nWeaponBlock)
			{
				return BLOCKFLAG_WEAPONBLOCK;
			}
		}
	}

	if (bAvoid)
	{
		const int32_t nAvoid = STATLIST_UnitGetStatValue(pDefender, STAT_PASSIVE_AVOID, 0);
		if (nAvoid > 0)
		{
			const int32_t nRand = ITEMS_RollRandomNumber(&pDefender->pSeed) % 100;
			//SUNIT_GetGameFromUnit(pAttacker);
			//D2GAME_Return_6FCCD430(pDefender, nAvoid, nRand, "avoids");
			if (nRand < nAvoid)
			{
				return BLOCKFLAG_AVOID;
			}
		}
	}
	else
	{
		const int32_t nDodge = STATLIST_UnitGetStatValue(pDefender, STAT_PASSIVE_DODGE, 0);
		if (nDodge > 0)
		{
			const int32_t nRand = ITEMS_RollRandomNumber(&pDefender->pSeed) % 100;
			//SUNIT_GetGameFromUnit(pAttacker);
			//D2GAME_Return_6FCCD430(pDefender, nDodge, nRand, "dodges");
			if (nRand < nDodge)
			{
				return BLOCKFLAG_DODGE;
			}
		}
	}

	return BLOCKFLAG_NONE;
}

//D2Game.0x6FCC2850
int32_t __fastcall SUNITDMG_GetWeaponBlock(D2UnitStrc* pUnit)
{
	if (!pUnit)
	{
		return 0;
	}

	D2UnitStrc* pRightHandItem = nullptr;
	D2UnitStrc* pLeftHandItem = nullptr;
	if (pUnit->pInventory)
	{
		pRightHandItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
		pLeftHandItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
	}

	D2StatStrc stats[32] = {};
	const int32_t nStats = STATLIST_CopyStats(pUnit, STAT_PASSIVE_WEAPONBLOCK, stats, std::size(stats));
	int32_t nMax = 0;
	for (int32_t i = 0; i < nStats; ++i)
	{
		const int32_t nValue = stats[i].nValue;
		const int32_t nItemType = stats[i].nLayer;
		if (nItemType <= 0 || (pLeftHandItem && ITEMS_CheckItemTypeId(pLeftHandItem, nItemType) || pRightHandItem && ITEMS_CheckItemTypeId(pRightHandItem, nItemType)) && nValue > nMax)
		{
			nMax = nValue;
		}
	}

	return nMax;
}

//D2Game.0x6FCC2910
int32_t __fastcall SUNITDMG_SetMissileDamageFlagsForNearbyUnits(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nX, int32_t nY, int32_t nSize, D2DamageStrc* pDamage, int32_t a7, int32_t a8, int32_t(__fastcall* pfCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*), int32_t a10)
{
	D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);

	int32_t nFlags = a10;
	if (!a10)
	{
		nFlags = 0x583;
	}

	D2UnitFindArgStrc unitFindArg = {};
	unitFindArg.nFlags = nFlags;
	unitFindArg.nSize = nSize;
	unitFindArg.nX = nX;
	unitFindArg.nY = nY;

	D2UnitFindDataStrc unitFindData = {};
	UNITFINDS_InitializeUnitFindData(pGame->pMemoryPool, &unitFindData, pRoom, nX, nY, nSize, nullptr, &unitFindArg);
	UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(&unitFindData);

	D2UnitStrc* pUnitOwner = SUNIT_GetOwner(pGame, pUnit);
	if (!pUnitOwner)
	{
		UNITFINDS_FreeUnitFindData(&unitFindData);
		return 0;
	}

	D2UnitStrc* pOwnerOwner = SUNIT_GetOwner(pGame, pUnitOwner);

	D2DamageStrc damageCopy = {};
	int32_t bResult = 0;
	for (int32_t i = 0; i < unitFindData.nIndex; ++i)
	{
		D2UnitStrc* pFoundUnit = unitFindData.pUnitsArray[i];

		if ((pOwnerOwner != pFoundUnit || a8) && (pUnitOwner != pFoundUnit || a7) && sub_6FCBD900(pGame, pUnitOwner, pFoundUnit) && D2Common_11026(nX, nY, pFoundUnit, 0x805u))
		{
			memcpy(&damageCopy, pDamage, sizeof(damageCopy));

			if (!pfCallback)
			{
				MISSMODE_SetDamageFlags(pGame, pUnit, pFoundUnit, &damageCopy);
				bResult = 1;
			}
			else
			{
				if (IsBadCodePtr((FARPROC)pfCallback))
				{
					FOG_DisplayAssert("pfnCallBack", __FILE__, __LINE__);
					exit(-1);
				}

				if (pfCallback(pGame, pUnit, pFoundUnit))
				{
					MISSMODE_SetDamageFlags(pGame, pUnit, pFoundUnit, &damageCopy);
					bResult = 1;
				}
			}
		}
	}

	UNITFINDS_FreeUnitFindData(&unitFindData);
	return bResult;
}

//D2Game.0x6FCC2BC0
void __fastcall SUNITDMG_RollDamage(D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel, D2DamageStrc* pDamage)
{
	D2GAME_RollPhysicalDamage_6FD14EC0(pUnit, pDamage, nSkillId, nSkillLevel);

	D2GAME_RollElementalDamage_6FD14DD0(pUnit, pDamage, nSkillId, nSkillLevel);
}

//D2Game.0x6FCC2BF0
void __fastcall SUNITDMG_RollSuckBloodDamage(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender, int32_t nSkillId, int32_t nSkillLevel, D2DamageStrc* pDamage)
{
    if (SKILLS_GetSkillsTxtRecord(nSkillId))
    {
        pDamage->dwPhysDamage = SUNITDMG_ApplyDamageBonuses(pAttacker, 0, nullptr, SKILLS_GetMinPhysDamage(pAttacker, nSkillId, nSkillLevel, 0), SKILLS_GetMaxPhysDamage(pAttacker, nSkillId, nSkillLevel, 0), 0, 0, 128);
        D2GAME_RollElementalDamage_6FD14DD0(pAttacker, pDamage, nSkillId, nSkillLevel);
    }
}

//D2Game.0x6FCC2C70
void __fastcall SUNITDMG_DistributeExperience(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender)
{
	D2UnitStrc* pPlayer = pAttacker;
	if (!pAttacker || !pDefender || pAttacker->dwUnitType != UNIT_PLAYER && pAttacker->dwUnitType != UNIT_MONSTER)
	{
		return;
	}

	const uint32_t nDefenderExperience = STATLIST_GetUnitBaseStat(pDefender, STAT_EXPERIENCE, 0);
	if (nDefenderExperience <= 0)
	{
		return;
	}

	if (pAttacker->dwUnitType != UNIT_PLAYER)
	{
		if (pAttacker->dwUnitType != UNIT_MONSTER)
		{
			return;
		}

		D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pAttacker);
		
		D2StatListStrc* pStatList = nullptr;
		if (STATES_CheckStateMaskExpOnUnit(pAttacker))
		{
			pStatList = STATLIST_GetStatListFromUnitAndFlag(pAttacker, 2048);
		}

		if (STATES_CheckStateMaskExpOnUnit(pDefender))
		{
			pStatList = STATLIST_GetStatListFromUnitAndFlag(pDefender, 2048);
		}

		if (pStatList)
		{
			pOwner = SUNIT_GetServerUnit(pGame, STATLIST_GetOwnerType(pStatList), STATLIST_GetOwnerGUID(pStatList));
		}

		if (!pOwner || pOwner->dwUnitType != UNIT_PLAYER)
		{
			return;
		}

		pPlayer = pOwner;
	}

	if (!pPlayer)
	{
		return;
	}

	const uint32_t nDefenderLevel = STATLIST_GetUnitBaseStat(pDefender, STAT_LEVEL, 0);
	D2UnitStrc* pPet = sub_6FC7E8B0(pGame, pPlayer, 7, 0);
	if (pPet)
	{
		const uint32_t nPetLevel = STATLIST_GetUnitBaseStat(pPet, STAT_LEVEL, 0);
		uint32_t nExperienceBonus = SUNITDMG_ComputeExperienceGain(pGame, pPet, nPetLevel, nDefenderLevel, nDefenderExperience);
		if (pAttacker != pPet)
		{
			nExperienceBonus = 86 * nExperienceBonus / 256;
		}

		SUNITDMG_AddExperienceForHireling(pGame, pPlayer, pPet, nPetLevel, nExperienceBonus);
	}

	const uint32_t nPlayerLevel = STATLIST_GetUnitBaseStat(pPlayer, STAT_LEVEL, 0);
	if (SUNIT_GetPartyId(pPlayer) == -1)
	{
		SUNITDMG_AddExperienceForPlayer(pGame, pPlayer, nPlayerLevel, SUNITDMG_ComputeExperienceGain(pGame, pPlayer, nPlayerLevel, nDefenderLevel, nDefenderExperience));
		return;
	}

	D2PartyExpStrc partyExp = {};
	partyExp.pDefenderUnit = pDefender;
	PARTY_IteratePartyMembersInSameLevel(pGame, pPlayer, SUNITDMG_PartyCallback_ComputePartyExperience, &partyExp);

	if (partyExp.nMembers <= 0 || partyExp.nLevelSum <= 0)
	{
		return;
	}

	if (partyExp.nMembers == 1)
	{
		SUNITDMG_AddExperienceForPlayer(pGame, pPlayer, nPlayerLevel, SUNITDMG_ComputeExperienceGain(pGame, pPlayer, nPlayerLevel, nDefenderLevel, nDefenderExperience));
		return;
	}

	const uint32_t nExperience = nDefenderExperience + 89 * nDefenderExperience * (partyExp.nMembers - 1) / 256;
	const float multiplier = (float)nExperience / (float)partyExp.nLevelSum;
	for (int32_t i = 0; i < partyExp.nMembers; ++i)
	{
		const uint32_t nExperienceGained = SUNITDMG_ComputeExperienceGain(pGame, partyExp.pMembers[i], partyExp.nMemberLevels[i], nDefenderLevel, (uint64_t)((double)partyExp.nMemberLevels[i] * multiplier));
		SUNITDMG_AddExperienceForPlayer(pGame, partyExp.pMembers[i], partyExp.nMemberLevels[i], nExperienceGained);
	}
}

//D2Game.0x6FCC2EC0
uint32_t __fastcall SUNITDMG_ComputeExperienceGain(D2GameStrc* pGame, D2UnitStrc* pAttacker, uint32_t nAttackerLevel, uint32_t nDefenderLevel, uint32_t nDefenderExperience)
{
	if (nDefenderExperience <= 0)
	{
		return 1;
	}

	nDefenderExperience = std::min(nDefenderExperience, (uint32_t)(INT_MAX >> 8));

	int32_t nClassId = 0;
	if (pAttacker && pAttacker->dwUnitType == UNIT_PLAYER)
	{
		nClassId = pAttacker->dwClassId;
	}

	if (nAttackerLevel >= DATATBLS_GetMaxLevel(nClassId))
	{
		return 0;
	}

	uint32_t nResult = nDefenderExperience;
	if (nDefenderLevel <= nAttackerLevel)
	{
		constexpr int32_t experienceFactors[] =
		{
			256, 256, 256, 256, 256, 256, 207, 159, 110, 61, 13
		};

		const uint32_t nIndex = std::min(nAttackerLevel - nDefenderLevel, std::size(experienceFactors) - 1);
		const int32_t nFactor = experienceFactors[nIndex];
		if (nFactor != 256)
		{
			nResult = MONSTERUNIQUE_CalculatePercentage(nDefenderExperience, nFactor, 256);
		}
	}
	else if (nAttackerLevel < 25 || nDefenderLevel <= 0)
	{
		constexpr int32_t experienceFactors[] =
		{
			256, 256, 256, 256, 256, 256, 225, 174, 92, 38, 5
		};

		const uint32_t nIndex = std::min(nDefenderLevel - nAttackerLevel, std::size(experienceFactors) - 1);
		const int32_t nFactor = experienceFactors[nIndex];
		if (nFactor != 256)
		{
			nResult = MONSTERUNIQUE_CalculatePercentage(nDefenderExperience, nFactor, 256);
		}
	}
	else
	{
		nResult = MONSTERUNIQUE_CalculatePercentage(nDefenderExperience, nAttackerLevel, nDefenderLevel);
	}

	if (nResult > 0)
	{
		const int32_t nRatio1 = DATATBLS_GetExpRatio(nAttackerLevel);
		const int32_t nRatio2 = DATATBLS_GetExpRatio(0);
		if (nRatio2 > 0 && nRatio2 < 32)
		{
			if (nResult <= INT_MAX >> (nRatio2 + (nRatio1 >> nRatio2)))
			{
				nResult = nResult * nRatio1 >> nRatio2;
			}
			else
			{
				nResult = nRatio1 * (nResult >> nRatio2);
			}
		}
	}

	const int32_t nAddExpPct = STATLIST_UnitGetStatValue(pAttacker, STAT_ITEM_ADDEXPERIENCE, 0);
	if (nAddExpPct)
	{
		nResult += MONSTERUNIQUE_CalculatePercentage(nResult, nAddExpPct, 100);
	}

	if (pAttacker && pAttacker->dwUnitType == UNIT_PLAYER)
	{
		return nResult;
	}

	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pAttacker);
	if (!pOwner || pOwner->dwUnitType != UNIT_PLAYER)
	{
		return 0;
	}

	D2PetInfoStrc* pPetInfo = PLAYERPETS_GetPetInfoFromPetGUID(pOwner, pAttacker ? pAttacker->dwUnitId : -1);
	if (!pPetInfo)
	{
		return 0;
	}

	D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, pPetInfo->nHirelingId, nAttackerLevel);
	if (!pHirelingTxtRecord)
	{
		return 0;
	}

	const uint32_t nMaxExp = (uint32_t)(MONSTERS_GetHirelingExpForNextLevel(nAttackerLevel + 1, pHirelingTxtRecord->dwExpPerLvl) - MONSTERS_GetHirelingExpForNextLevel(nAttackerLevel, pHirelingTxtRecord->dwExpPerLvl)) >> 6;
	return std::min(nResult, nMaxExp);
}

//D2Game.0x6FCC3170
void __fastcall SUNITDMG_AddExperienceForPlayer(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t nOldLevel, uint32_t nExperienceGained)
{
	if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER)
	{
		return;
	}

	const uint32_t nExperience = STATLIST_GetUnitBaseStat(pUnit, STAT_EXPERIENCE, 0);
	const uint32_t nMaxLevel = DATATBLS_GetMaxLevel(pUnit->dwClassId);
	const uint32_t nMaxExperience = DATATBLS_GetLevelThreshold(pUnit->dwClassId, nMaxLevel - 1);
	const uint32_t nNewExperience = std::min(nExperienceGained + nExperience, nMaxExperience);

	STATLIST_SetUnitStat(pUnit, STAT_LASTEXP, nNewExperience - nExperience, 0);
	STATLIST_SetUnitStat(pUnit, STAT_EXPERIENCE, nNewExperience, 0);

	if (nOldLevel != DATATBLS_GetCurrentLevelFromExp(pUnit->dwClassId, nNewExperience))
	{
		PLAYERSTATS_LevelUp(pGame, pUnit);
		SUNITEVENT_EventFunc_Handler(pGame, EVENT_LEVELUP, pUnit, 0, 0);
	}
}

//D2Game.0x6FCC3200
void __fastcall SUNITDMG_PartyCallback_ComputePartyExperience(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pArg)
{
	D2PartyExpStrc* pPartyExp = (D2PartyExpStrc*)pArg;
	if (SUNIT_IsDead(pUnit))
	{
		return;
	}

	if (pPartyExp->pDefenderUnit)
	{
		const int32_t nX1 = CLIENTS_GetUnitX(pPartyExp->pDefenderUnit);
		const int32_t nY1 = CLIENTS_GetUnitY(pPartyExp->pDefenderUnit);

		if (!nX1)
		{
			return;
		}

		const int32_t nX2 = CLIENTS_GetUnitX(pUnit);
		const int32_t nY2 = CLIENTS_GetUnitY(pUnit);

		if (!nX2)
		{
			return;
		}

		if ((nX1 - nX2) * (nX1 - nX2) + (nY1 - nY2) * (nY1 - nY2) > 6400)
		{
			return;
		}
	}

	D2_ASSERT(pPartyExp->nMembers < 8);

	pPartyExp->pMembers[pPartyExp->nMembers] = pUnit;
	pPartyExp->nMemberLevels[pPartyExp->nMembers] = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
	pPartyExp->nLevelSum += pPartyExp->nMemberLevels[pPartyExp->nMembers];
	++pPartyExp->nMembers;
	return;
}

//D2Game.0x6FCC3360
void __fastcall SUNITDMG_AddExperienceForHireling(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pHireling, uint32_t nLevel, uint32_t nExperienceBonus)
{
	if (nExperienceBonus <= 0)
	{
		return;
	}

	D2PetInfoStrc* pPetInfo = PLAYERPETS_GetPetInfoFromPetGUID(pPlayer, pHireling ? pHireling->dwUnitId : -1);
	if (!pPetInfo)
	{
		return;
	}

	D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, pPetInfo->nHirelingId, nLevel);
	if (!pHirelingTxtRecord || nLevel >= STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0))
	{
		return;
	}

	const uint32_t nCurrentExp = STATLIST_GetUnitBaseStat(pHireling, STAT_EXPERIENCE, 0);
	const uint32_t nNewExp = nCurrentExp + nExperienceBonus;
	const uint32_t nMaxLevel = DATATBLS_GetMaxLevel(0) - 1;
	if (nLevel >= nMaxLevel)
	{
		return;
	}

	STATLIST_SetUnitStat(pHireling, STAT_EXPERIENCE, nNewExp, 0);
	D2GAME_PACKETS_SendPacket0xA0_A1_A2_6FC3D610(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), pHireling, 0xDu, nCurrentExp, nNewExp);

	uint32_t nNewLevel = nLevel;
	do
	{
		if (MONSTERS_GetHirelingExpForNextLevel(nNewLevel + 1, pHirelingTxtRecord->dwExpPerLvl) > nNewExp)
		{
			break;
		}

		++nNewLevel;
	}
	while (nNewLevel < nMaxLevel);

	if (nNewLevel > nLevel)
	{
		MONSTERAI_UpdateMercStatsAndSkills(pGame, pPlayer, pHireling, nNewLevel);
		MONSTERAI_SendMercStats(pGame, pPlayer, 0);
		SUNIT_AttachSound(pHireling, 0x5Bu, pPlayer);
		SUNITEVENT_EventFunc_Handler(pGame, EVENT_LEVELUP, pHireling, 0, 0);
	}
}

//D2Game.0x6FCC34A0
void __fastcall SUNITDMG_AddExperience(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t nExperienceBonus)
{
	if (!pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
	{
		return;
	}

	const int32_t nLevel = STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0);
	if (pUnit->dwUnitType == UNIT_PLAYER)
	{
		SUNITDMG_AddExperienceForPlayer(pGame, pUnit, nLevel, nExperienceBonus);
	}
	else
	{
		D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
		if (pOwner && pOwner->dwUnitType == UNIT_PLAYER)
		{
			SUNITDMG_AddExperienceForHireling(pGame, pOwner, pUnit, nLevel, nExperienceBonus);
		}
	}
}

//D2Game.0x6FCC3510
void __fastcall SUNITDMG_SetExperienceForTargetLevel(D2GameStrc* pGame, D2UnitStrc* pUnit, uint32_t nTargetLevel)
{
	if (!pUnit || (pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER))
	{
		return;
	}

	const uint32_t nLevel = STATLIST_GetUnitBaseStat(pUnit, STAT_LEVEL, 0);
	if (nTargetLevel <= nLevel)
	{
		return;
	}

	if (pUnit->dwUnitType == UNIT_PLAYER)
	{
		const uint32_t nExperience = STATLIST_GetUnitBaseStat(pUnit, STAT_EXPERIENCE, 0);
		const uint32_t nThreshold = DATATBLS_GetLevelThreshold(pUnit->dwClassId, nTargetLevel);
		if (nThreshold > nExperience)
		{
			SUNITDMG_AddExperienceForPlayer(pGame, pUnit, nLevel, nThreshold - nExperience);
		}
		return;
	}

	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
	if (!pOwner || pOwner->dwUnitType != UNIT_PLAYER || nLevel >= DATATBLS_GetMaxLevel(0))
	{
		return;
	}

	D2PetInfoStrc* pPetInfo = PLAYERPETS_GetPetInfoFromPetGUID(pOwner, pUnit->dwUnitId);
	if (!pPetInfo)
	{
		return;
	}

	D2HirelingTxt* pHirelingTxtRecord = DATATBLS_GetHirelingTxtRecordFromIdAndLevel(pGame->bExpansion, pPetInfo->nHirelingId, nLevel);
	if (!pHirelingTxtRecord)
	{
		return;
	}

	SUNITDMG_AddExperienceForHireling(pGame, pOwner, pUnit, nLevel, MONSTERS_GetHirelingExpForNextLevel(nTargetLevel, pHirelingTxtRecord->dwExpPerLvl) - STATLIST_GetUnitBaseStat(pUnit, STAT_EXPERIENCE, 0));
}
