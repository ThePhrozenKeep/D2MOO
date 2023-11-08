#include "GAME/SCmd.h"

#include <intrin.h>
#include <algorithm>

#include <D2BitManip.h>
#include <Fog.h>
#include <Storm.h>

// D2Net
#include <Server.h>

#include "D2PacketDef.h"

#include <D2Items.h>
#include <D2QuestRecord.h>
#include <D2DataTbls.h>
#include <D2Skills.h>
#include <D2States.h>
#include <D2StatList.h>
#include <D2Dungeon.h>
#include <D2Monsters.h>

#include "AI/AiGeneral.h"
#include "GAME/Arena.h"
#include "GAME/Clients.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterUnique.h"
#include "OBJECTS/Objects.h"
#include "PLAYER/PlayerList.h"
#include "PLAYER/PlayerPets.h"
#include "SKILLS/Skills.h"
#include "UNIT/Party.h"
#include "UNIT/SUnit.h"


// Helper function
static PacketStatId PACKET_StatToPacketStatId(uint16_t wStat)
{
#if PACKETS_USE_16BITS_STATID
    return wStat;
#else
    D2_ASSERT(wStat <= 255);
    return (uint8_t)wStat;
#endif
}

//D2Game.0x6FC3C640
int32_t __fastcall sub_6FC3C640(int32_t nClientId, int16_t a2, int16_t a3, const char* szSource)
{
    D2GSPacketSrvB1 packetB1 = {};

    packetB1.nHeader = 0xB1;
    packetB1.unk0x33 = a2;
    packetB1.unk0x31 = a3;

    SStrCopy(packetB1.unk0x01, szSource, 0x10u);
    
    return D2NET_10006(0, nClientId, &packetB1, sizeof(packetB1));
}

//D2Game.0x6FC3C690
void __fastcall sub_6FC3C690(int32_t nClientId)
{
    D2GSPacketSrvAF packetAF = {};

    packetAF.nHeader = 0xAF;

    D2NET_10006(0, nClientId, &packetAF, sizeof(packetAF));
}

//D2Game.0x6FC3C6B0
void __fastcall sub_6FC3C6B0(int32_t nClientId)
{
    D2GSPacketSrv06 packet06 = {};

    packet06.nHeader = 0x06;

    D2NET_10006(1, nClientId, &packet06, sizeof(packet06));
}

//D2Game.0x6FC3C6D0
void __fastcall sub_6FC3C6D0(int32_t nClientId, uint32_t nErrorCode)
{
    D2GSPacketSrvB3 packetB3 = {};

    packetB3.nHeader = 0xB3;
    packetB3.nErrorCode = nErrorCode;
    
    D2NET_10006(0, nClientId, &packetB3, sizeof(packetB3));
}

//D2Game.0x6FC3C6F0
//It seems there were multiple functions for header only packets that got merged by the linker into a single one.
void __fastcall D2GAME_PACKETS_SendHeaderOnlyPacket(D2ClientStrc* pClient, uint8_t nHeader)
{
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &nHeader, sizeof(nHeader));
}

//D2Game.0x6FC3C710
void __fastcall D2GAME_PACKETS_SendPacket_6FC3C710(D2ClientStrc* pClient, void* pPacket, int32_t nPacketSize)
{
    if (!pClient)
    {
        return;
    }

    D2PacketDataStrc* pPacketData = CLIENTS_PacketDataList_GetTail(pClient);
    int32_t nSize = 0;
    if (pPacketData)
    {
        nSize = pPacketData->nPacketSize;
    }

    if (nPacketSize + nSize > 512 || !pPacketData)
    {
        pPacketData = pClient->tPacketDataList.pPacketDataPool;
        nSize = 0;
        if (pPacketData)
        {
            pClient->tPacketDataList.pPacketDataPool = pPacketData->pNext;
        }
        else
        {
            pPacketData = D2_ALLOC_STRC_POOL(nullptr, D2PacketDataStrc);
        }

        memset(pPacketData, 0x00, sizeof(D2PacketDataStrc));
        CLIENTS_PacketDataList_Append(pClient, pPacketData);
    }

    memcpy(&pPacketData->packetData[nSize], pPacket, nPacketSize);

    pPacketData->nPacketSize += nPacketSize;
}

//D2Game.0x6FC3C7C0
void __fastcall D2GAME_PACKETS_SendPacket0x01_6FC3C7C0(D2ClientStrc* pClient, char nHeader, D2GameStrc* pGame)
{
    D2GSPacketSrv01 packet01 = {};

    packet01.nHeader = nHeader;
    packet01.nDifficulty = pGame->nDifficulty;
    packet01.dwArenaFlags = ARENA_GetFlags(pGame);
    packet01.nExpansion = pGame->bExpansion != 0;
    packet01.nLadder = pGame->dwGameType != 0;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet01, sizeof(packet01));
}

//D2Game.0x6FC3C810
void __fastcall D2GAME_PACKETS_SendPacket0x03_6FC3C810(D2ClientStrc* pClient, char nHeader, char nAct, int32_t nInitSeed, int32_t nObjectSeed, int16_t nTownId)
{
    D2GSPacketSrv03 packet03 = {};

    packet03.nHeader = nHeader;
    packet03.nAct = nAct;
    packet03.dwGameSeed = nInitSeed;
    packet03.nTownLevelId = nTownId;
    packet03.nObjectSeed = nObjectSeed;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet03, sizeof(packet03));
}

//D2Game.0x6FC3C850
void __fastcall D2GAME_PACKETS_SendPacketSize06_6FC3C850(D2ClientStrc* pClient, DWORD nHeader, DWORD dwUnitType, DWORD dwUnitId)
{
    uint8_t packet[6] = {};

    packet[0] = nHeader;
    packet[1] = dwUnitType;
    *(uint32_t*)&packet[2] = dwUnitId;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, packet, sizeof(packet));
}

//D2Game.0x6FC3C880
void __fastcall D2GAME_SendPacketSize05_6FC3C880(D2ClientStrc* pClient, char nHeader, int32_t nArg)
{
    uint8_t packet[5] = {};

    packet[0] = nHeader;
    *(uint32_t*)&packet[1] = nArg;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, packet, sizeof(packet));
}

//D2Game.0x6FC3C8A0
void __fastcall D2GAME_PACKETS_SendPacket0x0C_6FC3C8A0(D2ClientStrc* pClient, char nHeader, D2C_UnitTypes nUnitType, int32_t nUnitGUID, char a5, char nHitClass, char nLifePct)
{
    D2GSPacketSrv0C packet0C = {};

    packet0C.nHeader = nHeader;
    packet0C.nUnitType = nUnitType;
    packet0C.dwUnitGUID = nUnitGUID;
    packet0C.unk0x06 = a5;
    packet0C.nHitClass = nHitClass;
    packet0C.nLife = nLifePct;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet0C, sizeof(packet0C));
}

//D2Game.0x6FC3C8E0
void __fastcall D2GAME_PACKETS_SendPacket0x0E_6FC3C8E0(D2ClientStrc* pClient, char nHeader, char a3, int32_t a4, char a5, char a6, int32_t nAnimMode)
{
    D2GSPacketSrv0E packet0E = {};

    packet0E.nHeader = nHeader;
    packet0E.nUnitType = a3;
    packet0E.dwUnitGUID = a4;
    packet0E.unk0x06 = a5;
    packet0E.unk0x07 = a6;
    packet0E.unk0x08 = nAnimMode;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet0E, sizeof(packet0E));
}

//D2Game.0x6FC3C920
void __fastcall D2GAME_PACKETS_SendPacket0x0D_6FC3C920(D2ClientStrc* pClient, char nPacketId, int32_t nUnitType, int32_t nUnitId, char a5, WORD nX, WORD nY, char a8, char nLife)
{
    D2GSPacketSrv0D packet0D = {};

    packet0D.nHeader = nPacketId;
    packet0D.dwUnitGUID = nUnitId;
    packet0D.unk0x06 = a5;
    packet0D.unk0x07 = nX;
    packet0D.unk0x09 = nY;
    packet0D.unk0x0B = a8;
    packet0D.nUnitType = nUnitType;
    packet0D.unk0x0C = nLife;

    if ((uint8_t)nUnitType >= 6u)
    {
        FOG_DisplayAssert("bTOU < NUM_UNITTYPE", __FILE__, __LINE__);
        exit(-1);
    }

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet0D, sizeof(packet0D));
}

//D2Game.0x6FC3C9A0
void __fastcall D2GAME_PACKETS_SendPacket0x10_6FC3C9A0(D2ClientStrc* pClient, char nHeader, char a3, int32_t a4, char a5, char a6, int32_t a7, int16_t a8, int16_t a9)
{
    D2GSPacketSrv10 packet10 = {};

    packet10.nHeader = nHeader;
    packet10.nUnitType = a3;
    packet10.dwUnitGUID = a4;
    packet10.nFlags = a5;
    packet10.nTargetType = a6;
    packet10.dwTargetGUID = a7;
    packet10.nCurrentPosX = a8;
    packet10.nCurrentPosY = a9;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet10, sizeof(packet10));
}

//D2Game.0x6FC3CA00
void __fastcall D2GAME_PACKETS_SendPacket0x0F_6FC3CA00(D2ClientStrc* pClient, char nHeader, uint8_t bTOU, int32_t a4, char a5, int16_t a6, int16_t a7, char a8, int16_t a9, int16_t a10)
{
    D2GSPacketSrv0F packet0F = {};

    packet0F.nHeader = nHeader;
    packet0F.nUnitType = bTOU;
    packet0F.dwUnitGUID = a4;
    packet0F.nFlags = a5;
    packet0F.nDestPosX = a6;
    packet0F.nDestPosY = a7;
    packet0F.unk0x00B = a8;
    packet0F.nCurrentPosX = a9;
    packet0F.nCurrentPosY = a10;

    if (bTOU >= 6u)
    {
        D2_ASSERT(bTOU < UNIT_TYPES_COUNT);
        exit(-1);
    }

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet0F, sizeof(packet0F));
}

//D2Game.0x6FC3CA90
void __fastcall D2GAME_PACKETS_SendPacket0x68_6FC3CA90(D2ClientStrc* pClient, uint8_t nHeader, int32_t nUnitGUID, int8_t a4, int8_t a5, int32_t a6, int8_t a7)
{
    D2GSPacketSrv68 packet68 = {};

    packet68.nHeader = nHeader;
    packet68.nUnitGUID = nUnitGUID;
    packet68.unk0x05 = a4;
    packet68.unk0x0A = a5;
    packet68.unk0x10 = a6;

    D2UnitStrc* pMonster = SUNIT_GetServerUnit(CLIENTS_GetGame(pClient), UNIT_MONSTER, nUnitGUID);

    packet68.nX = CLIENTS_GetUnitX(pMonster);
    packet68.nY = CLIENTS_GetUnitY(pMonster);

    packet68.unk0x0F = a7;
    packet68.nPathType = PATH_GetType(pMonster->pDynamicPath);
    if (packet68.nPathType >= 5u)
    {
        if (packet68.nPathType <= 6u)
        {
            packet68.nPathType = 2;
        }
        else if (packet68.nPathType == 8)
        {
            packet68.nPathType = 11;
        }
    }

    int32_t nVelocity = STATLIST_UnitGetStatValue(pMonster, STAT_VELOCITYPERCENT, 0);
    if (nVelocity < SHRT_MIN)
    {
        nVelocity = SHRT_MIN;
    }
    else if (nVelocity >= SHRT_MAX)
    {
        nVelocity = SHRT_MAX;
    }

    packet68.nVelocity = nVelocity;
    packet68.unk0x14 = PATH_GetMaxDistance(pMonster->pDynamicPath);
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet68, sizeof(packet68));
}

