#include "MONSTER/MonsterSpawn.h"

#include <D2BitManip.h>

#include <D2Collision.h>
#include <D2DataTbls.h>
#include <D2Dungeon.h>
#include <D2Monsters.h>
#include <DataTbls/LevelsTbls.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/SkillsIds.h>
#include <D2Skills.h>
#include <D2StatList.h>
#include <D2States.h>


#include "AI/AiGeneral.h"
#include "GAME/Clients.h"
#include "ITEMS/Items.h"
#include "MONSTER/Monster.h"
#include "MONSTER/MonsterAI.h"
#include "MONSTER/MonsterMode.h"
#include "MONSTER/MonsterRegion.h"
#include "MONSTER/MonsterUnique.h"
#include "QUESTS/Quests.h"
#include "SKILLS/SkillAss.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FC68350
int32_t __fastcall sub_6FC68350(int32_t nMonsterId, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t a5)
{
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
    if (pMonStatsTxtRecord)
    {
        nMonsterId = pMonStatsTxtRecord->nBaseId;
    }

    if (nMonsterId < 0 || nMonsterId >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        return 1;
    }

    int32_t nSpawnX = 0;
    int32_t nSpawnY = 0;
    uint16_t nMask = 0;

    switch (nMonsterId)
    {
    case MONSTER_CROWNEST1:
        nSpawnX = nX;
        nSpawnY = nY + 3;
        nMask = COLLIDE_MASK_MONSTER_PATH;
        break;

    case MONSTER_SARCOPHAGUS:
        nSpawnX = nX;
        nSpawnY = nY + 2;
        nMask = COLLIDE_MASK_MONSTER_PATH;
        break;

    case MONSTER_VILEMOTHER1:
        if (a5)
        {
            return 1;
        }

        nSpawnX = nX;
        nSpawnY = nY;
        nMask = COLLIDE_MASK_MONSTER_PATH;
        break;

    case MONSTER_SUCKERNEST1:
        nSpawnX = nX - 2;
        nSpawnY = nY - 2;
        nMask = 0x1C0;
        break;

    case MONSTER_EVILHUT:
        nSpawnX = nX + 2;
        nSpawnY = nY + 4;
        nMask = COLLIDE_MASK_MONSTER_PATH;
        break;

    default:
        return 1;
    }

    pRoom = D2GAME_GetRoom_6FC52070(pRoom, nSpawnX, nSpawnY);
    if (!pRoom || COLLISION_CheckMaskWithSize(pRoom, nSpawnX, nSpawnY, 2, nMask))
    {
        return 0;
    }

    return 1;
}

//D2Game.0x6FC68630
int32_t __fastcall sub_6FC68630(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSkillId, D2UnitStrc* pTarget, int32_t nX, int32_t nY)
{
    D2SkillsTxt* pSkillsTxtRecord = SKILLS_GetSkillsTxtRecord(nSkillId);
    if (!pSkillsTxtRecord)
    {
        return 0;
    }

    if (nSkillId == SKILL_NEST)
    {
        return 0;
    }

    if (pSkillsTxtRecord->dwFlags[1] & gdwBitMasks[SKILLSFLAGINDEX2_TGTPLACECHECK])
    {
        if (pTarget && pTarget->dwFlags & UNITFLAG_TARGETABLE)
        {
            return COLLISION_CheckAnyCollisionWithPattern(UNITS_GetRoom(pTarget), CLIENTS_GetUnitX(pTarget), CLIENTS_GetUnitY(pTarget), PATH_GetUnitCollisionPattern(pTarget), COLLIDE_MASK_MONSTER_PATH) == 0;
        }

        return 0;
    }

    if (nSkillId == SKILL_ANDRIALSPRAY)
    {
        return pTarget != nullptr;
    }

    const uint16_t nSrvDoFunc = pSkillsTxtRecord->wSrvDoFunc;
    if (nSrvDoFunc == 77 || nSrvDoFunc == 78)
    {
        if (!pTarget || MONSTERS_IsDead(pUnit))
        {
            return 0;
        }

        const int32_t nUnitX = CLIENTS_GetUnitX(pUnit);
        const int32_t nUnitY = CLIENTS_GetUnitY(pUnit);

        const int32_t nTargetX = 2 * CLIENTS_GetUnitX(pTarget) - nUnitX;
        const int32_t nTargetY = 2 * CLIENTS_GetUnitY(pTarget) - nUnitY;

        D2CoordStrc coords = {};
        coords.nX = nTargetX;
        coords.nY = nTargetY;
        
        D2ActiveRoomStrc* pRoom = COLLISION_GetFreeCoordinates(UNITS_GetRoom(pUnit), &coords, UNITS_GetUnitSizeX(pUnit), COLLIDE_MASK_MONSTER_PATH, 0);
        if (!pRoom || DUNGEON_IsRoomInTown(pRoom) || COLLISION_CheckAnyCollisionWithPattern(pRoom, nTargetX, nTargetY, PATH_GetUnitCollisionPattern(pUnit), COLLIDE_MASK_MONSTER_PATH))
        {
            return 0;
        }

        return D2Common_11025(nUnitX, nUnitY, nTargetX, nTargetY, pRoom, 0xC01) != 0;
    }

    if (nSkillId == SKILL_DIABPRISON)
    {
        return sub_6FC6A810(pGame, UNITS_GetRoom(pTarget), 0, 0, pTarget, MONSTER_BONEPRISON1, 1);
    }

    if (nSkillId == SKILL_MONTELEPORT)
    {
        if (!pUnit)
        {
            return 0;
        }

        D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
        if (!pRoom || DUNGEON_IsRoomInTown(pRoom) || COLLISION_CheckAnyCollisionWithPattern(pRoom, nX, nY, PATH_GetUnitCollisionPattern(pUnit), COLLIDE_MASK_MONSTER_PATH))
        {
            return 0;
        }

        return D2Common_11025(CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), nX, nY, pRoom, 0xC01) != 0;
    }

    if (nSrvDoFunc == 67)
    {
        if (!pUnit)
        {
            return 0;
        }

        D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
        if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
        {
            return 0;
        }

        return D2Common_11025(CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), nX, nY, pRoom, 0xC01) != 0;
    }

    if (nSkillId == SKILL_DESERTTURRET)
    {
        if (!pUnit)
        {
            return 0;
        }

        D2ActiveRoomStrc* pRoom = D2GAME_GetRoom_6FC52070(UNITS_GetRoom(pUnit), nX, nY);
        if (!pRoom || DUNGEON_IsRoomInTown(pRoom))
        {
            return 0;
        }

        return D2Common_11025(CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), nX, nY, pRoom, COLLIDE_MASK_RADIAL_BARRIER) != 0;
    }

    return 1;
}

//D2Game.0x6FC68CC0
int32_t __fastcall MONSTERSPAWN_GetResurrectMode(D2UnitStrc* pUnit, int32_t a2)
{
    if (!a2 || !pUnit)
    {
        return MONMODE_NEUTRAL;
    }

    D2MonStats2Txt* pMonStats2TxtRecord = MONSTERREGION_GetMonStats2TxtRecord(pUnit->dwClassId);
    if (!pMonStats2TxtRecord)
    {
        return MONMODE_NEUTRAL;
    }

    const int32_t nSkillId = pMonStats2TxtRecord->nResurrectSkill;
    if (nSkillId > 0 && nSkillId < sgptDataTables->nSkillsTxtRecordCount)
    {
        UNITS_SetUsedSkill(pUnit, SKILLS_GetHighestLevelSkillFromUnitAndId(pUnit, nSkillId));
    }

    if (pMonStats2TxtRecord->nResurrectMode < 16)
    {
        return pMonStats2TxtRecord->nResurrectMode;
    }

    return MONMODE_NEUTRAL;
}

