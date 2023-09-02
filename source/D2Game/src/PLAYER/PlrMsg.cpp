#include "PLAYER/PlrMsg.h"

#include <algorithm>

#include <Fog.h>
#include <Storm.h>
#include <D2BitManip.h>

#include <D2Collision.h>
#include <D2Skills.h>
#include <D2StatList.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <Drlg/D2DrlgDrlg.h>
#include <D2States.h>
#include <D2Chat.h>
#include <D2Items.h>
#include <Units/UnitRoom.h>
#include <Units/UnitFinds.h>
#include <D2Waypoints.h>
#include <D2QuestRecord.h>
#include <D2Inventory.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/SkillsIds.h>
#include <DataTbls/ObjectsIds.h>
#include <D2Monsters.h>
#include <UselessOrdinals.h>


#include "AI/AiGeneral.h"
#include "GAME/Arena.h"
#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Game.h"
#include "GAME/Level.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MISSILES/MissMode.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterUnique.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/PartyScreen.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerList.h"
#include "PLAYER/PlayerPets.h"
#include "PLAYER/PlayerStats.h"
#include "PLAYER/PlrIntro.h"
#include "PLAYER/PlrModes.h"
#include "PLAYER/PlrSave2.h"
#include "PLAYER/PlrTrade.h"
#include "QUESTS/Quests.h"
#include "SKILLS/SkillItem.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitNpc.h"


typedef int32_t(__fastcall* SERVERPACKETCALLBACK)(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int nPacketSize);

struct D2ServerPacketTableStrc
{
    SERVERPACKETCALLBACK pfCallback;		//0x00
    BOOL b;									//0x04
};


int32_t gbWhispReceiverInGame_6FD4DC20;
int32_t gbWhispReceiverNotListening_6FD4DC24;


#pragma pack(push, 1)
using PlrMsgFunc = void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2ClientStrc*, int32_t);
struct D2UnkPlrMsgStrc
{
    PlrMsgFunc field_0;
    int32_t field_4;
    int32_t field_8;
};
#pragma pack(pop)


constexpr D2UnkPlrMsgStrc stru_6FD30708[20] =
{
    { sub_6FC820C0, 0x08, 0x08 },
    { sub_6FC821C0, 0x07, 0x07 },
    { sub_6FC81E10, 0x01, 0x00 },
    { sub_6FC81E10, 0x17, 0x18 },
    { sub_6FC82010, 0x06, 0x06 },
    { sub_6FC821C0, 0x07, 0x07 },
    { sub_6FC81E10, 0x01, 0x00 },
    { sub_6FC81D20, 0x15, 0x16 },
    { sub_6FC81D20, 0x15, 0x16 },
    { sub_6FC82010, 0x12, 0x12 },
    { sub_6FC81D20, 0x15, 0x16 },
    { sub_6FC81D20, 0x15, 0x16 },
    { sub_6FC81D20, 0x15, 0x16 },
    { sub_6FC81D20, 0x15, 0x16 },
    { sub_6FC81D20, 0x15, 0x16 },
    { sub_6FC81D20, 0x15, 0x16 },
    { sub_6FC81D20, 0x15, 0x16 },
    { sub_6FC82010, 0x09, 0x09 },
    { sub_6FC81D20, 0x15, 0x16 },
    { sub_6FC81F60, 0x14, 0x14 },
};


//D2Game.0x6FC81C00
void __fastcall sub_6FC81C00(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    D2GAME_PACKETS_SendPacket0x0D_6FC3C920(pClient, 0xDu, 0, pUnit->dwUnitId, 19, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), pUnit->dwLastHitClass, UNITS_GetCurrentLifePercentage(pUnit));
}

//D2Game.0x6FC81CA0
void __fastcall sub_6FC81CA0(D2UnitStrc* pUnit, D2ClientStrc* pClient, char a3)
{
    D2GAME_PACKETS_SendPacket0x15_6FC3D0D0(pClient, 0x15u, pUnit->dwUnitType, pUnit->dwUnitId, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), a3);
}

//D2Game.0x6FC81D20
void __fastcall sub_6FC81D20(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t nAnimMode)
{
    D2SkillStrc* pSkill = UNITS_GetUsedSkill(pUnit);
    if (pSkill && (SKILLS_GetFlags(pSkill) & 4 || pUnit != CLIENTS_GetPlayerFromClient(pClient, 0)))
    {
        int32_t nUnitGUID = -1;
        if (pUnit)
        {
            nUnitGUID = pUnit->dwUnitId;
        }

        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
        const int32_t nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 1);

        if (pTarget && CLIENTS_IsInUnitsRoom(pTarget, pClient))
        {
            sub_6FC3F0C0(pClient, 0, nUnitGUID, pTarget->dwUnitType, pTarget->dwUnitId, nSkillId, 0, nSkillLevel, 0);
        }
        else
        {
            sub_6FC3F060(pClient, 0, nUnitGUID, D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath), D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath), nSkillId, 0, nSkillLevel, 0);
        }
    }
}

//D2Game.0x6FC81E10
void __fastcall sub_6FC81E10(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t nAnimMode)
{
    if (pUnit != CLIENTS_GetPlayerFromClient(pClient, 0))
    {
        D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pUnit);
        if (pTarget && CLIENTS_IsInUnitsRoom(pTarget, pClient))
        {
            D2GAME_PACKETS_SendPacket0x10_6FC3C9A0(pClient, 0x10, 0, pUnit->dwUnitId, (BYTE)stru_6FD30708[nAnimMode].field_8, pTarget->dwUnitType, pTarget->dwUnitId, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
        }
        else
        {
            D2GAME_PACKETS_SendPacket0x0F_6FC3CA00(pClient, 0x0F, 0, pUnit->dwUnitId, (BYTE)stru_6FD30708[nAnimMode].field_4, D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath), D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath), 0, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
        }
    }
}

//D2Game.0x6FC81F60
void __fastcall sub_6FC81F60(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t nAnimMode)
{
    D2GAME_PACKETS_SendPacket0x0F_6FC3CA00(pClient, 0x0F, 0, pUnit->dwUnitId, (BYTE)stru_6FD30708[nAnimMode].field_4, D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath), D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath), (BYTE)pUnit->dwLastHitClass, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
}

//D2Game.0x6FC82010
void __fastcall sub_6FC82010(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t nAnimMode)
{
    D2GAME_PACKETS_SendPacket0x0D_6FC3C920(pClient, 0x0D, 0, pUnit->dwUnitId, stru_6FD30708[nAnimMode].field_4, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), (BYTE)pUnit->dwLastHitClass, UNITS_GetCurrentLifePercentage(pUnit));
}

//D2Game.0x6FC820C0
void __fastcall sub_6FC820C0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t nAnimMode)
{
    sub_6FC82010(pGame, pUnit, pClient, nAnimMode);

    if (pUnit == CLIENTS_GetPlayerFromClient(pClient, 0))
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);

        D2_ASSERT(pClient);

        D2GAME_PACKETS_SendPacket0x1D_E_F_6FC3D480(pClient, STAT_GOLDLOST, STATLIST_UnitGetStatValue(pUnit, STAT_GOLDLOST, 0));
    }
}

//D2Game.0x6FC821C0
void __fastcall sub_6FC821C0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t nAnimMode)
{
    if (pUnit != CLIENTS_GetPlayerFromClient(pClient, 0))
    {
        D2GAME_PACKETS_SendPacket0x0D_6FC3C920(pClient, 0x0D, 0, pUnit->dwUnitId, stru_6FD30708[nAnimMode].field_4, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 0, UNITS_GetCurrentLifePercentage(pUnit));
    }
}

//D2Game.0x6FC82270
void __fastcall sub_6FC82270(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    int32_t nAnimMode = 0;
    if (pUnit)
    {
        nAnimMode = pUnit->dwAnimMode;
    }

    PlrMsgFunc pfSendMode = stru_6FD30708[nAnimMode].field_0;
    if (pfSendMode)
    {
        if (IsBadCodePtr((FARPROC)pfSendMode))
        {
            FOG_DisplayAssert("pfnSendMode", __FILE__, __LINE__);
            exit(-1);
        }

        pfSendMode(pGame, pUnit, pClient, nAnimMode);
    }
}

//D2Game.0x6FC822D0
void __fastcall D2GAME_UpdateAttribute_6FC822D0(D2UnitStrc* pUnit, WORD nStat, uint32_t nNewValue, D2UnitStrc* pPlayer)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    
    D2_ASSERT(pClient);

    if (pUnit == pPlayer)
    {
        D2GAME_PACKETS_SendPacket0x1D_E_F_6FC3D480(pClient, nStat, nNewValue);
    }
    else
    {
        sub_6FC3D830(pClient, 0x20u, pUnit ? pUnit->dwUnitId : -1, nStat, nNewValue);
    }
}

//D2Game.0x6FC82360
int32_t __fastcall sub_6FC82360(D2UnitStrc* pUnit, D2UnitStrc* pUnit2, int32_t bSkipHpCheck)
{
    if (!pUnit2 || !pUnit)
    {
        return 0;
    }
    
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit2, __FILE__, __LINE__);
    if (!pClient)
    {
        return 0;
    }
    
    D2ClientPlayerDataStrc* pClientPlayerData = CLIENTS_GetClientPlayerData(pClient);    
    if (!pClientPlayerData)
    {
        return 0;
    }
    
    const int32_t nShiftedHitpoints = STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) >> 8;
    const int32_t nShiftedMaxHp = STATLIST_GetMaxLifeFromUnit(pUnit) >> 8;
    if (nShiftedMaxHp <= 0)
    {
        return 0;
    }

    int32_t nHpDiff = pClientPlayerData->nHitPoints - nShiftedHitpoints;
    nHpDiff = std::abs(nHpDiff);

    if (!bSkipHpCheck && (100 * nHpDiff / nShiftedMaxHp < 10 || !nShiftedHitpoints && pClientPlayerData->nHitPoints))
    {
        return 0;
    }

    const int32_t nUnitX = CLIENTS_GetUnitX(pUnit);
    const int32_t nUnitY = CLIENTS_GetUnitY(pUnit);

    int32_t nX = 0;
    int32_t nY = 0;
    if (pUnit->pDynamicPath)
    {
        nX = nUnitX - D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
        nY = nUnitY - D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
    }
    
    D2StatListStrc* pHealthPotStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_HEALTHPOT);
    int32_t nPotionLifePercent = 0;
    if (pHealthPotStatList && nShiftedMaxHp)
    {
        D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
        const int32_t nFrameDiff = D2COMMON_10473(pHealthPotStatList) - pGame->dwGameFrame;

        nPotionLifePercent = 100 * ((STATLIST_UnitGetStatValue(pUnit, STAT_HITPOINTS, 0) + nFrameDiff * STATLIST_GetStatValue(pHealthPotStatList, STAT_HPREGEN, 0)) >> 8) / nShiftedMaxHp;
        if ((uint8_t)nPotionLifePercent > 100u)
        {
            nPotionLifePercent = 100;
        }
    }

    const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pUnit);
    D2StatListStrc* pManaPotStatList = STATLIST_GetStatListFromUnitAndState(pUnit, STATE_MANAPOT);
    int32_t nPotionManaPercent = 0;
    if (pManaPotStatList && nMaxMana)
    {
        D2CharStatsTxt* pCharStatsTxtRecord = PLRSAVE2_GetCharStatsTxtRecord(pUnit->dwClassId);
        if (pCharStatsTxtRecord)
        {
            int32_t nManaRegen = 25 * pCharStatsTxtRecord->nManaRegen;
            if (!nManaRegen)
            {
                nManaRegen = 7500;
            }

            int32_t nManaMultiplier = nMaxMana / nManaRegen;
            if (nManaMultiplier < 1)
            {
                nManaMultiplier = 1;
            }

            const int32_t nFrameMultiplier = nManaMultiplier * (STATLIST_UnitGetStatValue(pUnit, STAT_MANARECOVERYBONUS, 0) + 100) / 100 + STATLIST_UnitGetStatValue(pUnit, STAT_MANARECOVERY, 0);
            D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
            
            nPotionManaPercent = 100 * (STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0) + nFrameMultiplier * (D2COMMON_10473(pManaPotStatList) - pGame->dwGameFrame)) / nMaxMana;
            if ((uint8_t)nPotionManaPercent > 100u)
            {
                nPotionManaPercent = 100;
            }
        }
    }

    const int32_t nShiftedMana = STATLIST_UnitGetStatValue(pUnit, STAT_MANA, 0) >> 8;
    const int32_t nStamina = STATLIST_UnitGetStatValue(pUnit, STAT_STAMINA, 0) >> 8;

    if (pClientPlayerData->nPotionLifePercent != nPotionLifePercent || pClientPlayerData->nPotionManaPercent != nPotionManaPercent)
    {
        sub_6FC3D890(pClient, nShiftedHitpoints, nShiftedMana, nStamina, nPotionLifePercent, nPotionManaPercent, nUnitX, nUnitY, nX, nY);
    
        pClientPlayerData->nPosX = nUnitX;
        pClientPlayerData->nPosY = nUnitY;
        pClientPlayerData->nTargetOffsetX = nX;
        pClientPlayerData->nTargetOffsetY = nY;
        pClientPlayerData->dwInactivityTime = 0;
    }
    else
    {
        if (pClientPlayerData->nHitPoints != (uint16_t)nShiftedHitpoints || pClientPlayerData->nManaPoints != (uint16_t)nShiftedMana)
        {
            sub_6FC3D9A0(pClient, nShiftedHitpoints, nShiftedMana, nStamina, nUnitX, nUnitY, nX, nY);

            pClientPlayerData->nPosX = nUnitX;
            pClientPlayerData->nPosY = nUnitY;
            pClientPlayerData->nTargetOffsetX = nX;
            pClientPlayerData->nTargetOffsetY = nY;
            pClientPlayerData->dwInactivityTime = 0;
        }
        else if (pClientPlayerData->nStaminaPoints != (uint16_t)nStamina)
        {
            sub_6FC3DA90(pClient, nStamina, nUnitX, nUnitY, nX, nY);

            pClientPlayerData->nPosX = nUnitX;
            pClientPlayerData->nPosY = nUnitY;
            pClientPlayerData->nTargetOffsetX = nX;
            pClientPlayerData->nTargetOffsetY = nY;
            pClientPlayerData->dwInactivityTime = 0;
        }
        else
        {
            if (pClientPlayerData->dwInactivityTime <= 3)
            {
                ++pClientPlayerData->dwInactivityTime;
            }
            else
            {
                int32_t nXDiff = pClientPlayerData->nPosX - (uint16_t)nUnitX;
                nXDiff = std::abs(nXDiff);

                int32_t nYDiff = pClientPlayerData->nPosY - (uint16_t)nUnitY;
                nYDiff = std::abs(nYDiff);

                if (nXDiff < 2 && nYDiff < 2)
                {
                    ++pClientPlayerData->dwInactivityTime;
                }
                else
                {
                    sub_6FC3DA90(pClient, nStamina, nUnitX, nUnitY, nX, nY);

                    pClientPlayerData->nPosX = nUnitX;
                    pClientPlayerData->nPosY = nUnitY;
                    pClientPlayerData->nTargetOffsetX = nX;
                    pClientPlayerData->nTargetOffsetY = nY;
                    pClientPlayerData->dwInactivityTime = 0;
                }
            }
        }
    }

    const int32_t nGold = STATLIST_UnitGetStatValue(pUnit, STAT_GOLD, 0);
    if (nGold != pClientPlayerData->dwBeltGold)
    {
        D2GAME_PACKETS_SendPacket0x19_6FC3D3D0(pClient, nGold, pClientPlayerData->dwBeltGold);
        pClientPlayerData->dwBeltGold = nGold;
    }

    const int32_t nExperience = STATLIST_UnitGetStatValue(pUnit, STAT_EXPERIENCE, 0);
    if (nExperience != pClientPlayerData->dwExperience)
    {
        D2GAME_PACKETS_SendPacket0x1A_B_C_6FC3D410(pClient, nExperience, pClientPlayerData->dwExperience);
        pClientPlayerData->dwExperience = nExperience;
    }

    pClientPlayerData->nHitPoints = nShiftedHitpoints;
    pClientPlayerData->nStaminaPoints = nStamina;
    pClientPlayerData->nManaPoints = nShiftedMana;
    pClientPlayerData->nPotionLifePercent = nPotionLifePercent;
    pClientPlayerData->nPotionManaPercent = nPotionManaPercent;

    return 1;
}

