#include "MONSTER/MonsterMsg.h"

#include <D2Monsters.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>

#include "AI/AiGeneral.h"
#include "GAME/Clients.h"
#include "GAME/SCmd.h"
#include "INVENTORY/InvMode.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterUnique.h"
#include "UNIT/SUnit.h"
#include "UNIT/SUnitMsg.h"
#include "UNIT/SUnitProxy.h"


#pragma pack(push, 1)
struct D2UnkMonMsgStrc
{
    int32_t unk0x00;
    int32_t unk0x04;
    int32_t unk0x08;
    int32_t unk0x0C;
    int32_t unk0x10;
    int32_t unk0x14;
};
#pragma pack(pop)


//D2Game.0x6FC659E0
void __fastcall sub_6FC659E0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t a4)
{
    if (pUnit)
    {
        if (pUnit->dwFlagEx & UNITFLAGEX_TELEPORTED)
        {
            D2GAME_PACKETS_SendPacket0x15_6FC3D0D0(pClient, 0x15u, pUnit->dwUnitType, pUnit->dwUnitId, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), 1);
            sub_6FCBB190(pGame, pUnit, 0);
        }

        if (pUnit->dwFlags & UNITFLAG_DOUPDATE)
        {
            sub_6FC65C70(pGame, pUnit, pClient);
        }

        pUnit->dwFlags &= ~UNITFLAG_MONMODEISCHANGING;
    }

    sub_6FCC6540(pUnit, pClient);

    if (pUnit)
    {
        if (pUnit->dwFlags & UNITFLAG_HASTXTMSG)
        {
            sub_6FCC5FA0(pUnit, pClient);
        }

        if (pUnit->dwFlagEx & UNITFLAGEX_HASINV)
        {
            D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
            if (pPlayer && sub_6FC61B30(pUnit, pPlayer))
            {
                SUNITPROXY_UpdateGambleInventory(pGame, pUnit, pPlayer, pClient);
            }
            else if (MONSTERS_GetHirelingTypeId(pUnit) && AIGENERAL_GetMinionOwner(pUnit) == pPlayer)
            {
                D2GAME_INVMODE_First_6FC40FB0(pGame, pUnit, pClient, 0);
            }
        }

        if (pUnit->dwFlags & UNITFLAG_HASEVENTSOUND)
        {
            D2GAME_UpdateUnit_6FCC6080(pUnit, pClient);
        }

        if (pUnit->dwFlags & UNITFLAG_UPGRLIFENHITCLASS)
        {
            uint8_t nHpPct = sub_6FC62F50(pUnit);
            STATLIST_SetUnitStat(pUnit, STAT_LAST_SENT_HP_PCT, nHpPct, 0);
            if (nHpPct > 1u)
            {
                --nHpPct;
            }

            if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, 0x100u))
            {
                nHpPct |= 0x80;
            }

            D2GAME_PACKETS_SendPacket0x0C_6FC3C8A0(pClient, 0xCu, 1, pUnit->dwUnitId, 19, pUnit->dwLastHitClass, nHpPct);
        }
    }

    if (!a4 && STATES_IsAnyGfxStateFlagSet(pUnit))
    {
        D2GAME_STATES_SendStates_6FCC5F00(pUnit, pClient, 0);
    }

    if (D2COMMON_10530_D2CheckStatlistFlagDMGRed(pUnit))
    {
        sub_6FCC5F20(pUnit, pClient);
    }

    if (pUnit && pUnit->dwFlags & UNITFLAG_NOTC && MONSTER_CheckSummonerFlag(pUnit, 1u))
    {
        const uint8_t* const pUMods = MONSTERUNIQUE_GetUMods(pUnit);
        if (pUMods)
        {
            sub_6FC3F4D0(pClient, pUnit->dwUnitId, 1, MONSTERUNIQUE_GetNameSeed(pUnit), pUMods[0] | (pUMods[1] << 8), pUMods[2], MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_CHAMPION));
        }
    }
}

