#include "UNIT/Friendly.h"

#include <Fog.h>

#include "PLAYER/PlayerList.h"


//D2Game.0x6FCB9A50
int32_t __fastcall FRIENDLY_RemoveHostility(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2)
{
    D2_ASSERT(pPlayer1 && pPlayer1->dwUnitType == UNIT_PLAYER);

    PLAYERLIST_AllocPlayerList(pGame, pPlayer1, pPlayer2);
    PLAYERLIST_ToggleFlag(pGame, pPlayer1, pPlayer2, 8, 0);
    return 1;
}

//D2Game.0x6FCB9AB0
void __fastcall FRIENDLY_OpenHostility(D2GameStrc* pGame, D2UnitStrc* pPlayer1, D2UnitStrc* pPlayer2)
{
    D2_ASSERT(pPlayer1 && pPlayer1->dwUnitType == UNIT_PLAYER);

    PLAYERLIST_AllocPlayerList(pGame, pPlayer1, pPlayer2);
    PLAYERLIST_ToggleFlag(pGame, pPlayer1, pPlayer2, 8, 1);
}
