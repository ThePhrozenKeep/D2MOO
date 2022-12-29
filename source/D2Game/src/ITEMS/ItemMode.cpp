#include "ITEMS/ItemMode.h"

#include <algorithm>

#include <D2BitManip.h>
#include <D2Math.h>

#include <D2Collision.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Inventory.h>
#include <D2ItemMods.h>
#include <D2Items.h>
#include <D2Monsters.h>
#include <D2QuestRecord.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <DataTbls/ItemsTbls.h>
#include <Units/UnitRoom.h>

#include <UselessOrdinals.h>

#include "AI/AiGeneral.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/SCmd.h"
#include "ITEMS/Items.h"
#include "ITEMS/ItemsMagic.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlrTrade.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT5/A5Q3.h"
#include "SKILLS/SkillAss.h"
#include "SKILLS/SkillItem.h"
#include "SKILLS/Skills.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitEvent.h"
#include "UNIT/SUnitMsg.h"


#pragma pack(push, 1)
struct D2ItemUnkStrc
{
    int32_t nIndex;
    D2ItemsTxt* pItemsTxtRecord;
};

struct D2UnkUniqueItemStrc
{
    int32_t nIndex;
    int32_t nAccumulatedRarity;
};
#pragma pack(pop)


bool gbBoxItemIdInitialized;
int32_t gnBoxItemId;

D2UnkUniqueItemStrc stru_6FD45C18[4096];


//D2Game.0x6FC41900
int32_t __fastcall D2GAME_Return1_6FC41900()
{
    return 1;
}

//D2Game.0x6FC41910
void __fastcall D2GAME_ITEMMODE_ServerStatlistCallback_6FC41910(D2GameStrc* pGame, D2UnitStrc* pOwner, D2UnitStrc* pOther, int32_t nLayer_StatId, int32_t nPreviousValue, int32_t nNewValue)
{
	if (!pGame || !pOwner)
	{
		return;
	}

    const int32_t nStatId = nLayer_StatId >> 16;
    D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
    if (!pItemStatCostTxtRecord)
    {
        return;
    }

    int32_t nOtherUnitGUID = 0;
    if (pOther)
    {
        nOtherUnitGUID = pOther->dwUnitId;
    }

    if (pItemStatCostTxtRecord->wItemEvent[0] > 0)
    {
        if (nNewValue)
        {
            if (!SUNITEVENT_GetTimer(pGame, pOwner, 2, nLayer_StatId, nLayer_StatId))
            {
                sub_6FD156A0(pGame, pOwner, pItemStatCostTxtRecord->wItemEvent[0], nLayer_StatId, 0, pItemStatCostTxtRecord->wItemEventFunc[0], 2, nLayer_StatId);
                if (pItemStatCostTxtRecord->wItemEvent[1] > 0)
                {
                    sub_6FD156A0(pGame, pOwner, pItemStatCostTxtRecord->wItemEvent[1], nLayer_StatId, 0, pItemStatCostTxtRecord->wItemEventFunc[1], 2, nLayer_StatId);
                }
            }
        }
        else
        {
            SUNITEVENT_FreeTimer(pGame, pOwner, 2, nLayer_StatId);
        }
    }

    switch (nStatId)
    {
    case STAT_MAXHP:
    {
        if (nNewValue != nPreviousValue && nPreviousValue > 0)
        {
            const int32_t nHitpoints = STATLIST_GetUnitStatUnsigned(pOwner, STAT_HITPOINTS, 0);
            if (nHitpoints > 0)
            {
                nPreviousValue = std::max(nPreviousValue, 256);

                const int32_t nNewHp = D2Clamp((int32_t)(int64_t)((double)nHitpoints / (double)nPreviousValue * (double)nNewValue), 1, nNewValue);
                STATLIST_SetUnitStat(pOwner, STAT_HITPOINTS, nNewHp, 0);

                if (pOwner->dwUnitType == UNIT_MONSTER && pOwner->pMonsterData)
                {
                    D2MonStatsTxt* pMonStatsTxtRecord = pOwner->pMonsterData->pMonstatsTxt;
                    if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwDamageRegen)
                    {
                        STATLIST_SetUnitStat(pOwner, STAT_HPREGEN, (int32_t)(pMonStatsTxtRecord->dwDamageRegen * (nNewValue >> 8)) >> 4, 0);
                    }
                }
            }
        }
        break;
    }
    case STAT_MAXMANA:
    {
        if (nNewValue != nPreviousValue && nPreviousValue > 0)
        {
            const int32_t nMana = STATLIST_GetUnitStatUnsigned(pOwner, STAT_MANA, 0);
            if (nMana > 0)
            {
                nPreviousValue = std::max(nPreviousValue, 256);

                const int32_t nNewMana = D2Clamp((int32_t)(int64_t)((double)nMana / (double)nPreviousValue * (double)nNewValue), 1, nNewValue);
                STATLIST_SetUnitStat(pOwner, STAT_MANA, nNewMana, 0);
            }
        }
        break;
    }
    case STAT_MAXSTAMINA:
    {
        if (nNewValue != nPreviousValue && nPreviousValue > 0)
        {
            int32_t nStamina = STATLIST_GetUnitStatUnsigned(pOwner, STAT_STAMINA, 0);
            if (nStamina > 0)
            {
                nPreviousValue = std::max(nPreviousValue, 256);

                const int32_t nNewStamina = D2Clamp((int32_t)(int64_t)((double)nStamina / (double)nPreviousValue * (double)nNewValue), 1, nNewValue);
                STATLIST_SetUnitStat(pOwner, STAT_STAMINA, nNewStamina, 0);
            }
        }
        break;
    }
    case STAT_ITEM_NONCLASSSKILL:
    case STAT_ITEM_SINGLESKILL:
    {
        const int32_t nSkillId = (uint16_t)nLayer_StatId;
        D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);

        if (nSkillId && (nStatId != STAT_ITEM_SINGLESKILL || pOwner->dwUnitType == UNIT_PLAYER && pOwner->dwClassId == pSkillsTxtRecord->nCharClass))
        {
            D2SkillStrc* pSkill = SKILLS_GetSkillById(pOwner, nSkillId, -1);
            if (!pSkill)
            {
                pSkill = SKILLS_AddSkill(pOwner, nSkillId);
                SKILLS_AssignSkill(pOwner, nSkillId, 0, 0, __FILE__, __LINE__);
                D2GAME_PACKETS_SendPacket0x21_UpdateSkills_6FC3DB50(SUNIT_GetClientFromPlayer(pOwner, __FILE__, __LINE__), pOwner, nSkillId, 0, 0);
            }

            SKILLS_RefreshSkill(pOwner, nSkillId);
            sub_6FD108D0(pGame, pOwner, nSkillId);

            if (nNewValue <= 0)
            {
                if (!SKILLS_GetSkillLevel(pOwner, pSkill, 0))
                {
                    SKILLS_AssignSkill(pOwner, nSkillId, 0, 1, __FILE__, __LINE__);
                }

                if (!SKILLS_GetSkillLevel(pOwner, pSkill, 1))
                {
                    if (UNITS_GetLeftSkill(pOwner) == pSkill)
                    {
                        D2GAME_AssignSkill_6FD13800(pOwner, 1, 0, -1);
                    }

                    if (UNITS_GetRightSkill(pOwner) == pSkill)
                    {
                        D2GAME_AssignSkill_6FD13800(pOwner, 0, 0, -1);
                    }
                }
            }
        }
        break;
    }
    case STAT_ITEM_ADDCLASSSKILLS:
    {
        const uint32_t nSkillClassId = (uint16_t)nLayer_StatId;
        if (pOwner->dwUnitType != UNIT_PLAYER || pOwner->dwClassId != nSkillClassId)
        {
            if (MONSTERS_GetHirelingTypeId(pOwner))
            {
                D2GAME_RefreshPassiveSkills_6FD14CC0(pOwner);
            }
            else
            {
                int32_t nOwnerUnitType = pOwner->dwUnitType;
                int32_t nOwnerClassId = pOwner->dwClassId;
                int32_t nAnimMode = pOwner->dwAnimMode;
                D2COMMON_11013_ConvertMode(pOwner, &nOwnerUnitType, &nOwnerClassId, &nAnimMode, __FILE__, __LINE__);
                if (nOwnerUnitType == UNIT_PLAYER && nOwnerClassId == nSkillClassId)
                {
                    D2GAME_RefreshPassiveSkills_6FD14CC0(pOwner);
                }
            }
        }
        else
        {
            D2GAME_RefreshPassiveSkills_6FD14CC0(pOwner);
        }
        break;
    }
    case STAT_ITEM_ADDSKILL_TAB:
    {
        const uint32_t nSkillClassId = (uint16_t)nLayer_StatId >> 3;
        if (pOwner->dwUnitType != UNIT_PLAYER || pOwner->dwClassId != nSkillClassId)
        {
            if (MONSTERS_GetHirelingTypeId(pOwner))
            {
                D2GAME_RefreshPassiveSkills_6FD14CC0(pOwner);
            }
            else
            {
                int32_t nOwnerUnitType = pOwner->dwUnitType;
                int32_t nOwnerClassId = pOwner->dwClassId;
                int32_t nAnimMode = pOwner->dwAnimMode;
                D2COMMON_11013_ConvertMode(pOwner, &nOwnerUnitType, &nOwnerClassId, &nAnimMode, __FILE__, __LINE__);
                if (nOwnerUnitType == UNIT_PLAYER && nOwnerClassId == nSkillClassId)
                {
                    D2GAME_RefreshPassiveSkills_6FD14CC0(pOwner);
                }
            }
        }
        else
        {
            D2GAME_RefreshPassiveSkills_6FD14CC0(pOwner);
        }
        break;
    }
    case STAT_ITEM_ELEMSKILL:
    case STAT_ITEM_ALLSKILLS:
    {
        D2GAME_RefreshPassiveSkills_6FD14CC0(pOwner);
        break;
    }
    case STAT_ITEM_AURA:
    {
        if (nNewValue)
        {
            SKILLITEM_ActivateAura(pGame, pOwner, nOtherUnitGUID, (uint16_t)nLayer_StatId, nNewValue);
        }
        else
        {
            SKILLITEM_DeactivateAura(pGame, pOwner, nOtherUnitGUID, (uint16_t)nLayer_StatId);
        }
        break;
    }
    case STAT_ITEM_CHARGED_SKILL:
    {
        if (pOther)
        {
            const uint16_t nLayer = (uint16_t)nLayer_StatId;
            const int32_t nSkillLevel = nLayer & sgptDataTables->nShiftedStuff;
            const int32_t nSkillId = nLayer >> sgptDataTables->nStuff;
            const int32_t nCharges = (uint8_t)STATLIST_GetUnitStatUnsigned(pOther, nStatId, nLayer);
            if (nCharges <= 0 || STATLIST_GetOwner(pOther, nullptr) != pOwner)
            {
                D2Common_10954(pOwner, nOtherUnitGUID, nSkillId, nSkillLevel, 0, 1);
                sub_6FD108D0(pGame, pOwner, nSkillId);
            }
            else
            {
                D2Common_10954(pOwner, nOtherUnitGUID, nSkillId, nSkillLevel, nCharges, 0);
            }
        }
        break;
    }
    case STAT_STATE:
    {
        const uint16_t nStateId = (uint16_t)nLayer_StatId;
        if (nStateId >= 0 && nStateId < sgptDataTables->nStatesTxtRecordCount)
        {
            if (nNewValue)
            {
                if (!STATES_CheckState(pOwner, nStateId))
                {
                    sub_6FD11C90(pOwner, nStateId, 0);
                    STATES_ToggleState(pOwner, nStateId, 1);
                    STATES_ToggleGfxStateFlag(pOwner, nStateId, 1);
                }
            }
            else
            {
                if (STATES_CheckState(pOwner, nStateId))
                {
                    sub_6FD11C90(pOwner, nStateId, 1);
                    STATES_ToggleState(pOwner, nStateId, 0);
                    STATES_ToggleGfxStateFlag(pOwner, nStateId, 1);
                }
            }
        }
        break;
    }
    default:
        return;
    }
}

//D2Game.0x6FC42050
void __fastcall sub_6FC42050(D2UnitStrc* pItem, D2ClientStrc* pClient)
{
    if (!pItem || pItem->dwFlags & UNITFLAG_INITSEEDSET || pItem->dwAnimMode != IMODE_ONGROUND)
    {
        return;
    }

    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (pItem->dwFlags & UNITFLAG_SENDREFRESHMSG)
    {
        D2GAME_SendP0x9C_ItemAction_DropToGround_6FC3E8E0(pClient, pPlayer, pItem, 0);
    }
    else
    {
        D2GAME_SendP0x9C_ItemAction_OnGround_6FC3E930(pClient, pPlayer, pItem, 0);
    }
}

//D2Game.0x6FC420B0
void __fastcall sub_6FC420B0(D2UnitStrc* pItem, D2ClientStrc* pClient)
{
    if (!pItem || !(pItem->dwFlags & UNITFLAG_DOUPDATE) || pItem->dwFlags & UNITFLAG_INITSEEDSET || pItem->dwAnimMode != IMODE_ONGROUND)
    {
        return;
    }

    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (pItem->dwFlags & UNITFLAG_SENDREFRESHMSG)
    {
        D2GAME_SendP0x9C_ItemAction_DropToGround_6FC3E8E0(pClient, pPlayer, pItem, 0);
    }
    else
    {
        D2GAME_SendP0x9C_ItemAction_OnGround_6FC3E930(pClient, pPlayer, pItem, 0);
    }
}

//D2Game.0x6FC42120
int32_t __fastcall sub_6FC42120(D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t a3)
{
    D2_ASSERT(pItem);

    if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL) && !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
    {
        return 0;
    }
    
    D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
    if (!pBooksTxtRecord)
    {
        return 0;
    }

    int32_t nSkillId = -1;
    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
    {
        nSkillId = pBooksTxtRecord->dwScrollSkillId;
    }
    else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
    {
        nSkillId = pBooksTxtRecord->dwBookSkillId;
    }

    if (nSkillId == -1)
    {
        return 0;
    }

    int32_t nQuantity = STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0);
    if (!nQuantity)
    {
        if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
        {
            return 0;
        }

        nQuantity = 1;
    }

    D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, -1u);
    if (a3)
    {
        if (pSkill)
        {
            nQuantity += SKILLS_GetQuantity(pSkill);
        }
        else
        {
            STATLIST_AddUnitStat(pUnit, STAT_SKILLPTS, nQuantity, 0);
            sub_6FD136E0(pUnit, nSkillId);

            pSkill = SKILLS_GetSkillById(pUnit, nSkillId, -1u);
            D2_ASSERT(pSkill);
        }
    }
    else
    {
        D2_ASSERT(pSkill);

        nQuantity = SKILLS_GetQuantity(pSkill) - nQuantity;
        if (nQuantity < 0)
        {
            nQuantity = 0;
        }
    }

    SKILLS_SetQuantity(pSkill, nQuantity);

    int32_t nUnitGUID = -1;
    int32_t nUnitType = 6;
    if (pUnit)
    {
        nUnitGUID = pUnit->dwUnitId;
        nUnitType = pUnit->dwUnitType;
    }

    D2GAME_PACKETS_SendPacket0x22_6FC3DBE0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), nUnitType, nUnitGUID, nSkillId, nQuantity);

    if (!nQuantity)
    {
        if (UNITS_GetLeftSkill(pUnit) == pSkill)
        {
            D2GAME_AssignSkill_6FD13800(pUnit, 1, 0, -1);
        }

        if (UNITS_GetRightSkill(pUnit) == pSkill)
        {
            D2GAME_AssignSkill_6FD13800(pUnit, 0, 0, -1);
        }
    }

    return 1;
}

//D2Game.0x6FC42310
void __fastcall D2GAME_ITEMS_UpdateItemStatlist_6FC42310(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pUnit, int32_t a4)
{
    if (!pUnit || !pItem)
    {
        return;
    }

    //if (pItem->dwUnitType != UNIT_ITEM)
    //{
    //    Fog_10030(&unk_6FD447EC, "[sITEMSAddStatsList] tried to add stats for deleted/non item");
    //    return;
    //}

    if (pUnit->dwUnitType == UNIT_PLAYER && STATES_CheckState(pUnit, STATE_PLAYERBODY))
    {
        return;
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_GEM))
    {
        if (pUnit->dwUnitType != UNIT_ITEM)
        {
            return;
        }

        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
        if (!pItemsTxtRecord)
        {
            return;
        }

        D2GemsTxt* pGemsTxtRecord = DATATBLS_GetGemsTxtRecord(pItemsTxtRecord->dwGemOffset);
        if (!pGemsTxtRecord)
        {
            return;
        }

        ITEMMODS_AssignProperty(2, 0, pItem, pGemsTxtRecord, ITEMS_GetGemApplyType(pUnit), 0);
    }
    else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_RUNE))
    {
        if (pUnit->dwUnitType != UNIT_ITEM)
        {
            return;
        }

        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
        if (!pItemsTxtRecord)
        {
            return;
        }

        D2GemsTxt* pGemsTxtRecord = DATATBLS_GetGemsTxtRecord(pItemsTxtRecord->dwGemOffset);
        if (!pGemsTxtRecord)
        {
            return;
        }

        ITEMMODS_AssignProperty(5, pUnit, pItem, pGemsTxtRecord, ITEMS_GetGemApplyType(pUnit), 0);
    }
    else
    {
        if (ITEMS_GetItemQuality(pItem) == ITEMQUAL_SET)
        {
            ITEMS_UpdateSets(pUnit, pItem, 0, 0);
        }
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pUnit, pItem, 1);
    }

    D2UnitStrc* pMainWeapon = 0;
    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        pMainWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
    }

    int32_t bIsMainWeapon = 1;
    if (UNITS_CanDualWield(pUnit))
    {
        if (pItem != pMainWeapon && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
        {
            bIsMainWeapon = 0;
        }

        if (!ITEMS_CheckItemFlag(pItem, IFLAG_BROKEN, __LINE__, __FILE__) || a4)
        {
            STATLIST_MergeStatLists(pUnit, pItem, bIsMainWeapon);
        }
    }
    else
    {
        if (!ITEMS_CheckItemFlag(pItem, IFLAG_BROKEN, __LINE__, __FILE__) || a4)
        {
            STATLIST_MergeStatLists(pUnit, pItem, bIsMainWeapon);
        }
    }

    if (pUnit->pDynamicPath)
    {
        D2COMMON_10376_UpdateAnimRateAndVelocity(pUnit, __FILE__, __LINE__);
    }
}

//D2Game.0x6FC424E0
void __fastcall D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pTarget, int32_t bUpdateStatList, int32_t bUpdateVitals)
{
    if (!pTarget || !pItem || pItem->dwUnitType != UNIT_ITEM || (pTarget->dwUnitType == UNIT_PLAYER && STATES_CheckState(pTarget, STATE_PLAYERBODY)) || pTarget != STATLIST_GetOwner(pItem, nullptr))
    {
        return;
    }

    if (ITEMS_GetItemQuality(pItem) == ITEMQUAL_SET)
    {
        ITEMS_UpdateSets(pTarget, pItem, 1, 0);
    }

    D2UnitStrc* pWeapon = nullptr;
    if (pTarget->dwUnitType == UNIT_PLAYER)
    {
        pWeapon = INVENTORY_GetLeftHandWeapon(pTarget->pInventory);
    }

    if (!ITEMS_CheckItemFlag(pItem, IFLAG_BROKEN, __LINE__, __FILE__) || bUpdateStatList)
    {
        STATLIST_ExpireUnitStatlist(pTarget, pItem);
    }

    if (UNITS_CanDualWield(pTarget) && pWeapon && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON) && pWeapon != pItem
        && (!ITEMS_CheckItemFlag(pWeapon, IFLAG_BROKEN, __LINE__, __FILE__) || bUpdateStatList) && STATLIST_GetOwner(pWeapon, nullptr))
    {
        STATLIST_MergeStatLists(pTarget, pWeapon, 1);
    }

    if (pTarget->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pTarget, pItem, 0);
    }

    if (bUpdateVitals)
    {
        STATLIST_ClampStaminaManaHP(pTarget);
    }
}

//D2Game.0x6FC425F0
int32_t __fastcall sub_6FC425F0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    constexpr int nQuestIds[8] =
    {
        QUESTSTATEFLAG_A1Q3, QUESTSTATEFLAG_A1Q4,
        QUESTSTATEFLAG_A3Q2, QUESTSTATEFLAG_A3Q1,
        QUESTSTATEFLAG_A3Q3, QUESTSTATEFLAG_A3Q2,
        QUESTSTATEFLAG_A4Q3, QUESTSTATEFLAG_A4Q1
    };

    D2UnitStrc* pPreviousItem = INVENTORY_GetFirstItem(pUnit->pInventory);
    if (!pUnit->pInventory)
    {
        return 0;
    }
        
    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
    if (!pItemsTxtRecord)
    {
        return 0;
    }
    
    if (pItem && pItem->dwUnitType == UNIT_ITEM && ITEMS_GetItemQuality(pItem) == ITEMQUAL_UNIQUE)
    {
        D2UniqueItemsTxt* pUniqueItemsTxtRecord = ITEMS_GetUniqueItemsTxtRecord(ITEMS_GetFileIndex(pItem));
        if (pUniqueItemsTxtRecord && pUniqueItemsTxtRecord->dwUniqueItemFlags & gdwBitMasks[UNIQUEITEMSFLAGINDEX_CARRY1] && sub_6FC428F0(pItem, pItemsTxtRecord, pPreviousItem))
        {
            return 0;
        }
    }

    if (!pItemsTxtRecord->nQuest)
    {
        return 1;
    }

    const uint32_t nItemCode = pItemsTxtRecord->dwCode;
    if (nItemCode == ' ssa')
    {
        if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q1, QFLAG_CUSTOM1))
        {
            return 0;
        }
    }
    else if (nItemCode == ' 43j')
    {
        if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A3Q4, QFLAG_REWARDGRANTED))
        {
            return 0;
        }
    }
    else if (nItemCode == ' zyx')
    {
        if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A3Q4, QFLAG_CUSTOM1))
        {
            return 0;
        }
    }
    else if (nItemCode == ' 33g')
    {
        if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM3))
        {
            return 0;
        }

        if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A3Q3, QFLAG_CUSTOM4))
        {
            return 0;
        }
    }
    else if (nItemCode == ' 2rt')
    {
        if (!QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM4))
        {
            return 0;
        }

        if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM3))
        {
            return 0;
        }
    }

    if (pItemsTxtRecord->nQuest == QUESTSTATEFLAG_A2Q2)
    {
        if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A2Q2, QFLAG_REWARDGRANTED) && pItemsTxtRecord->dwCode != ' xob')
        {
            return 0;
        }
    }
    else if (pItemsTxtRecord->nQuest == QUESTSTATEFLAG_A1Q5)
    {
        if (QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], QUESTSTATEFLAG_A1Q4, QFLAG_REWARDGRANTED) && pItemsTxtRecord->dwCode != ' gel')
        {
            return 0;
        }
    }
    else
    {
        for (int32_t i = 0; i < 4; ++i)
        {
            if (pItemsTxtRecord->nQuest == nQuestIds[2 * i + 1] && QUESTRECORD_GetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], nQuestIds[2 * i], QFLAG_REWARDGRANTED))
            {
                return 0;
            }
        }
    }

    if (sub_6FC428F0(pItem, pItemsTxtRecord, pPreviousItem))
    {
        return 0;
    }

    for (D2CorpseStrc* pCorpse = INVENTORY_GetFirstCorpse(pUnit->pInventory); pCorpse; pCorpse = INVENTORY_GetNextCorpse(pCorpse))
    {
        D2UnitStrc* pPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, INVENTORY_GetUnitGUIDFromCorpse(pCorpse));
        if (pPlayer && pPlayer->pInventory)
        {
            D2UnitStrc* pFirstItem = INVENTORY_GetFirstItem(pPlayer->pInventory);
            if (pFirstItem && sub_6FC428F0(pItem, pItemsTxtRecord, pFirstItem))
            {
                return 0;
            }
        }
    }

    return 1;
}

//D2Game.0x6FC428F0
int32_t __fastcall sub_6FC428F0(D2UnitStrc* pItem, D2ItemsTxt* pItemsTxtRecordArg, D2UnitStrc* pPreviousItem)
{
    int32_t nUniqueId = -1;
    if (pItem && pItem->dwUnitType == UNIT_ITEM && ITEMS_GetItemQuality(pItem) == ITEMQUAL_UNIQUE)
    {
        D2UniqueItemsTxt* pUniqueItemsTxtRecord = ITEMS_GetUniqueItemsTxtRecord(ITEMS_GetFileIndex(pItem));
        if (pUniqueItemsTxtRecord && pUniqueItemsTxtRecord->dwUniqueItemFlags & gdwBitMasks[UNIQUEITEMSFLAGINDEX_CARRY1])
        {
            nUniqueId = pUniqueItemsTxtRecord->wId;
        }
    }

    D2UnitStrc* pCurrent = pPreviousItem;
    while (pCurrent)
    {
        D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(pCurrent);
        D2UnitStrc* pNext = INVENTORY_GetNextItem(pCurrent);
        if (pCheckedItem == pItem)
        {
            break;
        }

        if (pCheckedItem)
        {
            if (nUniqueId >= 0 && ITEMS_GetInvPage(pCheckedItem) != 1 && ITEMS_GetItemQuality(pCheckedItem) == ITEMQUAL_UNIQUE && ITEMS_GetFileIndex(pCheckedItem) == nUniqueId)
            {
                return 1;
            }

            D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pCheckedItem->dwClassId);
            if (pItemsTxtRecord->nQuest && ITEMS_GetInvPage(pCheckedItem) != 1 && pItemsTxtRecord->nQuest == pItemsTxtRecordArg->nQuest)
            {
                uint32_t nCode1 = pItemsTxtRecordArg->dwCode;
                uint32_t nCode2 = pItemsTxtRecord->dwCode;
                if (nCode1 == nCode2 
                    || nCode1 == ' 43j' && nCode2 == ' 43g' || nCode1 == ' 43g' && nCode2 == ' 43j'
                    || nCode1 == ' skb' && nCode2 == ' dkb' || nCode1 == ' dkb' && nCode2 == ' skb'
                    || nCode1 == ' 33d' && nCode2 == ' 33g' || nCode1 == ' 33g' && nCode2 == ' 33d'
                    || nCode1 == ' tsh' && nCode2 == ' fsm' || nCode1 == ' fsm' && nCode2 == ' tsh'
                    || nCode1 == ' tsh' && nCode2 == ' piv' || nCode1 == ' piv' && nCode2 == ' tsh'
                    || nCode1 == ' 2fq' && nCode2 == ' 1fq' || nCode1 == ' 1fq' && nCode2 == ' 2fq'
                    || nCode1 == ' 2fq' && nCode2 == ' rhq' || nCode1 == ' rhq' && nCode2 == ' 2fq'
                    || nCode1 == ' 2fq' && nCode2 == ' yeq' || nCode1 == ' yeq' && nCode2 == ' 2fq'
                    || nCode1 == ' 2fq' && nCode2 == ' rbq' || nCode1 == ' rbq' && nCode2 == ' 2fq')
                {
                    return 1;
                }
            }
        }

        pCurrent = pNext;
    }

    return 0;
}

