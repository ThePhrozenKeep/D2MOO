#include "SKILLS/SkillNec.h"

#include <algorithm>

#include <D2BitManip.h>
#include <D2Math.h>

#include <D2Monsters.h>
#include <D2Dungeon.h>
#include <D2StatList.h>
#include <D2DataTbls.h>
#include <D2Skills.h>
#include <D2Items.h>
#include <D2States.h>
#include <Units/UnitRoom.h>
#include <D2Collision.h>
#include <DataTbls/MonsterIds.h>
#include <Path/PathMisc.h>
#include <DataTbls/SkillsIds.h>

#include <UselessOrdinals.h>

#include "AI/AiGeneral.h"
#include "AI/AiThink.h"
#include "AI/AiUtil.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "GAME/Targets.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MISSILES/MissMode.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterUnique.h"
#include "PLAYER/PlayerPets.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitDmg.h"
#include "UNIT/SUnitEvent.h"
#include "UNIT/SUnitInactive.h"
#include "UNIT/SUnitMsg.h"


#pragma pack(push, 1)
struct D2UnkNecSkillStrc2
{
    D2DamageStrc* pDamage;
    int32_t nX;
    int32_t nY;
    int32_t unk0x0C;
};

struct D2UnkNecSkillStrc3
{
    D2GameStrc* pGame;
    D2UnitStrc* pUnit;
    int32_t nSkillId;
    int32_t nSkillLevel;
    uint8_t nUnitType;
    uint8_t pad0x11[3];
    int32_t nUnitGUID;
    int32_t nLength;
};

struct D2NecroSkeletonComponentStrc
{
    uint8_t nHead;
    uint8_t nShield;
    uint8_t nTorso;
    uint8_t nSpecial1;
    uint8_t nSpecial2;
    uint8_t nLegs;
    uint8_t nRightArm;
    uint8_t nLeftArm;
    uint8_t nRightHand;
};
#pragma pack(pop)



//D2Game.0x6FD0AF30
int32_t __fastcall SKILLS_SrvSt15_RaiseSkeleton_Mage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(pTarget);
        if (!DUNGEON_IsRoomInTown(pRoom))
        {
            return D2COMMON_11017_CheckUnitIfConsumeable(pTarget, 0);
        }
    }

    return 0;
}

//D2Game.0x6FD0AF70
int32_t __fastcall SKILLS_SrvSt16_PoisonDagger(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(pTarget);
        if (!DUNGEON_IsRoomInTown(pRoom))
        {
            D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
            if (pSkillsTxtRecord)
            {
                D2DamageStrc damage = {};
                damage.wResultFlags = SUNITDMG_GetResultFlags(pGame, pUnit, pTarget, SKILLS_GetToHitFactor(pUnit, nSkillId, nSkillLevel), 0);
                if (damage.wResultFlags & DAMAGERESULTFLAG_SUCCESSFULHIT)
                {
                    damage.dwEnDmgPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
                    if (pSkillsTxtRecord->nEType)
                    {
                        damage.dwConvPct = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[3], nSkillId, nSkillLevel);
                        if (damage.dwConvPct > 0)
                        {
                            damage.nConvType = pSkillsTxtRecord->nEType;
                        }

                        D2GAME_RollElementalDamage_6FD14DD0(pUnit, &damage, nSkillId, nSkillLevel);
                    }
                }

                uint8_t nSrcDam = pSkillsTxtRecord->nSrcDam;
                if (!pSkillsTxtRecord->nSrcDam)
                {
                    nSrcDam = 0x80u;
                }

                SUNITDMG_AllocCombat(pGame, pUnit, pTarget, &damage, nSrcDam);
                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FD0B0B0
int32_t __fastcall SKILLS_SrvSt17_Poison_CorpseExplosion(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(pTarget);
        if (!DUNGEON_IsRoomInTown(pRoom))
        {
            return D2COMMON_11021(pTarget);
        }
    }

    return 0;
}

//D2Game.0x6FD0B0F0
int32_t __fastcall SKILLS_SrvSt19_BonePrison(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        return !DUNGEON_IsRoomInTown(UNITS_GetRoom(pTarget));
    }

    return 0;
}

//D2Game.0x6FD0B120
int32_t __fastcall SKILLS_SrvSt20_IronGolem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget && pTarget->dwUnitType == UNIT_ITEM && pTarget->dwAnimMode == IMODE_ONGROUND && ITEMS_IsMetalItem(pTarget->dwClassId) && ITEMS_CheckItemFlag(pTarget, 0x10u, __LINE__, __FILE__))
    {
        return !STATLIST_GetOwner(pTarget, 0);
    }

    return 0;
}

//D2Game.0x6FD0B190
int32_t __fastcall SKILLS_SrvSt21_Revive(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget && pTarget->dwUnitType == UNIT_MONSTER)
    {
        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pTarget->dwClassId);
        if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwFlags & gdwBitMasks[8])
        {
            if (SUNIT_IsDead(pTarget) && D2COMMON_11017_CheckUnitIfConsumeable(pTarget, 0))
            {
                return sub_6FD15190(pTarget, 7) != 0;
            }
        }
    }

    return 0;
}

//D2Game.0x6FD0B250
void __fastcall sub_6FD0B250(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    D2_MAYBE_UNUSED(pStatList);
    if (pUnit)
    {
        D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
        SUNITEVENT_FreeTimer(pGame, pUnit, 1, nState);

        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2AiControlStrc* pAiControl = nullptr;
            if (pUnit->pMonsterData)
            {
                pAiControl = pUnit->pMonsterData->pAiControl;
            }

            AITHINK_ExecuteAiFn(pGame, pUnit, pAiControl, 0);
        }

        sub_6FD10E50(pUnit, nState, pStatList);
    }
}

//D2Game.0x6FD0B2B0
int32_t __fastcall sub_6FD0B2B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget)
{
    if (!pTarget)
    {
        return 0;
    }

    if (pTarget->dwUnitType == UNIT_MONSTER)
    {
        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pTarget->dwClassId);
        if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwModeFlags & gdwBitMasks[2]))
        {
            return 0;
        }

        if (MONSTERUNIQUE_CheckMonTypeFlag(pTarget, 0x20u))
        {
            return 0;
        }
    }

    if ((pTarget->dwFlags & UNITFLAG_CANBEATTACKED) && (pTarget->dwFlags & UNITFLAG_ISVALIDTARGET) && (pTarget->dwFlags & UNITFLAG_TARGETABLE))
    {
        if (!SUNIT_IsDead(pTarget))
        {
            return sub_6FCBD900(pGame, pUnit, pTarget) != 0;
        }
    }

    return 0;
}

//D2Game.0x6FD0B3D0
int32_t __fastcall sub_6FD0B3D0(D2UnitStrc* pUnit, int32_t nStatId, int32_t nResist)
{
    switch (nStatId)
    {
    case STAT_DAMAGERESIST:
    case STAT_MAGICRESIST:
    case STAT_FIRERESIST:
    case STAT_LIGHTRESIST:
    case STAT_COLDRESIST:
    case STAT_POISONRESIST:
        if (nResist > 0 || pUnit && pUnit->dwUnitType == UNIT_PLAYER || MONSTERS_GetHirelingTypeId(pUnit) || STATLIST_GetUnitBaseStat(pUnit, nStatId, 0) < 100)
        {
            return nResist;
        }

        return nResist / 5;
    }

    return nResist;
}

//D2Game.0x6FD0B450
int32_t __fastcall sub_6FD0B450(D2UnitStrc* pUnit, void* pArgs)
{
    // TODO: v2
    D2UnkNecSkillStrc* v2 = (D2UnkNecSkillStrc*)pArgs;

    if (v2->unk0x08)
    {
        if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_NEUTRAL)
        {
            return 0;
        }

        int32_t nSpecialAiState = 0;
        switch (v2->nAuraTargetState)
        {
        case STATE_DIMVISION:
            nSpecialAiState = 10;
            break;

        case STATE_TERROR:
            nSpecialAiState = 11;
            break;

        case STATE_TAUNT:
            nSpecialAiState = 12;
            break;
        }

        if (!sub_6FD15190(pUnit, nSpecialAiState))
        {
            return 0;
        }
    }

    int32_t nValue = 0;
    if (v2->nAuraStat[0] >= 0)
    {
        nValue = v2->nAuraStatCalc[0];
        switch (v2->nAuraStat[0])
        {
        case STAT_DAMAGERESIST:
        case STAT_MAGICRESIST:
        case STAT_FIRERESIST:
        case STAT_LIGHTRESIST:
        case STAT_COLDRESIST:
        case STAT_POISONRESIST:
            if (nValue <= 0 && (!pUnit || pUnit->dwUnitType != UNIT_PLAYER) && !MONSTERS_GetHirelingTypeId(pUnit) && STATLIST_GetUnitBaseStat(pUnit, v2->nAuraStat[0], 0) >= 100)
            {
                nValue /= 5;
            }
            break;

        default:
            break;
        }

        if (!nValue)
        {
            return 0;
        }
    }

    if (!sub_6FD0B2B0(v2->pGame, v2->pUnit, pUnit))
    {
        return 0;
    }

    D2CurseStrc curse = {};

    curse.pUnit = v2->pUnit;
    curse.nDuration = v2->nDuration;
    curse.nSkill = v2->nSkillId;
    curse.nSkillLevel = v2->nSkillLevel;
    curse.pStateRemoveCallback = v2->unk0x08 ? sub_6FD0B250 : nullptr;
    curse.pTarget = pUnit;
    curse.nStat = v2->nAuraStat[0];
    curse.nStatValue = nValue;
    curse.nState = v2->nAuraTargetState;

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    if (!pStatList)
    {
        return 0;
    }

    if (v2->unk0x08)
    {
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(v2->nSkillId);
        if (pSkillsTxtRecord)
        {
            int32_t nSpecialAiState = 0;
            switch (pSkillsTxtRecord->wAuraTargetState)
            {
            case STATE_DIMVISION:
                nSpecialAiState = 10;
                break;

            case STATE_TERROR:
                nSpecialAiState = 11;
                break;

            case STATE_TAUNT:
                nSpecialAiState = 12;
                break;
            }

            AITHINK_ExecuteAiFn(v2->pGame, pUnit, AIGENERAL_GetAiControlFromUnit(pUnit), nSpecialAiState);
        }
    }

    for (int32_t i = 1; i < 6; ++i)
    {
        const int32_t nStatId = v2->nAuraStat[i];
        if (nStatId >= 0 && nStatId < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            int32_t nStatValue = v2->nAuraStatCalc[i];
            switch (nStatId)
            {
            case STAT_DAMAGERESIST:
            case STAT_MAGICRESIST:
            case STAT_FIRERESIST:
            case STAT_LIGHTRESIST:
            case STAT_COLDRESIST:
            case STAT_POISONRESIST:
                if (nStatValue <= 0 && (!pUnit || pUnit->dwUnitType != UNIT_PLAYER) && !MONSTERS_GetHirelingTypeId(pUnit) && STATLIST_GetUnitBaseStat(pUnit, nStatId, 0) >= 100)
                {
                    nStatValue /= 5;
                }
                break;

            default:
                break;
            }

            if (nStatValue)
            {
                STATLIST_SetStatIfListIsValid(pStatList, nStatId, nStatValue, 0);
            }
        }
    }

    if (v2->bUpdateAnimRate)
    {
        D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
    }

    if (v2->nAuraEvent[0] >= 0 && v2->nAuraEventFunc[0] > 0)
    {
        SUNITEVENT_FreeTimer(v2->pGame, pUnit, 1, v2->nAuraTargetState);
        
        for (int32_t i = 0; i < 3; ++i)
        {
            if (v2->nAuraEvent[0] >= 0 && v2->nAuraEventFunc[0] > 0)
            {
                sub_6FD156A0(v2->pGame, pUnit, v2->nAuraEvent[i], v2->nSkillId, v2->nSkillLevel, v2->nAuraEventFunc[i], 1, v2->nAuraTargetState);
            }
        }
    }

    return 1;
}

