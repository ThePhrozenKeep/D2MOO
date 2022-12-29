#include "UNIT/SUnitEvent.h"

#include <Fog.h>

#include "GAME/Game.h"

//D2Game.0x6FCC3610
void __fastcall SUNITEVENT_FreeTimerList(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (!pUnit)
    {
        return;
    }

    D2UnitEventStrc* pTimer = pUnit->pSrvTimerList;
    while (pTimer)
    {
        D2UnitEventStrc* pNext = pTimer->pNext;
        D2_FREE_POOL(pGame->pMemoryPool, pTimer);
        pTimer = pNext;
    }

    pUnit->pSrvTimerList = nullptr;
}

//D2Game.0x6FCC3650
D2UnitEventStrc* __fastcall SUNITEVENT_AllocTimer(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nTimerType, int32_t nGUID1, int32_t nGUID2, D2UnitEventCallbackFunction pCallback, uint32_t nQueueNo, int32_t a8)
{
    if (!pUnit)
    {
        return nullptr;
    }

    D2UnitEventStrc* pTimer = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2UnitEventStrc);

    pTimer->nTimerType = nTimerType;
    pTimer->nGUID1 = nGUID1;
    pTimer->nGUID2 = nGUID2;
    pTimer->pCallback = pCallback;
    pTimer->pNext = pUnit->pSrvTimerList;
    pTimer->nQueueNo = nQueueNo;
    pTimer->unk0x08 = a8;

    if (pUnit->pSrvTimerList)
    {
        pUnit->pSrvTimerList->pPrevious = pTimer;
    }

    pUnit->pSrvTimerList = pTimer;
    return pTimer;
}

//D2Game.0x6FCC36D0
void __fastcall SUNITEVENT_FreeTimer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTimerQueueNo, int32_t a4)
{
    if (!pUnit)
    {
        return;
    }

    D2UnitEventStrc* pTimer = pUnit->pSrvTimerList;
    while (pTimer)
    {
        D2UnitEventStrc* pNext = pTimer->pNext;
        if (pTimer->nQueueNo == nTimerQueueNo && pTimer->unk0x08 == a4)
        {
            if (pTimer->nTimerFlags & 1)
            {
                pTimer->nTimerFlags |= 2;
            }
            else
            {
                if (pTimer->pPrevious)
                {
                    pTimer->pPrevious->pNext = pNext;
                }
                else
                {
                    pUnit->pSrvTimerList = pNext;
                }

                if (pTimer->pNext)
                {
                    pTimer->pNext->pPrevious = pTimer->pPrevious;
                }

                D2_FREE_POOL(pGame->pMemoryPool, pTimer);
            }
        }

        pTimer = pNext;
    }
}

//D2Game.0x6FCC3750
D2UnitEventStrc* __fastcall SUNITEVENT_GetTimer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTimerQueueNo, int32_t a4, int32_t nGUID1)
{
    if (!pUnit)
    {
        return nullptr;
    }

    for (D2UnitEventStrc* pTimer = pUnit->pSrvTimerList; pTimer; pTimer = pTimer->pNext)
    {
        if (pTimer->nQueueNo == nTimerQueueNo && pTimer->unk0x08 == a4 && pTimer->nGUID1 == nGUID1)
        {
            return pTimer;
        }
    }

    return nullptr;
}

//D2Game.0x6FCC3790
int32_t __fastcall SUNITEVENT_EventFunc_Handler(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage)
{
    if (!pAttacker)
    {
        return 0;
    }

    int32_t nResult = 0;
    D2UnitEventStrc* pTimer = pAttacker->pSrvTimerList;
    while (pTimer)
    {
        D2UnitEventStrc* pNext = pTimer->pNext;
        if (pTimer->nTimerType == nEvent)
        {
            pTimer->nTimerFlags |= 1u;
            nResult = pTimer->pCallback(pGame, nEvent, pAttacker, pUnit, pDamage, pTimer->nGUID1, pTimer->nGUID2);
            pTimer->nTimerFlags &= (uint16_t)~1;

            if (pTimer->nTimerFlags & 2 || !pTimer->nQueueNo)
            {
                if (pTimer->pPrevious)
                {
                    pTimer->pPrevious->pNext = pNext;
                }
                else
                {
                    pAttacker->pSrvTimerList = pNext;
                }

                if (pTimer->pNext)
                {
                    pTimer->pNext->pPrevious = pTimer->pPrevious;
                }

                D2_FREE_POOL(pGame->pMemoryPool, pTimer);
            }
        }
        pTimer = pNext;
    }

    return nResult;
}