//D2Game.0x6FC42B80
void __fastcall D2GAME_PickupItemEx_6FC42B80(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t* a4)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a4 = 0;

    D2InventoryStrc* pInventory = pUnit->pInventory;
    D2_ASSERT(pInventory);

    if (INVENTORY_GetCursorItem(pInventory) || PLAYER_IsBusy(pUnit))
    {
        return;
    }

    for (D2UnitStrc* pItem = INVENTORY_GetFirstItem(pInventory); pItem; pItem = INVENTORY_GetNextItem(pItem))
    {
        if (INVENTORY_UnitIsItem(pItem) && ITEMS_CheckItemFlag(pItem, IFLAG_TARGETING, __LINE__, __FILE__))
        {
            ITEMS_SetItemFlag(pItem, IFLAG_TARGETING, 0);

            if (pUnit->dwUnitType == UNIT_PLAYER)
            {
                D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
                D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(pClient, pItem, -1, 1, 0, -1);
            }
        }
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem || pItem->dwUnitType != UNIT_ITEM)
    {
        return;
    }

    if (pItem->dwAnimMode == IMODE_ONGROUND)
    {
        if (sub_6FC425F0(pGame, pUnit, pItem))
        {
            DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pItem), pItem->dwUnitType, pItem->dwUnitId);

            if (ITEMS_GetItemType(pItem) == ITEMTYPE_GOLD)
            {
                D2GAME_PickupGold_6FC42DD0(pGame, pUnit, pItem);
            }
            else
            {
                UNITS_FreeCollisionPath(pItem);
                UNITROOM_RemoveUnitFromRoom(pItem);
                pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
                INVENTORY_SetCursorItem(pInventory, pItem);
                UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
                ITEMS_SetItemCMDFlag(pItem, 64, 1);
                ITEMS_SetInvPage(pItem, 0xFFu);
                UNITS_RefreshInventory(pUnit, 1);
                pItem->dwFlags &= ~UNITFLAG_ISRESURRECT;
                INVENTORY_AddItemToTradeInventory(pInventory, pItem);
                QUESTS_ItemPickedUp(pGame, pUnit, pItem);
            }

            SUNIT_AttachSound(pUnit, 1u, pUnit);
        }
        else
        {
            DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pItem), pItem->dwUnitType, pItem->dwUnitId);
            ITEMS_SetInvPage(pItem, 0xFFu);
            pItem->dwFlags |= UNITFLAG_SENDREFRESHMSG;
            UNITS_ChangeAnimMode(pItem, IMODE_ONGROUND);
            SUNIT_AttachSound(pUnit, 0x13u, pUnit);
        }
    }
    else
    {
        *a4 = 1;
    }
}

//D2Game.0x6FC42DD0
void __fastcall D2GAME_PickupGold_6FC42DD0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pGoldPile)
{
    const uint32_t nInventoryLimit = UNITS_GetInventoryGoldLimit(pUnit);
    const uint32_t nUnitGold = STATLIST_GetUnitStatUnsigned(pUnit, STAT_GOLD, 0);
    const uint32_t nPileGold = STATLIST_GetUnitStatUnsigned(pGoldPile, STAT_GOLD, 0);
    uint32_t nRemainingGold = 0;
   
    uint32_t nPickedGold = nPileGold;
    if (nPileGold + nUnitGold > nInventoryLimit)
    {
        nPickedGold = nInventoryLimit - nUnitGold;
        nRemainingGold = nPileGold - nPickedGold;
    }

    D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pGoldPile);
    if (pOwner && pOwner->dwUnitType == UNIT_PLAYER || SUNIT_GetPartyId(pUnit) == -1)
    {
        PLRTRADE_AddGold(pUnit, STAT_GOLD, nPickedGold);
    }
    else
    {
        nRemainingGold = PARTY_ShareGoldDrop(pGame, pUnit, nPileGold);
    }

    int32_t nUnitGUID = -1;
    int32_t nUnitType = 6;
    if (pGoldPile)
    {
        nUnitGUID = pGoldPile->dwUnitId;
        nUnitType = pGoldPile->dwUnitType;
    }

    DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pGoldPile), nUnitType, nUnitGUID);
    UNITS_FreeCollisionPath(pGoldPile);
    UNITROOM_RemoveUnitFromRoom(pGoldPile);

    if (pGoldPile)
    {
        pGoldPile->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    ITEMS_RemoveFromAllPlayers(pGame, pGoldPile);

    if (nRemainingGold > 0)
    {
        ITEMS_DropGoldPile(pGame, pUnit, nRemainingGold);
        SUNIT_AttachSound(pUnit, 0x17u, pUnit);
    }
}

//D2Game.0x6FC42F20
int32_t __fastcall sub_6FC42F20(D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t* pBodyLoc, int32_t bSkipRequirementCheck)
{
    D2_ASSERT(pUnit);
    D2_ASSERT(pItem);

    if (!bSkipRequirementCheck && !ITEMS_CheckRequirements(pItem, pUnit, 0, nullptr, nullptr, nullptr))
    {
        return 0;
    }

    if (!ITEMS_IsBodyItem(pItem) || !ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, __FILE__) || !ITEMS_CanBeEquipped(pItem) || ITEMS_GetItemType(pItem) == ITEMTYPE_MISSILE_POTION)
    {
        return 0;
    }

    D2UnitStrc* pRightArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
    D2UnitStrc* pLeftArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
    if (ITEMS_GetQuiverTypeFromItemType(ITEMS_GetItemType(pItem)))
    {
        if (!pRightArmItem || !ITEMS_CheckItemTypeId(pItem, ITEMS_GetAmmoType(pRightArmItem)))
        {
            if (!pLeftArmItem || !ITEMS_CheckItemTypeId(pItem, ITEMS_GetAmmoType(pLeftArmItem)))
            {
                return 0;
            }
        }
    }

    uint8_t nBodyLoc1 = 0;
    uint8_t nBodyLoc2 = 0;
    ITEMS_GetAllowedBodyLocations(pItem, &nBodyLoc1, &nBodyLoc2);

    if (nBodyLoc1 == nBodyLoc2)
    {
        if (INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc1))
        {
            return 0;
        }

        *pBodyLoc = nBodyLoc1;
        return 1;
    }

    D2ItemModeArgStrc itemModeArg1 = {};
    D2ItemModeArgStrc itemModeArg2 = {};
    sub_6FC43160(pUnit, pItem, &itemModeArg1);

    D2UnitStrc* pItem1 = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc1);
    D2UnitStrc* pItem2 = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc2);
    if (pItem1)
    {
        if (pItem2)
        {
            return 0;
        }
        
        sub_6FC43160(pUnit, pItem1, &itemModeArg2);
        
        if (!sub_6FC43280(&itemModeArg1, &itemModeArg2))
        {
            return 0;
        }

        *pBodyLoc = nBodyLoc2;
        return 1;
    }
    else if (pItem2)
    {
        sub_6FC43160(pUnit, pItem2, &itemModeArg2);

        if (!sub_6FC43280(&itemModeArg1, &itemModeArg2))
        {
            return 0;
        }
    }

    *pBodyLoc = nBodyLoc1;
    return 1;
}

//D2Game.0x6FC43160
void __fastcall sub_6FC43160(D2UnitStrc* pUnit, D2UnitStrc* pItem, D2ItemModeArgStrc* pItemModeArg)
{
    D2_ASSERT(pItem);
    D2_ASSERT(pUnit);

    memset(pItemModeArg, 0x00, sizeof(D2ItemModeArgStrc));

    if (pUnit->dwUnitType == UNIT_PLAYER && pUnit->dwClassId == PCLASS_BARBARIAN)
    {
        pItemModeArg->bCanUse2Weapons = 1;
    }

    if (pUnit->dwUnitType == UNIT_PLAYER && pUnit->dwClassId == PCLASS_ASSASSIN && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_HAND_TO_HAND))
    {
        pItemModeArg->bCanUse2Weapons = 1;
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOW))
    {
        pItemModeArg->bIsBow = 1;
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_CROSSBOW))
    {
        pItemModeArg->bIsCrossBow = 1;
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOW_QUIVER))
    {
        pItemModeArg->bIsBowQuiver = 1;
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_CROSSBOW_QUIVER))
    {
        pItemModeArg->bIsCrossBowQuiver = 1;
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANY_SHIELD))
    {
        pItemModeArg->bIsShield = 1;
    }

    if (ITEMS_CheckWeaponIfTwoHanded(pItem))
    {
        pItemModeArg->bIsTwoHanded = 1;
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
    {
        pItemModeArg->bIsWeapon = 1;
    }

    if (ITEMS_CheckIfThrowable(pItem) || STATLIST_GetUnitStatSigned(pItem, STAT_ITEM_THROWABLE, 0))
    {
        pItemModeArg->bIsThrowable = 1;
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_RING))
    {
        pItemModeArg->bIsRing = 1;
    }
}

//D2Game.0x6FC43280
int32_t __fastcall sub_6FC43280(D2ItemModeArgStrc* pArg1, D2ItemModeArgStrc* pArg2)
{
    if (pArg1->bIsBow && pArg2->bIsBowQuiver || pArg1->bIsBowQuiver && pArg2->bIsBow)
    {
        return 1;
    }

    if (pArg1->bIsCrossBow && pArg2->bIsCrossBowQuiver || pArg1->bIsCrossBowQuiver && pArg2->bIsCrossBow)
    {
        return 1;
    }

    if (pArg1->bIsWeapon && !pArg1->bIsTwoHanded && pArg2->bIsShield || pArg2->bIsWeapon && !pArg2->bIsTwoHanded && pArg1->bIsShield)
    {
        return 1;
    }

    if (pArg1->bIsWeapon && !pArg1->bIsTwoHanded && pArg2->bIsWeapon && !pArg2->bIsTwoHanded && pArg1->bCanUse2Weapons && pArg2->bCanUse2Weapons)
    {
        return 1;
    }

    if (pArg1->bIsRing && pArg2->bIsRing)
    {
        return 1;
    }

    return 0;
}

//D2Game.0x6FC43340
int32_t __fastcall D2GAME_PickupItem_6FC43340(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t* a4)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a4 = 0;

    D2_ASSERT(pUnit->pInventory);

    if (INVENTORY_GetCursorItem(pUnit->pInventory) || PLAYER_IsBusy(pUnit))
    {
        return 0;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        *a4 = 1;
        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pItem->dwAnimMode != IMODE_ONGROUND)
    {
        *a4 = 1;
        return 0;
    }

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    if (!sub_6FC425F0(pGame, pUnit, pItem))
    {
        DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pItem), pItem->dwUnitType, pItem->dwUnitId);
        ITEMS_SetInvPage(pItem, 0xFFu);
        pItem->dwFlags |= UNITFLAG_SENDREFRESHMSG;
        UNITS_ChangeAnimMode(pItem, IMODE_ONGROUND);
        SUNIT_AttachSound(pUnit, 0x13u, pUnit);
        return 0;
    }

    SUNIT_AttachSound(pUnit, 1u, pUnit);

    if (ITEMS_GetItemType(pItem) == ITEMTYPE_GOLD)
    {
        D2GAME_PickupGold_6FC42DD0(pGame, pUnit, pItem);
        return 1;
    }

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
    {
        D2UnitStrc* pBook = INVENTORY_FindFillableBook(pUnit->pInventory, pItem, 0);
        if (pBook)
        {
            int32_t nUnused = 0;
            if (sub_6FC49AE0(pGame, pUnit, pItem->dwUnitId, pBook->dwUnitId, &nUnused))
            {
                return 0;
            }
        }
    }
    else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
    {
        if (sub_6FC43BF0(pGame, pUnit, pItem))
        {
            return 0;
        }
    }
    else if (ITEMS_CheckIfStackable(pItem) && ITEMS_GetAutoStack(pItem))
    {
        if (sub_6FC437F0(pGame, pUnit, pItem))
        {
            return 0;
        }
    }

    int32_t nBodyLoc = 0;
    if (sub_6FC42F20(pUnit, pItem, &nBodyLoc, 0))
    {
        if (D2Common_10299(pUnit, nBodyLoc, pItem, 0) != 1)
        {
            *a4 = 1;
            return 0;
        }

        D2RoomStrc* pRoom = UNITS_GetRoom(pItem);
        if (pRoom)
        {
            DUNGEON_AllocDrlgDelete(pRoom, pItem->dwUnitType, pItem->dwUnitId);
            UNITS_FreeCollisionPath(pItem);
            UNITROOM_RemoveUnitFromRoom(pItem);
        }

        if (sub_6FC4A9B0(pGame, pUnit, pItem, 0))
        {
            QUESTS_ItemPickedUp(pGame, pUnit, pItem);
            return 1;
        }

        return 0;
    }

    if (!ITEMS_CheckIfBeltable(pItem) || !ITEMS_CheckIfAutoBeltable(pUnit->pInventory, pItem) || !INVENTORY_PlaceItemInFreeBeltSlot(pUnit->pInventory, pItem))
    {
        if (sub_6FC43E60(pGame, pUnit, pItem, 1, 0, __FILE__, __LINE__))
        {
            return 1;
        }

        DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pItem), pItem->dwUnitType, pItem->dwUnitId);
        ITEMS_SetInvPage(pItem, 0xFFu);
        pItem->dwFlags |= UNITFLAG_SENDREFRESHMSG;
        UNITS_ChangeAnimMode(pItem, IMODE_ONGROUND);
        SUNIT_AttachSound(pUnit, 0x17u, pUnit);
        return 0;
    }

    if (UNITS_GetRoom(pItem))
    {
        DUNGEON_AllocDrlgDelete(UNITS_GetRoom(pItem), pItem->dwUnitType, pItem->dwUnitId);
        UNITS_FreeCollisionPath(pItem);
        UNITROOM_RemoveUnitFromRoom(pItem);
    }

    if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pItem, 2))
    {
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
    }

    INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);

    if (!pUnit->dwUnitType)
    {
        sub_6FC42120(pUnit, pItem, 1);
    }

    if (ITEMS_IsCharmUsable(pItem, pUnit))
    {
        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pUnit, 0);
    }

    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    UNITS_ChangeAnimMode(pItem, IMODE_INBELT);
    ITEMS_SetInvPage(pItem, 0xFFu);
    ITEMS_SetItemCMDFlag(pItem, 0x2000u, 1);
    pItem->dwFlags &= ~UNITFLAG_ISRESURRECT;
    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
    UNITS_RefreshInventory(pUnit, 1);

    if (ITEMS_IsCharmUsable(pItem, pUnit))
    {
        D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);
        UNITS_RefreshInventory(pUnit, 1);
    }

    return 1;
}

//D2Game.0x6FC437F0
int32_t __fastcall sub_6FC437F0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    D2UnitStrc* pStackItem = nullptr;
    int32_t nCounter = 0;

    while (STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0) > 0)
    {
        ++nCounter;

        if (ITEMS_GetAutoStack(pItem))
        {
            pStackItem = INVENTORY_FindEquippedItemForStack(pUnit->pInventory, pItem, pStackItem);
            if (ITEMS_GetQuiverType(pItem) && !pStackItem)
            {
                pStackItem = INVENTORY_FindBackPackItemForStack(pUnit->pInventory, pItem, nullptr);
            }
        }

        if (!pStackItem)
        {
            return 0;
        }

        if (ITEMS_AreStackablesEqual(pItem, pStackItem))
        {
            const int32_t nQuantity1 = STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0);
            const int32_t nQuantity2 = STATLIST_GetUnitStatUnsigned(pStackItem, STAT_QUANTITY, 0);
            int32_t nQuantity = nQuantity1 + nQuantity2;
            if (nQuantity <= ITEMS_GetTotalMaxStack(pStackItem))
            {
                D2_ASSERT(nQuantity >= 0);

                if (ITEMS_HasDurability(pItem))
                {
                    const int32_t nDurability = STATLIST_GetUnitStatUnsigned(pItem, STAT_DURABILITY, 0);
                    if (nDurability < STATLIST_GetUnitStatUnsigned(pStackItem, STAT_DURABILITY, 0))
                    {
                        STATLIST_SetUnitStat(pStackItem, STAT_DURABILITY, nDurability, 0);
                        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pStackItem, 1, STAT_DURABILITY, nDurability, 0);
                    }
                }

                STATLIST_SetUnitStat(pStackItem, STAT_QUANTITY, nQuantity, 0);
                STATLIST_SetUnitStat(pItem, STAT_QUANTITY, 0, 0);

                D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pStackItem, 1, STAT_QUANTITY, nQuantity, 0);

                if (ITEMS_GetItemType(pItem) == ITEMTYPE_BOOK && ITEMS_GetItemType(pStackItem) == ITEMTYPE_BOOK)
                {
                    sub_6FC43AF0(pUnit, pStackItem, nQuantity1);
                }

                D2RoomStrc* pRoom = UNITS_GetRoom(pItem);
                if (pRoom)
                {
                    int32_t nUnitGUID = -1;
                    int32_t nUnitType = 6;
                    if (pItem)
                    {
                        nUnitGUID = pItem->dwUnitId;
                        nUnitType = pItem->dwUnitType;
                    }

                    DUNGEON_AllocDrlgDelete(pRoom, nUnitType, nUnitGUID);
                    UNITS_FreeCollisionPath(pItem);
                    UNITROOM_RemoveUnitFromRoom(pItem);
                }

                if (pItem)
                {
                    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
                }

                ITEMS_RemoveFromAllPlayers(pGame, pItem);
                INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);
                return 1;
            }

            const int32_t nMaxStack = ITEMS_GetTotalMaxStack(pStackItem);
            D2_ASSERT(nMaxStack >= 0);

            STATLIST_SetUnitStat(pStackItem, STAT_QUANTITY, nMaxStack, 0);
            D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pStackItem, 1, STAT_QUANTITY, nMaxStack, 0);

            nQuantity -= nMaxStack;
            D2_ASSERT(nQuantity >= 0);

            STATLIST_SetUnitStat(pItem, STAT_QUANTITY, nQuantity, 0);

            if (ITEMS_GetItemType(pItem) == ITEMTYPE_BOOK && ITEMS_GetItemType(pStackItem) == ITEMTYPE_BOOK)
            {
                sub_6FC43AF0(pUnit, pStackItem, ITEMS_GetTotalMaxStack(pStackItem) - nQuantity2);
            }
        }
    }

    if (nCounter)
    {
        D2_ASSERT(0);
    }

    return 0;
}

//D2Game.0x6FC43AF0
void __fastcall sub_6FC43AF0(D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t nQuantityBonus)
{
    D2_ASSERT(pUnit);
    D2_ASSERT(pItem);

    if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL) && !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
    {
        return;
    }

    D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
    if (!pBooksTxtRecord)
    {
        return;
    }

    int32_t nSkillId = -1;
    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
    {
        nSkillId = pBooksTxtRecord->dwScrollSkillId;
    }
    else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
    {
        nSkillId = pBooksTxtRecord->dwBookSkillId;
    }

    if (nSkillId == -1)
    {
        return;
    }

    D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, -1u);
    if (pSkill)
    {
        const int32_t nQuantity = nQuantityBonus + SKILLS_GetQuantity(pSkill);
        SKILLS_SetQuantity(pSkill, nQuantity);
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        D2GAME_PACKETS_SendPacket0x22_6FC3DBE0(pClient, pUnit->dwUnitType, pUnit->dwUnitId, nSkillId, nQuantity);
    }
}

//D2Game.0x6FC43BF0
int32_t __fastcall sub_6FC43BF0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    D2_ASSERT(pGame);

    D2UnitStrc* pBook = INVENTORY_FindFillableBook(pUnit->pInventory, pItem, 0);
    if (!pBook)
    {
        return 0;
    }

    const int32_t nSrcValue = STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0);
    D2_ASSERT(nSrcValue >= 0);

    const int32_t nDstValue = STATLIST_GetUnitStatUnsigned(pBook, STAT_QUANTITY, 0);
    D2_ASSERT(nDstValue >= 0);

    const int32_t nDstMaxValue = ITEMS_GetTotalMaxStack(pBook);
    D2_ASSERT(nDstMaxValue >= 0);

    int32_t nQuantity = nSrcValue + nDstValue;
    if (nQuantity <= nDstMaxValue)
    {
        D2_ASSERT(nQuantity >= 0);

        STATLIST_SetUnitStat(pBook, STAT_QUANTITY, nQuantity, 0);

        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pBook, 1, STAT_QUANTITY, nQuantity, 0);

        sub_6FC43AF0(pUnit, pBook, nSrcValue);

        D2RoomStrc* pRoom = UNITS_GetRoom(pItem);
        if (pRoom)
        {
            int32_t nUnitGUID = -1;
            int32_t nUnitType = 6;
            if (pItem)
            {
                nUnitGUID = pItem->dwUnitId;
                nUnitType = pItem->dwUnitType;
            }

            DUNGEON_AllocDrlgDelete(pRoom, nUnitType, nUnitGUID);
            UNITS_FreeCollisionPath(pItem);
            UNITROOM_RemoveUnitFromRoom(pItem);
        }

        if (pItem)
        {
            pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
        }

        ITEMS_RemoveFromAllPlayers(pGame, pItem);
        INVENTORY_SetCursorItem(pUnit->pInventory, 0);
    }
    else
    {
        STATLIST_SetUnitStat(pBook, STAT_QUANTITY, nDstMaxValue, 0);

        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pBook, 1, STAT_QUANTITY, nDstMaxValue, 0);
        
        nQuantity = nSrcValue + nDstValue - nDstMaxValue;
        D2_ASSERT(nQuantity >= 0);

        STATLIST_SetUnitStat(pItem, STAT_QUANTITY, nQuantity, 0);
        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_QUANTITY, nQuantity, 0);
        sub_6FC43AF0(pUnit, pBook, nDstMaxValue - nDstValue);
    }

    return 1;
}

//D2Game.0x6FC43E60
int32_t __fastcall sub_6FC43E60(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t bRemove, int32_t nPage, const char* szFile, int32_t nLine)
{
    D2_ASSERT(pGame);

    D2InventoryStrc* pInventory = pUnit->pInventory;
    D2_ASSERT(pInventory);

    if (!INVENTORY_PlaceItemAtFreePosition(pInventory, pItem, UNITS_GetInventoryRecordId(pUnit, nPage, pGame->bExpansion), 0, 0, szFile, nLine))
    {
        return 0;
    }

    if (bRemove)
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(pItem);
        if (pRoom)
        {
            int32_t nUnitGUID = -1;
            int32_t nUnitType = 6;
            if (pItem)
            {
                nUnitGUID = pItem->dwUnitId;
                nUnitType = pItem->dwUnitType;
            }

            DUNGEON_AllocDrlgDelete(pRoom, nUnitType, nUnitGUID);
            UNITS_FreeCollisionPath(pItem);
            UNITROOM_RemoveUnitFromRoom(pItem);
        }
    }

    if (!INVENTORY_PlaceItemInSocket(pInventory, pItem, 1))
    {
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
    }

    INVENTORY_SetCursorItem(pInventory, nullptr);

    D2_ASSERT(pItem);

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pUnit, pItem, 1);
    }

    if (ITEMS_IsCharmUsable(pItem, pUnit))
    {
        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pUnit, 0);
    }

    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    UNITS_ChangeAnimMode(pItem, IMODE_STORED);
    ITEMS_SetItemCMDFlag(pItem, 128, 1);
    INVENTORY_AddItemToTradeInventory(pInventory, pItem);
    UNITS_RefreshInventory(pUnit, 1);
    pItem->dwFlags &= ~UNITFLAG_ISRESURRECT;
    ITEMS_SetInvPage(pItem, nPage);
    QUESTS_ItemPickedUp(pGame, pUnit, pItem);
    
    if (ITEMS_IsCharmUsable(pItem, pUnit))
    {
        D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);
        UNITS_RefreshInventory(pUnit, 1);
    }

    return 1;
}