//D2Game.0x6FC68D70
D2UnitStrc* __fastcall sub_6FC68D70(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nAnimMode, int32_t a5, int16_t nFlags)
{
    D2UnkMonCreateStrc monCreate = {};

    monCreate.nUnitGUID = 0;
    monCreate.nMonsterId = nMonsterId;
    monCreate.nAnimMode = nAnimMode;
    monCreate.pGame = pGame;
    monCreate.pRoomCoordList = nullptr;
    monCreate.pRoom = UNITS_GetRoom(pUnit);
    monCreate.field_20 = a5;
    monCreate.nFlags = nFlags;
    monCreate.nX = CLIENTS_GetUnitX(pUnit);
    monCreate.nY = CLIENTS_GetUnitY(pUnit);

    return D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
}

//D2Game.0x6FC68E30) --------------------------------------------------------
D2UnitStrc* __fastcall D2GAME_SpawnNormalMonster_6FC68E30(D2UnkMonCreateStrc* pMonCreate)
{
    return nullptr;

//    D2UnkMonCreateStrc* v1; // ebp@1
//    int32_t nMonsterId; // eax@1
//    int32_t v3; // edi@2
//    D2MonStatsTxt* ptMonStatsTxtRecord; // edx@3
//    int32_t v5; // eax@6
//    int32_t v6; // edx@8
//    D2MonStats2Txt* v7; // eax@8
//    D2MonStats2Txt* v8; // edi@8
//    D2ActiveRoomStrc* v9; // ebx@9
//    int16_t v10; // cx@9
//    int32_t v11; // eax@9
//    int32_t v12; // eax@14
//    int32_t v13; // edx@16
//    int32_t v14; // esi@18
//    int32_t v15; // eax@19
//    int32_t v16; // ecx@20
//    BYTE v17; // al@22
//    D2C_MonsterIds v18; // eax@23
//    D2ActiveRoomStrc** v19; // ecx@27
//    DWORD v20; // eax@29
//    int64_t v21; // qax@31
//    int32_t v22; // esi@33
//    int32_t v23; // esi@36
//    int32_t v24; // ebp@38
//    int32_t v25; // edi@38
//    int32_t v26; // esi@39
//    int32_t v27; // edi@45
//    int32_t v28; // eax@47
//    int32_t v29; // esi@47
//    uint32_t v30; // edx@51
//    D2ActiveRoomStrc* v31; // eax@52
//    int32_t v32; // ecx@55
//    int32_t v33; // ebp@61
//    uint32_t v34; // eax@61
//    int64_t v35; // qax@61
//    int32_t v36; // edx@61
//    uint64_t v37; // kr20_8@61
//    __int32_t64 v38; // qcx@61
//    uint64_t v39; // kr18_8@61
//    int32_t v40; // edi@61
//    uint32_t v41; // edx@63
//    int64_t v42; // qax@65
//    int64_t v43; // qax@66
//    int32_t v44; // edx@70
//    int64_t v45; // qax@72
//    int64_t v46; // qax@73
//    __int32_t64 v47; // qax@76
//    uint32_t v48; // kr08_4@76
//    __int32_t64 v49; // qax@76
//    int64_t v50; // qax@78
//    uint32_t v51; // edx@80
//    int32_t v52; // ebx@80
//    int32_t v53; // ebp@80
//    int32_t v54; // edi@83
//    int32_t v55; // eax@90
//    int32_t v56; // ebx@112
//    D2UnitStrc* result; // eax@113
//    D2UnkMonCreateStrc* v58; // edi@116
//    D2ActiveRoomStrc* v59; // ebp@116
//    D2UnitStrc* v60; // esi@116
//    int32_t v61; // eax@117
//    D2RoomCoordListStrc* v62; // eax@119
//    D2MonsterDataStrc* v63; // ecx@122
//    int32_t v64; // eax@130
//    D2GameStrc* v65; // edi@130
//    D2MonStatsTxt* v66; // ecx@132
//    int32_t v67; // ebp@152
//    D2GameStrc* v68; // edi@152
//    D2ActiveRoomStrc* v69; // ebx@152
//    D2MonStatsTxt* v70; // eax@152
//    int32_t v71; // eax@153
//    int32_t v72; // ST40_4@167
//    int32_t v73; // ST3C_4@167
//    int32_t v74; // eax@167
//    int32_t v75; // [sp+3Ch] [bp-ACh]@142
//    int32_t v76; // [sp+3Ch] [bp-ACh]@172
//    int32_t v77; // [sp+40h] [bp-A8h]@142
//    int32_t v78; // [sp+40h] [bp-A8h]@170
//    int32_t v79; // [sp+40h] [bp-A8h]@172
//    int32_t pNumRooms; // [sp+54h] [bp-94h]@35
//    uint32_t v81; // [sp+58h] [bp-90h]@20
//    D2ActiveRoomStrc** pFloorCount; // [sp+5Ch] [bp-8Ch]@22
//    int32_t nX; // [sp+60h] [bp-88h]@22
//    D2ActiveRoomStrc* pRoom; // [sp+64h] [bp-84h]@9
//    D2UnkMonCreateStrc* v85; // [sp+68h] [bp-80h]@1
//    int32_t v86; // [sp+6Ch] [bp-7Ch]@68
//    int32_t nY; // [sp+70h] [bp-78h]@22
//    uint32_t nCollisionFlags; // [sp+74h] [bp-74h]@10
//    D2DrlgTileDataStrc* v89; // [sp+78h] [bp-70h]@15
//    int32_t v90; // [sp+7Ch] [bp-6Ch]@9
//    int32_t v91; // [sp+80h] [bp-68h]@15
//    uint32_t v92; // [sp+84h] [bp-64h]@80
//    int32_t v93; // [sp+88h] [bp-60h]@80
//    D2MonStatsTxt* v94; // [sp+8Ch] [bp-5Ch]@3
//    int32_t v95; // [sp+90h] [bp-58h]@68
//    uint32_t v96; // [sp+94h] [bp-54h]@80
//    int32_t v97; // [sp+98h] [bp-50h]@15
//    int32_t v98; // [sp+9Ch] [bp-4Ch]@15
//    int32_t v99; // [sp+A0h] [bp-48h]@15
//    int32_t v100; // [sp+A4h] [bp-44h]@15
//    int32_t v101; // [sp+A8h] [bp-40h]@61
//    D2MonStats2Txt* v102; // [sp+B0h] [bp-38h]@8
//    uint32_t v103; // [sp+B4h] [bp-34h]@80
//    uint32_t v104; // [sp+BCh] [bp-2Ch]@76
//    int32_t v105; // [sp+C4h] [bp-24h]@76
//    int32_t a2; // [sp+C8h] [bp-20h]@15
//    int32_t v107; // [sp+CCh] [bp-1Ch]@15
//    int32_t v108; // [sp+D0h] [bp-18h]@15
//    int32_t v109; // [sp+D4h] [bp-14h]@15
//    int32_t v110; // [sp+D8h] [bp-10h]@38
//    int32_t v111; // [sp+DCh] [bp-Ch]@38
//
//    v1 = pMonCreate;
//    v85 = pMonCreate;
//    nMonsterId = pMonCreate->nMonsterId;
//    if (nMonsterId < 0
//        || (v3 = sgptDataTables->nMonStatsTxtRecordCount, nMonsterId >= v3)
//        || (ptMonStatsTxtRecord = sgptDataTables->pMonStatsTxt, (v94 = &ptMonStatsTxtRecord[nMonsterId]) == 0)
//        || nMonsterId < 0
//        || nMonsterId >= v3
//        || (v5 = ptMonStatsTxtRecord[nMonsterId].wMonStatsEx, v5 < 0)
//        || v5 >= sgptDataTables->nMonStats2TxtRecordCount
//        || (v6 = 77 * v5, v7 = sgptDataTables->pMonStats2Txt, v8 = &v7[4 * v6 / 0x134u], v102 = &v7[4 * v6 / 0x134u], !v8))
//        return 0;
//    v9 = pMonCreate->pRoom;
//    v10 = pMonCreate->nFlags;
//    pRoom = v1->pRoom;
//    v11 = v8->nSpawnCol;
//    LOWORD(v90) = v10;
//    switch (v11)
//    {
//    case 1:
//        nCollisionFlags = 0x1C0u;
//        break;
//    case 2:
//        nCollisionFlags = COLLIDE_MASK_PLACEMENT;
//        break;
//    case 3:
//        nCollisionFlags = 0;
//        break;
//    default:
//        nCollisionFlags = COLLIDE_MASK_MONSTER_PATH;
//        break;
//    }
//    v12 = v1->pRoomCoordList;
//    if (v12)
//    {
//        CopyRect((LPRECT)&v97, (const RECT*)(v12 + 16));
//        DUNGEON_GameTileToSubtileCoords(&v97, &v98);
//        DUNGEON_GameTileToSubtileCoords(&v99, &v100);
//        v13 = v1->pRoomCoordList;
//        v91 = 1;
//        v89 = *(D2DrlgTileDataStrc**)(v13 + 40);
//    }
//    else
//    {
//        DUNGEON_GetRoomCoordinates(v9, (D2DrlgCoordsStrc*)&a2);
//        v100 = v107 + v109;
//        v97 = a2;
//        v99 = a2 + v108;
//        v98 = v107;
//        v89 = 0;
//        v91 = 0;
//    }
//    if (!v9 || (v14 = (int32_t)&v9->pSeed, v9 == (D2ActiveRoomStrc*)-60))
//        return 0;
//    v15 = v1->field_20;
//    if (v15 >= 0)
//    {
//        v81 = 3;
//        v16 = 3 * v15;
//    }
//    else
//    {
//        v81 = 0;
//        v16 = 0;
//    }
//    v17 = v8->nSpawnCol;
//    pFloorCount = (D2ActiveRoomStrc**)v16;
//    nY = 0;
//    nX = 0;
//    if (v17 != 1
//        || (v18 = v1->nMonsterId, v18 >= MONSTER_TENTACLE1) && v18 <= MONSTER_TENTACLEHEAD3
//        || v18 == MONSTER_HELLMETEOR)
//    {
//        if ((int32_t)v81 > v16)
//            return 0;
//        while (1)
//        {
//            HIDWORD(v38) = v85->nX;
//            v33 = v85->nY;
//            v34 = *(_DWORD*)v14;
//            v101 = v85->nX;
//            v35 = 1791398085i64 * v34;
//            v37 = __PAIR__(HIDWORD(v35), v81);
//            LODWORD(v38) = v37 >> 32;
//            v36 = v37;
//            v39 = __PAIR__(v38, *(_DWORD*)(v14 + 4)) + (uint32_t)v35;
//            LODWORD(v38) = (__PAIR__(v38, *(_DWORD*)(v14 + 4)) + (uint32_t)v35) >> 32;
//            *(_DWORD*)v14 = v39;
//            *(_DWORD*)(v14 + 4) = v38;
//            v40 = 2 * v36 >> 1;
//            if (v39 & 1)
//            {
//                pNumRooms = v36;
//                if (v40 > 0)
//                {
//                    HIDWORD(v38) = 0;
//                    if ((v40 - 1) & v40)
//                    {
//                        v42 = v38 + 1791398085i64 * (uint32_t)v39;
//                        *(_QWORD*)v14 = v42;
//                        v41 = (uint32_t)v42 % v40;
//                    }
//                    else
//                    {
//                        v43 = v38 + 1791398085i64 * (uint32_t)v39;
//                        *(_QWORD*)v14 = v43;
//                        v41 = v43 & (v40 - 1);
//                    }
//                    HIDWORD(v38) = v101;
//                }
//                else
//                {
//                    v41 = 0;
//                }
//                LODWORD(v38) = v41;
//                v95 = 0;
//                v86 = 1;
//            }
//            else
//            {
//                if (v40 > 0)
//                {
//                    HIDWORD(v38) = 0;
//                    if ((v40 - 1) & v40)
//                    {
//                        v45 = v38 + 1791398085i64 * (uint32_t)v39;
//                        *(_QWORD*)v14 = v45;
//                        v44 = (uint32_t)v45 % v40;
//                    }
//                    else
//                    {
//                        v46 = v38 + 1791398085i64 * (uint32_t)v39;
//                        *(_QWORD*)v14 = v46;
//                        v44 = v46 & (v40 - 1);
//                    }
//                    HIDWORD(v38) = v101;
//                }
//                else
//                {
//                    v44 = 0;
//                }
//                LODWORD(v38) = v81;
//                pNumRooms = v44;
//                v95 = 1;
//                v86 = 0;
//            }
//            LODWORD(v47) = 1791398085 * *(_DWORD*)v14;
//            v105 = 1791398085 * (uint64_t)*(_DWORD*)v14 >> 32;
//            v48 = *(_DWORD*)(v14 + 4);
//            HIDWORD(v47) = v105;
//            *(_DWORD*)v14 = *(_DWORD*)(v14 + 4) + v47;
//            v49 = v47 + v48;
//            v104 = HIDWORD(v49);
//            *(_DWORD*)(v14 + 4) = HIDWORD(v49);
//            if (v49 & 1)
//                pNumRooms = -pNumRooms;
//            v50 = v104 + 1791398085i64 * (uint32_t)v49;
//            *(_QWORD*)v14 = v50;
//            if (v50 & 1)
//                LODWORD(v38) = -(int32_t)v38;
//            v51 = v81;
//            HIDWORD(v38) += pNumRooms;
//            v53 = v38 + v33;
//            v96 = v85->nX - v81;
//            v103 = 2 * v81 + v96;
//            v92 = v85->nY - v81;
//            v93 = 2 * v81 + v92;
//            pNumRooms = 8 * v81;
//            if (!(8 * v81))
//                break;
//            if ((int32_t)(8 * v81) > 0)
//                goto LABEL_83;
//LABEL_110:
//            v81 = v51 + 3;
//            if ((int32_t)(v51 + 3) > (int32_t)pFloorCount)
//                goto LABEL_111;
//        }
//        pNumRooms = 1;
//LABEL_83:
//        v54 = v95;
//        while (1)
//        {
//            if (v52 == v96 && v53 == v92)
//            {
//                v54 = 1;
//                v86 = 0;
//            }
//            if (v52 == v103)
//            {
//                if (v53 == v92)
//                {
//                    v54 = 0;
//                    v86 = 1;
//                }
//                v55 = v93;
//                if (v53 == v93)
//                {
//                    v54 = -1;
//                    v86 = 0;
//                }
//            }
//            else
//            {
//                v55 = v93;
//            }
//            if (v52 == v96)
//            {
//                if (v53 == v55)
//                {
//                    v54 = 0;
//                    v86 = -1;
//                }
//                if (v53 == v92 && v52 == v103 && v53 == v55)
//                {
//                    v54 = 0;
//                    v86 = 0;
//                }
//            }
//            v52 += v54;
//            v53 += v86;
//            if (PtInRect((const RECT*)&v97, (POINT)__PAIR__(v53, v52)))
//            {
//                if (!v91 || (D2DrlgTileDataStrc*)D2Common_10095(pRoom, v52, v53) == v89)
//                {
//                    if (sub_6FC68350(v85->nMonsterId, pRoom, v52, (D2ActiveRoomStrc**)v53, 1)
//                        && (!COLLISION_CheckMaskWithSize(pRoom, v52, v53, v102->nSizeX, nCollisionFlags)
//                            || SLOBYTE(v85->nFlags) < 0))
//                        break;
//                }
//            }
//            --pNumRooms;
//            if (pNumRooms <= 0)
//                goto LABEL_109;
//        }
//        nX = v52;
//        nY = v53;
//        v81 = (uint32_t)pFloorCount;
//LABEL_109:
//        v51 = v81;
//        goto LABEL_110;
//    }
//    v89 = DUNGEON_GetFloorTilesFromRoom(v9, (int32_t*)&pFloorCount);
//    if (v89)
//    {
//        v19 = pFloorCount;
//        if (pFloorCount)
//        {
//            if ((int32_t)pFloorCount > 0)
//            {
//                if ((uint32_t)((char*)pFloorCount - 1) & (uint32_t)pFloorCount)
//                {
//                    v21 = v9->pSeed.nHighSeed + 1791398085i64 * *(_DWORD*)v14;
//                    *(_QWORD*)v14 = v21;
//                    v20 = (uint32_t)v21 % (uint32_t)v19;
//                }
//                else
//                {
//                    v20 = (uint32_t)((char*)pFloorCount - 1) & ITEMS_RollRandomNumber(&v9->pSeed);
//                }
//            }
//            else
//            {
//                v20 = 0;
//            }
//            v22 = v20;
//            if (!v20)
//                v22 = 1;
//            DUNGEON_GetRoomCoordinates(v9, (D2DrlgCoordsStrc*)&a2);
//            pNumRooms = v22;
//            v91 = v22 - 1;
//            if (v22 != v22 - 1)
//            {
//                while (1)
//                {
//                    v23 = (int32_t)&v89[pNumRooms];
//                    if (*(_DWORD*)(v23 + 24))
//                    {
//                        if (D2CMP_10079(*(_DWORD*)(v23 + 24)) & 2)
//                        {
//                            v24 = 5 * (v111 + *(_DWORD*)(v23 + 12)) + 3;
//                            v25 = 5 * (v110 + *(_DWORD*)(v23 + 8)) + 3;
//                            if (!COLLISION_CheckMask(v9, v25, v24, COLLIDE_MONSTER))
//                                break;
//                        }
//                    }
//LABEL_42:
//                    pNumRooms = (pNumRooms + 1) % (uint32_t)pFloorCount;
//                    if (pNumRooms == v91)
//                        goto LABEL_45;
//                }
//                v26 = (int32_t)&unk_6FD28BC8;
//                while (COLLISION_CheckMaskWithSize(v9, v25 + *(_DWORD*)v26, v24 + *(_DWORD*)(v26 + 4), 2, 0x1C09u))
//                {
//                    v26 += 8;
//                    if (v26 >= (int32_t)&unk_6FD28BE8)
//                        goto LABEL_42;
//                }
//                nX = v25;
//                nY = v24;
//            }
//        }
//    }
//LABEL_45:
//    v27 = nX;
//    if (nX >= v9->nSubtileX && nX < v9->nSubtileX + v9->nSubtileWidth)
//    {
//        v28 = v9->nSubtileY;
//        v29 = nY;
//        if (nY < v28)
//            goto LABEL_51;
//        if (nY < v28 + v9->nSubtileHeight)
//            goto LABEL_111;
//        v9 = pRoom;
//    }
//    v29 = nY;
//LABEL_51:
//    pFloorCount = 0;
//    pNumRooms = 0;
//    DUNGEON_GetAdjacentRoomsListFromRoom(v9, &pFloorCount, &pNumRooms);
//    v30 = 0;
//    if ((uint32_t)pNumRooms <= 0)
//    {
//LABEL_58:
//        pRoom = 0;
//    }
//    else
//    {
//        while (1)
//        {
//            v31 = pFloorCount[v30];
//            if (v31)
//            {
//                if (v27 >= v31->nSubtileX)
//                {
//                    if (v27 < v31->nSubtileX + v31->nSubtileWidth)
//                    {
//                        v32 = v31->nSubtileY;
//                        if (v29 >= v32)
//                        {
//                            if (v29 < v32 + v31->nSubtileHeight)
//                                break;
//                        }
//                    }
//                }
//            }
//            ++v30;
//            if (v30 >= pNumRooms)
//                goto LABEL_58;
//        }
//        pRoom = pFloorCount[v30];
//    }
//LABEL_111:
//    if (nX)
//    {
//        v56 = nY;
//        if (nY)
//        {
//            result = (D2UnitStrc*)1;
//            if (v90 & 1)
//                return result;
//            if (v90 & 0x20)
//                LOBYTE(result) = 3;
//            v58 = v85;
//            v59 = pRoom;
//            v60 = SUNIT_AllocUnitData(1, v85->nMonsterId, nX, nY, v85->pGame, pRoom, (char)result, v85->nAnimMode, v85->nUnitGUID);
//            if (!v60)
//                return v60;
//            v61 = v90 & 8;
//            if (BYTE2(v94->dwMonStatsFlags) & gdwBitMasks[2])
//                v61 = 1;
//            sub_6FC68180(v58->pGame->pMonReg, v59, v60, v61);
//            v62 = (D2RoomCoordListStrc*)v58->pRoomCoordList;
//            if (!v62)
//                v62 = D2Common_10096(v59, nX, v56);
//            if (v60->dwUnitType == 1)
//            {
//                v63 = (D2MonsterDataStrc*)v60->pPlayerData;
//                if (v63)
//                    v63->pVision = (DWORD)v62;
//            }
//            if (v94->nAlign == 1)
//            {
//                sub_6FCBDD30(v60, 2u, 1);
//                v60->dwFlags |= UNITFLAG_NOTC;
//            }
//            else
//            {
//                if (v94->nAlign == 2)
//                    sub_6FCBDD30(v60, 1u, 1);
//                else
//                    sub_6FCBDD30(v60, 0, 1);
//            }
//            if (!(v90 & 2))
//            {
//                MONSTERSPAWN_SetupNormalMods(v58, v60)
//            }
//            MONSTERSPAWN_SetupBossMods(pMonCreate, v60);
//            if (!(v90 & 0x40))
//                sub_6FC69C00((int32_t)v85, v60);
//            return v60;
//        }
//    }
//    return 0;
}