//D2Game.0x6FC82830
void __fastcall sub_6FC82830(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    constexpr int32_t dword_6FD307F8[] =
    {
        STAT_VELOCITYPERCENT,
        STAT_ATTACKRATE,
        STAT_LEVEL,
        STAT_STRENGTH,
        STAT_DEXTERITY
    };

    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (pPlayer != pUnit)
    {
        for (int32_t i = 0; i < std::size(dword_6FD307F8); ++i)
        {
            D2ClientStrc* pPlayerClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
            D2_ASSERT(pPlayerClient);

            sub_6FC3D830(pPlayerClient, 0x20u, pUnit ? pUnit->dwUnitId : -1, dword_6FD307F8[i], STATLIST_GetUnitBaseStat(pUnit, dword_6FD307F8[i], 0));
        }
    }
}

//D2Game.0x6FC828D0
int32_t __fastcall sub_6FC828D0(D2UnitStrc* pPlayer, int32_t nUnitType, int32_t nUnitGUID, int32_t a4, D2GameStrc* pGame)
{
    switch (nUnitType)
    {
    case UNIT_PLAYER:
    {
        D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, 0, nUnitGUID);
        if (!pUnit)
        {
            return 1;
        }

        const int32_t nDistance = D2Common_10399(pPlayer, pUnit);
        if (nDistance > 50)
        {
            return 1;
        }

        if (nDistance <= 8)
        {
            if (pUnit->dwAnimMode != PLRMODE_DEAD || sub_6FC937A0(pGame, pPlayer) || sub_6FC937A0(pGame, pUnit))
            {
                PLRTRADE_TryToTrade(pGame, pPlayer, pUnit);
            }
            else
            {
                sub_6FC80440(pGame, pPlayer, pUnit);
            }
        }
        else
        {
            D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pPlayer, 0, 3u, 0, nUnitGUID, 0);
            UNITS_SetInteractData(pPlayer, -(a4 != 0) - 1, UNIT_PLAYER, nUnitGUID);
        }

        return 0;
    }
    case UNIT_MONSTER:
    {
        D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nUnitGUID);
        if (!pUnit)
        {
            return 1;
        }

        const int32_t nDistance = D2Common_10399(pPlayer, pUnit);
        if (nDistance > 50)
        {
            return 1;
        }

        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
        if (pMonStatsTxtRecord)
        {
            if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[9] && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[8])
            {
                D2Common_10153(pUnit->pDynamicPath);
                AIGENERAL_SetAiControlParam(pUnit, 1, 40);
                D2GAME_EVENTS_Delete_6FC34840(pGame, pUnit, 2, 0);
                EVENT_SetEvent(pGame, pUnit, 2, pGame->dwGameFrame + 1, 0, 0);
            }
        }

        if (nDistance <= 6)
        {
            if (!PLAYER_IsBusy(pPlayer))
            {
                D2Common_10153(pPlayer->pDynamicPath);
                if (pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
                {
                    sub_6FC61C70(pGame, pPlayer, pUnit, pUnit->pMonsterData->pMonInteract, 0);
                    return 0;
                }

                sub_6FC61C70(pGame, pPlayer, pUnit, 0, 0);
            }
        }
        else if (nDistance < 9)
        {
            D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pPlayer, 0, 3u, UNIT_MONSTER, nUnitGUID, 0);
            UNITS_SetInteractData(pPlayer, -(a4 != 0) - 1, UNIT_MONSTER, nUnitGUID);
        }
        return 0;
    }
    case UNIT_OBJECT:
    {
        D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, nUnitGUID);
        if (!pUnit)
        {
            return 1;
        }

        if (!DATATBLS_GetObjectsTxtRecord(pUnit->dwClassId))
        {
            FOG_DisplayAssert("ptObjectData", __FILE__, __LINE__);
            exit(-1);
        }

        if (D2Common_10399(pPlayer, pUnit) > 50)
        {
            return 1;
        }

        if (!UNITS_IsObjectInInteractRange(pPlayer, pUnit) || UNITS_TestCollisionBetweenInteractingUnits(pPlayer, pUnit, 0x804u))
        {
            D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pPlayer, 0, 3, UNIT_OBJECT, nUnitGUID, 0);
            UNITS_SetInteractData(pPlayer, -(a4 != 0) - 1, UNIT_OBJECT, nUnitGUID);
            return 0;
        }

        D2Common_10153(pPlayer->pDynamicPath);

        if (sub_6FC78B20(pGame, pPlayer, UNIT_OBJECT, nUnitGUID))
        {
            return 0;
        }

        return 3;
    }
    case UNIT_ITEM:
    {
        D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nUnitGUID);
        if (!pUnit || pUnit->dwAnimMode != 3)
        {
            return 1;
        }

        const int32_t nDistance = D2Common_10399(pPlayer, pUnit);
        if (nDistance > 50)
        {
            return 1;
        }

        if (nDistance > 4 || UNITS_TestCollisionBetweenInteractingUnits(pPlayer, pUnit, 0x804))
        {
            D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pPlayer, 0, 3, UNIT_ITEM, nUnitGUID, 0);
            UNITS_SetInteractData(pPlayer, -(a4 != 0) - 1, UNIT_ITEM, nUnitGUID);
            return 0;
        }

        if (a4)
        {
            D2GAME_PickupItemEx_6FC42B80(pGame, pPlayer, nUnitGUID, &a4);
        }
        else
        {
            D2GAME_PickupItem_6FC43340(pGame, pPlayer, nUnitGUID, &a4);
        }

        if (!a4)
        {
            return 0;
        }

        return 3;
    }
    case UNIT_TILE:
    {
        D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, UNIT_TILE, nUnitGUID);
        if (!pUnit)
        {
            return 1;
        }
        
        const int32_t nDistance = D2Common_10399(pPlayer, pUnit);
        if (nDistance > 50)
        {
            return 1;
        }

        if (nDistance > 4)
        {
            D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pPlayer, 0, 3, UNIT_TILE, nUnitGUID, 0);
            UNITS_SetInteractData(pPlayer, -(a4 != 0) - 1, UNIT_TILE, nUnitGUID);
        }
        else
        {
            SUNIT_WarpPlayer(pGame, pPlayer, pUnit);
        }

        return 0;
    }
    }

    return 1;
}

//D2Game.0x6FC82CB0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x01_Walk_6FC82CB0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    uint16_t nX = 0;
    uint16_t nY = 0;

    const int32_t nResult = sub_6FC82D10(pGame, pUnit, pPacket, nSize, &nX, &nY);
    if (nResult)
    {
        return nResult;
    }

    sub_6FC817D0(pGame, pUnit, 0, 2, nX, nY, 0);
    return 0;
}

//D2Game.0x6FC82D10
int32_t __fastcall sub_6FC82D10(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nPacketSize, uint16_t* pX, uint16_t* pY)
{
    *pX = 0;
    *pY = 0;

    if (nPacketSize == 5)
    {
        D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);
        if (pPlayerData)
        {
            const uint16_t nX = *(uint16_t*)((char*)pPacket + 1);
            const uint16_t nY = *(uint16_t*)((char*)pPacket + 3);

            if (!sub_6FC83C60(pUnit, nX, nY, 50))
            {
                pPlayerData->dwGameFrame = pGame->dwGameFrame;
                *pX = nX;
                *pY = nY;

                return 0;
            }
            else
            {
                if ((int32_t)(pGame->dwGameFrame - pPlayerData->dwGameFrame) > 25)
                {
                    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
                    if (pClient)
                    {
                        D2GAME_PACKETS_SendPacket0x15_6FC3D0D0(pClient, 0x15u, 0, pUnit->dwUnitId, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 1);
                    }
                }

                return 1;
            }
        }
        
        return 2;
    }

    return 3;
}

//D2Game.0x6FC82ED0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x02_WalkToEntity_6FC82ED0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 9)
    {
        return 3;
    }

    const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 1);
    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 5);
    if (nUnitType >= 6)
    {
        return 2;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, nUnitType, nItemGUID);
    if (!pItem)
    {
        return 1;
    }

    if (nUnitType != 4 || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50))
        {
            return 1;
        }
    }

    D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pUnit, 0, 2u, nUnitType, nItemGUID, 0);
    return 0;
}

//D2Game.0x6FC83090
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x03_Run_6FC83090(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    uint16_t nX = 0;
    uint16_t nY = 0;

    const int32_t nResult = sub_6FC82D10(pGame, pUnit, pPacket, nSize, &nX, &nY);
    if (nResult)
    {
        return nResult;
    }

    sub_6FC817D0(pGame, pUnit, 0, 3, nX, nY, 0);
    return 0;
}

//D2Game.0x6FC830F0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x04_RunToEntity_6FC830F0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 9)
    {
        return 3;
    }

    const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 1);
    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 5);
    if (nUnitType >= 6)
    {
        return 2;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, nUnitType, nItemGUID);
    if (!pItem)
    {
        return 1;
    }

    if (nUnitType != 4 || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50))
        {
            return 1;
        }
    }

    D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pUnit, 0, 3u, nUnitType, nItemGUID, 0);
    return 0;
}

//D2Game.0x6FC832B0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x05_ShiftLeftClickSkill_6FC832B0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    uint16_t nX = 0;
    uint16_t nY = 0;

    const int32_t nResult = sub_6FC82D10(pGame, pUnit, pPacket, nSize, &nX, &nY);
    if (nResult)
    {
        return nResult;
    }

    if (UNITS_GetLeftSkill(pUnit))
    {
        STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
        sub_6FC83340(pGame, pUnit, UNITS_GetLeftSkill(pUnit), nX, nY);
        return 0;
    }

    return 3;
}

//D2Game.0x6FC83340
int32_t __fastcall sub_6FC83340(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nX, int32_t nY)
{
    if (!pUnit || pUnit->dwAnimMode == PLRMODE_DEATH || pUnit->dwAnimMode == PLRMODE_DEAD)
    {
        return 2;
    }

    D2SkillStrc* pUsedSkill = pSkill;
    if (UNITS_CanDualWield(pUnit))
    {
        pUsedSkill = sub_6FC83450(pUnit, pSkill);
    }

    int32_t nSkillUseState = SKILLS_GetUseState(pUnit, pUsedSkill);
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecordFromSkill(pUsedSkill);
    if (!pSkillsTxtRecord)
    {
        return 2;
    }

    if (nSkillUseState == SKILLUSTATE_NOMANA || nSkillUseState == SKILLUSTATE_NOQUANTITY || nSkillUseState == SKILLUSTATE_SHAPERESTRICT)
    {
        if (pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[SKILLSFLAGINDEX_ATTACKNOMANA])
        {
            pUsedSkill = SKILLS_GetSkillById(pUnit, 0, -1);
            nSkillUseState = SKILLS_GetUseState(pUnit, pUsedSkill);
        }
    }

    const int32_t nSkillMode = SKILLS_GetSkillModeFromUnit(pUnit, pUsedSkill);
    if (!nSkillMode)
    {
        return 2;
    }

    if (nSkillUseState == SKILLUSTATE_USABLE)
    {
        sub_6FC817D0(pGame, pUnit, pUsedSkill, nSkillMode, nX, nY, 0);
        return 0;
    }
    else if (nSkillUseState == SKILLUSTATE_NOMANA)
    {
        D2GSPacketSrv5A packet5A = {};
        packet5A.nHeader = 0x5Au;
        packet5A.nColor = STRCOLOR_RED;
        packet5A.dwParam = 0;
        packet5A.nType = EVENTTYPE_CANTDOTHAT;
        D2GAME_PACKETS_SendPacket0x5A_6FC3DEC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet5A);
    }

    return 2;
}

//D2Game.0x6FC83450
D2SkillStrc* __fastcall sub_6FC83450(D2UnitStrc* pUnit, D2SkillStrc* pSkill)
{
    if (SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__))
    {
        return pSkill;
    }

    D2UnitStrc* pRightWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_RARM);
    if (pRightWeapon && (!ITEMS_CanBeEquipped(pRightWeapon) || ITEMS_GetItemType(pRightWeapon) == ITEMTYPE_MISSILE_POTION))
    {
        pRightWeapon = nullptr;
    }

    D2UnitStrc* pLeftWeapon = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, BODYLOC_LARM);
    if (pLeftWeapon && (!ITEMS_CanBeEquipped(pLeftWeapon) || ITEMS_GetItemType(pLeftWeapon) == ITEMTYPE_MISSILE_POTION))
    {
        pLeftWeapon = nullptr;
    }

    if (!pRightWeapon || !pLeftWeapon || !ITEMS_CheckItemTypeId(pRightWeapon, ITEMTYPE_WEAPON) || !ITEMS_CheckItemTypeId(pLeftWeapon, ITEMTYPE_WEAPON))
    {
        return pSkill;
    }

    if (SKILLS_GetParam4(pSkill))
    {
        SKILLS_SetParam4(pSkill, 0);
        return pSkill;
    }

    D2SkillStrc* pSwapSkill = SKILLS_GetSkillById(pUnit, SKILL_LEFTHANDSWING, -1);

    D2_ASSERT(pSwapSkill);

    SKILLS_SetParam4(pSkill, 5);

    return pSwapSkill;
}

//D2Game.0x6FC83550
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x06_LeftSkillOnUnit_6FC83550(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 9)
    {
        return 3;
    }

    const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 1);
    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 5);
    if (nUnitType >= 6)
    {
        return 2;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, nUnitType, nItemGUID);
    if (!pItem)
    {
        return 1;
    }

    if (nUnitType != 4 || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50))
        {
            return 1;
        }
    }

    if (UNITS_GetLeftSkill(pUnit))
    {
        STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
        sub_6FC836D0(pGame, pUnit, UNITS_GetLeftSkill(pUnit), nUnitType, nItemGUID, 1);
        return 0;
    }

    return 3;
}

//D2Game.0x6FC836D0
int32_t __fastcall sub_6FC836D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2SkillStrc* pSkill, int32_t nUnitType, int32_t nUnitGUID, int32_t a6)
{
    D2SkillStrc* pUsedSkill = nullptr;
    if (UNITS_CanDualWield(pUnit))
    {
        pUsedSkill = sub_6FC83450(pUnit, pSkill);
        pSkill = pUsedSkill;
    }
    else
    {
        pUsedSkill = pSkill;
    }
    
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecordFromSkill(pUsedSkill);
    if (!pSkillsTxtRecord)
    {
        return 2;
    }

    int32_t nUseState = SKILLS_GetUseState(pUnit, pUsedSkill);
    if (nUseState > 0)
    {
        if ((nUseState <= 2 || nUseState == 4) && pSkillsTxtRecord->dwFlags[0] &gdwBitMasks[27])
        {
            pSkill = SKILLS_GetSkillById(pUnit, 0, -1);
            nUseState = SKILLS_GetUseState(pUnit, pSkill);
            pUsedSkill = pSkill;
        }
    }

    if (nUseState != 0)
    {
        return 2;
    }

    const int32_t nSkillMode = SKILLS_GetSkillModeFromUnit(pUnit, pUsedSkill);
    if (!nSkillMode)
    {
        return 2;
    }

    if (!(pSkillsTxtRecord->dwFlags[0] & gdwBitMasks[27]) && (nUnitType < 0 || nUnitType > 1 && nUnitType != 3))
    {
        return 2;
    }

    int32_t nType = nUnitType;
    int32_t nGUID = nUnitGUID;

    D2UnitStrc* pOtherUnit = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
    if (pOtherUnit && STATES_CheckState(pOtherUnit, STATE_ATTACHED))
    {
        D2UnitStrc* pOwner = SUNIT_GetOwner(pGame, pOtherUnit);
        if (pOwner)
        {
            nType = pOwner->dwUnitType;
            nGUID = pOwner->dwUnitId;
            pOtherUnit = pOwner;
        }
    }

    if (!a6 || !pOtherUnit)
    {
        D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pUnit, pSkill, nSkillMode, nType, nGUID, 0);
        return 0;
    }

    // TODO: v18
    const int32_t v18 = D2Common_11016(pUnit, pSkill);
    if (v18 == 1)
    {
        if (UNITS_IsInMeleeRange(pUnit, pOtherUnit, UNITS_IsInMovingModeEx(pOtherUnit)))
        {
            D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pUnit, pSkill, nSkillMode, nType, nGUID, 0);
            return 0;
        }
    }
    else if (v18 != 4)
    {
        D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pUnit, pSkill, nSkillMode, nType, nGUID, 0);
        return 0;
    }

    D2GAME_PLAYERMODE_Change_6FC81A00(pGame, pUnit, nullptr, PLRMODE_RUN, nType, nGUID, 0);
    UNITS_SetInteractData(pUnit, SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__), nType, nGUID);
    return 0;
}

//D2Game.0x6FC83890
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x07_ShiftLeftSkillOnUnit_6FC83890(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 9)
    {
        return 3;
    }

    const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 1);
    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 5);

    if (nUnitType >= 6)
    {
        return 2;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, nUnitType, nItemGUID);
    if (!pItem)
    {
        return 1;
    }

    if (nUnitType != 4 || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50))
        {
            return 1;
        }
    }

    if (UNITS_GetLeftSkill(pUnit))
    {
        STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
        sub_6FC836D0(pGame, pUnit, UNITS_GetLeftSkill(pUnit), nUnitType, nItemGUID, 0);
        return 0;
    }

    return 3;
}