//D2Game.0x6FC44030
void __fastcall sub_6FC44030(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, 4, nItemGUID);
    if (!pItem || pItem->dwUnitType != UNIT_ITEM || pItem->dwAnimMode != IMODE_ONCURSOR)
    {
        return;
    }

    D2CoordStrc returnCoords = {};
    D2CoordStrc coords = {};
    UNITS_GetCoords(pUnit, &coords);
    D2RoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pUnit), &coords, &returnCoords, 1);
    if (!pRoom)
    {
        return;
    }

    D2GAME_DropItem_6FC52260(pGame, pUnit, pItem, pRoom, returnCoords.nX, returnCoords.nY);

    INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);

    if (pItem->dwUnitType == UNIT_ITEM && ITEMS_CheckIfSocketable(pItem) && pItem->pInventory && INVENTORY_GetItemCount(pItem->pInventory))
    {
        ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
    }

    if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);
    }

    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
    D2_ASSERT(pItemsTxtRecord);

    if (pItemsTxtRecord->dwCode != ' xob')
    {
        return;
    }

    D2UnitStrc* pInvItem = INVENTORY_GetFirstItem(pUnit->pInventory);
    while (pInvItem)
    {
        D2UnitStrc* pNext = INVENTORY_GetNextItem(pInvItem);
        if (INVENTORY_UnitIsItem(pInvItem) && ITEMS_GetInvPage(pInvItem) == INVPAGE_CUBE)
        {
            ITEMS_SetItemCell(pItem, 3);
            D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pInvItem, 0x20);
            
            D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pInvItem);
            D2_ASSERT(pRemove);
            D2_ASSERT(pInvItem == pRemove);

            //const uint8_t nInvPage = ITEMS_GetInvPage(pInvItem);
            //INVENTORY_Return(__FILE__, __LINE__, pUnit->pInventory, CLIENTS_GetUnitX(pInvItem), CLIENTS_GetUnitY(pInvItem), UNITS_GetInventoryRecordId(pUnit, nInvPage, pGame->bExpansion), 0, nInvPage);

            UNITS_ChangeAnimMode(pRemove, IMODE_ONCURSOR);
            ITEMS_SetInvPage(pInvItem, INVPAGE_INVENTORY);

            if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pUnit, pInvItem->dwUnitId, 0, 0, 1, 1, 0))
            {
                ITEMS_SetInvPage(pInvItem, INVPAGE_NULL);
                UNITS_GetCoords(pUnit, &coords);
                
                pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pUnit), &coords, &returnCoords, 1);
                if (pRoom)
                {
                    D2GAME_DropItem_6FC52260(pGame, pUnit, pInvItem, pRoom, returnCoords.nX, returnCoords.nY);
                }
            }
        }

        pInvItem = pNext;
    }
}

//D2Game.0x6FC44410
int32_t __fastcall D2GAME_PlaceItem_6FC44410(const char* szFile, int32_t nLine, D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nUnitId, int32_t nX, int32_t nY, int32_t a8, int32_t a9, D2InventoryStrc* pInventory)
{
    if (!pGame || !pPlayer)
    {
        return 0;
    }

    if (pPlayer->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pPlayer->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pPlayer->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nUnitId);
    if (!pItem || pItem->dwUnitType != UNIT_ITEM || pItem->dwAnimMode != IMODE_ONCURSOR)
    {
        return 0;
    }
    
    const uint8_t nPage = ITEMS_GetInvPage(pItem);    
    if (!pInventory)
    {
        pInventory = pPlayer->pInventory;
        if (!pInventory)
        {
            return 0;
        }
    }
    
    int32_t bTrade = 0;
    int32_t bEquip = 1;
    const int32_t nRecordId = UNITS_GetInventoryRecordId(pPlayer, nPage, pGame->bExpansion);
    if (pPlayer->dwUnitType == UNIT_PLAYER)
    {
        if (nPage == INVPAGE_EQUIP)
        {
            bEquip = 0;
        }
        else if (nPage == INVPAGE_TRADE)
        {
            bTrade = 1;
        }
    }

    if (a8)
    {
        if (!INVENTORY_PlaceItemAtFreePosition(pInventory, pItem, nRecordId, 0, nPage, szFile, nLine))
        {
            return 0;
        }
    }
    else
    {
        if (!INVENTORY_PlaceItemAtInventoryPage(pInventory, pItem, std::max(nX, 0), std::max(nY, 0), nRecordId, 0, nPage))
        {
            return 0;
        }
    }

    if (!INVENTORY_PlaceItemInSocket(pInventory, pItem, 1))
    {
        return 0;
    }

    if (ITEMS_GetInvPage(pItem) != INVPAGE_STASH && pPlayer->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pPlayer, pItem, 1);
    }

    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;

    if (ITEMS_IsCharmUsable(pItem, pPlayer))
    {
        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pPlayer, 0);
    }

    if (bEquip)
    {
        INVENTORY_SetCursorItem(pInventory, nullptr);
    }

    UNITS_ChangeAnimMode(pItem, 0);

    if (a9)
    {
        ITEMS_SetItemCMDFlag(pItem, 2, 1);

        if (pItem->dwUnitType == UNIT_ITEM && ITEMS_CheckIfSocketable(pItem) && pItem->pInventory && INVENTORY_GetItemCount(pItem->pInventory))
        {
            ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
        }

        if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
        {
            ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);
        }

        UNITS_RefreshInventory(pPlayer, 1);
        INVENTORY_AddItemToTradeInventory(pInventory, pItem);
    }

    if (bTrade == 1)
    {
        sub_6FC92F10(pGame, pPlayer, pItem);
    }

    if (ITEMS_IsCharmUsable(pItem, pPlayer))
    {
        D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pPlayer, 0);
        UNITS_RefreshInventory(pPlayer, 1);
    }

    return 1;
}

//D2Game.0x6FC446B0
int32_t __fastcall sub_6FC446B0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t* a4, int32_t a5, int32_t bSetQuantityFlag, D2InventoryStrc* pInventoryArg, int32_t a8)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a4 = 0;

    D2InventoryStrc* pInventory = pUnit->pInventory;
    if (pInventoryArg)
    {
        pInventory = pInventoryArg;
    }

    D2_ASSERT(pInventory);

    if (INVENTORY_GetCursorItem(pInventory) && !a8)
    {
        return 0;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        *a4 = 1;
        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pUnit->dwUnitType == UNIT_PLAYER && !PLAYER_IsBusy(pUnit) && ITEMS_GetInvPage(pItem))
    {
        *a4 = 1;
        return 0;
    }

    if (pItem->dwAnimMode != IMODE_STORED)
    {
        *a4 = 1;
        return 0;
    }

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;

    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pInventory, pItem);
    D2_ASSERT(pRemove);

    D2_ASSERT(pItem == pRemove);

    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pItem, pUnit, 0, 1);

    if (ITEMS_IsCharmUsable(pItem, pUnit))
    {
        D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);
        UNITS_RefreshInventory(pUnit, 1);
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pUnit, pItem, 0);
    }

    const uint8_t nInvPage = ITEMS_GetInvPage(pItem);
    const int32_t nInventoryRecordId = UNITS_GetInventoryRecordId(pUnit, nInvPage, pGame->bExpansion);
    
    int32_t bTrade = 0;
    int32_t bEquip = 1;

    if (pUnit->dwUnitType != UNIT_PLAYER || nInvPage == INVPAGE_EQUIP)
    {
        bEquip = 0;
    }
    else if (nInvPage == INVPAGE_TRADE)
    {
        bTrade = 1;
    }

    //INVENTORY_Return(__FILE__, __LINE__, pInventory, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem),  nInventoryRecordId, 0, nInvPage);

    ITEMS_SetItemCell(pItem, nInvPage);
    ITEMS_SetInvPage(pItem, 0xFFu);

    if (bEquip)
    {
        INVENTORY_SetCursorItem(pInventory, pItem);
    }

    UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);

    if (a5 == 1)
    {
        if (bSetQuantityFlag)
        {
            ITEMS_SetItemFlag(pItem, IFLAG_QUANTITY, 1);
        }

        if (bTrade == 1)
        {
            sub_6FC931D0(pGame, pUnit, pItem);
        }

        ITEMS_SetItemCMDFlag(pItem, 4, 1);

        if (pItem->dwUnitType == UNIT_ITEM && ITEMS_CheckIfSocketable(pItem) && pItem->pInventory && INVENTORY_GetItemCount(pItem->pInventory))
        {
            ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
        }

        if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
        {
            ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);
        }

        INVENTORY_AddItemToTradeInventory(pInventory, pItem);
        UNITS_RefreshInventory(pUnit, 1);
    }

    return 1;
}

//D2Game.0x6FC44A90
void __fastcall D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t bRefresh)
{
    D2UnkItemModeStrc a2 = {};

    if (!pUnit || (pUnit->dwUnitType != UNIT_PLAYER && !(pUnit->dwFlags & UNITFLAG_ISMERC)))
    {
        return;
    }

    sub_6FC44F00(pUnit, &a2);
    if (!pUnit->pInventory)
    {
        return;
    }

    D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory);
    while (pItem)
    {
        if (INVENTORY_UnitIsItem(pItem)
            && INVENTORY_GetItemNodePage(pItem) == 1
            && ITEMS_IsCharmUsable(pItem, pUnit)
            && !STATLIST_GetOwner(pItem, nullptr)
            && ITEMS_CheckRequirements(pItem, pUnit, 0, nullptr, nullptr, nullptr))
        {
            int32_t bUpdate = 0;
            if (!ITEMS_GetQuiverType(pItem))
            {
                bUpdate = 1;
            }
            else
            {
                D2UnitStrc* pOtherArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
                if (pOtherArmItem == pItem)
                {
                    pOtherArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
                }

                if (pOtherArmItem && ITEMS_CheckItemTypeId(pOtherArmItem, ITEMS_GetQuiverType(pItem)))
                {
                    bUpdate = 1;
                }
            }

            if (bUpdate)
            {
                ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);

                if (pUnit->pInventory)
                {
                    const uint8_t nBodyLoc = ITEMS_GetBodyLocation(pItem);
                    if (nBodyLoc != BODYLOC_SWRARM && nBodyLoc != BODYLOC_SWLARM)
                    {
                        INVENTORY_UpdateWeaponGUIDOnInsert(pUnit->pInventory, pItem);
                        if (UNITS_GetAnimOrSeqMode(pItem) == 1 || !UNITS_GetAnimOrSeqMode(pItem) && ITEMS_IsCharmUsable(pItem, pUnit))
                        {
                            D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pUnit, 1);
                        }
                    }
                }
            }
        }

        pItem = INVENTORY_GetNextItem(pItem);
    }

    for (int32_t i = 0; i < 11; ++i)
    {
        D2UnitStrc* pBodyLocItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, i);
        if (pBodyLocItem
            && (!ITEMS_CheckItemFlag(pBodyLocItem, IFLAG_BROKEN, __LINE__, __FILE__) || STATLIST_GetOwner(pBodyLocItem, 0))
            && (!ITEMS_CheckItemFlag(pBodyLocItem, IFLAG_NOEQUIP, __LINE__, __FILE__) || STATLIST_GetOwner(pBodyLocItem, 0)))
        {
            int32_t bUpdate = 0;
            if (!ITEMS_CheckRequirements(pBodyLocItem, pUnit, 0, nullptr, nullptr, nullptr))
            {
                bUpdate = 1;
            }
            else
            {
                if (ITEMS_GetQuiverType(pBodyLocItem))
                {
                    D2UnitStrc* pOtherArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
                    if (pOtherArmItem == pBodyLocItem)
                    {
                        pOtherArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
                    }

                    if (!pOtherArmItem || !ITEMS_CheckItemTypeId(pOtherArmItem, ITEMS_GetQuiverType(pBodyLocItem)))
                    {
                        bUpdate = 1;
                    }
                }
            }

            if (bUpdate)
            {
                ITEMS_SetItemFlag(pBodyLocItem, IFLAG_NOEQUIP, 1);
                INVENTORY_UpdateWeaponGUIDOnRemoval(pUnit->pInventory, pBodyLocItem);
                if (UNITS_GetAnimOrSeqMode(pBodyLocItem) == 1)
                {
                    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pBodyLocItem, pUnit, 1, 1);
                }
            }
        }
    }

    int32_t bContinue = 0;
    do
    {
        bContinue = 0;
        
        for (int32_t i = 0; i < 11; ++i)
        {
            D2UnitStrc* pBodyLocItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, i);
            if (pBodyLocItem
                && !ITEMS_CheckItemFlag(pBodyLocItem, IFLAG_BROKEN, __LINE__, __FILE__)
                && (ITEMS_CheckItemFlag(pBodyLocItem, IFLAG_NOEQUIP, __LINE__, __FILE__) || !STATLIST_GetOwner(pBodyLocItem, 0))
                && ITEMS_CheckRequirements(pBodyLocItem, pUnit, 0, nullptr, nullptr, nullptr))
            {
                int32_t bUpdate = 0;
                if (!ITEMS_GetQuiverType(pBodyLocItem))
                {
                    bUpdate = 1;
                }
                else
                {
                    D2UnitStrc* pOtherArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
                    if (pOtherArmItem == pBodyLocItem)
                    {
                        pOtherArmItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
                    }

                    if (pOtherArmItem && ITEMS_CheckItemTypeId(pOtherArmItem, ITEMS_GetQuiverType(pBodyLocItem)))
                    {
                        bUpdate = 1;
                    }
                }

                if (bUpdate)
                {
                    ITEMS_SetItemFlag(pBodyLocItem, IFLAG_NOEQUIP, 0);

                    if (pUnit->pInventory)
                    {
                        const uint8_t nBodyLoc = ITEMS_GetBodyLocation(pBodyLocItem);
                        if (nBodyLoc != BODYLOC_SWRARM && nBodyLoc != BODYLOC_SWLARM)
                        {
                            INVENTORY_UpdateWeaponGUIDOnInsert(pUnit->pInventory, pBodyLocItem);
                            if (UNITS_GetAnimOrSeqMode(pBodyLocItem) == 1 || !UNITS_GetAnimOrSeqMode(pBodyLocItem) && ITEMS_IsCharmUsable(pBodyLocItem, pUnit))
                            {
                                D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pBodyLocItem, pUnit, 1);
                            }
                        }
                    }

                    bContinue = 1;
                }
            }
        }
    }
    while (bContinue);

    for (int32_t i = 0; i < 11; ++i)
    {
        D2UnitStrc* pBodyLocItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, i);
        if (pBodyLocItem
            && ITEMS_GetItemQuality(pBodyLocItem) == ITEMQUAL_SET
            && !ITEMS_CheckItemFlag(pBodyLocItem, IFLAG_BROKEN, __LINE__, __FILE__)
            && !ITEMS_CheckItemFlag(pBodyLocItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
        {
            D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pBodyLocItem, pUnit, 1, 1);
            D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pBodyLocItem, pUnit, 1);
        }
    }

    sub_6FC44FB0(pUnit, &a2);
    UNITS_RefreshInventory(pUnit, 1);

    if (bRefresh)
    {
        D2ClientStrc* pClient = nullptr;
        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        }

        if (!pClient)
        {
            D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
            if (pOwner && pOwner->dwUnitType == UNIT_PLAYER)
            {
                pClient = SUNIT_GetClientFromPlayer(pOwner, __FILE__, __LINE__);
            }
        }

        if (pClient)
        {
            D2GAME_SendPacket0x48_6FC3EF40(pClient, pUnit, 0);
        }
    }
}

//D2Game.0x6FC44F00
void __fastcall sub_6FC44F00(D2UnitStrc* pUnit, D2UnkItemModeStrc* a2)
{
    a2->nHitpoints = STATLIST_GetUnitStatUnsigned(pUnit, STAT_HITPOINTS, 0);
    a2->nMana = STATLIST_GetUnitStatUnsigned(pUnit, STAT_MANA, 0);
    a2->nStamina = STATLIST_GetUnitStatUnsigned(pUnit, STAT_STAMINA, 0);

    D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pUnit);
    if (pLeftSkill)
    {
        a2->nLeftSkillOwnerGUID = SKILLS_GetOwnerGUIDFromSkill(pLeftSkill);
        a2->nLeftSkillId = SKILLS_GetSkillIdFromSkill(pLeftSkill, __FILE__, __LINE__);
    }
    else
    {
        a2->nLeftSkillOwnerGUID = -1;
        a2->nLeftSkillId = 0;
    }

    D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pUnit);
    if (pRightSkill)
    {
        a2->nRightSkillOwnerGUID = SKILLS_GetOwnerGUIDFromSkill(pRightSkill);
        a2->nRightSkillId = SKILLS_GetSkillIdFromSkill(pRightSkill, __FILE__, __LINE__);
    }
    else
    {
        a2->nRightSkillOwnerGUID = -1;
        a2->nRightSkillId = 0;
    }
}

//D2Game.0x6FC44FB0
void __fastcall sub_6FC44FB0(D2UnitStrc* pUnit, D2UnkItemModeStrc* a2)
{
    if (a2->nLeftSkillId)
    {
        D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, a2->nLeftSkillId, a2->nLeftSkillOwnerGUID);
        if (pSkill && pSkill != UNITS_GetLeftSkill(pUnit))
        {
            const int32_t nUseState = SKILLS_GetUseState(pUnit, pSkill);
            if (nUseState != SKILLUSTATE_NOQUANTITY && nUseState != SKILLUSTATE_NOLEVEL)
            {
                D2GAME_AssignSkill_6FD13800(pUnit, 1, a2->nLeftSkillId, a2->nLeftSkillOwnerGUID);
            }
        }
    }

    if (a2->nRightSkillId)
    {
        D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, a2->nRightSkillId, a2->nRightSkillOwnerGUID);
        if (pSkill && pSkill != UNITS_GetRightSkill(pUnit))
        {
            const int32_t nUseState = SKILLS_GetUseState(pUnit, pSkill);
            if (nUseState != SKILLUSTATE_NOQUANTITY && nUseState != SKILLUSTATE_NOLEVEL)
            {
                D2GAME_AssignSkill_6FD13800(pUnit, 0, a2->nRightSkillId, a2->nRightSkillOwnerGUID);
            }
        }
    }
}

//D2Game.0x6FC45050
void __fastcall D2GAME_ITEMS_UpdateInventoryItems_6FC45050(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3, int32_t bRefresh)
{
    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, bRefresh);
}

//D2Game.0x6FC45060
int32_t __fastcall sub_6FC45060(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nItemGUID, uint8_t nBodyLoc, int32_t bSkipRequirementCheck, int32_t* a6)
{
    int32_t bIsSwitchWeapon = 0;
    if (nBodyLoc == BODYLOC_SWRARM || nBodyLoc == BODYLOC_SWLARM)
    {
        bIsSwitchWeapon = 1;
    }

    D2_ASSERT(pGame);
    D2_ASSERT(pPlayer);

    if (a6)
    {
        *a6 = 0;
    }

    if (pPlayer->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pPlayer->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pPlayer->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        if (a6)
        {
            *a6 = 1;
        }

        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pItem->dwAnimMode != IMODE_ONCURSOR)
    {
        if (a6)
        {
            *a6 = 1;
        }

        return 0;
    }

    if (D2Common_10299(pPlayer, nBodyLoc, pItem, bSkipRequirementCheck) != 1)
    {
        if (a6)
        {
            *a6 = 0;
        }

        return 0;
    }

    if (bSkipRequirementCheck || ITEMS_CheckRequirements(pItem, pPlayer, 0, nullptr, nullptr, nullptr))
    {
        if (!bIsSwitchWeapon)
        {
            D2UnitStrc* pMainWeapon = INVENTORY_GetLeftHandWeapon(pPlayer->pInventory);
            if (pMainWeapon)
            {
                UNITS_SetWeaponGUID(pPlayer, pMainWeapon);
            }
        }

        if (INVENTORY_PlaceItemInBodyLoc(pPlayer->pInventory, pItem, nBodyLoc))
        {
            if (!INVENTORY_PlaceItemInSocket(pPlayer->pInventory, pItem, bIsSwitchWeapon ? 4 : 3))
            {
                if (a6)
                {
                    *a6 = 1;
                }

                return 0;
            }

            ITEMS_SetBodyLocation(pItem, nBodyLoc);

            if (!bIsSwitchWeapon)
            {
                INVENTORY_UpdateWeaponGUIDOnInsert(pPlayer->pInventory, pItem);
                D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pPlayer, 0);
            }

            INVENTORY_SetCursorItem(pPlayer->pInventory, 0);

            pItem->dwFlags &= ~UNITFLAG_TARGETABLE;

            UNITS_ChangeAnimMode(pItem, IMODE_EQUIP);
            ITEMS_SetInvPage(pItem, 0xFFu);
            ITEMS_SetItemCMDFlag(pItem, 8, 1);
            ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);

            if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);
            }

            INVENTORY_AddItemToTradeInventory(pPlayer->pInventory, pItem);
            UNITS_RefreshInventory(pPlayer, 1);
            sub_6FC45300(pPlayer);
            D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pPlayer, 0);
        }

        return 1;
    }

    if (a6)
    {
        *a6 = 1;
    }

    return 0;
}

//Inlined in D2Game.0x6FC45300
int32_t __fastcall ITEMMODE_SkillNeedsToBeInitialized(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
    if (!pSkill)
    {
        return 1;
    }
    
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecordFromSkill(pSkill);
    if (!pSkillsTxtRecord || pSkillsTxtRecord->nITypeA[0] <= 0 || !ITEMS_CheckType(pSkillsTxtRecord->nITypeA[0], ITEMTYPE_THROWN_WEAPON) || pSkillsTxtRecord->nRange != 2)
    {
        return 1;
    }
    
    const int32_t nSkillUseState = SKILLS_GetUseState(pUnit, pSkill);
    if (nSkillUseState == SKILLUSTATE_NOQUANTITY || nSkillUseState == SKILLUSTATE_NOLEVEL)
    {
        return 1;
    }

    return 0;
}

//D2Game.0x6FC45300
void __fastcall sub_6FC45300(D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_PLAYER || !pUnit->pInventory)
    {
        return;
    }

    D2UnitStrc* pMainWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
    D2UnitStrc* pRightHandWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
    D2UnitStrc* pLeftHandWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
    if (pMainWeapon == pRightHandWeapon)
    {
        pRightHandWeapon = pLeftHandWeapon;
    }

    if (pRightHandWeapon && !ITEMS_CheckItemTypeId(pRightHandWeapon, ITEMTYPE_WEAPON))
    {
        pRightHandWeapon = nullptr;
    }

    if (!pMainWeapon)
    {
        return;
    }

    D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pUnit);
    if (ITEMS_CheckItemTypeId(pMainWeapon, ITEMTYPE_THROWN_WEAPON) && !ITEMS_CheckItemTypeId(pMainWeapon, ITEMTYPE_MELEE_WEAPON) && ITEMMODE_SkillNeedsToBeInitialized(pUnit, pLeftSkill))
    {
        UNITS_InitLeftSkill(pUnit);
        D2GAME_AssignSkill_6FD13800(pUnit, 1, 2, -1);
        pLeftSkill = UNITS_GetLeftSkill(pUnit);
    }

    const int32_t nLeftSkillUseState = SKILLS_GetUseState(pUnit, pLeftSkill);
    if (nLeftSkillUseState == SKILLUSTATE_NOQUANTITY || nLeftSkillUseState == SKILLUSTATE_NOLEVEL)
    {
        if (pLeftSkill)
        {
            int32_t nSkillId = 0;
            int32_t nOwnerGUID = 0;
            UNITS_GetLeftSkillData(pUnit, &nSkillId, &nOwnerGUID);
            D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, nOwnerGUID);
            if (pSkill && pSkill != pLeftSkill)
            {
                const int32_t nSkillUseState = SKILLS_GetUseState(pUnit, pSkill);
                if (nSkillUseState != SKILLUSTATE_NOQUANTITY && nSkillUseState != SKILLUSTATE_NOLEVEL)
                {
                    D2GAME_AssignSkill_6FD13800(pUnit, 1, nSkillId, nOwnerGUID);
                }
            }
        }
    }

    D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pUnit);
    if (pRightHandWeapon && ITEMS_CheckItemTypeId(pRightHandWeapon, ITEMTYPE_THROWN_WEAPON) && !ITEMS_CheckItemTypeId(pRightHandWeapon, ITEMTYPE_MELEE_WEAPON) && ITEMMODE_SkillNeedsToBeInitialized(pUnit, pRightSkill))
    {
        UNITS_InitRightSkill(pUnit);
        D2GAME_AssignSkill_6FD13800(pUnit, 0, 4, -1);
        pRightSkill = UNITS_GetRightSkill(pUnit);
    }

    const int32_t nRightSkillUseState = SKILLS_GetUseState(pUnit, pRightSkill);
    if (nRightSkillUseState == SKILLUSTATE_NOQUANTITY || nRightSkillUseState == SKILLUSTATE_NOLEVEL)
    {
        if (pRightSkill)
        {
            int32_t nSkillId = 0;
            int32_t nOwnerGUID = 0;
            UNITS_GetRightSkillData(pUnit, &nSkillId, &nOwnerGUID);
            D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, nOwnerGUID);
            if (pSkill && pSkill != pRightSkill)
            {
                const int32_t nSkillUseState = SKILLS_GetUseState(pUnit, pSkill);
                if (nSkillUseState != SKILLUSTATE_NOQUANTITY && nSkillUseState != SKILLUSTATE_NOLEVEL)
                {
                    D2GAME_AssignSkill_6FD13800(pUnit, 0, nSkillId, nOwnerGUID);
                }
            }
        }
    }
}

//D2Game.0x6FC45550
int32_t __fastcall sub_6FC45550(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, uint8_t nBodyLoc, int32_t bSkipRequirementCheck, int32_t* a6)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a6 = 0;

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        *a6 = 1;
        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pItem->dwAnimMode != IMODE_ONCURSOR)
    {
        *a6 = 1;
        return 0;
    }

    uint8_t nOtherBodyLoc = 0;
    if (nBodyLoc == BODYLOC_LARM)
    {
        nOtherBodyLoc = BODYLOC_RARM;
    }
    else if (nBodyLoc == BODYLOC_RARM)
    {
        nOtherBodyLoc = BODYLOC_LARM;
    }
    else
    {
        *a6 = 1;
        return 0;
    }

    if (!INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nOtherBodyLoc))
    {
        *a6 = 1;
        return 0;
    }

    D2UnitStrc* pOppositeWeapon = nullptr;
    INVENTORY_GetSecondWieldingWeapon(pUnit, pUnit->pInventory, &pOppositeWeapon, nOtherBodyLoc);
    if (!pOppositeWeapon)
    {
        *a6 = 1;
        return 0;
    }

    if (D2Common_10299(pUnit, nBodyLoc, pItem, bSkipRequirementCheck) != 2)
    {
        *a6 = 1;
        return 0;
    }

    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pOppositeWeapon, pUnit, 0, 1);

    if (!bSkipRequirementCheck && !ITEMS_CheckRequirements(pItem, pUnit, 0, nullptr, nullptr, nullptr))
    {
        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pOppositeWeapon, pUnit, 0);
        SUNIT_AttachSound(pUnit, 0x14u, 0);
        return 0;
    }

    D2UnitStrc* pMainWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
    if (pMainWeapon)
    {
        UNITS_SetWeaponGUID(pUnit, pMainWeapon);
    }

    //D2COMMON_10835_Return0(pUnit, pOppositeWeapon);

    INVENTORY_UpdateWeaponGUIDOnRemoval(pUnit->pInventory, pOppositeWeapon);

    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pOppositeWeapon);
    D2_ASSERT(pRemove);

    D2_ASSERT(pOppositeWeapon == pRemove);

    //D2COMMON_10254_Return0(pUnit->pInventory, nOtherBodyLoc);

    if (ITEMS_GetItemType(pRemove) == ITEMTYPE_BELT)
    {
        sub_6FC45930(pGame, pUnit, nullptr);
    }

    INVENTORY_SetCursorItem(pUnit->pInventory, pOppositeWeapon);

    if (pOppositeWeapon)
    {
        pOppositeWeapon->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    UNITS_ChangeAnimMode(pOppositeWeapon, IMODE_ONCURSOR);

    if (!INVENTORY_PlaceItemInBodyLoc(pUnit->pInventory, pItem, nBodyLoc))
    {
        UNITS_RefreshInventory(pUnit, 1);
        return 1;
    }

    if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pItem, 3))
    {
        *a6 = 1;
        return 0;
    }

    ITEMS_SetBodyLocation(pItem, nBodyLoc);
    INVENTORY_UpdateWeaponGUIDOnInsert(pUnit->pInventory, pItem);
    D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pUnit, 0);
    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    UNITS_ChangeAnimMode(pItem, IMODE_EQUIP);
    ITEMS_SetInvPage(pItem, 0xFFu);
    ITEMS_SetItemCMDFlag(pItem, 0x10000u, 1);
    ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);

    if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);
    }

    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
    sub_6FC45300(pUnit);
    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);
    UNITS_RefreshInventory(pUnit, 1);
    return 1;
}