//D2Game.0x6FD0B790
int32_t __fastcall SKILLS_SrvDo030_Curse(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pSkillsTxtRecord->wAuraStat[0] < -1 || pSkillsTxtRecord->wAuraStat[0] >= sgptDataTables->nItemStatCostTxtRecordCount)
    {
        return 0;
    }

    if (pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    int32_t bIsDimVisionOrTerrorCurse = 0;
    if (pSkillsTxtRecord->wAuraTargetState == STATE_DIMVISION || pSkillsTxtRecord->wAuraTargetState == STATE_TERROR)
    {
        bIsDimVisionOrTerrorCurse = 1;
    }

    const int32_t nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    const int32_t nBaseLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);

    int32_t nLength = nBaseLength;
    if (bIsDimVisionOrTerrorCurse)
    {
        D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
        if (pDifficultyLevelsTxtRecord && pDifficultyLevelsTxtRecord->dwAiCurseDiv)
        {
            nLength = nBaseLength / pDifficultyLevelsTxtRecord->dwAiCurseDiv;
        }
    }

    // TODO: v29
    D2UnkNecSkillStrc v29 = {};
    v29.pUnit = pUnit;
    v29.pGame = pGame;
    v29.nSkillLevel = nSkillLevel;
    v29.unk0x08 = bIsDimVisionOrTerrorCurse;
    v29.nSkillId = nSkillId;
    v29.nDuration = nLength;
    v29.bUpdateAnimRate = 0;

    for (int32_t i = 0; i < 6; ++i)
    {
        D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pSkillsTxtRecord->wAuraStat[i]);
        if (!pItemStatCostTxtRecord)
        {
            v29.nAuraStat[i] = -1;
            break;
        }

        if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_UPDATEANIMRATE])
        {
            v29.bUpdateAnimRate = 1;
        }

        v29.nAuraStat[i] = pSkillsTxtRecord->wAuraStat[i];
        v29.nAuraStatCalc[i] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], nSkillId, nSkillLevel);
    }

    v29.nAuraTargetState = pSkillsTxtRecord->wAuraTargetState;

    for (int32_t i = 0; i < 3; ++i)
    {
        v29.nAuraEvent[i] = pSkillsTxtRecord->wAuraEvent[i];
        v29.nAuraEventFunc[i] = pSkillsTxtRecord->wAuraEventFunc[i];
    }

    return sub_6FD10360(pGame, pSkillsTxtRecord->dwAuraFilter, pUnit, nRange, sub_6FD0B450, &v29);
}

//D2Game.0x6FD0B9F0
void __fastcall sub_6FD0B9F0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pTarget, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(SKILL_DIMVISION);
    if (!pSkillsTxtRecord)
    {
        return;
    }

    int32_t nLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, SKILL_DIMVISION, nSkillLevel);
    
    D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
    if (pDifficultyLevelsTxtRecord && pDifficultyLevelsTxtRecord->dwAiCurseDiv)
    {
        nLength /= pDifficultyLevelsTxtRecord->dwAiCurseDiv;
    }

    D2UnkNecSkillStrc args = {};

    args.nDuration = nLength;
    args.pGame = pGame;
    args.pUnit = pUnit;
    args.unk0x08 = 1;
    args.nSkillId = SKILL_DIMVISION;
    args.nSkillLevel = nSkillLevel;
    args.bUpdateAnimRate = 0;

    for (int32_t i = 0; i < 6; ++i)
    {
        D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pSkillsTxtRecord->wAuraStat[i]);
        if (!pItemStatCostTxtRecord)
        {
            args.nAuraStat[i] = -1;
            break;
        }

        if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_UPDATEANIMRATE])
        {
            args.bUpdateAnimRate = 1;
        }

        args.nAuraStat[i] = pSkillsTxtRecord->wAuraStat[i];
        args.nAuraStatCalc[i] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], SKILL_DIMVISION, nSkillLevel);
    }

    args.nAuraTargetState = pSkillsTxtRecord->wAuraTargetState;
    sub_6FD0B450(pTarget, &args);
}

//D2Game.0x6FD0BB60
int32_t __fastcall SKILLS_SrvDo059_Attract(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTargetUnit || STATLIST_GetUnitAlignment(pTargetUnit) != UNIT_ALIGNMENT_EVIL || !sub_6FD15190(pTargetUnit, 19) || SUNIT_IsDead(pTargetUnit) || DUNGEON_IsRoomInTown(UNITS_GetRoom(pTargetUnit)) || !sub_6FCBD900(pGame, pUnit, pTargetUnit))
    {
        return 0;
    }

    if (pSkillsTxtRecord->wAuraStat[0] < -1 || pSkillsTxtRecord->wAuraStat[0] >= sgptDataTables->nItemStatCostTxtRecordCount || pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    sub_6FCBDD30(pTargetUnit, 1u, 1);
    sub_6FC40280(pGame, pTargetUnit, 0, 9);
    sub_6FD154D0(pTargetUnit);

    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    int32_t nLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    
    D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
    if (pDifficultyLevelsTxtRecord && pDifficultyLevelsTxtRecord->dwAiCurseDiv)
    {
        nLength /= pDifficultyLevelsTxtRecord->dwAiCurseDiv;
    }

    D2UnkNecSkillStrc3 arg = {};
    arg.pGame = pGame;
    arg.pUnit = pUnit;
    arg.nSkillId = nSkillId;
    arg.nSkillLevel = nSkillLevel;
    arg.nLength = nLength;
    arg.nUnitType = pTargetUnit->dwUnitType;
    arg.nUnitGUID = pTargetUnit->dwUnitId;
    sub_6FD10360(pGame, pSkillsTxtRecord->dwAuraFilter, pUnit, nAuraRange, sub_6FD0BDA0, &arg);

    D2CurseStrc curse = {};
    curse.pUnit = pUnit;
    curse.pTarget = pTargetUnit;
    curse.nSkill = nSkillId;
    curse.nSkillLevel = nSkillLevel;
    curse.nDuration = nLength;
    curse.nStat = -1;
    curse.nState = pSkillsTxtRecord->wAuraTargetState;
    curse.pStateRemoveCallback = sub_6FD0C2B0;
    sub_6FD10EC0(&curse);

    return 1;
}

//D2Game.0x6FD0BDA0
int32_t __fastcall sub_6FD0BDA0(D2UnitStrc* pUnit, void* pArg)
{
    D2UnkNecSkillStrc3* pParam = (D2UnkNecSkillStrc3*)pArg;

    if (pUnit && STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_EVIL && sub_6FD15190(pUnit, 19) && !SUNIT_IsDead(pUnit) && !DUNGEON_IsRoomInTown(UNITS_GetRoom(pUnit)) && sub_6FCBD900(pParam->pGame, pParam->pUnit, pUnit))
    {
        int32_t nParam = 1;
        if (pParam->nUnitType != UNIT_PLAYER)
        {
            nParam = 2;
        }

        sub_6FC61E30(pUnit, nParam, pParam->nUnitGUID);
        EVENT_SetEvent(pParam->pGame, pUnit, 10, pParam->nLength + pParam->pGame->dwGameFrame, 0, 0);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD0BE50
int32_t __fastcall SKILLS_SrvDo061_Confuse(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pSkillsTxtRecord->wAuraStat[0] < -1 || pSkillsTxtRecord->wAuraStat[0] >= sgptDataTables->nItemStatCostTxtRecordCount)
    {
        return 0;
    }

    if (pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount)
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    const int32_t nRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    const int32_t nBaseLength = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraLenCalc, nSkillId, nSkillLevel);
    
    int32_t nLength = nBaseLength;
    D2DifficultyLevelsTxt* pDifficultyLevelsTxtRecord = DATATBLS_GetDifficultyLevelsTxtRecord(pGame->nDifficulty);
    if (pDifficultyLevelsTxtRecord && pDifficultyLevelsTxtRecord->dwAiCurseDiv)
    {
        nLength = nBaseLength / pDifficultyLevelsTxtRecord->dwAiCurseDiv;
    }

    // TODO: v25
    D2UnkNecSkillStrc v25 = {};

    v25.nSkillLevel = nSkillLevel;
    v25.pGame = pGame;
    v25.pUnit = pUnit;
    v25.nSkillId = nSkillId;
    v25.nDuration = nLength;
    v25.bUpdateAnimRate = 0;

    for (int32_t i = 1; i < 6; ++i)
    {
        D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pSkillsTxtRecord->wAuraStat[i]);
        if (!pItemStatCostTxtRecord)
        {
            v25.nAuraStat[i] = -1;
            break;
        }

        if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_UPDATEANIMRATE])
        {
            v25.bUpdateAnimRate = 1;
        }

        v25.nAuraStat[i] = pSkillsTxtRecord->wAuraStat[i];
        v25.nAuraStatCalc[i] = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], nSkillId, nSkillLevel);
    }

    v25.nAuraTargetState = pSkillsTxtRecord->wAuraTargetState;

    return sub_6FD10360(pGame, pSkillsTxtRecord->dwAuraFilter, pUnit, nRange, sub_6FD0C060, &v25);
}

