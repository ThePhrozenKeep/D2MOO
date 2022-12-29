#include "DEBUG/Debug.h"

#include <D2Dungeon.h>


#include "GAME/Game.h"
#include "UNIT/SUnit.h"


//D2Game.0x6FCCD2E0 (#10025)
D2UnitStrc* __stdcall DEBUG_GetUnitFromTypeAndGUID(int32_t nUnitType, int32_t nUnitGUID)
{
    D2GameStrc* pGame = sub_6FC3B160();
    if (!pGame)
    {
        return nullptr;
    }

    D2UnitStrc* pUnit = SUNIT_GetServerUnit(pGame, nUnitType, nUnitGUID);
    GAME_LeaveGlobalGamesCriticalSection();
    return pUnit;
}

//D2Game.0x6FCCD310 (#10026)
D2DrlgActStrc* __stdcall DEBUG_GetAct(uint8_t nAct)
{
    D2GameStrc* pGame = sub_6FC3B160();

    D2_ASSERT(pGame);

    D2DrlgActStrc* pAct = pGame->pAct[nAct];
    GAME_LeaveGlobalGamesCriticalSection();
    return pAct;
}

//D2Game.0x6FCCD350 (#10027)
int32_t __stdcall DEBUG_GetFreeMonsterIndex()
{
    D2GameStrc* pGame = sub_6FC3B160();

    D2_ASSERT(pGame);

    int32_t nIndex = 0;
    for (int32_t i = 0; i < 128; ++i)
    {
        for (D2UnitStrc* pUnit = pGame->pUnitList[1][i]; pUnit; pUnit = SUNIT_GetNextUnitFromList(pUnit))
        {
            ++nIndex;
        }
    }

    GAME_LeaveGlobalGamesCriticalSection();
    return nIndex;
}

//D2Game.0x6FCCD3B0 (#10028)
D2RoomStrc* __stdcall DEBUG_GetRoomBySubtileCoordinates(uint8_t nAct, int32_t nX, int32_t nY)
{
    D2GameStrc* pGame = sub_6FC3B160();

    D2_ASSERT(nAct < NUM_ACTS);

    D2RoomStrc* pRoom = DUNGEON_FindRoomBySubtileCoordinates(pGame->pAct[nAct], nX, nY);
    GAME_LeaveGlobalGamesCriticalSection();
    return pRoom;
}
