#include "GAME/Event.h"

#include <D2States.h>

#include "ITEMS/ItemMode.h"
#include "GAME/Game.h"
#include "MISSILES/MissMode.h"
#include "MONSTER/MonsterMode.h"
#include "OBJECTS/ObjMode.h"
#include "PLAYER/PlrModes.h"
#include "UNIT/SUnit.h"


__forceinline int32_t EVENT_MapUnitTypeToIndex(int32_t nUnitType)
{
    static const int32_t dword_6FD27D6C[] =
    {
        0, 1, 3, 2, 4, -1, 0
    };

    return dword_6FD27D6C[nUnitType];
}


//D2Game.0x6FC34840
void __fastcall D2GAME_EVENTS_Delete_6FC34840(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t nEventCustomId)
{
    D2EventTimerStrc* pEventTimer = SUNIT_GetTimerFromUnit(pUnit);
    while (pEventTimer)
    {
        D2EventTimerStrc* pNextTimer = pEventTimer->pNext;
        if (pEventTimer->nEventType == nEventType && (!nEventCustomId || pEventTimer->dwEventCustomId == nEventCustomId))
        {
            sub_6FC34890(pGame, pEventTimer);
        }

        pEventTimer = pNextTimer;
    }
}

//D2Game.0x6FC34890
void __fastcall sub_6FC34890(D2GameStrc* pGame, D2EventTimerStrc* pTimer)
{
    if (pTimer->nFlags & EVENTFLAG_0x1)
    {
        pTimer->nFlags |= EVENTFLAG_0x8;
        return;
    }

    if (pTimer == pGame->pTimerQueue->unk0xA18)
    {
        pGame->pTimerQueue->unk0xA18 = pTimer->pNextFreeEventTimer;
    }

    if (pTimer->unk0x20)
    {
        pTimer->unk0x20->pNextFreeEventTimer = pTimer->pNextFreeEventTimer;
    }
    else
    {
        D2EventTimerStrc** ppEventTimer = sub_6FC353D0(pGame, pTimer->nUnitType, pTimer->nExpireFrame);
        *ppEventTimer = pTimer->pNextFreeEventTimer;
    }

    if (pTimer->pNextFreeEventTimer)
    {
        pTimer->pNextFreeEventTimer->unk0x20 = pTimer->unk0x20;
    }
    else if (pTimer->nExpireFrame != -1)
    {
        D2EventTimerStrc** ppEventTimer = &pGame->pTimerQueue->unk0x04[5][pTimer->nExpireFrame % 64 + (EVENT_MapUnitTypeToIndex(pTimer->nUnitType) << 6)];
        if (ppEventTimer && *ppEventTimer == pTimer)
        {
            *ppEventTimer = pTimer->unk0x20;
        }
    }

    if (pTimer->pUnit)
    {
        if (pTimer->pNext)
        {
            pTimer->pNext->pPrevious = pTimer->pPrevious;
        }

        if (pTimer->pPrevious)
        {
            pTimer->pPrevious->pNext = pTimer->pNext;
        }
        else
        {
            SUNIT_SetTimerOnUnit(pTimer->pUnit, pTimer->pNext);
        }
    }

    D2EventTimerSlabListStrc* pEventTimerArray = pGame->pTimerQueue->pSlabListHead;
    pTimer->unk0x20 = nullptr;
    pTimer->pNextFreeEventTimer = pEventTimerArray->pFreeEventTimerListHead;
    pEventTimerArray->pFreeEventTimerListHead = pTimer;
    pTimer->nFlags = EVENTFLAG_0x2;
}

//D2Game.0x6FC349B0
void __fastcall sub_6FC349B0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEvent, EventTimerCallback pCallback)
{
    D2EventTimerStrc* pEventTimer = SUNIT_GetTimerFromUnit(pUnit);
    while (pEventTimer)
    {
        D2EventTimerStrc* pNext = pEventTimer->pNext;
        if (pEventTimer->nEventType == nEvent)
        {
            if (pCallback == pEventTimer->pCallback)
            {
                sub_6FC34890(pGame, pEventTimer);
            }
        }
        pEventTimer = pNext;
    }
}

//D2Game.0x6FC349F0
void __fastcall sub_6FC349F0(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    D2EventTimerStrc* pEventTimer = SUNIT_GetTimerFromUnit(pUnit);
    while (pEventTimer)
    {
        D2EventTimerStrc* pNext = pEventTimer->pNext;
        sub_6FC34890(pGame, pEventTimer);
        pEventTimer->pUnit = nullptr;
        pEventTimer = pNext;
    }

    SUNIT_SetTimerOnUnit(pUnit, nullptr);
}

