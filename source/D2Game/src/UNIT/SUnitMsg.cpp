#include "UNIT/SUnitMsg.h"

#include <algorithm>

#include "D2PacketDef.h"

#include <D2BitManip.h>
#include <D2Math.h>

#include <D2Chat.h>
#include <D2DataTbls.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2Items.h>
#include <D2StatList.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <Units/UnitRoom.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ItemsTbls.h>


#include "AI/AiGeneral.h"
#include "GAME/Clients.h"
#include "GAME/SCmd.h"
#include "ITEMS/ItemMode.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterMsg.h"
#include "PLAYER/Player.h"
#include "PLAYER/PlayerList.h"
#include "PLAYER/PlrMsg.h"
#include "SKILLS/Skills.h"
#include "UNIT/SUnit.h"



//D2Game.0x6FCC5520
void __fastcall D2GAME_SUNITMSG_FirstFn_6FCC5520(D2GameStrc* pGame, D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    if (!pUnit)
    {
        return;
    }

    D2CoordStrc coords = {};
    UNITS_GetCoords(pUnit, &coords);

    D2UnitStrc* pLocalPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    {
        sub_6FC3D1F0(pClient, pUnit->dwUnitId, (uint8_t)pUnit->dwClassId, pUnit->pPlayerData->szName, coords.nX, coords.nY);
        break;
    }
    case UNIT_ITEM:
    {
        if (pUnit->dwFlags & UNITFLAG_INITSEEDSET)
        {
            if (pUnit->dwAnimMode == IMODE_ONGROUND && pUnit->dwFlags & UNITFLAG_SENDREFRESHMSG)
            {
                D2GAME_SendP0x9C_ItemAction_DropToGround_6FC3E8E0(pClient, pLocalPlayer, pUnit, 0);
            }
            else
            {
                D2GAME_SendP0x9C_ItemAction_AddToGround_6FC3E850(pClient, pUnit, 0);
            }
        }
        break;
    }
    case UNIT_OBJECT:
    {
        D2GAME_PACKETS_SendPacket0x51_6FC3D300(pClient, 0x51u, 2u, pUnit->dwUnitId, (uint16_t)pUnit->dwClassId, coords.nX, coords.nY, pUnit->dwAnimMode, pUnit->pObjectData->InteractType);
        if (pUnit->pObjectData->pObjectTxt->nSubClass & 4)
        {
            sub_6FC3F550(pClient, pUnit);
            if (pUnit->dwClassId == OBJECT_TOWN_PORTAL)
            {
                D2UnitStrc* pOwner = SUNIT_GetPortalOwner(pGame, pUnit);
                D2GAME_PACKETS_SendPacket0x82_6FC3F790(pGame, pClient, pUnit->dwUnitId, pOwner ? pOwner->dwUnitId : -1);
            }
        }
        break;
    }
    case UNIT_MONSTER:
    {
        if (!SUNIT_IsDead(pUnit) || !STATES_CheckStateMaskHideOnUnit(pUnit))
        {
            sub_6FC3FC80(pClient, pUnit);
            STATLIST_SetUnitStat(pUnit, STAT_PIERCE_IDX, (uint16_t)(coords.nY + coords.nX), 0);
            if (pUnit->dwClassId == MONSTER_EVILHUT)
            {
                sub_6FC3FAF0(pClient, pUnit->dwUnitId, AIGENERAL_GetMinionSpawnClassId(pUnit));
            }

            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pUnit->dwClassId);
            if (pMonStatsTxtRecord)
            {
                for (int32_t i = 0; i < 8; ++i)
                {
                    if (pMonStatsTxtRecord->nSendSkills[i >> 3] & gdwBitMasks[i & 7])
                    {
                        const int32_t nSkillId = pMonStatsTxtRecord->nSkill[i];
                        if (nSkillId >= 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
                        {
                            D2SkillStrc* pSkill = SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId);
                            if (pSkill)
                            {
                                D2GAME_PACKETS_SendPacket0x21_UpdateSkills_6FC3DB50(pClient, pUnit, nSkillId, SKILLS_GetSkillLevel(pUnit, pSkill, 0), 0);
                            }
                        }
                    }
                }
            }

            D2GAME_STATES_SendUnitStates_6FCC58E0(pUnit, pClient);
            sub_6FCC6540(pUnit, pClient);
        }
        break;
    }
    case UNIT_MISSILE:
    {
        MISSILES_SyncToClient(pClient, pGame, pUnit, PATH_GetVelocity(pUnit->pDynamicPath));
        break;
    }
    default:
    {
        D2GAME_PACKETS_SendPacket0x09_6FC3D1A0(pClient, 9u, pUnit->dwUnitType, pUnit->dwUnitId, pUnit->dwClassId, coords.nX, coords.nY);
        break;
    }
    }

    switch (pUnit->dwUnitType)
    {
    case UNIT_PLAYER:
    {
        sub_6FC82830(pUnit, pClient);

        if (SUNIT_IsDead(pUnit) && STATES_CheckState(pUnit, STATE_PLAYERBODY))
        {
            D2UnitStrc* pPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, INVENTORY_GetOwnerId(pUnit->pInventory));
            D2_ASSERT(pPlayer);
            D2GAME_PACKETS_SendPacket0x74_6FC3F640(pClient, pPlayer, pUnit, (pUnit->dwFlagEx & UNITFLAGEX_ISCORPSE) != 0);
        }
        else
        {
            PLAYER_SynchronizeItemsToClient(pUnit, pClient);
        }

        D2GAME_STATES_SendUnitStates_6FCC58E0(pUnit, pClient);
        sub_6FC82270(pGame, pUnit, pClient);
        sub_6FCC6540(pUnit, pClient);
        sub_6FCC5FA0(pUnit, pClient);
        break;
    }
    case UNIT_MONSTER:
    {
        sub_6FC65C70(pGame, pUnit, pClient);

        if (STATES_CheckState(pUnit, STATE_VALKYRIE) || pUnit->dwClassId == MONSTER_IRONGOLEM || pUnit->dwClassId == MONSTER_SHADOWWARRIOR || pUnit->dwClassId == MONSTER_SHADOWMASTER)
        {
            PLAYER_SynchronizeItemsToClient(pUnit, pClient);
        }
        break;
    }
    case UNIT_ITEM:
    {
        sub_6FC42050(pUnit, pClient);
        break;
    }
    default:
        return;
    }
}

