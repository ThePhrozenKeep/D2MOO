#include "MONSTER/MonsterRegion.h"

#include <algorithm>

#include <D2BitManip.h>

#include <D2DataTbls.h>
#include <DataTbls/MonsterIds.h>
#include <D2Dungeon.h>
#include <DataTbls/LevelsIds.h>
#include <D2StatList.h>
#include <D2Monsters.h>

#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "GAME/Targets.h"
#include "ITEMS/Items.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterChoose.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterSpawn.h"
#include "MONSTER/MonsterUnique.h"
#include "QUESTS/Quests.h"
#include "QUESTS/ACT4/A4Q2.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FC66260
int32_t __fastcall sub_6FC66260(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nSuperUniqueId, int32_t* pX, int32_t* pY, int32_t a7)
{
    int32_t nLeft = 0;
    int32_t nTop = 0;
    int32_t nRight = 0;
    int32_t nBottom = 0;

    if (pRoomCoordList)
    {
        D2DrlgCoordStrc drlgCoord = {};
        memcpy(&drlgCoord, &pRoomCoordList->pBox[1], sizeof(drlgCoord));
        DUNGEON_GameTileToSubtileCoords(&drlgCoord.nPosX, &drlgCoord.nPosY);
        DUNGEON_GameTileToSubtileCoords(&drlgCoord.nWidth, &drlgCoord.nHeight);
        nLeft = drlgCoord.nPosX + 1;
        nTop = drlgCoord.nPosY + 1;
        nRight = drlgCoord.nWidth - (drlgCoord.nPosX + 1);
        nBottom = drlgCoord.nHeight - (drlgCoord.nPosY + 1);
    }
    else
    {
        D2DrlgCoordsStrc drlgCoords = {};
        DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);
        nLeft = drlgCoords.dwSubtilesLeft + 1;
        nTop = drlgCoords.dwSubtilesTop + 1;
        nRight = drlgCoords.dwSubtilesWidth - 1;
        nBottom = drlgCoords.dwSubtilesHeight - 1;
    }

    int32_t nRoomCoordListIndex = 0;
    if (pRoomCoordList)
    {
        nRoomCoordListIndex = pRoomCoordList->nIndex;
    }

    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    for (int32_t i = 0; i < 20; ++i)
    {
        const int32_t nX = ITEMS_RollLimitedRandomNumber(pSeed, nRight) + nLeft;
        const int32_t nY = ITEMS_RollLimitedRandomNumber(pSeed, nBottom) + nTop;

        int32_t bSpawnMonster = 0;
        if (a7 && pRoom)
        {
            const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
            if (nLevelId)
            {
                int32_t* pWarpCoordinates = DUNGEON_GetWarpCoordinatesFromRoom(pRoom);
                if (pWarpCoordinates)
                {
                    const int32_t nWarpDist = DATATBLS_GetLevelsTxtRecord(nLevelId)->dwWarpDist;
                    for (int32_t j = 0; j < pWarpCoordinates[18]; ++j)
                    {
                        const int32_t nXDiff = nX - pWarpCoordinates[j];
                        const int32_t nYDiff = nY - pWarpCoordinates[j + 9];
                        if (nXDiff * nXDiff + nYDiff * nYDiff < nWarpDist)
                        {
                            bSpawnMonster = 1;
                            break;
                        }
                    }

                    if (!bSpawnMonster)
                    {
                        int32_t nSpawnX = 0;
                        int32_t nSpawnY = 0;
                        DUNGEON_FindActSpawnLocation(pGame->pAct[DRLG_GetActNoFromLevelId(nLevelId)], nLevelId, 11, &nSpawnX, &nSpawnY);

                        if (nSpawnX > 0 && nSpawnY > 0)
                        {
                            DUNGEON_GameTileToSubtileCoords(&nSpawnX, &nSpawnY);
                            const int32_t nXDiff = nX - nSpawnX;
                            const int32_t nYDiff = nY - nSpawnY;
                            if (nXDiff * nXDiff + nYDiff * nYDiff < nWarpDist)
                            {
                                bSpawnMonster = 1;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (bSpawnMonster)
        {
            if (pRoomCoordList)
            {
                if (D2Common_10095(pRoom, nX, nY) == nRoomCoordListIndex && sub_6FC6A030(pGame, pRoom, pRoomCoordList, nX, nY, nSuperUniqueId, 1, -1, 1))
                {
                    *pX = nX;
                    *pY = nY;
                    return 1;
                }
            }
            else if (D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nSuperUniqueId, 1, -1, 1))
            {
                *pX = nX;
                *pY = nY;
                return 1;
            }
        }
    }

    return 0;
}

//D2Game.0x6FC66560
D2UnitStrc* __fastcall D2GAME_SpawnPresetMonster_6FC66560(D2GameStrc* pGame, D2RoomStrc* pRoom, int32_t nClassId, int32_t nX, int32_t nY, int32_t nMode)
{
    if (nClassId < 0)
    {
        return 0;
    }

    int32_t bRegularId = 1;
    if (nClassId >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        const int32_t nSuperUniqueId = nClassId - sgptDataTables->nMonStatsTxtRecordCount;
        if (nSuperUniqueId < sgptDataTables->nSuperUniquesTxtRecordCount)
        {
            return D2GAME_SpawnSuperUnique_6FC6F690(pGame, pRoom, nX, nY, nSuperUniqueId);
        }

        nClassId = nSuperUniqueId - sgptDataTables->nSuperUniquesTxtRecordCount;
        bRegularId = 0;
    }

    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);

    if (bRegularId == 1)
    {
        if (nClassId == MONSTER_CATAPULT2)
        {
            if (QUESTS_CheckNotIntroQuest(pGame, QUEST_A5Q1_SHENK) && DUNGEON_GetLevelIdFromRoom(pRoom) == LEVEL_BLOODYFOOTHILLS)
            {
                nClassId = MONSTER_CATAPULT3;
            }
        }
        else if (nClassId == MONSTER_CATAPULTSPOTTER2)
        {
            if (QUESTS_CheckNotIntroQuest(pGame, QUEST_A5Q1_SHENK) && DUNGEON_GetLevelIdFromRoom(pRoom) == LEVEL_BLOODYFOOTHILLS)
            {
                nClassId = MONSTER_CATAPULTSPOTTER3;
            }
        }

        if (pGame->nDifficulty != DIFFMODE_NORMAL && (nClassId == MONSTER_MINION1 || nClassId == MONSTER_DEATHMAULER1) && nLevelId == LEVEL_BLOODYFOOTHILLS)
        {
            return nullptr;
        }

        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nClassId);
        if (!pMonStatsTxtRecord)
        {
            return nullptr;
        }

        if (nClassId == MONSTER_PRISONDOOR)
        {
            --nX;

            if (!QUESTS_CheckNotIntroQuest(pGame, QUEST_A5Q2_RESCUESOLDIERS))
            {
                nMode = MONMODE_DEAD;
            }
        }

        D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(nClassId);
        if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_CRITTER])
        {
            return nullptr;
        }

        int16_t nFlags = 0;
        if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NEVERCOUNT])
        {
            nFlags = 8;
        }

        D2UnitStrc* pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nClassId, nMode, -1, nFlags);
        if (!pMonster)
        {
            if (!sub_6FC670A0(nClassId))
            {
                return nullptr;
            }

            pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nClassId, nMode, 4, nFlags);
            if (!pMonster)
            {
                return nullptr;
            }
        }

        if (MONSTERREGION_CheckMonStats2Flag(nClassId, MONSTATS2FLAGINDEX_OBJCOL))
        {
            if (nClassId == MONSTER_BARRICADEDOOR1)
            {
                SUNIT_AllocUnitData(UNIT_OBJECT, 571, CLIENTS_GetUnitX(pMonster), CLIENTS_GetUnitY(pMonster), pGame, pRoom, 1, 0, 0);
            }
            else if (nClassId == MONSTER_BARRICADEDOOR2)
            {
                SUNIT_AllocUnitData(UNIT_OBJECT, 572, CLIENTS_GetUnitX(pMonster), CLIENTS_GetUnitY(pMonster), pGame, pRoom, 1, 0, 0);
            }
        }

        return pMonster;
    }

    switch (nClassId)
    {
    case 2:
    {
        D2MonStatsTxt* pMonStatsTxtRecord = nullptr;
        const int32_t nMonsterId = MONSTERCHOOSE_GetPresetMonsterId(pGame, MONSTERREGION_GetMonsterRegionFromLevelId(pGame->pMonReg, DUNGEON_GetLevelIdFromPopulatedRoom(pRoom)), pRoom, &pMonStatsTxtRecord, 0, 1);
        if (pMonStatsTxtRecord)
        {
            sub_6FC6E8D0(pGame, pRoom, nullptr, nMonsterId, 0, 0, 0, 1);
        }
        return nullptr;
    }
    case 3:
    {
        D2MonStatsTxt* pMonStatsTxtRecord = nullptr;
        const int32_t nMonsterId = MONSTERCHOOSE_GetPresetMonsterId(pGame, MONSTERREGION_GetMonsterRegionFromLevelId(pGame->pMonReg, DUNGEON_GetLevelIdFromPopulatedRoom(pRoom)), pRoom, &pMonStatsTxtRecord, 0, 1);
        if (!pMonStatsTxtRecord)
        {
            return nullptr;
        }

        D2UnitStrc* pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, nMode, -1, 0);
        if (!pMonster)
        {
            return nullptr;
        }

        D2GAME_MONSTERS_Unk_6FC6FFD0(pGame, pMonster, MONUMOD_CHAMPMODS);

        if (!MONSTERUNIQUE_CheckMonTypeFlag(pMonster, MONTYPEFLAG_CHAMPION))
        {
            return pMonster;
        }

        const int32_t nMinions = ITEMS_RollRandomNumber(&pMonster->pSeed) % 3 + 1;
        for (int32_t i = 0; i < nMinions; ++i)
        {
            D2UnitStrc* pMinion = sub_6FC69F70(pGame, 0, pMonster, nMonsterId, 1, 4, 0);
            if (pMinion)
            {
                D2GAME_MONSTERS_Unk_6FC6FFD0(pGame, pMinion, MONUMOD_CHAMPMODS);
            }
        }
        return pMonster;
    }
    case 4:
    {
        return D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, MONSTER_NAVI, nMode, -1, 0);
    }
    case 5:
    {
        return D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, MONSTER_BLOODRAVEN, nMode, -1, 0);
    }
    case 8:
    {
        return D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, MONSTER_MAGGOTQUEEN1, nMode, -1, 8);
    }
    case 10:
    case 11:
    {
        int16_t nFlags = 0;
        if (nClassId == 10)
        {
            nFlags = 4;
        }

        int32_t nChainIncrements = 0;
        switch (nLevelId)
        {
        case LEVEL_SPIDERFOREST:
        case LEVEL_GREATMARSH:
        case LEVEL_FLAYERJUNGLE:
        {
            nChainIncrements = 1;
            break;
        }
        case LEVEL_SEWERSA3LEV1:
        case LEVEL_SEWERSA3LEV2:
        {
            nChainIncrements = 2;
            break;
        }
        case LEVEL_SWAMPYPITLEV1:
        case LEVEL_SWAMPYPITLEV2:
        case LEVEL_FLAYERDUNGEONLEV1:
        case LEVEL_FLAYERDUNGEONLEV2:
        case LEVEL_SWAMPYPITLEV3:
        case LEVEL_FLAYERDUNGEONLEV3:
        {
            nChainIncrements = 0;
            break;
        }
        default:
            break;
        }

        int32_t nMonsterId = -1;
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(261);
        if (pMonStatsTxtRecord)
        {
            nMonsterId = pMonStatsTxtRecord->nBaseId;
            for (int32_t i = 0; i < nChainIncrements; ++i)
            {
                nMonsterId = pMonStatsTxtRecord->nNextInClass;
                pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pMonStatsTxtRecord->nNextInClass);
                if (!pMonStatsTxtRecord)
                {
                    break;
                }
            }
        }

        return D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, nMode, -1, nFlags);
    }
    case 17:
    case 18:
    case 22:
    case 23:
    case 25:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    {
        int32_t nMonsterId = 0;
        switch (nClassId)
        {
        case 25:
        case 27:
        case 28:
            return nullptr;
        case 17:
            nMonsterId = MONSTER_FALLEN1;
            break;
        case 18:
            nMonsterId = MONSTER_FALLENSHAMAN1;
            break;
        case 22:
            nMonsterId = MONSTER_FETISH1;
            break;
        case 23:
            nMonsterId = MONSTER_FETISHSHAMAN1;
            break;
        case 29:
            nMonsterId = MONSTER_MINION1;
            nMode = MONMODE_DEAD;
            break;
        case 30:
            if (nLevelId == LEVEL_BLOODYFOOTHILLS)
            {
                nMonsterId = MONSTER_DEATHMAULER1;
            }
            else
            {
                nMonsterId = MONSTER_IMP1;
            }
            nMode = MONMODE_DEAD;
            break;
        case 31:
            nMonsterId = MONSTER_ACT5BARB1;
            nMode = MONMODE_DEAD;
            break;
        case 32:
            nMonsterId = MONSTER_REANIMATEDHORDE3;
            nMode = MONMODE_DEAD;
            break;
        default:
            break;
        }

        nMonsterId = D2Common_11063(pRoom, nMonsterId);
        int32_t nBaseId = nMonsterId;
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
        if (pMonStatsTxtRecord)
        {
            nBaseId = pMonStatsTxtRecord->nBaseId;
        }

        if (nBaseId == MONSTER_FALLEN1)
        {
            switch (nLevelId)
            {
            case LEVEL_BLACKMARSH:
                nMonsterId = MONSTER_FALLEN2;
                break;
            case LEVEL_TAMOEHIGHLAND:
            case LEVEL_PITLEV1:
            case LEVEL_PITLEV2:
                nMonsterId = MONSTER_FALLEN3;
                break;
            default:
                break;
            }
        }
        else if (nBaseId == MONSTER_FALLENSHAMAN1)
        {
            switch (nLevelId)
            {
            case LEVEL_BLACKMARSH:
            case LEVEL_TAMOEHIGHLAND:
                nMonsterId = MONSTER_FALLENSHAMAN2;
                break;
            case LEVEL_PITLEV1:
            case LEVEL_PITLEV2:
                nMonsterId = MONSTERREGION_SanitizeMonsterId(MONSTER_FALLENSHAMAN3);
                break;
            default:
                break;
            }
        }

        pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
        int16_t nFlags = 0;
        if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_NEVERCOUNT])
        {
            nFlags = 8;
        }

        D2UnitStrc* pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, nMode, -1, nFlags);
        if (!pMonster)
        {
            pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, nMode, 4, nFlags);
            if (!pMonster)
            {
                return nullptr;
            }
        }

        if (nMonsterId == MONSTER_REANIMATEDHORDE3)
        {
            EVENT_SetEvent(pGame, pMonster, UNITEVENTCALLBACK_MONUMOD, pGame->dwGameFrame + ITEMS_RollRandomNumber(&pMonster->pSeed) % 50 + 250, 0, 0);
        }

        if (!pMonster || nMode != MONMODE_DEAD)
        {
            return pMonster;
        }

        pMonster->dwFlags |= UNITFLAG_ISRESURRECT;
        return pMonster;
    }
    case 24:
    case 26:
    {
        if (pGame->nDifficulty != DIFFMODE_NORMAL)
        {
            return nullptr;
        }

        int32_t nMonsterId = 0;
        if (nClassId == 24)
        {
            if (nLevelId == LEVEL_BLOODYFOOTHILLS)
            {
                nMonsterId = MONSTER_DEATHMAULER1;
            }
            else
            {
                nMonsterId = MONSTER_IMP1;
            }
        }
        else if (nClassId == 26)
        {
            nMonsterId = MONSTER_MINION1;
        }

        nMonsterId = D2Common_11063(pRoom, nMonsterId);
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
        
        const uint8_t nMaxGrp = pMonStatsTxtRecord->nMaxGrp;
        const uint8_t nMinGrp = pMonStatsTxtRecord->nMinGrp;
        if (nMinGrp && nMaxGrp && nMaxGrp >= nMinGrp)
        {
            D2RoomCoordListStrc* pRoomCoordList = D2Common_10096(pRoom, nX, nY);
            if (pRoomCoordList)
            {
                D2UnitStrc* pMonster = sub_6FC6A030(pGame, pRoom, pRoomCoordList, nX, nY, nMonsterId, 1, -1, 0);
                if (pMonster)
                {
                    const int32_t nMinions = nMinGrp + ITEMS_RollLimitedRandomNumber(&pMonster->pSeed, nMaxGrp - nMinGrp + 1) - 1;
                    for (int32_t i = 0; i < nMinions; ++i)
                    {
                        sub_6FC69F70(pGame, pRoomCoordList, pMonster, nMonsterId, 1, 3, 0);
                    }
                }
            }
        }
        break;
    }
    default:
        break;
    }

    return nullptr;
}