//D2Game.0x6FC3CBC0
void __fastcall D2GAME_PACKETS_SendPacket0x67_6FC3CBC0(D2ClientStrc* pClient, uint8_t nHeader, int32_t nUnitGUID, int8_t a4, int16_t nX, int16_t nY, int8_t a7)
{
    D2GSPacketSrv67 packet67 = {};

    packet67.nHeader = nHeader;
    packet67.nUnitGUID = nUnitGUID;
    packet67.unk0x05 = a4;
    packet67.nX = nX;
    packet67.nY = nY;

    D2UnitStrc* pMonster = SUNIT_GetServerUnit(CLIENTS_GetGame(pClient), UNIT_MONSTER, nUnitGUID);
    if (!pMonster)
    {
        return;
    }

    packet67.unk0x0A = a7;
    packet67.nPathType = PATH_GetType(pMonster->pDynamicPath);
    if (packet67.nPathType >= 5u)
    {
        if (packet67.nPathType <= 6u)
        {
            packet67.nPathType = 1;
            packet67.nX = D2COMMON_10177_PATH_GetLastPointX(pMonster->pDynamicPath);
            packet67.nY = D2COMMON_10178_PATH_GetLastPointY(pMonster->pDynamicPath);
        }
        else if (packet67.nPathType == 8)
        {
            packet67.nPathType = 0xBu;
        }
    }

    int32_t nVelocity = STATLIST_UnitGetStatValue(pMonster, STAT_VELOCITYPERCENT, 0);
    if (nVelocity < SHRT_MIN)
    {
        nVelocity = SHRT_MIN;
    }
    else if (nVelocity >= SHRT_MAX)
    {
        nVelocity = SHRT_MAX;
    }

    packet67.nVelocity = nVelocity;
    packet67.unk0x0F = PATH_GetMaxDistance(pMonster->pDynamicPath);
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet67, sizeof(packet67));
}

//D2Game.0x6FC3CCB0
void __fastcall D2GAME_PACKETS_SendPacket0x68_6FC3CCB0(D2ClientStrc* pClient, uint8_t nHeader, int32_t nUnitGUID, int8_t a4, int8_t a5, int32_t a6, int8_t a7, int8_t a8, int8_t a9)
{
    D2GSPacketSrv68 packet68 = {};

    packet68.nHeader = nHeader;
    packet68.nUnitGUID = nUnitGUID;
    packet68.unk0x05 = a4;
    packet68.unk0x0A = a5;
    packet68.unk0x0B = a6;

    D2UnitStrc* pMonster = SUNIT_GetServerUnit(CLIENTS_GetGame(pClient), UNIT_MONSTER, nUnitGUID);
    if (!pMonster)
    {
        return;
    }

    packet68.nX = CLIENTS_GetUnitX(pMonster);
    packet68.nY = CLIENTS_GetUnitY(pMonster);
    packet68.unk0x0F = a7;
    packet68.unk0x10 = a9;
    packet68.nPathType = PATH_GetType(pMonster->pDynamicPath);
    if (packet68.nPathType == 8)
    {
        packet68.nPathType = 11;
    }

    int32_t nVelocity = STATLIST_UnitGetStatValue(pMonster, STAT_VELOCITYPERCENT, 0);
    if (nVelocity < SHRT_MIN)
    {
        nVelocity = SHRT_MIN;
    }
    else if (nVelocity >= SHRT_MAX)
    {
        nVelocity = SHRT_MAX;
    }

    packet68.nVelocity = nVelocity;
    packet68.unk0x14 = a8;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet68, sizeof(packet68));
}

//D2Game.0x6FC3CDE0
void __fastcall D2GAME_PACKETS_SendPacket0x67_6FC3CDE0(D2ClientStrc* pClient, uint8_t nHeader, int32_t nUnitGUID, int8_t a4, int16_t nX, int16_t nY, int8_t a7, int8_t a8, int8_t a9)
{
    D2GSPacketSrv67 packet67 = {};

    packet67.nHeader = nHeader;
    packet67.nUnitGUID = nUnitGUID;
    packet67.unk0x05 = a4;
    packet67.nX = nX;
    packet67.nY = nY;

    D2UnitStrc* pMonster = SUNIT_GetServerUnit(CLIENTS_GetGame(pClient), UNIT_MONSTER, nUnitGUID);
    if (!pMonster)
    {
        return;
    }

    packet67.unk0x0A = a7;
    packet67.unk0x0B = a9;
    packet67.nPathType = PATH_GetType(pMonster->pDynamicPath);
    if (packet67.nPathType >= 5u)
    {
        if (packet67.nPathType <= 6u)
        {
            packet67.nPathType = 1;
            packet67.nX = D2COMMON_10177_PATH_GetLastPointX(pMonster->pDynamicPath);
            packet67.nY = D2COMMON_10178_PATH_GetLastPointY(pMonster->pDynamicPath);
        }
        else if (packet67.nPathType == 8)
        {
            packet67.nPathType = 11;
        }
    }

    int32_t nVelocity = STATLIST_UnitGetStatValue(pMonster, STAT_VELOCITYPERCENT, 0);
    if (nVelocity < SHRT_MIN)
    {
        nVelocity = SHRT_MIN;
    }
    else if (nVelocity >= SHRT_MAX)
    {
        nVelocity = SHRT_MAX;
    }

    packet67.nVelocity = nVelocity;
    packet67.unk0x0F = a8;
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet67, sizeof(packet67));
}

//D2Game.0x6FC3CEE0
void __fastcall D2GAME_PACKETS_SendPacket0x6A_6FC3CEE0(D2ClientStrc* pClient, char a2, int32_t nUnitGUID, char a4, char a5, int32_t a6, char nDirection)
{
    D2GSPacketSrv6A packet6A = {};

    packet6A.nHeader = a2;
    packet6A.nUnitGUID = nUnitGUID;
    packet6A.unk0x05 = a4;
    packet6A.nTargetUnitType = a5;
    packet6A.nTargetUnitGuid = a6;
    packet6A.nDirection = nDirection;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet6A, sizeof(packet6A));
}

//D2Game.0x6FC3CF30
void __fastcall D2GAME_PACKETS_SendPacket0x69_6FC3CF30(D2ClientStrc* pClient, char a2, int32_t a3, char a4, int16_t a5, int16_t a6, char a7, char a8)
{
    D2GSPacketSrv69 packet69 = {};

    packet69.nHeader = a2;
    packet69.unk0x01 = a3;
    packet69.unk0x05 = a4;
    packet69.unk0x06 = a5;
    packet69.unk0x08 = a6;
    packet69.unk0x0A = a7;
    packet69.unk0x0B = a8;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet69, sizeof(packet69));
}

//D2Game.0x6FC3CF90
void __fastcall D2GAME_PACKETS_SendPacket0x6C_6FC3CF90(D2ClientStrc* pClient, char a2, int32_t a3, char a4, char a5, int32_t a6, char a7, int16_t a8, int16_t a9)
{
    D2GSPacketSrv6C packet6C = {};

    packet6C.nHeader = a2;
    packet6C.unk0x01 = a3;
    packet6C.unk0x05 = a4;
    packet6C.unk0x06 = a5;
    packet6C.unk0x07 = a6;
    packet6C.unk0x0B = a7;
    packet6C.unk0x0C = a8;
    packet6C.unk0x0E = a9;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet6C, sizeof(packet6C));
}

//D2Game.0x6FC3D000
void __fastcall D2GAME_PACKETS_SendPacket0x6B_6FC3D000(D2ClientStrc* pClient, char a2, int32_t a3, char a4, int16_t a5, int16_t a6, char a7, char a8, int16_t a9, int16_t a10)
{
    D2GSPacketSrv6B packet6B = {};

    packet6B.nHeader = a2;
    packet6B.unk0x01 = a3;
    packet6B.unk0x05 = a4;
    packet6B.unk0x06 = a5;
    packet6B.unk0x08 = a6;
    packet6B.unk0x0A = a7;
    packet6B.unk0x0B = a8;
    packet6B.unk0x0C = a9;
    packet6B.unk0x0E = a10;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet6B, sizeof(packet6B));
}

//D2Game.0x6FC3D080
void __fastcall D2GAME_PACKETS_SendPacket0x6D_6FC3D080(D2ClientStrc* pClient, DWORD dwUnitId, WORD nX, WORD nY, BYTE nUnitLife)
{
    D2GSPacketSrv6D packet6D = {};

    packet6D.nHeader = 0x6Du;
    packet6D.unk0x01 = dwUnitId;
    packet6D.unk0x05 = nX;
    packet6D.unk0x07 = nY;
    packet6D.unk0x09 = nUnitLife;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet6D, sizeof(packet6D));
}

//D2Game.0x6FC3D0D0
void __fastcall D2GAME_PACKETS_SendPacket0x15_6FC3D0D0(D2ClientStrc* pClient, char a2, char a3, int32_t a4, int16_t a5, int16_t a6, char a7)
{
    D2GSPacketSrv15 packet15 = {};

    packet15.nHeader = a2;
    packet15.nUnitType = a3;
    packet15.dwUnitGUID = a4;
    packet15.nPosX = a5;
    packet15.nPosY = a6;
    packet15.unk0x00A = a7;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet15, sizeof(packet15));
}

//D2Game.0x6FC3D120
void __fastcall D2GAME_PACKETS_SendPacket0x07_6FC3D120(D2ClientStrc* pClient, uint8_t nAreaId, uint16_t nTileX, uint16_t nTileY)
{
    D2GSPacketSrv07 packet07 = {};

    packet07.nHeader = 0x07;
    packet07.nX = nTileX;
    packet07.nY = nTileY;
    packet07.nLevelId = nAreaId;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet07, sizeof(packet07));
}

//D2Game.0x6FC3D160
void __fastcall D2GAME_PACKETS_SendPacket0x08_6FC3D160(D2ClientStrc* pClient, uint8_t nAreaId, uint16_t nTileX, uint16_t nTileY)
{
    D2GSPacketSrv08 packet08 = {};

    packet08.nHeader = 0x08;
    packet08.nX = nTileX;
    packet08.nY = nTileY;
    packet08.nLevelId = nAreaId;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet08, sizeof(packet08));
}

//D2Game.0x6FC3D1A0
void __fastcall D2GAME_PACKETS_SendPacket0x09_6FC3D1A0(D2ClientStrc* pClient, BYTE a2, BYTE a3, DWORD a4, BYTE a5, WORD a6, WORD a7)
{
    D2GSPacketSrv09 packet09 = {};

    packet09.nHeader = a2;
    packet09.nUnitType = a3;
    packet09.dwUnitGUID = a4;
    packet09.unk0x06 = a5;
    packet09.nPosX = a6;
    packet09.nPosY = a7;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet09, sizeof(packet09));
}

//D2Game.0x6FC3D1F0
void __fastcall sub_6FC3D1F0(D2ClientStrc* pClient, int32_t nUnitGUID, uint8_t nClassId, const char* szName, int16_t nX, int16_t nY)
{
    D2GSPacketSrv59 packet59 = {};

    packet59.nHeader = 0x59u;
    packet59.dwGUID = nUnitGUID;
    packet59.nClass = nClassId;
    SStrCopy(packet59.szName, szName, INT_MAX);
    packet59.nPosX = nX;
    packet59.nPosY = nY;
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet59, sizeof(packet59));

    D2UnitStrc* pLocalPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    D2UnitStrc* pOtherPlayer = SUNIT_GetServerUnit(CLIENTS_GetGame(pClient), UNIT_PLAYER, nUnitGUID);
    if (pOtherPlayer)
    {
        SCMD_Send0x75_PartyRosterUpdate(pLocalPlayer, pOtherPlayer);
    }
}

//D2Game.0x6FC3D300
void __fastcall D2GAME_PACKETS_SendPacket0x51_6FC3D300(D2ClientStrc* pClient, char nHeader, uint8_t nUnitType, int32_t nUnitGUID, int16_t nObjectId, int16_t nX, int16_t nY, char a8, char a9)
{
    D2_ASSERT(SUNIT_GetServerUnit(CLIENTS_GetGame(pClient), nUnitType, nUnitGUID));

    D2GSPacketSrv51 packet51 = {};

    packet51.nHeader = nHeader;
    packet51.nUnitType = nUnitType;
    packet51.dwUnitGUID = nUnitGUID;
    packet51.nObject = nObjectId;
    packet51.nPosX = nX;
    packet51.nPosY = nY;
    packet51.unk0x0C[0] = a8;
    packet51.unk0x0C[1] = a9;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet51, sizeof(packet51));
}

//D2Game.0x6FC3D3A0
void __fastcall D2GAME_PACKETS_SendPacket0x0A_RemoveObject_6FC3D3A0(D2ClientStrc* pClient, char alw0x0A, char nUnitType, int32_t nUnitId)
{
    if (nUnitType != UNIT_MISSILE)
    {
        D2GSPacketSrv0A packet0A = {};
        
        packet0A.nUnitType = nUnitType;
        packet0A.nHeader = alw0x0A;
        packet0A.dwUnitGUID = nUnitId;

        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet0A, sizeof(packet0A));
    }
}