//D2Game.0x6FC83A10
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x08_ShiftLeftSkillHold_6FC83A10(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    uint16_t nX = 0;
    uint16_t nY = 0;

    const int32_t nResult = sub_6FC82D10(pGame, pUnit, pPacket, nSize, &nX, &nY);
    if (nResult)
    {
        return nResult;
    }

    if (!UNITS_GetLeftSkill(pUnit))
    {
        return 3;
    }

    if (!sub_6FC82D10(pGame, pUnit, pPacket, nSize, &nX, &nY))
    {
        STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
        sub_6FC83340(pGame, pUnit, UNITS_GetLeftSkill(pUnit), nX, nY);
    }

    return 0;
}

//D2Game.0x6FC83AC0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x09_LeftSkillOnUnitHold_6FC83AC0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt09* pPacket09 = (D2GSPacketClt09*)pPacket;

    if (nSize != sizeof(D2GSPacketClt09))
    {
        return 3;
    }

    if (pPacket09->dwUnitType >= 6)
    {
        return 2;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, pPacket09->dwUnitType, pPacket09->dwUnitGUID);
    if (!pItem)
    {
        return 1;
    }

    if (pPacket09->dwUnitType != UNIT_ITEM || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        const int32_t nResult = sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50);
        if (nResult)
        {
            return nResult;
        }
    }

    if (!UNITS_GetLeftSkill(pUnit))
    {
        return 3;
    }

    if (pItem && pPacket09->dwUnitType == UNIT_ITEM && ITEMS_IsInPlayersInventory(pUnit, pItem, 0) || !sub_6FC83D00(pUnit, pItem) && (!sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50)))
    {
        STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
        sub_6FC836D0(pGame, pUnit, UNITS_GetLeftSkill(pUnit), pPacket09->dwUnitType, pPacket09->dwUnitGUID, 1);
    }

    return 0;
}

//D2Game.0x6FC83C60
int32_t __fastcall sub_6FC83C60(D2UnitStrc* pUnit, uint16_t nX, uint16_t nY, int32_t nMaxDistance)
{
    int32_t nXDiff = CLIENTS_GetUnitX(pUnit) - nX;
    nXDiff = std::abs(nXDiff);

    if (nXDiff > nMaxDistance)
    {
        return 1;
    }

    int32_t nYDiff = CLIENTS_GetUnitY(pUnit) - nY;
    nYDiff = std::abs(nYDiff);

    if (nYDiff > nMaxDistance)
    {
        return 1;
    }

    return 0;
}

//D2Game.0x6FC83D00
int32_t __fastcall sub_6FC83D00(D2UnitStrc* pUnit, D2UnitStrc* a2)
{
    if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) == DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(a2))))
    {
        return 0;
    }

    return 2;
}

//D2Game.0x6FC83D40
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x0A_ShiftLeftSkillOnUnitHold_6FC83D40(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt0A* pPacket0A = (D2GSPacketClt0A*)pPacket;

    if (nSize != sizeof(D2GSPacketClt0A))
    {
        return 3;
    }

    if (pPacket0A->dwUnitType >= 6)
    {
        return 2;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, pPacket0A->dwUnitType, pPacket0A->dwUnitGUID);
    if (!pItem)
    {
        return 1;
    }

    if (pPacket0A->dwUnitType != UNIT_ITEM || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        const int32_t nResult = sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50);
        if (nResult)
        {
            return nResult;
        }
    }

    if (!UNITS_GetLeftSkill(pUnit))
    {
        return 3;
    }

    if (pPacket0A->dwUnitType == UNIT_ITEM && ITEMS_IsInPlayersInventory(pUnit, pItem, 0) || !sub_6FC83D00(pUnit, pItem) && !sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50) && UNITS_GetLeftSkill(pUnit))
    {
        STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
        sub_6FC836D0(pGame, pUnit, UNITS_GetLeftSkill(pUnit), pPacket0A->dwUnitType, pPacket0A->dwUnitGUID, 0);
    }

    return 0;
}

//D2Game.0x6FC83EE0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x0B_6FC83EE0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    return nSize != 1 ? 3 : 0;
}

//D2Game.0x6FC83EF0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x0C_RightSkill_6FC83EF0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    uint16_t nX = 0;
    uint16_t nY = 0;

    const int32_t nResult = sub_6FC82D10(pGame, pUnit, pPacket, nSize, &nX, &nY);
    if (nResult)
    {
        return nResult;
    }

    if (UNITS_GetRightSkill(pUnit))
    {
        STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
        sub_6FC83340(pGame, pUnit, UNITS_GetRightSkill(pUnit), nX, nY);
        return 0;
    }

    return 3;
}

//D2Game.0x6FC83F80
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x0D_RightSkillOnUnit_6FC83F80(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 9)
    {
        return 3;
    }

    const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 1);
    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 5);

    if ((uint32_t)nUnitType >= 6)
    {
        return 2;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, nUnitType, nItemGUID);
    if (!pItem)
    {
        return 1;
    }

    if (nUnitType != 4 || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50))
        {
            return 1;
        }
    }

    if (UNITS_GetRightSkill(pUnit))
    {
        STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
        sub_6FC836D0(pGame, pUnit, UNITS_GetRightSkill(pUnit), nUnitType, nItemGUID, 1);
        return 0;
    }

    return 3;
}

//D2Game.0x6FC84100
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x0E_ShiftRightSkillOnUnit_6FC84100(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 9)
    {
        return 3;
    }

    const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 1);
    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 5);
    if (nUnitType >= 6)
    {
        return 2;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, nUnitType, nItemGUID);
    if (!pItem)
    {
        return 1;
    }

    if (nUnitType != UNIT_ITEM || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50))
        {
            return 1;
        }
    }

    if (UNITS_GetRightSkill(pUnit))
    {
        STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
        sub_6FC836D0(pGame, pUnit, UNITS_GetRightSkill(pUnit), nUnitType, nItemGUID, 0);
        return 0;
    }

    return 3;
}

//D2Game.0x6FC84280
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x0F_RightSkillHold_6FC84280(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    uint16_t nX = 0;
    uint16_t nY = 0;

    const int32_t nResult = sub_6FC82D10(pGame, pUnit, pPacket, nSize, &nX, &nY);
    if (nResult)
    {
        return nResult;
    }

    if (UNITS_GetRightSkill(pUnit))
    {
        if (!sub_6FC82D10(pGame, pUnit, pPacket, nSize, &nX, &nY))
        {
            if (UNITS_GetRightSkill(pUnit))
            {
                STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
                sub_6FC83340(pGame, pUnit, UNITS_GetRightSkill(pUnit), nX, nY);
            }
        }

        return 0;
    }

    return 3;
}

//D2Game.0x6FC84330
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x10_RightSkillOnUnitHold_6FC84330(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 9)
    {
        return 3;
    }

    const int32_t nUnitType = *(int32_t*)((char*)pPacket + 1);
    if (nUnitType >= 6)
    {
        return 2;
    }

    const int32_t nUnitGUID = *(int32_t*)((char*)pPacket + 5);

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
    if (!pItem)
    {
        return 1;
    }

    if (nUnitType != UNIT_ITEM || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50))
        {
            return 1;
        }
    }

    D2SkillStrc* pSkill = UNITS_GetRightSkill(pUnit);
    if (pSkill)
    {
        if (nUnitType == UNIT_ITEM && ITEMS_IsInPlayersInventory(pUnit, pItem, 0) || !sub_6FC83D00(pUnit, pItem) && !sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50))
        {
            STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
            sub_6FC836D0(pGame, pUnit, pSkill, nUnitType, nUnitGUID, 1);
        }

        return 0;
    }

    return 3;
}

//D2Game.0x6FC844D0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x11_ShiftRightSkillOnUnitHold_6FC844D0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt11* pPacket11 = (D2GSPacketClt11*)pPacket;

    if (nSize != sizeof(D2GSPacketClt11))
    {
        return 3;
    }

    if (pPacket11->dwUnitType >= 6)
    {
        return 2;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, pPacket11->dwUnitType, pPacket11->dwUnitGUID);
    if (!pItem)
    {
        return 1;
    }

    if (pPacket11->dwUnitType != UNIT_ITEM || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        const int32_t nResult = sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50);
        if (nResult)
        {
            return nResult;
        }
    }

    if (!UNITS_GetRightSkill(pUnit))
    {
        return 3;
    }

    if (pPacket11->dwUnitType == UNIT_ITEM && ITEMS_IsInPlayersInventory(pUnit, pItem, 0) || !sub_6FC83D00(pUnit, pItem) && !sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50) && UNITS_GetRightSkill(pUnit))
    {
        STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pUnit, STAT_PIERCE_IDX, 0) + 1, 0);
        sub_6FC836D0(pGame, pUnit, UNITS_GetRightSkill(pUnit), pPacket11->dwUnitType, pPacket11->dwUnitGUID, 0);
    }

    return 0;
}

//D2Game.0x6FC84670
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x12_6FC84670(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 1)
    {
        STATES_ToggleState(pUnit, STATE_INFERNO, 0);
        return 0;
    }

    return 3;
}

//D2Game.0x6FC84690
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x14_HandleOverheadChat_6FC84690(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize < 4 || nSize > 275)
    {
        return 3;
    }

    char szText[256] = {};
    SStrCopy(szText, (char*)pPacket + 3, sizeof(szText));

    const int32_t nStringLength = SStrLen((char*)pPacket + 3);
    if (nStringLength < 256)
    {
        char szName[16] = {};
        SStrCopy(szName, (char*)pPacket + nStringLength + 4, sizeof(szName));

        if (pUnit && pUnit->pHoverText)
        {
            CHAT_FreeHoverMsg(pGame->pMemoryPool, pUnit->pHoverText);
        }

        D2HoverTextStrc* pHoverText = CHAT_AllocHoverMsg(pGame->pMemoryPool, szText, pGame->dwGameFrame);
        if (pHoverText)
        {
            CHAT_SetLangIdInHoverMsg(pHoverText, *((uint8_t*)pPacket + 2));

            if (pUnit)
            {
                pUnit->pHoverText = pHoverText;
            }

            UNITROOM_RefreshUnit(pUnit);

            if (pUnit)
            {
                pUnit->dwFlags |= UNITFLAG_HASTXTMSG;
            }

            int32_t nTimeout = CHAT_GetTimeoutFromHoverMsg(pHoverText);
            if (nTimeout < pGame->dwGameFrame + 1)
            {
                nTimeout = pGame->dwGameFrame + 1;
            }

            EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_FREEHOVER, nTimeout, 0, 0);
        }

        return 0;
    }

    return 2;
}

//D2Game.0x6FC847B0
void __fastcall D2GAME_PACKETS_HandlePlayerMessage_6FC847B0(D2ClientStrc* pClient, void* pArgs)
{
    D2GSPacketSrv26Args* pPacket26Args = (D2GSPacketSrv26Args*)pArgs;
    D2GameStrc* pGame = CLIENTS_GetGame(pClient);

    if (pPacket26Args->nUnitId != -1)
    {
        D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, pPacket26Args->nUnitId);
        D2UnitStrc* pLocalPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);

        if (pOtherPlayer && pLocalPlayer && pOtherPlayer != pLocalPlayer)
        {
            if (PLAYERLIST_CheckFlag(pLocalPlayer, pOtherPlayer, 4) || PLAYERLIST_CheckFlag(pOtherPlayer, pLocalPlayer, 2))
            {
                return;
            }

            int32_t nUnitType = 0;
            int32_t nUnitGUID = 0;
            
            if (SUNIT_GetInteractInfo(pOtherPlayer, &nUnitType, &nUnitGUID) && nUnitType == UNIT_PLAYER)
            {
                D2UnitStrc* pInteractUnit = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID);
                if (pInteractUnit && pInteractUnit != pLocalPlayer)
                {
                    return;
                }
            }

            if (SUNIT_GetInteractInfo(pLocalPlayer, &nUnitType, &nUnitGUID) && nUnitType == UNIT_PLAYER)
            {
                D2UnitStrc* pInteractUnit = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID);
                if (pInteractUnit && pInteractUnit != pOtherPlayer)
                {
                    return;
                }
            }
        }
    }

    D2GAME_PACKETS_SendPacket0x26_ServerMessage_6FC3DDF0(pClient, pPacket26Args->pPacket26);
}

//D2Game.0x6FC848A0
void __fastcall D2GAME_PACKETS_HandleWhisper_6FC848A0(D2ClientStrc* pClient, void* pArgs)
{
    D2GSPacketSrv26Args* pPacket26Args = (D2GSPacketSrv26Args*)pArgs;
    D2GameStrc* pGame = CLIENTS_GetGame(pClient);
    if (!SStrCmpI(CLIENTS_GetName(pClient), pPacket26Args->pPacket15->szName, INT_MAX))
    {
        const int32_t nUnitGUID = pPacket26Args->nUnitId;
        if (nUnitGUID != -1)
        {
            D2UnitStrc* pReceiver = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, nUnitGUID);
            D2UnitStrc* pLocalPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
            if (pReceiver && pLocalPlayer && (PLAYERLIST_CheckFlag(pLocalPlayer, pReceiver, 4) || PLAYERLIST_CheckFlag(pReceiver, pLocalPlayer, 2)))
            {
                gbWhispReceiverNotListening_6FD4DC24 = 1;
                return;
            }
        }

        gbWhispReceiverInGame_6FD4DC20 = 1;
        D2GAME_PACKETS_SendPacket0x26_ServerMessage_6FC3DDF0(pClient, pPacket26Args->pPacket26);
    }
}

//D2Game.0x6FC84940
void __fastcall j_D2GAME_PACKETS_SendPacket0x26_ServerMessage_6FC3DDF0(D2ClientStrc* pClient, void* pMsg)
{
    D2GAME_PACKETS_SendPacket0x26_ServerMessage_6FC3DDF0(pClient, (D2GSPacketSrv26*)pMsg);
}

//D2Game.0x6FC84950
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x15_HandleChatMessage_6FC84950(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt15* pPacket15 = (D2GSPacketClt15*)pPacket;

    D2GSPacketClt15 packet15 = {};

    packet15.nMessageType = pPacket15->nMessageType;
    packet15.nHeader = pPacket15->nHeader;
    packet15.nLang = pPacket15->nLang;
    SStrCopy(packet15.szMessage, pPacket15->szMessage, sizeof(packet15.szMessage));

    const int32_t nMessageLen = SStrLen(pPacket15->szMessage);
    if (nMessageLen >= 256 || nMessageLen + 4 >= nSize)
    {
        return 2;
    }

    const char* szWhispReceiverName = &pPacket15->szMessage[nMessageLen + 1];
    SStrCopy(packet15.szName, szWhispReceiverName, 0x10u);
    //SStrLen(szWhispReceiverName);

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    const char* szClientName = CLIENTS_GetName(pClient);
    
    D2GSPacketSrv26 packet26 = {};
    if (SStrLen(szClientName))
    {
        SStrCopy(packet26.szName, szClientName, INT_MAX);
    }
    else
    {
        packet26.szName[0] = 0;
    }

    const int32_t nNameLen = SStrLen(packet15.szName);
    int32_t bWhisper = 1;
    if (nNameLen <= 0 || nNameLen >= 16)
    {
        bWhisper = 0;
    }

    packet15.szName[15] = 0;

    packet26.nNameColor = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);

    if (packet15.szMessage[255])
    {
        return 2;
    }

    memcpy(packet26.szMessage, packet15.szMessage, sizeof(packet26.szMessage));
    packet26.nLang = packet15.nLang;
    packet26.nMessageColor = 0;
    packet26.nHeader = 0x26u;
    packet26.dwUnitGUID = 0;
    packet26.nUnitType = 2;

    void(__fastcall * pFn)(D2ClientStrc*, void*) = nullptr;
    if (bWhisper)
    {
        packet26.nMessageType = 2;
        pFn = D2GAME_PACKETS_HandleWhisper_6FC848A0;
    }
    else
    {
        packet26.nMessageType = 1;
        pFn = D2GAME_PACKETS_HandlePlayerMessage_6FC847B0;
    }

    gbWhispReceiverInGame_6FD4DC20 = 0;
    gbWhispReceiverNotListening_6FD4DC24 = 0;

    D2GSPacketSrv26Args packet26Args = {};
    
    packet26Args.pPacket26 = &packet26;
    packet26Args.pPacket15 = &packet15;
    if (pUnit && pUnit->dwUnitType == UNIT_PLAYER)
    {
        packet26Args.nUnitId = pUnit->dwUnitId;
    }
    else
    {
        packet26Args.nUnitId = -1;
    }

    GAME_ForEachIngameClient(pGame, pFn, &packet26Args);

    if (packet26.nMessageType == 2)
    {
        if (gbWhispReceiverNotListening_6FD4DC24)
        {
            D2GSPacketSrv5A packet5A = {};

            packet5A.nHeader = 0x5Au;
            packet5A.nType = EVENTTYPE_NOTLISTENINGTOYOU;
            packet5A.nColor = STRCOLOR_WHITE;
            packet5A.dwParam = 0;
            SStrCopy(packet5A.szText, packet15.szName, INT_MAX);
            packet5A.szText[15] = 0;
            D2GAME_PACKETS_SendPacket0x5A_6FC3DEC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet5A);
            return 0;
        }

        if (!gbWhispReceiverInGame_6FD4DC20)
        {
            D2GSPacketSrv5A packet5A = {};

            packet5A.nHeader = 0x5Au;
            packet5A.nType = EVENTTYPE_NOTINGAME;
            packet5A.nColor = STRCOLOR_WHITE;
            packet5A.dwParam = 0;
            SStrCopy(packet5A.szText, packet15.szName, INT_MAX);
            packet5A.szText[15] = 0;
            D2GAME_PACKETS_SendPacket0x5A_6FC3DEC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet5A);
            return 0;
        }

        memset(&packet26, 0x00, sizeof(packet26));
        packet26.nHeader = 0x26u;
        packet26.nMessageType = 6;
        packet26.nLang = 0;
        packet26.nUnitType = 2;
        packet26.dwUnitGUID = 0;
        packet26.nMessageColor = 0;
        SStrCopy(packet26.szName, packet15.szName, INT_MAX);
        packet26.szName[15] = 0;
        memcpy(packet26.szMessage, packet15.szMessage, sizeof(packet26.szMessage));
        D2GAME_PACKETS_SendPacket0x26_ServerMessage_6FC3DDF0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet26);
    }

    return 0;
}

