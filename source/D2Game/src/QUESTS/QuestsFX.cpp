#include "QUESTS/QuestsFX.h"

#include <algorithm>

#include "D2DataTbls.h"
#include <DataTbls/MissilesIds.h>
#include <DataTbls/MonsterIds.h>
#include <DataTbls/ObjectsIds.h>
#include <DataTbls/ObjectsTbls.h>
#include <Units/UnitFinds.h>
#include "Units/Missile.h"
#include "D2Monsters.h"
#include "D2StatList.h"
#include "D2Dungeon.h"


#include "GAME/Clients.h"
#include "GAME/Event.h"
#include "ITEMS/ItemMode.h"
#include "ITEMS/Items.h"
#include "MISSILES/Missiles.h"
#include "MONSTER/MonsterUnique.h"
#include "SKILLS/Skills.h"


//D2Game.0x6FCB95A0
void __fastcall QUESTSFX_Bloodraven(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    return QUESTSFX_MainHandler(pGame, pUnit, 35, 25, 125, 1);
}

//D2Game.0x6FCB95B0
void __fastcall QUESTSFX_MainHandler(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nSize, int32_t nDelayMin, int32_t nDelayMax, int32_t bCheckIfUndead)
{
    D2_ASSERT(nDelayMax);
    D2_ASSERT(nDelayMax > nDelayMin);

    D2UnitFindArgStrc unitFindArg = {};
    unitFindArg.nX = CLIENTS_GetUnitX(pUnit);
    unitFindArg.nY = CLIENTS_GetUnitY(pUnit);
    unitFindArg.nSize = nSize;
    unitFindArg.nFlags = 0x583;
    unitFindArg.pUnit = pUnit;

    D2UnitFindDataStrc unitFindData = {};
    UNITFINDS_InitializeUnitFindData(pGame->pMemoryPool, &unitFindData, UNITS_GetRoom(pUnit), unitFindArg.nX, unitFindArg.nY, nSize, nullptr, &unitFindArg);
    UNITFINDS_FindAllMatchingUnitsInNeighboredRooms(&unitFindData);

    for (int32_t i = 0; i < unitFindData.nIndex; ++i)
    {
        D2UnitStrc* pMonster = unitFindData.pUnitsArray[i];
        if (pMonster && pMonster->dwUnitType == UNIT_MONSTER)
        {
            int32_t bAllied = 0;
            if (STATLIST_GetUnitAlignment(pMonster) == UNIT_ALIGNMENT_EVIL)
            {
                bAllied = 1;
            }

            if ((!bCheckIfUndead || MONSTERS_IsUndead(pMonster)) && bAllied)
            {
                EVENT_SetEvent(pGame, pMonster, EVENTTYPE_MONUMOD, nDelayMin + ITEMS_RollLimitedRandomNumber(&pUnit->pSeed, nDelayMax - nDelayMin) + pGame->dwGameFrame, 0, 0);
                D2GAME_BOSSES_AssignUMod_6FC6FF10(pGame, pMonster, MONUMOD_TEMPSUMMON, 0);
            }
        }
    }
    
    UNITFINDS_FreeUnitFindData(&unitFindData);
}

//D2Game.0x6FCB97D0
void __fastcall QUESTSFX_SpawnMephistoDeathControlMissile(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2MissileStrc missileParams = {};

    missileParams.pOwner = pUnit;
    missileParams.pOrigin = pUnit;
    missileParams.dwFlags = 0x8000u;
    missileParams.nMissile = MISSILE_MEPHISTODEATHCONTROL;
    missileParams.nSkillLevel = 1;
    missileParams.nRange = 100;

    MISSILES_CreateMissileFromParams(pGame, &missileParams);
}

//D2Game.0x6FCB9820
void __fastcall QUESTSFX_Andariel(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    return QUESTSFX_MainHandler(pGame, pUnit, 35, 1, 51, 0);
}