//D2Game.0x6FCC58E0
void __fastcall D2GAME_STATES_SendUnitStates_6FCC58E0(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    D2BitBufferStrc bitBuffer = {};
    uint32_t nFlags[8] = {};
    D2StatStrc stats[16] = {};

    if (!pUnit || !pClient)
    {
        return;
    }

    const int32_t nTotalStatFlagCount = (sgptDataTables->nStatesTxtRecordCount + 31) / 32;
    memcpy(nFlags, D2COMMON_10494_STATES_GetStatFlags(pUnit), sizeof(uint32_t) * nTotalStatFlagCount);

    D2GSPacketSrvAA packetAA = {};
    packetAA.nHeader = 0xAAu;
    packetAA.nUnitType = pUnit->dwUnitType;
    packetAA.nUnitId = pUnit->dwUnitId;
    BITMANIP_Initialize(&bitBuffer, packetAA.pStream, 244);

    for (int32_t i = 0; i < nTotalStatFlagCount; ++i)
    {
        int32_t nBit = FOG_LeadingZeroesCount(nFlags[i]);
        while (nBit >= 0)
        {
            nFlags[i + (nBit >> 5)] &= gdwInvBitMasks[nBit & 31];

            const int32_t nStateId = nBit + 32 * i;

            D2StatesTxt* pStatesTxtRecord = SKILLS_GetStatesTxtRecord(nStateId);
            if (pStatesTxtRecord && !(pStatesTxtRecord->nStateFlags[0] & gdwBitMasks[0]))
            {
                BITMANIP_Write(&bitBuffer, nStateId, 8);

                D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nStateId);
                if (pStatList)
                {
                    const int32_t nStatCount = STATLIST_GetBaseStatsData(pStatList, stats, std::size(stats));
                    if (nStatCount > 0)
                    {
                        BITMANIP_Write(&bitBuffer, 1, 1);

                        for (int32_t j = 0; j < nStatCount; ++j)
                        {
                            int32_t nValue = stats[j].nValue;
                            const int32_t nLayer = stats[j].nLayer;
                            const int32_t nStatId = *(&stats[0].nStat + 4 * j);

                            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                            if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->nSendBits)
                            {
                                if (pItemStatCostTxtRecord->nSendBits < 32u)
                                {
                                    if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[1])
                                    {
                                        const int32_t nMin = -(1 << (pItemStatCostTxtRecord->nSendBits - 1));
                                        const int32_t nMax = (1 << (pItemStatCostTxtRecord->nSendBits - 1)) - 1;
                                        nValue = D2Clamp(nValue, nMin, nMax);
                                    }
                                    else
                                    {
                                        const uint32_t nMax = (1 << pItemStatCostTxtRecord->nSendBits) - 1;
                                        nValue &= (nValue <= 0) - 1;
                                        nValue = std::min((uint32_t)nValue, nMax);
                                    }
                                }

                                BITMANIP_Write(&bitBuffer, nStatId, 9);

                                if (pItemStatCostTxtRecord->nSendParamBits)
                                {
                                    BITMANIP_Write(&bitBuffer, nLayer, pItemStatCostTxtRecord->nSendParamBits);
                                }

                                BITMANIP_Write(&bitBuffer, nValue, pItemStatCostTxtRecord->nSendBits);
                            }
                        }

                        BITMANIP_Write(&bitBuffer, 511, 9);
                    }
                    else
                    {
                        BITMANIP_Write(&bitBuffer, 0, 1);
                    }
                }
                else
                {
                    BITMANIP_Write(&bitBuffer, 0, 1);
                }
            }

            nBit = FOG_LeadingZeroesCount(nFlags[i]);
        }
    }

    BITMANIP_Write(&bitBuffer, 0xFFu, 8);

    packetAA.nSize = BITMANIP_GetSize(&bitBuffer) + 7;
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetAA, packetAA.nSize);
}