//D2Game.0x6FC45930
void __fastcall sub_6FC45930(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    D2_ASSERT(pUnit->pInventory);

    D2BeltsTxt beltsTxtRecord = {};
    DATATBLS_GetBeltsTxtRecord(pItem ? UNITS_GetBeltType(pItem) : 2, 0, &beltsTxtRecord);

    for (int32_t i = 0; i < 16; ++i)
    {
        D2UnitStrc* pBeltItem = INVENTORY_GetItemFromBeltSlot(pUnit->pInventory, i);
        if (i >= beltsTxtRecord.nBoxes && pBeltItem)
        {
            D2GAME_SendP0x9C_ItemAction_RemoveFromBelt_6FC3EBE0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pBeltItem, 0x20);

            D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pBeltItem);
            D2_ASSERT(pRemove);

            D2_ASSERT(pItem == pRemove);

            UNITS_ChangeAnimMode(pRemove, IMODE_ONCURSOR);

            if (pUnit->dwUnitType == UNIT_PLAYER)
            {
                sub_6FC42120(pUnit, pBeltItem, 0);
            }

            ITEMS_SetInvPage(pBeltItem, INVPAGE_INVENTORY);

            if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pUnit, pBeltItem->dwUnitId, 0, 0, 1, 1, 0))
            {
                D2CoordStrc coords = {};
                UNITS_GetCoords(pUnit, &coords);

                D2CoordStrc returnCoords = {};
                D2RoomStrc* pRoom = D2GAME_GetFreeSpaceEx_6FC4BF00(UNITS_GetRoom(pUnit), &coords, &returnCoords, 1);
                if (pRoom)
                {
                    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pBeltItem, pUnit, 0, 1);
                    D2GAME_DropItem_6FC52260(pGame, pUnit, pBeltItem, pRoom, returnCoords.nX, returnCoords.nY);
                }
            }
        }
    }
}

//D2Game.0x6FC45B30
int32_t __fastcall sub_6FC45B30(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nBodyLoc, int32_t bSetQuantityFlag, int32_t* a5)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a5 = 0;

    D2InventoryStrc* pInventory = pUnit->pInventory;
    D2_ASSERT(pInventory);

    if (INVENTORY_GetCursorItem(pInventory))
    {
        return 0;
    }

    D2UnitStrc* pItem = INVENTORY_GetItemFromBodyLoc(pInventory, nBodyLoc);
    if (!pItem)
    {
        *a5 = 0;
        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    const int32_t v12 = D2Common_10299(pUnit, nBodyLoc, nullptr, 0);
    if (v12 != 3 && v12 != 4)
    {
        *a5 = 1;
        return 0;
    }

    INVENTORY_GetSecondWieldingWeapon(pUnit, pInventory, &pItem, nBodyLoc);
    if (!pItem)
    {
        *a5 = 1;
        return 0;
    }

    //D2COMMON_10835_Return0(pUnit, pItem);
    INVENTORY_UpdateWeaponGUIDOnRemoval(pInventory, pItem);
    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pInventory, pItem);

    D2_ASSERT(pRemove);
    D2_ASSERT(pItem == pRemove);

    //D2COMMON_10254_Return0(pInventory, nBodyLoc);

    if (ITEMS_GetItemType(pRemove) == ITEMTYPE_BELT)
    {
        sub_6FC45930(pGame, pUnit, nullptr);
    }

    if (bSetQuantityFlag)
    {
        ITEMS_SetItemFlag(pItem, IFLAG_QUANTITY, 1);
    }
    else
    {
        INVENTORY_SetCursorItem(pInventory, pItem);
    }

    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pItem, pUnit, 0, 1);

    if (pItem)
    {
        pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
    ITEMS_SetItemCMDFlag(pItem, 0x10, 1);

    if (pItem && pItem->dwUnitType == UNIT_ITEM)
    {
        if (ITEMS_CheckIfSocketable(pItem) && pItem->pInventory && INVENTORY_GetItemCount(pItem->pInventory))
        {
            ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
        }
    }

    if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);
    }

    INVENTORY_AddItemToTradeInventory(pInventory, pItem);
    UNITS_RefreshInventory(pUnit, 1);
    sub_6FC45300(pUnit);
    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);

    return 1;
}

//D2Game.0x6FC45E60
int32_t __fastcall sub_6FC45E60(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, uint8_t nBodyLoc, int32_t bSkipRequirementCheck, int32_t* a6)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    if (nBodyLoc >= 11u)
    {
        return 0;
    }

    *a6 = 0;

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        *a6 = 0;
        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pItem->dwAnimMode != IMODE_ONCURSOR)
    {
        *a6 = 0;
        return 0;
    }

    if (D2Common_10299(pUnit, nBodyLoc, pItem, bSkipRequirementCheck) != 5)
    {
        *a6 = 0;
        return 0;
    }

    D2UnitStrc* pWeapon = INVENTORY_GetLeftHandWeapon(pUnit->pInventory);
    if (pWeapon)
    {
        UNITS_SetWeaponGUID(pUnit, pWeapon);
    }

    D2UnitStrc* pBodyItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
    INVENTORY_GetSecondWieldingWeapon(pUnit, pUnit->pInventory, &pBodyItem, nBodyLoc);
    if (!pBodyItem || pBodyItem->dwAnimMode != IMODE_EQUIP)
    {
        *a6 = 1;
        return 0;
    }

    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pBodyItem, pUnit, 0, 1);

    if (!bSkipRequirementCheck && !ITEMS_CheckRequirements(pItem, pUnit, 0, nullptr, nullptr, nullptr))
    {
        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pBodyItem, pUnit, 0);
        SUNIT_AttachSound(pUnit, 0x14u, 0);
        return 0;
    }

    if (ITEMS_GetItemType(pItem) == ITEMTYPE_BELT)
    {
        sub_6FC45930(pGame, pUnit, pItem);
    }

    //D2COMMON_10835_Return0(pUnit, pBodyItem);
    INVENTORY_UpdateWeaponGUIDOnRemoval(pUnit->pInventory, pBodyItem);
    
    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pBodyItem);

    D2_ASSERT(pRemove);
    D2_ASSERT(pBodyItem == pRemove);

    //D2COMMON_10254_Return0(pUnit->pInventory, nBodyLoc);

    INVENTORY_SetCursorItem(pUnit->pInventory, pBodyItem);
    if (pBodyItem)
    {
        pBodyItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    UNITS_ChangeAnimMode(pBodyItem, IMODE_ONCURSOR);
    ITEMS_SetItemFlag(pBodyItem, IFLAG_SWITCHOUT, 1);
    ITEMS_SetItemCMDFlag(pBodyItem, 0x20u, 1);
    ITEMS_SetItemFlag(pBodyItem, IFLAG_NEWITEM, 1);

    if (ITEMS_CheckItemFlag(pBodyItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pBodyItem, IFLAG_NOEQUIP, 0);
    }

    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pBodyItem);
    if (!INVENTORY_PlaceItemInBodyLoc(pUnit->pInventory, pItem, nBodyLoc))
    {
        *a6 = 1;
        return 0;
    }

    if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pItem, 3))
    {
        *a6 = 1;
        return 0;
    }

    ITEMS_SetBodyLocation(pItem, nBodyLoc);
    INVENTORY_UpdateWeaponGUIDOnInsert(pUnit->pInventory, pItem);
    D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pUnit, 0);
    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    UNITS_ChangeAnimMode(pItem, IMODE_EQUIP);
    ITEMS_SetInvPage(pItem, INVPAGE_NULL);
    ITEMS_SetItemFlag(pItem, IFLAG_SWITCHIN, 1);
    ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
    ITEMS_SetItemCMDFlag(pItem, 0x20u, 1);
    ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);

    if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);
    }

    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
    sub_6FC45300(pUnit);
    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);

    return 1;
}

//D2Game.0x6FC46270
int32_t __fastcall sub_6FC46270(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, uint8_t nBodyLoc, int32_t* a5)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a5 = 0;

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem || pItem->dwUnitType != UNIT_ITEM || pItem->dwAnimMode != IMODE_ONCURSOR || D2Common_10299(pUnit, nBodyLoc, pItem, 0) != 7 || !ITEMS_CheckRequirements(pItem, pUnit, 0, nullptr, nullptr, nullptr))
    {
        return 0;
    }

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    const int32_t nInventoryRecordId = UNITS_GetInventoryRecordId(pUnit, 0, pGame->bExpansion);
    D2UnitStrc* pOpposite = nullptr;
    if (nBodyLoc == BODYLOC_LARM)
    {
        pOpposite = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
        if (!INVENTORY_IsItemBodyLocFree(pUnit->pInventory, pItem, BODYLOC_RARM, nInventoryRecordId))
        {
            *a5 = 1;
            return 0;
        }
    }
    else
    {
        pOpposite = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
        if (!INVENTORY_IsItemBodyLocFree(pUnit->pInventory, pItem, BODYLOC_LARM, nInventoryRecordId))
        {
            *a5 = 1;
            return 0;
        }
    }

    D2_ASSERT(pOpposite);

    if (pOpposite->dwAnimMode != 1)
    {
        *a5 = 1;
        return 0;
    }

    INVENTORY_UpdateWeaponGUIDOnRemoval(pUnit->pInventory, pOpposite);
    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pOpposite);
    D2_ASSERT(pRemove);
    D2_ASSERT(pOpposite == pRemove);

    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pRemove, pUnit, 0, 1);

    if (INVENTORY_PlaceItemAtFreePosition(pUnit->pInventory, pRemove, nInventoryRecordId, 0, 0, __FILE__, __LINE__))
    {
        if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pRemove, 1))
        {
            *a5 = 1;
            return 0;
        }

        ITEMS_SetInvPage(pRemove, INVPAGE_INVENTORY);
        pRemove->dwFlags &= ~UNITFLAG_TARGETABLE;
        INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);
        UNITS_ChangeAnimMode(pRemove, IMODE_STORED);

        if (pRemove->dwUnitType == UNIT_ITEM && ITEMS_CheckIfSocketable(pRemove) && pRemove->pInventory && INVENTORY_GetItemCount(pRemove->pInventory))
        {
            ITEMS_SetItemFlag(pRemove, IFLAG_NEWITEM, 1);
        }

        if (ITEMS_CheckItemFlag(pRemove, IFLAG_NOEQUIP, __LINE__, __FILE__))
        {
            ITEMS_SetItemFlag(pRemove, IFLAG_NOEQUIP, 0);
        }

        ITEMS_SetItemCMDFlag(pRemove, 0x4000u, 1);
        ITEMS_SetItemFlag(pRemove, IFLAG_NEWITEM, 1);
        INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pRemove);
    }

    D2UnitStrc* pSelected = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
    D2_ASSERT(pSelected);

    if (pSelected->dwAnimMode != IMODE_EQUIP)
    {
        *a5 = 1;
        return 0;
    }

    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pSelected, pUnit, 0, 1);

    if (!ITEMS_CheckRequirements(pItem, pUnit, 0, nullptr, nullptr, nullptr))
    {
        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pSelected, pUnit, 0);
        INVENTORY_SetCursorItem(pUnit->pInventory, pItem);
        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
        ITEMS_SetItemCMDFlag(pItem, 0x100000, 1);
        INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
        UNITS_RefreshInventory(pUnit, 1);
        SUNIT_AttachSound(pUnit, 0x14u, 0);
        UNITS_RefreshInventory(pUnit, 1);
        return 0;
    }

    INVENTORY_UpdateWeaponGUIDOnRemoval(pUnit->pInventory, pSelected);
    pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pSelected);
    D2_ASSERT(pRemove);
    D2_ASSERT(pSelected == pRemove);

    INVENTORY_SetCursorItem(pUnit->pInventory, pSelected);
    pSelected->dwFlags &= ~UNITFLAG_TARGETABLE;
    UNITS_ChangeAnimMode(pSelected, IMODE_ONCURSOR);
    ITEMS_SetItemCMDFlag(pSelected, 0x10, 1);
    ITEMS_SetItemFlag(pSelected, IFLAG_NEWITEM, 1);

    if (ITEMS_CheckItemFlag(pSelected, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pSelected, IFLAG_NOEQUIP, 0);
    }

    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pSelected);
    if (!INVENTORY_PlaceItemInBodyLoc(pUnit->pInventory, pItem, nBodyLoc))
    {
        *a5 = 1;
        return 0;
    }

    if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pItem, 3))
    {
        *a5 = 1;
        return 0;
    }

    ITEMS_SetBodyLocation(pItem, nBodyLoc);
    INVENTORY_UpdateWeaponGUIDOnInsert(pUnit->pInventory, pItem);
    D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pUnit, 0);
    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    UNITS_ChangeAnimMode(pItem, IMODE_EQUIP);
    ITEMS_SetInvPage(pItem, INVPAGE_NULL);
    ITEMS_SetItemFlag(pItem, IFLAG_DELETED, 1);
    ITEMS_SetItemCMDFlag(pItem, 8, 1);
    ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);

    if (pItem->dwUnitType == UNIT_ITEM && ITEMS_CheckIfSocketable(pItem) && pItem->pInventory && INVENTORY_GetItemCount(pItem->pInventory))
    {
        ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
    }

    if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);
    }

    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
    sub_6FC45300(pUnit);
    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);

    return 1;
}

//Inlined in D2Game.0x6FC46840
// TODO: Better name
__forceinline int32_t ITEMMODE_CheckSkillUseState(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
    if (!pSkill)
    {
        return 0;
    }

    const int32_t nSkillUseState = SKILLS_GetUseState(pUnit, pSkill);
    if (nSkillUseState == SKILLUSTATE_NOQUANTITY || nSkillUseState == SKILLUSTATE_NOLEVEL)
    {
        return 0;
    }

    return 1;
}

//D2Game.0x6FC46840
int32_t __fastcall sub_6FC46840(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t* a3)
{
    // TODO: v49
    D2UnkItemModeStrc2 v49[4] =
    {
        { BODYLOC_RARM, nullptr, BODYLOC_SWRARM, 4 },
        { BODYLOC_LARM, nullptr, BODYLOC_SWLARM, 4 },
        { BODYLOC_SWRARM, nullptr, BODYLOC_RARM, 3 },
        { BODYLOC_SWLARM, nullptr, BODYLOC_LARM, 3 },
    };

    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a3 = 0;

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    int32_t nSwitchLeftSkillFlags = 0;
    int32_t nSwitchLeftSkillId = 0;
    UNITS_GetSwitchLeftSkillDataResetLeftSkill(pUnit, &nSwitchLeftSkillId, &nSwitchLeftSkillFlags);

    int32_t nSwitchRightSkillFlags = 0;
    int32_t nSwitchRightSkillId = 0;
    UNITS_GetSwitchRightSkillDataResetRightSkill(pUnit, &nSwitchRightSkillId, &nSwitchRightSkillFlags);

    UNITS_InitSwitchRightSkill(pUnit);
    UNITS_InitSwitchLeftSkill(pUnit);

    for (int32_t i = 0; i < std::size(v49); ++i)
    {
        v49[i].pItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, v49[i].nBodyLoc);
        if (v49[i].pItem)
        {
            INVENTORY_GetSecondWieldingWeapon(pUnit, pUnit->pInventory, &v49[i].pItem, v49[i].nBodyLoc);

            if (!v49[i].pItem || v49[i].pItem->dwAnimMode != IMODE_EQUIP)
            {
                *a3 = 1;
                return 0;
            }
        }
    }

    if (v49[0].pItem)
    {
        D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, v49[0].pItem, pUnit, 0, 1);
    }

    if (v49[1].pItem)
    {
        D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, v49[1].pItem, pUnit, 0, 1);
    }

    for (int32_t i = 0; i < std::size(v49); ++i)
    {
        if (v49[i].pItem)
        {
            if (i < 2)
            {
                INVENTORY_UpdateWeaponGUIDOnRemoval(pUnit->pInventory, v49[i].pItem);
            }

            D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, v49[i].pItem);
            if (!pRemove || pRemove != v49[i].pItem)
            {
                *a3 = 1;
                return 0;
            }

            ITEMS_SetInvPage(pRemove, INVPAGE_NULL);
            ITEMS_SetItemCMDFlag(pRemove, 0x200000u, 1);
            ITEMS_SetItemFlag(pRemove, IFLAG_NEWITEM, 1);

            if (ITEMS_CheckItemFlag(pRemove, IFLAG_NOEQUIP, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(pRemove, IFLAG_NOEQUIP, 0);
            }

            INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pRemove);
        }
    }

    for (int32_t i = 0; i < std::size(v49); ++i)
    {
        D2UnitStrc* pItem = v49[i].pItem;
        if (pItem)
        {
            if (!INVENTORY_PlaceItemInBodyLoc(pUnit->pInventory, pItem, v49[i].nSwitchBodyLoc))
            {
                *a3 = 1;
                return 0;
            }

            if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pItem, v49[i].nSocketArg))
            {
                *a3 = 1;
                return 0;
            }

            ITEMS_SetBodyLocation(pItem, v49[i].nSwitchBodyLoc);

            if (v49[i].nSwitchBodyLoc == BODYLOC_RARM || v49[i].nSwitchBodyLoc == BODYLOC_LARM)
            {
                INVENTORY_UpdateWeaponGUIDOnInsert(pUnit->pInventory, pItem);
                D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pUnit, 0);
                ITEMS_SetItemFlag(pItem, IFLAG_SWITCHIN, 1);
            }
            else
            {
                ITEMS_SetItemFlag(pItem, IFLAG_SWITCHOUT, 1);
            }

            pItem->dwFlags &= ~UNITFLAG_TARGETABLE;

            UNITS_ChangeAnimMode(pItem, 1);
            ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
            ITEMS_SetItemCMDFlag(pItem, 0x200000u, 1);
            ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);

            if (i < 2)
            {
                sub_6FC45300(pUnit);
            }
        }
    }

    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);

    D2SkillStrc* pSwitchLeftSkill = SKILLS_GetSkillById(pUnit, nSwitchLeftSkillId, nSwitchLeftSkillFlags);
    D2SkillStrc* pSwitchRightSkill = SKILLS_GetSkillById(pUnit, nSwitchRightSkillId, nSwitchRightSkillFlags);
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    D2_ASSERT(pClient);
    CLIENTS_SetWeaponSwitch(pClient, CLIENTS_GetWeaponSwitch(pClient) == 0);
    D2GAME_PACKETS_SendPacket0x97_6FC3FB60(pClient);

    if (ITEMMODE_CheckSkillUseState(pUnit, pSwitchLeftSkill))
    {
        D2GAME_AssignSkill_6FD13800(pUnit, 1, nSwitchLeftSkillId, nSwitchLeftSkillFlags);
        sub_6FCC64D0(pUnit, 1, nSwitchLeftSkillId, nSwitchLeftSkillFlags);
    }
    else
    {
        D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pUnit);
        if (pLeftSkill)
        {
            const int32_t nLeftSkillId = SKILLS_GetSkillIdFromSkill(pLeftSkill, __FILE__, __LINE__);
            sub_6FCC64D0(pUnit, 1, nLeftSkillId, SKILLS_GetOwnerGUIDFromSkill(pLeftSkill));
        }
        else
        {
            sub_6FCC64D0(pUnit, 1, 0, -1);
        }
    }

    if (ITEMMODE_CheckSkillUseState(pUnit, pSwitchRightSkill))
    {
        D2GAME_AssignSkill_6FD13800(pUnit, 0, nSwitchRightSkillId, nSwitchRightSkillFlags);
        sub_6FCC64D0(pUnit, 0, nSwitchRightSkillId, nSwitchRightSkillFlags);
    }
    else
    {
        D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pUnit);
        if (pRightSkill)
        {
            const int32_t nRightSkillId = SKILLS_GetSkillIdFromSkill(pRightSkill, __FILE__, __LINE__);
            sub_6FCC64D0(pUnit, 0, nRightSkillId, SKILLS_GetOwnerGUIDFromSkill(pRightSkill));
        }
        else
        {
            sub_6FCC64D0(pUnit, 0, 0, -1);
        }
    }

    return 1;
}

//D2Game.0x6FC46D40
int32_t __fastcall sub_6FC46D40(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCursorItemGUID, int32_t nGridItemGUID, int32_t nX, int32_t nY, int32_t* a7)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a7 = 0;

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pCursorItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nCursorItemGUID);
    if (!pCursorItem || pCursorItem->dwUnitType != UNIT_ITEM || pCursorItem->dwAnimMode != IMODE_ONCURSOR)
    {
        return 0;
    }
    
    D2UnitStrc* pGridItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nGridItemGUID);
    if (!pGridItem || pGridItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    const uint8_t nInvPage = ITEMS_GetInvPage(pGridItem);
    if (nInvPage == INVPAGE_CUBE)
    {
        if (!gbBoxItemIdInitialized)
        {
            if (!DATATBLS_GetItemRecordFromItemCode(' xob', &gnBoxItemId))
            {
                return 0;
            }

            gbBoxItemIdInitialized = 1;
        }

        if (pCursorItem->dwClassId == gnBoxItemId)
        {
            return 0;
        }
    }
    else if (nInvPage == INVPAGE_TRADE)
    {
        *a7 = 1;
        return 0;
    }

    if (pGridItem->dwAnimMode != IMODE_STORED)
    {
        return 0;
    }

    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pGridItem);
    D2_ASSERT(pRemove);

    D2_ASSERT(pGridItem == pRemove);

    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pGridItem, pUnit, 0, 1);
    //INVENTORY_Return(__FILE__, __LINE__, pUnit->pInventory, CLIENTS_GetUnitX(pGridItem), CLIENTS_GetUnitY(pGridItem), UNITS_GetInventoryRecordId(pUnit, nInvPage, pGame->bExpansion), 0, nInvPage);

    ITEMS_SetInvPage(pGridItem, INVPAGE_NULL);
    INVENTORY_SetCursorItem(pUnit->pInventory, pGridItem);

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pUnit, pGridItem, 0);
    }

    ITEMS_SetCellOverlap(pGridItem, nInvPage);
    pGridItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    UNITS_ChangeAnimMode(pGridItem, IMODE_ONCURSOR);
    ITEMS_SetItemCMDFlag(pGridItem, 0x40000u, 1);

    if (pGridItem->dwUnitType == UNIT_ITEM && ITEMS_CheckIfSocketable(pGridItem) && pGridItem->pInventory && INVENTORY_GetItemCount(pGridItem->pInventory))
    {
        ITEMS_SetItemFlag(pGridItem, IFLAG_NEWITEM, 1);
    }

    if (ITEMS_CheckItemFlag(pGridItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pGridItem, IFLAG_NOEQUIP, 0);
    }

    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pGridItem);

    if (!INVENTORY_PlaceItemAtInventoryPage(pUnit->pInventory, pCursorItem, nX, nY, UNITS_GetInventoryRecordId(pUnit, nInvPage, pGame->bExpansion), 0, nInvPage))
    {
        *a7 = 1;
        return 0;
    }

    ITEMS_SetCellOverlap(pCursorItem, 0);
    ITEMS_SetInvPage(pCursorItem, nInvPage);
    UNITS_SetXForStaticUnit(pCursorItem, nX);
    UNITS_SetYForStaticUnit(pCursorItem, nY);

    if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pCursorItem, 1))
    {
        *a7 = 1;
        return 0;
    }

    pCursorItem->dwFlags &= ~UNITFLAG_TARGETABLE;

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pUnit, pCursorItem, 1);
    }

    if (ITEMS_IsCharmUsable(pCursorItem, pUnit))
    {
        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pCursorItem, pUnit, 0);
    }

    UNITS_ChangeAnimMode(pCursorItem, IMODE_STORED);

    if (pCursorItem->dwUnitType == UNIT_ITEM && ITEMS_CheckIfSocketable(pCursorItem) && pCursorItem->pInventory && INVENTORY_GetItemCount(pCursorItem->pInventory))
    {
        ITEMS_SetItemFlag(pCursorItem, IFLAG_NEWITEM, 1);
    }

    if (ITEMS_CheckItemFlag(pCursorItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pCursorItem, IFLAG_NOEQUIP, 0);
    }

    ITEMS_SetItemCMDFlag(pCursorItem, 0x40000u, 1);
    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pCursorItem);

    if (nInvPage == INVPAGE_TRADE)
    {
        sub_6FC92F10(pGame, pUnit, pCursorItem);
    }

    UNITS_RefreshInventory(pUnit, 1);

    if (ITEMS_IsCharmUsable(pGridItem, pUnit))
    {
        D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);
        UNITS_RefreshInventory(pUnit, 1);
    }

    return 1;
}