//D2Game.0x6FD0C060
int32_t __fastcall sub_6FD0C060(D2UnitStrc* pUnit, void* pArg)
{
    D2UnkNecSkillStrc* pParam = (D2UnkNecSkillStrc*)pArg;

    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER || STATLIST_GetUnitAlignment(pUnit) != UNIT_ALIGNMENT_EVIL || !sub_6FCBD900(pParam->pGame, pParam->pUnit, pUnit) || SUNIT_IsDead(pUnit) || !sub_6FD15190(pUnit, 11))
    {
        return 0;
    }

    int32_t nValue = pParam->nAuraStatCalc[0];
    int32_t nStatId = pParam->nAuraStat[0];
    
    switch (nStatId)
    {
    case STAT_DAMAGERESIST:
    case STAT_MAGICRESIST:
    case STAT_FIRERESIST:
    case STAT_LIGHTRESIST:
    case STAT_COLDRESIST:
    case STAT_POISONRESIST:
        if (nValue <= 0)
        {
            if (pUnit->dwUnitType != UNIT_PLAYER && !MONSTERS_GetHirelingTypeId(pUnit) && STATLIST_GetUnitBaseStat(pUnit, nStatId, 0) >= 100)
            {
                nValue /= 5;
            }

            if (!nValue)
            {
                nStatId = -1;
            }
        }
        break;

    default:
        if (!nValue)
        {
            nStatId = -1;
        }
        break;
    }

    D2CurseStrc curse = {};

    curse.pUnit = pParam->pUnit;
    curse.nSkill = pParam->nSkillId;
    curse.nSkillLevel = pParam->nSkillLevel;
    curse.nDuration = pParam->nDuration;
    curse.pTarget = pUnit;
    curse.nStat = nStatId;
    curse.nStatValue = nValue;
    curse.nState = pParam->nAuraTargetState;
    curse.pStateRemoveCallback = sub_6FD0C2B0;

    D2StatListStrc* pStatList = sub_6FD10EC0(&curse);
    if (!pStatList)
    {
        return 0;
    }

    for (int32_t i = 1; i < 6; ++i)
    {
        nStatId = pParam->nAuraStat[i];
        if (nStatId >= 0 && nStatId < sgptDataTables->nItemStatCostTxtRecordCount)
        {
            nValue = pParam->nAuraStatCalc[i];
            switch (nStatId)
            {
            case STAT_DAMAGERESIST:
            case STAT_MAGICRESIST:
            case STAT_FIRERESIST:
            case STAT_LIGHTRESIST:
            case STAT_COLDRESIST:
            case STAT_POISONRESIST:
                if (nValue <= 0 && pUnit->dwUnitType != UNIT_PLAYER && !MONSTERS_GetHirelingTypeId(pUnit) && STATLIST_GetUnitBaseStat(pUnit, nStatId, 0) >= 100)
                {
                    nValue /= 5;
                }
                break;
            default:
                break;
            }

            if (nValue)
            {
                STATLIST_SetStatIfListIsValid(pStatList, nStatId, nValue, 0);
            }
        }
    }

    if (pParam->bUpdateAnimRate)
    {
        D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
    }

    sub_6FCBDD30(pUnit, 1u, 1);
    sub_6FC40280(pParam->pGame, pUnit, 0, 9);
    sub_6FC61E30(pUnit, 3, 0);
    EVENT_SetEvent(pParam->pGame, pUnit, UNITEVENTCALLBACK_AIRESET, pParam->nDuration + pParam->pGame->dwGameFrame, 0, 0);

    return 1;
}

//D2Game.0x6FD0C2B0
void __fastcall sub_6FD0C2B0(D2UnitStrc* pUnit, int32_t nState, D2StatListStrc* pStatList)
{
    D2_MAYBE_UNUSED(pStatList);
    sub_6FCBDD30(pUnit, 0, 1);
    STATES_ToggleState(pUnit, nState, 0);
    D2GAME_TARGETS_Last_6FC40380(SUNIT_GetGameFromUnit(pUnit), pUnit);
}

//D2Game.0x6FD0C2E0
void __fastcall D2GAME_SetSummonResistance_6FD0C2E0(D2UnitStrc* pUnit, D2UnitStrc* pPet)
{
    const int32_t nSummonRes = STATLIST_UnitGetStatValue(pUnit, STAT_PASSIVE_SUMMON_RESIST, 0);
    if (nSummonRes)
    {
        int32_t nUnitGUID = -1;
        int32_t nUnitType = 6;
        void* pMemPool = nullptr;
        if (pPet)
        {
            nUnitGUID = pPet->dwUnitId;
            nUnitType = pPet->dwUnitType;
            pMemPool = pPet->pMemoryPool;
        }

        D2StatListStrc* pStatList = STATLIST_AllocStatList(pMemPool, 0, 0, nUnitType, nUnitGUID);
        if (pStatList)
        {
            D2COMMON_10475_PostStatToStatList(pPet, pStatList, 1);

            if ((int32_t)STATLIST_UnitGetStatValue(pPet, STAT_ITEM_ABSORBFIRE_PERCENT, 0) <= 0)
            {
                STATLIST_SetStatIfListIsValid(pStatList, STAT_FIRERESIST, nSummonRes, 0);
            }

            if ((int32_t)STATLIST_UnitGetStatValue(pPet, STAT_ITEM_ABSORBLIGHT_PERCENT, 0) <= 0)
            {
                STATLIST_SetStatIfListIsValid(pStatList, STAT_LIGHTRESIST, nSummonRes, 0);
            }

            if ((int32_t)STATLIST_UnitGetStatValue(pPet, STAT_ITEM_ABSORBCOLD_PERCENT, 0) <= 0)
            {
                STATLIST_SetStatIfListIsValid(pStatList, STAT_COLDRESIST, nSummonRes, 0);
            }

            STATLIST_SetStatIfListIsValid(pStatList, STAT_POISONRESIST, nSummonRes, 0);
        }
    }
}

//D2Game.0x6FD0C3A0
void __fastcall D2GAME_SetUnitComponent_6FD0C3A0(D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nLevel, int32_t bHasShield, int32_t bSpecial)
{
    constexpr D2NecroSkeletonComponentStrc necroSkeletonComponents[11] =
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 1 },
        { 0, 1, 0, 0, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 0, 1, 1, 2 },
        { 1, 1, 0, 0, 0, 0, 1, 1, 2 },
        { 1, 1, 1, 0, 0, 1, 1, 1, 3 },
        { 2, 2, 1, 0, 0, 1, 1, 1, 3 },
        { 2, 2, 1, 0, 0, 1, 1, 2, 4 },
        { 2, 3, 1, 0, 0, 1, 2, 2, 4 },
        { 2, 3, 1, 0, 0, 1, 2, 2, 4 },
    };

    if (nLevel > 10)
    {
        nLevel = 10;
    }

    if (!pPet || pPet->dwUnitType != UNIT_MONSTER || !pPet->pMonsterData)
    {
        return;
    }

    pPet->pMonsterData->nComponent[0] = necroSkeletonComponents[nLevel].nHead;
    pPet->pMonsterData->nComponent[1] = necroSkeletonComponents[nLevel].nTorso;
    pPet->pMonsterData->nComponent[8] = necroSkeletonComponents[nLevel].nSpecial1;
    pPet->pMonsterData->nComponent[9] = necroSkeletonComponents[nLevel].nSpecial2;
    pPet->pMonsterData->nComponent[2] = necroSkeletonComponents[nLevel].nLegs;
    pPet->pMonsterData->nComponent[3] = necroSkeletonComponents[nLevel].nRightArm;
    pPet->pMonsterData->nComponent[4] = necroSkeletonComponents[nLevel].nLeftArm;

    if (bHasShield)
    {
        pPet->pMonsterData->nComponent[7] = necroSkeletonComponents[nLevel].nShield;
    }

    if (bSpecial)
    {
        const uint8_t nComponent = ITEMS_RollRandomNumber(&pUnit->pSeed) & 3;
        pPet->pMonsterData->nComponent[11] = nComponent;
        pPet->pMonsterData->nComponent[12] = nComponent;
        AITHINK_SetAiControlParams(pPet->pMonsterData->pAiControl, 1, 0, -666);
    }
    else
    {
        pPet->pMonsterData->nComponent[5] = necroSkeletonComponents[nLevel].nRightHand;
    }
}

//D2Game.0x6FD0C500
void __fastcall sub_6FD0C500(D2UnitStrc* pMonster, uint8_t nIndex, uint8_t nComponent)
{
    if (pMonster && pMonster->dwUnitType == UNIT_MONSTER && nIndex < 16u && pMonster->pMonsterData)
    {
        pMonster->pMonsterData->nComponent[nIndex] = nComponent;
    }
}

