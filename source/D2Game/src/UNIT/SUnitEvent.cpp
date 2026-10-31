#include "UNIT/SUnitEvent.h"

#include <Fog.h>

#include "GAME/Game.h"

//D2Game.0x6FCC3610
//1.14d: 0x005C0A90
void __fastcall SUNITEVENT_FreeEventList(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return;
    }

    D2UnitEventStrc* pUnitEvent = pUnit->pSrvUnitEventsList;
    while (pUnitEvent)
    {
        D2UnitEventStrc* pNext = pUnitEvent->pNext;
        D2_FREE_POOL(pGame->pMemoryPool, pUnitEvent);
        pUnitEvent = pNext;
    }

    pUnit->pSrvUnitEventsList = nullptr;
}

//D2Game.0x6FCC3650
//1.14d: 0x005C0AD0
D2UnitEventStrc* __fastcall SUNITEVENT_Register(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_UnitEventTypes nUnitEvent, int32_t nParam1, int32_t nParam2, D2UnitEventCallbackFunction pCallback, uint32_t nEventGroup, int32_t nEventKey)
{
    if (!pUnit)
    {
        return nullptr;
    }

    D2UnitEventStrc* pUnitEvent = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2UnitEventStrc);

    pUnitEvent->nUnitEvent = nUnitEvent;
    pUnitEvent->nParam1 = nParam1;
    pUnitEvent->nParam2 = nParam2;
    pUnitEvent->pCallback = pCallback;
    pUnitEvent->pNext = pUnit->pSrvUnitEventsList;
    pUnitEvent->nEventGroup = nEventGroup;
    pUnitEvent->nEventKey = nEventKey;

    if (pUnit->pSrvUnitEventsList)
    {
        pUnit->pSrvUnitEventsList->pPrevious = pUnitEvent;
    }

    pUnit->pSrvUnitEventsList = pUnitEvent;
    return pUnitEvent;
}

// Helper function
static void __fastcall SUNITEVENT_FreeEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, D2UnitEventStrc* pUnitEvent)
{
	D2UnitEventStrc* pNext = pUnitEvent->pNext;
	if (pUnitEvent->pPrevious)
	{
		pUnitEvent->pPrevious->pNext = pNext;
	}
	else
	{
		pUnit->pSrvUnitEventsList = pNext;
	}

	if (pUnitEvent->pNext)
	{
		pUnitEvent->pNext->pPrevious = pUnitEvent->pPrevious;
	}

	D2_FREE_POOL(pGame->pMemoryPool, pUnitEvent);
}

//D2Game.0x6FCC36D0
//1.14d: 0x005C0B50
void __fastcall SUNITEVENT_Unregister(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventGroup, int32_t nEventKey)
{
    if (!pUnit)
    {
        return;
    }

    D2UnitEventStrc* pUnitEvent = pUnit->pSrvUnitEventsList;
    while (pUnitEvent)
    {
        D2UnitEventStrc* pNext = pUnitEvent->pNext;
        if (pUnitEvent->nEventGroup == nEventGroup && pUnitEvent->nEventKey == nEventKey)
        {
            if (pUnitEvent->nFlags & UNITEVENTFLAG_IN_CALLBACK)
            {
                pUnitEvent->nFlags |= UNITEVENTFLAG_SHOULD_BE_FREED;
            }
            else
            {
				SUNITEVENT_FreeEvent(pGame, pUnit, pUnitEvent);
            }
        }

        pUnitEvent = pNext;
    }
}

//D2Game.0x6FCC3750
//1.14d: 0x005C0BE0
D2UnitEventStrc* __fastcall SUNITEVENT_GetEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventGroup, int32_t nEventKey, int32_t nParam1)
{
    if (!pUnit)
    {
        return nullptr;
    }

    for (D2UnitEventStrc* pUnitEvent = pUnit->pSrvUnitEventsList; pUnitEvent; pUnitEvent = pUnitEvent->pNext)
    {
        if (pUnitEvent->nEventGroup == nEventGroup && pUnitEvent->nEventKey == nEventKey && pUnitEvent->nParam1 == nParam1)
        {
            return pUnitEvent;
        }
    }

    return nullptr;
}

//D2Game.0x6FCC3790
//1.14d: 0x005C0C30
int32_t __fastcall SUNITEVENT_Trigger(D2GameStrc* pGame, D2C_UnitEventTypes nUnitEvent, D2UnitStrc* pUnit, D2UnitStrc* pEventSourceUnit, D2DamageStrc* pDamage)
{
    if (!pUnit)
    {
        return 0;
    }

    int32_t nResult = 0;
    D2UnitEventStrc* pUnitEvent = pUnit->pSrvUnitEventsList;
    while (pUnitEvent)
    {
        D2UnitEventStrc* pNext = pUnitEvent->pNext;
        if (pUnitEvent->nUnitEvent == nUnitEvent)
        {
            pUnitEvent->nFlags |= UNITEVENTFLAG_IN_CALLBACK;
            nResult = pUnitEvent->pCallback(pGame, nUnitEvent, pUnit, pEventSourceUnit, pDamage, pUnitEvent->nParam1, pUnitEvent->nParam2);
            pUnitEvent->nFlags &= ~UNITEVENTFLAG_IN_CALLBACK;

            if ((pUnitEvent->nFlags & UNITEVENTFLAG_SHOULD_BE_FREED) || !pUnitEvent->nEventGroup)
            {
				SUNITEVENT_FreeEvent(pGame, pUnit, pUnitEvent);
            }
        }
        pUnitEvent = pNext;
    }

    return nResult;
}