//D2Game.0x6FC3D3D0
void __fastcall D2GAME_PACKETS_SendPacket0x19_6FC3D3D0(D2ClientStrc* pClient, int32_t nValue, int32_t a3)
{
    const int32_t nDiff = nValue - a3;
    if (nDiff <= 0 || nDiff >= ((1 << 8) - 1))
    {
        D2GAME_PACKETS_SendPacket0x1D_E_F_6FC3D480(pClient, STAT_GOLD, nValue);
    }
    else
    {
        D2GSPacketSrv19 packet19 = {};
        packet19.nHeader = 0x19;
        packet19.nGold = nDiff;

        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet19, sizeof(packet19));
    }
}

//D2Game.0x6FC3D410
void __fastcall D2GAME_PACKETS_SendPacket0x1A_B_C_6FC3D410(D2ClientStrc* pClient, int32_t nExperience, int32_t a3)
{
    const int32_t nDiff = nExperience - a3;

    if (nDiff < 0 || nDiff >= ((1 << 16) - 1))
    {
        D2GSPacketSrv1C packet1C = {};

        packet1C.nHeader = 0x1C;
        packet1C.nExperience = nExperience;

        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet1C, sizeof(packet1C));
    }
    else
    {
        if (nDiff >= ((1 << 8) - 1))
        {
            D2GSPacketSrv1B packet1B = {};

            packet1B.nHeader = 0x1B;
            packet1B.nExperience = nExperience;

            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet1B, sizeof(packet1B));
        }
        else
        {
            D2GSPacketSrv1A packet1A = {};

            packet1A.nHeader = 0x1A;
            packet1A.nExperience = nExperience;

            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet1A, sizeof(packet1A));
        }
    }
}

//D2Game.0x6FC3D480
void __fastcall D2GAME_PACKETS_SendPacket0x1D_E_F_6FC3D480(D2ClientStrc* pClient, uint16_t nStatId, uint32_t nValue)
{
    const PacketStatId nPacketStatId = PACKET_StatToPacketStatId(nStatId);
    if (nValue >= ((1 << 8) - 1))
    {
        if (nValue >= ((1 << 16) - 1))
        {
            D2GSPacketSrv1F packet1F = {};
            packet1F.nHeader = 0x1F;
            packet1F.nStat = nPacketStatId;
            packet1F.nValue = nValue;

            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet1F, sizeof(packet1F));
        }
        else
        {
            D2GSPacketSrv1E packet1E = {};
            packet1E.nHeader = 0x1E;
            packet1E.nStat = nPacketStatId;
            packet1E.nValue = nValue;

            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet1E, sizeof(packet1E));
        }
    }
    else
    {
        D2GSPacketSrv1D packet1D = {};
        packet1D.nHeader = 0x1D;
        packet1D.nStat = nPacketStatId;
        packet1D.nValue = nValue;

        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet1D, sizeof(packet1D));
    }
}

//D2Game.0x6FC3D520
void __fastcall D2GAME_PACKETS_SendPacket0x9E_9F_A0_6FC3D520(D2ClientStrc* pClient, D2UnitStrc* pUnit, uint16_t nStatId, uint32_t nValue)
{
    const PacketStatId nPacketStatId = PACKET_StatToPacketStatId(nStatId);

    if (nValue >= ((1 << 8) - 1))
    {
        if (nValue >= ((1 << 16) - 1))
        {
            D2GSPacketSrvA0 packetA0 = {};
            packetA0.nHeader = 0xA0;
            packetA0.nStat = nPacketStatId;
            packetA0.dwGUID = pUnit ? pUnit->dwUnitId : -1;
            packetA0.nValue = nValue;

            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetA0, sizeof(packetA0));
        }
        else
        {
            D2GSPacketSrv9F packet9F = {};
            packet9F.nHeader = 0x9F;
            packet9F.nStat = nPacketStatId;
            packet9F.dwGUID = pUnit ? pUnit->dwUnitId : -1;
            packet9F.nValue = nValue;

            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet9F, sizeof(packet9F));
        }
    }
    else
    {
        D2GSPacketSrv9E packet9E = {};
        packet9E.nHeader = 0x9E;
        packet9E.nStat = nPacketStatId;
        packet9E.dwGUID = pUnit ? pUnit->dwUnitId : -1;
        packet9E.nValue = nValue;

        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet9E, sizeof(packet9E));
    }
}

//D2Game.0x6FC3D610
void __fastcall D2GAME_PACKETS_SendPacket0xA0_A1_A2_6FC3D610(D2ClientStrc* pClient, D2UnitStrc* pUnit, uint16_t nStatId, uint32_t nBaseValue, uint32_t nFullValue)
{
    const PacketStatId nPacketStatId = PACKET_StatToPacketStatId(nStatId);

    const uint32_t nDiff = nFullValue - nBaseValue;
    if (nDiff >= ((1 << 8) - 1))
    {
        if (nDiff >= ((1 << 16) - 1))
        {
            D2GSPacketSrvA0 packetA0 = {};
            packetA0.nHeader = 0xA0;
            packetA0.nStat = nPacketStatId;
            packetA0.dwGUID = pUnit ? pUnit->dwUnitId : -1;
            packetA0.nValue = nDiff;

            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetA0, sizeof(packetA0));
        }
        else
        {
            D2GSPacketSrvA2 packetA2 = {};
            packetA2.nHeader = 0xA2;
            packetA2.nStat = nPacketStatId;
            packetA2.dwGUID = pUnit ? pUnit->dwUnitId : -1;
            packetA2.nValue = nDiff;

            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetA2, sizeof(packetA2));
        }
    }
    else
    {
        D2GSPacketSrvA1 packetA1 = {};
        packetA1.nHeader = 0xA1;
        packetA1.nStat = nPacketStatId;
        packetA1.dwGUID = pUnit ? pUnit->dwUnitId : -1;
        packetA1.nValue = nDiff;

        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetA1, sizeof(packetA1));
    }
}

//D2Game.0x6FC3D730
void __fastcall D2GAME_PACKETS_SendPacket0xA3_6FC3D730(D2ClientStrc* pClient, BYTE a2, WORD a3, WORD a4, BYTE a5, int32_t a6, BYTE a7, int32_t a8, int32_t a9, int32_t a10)
{
    D2GSPacketSrvA3 packetA3 = {};

    packetA3.nHeader = 0xA3u;
    packetA3.unk0x01 = a2;
    packetA3.unk0x02 = a3;
    packetA3.unk0x04 = a4;
    packetA3.unk0x06 = a5;
    packetA3.unk0x07 = a6;
    packetA3.unk0x0B = a7;
    packetA3.unk0x0C = a8;
    packetA3.unk0x10 = a9;
    packetA3.unk0x14 = a10;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetA3, sizeof(packetA3));
}

//D2Game.0x6FC3D7B0
void __fastcall D2GAME_PACKETS_SendPacket0xAB_6FC3D7B0(D2ClientStrc* pClient, uint8_t nUnitType, int32_t dwUnitId, uint8_t nUnitLife)
{
    D2GSPacketSrvAB packetAB = {};

    packetAB.nHeader = 0xAB;
    packetAB.nUnitType = nUnitType;
    packetAB.dwUnitGUID = dwUnitId;
    packetAB.nLife = nUnitLife;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetAB, sizeof(packetAB));
}

//D2Game.0x6FC3D7F0
void __fastcall D2GAME_PACKETS_SendPacket0xA5_6FC3D7F0(D2ClientStrc* pClient, uint8_t a2, int32_t a3, uint16_t a4)
{
    D2GSPacketSrvA5 packetA5 = {};

    packetA5.nHeader = 0xA5;
    packetA5.unk0x01 = a2;
    packetA5.unk0x02 = a3;
    packetA5.unk0x06 = a4;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetA5, sizeof(packetA5));
}

//D2Game.0x6FC3D830
void __fastcall sub_6FC3D830(D2ClientStrc* pClient, BYTE nHeader, int32_t nUnitGUID, WORD nStatId, int32_t nValue)
{
    const PacketStatId nPacketStatId = PACKET_StatToPacketStatId(nStatId);

    D2GSPacketSrv20 packet20 = {};

    packet20.nHeader = nHeader;
    packet20.dwUnitGUID = nUnitGUID;
    packet20.nStat = nPacketStatId;
    packet20.nValue = nValue;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet20, sizeof(packet20));
}

//D2Game.0x6FC3D890
void __fastcall sub_6FC3D890(D2ClientStrc* pClient, uint16_t a2, uint16_t a3, uint16_t a4, uint8_t a5, uint8_t a6, uint16_t a7, uint16_t a8, uint8_t a9, uint8_t a10)
{
    D2GSPacketSrv18 packet18 = {};
    D2BitBufferStrc bitBuffer = {};

    BITMANIP_Initialize(&bitBuffer, &packet18.nHeader, sizeof(D2GSPacketSrv18));

    BITMANIP_Write(&bitBuffer, 0x18, 8);
    BITMANIP_Write(&bitBuffer, a2, 15);
    BITMANIP_Write(&bitBuffer, a3, 15);
    BITMANIP_Write(&bitBuffer, a4, 15);
    BITMANIP_Write(&bitBuffer, a5, 7);
    BITMANIP_Write(&bitBuffer, a6, 7);
    BITMANIP_Write(&bitBuffer, a7, 16);
    BITMANIP_Write(&bitBuffer, a8, 16);
    BITMANIP_Write(&bitBuffer, a9, 8);
    BITMANIP_Write(&bitBuffer, a10, 8);

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet18, BITMANIP_GetSize(&bitBuffer));
}

//D2Game.0x6FC3D9A0
void __fastcall sub_6FC3D9A0(D2ClientStrc* pClient, uint16_t a2, uint16_t a3, uint16_t a4, uint16_t a5, uint16_t a6, uint8_t a7, uint8_t a8)
{
    D2GSPacketSrv95 packet95 = {};
    D2BitBufferStrc bitBuffer = {};

    BITMANIP_Initialize(&bitBuffer, &packet95.nHeader, sizeof(D2GSPacketSrv95));
    BITMANIP_Write(&bitBuffer, 0x95u, 8);
    BITMANIP_Write(&bitBuffer, a2, 15);
    BITMANIP_Write(&bitBuffer, a3, 15);
    BITMANIP_Write(&bitBuffer, a4, 15);
    BITMANIP_Write(&bitBuffer, a5, 16);
    BITMANIP_Write(&bitBuffer, a6, 16);
    BITMANIP_Write(&bitBuffer, a7, 8);
    BITMANIP_Write(&bitBuffer, a8, 8);

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet95, BITMANIP_GetSize(&bitBuffer));
}

//D2Game.0x6FC3DA90
void __fastcall sub_6FC3DA90(D2ClientStrc* pClient, uint16_t a2, uint16_t a3, uint16_t a4, uint8_t a5, uint8_t a6)
{
    D2GSPacketSrv96 packet96 = {};
    D2BitBufferStrc bitBuffer = {};

    BITMANIP_Initialize(&bitBuffer, &packet96.nHeader, sizeof(D2GSPacketSrv96));
    BITMANIP_Write(&bitBuffer, 150, 8);
    BITMANIP_Write(&bitBuffer, a2, 15);
    BITMANIP_Write(&bitBuffer, a3, 16);
    BITMANIP_Write(&bitBuffer, a4, 16);
    BITMANIP_Write(&bitBuffer, a5, 8);
    BITMANIP_Write(&bitBuffer, a6, 8);

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet96, BITMANIP_GetSize(&bitBuffer));
}

//D2Game.0x6FC3DB50
void __fastcall D2GAME_PACKETS_SendPacket0x21_UpdateSkills_6FC3DB50(D2ClientStrc* pClient, D2UnitStrc* pUnit, WORD nSkillId, BYTE nSkillLevel, BYTE a4)
{
    if (pUnit)
    {
        if (!pClient && pUnit->dwUnitType == UNIT_MONSTER)
        {
            D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
            if (pOwner)
            {
                pClient = SUNIT_GetClientFromPlayer(pOwner, __FILE__, __LINE__);
            }
        }

        if (pClient)
        {
            D2GSPacketSrv21 packet21 = {};

            packet21.nHeader = 0x21;
            packet21.nUnitType = pUnit->dwUnitType;
            packet21.unk0x02 = a4;
            packet21.nUnitGUID = pUnit->dwUnitId;
            packet21.nSkillId = nSkillId;
            packet21.nSkillLevel = nSkillLevel;
            packet21.nBonusSkillLevel = SKILLS_GetBonusSkillLevelFromSkillId(pUnit, nSkillId);
            
            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet21, sizeof(packet21));
        }
    }
}