//D2Game.0x6FC84C70
void __fastcall sub_6FC84C70(D2GameStrc* pGame, const char* szMessage, uint8_t nColor)
{
    D2GSPacketSrv26 pPacket = {};

    pPacket.dwUnitGUID = 0;
    pPacket.szName[0] = 0;
    pPacket.nLang = 0;
    pPacket.nHeader = 0x26u;
    pPacket.nMessageType = 1;
    pPacket.nUnitType = UNIT_OBJECT;
    pPacket.nMessageColor = nColor;
    memcpy(pPacket.szMessage, szMessage, sizeof(pPacket.szMessage));

    GAME_ForEachIngameClient(pGame, j_D2GAME_PACKETS_SendPacket0x26_ServerMessage_6FC3DDF0, &pPacket);
}

//D2Game.0x6FC84CD0
void __fastcall sub_6FC84CD0(D2GameStrc* pGame, const char* szMessage, uint8_t nColor)
{
    D2GSPacketSrv26 pPacket = {};

    pPacket.nMessageColor = nColor;
    pPacket.nHeader = 0x26u;
    pPacket.nMessageType = 4;
    pPacket.dwUnitGUID = 0;
    pPacket.nUnitType = UNIT_PLAYER;
    pPacket.nLang = 0;
    memcpy(pPacket.szMessage, szMessage, sizeof(pPacket.szMessage));

    GAME_ForEachIngameClient(pGame, j_D2GAME_PACKETS_SendPacket0x26_ServerMessage_6FC3DDF0, &pPacket);
}

//D2Game.0x6FC84D30
void __fastcall j_D2GAME_SendPacket0x5A_6FC3DEC0(D2ClientStrc* pClient, void* pPacket)
{
    D2GAME_PACKETS_SendPacket0x5A_6FC3DEC0(pClient, (D2GSPacketSrv5A*)pPacket);
}

//D2Game.0x6FC84D40
void __fastcall sub_6FC84D40(D2GameStrc* pGame, D2GSPacketSrv5A* pPacket5A)
{
    for (int32_t i = 0; i < 16; ++i)
    {
        if (!pPacket5A->szText[i])
        {
            GAME_ForEachIngameClient(pGame, j_D2GAME_SendPacket0x5A_6FC3DEC0, pPacket5A);
            return;
        }
    }
}

//D2Game.0x6FC84D70
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x13_InteractWithEntity_6FC84D70(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 1);
        if (nUnitType < 6)
        {
            return sub_6FC828D0(pUnit, nUnitType, *(int32_t*)((char*)pPacket + 5), 0, pGame);
        }

        return 2;
    }

    return 3;
}

//D2Game.0x6FC84DB0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x16_PickItemOnGround_6FC84DB0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 13)
    {
        const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 1);
        const int32_t nUnitGUID = *(int32_t*)((char*)pPacket + 5);
        if (nUnitType < 6)
        {
            if ((pUnit ? pUnit->dwUnitId : -1) != nUnitGUID || nUnitType != UNIT_PLAYER)
            {
                return sub_6FC828D0(pUnit, nUnitType, nUnitGUID, *(int32_t*)((char*)pPacket + 9), pGame);
            }

            return 3;
        }
        
        return 2;
    }
    
    return 3;
}

//D2Game.0x6FC84E20
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x17_DropItemOnGround_6FC84E20(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 5)
    {
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);
        
        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem && pItem->dwAnimMode == IMODE_ONCURSOR && INVENTORY_GetCursorItem(pUnit->pInventory) == pItem)
        {
            if (PLAYER_IsBusy(pUnit) && sub_6FC937A0(pGame, pUnit))
            {
                return 3;
            }

            sub_6FC44030(pGame, pUnit, nItemGUID);
            return 0;
        }

        return 1;
    }

    return 3;
}

//D2Game.0x6FC84ED0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x18_InsertItemInBuffer_6FC84ED0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 17)
    {
        FOG_Trace("svalidatemessage failed for sInsertItemGrid()\n");
        return 3;
    }

    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);
    const int32_t nX = *(int32_t*)((char*)pPacket + 5);
    const int32_t nY = *(int32_t*)((char*)pPacket + 9);
    const int32_t nInvPage = *(int32_t*)((char*)pPacket + 13);

    D2_ASSERT(pGame);

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem || pItem->dwAnimMode != IMODE_ONCURSOR || INVENTORY_GetCursorItem(pUnit->pInventory) != pItem)
    {
        return 1;
    }

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        if (!PLAYER_IsBusy(pUnit) && nInvPage)
        {
            FOG_TraceF(gszEmptyString_6FD447EC, " tell jon and steve about error at line %d of file %s\n", __LINE__, __FILE__);
            return 2;
        }

        if (nInvPage == 4)
        {
            D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
            if (!pRoom || !DUNGEON_IsRoomInTown(pRoom))
            {
                FOG_TraceF(gszEmptyString_6FD447EC, " Player %s insert itemgrid error #1\n", UNITS_GetPlayerData(pUnit)->szName);
                return 3;
            }
        }
        else if (nInvPage != 3)
        {
            if (nInvPage == 2)
            {
                if (!sub_6FC937A0(pGame, pUnit))
                {
                    return 3;
                }
            }

            if (nInvPage != 0)
            {
                return 2;
            }
        }
    }
    else
    {
        if (nInvPage >= 5)
        {
            return 2;
        }
    }

    ITEMS_SetInvPage(pItem, nInvPage);

    if (D2GAME_PlaceItem_6FC44410(__FILE__, __LINE__, pGame, pUnit, nItemGUID, nX, nY, 0, 1, 0))
    {
        return 0;
    }

    char szName[32] = {};
    GAME_LogMessage(3, "[sInsertItemGrid] ITEMSInsertInventoryGrid err for player %s and item %s", UNITS_GetPlayerData(pUnit)->szName, DATATBLS_GetUnitNameFromUnit(pItem, szName));
    return 3;
}

//D2Game.0x6FC850C0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x19_RemoveItemFromBuffer_6FC850C0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 5)
    {
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);

        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem && pItem->dwAnimMode == IMODE_STORED && INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pItem))
        {
            if (INVENTORY_GetCursorItem(pUnit->pInventory))
            {
                D2GSPacketSrv5A packet5A = {};
                packet5A.nHeader = 0x5A;
                packet5A.nColor = STRCOLOR_RED;
                packet5A.dwParam = 0;
                packet5A.nType = EVENTTYPE_CANTDOTHAT;
                D2GAME_PACKETS_SendPacket0x5A_6FC3DEC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet5A);
                return 2;
            }
            else
            {
                if (ITEMS_GetInvPage(pItem) == 1)
                {
                    GAME_LogMessage(3, "[sRemoveItemGrid] Tried to remove item from page 1");
                    return 3;
                }
                else
                {
                    int32_t a6 = 0;
                    if (sub_6FC7C3A0(pGame, pUnit, pItem) && !sub_6FC446B0(pGame, pUnit, nItemGUID, &a6, 1, 0, 0, 0) && a6)
                    {
                        char szName[32] = {};
                        GAME_LogMessage(3, "[sRemoveItemGrid] error #2 item '%s' from player %s", DATATBLS_GetUnitNameFromUnit(pItem, szName), UNITS_GetPlayerData(pUnit)->szName);
                        return 3;
                    }

                    return 0;
                }
                
                return 2;
            }
        }
        
        return 1;
    }

    GAME_LogMessage(3, "[sRemoveItemGrid] sValidateMessageSize err %d", 3);
    return 3;
}

//D2Game.0x6FC85280
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x1A_EquipItem_6FC85280(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);
        const int32_t nBodyLoc = *((uint8_t*)pPacket + 5);

        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem && pItem->dwAnimMode == IMODE_ONCURSOR && INVENTORY_GetCursorItem(pUnit->pInventory) == pItem)
        {
            if (nBodyLoc > 0 && nBodyLoc < 11)
            {
                int32_t a6 = 0;
                if (sub_6FC45060(pGame, pUnit, nItemGUID, nBodyLoc, 0, &a6) || !a6)
                {
                    return 0;
                }

                const char* szPlayerName = UNITS_GetPlayerData(pUnit)->szName;
                if (pItem)
                {
                    char szName[32] = {};
                    FOG_Trace("sinsertitembody cheat detected for item %s player %s\n", DATATBLS_GetUnitNameFromUnit(pItem, szName), szPlayerName);
                }
                else
                {
                    FOG_Trace("sinsertitembody cheat detected for player %s\n", szPlayerName);
                }

                return 3;
            }
            
            return 2;
        }
        
        return 1;
    }

    return 3;
}

//D2Game.0x6FC853C0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x1B_Swap2HandedItem_6FC853C0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);
        const uint8_t nBodyLoc = *((uint8_t*)pPacket + 5);
        
        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem && pItem->dwAnimMode == IMODE_ONCURSOR && INVENTORY_GetCursorItem(pUnit->pInventory) == pItem)
        {
            if (nBodyLoc > 0 && nBodyLoc < 11)
            {
                int32_t a5 = 0;
                if (sub_6FC45550(pGame, pUnit, nItemGUID, nBodyLoc, 0, &a5) || !a5)
                {
                    return 0;
                }

                GAME_LogMessage(3, "[sInsertItemBodyRmvOpp] error #1 for player %s", UNITS_GetPlayerData(pUnit)->szName);
                return 3;
            }
            
            return 2;
        }
        
        return 1;
    }

    return 3;
}

//D2Game.0x6FC854B0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x1C_RemoveBodyItem_6FC854B0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 3)
    {
        const uint16_t nBodyLoc = *(uint16_t*)((char*)pPacket + 1);
        if (nBodyLoc > 0 && nBodyLoc < 11)
        {
            D2UnitStrc* pItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
            if (sub_6FC7C3A0(pGame, pUnit, pItem) && (nBodyLoc != BODYLOC_BELT || sub_6FC93740(pGame, pUnit)))
            {
                int32_t a5 = 0;
                if (!sub_6FC45B30(pGame, pUnit, nBodyLoc, 0, &a5) && a5)
                {
                    return 3;
                }
            }

            return 0;
        }

        return 2;
    }

    return 3;
}

//D2Game.0x6FC85550
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x1D_SwapCursorItemWithBody_6FC85550(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);
        const int32_t nBodyLoc = *((uint8_t*)pPacket + 5);
        
        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem && pItem->dwAnimMode == 4 && INVENTORY_GetCursorItem(pUnit->pInventory) == pItem)
        {
            if (nBodyLoc > 0 && nBodyLoc < 11)
            {
                if (INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc))
                {
                    if (nBodyLoc == BODYLOC_BELT && !sub_6FC93740(pGame, pUnit))
                    {
                        return 0;
                    }

                    D2UnitStrc* pEquippedItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
                    if (!sub_6FC7C3A0(pGame, pUnit, pEquippedItem))
                    {
                        return 0;
                    }

                    int32_t a5 = 0;
                    if (sub_6FC45E60(pGame, pUnit, nItemGUID, nBodyLoc, 0, &a5) || !a5)
                    {
                        return 0;
                    }

                    FOG_Trace("sSwapItemBody Cheat detected for player %s\n", UNITS_GetPlayerData(pUnit)->szName);
                    return 3;
                }
                
                return 1;
            }
            
            return 2;
        }
        
        return 1;
    }

    return 3;
}

//D2Game.0x6FC85690
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x1E_SwapTwo1HandedWithOne2HandedItem_6FC85690(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);
        const int32_t nBodyLoc = *((uint8_t*)pPacket + 5);
        
        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem && pItem->dwAnimMode == IMODE_ONCURSOR && INVENTORY_GetCursorItem(pUnit->pInventory) == pItem)
        {
            if (nBodyLoc > 0 && nBodyLoc < 11)
            {
                if (nBodyLoc == BODYLOC_RARM || nBodyLoc == BODYLOC_LARM)
                {
                    const int32_t nResult = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc) == 0;
                    if (nResult)
                    {
                        return nResult;
                    }

                    D2UnitStrc* pEquippedItem = INVENTORY_GetItemFromBodyLoc(pUnit->pInventory, nBodyLoc);
                    int32_t a5 = 0;
                    if (sub_6FC7C3A0(pGame, pUnit, pEquippedItem) && !sub_6FC46270(pGame, pUnit, nItemGUID, nBodyLoc, &a5) && a5)
                    {
                        FOG_Trace("sDoubleSwapItemBody Cheat detected for player %s\n", UNITS_GetPlayerData(pUnit)->szName);
                        return 3;
                    }

                    return 0;
                }
                
                return 3;
            }
            
            return 2;
        }
        
        return 1;
    }

    return 3;
}

//D2Game.0x6FC857C0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x1F_SwapCursorBufferItems_6FC857C0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 17)
    {
        const int32_t nCursorItemGUID = *(int32_t*)((char*)pPacket + 1);
        const int32_t nInventoryItemGUID = *(int32_t*)((char*)pPacket + 5);
        const int32_t nX = *(int32_t*)((char*)pPacket + 9);
        const int32_t nY = *(int32_t*)((char*)pPacket + 13);

        D2_ASSERT(pGame);

        D2UnitStrc* pCursorItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nCursorItemGUID);
        if (pCursorItem && pCursorItem->dwAnimMode == IMODE_ONCURSOR && INVENTORY_GetCursorItem(pUnit->pInventory) == pCursorItem)
        {
            D2UnitStrc* pInventoryItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nInventoryItemGUID);
            if (pInventoryItem && pInventoryItem->dwAnimMode == IMODE_STORED && INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pInventoryItem))
            {
                if (ITEMS_GetInvPage(pInventoryItem) == 1)
                {
                    FOG_Trace("Tried to swap item from page 1\n");
                    return 3;
                }
                else if (ITEMS_GetInvPage(pInventoryItem) == 2)
                {
                    FOG_Trace("Tried to swap item from page 2\n");
                    return 3;
                }
                else
                {
                    int32_t bPlaced = 0;
                    if (sub_6FC7C3A0(pGame, pUnit, pInventoryItem) && !sub_6FC46D40(pGame, pUnit, nCursorItemGUID, nInventoryItemGUID, nX, nY, &bPlaced) && bPlaced)
                    {
                        char szName[32] = {};
                        char szCursorItemName[32] = {};
                        FOG_Trace("sSwapItemGrid Cheat detected of item in hand of %s and item inv %s for player %s\n", 
                                                  DATATBLS_GetUnitNameFromUnit(pCursorItem, szCursorItemName), DATATBLS_GetUnitNameFromUnit(pInventoryItem, szName), 
                                                  UNITS_GetPlayerData(pUnit)->szName);
                        return 3;
                    }
                    else
                    {
                        return 0;
                    }
                }

                return 2;
            }
        }

        return 1;
    }

    return 3;
}