//D2Game.0x6FD0C530
int32_t __fastcall D2GAME_SetSummonPassiveStats_6FD0C530(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nSkillId, int32_t nSkillLevel, int32_t nItemLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    for (int32_t i = 0; i < 5; ++i)
    {
        if (pSkillsTxtRecord->nPassiveStat[i] >= 0)
        {
            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pSkillsTxtRecord->nPassiveStat[i]);
            if (pItemStatCostTxtRecord)
            {
                STATLIST_AddUnitStat(pPet, pSkillsTxtRecord->nPassiveStat[i], SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPassiveCalc[i], nSkillId, nSkillLevel), 0);

                if (pItemStatCostTxtRecord->wItemEvent[0] > 0)
                {
                    const int32_t nPetGUID = pPet ? pPet->dwUnitId : -1;
                    if (!SUNITEVENT_GetTimer(pGame, pPet, 2, nPetGUID, pSkillsTxtRecord->nPassiveStat[i]))
                    {
                        const int32_t nLayer_StatId = pSkillsTxtRecord->nPassiveStat[i] << 16;
                        sub_6FD156A0(pGame, pPet, pItemStatCostTxtRecord->wItemEvent[0], nLayer_StatId, 0, pItemStatCostTxtRecord->wItemEventFunc[0], 2, nPetGUID);

                        if (pItemStatCostTxtRecord->wItemEvent[1] > 0)
                        {
                            sub_6FD156A0(pGame, pPet, pItemStatCostTxtRecord->wItemEvent[1], nLayer_StatId, 0, pItemStatCostTxtRecord->wItemEventFunc[1], 2, nPetGUID);
                        }
                    }
                }
            }
        }
    }

    D2StatListStrc* pStatList = nullptr;
    for (int32_t i = 0; i < 6; ++i)
    {
        if (pSkillsTxtRecord->wAuraStat[i] >= 0)
        {
            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(pSkillsTxtRecord->wAuraStat[i]);
            if (pItemStatCostTxtRecord)
            {
                const int32_t nValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraStatCalc[i], nSkillId, nSkillLevel);
                if (nValue)
                {
                    if (!pStatList)
                    {
                        int32_t nUnitGUID = -1;
                        int32_t nUnitType = 6;
                        if (pPet)
                        {
                            nUnitGUID = pPet->dwUnitId;
                            nUnitType = pPet->dwUnitType;
                        }

                        pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 0, 0, nUnitType, nUnitGUID);
                        if (!pStatList)
                        {
                            return 0;
                        }

                        D2COMMON_10475_PostStatToStatList(pPet, pStatList, 1);
                    }

                    STATLIST_SetStatIfListIsValid(pStatList, pSkillsTxtRecord->wAuraStat[i], nValue, 0);

                    if (pItemStatCostTxtRecord->wItemEvent[0] > 0)
                    {
                        const int32_t nPetGUID = pPet ? pPet->dwUnitId : -1;
                        if (!SUNITEVENT_GetTimer(pGame, pPet, 2, nPetGUID, pSkillsTxtRecord->wAuraStat[i]))
                        {
                            const int32_t nLayer_StatId = pSkillsTxtRecord->wAuraStat[i] << 16;
                            sub_6FD156A0(pGame, pPet, pItemStatCostTxtRecord->wItemEvent[0], nLayer_StatId, 0, pItemStatCostTxtRecord->wItemEventFunc[0], 2, nPetGUID);
                            if (pItemStatCostTxtRecord->wItemEvent[1] > 0)
                            {
                                sub_6FD156A0(pGame, pPet, pItemStatCostTxtRecord->wItemEvent[1], nLayer_StatId, 0, pItemStatCostTxtRecord->wItemEventFunc[1], 2, nPetGUID);
                            }
                        }
                    }
                }
            }
        }
    }

    if (pSkillsTxtRecord->nAuraState > 0 && pSkillsTxtRecord->nAuraState <= sgptDataTables->nStatesTxtRecordCount)
    {
        STATES_ToggleState(pPet, pSkillsTxtRecord->nAuraState, 1);
        if (pStatList)
        {
            STATLIST_SetState(pStatList, pSkillsTxtRecord->nAuraState);
        }
    }

    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pPet);
    const int32_t nNewMaxHp = MONSTERUNIQUE_CalculatePercentage(nMaxHp, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel), 100) + nMaxHp;
    STATLIST_SetUnitStat(pPet, STAT_MAXHP, nNewMaxHp, 0);
    STATLIST_SetUnitStat(pPet, STAT_HITPOINTS, nNewMaxHp, 0);

    for (int32_t i = 0; i < 5; ++i)
    {
        D2SkillsTxt* pSummonSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pSkillsTxtRecord->wSumSkill[i]);
        if (pSummonSkillsTxtRecord)
        {
            const int32_t nValue = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwSumSkCalc[i], nSkillId, nSkillLevel);
            if (nValue > 0)
            {
                D2GAME_SetSkills_6FD14C60(pPet, pSkillsTxtRecord->wSumSkill[i], nValue, 0);

                if (pSummonSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_AURA])
                {
                    D2GAME_AssignSkill_6FD13800(pPet, 0, pSkillsTxtRecord->wSumSkill[i], -1);
                }
            }
        }
    }

    if (pSkillsTxtRecord->wAuraEvent[0] >= 0)
    {
        SUNITEVENT_FreeTimer(pGame, pPet, 1, pSkillsTxtRecord->nAuraState);

        for (int32_t i = 0; i < 3; ++i)
        {
            if (pSkillsTxtRecord->wAuraEvent[i] < 0)
            {
                break;
            }

            sub_6FD156A0(pGame, pPet, pSkillsTxtRecord->wAuraEvent[i], nSkillId, nSkillLevel, pSkillsTxtRecord->wAuraEventFunc[i], 1, pSkillsTxtRecord->nAuraState);
        }
    }

    if (pSkillsTxtRecord->wSumUMod > 0 && pSkillsTxtRecord->wSumUMod < 43)
    {
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pPet, pSkillsTxtRecord->wSumUMod, 1);
    }

    if (pSkillsTxtRecord->wSumOverlay > 0 && pSkillsTxtRecord->wSumOverlay < sgptDataTables->nOverlayTxtRecordCount)
    {
        UNITS_SetOverlay(pPet, pSkillsTxtRecord->wSumOverlay, 0);
    }

    if (nItemLevel)
    {
        sub_6FCF9580(pGame, pUnit, pPet, nSkillId, nSkillLevel, nItemLevel, 0);
    }
    else
    {
        const int32_t nLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
        nItemLevel = D2Clamp(3 * nSkillLevel, 1, nLevel);
        sub_6FCF9580(pGame, pUnit, pPet, nSkillId, nSkillLevel, nItemLevel, 0);
    }

    return 1;
}

//D2Game.0x6FD0CB10
int32_t __fastcall D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pPet, int32_t nPetLevelArg, int32_t nSkillLevel)
{
    int32_t nPetLevel = nPetLevelArg;
    if (nPetLevelArg <= 0)
    {
        const int32_t nUnitLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);

        nPetLevel = nSkillLevel + 3 * nUnitLevel / 4;
        if (nPetLevel < 1)
        {
            nPetLevel = 1;
        }
        else if (nPetLevel >= nUnitLevel)
        {
            nPetLevel = nUnitLevel;
        }
    }

    if (nPetLevel)
    {
        STATLIST_SetUnitStat(pPet, STAT_LEVEL, nPetLevel, 0);
    }

    if (nPetLevel >= sgptDataTables->nMonLvlTxtRecordCount - 1)
    {
        nPetLevel = sgptDataTables->nMonLvlTxtRecordCount - 1;
    }

    if (nPetLevel < 0 || nPetLevel >= sgptDataTables->nMonLvlTxtRecordCount)
    {
        return 0;
    }
    
    D2MonLvlTxt* pMonLvlTxtRecord = &sgptDataTables->pMonLvlTxt[nPetLevel];
    if (!pMonLvlTxtRecord)
    {
        return 0;
    }

    uint8_t nDifficulty = pGame->nDifficulty;
    if (nDifficulty < 0)
    {
        nDifficulty = 0;
    }
    else if (nDifficulty > 2)
    {
        nDifficulty = 2;
    }

    const uint8_t nGameType = pGame->nGameType || pGame->dwGameType;
    STATLIST_AddUnitStat(pPet, STAT_ARMORCLASS, nGameType ? pMonLvlTxtRecord->dwLAC[nDifficulty] : pMonLvlTxtRecord->dwAC[nDifficulty], 0);
    STATLIST_AddUnitStat(pPet, STAT_TOHIT, nGameType ? pMonLvlTxtRecord->dwLTH[nDifficulty] : pMonLvlTxtRecord->dwTH[nDifficulty], 0);

    return 1;
}