//D2Game.0x6FC66FC0
D2MonStats2Txt* __fastcall MONSTERREGION_GetMonStats2TxtRecord(int32_t nMonsterId)
{
    if (nMonsterId >= 0 && nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        const int32_t nMonStatsEx = sgptDataTables->pMonStatsTxt[nMonsterId].wMonStatsEx;
        if (nMonStatsEx >= 0 && nMonStatsEx < sgptDataTables->nMonStats2TxtRecordCount)
        {
            return &sgptDataTables->pMonStats2Txt[nMonStatsEx];
        }
    }

    return nullptr;
}

//D2Game.0x6FC67010
uint32_t __fastcall MONSTERREGION_CheckMonStats2Flag(int32_t nMonsterId, int32_t nFlag)
{
    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(nMonsterId);

    if (pMonStats2TxtRecord)
    {
        return pMonStats2TxtRecord->nFlags[nFlag >> 3] & gdwBitMasks[nFlag & 7];
    }

    return 0;
}

//D2Game.0x6FC67080
int32_t __fastcall MONSTERREGION_SanitizeMonsterId(int32_t nMonsterId)
{
    if (nMonsterId >= 0 && nMonsterId < sgptDataTables->nMonStatsTxtRecordCount)
    {
        return nMonsterId;
    }

    return -1;
}