//D2Game.0x6FC85990
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x20_UseGridItem_6FC85990(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt20* pPacket20 = (D2GSPacketClt20*)pPacket;

    if (nSize == 13)
    {
        const int32_t nItemGUID = pPacket20->nItemGUID;
        const int32_t nX = pPacket20->nPosX;
        const int32_t nY = pPacket20->nPosY;

        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem && pItem->dwAnimMode == IMODE_STORED && INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pItem))
        {
            if (sub_6FC83C60(pUnit, nX, nY, 50))
            {
                return 1;
            }

            int32_t a5 = 0;
            if (sub_6FC47D30(pGame, pUnit, nItemGUID, nX, nY, &a5) || !a5)
            {
                return 0;
            }

            char szName[32] = {};
            FOG_Trace("sUseItemGridXY Cheat detected for item %s player %s\n", DATATBLS_GetUnitNameFromUnit(pItem, szName), UNITS_GetPlayerData(pUnit)->szName);
            return 3;
        }
       
        return 1;
    }

    return 3;
}

//D2Game.0x6FC85B50
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x27_UseItemAction_6FC85B50(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt27* pPacket27 = (D2GSPacketClt27*)pPacket;

    if (nSize == 9)
    {        
        D2_ASSERT(pGame);

        D2UnitStrc* pTargetItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, pPacket27->nTargetItemGUID);
        if (pTargetItem && (INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pTargetItem) || INVENTORY_GetCursorItem(pUnit->pInventory) == pTargetItem))
        {
            D2UnitStrc* pUseItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, pPacket27->nUseItemGUID);
            if (pUseItem && (INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pUseItem) || INVENTORY_GetCursorItem(pUnit->pInventory) == pUseItem))
            {
                int32_t a5 = 0;
                if (sub_6FC47470(pGame, pUnit, pPacket27->nTargetItemGUID, pPacket27->nUseItemGUID, &a5) || !a5)
                {
                    return 0;
                }

                FOG_Trace("sUseItemAction Cheat detected for player %s\n", UNITS_GetPlayerData(pUnit)->szName);
                return 3;
            }
        }

        return 1;
    }

    return 3;
}

//D2Game.0x6FC85C80
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x21_StackItems_6FC85C80(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        const int32_t nItemGUID1 = *(int32_t*)((char*)pPacket + 1);
        const int32_t nItemGUID2 = *(int32_t*)((char*)pPacket + 5);
        
        D2_ASSERT(pGame);

        D2UnitStrc* pItem1 = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID1);
        if (pItem1 && (INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pItem1) || INVENTORY_GetCursorItem(pUnit->pInventory) == pItem1))
        {
            D2UnitStrc* pItem2 = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID2);
            if (pItem2 && (INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pItem2) || INVENTORY_GetCursorItem(pUnit->pInventory) == pItem2))
            {
                if (nItemGUID1 != nItemGUID2)
                {
                    int32_t a5 = 0;
                    if (sub_6FC484E0(pGame, pUnit, nItemGUID1, nItemGUID2, &a5) || !a5)
                    {
                        return 0;
                    }
                }

                return 3;
            }
        }

        return 1;
    }

    return 3;
}

//D2Game.0x6FC85DA0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x22_UnstackItems_6FC85DA0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 5)
    {
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);

        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem && (INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pItem) || INVENTORY_GetCursorItem(pUnit->pInventory) == pItem))
        {
            int32_t a2 = 0;
            if (D2GAME_Return0_6FC48930(nItemGUID, &a2) || !a2)
            {
                return 0;
            }

            FOG_Trace("sRemoveStackItem Cheat detected for player %s\n", UNITS_GetPlayerData(pUnit)->szName);
            return 3;
        }
        
        return 1;
    }

    return 3;
}

//D2Game.0x6FC85E70
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x23_ItemToBelt_6FC85E70(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    // TODO: v7
    if (nSize == 9)
    {
        const int32_t nCursorItemGUID = *(int32_t*)((char*)pPacket + 1);
        const int32_t v7 = *(int32_t*)((char*)pPacket + 5);
        
        D2_ASSERT(pGame);

        D2UnitStrc* pCursorItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nCursorItemGUID);
        if (pCursorItem && pCursorItem->dwAnimMode == IMODE_ONCURSOR && INVENTORY_GetCursorItem(pUnit->pInventory) == pCursorItem)
        {
            int32_t a5 = 0;
            if (sub_6FC48940(pGame, pUnit, nCursorItemGUID, v7, 0, &a5) || !a5)
            {
                return 0;
            }

            FOG_Trace("sInsertItemBelt cheat detected %s\n", UNITS_GetPlayerData(pUnit)->szName);
            return 3;
        }
        
        return 1;
    }

    return 3;
}

//D2Game.0x6FC85F50
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x24_ItemFromBelt_6FC85F50(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 5)
    {
        return 3;
    }

    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);

    D2_ASSERT(pGame);

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem || pItem->dwAnimMode != IMODE_INBELT || !INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pItem))
    {
        return 1;
    }

    if (!sub_6FC7C3A0(pGame, pUnit, 0))
    {
        return 0;
    }

    if (INVENTORY_GetCursorItem(pUnit->pInventory))
    {
        return 2;
    }

    int32_t a5 = 0;
    if (sub_6FC48B40(pGame, pUnit, nItemGUID, &a5) || !a5)
    {
        return 0;
    }

    FOG_Trace("sRemoveItemBelt cheat detected%s\n", UNITS_GetPlayerData(pUnit)->szName);
    return 3;
}

//D2Game.0x6FC86040
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x25_SwitchBeltItem_6FC86040(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        const int32_t nCursorItemGUID = *(int32_t*)((char*)pPacket + 1);
        const int32_t nBeltItemGUID = *(int32_t*)((char*)pPacket + 5);

        D2_ASSERT(pGame);

        D2UnitStrc* pCursorItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nCursorItemGUID);
        if (pCursorItem && pCursorItem->dwAnimMode == IMODE_ONCURSOR && INVENTORY_GetCursorItem(pUnit->pInventory) == pCursorItem)
        {
            D2UnitStrc* pBeltItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nBeltItemGUID);
            if (pBeltItem && pBeltItem->dwAnimMode == IMODE_INBELT && INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pBeltItem))
            {
                int32_t a5 = 0;
                if (sub_6FC48D50(pGame, pUnit, nCursorItemGUID, nBeltItemGUID, &a5) || !a5)
                {
                    return 0;
                }

                FOG_Trace("sSwapItemBelt cheat detected for player %s\n", UNITS_GetPlayerData(pUnit)->szName);
                return 3;
            }
        }
        
        return 1;
    }

    return 3;
}

//D2Game.0x6FC86150
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x26_UseBeltItem_6FC86150(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt26* pPacket26 = (D2GSPacketClt26*)pPacket;

    if (nSize != sizeof(D2GSPacketClt26))
    {
        return 3;
    }
   
    D2_ASSERT(pGame);

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, pPacket26->nItemGUID);
    if (!pItem || pItem->dwAnimMode != IMODE_INBELT || !INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pItem))
    {
        return 1;
    }

    int32_t a6 = 0;
    if (sub_6FC49220(pGame, pUnit, pPacket26->nItemGUID, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), &a6, pPacket26->bUseOnMerc) || !a6)
    {
        return 0;
    }

    return 3;
}

//D2Game.0x6FC86310
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x28_SocketItem_6FC86310(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        const int32_t nCursorItemGUID = *(int32_t*)((char*)pPacket + 1);
        const int32_t nSocketableItemGUID = *(int32_t*)((char*)pPacket + 5);

        D2_ASSERT(pGame);

        D2UnitStrc* pCursorItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nCursorItemGUID);
        if (pCursorItem && pCursorItem->dwAnimMode == IMODE_ONCURSOR && INVENTORY_GetCursorItem(pUnit->pInventory) == pCursorItem)
        {
            D2UnitStrc* pSocketableItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nSocketableItemGUID);
            if (pSocketableItem && (pSocketableItem->dwAnimMode == IMODE_STORED || pSocketableItem->dwAnimMode == IMODE_EQUIP) && INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pSocketableItem))
            {
                int32_t nUnitType = 0;
                int32_t nUnitGUID = 0;
                if (SUNIT_GetInteractInfo(pUnit, &nUnitType, &nUnitGUID))
                {
                    D2UnitStrc* pInteractUnit = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
                    if (pInteractUnit && pInteractUnit->dwUnitType == UNIT_PLAYER)
                    {
                        return 0;
                    }
                    
                    int32_t bPlaced = 0;
                    if (D2GAME_ITEMSOCKET_PlaceItem_6FC497E0(pGame, pUnit, nCursorItemGUID, nSocketableItemGUID, &bPlaced, 1u, 1u, 1u) || !bPlaced)
                    {
                        return 0;
                    }
                }

                return 3;
            }
        }

        return 1;
    }

    return 3;
}

//D2Game.0x6FC86450
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x29_ScrollToBook_6FC86450(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    // TODO: v7, a5
    if (nSize == 9)
    {
        const int32_t v7 = *(int32_t*)((char*)pPacket + 1);
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 5);

        const int32_t nResult = sub_6FC86520(pGame, pUnit, v7);
        if (nResult)
        {
            return nResult;
        }

        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem && pItem->dwAnimMode == IMODE_STORED && INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pItem))
        {
            int32_t a5 = 0;
            if (sub_6FC49AE0(pGame, pUnit, v7, nItemGUID, &a5) || !a5)
            {
                return 0;
            }

            return 3;
        }

        return 1;
    }

    return 3;
}

//D2Game.0x6FC86520
int32_t __fastcall sub_6FC86520(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nItemGUID)
{
    D2_ASSERT(pGame);

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (pItem)
    {
        switch (pItem->dwAnimMode)
        {
        case IMODE_ONGROUND:
            if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pPlayer))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
            {
                return 2;
            }

            if (sub_6FC83C60(pPlayer, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 10))
            {
                return 1;
            }

            return 0;

        case IMODE_STORED:
        case IMODE_EQUIP:
        case IMODE_INBELT:
        case IMODE_ONCURSOR:
            if (!INVENTORY_CompareWithItemsParentInventory(pPlayer->pInventory, pItem) && INVENTORY_GetCursorItem(pPlayer->pInventory) != pItem)
            {
                return 1;
            }
            return 0;

        default:
            return 1;
        }
    }

    return 1;
}

//D2Game.0x6FC866E0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x2A_ItemToCubeIndirect_6FC866E0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    // TODO: v7, a5
    if (nSize == 9)
    {
        const int32_t v7 = *(int32_t*)((char*)pPacket + 1);
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 5);

        const int32_t nResult = sub_6FC86520(pGame, pUnit, v7);
        if (nResult)
        {
            return nResult;
        }

        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem && pItem->dwAnimMode == IMODE_STORED && INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pItem))
        {
            int32_t a5 = 0;
            if (sub_6FC49DC0(pGame, pUnit, v7, nItemGUID, &a5) || !a5)
            {
                return 0;
            }

            return 3;
        }

        return 1;
    }

    return 3;
}

//D2Game.0x6FC867B0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x2C_2D_52_6FC867B0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    return 3;
}

//D2Game.0x6FC867C0
void __fastcall D2GAME_PlayerChangeAct_6FC867C0(D2GameStrc* pGame, D2UnitStrc* pUnit, DWORD dwDestLvl, DWORD nTileCalc)
{
    D2RoomStrc* pRoom = UNITS_GetRoom(pUnit);
    const uint8_t nSourceAct = DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(pRoom));
    const uint8_t nDestAct = DRLG_GetActNoFromLevelId(dwDestLvl);

    if (dwDestLvl)
    {
        if (nDestAct == nSourceAct)
        {
            D2GAME_CreatePortalObject_6FD13DF0(pGame, pUnit, pRoom, CLIENTS_GetUnitX(pUnit) - 5, CLIENTS_GetUnitY(pUnit), dwDestLvl, 0, OBJECT_TOWN_PORTAL, 0);
        }
        else
        {
            LEVEL_WarpUnit(pGame, pUnit, dwDestLvl, nTileCalc);
        }
    }
    else
    {
        LEVEL_WarpUnit(pGame, pUnit, DUNGEON_GetTownLevelIdFromActNo(nSourceAct), nTileCalc);
    }
}

//D2Game.0x6FC868C0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x2F_InitEntityChat_6FC868C0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        D2UnitStrc* pMonster = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, *(int32_t*)((char*)pPacket + 5));
        if (pMonster)
        {
            if (pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData && pMonster->pMonsterData->pMonInteract)
            {
                if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) == DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pMonster))))
                {
                    const int32_t nResult = sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pMonster), CLIENTS_GetUnitY(pMonster), 10);
                    if (nResult)
                    {
                        return nResult;
                    }

                    sub_6FC61AB0(pGame, pUnit, pMonster, pMonster->pMonsterData->pMonInteract);
                    return 0;

                }
                
                return 2;
            }
            
            return 3;
        }
        
        return 1;
    }

    return 3;
}

//D2Game.0x6FC869F0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x30_TerminateEntityChat_6FC869F0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        D2UnitStrc* pMonster = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, *(int32_t*)((char*)pPacket + 5));
        if (pMonster)
        {
            if (pMonster->dwUnitType == UNIT_MONSTER && pMonster->pMonsterData && pMonster->pMonsterData->pMonInteract)
            {
                if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) == DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pMonster))))
                {
                    sub_6FC61B70(pGame, pUnit, pMonster, pMonster->pMonsterData->pMonInteract);
                    return 0;
                }
                
                return 2;
            }

            return 3;
        }
        
        return 1;
    }

    return 3;
}

//D2Game.0x6FC86AB0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x31_QuestMessage_6FC86AB0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        QUESTS_NPCMessage(pGame, pUnit, *(int32_t*)((char*)pPacket + 1), *(uint16_t*)((char*)pPacket + 5));
        return 0;
    }

    return 3;
}

//D2Game.0x6FC86AE0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x32_BuyItemFromNpcBuffer_6FC86AE0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 17)
    {
        D2GSPacketClt32* pPacket32 = (D2GSPacketClt32*)pPacket;
        return D2GAME_NPC_BuyItemHandler_6FCC92A0(pGame, pUnit, pPacket32->dwNpcGUID, pPacket32->dwItemGUID, pPacket32->nItemMode, pPacket32->nTransactionType, pPacket32->dwCost, pPacket32->bMultibuy);
    }

    return 3;
}

//D2Game.0x6FC86B30
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x33_SellItemToNpcBuffer_6FC86B30(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 17)
    {
        return D2GAME_STORES_SellItem_6FCC7680(pGame, pUnit, *(int32_t*)((char*)pPacket + 1), *(int32_t*)((char*)pPacket + 5), *(uint16_t*)((char*)pPacket + 9), *(int32_t*)((char*)pPacket + 13));
    }

    return 3;
}

//D2Game.0x6FC86B70
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x35_Repair_6FC86B70(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 17)
    {
        D2GAME_NPC_Repair_6FCC95B0(pGame, pUnit, *(int32_t*)((char*)pPacket + 1), *(int32_t*)((char*)pPacket + 5), *(int16_t*)((char*)pPacket + 9), *(int32_t*)((char*)pPacket + 13));
        return 0;
    }

    return 3;
}

//D2Game.0x6FC86BA0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x34_IdentifyItemsWithNpc_6FC86BA0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 5)
    {
        D2GAME_NPC_IdentifyAllItems_6FCC9C90(pGame, pUnit, *(int32_t*)((char*)pPacket + 1));
        return 0;
    }

    return 3;
}

//D2Game.0x6FC86BD0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x36_HireMerc_6FC86BD0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        D2GAME_NPC_HireMerc_6FCC9540(pGame, pUnit, *(int32_t*)((char*)pPacket + 1), *(uint16_t*)((char*)pPacket + 5));
        return 0;
    }

    return 3;
}

//D2Game.0x6FC86C00
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x62_ResurrectMerc_6FC86C00(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (pGame->bExpansion && nSize == 5)
    {
        D2GAME_NPC_ResurrectMerc_6FCC9350(pGame, pUnit, *(int32_t*)((char*)pPacket + 1));
        return 0;
    }

    return 3;
}

//D2Game.0x6FC86C30
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x37_IdentifyFromGamble_6FC86C30(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 5)
    {
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);
        if (SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID))
        {
            if (UNITS_GetPlayerData(pUnit)->dwBoughtItemId == nItemGUID)
            {
                D2GAME_NPC_IdentifyBoughtItem_6FCCA990(pGame, pUnit, nItemGUID);
                return 0;
            }

            FOG_Trace("identify cheat detected for player %s\n", UNITS_GetPlayerData(pUnit)->szName);
            return 3;
        }

        return 2;
    }

    return 3;
}

//D2Game.0x6FC86CB0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x38_EntityAction_6FC86CB0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt38* pPacket38 = (D2GSPacketClt38*)pPacket;
    if (nSize == 13)
    {
        const int32_t nNpcGUID = pPacket38->nNpcGUID;
        D2UnitStrc* pNpc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nNpcGUID);
        if (pNpc)
        {
            if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) == DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pNpc))))
            {
                if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pNpc), CLIENTS_GetUnitY(pNpc), 50))
                {
                    return 1;
                }

                return NPC_HandleDialogMessage(pGame, pUnit, pPacket38->nAction, nNpcGUID, pPacket38->nItemGUID);
            }

            return 2;
        }

        return 1;
    }

    return 3;
}