//D2Game.0x6FCC5BE0
void __fastcall D2GAME_STATES_SendUnitStateUpdates_6FCC5BE0(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    uint32_t nFlags[8] = {};
    D2StatStrc stats[16] = {};

    if (!pUnit || !pClient)
    {
        return;
    }
    
    const int32_t nTotalStatFlagCount = (sgptDataTables->nStatesTxtRecordCount + 31) / 32;
    memcpy(nFlags, STATES_GetGfxStateFlags(pUnit), sizeof(uint32_t) * nTotalStatFlagCount);

    D2BitBufferStrc bitBuffer = {};
    for (int32_t i = 0; i < nTotalStatFlagCount; ++i)
    {
        int32_t nBit = FOG_LeadingZeroesCount(nFlags[i]);
        while (nBit >= 0)
        {
            nFlags[i + (nBit >> 5)] &= gdwInvBitMasks[nBit & 31];

            const int32_t nStateId = nBit + 32 * i;
            D2StatesTxt* pStatesTxtRecord = SKILLS_GetStatesTxtRecord(nStateId);
            if (pStatesTxtRecord && !(pStatesTxtRecord->nStateFlags[0] & gdwBitMasks[0]))
            {
                if (STATES_CheckState(pUnit, nStateId))
                {
                    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndState(pUnit, nStateId);
                    if (pStatList)
                    {
                        const int32_t nStatCount = STATLIST_GetBaseStatsData(pStatList, stats, 16);
                        if (nStatCount > 0)
                        {
                            D2GSPacketSrvA8 packetA8 = {};

                            packetA8.nHeader = 0xA8u;
                            packetA8.nUnitType = pUnit->dwUnitType;
                            packetA8.nUnitId = pUnit->dwUnitId;
                            packetA8.nState = nStateId;
                            BITMANIP_Initialize(&bitBuffer, packetA8.pStream, 244);

                            for (int32_t j = 0; j < nStatCount; ++j)
                            {
                                int32_t nValue = stats[j].nValue;
                                const int32_t nLayer = stats[j].nLayer;
                                const int32_t nStatId = *(&stats[0].nStat + 4 * j);

                                D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(nStatId);
                                if (pItemStatCostTxtRecord && pItemStatCostTxtRecord->nSendBits)
                                {
                                    if (pItemStatCostTxtRecord->nSendBits < 32u)
                                    {
                                        if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[1])
                                        {
                                            const int32_t nMin = -(1 << (pItemStatCostTxtRecord->nSendBits - 1));
                                            const int32_t nMax = (1 << (pItemStatCostTxtRecord->nSendBits - 1)) - 1;
                                            nValue = D2Clamp(nValue, nMin, nMax);
                                        }
                                        else
                                        {
                                            const uint32_t nMax = (1 << pItemStatCostTxtRecord->nSendBits) - 1;
                                            nValue &= (nValue <= 0) - 1;
                                            nValue = std::min((uint32_t)nValue, nMax);
                                        }
                                    }

                                    BITMANIP_Write(&bitBuffer, nStatId, 9);

                                    if (pItemStatCostTxtRecord->nSendParamBits)
                                    {
                                        BITMANIP_Write(&bitBuffer, nLayer, pItemStatCostTxtRecord->nSendParamBits);
                                    }

                                    BITMANIP_Write(&bitBuffer, nValue, pItemStatCostTxtRecord->nSendBits);
                                }
                            }

                            BITMANIP_Write(&bitBuffer, 0x1FFu, 9);

                            packetA8.nSize = BITMANIP_GetSize(&bitBuffer) + 8;
                            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetA8, packetA8.nSize);
                        }
                        else
                        {
                            D2GAME_PACKETS_SendPacket0xA7_StateOn_6FC3FC20(pClient, pUnit->dwUnitType, pUnit->dwUnitId, nStateId);
                        }
                    }
                    else
                    {
                        D2GAME_PACKETS_SendPacket0xA7_StateOn_6FC3FC20(pClient, pUnit->dwUnitType, pUnit->dwUnitId, nStateId);
                    }
                }
                else
                {
                    D2GAME_PACKETS_SendPacket0xA9_StateOff_6FC3FC50(pClient, pUnit->dwUnitType, pUnit->dwUnitId, nStateId);
                }
            }

            nBit = FOG_LeadingZeroesCount(nFlags[i]);
        }
    }
}