//D2Game.0x6FC3DBE0
void __fastcall D2GAME_PACKETS_SendPacket0x22_6FC3DBE0(D2ClientStrc* pClient, BYTE nUnitType, int32_t nUnitGUID, int16_t nSkillId, char a5)
{
    D2GameStrc* pGame = CLIENTS_GetGame(pClient);
    D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);

    D2GSPacketSrv22 packet22 = {};

    packet22.nHeader = 0x22;
    packet22.nUnitType = nUnitType;
    packet22.nUnitGUID = nUnitGUID;
    packet22.nSkillId = nSkillId;
    packet22.nQuantity = a5;
    packet22.bIsCorpse = (!pUnit || !STATES_CheckState(pUnit, STATE_PLAYERBODY));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet22, sizeof(packet22));
}

//D2Game.0x6FC3DC60
void __fastcall D2GAME_PACKETS_SendPacket0x23_6FC3DC60(D2ClientStrc* pClient, BYTE nUnitType, int32_t nUnitGUID, BYTE a4, WORD a5, int32_t a6)
{
    D2GSPacketSrv23 packet23 = {};

    packet23.nHeader = 0x23;
    packet23.nUnitType = nUnitType;
    packet23.dwUnitGUID = nUnitGUID;
    packet23.nPosition = a4;
    packet23.nSkill = a5;
    packet23.unk0x009 = a6;
    
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet23, sizeof(packet23));
}

//D2Game.0x6FC3DCA0
void __fastcall sub_6FC3DCA0(D2ClientStrc* pClient, D2UnitStrc* pUnit)
{
    D2_ASSERT(pUnit);
    D2_ASSERT(pUnit->pSkills);

    int32_t nSkillCount = 0;
    for (D2SkillStrc* pSkill = SKILLS_GetFirstSkillFromSkillList(pUnit->pSkills); pSkill; pSkill = SKILLS_GetNextSkill(pSkill))
    {
        ++nSkillCount;
    }

    const int32_t nPacketLength = sizeof(D2GSPacketSrv94::SkillInfo) * nSkillCount + offsetof(D2GSPacketSrv94, Skills);
    D2GameStrc* pGame = CLIENTS_GetGame(pClient);
    D2GSPacketSrv94* pPacket94 = (D2GSPacketSrv94*)D2_ALLOC_POOL(pGame->pMemoryPool, nPacketLength);
    
    pPacket94->nHeader = 0x94u;
    
    D2_ASSERT(nSkillCount < 256);

    pPacket94->nSkills = nSkillCount;
    pPacket94->dwUnitGUID = pUnit->dwUnitId;

    int32_t i = 0;
    for (D2SkillStrc* pSkill = SKILLS_GetFirstSkillFromSkillList(pUnit->pSkills); pSkill; pSkill = SKILLS_GetNextSkill(pSkill))
    {
        pPacket94->Skills[i].nSkill = SKILLS_GetSkillIdFromSkill(pSkill, __FILE__, __LINE__);
        pPacket94->Skills[i].nSkillLevel = SKILLS_GetSkillLevel(pUnit, pSkill, 0);
        ++i;
    }

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, pPacket94, nPacketLength);

    D2_FREE_POOL(pGame->pMemoryPool, pPacket94);
}

//D2Game.0x6FC3DDF0
void __fastcall D2GAME_PACKETS_SendPacket0x26_ServerMessage_6FC3DDF0(D2ClientStrc* pClient, D2GSPacketSrv26* pMsg)
{
    if (strlen(pMsg->szMessage) >= 256)
    {
        FOG_DisplayAssert("strlen((char *)pChatData->bCustomText) < CHAT_MAX_CHAR_LENGTH", __FILE__, __LINE__);
        exit(-1);
    }

    if (pMsg->szMessage[255])
    {
        pMsg->szMessage[255] = 0;
    }

    D2GSPacketSrv26 packet26 = {};

    packet26.nHeader = pMsg->nHeader;
    packet26.nMessageType = pMsg->nMessageType;
    packet26.nLang = pMsg->nLang;
    packet26.nUnitType = pMsg->nUnitType;
    packet26.dwUnitGUID = pMsg->dwUnitGUID;
    packet26.nMessageColor = pMsg->nMessageColor;
    packet26.nNameColor = pMsg->nNameColor;

    const int32_t nNameLength = SStrCopy(packet26.szName, pMsg->szName, sizeof(pMsg->szName));
    const int32_t nMessageLength = SStrCopy(&packet26.szName[nNameLength + 1], pMsg->szMessage, sizeof(pMsg->szMessage));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet26, nMessageLength + 1 + nNameLength + 1 + offsetof(D2GSPacketSrv26, szName));
}

//D2Game.0x6FC3DEC0
void __fastcall D2GAME_PACKETS_SendPacket0x5A_6FC3DEC0(D2ClientStrc* pClient, D2GSPacketSrv5A* pPacket)
{
    D2GSPacketSrv5A packet5A = {};

    if (strlen(pPacket->szText) >= 16)
    {
        FOG_DisplayAssert("strlen((char*)pSystemMsg->bPlayerName) < MAX_PLR_NAME_LEN", __FILE__, __LINE__);
        exit(-1);
    }

    memcpy(&packet5A, pPacket, sizeof(D2GSPacketSrv5A));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5A, sizeof(packet5A));
}

//D2Game.0x6FC3DF20
void __fastcall D2GAME_PACKETS_SendPacket0x27_6FC3DF20(D2ClientStrc* pClient, D2GSPacketSrv27* pPacket)
{
    D2GSPacketSrv27 packet27 = {};

    memcpy(&packet27, pPacket, sizeof(D2GSPacketSrv27));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet27, sizeof(packet27));
}

//D2Game.0x6FC3DF50
void __fastcall D2GAME_SendPacket0x53_6FC3DF50(D2ClientStrc* pClient, D2GSPacketSrv53* pPacket)
{
    D2GSPacketSrv53 packet53 = {};

    memcpy(&packet53, pPacket, sizeof(D2GSPacketSrv53));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet53, sizeof(packet53));
}

//D2Game.0x6FC3DF80
void __fastcall sub_6FC3DF80(D2ClientStrc* pClient1, D2UnitStrc* pPlayer, D2ClientStrc* pClient2)
{
    D2GSPacketSrv5B packet5B = {};

    packet5B.nHeader = 0x5Bu;
    packet5B.nUnitGUID = pPlayer ? pPlayer->dwUnitId : -1;
    packet5B.nClass = pPlayer ? pPlayer->dwClassId : -1;
    packet5B.nLevel = STATLIST_UnitGetStatValue(pPlayer, STAT_LEVEL, 0);
    packet5B.nPartyId = SUNIT_GetPartyId(pPlayer);
    packet5B.unk0x1C = 0;
    packet5B.nPartyFlags = 0;
    packet5B.unk0x20 = CLIENTS_GetGuildFlags(pClient2);

    SStrCopy(packet5B.szName, UNITS_GetPlayerData(pPlayer)->szName, INT_MAX);

    CLIENTS_GetGuildTag(pClient2, (int32_t*)packet5B.unk0x22);

    const int32_t nLength = strlen(packet5B.unk0x22) + 1;

    char* szString = &packet5B.unk0x22[nLength];
    CLIENTS_GetGuildName(pClient2, szString);

    packet5B.nPacketLen = strlen(szString) + 1 + nLength + offsetof(D2GSPacketSrv5B, unk0x22);
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient1, &packet5B, packet5B.nPacketLen);

    if (pPlayer && pPlayer->dwUnitType == UNIT_PLAYER)
    {
        D2GAME_PLAYERPETS_Last_6FC7E970(pClient1, pPlayer);
    }
}

//D2Game.0x6FC3E090
void __fastcall D2GAME_PACKETS_SendPacket0x5C_6FC3E090(D2ClientStrc* pClient, DWORD dwUnitId)
{
    D2GSPacketSrv5C packet5C = {};

    packet5C.nHeader = 0x5C;
    packet5C.dwPlayerGUID = dwUnitId;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5C, sizeof(packet5C));
}

//D2Game.0x6FC3E0B0
void __fastcall D2GAME_PACKETS_SendPacket0x77_Ui_6FC3E0B0(D2ClientStrc* pClient, BYTE nUiNo)
{
    D2GSPacketSrv77 packet77 = {};

    packet77.nHeader = 0x77;
    packet77.nAction = nUiNo;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet77, sizeof(packet77));
}

//D2Game.0x6FC3E0D0
void __fastcall sub_6FC3E0D0(D2ClientStrc* pClient, D2GSPacketSrv78* pPacket)
{
    D2GSPacketSrv78 packet78 = {};

    memcpy(&packet78, pPacket, sizeof(D2GSPacketSrv78));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet78, sizeof(packet78));
}

//D2Game.0x6FC3E100
void __fastcall D2GAME_PACKETS_SendPacket0x7A_6FC3E100(D2ClientStrc* pClient, char a2, char a3, int32_t a4, int32_t a5, int16_t a6)
{
    D2GSPacketSrv7A packet7A = {};

    packet7A.nHeader = 0x7A;
    packet7A.unk0x01 = a2;
    packet7A.unk0x02 = a3;
    packet7A.unk0x03 = a6;
    packet7A.unk0x05 = a5;
    packet7A.unk0x09 = a4;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet7A, sizeof(packet7A));
}

//D2Game.0x6FC3E160
void __fastcall D2GAME_PACKETS_SendPacket0x81_6FC3E160(D2ClientStrc* pClient, char a2, int32_t a3, int32_t a4, int16_t a5, int32_t a6, int32_t a7, int32_t a8)
{
    D2GSPacketSrv81 packet81 = {};

    packet81.nHeader = 0x81;
    packet81.unk0x01 = a3;
    packet81.unk0x02 = a6;
    packet81.unk0x04 = a5;
    packet81.unk0x08 = a4;
    packet81.unk0x0C = a7;
    packet81.unk0x10 = a8;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet81, sizeof(packet81));
}

//D2Game.0x6FC3E1D0
void __fastcall D2GAME_PACKETS_SendPacket0x79_6FC3E1D0(D2ClientStrc* pClient, int32_t a2, char a3)
{
    D2GSPacketSrv79 packet79 = {};

    packet79.nHeader = 0x79;
    packet79.nOwnerGUID = a3;
    packet79.dwGold = a2;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet79, sizeof(packet79));
}

//D2Game.0x6FC3E200
void __fastcall sub_6FC3E200(D2ClientStrc* pClient, D2UnitStrc* pUnit)
{
    // TODO: v8
    D2_ASSERT(pUnit);

    const int32_t nLifePercentage = UNITS_GetCurrentLifePercentage(pUnit);
    D2GSPacketSrv7F packet7F = {};
    packet7F.nHeader = 0x7Fu;

    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        packet7F.bIsPlayer = 1;
    }

    packet7F.nUnitGUID = pUnit->dwUnitId;
    packet7F.nLifePercentage = nLifePercentage;
    if (nLifePercentage >= USHRT_MAX)
    {
        packet7F.nLifePercentage = USHRT_MAX;
    }

    packet7F.nLevelId = DUNGEON_GetLevelIdFromRoom(UNITS_GetRoom(pUnit));

    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);
    D2_ASSERT(pPlayer);

    int32_t bSendPacket = 0;
    if (pUnit->dwUnitType == UNIT_PLAYER)
    {
        bSendPacket = sub_6FC3E440(pPlayer, pUnit, nLifePercentage, packet7F.nLevelId);
    }
    else if (pUnit->dwUnitType == UNIT_MONSTER)
    {
        if (AIGENERAL_GetMinionOwner(pUnit) == pPlayer)
        {
            D2UnkMonsterDataStrc* v8 = sub_6FC3E3D0(pUnit);
            if (v8 && v8->nLifePercentage != nLifePercentage)
            {
                bSendPacket = sub_6FC3E3F0(v8, nLifePercentage);
            }
        }
    }

    if (bSendPacket)
    {
        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet7F, sizeof(packet7F));
    }

    if (packet7F.bIsPlayer && UNITS_GetDistanceToOtherUnit(CLIENTS_GetPlayerFromClient(pClient, 0), pUnit) > 50)
    {
        D2GSPacketSrv90 packet90 = {};

        packet90.nHeader = 0x90u;
        packet90.dwGUID = pUnit->dwUnitId;
        packet90.dwPosX = CLIENTS_GetUnitX(pUnit);
        packet90.dwPosY = CLIENTS_GetUnitY(pUnit);
        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet90, sizeof(packet90));
    }
}

//D2Game.0x6FC3E3D0
D2UnkMonsterDataStrc* __fastcall sub_6FC3E3D0(D2UnitStrc* pUnit)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && pUnit->pMonsterData)
    {
        return &pUnit->pMonsterData->unk0x3C;
    }

    return nullptr;
}