//D2Game.0x6FC86E50
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x39_PurchaseLife_6FC86E50(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 5)
    {
        D2GAME_NPC_PurchaseHeal_6FCCB280(pGame, pUnit, *(int32_t*)((char*)pPacket + 1));
        return 0;
    }

    return 3;
}

//D2Game.0x6FC86E80
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x3A_AddStatPoint_6FC86E80(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt3A* pPacket3A = (D2GSPacketClt3A*)pPacket;

    if (nSize != 3)
    {
        return 3;
    }
    
    const int32_t nStatId = (uint8_t)pPacket3A->nStat;
    int32_t nCount = (pPacket3A->nStat >> 8) + 1;
    
    if (nStatId <= -1 || nStatId >= 16 || nCount < 1 || nCount > 100)
    {
        return 3;
    }

    while (nCount)
    {
        --nCount;

        if (!PLAYERSTATS_SpendStatPoint(pUnit, nStatId))
        {
            return 2;
        }
    }

    return 0;
}

//D2Game.0x6FC86EF0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x3B_AddSkillPoint_6FC86EF0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt3B* pPacket3B = (D2GSPacketClt3B*)pPacket;
    if (nSize == 3)
    {
        const int32_t nSkillId = *(int16_t*)((char*)pPacket + 1);
        if (nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
        {
            D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
            D2GAME_PACKETS_SendPacket0x21_UpdateSkills_6FC3DB50(pClient, pUnit, 0, 1u, 0);
            return 2;
        }

        if (D2GAME_CheckSkillIfUnitsClassSkill_6FD11B50(pUnit, nSkillId) && SKILLS_CheckRequiredSkills(pUnit, nSkillId) && SKILLS_CheckRequiredAttributes(pUnit, nSkillId))
        {
            D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, -1u);
            if (pSkill)
            {
                int32_t nMaxLvl = sgptDataTables->pSkillsTxt[nSkillId].wMaxLvl;
                if (nMaxLvl <= 0)
                {
                    nMaxLvl = 20;
                }

                if (SKILLS_GetSkillLevel(pUnit, pSkill, 0) >= nMaxLvl)
                {
                    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
                    D2GAME_PACKETS_SendPacket0x21_UpdateSkills_6FC3DB50(pClient, pUnit, 0, 1u, 0);
                    return 2;
                }
            }

            if (sub_6FD136E0(pUnit, nSkillId))
            {
                //D2Game_10034_Return(1);
                sub_6FD14C30(pUnit);
                return 0;
            }

            return 2;
        }
    }

    return 3;
}

//D2Game.0x6FC87050
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x3C_SelectSkill_6FC87050(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        const int32_t nOwnerGUID = *(int32_t*)((char*)pPacket + 5);
        const int32_t bLeftSkill = *(int32_t*)((char*)pPacket + 1) >> 31;
        const int32_t nSkillId = *(int32_t*)((char*)pPacket + 1) & 0x7FFFFFFF;
        if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
        {
            D2SkillStrc* pSkill = SKILLS_GetSkillById(pUnit, nSkillId, nOwnerGUID);
            if (pSkill && SKILLS_GetSkillLevel(pUnit, pSkill, 1))
            {
                D2GAME_AssignSkill_6FD13800(pUnit, bLeftSkill, nSkillId, nOwnerGUID);
                if (bLeftSkill)
                {
                    UNITS_InitLeftSkill(pUnit);
                }
                else
                {
                    UNITS_InitRightSkill(pUnit);
                }

                return 0;
            }
        }
    }

    return 3;
}

//D2Game.0x6FC870F0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x3D_HighlightADoor_6FC870F0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 5)
    {
        const int32_t nObjectGUID = *(int32_t*)((char*)pPacket + 1);
        D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, nObjectGUID);
        if (pObject)
        {
            if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) == DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pObject))))
            {
                if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pObject), CLIENTS_GetUnitY(pObject), 10))
                {
                    return 1;
                }

                sub_6FC78BB0(pGame, 0, nObjectGUID);

                return 0;
            }

            return 2;
        }

        return 1;
    }

    return 3;
}

//D2Game.0x6FC87270
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x3E_ActivateInifussScroll_6FC87270(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 5)
    {
        return 3;
    }

    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);
    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem)
    {
        return 1;
    }

    if (!ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }

        if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 10))
        {
            return 1;
        }
    }

    QUESTS_UseClueItem(pGame, pUnit, nItemGUID);
    return 0;
}

//D2Game.0x6FC87410
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x3F_PlayAudio_6FC87410(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 3)
    {
        const int32_t nSoundId = *(int16_t*)((char*)pPacket + 1);
        if (nSoundId >= 25 && nSoundId <= 32)
        {
            if (pUnit && pUnit->dwFlags & UNITFLAG_HASEVENTSOUND)
            {
                return 1;
            }

            SUNIT_AttachSound(pUnit, nSoundId, 0);
        }

        return 0;
    }

    return 3;
}

//D2Game.0x6FC87460
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x40_RequestQuestData_6FC87460(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 1)
    {
        QUESTS_StatusCallback(pGame, pUnit);
        return 0;
    }

    return 3;
}

//D2Game.0x6FC87480
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x41_Resurrect_6FC87480(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 1)
    {
        return 3;
    }

    if (pUnit && pUnit->dwAnimMode == PLRMODE_DEAD)
    {
        D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
        if (CLIENTS_CheckFlag(pClient, CLIENTSAVEFLAG_HARDCORE))
        {
            GAME_DisconnectClient(pGame, pClient, EVENTTYPE_PLAYERLEFT);
            return 0;
        }

        const int32_t n0 = ARENA_Return0();
        D2GAME_RefreshPassiveSkills_6FD14CC0(pUnit);

        const int32_t nMaxLife = STATLIST_GetMaxLifeFromUnit(pUnit);
        const int32_t nMaxMana = STATLIST_GetMaxManaFromUnit(pUnit);
        const int32_t nMaxStamina = STATLIST_GetMaxStaminaFromUnit(pUnit);
        STATLIST_SetUnitStat(pUnit, STAT_HITPOINTS, nMaxLife, 0);
        STATLIST_SetUnitStat(pUnit, STAT_MANA, nMaxMana, 0);
        STATLIST_SetUnitStat(pUnit, STAT_STAMINA, nMaxStamina, 0);

        D2GAME_PACKETS_SendPacket0x1D_E_F_6FC3D480(pClient, STAT_HITPOINTS, nMaxLife);
        D2GAME_PACKETS_SendPacket0x1D_E_F_6FC3D480(pClient, STAT_MANA, nMaxMana);
        D2GAME_PACKETS_SendPacket0x1D_E_F_6FC3D480(pClient, STAT_STAMINA, nMaxStamina);

        pUnit->dwFlags |= UNITFLAG_TARGETABLE;

        STATES_ToggleState(pUnit, STATE_UNINTERRUPTABLE, 1);
        STATES_ToggleState(pUnit, STATE_UNINTERRUPTABLE, 0);

        D2GAME_PlayerChangeAct_6FC867C0(pGame, pUnit, 0, n0);

        sub_6FC817D0(pGame, pUnit, 0, 1, 0, 0, 1);

        D2SkillStrc* pLeftSkill = UNITS_GetLeftSkill(pUnit);
        if (pLeftSkill)
        {
            const int32_t nLeftSkillFlags = SKILLS_GetOwnerGUIDFromSkill(pLeftSkill);
            const int32_t nLeftSkillId = SKILLS_GetSkillIdFromSkill(pLeftSkill, __FILE__, __LINE__);
            D2GAME_AssignSkill_6FD13800(pUnit, 1, nLeftSkillId, nLeftSkillFlags);
        }

        D2SkillStrc* pRightSkill = UNITS_GetRightSkill(pUnit);
        if (pRightSkill)
        {
            const int32_t nRightSkillFlags = SKILLS_GetOwnerGUIDFromSkill(pRightSkill);
            const int32_t nRightSkillId = SKILLS_GetSkillIdFromSkill(pRightSkill, __FILE__, __LINE__);
            D2GAME_AssignSkill_6FD13800(pUnit, 0, nRightSkillId, nRightSkillFlags);
        }
    }

    return 0;
}

//D2Game.0x6FC876B0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x5D_SquelchHostile_6FC876B0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 7 && !ARENA_IsActive(pGame))
    {
        const uint8_t nCommand = *((uint8_t*)pPacket + 1);
        if (nCommand <= 10u)
        {
            return PARTYSCREEN_CommandHandler(pGame, pUnit, nCommand, *(int32_t*)((char*)pPacket + 3), *((uint8_t*)pPacket + 2));
        }

        return 2;
    }

    return 3;
}

//D2Game.0x6FC87720
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x5E_InviteToParty_6FC87720(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 6 && !ARENA_IsActive(pGame))
    {
        const uint8_t nCommand = *((uint8_t*)pPacket + 1);
        if (nCommand <= 10u)
        {
            PARTYSCREEN_CommandHandler(pGame, pUnit, nCommand, *(int32_t*)((char*)pPacket + 2), 0);
            return 0;
        }

        return 2;
    }

    return 3;
}

//D2Game.0x6FC87780
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x44_StaffInOrifice_6FC87780(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt44* pPacket44 = (D2GSPacketClt44*)pPacket;
    if (nSize != 17)
    {
        return 3;
    }

    if (PLAYER_IsBusy(pUnit) && sub_6FC937A0(pGame, pUnit))
    {
        return 3;
    }

    D2UnitStrc* pObject = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pPacket44->nAnvilInteractGUID);
    if (!pObject)
    {
        return 1;
    }

    if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pObject))))
    {
        return 2;
    }

    const int32_t nResult = sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pObject), CLIENTS_GetUnitY(pObject), 50);
    if (nResult)
    {
        return nResult;
    }

    if (pPacket44->nState != 2)
    {
        if (pPacket44->nState != 3)
        {
            return 2;
        }

        D2_ASSERT(pGame);

        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, pPacket44->nItemGUID);
        if (!pItem || pItem->dwAnimMode != IMODE_ONCURSOR || INVENTORY_GetCursorItem(pUnit->pInventory) != pItem)
        {
            return 1;
        }
    }

    OBJMODE_MainObeliskHandler(pGame, pUnit ? pUnit->dwUnitId : -1, pPacket44->nAnvilInteractGUID, pPacket44->nItemGUID, pPacket44->nState);
    return 0;
}

//D2Game.0x6FC87930
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x45_ChangeTpLocation_6FC87930(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 9)
    {
        return 3;
    }

    D2GSPacketClt45* pPacket45 = (D2GSPacketClt45*)pPacket;

    const int32_t nPortalGUID = pPacket45->nPortalGUID;
    const uint16_t nLevelId = pPacket45->nLevelId;
    D2UnitStrc* pPortal = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, nPortalGUID);
    if (pPortal)
    {
        if (pPortal->dwClassId != OBJECT_TOWN_PORTAL || nLevelId >= sgptDataTables->nLevelsTxtRecordCount || !nLevelId)
        {
            return 3;
        }

        if (UNITS_GetPlayerPortalFlags(pUnit) & DUNGEON_GetPortalFlagFromLevelId(nLevelId))
        {
            if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) == DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pPortal))))
            {
                if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pPortal), CLIENTS_GetUnitY(pPortal), 10))
                {
                    return 1;
                }

                sub_6FC79C30(pGame, 0, nPortalGUID, nLevelId);
                return 0;
            }
        }

        return 2;
    }

    return 1;
}

//D2Game.0x6FC87B00
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x46_MercInteract_6FC87B00(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 13)
    {
        return 3;
    }

    const int32_t nMercGUID = *(int32_t*)((char*)pPacket + 1);
    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 5);
    const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 9);
    if (nUnitType >= 6)
    {
        return 2;
    }

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, nUnitType, nItemGUID);
    if (!pItem)
    {
        return 1;
    }

    if (nUnitType != UNIT_ITEM || !ITEMS_IsInPlayersInventory(pUnit, pItem, 0))
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) != DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pItem))))
        {
            return 2;
        }


        if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), 50))
        {
            return 1;
        }
    }

    D2UnitStrc* pMerc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nMercGUID);
    if (!pMerc || sub_6FC7E8B0(pGame, pUnit, 7, 0) != pMerc)
    {
        return 1;
    }

    AIGENERAL_FreeAllAiCommands(pGame, pMerc);
    D2AiCmdStrc aiCmd = {};
    aiCmd.nCmdParam[0] = 12;
    aiCmd.nCmdParam[1] = nItemGUID;
    aiCmd.nCmdParam[2] = nUnitType;
    AIGENERAL_CopyAiCommand(pGame, pMerc, &aiCmd);

    SUNIT_AttachSound(pMerc, 0xFu, pUnit);

    return 0;
}

//D2Game.0x6FC87CE0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x47_MoveMerc_6FC87CE0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 13)
    {
        const uint16_t nX = *(uint16_t*)((char*)pPacket + 5);
        const uint16_t nY = *(uint16_t*)((char*)pPacket + 9);
        const int32_t nMercGUID = *(int32_t*)((char*)pPacket + 1);

        if (sub_6FC83C60(pUnit, nX, nY, 50))
        {
            return 1;
        }

        D2UnitStrc* pMerc = SUNIT_GetServerUnit(pGame, UNIT_MONSTER, nMercGUID);
        if (pMerc && sub_6FC7E8B0(pGame, pUnit, 7, 0) == pMerc)
        {
            AIGENERAL_FreeAllAiCommands(pGame, pMerc);
            D2AiCmdStrc aiCmd = {};
            aiCmd.nCmdParam[0] = 13;
            aiCmd.nCmdParam[1] = nX;
            aiCmd.nCmdParam[2] = nY;
            AIGENERAL_CopyAiCommand(pGame, pMerc, &aiCmd);
            SUNIT_AttachSound(pMerc, 0xFu, pUnit);
        }

        return 0;
    }

    return 3;
}

//D2Game.0x6FC87E20
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x48_TurnOffBusyState_6FC87E20(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 1)
    {
        if (UNITS_GetPlayerData(pUnit)->bBusy)
        {
            PLAYER_ResetBusyState(pUnit);
            return 0;
        }

        return 2;
    }

    return 3;
}

//D2Game.0x6FC87E60
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x49_TakeOrCloseWp_6FC87E60(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nPacketSize)
{
    D2GSPacketClt49* pPacket49 = (D2GSPacketClt49*)pPacket;
    if (nPacketSize != 9)
    {
        return 3;
    }

    D2UnitStrc* pWaypoint = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, pPacket49->nWaypointGUID);
    int32_t nResult = 0;
    if (pWaypoint)
    {
        if (DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit))) == DRLG_GetActNoFromLevelId(DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pWaypoint))))
        {
            int32_t nMaxDistance = 10;
            if (pUnit && pUnit->dwClassId == PCLASS_SORCERESS)
            {
                nMaxDistance = 22;
            }


            if (sub_6FC83C60(pUnit, CLIENTS_GetUnitX(pWaypoint), CLIENTS_GetUnitY(pWaypoint), nMaxDistance))
            {
                return 1;
            }

            if (!pPacket49->nLevelId)
            {
                D2GAME_WAYPOINT_Unk_6FC79600(pGame, pUnit, pPacket49->nWaypointGUID, pPacket49->nLevelId);
                return 0;
            }

            if (pPacket49->nLevelId >= sgptDataTables->nLevelsTxtRecordCount)
            {
                nResult = 3;
            }
            else
            {
                D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit);

                D2_ASSERT(pPlayerData);

                int16_t nWaypointNo = 0;
                if (!WAYPOINTS_GetWaypointNoFromLevelId(pPacket49->nLevelId, &nWaypointNo))
                {
                    nResult = 3;
                }
                else
                {
                    if (WAYPOINTS_IsActivated(pPlayerData->pWaypointData[pGame->nDifficulty], nWaypointNo))
                    {
                        D2GAME_WAYPOINT_Unk_6FC79600(pGame, pUnit, pPacket49->nWaypointGUID, pPacket49->nLevelId);
                        return 0;
                    }

                    nResult = 2;
                }
            }
        }
        else
        {
            nResult = 2;
        }
    }
    else
    {
        nResult = 1;
    }

    if (pWaypoint && pWaypoint == SUNIT_GetInteractUnit(pGame, pUnit))
    {
        SUNIT_ResetInteractInfo(pUnit);
    }

    return nResult;
}

//D2Game.0x6FC880A0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x4D_PlayNpcMessage_6FC880A0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 3)
    {
        uint16_t nMonsterId = *(uint16_t*)((char*)pPacket + 1);
        if (nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
        {
            PLRINTRO_MaskNpcIntroFlag(pGame, pUnit, nMonsterId);
            return 0;
        }
        
        return 2;
    }

    return 3;
}