//D2Game.0x6FD0CC10
int32_t __fastcall SKILLS_SrvDo031_RaiseSkeleton_Mage(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || !D2COMMON_11017_CheckUnitIfConsumeable(pTarget, 0))
    {
        return 0;
    }

    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && pUnit->dwClassId == PCLASS_PALADIN)
    {
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);

        D2DamageStrc damage = {};

        damage.wResultFlags |= 4u;
        damage.dwHitFlags |= 0x1000u;
        damage.dwPhysDamage = nMaxHp / 8;
        damage.dwDmgTotal = nMaxHp / 8;

        SUNITDMG_ExecuteEvents(pGame, pUnit, pUnit, 0, &damage);
        SUNITDMG_ExecuteMissileDamage(pGame, pUnit, pUnit, &damage);
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, 0, 0);
    if (nSummonId < 0 || pSkillsTxtRecord->nPetType < 0 || pSkillsTxtRecord->nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        return 0;
    }

    const int32_t nX = CLIENTS_GetUnitX(pTarget);
    const int32_t nY = CLIENTS_GetUnitY(pTarget);
    DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pTarget), pTarget->dwUnitType, pTarget->dwUnitId);
    SUNIT_RemoveUnit(pGame, pTarget);

    D2SummonArgStrc summonArg = {};

    summonArg.nHcIdx = nSummonId;
    summonArg.pPosition.nX = nX;
    summonArg.pPosition.nY = nY;
    summonArg.nMonMode = nSpawnMode;
    summonArg.dwFlags = 1;
    summonArg.pOwner = pUnit;
    summonArg.nSpecialAiState = 0;
    summonArg.nPetType = pSkillsTxtRecord->nPetType;
    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
    if (!pPet)
    {
        return 0;
    }

    D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(pGame, pUnit, pPet, 0, nSkillLevel);
    if (pPet->dwClassId == MONSTER_NECROSKELETON)
    {
        int32_t nLevel = 1;
        if (STATES_CheckState(pUnit, STATE_ITEMFULLSET))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_ITEMFULLSET);
            if (pStatList)
            {
                //STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);
                nLevel = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0);
            }
        }

        int32_t bHasShield = 0;
        if (nSkillLevel > 2 && (ITEMS_RollRandomNumber(&pUnit->pSeed) % 100) < pSkillsTxtRecord->dwParam[0])
        {
            bHasShield = 1;
        }

        D2GAME_SetUnitComponent_6FD0C3A0(pUnit, pPet, nLevel, bHasShield, 0);
    }
    else if (pPet->dwClassId == MONSTER_NECROMAGE)
    {
        int32_t nLevel = 1;
        if (STATES_CheckState(pUnit, STATE_ITEMFULLSET))
        {
            D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_ITEMFULLSET);
            if (pStatList)
            {
                //STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_SKILL, 0);
                nLevel = STATLIST_GetStatValue(pStatList, STAT_MODIFIERLIST_LEVEL, 0);
            }
        }

        D2GAME_SetUnitComponent_6FD0C3A0(pUnit, pPet, nLevel, 0, 1);
    }

    D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pPet, nSkillId, nSkillLevel, 0);
    D2GAME_SetSummonResistance_6FD0C2E0(pUnit, pPet);
    D2GAME_UpdateSummonAI_6FC401F0(pGame, pPet, 0, pUnit->dwNodeIndex);
    return 1;
}

//D2Game.0x6FD0CFC0
int32_t __fastcall SKILLS_SrvDo032_PoisonDagger(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (pTarget)
    {
        SUNITDMG_DrainItemDurability(pGame, pUnit, pTarget, 0);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD0D000
int32_t __fastcall sub_6FD0D000(D2AuraCallbackStrc* pAuraCallback, D2UnitStrc* pUnit)
{
    D2UnkNecSkillStrc2* pArg = (D2UnkNecSkillStrc2*)pAuraCallback->pArgs;
    if (pArg)
    {
        D2DamageStrc damage = {};
        memcpy(&damage, pArg->pDamage, sizeof(damage));

        if (pArg->unk0x0C < PATH_ComputeSquaredDistance(pArg->nX, pArg->nY, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit)))
        {
            damage.dwPhysDamage = 0;
        }

        SUNITDMG_ExecuteEvents(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, 1, &damage);
        SUNITDMG_ExecuteMissileDamage(pAuraCallback->pGame, pAuraCallback->pOwner, pUnit, &damage);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD0D0D0
int32_t __fastcall SKILLS_SrvDo055_CorpseExplosion(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);

    if (!pTarget || !D2COMMON_11021(pTarget))
    {
        return 0;
    }

    STATES_ToggleState(pTarget, STATE_CORPSE_NODRAW, 1);
    UNITROOM_RefreshUnit(pTarget);

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nMaxHp = STATLIST_GetUnitBaseStat(pTarget, STAT_MAXHP, 0);
    if (pTarget->dwUnitType == UNIT_MONSTER)
    {
        D2MonStatsInitStrc monStatsInit = {};
        DATATBLS_CalculateMonsterStatsByLevel(pTarget->dwClassId, pGame->nGameType || pGame->dwGameType, pGame->nDifficulty, STATLIST_UnitGetStatValue(pTarget, STAT_LEVEL, 0), 1, &monStatsInit);
        nMaxHp = (monStatsInit.nMaxHP + monStatsInit.nMinHP) << 7;
    }

    const int32_t nX = CLIENTS_GetUnitX(pTarget);
    const int32_t nY = CLIENTS_GetUnitY(pTarget);
    
    const int32_t nAuraRange = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwAuraRangeCalc, nSkillId, nSkillLevel);
    const int32_t nHalfRange = nAuraRange / 2;
    const int32_t nRange = (nAuraRange + 1) / 2;
    const int32_t nMinDamage = MONSTERUNIQUE_CalculatePercentage(SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel), nMaxHp, 100);
    const int32_t nMaxDamage = MONSTERUNIQUE_CalculatePercentage(SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel), nMaxHp, 100);
    
    int32_t nDamage = ITEMS_RollLimitedRandomNumber(&pTarget->pSeed, nMaxDamage - nMinDamage) + nMinDamage;
    
    const int32_t nTargetLevel = STATLIST_UnitGetStatValue(pTarget, STAT_LEVEL, 0);
    const int32_t nUnitLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);

    if (nTargetLevel && nUnitLevel < nTargetLevel)
    {
        nDamage = MONSTERUNIQUE_CalculatePercentage(nDamage, nUnitLevel, nTargetLevel);
    }

    D2DamageStrc damage = {};

    int32_t nPercentage = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
    if (nPercentage < 0)
    {
        nPercentage = 0;
    }
    else if (nPercentage > 100)
    {
        nPercentage = 100;
    }

    if (nPercentage > 0 && pSkillsTxtRecord->nEType)
    {
        sub_6FD11E40(pUnit, &damage, pSkillsTxtRecord->nEType, MONSTERUNIQUE_CalculatePercentage(nDamage, nPercentage, 100), SKILLS_GetElementalLength(pUnit, nSkillId, nSkillLevel, 1), 0, 0);

        nDamage = MONSTERUNIQUE_CalculatePercentage(nDamage, 100 - nPercentage, 100);
    }

    damage.dwPhysDamage = nDamage;

    D2UnkNecSkillStrc2 args = {};

    args.pDamage = &damage;
    args.nX = nX;
    args.nY = nY;
    args.unk0x0C = nHalfRange * nHalfRange;

    sub_6FD0FE80(pGame, pUnit, nX, nY, nRange, pSkillsTxtRecord->dwAuraFilter, sub_6FD0D000, &args, 0, __FILE__, __LINE__);

    return 1;
}

//D2Game.0x6FD0D620
int32_t __fastcall SKILLS_SrvDo056_Golem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->wSummon < 0 || pSkillsTxtRecord->wSummon >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        return 0;
    }

    int32_t nPetType = pSkillsTxtRecord->nPetType;
    if (nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        nPetType = 0;
    }

    uint8_t nSumMode = pSkillsTxtRecord->nSumMode;
    if (pSkillsTxtRecord->nSumMode >= 16)
    {
        nSumMode = 1;
    }

    D2SummonArgStrc pSummonArg = {};
    pSummonArg.dwFlags = 0;
    pSummonArg.pOwner = pUnit;
    pSummonArg.nSpecialAiState = 0;
    pSummonArg.nHcIdx = pSkillsTxtRecord->wSummon;
    pSummonArg.nPetType = nPetType;
    pSummonArg.nMonMode = nSumMode;
    pSummonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &pSummonArg);
    if (pPet)
    {
        D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(pGame, pUnit, pPet, 0, nSkillLevel);
        D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pPet, nSkillId, nSkillLevel, 0);
        D2GAME_SetSummonResistance_6FD0C2E0(pUnit, pPet);
        sub_6FC3E200(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pPet);
        D2GAME_UpdateSummonAI_6FC401F0(pGame, pPet, 0, pUnit->dwNodeIndex);
        return 1;
    }

    return 0;
}

//D2Game.0x6FD0D7B0
int32_t __fastcall SKILLS_SrvDo057_IronGolem(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTargetUnit || pTargetUnit->dwUnitType != UNIT_ITEM || pTargetUnit->dwAnimMode != IMODE_ONGROUND || !ITEMS_IsMetalItem(pTargetUnit->dwClassId) || !ITEMS_CheckItemFlag(pTargetUnit, IFLAG_IDENTIFIED, __LINE__, __FILE__) || STATLIST_GetOwner(pTargetUnit, 0))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, 0, 0);
    if (nSummonId < 0)
    {
        return 0;
    }

    uint8_t nPetType = pSkillsTxtRecord->nPetType;
    if (nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        nPetType = 0;
    }

    D2SummonArgStrc summonArg = {};
    summonArg.nSpecialAiState = 0;
    summonArg.nMonMode = nSpawnMode;
    summonArg.pOwner = pUnit;
    summonArg.nHcIdx = nSummonId;
    summonArg.nPetType = nPetType;
    summonArg.dwFlags = 1;
    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);
    summonArg.pPosition.nX = CLIENTS_GetUnitX(pTargetUnit);
    summonArg.pPosition.nY = CLIENTS_GetUnitY(pTargetUnit);

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
    if (!pPet)
    {
        return 0;
    }

    DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pTargetUnit), pTargetUnit->dwUnitType, pTargetUnit->dwUnitId);
    UNITS_FreeCollisionPath(pTargetUnit);
    UNITROOM_RemoveUnitFromRoom(pTargetUnit);

    uint8_t nBodyLoc1 = 0;
    uint8_t nBodyLoc2 = 0;
    ITEMS_GetAllowedBodyLocations(pTargetUnit, &nBodyLoc1, &nBodyLoc2);
    UNITS_ChangeAnimMode(pTargetUnit, IMODE_ONCURSOR);
    sub_6FC45060(pGame, pPet, pTargetUnit->dwUnitId, nBodyLoc1, 1, 0);

    D2GAME_SKILLS_SetSummonBaseStats_6FD0CB10(pGame, pUnit, pPet, 0, nSkillLevel);
    D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pPet, nSkillId, nSkillLevel, 0);
    D2GAME_SetSummonResistance_6FD0C2E0(pUnit, pPet);
    sub_6FC3E200(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pPet);
    D2GAME_UpdateSummonAI_6FC401F0(pGame, pPet, 0, pUnit->dwNodeIndex);
    return 1;
}