//D2Game.0x6FC670A0
int32_t __fastcall sub_6FC670A0(int32_t nMonsterId)
{
    switch (MONSTERREGION_SanitizeMonsterId(nMonsterId))
    {
    case MONSTER_RADAMENT:
    case MONSTER_MAGGOTQUEEN1:
    case MONSTER_MAGGOTQUEEN2:
    case MONSTER_MAGGOTQUEEN3:
    case MONSTER_MAGGOTQUEEN4:
    case MONSTER_MAGGOTQUEEN5:
    case MONSTER_WINDOW1:
    case MONSTER_WINDOW2:
        return 0;
    }

    return 1;
}

//D2Game.0x6FC67190
void __fastcall D2GAME_PopulateRoom_6FC67190(D2GameStrc* pGame, D2RoomStrc* pRoom)
{
    // TODO: v24
    D2MonsterRegionStrc* pMonsterRegion = MONSTERREGION_GetMonsterRegionFromLevelId(pGame->pMonReg, DUNGEON_GetLevelIdFromRoom(pRoom));
    if (!pMonsterRegion)
    {
        return;
    }

    ++pMonsterRegion->unk0x04;

    const int32_t nLevelId = DUNGEON_GetLevelIdFromPopulatedRoom(pRoom);
    if (!nLevelId)
    {
        return;
    }

    if (pMonsterRegion->unk0x0C < 0)
    {
        pMonsterRegion->unk0x0C = DUNGEON_GetNumberOfPopulatedRoomsInLevel(pGame->pAct[pMonsterRegion->nAct], nLevelId);
    }

    if (!pMonsterRegion->dwMonDen || !pMonsterRegion->unk0x0C || (DUNGEON_GetLevelIdFromRoom(pRoom) == LEVEL_CHAOSSANCTUM && ACT4Q2_IsChaosSanctumCleared(pGame)))
    {
        return;
    }

    D2RoomCoordListStrc* pRoomCoordList = DUNGEON_GetRoomCoordList(pRoom);
    if (!pRoomCoordList)
    {
        return;
    }

    pMonsterRegion = MONSTERREGION_GetMonsterRegionFromLevelId(pGame->pMonReg, DUNGEON_GetLevelIdFromPopulatedRoom(pRoom));
    if (pMonsterRegion->dwMonDen > 10000)
    {
        pMonsterRegion->dwMonDen = 10000;
    }

    int32_t bIncrease = 0;
    
    while (pRoomCoordList)
    {
        if (pRoomCoordList->nIndex && !pRoomCoordList->bNode)
        {
            D2DrlgCoordStrc drlgCoord = {};
            memcpy(&drlgCoord, &pRoomCoordList->pBox[1], sizeof(D2DrlgCoordStrc));

            if (drlgCoord.nPosX || drlgCoord.nWidth)
            {
                DUNGEON_GameTileToSubtileCoords(&drlgCoord.nPosX, &drlgCoord.nPosY);
                DUNGEON_GameTileToSubtileCoords(&drlgCoord.nWidth, &drlgCoord.nHeight);

                for (int32_t i = (drlgCoord.nWidth - drlgCoord.nPosX) / 3 * (drlgCoord.nHeight - drlgCoord.nPosY) / 3; i > 0; --i)
                {
                    if ((ITEMS_RollRandomNumber(&pGame->pGameSeed) % 100000) <= pMonsterRegion->dwMonDen)
                    {
                        D2MonStatsTxt* pMonStatsTxtRecord = nullptr;
                        const int32_t nMonsterId = MONSTERCHOOSE_GetPresetMonsterId(pGame, pMonsterRegion, pRoom, &pMonStatsTxtRecord, 20u, 0);
                        if (!pMonStatsTxtRecord)
                        {
                            return;
                        }

                        int32_t nType = MONSTERCHOOSE_GetBossSpawnType(pMonsterRegion, pRoom);
                        if (nType == 1)
                        {
                            nType = 2;
                        }

                        if (nType == 0)
                        {
                            const int32_t nChampionId = MONSTERCHOOSE_GetPresetMonsterId(pGame, pMonsterRegion, pRoom, &pMonStatsTxtRecord, 0, 1);
                            D2UnitStrc* pChampion = sub_6FC6E8D0(pGame, pRoom, pRoomCoordList, nChampionId, 1, 0, 0, 1);
                            if (pChampion)
                            {
                                if (MONSTERUNIQUE_CheckMonTypeFlag(pChampion, MONTYPEFLAG_CHAMPION))
                                {
                                    for (int32_t j = ITEMS_RollLimitedRandomNumber(&pChampion->pSeed, 3) + 1; j > 0; --j)
                                    {
                                        D2UnitStrc* pMinion = sub_6FC69F70(pGame, pRoomCoordList, pChampion, nChampionId, 1, 4, 0);
                                        if (pMinion)
                                        {
                                            D2GAME_MONSTERS_Unk_6FC6FFD0(pGame, pMinion, MONUMOD_CHAMPMODS);
                                        }
                                    }
                                }

                                bIncrease = 1;
                            }
                        }
                        else if (nType == 1)
                        {
                            D2UnkMonCreateStrc2* v24 = sub_6FC62640(pRoom);
                            int32_t nX = 0;
                            int32_t nY = 0;
                            if (sub_6FC67570(pGame, pRoom, nullptr, nMonsterId, v24, &nX, &nY) && sub_6FC6A350(pGame, pRoom, nX, nY, nullptr, nMonsterId, 1, v24, 0))
                            {
                                bIncrease = 1;
                            }
                        }
                        else
                        {
                            uint8_t nMinGrp = 1;
                            uint8_t nMaxGrp = 1;

                            const int32_t nBaseId = pMonStatsTxtRecord->nBaseId;
                            if (nBaseId != MONSTER_FALLEN1 && nBaseId != MONSTER_SCARAB1)
                            {
                                nMinGrp = pMonStatsTxtRecord->nMinGrp;
                                nMaxGrp = pMonStatsTxtRecord->nMaxGrp;
                            }

                            if ((!pMonStatsTxtRecord->nSparsePopulate || (ITEMS_RollRandomNumber(&pGame->pGameSeed) % 100) <= pMonStatsTxtRecord->nSparsePopulate) && sub_6FC677D0(pGame, pRoom, pRoomCoordList, nMonsterId, nMinGrp, nMaxGrp))
                            {
                                bIncrease = 1;
                            }
                        }

                    }
                }
            }
        }

        pRoomCoordList = pRoomCoordList->pNext;
    }

    if (bIncrease)
    {
        ++pMonsterRegion->unk0x08;
    }
}