//D2Game.0x6FC471F0
void __fastcall D2GAME_RemoveItem_6FC471F0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t bSendPacket)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    if (pItem)
    {
        pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pItem);

    D2_ASSERT(pRemove);
    D2_ASSERT(pItem == pRemove);

    //INVENTORY_Return(__FILE__, __LINE__, pUnit->pInventory, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), UNITS_GetInventoryRecordId(pUnit, ITEMS_GetInvPage(pItem), pGame->bExpansion), 0, nPage);
    
    ITEMS_SetInvPage(pItem, INVPAGE_NULL);

    if (bSendPacket)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        D2GAME_PACKETS_SendPacket0x0A_RemoveObject_6FCC5F80(pItem, pClient);
    }

    ITEMS_RemoveFromAllPlayers(pGame, pItem);
}

//D2Game.0x6FC47380
void __fastcall sub_6FC47380(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    D2GAME_SendP0x9C_ItemAction_RemoveFromBelt_6FC3EBE0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pItem, 0x20);

    if (pItem)
    {
        pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    D2InventoryStrc* pInventory = pUnit->pInventory;
    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pInventory, pItem);

    D2_ASSERT(pRemove);
    D2_ASSERT(pItem == pRemove);

    //D2COMMON_10267_Return(pInventory, CLIENTS_GetUnitX(pItem));
    ITEMS_RemoveFromAllPlayers(pGame, pItem);
}

//D2Game.0x6FC47470
int32_t __fastcall sub_6FC47470(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTargetGUID, int32_t nUseItemGUID, int32_t* a5)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    D2UnitStrc* pTargetItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nTargetGUID);
    D2UnitStrc* pUseItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nUseItemGUID);

    *a5 = 0;

    if (!pUseItem)
    {
        *a5 = 1;
        return 0;
    }

    if (!pTargetItem || pTargetItem == pUseItem)
    {
        if (pUnit->pInventory)
        {
            for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
            {
                if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
                {
                    ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                    if (pUnit->dwUnitType == UNIT_PLAYER)
                    {
                        D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                    }
                }
            }
        }

        return 0;
    }

    if (pTargetItem->dwUnitType != UNIT_ITEM || pUseItem->dwUnitType != UNIT_ITEM || INVENTORY_GetCursorItem(pUnit->pInventory))
    {
        return 0;
    }

    const int32_t nTargetAnimMode = pTargetItem->dwAnimMode;
    const int32_t nUseItemAnimMode = pUseItem->dwAnimMode;

    if (nTargetAnimMode != IMODE_STORED && nTargetAnimMode != IMODE_EQUIP)
    {
        if (nUseItemAnimMode == IMODE_INBELT)
        {
            if (pUnit->pInventory)
            {
                for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
                {
                    if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
                    {
                        ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                        if (pUnit->dwUnitType == UNIT_PLAYER)
                        {
                            D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                        }
                    }
                }
            }

            D2GAME_SendP0x9C_ItemAction_RemoveFromBelt_6FC3EBE0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pUseItem, 0x20);
            sub_6FC49090(pGame, pUnit, pUseItem);
        }
        else if (nUseItemAnimMode == IMODE_STORED)
        {
            if (ITEMS_GetItemType(pUseItem) == ITEMTYPE_BOOK)
            {
                int32_t nQuantity = STATLIST_GetUnitStatUnsigned(pUseItem, STAT_QUANTITY, 0) - 1;
                if (nQuantity < 0)
                {
                    nQuantity = 0;
                }

                STATLIST_SetUnitStat(pUseItem, STAT_QUANTITY, nQuantity, 0);
                D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUseItem, 1, STAT_QUANTITY, nQuantity, 0);
            }
        }
        else
        {
            *a5 = 1;
        }

        return 0;
    }

    if (ITEMS_GetItemType(pUseItem) == ITEMTYPE_BOOK && STATLIST_GetUnitStatUnsigned(pUseItem, STAT_QUANTITY, 0) <= 0)
    {
        if (pUnit->pInventory)
        {
            for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
            {
                if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
                {
                    ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                    if (pUnit->dwUnitType == UNIT_PLAYER)
                    {
                        D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                    }
                }
            }
        }

        D2GAME_PACKETS_SendPacketSize06_6FC3C850(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x7C, pUseItem->dwUnitType, pUseItem->dwUnitId);
        return 0;
    }

    if (!SKILLITEM_pSpell_Handler(pGame, pUnit, pUseItem, pTargetItem, 0, 0))
    {
        return 1;
    }

    if (nUseItemAnimMode == IMODE_INBELT)
    {
        int32_t nSkillId = -1;
        if (ITEMS_CheckItemTypeId(pUseItem, ITEMTYPE_BOOK))
        {
            D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pUseItem, 0));
            D2_ASSERT(pBooksTxtRecord);
            nSkillId = pBooksTxtRecord->dwBookSkillId;
        }
        else if (ITEMS_CheckItemTypeId(pUseItem, ITEMTYPE_SCROLL))
        {
            D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pUseItem, 0));
            D2_ASSERT(pBooksTxtRecord);
            nSkillId = pBooksTxtRecord->dwScrollSkillId;
        }

        if (nSkillId != -1)
        {
            sub_6FC47C90(pUnit, nSkillId);
            D2GAME_SendP0x9C_ItemAction_RemoveFromBelt_6FC3EBE0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pUseItem, 0x20);
            sub_6FC49090(pGame, pUnit, pUseItem);

            if (pUnit->pInventory)
            {
                for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
                {
                    if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
                    {
                        ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                        if (pUnit->dwUnitType == UNIT_PLAYER)
                        {
                            D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                        }
                    }
                }
            }

            return 1;
        }

        D2GAME_PACKETS_SendPacketSize06_6FC3C850(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x7C, pUseItem->dwUnitType, pUseItem->dwUnitId);

        if (pUnit->pInventory)
        {
            for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
            {
                if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
                {
                    ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                    if (pUnit->dwUnitType == UNIT_PLAYER)
                    {
                        D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                    }
                }
            }
        }

        return 1;
    }

    if (nUseItemAnimMode)
    {
        *a5 = 1;
        return 0;
    }

    if (ITEMS_GetItemType(pUseItem) != ITEMTYPE_BOOK)
    {
        int32_t nSkillId = -1;
        if (ITEMS_CheckItemTypeId(pUseItem, ITEMTYPE_BOOK))
        {
            D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pUseItem, 0));
            D2_ASSERT(pBooksTxtRecord);
            nSkillId = pBooksTxtRecord->dwBookSkillId;
        }
        else if (ITEMS_CheckItemTypeId(pUseItem, ITEMTYPE_SCROLL))
        {
            D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pUseItem, 0));
            D2_ASSERT(pBooksTxtRecord);
            nSkillId = pBooksTxtRecord->dwScrollSkillId;
        }

        if (nSkillId != -1)
        {
            sub_6FC47C90(pUnit, nSkillId);
        }

        D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pUseItem, 0x20);
        D2GAME_RemoveItem_6FC471F0(pGame, pUnit, pUseItem, 0);

        if (pUnit->pInventory)
        {
            for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
            {
                if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
                {
                    ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                    if (pUnit->dwUnitType == UNIT_PLAYER)
                    {
                        D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                    }
                }
            }
        }

        return 1;
    }

    int32_t nSkillId = -1;
    if (ITEMS_CheckItemTypeId(pUseItem, ITEMTYPE_BOOK))
    {
        D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pUseItem, 0));
        D2_ASSERT(pBooksTxtRecord);
        nSkillId = pBooksTxtRecord->dwBookSkillId;
    }
    else if (ITEMS_CheckItemTypeId(pUseItem, ITEMTYPE_SCROLL))
    {
        D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pUseItem, 0));
        D2_ASSERT(pBooksTxtRecord);
        nSkillId = pBooksTxtRecord->dwScrollSkillId;
    }

    if (nSkillId != -1 && STATLIST_GetUnitStatUnsigned(pUseItem, STAT_QUANTITY, 0) > 0)
    {
        STATLIST_AddUnitStat(pUseItem, STAT_QUANTITY, -1, 0);
        D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUseItem, 1, STAT_QUANTITY, STATLIST_GetUnitStatUnsigned(pUseItem, STAT_QUANTITY, 0), 0);

        sub_6FC47C90(pUnit, nSkillId);
        D2GAME_PACKETS_SendPacketSize06_6FC3C850(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x7C, pUseItem->dwUnitType, pUseItem->dwUnitId);
    }
    else
    {
        D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pUseItem, 0x20);
        D2GAME_RemoveItem_6FC471F0(pGame, pUnit, pUseItem, 0);
    }

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    return 1;
}

//D2Game.0x6FC47C90
void __fastcall sub_6FC47C90(D2UnitStrc* pUnit, int32_t nSkillId)
{
    D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, -1);

    D2_ASSERT(pSkill);

    int32_t nQuantity = SKILLS_GetQuantity(pSkill) - 1;
    if (nQuantity <= 0)
    {
        if (UNITS_GetRightSkill(pUnit) == pSkill)
        {
            D2GAME_AssignSkill_6FD13800(pUnit, 0, 0, -1);
        }

        nQuantity = 0;
    }

    SKILLS_SetQuantity(pSkill, nQuantity);

    int32_t nUnitGUID = -1;
    int32_t nUnitType = 6;
    if (pUnit)
    {
        nUnitGUID = pUnit->dwUnitId;
        nUnitType = pUnit->dwUnitType;
    }

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket0x22_6FC3DBE0(pClient, nUnitType, nUnitGUID, nSkillId, nQuantity);
}

//D2Game.0x6FC47D30
int32_t __fastcall sub_6FC47D30(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t nX, int32_t nY, int32_t* a6)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a6 = 0;

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        *a6 = 1;
        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM || INVENTORY_GetCursorItem(pUnit->pInventory))
    {
        return 0;
    }

    if (pItem->dwAnimMode == IMODE_STORED && ITEMS_CheckIfUseable(pItem))
    {
        if ((ITEMS_GetItemType(pItem) != ITEMTYPE_BOOK || STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0) > 0) && !sub_6FC937A0(pGame, pUnit))
        {
            if (SKILLITEM_pSpell_Handler(pGame, pUnit, pItem, pItem, nX, nY))
            {
                if (ITEMS_GetItemType(pItem) == ITEMTYPE_BOOK)
                {
                    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
                    {
                        D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
                        D2_ASSERT(pBooksTxtRecord);

                        if (pBooksTxtRecord->dwBookSkillId != -1 && STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0) > 0)
                        {
                            STATLIST_AddUnitStat(pItem, STAT_QUANTITY, -1, 0);

                            D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_QUANTITY, STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0), 0);
                            D2GAME_PACKETS_SendPacketSize06_6FC3C850(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x7C, pItem->dwUnitType, pItem->dwUnitId);
                            sub_6FC47C90(pUnit, pBooksTxtRecord->dwBookSkillId);
                        }
                    }
                    else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
                    {
                        D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
                        D2_ASSERT(pBooksTxtRecord);

                        if (pBooksTxtRecord->dwScrollSkillId != -1 && STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0) > 0)
                        {
                            STATLIST_AddUnitStat(pItem, STAT_QUANTITY, -1, 0);

                            D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_QUANTITY, STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0), 0);
                            D2GAME_PACKETS_SendPacketSize06_6FC3C850(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), 0x7C, pItem->dwUnitType, pItem->dwUnitId);
                            sub_6FC47C90(pUnit, pBooksTxtRecord->dwScrollSkillId);
                        }
                    }
                }
                else
                {
                    int32_t nSkillId = -1;
                    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
                    {
                        D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
                        D2_ASSERT(pBooksTxtRecord);
                        nSkillId = pBooksTxtRecord->dwBookSkillId;
                    }
                    else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
                    {
                        D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
                        D2_ASSERT(pBooksTxtRecord);
                        nSkillId = pBooksTxtRecord->dwScrollSkillId;
                    }

                    if (nSkillId != -1)
                    {
                        sub_6FC47C90(pUnit, nSkillId);
                    }

                    if (pUnit->pInventory)
                    {
                        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
                        {
                            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
                            {
                                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                                if (pUnit->dwUnitType == UNIT_PLAYER)
                                {
                                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                                }
                            }
                        }
                    }

                    D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pItem, 0x20);
                    D2GAME_RemoveItem_6FC471F0(pGame, pUnit, pItem, 0);
                }
            }
            else
            {
                D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
                D2BitBufferStrc* pQuestFlags = UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty];
                const uint32_t dwItemCode = pItemsTxtRecord->dwCode;

                if (dwItemCode == ' ssa')
                {
                    if (pUnit->pInventory)
                    {
                        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
                        {
                            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
                            {
                                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                                if (pUnit->dwUnitType == UNIT_PLAYER)
                                {
                                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                                }
                            }
                        }
                    }

                    if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_CUSTOM1))
                    {
                        QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A2Q1, QFLAG_CUSTOM1);
                        STATLIST_AddUnitStat(pUnit, STAT_SKILLPTS, 1, 0);
                        QUESTS_SendLogUpdate(pUnit, QUEST_A2Q1_RADAMENT);
                        D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pItem, 0x20);
                        D2GAME_RemoveItem_6FC471F0(pGame, pUnit, pItem, 0);
                        return 1;
                    }
                }
                else if (dwItemCode == ' zyx')
                {
                    if (pUnit->pInventory)
                    {
                        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
                        {
                            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
                            {
                                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                                if (pUnit->dwUnitType == UNIT_PLAYER)
                                {
                                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                                }
                            }
                        }
                    }

                    if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_CUSTOM1))
                    {
                        QUESTRECORD_ClearQuestState(pQuestFlags, QUESTSTATEFLAG_A3Q4, QFLAG_CUSTOM1);
                        STATLIST_AddUnitStat(pUnit, STAT_MAXHP, 5120, 0);
                        QUESTS_SendLogUpdate(pUnit, QUEST_A3Q4_GOLDENBIRD);
                        D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pItem, 0x20);
                        D2GAME_RemoveItem_6FC471F0(pGame, pUnit, pItem, 0);
                        return 1;
                    }
                }
                else if (dwItemCode == ' 2rt')
                {
                    if (pUnit->pInventory)
                    {
                        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
                        {
                            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
                            {
                                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                                if (pUnit->dwUnitType == UNIT_PLAYER)
                                {
                                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                                }
                            }
                        }
                    }

                    if (QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM4) && !QUESTRECORD_GetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM3))
                    {
                        QUESTRECORD_SetQuestState(pQuestFlags, QUESTSTATEFLAG_A5Q3, QFLAG_CUSTOM3);
                        ACT5Q3_ApplyResistanceReward(pGame, pUnit);
                        QUESTS_SendLogUpdate(pUnit, QUEST_A5Q3_PRISONOFICE);
                        D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pItem, 0x20);
                        D2GAME_RemoveItem_6FC471F0(pGame, pUnit, pItem, 0);
                        return 1;
                    }
                }

                SUNIT_AttachSound(pUnit, 19, pUnit);
            }

            return 1;
        }
    }
    else
    {
        *a6 = 1;
    }

    return 0;
}

//D2Game.0x6FC484E0
int32_t __fastcall sub_6FC484E0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID1, int32_t nItemGUID2, int32_t* a5)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a5 = 0;

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    if (nItemGUID1 == nItemGUID2)
    {
        *a5 = 1;
        return 0;
    }

    D2UnitStrc* pItem1 = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID1);
    if (!pItem1)
    {
        *a5 = 1;
        return 0;
    }

    if (pItem1->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    D2UnitStrc* pItem2 = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID2);
    if (!pItem2)
    {
        *a5 = 1;
        return 0;
    }

    if (pItem2->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (ITEMS_GetInvPage(pItem2) == INVPAGE_TRADE)
    {
        *a5 = 1;
        return 0;
    }

    if (!ITEMS_AreStackablesEqual(pItem1, pItem2))
    {
        return 0;
    }

    if (pItem1->dwClassId != pItem2->dwClassId)
    {
        return 1;
    }

    if (ITEMS_AreStackablesEqual(pItem1, pItem2))
    {
        const int32_t nSrcValue = STATLIST_GetUnitStatUnsigned(pItem1, STAT_QUANTITY, 0);
        D2_ASSERT(nSrcValue >= 0);

        const int32_t nDstValue = STATLIST_GetUnitStatUnsigned(pItem2, STAT_QUANTITY, 0);
        D2_ASSERT(nDstValue >= 0);

        const int32_t nMaxStack = ITEMS_GetTotalMaxStack(pItem2);
        D2_ASSERT(nMaxStack >= 0);

        int32_t nQuantity = nDstValue + nSrcValue;
        if (nQuantity <= nMaxStack)
        {
            if (ITEMS_HasDurability(pItem1))
            {
                const int32_t nDurability = STATLIST_GetUnitStatUnsigned(pItem1, STAT_DURABILITY, 0);
                if (nDurability < STATLIST_GetUnitStatUnsigned(pItem2, STAT_DURABILITY, 0))
                {
                    STATLIST_SetUnitStat(pItem2, STAT_DURABILITY, nDurability, 0);
                    D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem2, 1, STAT_DURABILITY, nDurability, 0);
                }
            }

            D2_ASSERT(nQuantity >= 0);

            STATLIST_SetUnitStat(pItem2, STAT_QUANTITY, nQuantity, 0);

            D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem2, 1, STAT_QUANTITY, nQuantity, 0);

            if (ITEMS_GetItemType(pItem1) == ITEMTYPE_BOOK && ITEMS_GetItemType(pItem2) == ITEMTYPE_BOOK)
            {
                sub_6FC43AF0(pUnit, pItem2, nSrcValue);
            }

            INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);

            D2GAME_PACKETS_SendPacket0x42_6FC3EE40(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit);
            ITEMS_RemoveFromAllPlayers(pGame, pItem1);
        }
        else
        {
            STATLIST_SetUnitStat(pItem2, STAT_QUANTITY, nMaxStack, 0);
            D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem2, 1, STAT_QUANTITY, nMaxStack, 0);
            
            nQuantity -= nMaxStack;
            D2_ASSERT(nQuantity >= 0);

            STATLIST_SetUnitStat(pItem1, STAT_QUANTITY, nQuantity, 0);

            D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem1, 1, STAT_QUANTITY, nQuantity, 0);

            if (ITEMS_GetItemType(pItem1) == ITEMTYPE_BOOK && ITEMS_GetItemType(pItem2) == ITEMTYPE_BOOK)
            {
                sub_6FC43AF0(pUnit, pItem2, ITEMS_GetTotalMaxStack(pItem2) - nDstValue);
            }

            ITEMS_SetItemFlag(pItem2, IFLAG_DELETED, 1);
        }
    }

    ITEMS_SetItemCMDFlag(pItem2, 0x100u, 1);
    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem2);
    UNITS_RefreshInventory(pUnit, 1);

    return 1;
}

//D2Game.0x6FC48930
int32_t __stdcall D2GAME_Return0_6FC48930(int32_t a1, int32_t* a2)
{
    return 0;
}

//D2Game.0x6FC48940
int32_t __fastcall sub_6FC48940(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t nBeltSlot, int32_t bFindFreeSlot, int32_t* a6)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a6 = 0;

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        *a6 = 1;
        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pItem->dwAnimMode != IMODE_ONCURSOR)
    {
        *a6 = 1;
        return 0;
    }

    if (!ITEMS_CheckIfBeltable(pItem))
    {
        return 0;
    }

    if (bFindFreeSlot)
    {
        if (!INVENTORY_PlaceItemInFreeBeltSlot(pUnit->pInventory, pItem))
        {
            return 0;
        }
    }
    else
    {
        //ITEMS_GetDimensions(pItem, (BYTE*)&nItemGUID, (BYTE*)&a6, __FILE__, __LINE__);

        if (!INVENTORY_PlaceItemInBeltSlot(pUnit->pInventory, pItem, nBeltSlot))
        {
            return 0;
        }

        ITEMS_SetInvPage(pItem, 0xFFu);
    }

    if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pItem, 2))
    {
        *a6 = 1;
        return 0;
    }

    INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pUnit, pItem, 1);
    }

    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;

    if (ITEMS_IsCharmUsable(pItem, pUnit))
    {
        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pUnit, 0);
    }

    UNITS_ChangeAnimMode(pItem, IMODE_INBELT);
    ITEMS_SetInvPage(pItem, 0xFFu);
    ITEMS_SetItemCMDFlag(pItem, 0x400u, 1);
    UNITS_RefreshInventory(pUnit, 1);
    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
    return 1;
}

//D2Game.0x6FC48B40
int32_t __fastcall sub_6FC48B40(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t* a4)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a4 = 0;

    D2InventoryStrc* pInventory = pUnit->pInventory;
    D2_ASSERT(pInventory);

    if (INVENTORY_GetCursorItem(pInventory))
    {
        return 0;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        *a4 = 1;
        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pItem->dwAnimMode != IMODE_INBELT)
    {
        *a4 = 1;
        return 0;
    }

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    const int32_t nX = CLIENTS_GetUnitX(pItem);
    INVENTORY_RemoveItemFromInventory(pInventory, pItem);
    //D2COMMON_10267_Return(pInventory, nX);
    INVENTORY_SetCursorItem(pInventory, pItem);

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pUnit, pItem, 0);
    }

    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;

    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pItem, pUnit, 0, 1);
    UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
    ITEMS_SetItemCMDFlag(pItem, 0x800u, 1);
    UNITS_RefreshInventory(pUnit, 1);
    INVENTORY_AddItemToTradeInventory(pInventory, pItem);
    sub_6FC49140(pGame, pUnit, nX);

    return 1;
}

//D2Game.0x6FC48D50
int32_t __fastcall sub_6FC48D50(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nCursorItemGUID, int32_t nBeltItemGUID, int32_t* a5)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a5 = 0;

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pCursorItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nCursorItemGUID);
    if (!pCursorItem || pCursorItem->dwUnitType != UNIT_ITEM || pCursorItem->dwAnimMode != IMODE_ONCURSOR || !ITEMS_CheckIfBeltable(pCursorItem))
    {
        return 0;
    }
    
    D2UnitStrc* pBeltItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nBeltItemGUID);
    if (!pBeltItem || pBeltItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pBeltItem->dwAnimMode != IMODE_INBELT)
    {
        *a5 = 1;
        return 0;
    }

    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pBeltItem);
    D2_ASSERT(pRemove);
    D2_ASSERT(pBeltItem == pRemove);

    INVENTORY_SetCursorItem(pUnit->pInventory, pBeltItem);
    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pUnit, pBeltItem, 0);
    }

    pBeltItem->dwFlags &= ~UNITFLAG_TARGETABLE;

    D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pBeltItem, pUnit, 0, 1);
    UNITS_ChangeAnimMode(pBeltItem, IMODE_ONCURSOR);
    ITEMS_SetItemCMDFlag(pBeltItem, 0x1000u, 1);
    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pBeltItem);
    if (!INVENTORY_PlaceItemInBeltSlot(pUnit->pInventory, pCursorItem, CLIENTS_GetUnitX(pBeltItem)))
    {
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
    }

    UNITS_SetXForStaticUnit(pCursorItem, CLIENTS_GetUnitX(pBeltItem));
    UNITS_SetYForStaticUnit(pCursorItem, 0);
    if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pCursorItem, 2))
    {
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        sub_6FC42120(pUnit, pCursorItem, 1);
    }

    pCursorItem->dwFlags &= ~UNITFLAG_TARGETABLE;

    if (ITEMS_IsCharmUsable(pCursorItem, pUnit))
    {
        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pCursorItem, pUnit, 0);
    }

    UNITS_ChangeAnimMode(pCursorItem, IMODE_INBELT);
    ITEMS_SetInvPage(pCursorItem, INVPAGE_NULL);
    ITEMS_SetItemCMDFlag(pCursorItem, 0x1000u, 1);
    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pCursorItem);
    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);
    return 1;
}

//D2Game.0x6FC49090
void __fastcall sub_6FC49090(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    if (pItem)
    {
        pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pItem);

    D2_ASSERT(pRemove);
    D2_ASSERT(pItem == pRemove);

    ITEMS_RemoveFromAllPlayers(pGame, pItem);
}

//D2Game.0x6FC49140
void __fastcall sub_6FC49140(D2GameStrc* pGame, D2UnitStrc* pUnit, int8_t nX)
{
    const int32_t nPosition = D2Clamp(nX & 3, 0, 3);
    int32_t j = nPosition;
    for (int32_t i = nPosition; i < 4; ++i)
    {
        D2UnitStrc* pBeltItem = INVENTORY_GetItemFromBeltSlot(pUnit->pInventory, 4 * i);
        if (pBeltItem)
        {
            if (j != i)
            {
                INVENTORY_PlaceItemInBeltSlot(pUnit->pInventory, pBeltItem, 4 * j);
                ITEMS_SetItemFlag(pBeltItem, IFLAG_UNK1, 1);
                ITEMS_SetItemFlag(pBeltItem, IFLAG_NEWITEM, 1);

                if (ITEMS_CheckItemFlag(pBeltItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
                {
                    ITEMS_SetItemFlag(pBeltItem, IFLAG_NOEQUIP, 0);
                }

                UNITS_RefreshInventory(pUnit, 1);
                INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pBeltItem);
            }

            ++j;
        }
    }
}

//D2Game.0x6FC49220
int32_t __fastcall sub_6FC49220(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t nX, int32_t nY, int32_t* a6, int32_t bUseOnMerc)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    *a6 = 0;

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        *a6 = 1;
        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM || INVENTORY_GetCursorItem(pUnit->pInventory))
    {
        return 0;
    }

    if (pItem->dwAnimMode != IMODE_INBELT || !ITEMS_CheckIfUseable(pItem))
    {
        *a6 = 1;
        return 0;
    }

    if (sub_6FC937A0(pGame, pUnit))
    {
        return 0;
    }

    if (bUseOnMerc && pUnit->dwUnitType == UNIT_PLAYER && pGame->bExpansion && (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_HEALING_POTION) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_THAWING_POTION) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANTIDOTE_POTION)))
    {
        D2UnitStrc* pMerc = sub_6FC7E8B0(pGame, pUnit, PETTYPE_HIREABLE, 0);
        if (pMerc && !SKILLITEM_pSpell_Handler(pGame, pMerc, pItem, pItem, nX, nY))
        {
            return 0;
        }
    }
    else
    {
        if (!SKILLITEM_pSpell_Handler(pGame, pUnit, pItem, pItem, nX, nY))
        {
            return 0;
        }
    }

    const int8_t nOldX = CLIENTS_GetUnitX(pItem);

    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
    {
        D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
        D2_ASSERT(pBooksTxtRecord);

        const int32_t nSkillId = ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK) ? pBooksTxtRecord->dwBookSkillId : (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL) ? pBooksTxtRecord->dwScrollSkillId : -1);
        if (nSkillId != -1)
        {
            sub_6FC47C90(pUnit, nSkillId);
        }
    }

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            if (INVENTORY_UnitIsItem(i) && ITEMS_CheckItemFlag(i, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(i, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), i, -1, 1, 0, -1);
                }
            }
        }
    }

    D2GAME_SendP0x9C_ItemAction_RemoveFromBelt_6FC3EBE0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pItem, 0x20);

    sub_6FC49090(pGame, pUnit, pItem);
    sub_6FC49140(pGame, pUnit, nOldX);
    return 1;
}