//D2Game.0x6FD0DAC0
int32_t __fastcall SKILLS_SrvDo058_Revive(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    int32_t nPetType = pSkillsTxtRecord->nPetType;
    if (nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        nPetType = 0;
    }

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
    if (!pTarget || pTarget->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pTarget->dwClassId);
    if (!pMonStats2TxtRecord || !(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_REVIVE]) || !SUNIT_IsDead(pTarget) || !D2COMMON_11017_CheckUnitIfConsumeable(pTarget, 0) || !sub_6FD15190(pTarget, 7))
    {
        return 0;
    }

    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && pUnit->dwClassId == PCLASS_PALADIN)
    {
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
        D2DamageStrc damage = {};

        damage.wResultFlags |= 4u;
        damage.dwHitFlags |= 0x1000u;
        damage.dwPhysDamage = nMaxHp / 8;
        damage.dwDmgTotal = nMaxHp / 8;
        SUNITDMG_ExecuteEvents(pGame, pUnit, pUnit, 0, &damage);
        SUNITDMG_ExecuteMissileDamage(pGame, pUnit, pUnit, &damage);
    }

    D2CoordStrc coords = {};
    coords.nX = CLIENTS_GetUnitX(pTarget);
    coords.nY = CLIENTS_GetUnitY(pTarget);

    COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pTarget), coords.nX, coords.nY, PATH_GetUnitCollisionPattern(pTarget), 0x8000u);

    D2RoomStrc* pRoom = COLLISION_GetFreeCoordinates(UNITS_GetRoom(pTarget), &coords, UNITS_GetUnitSizeX(pTarget), 0x3C01u, 1);
    if (pRoom)
    {
        MONSTERUNIQUE_ToggleUnitFlag(pTarget, UNITFLAG_CANBEATTACKED, 1);
        MONSTERUNIQUE_ToggleUnitFlag(pTarget, UNITFLAG_ISVALIDTARGET, 1);
        MONSTERUNIQUE_ToggleUnitFlag(pTarget, UNITFLAG_TARGETABLE, 1);
        MONSTERUNIQUE_ToggleUnitFlag(pTarget, UNITFLAG_NOXP, 1);
        MONSTERUNIQUE_ToggleUnitFlag(pTarget, UNITFLAG_NOTC, 1);
        MONSTER_UpdateAiCallbackEvent(pGame, pTarget);

        D2ModeChangeStrc modeChange = {};
        MONSTERMODE_GetModeChangeInfo(pTarget, MONMODE_NEUTRAL, &modeChange);
        D2GAME_ModeChange_6FC65220(pGame, &modeChange, 1);

        sub_6FD154D0(pTarget);
        D2GAME_EVENTS_Delete_6FC34840(pGame, pTarget, 8, 0);
        if (UNITS_GetRightSkill(pTarget))
        {
            SKILLS_SetRightActiveSkill(pTarget, 0, -1);
        }

        sub_6FCBDFE0(pGame, pTarget, pRoom, coords.nX, coords.nY, 0, 0);
    }

    D2MonStatsInitStrc monStatsInit = {};
    DATATBLS_CalculateMonsterStatsByLevel(pTarget->dwClassId, pGame->nGameType || pGame->dwGameType, pGame->nDifficulty, STATLIST_UnitGetStatValue(pTarget, STAT_LEVEL, 0), 1, &monStatsInit);

    const int32_t nNewHp = (ITEMS_RollLimitedRandomNumber(&pTarget->pSeed, monStatsInit.nMaxHP - monStatsInit.nMinHP + 1) + monStatsInit.nMinHP) << 8;
    STATLIST_SetUnitStat(pTarget, STAT_MAXHP, nNewHp, 0);
    STATLIST_SetUnitStat(pTarget, STAT_HITPOINTS, nNewHp, 0);

    const int32_t nUnitLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);
    const int32_t nTargetLevel = STATLIST_UnitGetStatValue(pTarget, STAT_LEVEL, 0);
    if (nTargetLevel && nUnitLevel < nTargetLevel)
    {
        int32_t nHitpoints = MONSTERUNIQUE_CalculatePercentage(nNewHp, nUnitLevel, nTargetLevel);
        if (nHitpoints < 1)
        {
            nHitpoints = 1;
        }
        STATLIST_SetUnitStat(pTarget, STAT_MAXHP, nHitpoints, 0);
        STATLIST_SetUnitStat(pTarget, STAT_HITPOINTS, nHitpoints, 0);
        STATLIST_SetUnitStat(pTarget, STAT_LEVEL, nUnitLevel, 0);
    }

    D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pTarget, nSkillId, nSkillLevel, 0);
    sub_6FD0DF40(pGame, pUnit, pTarget, nSkillId, nSkillLevel);
    sub_6FC7D7A0(pGame, pUnit, pTarget, nPetType, SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel));
    D2GAME_UpdateSummonAI_6FC401F0(pGame, pTarget, 0, pUnit->dwNodeIndex);
    D2Common_10214(pTarget);

    return 1;
}

//D2Game.0x6FD0DF40
void __fastcall sub_6FD0DF40(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pMonster, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (nSkillId)
    {
        int32_t nUnitType = 6;
        int32_t nUnitGUID = -1;
        if (pUnit)
        {
            nUnitType = pUnit->dwUnitType;
            nUnitGUID = pUnit->dwUnitId;
        }

        AIGENERAL_SetOwnerData(pGame, pMonster, nUnitGUID, nUnitType, 0, 0);
        AIUTIL_SetOwnerGUIDAndType(pMonster, pUnit);
        D2GAME_EVENTS_Delete_6FC34840(pGame, pMonster, UNITEVENTCALLBACK_AITHINK, 0);
        EVENT_SetEvent(pGame, pMonster, UNITEVENTCALLBACK_AITHINK, pGame->dwGameFrame + 15, 0, 0);
        sub_6FCBDD30(pMonster, 2u, 0);
        sub_6FC61F00(pMonster);

        if (pMonster)
        {
            pMonster->dwFlags |= UNITFLAG_ISREVIVE;
        }

        STATES_ToggleState(pMonster, STATE_REVIVE, 1);

        const int32_t nDuration = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        if (nDuration > 0)
        {
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pMonster, 21, 0);
            EVENT_SetEvent(pGame, pMonster, UNITEVENTCALLBACK_MONUMOD, nDuration + pGame->dwGameFrame, 0, 0);
        }
    }
}

//D2Game.0x6FD0E050
int32_t __fastcall SKILLS_SrvDo060_BoneWall(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }
    
    D2RoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nTargetX, nTargetY);    
    if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }
    
    int32_t nSpawnMode = 0;
    const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, 0, 0);
    if (nSummonId < 0)
    {
        return 0;
    }

    int32_t nPetType = pSkillsTxtRecord->nPetType;
    if (nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
    {
        nPetType = 0;
    }

    D2SummonArgStrc summonArg = {};

    summonArg.nSpecialAiState = 0;
    summonArg.nPetType = nPetType;
    summonArg.nMonMode = nSpawnMode;
    summonArg.pPosition.nX = nTargetX;
    summonArg.pPosition.nY = nTargetY;
    summonArg.dwFlags = 9;
    summonArg.pOwner = pUnit;
    summonArg.nHcIdx = nSummonId;
    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
    if (!pPet)
    {
        return 0;
    }

    if (pUnit)
    {
        AIGENERAL_SetOwnerData(pGame, pPet, pUnit->dwUnitId, pUnit->dwUnitType, 0, 1);
    }
    else
    {
        AIGENERAL_SetOwnerData(pGame, pPet, -1, 6, 0, 1);
    }

    D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pPet, MONUMOD_KILLMINIONSDEATH, 0);
    D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pPet, nSkillId, nSkillLevel, 0);
    sub_6FC40280(pGame, pPet, 0, 9);

    nTargetX = CLIENTS_GetUnitX(pPet);
    nTargetY = CLIENTS_GetUnitY(pPet);
     
    if (DUNGEON_IsRoomInTown(pRoom))
    {
        return 0;
    }

    const int32_t nParam = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel) / 2;
    if (nParam > 1)
    {
        if (pSkillsTxtRecord->wSrvMissileA >= 0 && pSkillsTxtRecord->wSrvMissileA < sgptDataTables->nMissilesTxtRecordCount)
        {
            D2MissileStrc missileParams = {};

            missileParams.nX = nTargetX;
            missileParams.nY = nTargetY;
            missileParams.dwFlags = 0x21u;
            missileParams.pOwner = pUnit;
            missileParams.nSkill = nSkillId;
            missileParams.nSkillLevel = nSkillLevel;
            missileParams.nMissile = pSkillsTxtRecord->wSrvMissileA;

            int32_t nX = CLIENTS_GetUnitX(pUnit) - nTargetX;
            int32_t nY = CLIENTS_GetUnitY(pUnit) - nTargetY;
            if (!nX && !nY)
            {
                nX = 1;
            }

            missileParams.nTargetX = nTargetX - nY;
            missileParams.nTargetY = nTargetY + nX;
            D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
            if (pMissile)
            {
                MISSILE_SetTargetX(pMissile, pPet->dwUnitId);
                MISSILE_SetTargetY(pMissile, nParam);
            }

            missileParams.nTargetX = nTargetX + nY;
            missileParams.nTargetY = nTargetY - nX;
            pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
            if (pMissile)
            {
                MISSILE_SetTargetX(pMissile, pPet->dwUnitId);
                MISSILE_SetTargetY(pMissile, nParam);
            }
        }
    }

    return 1;
}