//D2Game.0x6FC67570
int32_t __fastcall sub_6FC67570(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nMonsterId, D2UnkMonCreateStrc2* a5, int32_t* pX, int32_t* pY)
{
    int32_t nLeft = 0;
    int32_t nTop = 0;
    int32_t nRight = 0;
    int32_t nBottom = 0;

    if (pRoomCoordList)
    {
        D2DrlgCoordStrc drlgCoord = {};
        memcpy(&drlgCoord, &pRoomCoordList->pBox[1], sizeof(drlgCoord));
        DUNGEON_GameTileToSubtileCoords(&drlgCoord.nPosX, &drlgCoord.nPosY);
        DUNGEON_GameTileToSubtileCoords(&drlgCoord.nWidth, &drlgCoord.nHeight);
        nLeft = drlgCoord.nPosX + 1;
        nRight = drlgCoord.nWidth - (drlgCoord.nPosX + 1);
        nTop = drlgCoord.nPosY + 1;
        nBottom = drlgCoord.nHeight - (drlgCoord.nPosY + 1);
    }
    else
    {
        D2DrlgCoordsStrc drlgCoords = {};
        DUNGEON_GetRoomCoordinates(pRoom, &drlgCoords);
        nLeft = drlgCoords.dwSubtilesLeft + 1;
        nTop = drlgCoords.dwSubtilesTop + 1;
        nBottom = drlgCoords.dwSubtilesHeight - 1;
        nRight = drlgCoords.dwSubtilesWidth - 1;
    }

    int32_t nRoomCoordListIndex = 0;
    if (pRoomCoordList)
    {
        nRoomCoordListIndex = pRoomCoordList->nIndex;
    }

    D2SeedStrc* pSeed = nullptr;
    if (pRoom)
    {
        pSeed = &pRoom->pSeed;
    }

    for (int32_t i = 0; i < 20; ++i)
    {
        const int32_t nX = ITEMS_RollLimitedRandomNumber(pSeed, nRight) + nLeft;
        const int32_t nY = ITEMS_RollLimitedRandomNumber(pSeed, nBottom) + nTop;

        int32_t j = 0;
        while (j < a5->nRecordCount)
        {
            D2UnitStrc* pMonster = nullptr;
            if (pRoomCoordList)
            {
                if (D2Common_10095(pRoom, nX + a5->records[j].nXOffset, a5->records[j].nYOffset) == nRoomCoordListIndex)
                {
                    pMonster = sub_6FC6A030(pGame, pRoom, pRoomCoordList, nX + a5->records[j].nXOffset, nY + a5->records[j].nYOffset, nMonsterId, 1, -1, 1);
                }
            }
            else
            {
                pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX + a5->records[j].nXOffset, nY + a5->records[j].nYOffset, nMonsterId, 1, -1, 1);
            }

            if (!pMonster)
            {
                break;
            }

            ++j;
        }

        if (j >= a5->nRecordCount)
        {
            *pX = nX;
            *pY = nY;
            return 1;
        }
    }

    return 0;
}