//D2Game.0x6FC49670
void __fastcall D2GAME_ITEMS_Identify_6FC49670(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
    if (ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, __FILE__) || !pPlayer->pInventory)
    {
        return;
    }

    if (ITEMS_CheckItemCMDFlag(pItem, 0x100000u))
    {
        FOG_DisplayAssert("!ITEMSDataTestCmdFlags(hItem, ITEM_CMDFLAG_SETHANDITEM)", __FILE__, __LINE__);
        exit(-1);
    }

    ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 1);

    if ((!pItem || pItem->dwAnimMode == IMODE_STORED) && ITEMS_IsCharmUsable(pItem, pPlayer))
    {
        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pPlayer, 0);
        D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pPlayer, 0);
        UNITS_RefreshInventory(pPlayer, 1);
    }

    ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);

    if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);
    }

    INVENTORY_AddItemToTradeInventory(pPlayer->pInventory, pItem);
    UNITS_RefreshInventory(pPlayer, 1);
    SUNIT_AttachSound(pPlayer, 6u, pPlayer);
}

//D2Game.0x6FC49760
int32_t __fastcall D2GAME_RemoveItemIfOnCursor_6FC49760(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
    if (!pItem || INVENTORY_GetCursorItem(pPlayer->pInventory) != pItem)
    {
        return 0;
    }

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket0x42_6FC3EE40(pClient, pPlayer);
    INVENTORY_SetCursorItem(pPlayer->pInventory, 0);
    ITEMS_RemoveFromAllPlayers(pGame, pItem);
    return 1;
}

//D2Game.0x6FC497E0
int32_t __fastcall D2GAME_ITEMSOCKET_PlaceItem_6FC497E0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSocketFillerGUID, int32_t nItemGUID, int32_t* bPlaced, int32_t bRefresh, int32_t bResetCursorItem, int32_t bCheckMode)
{
    D2_ASSERT(pGame);

    *bPlaced = 0;

    if (pUnit && pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            D2UnitStrc* pItem = INVENTORY_UnitIsItem(i);
            if (ITEMS_CheckItemFlag(pItem, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(pItem, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pSocketFiller = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nSocketFillerGUID);
    if (!pSocketFiller)
    {
        *bPlaced = 1;
        return 0;
    }

    if (pSocketFiller->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pSocketFiller->dwAnimMode != IMODE_ONCURSOR)
    {
        *bPlaced = 1;
        return 0;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        *bPlaced = 1;
        return 0;
    }

    if (pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (!ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, __FILE__))
    {
        return 0;
    }

    if (bCheckMode && pItem->dwAnimMode != IMODE_STORED && pItem->dwAnimMode != IMODE_EQUIP)
    {
        return 0;
    }

    if (!ITEMS_IsSocketFiller(pSocketFiller) || !ITEMS_CheckItemFlag(pSocketFiller, IFLAG_IDENTIFIED, __LINE__, __FILE__))
    {
        return 0;
    }

    if (!ITEMS_CheckItemFlag(pItem, IFLAG_SOCKETED, __LINE__, __FILE__) || !ITEMS_CheckIfSocketable(pItem))
    {
        return 0;
    }

    D2InventoryStrc* pInventory = pItem->pInventory;
    if (!pInventory)
    {
        pInventory = INVENTORY_AllocInventory(nullptr, pItem);
        if (!pInventory)
        {
            return 0;
        }
    }

    if (INVENTORY_GetItemCount(pInventory) >= ITEMS_GetSockets(pItem))
    {
        return 0;
    }

    if (!INVENTORY_PlaceItemInSocket(pInventory, pSocketFiller, 1))
    {
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
    }

    pSocketFiller->dwFlags &= ~UNITFLAG_TARGETABLE;

    if (bResetCursorItem && pUnit)
    {
        INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);
    }

    D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pSocketFiller, pItem, 0);
    D2Common_10525(pSocketFiller, pItem);
    UNITS_ChangeAnimMode(pSocketFiller, IMODE_SOCKETED);

    if (ITEMS_GetRunesTxtRecordFromItem(pItem))
    {
        ITEMMODS_UpdateRuneword(pItem, pSocketFiller, 0);
        sub_6FC512C0(pGame, pItem);
        sub_6FC51310(pGame, pItem);
    }
    else if (bRefresh)
    {
        ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);

        if (ITEMS_CheckItemFlag(pItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
        {
            ITEMS_SetItemFlag(pItem, IFLAG_NOEQUIP, 0);
        }

        if (pUnit)
        {
            UNITS_RefreshInventory(pUnit, 1);
            INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
        }
    }

    return 1;
}

//D2Game.0x6FC49AE0
int32_t __fastcall sub_6FC49AE0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nScrollGUID, int32_t nBookGUID, int32_t* a5)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            D2UnitStrc* pItem = INVENTORY_UnitIsItem(i);
            if (ITEMS_CheckItemFlag(pItem, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(pItem, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, -1, 1, 0, -1);
                }
            }
        }
    }

    *a5 = 0;
    
    D2UnitStrc* pScroll = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nScrollGUID);
    if (!pScroll)
    {
        *a5 = 1;
        return 0;
    }

    if (pScroll->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pScroll->dwAnimMode != IMODE_ONGROUND && pScroll->dwAnimMode != IMODE_ONCURSOR || !ITEMS_CheckItemTypeId(pScroll, ITEMTYPE_SCROLL))
    {
        *a5 = 1;
        return 0;
    }

    D2UnitStrc* pBook = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nBookGUID);
    if (!pBook)
    {
        *a5 = 1;
        return 0;
    }

    if (pBook->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pBook->dwAnimMode || ITEMS_GetItemType(pBook) != ITEMTYPE_BOOK)
    {
        *a5 = 1;
        return 0;
    }

    D2InventoryStrc* pInventory = pUnit->pInventory;
    D2_ASSERT(pInventory);

    if (ITEMS_GetSuffixId(pScroll, 0) != ITEMS_GetSuffixId(pBook, 0))
    {
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
    }

    const int32_t nQuantity = STATLIST_GetUnitStatUnsigned(pBook, STAT_QUANTITY, 0);
    if (nQuantity >= ITEMS_GetTotalMaxStack(pBook))
    {
        return 0;
    }

    if (INVENTORY_GetCursorItem(pUnit->pInventory) == pScroll)
    {
        D2GAME_PACKETS_SendPacket0x42_6FC3EE40(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit);
        INVENTORY_SetCursorItem(pInventory, 0);
        ITEMS_RemoveFromAllPlayers(pGame, pScroll);
    }
    else
    {
        D2RoomStrc* pRoom = UNITS_GetRoom(pScroll);
        if (pRoom)
        {
            DUNGEON_AllocDrlgDelete(pRoom, pScroll->dwUnitType, pScroll->dwUnitId);
            UNITS_FreeCollisionPath(pScroll);
            UNITROOM_RemoveUnitFromRoom(pScroll);
        }

        pScroll->dwFlags &= ~UNITFLAG_TARGETABLE;
        ITEMS_RemoveFromAllPlayers(pGame, pScroll);
        INVENTORY_SetCursorItem(pInventory, 0);
    }

    STATLIST_AddUnitStat(pBook, STAT_QUANTITY, 1, 0);
    D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pBook, 1, STAT_QUANTITY, STATLIST_GetUnitStatUnsigned(pBook, STAT_QUANTITY, 0), 0);
    sub_6FC43AF0(pUnit, pBook, 1);
    return 1;
}

//D2Game.0x6FC49DC0
int32_t __fastcall sub_6FC49DC0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, int32_t nCubeGUID, int32_t* a5)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            D2UnitStrc* pItem = INVENTORY_UnitIsItem(i);
            if (ITEMS_CheckItemFlag(pItem, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(pItem, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, -1, 1, 0, -1);
                }
            }
        }
    }

    *a5 = 0;
    D2UnitStrc* pCube = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nCubeGUID);
	if (!pCube || pCube->dwAnimMode || ITEMS_GetBaseCode(pCube) != ' xob')
	{
		*a5 = 1;
		return 0;
	}

    if (sub_6FC937A0(pGame, pUnit) && ITEMS_GetInvPage(pCube))
    {
        SUNIT_AttachSound(pUnit, 0x13u, pUnit);
        return 0;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
	if (!pItem)
	{
		*a5 = 1;
		return 0;
	}
		
    if (pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (pItem->dwAnimMode != IMODE_ONGROUND && pItem->dwAnimMode != IMODE_ONCURSOR)
    {
        *a5 = 1;
        return 0;
    }

    if (pCube->dwUnitType == UNIT_ITEM)
    {
        if (pCube->dwAnimMode == IMODE_STORED)
        {
            ITEMS_SetInvPage(pItem, INVPAGE_CUBE);
            D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pUnit, pItem->dwUnitId, 0, 0, 1, 1, nullptr);
            return 1;
        }

		*a5 = 1;
    }

    return 0;
}

//D2Game.0x6FC49F80
int32_t __fastcall sub_6FC49F80(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nItemGUID, uint8_t nBodyLoc)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    if (pUnit->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            D2UnitStrc* pItem = INVENTORY_UnitIsItem(i);
            if (ITEMS_CheckItemFlag(pItem, IFLAG_TARGETING, __LINE__, __FILE__))
            {
                ITEMS_SetItemFlag(pItem, IFLAG_TARGETING, 0);
                if (pUnit->dwUnitType == UNIT_PLAYER)
                {
                    D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, -1, 1, 0, -1);
                }
            }
        }
    }

    D2UnitStrc* pGridItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pGridItem || pGridItem->dwUnitType != UNIT_ITEM || pGridItem->dwAnimMode != IMODE_STORED || !ITEMS_CheckRequirements(pGridItem, pUnit, 0, nullptr, nullptr, nullptr))
    {
        return 0;
    }

    D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pGridItem);

    D2_ASSERT(pRemove);
    D2_ASSERT(pGridItem == pRemove);

    ITEMS_SetInvPage(pGridItem, 0xFFu);

    if (!INVENTORY_PlaceItemInBodyLoc(pUnit->pInventory, pGridItem, nBodyLoc))
    {
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
    }

    if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pGridItem, 3))
    {
        //D2COMMON_10254_Return0(pUnit->pInventory, nBodyLoc);
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
    }

    ITEMS_SetBodyLocation(pGridItem, nBodyLoc);
    INVENTORY_UpdateWeaponGUIDOnInsert(pUnit->pInventory, pGridItem);
    INVENTORY_SetCursorItem(pUnit->pInventory, 0);

    D2_ASSERT(!ITEMS_CheckItemTypeId(pGridItem, ITEMTYPE_CHARM));

    D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pGridItem, pUnit, 0);
    pGridItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    UNITS_ChangeAnimMode(pGridItem, 1);
    ITEMS_SetInvPage(pGridItem, 0xFFu);
    ITEMS_SetItemCMDFlag(pGridItem, 0x8000, 1);

    if (pGridItem->dwUnitType == UNIT_ITEM && ITEMS_CheckIfSocketable(pGridItem) && pGridItem->pInventory && INVENTORY_GetItemCount(pGridItem->pInventory))
    {
        ITEMS_SetItemFlag(pGridItem, IFLAG_NEWITEM, 1);
    }

    if (ITEMS_CheckItemFlag(pGridItem, IFLAG_NOEQUIP, __LINE__, __FILE__))
    {
        ITEMS_SetItemFlag(pGridItem, IFLAG_NOEQUIP, 0);
    }

    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pGridItem);
    sub_6FC45300(pUnit);
    UNITS_RefreshInventory(pUnit, 1);
    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);
    return 1;
}

//D2Game.0x6FC4A2E0
void __fastcall sub_6FC4A2E0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!ITEMS_CheckItemFlag(pUnit, IFLAG_BROKEN, __LINE__, __FILE__) && ITEMS_HasDurability(pUnit) && sub_6FC4A350(pGame, pUnit, STAT_ITEM_REPLENISH_DURABILITY, STAT_DURABILITY, STATLIST_GetMaxDurabilityFromUnit(pUnit)))
    {
        return;
    }

    if (ITEMS_CheckIfStackable(pUnit))
    {
        sub_6FC4A350(pGame, pUnit, STAT_ITEM_REPLENISH_QUANTITY, STAT_QUANTITY, ITEMS_GetTotalMaxStack(pUnit));
    }
}

//D2Game.0x6FC4A350
int32_t __fastcall sub_6FC4A350(D2GameStrc* pGame, D2UnitStrc* pItem, int32_t nLengthStatId, int32_t nValueStatId, int32_t nMaxValue)
{
    const int32_t nLength = STATLIST_GetUnitStatUnsigned(pItem, nLengthStatId, 0);
    if (!nLength)
    {
        return 0;
    }

    const int32_t nValue = STATLIST_GetUnitStatUnsigned(pItem, nValueStatId, 0) + 1;
    if (nValue > nMaxValue)
    {
        return 1;
    }

    STATLIST_SetUnitStat(pItem, nValueStatId, nValue, 0);

    const int32_t nOwnerGUID = ITEMS_GetOwnerId(pItem);
    if (nOwnerGUID != -1)
    {
        D2UnitStrc* pOwner = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nOwnerGUID);
        if (pOwner)
        {
            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pOwner, __FILE__, __LINE__);
            if (pClient)
            {
                D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(pClient, pItem, 1, nValueStatId, nValue, 0);
                if (nValue >= 1)
                {
                    if (ITEMS_CheckItemFlag(pItem, IFLAG_BROKEN, __LINE__, __FILE__))
                    {
                        D2GAME_RepairBrokenItem_6FC4B630(pGame, pOwner, pItem);
                    }
                }
            }
        }
    }

    int32_t nDuration = 2500 / nLength + 1;
    if (nDuration < 125)
    {
        nDuration = 125;
    }

    EVENT_SetEvent(pGame, pItem, UNITEVENTCALLBACK_STATREGEN, pGame->dwGameFrame + nDuration, 0, 0);

    return 1;
}

//D2Game.0x6FC4A450
void __fastcall sub_6FC4A450(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    STATLIST_UpdateStatListsExpiration(pUnit, pGame->dwGameFrame);
}

void __fastcall ITEMMODE_EventFn4_Return(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
}

//D2Game.0x6FC4A460
void __fastcall sub_6FC4A460(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventType)
{
    void(__fastcall* dword_6FD27DDC[])(D2GameStrc*, D2UnitStrc*) =
    {
        nullptr,
        nullptr,
        nullptr,
        sub_6FC4A2E0,
        ITEMMODE_EventFn4_Return,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        sub_6FC4A450,
        nullptr,
        nullptr,
    };

    if (IsBadCodePtr((FARPROC)dword_6FD27DDC[nEventType]))
    {
        FOG_DisplayAssert("scpfnEventFunctions[eEventType]", __FILE__, __LINE__);
        exit(-1);
    }

    dword_6FD27DDC[nEventType](pGame, pUnit);
}

//D2Game.0x6FC4A4B0
int32_t __fastcall D2GAME_DoKeyCheck_6FC4A4B0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);

    if (!pUnit->pInventory)
    {
        return 0;
    }

    for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
    {
        D2UnitStrc* pItem = INVENTORY_UnitIsItem(i);
        if (ITEMS_CheckItemFlag(pItem, IFLAG_TARGETING, __LINE__, __FILE__))
        {
            ITEMS_SetItemFlag(pItem, IFLAG_TARGETING, 0);
            if (pUnit->dwUnitType == UNIT_PLAYER)
            {
                D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, -1, 1, 0, -1);
            }
        }
    }

    for (D2UnitStrc* i = INVENTORY_GetFirstItem(pUnit->pInventory); i; i = INVENTORY_GetNextItem(i))
    {
        D2UnitStrc* pItem = INVENTORY_UnitIsItem(i);
        if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_KEY) && ITEMS_GetInvPage(pItem) == 0)
        {
            if (STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0) - 1 <= 0)
            {
                D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pItem, 0x20);
                D2GAME_RemoveItem_6FC471F0(pGame, pUnit, pItem, 0);
            }
            else
            {
                STATLIST_AddUnitStat(pItem, STAT_QUANTITY, -1, 0);
                D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pItem, 1, STAT_QUANTITY, STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0), 0);
            }

            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC4A660
int32_t __fastcall D2GAME_Transmogrify_6FC4A660(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    D2_ASSERT(pGame);
    D2_ASSERT(pUnit);
    D2_ASSERT(pItem);
    D2_ASSERT(pItem->dwUnitType == UNIT_ITEM);

    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem->dwClassId);
    D2_ASSERT(pItemsTxtRecord);

    if (!pItemsTxtRecord->nTransmogrify)
    {
        return 0;
    }

    int32_t nItemId = 0;
    D2_ASSERT(DATATBLS_GetItemRecordFromItemCode(pItemsTxtRecord->dwTransmogrifyType, &nItemId));

    if (pItem->dwAnimMode != IMODE_STORED)
    {
        return 0;
    }

    D2ItemDropStrc itemDrop = {};
    itemDrop.pGame = pGame;
    itemDrop.nItemLvl = ITEMS_GetItemLevel(pItem);
    itemDrop.wItemFormat = pGame->wItemFormat;
    itemDrop.nId = nItemId;
    itemDrop.nSpawnType = 4;
    itemDrop.nX = CLIENTS_GetUnitX(pItem);
    itemDrop.nY = CLIENTS_GetUnitY(pItem);
    itemDrop.pSeed = &pItem->pSeed;
    itemDrop.wUnitInitFlags = 1;
    itemDrop.nQuality = 2;
    itemDrop.dwSeed = pItem->dwInitSeed;
    itemDrop.dwItemSeed = ITEMS_GetItemStartSeed(pItem);
    itemDrop.nItemIndex = 2;

    D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pItem, 0x20);
    D2GAME_RemoveItem_6FC471F0(pGame, pUnit, pItem, 0);

    D2UnitStrc* pNewItem = D2GAME_CreateItemEx_6FC4ED80(pGame, &itemDrop, 0);
    D2_ASSERT(pNewItem);

    int32_t nQuantity = 0;
    if (pItemsTxtRecord->nTmogMin && pItemsTxtRecord->nTmogMax)
    {
        nQuantity = ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, pItemsTxtRecord->nTmogMax - pItemsTxtRecord->nTmogMin) + pItemsTxtRecord->nTmogMin;
    }

    STATLIST_SetUnitStat(pNewItem, STAT_QUANTITY, nQuantity, 0);
    ITEMS_SetInvPage(pNewItem, INVPAGE_INVENTORY);

    if (!D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pUnit, pNewItem->dwUnitId, itemDrop.nX, itemDrop.nY, 0, 1, 0)
        && !D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pUnit, pNewItem->dwUnitId, 0, 0, 1, 1, 0))
    {
        sub_6FC44030(pGame, pUnit, pNewItem->dwUnitId);
    }

    return 1;
}

//D2Game.0x6FC4A9B0
int32_t __fastcall sub_6FC4A9B0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, uint8_t nBodyLoc)
{
    int32_t a3 = 0;
    if (!sub_6FC42F20(pUnit, pItem, &a3, nBodyLoc))
    {
        return 0;
    }

    if (!INVENTORY_PlaceItemInBodyLoc(pUnit->pInventory, pItem, nBodyLoc))
    {
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
    }

    if (nBodyLoc == BODYLOC_SWRARM || nBodyLoc == BODYLOC_SWLARM)
    {
        FOG_DisplayAssert("eWeaponPlace != INVBODYLOC_RARM2 && eWeaponPlace != INVBODYLOC_LARM2", __FILE__, __LINE__);
        exit(-1);
    }

    if (!INVENTORY_PlaceItemInSocket(pUnit->pInventory, pItem, 3))
    {
        FOG_DisplayAssert("0", __FILE__, __LINE__);
        exit(-1);
    }

    ITEMS_SetBodyLocation(pItem, nBodyLoc);
    INVENTORY_UpdateWeaponGUIDOnInsert(pUnit->pInventory, pItem);
    INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);
    D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pUnit, 0);

    if (pItem)
    {
        pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
    }

    UNITS_ChangeAnimMode(pItem, IMODE_EQUIP);
    ITEMS_SetItemCMDFlag(pItem, 512, 1);

    if (pItem)
    {
        pItem->dwFlags &= ~UNITFLAG_ISRESURRECT;
    }

    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
    UNITS_RefreshInventory(pUnit, 1);
    ITEMS_SetInvPage(pItem, 0xFFu);
    sub_6FC45300(pUnit);

    return 1;
}

//D2Game.0x6FC4AB10
int32_t __fastcall sub_6FC4AB10(D2UnitStrc* pPlayer, D2UnitStrc* pItem, D2UnitStrc** ppItem1, D2UnitStrc** ppItem2, int32_t* pBodyLoc)
{
    if (!pPlayer || !pItem || pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    if (*ppItem1)
    {
        if (*ppItem2)
        {
            return 0;
        }
    }
    else
    {
        if (!*ppItem2)
        {
            return 1;
        }
    }

    int32_t nBodyLoc = 0;
    switch (*pBodyLoc)
    {
    case BODYLOC_RARM:
        nBodyLoc = BODYLOC_LARM;
        break;
    case BODYLOC_LARM:
        nBodyLoc = BODYLOC_RARM;
        break;
    case BODYLOC_RRIN:
        nBodyLoc = BODYLOC_LRIN;
        break;
    case BODYLOC_LRIN:
        nBodyLoc = BODYLOC_RRIN;
        break;
    case BODYLOC_SWRARM:
        nBodyLoc = BODYLOC_SWLARM;
        break;
    case BODYLOC_SWLARM:
        nBodyLoc = BODYLOC_SWRARM;
        break;
    default:
        nBodyLoc = 0;
        break;
    }

    D2UnitStrc* pItem1 = *ppItem1;
    D2UnitStrc* pItem2 = *ppItem2;

    if (pItem1)
    {
        *ppItem1 = pItem2;
        *ppItem2 = pItem1;
        *pBodyLoc = nBodyLoc;
    }

    switch (*pBodyLoc)
    {
    case BODYLOC_RARM:
    case BODYLOC_LARM:
    case BODYLOC_SWRARM:
    case BODYLOC_SWLARM:
    {
        int32_t nAmmoType = ITEMS_GetAmmoType(pItem);
        if (nAmmoType > 0)
        {
            if (*ppItem2)
            {
                return ITEMS_CheckItemTypeId(*ppItem2, nAmmoType) != 0;
            }

            return 1;
        }

        if (ITEMS_GetQuiverType(pItem))
        {
            if (!*ppItem2)
            {
                return 1;
            }

            nAmmoType = ITEMS_GetAmmoType(*ppItem2);
            if (nAmmoType > 0)
            {
                return ITEMS_CheckItemTypeId(pItem, nAmmoType) != 0;
            }

            return 0;
        }

        nAmmoType = ITEMS_GetAmmoType(*ppItem2);
        if (nAmmoType > 0)
        {
            return ITEMS_CheckItemTypeId(pItem, nAmmoType) != 0;
        }

        const int32_t nQuiverType = ITEMS_GetQuiverType(*ppItem2);
        if (nQuiverType > 0)
        {
            return ITEMS_CheckItemTypeId(pItem, nQuiverType) != 0;
        }

        if (ITEMS_CheckWeaponIfTwoHanded(pItem) && !ITEMS_Is1Or2Handed(pPlayer, pItem) || ITEMS_CheckWeaponIfTwoHanded(*ppItem2) && !ITEMS_Is1Or2Handed(pPlayer, *ppItem2))
        {
            return 0;
        }

        const int32_t bIsWeapon = ITEMS_CheckItemTypeId(*ppItem2, ITEMTYPE_WEAPON);
        if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_WEAPON))
        {
            if (!bIsWeapon)
            {
                return 0;
            }

            return *ppItem1 == 0;
        }

        if (!bIsWeapon)
        {
            return *ppItem1 == 0;
        }

        if (pPlayer->dwUnitType == UNIT_MONSTER)
        {
            return 0;
        }

        if (pPlayer->dwClassId != PCLASS_BARBARIAN && (pPlayer->dwClassId != PCLASS_ASSASSIN || !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_HAND_TO_HAND) || !ITEMS_CheckItemTypeId(*ppItem2, ITEMTYPE_HAND_TO_HAND)))
        {
            return 0;
        }

        return 1;
    }
    default:
        return *ppItem1 == 0;
    }
}