//D2Game.0x6FC69B60
void __fastcall MONSTERSPAWN_EquipAncientBarbarians(D2UnitStrc* pUnit)
{
    constexpr Unk dword_6FD28B08[] =
    {
        { ' dsb', BODYLOC_RARM, 1, 0, 0 },
        { ' xat', BODYLOC_RARM, 1, 0, 0 },
        { ' uov', BODYLOC_RARM, 1, 0, 0 },
        { ' wot', BODYLOC_LARM, 1, 0, 0 },
        { ' xat', BODYLOC_LARM, 1, 0, 0 },
        { ' nir', BODYLOC_RRIN, 1, 0, 0 },
        { ' dlf', BODYLOC_TORSO, 1, 0, 0 },
        { ' lgh', BODYLOC_GLOVES, 1, 0, 0 },
        { ' dlf', BODYLOC_TORSO, 1, 0, 0 },
        { ' tbh', BODYLOC_FEET, 1, 0, 0 },
        { ' tbh', BODYLOC_FEET, 1, 0, 0 },
        { ' nrc', BODYLOC_HEAD, 1, 0, 0 },
    };

    if (!pUnit)
    {
        return;
    }

    D2GameStrc* pGame = SUNIT_GetGameFromUnit(pUnit);
    const Unk* pItemInfo = &dword_6FD28B08[pUnit->dwClassId - MONSTER_ANCIENTBARB1];
    const int32_t nLevel = STATLIST_UnitGetStatValue(pUnit, STAT_LEVEL, 0);

    for (int32_t i = 0; i < 4; ++i)
    {
        Unk itemInfo = {};

        itemInfo.nItemCode = pItemInfo->nItemCode;
        itemInfo.nBodyLoc = pItemInfo->nBodyLoc;
        itemInfo.unk0x05 = pItemInfo->unk0x05;
        itemInfo.unk0x06 = pItemInfo->unk0x06;
        itemInfo.unk0x07 = pItemInfo->unk0x07;

        int32_t nItemId = 0;
        D2ItemsTxt* pItemsTxtRecord = DATATBLS_GetItemRecordFromItemCode(itemInfo.nItemCode, &nItemId);
        if (pGame->nDifficulty == DIFFMODE_NIGHTMARE)
        {
            itemInfo.nItemCode = pItemsTxtRecord->dwUberCode;
        }
        else if (pGame->nDifficulty == DIFFMODE_HELL)
        {
            itemInfo.nItemCode = pItemsTxtRecord->dwUltraCode;
        }

        sub_6FC61190(pGame, pUnit, &itemInfo, nLevel, 4u);
        pItemInfo += 3;
    }
}