//D2Game.0x6FC677D0
D2UnitStrc* __fastcall sub_6FC677D0(D2GameStrc* pGame, D2RoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nMonsterId, uint8_t nMin, uint8_t nMax)
{
    if (!nMin || !nMax || nMax < nMin)
    {
        return nullptr;
    }

    int32_t nX = 0;
    int32_t nY = 0;
    if (!sub_6FC66260(pGame, pRoom, pRoomCoordList, nMonsterId, &nX, &nY, 1))
    {
        return nullptr;
    }

    D2UnitStrc* pMonster = sub_6FC6A030(pGame, pRoom, pRoomCoordList, nX, nY, nMonsterId, 1, -1, 0);
    if (!pMonster)
    {
        return nullptr;
    }

    --nMin;
    --nMax;

    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(nMonsterId);
    if (pMonStats2TxtRecord && pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_OBJCOL] && nMonsterId == MONSTER_EVILHUT)
    {
        SUNIT_AllocUnitData(UNIT_OBJECT, 562, CLIENTS_GetUnitX(pMonster), CLIENTS_GetUnitY(pMonster), pGame, pRoom, 1, 0, 0);
    }

    const int32_t nCount = nMin + ITEMS_RollLimitedRandomNumber(&pMonster->pSeed, nMax - nMin + 1);
    for (int32_t i = 0; i < nCount; ++i)
    {
        sub_6FC69F70(pGame, pRoomCoordList, pMonster, nMonsterId, 1, 3, 0);
    }

    return pMonster;
}