//D2Game.0x6FCC5F00
void __fastcall D2GAME_STATES_SendStates_6FCC5F00(D2UnitStrc* pUnit, D2ClientStrc* pClient, int32_t bPlayer)
{
    if (bPlayer)
    {
        D2GAME_STATES_SendUnitStates_6FCC58E0(pUnit, pClient);
    }
    else
    {
        D2GAME_STATES_SendUnitStateUpdates_6FCC5BE0(pUnit, pClient);
    }
}

//D2Game.0x6FCC5F20
void __fastcall sub_6FCC5F20(D2UnitStrc* pItem, D2ClientStrc* pClient)
{
    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitAndFlag(pItem, 0x80);
    if (pStatList)
    {
        const int32_t nOverlay = STATLIST_GetTotalStatValue_Layer0((D2StatListExStrc*)pStatList, STAT_UNIT_DOOVERLAY);
        if (nOverlay >= 0 && nOverlay <= sgptDataTables->nOverlayTxtRecordCount)
        {
            if (pItem)
            {
                D2GAME_PACKETS_SendPacket0x11_6FC3F4A0(pClient, pItem->dwUnitType, pItem->dwUnitId, nOverlay);
            }
            else
            {
                D2GAME_PACKETS_SendPacket0x11_6FC3F4A0(pClient, 6, -1, nOverlay);
            }
        }
    }
}