//1.10: Inlined
//1.14d: 0x005B1CF0
void __fastcall MONSTERSPAWN_SetupBossMods(D2UnkMonCreateStrc* pMonCreate, D2UnitStrc* pUnit)
{
    int32_t nMonsterId = pMonCreate->nMonsterId;
    D2GameStrc* pGame = pMonCreate->pGame;

    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);
    int32_t nBaseId;
    if (pMonStatsTxtRecord)
    {
        nBaseId = pMonStatsTxtRecord->nBaseId;
    }
    else
    {
        nBaseId = nMonsterId;
    }

    nBaseId = MONSTERS_ValidateMonsterId(nBaseId);

    switch (nBaseId)
    {
    case MONSTER_ANDARIEL:
#ifdef D2_VERSION_111_UBERS
        if (nMonsterId == MONSTER_UBERANDARIEL)
        {
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_POISONFIELD, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_EXTRAFAST, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_MULTISHOT, 1);
            return;
        }
#endif
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
        QUESTS_CreateChainRecord(pGame, pUnit, 6);
        return;
    case MONSTER_WARRIV2:
        // this call does nothing
        //D2GAME_QUESTS_WarrivSpawn_6FC95740(CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit), MONSTERS_ValidateMonsterId(MONSTER_WARRIV2));
        return;
    case MONSTER_DURIEL:
#ifdef D2_VERSION_111_UBERS
        if (nMonsterId == MONSTER_UBERDURIEL)
        {
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_EXTRAFAST, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_COLDCHANT, 1);
            return;
        }
#endif
        QUESTS_CreateChainRecord(pGame, pUnit, 13);
        QUESTS_CreateChainRecord(pGame, pUnit, 9);
        return;
    case MONSTER_RADAMENT:
        QUESTS_CreateChainRecord(pGame, pUnit, 8);
        return;
    case MONSTER_MEPHISTO:
#ifdef D2_VERSION_111_UBERS
        if (nMonsterId == MONSTER_UBERMEPHISTO)
        {
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_AURACHANT, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_LIGHTCHANT, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_MAGICRESISTANT, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_EXTRAFAST, 1);
            return;
        }
#endif
        QUESTS_CreateChainRecord(pGame, pUnit, 20);
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
        return;
    case MONSTER_DIABLO:
#ifdef D2_VERSION_111_UBERS
        if (nMonsterId == MONSTER_UBERDIABLO)
        {
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_MAGICRESISTANT, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_EXTRAFAST, 1);
            return;
        }
#endif
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
        QUESTS_CreateChainRecord(pGame, pUnit, 23);
        return;
    case MONSTER_SUMMONER:
        QUESTS_CreateChainRecord(pGame, pUnit, 12);
        MONSTERUNIQUE_ToggleUnitFlag(pUnit, 0x800, 1);
        MONSTER_ToggleSummonerFlag(pUnit, 1, 1);
        return;
    case MONSTER_IZUAL:
#ifdef D2_VERSION_111_UBERS
        if (nMonsterId == MONSTER_UBERIZUAL)
        {
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_EXTRAFAST, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_COLDCHANT, 1);
            return;
        }
#endif
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
        QUESTS_CreateChainRecord(pGame, pUnit, 22);
        return;
    case MONSTER_BLOODRAVEN:
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_BRAVENDEATH, 1);
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
        QUESTS_CreateChainRecord(pGame, pUnit, 2);
        STATES_ToggleState(pUnit, STATE_CORPSE_NOSELECT, 1);
        return;
    case MONSTER_MAGGOTQUEEN1:
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_POISONFIELD, 1);
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
        return;
    case MONSTER_FIREGOLEM:
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_CORPSEBOOMDEATH, 0);
        return;
    case MONSTER_BONEPRISON1:
    case MONSTER_BONEPRISON2:
    case MONSTER_BONEPRISON3:
    case MONSTER_BONEPRISON4:
        MONSTERUNIQUE_ToggleUnitFlag(pUnit, 0x20000, 1);
        return;
    case MONSTER_COMPELLINGORB:
        QUESTS_CreateChainRecord(pGame, pUnit, 19);
        MONSTERUNIQUE_ToggleUnitFlag(pUnit, 0x20000, 1);
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
        return;
    case MONSTER_SMITH:
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
        return;
    case MONSTER_FETISH11:
        QUESTS_CreateChainRecord(pGame, pUnit, 17);
        return;
    case MONSTER_HEPHASTO:
        QUESTS_CreateChainRecord(pGame, pUnit, 24);
        return;
    case MONSTER_PRISONDOOR:
        QUESTS_CreateChainRecord(pGame, pUnit, 32);
        return;
    case MONSTER_NIHLATHAKBOSS:
        QUESTS_CreateChainRecord(pGame, pUnit, 34);
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
        return;
    case MONSTER_ANCIENTBARB1:
        MONSTERSPAWN_EquipAncientBarbarians(pUnit);
        return;
    case MONSTER_BAALCRAB:
#ifdef D2_VERSION_111_UBERS
        if (nMonsterId == MONSTER_UBERBAAL)
        {
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_COLDCHANT, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_MAGICRESISTANT, 1);
            D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_EXTRAFAST, 1);
            return;
        }
#endif
        QUESTS_CreateChainRecord(pGame, pUnit, 36);
        D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pUnit, MONUMOD_QUESTMOD, 1);
        return;
    }
}