//D2Game.0x6FC3E3F0
int32_t __stdcall sub_6FC3E3F0(D2UnkMonsterDataStrc* a1, int32_t nCurrentLifePercentage)
{
    int32_t nDiff = nCurrentLifePercentage - a1->nLifePercentage;

    if (!nDiff)
    {
        return 0;
    }

    nDiff = std::abs(nDiff);

    if (nDiff > 8)
    {
        a1->nCount = 0;
        a1->nLifePercentage = nCurrentLifePercentage;
        return 1;
    }

    ++a1->nCount;

    if (a1->nCount <= 6u)
    {
        return 0;
    }

    a1->nCount = 0;
    a1->nLifePercentage = nCurrentLifePercentage;
    return 1;
}

//D2Game.0x6FC3E440
int32_t __fastcall sub_6FC3E440(D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int32_t a3, int32_t a4)
{    
    // TODO: Names
    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit1);
    if (!pGame)
    {
        return 0;
    }
    
    const int16_t nPartyId = PARTY_GetPartyIdForUnitOwner(pGame, pUnit1);
    if (nPartyId == -1 || nPartyId != PARTY_GetPartyIdForUnitOwner(pGame, pUnit2))
    {
        return 0;
    }
    
    D2PlayerDataStrc* pPlayerData = UNITS_GetPlayerData(pUnit1);
    if (!pPlayerData)
    {
        return 0;
    }

    int32_t nUnitGUID = -1;
    if (pUnit2)
    {
        nUnitGUID = pUnit2->dwUnitId;
    }

    D2UnkPlayerDataStrc* pData = pPlayerData->unk0x98;
    while (pData)
    {
        if (nUnitGUID == pData->nUnitGUID)
        {
            break;
        }

        pData = pData->pNext;
    }

    if (!pData)
    {
        pData = D2_ALLOC_STRC_POOL(pGame->pMemoryPool, D2UnkPlayerDataStrc);
        if (!pData)
        {
            return 0;
        }

        memset(pData, 0x00, sizeof(*pData));
        pData->pNext = pPlayerData->unk0x98;
        pPlayerData->unk0x98 = pData;
        pData->nUnitGUID = nUnitGUID;
    }

    if (pData->unk0x08 != a4)
    {
        pData->unk0x04 = a3;
        pData->unk0x08 = a4;
        return 1;
    }

    int32_t v15 = a3 - pData->unk0x04;
    if (v15 == 0)
    {
        return 0;
    }

    v15 = std::abs(v15);

    if (v15 > 8)
    {
        pData->unk0x0C = 0;
        pData->unk0x04 = a3;
        return 1;
    }

    ++pData->unk0x0C;
    if (pData->unk0x0C <= 6u)
    {
        return 0;
    }

    pData->unk0x0C = 0;
    pData->unk0x04 = a3;
    return 1;
}

//D2Game.0x6FC3E570
void __fastcall D2GAME_SendPacket0x9C_6FC3E570(D2ClientStrc* pClient, D2UnitStrc* pItem, char nAction, DWORD dwFlag, int32_t bGamble)
{
    D2UnitStrc* pPlayer = CLIENTS_GetPlayerFromClient(pClient, 0);

    D2_ASSERT(pPlayer);

    D2GSPacketSrv9C packet9C = {};

    packet9C.nHeader = 0x9Cu;
    packet9C.nAction = nAction;
    packet9C.nItemId = pItem ? pItem->dwUnitId : -1;
    packet9C.nComponent = UNITS_GetItemComponentId(pPlayer, pItem);

    const uint32_t nOldItemFlags = ITEMS_GetItemFlags(pItem);
    ITEMS_SetItemFlag(pItem, dwFlag, 1);

    memset(packet9C.pBitstream, 0, sizeof(packet9C.pBitstream));

    const uint32_t nLength = ITEMS_SerializeItemToBitstream(pItem, packet9C.pBitstream, 0xF4u, 0, 0, bGamble) + 8;

    if (nLength > 252)
    {
        FOG_DisplayAssert("len <= sizeof(SCmdItemEx)", __FILE__, __LINE__);
        exit(-1);
    }

    packet9C.nPacketSize = nLength;
    ITEMS_SetItemFlag(pItem, 0xFFFFFFFFu, 0);
    ITEMS_SetItemFlag(pItem, nOldItemFlags, 1);

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet9C, packet9C.nPacketSize);

    int32_t nFlags = dwFlag;
    if (ITEMS_CheckIfSocketable(pItem) && !ITEMS_CheckIfFlagIsSet(nFlags, IFLAG_QUANTITY) && pItem->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pItem->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            ITEMS_SetOrRemoveFlag(&nFlags, IFLAG_DELETED, 1);
            D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pItem, INVENTORY_UnitIsItem(i), ITEMACTION_ITEMINSOCKET, nFlags, 0);
        }
    }
}

//D2Game.0x6FC3E6F0
void __fastcall D2GAME_SendPacket0x9D_6FC3E6F0(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, char nAction, DWORD dwFlag, int32_t bGamble)
{
    D2GSPacketSrv9D packet9D = {};

    packet9D.nHeader = 0x9Du;
    packet9D.nAction = nAction;
    packet9D.nComponent = UNITS_GetItemComponentId(pUnit, pItem);

    if (pItem)
    {
        packet9D.nItemId = pItem->dwUnitId;
    }
    else
    {
        packet9D.nItemId = -1;
    }

    if (pUnit)
    {
        packet9D.nUnitType = pUnit->dwUnitType;
        packet9D.nUnitId = pUnit->dwUnitId;
    }
    else
    {
        packet9D.nUnitType = 6;
        packet9D.nUnitId = -1;
    }

    const uint32_t nOldItemFlags = ITEMS_GetItemFlags(pItem);
    ITEMS_SetItemFlag(pItem, dwFlag, 1);
    
    const uint32_t nLength = ITEMS_SerializeItemToBitstream(pItem, packet9D.pBitstream, 0xF4u, 0, 0, bGamble) + 13;
    if (nLength > 252)
    {
        FOG_DisplayAssert("len <= sizeof(SCmdItemEx)", __FILE__, __LINE__);
        exit(-1);
    }

    ITEMS_SetItemFlag(pItem, 0xFFFFFFFFu, 0);
    ITEMS_SetItemFlag(pItem, nOldItemFlags, 1);

    packet9D.nPacketSize = nLength;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet9D, nLength);

    int32_t nFlags = dwFlag;
    if (ITEMS_CheckIfSocketable(pItem) && !ITEMS_CheckIfFlagIsSet(nFlags, IFLAG_QUANTITY) && pItem->pInventory)
    {
        for (D2UnitStrc* i = INVENTORY_GetFirstItem(pItem->pInventory); i; i = INVENTORY_GetNextItem(i))
        {
            ITEMS_SetOrRemoveFlag(&nFlags, 8, 1);
            D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pItem, INVENTORY_UnitIsItem(i), 0x13u, nFlags, 0);
        }
    }
}

//D2Game.0x6FC3E850
void __fastcall D2GAME_SendP0x9C_ItemAction_AddToGround_6FC3E850(D2ClientStrc* pClient, D2UnitStrc* pItem, DWORD dwFlag)
{
    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_ADDTOGROUND, dwFlag, 0);
}

//D2Game.0x6FC3E870
void __fastcall D2GAME_SendP0x9C_ItemAction_GroundToCursor_6FC3E870(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem, DWORD a4)
{
    D2_ASSERT(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__) == pClient);

    UNITS_SetXForStaticUnit(pItem, 0);
    UNITS_SetYForStaticUnit(pItem, 0);
    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_GROUNDTOCURSOR, a4, 0);
}

//D2Game.0x6FC3E8E0
void __fastcall D2GAME_SendP0x9C_ItemAction_DropToGround_6FC3E8E0(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem, DWORD a4)
{
    D2_ASSERT(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__) == pClient);

    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_DROPTOGROUND, a4, 0);
}

//D2Game.0x6FC3E930
void __fastcall D2GAME_SendP0x9C_ItemAction_OnGround_6FC3E930(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem, DWORD a4)
{
    D2_ASSERT(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__) == pClient);

    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_ONGROUND, a4, 0);
}

//D2Game.0x6FC3E980
void __fastcall D2GAME_SendP0x9C_ItemAction_PutInContainer_6FC3E980(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem, DWORD a4)
{
    D2_ASSERT(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__) == pClient);

    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_PUTINCONTAINER, a4, 0);
}

//D2Game.0x6FC3E9D0
void __fastcall D2GAME_UpdateClientItem_6FC3E9D0(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem, DWORD dwCmdFlag)
{
    D2_ASSERT(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__) == pClient);

    const uint8_t nPage = ITEMS_GetInvPage(pItem);
    ITEMS_SetInvPage(pItem, ITEMS_GetItemCell(pItem));
    D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pPlayer, pItem, ITEMACTION_REMOVEFROMCONTAINER, dwCmdFlag, 0);
    ITEMS_SetInvPage(pItem, nPage);
}

//D2Game.0x6FC3EA50
void __fastcall D2GAME_SendP0x9D_ItemAction_Equip_6FC3EA50(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, DWORD dwFlags)
{
    D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pUnit, pItem, ITEMACTION_EQUIP, dwFlags, 0);
}

//D2Game.0x6FC3EA70
void __fastcall D2GAME_SendP0x9D_ItemAction_IndirectlySwapBodyItem_6FC3EA70(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, DWORD dwFlags)
{
    D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pUnit, pItem, ITEMACTION_INDIRECTLYSWAPBODYITEM, dwFlags, 0);
}

//D2Game.0x6FC3EA90
void __fastcall D2GAME_SendP0x9D_ItemAction_Unequip_6FC3EA90(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, DWORD dwFlags)
{
    D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pUnit, pItem, ITEMACTION_UNEQUIP, dwFlags, 0);
}

//D2Game.0x6FC3EAB0
void __fastcall D2GAME_SendP0x9D_ItemAction_SwapBodyItem_6FC3EAB0(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, DWORD dwFlags)
{
    D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pUnit, pItem, ITEMACTION_SWAPBODYITEM, dwFlags, 0);
}

//D2Game.0x6FC3EAD0
void __fastcall D2GAME_SendP0x9D_ItemAction_WeaponSwitch_6FC3EAD0(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, DWORD dwFlags)
{
    D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pUnit, pItem, ITEMACTION_WEAPONSWITCH, dwFlags, 0);
}

//D2Game.0x6FC3EAF0
void __fastcall D2GAME_SendP0x9C_ItemAction_AddQuantity_6FC3EAF0(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem, DWORD a4)
{
    D2_ASSERT(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__) == pClient);

    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_ADDQUANTITY, a4, 0);
}

//D2Game.0x6FC3EB40
void __fastcall D2GAME_SendP0x9C_ItemAction_SwapInContainer_6FC3EB40(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem, DWORD a4)
{
    D2_ASSERT(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__) == pClient);

    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_SWAPINCONTAINER, a4, 0);
}

//D2Game.0x6FC3EB90
void __fastcall D2GAME_SendP0x9C_ItemAction_PutInBelt_6FC3EB90(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem, DWORD a4)
{
    D2_ASSERT(SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__) == pClient);

    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_PUTINBELT, a4, 0);
}

//D2Game.0x6FC3EBE0
void __fastcall D2GAME_SendP0x9C_ItemAction_RemoveFromBelt_6FC3EBE0(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, DWORD dwFlag)
{
    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_REMOVEFROMBELT, dwFlag, 0);
}

//D2Game.0x6FC3EC00
void __fastcall D2GAME_SendP0x9C_ItemAction_SwapInBelt_6FC3EC00(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem, DWORD a3)
{
    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_SWAPINBELT, a3, 0);
}