//D2Game.0x6FCC5F80
void __fastcall D2GAME_PACKETS_SendPacket0x0A_RemoveObject_6FCC5F80(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    D2GAME_PACKETS_SendPacket0x0A_RemoveObject_6FC3D3A0(pClient, 0x0A, pUnit->dwUnitType, pUnit->dwUnitId);
}

//D2Game.0x6FCC5FA0
void __fastcall sub_6FCC5FA0(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    if (!pUnit)
    {
        D2GAME_PACKETS_SendPacketSize06_6FC3C850(pClient, 0x76u, 6, -1);
        return;
    }

    if (!pUnit->pHoverText)
    {
        D2GAME_PACKETS_SendPacketSize06_6FC3C850(pClient, 0x76u, pUnit->dwUnitType, pUnit->dwUnitId);
        return;
    }

    //CLIENTS_GetGame(pClient);
    D2UnitStrc* pClientPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    if (!pClientPlayer || pUnit->dwUnitType != UNIT_PLAYER || !PLAYERLIST_CheckFlag(pClientPlayer, pUnit, 4) && !PLAYERLIST_CheckFlag(pUnit, pClientPlayer, 2))
    {
        D2GSPacketSrv26 packet26 = {};
        packet26.nHeader = 0x26u;
        packet26.nMessageType = 5;
        packet26.nLang = CHAT_GetLangIdFromHoverMsg(pUnit->pHoverText);
        packet26.nUnitType = pUnit->dwUnitType;
        packet26.dwUnitGUID = pUnit->dwUnitId;
        packet26.szName[0] = 0;
        memset(packet26.szMessage, 0, sizeof(packet26.szMessage));
        CHAT_CopyHoverMsgToBuffer(pUnit->pHoverText, packet26.szMessage);
        D2GAME_PACKETS_SendPacket0x26_ServerMessage_6FC3DDF0(pClient, &packet26);
    }
}

//D2Game.0x6FCC6080
void __fastcall D2GAME_UpdateUnit_6FCC6080(D2UnitStrc* pPlayer, D2ClientStrc* pClient)
{
    if (!pPlayer->pUpdateUnit || CLIENTS_GetPlayerFromClient(pClient, 0) == pPlayer->pUpdateUnit)
    {
        D2GSPacketSrv2C packet2C = {};
        packet2C.nHeader = 0x2Cu;
        packet2C.dwUnitGUID = pPlayer->dwUnitId;
        packet2C.nUnitType = pPlayer->dwUnitType;
        packet2C.nSoundEvent = pPlayer->nUpdateType;
        sub_6FC3F3F0(pClient, &packet2C);
    }
}

//D2Game.0x6FCC60D0
void __fastcall sub_6FCC60D0(D2UnitStrc* pUnit, int16_t nSkillId, uint8_t nSkillLevel, uint8_t nUnitType, int32_t nUnitGUID, uint8_t a6)
{
    D2UnitPacketListStrc* pMsg = (D2UnitPacketListStrc*)D2_ALLOC_POOL(pUnit->pMemoryPool, 20);
    pMsg->pNext = nullptr;
    pMsg->nHeader = 0x99u;
    *((int16_t*)pMsg + 4) = nSkillId;
    *((uint8_t*)pMsg + 10) = nSkillLevel;
    *((uint8_t*)pMsg + 11) = nUnitType;
    *((int32_t*)pMsg + 3) = nUnitGUID;
    *((uint8_t*)pMsg + 16) = a6;

    if (!pUnit->pMsgFirst)
    {
        pUnit->pMsgFirst = pMsg;
    }

    if (pUnit->pMsgLast)
    {
        pUnit->pMsgLast->pNext = pMsg;
    }

    pUnit->pMsgLast = pMsg;
    UNITROOM_RefreshUnit(pUnit);
}