//D2Game.0x6FCB9830
void __fastcall QUESTSFX_SpawnRadamentDeathMissile(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2MissilesTxt* pMissilesTxtRecord = SKILLS_GetMissilesTxtRecord(MISSILE_RADAMENTDEATH);

    const int32_t nDelayMax = std::max(pMissilesTxtRecord->wRange - 100, 100);

    QUESTSFX_MainHandler(pGame, pUnit, 35, 40, nDelayMax, 0);

    D2GAME_CreateMissile_6FD115E0(pGame, pUnit, 0, 1, MISSILE_RADAMENTDEATH, CLIENTS_GetUnitX(pUnit), CLIENTS_GetUnitY(pUnit));
}

//D2Game.0x6FCB98F0
void __fastcall QUESTSFX_SpawnCompellingOrbMissile(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2UnitStrc* pStairs = UNITFINDS_FindUnitInNeighboredRooms(UNITS_GetRoom(pUnit), UNIT_OBJECT, 386);

    D2_ASSERT(pStairs);

    D2MissileStrc missileParams = {};
    missileParams.dwFlags = 0x420;
    missileParams.nMissile = MISSILE_ORBMIST;
    missileParams.pOwner = pUnit;
    missileParams.pOrigin = pUnit;
    missileParams.nTargetX = CLIENTS_GetUnitX(pStairs);
    missileParams.nTargetY = CLIENTS_GetUnitY(pStairs);
    missileParams.nSkillLevel = 1;

    D2UnitStrc* pMissile = MISSILES_CreateMissileFromParams(pGame, &missileParams);
    if (pMissile)
    {
        MISSILE_SetTargetX(pMissile, pStairs->dwUnitId);
        DUNGEON_ToggleHasPortalFlag(UNITS_GetRoom(pStairs), 0);
    }
    else if (pStairs->dwAnimMode == OBJMODE_NEUTRAL)
    {
        UNITS_ChangeAnimMode(pStairs, OBJMODE_OPERATING);
        D2ObjectsTxt* pObjectsTxtRecord = DATATBLS_GetObjectsTxtRecord(386u);
        EVENT_SetEvent(pGame, pStairs, EVENTTYPE_ENDANIM, pGame->dwGameFrame + (pObjectsTxtRecord->dwFrameCnt[1] >> 8), 0, 0);
    }
}

//D2Game.0x6FCB9A30
void __fastcall QUESTSFX_Izual(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    return QUESTSFX_MainHandler(pGame, pUnit, 105, 1, 2, 0);
}

//D2Game.0x6FCB9A40
void __fastcall QUESTSFX_ShenkTheOverseer(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    return QUESTSFX_MainHandler(pGame, pUnit, 35, 25, 125, 0);
}

#ifdef D2_VERSION_HAS_UBERS
//1.14d: 0x005E0070
void __fastcall QUESTSFX_UberPrimeEvil(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    int32_t nMonsterId;

    if (pUnit)
    {
        nMonsterId = pUnit->dwClassId;
    }
    else
    {
        nMonsterId = -1;
    }

    switch (nMonsterId)
    {
    case MONSTER_UBERMEPHISTO:
        pGame->bUberMephistoKilled = 1;
        break;
    case MONSTER_UBERDIABLO:
        pGame->bUberDiabloKilled = 1;
        break;
    case MONSTER_UBERBAAL:
        pGame->bUberBaalKilled = 1;
        break;
    default:
        //ASSERT();
        return;
    }

    if (pGame->bUberMephistoKilled && pGame->bUberDiabloKilled && pGame->bUberBaalKilled)
    {
        int32_t nItemLevel;
        pUnit->dwDropItemCode = ' 2mc';
        D2UnitStrc* pItem = D2GAME_DropItemAtUnit_6FC4FEC0(pGame, pUnit, ITEMQUAL_UNIQUE, &nItemLevel, 0, -1, 0);
        D2GAME_RechargeItem_6FC4BD50(pGame, nullptr, pItem);
        pUnit->dwDropItemCode = ' dts';
        for (int32_t i = 0; i < pGame->nClients; i++)
        {
            D2GAME_DropItemAtUnit_6FC4FEC0(pGame, pUnit, ITEMQUAL_NORMAL, &nItemLevel, 0, -1, 0);
        }
    }
}
#endif