//D2Game.0x6FC65C70
void __fastcall sub_6FC65C70(D2GameStrc* pGame, D2UnitStrc* pMonster, D2ClientStrc* pClient)
{
    // TODO: Names
    constexpr D2UnkMonMsgStrc stru_6FD28988[16] =
    {
        { 0, 1, 0, 0, 8, 8 },
        { 0, 0, 0, 0, 7, 7 },
        { 1, 1, 1, 0, 1, 0 },
        { 0, 0, 0, 0, 6, 6 },
        { 1, 1, 0, 1, 11, 10 },
        { 1, 1, 0, 1, 17, 16 },
        { 1, 1, 0, 0, 18, 18 },
        { 1, 1, 0, 1, 4, 5 },
        { 1, 1, 0, 1, 12, 13 },
        { 1, 1, 0, 1, 14, 15 },
        { 1, 1, 0, 1, 26, 27 },
        { 1, 1, 0, 1, 28, 29 },
        { 0, 0, 0, 0, 9, 9 },
        { 0, 1, 1, 0, 20, 20 },
        { 1, 1, 0, 1, 12, 13 },
        { 1, 1, 1, 0, 23, 24 },
    };

    D2_ASSERT(pMonster);
    D2_ASSERT(pClient);

    const int32_t nAnimMode = pMonster->dwAnimMode;
    const D2UnkMonMsgStrc* v5 = &stru_6FD28988[nAnimMode];

    D2UnitStrc* pTarget = SUNIT_GetTargetUnit(pGame, pMonster);
    if (pTarget && !CLIENTS_IsInUnitsRoom(pTarget, pClient) || !v5->unk0x00)
    {
        pTarget = nullptr;
    }

    if (nAnimMode == MONMODE_SEQUENCE || UNITS_GetUsedSkill(pMonster))
    {
        D2SkillStrc* pSkill = UNITS_GetUsedSkill(pMonster);
        if (pSkill)
        {
            const int32_t nSkillId = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
            const int32_t nSkillLevel = SKILLS_GetSkillLevel(pMonster, pSkill, 1);
            if (pTarget && CLIENTS_IsInUnitsRoom(pTarget, pClient))
            {
                sub_6FC3F0C0(pClient, 1u, pMonster->dwUnitId, pTarget->dwUnitType, pTarget->dwUnitId, nSkillId, 0, nSkillLevel, 0);
            }
            else
            {
                sub_6FC3F060(pClient, 1u, pMonster->dwUnitId, D2COMMON_10175_PathGetFirstPointX(pMonster->pDynamicPath), D2COMMON_10176_PathGetFirstPointY(pMonster->pDynamicPath), nSkillId, 0, nSkillLevel, 0);
            }
        }
        return;
    }

    uint8_t nDirection = 0;
    uint8_t v39 = 0;
    int a7 = 0;
    uint8_t nHeader = 0;
    int32_t a4 = 0;
    int32_t v8 = 0;
    int32_t v9 = 0;
    int32_t v10 = 0;

    D2DynamicPathStrc* pDynamicPath = pMonster->pDynamicPath;
    D2_ASSERT(pDynamicPath);

    if (pTarget)
    {
        if (nAnimMode != MONMODE_WALK && nAnimMode != MONMODE_RUN || (v10 = PATH_GetType(pDynamicPath), v10 < 5) || v10 > 6)
        {
            nHeader = 0x68u;
            a4 = v5->unk0x14;
            v8 = pTarget->dwUnitType;
            v9 = pTarget->dwUnitId;
        }
        else
        {
            nHeader = 0x67u;
            a4 = v5->unk0x10;
            v8 = D2COMMON_10177_PATH_GetLastPointX(pDynamicPath);
            v9 = D2COMMON_10178_PATH_GetLastPointY(pDynamicPath);
        }
    }
    else
    {
        a4 = v5->unk0x10;
        nHeader = 0x67u;
        if (v5->unk0x04)
        {
            v8 = D2COMMON_10177_PATH_GetLastPointX(pDynamicPath);
            v9 = D2COMMON_10178_PATH_GetLastPointY(pDynamicPath);
        }
        else
        {
            v8 = PATH_GetXPosition(pDynamicPath);
            v9 = PATH_GetYPosition(pDynamicPath);
        }
    }

    switch (nAnimMode)
    {
    case MONMODE_BLOCK:
    {
        if (!pTarget)
        {
            v8 = 0;
            v9 = -1;
        }
        break;
    }
    case MONMODE_DEATH:
    {
        nDirection = PATH_GetDirection(pDynamicPath);
        v39 = LOBYTE(pMonster->dwLastHitClass);
        break;
    }
    case MONMODE_DEAD:
    {
        nDirection = PATH_GetDirection(pDynamicPath);
        break;
    }
    case MONMODE_KNOCKBACK:
    {
        const int32_t nDistance = _10191_PATH_GetDistance(pDynamicPath);
        if (nHeader == 0x68)
        {
            D2GAME_PACKETS_SendPacket0x68_6FC3CCB0(pClient, 0x68u, pMonster->dwUnitId, a4, v8, v9, nDistance, sub_6FC62F50(pMonster), pMonster->dwLastHitClass);
        }
        else
        {
            D2GAME_PACKETS_SendPacket0x67_6FC3CDE0(pClient, nHeader, pMonster->dwUnitId, a4, v8, v9, nDistance, sub_6FC62F50(pMonster), pMonster->dwLastHitClass);
        }
        return;
    }
    case MONMODE_GETHIT:
    {
        uint8_t nUnitLife = sub_6FC62F50(pMonster);
        if (nUnitLife > 1u)
        {
            --nUnitLife;
        }

        if (MONSTERUNIQUE_CheckMonTypeFlag(pMonster, 0x100u))
        {
            nUnitLife |= 0x80u;
        }

        v39 = LOBYTE(pMonster->dwLastHitClass);
        break;
    }
    default:
    {
        a7 = PATH_GetStepNum(pDynamicPath);
        if (nAnimMode == MONMODE_NEUTRAL)
        {
            D2GAME_PACKETS_SendPacket0x6D_6FC3D080(pClient, pMonster->dwUnitId, CLIENTS_GetUnitX(pMonster), CLIENTS_GetUnitY(pMonster), sub_6FC62F50(pMonster));
            STATLIST_SetUnitStat(pMonster, STAT_PIERCE_IDX, STATLIST_GetUnitBaseStat(pMonster, STAT_PIERCE_IDX, 0) + 1, 0);
            return;
        }

        if (nAnimMode == MONMODE_KNOCKBACK)
        {
            if (nHeader == 0x68)
            {
                D2GAME_PACKETS_SendPacket0x68_6FC3CCB0(pClient, 0x68u, pMonster->dwUnitId, a4, v8, v9, nDirection, 0, 0);
            }
            else
            {
                D2GAME_PACKETS_SendPacket0x67_6FC3CDE0(pClient, nHeader, pMonster->dwUnitId, a4, v8, v9, nDirection, 0, 0);
            }
            return;
        }
        break;
    }
    }

    if (v5->unk0x08)
    {
        if (nHeader == 0x68)
        {
            D2GAME_PACKETS_SendPacket0x68_6FC3CA90(pClient, 0x68u, pMonster->dwUnitId, a4, v8, v9, a7);
        }
        else
        {
            D2GAME_PACKETS_SendPacket0x67_6FC3CBC0(pClient, nHeader, pMonster->dwUnitId, a4, v8, v9, a7);
        }
    }
    else if (v5->unk0x0C)
    {
        if (nHeader == 0x68)
        {
            D2GAME_PACKETS_SendPacket0x6C_6FC3CF90(pClient, 0x6Cu, pMonster->dwUnitId, a4, v8, v9, nDirection, CLIENTS_GetUnitX(pMonster), CLIENTS_GetUnitY(pMonster));
        }
        else
        {
            D2GAME_PACKETS_SendPacket0x6B_6FC3D000(pClient, 0x6Bu, pMonster->dwUnitId, a4, v8, v9, nDirection, v39, CLIENTS_GetUnitX(pMonster), CLIENTS_GetUnitY(pMonster));
        }
    }
    else
    {
        if (nHeader == 0x68)
        {
            D2GAME_PACKETS_SendPacket0x6A_6FC3CEE0(pClient, 0x6Au, pMonster->dwUnitId, a4, v8, v9, nDirection);
        }
        else
        {
            D2GAME_PACKETS_SendPacket0x69_6FC3CF30(pClient, 0x69u, pMonster->dwUnitId, a4, v8, v9, nDirection, v39);
        }
    }
}