//D2Game.0x6FC34A30
void __fastcall D2GAME_DeleteTimersOnUnit_6FC34A30(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
    if (SUNIT_GetTimerFromUnit(pUnit))
    {
        sub_6FC349F0(pGame, pUnit);
    }

    SUNIT_SetTimerOnUnit(pUnit, nullptr);
}

//1.10f: D2Game.0x6FC34A80
//1.13c: D2Game.6FCAE4D0
void __fastcall EVENT_FreeEventQueue(D2GameStrc* pGame)
{
    if (!pGame->pTimerQueue)
    {
        return;
    }

    D2EventTimerSlabListStrc* pTimerArray = pGame->pTimerQueue->pSlabListHead;
    while (pTimerArray)
    {
        D2EventTimerSlabListStrc* pNext = pTimerArray->pNextSlab;
        D2_FREE_POOL(pGame->pMemoryPool, pTimerArray);
        pTimerArray = pNext;
    }

    D2_FREE_POOL(pGame->pMemoryPool, pGame->pTimerQueue);
    pGame->pTimerQueue = nullptr;
}

//D2Game.0x6FC34AE0
void __fastcall EVENT_AllocEventQueue(D2GameStrc* pGame)
{
    EVENT_FreeEventQueue(pGame);

    D2EventTimerQueueStrc* pTimerQueue = D2_CALLOC_STRC_POOL(pGame->pMemoryPool, D2EventTimerQueueStrc);

    pTimerQueue->pSlabListHead = EVENT_AllocTimerSlab(pGame);
    pGame->pTimerQueue = pTimerQueue;
}

//D2Game.0x6FC34BD0
void __fastcall EVENT_IterateEvents(D2GameStrc* pGame)
{
    D2EventTimerQueueStrc* pTimerQueue = pGame->pTimerQueue;

    pTimerQueue->nArrayIndex = pGame->dwGameFrame % 64;

    const int nMissileEventIdx = EVENT_MapUnitTypeToIndex(UNIT_MISSILE);
    EVENT_ExecuteMissileEvents(pGame, pTimerQueue, pTimerQueue->unk0xA04[nMissileEventIdx], 0);
    EVENT_ExecuteMissileEvents(pGame, pTimerQueue, pTimerQueue->unk0x04 [nMissileEventIdx][pTimerQueue->nArrayIndex], 1);

    const int nPlayerEventIdx = EVENT_MapUnitTypeToIndex(UNIT_PLAYER);
    EVENT_ExecutePlayerEvents(pGame, pTimerQueue, pTimerQueue->unk0xA04[nPlayerEventIdx], 0);
    EVENT_ExecutePlayerEvents(pGame, pTimerQueue, pTimerQueue->unk0x04 [nPlayerEventIdx][pTimerQueue->nArrayIndex], 1);

    const int nMonsterEventIdx = EVENT_MapUnitTypeToIndex(UNIT_MONSTER);
    EVENT_ExecuteMonsterEvents(pGame, pTimerQueue, pTimerQueue->unk0xA04[nMonsterEventIdx], 0);
    EVENT_ExecuteMonsterEvents(pGame, pTimerQueue, pTimerQueue->unk0x04 [nMonsterEventIdx][pTimerQueue->nArrayIndex], 1);

    const int nObjectEventIdx = EVENT_MapUnitTypeToIndex(UNIT_OBJECT);
    EVENT_ExecuteObjectEvents(pGame, pTimerQueue, pTimerQueue->unk0xA04[nObjectEventIdx], 0);
    EVENT_ExecuteObjectEvents(pGame, pTimerQueue, pTimerQueue->unk0x04 [nObjectEventIdx][pTimerQueue->nArrayIndex], 1);

    const int nItemEventIdx = EVENT_MapUnitTypeToIndex(UNIT_ITEM);
    EVENT_ExecuteItemEvents(pGame, pTimerQueue, pTimerQueue->unk0xA04[nItemEventIdx], 0);
    EVENT_ExecuteItemEvents(pGame, pTimerQueue, pTimerQueue->unk0x04 [nItemEventIdx][pTimerQueue->nArrayIndex], 1);
}