//1.10: Inlined
//1.14d: 0x005B21B0
//void __fastcall MONSTERSPAWN_SetupNormalMods(D2UnkMonCreateStrc* pMonCreate, D2UnitStrc* pUnit)
//{
//    v64 = v58->nMonsterId;
//    v65 = v58->pGame;
//    if (v64 >= 0)
//    {
//        if (v64 < sgptDataTables->nMonStatsTxtRecordCount)
//        {
//            v66 = &sgptDataTables->pMonStatsTxt[v64];
//            if (v66)
//                v64 = v66->nBaseId;
//        }
//        if (v64 >= 0 && v64 < sgptDataTables->nMonStatsTxtRecordCount)
//        {
//            if (v64 > MONSTER_BONEPRISON4)
//            {
//                switch (v64)
//                {
//                case MONSTER_TRAP_MELEE:
//                    goto LABEL_148;
//                case MONSTER_ANCIENTBARB1:
//                case MONSTER_ANCIENTBARB2:
//                case MONSTER_ANCIENTBARB3:
//                    goto LABEL_150;
//                default:
//                    goto LABEL_152;
//                case MONSTER_SUICIDEMINION1:
//                    v77 = 0;
//                    v75 = MONUMOD_FREEZINDEATH;
//                    break;
//                case MONSTER_REANIMATEDHORDE1:
//                    v77 = 0;
//                    v75 = MONUMOD_SELFRESURRECT;
//                    break;
//                case MONSTER_FROZENHORROR1:
//                    v77 = 0;
//                    v75 = MONUMOD_ICESHATTERDEATH;
//                    break;
//                }
//            }
//            else
//            {
//                if (v64 >= MONSTER_BONEPRISON1)
//                {
//                    v77 = 0;
//                    v75 = MONUMOD_KILLMINIONSDEATH;
//                }
//                else
//                {
//                    if (v64 > MONSTER_MUMMY1)
//                    {
//                        if (v64 == MONSTER_DURIEL)
//                        {
//                            D2GAME_BOSSES_AssignUMod_6FC6FF10(v65, v60, 11, 0);
//LABEL_150:
//                            v77 = 1;
//                            v75 = MONUMOD_QUESTMOD;
//                        }
//                        else
//                        {
//                            if (v64 <= MONSTER_EVILHOLE5 || v64 > MONSTER_TRAP_LIGHTNING)
//                                goto LABEL_152;
//LABEL_148:
//                            v77 = 0;
//                            v75 = MONUMOD_SPCDAMAGE;
//                        }
//                    }
//                    else
//                    {
//                        if (v64 == MONSTER_MUMMY1)
//                        {
//                            v77 = 0;
//                            v75 = MONUMOD_POISDEATH;
//                        }
//                        else
//                        {
//                            if (v64 == MONSTER_BRUTE2)
//                            {
//                                v77 = 0;
//                                v75 = MONUMOD_IGNOREAC;
//                            }
//                            else
//                            {
//                                if (v64 != MONSTER_SCARAB1)
//                                    goto LABEL_152;
//                                v77 = 0;
//                                v75 = MONUMOD_CHARGEDBOLTS;
//                            }
//                        }
//                    }
//                }
//            }
//            D2GAME_BOSSES_AssignUMod_6FC6FF10(v65, v60, v75, v77);
//        }
//    }
//LABEL_152:
//}

//D2Game.0x6FC69C00
void __fastcall sub_6FC69C00(D2UnkMonCreateStrc* pMonCreate, D2UnitStrc* pUnit)
{
    D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(pMonCreate->nMonsterId);
    if (!pMonStatsTxtRecord)
    {
        return;
    }

    int32_t nChainId = 0;
    DATATBLS_GetMonsterChainInfo(pMonCreate->nMonsterId, nullptr, &nChainId);
    if (pMonStatsTxtRecord->wMinion[0] < 0 || pMonStatsTxtRecord->wMinion[0] >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        return;
    }

    const int32_t nBaseId = pMonStatsTxtRecord->nBaseId;
    if (nBaseId < 0 || nBaseId >= sgptDataTables->nMonStatsTxtRecordCount || nBaseId != MONSTER_TENTACLEHEAD1)
    {
        if (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_SETBOSS])
        {
            AIGENERAL_SetOwnerData(pMonCreate->pGame, pUnit, pUnit ? pUnit->dwUnitId : -1, 1, 1, pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_BOSSXFER]);
        }

        int32_t nMinionIndex = 0;
        int32_t nThreshold = 0;
        if (pMonStatsTxtRecord->wMinion[1] >= 0 && pMonStatsTxtRecord->wMinion[1] < sgptDataTables->nMonStatsTxtRecordCount)
        {
            nThreshold = 1;
        }

        int32_t nMinions = pMonStatsTxtRecord->nPartyMin;
        if (nMinions < pMonStatsTxtRecord->nPartyMax)
        {
            nMinions += ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, pMonStatsTxtRecord->nPartyMax - nMinions + 1);
        }

        for (int32_t i = 0; i < nMinions; ++i)
        {
            D2UnkMonCreateStrc monCreate = {};
            monCreate.nUnitGUID = 0;
            monCreate.nAnimMode = 1;
            monCreate.nMonsterId = pMonStatsTxtRecord->wMinion[nMinionIndex];
            monCreate.field_20 = 4;
            monCreate.pRoomCoordList = nullptr;
            monCreate.pRoom = UNITS_GetRoom(pUnit);
            monCreate.pGame = pMonCreate->pGame;
            monCreate.nFlags = 64;
            monCreate.nX = CLIENTS_GetUnitX(pUnit);
            monCreate.nY = CLIENTS_GetUnitY(pUnit);

            D2UnitStrc* pMonster = D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
            if (pMonster && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_SETBOSS])
            {
                AIGENERAL_SetOwnerData(pMonCreate->pGame, pMonster, pUnit->dwUnitId, 1, 0, 0);
                AIGENERAL_AllocMinionList(pMonCreate->pGame, pUnit, pMonster);
            }

            ++nMinionIndex;

            if (nMinionIndex > nThreshold)
            {
                nMinionIndex = 0;
            }
        }
    }
    else
    {
        AIGENERAL_SetOwnerData(pMonCreate->pGame, pUnit, pUnit ? pUnit->dwUnitId : -1, 1, 1, pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_BOSSXFER]);
        int32_t nMinions = pMonStatsTxtRecord->nPartyMin;
        if (nMinions < pMonStatsTxtRecord->nPartyMax)
        {
            nMinions += ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, pMonStatsTxtRecord->nPartyMax - nMinions + 1);
        }
        uint32_t nFlags = (uint32_t)pMonCreate->nFlags;
        nFlags &= (0xFFFFFF00 & ~(uint8_t)pMonCreate->nFlags);
        sub_6FC6A8C0(pMonCreate->pGame, pUnit, pMonStatsTxtRecord->wMinion[0], 1, nMinions, ((nFlags >> 2) & 1), 0x40u);
    }
}

//D2Game.0x6FC69F10
D2UnitStrc* __fastcall D2GAME_SpawnMonster_6FC69F10(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nAnimMode, int32_t a7, int16_t nFlags)
{
    D2UnkMonCreateStrc monCreate = {};

    monCreate.pRoom = pRoom;
    monCreate.pGame = pGame;
    monCreate.nUnitGUID = 0;
    monCreate.nMonsterId = nMonsterId;
    monCreate.field_20 = a7;
    monCreate.pRoomCoordList = nullptr;
    monCreate.nX = nX;
    monCreate.nAnimMode = nAnimMode;
    monCreate.nFlags = nFlags;
    monCreate.nY = nY;

    return D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
}

//D2Game.0x6FC69F70
D2UnitStrc* __fastcall sub_6FC69F70(D2GameStrc* pGame, D2RoomCoordListStrc* pRoomCoordList, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nAnimMode, int32_t a6, int16_t nFlags)
{
    D2UnkMonCreateStrc monCreate = {};

    monCreate.nUnitGUID = 0;
    monCreate.nMonsterId = nMonsterId;
    monCreate.nAnimMode = nAnimMode;
    monCreate.pGame = pGame;
    monCreate.pRoomCoordList = pRoomCoordList;
    monCreate.pRoom = UNITS_GetRoom(pUnit);
    monCreate.field_20 = a6;
    monCreate.nFlags = nFlags;
    monCreate.nX = CLIENTS_GetUnitX(pUnit);
    monCreate.nY = CLIENTS_GetUnitY(pUnit);

    return D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
}