//D2Game.0x6FC3EC20
void __fastcall D2GAME_PACKETS_SendPacket0x3E_6FC3EC20(D2ClientStrc* pClient, D2UnitStrc* pItem, uint8_t a3, int32_t nStatId, uint32_t nValue, uint16_t a6)
{
    D2BitBufferStrc bitBuffer = {};
    D2GSPacketSrv3E packet3E = {};

    packet3E.nHeader = 0x3Eu;

    BITMANIP_Initialize(&bitBuffer, packet3E.pBuffer, sizeof(packet3E.pBuffer));

    const uint32_t nUnitGUID = pItem ? pItem->dwUnitId : -1;
    if (nUnitGUID >= (1 << 8))
    {
        BITMANIP_Write(&bitBuffer, 1, 1);

        if (nUnitGUID >= (1 << 16))
        {
            BITMANIP_Write(&bitBuffer, 1, 1);
            BITMANIP_Write(&bitBuffer, nUnitGUID, 32);
        }
        else
        {
            BITMANIP_Write(&bitBuffer, 0, 1);
            BITMANIP_Write(&bitBuffer, nUnitGUID, 16);
        }
    }
    else
    {
        BITMANIP_Write(&bitBuffer, 0, 1);
        BITMANIP_Write(&bitBuffer, nUnitGUID, 8);
    }

    BITMANIP_Write(&bitBuffer, a3 != 0, 1);
    BITMANIP_Write(&bitBuffer, nStatId, 9);

    if (nValue >= (1 << 8))
    {
        BITMANIP_Write(&bitBuffer, 1, 1);

        if (nValue >= (1 << 16))
        {
            BITMANIP_Write(&bitBuffer, 1, 1);
            BITMANIP_Write(&bitBuffer, nValue, 32);
        }
        else
        {
            BITMANIP_Write(&bitBuffer, 0, 1);
            BITMANIP_Write(&bitBuffer, nValue, 16);
        }
    }
    else
    {
        BITMANIP_Write(&bitBuffer, 0, 1);
        BITMANIP_Write(&bitBuffer, nValue, 8);
    }

    if (a6 >= (1 << 8))
    {
        BITMANIP_Write(&bitBuffer, 1, 1);
        BITMANIP_Write(&bitBuffer, a6, 16);
    }
    else
    {
        BITMANIP_Write(&bitBuffer, 0, 1);
        BITMANIP_Write(&bitBuffer, a6, 8);
    }

    packet3E.nSize = BITMANIP_GetSize(&bitBuffer) + 2;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet3E, sizeof(packet3E));
}

//D2Game.0x6FC3EDC0
void __fastcall D2GAME_PACKETS_SendPacket0x3F_6FC3EDC0(D2ClientStrc* pClient, D2UnitStrc* pUnit, char a3, int32_t a4, int32_t a5, int16_t a6)
{
    D2GSPacketSrv3F packet3F = {};

    packet3F.nHeader = 0x3Fu;

    if (!a4)
    {
        packet3F.nCursor = a3;
    }
    else
    {
        packet3F.nCursor = -1;
    }

    if (pUnit)
    {
        packet3F.dwItemId = pUnit->dwUnitId;
    }
    else
    {
        packet3F.dwItemId = 0;
    }

    packet3F.nSkill = a6;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet3F, sizeof(packet3F));
}

//D2Game.0x6FC3EE20
void __fastcall D2GAME_SendP0x9D_ItemAction_AutoUnequip_6FC3EE20(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, DWORD dwFlags)
{
    D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pUnit, pItem, ITEMACTION_AUTOUNEQUIP, dwFlags, 0);
}

//D2Game.0x6FC3EE40
void __fastcall D2GAME_PACKETS_SendPacket0x42_6FC3EE40(D2ClientStrc* pClient, D2UnitStrc* pUnit)
{
    D2GSPacketSrv42 packet42 = {};

    packet42.nHeader = 0x42u;

    if (pUnit)
    {
        packet42.nUnitType = pUnit->dwUnitType;
        packet42.dwUnitGUID = pUnit->dwUnitId;
    }
    else
    {
        packet42.nUnitType = 6;
        packet42.dwUnitGUID = -1;
    }

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet42, sizeof(packet42));
}

//D2Game.0x6FC3EEA0
void __fastcall D2GAME_SendP0x9C_ItemAction_ToCursor_6FC3EEA0(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem)
{
    D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_TOCURSOR, 0, 0);
}

//D2Game.0x6FC3EEC0
void __fastcall D2GAME_SendP0x9D_ItemAction_Unknown0x14_6FC3EEC0(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, DWORD dwFlags)
{
    D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pUnit, pItem, ITEMACTION_UNKNOWN0x14, dwFlags, 0);
}

//D2Game.0x6FC3EEE0
void __fastcall D2GAME_SendPacket0x47_6FC3EEE0(D2ClientStrc* pClient, D2UnitStrc* pUnit)
{
    D2GSPacketSrv47 packet47 = {};

    packet47.nHeader = 0x47;
    packet47.nUnitType = pUnit ? pUnit->dwUnitType : 6;
    packet47.nUnitGUID = pUnit ? pUnit->dwUnitId : -1;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet47, sizeof(packet47));
}

//D2Game.0x6FC3EF40
void __fastcall D2GAME_SendPacket0x48_6FC3EF40(D2ClientStrc* pClient, D2UnitStrc* pUnit, BYTE a3)
{
    D2GSPacketSrv48 packet48 = {};

    packet48.nHeader = 0x48;
    packet48.nUnitType = pUnit ? pUnit->dwUnitType : 6;
    packet48.unk0x02 = a3;
    packet48.nUnitGUID = pUnit ? pUnit->dwUnitId : -1;
    
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet48, sizeof(packet48));
}

//D2Game.0x6FC3EFA0
void __fastcall D2GAME_SendP0x9D_ItemAction_UpdateStats_6FC3EFA0(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, DWORD dwFlags)
{
    D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pUnit, pItem, ITEMACTION_UPDATESTATS, dwFlags, 0);
}

//D2Game.0x6FC3EFC0
void __fastcall sub_6FC3EFC0(D2ClientStrc* pClient, D2UnitStrc* pUnit1, D2UnitStrc* pUnit2, int32_t a4, int32_t a5)
{
    D2GSPacketSrv7D packet7D = {};

    packet7D.nHeader = 0x7D;
    packet7D.nUnitType = pUnit1 ? pUnit1->dwUnitType : 6;
    packet7D.nUnitGUID1 = pUnit1 ? pUnit1->dwUnitId : -1;
    packet7D.nUnitGUID2 = pUnit2 ? pUnit2->dwUnitId : -1;
    packet7D.unk0x0A = a4;
    packet7D.unk0x0E = a5;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet7D, sizeof(packet7D));
}

//D2Game.0x6FC3F040
void __fastcall D2GAME_SendP0x9D_ItemAction_Unknown0x16_6FC3F040(D2ClientStrc* pClient, D2UnitStrc* pUnit, D2UnitStrc* pItem, DWORD dwFlags)
{
    D2GAME_SendPacket0x9D_6FC3E6F0(pClient, pUnit, pItem, ITEMACTION_UNKNOWN0x16, dwFlags, 0);
}

//D2Game.0x6FC3F060
void __fastcall sub_6FC3F060(D2ClientStrc* pClient, uint8_t a2, int32_t a3, uint16_t a4, uint16_t a5, int32_t a6, uint16_t a7, uint8_t a8, int32_t a9)
{
    D2GSPacketSrv4D packet = {};

    packet.unk0x01 = a2;
    packet.unk0x02 = a3;
    packet.unk0x0D = a5;
    packet.nHeader = a9 != 0 ? 0x9Au : 0x4Du;
    packet.unk0x0A = a8;
    packet.unk0x0B = a4;
    packet.unk0x06 = a6;
    packet.unk0x0F = a7;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet, sizeof(packet));
}

//D2Game.0x6FC3F0C0
void __fastcall sub_6FC3F0C0(D2ClientStrc* pClient, uint8_t a2, int32_t a3, uint8_t nUnitType, int32_t nUnitGUID, uint16_t a6, uint16_t a7, uint8_t a8, int32_t a9)
{
    D2UnitStrc* pUnit = SUNIT_GetServerUnit(CLIENTS_GetGame(pClient), nUnitType, nUnitGUID);
    if (!pUnit)
    {
        D2GSPacketSrv99 packet99 = {};
        packet99.nHeader = a9 ? 0x99 : 0x4C;
        packet99.unk0x01 = a2;
        packet99.unk0x02 = a3;
        packet99.unk0x06 = a6;
        packet99.unk0x08 = a8;
        packet99.nUnitType = nUnitType;
        packet99.nUnitGUID = nUnitGUID;
        packet99.unk0x0E = a7;
        D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet99, sizeof(packet99));
        return;
    }

    D2RoomStrc* pRoom = UNITS_GetRoom(CLIENTS_GetPlayerFromClient(pClient, 0));
    if (pRoom)
    {
        D2RoomStrc** ppRoomList = nullptr;
        int32_t nNumRooms = 0;
        DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &ppRoomList, &nNumRooms);
        for (int32_t i = 0; i < nNumRooms; ++i)
        {
            if (UNITS_GetRoom(pUnit) == ppRoomList[i])
            {
                D2GSPacketSrv99 packet99 = {};
                packet99.nHeader = a9 ? 0x99 : 0x4C;
                packet99.unk0x01 = a2;
                packet99.unk0x02 = a3;
                packet99.unk0x06 = a6;
                packet99.unk0x08 = a8;
                packet99.nUnitType = nUnitType;
                packet99.nUnitGUID = nUnitGUID;
                packet99.unk0x0E = a7;
                D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet99, sizeof(packet99));
                return;
            }
        }
    }

    D2GSPacketSrv9A packet9A = {};
    packet9A.nHeader = a9 ? 0x9A : 0x4D;
    packet9A.unk0x01 = a2;
    packet9A.unk0x02 = a3;
    packet9A.unk0x06 = a6;
    packet9A.unk0x0A = a8;
    packet9A.nX = D2COMMON_10175_PathGetFirstPointX(pUnit->pDynamicPath);
    packet9A.nY = D2COMMON_10176_PathGetFirstPointY(pUnit->pDynamicPath);
    packet9A.unk0x0F = a7;
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet9A, sizeof(packet9A));
}

//D2Game.0x6FC3F260
void __fastcall D2GAME_SendP0x9C_ItemAction_AddOrRemoveFromShop_6FC3F260(D2ClientStrc* pClient, D2UnitStrc* pPlayer, D2UnitStrc* pItem, char a3)
{
    if (a3 == 56)
    {
        int32_t bIdentified = 0;
        if (ITEMS_IsMagSetRarUniCrfOrTmp(pItem) && !ITEMS_CheckItemFlag(pItem, IFLAG_IDENTIFIED, __LINE__, __FILE__))
        {
            bIdentified = 1;
        }
        D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_ADDTOSHOP, 0, bIdentified);
    }
    else if (a3 == 57)
    {
        const uint8_t nPage = ITEMS_GetInvPage(pItem);
        ITEMS_SetInvPage(pItem, ITEMS_GetItemCell(pItem));
        D2GAME_SendPacket0x9C_6FC3E570(pClient, pItem, ITEMACTION_REMOVEFROMSHOP, 0, 0);
        ITEMS_SetInvPage(pItem, nPage);
    }
}

//D2Game.0x6FC3F2F0
void __fastcall D2GAME_SendPacket0x28_6FC3F2F0(D2ClientStrc* pClient, BYTE nHeader, BYTE a3, int32_t a4, D2BitBufferStrc* a5, BYTE a6)
{
    D2GSPacketSrv28 packet28 = {};

    packet28.nHeader = nHeader;
    packet28.unk0x01 = a3;
    packet28.unk0x02 = a4;
    packet28.unk0x06 = a6;

    QUESTRECORD_CopyRecordToBuffer(a5, packet28.pData, 0x60u, 0);

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet28, sizeof(packet28));
}

//D2Game.0x6FC3F340
void __fastcall sub_6FC3F340(D2ClientStrc* pClient, BYTE nHeader, BYTE nUnitType, int32_t nUnitId, int32_t a5)
{
    D2GSPacketSrv62 packet62 = {};

    packet62.nHeader = nHeader;
    packet62.nUnitType = nUnitType;
    packet62.nUnitId = nUnitId;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet62, sizeof(packet62));
}

//D2Game.0x6FC3F370
void __fastcall sub_6FC3F370(D2ClientStrc* pClient, void* a2)
{
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, a2, 0x61u);
}

//D2Game.0x6FC3F380
void __fastcall sub_6FC3F380(D2ClientStrc* pClient, D2GSPacketSrv5D* pPacket)
{
    D2GSPacketSrv5D packet5D = {};

    memcpy(&packet5D, pPacket, sizeof(D2GSPacketSrv5D));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet5D, sizeof(packet5D));
}

//D2Game.0x6FC3F3B0
void __fastcall D2GAME_SendPacket0x2A_6FC3F3B0(D2ClientStrc* pClient, char a2, char a3, int32_t a4, int32_t a5, char a6)
{
    D2GSPacketSrv2A packet2A = {};

    packet2A.nHeader = a2;
    packet2A.unk0x01 = a6;
    packet2A.unk0x02 = a3;
    packet2A.unk0x07 = a5;
    packet2A.unk0x0B = a4;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet2A, sizeof(packet2A));
}