//D2Game.0x6FC4AD80
int32_t __fastcall sub_6FC4AD80(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pCorpse)
{
    if (!pPlayer || !pCorpse)
    {
        return 0;
    }
    
    D2InventoryStrc* pPlayerInventory = pPlayer->pInventory;
    D2InventoryStrc* pCorpseInventory = pCorpse->pInventory;
    if (!pPlayerInventory || !pCorpseInventory)
    {
        return 0;
    }

    int32_t bItemRemoved = 0;
    int32_t nCounter = 0;
    do
    {
        bItemRemoved = 0;
        nCounter = 0;
        for (int32_t i = 1; i < 13; ++i)
        {
            int32_t nBodyLoc = i;
            D2UnitStrc* pCorpseItem = INVENTORY_GetItemFromBodyLoc(pCorpseInventory, i);
            if (pCorpseItem)
            {
                ++nCounter;

                if (ITEMS_CheckRequirements(pCorpseItem, pPlayer, 0, nullptr, nullptr, nullptr))
                {
                    D2UnitStrc* pPlayerItem = INVENTORY_GetItemFromBodyLoc(pPlayerInventory, i);
                    D2UnitStrc* pOtherPlayerItem = pPlayerItem;
                    switch (i)
                    {
                    case BODYLOC_RARM:
                        pOtherPlayerItem = INVENTORY_GetItemFromBodyLoc(pPlayerInventory, BODYLOC_LARM);
                        break;
                    case BODYLOC_LARM:
                        pOtherPlayerItem = INVENTORY_GetItemFromBodyLoc(pPlayerInventory, BODYLOC_RARM);
                        break;
                    case BODYLOC_RRIN:
                        pOtherPlayerItem = INVENTORY_GetItemFromBodyLoc(pPlayerInventory, BODYLOC_LRIN);
                        break;
                    case BODYLOC_LRIN:
                        pOtherPlayerItem = INVENTORY_GetItemFromBodyLoc(pPlayerInventory, BODYLOC_RRIN);
                        break;
                    case BODYLOC_SWRARM:
                        pOtherPlayerItem = INVENTORY_GetItemFromBodyLoc(pPlayerInventory, BODYLOC_SWLARM);
                        break;
                    case BODYLOC_SWLARM:
                        pOtherPlayerItem = INVENTORY_GetItemFromBodyLoc(pPlayerInventory, BODYLOC_SWRARM);
                        break;
                    default:
                        break;
                    }

                    if (sub_6FC4AB10(pPlayer, pCorpseItem, &pPlayerItem, &pOtherPlayerItem, &nBodyLoc))
                    {
                        D2_ASSERT(INVENTORY_GetItemFromBodyLoc(pPlayerInventory, nBodyLoc));

                        if (INVENTORY_PlaceItemInBodyLoc(pPlayerInventory, pCorpseItem, nBodyLoc))
                        {
                            int32_t nSocketType = 3;
                            if (nBodyLoc == BODYLOC_SWRARM || nBodyLoc == BODYLOC_SWLARM)
                            {
                                nSocketType = 4;
                            }

                            if (INVENTORY_PlaceItemInSocket(pPlayerInventory, pCorpseItem, nSocketType))
                            {
                                INVENTORY_SetCursorItem(pPlayerInventory, nullptr);
                                ITEMS_SetBodyLocation(pCorpseItem, nBodyLoc);
                                pCorpseItem->dwFlags &= ~UNITFLAG_TARGETABLE;
                                UNITS_ChangeAnimMode(pCorpseItem, IMODE_EQUIP);
                                ITEMS_SetItemCMDFlag(pCorpseItem, 8, 1);
                                INVENTORY_AddItemToTradeInventory(pPlayerInventory, pCorpseItem);
                                UNITS_RefreshInventory(pPlayer, 1);
                                pCorpseItem->dwFlags &= ~UNITFLAG_ISRESURRECT;
                                ITEMS_SetInvPage(pCorpseItem, INVPAGE_NULL);

                                if (nSocketType == 3)
                                {
                                    INVENTORY_UpdateWeaponGUIDOnInsert(pPlayerInventory, pCorpseItem);
                                    D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pCorpseItem, pPlayer, 0);

                                    if (pPlayer->dwUnitType == UNIT_PLAYER)
                                    {
                                        sub_6FC42120(pPlayer, pCorpseItem, 1);
                                    }

                                    sub_6FC45300(pPlayer);
                                }

                                QUESTS_ItemPickedUp(pGame, pPlayer, pCorpseItem);
                                sub_6FC512C0(pGame, pCorpseItem);
                                sub_6FC51310(pGame, pCorpseItem);
                                ITEMS_SetItemFlag(pCorpseItem, IFLAG_NEWITEM, 1);
                                INVENTORY_RemoveItemFromInventory(pCorpseInventory, pCorpseItem);
                                bItemRemoved = 1;
                                --nCounter;
                            }
                        }
                    }
                    else
                    {
                        if (sub_6FC43E60(pGame, pPlayer, pCorpseItem, 0, 0, __FILE__, __LINE__))
                        {
                            sub_6FC512C0(pGame, pCorpseItem);
                            sub_6FC51310(pGame, pCorpseItem);
                            ITEMS_SetItemFlag(pCorpseItem, IFLAG_NEWITEM, 1);
                            INVENTORY_RemoveItemFromInventory(pCorpseInventory, pCorpseItem);
                            bItemRemoved = 1;
                            --nCounter;
                        }
                    }
                }
            }
        }
    }
    while (nCounter && bItemRemoved);

    // TODO: v22
    int32_t v22 = 0;
    while (1)
    {
        nCounter = 0;
        int32_t nResult = 1;
        D2UnitStrc* pCorpseItem = INVENTORY_GetFirstItem(pCorpseInventory);
        if (!pCorpseItem)
        {
            D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pPlayer, 0);
            return 1;
        }

        do
        {
            D2UnitStrc* pNextItem = INVENTORY_GetNextItem(pCorpseItem);
            D2UnitStrc* pItem = INVENTORY_UnitIsItem(pCorpseItem);
            if (!sub_6FC425F0(pGame, pPlayer, pItem))
            {
                nResult = 0;
            }
            else if (sub_6FC4A9B0(pGame, pPlayer, pItem, 0))
            {
                ++nCounter;
                ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
            }
            else if (ITEMS_CheckIfBeltable(pItem) && INVENTORY_PlaceItemInFreeBeltSlot(pPlayerInventory, pItem))
            {
                if (INVENTORY_PlaceItemInSocket(pPlayerInventory, pItem, 2))
                {
                    INVENTORY_SetCursorItem(pPlayerInventory, nullptr);

                    D2_ASSERT(pItem);
                    if (pPlayer->dwUnitType == UNIT_PLAYER)
                    {
                        sub_6FC42120(pPlayer, pItem, 1);
                    }

                    pItem->dwFlags &= ~UNITFLAG_TARGETABLE;
                    if (ITEMS_IsCharmUsable(pItem, pPlayer))
                    {
                        D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pPlayer, 0);
                    }

                    UNITS_ChangeAnimMode(pItem, IMODE_INBELT);
                    pItem->dwFlags &= ~UNITFLAG_ISRESURRECT;
                    ITEMS_SetItemCMDFlag(pItem, 0x2000u, 1);
                    ITEMS_SetInvPage(pItem, INVPAGE_NULL);
                    INVENTORY_AddItemToTradeInventory(pPlayerInventory, pItem);
                    UNITS_RefreshInventory(pPlayer, 1);
                    ++nCounter;
                    ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
                }
            }
            else if (v22)
            {
                if (sub_6FC43E60(pGame, pPlayer, pItem, 0, 0, __FILE__, __LINE__))
                {
                    ++nCounter;
                    ITEMS_SetItemFlag(pItem, IFLAG_NEWITEM, 1);
                }
                else
                {
                    nResult = 0;
                }
            }

            pCorpseItem = pNextItem;
        }
        while (pCorpseItem);

        if (!nCounter)
        {
            if (v22)
            {
                D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pPlayer, 0);
                return nResult;
            }

            v22 = 1;
        }
    }
}

//D2Game.0x6FC4B240
void __fastcall D2GAME_UpdatePlayerVitals_6FC4B240(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit || !pUnit->pInventory)
    {
        return;
    }

    D2UnkItemModeStrc unused = {};
    sub_6FC44F00(pUnit, &unused);

    D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory);
    while (pItem)
    {
        D2UnitStrc* pNextItem = INVENTORY_GetNextItem(pItem);
        if (INVENTORY_UnitIsItem(pItem) && pItem->dwUnitType == UNIT_ITEM)
        {
            int32_t bType = 0;
            if (STATLIST_GetOwner(pItem, &bType))
            {
                STATLIST_ExpireUnitStatlist(pUnit, pItem);
                STATLIST_MergeStatLists(pUnit, pItem, bType);
            }
        }
        pItem = pNextItem;
    }

    sub_6FC44FB0(pUnit, &unused);
}

//D2Game.0x6FC4B2D0
int32_t __fastcall D2GAME_CheckHasFilledSockets_6FC4B2D0(D2UnitStrc* pItem, const char* szFile, int32_t nLine)
{
    if (pItem && pItem->dwUnitType == UNIT_ITEM && ITEMS_CheckIfSocketable(pItem) && pItem->pInventory && INVENTORY_GetItemCount(pItem->pInventory))
    {
        return 1;
    }

    return 0;
}

//D2Game.0x6FC4B310
int32_t __fastcall sub_6FC4B310(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, D2UnitStrc* pSocketable, int32_t a5)
{
    if (!ITEMS_CheckIfSocketable(pItem))
    {
        return 1;
    }

    if (!pItem->pInventory)
    {
        return 0;
    }

    for (D2UnitStrc* i = INVENTORY_GetFirstItem(pItem->pInventory); i; i = INVENTORY_GetNextItem(i))
    {
        D2UnitStrc* pCheckedItem = INVENTORY_UnitIsItem(i);
        D2UnitStrc* pDupedItem = ITEMS_Duplicate(pGame, pCheckedItem, nullptr, 1);
        UNITS_ChangeAnimMode(pDupedItem, IMODE_ONCURSOR);
        UNITS_SetXForStaticUnit(pDupedItem, CLIENTS_GetUnitX(pCheckedItem));
        UNITS_SetYForStaticUnit(pDupedItem, CLIENTS_GetUnitY(pCheckedItem));

        int32_t bPlaced = 0;
        if (!D2GAME_ITEMSOCKET_PlaceItem_6FC497E0(pGame, pUnit, pDupedItem ? pDupedItem->dwUnitId : -1, pSocketable ? pSocketable->dwUnitId : -1, &bPlaced, a5, 0, 1u))
        {
            return 0;
        }
    }

    return 1;
}

//D2Game.0x6FC4B430
D2UnitStrc* __fastcall sub_6FC4B430(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t* a4)
{
    D2_MAYBE_UNUSED(pGame);

    if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL) || !pUnit->pInventory)
    {
        return nullptr;
    }

    for (D2UnitStrc* j = INVENTORY_GetFirstItem(pUnit->pInventory); j; j = INVENTORY_GetNextItem(j))
    {
        if (INVENTORY_UnitIsItem(j) && ITEMS_GetItemType(j) == ITEMTYPE_BOOK && ITEMS_GetInvPage(j) == 0)
        {
            int32_t bSkip = 0;
            for (int32_t i = 0; i < 3; ++i)
            {
                if (ITEMS_GetPrefixId(j, i) != ITEMS_GetPrefixId(pItem, i) || ITEMS_GetSuffixId(j, i) != ITEMS_GetSuffixId(pItem, i))
                {
                    bSkip = 1;
                }
            }

            if (!bSkip && ITEMS_GetAutoAffix(j) == ITEMS_GetAutoAffix(pItem))
            {
                int32_t nQuantity = STATLIST_GetUnitStatUnsigned(j, STAT_QUANTITY, 0);
                if (nQuantity < 0)
                {
                    nQuantity = 0;
                }

                const int32_t nMaxStack = ITEMS_GetTotalMaxStack(j);
                if (nQuantity < nMaxStack)
                {
                    *a4 = nMaxStack - nQuantity;
                    return j;
                }
            }
        }
    }

    return nullptr;
}

//D2Game.0x6FC4B520
void __fastcall sub_6FC4B520(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem, int32_t nValue)
{
    STATLIST_AddUnitStat(pItem, STAT_QUANTITY, nValue, 0);
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(pClient, pItem, 1, STAT_QUANTITY, STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0), 0);
    sub_6FC43AF0(pUnit, pItem, nValue);
}

//D2Game.0x6FC4B580
void __fastcall sub_6FC4B580(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    ITEMS_SetItemFlag(pItem, IFLAG_BROKEN, 1);
    INVENTORY_UpdateWeaponGUIDOnRemoval(pUnit->pInventory, pItem);
    if (UNITS_GetAnimOrSeqMode(pItem) == 1)
    {
        D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pItem, pUnit, 1, 1);
    }

    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);
    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
    UNITS_RefreshInventory(pUnit, 1);
    STATLIST_SetUnitStat(pItem, STAT_DURABILITY, 0, 0);

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(pClient, pItem, 1, STAT_DURABILITY, 0, 0);
    ITEMS_UpdateSets(pUnit, pItem, 1, 1);
    SUNIT_AttachSound(pUnit, 9u, pUnit);
}

//D2Game.0x6FC4B630
void __fastcall D2GAME_RepairBrokenItem_6FC4B630(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
    D2InventoryStrc* pInventory = nullptr;
    if (pPlayer)
    {
        pInventory = pPlayer->pInventory;
    }

    ITEMS_SetItemFlag(pItem, IFLAG_REPAIRED, 1);
    ITEMS_SetItemFlag(pItem, IFLAG_BROKEN, 0);

    if (pInventory)
    {
        const uint8_t nBodyLoc = ITEMS_GetBodyLocation(pItem);
        if (nBodyLoc != BODYLOC_SWRARM && nBodyLoc != BODYLOC_SWLARM)
        {
            INVENTORY_UpdateWeaponGUIDOnInsert(pPlayer->pInventory, pItem);
            if (UNITS_GetAnimOrSeqMode(pItem) == 1 || !UNITS_GetAnimOrSeqMode(pItem) && ITEMS_IsCharmUsable(pItem, pPlayer))
            {
                D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pPlayer, 1);
            }
        }

        D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pPlayer, 0);
        INVENTORY_AddItemToTradeInventory(pInventory, pItem);
        UNITS_RefreshInventory(pPlayer, 1);
    }

    const int32_t nMaxDurability = STATLIST_GetMaxDurabilityFromUnit(pItem);
    if (nMaxDurability > 0)
    {
        STATLIST_SetUnitStat(pItem, STAT_DURABILITY, nMaxDurability, 0);

        if (pPlayer)
        {
            D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__), pItem, 1, STAT_DURABILITY, nMaxDurability, 0);
        }
    }

    ITEMS_UpdateSets(pPlayer, pItem, 0, 0);

    if (pPlayer)
    {
        sub_6FC45300(pPlayer);
    }
}

//D2Game.0x6FC4B740
void __fastcall sub_6FC4B740(D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    D2_ASSERT(pItem);

    if (!ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL) && !ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
    {
        return;
    }

    D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
    if (!pBooksTxtRecord)
    {
        return;
    }

    int32_t nSkillId = -1;
    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
    {
        nSkillId = pBooksTxtRecord->dwScrollSkillId;
    }
    else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
    {
        nSkillId = pBooksTxtRecord->dwBookSkillId;
    }

    if (nSkillId == -1)
    {
        return;
    }

    STATLIST_GetUnitStatUnsigned(pItem, STAT_QUANTITY, 0);

    D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, -1);
    SKILLS_SetQuantity(pSkill, 0);

    int32_t nUnitGUID = -1;
    int32_t nUnitType = 6;
    if (pUnit)
    {
        nUnitGUID = pUnit->dwUnitId;
        nUnitType = pUnit->dwUnitType;
    }

    D2GAME_PACKETS_SendPacket0x22_6FC3DBE0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), nUnitType, nUnitGUID, nSkillId, 0);
}

//D2Game.0x6FC4B830
void __fastcall sub_6FC4B830(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    int32_t nInventoryItemCounter = 0;

    D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory);
    while (pItem)
    {
        D2UnitStrc* pNextItem = INVENTORY_GetNextItem(pItem);
        D2_ASSERT(INVENTORY_UnitIsItem(pItem));

        if (pItem->dwAnimMode == IMODE_STORED)
        {
            if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
            {
                D2BooksTxt* pBooksTxtRecord = DATATBLS_GetBooksTxtRecord(ITEMS_GetSuffixId(pItem, 0));
                if (pBooksTxtRecord)
                {
                    int32_t nSkillId = -1;
                    if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SCROLL))
                    {
                        nSkillId = pBooksTxtRecord->dwScrollSkillId;
                    }
                    else if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOOK))
                    {
                        nSkillId = pBooksTxtRecord->dwBookSkillId;
                    }

                    if (nSkillId != -1)
                    {
                        SKILLS_SetQuantity(SKILLS_GetSkillById(pUnit, nSkillId, -1u), 0);
                        D2GAME_PACKETS_SendPacket0x22_6FC3DBE0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit->dwUnitType, pUnit->dwUnitId, nSkillId, 0);
                    }
                }
            }

            if (ITEMS_GetInvPage(pItem) == INVPAGE_INVENTORY)
            {
                ++nInventoryItemCounter;
            }
        }

        pItem = pNextItem;
    }

    if (nInventoryItemCounter)
    {
        pItem = INVENTORY_GetFirstItem(pUnit->pInventory);
        while (pItem)
        {
            D2UnitStrc* pNextItem = INVENTORY_GetNextItem(pItem);
            if (!INVENTORY_UnitIsItem(pItem) || pItem->dwAnimMode == IMODE_STORED)
            {
                if (ITEMS_GetInvPage(pItem) == INVPAGE_INVENTORY)
                {
                    sub_6FC42120(pUnit, pItem, 1);

                    --nInventoryItemCounter;
                    if (!nInventoryItemCounter)
                    {
                        break;
                    }
                }
            }

            pItem = pNextItem;
        }
    }
}

//D2Game.0x6FC4B9D0
int32_t __fastcall sub_6FC4B9D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    D2_ASSERT(pUnit);
    D2_ASSERT(pItem);

    UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);
    ITEMS_SetItemCMDFlag(pItem, 0x100000u, 1);
    INVENTORY_AddItemToTradeInventory(pUnit->pInventory, pItem);
    UNITS_RefreshInventory(pUnit, 1);
    return 1;
}

//D2Game.0x6FC4BA50
void __fastcall sub_6FC4BA50(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit || !pUnit->pInventory)
    {
        return;
    }

    D2UnitStrc* pItem = INVENTORY_GetFirstItem(pUnit->pInventory);
    while (pItem)
    {
        D2UnitStrc* pNext = INVENTORY_GetNextItem(pItem);

        D2_ASSERT(INVENTORY_UnitIsItem(pItem));

        if (INVENTORY_GetItemNodePage(pItem) == 3 || INVENTORY_GetItemNodePage(pItem) == 1 && ITEMS_IsCharmUsable(pItem, pUnit))
        {
            INVENTORY_UpdateWeaponGUIDOnRemoval(pUnit->pInventory, pItem);
            D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pItem);
            D2_ASSERT(pRemove);
            D2_ASSERT(pItem == pRemove);

            D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pItem, pUnit, 0, 1);
            D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);
        }

        pItem = pNext;
    }

    INVENTORY_RemoveAllItems(pUnit->pInventory);
}

//D2Game.0x6FC4BB90
void __fastcall D2GAME_UpdatePlayerItems_6FC4BB90(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t a3)
{
    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pUnit, 0);

    if (a3)
    {
        UNITS_RefreshInventory(pUnit, 1);
    }
}

//D2Game.0x6FC4BBB0
void __fastcall sub_6FC4BBB0(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pPlayer)
{
    ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 1);

    if (pItem && pItem->dwAnimMode == IMODE_STORED)
    {
        return;
    }

    if (!ITEMS_IsCharmUsable(pItem, pPlayer))
    {
        return;
    }

    D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pPlayer, 0);
    D2GAME_ITEMS_UpdateInventoryItems_6FC44A90(pGame, pPlayer, 0);
    UNITS_RefreshInventory(pPlayer, 1);
}

//D2Game.0x6FC4BC00
void __fastcall D2GAME_ITEMS_UpdateStatList_6FC4BC00(D2GameStrc* pGame, D2UnitStrc* pItem, D2UnitStrc* pUnit)
{
	D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pItem, pUnit, 0);
}

//D2Game.0x6FC4BC10
void __fastcall D2GAME_ITEMMODE_Unk_6FC4BC10(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    if (!pGame || !pUnit || !pItem || pItem->dwUnitType != UNIT_ITEM || !pUnit->pInventory)
    {
        return;
    }

    switch (pItem->dwAnimMode)
    {
    case IMODE_STORED:
    {
        ITEMS_SetItemCell(pItem, ITEMS_GetInvPage(pItem));

        if (pUnit->dwUnitType == UNIT_PLAYER)
        {
            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
            if (pClient)
            {
                D2GAME_UpdateClientItem_6FC3E9D0(pClient, pUnit, pItem, 0x20);
            }
        }

        D2GAME_RemoveItem_6FC471F0(pGame, pUnit, pItem, 0);
        return;
    }
    case IMODE_EQUIP:
    {
        int32_t nUnused = 0;
        sub_6FC45B30(pGame, pUnit, INVENTORY_GetItemsXPosition(pUnit->pInventory, pItem), 1, &nUnused);
        return;
    }
    case IMODE_ONCURSOR:
    {
        if (INVENTORY_GetCursorItem(pUnit->pInventory) != pItem)
        {
            return;
        }

        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        D2GAME_PACKETS_SendPacket0x42_6FC3EE40(pClient, pUnit);
        INVENTORY_SetCursorItem(pUnit->pInventory, nullptr);
        ITEMS_RemoveFromAllPlayers(pGame, pItem);
        return;
    }
    }
}

//D2Game.0x6FC4BD50
int32_t __fastcall D2GAME_RechargeItem_6FC4BD50(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitStrc* pItem)
{
    if (!pGame || !pItem || pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    D2ClientStrc* pClient = nullptr;
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    }

    int32_t bRecharged = 0;
    D2StatStrc stats[64] = {};
    const int32_t nStats = STATLIST_CopyStats(pItem, STAT_ITEM_CHARGED_SKILL, stats, std::size(stats));
    for (int32_t i = 0; i < nStats; ++i)
    {
        const int32_t nSkillLevel = stats[i].nLayer & sgptDataTables->nShiftedStuff;
        const int32_t nValue = stats[i].nValue >> 8;
        const int32_t nSkillId = stats[i].nLayer >> sgptDataTables->nStuff;
        if ((stats[i].nValue & 0xFF) < nValue)
        {
            ITEMMODS_UpdateItemWithSkillCharges(pItem, nSkillId, nSkillLevel, nValue);

            bRecharged = 1;
            if (pClient)
            {
                D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(pClient, pItem, 1, STAT_ITEM_CHARGED_SKILL, (nValue << 8) + (uint8_t)nValue, (nSkillLevel & (uint16_t)sgptDataTables->nShiftedStuff) + nSkillId << sgptDataTables->nStuff);
            }
        }
    }

    return bRecharged;
}

//D2Game.0x6FC4BE80
void __fastcall sub_6FC4BE80(D2UnitStrc* pUnit, D2GameStrc* pGame, int32_t nUnitGUID, D2RoomStrc* pRoom)
{
    if (pUnit)
    {
        pUnit->dwUnitId = nUnitGUID;
        pUnit->dwFlags = pUnit->dwFlags & ~UNITFLAG_ISVALIDTARGET | UNITFLAG_TARGETABLE;
    }

    ITEMS_SetItemFlag(pUnit, IFLAG_IDENTIFIED, 1);
    STATLIST_AllocStatListEx(pUnit, 0, nullptr, nullptr);

    int32_t nClassId = -1;
    if (pUnit)
    {
        nClassId = pUnit->dwClassId;
    }

    const int32_t nQuest = DATATBLS_GetItemsTxtRecord(nClassId)->nQuest;
    if (nQuest)
    {
        QUESTS_CreateChainRecord(pGame, pUnit, nQuest - 1);
    }

    D2GAME_DeleteTimersOnUnit_6FC34A30(pGame, pUnit);
}

//D2Game.0x6FC4BF00
D2RoomStrc* __fastcall D2GAME_GetFreeSpaceEx_6FC4BF00(D2RoomStrc* pRoom, D2CoordStrc* pCoords, D2CoordStrc* pReturnCoords, int32_t nUnitSize)
{
    const int32_t nX = pCoords->nX + 2;
    const int32_t nY = pCoords->nY + 3;

    D2RoomStrc* pTargetRoom = D2GAME_GetRoom_6FC52070(pRoom, nX, nY);
    if (pTargetRoom)
    {
        pReturnCoords->nX = nX;
        pReturnCoords->nY = nY;
        return COLLISION_GetFreeCoordinatesWithField(pRoom, pReturnCoords, pCoords, nUnitSize, 0x3E01u, 0x801u, 1);
    }

    pReturnCoords->nX = pCoords->nX;
    pReturnCoords->nY = pCoords->nY;
    return COLLISION_GetFreeCoordinatesWithField(pRoom, pReturnCoords, pCoords, nUnitSize, 0x3E01u, 0x801u, 1);
}

//D2Game.0x6FC4BFF0
int32_t __fastcall sub_6FC4BFF0(D2GameStrc* pGame, D2SeedStrc* pSeed, int32_t nLevelId, int32_t nItemType, int32_t a5)
{
    D2ItemDataTbl* pItemDataTbl = DATATBLS_GetItemDataTables();
    const int32_t nBase = pItemDataTbl->pArmor - pItemDataTbl->pItemsTxt;
    const int32_t nCount = pItemDataTbl->pMisc - pItemDataTbl->pArmor;
    
    D2ItemsTxt* pItemsTxtRecord = pItemDataTbl->pArmor;
    if (!pItemsTxtRecord)
    {
        return -1;
    }

    if (nLevelId <= 0)
    {
        nLevelId = 1;
    }

    D2ItemUnkStrc data[1024] = {};
    int32_t nMax = 0;
    for (int32_t i = 0; i < nCount; ++i)
    {
        if (pItemsTxtRecord[i].nSpawnable && !pItemsTxtRecord[i].nQuest && pItemsTxtRecord[i].nLevel <= nLevelId)
        {
            if (a5 || !ITEMS_RollLimitedRandomNumber(pSeed, pItemsTxtRecord[i].nRarity - DRLG_GetActNoFromLevelId(nLevelId)))
            {
                if ((nItemType == -1 || pItemsTxtRecord[i].wType[0] == nItemType) && (pGame->bExpansion || pItemsTxtRecord[i].wVersion < 100u) && nMax < 1023)
                {
                    data[nMax].pItemsTxtRecord = &pItemsTxtRecord[i];
                    data[nMax].nIndex = nBase + i;
                    ++nMax;
                }
            }
        }
    }

    const int32_t nIndex = ITEMS_RollLimitedRandomNumber(pSeed, nMax);
    if (nIndex > nMax)
    {
        return -1;
    }

    D2ItemUnkStrc* pData = &data[nIndex];
    if (!pData)
    {
        return -1;
    }

    return pData->nIndex;
}