// Only differs from EventTimerCallback by return type
using ExecuteEventCallback = void(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2C_EventTypes, int32_t, int32_t);
// Helper function
static void __fastcall EVENT_ExecuteEventsImpl(ExecuteEventCallback pDefaultCallback, D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4)
{
    pTimerQueue->unk0xA18 = nullptr;

    D2EventTimerStrc* pTimer = pEventTimer;
    if (a4)
    {
        while (pTimer)
        {
            pTimerQueue->unk0xA18 = pTimer->pNextFreeEventTimer;
            if (pTimer->nExpireFrame == pGame->dwGameFrame)
            {
                pTimer->nFlags |= EVENTFLAG_0x1;
                if (pTimer->pCallback)
                {
                    pTimer->pCallback(pGame, pTimer->pUnit, pTimer->nEventType, pTimer->dwEventCustomId, pTimer->dwEventCustomParam);
                }
                else
                {
                    pDefaultCallback(pGame, pTimer->pUnit, pTimer->nEventType, pTimer->dwEventCustomId, pTimer->dwEventCustomParam);
                }
                pTimer->nFlags &= ~EVENTFLAG_0x1;

                sub_6FC34890(pGame, pTimer);
            }

            pTimer = pTimerQueue->unk0xA18;
        }
    }
    else
    {
        while (pTimer)
        {
            pTimerQueue->unk0xA18 = pTimer->pNextFreeEventTimer;

            pTimer->nFlags |= EVENTFLAG_0x1;
            if (pTimer->pCallback)
            {
                pTimer->pCallback(pGame, pTimer->pUnit, pTimer->nEventType, pTimer->dwEventCustomId, pTimer->dwEventCustomParam);
            }
            else
            {
                pDefaultCallback(pGame, pTimer->pUnit, pTimer->nEventType, pTimer->dwEventCustomId, pTimer->dwEventCustomParam);
            }
            pTimer->nFlags &= ~EVENTFLAG_0x1;

            if (pTimer->nFlags & EVENTFLAG_0x8)
            {
                sub_6FC34890(pGame, pTimer);
            }

            pTimer = pTimerQueue->unk0xA18;
        }
    }
}

//D2Game.0x6FC34CC0
void __fastcall EVENT_ExecutePlayerEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4)
{
    EVENT_ExecuteEventsImpl(D2GAME_EVENTS_Callback_6FC81BD0, pGame, pTimerQueue, pEventTimer, a4);
}

//D2Game.0x6FC34DB0
void __fastcall EVENT_ExecuteMonsterEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4)
{
    EVENT_ExecuteEventsImpl(MONSTERMODE_EventHandler, pGame, pTimerQueue, pEventTimer, a4);
}

//D2Game.0x6FC34EA0
void __fastcall EVENT_ExecuteObjectEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4)
{
    EVENT_ExecuteEventsImpl(
        [](D2GameStrc* pGame, D2UnitStrc* pObject, D2C_EventTypes nEventType, int32_t, int32_t) {
            D2GAME_OBJMODE_InvokeEventFunction_6FC75250(pGame, pObject, nEventType);
        }, pGame, pTimerQueue, pEventTimer, a4 
    );
}

//D2Game.0x6FC34F90
void __fastcall EVENT_ExecuteMissileEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4)
{
    EVENT_ExecuteEventsImpl(
        [](D2GameStrc* pGame, D2UnitStrc* pMissile, D2C_EventTypes nEventType, int32_t, int32_t) {
            MISSMODE_SrvDoHandler(pGame, pMissile, nEventType);
        }, pGame, pTimerQueue, pEventTimer, a4 
    );
}

//D2Game.0x6FC35080
void __fastcall EVENT_ExecuteItemEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4)
{
    EVENT_ExecuteEventsImpl(
        [](D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t, int32_t) {
            D2GAME_Items_EventsHandler_6FC4A460(pGame, pUnit, nEventType);
        }, pGame, pTimerQueue, pEventTimer, a4
    );
    
}

//D2Game.0x6FC35170
int32_t __fastcall EVENT_GetEventFrame(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEvent)
{
    int32_t nEventFrame = 0;
    D2EventTimerStrc* pEventTimer = SUNIT_GetTimerFromUnit(pUnit);
    while (pEventTimer)
    {
        D2EventTimerStrc* pNext = pEventTimer->pNext;
        if (pEventTimer->nEventType == nEvent)
        {
            const int32_t nFrame = pEventTimer->nExpireFrame;
            if (nFrame > 0 && (nFrame < nEventFrame || !nEventFrame))
            {
                nEventFrame = nFrame;
            }
        }
        pEventTimer = pNext;
    }

    return nEventFrame;
}