//D2Game.0x6FC880F0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x4B_RequestEntityUpdate_6FC880F0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 9)
    {
        const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 1);
        if (nUnitType < 6)
        {
            D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, nUnitType, *(int32_t*)((char*)pPacket + 5));
            if (pUnit && UNITFINDS_AreUnitsInNeighboredRooms(pUnit, pUnit))
            {
                UNITROOM_RefreshUnit(pUnit);
                pUnit->dwFlagEx |= UNITFLAGEX_TELEPORTED;
                return 0;
            }
            
            return 1;
        }
        
        return 2;
    }

    return 3;
}

//D2Game.0x6FC88170
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x4C_Transmogrify_6FC88170(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 5)
    {
        const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);
        D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
        if (pItem || nItemGUID == -1)
        {
            sub_6FD11AE0(pGame, pUnit, pItem);
            return 0;
        }
        
        return 1;
    }

    return 3;
}

//D2Game.0x6FC881D0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x4F_ClickButton_6FC881D0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 7)
    {
        return sub_6FC91250(pGame, pUnit, *(uint16_t*)((char*)pPacket + 1), *(uint16_t*)((char*)pPacket + 5) | (*(uint16_t*)((char*)pPacket + 3) << 16));
    }

    return 3;
}

//D2Game.0x6FC88210
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x50_DropGold_6FC88210(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 9)
    {
        return 3;
    }

    const int32_t nUnitGUID = *(int32_t*)((char*)pPacket + 1);
    const int32_t nGoldValue = *(int32_t*)((char*)pPacket + 5);

    if (!PLAYER_IsBusy(pUnit) || !sub_6FC937A0(pGame, pUnit))
    {
        if (sub_6FC7C260(pGame, pUnit, nUnitGUID, nGoldValue))
        {
            return 0;
        }

        return 3;
    }

    FOG_Trace("Player %s should be banned for attempting to drop gold\n", UNITS_GetPlayerData(pUnit)->szName);
    return 3;
}

//D2Game.0x6FC88290
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x51_BindHotkeyToSkill_6FC88290(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt51* pPacket51 = (D2GSPacketClt51*)pPacket;

    if (nSize == 9)
    {
        const uint32_t nHotKeyId = (uint32_t)pPacket51->nSkillId >> 16;
        if (nHotKeyId >= 0 && nHotKeyId < 16)
        {
            const int16_t nSkillId = pPacket51->nSkillId & 0x7FFF;
            if (nSkillId > sgptDataTables->nSkillsTxtRecordCount || nSkillId == -1)
            {
                if (sub_6FC7C450(pGame, pUnit, nHotKeyId, -1, ((uint32_t)pPacket51->nSkillId >> 15) & 1, pPacket51->nOwnerGUID))
                {
                    return 0;
                }

                return 3;
            }

            if (nSkillId < sgptDataTables->nSkillsTxtRecordCount && SKILLS_GetSkillById(pUnit, pPacket51->nSkillId & 0x7FFF, pPacket51->nOwnerGUID))
            {
                if (sub_6FC7C450(pGame, pUnit, nHotKeyId, nSkillId, ((uint32_t)pPacket51->nSkillId >> 15) & 1, pPacket51->nOwnerGUID))
                {
                    return 0;
                }

                return 3;
            }
        }
    }

    return 3;
}

//D2Game.0x6FC88340
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x53_TurnStaminaOn_6FC88340(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 1)
    {
        if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && pUnit->dwAnimMode == PLRMODE_WALK)
        {
            if (STATLIST_UnitGetStatValue(pUnit, STAT_STAMINA, 0))
            {
                UNITS_ChangeAnimMode(pUnit, PLRMODE_RUN);
            }
        }

        return 0;
    }

    return 3;
}

//D2Game.0x6FC88380
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x54_TurnStaminaOff_6FC88380(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 1)
    {
        if (pUnit && pUnit->dwUnitType == UNIT_PLAYER && pUnit->dwAnimMode == PLRMODE_RUN)
        {
            UNITS_ChangeAnimMode(pUnit, PLRMODE_WALK);
        }

        return 0;
    }

    return 3;
}

//D2Game.0x6FC883B0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x58_QuestCompleted_6FC883B0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 3)
    {
        return 3;
    }

    const uint16_t nQuestId = *(uint16_t*)((char*)pPacket + 1);
    if (nQuestId >= 41)
    {
        return 2;
    }

    QUESTRECORD_SetQuestState(UNITS_GetPlayerData(pUnit)->pQuestData[pGame->nDifficulty], nQuestId, QFLAG_UPDATEQUESTLOG);
    return 0;
}

//D2Game.0x6FC88400
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x59_MakeEntityMove_6FC88400(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 17)
    {
        const uint32_t nUnitType = *(uint32_t*)((char*)pPacket + 1);
        const int32_t nUnitGUID = *(int32_t*)((char*)pPacket + 5);
        const int32_t v8 = *(int32_t*)((char*)pPacket + 9);
        const int32_t a3 = *(int32_t*)((char*)pPacket + 13);

        if (nUnitType < 6)
        {
            D2UnitStrc* pNpc = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
            if (pNpc && D2Common_10399(pUnit, pNpc) <= 50)
            {
                D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pNpc->dwClassId);
                if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT] && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NPC])
                {
                    if (pNpc->pDynamicPath)
                    {
                        D2Common_10153(pNpc->pDynamicPath);
                    }

                    D2GAME_EVENTS_Delete_6FC34840(pGame, pNpc, 2, 0);
                    EVENT_SetEvent(pGame, pNpc, 2, pGame->dwGameFrame + 1, 0, 0);
                    AIGENERAL_SetAiControlParam(pNpc, 1, 40);
                    AIGENERAL_SetAiControlParam(pNpc, 2, v8);
                    AIGENERAL_SetAiControlParam(pNpc, 3, a3);
                    return 0;
                }
            }
            
            return 1;
        }
        
        return 2;
    }

    return 3;
}

//D2Game.0x6FC88530
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x5F_UpdatePlayerPos_6FC88530(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2GSPacketClt5F* pPacket5F = (D2GSPacketClt5F*)pPacket;

    if (nSize != sizeof(D2GSPacketClt5F))
    {
        return 3;
    }

    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__);
    D2_ASSERT(pClient);

    if (UNITS_GetUsedSkill(pUnit) || SUNIT_IsDead(pUnit))
    {
        return 0;
    }

    const int32_t nDistance = UNITS_GetDistanceToCoordinates(pUnit, pPacket5F->nX, pPacket5F->nY);
    if (nDistance < 5)
    {
        return 0;
    }

    if (STATES_CheckState(pUnit, STATE_SYNC_WARPED) || nDistance < 15 || nDistance > 45)
    {
        if (!SUNIT_GetTargetUnit(pGame, pUnit))
        {
            if (pUnit)
            {
                sub_6FC817D0(pGame, pUnit, 0, (pUnit->dwAnimMode == 3) + 2, pPacket5F->nX, pPacket5F->nY, 0);
            }
            else
            {
                sub_6FC817D0(pGame, pUnit, 0, 2, pPacket5F->nX, pPacket5F->nY, 0);
            }
        }
        return 0;
    }

    if (!D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), pPacket5F->nX, pPacket5F->nY) || STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE) || STATES_CheckState(pUnit, STATE_SYNC_WARPED))
    {
        D2GAME_PACKETS_SendPacket0x15_6FC3D0D0(pClient, 0x15u, pUnit->dwUnitType, pUnit->dwUnitId, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 0);
        return 0;
    }

    // TODO: a2
    const int32_t a2 = PATH_GetMoveTestCollisionMask(pUnit->pDynamicPath);
    const int32_t nPathType = PATH_GetType(pUnit->pDynamicPath);
    const uint8_t nNewDistance = PATH_GetMaxDistance(pUnit->pDynamicPath);
    D2UnitStrc* pTargetUnit = SUNIT_GetTargetUnit(pGame, pUnit);

    const int32_t nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
    const int32_t nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);

    PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, COLLIDE_BLOCK_PLAYER | COLLIDE_BLOCK_LEAP | COLLIDE_OBJECT);
    PATH_SetType(pUnit->pDynamicPath, 15);
    PATH_SetNewDistance(pUnit->pDynamicPath, 77);
    D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, pPacket5F->nX, pPacket5F->nY);

    int32_t bAtSameCoordinates = D2Common_10142(pUnit->pDynamicPath, pUnit, 0);
    if (bAtSameCoordinates)
    {
        if (pPacket5F->nX == D2COMMON_10177_PATH_GetLastPointX(pUnit->pDynamicPath) && pPacket5F->nY == D2COMMON_10178_PATH_GetLastPointY(pUnit->pDynamicPath))
        {
            bAtSameCoordinates = 1;
        }
        else
        {
            bAtSameCoordinates = 0;
        }
    }

    PATH_SetMoveTestCollisionMask(pUnit->pDynamicPath, a2);
    PATH_SetType(pUnit->pDynamicPath, nPathType);
    PATH_SetNewDistance(pUnit->pDynamicPath, nNewDistance);

    if (pTargetUnit)
    {
        PATH_SetTargetUnit(pUnit->pDynamicPath, pTargetUnit);
    }
    else
    {
        D2COMMON_10170_PathSetTargetPos(pUnit->pDynamicPath, nX, nY);
    }

    if (!bAtSameCoordinates || !sub_6FCBDFE0(pGame, pUnit, 0, pPacket5F->nX, pPacket5F->nY, 0, 1))
    {
        D2GAME_PACKETS_SendPacket0x15_6FC3D0D0(pClient, 0x15u, pUnit->dwUnitType, pUnit->dwUnitId, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 0);
        return 0;
    }

    sub_6FC34700(pGame, pUnit);

    int32_t nDelay = 125;
    if (sub_6FC347A0(pGame, pUnit) && !pGame->nGameType)
    {
        constexpr int32_t dword_6FD28F80[][2] =
        {
            { 50, 1500 },
            { 25, 3000 },
            { 25, 4500 },
        };

        int32_t nRand = ITEMS_RollRandomNumber(&pUnit->pSeed) % 100;
        for (size_t i = 0; i < std::size(dword_6FD28F80); ++i)
        {
            const int32_t nProbability = dword_6FD28F80[i][0];
            if (nRand < nProbability)
            {
                nDelay = dword_6FD28F80[i][1];
                break;
            }

            nRand -= nProbability;
        }
    }

    STATES_ToggleState(pUnit, STATE_SYNC_WARPED, 1);

    D2StatListStrc* pStatList = STATLIST_AllocStatList(pGame->pMemoryPool, 2u, pGame->dwGameFrame + nDelay, pUnit->dwUnitType, pUnit->dwUnitId);
    STATLIST_SetState(pStatList, STATE_SYNC_WARPED);
    STATLIST_SetStatRemoveCallback(pStatList, MISSMODE_ToggleStateOff);
    D2COMMON_10475_PostStatToStatList(pUnit, pStatList, 1);

    EVENT_SetEvent(pGame, pUnit, UNITEVENTCALLBACK_REMOVESTATE, pGame->dwGameFrame + nDelay, 0, 0);
    return 0;
}

//D2Game.0x6FC888A0
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x60_SwapWeapons_6FC888A0(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize == 1 && pGame->bExpansion)
    {
        if (UNITS_GetUsedSkill(pUnit) || SUNIT_IsDead(pUnit))
        {
            return 0;
        }
        
        int32_t a3 = 0;
        if (sub_6FC46840(pGame, pUnit, &a3) || !a3)
        {
            return 0;
        }

        FOG_Trace("sWeaponSwap Cheat detected for player %s\n", UNITS_GetPlayerData(pUnit)->szName);
    }

    return 3;
}

//D2Game.0x6FC88930
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x61_DropPickupMercItem_6FC88930(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    if (nSize != 3 || !pGame->bExpansion)
    {
        return 3;
    }

    const int16_t nBodyLoc = *(int16_t*)((char*)pPacket + 1);

    if (PLAYER_IsBusy(pUnit) && sub_6FC937A0(pGame, pUnit))
    {
        FOG_Trace("Player %s should be banned\n", UNITS_GetPlayerData(pUnit)->szName);
        return 3;
    }

    if (UNITS_GetUsedSkill(pUnit) || SUNIT_IsDead(pUnit))
    {
        return 0;
    }
    
    D2UnitStrc* pMerc = sub_6FC7E8B0(pGame, pUnit, 7, 0);
    if (!pMerc || SUNIT_IsDead(pMerc) || !UNITFINDS_AreUnitsInNeighboredRooms(pMerc, pUnit))
    {
        return 0;
    }

    if (!pUnit->pInventory)
    {
        return 3;
    }

    D2UnitStrc* pItem = INVENTORY_GetCursorItem(pUnit->pInventory);
    if (pItem)
    {
        if (ITEMS_IsQuestItem(pItem) || !pGame->bExpansion 
            || !ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, __FILE__) 
            || ITEMS_CheckItemFlag(pItem, IFLAG_BROKEN, __LINE__, __FILE__))
        {
            return 0;
        }

        if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_HEALING_POTION) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_THAWING_POTION) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ANTIDOTE_POTION))
        {
            SKILLITEM_pSpell_Handler(pGame, pMerc, pItem, pItem, 0, 0);
            D2GAME_RemoveItemIfOnCursor_6FC49760(pGame, pUnit, pItem);
            INVENTORY_SetCursorItem(pUnit->pInventory, 0);
            return 0;
        }

        int32_t bCanBeEquipped = 0;
        if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_ARMOR) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_HELM))
        {
            bCanBeEquipped = 1;
        }
        else
        {
            switch (pMerc->dwClassId)
            {
            case MONSTER_ROGUEHIRE:
                if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_BOW))
                {
                    bCanBeEquipped = 1;
                }
                break;

            case MONSTER_ACT2HIRE:
                if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SPEAR) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_POLEARM))
                {
                    bCanBeEquipped = 1;
                }
                break;

            case MONSTER_ACT3HIRE:
                if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SHIELD) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SWORD) && !ITEMS_CheckWeaponIfTwoHanded(pItem))
                {
                    bCanBeEquipped = 1;
                }
                break;

            case MONSTER_ACT5HIRE1:
                if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_AXE) && !ITEMS_CheckWeaponIfTwoHanded(pItem) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_PRIMAL_HELM))
                {
                    bCanBeEquipped = 1;
                }
                break;

            case MONSTER_ACT5HIRE2:
                if (ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SWORD) || ITEMS_CheckItemTypeId(pItem, ITEMTYPE_PRIMAL_HELM))
                {
                    bCanBeEquipped = 1;
                }
                break;

            default:
                break;
            }
        }

        if (!bCanBeEquipped || !ITEMS_CheckRequirements(pItem, pMerc, 0, nullptr, nullptr, nullptr))
        {
            SUNIT_AttachSound(pMerc, 0x55u, pUnit);
        }
        else if (D2GAME_MERCS_EquipItem_6FC88D10(pGame, pUnit, pMerc, pItem))
        {
            SUNIT_AttachSound(pMerc, 0x57u, pUnit);
        }
        else
        {
            SUNIT_AttachSound(pMerc, 0x56u, pUnit);
        }

        return 0;
    }

    if (nBodyLoc == BODYLOC_NONE)
    {
        return 0;
    }

    if (!pGame->bExpansion || !pMerc->pInventory || nBodyLoc <= 0 || nBodyLoc >= 11)
    {
        return 3;
    }

    D2UnitStrc* pEquippedItem = INVENTORY_GetItemFromBodyLoc(pMerc->pInventory, nBodyLoc);
    if (pEquippedItem && pEquippedItem->dwAnimMode == IMODE_EQUIP && pEquippedItem == INVENTORY_RemoveItemFromInventory(pMerc->pInventory, pEquippedItem))
    {
        //D2COMMON_10254_Return0(pMerc->pInventory, nBodyLoc);
        D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pEquippedItem, pMerc, 0, 0);
        ITEMS_SetItemCMDFlag(pEquippedItem, 16, 1);
        INVENTORY_AddItemToTradeInventory(pMerc->pInventory, pEquippedItem);
        UNITS_RefreshInventory(pMerc, 1);
        D2UnitStrc* pCursorItem = ITEMS_Duplicate(pGame, pEquippedItem, pUnit, 1);
        INVENTORY_SetCursorItem(pUnit->pInventory, pCursorItem);
        sub_6FC4B9D0(pGame, pUnit, pCursorItem);
        ITEMS_SetItemFlag(pEquippedItem, IFLAG_QUANTITY, 1);
        D2GAME_ITEMS_UpdateInventoryItems_6FC45050(pGame, pMerc, 0, 0);
        //D2Game_10034_Return(0);
        return 0;
    }

    return 2;
}