//D2Game.0x6FC4C1E0
int32_t __fastcall sub_6FC4C1E0(D2GameStrc* pGame, D2SeedStrc* pSeed, int32_t nLevelId, int32_t nItemType, int32_t a5)
{
    D2ItemDataTbl* pItemDataTbl = DATATBLS_GetItemDataTables();
    const int32_t nBase = pItemDataTbl->pWeapons - pItemDataTbl->pItemsTxt;
    const int32_t nCount = pItemDataTbl->pArmor - pItemDataTbl->pWeapons;

    D2ItemsTxt* pItemsTxtRecord = pItemDataTbl->pWeapons;
    if (!pItemsTxtRecord)
    {
        return -1;
    }

    if (nLevelId <= 0)
    {
        nLevelId = 1;
    }

    D2ItemUnkStrc data[1024] = {};
    int32_t nMax = 0;
    for (int32_t i = 0; i < nCount; ++i)
    {
        if (pItemsTxtRecord[i].nSpawnable && !pItemsTxtRecord[i].nQuest && pItemsTxtRecord[i].nLevel <= nLevelId)
        {
            if (a5 || !ITEMS_RollLimitedRandomNumber(pSeed, pItemsTxtRecord[i].nRarity - DRLG_GetActNoFromLevelId(nLevelId)))
            {
                if ((nItemType == -1 || pItemsTxtRecord[i].wType[0] == nItemType) && (pGame->bExpansion || pItemsTxtRecord[i].wVersion < 100u) && nMax < 1023)
                {
                    data[nMax].pItemsTxtRecord = &pItemsTxtRecord[i];
                    data[nMax].nIndex = nBase + i;
                    ++nMax;
                }
            }
        }
    }

    const int32_t nIndex = ITEMS_RollLimitedRandomNumber(pSeed, nMax);
    if (nIndex > nMax)
    {
        return -1;
    }

    D2ItemUnkStrc* pData = &data[nIndex];
    if (!pData)
    {
        return -1;
    }

    return pData->nIndex;
}

//D2Game.0x6FC4C3D0
int32_t __fastcall sub_6FC4C3D0(D2GameStrc* pGame, D2SeedStrc* pSeed, int32_t nLevelId, int32_t nItemType, int32_t a5, int32_t a6)
{
    D2ItemDataTbl* pItemDataTbl = DATATBLS_GetItemDataTables();
    const int32_t nBase = pItemDataTbl->pMisc - pItemDataTbl->pItemsTxt;
    const int32_t nCount = &pItemDataTbl->pItemsTxt[pItemDataTbl->nItemsTxtRecordCount] - pItemDataTbl->pMisc;

    D2ItemsTxt* pItemsTxtRecord = pItemDataTbl->pMisc;
    if (!pItemsTxtRecord)
    {
        return -1;
    }

    if (nLevelId <= 0)
    {
        nLevelId = 1;
    }

    D2ItemUnkStrc data[1024] = {};
    int32_t nMax = 0;
    for (int32_t i = 0; i < nCount; ++i)
    {
        if (a6 || pItemsTxtRecord[i].wType[0] != ITEMTYPE_BODY_PART)
        {
            if (pItemsTxtRecord[i].nSpawnable && !pItemsTxtRecord[i].nQuest && pItemsTxtRecord[i].nLevel <= nLevelId)
            {
                if (a5 || !ITEMS_RollLimitedRandomNumber(pSeed, pItemsTxtRecord[i].nRarity - DRLG_GetActNoFromLevelId(nLevelId)))
                {
                    if ((nItemType == -1 || pItemsTxtRecord[i].wType[0] == nItemType) && (pGame->bExpansion || pItemsTxtRecord[i].wVersion < 100u) && nMax < 1023)
                    {
                        data[nMax].pItemsTxtRecord = &pItemsTxtRecord[i];
                        data[nMax].nIndex = nBase + i;
                        ++nMax;
                    }
                }
            }
        }
    }

    const int32_t nIndex = ITEMS_RollLimitedRandomNumber(pSeed, nMax);
    if (nIndex > nMax)
    {
        return -1;
    }

    D2ItemUnkStrc* pData = &data[nIndex];
    if (!pData)
    {
        return -1;
    }

    return pData->nIndex;
}

//Inlined in D2Game.0x6FC4C5F0
int32_t __fastcall sub_6FC4C5F0_End(D2GameStrc* pGame, D2UnitStrc* pItem, D2ItemDropStrc* pItemDrop, D2ItemsTxt* pItemsTxtRecord)
{
    const int32_t nItemQuality = ITEMS_GetItemQuality(pItem);
    if (nItemQuality <= 0 || nItemQuality >= 10)
    {
        return 0;
    }

    if (ITEMS_GetItemFormat(pItem) >= 100)
    {
        ITEMS_MakeEthereal(pItem, pItemDrop);
    }

    if (nItemQuality > 0 && nItemQuality <= 3)
    {
        sub_6FC4D6B0(pGame, pItem, pItemDrop);
    }

    if (ITEMS_GetItemFormat(pItem) >= 100)
    {
        switch (nItemQuality)
        {
        case ITEMQUAL_INFERIOR:
        case ITEMQUAL_NORMAL:
        case ITEMQUAL_SUPERIOR:
        case ITEMQUAL_MAGIC:
        case ITEMQUAL_RARE:
        case ITEMQUAL_CRAFT:
        case ITEMQUAL_TEMPERED:
            if (pItemsTxtRecord->wAutoPrefix)
            {
                const int16_t nAutoAffix = ITEMS_RollMagicAffixes(pItem, 0, 1, 0, 1, 0, pItemsTxtRecord->wAutoPrefix);
                if (nAutoAffix)
                {
                    ITEMS_SetAutoAffix(pItem, nAutoAffix);
                    ITEMMODS_AssignProperty(0, nullptr, pItem, DATATBLS_GetMagicAffixTxtRecord(nAutoAffix), 0, 0);
                }
            }
            break;
        default:
            break;
        }
    }

    return 1;
}

//D2Game.0x6FC4C5F0
int32_t __fastcall sub_6FC4C5F0(D2GameStrc* pGame, D2UnitStrc** ppUnit, D2ItemDropStrc* pItemDrop)
{
    D2UnitStrc* pItem = *ppUnit;
    if (!pItem || pItem->dwUnitType != UNIT_ITEM)
    {
        return 0;
    }

    D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemsTxtRecord(pItem ? pItem->dwClassId : -1);
    if (!pItemsTxtRecord)
    {
        return 0;
    }

    ITEMS_SetAutoAffix(pItem, 0);

    for (int32_t i = 0; i < 3; ++i)
    {
        ITEMS_AssignPrefix(pItem, 0, i);
        ITEMS_AssignSuffix(pItem, 0, i);
    }

    ITEMS_AssignRarePrefix(pItem, 0);
    ITEMS_AssignRareSuffix(pItem, 0);

    int32_t nQuality = ITEMS_RollItemQuality(pItem, pItemDrop);
    if (pItemDrop->nQuality)
    {
        nQuality = pItemDrop->nQuality;
    }
    ITEMS_SetItemQuality(pItem, nQuality);

    if (ITEMS_CanBeMagic(pItem))
    {
        if (ITEMS_IsQuestItem(pItem))
        {
            ITEMS_SetItemQuality(pItem, ITEMQUAL_UNIQUE);
        }
        else if (!ITEMS_IsMagSetRarUniCrfOrTmp(pItem))
        {
            ITEMS_SetItemQuality(pItem, ITEMQUAL_MAGIC);
        }
    }

    if (!ITEMS_CanBeRare(pItem) && ITEMS_GetItemQuality(pItem) == ITEMQUAL_RARE)
    {
        ITEMS_SetItemQuality(pItem, ITEMQUAL_MAGIC);
    }

    if (ITEMS_GetUniqueColumnFromItemsTxt(pItem))
    {
        ITEMS_SetItemQuality(pItem, ITEMQUAL_UNIQUE);
    }

    if (ITEMS_CanBeNormal(pItem))
    {
        ITEMS_SetItemQuality(pItem, ITEMQUAL_NORMAL);
    }

    uint32_t nSeed = 0;
    switch (ITEMS_GetItemQuality(pItem))
    {
    case ITEMQUAL_INFERIOR:
    {
        nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
        if (!sub_6FC55270(pItem, pItemDrop))
        {
            for (int32_t i = 0; i < 3; ++i)
            {
                ITEMS_AssignPrefix(pItem, 0, i);
                ITEMS_AssignSuffix(pItem, 0, i);
            }

            ITEMS_AssignRarePrefix(pItem, 0);
            ITEMS_AssignRareSuffix(pItem, 0);

            ITEMS_SetFileIndex(pItem, -1);
            ITEMS_SetItemStartSeed(pItem, nSeed);
            SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
            ITEMS_RollItemQuality(pItem, pItemDrop);
            ITEMS_SetItemQuality(pItem, ITEMQUAL_NORMAL);
            pItemDrop->nQuality = ITEMQUAL_NORMAL;
            sub_6FC4DC20(pGame, pItem, pItemDrop);
        }
        break;
    }
    case ITEMQUAL_NORMAL:
    {
        if (ITEMS_GetItemFormat(pItem))
        {
            D2GAME_ITEMS_AssignSpecial_6FC4DB60(pGame, pItem, pItemDrop);
        }
        else
        {
            sub_6FC4D900(pGame, pItem, pItemDrop);
        }

        break;
    }
    case ITEMQUAL_SUPERIOR:
    {
        ITEMS_SetFileIndex(pItem, -1);
        nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));

        if (!sub_6FC54690(pItem, pItemDrop))
        {
            for (int32_t i = 0; i < 3; ++i)
            {
                ITEMS_AssignPrefix(pItem, 0, i);
                ITEMS_AssignSuffix(pItem, 0, i);
            }

            ITEMS_AssignRarePrefix(pItem, 0);
            ITEMS_AssignRareSuffix(pItem, 0);

            ITEMS_SetFileIndex(pItem, -1);
            ITEMS_SetItemStartSeed(pItem, nSeed);
            SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
            ITEMS_RollItemQuality(pItem, pItemDrop);
            ITEMS_SetItemQuality(pItem, ITEMQUAL_NORMAL);
            pItemDrop->nQuality = ITEMQUAL_NORMAL;
            sub_6FC4DC20(pGame, pItem, pItemDrop);
        }
        break;
    }
    case ITEMQUAL_RARE:
    {
        ITEMS_AssignRarePrefix(pItem, 0);
        ITEMS_AssignRareSuffix(pItem, 0);

        nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
        if (!D2GAME_RollRareItem_6FC53360(pItem, pItemDrop))
        {
            for (int32_t i = 0; i < 3; ++i)
            {
                ITEMS_AssignPrefix(pItem, 0, i);
                ITEMS_AssignSuffix(pItem, 0, i);
            }

            ITEMS_AssignRarePrefix(pItem, 0);
            ITEMS_AssignRareSuffix(pItem, 0);

            ITEMS_SetFileIndex(pItem, -1);
            ITEMS_SetItemStartSeed(pItem, nSeed);
            SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
            ITEMS_RollItemQuality(pItem, pItemDrop);
            ITEMS_SetItemQuality(pItem, ITEMQUAL_MAGIC);
            pItemDrop->nQuality = ITEMQUAL_MAGIC;

            nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
            if (!sub_6FC4D5E0(ppUnit, pItemDrop))
            {
                for (int32_t i = 0; i < 3; ++i)
                {
                    ITEMS_AssignPrefix(pItem, 0, i);
                    ITEMS_AssignSuffix(pItem, 0, i);
                }

                ITEMS_AssignRarePrefix(pItem, 0);
                ITEMS_AssignRareSuffix(pItem, 0);

                ITEMS_SetFileIndex(pItem, -1);
                ITEMS_SetItemStartSeed(pItem, nSeed);
                SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
                ITEMS_RollItemQuality(pItem, pItemDrop);
                ITEMS_SetItemQuality(pItem, ITEMQUAL_SUPERIOR);
                pItemDrop->nQuality = ITEMQUAL_SUPERIOR;

                nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
                if (!sub_6FC54690(pItem, pItemDrop))
                {
                    for (int32_t i = 0; i < 3; ++i)
                    {
                        ITEMS_AssignPrefix(pItem, 0, i);
                        ITEMS_AssignSuffix(pItem, 0, i);
                    }

                    ITEMS_AssignRarePrefix(pItem, 0);
                    ITEMS_AssignRareSuffix(pItem, 0);

                    ITEMS_SetFileIndex(pItem, -1);
                    ITEMS_SetItemStartSeed(pItem, nSeed);
                    SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
                    ITEMS_RollItemQuality(pItem, pItemDrop);
                    ITEMS_SetItemQuality(pItem, ITEMQUAL_NORMAL);
                    pItemDrop->nQuality = ITEMQUAL_NORMAL;
                    sub_6FC4DC20(pGame, pItem, pItemDrop);
                }
            }
        }
        break;
    }
    case ITEMQUAL_MAGIC:
    {
        nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
        if (!sub_6FC4D5E0(ppUnit, pItemDrop))
        {
            for (int32_t i = 0; i < 3; ++i)
            {
                ITEMS_AssignPrefix(pItem, 0, i);
                ITEMS_AssignSuffix(pItem, 0, i);
            }

            ITEMS_AssignRarePrefix(pItem, 0);
            ITEMS_AssignRareSuffix(pItem, 0);

            ITEMS_SetFileIndex(pItem, -1);
            ITEMS_SetItemStartSeed(pItem, nSeed);
            SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
            ITEMS_RollItemQuality(pItem, pItemDrop);
            ITEMS_SetItemQuality(pItem, ITEMQUAL_SUPERIOR);
            pItemDrop->nQuality = ITEMQUAL_SUPERIOR;
            nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
            if (!sub_6FC54690(pItem, pItemDrop))
            {
                for (int32_t i = 0; i < 3; ++i)
                {
                    ITEMS_AssignPrefix(pItem, 0, i);
                    ITEMS_AssignSuffix(pItem, 0, i);
                }

                ITEMS_AssignRarePrefix(pItem, 0);
                ITEMS_AssignRareSuffix(pItem, 0);

                ITEMS_SetFileIndex(pItem, -1);
                ITEMS_SetItemStartSeed(pItem, nSeed);
                SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
                ITEMS_RollItemQuality(pItem, pItemDrop);
                ITEMS_SetItemQuality(pItem, ITEMQUAL_NORMAL);
                pItemDrop->nQuality = ITEMQUAL_NORMAL;
                sub_6FC4DC20(pGame, pItem, pItemDrop);
            }
        }
        break;
    }
    case ITEMQUAL_UNIQUE:
    {
        ITEMS_SetFileIndex(pItem, -1);
        nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));

        const uint16_t nItemFormat = ITEMS_GetItemFormat(pItem);
        if (ITEMS_HasDurability(pItem) && !nItemFormat)
        {
            STATLIST_SetUnitStat(pItem, STAT_DURABILITY, std::min(5 * STATLIST_GetUnitStatUnsigned(pItem, STAT_DURABILITY, 0), 255u), 0);
            STATLIST_SetUnitStat(pItem, STAT_MAXDURABILITY, std::min(5 * STATLIST_GetUnitBaseStat(pItem, STAT_MAXDURABILITY, 0), 255), 0);
        }

        if (!pItemDrop->bForce)
        {
            if (!nItemFormat)
            {
                for (int32_t i = 0; i < sgptDataTables->nUniqueItemsTxtRecordCount; ++i)
                {
                    D2UniqueItemsTxt* pUniqueItemsTxtRecord = &sgptDataTables->pUniqueItemsTxt[i];
                    if (pUniqueItemsTxtRecord->wVersion < 100)
                    {
                        if (pUniqueItemsTxtRecord->dwUniqueItemFlags & gdwBitMasks[UNIQUEITEMSFLAGINDEX_ENABLED])
                        {
                            if (pGame->nGameType || pGame->dwGameType || !(pUniqueItemsTxtRecord->dwUniqueItemFlags & gdwBitMasks[UNIQUEITEMSFLAGINDEX_LADDER]))
                            {
                                if (pItemsTxtRecord->dwCode == pUniqueItemsTxtRecord->dwBaseItemCode)
                                {
                                    if (!ITEMS_HasUniqueBeenDroppedAlready(pGame, i) || pItemsTxtRecord->nQuest)
                                    {
                                        ITEMS_SetFileIndex(pItem, i);
                                        ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 0);
                                        ITEMMODS_AssignProperty(3, nullptr, pItem, nullptr, 0, 0);
                                        return sub_6FC4C5F0_End(pGame, pItem, pItemDrop, pItemsTxtRecord);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                int32_t nCount = 0;
                int32_t nUniqueItemId = -1;
                int32_t nMax = 0;
                for (int32_t i = 0; i < sgptDataTables->nUniqueItemsTxtRecordCount; ++i)
                {
                    D2UniqueItemsTxt* pUniqueItemsTxtRecord = &sgptDataTables->pUniqueItemsTxt[i];
                    if (pUniqueItemsTxtRecord->wVersion < 100 || nItemFormat >= 100)
                    {
                        if (pUniqueItemsTxtRecord->dwUniqueItemFlags & gdwBitMasks[UNIQUEITEMSFLAGINDEX_ENABLED] && pItemsTxtRecord->dwCode == pUniqueItemsTxtRecord->dwBaseItemCode && (pGame->nGameType || pGame->dwGameType || !(pUniqueItemsTxtRecord->dwUniqueItemFlags & gdwBitMasks[UNIQUEITEMSFLAGINDEX_LADDER])))
                        {
                            if (pUniqueItemsTxtRecord->wLvl <= ITEMS_GetItemLevel(pItem))
                            {
                                if (pItemDrop->nItemIndex && pItemDrop->nItemIndex - 1 == i)
                                {
                                    nUniqueItemId = i;
                                }

                                stru_6FD45C18[nCount].nIndex = i;
                                stru_6FD45C18[nCount].nAccumulatedRarity = nMax;

                                ++nCount;
                                nMax += std::max((int32_t)pUniqueItemsTxtRecord->wRarity, 1);
                            }
                        }
                    }
                }

                if (nCount > 0)
                {
                    if (nUniqueItemId < 0)
                    {
                        const uint32_t nChance = ITEMS_RollLimitedRandomNumber(ITEMS_GetItemSeed(pItem), nMax);
                        int32_t i = 1;
                        while (i < nCount && nChance >= stru_6FD45C18[i].nAccumulatedRarity)
                        {
                            ++i;
                        }
                        nUniqueItemId = stru_6FD45C18[i - 1].nIndex;
                    }

                    if (!pItemsTxtRecord->nQuest && ITEMS_HasUniqueBeenDroppedAlready(pGame, nUniqueItemId))
                    {
                        ITEMS_SetFileIndex(pItem, -1);
                    }
                    else
                    {
                        D2UniqueItemsTxt* pUniqueItemsTxtRecord = ITEMS_GetUniqueItemsTxtRecord(nUniqueItemId);
                        if (pUniqueItemsTxtRecord && pItemsTxtRecord->dwCode == pUniqueItemsTxtRecord->dwBaseItemCode)
                        {
                            ITEMS_SetFileIndex(pItem, nUniqueItemId);
                            if (ITEMS_CanUniqueItemBeDropped(pGame, pItem))
                            {
                                ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 0);
                                ITEMMODS_AssignProperty(3, nullptr, pItem, nullptr, 0, 0);
                                return sub_6FC4C5F0_End(pGame, pItem, pItemDrop, pItemsTxtRecord);
                            }
                            else
                            {
                                ITEMS_SetFileIndex(pItem, -1);
                            }
                        }
                    }
                }
                else
                {
                    if (ITEMS_GetUniqueColumnFromItemsTxt(pItem))
                    {
                        return sub_6FC4C5F0_End(pGame, pItem, pItemDrop, pItemsTxtRecord);
                    }

                    ITEMS_SetFileIndex(pItem, -1);
                }
            }
        }
        else
        {
            D2UniqueItemsTxt* pUniqueItemsTxtRecord = ITEMS_GetUniqueItemsTxtRecord(pItemDrop->nItemIndex);
            if (pUniqueItemsTxtRecord)
            {
                ITEMS_SetFileIndex(pItem, pItemDrop->nItemIndex);
                if (pUniqueItemsTxtRecord->dwBaseItemCode == pItemsTxtRecord->dwCode)
                {
                    ITEMS_SetItemFlag(pItem, IFLAG_IDENTIFIED, 0);
                    ITEMMODS_AssignProperty(3, nullptr, pItem, nullptr, 0, 0);
                }

                return sub_6FC4C5F0_End(pGame, pItem, pItemDrop, pItemsTxtRecord);
            }
        }

        if (ITEMS_HasDurability(pItem) && ITEMS_GetItemFormat(pItem) >= 1u)
        {
            STATLIST_SetUnitStat(pItem, STAT_DURABILITY, std::min(3 * STATLIST_GetUnitStatUnsigned(pItem, STAT_DURABILITY, 0), 255u), 0);
            STATLIST_SetUnitStat(pItem, STAT_MAXDURABILITY, std::min(3 * STATLIST_GetUnitBaseStat(pItem, STAT_MAXDURABILITY, 0), 255), 0);
        }

        sub_6FC4E1A0(ppUnit);
        ITEMS_SetItemStartSeed(pItem, nSeed);
        SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
        ITEMS_RollItemQuality(pItem, pItemDrop);
        ITEMS_SetItemQuality(pItem, ITEMQUAL_RARE);
        pItemDrop->nQuality = ITEMQUAL_RARE;

        nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
        if (!D2GAME_RollRareItem_6FC53360(pItem, pItemDrop))
        {
            sub_6FC4E1A0(ppUnit);
            ITEMS_SetItemStartSeed(pItem, nSeed);
            SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
            ITEMS_RollItemQuality(pItem, pItemDrop);
            ITEMS_SetItemQuality(pItem, ITEMQUAL_MAGIC);
            pItemDrop->nQuality = ITEMQUAL_MAGIC;
            nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
            if (!sub_6FC4D5E0(ppUnit, pItemDrop))
            {
                sub_6FC4E1A0(ppUnit);
                ITEMS_SetItemStartSeed(pItem, nSeed);
                SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
                ITEMS_RollItemQuality(pItem, pItemDrop);
                ITEMS_SetItemQuality(pItem, ITEMQUAL_SUPERIOR);
                pItemDrop->nQuality = ITEMQUAL_SUPERIOR;
                nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
                if (!sub_6FC54690(pItem, pItemDrop))
                {
                    sub_6FC4E1A0(ppUnit);
                    ITEMS_SetItemStartSeed(pItem, nSeed);
                    SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
                    ITEMS_RollItemQuality(pItem, pItemDrop);
                    ITEMS_SetItemQuality(pItem, ITEMQUAL_NORMAL);
                    pItemDrop->nQuality = ITEMQUAL_NORMAL;
                    sub_6FC4DC20(pGame, pItem, pItemDrop);
                }
            }
        }

        break;
    }
    case ITEMQUAL_CRAFT:
    {
        ITEMS_AssignRarePrefix(pItem, 0);
        ITEMS_AssignRareSuffix(pItem, 0);

        nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
        if (!sub_6FC53CD0(pItem, pItemDrop))
        {
            sub_6FC4E1A0(ppUnit);
            ITEMS_SetItemStartSeed(pItem, nSeed);
            SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
            ITEMS_RollItemQuality(pItem, pItemDrop);
            ITEMS_SetItemQuality(pItem, ITEMQUAL_NORMAL);
            pItemDrop->nQuality = ITEMQUAL_NORMAL;
            sub_6FC4DC20(pGame, pItem, pItemDrop);
        }
        break;
    }
    case ITEMQUAL_TEMPERED:
    {
        ITEMS_AssignRarePrefix(pItem, 0);
        ITEMS_AssignRareSuffix(pItem, 0);

        nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));

        const int16_t nPrefix = ITEMS_RollTemperedItemAffix(pItem, 1);
        const int16_t nSuffix = ITEMS_RollTemperedItemAffix(pItem, 0);
        if (nPrefix && nSuffix)
        {
            ITEMS_AssignRarePrefix(pItem, nPrefix);
            ITEMS_AssignRareSuffix(pItem, nSuffix);
        }
        else
        {
            sub_6FC4E1A0(ppUnit);
            ITEMS_SetItemStartSeed(pItem, nSeed);
            SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
            ITEMS_RollItemQuality(pItem, pItemDrop);
            ITEMS_SetItemQuality(pItem, ITEMQUAL_NORMAL);
            pItemDrop->nQuality = ITEMQUAL_NORMAL;
            sub_6FC4DC20(pGame, pItem, pItemDrop);
        }
        break;
    }
    case ITEMQUAL_SET:
    {
        ITEMS_SetFileIndex(pItem, -1);

        nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
        if (!sub_6FC542C0(pItem, pItemDrop))
        {
            if (ITEMS_HasDurability(pItem) && ITEMS_GetItemFormat(pItem) >= 1u)
            {
                STATLIST_SetUnitStat(pItem, STAT_DURABILITY, std::min(2 * STATLIST_GetUnitStatUnsigned(pItem, STAT_DURABILITY, 0), 255u), 0);
                STATLIST_SetUnitStat(pItem, STAT_MAXDURABILITY, std::min(2 * STATLIST_GetUnitBaseStat(pItem, STAT_MAXDURABILITY, 0), 255), 0);
            }

            sub_6FC4E1A0(ppUnit);
            ITEMS_SetItemStartSeed(pItem, nSeed);
            SEED_InitLowSeed(ITEMS_GetItemSeed(pItem), nSeed);
            ITEMS_RollItemQuality(pItem, pItemDrop);
            ITEMS_SetItemQuality(pItem, ITEMQUAL_MAGIC);
            pItemDrop->nQuality = ITEMQUAL_MAGIC;

            nSeed = SEED_GetLowSeed(ITEMS_GetItemSeed(pItem));
            if (!sub_6FC4D5E0(ppUnit, pItemDrop) && !sub_6FC4E430(pGame, pItemDrop, ppUnit, &nSeed))
            {
                sub_6FC4E1F0(pGame, pItemDrop, ppUnit, nSeed);
            }
        }
        break;
    }
    default:
        return 0;
    }

    return sub_6FC4C5F0_End(pGame, pItem, pItemDrop, pItemsTxtRecord);
}