//D2Game.0x6FC679F0
void __fastcall sub_6FC679F0(D2GameStrc* pGame, D2RoomStrc* pRoom)
{
    constexpr int32_t dword_6FD2EE44[] = { MONSTER_ROGUE2 };
    constexpr uint8_t byte_6FD2EE48[10] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };

    if (!pRoom || ITEMS_RollRandomNumber(&pRoom->pSeed) & 0x7FFF || pRoom->nNumClients)
    {
        return;
    }

    const int32_t nLevelId = DUNGEON_GetLevelIdFromRoom(pRoom);
    D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(nLevelId);
    if (!pLevelsTxtRecord || !pLevelsTxtRecord->nMonWndr)
    {
        return;
    }

    if ((int32_t)(ITEMS_RollRandomNumber(&pRoom->pSeed) % 100) >= 3)
    {
        return;
    }

    D2MonsterRegionStrc* pMonsterRegion = MONSTERREGION_GetMonsterRegionFromLevelId(pGame->pMonReg, nLevelId);
    if (pMonsterRegion->unk0x2C4 >= 3 || pLevelsTxtRecord->nAct >= 5u)
    {
        return;
    }

    const uint32_t nIndex = 2 * pLevelsTxtRecord->nAct;
    if (!byte_6FD2EE48[nIndex + 1])
    {
        return;
    }

    const int32_t nMonsterId = dword_6FD2EE44[ITEMS_RollLimitedRandomNumber(&pRoom->pSeed, byte_6FD2EE48[nIndex + 1]) + byte_6FD2EE48[nIndex]];
    
    int32_t nX = 0;
    int32_t nY = 0;
    if (!sub_6FC66260(pGame, pRoom, nullptr, nMonsterId, &nX, &nY, 0))
    {
        return;
    }

    D2UnitStrc* pMonster = D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, 1, -1, 0);
    if (!pMonster)
    {
        return;
    }

    sub_6FC40280(pGame, pMonster, 0, 8);
    ++pMonsterRegion->unk0x2C4;
}