//D2Game.0x6FD0E4C0
int32_t __fastcall SKILLS_SrvDo062_BonePrison(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    constexpr int32_t xOffsets[] =
    {
        -1, 1, 3, 4, 4, 3, -1, 1, -3, -4, -4, -3
    };
    
    constexpr int32_t yOffsets[] =
    {
        -4, -4, -3, -1, 1, 3, 4, 4, 3, -1, 1, -3
    };

    int32_t nTargetX = 0;
    int32_t nTargetY = 0;
    if (!D2GAME_GetXAndYFromTargetUnit_6FD14020(pGame, pUnit, &nTargetX, &nTargetY))
    {
        return 0;
    }

    if (pUnit)
    {
        pUnit->dwFlags |= UNITFLAG_SKSRVDOFUNC;
    }

    D2UnitStrc* pOwner = nullptr;
    for (int32_t i = 0; i < 12; ++i)
    {
        const int32_t nX = nTargetX + xOffsets[i];
        const int32_t nY = nTargetY + yOffsets[i];

        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
        if (pSkillsTxtRecord)
        {
            int32_t nSpawnMode = 0;
            const int32_t nSummonId = D2GAME_GetSummonIdFromSkill_6FD15580(pUnit, 0, nSkillId, nSkillLevel, &nSpawnMode, 0, 0);
            if (nSummonId >= 0)
            {
                int32_t nPetType = pSkillsTxtRecord->nPetType;
                if (nPetType >= sgptDataTables->nPetTypeTxtRecordCount)
                {
                    nPetType = 0;
                }

                if (!DUNGEON_IsRoomInTown(D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY)))
                {
                    D2SummonArgStrc summonArg = {};
                    summonArg.nHcIdx = nSummonId;
                    summonArg.nMonMode = nSpawnMode;
                    summonArg.dwFlags = 9;
                    summonArg.pOwner = pUnit;
                    summonArg.nSpecialAiState = 0;
                    summonArg.nPetType = nPetType;
                    summonArg.pPosition.nX = nX;
                    summonArg.pPosition.nY = nY;
                    summonArg.nPetMax = SKILLS_EvaluateSkillFormula(pUnit, pSkillsTxtRecord->dwPetMax, nSkillId, nSkillLevel);

                    D2UnitStrc* pPet = D2GAME_SummonPet_6FD14430(pGame, &summonArg);
                    if (pPet)
                    {
                        D2GAME_SetSummonPassiveStats_6FD0C530(pGame, pUnit, pPet, nSkillId, nSkillLevel, 0);
                        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pPet, MONUMOD_KILLMINIONSDEATH, 0);
                        sub_6FC40280(pGame, pPet, 0, 9);

                        if (pOwner)
                        {
                            AIGENERAL_SetOwnerData(pGame, pPet, pOwner->dwUnitId, pOwner->dwUnitType, 0, 0);
                            AIGENERAL_AllocMinionList(pGame, pOwner, pPet);
                        }
                        else
                        {
                            pOwner = pPet;
                            if (pUnit)
                            {
                                AIGENERAL_SetOwnerData(pGame, pPet, pUnit->dwUnitId, pUnit->dwUnitType, 0, 1);
                            }
                            else
                            {
                                AIGENERAL_SetOwnerData(pGame, pPet, -1, 6, 0, 1);
                            }
                        }

                        UNITS_StoreOwner(pPet, pUnit);
                        PATH_SetDirection(pPet->pDynamicPath, PATH_ComputeDirection(CLIENTS_GetUnitX(pPet), CLIENTS_GetUnitY(pPet), nTargetX, nTargetY));
                    }
                }
            }
        }
    }

    return 1;
}

//D2Game.0x6FD0E790
int32_t __fastcall SKILLS_SrvDo063_PoisonExplosion(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, int32_t nSkillLevel)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!nSkillId)
    {
        return 0;
    }

    const int16_t nMissileId = pSkillsTxtRecord->wSrvMissileA;
    if (nMissileId >= 0 && nMissileId < sgptDataTables->nMissilesTxtRecordCount)
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (!pTarget || !D2COMMON_11021(pTarget))
        {
            return 0;
        }

        STATES_ToggleState(pTarget, STATE_CORPSE_NOSELECT, 1);
        UNITROOM_RefreshUnit(pTarget);
        MISSMODE_CreatePoisonCloudHitSubmissiles(pGame, pUnit, pTarget, pSkillsTxtRecord->wSrvMissileA, nSkillId, nSkillLevel, 0, 2, 0);
    }

    return 1;
}

//D2Game.0x6FD0E840
int32_t __fastcall D2GAME_EventFunc04_6FD0E840(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pUnit || !pAttacker || (pAttacker->dwUnitType != UNIT_PLAYER && pAttacker->dwUnitType != UNIT_MONSTER) || SUNIT_IsDead(pAttacker) || !pDamage || pDamage->dwPhysDamage <= 0)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (nSkillLevel < 0)
    {
        nSkillLevel = 1;
    }

    if (pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount || !STATES_CheckState(pAttacker, pSkillsTxtRecord->wAuraTargetState))
    {
        return 0;
    }

    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pAttacker, pSkillsTxtRecord->wAuraTargetState);
    if (!pStatList)
    {
        return 0;
    }

    D2UnitStrc* pStatListOwner = SUNIT_GetServerUnit(pGame, STATLIST_GetOwnerType(pStatList), STATLIST_GetOwnerGUID(pStatList));
    int32_t nDamagePercent = 0;
    if (pAttacker->dwUnitType == UNIT_MONSTER || MONSTERS_GetHirelingTypeId(pAttacker))
    {
        if (pUnit->dwUnitType == UNIT_MONSTER)
        {
            nDamagePercent = SKILLS_EvaluateSkillFormula(pStatListOwner, pSkillsTxtRecord->dwCalc[1], nSkillId, nSkillLevel);
        }
        else
        {
            nDamagePercent = SKILLS_EvaluateSkillFormula(pStatListOwner, pSkillsTxtRecord->dwCalc[2], nSkillId, nSkillLevel);
        }
    }
    else
    {
        nDamagePercent = SKILLS_EvaluateSkillFormula(pStatListOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel);
    }

    D2DamageStrc damage = {};
    damage.dwPhysDamage = MONSTERUNIQUE_CalculatePercentage(pDamage->dwPhysDamage, nDamagePercent, 100);
    damage.wResultFlags |= pSkillsTxtRecord->wResultFlags | 0x20;
    damage.dwHitFlags |= pSkillsTxtRecord->dwHitFlags;
    damage.dwHitClass = pSkillsTxtRecord->dwHitClass;
    SUNITDMG_ExecuteEvents(pGame, pUnit, pAttacker, 1, &damage);
    SUNITDMG_ExecuteMissileDamage(pGame, pUnit, pAttacker, &damage);

    if (pUnit->dwUnitType != UNIT_MONSTER || pUnit->dwClassId != MONSTER_BLOODGOLEM || !pUnit->pMonsterData || !pUnit->pMonsterData->pMonstatsTxt)
    {
        return 1;
    }

    const uint8_t nDrain = pUnit->pMonsterData->pMonstatsTxt->nDrain[pGame->nDifficulty];
    if (!nDrain || STATLIST_GetUnitAlignment(pUnit) != UNIT_ALIGNMENT_EVIL)
    {
        return 1;
    }

    int32_t nBaseDamage = damage.dwPhysDamage;
    if (nDrain != 100)
    {
        nBaseDamage = MONSTERUNIQUE_CalculatePercentage(damage.dwPhysDamage, nDrain, 100);
    }

    D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pAttacker);
    if (pOwner && SUNIT_IsDead(pOwner) || SUNIT_IsDead(pAttacker))
    {
        return 1;
    }

    int32_t nDamage = MONSTERUNIQUE_CalculatePercentage(nBaseDamage, 20, 100);
    if (nDamage <= 0)
    {
        return 1;
    }

    if (pOwner)
    {
        int32_t nReducedDamage = MONSTERUNIQUE_CalculatePercentage(nDamage, 50, 100);
        if (nReducedDamage > 0)
        {
            const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pOwner);
            int32_t nNewHp = nReducedDamage + STATLIST_UnitGetStatValue(pOwner, STAT_HITPOINTS, 0);
            if (nNewHp > nMaxHp)
            {
                const int32_t nHpDiff = nMaxHp - nNewHp;
                nNewHp = nMaxHp;
                nReducedDamage += nHpDiff;
            }

            STATLIST_SetUnitStat(pOwner, STAT_HITPOINTS, nNewHp, 0);
        }
        else
        {
            nReducedDamage = 0;
        }
        nDamage -= nReducedDamage;
    }

    int32_t nReducedDamage = nDamage;
    if (nDamage > 0)
    {
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pAttacker);
        int32_t nNewHp = nDamage + STATLIST_UnitGetStatValue(pAttacker, STAT_HITPOINTS, 0);
        if (nNewHp > nMaxHp)
        {
            const int32_t nHpDiff = nMaxHp - nNewHp;
            nNewHp = nMaxHp;
            nReducedDamage = nDamage + nHpDiff;
        }

        STATLIST_SetUnitStat(pAttacker, STAT_HITPOINTS, nNewHp, 0);
    }
    else
    {
        nReducedDamage = 0;
    }

    const int32_t nDamageDiff = nDamage - nReducedDamage;
    if (pOwner && nDamageDiff > 0)
    {
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pOwner);
        int32_t nNewHp = nDamageDiff + STATLIST_UnitGetStatValue(pOwner, STAT_HITPOINTS, 0);
        if (nNewHp > nMaxHp)
        {
            nNewHp = nMaxHp;
        }

        STATLIST_SetUnitStat(pOwner, STAT_HITPOINTS, nNewHp, 0);
    }

    UNITS_SetOverlay(pAttacker, 151, 0);

    if (pOwner)
    {
        UNITS_SetOverlay(pOwner, 151, 0);
    }

    return 1;
}