//D2Game.0x6FCC6150
void __fastcall sub_6FCC6150(D2UnitStrc* pUnit, int16_t nSkillId, uint8_t nSkillLevel, int16_t nX, int16_t nY, uint8_t a6)
{
    D2UnitPacketListStrc* pMsg = (D2UnitPacketListStrc*)D2_ALLOC_POOL(pUnit->pMemoryPool, 20);
    pMsg->pNext = nullptr;
    pMsg->nHeader = 0x9Au;
    *((int16_t*)pMsg + 4) = nSkillId;
    *((uint8_t*)pMsg + 10) = nSkillLevel;
    *((int16_t*)pMsg + 6) = nX;
    *((int16_t*)pMsg + 7) = nY;
    *((uint8_t*)pMsg + 16) = a6;

    if (!pUnit->pMsgFirst)
    {
        pUnit->pMsgFirst = pMsg;
    }

    if (pUnit->pMsgLast)
    {
        pUnit->pMsgLast->pNext = pMsg;
    }

    pUnit->pMsgLast = pMsg;
    UNITROOM_RefreshUnit(pUnit);
}

//D2Game.0x6FCC61D0
void __fastcall D2GAME_MERCS_SendStat_6FCC61D0(D2UnitStrc* pUnit, uint16_t nStatId, int32_t nValue)
{
    D2_ASSERT(nStatId < ((uint8_t)-1));

    D2UnitPacketListStrc* pMsg = (D2UnitPacketListStrc*)D2_ALLOC_POOL(pUnit->pMemoryPool, 20);
    pMsg->pNext = nullptr;
    pMsg->nHeader = 0x9Eu;
    *((int32_t*)pMsg + 2) = pUnit->dwUnitId;
    *((uint8_t*)pMsg + 12) = nStatId;
    *((int32_t*)pMsg + 4) = nValue;

    if (!pUnit->pMsgFirst)
    {
        pUnit->pMsgFirst = pMsg;
    }

    if (pUnit->pMsgLast)
    {
        pUnit->pMsgLast->pNext = pMsg;
    }

    pUnit->pMsgLast = pMsg;
    UNITROOM_RefreshUnit(pUnit);
}

//D2Game.0x6FCC6270
void __fastcall sub_6FCC6270(D2UnitStrc* pUnit, uint8_t a2)
{
    D2UnitPacketListStrc* pMsg = (D2UnitPacketListStrc*)D2_CALLOC_POOL(pUnit->pMemoryPool, 20);

    pMsg->pNext = nullptr;
    pMsg->nHeader = 0xABu;
    *((uint8_t*)pMsg + 8) = pUnit->dwUnitType;
    *((int32_t*)pMsg + 3) = pUnit->dwUnitId;
    *((uint8_t*)pMsg + 16) = a2;

    if (!pUnit->pMsgFirst)
    {
        pUnit->pMsgFirst = pMsg;
    }

    if (pUnit->pMsgLast)
    {
        pUnit->pMsgLast->pNext = pMsg;
    }

    pUnit->pMsgLast = pMsg;
    UNITROOM_RefreshUnit(pUnit);
}