//D2Game.0x6FC351B0
void __fastcall EVENT_SetEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t nExpireFrame, int32_t dwEventCustomId, int32_t dwEventCustomParam)
{
    D2GAME_InitTimer_6FC351D0(pGame, pUnit, nEventType, nExpireFrame, nullptr, dwEventCustomId, dwEventCustomParam);
}

//D2Game.0x6FC351D0
void __fastcall D2GAME_InitTimer_6FC351D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t nExpireFrame, EventTimerCallback pfCallBack, int32_t dwEventCustomId, int32_t dwEventCustomParam)
{
    // TODO: v19
    if (nEventType >= 15)
    {
        return;
    }

    D2EventTimerStrc* pEventTimer = nullptr;
    D2EventTimerStrc** ppEventTimer = nullptr;
    if (nExpireFrame == -1)
    {
        pEventTimer = EVENT_AllocateUnitTimer(pGame, pUnit);
        pEventTimer->nFlags |= 4u;
        pEventTimer->nEventType = nEventType;
        pEventTimer->nExpireFrame = -1;
        pEventTimer->pCallback = nullptr;
        pEventTimer->dwEventCustomId = dwEventCustomId;
        pEventTimer->dwEventCustomParam = dwEventCustomParam;

        ppEventTimer = sub_6FC353D0(pGame, pUnit ? pUnit->dwUnitType : 6, -1);
        pEventTimer->unk0x20 = nullptr;
        pEventTimer->pNextFreeEventTimer = *ppEventTimer;
        if (*ppEventTimer)
        {
            (*ppEventTimer)->unk0x20 = pEventTimer;
        }

        *ppEventTimer = pEventTimer;
        return;
    }

    if (nExpireFrame <= pGame->dwGameFrame)
    {
        nExpireFrame = pGame->dwGameFrame + 1;
    }

    if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && nEventType == EVENTTYPE_AITHINK && STATES_CheckState(pUnit, STATE_UNINTERRUPTABLE))
    {
        sub_6FCBDE90(pUnit, 0);
    }

    pEventTimer = EVENT_AllocateUnitTimer(pGame, pUnit);
    pEventTimer->dwEventCustomId = dwEventCustomId;
    pEventTimer->nEventType = nEventType;
    pEventTimer->dwEventCustomParam = dwEventCustomParam;
    pEventTimer->nExpireFrame = nExpireFrame;
    pEventTimer->pCallback = pfCallBack;

    D2EventTimerStrc** v19 = &pGame->pTimerQueue->unk0x04[5][nExpireFrame % 64 + (EVENT_MapUnitTypeToIndex(pEventTimer->nUnitType) << 6)];

    pEventTimer->pNextFreeEventTimer = nullptr;
    pEventTimer->unk0x20 = *v19;
    if (*v19)
    {
        (*v19)->pNextFreeEventTimer = pEventTimer;
    }
    *v19 = pEventTimer;

    ppEventTimer = sub_6FC353D0(pGame, pEventTimer->nUnitType, pEventTimer->nExpireFrame);
    if (!*ppEventTimer)
    {
        *ppEventTimer = pEventTimer;
    }
}

//D2Game.0x6FC353D0
D2EventTimerStrc** __fastcall sub_6FC353D0(D2GameStrc* pGame, int32_t nUnitType, int32_t nExpireFrame)
{
    const int32_t nIndex = EVENT_MapUnitTypeToIndex(nUnitType);
    if (nExpireFrame == -1)
    {
        return &pGame->pTimerQueue->unk0xA04[nIndex];
    }

    return (D2EventTimerStrc**)((char*)pGame->pTimerQueue->unk0x04 + 4 * (nExpireFrame % 64 + (nIndex << 6)));
}

//1.10f: D2Game.0x6FC35410
//1.13c: D2Game.0x6FCAE420
D2EventTimerSlabListStrc* __fastcall EVENT_AllocTimerSlab(D2GameStrc* pGame)
{
    D2EventTimerSlabListStrc* pSlab = D2_ALLOC_STRC_POOL(pGame->pMemoryPool,D2EventTimerSlabListStrc);
    if (!pSlab)
    {
        return nullptr;
    }

    memset(pSlab, 0, sizeof(D2EventTimerSlabListStrc));

    pSlab->pFreeEventTimerListHead = &pSlab->tEventTimersStorage[0];
    const size_t nLastEventTimerIdx = ARRAY_SIZE(pSlab->tEventTimersStorage) - 1;
    for (int32_t i = 0; i < nLastEventTimerIdx; ++i)
    {
        pSlab->tEventTimersStorage[i].pNextFreeEventTimer = &pSlab->tEventTimersStorage[i + 1];
    }
    pSlab->tEventTimersStorage[nLastEventTimerIdx].pNextFreeEventTimer = nullptr;

    return pSlab;
}