//D2Game.0x6FC6A030
D2UnitStrc* __fastcall sub_6FC6A030(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, D2RoomCoordListStrc* pRoomCoordList, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nAnimMode, int32_t a8, int16_t nFlags)
{
    D2UnkMonCreateStrc monCreate = {};

    monCreate.pRoom = pRoom;
    monCreate.nMonsterId = nMonsterId;
    monCreate.pGame = pGame;
    monCreate.nAnimMode = nAnimMode;
    monCreate.field_20 = a8;
    monCreate.pRoomCoordList = pRoomCoordList;
    monCreate.nFlags = nFlags;
    monCreate.nUnitGUID = 0;
    monCreate.nX = nX;
    monCreate.nY = nY;

    return D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
}

//D2Game.0x6FC6A090
D2UnitStrc* __fastcall sub_6FC6A090(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nAnimMode, int16_t nFlags)
{
    D2UnkMonCreateStrc monCreate = {};

    monCreate.pGame = pGame;
    monCreate.nUnitGUID = 0;
    monCreate.nMonsterId = nMonsterId;
    monCreate.pRoom = pRoom;
    monCreate.pRoomCoordList = nullptr;
    monCreate.nX = nX;
    monCreate.nAnimMode = nAnimMode;
    monCreate.field_20 = -1;
    monCreate.nFlags = nFlags;
    monCreate.nY = nY;

    return D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
}

//D2Game.0x6FC6A0F0
D2UnitStrc* __fastcall sub_6FC6A0F0(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, int32_t nMonsterId, int32_t nMode, int32_t nUnitId, int32_t a8, int16_t nFlags)
{
    D2UnkMonCreateStrc monCreate = {};

    monCreate.pRoom = pRoom;
    monCreate.nUnitGUID = nUnitId;
    monCreate.pGame = pGame;
    monCreate.nMonsterId = nMonsterId;
    monCreate.field_20 = a8;
    monCreate.nAnimMode = nMode;
    monCreate.nX = nX;
    monCreate.pRoomCoordList = nullptr;
    monCreate.nFlags = nFlags;
    monCreate.nY = nY;

    return D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
}

//D2Game.0x6FC6A150
D2UnitStrc* __fastcall sub_6FC6A150(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nAnimMode, int32_t a5, int16_t nFlags)
{
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    if (DUNGEON_GetLevelIdFromPopulatedRoom(pRoom))
    {
        D2UnkMonCreateStrc monCreate = {};
        monCreate.nUnitGUID = 0;
        monCreate.nMonsterId = D2Common_11063(pRoom, nMonsterId);
        monCreate.nAnimMode = nAnimMode;
        monCreate.pGame = pGame;
        monCreate.pRoomCoordList = nullptr;
        monCreate.pRoom = pRoom;
        monCreate.field_20 = a5;
        monCreate.nFlags = nFlags;
        monCreate.nX = CLIENTS_GetUnitX(pUnit);
        monCreate.nY = CLIENTS_GetUnitY(pUnit);

        return D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
    }

    return nullptr;
}

//D2Game.0x6FC6A230
int32_t __fastcall sub_6FC6A230(D2GameStrc* pGame, D2UnitStrc* pOwner, int32_t nMonsterId, int32_t nAnimMode, int32_t a5, int32_t nCount, int16_t nFlags)
{
    if (!pOwner)
    {
        return 0;
    }

    int32_t nResult = 0;
    const int32_t nOwnerId = pOwner->dwUnitId;

    D2UnkMonCreateStrc monCreate = {};

    monCreate.nUnitGUID = 0;
    monCreate.nMonsterId = nMonsterId;
    monCreate.nAnimMode = nAnimMode;
    monCreate.pGame = pGame;
    monCreate.pRoomCoordList = nullptr;
    monCreate.pRoom = UNITS_GetRoom(pOwner);
    monCreate.nFlags = nFlags;
    monCreate.field_20 = a5;
    monCreate.nX = CLIENTS_GetUnitX(pOwner);
    monCreate.nY = CLIENTS_GetUnitY(pOwner);

    for (int32_t i = 0; i < nCount; ++i)
    {
        D2UnitStrc* pMinion = D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
        if (pMinion)
        {
            nResult = 1;
            AIGENERAL_SetOwnerData(pGame, pMinion, nOwnerId, 1, 0, 0);
            AIGENERAL_AllocMinionList(pGame, pOwner, pMinion);
        }
    }

    return nResult;
}

//D2Game.0x6FC6A350
int32_t __fastcall sub_6FC6A350(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY, D2UnitStrc* pOwner, int32_t nClassId, int32_t nAnimMode, D2UnkMonCreateStrc2* a8, int16_t nFlags)
{
    D2UnitStrc* pUnit = nullptr;
    
    int32_t bIncreaseClassId = 0;
    if (a8->unk0x08 & 1)
    {
        bIncreaseClassId = 1;
    }

    int32_t bUseDefaultSpawnType = 0;
    if (nFlags & 1)
    {
        bUseDefaultSpawnType = 1;
    }

    int32_t nResult = 0;
    for (int32_t i = 0; i < a8->nRecordCount; ++i)
    {
        int32_t nSpawnType = 4;
        if (!bUseDefaultSpawnType)
        {
            nSpawnType = a8->records[i].nSpawnType;
        }

        switch (nSpawnType)
        {
        case 0:
        {
            D2UnkMonCreateStrc monCreate = {};
            monCreate.pGame = pGame;
            monCreate.pRoom = pRoom;
            monCreate.pRoomCoordList = nullptr;
            monCreate.nMonsterId = nClassId;
            monCreate.nAnimMode = nAnimMode;
            monCreate.nUnitGUID = 0;
            monCreate.nX = nX + a8->records[i].nXOffset;
            monCreate.nY = nY + a8->records[i].nYOffset;
            monCreate.field_20 = -1;
            monCreate.nFlags = nFlags;

            if (D2GAME_SpawnNormalMonster_6FC68E30(&monCreate))
            {
                nResult = 1;
            }

            break;
        }
        case 1:
        {
            D2UnkMonCreateStrc monCreate = {};
            monCreate.pGame = pGame;
            monCreate.pRoom = pRoom;
            monCreate.pRoomCoordList = nullptr;
            monCreate.nMonsterId = nClassId;
            monCreate.nAnimMode = nAnimMode;
            monCreate.nUnitGUID = 0;
            monCreate.nX = nX + a8->records[i].nXOffset;
            monCreate.nY = nY + a8->records[i].nYOffset;
            monCreate.field_20 = -1;
            monCreate.nFlags = 0;

            pUnit = D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
            if (pUnit)
            {
                AIGENERAL_SetOwnerData(pGame, pUnit, pUnit->dwUnitId, 1, 1, 0);
                nResult = 1;
            }
            break;
        }
        case 2:
        {
            D2UnkMonCreateStrc monCreate = {};
            monCreate.pGame = pGame;
            monCreate.pRoom = pRoom;
            monCreate.pRoomCoordList = nullptr;
            monCreate.nMonsterId = nClassId;
            monCreate.nAnimMode = nAnimMode;
            monCreate.nUnitGUID = 0;
            monCreate.nX = nX + a8->records[i].nXOffset;
            monCreate.nY = nY + a8->records[i].nYOffset;
            monCreate.field_20 = -1;
            monCreate.nFlags = 0;

            pUnit = D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
            if (pUnit)
            {
                AIGENERAL_SetOwnerData(pGame, pUnit, pUnit->dwUnitId, 1, 1, 1);
                nResult = 1;
            }
            break;
        }
        case 3:
        {
            if (pUnit)
            {
                D2UnkMonCreateStrc monCreate = {};
                monCreate.pGame = pGame;
                monCreate.pRoom = UNITS_GetRoom(pUnit);
                monCreate.pRoomCoordList = nullptr;
                monCreate.nMonsterId = nClassId;
                monCreate.nAnimMode = nAnimMode;
                monCreate.nUnitGUID = 0;
                monCreate.nX = nX + a8->records[i].nXOffset;
                monCreate.nY = nY + a8->records[i].nYOffset;
                monCreate.field_20 = -1;
                monCreate.nFlags = 0;

                D2UnitStrc* pMonster = D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
                if (pMonster)
                {
                    AIGENERAL_SetOwnerData(pGame, pMonster, pUnit->dwUnitId, 1, 0, 0);
                    AIGENERAL_AllocMinionList(pGame, pUnit, pMonster);
                    nResult = 1;
                }
            }
            break;
        }
        case 4:
        {
            D2UnkMonCreateStrc monCreate = {};
            monCreate.pGame = pGame;
            monCreate.pRoom = pRoom;
            monCreate.pRoomCoordList = nullptr;
            monCreate.nMonsterId = nClassId;
            monCreate.nAnimMode = nAnimMode;
            monCreate.nUnitGUID = 0;
            monCreate.nX = nX + a8->records[i].nXOffset;
            monCreate.nY = nY + a8->records[i].nYOffset;
            monCreate.field_20 = -1;
            monCreate.nFlags = nFlags;

            nResult = 1;
            
            if (!D2GAME_SpawnNormalMonster_6FC68E30(&monCreate))
            {
                return 0;
            }

            break;
        }
        case 5:
        {
            if (pOwner)
            {
                D2UnkMonCreateStrc monCreate = {};
                monCreate.pGame = pGame;
                monCreate.pRoom = pRoom;
                monCreate.pRoomCoordList = nullptr;
                monCreate.nMonsterId = nClassId;
                monCreate.nAnimMode = nAnimMode;
                monCreate.nUnitGUID = 0;
                monCreate.nX = nX + a8->records[i].nXOffset;
                monCreate.nY = nY + a8->records[i].nYOffset;
                monCreate.field_20 = -1;
                monCreate.nFlags = 0;
                
                D2UnitStrc* pMonster = D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
                if (pMonster)
                {
                    AIGENERAL_SetOwnerData(pGame, pMonster, pOwner->dwUnitId, pOwner->dwUnitType, 0, 0);
                    nResult = 1;
                }
            }
            break;
        }
        default:
            break;
        }

        if (bIncreaseClassId)
        {
            ++nClassId;
        }
    }

    return nResult;
}