//D2Game.0x6FC67B90
void __fastcall MONSTERREGION_InitializeAll(void* pMemPool, D2MonsterRegionStrc** ppMonsterRegion, D2SeedStrc* pSeed, uint32_t nLowSeed, uint8_t nDifficulty, int32_t bExpansion)
{
    SEED_InitLowSeed(pSeed, nLowSeed);

    ++ppMonsterRegion;

    if (!DATATBLS_GetDifficultyLevelsTxtRecord(nDifficulty))
    {
        FOG_DisplayWarning("pctDifficultyLevel", __FILE__, __LINE__);
    }

    for (int32_t nLevelId = 1; nLevelId < sgptDataTables->nLevelsTxtRecordCount; ++nLevelId)
    {
        D2MonsterRegionStrc* pMonsterRegion = D2_CALLOC_STRC_POOL(pMemPool, D2MonsterRegionStrc);

        D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(nLevelId);
        pMonsterRegion->dwlevel = nLevelId;
        pMonsterRegion->nAct = pLevelsTxtRecord->nAct;
        pMonsterRegion->dwMonDen = pLevelsTxtRecord->dwMonDen[nDifficulty];
        pMonsterRegion->nBossMin = pLevelsTxtRecord->nMonUMin[nDifficulty];
        pMonsterRegion->nBossMax = pLevelsTxtRecord->nMonUMax[nDifficulty];
        pMonsterRegion->nMonWander = pLevelsTxtRecord->nMonWndr;
        pMonsterRegion->unk0x2D4 = -1;
        pMonsterRegion->unk0x0C = -1;
        pMonsterRegion->nQuest = pLevelsTxtRecord->nQuest;

        if (bExpansion)
        {
            pMonsterRegion->dwDungeonLevel = pLevelsTxtRecord->wMonLvlEx[nDifficulty];
            pMonsterRegion->dwDungeonLevelEx = pLevelsTxtRecord->wMonLvlEx[nDifficulty];
        }
        else
        {
            pMonsterRegion->dwDungeonLevel = pLevelsTxtRecord->wMonLvl[nDifficulty];
            pMonsterRegion->dwDungeonLevelEx = pLevelsTxtRecord->wMonLvl[nDifficulty];
        }

        int32_t nNumMon = std::min(pLevelsTxtRecord->nNumMon, 13ui8);
        int32_t nNumNMon = pLevelsTxtRecord->nNumNormMon;
        if (nDifficulty > 0)
        {
            nNumNMon = pLevelsTxtRecord->nNumNMon;
        }

        nNumMon = std::min(nNumMon, nNumNMon);

        int16_t monsterIds[26] = {};
        D2MonRegDataStrc* pMonRegData = pMonsterRegion->pMonData;
        if (nDifficulty > 0)
        {
            if (pLevelsTxtRecord->nNumNMon > 0)
            {
                memcpy(monsterIds, pLevelsTxtRecord->wNMon, pLevelsTxtRecord->nNumNMon);
            }
        }
        else
        {
            if (pLevelsTxtRecord->nNumNormMon > 0)
            {
                memcpy(monsterIds, pLevelsTxtRecord->wMon, pLevelsTxtRecord->nNumNormMon);
            }
        }

        for (int32_t i = 0; i < nNumMon && nNumNMon > 0; ++i)
        {
            int32_t nIndex = ITEMS_RollLimitedRandomNumber(pSeed, nNumNMon);
            int32_t nMonsterId = monsterIds[nIndex];
            if (!i && pLevelsTxtRecord->nRangedSpawn)
            {
                for (int32_t j = 0; j < 20; ++j)
                {
                    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
                    if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_RANGEDTYPE])
                    {
                        break;
                    }

                    nIndex = ITEMS_RollLimitedRandomNumber(pSeed, nNumNMon);
                    nMonsterId = monsterIds[nIndex];
                }
            }

            --nNumNMon;
            if (nNumNMon > nIndex)
            {
                memcpy(&monsterIds[nIndex], &monsterIds[nIndex + 1], 2 * (nNumNMon - nIndex));
            }

            D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
            if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_ISSPAWN])
            {
                pMonRegData->nMonHcIdx = nMonsterId;
                pMonRegData->nRarity = pMonStatsTxtRecord->nRarity;
                pMonsterRegion->nTotalRarity += pMonStatsTxtRecord->nRarity;
                ++pMonsterRegion->nSpawnCount;
                ++pMonsterRegion->nMonCount;
                ++pMonRegData;
            }
        }

        for (int32_t i = 0; i < pMonsterRegion->nMonCount; ++i)
        {
            sub_6FC62020(pSeed, &pMonsterRegion->pMonData[i], 3);
        }

        *ppMonsterRegion = pMonsterRegion;
        ++ppMonsterRegion;
    }
}

//D2Game.0x6FC67F30
void __fastcall MONSTERREGION_FreeAll(void* pMemPool, D2MonsterRegionStrc** ppMonsterRegion)
{
    for (int32_t i = 1; i < sgptDataTables->nLevelsTxtRecordCount; ++i)
    {        
        if (ppMonsterRegion[i])
        {
            D2_FREE_POOL(pMemPool, ppMonsterRegion[i]);
        }
    }
}

//D2Game.0x6FC67F90
D2MonsterRegionStrc* __fastcall MONSTERREGION_GetMonsterRegionFromLevelId(D2MonsterRegionStrc** ppMonsterRegion, int32_t nLevelId)
{
    return ppMonsterRegion[nLevelId];
}

//D2Game.0x6FC67FA0
D2MonRegDataStrc* __fastcall sub_6FC67FA0(D2MonsterRegionStrc** ppMonsterRegion, D2RoomStrc* pRoom, D2UnitStrc* pUnit)
{
    int32_t nClassId = -1;
    if (pUnit)
    {
        nClassId = pUnit->dwClassId;
    }

    switch (MONSTERREGION_SanitizeMonsterId(nClassId))
    {
    case MONSTER_ACT2MALE:
    case MONSTER_ACT2FEMALE:
    case MONSTER_ACT3MALE:
    case MONSTER_ACT3FEMALE:
        return nullptr;
    }

    D2MonsterRegionStrc* pMonsterRegion = ppMonsterRegion[DUNGEON_GetLevelIdFromRoom(pRoom)];
    if (!pMonsterRegion)
    {
        return nullptr;
    }

    D2MonRegDataStrc* pMonsterRegionData = pMonsterRegion->pMonData;
    const int32_t nSpawnCount = pMonsterRegion->nSpawnCount;
    int32_t nCounter = 0;
    while (nCounter < nSpawnCount)
    {
        if (pMonsterRegionData->nMonHcIdx == nClassId)
        {
            return pMonsterRegionData;
        }

        ++pMonsterRegionData;
        ++nCounter;
    }

    if (nCounter >= 13)
    {
        return nullptr;
    }

    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(nClassId);
    if (pMonStats2TxtRecord && pMonStats2TxtRecord->nTotalPieces > 2u)
    {
        pMonsterRegionData->nMonHcIdx = nClassId;
        ++pMonsterRegion->nSpawnCount;
        sub_6FC62020(&pUnit->pSeed, pMonsterRegionData, 3);
    }

    return pMonsterRegionData;
}