// Helper function
static void EVENT_AssignTimerToUnit(D2GameStrc* pGame, D2UnitStrc* pUnit, D2EventTimerStrc* pTimer)
{
	pTimer->pUnit = pUnit;
	pTimer->pPrevious = nullptr;
	pTimer->pNext = SUNIT_GetTimerFromUnit(pUnit);
	if (pTimer->pNext)
	{
		pTimer->pNext->pPrevious = pTimer;
	}
	SUNIT_SetTimerOnUnit(pUnit, pTimer);

	if (pUnit)
	{
		pTimer->nUnitType = pUnit->dwUnitType;
		pTimer->nUnitGUID = pUnit->dwUnitId;
	}
	else
	{
		pTimer->nUnitType = UNIT_TYPES_COUNT;
		pTimer->nUnitGUID = D2UnitInvalidGUID;
	}

}

//1.10f: Inlined
//1.13c: D2Game.0x6FCAE540
D2EventTimerStrc* EVENT_AllocateTimer(D2GameStrc* pGame)
{
	D2EventTimerSlabListStrc* pEventTimerArray = pGame->pTimerQueue->pSlabListHead;
	D2_ASSERT(pEventTimerArray); // Event queue always has at least one slab, that will always be the head
	while (pEventTimerArray)
	{
		if (pEventTimerArray->pFreeEventTimerListHead)
		{
			break;
		}

		pEventTimerArray = pEventTimerArray->pNextSlab;
	}

	if (!pEventTimerArray)
	{
		pEventTimerArray = EVENT_AllocTimerSlab(pGame);
		pEventTimerArray->pNextSlab = pGame->pTimerQueue->pSlabListHead->pNextSlab;
		pGame->pTimerQueue->pSlabListHead->pNextSlab = pEventTimerArray;
	}

	D2EventTimerStrc* pNewTimer = pEventTimerArray->pFreeEventTimerListHead;
	pEventTimerArray->pFreeEventTimerListHead = pNewTimer->pNextFreeEventTimer;

	memset(pNewTimer, 0, sizeof(D2EventTimerStrc));
	return pNewTimer;
}


//1.10f: D2Game.0x6FC35460
//1.13c: D2Game.0x6FCAE680
D2EventTimerStrc* __fastcall EVENT_AllocateUnitTimer(D2GameStrc* pGame, D2UnitStrc* pUnit)
{
	D2EventTimerStrc* pNewTimer = EVENT_AllocateTimer(pGame);
	EVENT_AssignTimerToUnit(pGame, pUnit, pNewTimer);

    pNewTimer->nFlags &= (~EVENTFLAG_0x2);
    return pNewTimer;
}

//D2Game.0x6FC35570
void __fastcall sub_6FC35570(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t dwEventCustomId, int32_t dwEventCustomParam)
{
    if (nEventType >= EVENTTYPE_COUNT)
    {
        return;
    }
	D2EventTimerStrc* pNewTimer = EVENT_AllocateUnitTimer(pGame, pUnit);
    pNewTimer->nEventType = nEventType;
    pNewTimer->nExpireFrame = -1;
    pNewTimer->nFlags |= 4;
    pNewTimer->dwEventCustomId = dwEventCustomId;
    pNewTimer->dwEventCustomParam = dwEventCustomParam;
    pNewTimer->pCallback = nullptr;

    D2EventTimerStrc** ppEventTimer = sub_6FC353D0(pGame, pUnit ? pUnit->dwUnitType : UNIT_TYPES_COUNT, -1);
    pNewTimer->unk0x20 = nullptr;
    pNewTimer->pNextFreeEventTimer = *ppEventTimer;
    if (pNewTimer->pNextFreeEventTimer)
    {
        pNewTimer->pNextFreeEventTimer->unk0x20 = pNewTimer;
    }

    *ppEventTimer = pNewTimer;
}

//D2Game.0x6FC351D0
void __fastcall j_D2GAME_InitTimer_6FC351D0(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t nExpireFrame, EventTimerCallback pfCallBack, int32_t nSkillId, int32_t nSkillLevel)
{
    D2GAME_InitTimer_6FC351D0(pGame, pUnit, nEventType, nExpireFrame, pfCallBack, nSkillId, nSkillLevel);
}