//D2Game.0x6FCC6300
void __fastcall sub_6FCC6300(D2UnitStrc* pUnit, D2UnitStrc* pTargetUnit, int16_t nSkillId, int16_t nSkillLevel, int32_t nX, int32_t nY, uint8_t a7)
{
    D2UnitPacketListStrc* pMsg = (D2UnitPacketListStrc*)D2_CALLOC_POOL(pUnit->pMemoryPool, 40);

    pMsg->pNext = nullptr;
    pMsg->nHeader = 0xA3u;
    *((uint8_t*)pMsg + 8) = a7;
    *((int16_t*)pMsg + 6) = nSkillLevel;
    *((int16_t*)pMsg + 5) = nSkillId;
    *((int32_t*)pMsg + 4) = pUnit->dwUnitType;
    *((int32_t*)pMsg + 5) = pUnit->dwUnitId;
    *((int32_t*)pMsg + 8) = nX;
    *((int32_t*)pMsg + 9) = nY;

    if (pTargetUnit)
    {
        *((int32_t*)pMsg + 6) = pTargetUnit->dwUnitType;
        *((int32_t*)pMsg + 7) = pTargetUnit->dwUnitId;
    }
    else
    {
        *((int32_t*)pMsg + 7) = -1;
    }

    if (!pUnit->pMsgFirst)
    {
        pUnit->pMsgFirst = pMsg;
    }

    if (pUnit->pMsgLast)
    {
        pUnit->pMsgLast->pNext = pMsg;
    }

    pUnit->pMsgLast = pMsg;
    UNITROOM_RefreshUnit(pUnit);
}

//D2Game.0x6FCC63D0
void __fastcall sub_6FCC63D0(D2UnitStrc* pUnit, int16_t a2)
{
    STATES_ToggleState(pUnit, STATE_SKILL_MOVE, 0);

    D2UnitPacketListStrc* pMsg = (D2UnitPacketListStrc*)D2_CALLOC_POOL(pUnit->pMemoryPool, 20);

    pMsg->pNext = nullptr;
    pMsg->nHeader = 0xA5u;
    *((uint8_t*)pMsg + 8) = pUnit->dwUnitType;
    *((uint32_t*)pMsg + 3) = pUnit->dwUnitId;
    *((int16_t*)pMsg + 8) = a2;

    if (!pUnit->pMsgFirst)
    {
        pUnit->pMsgFirst = pMsg;
    }

    if (pUnit->pMsgLast)
    {
        pUnit->pMsgLast->pNext = pMsg;
    }

    pUnit->pMsgLast = pMsg;
    UNITROOM_RefreshUnit(pUnit);
}

//D2Game.0x6FCC6470
void __fastcall sub_6FCC6470(D2UnitStrc* pUnit, int16_t a2)
{
    D2UnitPacketListStrc* pMsg = (D2UnitPacketListStrc*)D2_ALLOC_POOL(pUnit->pMemoryPool, 12);
    pMsg->pNext = nullptr;
    pMsg->nHeader = 0xA4u;
    *((int16_t*)pMsg + 4) = a2;

    if (!pUnit->pMsgFirst)
    {
        pUnit->pMsgFirst = pMsg;
    }

    if (pUnit->pMsgLast)
    {
        pUnit->pMsgLast->pNext = pMsg;
    }

    pUnit->pMsgLast = pMsg;
    UNITROOM_RefreshUnit(pUnit);
}

//D2Game.0x6FCC64D0
void __fastcall sub_6FCC64D0(D2UnitStrc* pUnit, uint8_t bLeftSkill, int16_t nSkillId, int32_t nOwnerGUID)
{
    D2UnitPacketListStrc* pMsg = (D2UnitPacketListStrc*)D2_ALLOC_POOL(pUnit->pMemoryPool, 16);
    pMsg->pNext = nullptr;
    pMsg->nHeader = 0x23u;
    *((int32_t*)pMsg + 2) = nOwnerGUID;
    *((int16_t*)pMsg + 6) = nSkillId;
    *((uint8_t*)pMsg + 14) = bLeftSkill;

    if (!pUnit->pMsgFirst)
    {
        pUnit->pMsgFirst = pMsg;
    }

    if (pUnit->pMsgLast)
    {
        pUnit->pMsgLast->pNext = pMsg;
    }

    pUnit->pMsgLast = pMsg;
    UNITROOM_RefreshUnit(pUnit);
}