//D2Game.0x6FC3F3F0
void __fastcall sub_6FC3F3F0(D2ClientStrc* pClient, D2GSPacketSrv2C* pPacket)
{
    D2GSPacketSrv2C packet2C = {};

    memcpy(&packet2C, pPacket, sizeof(D2GSPacketSrv2C));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet2C, sizeof(packet2C));
}

//D2Game.0x6FC3F410
void __fastcall D2GAME_SendPacket0x4E_6FC3F410(D2ClientStrc* pClient, D2GSPacketSrv4E* pPacket)
{
    D2GSPacketSrv4E packet4E = {};

    memcpy(&packet4E, pPacket, sizeof(D2GSPacketSrv4E));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet4E, sizeof(packet4E));
}

//D2Game.0x6FC3F440
void __fastcall D2GAME_SendPacket0x50_6FC3F440(D2ClientStrc* pClient, D2GSPacketSrv50* pPacket)
{
    D2GSPacketSrv50 packet50 = {};

    memcpy(&packet50, pPacket, sizeof(D2GSPacketSrv50));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet50, sizeof(packet50));
}

//D2Game.0x6FC3F480
void __fastcall sub_6FC3F480(D2ClientStrc* pClient, void* pPacket)
{
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, pPacket, 0x26u);
}

//D2Game.0x6FC3F490
void __fastcall D2GAME_SendPacket0x52_6FC3F490(D2ClientStrc* pClient, D2GSPacketSrv52* pPacket)
{
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, pPacket, sizeof(D2GSPacketSrv52));
}

//D2Game.0x6FC3F4A0
void __fastcall D2GAME_PACKETS_SendPacket0x11_6FC3F4A0(D2ClientStrc* pClient, BYTE nUnitType, DWORD dwUnitId, WORD unk)
{
    D2GSPacketSrv11 packet11 = {};

    packet11.nHeader = 0x11;
    packet11.nUnitType = nUnitType;
    packet11.dwUnitGUID = dwUnitId;
    packet11.unk0x006 = unk;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet11, sizeof(packet11));
}

//D2Game.0x6FC3F4D0
void __fastcall sub_6FC3F4D0(D2ClientStrc* pClient, int32_t a2, char a3, int16_t a4, int16_t a5, int16_t a6, int16_t a7)
{
    D2GSPacketSrv57 packet57 = {};

    packet57.nHeader = 0x57;
    packet57.unk0x01 = a2;
    packet57.unk0x05 = a3;
    packet57.unk0x06 = a4;
    packet57.unk0x08 = a5;
    packet57.unk0x0A = a6;
    packet57.unk0x0C = a7;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet57, sizeof(packet57));
}

//D2Game.0x6FC3F520
void __fastcall D2GAME_SendPacket0x58_6FC3F520(D2ClientStrc* pClient, D2GSPacketSrv58* pPacket)
{
    D2GSPacketSrv58 packet58 = {};

    memcpy(&packet58, pPacket, sizeof(D2GSPacketSrv58));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet58, sizeof(packet58));
}

//D2Game.0x6FC3F550
void __fastcall sub_6FC3F550(D2ClientStrc* pClient, D2UnitStrc* pUnit)
{
    D2GSPacketSrv60 packet60 = {};

    packet60.nHeader = 0x60;
    packet60.nFlags = UNITS_GetObjectPortalFlags(pUnit);
    packet60.nLevel = pUnit->pObjectData->InteractType;
    packet60.dwUnitGUID = pUnit->dwUnitId;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet60, sizeof(packet60));
}

//D2Game.0x6FC3F590
void __fastcall D2GAME_PACKETS_SendPacket0x61_6FC3F590(D2ClientStrc* pClient, BYTE unk)
{
    D2GSPacketSrv61 packet61 = {};

    packet61.nHeader = 0x61;
    packet61.unk0x001 = unk;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet61, sizeof(packet61));
}

//D2Game.0x6FC3F5B0
void __fastcall D2GAME_PACKETS_SendPacket0x63_WaypointMenu_6FC3F5B0(D2ClientStrc* pClient, D2GSPacketSrv63* pPacket)
{
    D2GSPacketSrv63 packet63 = {};

    memcpy(&packet63, pPacket, sizeof(D2GSPacketSrv63));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet63, sizeof(packet63));
}

//D2Game.0x6FC3F5E0
void __fastcall D2GAME_PACKETS_SendPacket0x65_6FC3F5E0(D2ClientStrc* pClient, DWORD dwPlayerId, WORD nCount)
{
    D2GSPacketSrv65 packet65 = {};

    packet65.nHeader = 0x65;
    packet65.dwUnitId = dwPlayerId;
    packet65.unk0x05 = nCount;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet65, sizeof(packet65));
}

//D2Game.0x6FC3F610
void __fastcall sub_6FC3F610(D2ClientStrc* pClient, D2GSPacketSrv73* pPacket)
{
    D2GSPacketSrv73 packet73 = {};

    memcpy(&packet73, pPacket, sizeof(D2GSPacketSrv73));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet73, sizeof(packet73));
}

//D2Game.0x6FC3F640
void __fastcall D2GAME_PACKETS_SendPacket0x74_6FC3F640(D2ClientStrc* pClient, D2UnitStrc* a2, D2UnitStrc* a3, BYTE bAssign)
{
    D2GSPacketSrv74 packet74 = {};

    packet74.nHeader = 0x74;
    packet74.unk0x01 = bAssign;
    packet74.unk0x02 = a2 ? a2->dwUnitId : -1;
    packet74.unk0x06 = a3 ? a3->dwUnitId : -1;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet74, sizeof(packet74));
}

//D2Game.0x6FC3F690
void __fastcall SCMD_Send0x75_PartyRosterUpdate(D2UnitStrc* pLocalPlayer, D2UnitStrc* pOtherPlayer)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pLocalPlayer, __FILE__, __LINE__);
    
    D2GSPacketSrv75 packet75 = {};

    packet75.nHeader = 0x75u;
    packet75.nUnitGUID = pOtherPlayer ? pOtherPlayer->dwUnitId : -1;
    packet75.nPartyId = SUNIT_GetPartyId(pOtherPlayer);
    packet75.nLevel = STATLIST_UnitGetStatValue(pOtherPlayer, STAT_LEVEL, 0);
    packet75.nFlags = PLAYERLIST_GetFlags(pLocalPlayer, pOtherPlayer);
    packet75.unk0x0B = sub_6FCBA9D0(pLocalPlayer, pOtherPlayer);

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet75, sizeof(packet75));
}

//D2Game.0x6FC3F720
void __fastcall D2GAME_PACKETS_SendPacket0x7B_6FC3F720(D2ClientStrc* pClient, BYTE slot, int16_t a3, int32_t a4, int32_t a5)
{
    D2GSPacketSrv7B packet7B = {};

    packet7B.nHeader = 0x7Bu;
    packet7B.unk0x01 = slot;
    packet7B.unk0x04 = a5;

    if (a4)
    {
        packet7B.unk0x02 = (a3 & 0xFFF) | 0x8000;
    }
    else
    {
        packet7B.unk0x02 = a3 & 0xFFF;
    }

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet7B, sizeof(packet7B));
}

//D2Game.0x6FC3F770
void __fastcall D2GAME_PACKETS_SendPacket0x7E_6FC3F770(D2ClientStrc* pClient)
{
    D2GSPacketSrv7E packet7E = {};

    packet7E.nHeader = 0x7E;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet7E, sizeof(packet7E));
}

//D2Game.0x6FC3F790
void __fastcall D2GAME_PACKETS_SendPacket0x82_6FC3F790(D2GameStrc* pGame, D2ClientStrc* pClient, int32_t nPortalGUID, int32_t nPortalOwnerGUID)
{
    D2UnitStrc* pPortal = SUNIT_GetServerUnit(pGame, UNIT_OBJECT, nPortalGUID);

    if (pPortal)
    {
        D2UnitStrc* pPlayer = SUNIT_GetServerUnit(pGame, UNIT_PLAYER, OBJECTS_GetUnitIdFromTimerArg(pPortal));
        if (pPlayer)
        {
            D2GSPacketSrv82 packet82 = {};

            packet82.nHeader = 0x82u;

            SStrCopy(packet82.szOwnerName, UNITS_GetPlayerData(pPlayer)->szName, sizeof(packet82.szOwnerName));
           
            packet82.dwOwnerGUID = pPlayer->dwUnitId;
            packet82.dwPortalGUID[0] = nPortalOwnerGUID; // TODO: Maybe [1]
            packet82.dwPortalGUID[1] = nPortalGUID; // TODO: Maybe [0]

            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet82, sizeof(packet82));
        }
    }
}

//D2Game.0x6FC3F810
void __fastcall D2GAME_PACKETS_SendPacket0x8B_6FC3F810(D2ClientStrc* pClient, DWORD dwUnitId, BYTE bParty)
{
    D2GSPacketSrv8B packet8B = {};

    packet8B.nHeader = 0x8B;
    packet8B.nUnitGUID = dwUnitId;
    packet8B.bParty = bParty;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet8B, sizeof(packet8B));
}

//D2Game.0x6FC3F840
void __fastcall sub_6FC3F840(D2GameStrc* pGame, int32_t dwPlayerId1, int32_t dwPlayerId2, int16_t wRelationState)
{
    D2GSPacketSrv8C packet8C = {};

    packet8C.nHeader = 0x8C;
    packet8C.dwPlayerGUID[0] = dwPlayerId1;
    packet8C.dwPlayerGUID[1] = dwPlayerId2;
    packet8C.nFlags = wRelationState;

    SUNIT_IterateLivingPlayers(pGame, sub_6FC3F880, &packet8C);
}

//D2Game.0x6FC3F880
void __fastcall sub_6FC3F880(D2GameStrc*, D2UnitStrc* pPlayer, void* packet)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, packet, sizeof(D2GSPacketSrv8C));
}

//D2Game.0x6FC3F8B0
void __fastcall D2GAME_PACKETS_SendPacket0x8C_6FC3F8B0(D2UnitStrc* pUnit, DWORD dwPlayerId1, DWORD dwPlayerId2, WORD wRelationState)
{
    D2GSPacketSrv8C packet8C = {};

    packet8C.nHeader = 0x8C;
    packet8C.dwPlayerGUID[0] = dwPlayerId1;
    packet8C.dwPlayerGUID[1] = dwPlayerId2;
    packet8C.nFlags = wRelationState;

    D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet8C, sizeof(packet8C));
}

//D2Game.0x6FC3F8F0
void __fastcall sub_6FC3F8F0(D2GameStrc* pGame, D2UnitStrc* pPlayer)
{
    D2GSPacketSrv8D packet8D = {};

    packet8D.nHeader = 0x8D;
    packet8D.dwPlayerGUID = pPlayer ? pPlayer->dwUnitId : -1;
    packet8D.nPartyGUID = SUNIT_GetPartyId(pPlayer);

    SUNIT_IterateLivingPlayers(pGame, sub_6FC3F930, &packet8D);
}

//D2Game.0x6FC3F930
void __fastcall sub_6FC3F930(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* packet)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, packet, sizeof(D2GSPacketSrv8D));
}

//D2Game.0x6FC3F960
void __fastcall D2GAME_PACKETS_SendPacket0x8D_6FC3F960(D2UnitStrc* pUnit, D2UnitStrc* a2)
{
    D2GSPacketSrv8D packet8D = {};

    packet8D.nHeader = 0x8D;
    packet8D.dwPlayerGUID = a2 ? a2->dwUnitId : -1;
    packet8D.nPartyGUID = SUNIT_GetPartyId(a2);

    D2GAME_PACKETS_SendPacket_6FC3C710(SUNIT_GetClientFromPlayer(pUnit, __FILE__, __LINE__), &packet8D, sizeof(packet8D));
}

//D2Game.0x6FC3F9B0
void __fastcall D2GAME_SendPacket0x8E_CorpseAssign_ToAllPlayers_6FC3F9B0(D2GameStrc* pGame, int32_t nUnitId, int32_t nCorpseId, BYTE bAssign)
{
    D2GSPacketSrv8E packet8E = {};
       
    packet8E.nHeader = 0x8E;
    packet8E.nType = bAssign;
    packet8E.dwPlayerGUID = nUnitId;
    packet8E.dwCorpseGUID = nCorpseId;

    SUNIT_IterateLivingPlayers(pGame, sub_6FC3F9F0, &packet8E);
}