//D2Game.0x6FC68110
void __fastcall sub_6FC68110(D2GameStrc* pGame)
{
    D2SeedStrc seed = {};
    SEED_InitSeed(&seed);

    const int32_t nSeed = ITEMS_RollRandomNumber(&pGame->pGameSeed);
    SEED_InitLowSeed(&seed, nSeed);
    pGame->dwMonSeed = nSeed;
    MONSTERREGION_InitializeAll(pGame->pMemoryPool, pGame->pMonReg, &seed, nSeed, pGame->nDifficulty, pGame->bExpansion);
}

//D2Game.0x6FC68180
void __fastcall sub_6FC68180(D2MonsterRegionStrc** ppMonsterRegion, D2RoomStrc* pRoom, D2UnitStrc* pUnit, int32_t bToggleFlag)
{
    if (bToggleFlag)
    {
        MONSTER_ToggleSummonerFlag(pUnit, 2, 1);
    }
    else
    {
        D2MonsterRegionStrc* pMonsterRegion = ppMonsterRegion[DUNGEON_GetLevelIdFromRoom(pRoom)];
        ++pMonsterRegion->dwMonSpawnCount;
    }
}

//D2Game.0x6FC681C0
void __fastcall sub_6FC681C0(D2MonsterRegionStrc** ppMonsterRegion, D2UnitStrc* pUnit, int32_t nPreviousAlignment, int32_t nNewAlignment)
{
    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && !SUNIT_IsDead(pUnit) && !MONSTER_CheckSummonerFlag(pUnit, 2u))
    {
        D2MonsterRegionStrc* pMonsterRegion = ppMonsterRegion[MONSTER_GetLevelId(pUnit)];
        if (pMonsterRegion && !pMonsterRegion->nQuest && nPreviousAlignment != nNewAlignment)
        {
            if (nPreviousAlignment == UNIT_ALIGNMENT_EVIL && (nNewAlignment == UNIT_ALIGNMENT_NEUTRAL || nNewAlignment == UNIT_ALIGNMENT_GOOD))
            {
                --pMonsterRegion->dwMonSpawnCount;
            }

            if (nNewAlignment == UNIT_ALIGNMENT_EVIL && nPreviousAlignment != UNIT_ALIGNMENT_UNASSIGNED)
            {
                ++pMonsterRegion->dwMonSpawnCount;
            }
        }
    }
}

//D2Game.0x6FC68240
void __fastcall sub_6FC68240(D2MonsterRegionStrc** ppMonsterRegion, D2UnitStrc* pUnit)
{
    if (!MONSTER_CheckSummonerFlag(pUnit, 2u))
    {
        D2MonsterRegionStrc* pMonsterRegion = ppMonsterRegion[MONSTER_GetLevelId(pUnit)];
        if (STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_EVIL)
        {
            ++pMonsterRegion->dwMonKillCount;
        }
    }
}

//D2Game.0x6FC68280
void __fastcall sub_6FC68280(D2MonsterRegionStrc** ppMonsterRegion, D2UnitStrc* pUnit)
{
    if (!MONSTER_CheckSummonerFlag(pUnit, 2u))
    {
        D2MonsterRegionStrc* pMonsterRegion = ppMonsterRegion[MONSTER_GetLevelId(pUnit)];
        if (!STATLIST_GetUnitAlignment(pUnit))
        {
            --pMonsterRegion->dwMonKillCount;
        }
    }
}

//D2Game.0x6FC682C0
void __fastcall sub_6FC682C0(D2MonsterRegionStrc** ppMonRegion, int32_t nLevelId1, int32_t nLevelId2, int32_t nAlignment, int32_t bDead, int32_t a6, D2UnitStrc* pUnit)
{
    D2MonsterRegionStrc* pMonsterRegion1 = ppMonRegion[nLevelId1];
    D2MonsterRegionStrc* pMonsterRegion2 = ppMonRegion[nLevelId2];
    if (pUnit)
    {
        if (pMonsterRegion2 != pMonsterRegion1)
        {
            MONSTER_SetLevelId(pUnit, nLevelId1);
        }

        if (!a6 && STATLIST_GetUnitAlignment(pUnit) == UNIT_ALIGNMENT_EVIL)
        {
            --pMonsterRegion2->dwMonSpawnCount;

            if (MONSTERUNIQUE_CheckMonTypeFlag(pUnit, MONTYPEFLAG_OTHER))
            {
                --pMonsterRegion2->dwUniqueCount;
            }
        }
    }
    else
    {
        if (!a6 && !bDead && nAlignment == UNIT_ALIGNMENT_EVIL)
        {
            --pMonsterRegion1->dwMonSpawnCount;
        }
    }
}