//D2Game.0x6FCC6540
void __fastcall sub_6FCC6540(D2UnitStrc* pUnit, D2ClientStrc* pClient)
{
    for (D2UnitPacketListStrc* pMsg = pUnit->pMsgFirst; pMsg; pMsg = pMsg->pNext)
    {
        int32_t i = (int32_t)pMsg;
        switch (pMsg->nHeader)
        {
        case 0x23:
            D2GAME_PACKETS_SendPacket0x23_6FC3DC60(pClient, pUnit->dwUnitType, pUnit->dwUnitId, *(uint8_t*)(i + 14), *(uint16_t*)(i + 12), *(uint32_t*)(i + 8));
            break;

        case 0x99:
            sub_6FC3F0C0(pClient, pUnit->dwUnitType, pUnit->dwUnitId, *(uint8_t*)(i + 11), *(uint32_t*)(i + 12), *(uint16_t*)(i + 8), *(uint8_t*)(i + 16), *(uint8_t*)(i + 10), 1);
            break;

        case 0x9A:
            sub_6FC3F060(pClient, pUnit->dwUnitType, pUnit->dwUnitId, *(uint16_t*)(i + 12), *(uint16_t*)(i + 14), *(uint16_t*)(i + 8), *(uint8_t*)(i + 16), *(uint8_t*)(i + 10), 1);
            break;

        case 0x9E:
            D2GAME_PACKETS_SendPacket0x9E_9F_A0_6FC3D520(pClient, pUnit, *(uint8_t*)(i + 12), *(uint32_t*)(i + 16));
            break;

        case 0xA1:
            D2GAME_PACKETS_SendPacket0xA0_A1_A2_6FC3D610(pClient, pUnit, *(uint8_t*)(i + 12), *(uint32_t*)(i + 16), *(uint32_t*)(i + 20));
            break;

        case 0xA3:
            D2GAME_PACKETS_SendPacket0xA3_6FC3D730(pClient, *(uint8_t*)(i + 8), *(uint16_t*)(i + 10), *(uint16_t*)(i + 12), *(uint32_t*)(i + 16), *(uint32_t*)(i + 20), *(uint32_t*)(i + 24), *(uint32_t*)(i + 28), *(uint32_t*)(i + 32), *(uint32_t*)(i + 36));
            break;

        case 0xA4:
            D2GAME_PACKETS_SendPacket0xA4_6FC3FC00(pClient, *(uint16_t*)(i + 8));
            break;

        case 0xA5:
            D2GAME_PACKETS_SendPacket0xA5_6FC3D7F0(pClient, *(uint8_t*)(i + 8), *(uint32_t*)(i + 12), *(uint16_t*)(i + 16));
            break;

        case 0xAB:
            if (!(pUnit->dwFlags & UNITFLAG_UPGRLIFENHITCLASS))
            {
                D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
                if (pPlayer && sub_6FCBD900(pPlayer->pGame, pPlayer, pUnit))
                {
                    D2GAME_PACKETS_SendPacket0xAB_6FC3D7B0(pClient, *(uint8_t*)(i + 8), *(uint32_t*)(i + 12), *(uint8_t*)(i + 16));
                }
            }
            break;

        default:
            break;
        }
    }
}

//D2Game.0x6FCC6790
void __fastcall D2GAME_SUNITMSG_FreeUnitMessages_6FCC6790(D2UnitStrc* pUnit)
{
    D2UnitPacketListStrc* pMsg = pUnit->pMsgFirst;
    if (pMsg)
    {
        do
        {
            D2UnitPacketListStrc* pNext = pMsg->pNext;
            D2_FREE_POOL(pUnit->pMemoryPool, pMsg);
            pMsg = pNext;
        }
        while (pMsg);

        pUnit->pMsgLast = nullptr;
        pUnit->pMsgFirst = nullptr;
    }
}