//D2Game.0x6FC3F9F0
void __fastcall sub_6FC3F9F0(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pPacket)
{
    D2ClientStrc* pClient = SUNIT_GetClientFromPlayer(pPlayer, __FILE__, __LINE__);
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, pPacket, sizeof(D2GSPacketSrv8E));
}

//D2Game.0x6FC3FA20
void __fastcall D2GAME_PACKETS_SendPacket0x8E_6FC3FA20(D2ClientStrc* pClient, int32_t nUnitId, int32_t nCorpseId, BYTE bAssign)
{
    D2GSPacketSrv8E packet8E = {};

    packet8E.nHeader = 0x8E;
    packet8E.nType = bAssign;
    packet8E.dwPlayerGUID = nUnitId;
    packet8E.dwCorpseGUID = nCorpseId;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet8E, sizeof(packet8E));
}

//D2Game.0x6FC3FA50
void __fastcall sub_6FC3FA50(D2ClientStrc* pClient, void* a2)
{
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, a2, 2u);
}

//D2Game.0x6FC3FA60
void __fastcall D2GAME_SendPacket0x8A_6FC3FA60(D2ClientStrc* pClient, D2GSPacketSrv8A* pPacket)
{
    D2GSPacketSrv8A packet8A = {};

    memcpy(&packet8A, pPacket, sizeof(D2GSPacketSrv8A));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet8A, sizeof(packet8A));
}

//D2Game.0x6FC3FA90
void __fastcall D2GAME_PACKETS_SendPacket0x8F_6FC3FA90(D2ClientStrc* pClient)
{
    D2GSPacketSrv8F packet8F = {};

    packet8F.nHeader = 0x8F;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet8F, sizeof(packet8F));
}

//D2Game.0x6FC3FAC0
void __fastcall sub_6FC3FAC0(D2ClientStrc* pClient, D2GSPacketSrv91* pPacket)
{
    D2GSPacketSrv91 packet91 = {};

    memcpy(&packet91, pPacket, sizeof(D2GSPacketSrv91));

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet91, sizeof(packet91));
}

//D2Game.0x6FC3FAF0
void __fastcall sub_6FC3FAF0(D2ClientStrc* pClient, uint32_t a2, uint16_t a3)
{
    D2GSPacketSrv98 packet98 = {};

    packet98.nHeader = 0x98;
    packet98.unk0x01 = a2;
    packet98.unk0x05 = a3;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet98, sizeof(packet98));
}

//D2Game.0x6FC3FB30
void __fastcall D2GAME_PACKETS_SendPacket0x9B_6FC3FB30(D2ClientStrc* pClient, int16_t a2, int32_t a3)
{
    D2GSPacketSrv9B packet9B = {};

    packet9B.nHeader = 0x9B;
    packet9B.unk0x01 = a2;
    packet9B.unk0x03 = a3;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet9B, sizeof(packet9B));
}

//D2Game.0x6FC3FB60
void __fastcall D2GAME_PACKETS_SendPacket0x97_6FC3FB60(D2ClientStrc* pClient)
{
    D2GSPacketSrv97 packet97 = {};

    packet97.nHeader = 0x97;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packet97, sizeof(packet97));
}

//D2Game.0x6FC3FB80
void __fastcall D2GAME_PACKETS_SendPacket0x16_UnitsUpdate(D2ClientStrc* pClient)
{
    if (!pClient->nUnitUpdateIndex)
    {
        return;
    }

    void* pPacketData = CLIENTS_PacketDataList_GetHead(pClient);

    D2GSPacketSrv16* pPacket16 = (D2GSPacketSrv16*)((uint8_t*)pPacketData + *(uint32_t*)pPacketData + 4);

    pPacket16->nHeader = 0x16;
    pPacket16->nSize = sizeof(D2ClientUnitUpdateStrc) * (pClient->nUnitUpdateIndex + 1) + offsetof(D2GSPacketSrv16, unitUpdate);
    pPacket16->nNumUpdates = pClient->nUnitUpdateIndex;

    for (int32_t i = 0; i < pClient->nUnitUpdateIndex; ++i)
    {
        pPacket16->unitUpdate[i].nUnitType = pClient->unitUpdate[i].nUnitType;
        pPacket16->unitUpdate[i].nUnitGUID = pClient->unitUpdate[i].nUnitGUID;
        pPacket16->unitUpdate[i].nX = pClient->unitUpdate[i].nX;
        pPacket16->unitUpdate[i].nY = pClient->unitUpdate[i].nY;
    }

    *(uint32_t*)pPacketData += pPacket16->nSize;
}

//D2Game.0x6FC3FC00
void __fastcall D2GAME_PACKETS_SendPacket0xA4_6FC3FC00(D2ClientStrc* pClient, int16_t a2)
{
    D2GSPacketSrvA4 packetA4 = {};

    packetA4.nHeader = 0xA4;
    packetA4.unk0x01 = a2;

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetA4, sizeof(packetA4));
}

//D2Game.0x6FC3FC20
void __fastcall D2GAME_PACKETS_SendPacket0xA7_StateOn_6FC3FC20(D2ClientStrc* pClient, BYTE nUnitType, DWORD dwUnitId, uint16_t nState)
{
    D2GSPacketSrvA7 packetA7 = {};

    packetA7.nHeader = 0xA7;
    packetA7.nUnitType = nUnitType;
    packetA7.nUnitId = dwUnitId;
    packetA7.nState = nState; // TODO: Originally a BYTE

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetA7, sizeof(packetA7));
}

//D2Game.0x6FC3FC50
void __fastcall D2GAME_PACKETS_SendPacket0xA9_StateOff_6FC3FC50(D2ClientStrc* pClient, BYTE nUnitType, DWORD dwUnitId, uint16_t nState)
{
    D2GSPacketSrvA9 packetA9 = {};

    packetA9.nHeader = 0xA9;
    packetA9.nUnitType = nUnitType;
    packetA9.nUnitId = dwUnitId;
    packetA9.nState = nState; // TODO: Originally a BYTE

    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetA9, sizeof(packetA9));
}

//D2Game.0x6FC3FC80
void __fastcall sub_6FC3FC80(D2ClientStrc* pClient, D2UnitStrc* pUnit)
{
    if (!pUnit || pUnit->dwUnitType != UNIT_MONSTER)
    {
        return;
    }

    D2GSPacketSrvAC packetAC = {};
    packetAC.nHeader = 0xACu;
    packetAC.dwUnitGUID = pUnit->dwUnitId;
    packetAC.nUnitIndex = pUnit->dwClassId;
    packetAC.nPosX = CLIENTS_GetUnitX(pUnit);
    packetAC.nPosY = CLIENTS_GetUnitY(pUnit);
    packetAC.nLifePct = sub_6FC62F50(pUnit);

    D2BitBufferStrc bitBuffer = {};
    BITMANIP_Initialize(&bitBuffer, packetAC.bitstream, sizeof(packetAC.bitstream));

    if (pUnit->dwAnimMode == MONMODE_SKILL1 || pUnit->dwAnimMode == MONMODE_SKILL2 || pUnit->dwAnimMode == MONMODE_DEAD || pUnit->dwAnimMode == MONMODE_DEATH)
    {
        BITMANIP_Write(&bitBuffer, pUnit->dwAnimMode, 4);
    }
    else
    {
        BITMANIP_Write(&bitBuffer, MONMODE_NEUTRAL, 4);
    }

    if (MONSTER_HasComponents(pUnit))
    {
        BITMANIP_Write(&bitBuffer, 1, 1);

        if (pUnit->pMonsterData)
        {
            for (int32_t i = 0; i < 16; ++i)
            {
                const uint8_t nComponent = pUnit->pMonsterData->nComponent[i];
                
                const int32_t nCompInfo = D2COMMON_11068_GetCompInfo(pUnit, i);
                if (nCompInfo >= 3)
                {
                    // TODO: ...

                    //nBitCount = nCompInfo - 1;
                    //if (nBitCount)
                    //{
                    //    __asm { bsr     eax, [esp + 10h] }
                    //    nBitCount = _EAX + 1;
                    //}

                    DWORD nBitCount = 0;
                    _BitScanReverse(&nBitCount, nCompInfo - 1);

                    BITMANIP_Write(&bitBuffer, nComponent, nBitCount + 1);
                }
                else
                {
                    BITMANIP_Write(&bitBuffer, nComponent, 1);
                }
            }
        }
    }
    else
    {
        BITMANIP_Write(&bitBuffer, 0, 1);
    }

    if (MONSTERUNIQUE_HasUMods(pUnit) || pUnit->dwFlags & UNITFLAG_ISMERC)
    {
        BITMANIP_Write(&bitBuffer, 1, 1);

        BITMANIP_Write(&bitBuffer, MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_CHAMPION) != 0, 1);
        BITMANIP_Write(&bitBuffer, MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_UNIQUE) != 0, 1);
        BITMANIP_Write(&bitBuffer, MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_SUPERUNIQUE) != 0, 1);
        BITMANIP_Write(&bitBuffer, MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_MINION) != 0, 1);
        BITMANIP_Write(&bitBuffer, MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_GHOSTLY) != 0, 1);

        if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_SUPERUNIQUE))
        {
            BITMANIP_Write(&bitBuffer, MONSTERUNIQUE_GetBossHcIdx(pUnit), 16);
        }

        const uint8_t* pUMods = MONSTERUNIQUE_GetUMods(pUnit);
        if (pUMods)
        {
            for (int32_t i = 0; i < 9 && pUMods[i]; ++i)
            {
                BITMANIP_Write(&bitBuffer, pUMods[i], 8);
            }
        }

        BITMANIP_Write(&bitBuffer, 0, 8);

        BITMANIP_Write(&bitBuffer, MONSTERUNIQUE_GetNameSeed(pUnit), 16);

        D2UnitStrc* pOwner = AIGENERAL_GetMinionOwner(pUnit);
        if (pOwner && pOwner->dwUnitType == UNIT_PLAYER && PLAYERPETS_GetPetTypeFromPetGUID(pOwner, packetAC.dwUnitGUID) == PETTYPE_HIREABLE)
        {
            BITMANIP_Write(&bitBuffer, 1, 1);
            BITMANIP_Write(&bitBuffer, pOwner->dwUnitId, 32);
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

    if (pUnit->dwFlagEx & UNITFLAGEX_STOREOWNERINFO && pUnit->dwOwnerType == UNIT_PLAYER)
    {
        BITMANIP_Write(&bitBuffer, 1, 1);
        BITMANIP_Write(&bitBuffer, pUnit->dwOwnerGUID & 0x8FFFFFFF, 31);
    }
    else
    {
        BITMANIP_Write(&bitBuffer, 0, 1);
    }

    int32_t bStatsSent = 0;
    D2StatListStrc* pStatList = STATLIST_GetStatListFromUnitStateAndFlag(pUnit, 0, 0x40);
    if (pStatList)
    {
        D2StatStrc stats[16] = {};
        const int32_t nStatCount = STATLIST_GetBaseStatsData(pStatList, stats, 16);
        for (int32_t i = 0; i < nStatCount; ++i)
        {
            D2ItemStatCostTxt* pItemStatCostTxtRecord = SKILLS_GetItemStatCostTxtRecord(stats[i].nStat);
            if (pItemStatCostTxtRecord)
            {
                if (pItemStatCostTxtRecord->dwItemStatFlags & gdwBitMasks[ITEMSTATCOSTFLAGINDEX_SEND_OTHER] && pItemStatCostTxtRecord->nSendBits)
                {
                    if (!bStatsSent)
                    {
                        bStatsSent = 1;
                        BITMANIP_Write(&bitBuffer, 1, 1);
                    }

                    BITMANIP_Write(&bitBuffer, stats[i].nStat, 9);

                    if (pItemStatCostTxtRecord->nSendParamBits)
                    {
                        BITMANIP_Write(&bitBuffer, stats[i].nLayer, pItemStatCostTxtRecord->nSendParamBits);
                    }

                    BITMANIP_Write(&bitBuffer, stats[i].nValue, pItemStatCostTxtRecord->nSendBits);
                }
            }
        }

        if (bStatsSent)
        {
            BITMANIP_Write(&bitBuffer, 511, 9);
            packetAC.nPacketLength = BITMANIP_GetSize(&bitBuffer) + 13;
            D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetAC, packetAC.nPacketLength);
            return;
        }

        BITMANIP_Write(&bitBuffer, 0, 1);
    }

    BITMANIP_Write(&bitBuffer, 0, 1);
    packetAC.nPacketLength = BITMANIP_GetSize(&bitBuffer) + 13;
    D2GAME_PACKETS_SendPacket_6FC3C710(pClient, &packetAC, packetAC.nPacketLength);
}