//D2Game.0x6FD0EDE0
int32_t __fastcall D2GAME_EventFunc05_6FD0EDE0(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit || pAttacker->dwUnitType != UNIT_PLAYER && pAttacker->dwUnitType != UNIT_MONSTER || pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    if (SUNIT_IsDead(pUnit) || !pDamage || pDamage->dwPhysDamage <= 0)
    {
        return 0;
    }
    
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (nSkillLevel < 0)
    {
        nSkillLevel = 1;
    }

    if (pSkillsTxtRecord->wAuraTargetState < 0 || pSkillsTxtRecord->wAuraTargetState >= sgptDataTables->nStatesTxtRecordCount || !STATES_CheckState(pAttacker, pSkillsTxtRecord->wAuraTargetState))
    {
        return 0;
    }
    
    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pAttacker, pSkillsTxtRecord->wAuraTargetState);
    if (!pStatList)
    {
        return 0;
    }

    D2UnitStrc* pOwner = SUNIT_GetServerUnit(pGame, STATLIST_GetOwnerType(pStatList), STATLIST_GetOwnerGUID(pStatList));
    const int32_t nHeal = MONSTERUNIQUE_CalculatePercentage(pDamage->dwPhysDamage, SKILLS_EvaluateSkillFormula(pOwner, pSkillsTxtRecord->dwCalc[0], nSkillId, nSkillLevel), 100);

    const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
    const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);

    if (nHitpoints <= 0 || nMaxHp <= 0)
    {
        return 0;
    }

    int32_t nNewHp = nHeal + nHitpoints;
    if (nNewHp < 1)
    {
        nNewHp = 1;
    }

    if (nNewHp > nMaxHp)
    {
        nNewHp = nMaxHp;
    }

    STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);

    if (pSkillsTxtRecord->wPrgOverlay > 0 && pSkillsTxtRecord->wPrgOverlay < sgptDataTables->nOverlayTxtRecordCount)
    {
        UNITS_SetOverlay(pUnit, pSkillsTxtRecord->wPrgOverlay, 0);
    }

    return 1;
}

//D2Game.0x6FD0F000
int32_t __fastcall D2GAME_EventFunc22_6FD0F000(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pDamage)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (pSkillsTxtRecord->nAuraState >= 0 && pSkillsTxtRecord->nAuraState < sgptDataTables->nStatesTxtRecordCount)
    {
        D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pAttacker, pSkillsTxtRecord->nAuraState);
        if (pStatList)
        {
            int32_t nStat1 = pSkillsTxtRecord->wAuraStat[1];
            if (nStat1 >= 0 && nStat1 < sgptDataTables->nItemStatCostTxtRecordCount)
            {
                int32_t nValue1 = STATLIST_GetStatValue(pStatList, nStat1, 0);
                if (nValue1 > 0)
                {
                    int32_t nStat0 = pSkillsTxtRecord->wAuraStat[0];
                    if (nStat0 >= 0 && nStat0 < sgptDataTables->nItemStatCostTxtRecordCount)
                    {
                        int32_t nValue0 = STATLIST_GetStatValue(pStatList, nStat0, 0);
                        
                        if (nValue0 > 0)
                        {
                            if (nValue0 > pDamage->dwPhysDamage)
                            {
                                nValue0 -= pDamage->dwPhysDamage;
                                pDamage->dwPhysDamage = 0;
                            }
                            else
                            {
                                nValue0 = 0;
                                pDamage->dwPhysDamage -= nValue0;
                            }

                            STATLIST_SetStatIfListIsValid(pStatList, nStat0, nValue0, 0);
                        }

                        if (!nValue0)
                        {
                            STATES_ToggleState(pAttacker, pSkillsTxtRecord->nAuraState, 0);
                            D2Common_10474(pAttacker, pStatList);
                            STATLIST_FreeStatList(pStatList);
                            return 1;
                        }

                        const int32_t nParam = MONSTERUNIQUE_CalculatePercentage(nValue0, 100, nValue1);
                        int32_t nParamDiff = nParam - STATLIST_GetStatValue(pStatList, STAT_UNSENTPARAM1, 0);
                        nParamDiff = std::abs(nParamDiff);

                        if (nParamDiff >= 5)
                        {
                            STATES_ToggleGfxStateFlag(pAttacker, pSkillsTxtRecord->nAuraState, 1);
                            STATLIST_SetStatIfListIsValid(pStatList, STAT_UNSENTPARAM1, nParam, 0);
                        }

                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

//D2Game.0x6FD0F1F0
int32_t __fastcall D2GAME_EventFunc23_6FD0F1F0(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit || !pDamage || pUnit->dwUnitType != UNIT_PLAYER && pUnit->dwUnitType != UNIT_MONSTER)
    {
        return 0;
    }

    int32_t nDrain = 100;
    if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        nDrain = 0;
        if (pUnit->pMonsterData && pUnit->pMonsterData->pMonstatsTxt)
        {
            nDrain = pUnit->pMonsterData->pMonstatsTxt->nDrain[pGame->nDifficulty];
        }

        if (!nDrain)
        {
            return 0;
        }
    }

    if (STATLIST_GetUnitAlignment(pUnit) != UNIT_ALIGNMENT_EVIL)
    {
        return 0;
    }

    int32_t nDamage = pDamage->dwPhysDamage;
    if (nDrain != 100)
    {
        nDamage = MONSTERUNIQUE_CalculatePercentage(nDamage, nDrain, 100);
    }

    const int32_t nHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
    if (nDamage > nHitpoints)
    {
        nDamage = nHitpoints;
    }

    D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pAttacker);
    if (pOwner && SUNIT_IsDead(pOwner) || SUNIT_IsDead(pAttacker))
    {
        return 0;
    }

    const int32_t nPercentage = D2Common_11034(nSkillLevel, nSkillId);
    if (nPercentage <= 0)
    {
        return 0;
    }

    int32_t nBaseDamage = MONSTERUNIQUE_CalculatePercentage(nDamage, nPercentage, 100);
    if (nBaseDamage <= 0)
    {
        return 0;
    }

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    int32_t nParam = 0;
    if (pSkillsTxtRecord)
    {
        nParam = pSkillsTxtRecord->dwParam[2];
    }

    if (pOwner && nParam > 0)
    {
        nBaseDamage -= sub_6FD0F590(pOwner, MONSTERUNIQUE_CalculatePercentage(nBaseDamage, nParam, 100));
    }

    const int32_t nFinalDamage = nBaseDamage - sub_6FD0F590(pAttacker, nBaseDamage);
    if (pOwner && nFinalDamage > 0)
    {
        sub_6FD0F590(pOwner, nFinalDamage);
    }

    UNITS_SetOverlay(pAttacker, 151, 0);

    if (nParam > 0 && pOwner)
    {
        UNITS_SetOverlay(pOwner, 151, 0);
    }

    return 1;
}

//D2Game.0x6FD0F590
int32_t __fastcall sub_6FD0F590(D2UnitStrc* pUnit, int32_t nValue)
{
    if (nValue > 0)
    {
        const int32_t nHitPoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0);
        const int32_t nMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit);
        int32_t nNewHp = nValue + nHitPoints;
        if (nNewHp > nMaxHp)
        {
            nValue += (nMaxHp - nNewHp);
            nNewHp = nMaxHp;
        }

        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nNewHp, 0);
        return nValue;
    }

    return 0;
}

//D2Game.0x6FD0F5E0
int32_t __fastcall D2GAME_EventFunc26_6FD0F5E0(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
	if (!pAttacker || !pDamage || pAttacker->dwUnitType != UNIT_MONSTER || pDamage->dwDmgTotal <= 0)
	{
		return 0;
	}

	D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pAttacker);
	if (SUNIT_IsDead(pOwner))
	{
		return 0;
	}

	const int32_t nHitpoints = STATLIST_UnitGetStatValue(pOwner, STAT_HITPOINTS, 0);
	if (nHitpoints < 256)
	{
		return 0;
	}

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pAttacker->dwClassId);
	if (!pMonStatsTxtRecord || pMonStatsTxtRecord->nSkill[0] < 0)
	{
		return 0;
	}

    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(pMonStatsTxtRecord->nSkill[0]);
    int32_t nPercentage = 0;
    if (pSkillsTxtRecord)
    {
        nPercentage = pSkillsTxtRecord->dwParam[4];
    }

    const int32_t nReducedDamage = MONSTERUNIQUE_CalculatePercentage(pDamage->dwDmgTotal, nPercentage, 100);

    int32_t nNewHp = nHitpoints - nReducedDamage;
    if (nNewHp < 256)
    {
        nNewHp = 256;
    }

    STATLIST_SetUnitStat(pAttacker, STAT_HITPOINTS, nNewHp, 0);

    pDamage->dwDmgTotal -= nReducedDamage;

    if (pDamage->dwDmgTotal < 0)
    {
        pDamage->dwDmgTotal = 0;
    }

    return 1;
}

//D2Game.0x6FD0F7A0
int32_t __fastcall D2GAME_EventFunc27_6FD0F7A0(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage, int32_t nSkillId, int32_t nSkillLevel)
{
    if (!pAttacker || !pUnit)
    {
        return 0;
    }

    int32_t nSlow = STATLIST_UnitGetItemStatOrSkillStatValue(pAttacker, STAT_ITEM_SLOW, 0);
    if (!nSlow)
    {
        return 0;
    }

    if (pUnit->dwUnitType)
    {
        if (pUnit->dwUnitType != UNIT_MONSTER)
        {
            return 0;
        }

        if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, 12u))
        {
            if (nSlow >= 50)
            {
                nSlow = 50;
            }
        }
        else
        {
            if (nSlow >= 90)
            {
                nSlow = 90;
            }
        }
    }
    else
    {
        if (nSlow >= 50)
        {
            nSlow = 50;
        }
    }

    D2CurseStrc pCurse = {};
    pCurse.pUnit = pAttacker;
    pCurse.pTarget = pUnit;
    pCurse.nSkill = 0;
    pCurse.nSkillLevel = 1;
    pCurse.nDuration = 750;
    pCurse.nStat = STAT_VELOCITYPERCENT;
    pCurse.nStatValue = -nSlow;
    pCurse.nState = STATE_SLOWED;

    D2StatListStrc* pStatList = sub_6FD10EC0(&pCurse);
    if (pStatList)
    {
        STATLIST_SetStat(pStatList, STAT_ATTACKRATE, -nSlow, 0);
        STATLIST_SetStat(pStatList, STAT_OTHER_ANIMRATE, -nSlow, 0);
        D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
        return 1;
    }

    return 0;
}