//D2Game.0x6FC6A810
int32_t __fastcall sub_6FC6A810(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t a3, int32_t a4, D2UnitStrc* pTargetUnit, int32_t a6, int16_t a7)
{
    D2UnkMonCreateStrc2 dword_6FD2EEAC =
    {
        4, 0, 1, 0, 0, 0,
        {
            { 1, 1, 1 },
            { 1, -1, 3 },
            { -1, -1, 3 },
            { -1, 1, 3 }
        }
    };

    if (a6 == MONSTER_BONEPRISON1 && pTargetUnit)
    {
        return sub_6FC6A350(pGame, pRoom, CLIENTS_GetUnitX(pTargetUnit), CLIENTS_GetUnitY(pTargetUnit), 0, MONSTER_BONEPRISON1, 8, &dword_6FD2EEAC, a7);
    }

    return 0;
}

//D2Game.0x6FC6A8C0
int32_t __fastcall sub_6FC6A8C0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nMonsterId, int32_t nAnimMode, int32_t nCount, int32_t a6, int16_t nFlags)
{
    D2CoordStrc stru_6FD28B68[12] =
    {
        {-1,-4 },
        { 1, 4 },
        { 1,-3 },
        {-1, 3 },
        { 0, 2 },
        { 0,-2 },
        {-3,-1 },
        { 3, 1 },
        { 2,-1 },
        {-2, 1 },
        { 1, 0 },
        {-1, 0 }
    };

    if (!pUnit)
    {
        return 0;
    }

    int32_t v9 = a6;
    if (a6 < 0 || a6 > 2)
    {
        v9 = 0;
    }

    int32_t nResult = 0;
    D2ActiveRoomStrc* pRoom = UNITS_GetRoom(pUnit);
    const int32_t nX = CLIENTS_GetUnitX(pUnit);
    const int32_t nY = CLIENTS_GetUnitY(pUnit);

    int32_t nParam1 = ITEMS_RollRandomNumber(&pUnit->pSeed) % 6;
    const int32_t nParam2 = 6 * v9;

    for (int32_t i = 0; i < nCount; ++i)
    {
        const int32_t nIndex = 2 * (nParam1 + nParam2);

        D2UnkMonCreateStrc monCreate = {};
        monCreate.nUnitGUID = 0;
        monCreate.pRoomCoordList = nullptr;
        monCreate.field_20 = -1;
        monCreate.nX = nX + stru_6FD28B68[nIndex].nX;
        monCreate.nY = nY + stru_6FD28B68[nIndex].nY;
        monCreate.nFlags = nFlags;
        monCreate.nMonsterId = nMonsterId;
        monCreate.nAnimMode = nAnimMode;
        monCreate.pRoom = pRoom;
        monCreate.pGame = pGame;

        nParam1 = (nParam1 + 5) % 6;

        D2UnitStrc* pMinion = D2GAME_SpawnNormalMonster_6FC68E30(&monCreate);
        if (pMinion)
        {
            nResult = 1;
            AIGENERAL_SetOwnerData(pGame, pMinion, pUnit->dwUnitId, 1, 0, 0);
            AIGENERAL_AllocMinionList(pGame, pUnit, pMinion);
        }
    }

    return nResult;
}

//D2Game.0x6FC6AA70
int32_t __fastcall MONSTERSPAWN_SpawnRandomMonsterForLevel(D2GameStrc* pGame, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY)
{
    D2LevelsTxt* pLevelsTxtRecord = DATATBLS_GetLevelsTxtRecord(DUNGEON_GetLevelIdFromRoom(pRoom));
    if (!pLevelsTxtRecord->nNumNormalMonsters)
    {
        FOG_DisplayWarning("ptLevelData->bMonTot", __FILE__, __LINE__);
        return 0;
    }

    int32_t nIndex = ITEMS_RollLimitedRandomNumber(pRoom ? &pRoom->pSeed : nullptr, pLevelsTxtRecord->nNumNormalMonsters);

    for (int32_t i = 0; i < pLevelsTxtRecord->nNumNormalMonsters; ++i)
    {
        ++nIndex;
        if (nIndex >= pLevelsTxtRecord->nNumNormalMonsters)
        {
            nIndex = 0;
        }

        const int32_t nMonsterId = pLevelsTxtRecord->wNormalMonsters[nIndex];
        D2MonStatsTxt* pMonStatsTxtRecord = MONSTERMODE_GetMonStatsTxtRecord(nMonsterId);

        if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_ISSPAWN])
        {
            if (nMonsterId != -1)
            {
                return D2GAME_SpawnMonster_6FC69F10(pGame, pRoom, nX, nY, nMonsterId, MONMODE_NEUTRAL, 5, 0) != nullptr;
            }
            break;
        }
    }

    return 0;
}