//D2Game.0x6FC88D10
int32_t __fastcall D2GAME_MERCS_EquipItem_6FC88D10(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pMerc, D2UnitStrc* pItem)
{
    if (!pGame->bExpansion)
    {
        return 3;
    }

    if (!pPlayer->pInventory)
    {
        return 0;
    }

    if (!pMerc->pInventory)
    {
        INVENTORY_AllocInventory(nullptr, pMerc);
    }

    D2InventoryStrc* pMercInventory = pMerc->pInventory;
    if (!pMercInventory)
    {
        return 0;
    }

    uint8_t nBodyLoc1 = 0;
    uint8_t nBodyLoc2 = 0;
    ITEMS_GetAllowedBodyLocations(pItem, &nBodyLoc1, &nBodyLoc2);

    D2UnitStrc* pExchangeItem = INVENTORY_GetItemFromBodyLoc(pMercInventory, nBodyLoc1);
    D2UnitStrc* pTempItem = INVENTORY_GetItemFromBodyLoc(pMercInventory, nBodyLoc2);

    uint8_t nTargetBodyLoc = nBodyLoc1;
    if (pMerc->dwClassId == MONSTER_ACT3HIRE && ITEMS_CheckItemTypeId(pItem, ITEMTYPE_SHIELD))
    {
        pExchangeItem = pTempItem;
        nTargetBodyLoc = nBodyLoc2;
    }

    if (pExchangeItem)
    {
        if (pExchangeItem != INVENTORY_RemoveItemFromInventory(pMercInventory, pExchangeItem))
        {
            return 0;
        }

        //D2COMMON_10254_Return0(pMercInventory, nTargetBodyLoc);
        D2GAME_ITEMS_UpdateTransferredProperties_6FC424E0(pGame, pExchangeItem, pMerc, 0, 0);
        D2GAME_ITEMS_UpdateInventoryItems_6FC45050(pGame, pMerc, 0, 0);

        if (!ITEMS_CheckRequirements(pItem, pMerc, 0, nullptr, nullptr, nullptr))
        {
            INVENTORY_PlaceItemInBodyLoc(pMercInventory, pExchangeItem, nTargetBodyLoc);
            INVENTORY_PlaceItemInSocket(pMercInventory, pExchangeItem, 3);
            ITEMS_SetBodyLocation(pExchangeItem, nTargetBodyLoc);
            UNITS_ChangeAnimMode(pExchangeItem, IMODE_EQUIP);
            ITEMS_SetInvPage(pExchangeItem, 0xFFu);
            D2GAME_ITEMS_UpdateItemStatlist_6FC42310(pGame, pExchangeItem, pMerc, 0);
            //D2Game_10034_Return(0);
            return 0;
        }

        ITEMS_SetItemCMDFlag(pExchangeItem, 16, 1);
        INVENTORY_AddItemToTradeInventory(pMercInventory, pExchangeItem);
        ITEMS_SetItemFlag(pExchangeItem, IFLAG_QUANTITY, 1);
        UNITS_RefreshInventory(pMerc, 1);
    }

    D2UnitStrc* pDupeItem = ITEMS_Duplicate(pGame, pItem, pMerc, 1);
    UNITS_ChangeAnimMode(pDupeItem, IMODE_ONCURSOR);

    int32_t nUnused = 0;
    sub_6FC45060(pGame, pMerc, pDupeItem ? pDupeItem->dwUnitId : -1, nTargetBodyLoc, 1, &nUnused);

    D2GAME_RemoveItemIfOnCursor_6FC49760(pGame, pPlayer, pItem);
    INVENTORY_SetCursorItem(pPlayer->pInventory, nullptr);

    if (pExchangeItem)
    {
        D2UnitStrc* pCursorItem = ITEMS_Duplicate(pGame, pExchangeItem, pPlayer, 1);
        INVENTORY_SetCursorItem(pPlayer->pInventory, pCursorItem);
        sub_6FC4B9D0(pGame, pPlayer, pCursorItem);
    }

    D2GAME_ITEMS_UpdateInventoryItems_6FC45050(pGame, pMerc, 0, 0);
    //D2Game_10034_Return(0);
    D2GAME_EVENTS_Delete_6FC34840(pGame, pMerc, UNITEVENTCALLBACK_STATREGEN, 0);
    EVENT_SetEvent(pGame, pMerc, UNITEVENTCALLBACK_STATREGEN, pGame->dwGameFrame + 1, 0, 0);

    return 1;
}

//D2Game.0x6FC88F80
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x63_ShiftLeftClickItemToBelt_6FC88F80(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    // TODO: nSlot doesn't seem to be defined in the original code

    if (nSize != 5)
    {
        FOG_Trace("svalidatemessage failed for sGridToBelt()\n");
        return 3;
    }

    const int32_t nItemGUID = *(int32_t*)((char*)pPacket + 1);

    D2_ASSERT(pGame);

    D2UnitStrc* pItem = SUNIT_GetServerUnit(pGame, UNIT_ITEM, nItemGUID);
    if (!pItem || pItem->dwAnimMode != IMODE_STORED || !INVENTORY_CompareWithItemsParentInventory(pUnit->pInventory, pItem))
    {
        return 1;
    }

    if (INVENTORY_GetCursorItem(pUnit->pInventory))
    {
        D2GSPacketSrv5A packet5A = {};
        packet5A.nHeader = 0x5Au;
        packet5A.nColor = STRCOLOR_RED;
        packet5A.dwParam = 0;
        packet5A.nType = EVENTTYPE_CANTDOTHAT;
        D2GAME_PACKETS_SendPacket0x5A_6FC3DEC0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet5A);
        return 2;
    }

    if (!ITEMS_CheckIfBeltable(pItem))
    {
        return 2;
    }

    if (ITEMS_GetInvPage(pItem) != 0)
    {
        FOG_Trace("Tried to remove item from non-main page\n");
        return 3;
    }

    int32_t nFreeSlotId = 0;
    if (INVENTORY_GetFreeBeltSlot(pUnit->pInventory, pItem, &nFreeSlotId) && nFreeSlotId >= 0 && sub_6FC7C3A0(pGame, pUnit, pItem))
    {
        D2_ASSERT(pUnit->pInventory);

        if (ITEMS_GetInvPage(pItem) || pItem->dwAnimMode != IMODE_STORED)
        {
            return 2;
        }

        D2UnitStrc* pRemove = INVENTORY_RemoveItemFromInventory(pUnit->pInventory, pItem);

        D2_ASSERT(pRemove);

        D2_ASSERT(pItem == pRemove);

        const int32_t nPage = ITEMS_GetInvPage(pItem);
        //INVENTORY_Return(__FILE__, __LINE__, pUnit->pInventory, CLIENTS_GetUnitX(pItem), CLIENTS_GetUnitY(pItem), UNITS_GetInventoryRecordId(pUnit, nPage, pGame->bExpansion), 0, nPage);
        ITEMS_SetItemCell(pItem, nPage);
        ITEMS_SetInvPage(pItem, 0xFFu);

        D2GAME_UpdateClientItem_6FC3E9D0(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pItem, 0);

        UNITS_ChangeAnimMode(pItem, IMODE_ONCURSOR);

        pItem->dwFlags &= 0xFFFFFFFDu;

        ITEMS_GetDimensions(pItem, (BYTE*)&pPacket, (BYTE*)&nSize, __FILE__, __LINE__);

        if (INVENTORY_PlaceItemInBeltSlot(pUnit->pInventory, pItem, nFreeSlotId))
        {
            ITEMS_SetInvPage(pItem, 0xFFu);

            if (INVENTORY_PlaceItemInSocket(pUnit->pInventory, pItem, 2))
            {
                UNITS_ChangeAnimMode(pItem, IMODE_INBELT);
                D2GAME_SendP0x9C_ItemAction_PutInBelt_6FC3EB90(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), pUnit, pItem, 0);
            }
        }

        UNITS_RefreshInventory(pUnit, 1);
    }

    return 0;
}

//D2Game.0x6FC89310
int32_t __fastcall D2GAME_PACKETCALLBACK_Rcv0x2E_42_43_64_65_6FC89310(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    return 0;
}

//D2Game.0x6FC89320
int32_t __fastcall D2GAME_PACKET_Handler_6FC89320(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pPacket, int32_t nSize)
{
    D2ServerPacketTableStrc gpPacketCallbackTable_6FD28FA0[] =
    {
        { nullptr, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x01_Walk_6FC82CB0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x02_WalkToEntity_6FC82ED0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x03_Run_6FC83090, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x04_RunToEntity_6FC830F0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x05_ShiftLeftClickSkill_6FC832B0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x06_LeftSkillOnUnit_6FC83550, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x07_ShiftLeftSkillOnUnit_6FC83890, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x08_ShiftLeftSkillHold_6FC83A10, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x09_LeftSkillOnUnitHold_6FC83AC0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x0A_ShiftLeftSkillOnUnitHold_6FC83D40, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x0B_6FC83EE0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x0C_RightSkill_6FC83EF0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x0D_RightSkillOnUnit_6FC83F80, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x0E_ShiftRightSkillOnUnit_6FC84100, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x0F_RightSkillHold_6FC84280, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x10_RightSkillOnUnitHold_6FC84330, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x11_ShiftRightSkillOnUnitHold_6FC844D0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x12_6FC84670, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x13_InteractWithEntity_6FC84D70, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x14_HandleOverheadChat_6FC84690, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x15_HandleChatMessage_6FC84950, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x16_PickItemOnGround_6FC84DB0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x17_DropItemOnGround_6FC84E20, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x18_InsertItemInBuffer_6FC84ED0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x19_RemoveItemFromBuffer_6FC850C0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x1A_EquipItem_6FC85280, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x1B_Swap2HandedItem_6FC853C0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x1C_RemoveBodyItem_6FC854B0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x1D_SwapCursorItemWithBody_6FC85550, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x1E_SwapTwo1HandedWithOne2HandedItem_6FC85690, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x1F_SwapCursorBufferItems_6FC857C0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x20_UseGridItem_6FC85990, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x21_StackItems_6FC85C80, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x22_UnstackItems_6FC85DA0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x23_ItemToBelt_6FC85E70, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x24_ItemFromBelt_6FC85F50, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x25_SwitchBeltItem_6FC86040, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x26_UseBeltItem_6FC86150, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x27_UseItemAction_6FC85B50, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x28_SocketItem_6FC86310, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x29_ScrollToBook_6FC86450, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x2A_ItemToCubeIndirect_6FC866E0, 1 },
        { nullptr, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x2C_2D_52_6FC867B0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x2C_2D_52_6FC867B0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x2E_42_43_64_65_6FC89310, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x2F_InitEntityChat_6FC868C0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x30_TerminateEntityChat_6FC869F0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x31_QuestMessage_6FC86AB0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x32_BuyItemFromNpcBuffer_6FC86AE0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x33_SellItemToNpcBuffer_6FC86B30, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x34_IdentifyItemsWithNpc_6FC86BA0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x35_Repair_6FC86B70, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x36_HireMerc_6FC86BD0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x37_IdentifyFromGamble_6FC86C30, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x38_EntityAction_6FC86CB0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x39_PurchaseLife_6FC86E50, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x3A_AddStatPoint_6FC86E80, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x3B_AddSkillPoint_6FC86EF0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x3C_SelectSkill_6FC87050, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x3D_HighlightADoor_6FC870F0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x3E_ActivateInifussScroll_6FC87270, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x3F_PlayAudio_6FC87410, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x40_RequestQuestData_6FC87460, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x41_Resurrect_6FC87480, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x2E_42_43_64_65_6FC89310, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x2E_42_43_64_65_6FC89310, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x44_StaffInOrifice_6FC87780, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x45_ChangeTpLocation_6FC87930, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x46_MercInteract_6FC87B00, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x47_MoveMerc_6FC87CE0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x48_TurnOffBusyState_6FC87E20, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x49_TakeOrCloseWp_6FC87E60, 0 },
        { nullptr, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x4B_RequestEntityUpdate_6FC880F0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x4C_Transmogrify_6FC88170, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x4D_PlayNpcMessage_6FC880A0, 0 },
        { nullptr, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x4F_ClickButton_6FC881D0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x50_DropGold_6FC88210, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x51_BindHotkeyToSkill_6FC88290, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x2C_2D_52_6FC867B0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x53_TurnStaminaOn_6FC88340, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x54_TurnStaminaOff_6FC88380, 0 },
        { nullptr, 1 },
        { nullptr, 1 },
        { nullptr, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x58_QuestCompleted_6FC883B0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x59_MakeEntityMove_6FC88400, 0 },
        { nullptr, 1 },
        { nullptr, 1 },
        { nullptr, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x5D_SquelchHostile_6FC876B0, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x5E_InviteToParty_6FC87720, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x5F_UpdatePlayerPos_6FC88530, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x60_SwapWeapons_6FC888A0, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x61_DropPickupMercItem_6FC88930, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x62_ResurrectMerc_6FC86C00, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x63_ShiftLeftClickItemToBelt_6FC88F80, 1 },
        { D2GAME_PACKETCALLBACK_Rcv0x2E_42_43_64_65_6FC89310, 0 },
        { D2GAME_PACKETCALLBACK_Rcv0x2E_42_43_64_65_6FC89310, 0 },
    };

    const uint8_t nHeader = *(uint8_t*)pPacket;

    if (nHeader != 0 && nHeader < std::size(gpPacketCallbackTable_6FD28FA0) && gpPacketCallbackTable_6FD28FA0[nHeader].pfCallback)
    {
        switch (nHeader)
        {
        case 0x41u:
            if (!pUnit || pUnit->dwAnimMode != PLRMODE_DEAD)
            {
                return 0;
            }

            break;

        case 0x14u:
        case 0x15u:
        case 0x3Cu:
        case 0x43u:
            break;

        default:
            if (!D2GAME_PLRMODES_First_6FC7F340(pGame, pUnit, 1, 0, 0))
            {
                return 0;
            }

            break;
        }

        if (pGame->nSyncTimer > 1)
        {
            pGame->nSyncTimer = FOG_10055_GetSyncTime();
        }

        if (IsBadCodePtr((FARPROC)gpPacketCallbackTable_6FD28FA0[nHeader].pfCallback))
        {
            FOG_DisplayAssert("sgcptIncomingMessageChart[bCmd].pfnProcess", __FILE__, __LINE__);
            exit(-1);
        }

        return gpPacketCallbackTable_6FD28FA0[nHeader].pfCallback(pGame, pUnit, pPacket, nSize);
    }

    return 3;
}

//D2Game.0x6FC89450
void __fastcall D2GAME_PLRMSG_Last_6FC89450(D2GameStrc* pGame, D2UnitStrc* pAttacker, D2UnitStrc* pDefender)
{
    D2UnitStrc* pPlayer = pAttacker;

    D2_ASSERT(pDefender && pDefender->dwUnitType == UNIT_PLAYER);

    if (pAttacker && pAttacker->dwUnitType == UNIT_MONSTER)
    {
        D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pAttacker);
        if (pOwner && pOwner->dwUnitType == UNIT_PLAYER)
        {
            pPlayer = pOwner;
        }
    }

    D2GSPacketSrv5A packet5A = {};
    packet5A.nHeader = 0x5Au;
    packet5A.nType = EVENTTYPE_SLAIN_BY;
    packet5A.nColor = STRCOLOR_DARK_GOLD;

    if (pPlayer)
    {
        packet5A.dwParam = pPlayer->dwClassId;
        packet5A.nParam = pPlayer->dwUnitType;
    }
    else
    {
        packet5A.dwParam = -1;
        packet5A.nParam = 6;
    }

    packet5A.szText[16] = 0;

    const char* szDefenderName = CLIENTS_GetName(SUNIT_GetClientFromPlayer(pDefender, __FILE__, __LINE__));
    if (SStrLen(szDefenderName))
    {
        SStrCopy(packet5A.szText, szDefenderName, 16u);
    }
    else
    {
        packet5A.szText[0] = 0;
    }

    if (pPlayer)
    {
        if (pPlayer->dwUnitType == UNIT_PLAYER)
        {
            const char* szPlayerName = CLIENTS_GetName(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__));

            char szName[16] = {};
            SStrCopy(szName, szPlayerName, 16u);

            if (SStrLen(szName))
            {
                SStrCopy(&packet5A.szText[16], szName, 16u);
            }
        }
        else if (pPlayer->dwUnitType == UNIT_MONSTER && MONSTERUNIQUE_CheckMonTypeFlag(pPlayer, MONTYPEFLAG_SUPERUNIQUE))
        {
            *(uint16_t*)&packet5A.szText[16] = MONSTERUNIQUE_GetBossHcIdx(pPlayer);
        }
    }

    for (int32_t i = 0; i < 16; ++i)
    {
        if (!packet5A.szText[i])
        {
            GAME_ForEachIngameClient(pGame, j_D2GAME_SendPacket0x5A_6FC3DEC0, &packet5A);
            return;
        }
    }
}
