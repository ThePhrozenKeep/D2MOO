#include "GAME/Task.h"

#include "GAME/Clients.h"
#include "GAME/Game.h"
#include "GAME/SCmd.h"


BOOL gbHasHighResolutionClock;
LARGE_INTEGER gPerformanceFrequency;
CRITICAL_SECTION_WRAPPER gTaskSlotsCriticalSection;


int32_t gnNumberOfTasksCreated;
// D2Game.0x6FD45858
int32_t gnGamesCount;
// D2Game.0x6FD45888
CRITICAL_SECTION_WRAPPER gtTaskQueuesCriticalSections[TASKQSLOTS];
// D2Game.0x6FD45B88
D2TaskStrc* gtTaskSlots[TASKQSLOTS];
// D2Game.0x6FD45868
D2LinkStrc gpTaskQueueLink;


//D2Game.0x6FC404E0 (#10039)
void __stdcall TASK_InitializeClock()
{
    gbHasHighResolutionClock = QueryPerformanceFrequency(&gPerformanceFrequency);
}

static int32_t TASK_GetClockTime()
{
    if (gbHasHighResolutionClock)
    {
        LARGE_INTEGER nQPC;
        QueryPerformanceCounter(&nQPC);
        return 1000 * nQPC.QuadPart / gPerformanceFrequency.QuadPart; // Note: truncated to 32bits
    }
    else
    {
        return GetTickCount();
    }
}


//D2Game.0x6FC40500 (#10040)
void __stdcall TASK_FreeAllQueueSlots()
{
    EnterCriticalSection(&gTaskSlotsCriticalSection.cs);
    
    for (int32_t i = 0; i < TASKQSLOTS; ++i)
    {
        EnterCriticalSection(&gtTaskQueuesCriticalSections[i].cs);

        if (D2TaskStrc* pTask = gtTaskSlots[i])
        {
            TASK_LinkList_Remove(&pTask->pTaskQueueLink);
            D2_FREE_POOL(nullptr, gtTaskSlots[i]);
            gtTaskSlots[i] = nullptr;
        }

        LeaveCriticalSection(&gtTaskQueuesCriticalSections[i].cs);
    }

    gnNumberOfTasksCreated = 0;
    gnGamesCount = 0;
    LeaveCriticalSection(&gTaskSlotsCriticalSection.cs);
}

//D2Game.0x6FC405B0 (#10041) 
int __cdecl D2Game_10041_TASK_Create()
{
    UNIMPLEMENTED();
    EnterCriticalSection(&gTaskSlotsCriticalSection.cs);
    if (!gpTaskQueueLink.pPrev)
    {
        gpTaskQueueLink.pPrev = &gpTaskQueueLink;
        gpTaskQueueLink.pNext = &gpTaskQueueLink;
    }
    int nTaskSlotIndex = gnNumberOfTasksCreated++ & 0x1F;
    CRITICAL_SECTION* pTaskQueueCriticalSection = &gtTaskQueuesCriticalSections[nTaskSlotIndex].cs;
    EnterCriticalSection(pTaskQueueCriticalSection);
    if (!gtTaskSlots[nTaskSlotIndex])
    {
        D2TaskStrc* ptTask = D2_ALLOC_STRC_POOL(nullptr, D2TaskStrc);
        D2LinkStrc* pTaskBalanceLink = &ptTask->pTaskBalanceLink;
        ptTask->nTaskQ = nTaskSlotIndex;
        ptTask->nType = 0;
        ptTask->pTaskBalanceLink.pPrev = &ptTask->pTaskBalanceLink;
        ptTask->pTaskBalanceLink.pNext = &ptTask->pTaskBalanceLink;
        int v6 = gnNumberOfTasksCreated;
        uint32_t v4 = gnGamesCount / (uint32_t)gnNumberOfTasksCreated;
        uint32_t v5 = gnGamesCount % (uint32_t)gnNumberOfTasksCreated;
        gtTaskSlots[nTaskSlotIndex] = ptTask;
        int v7 = v6 - 1;
        if (v6 - 1 >= (unsigned int)TASKQSLOTS)
            v7 = 32;
        int v21 = v7 - v5;
        for (D2LinkStrc* pLink = gpTaskQueueLink.pNext; pLink != &gpTaskQueueLink; pLink = pLink->pNext)
        {
            bool v9 = (int)--v21 < 0;
#define D2_PAIR64(high, low) ((uint64_t(uint32_t(high)) << 32) |(uint32_t(low)))
            int v10 = (D2_PAIR64(CONTAINING_RECORD(pLink, D2TaskStrc, pTaskQueueLink)->nType, v4 + !v9) - 1) >> 32;
            EnterCriticalSection(&gtTaskQueuesCriticalSections[CONTAINING_RECORD( pLink, D2TaskStrc, pTaskQueueLink)->nTaskQ].cs);
            D2LinkStrc* pTaskBalanceIterator = pLink[-1].pPrev;
            D2LinkStrc* pTaskBalancePrevLink = pLink - 1;
            D2LinkStrc* i = pTaskBalanceIterator;
            for (; v10; pTaskBalanceIterator = pTaskBalanceIterator->pNext)
            {
                --v10;
                if (pTaskBalanceIterator == pTaskBalancePrevLink)
                    break;
            }
            D2LinkStrc* pTaskBalanceIteratorpNext = pTaskBalanceIterator->pNext;
            pTaskBalanceIterator->pNext = pTaskBalancePrevLink;
            pTaskBalancePrevLink->pPrev = pTaskBalanceIterator;
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[CONTAINING_RECORD( pLink, D2TaskStrc, pTaskQueueLink)->nTaskQ].cs);
            if (pTaskBalanceIteratorpNext != pTaskBalancePrevLink)
            {
                D2LinkStrc* v14 = i;
                i->pNext = 0;
                D2LinkStrc* nPreviousTypeTaskLink;
                for (nPreviousTypeTaskLink = pTaskBalanceLink->pPrev;
                    nPreviousTypeTaskLink != pTaskBalanceLink;
                    nPreviousTypeTaskLink = nPreviousTypeTaskLink->pPrev)
                {
                    if ((char*)pTaskBalanceIteratorpNext[-2].pNext - (char*)nPreviousTypeTaskLink[-2].pNext >= 0)// CONTAINING_RECORD(pLink,D2TaskStrc,pTaskBalanceLink)->nTaskQ
                        break;
                }
                if (nPreviousTypeTaskLink->pNext == pTaskBalanceLink)
                {
                LABEL_22:
                    pTaskBalanceIteratorpNext->pPrev = nPreviousTypeTaskLink;
                    v14->pNext = pTaskBalanceLink;
                    nPreviousTypeTaskLink->pNext = pTaskBalanceIteratorpNext;
                    pTaskBalanceLink->pPrev = v14;
                }
                else
                {
                    while (1)
                    {
                        D2LinkStrc* v16 = pTaskBalanceIteratorpNext->pNext;
                        pTaskBalanceIteratorpNext->pPrev = nPreviousTypeTaskLink;
                        D2LinkStrc* v17 = nPreviousTypeTaskLink->pNext;
                        pTaskBalanceIteratorpNext->pNext = v17;
                        v17->pPrev = pTaskBalanceIteratorpNext;
                        nPreviousTypeTaskLink->pNext = pTaskBalanceIteratorpNext;
                        if (!v16)
                            break;
                        nPreviousTypeTaskLink = pTaskBalanceIteratorpNext;
                        pTaskBalanceIteratorpNext = v16;
                        D2LinkStrc* v18 = nPreviousTypeTaskLink->pNext;
                        if (v18 != pTaskBalanceLink)
                        {
                            D2LinkStrc* v19 = v16[-2u].pNext; // NOLINT TODO proper list
                            do
                            {
                                if ((char*)v19 - (char*)v18[-2].pNext <= 0) // NOLINT TODO proper list
                                    break;
                                nPreviousTypeTaskLink = v18;
                                v18 = v18->pNext;
                            } while (v18 != pTaskBalanceLink);
                        }
                        if (nPreviousTypeTaskLink->pNext == pTaskBalanceLink)
                        {
                            v14 = i;
                            goto LABEL_22;
                        }
                    }
                }
            }
        }
        TASK_LinkList_PushFront(&gpTaskQueueLink, &ptTask->pTaskQueueLink);
    }
    LeaveCriticalSection(pTaskQueueCriticalSection);
    LeaveCriticalSection(&gTaskSlotsCriticalSection.cs);
    return nTaskSlotIndex;
}

//D2Game.0x6FC407A0 (#10042) --------------------------------------------------------
BOOL __fastcall D2Game_10042(D2TaskStrc* pTask, int nTaskType, D2LinkStrc* pPrevTaskBalanceLink)
{
    BOOL nResult = FALSE;
    
    pTask->nTaskQ = TASK_GetClockTime();
    pTask->nType = nTaskType;
    pTask->pTaskBalanceLink.pPrev = pPrevTaskBalanceLink;
    EnterCriticalSection(&gTaskSlotsCriticalSection.cs);
    if (gpTaskQueueLink.pPrev)
    {
        D2LinkStrc* pTaskQueueHeadLink = gpTaskQueueLink.pNext;
        if (gpTaskQueueLink.pNext != &gpTaskQueueLink && gpTaskQueueLink.pNext)
        {
            D2TaskStrc* ptTask = CONTAINING_RECORD(gpTaskQueueLink.pNext, D2TaskStrc, pTaskQueueLink);
            EnterCriticalSection(&gtTaskQueuesCriticalSections[ptTask->nTaskQ].cs);
            D2TaskStrc* ptTaskQueueHead = CONTAINING_RECORD(pTaskQueueHeadLink, D2TaskStrc, pTaskQueueLink);

            TASK_LinkList_PushFront(&ptTaskQueueHead->pTaskBalanceLink, (D2LinkStrc*)&pTask->pTaskBalanceLink.pNext);

            LeaveCriticalSection(&gtTaskQueuesCriticalSections[ptTask->nTaskQ].cs);
            ++ptTask->nType;
            ++gnGamesCount;
            TASK_LinkList_Remove(pTaskQueueHeadLink);
            TASK_LinkList_Insert(gpTaskQueueLink.pPrev, pTaskQueueHeadLink);

            nResult = TRUE;
        }
    }
    else
    {
        gpTaskQueueLink.pPrev = &gpTaskQueueLink;
        gpTaskQueueLink.pNext = &gpTaskQueueLink;
    }
    LeaveCriticalSection(&gTaskSlotsCriticalSection.cs);
    return nResult;
}

//D2Game.0x6FC40930 (#10043)
int32_t __fastcall D2Game_10043(int8_t a1, int32_t* pOutBalanceTaskType)
{
    const int8_t nIndex = a1 & 31;
    int32_t nResult = 100;

    *pOutBalanceTaskType = 0;

    EnterCriticalSection(&gtTaskQueuesCriticalSections[nIndex].cs);

    if (D2TaskStrc* pTask = gtTaskSlots[nIndex])
    {
        D2LinkStrc* pLink = pTask->pTaskBalanceLink.pNext;
        if (pLink != &pTask->pTaskBalanceLink && pLink)
        {
            const int64_t nTickCount = TASK_GetClockTime();
            nResult = CONTAINING_RECORD(pLink, D2TaskStrc, pTaskQueueLink)->nType - nTickCount;
            if (nResult <= 0)
            {
                TASK_LinkList_Remove(pLink);
                *pOutBalanceTaskType = CONTAINING_RECORD(pLink, D2TaskStrc, pTaskQueueLink)->nType;
            }
        }

        D2_ASSERT(pTask->nTaskQ < 32);

        LeaveCriticalSection(&gtTaskQueuesCriticalSections[pTask->nTaskQ].cs);
        return nResult;
    }

    LeaveCriticalSection(&gtTaskQueuesCriticalSections[nIndex].cs);
    return 100;
}

//D2Game.0x6FC40A40 (#10044)
int32_t __fastcall D2Game_10044(int8_t nTaskNumber)
{
    const int8_t nIndex = nTaskNumber & 31;
    int32_t nResult = 100;
    EnterCriticalSection(&gtTaskQueuesCriticalSections[nIndex].cs);
    
    if (D2TaskStrc* pTask = gtTaskSlots[nIndex])
    {
        D2LinkStrc* pLink = pTask->pTaskBalanceLink.pNext;
        if (pLink != &pTask->pTaskBalanceLink && pLink)
        {
            const int64_t nTickCount = TASK_GetClockTime();
            nResult = CONTAINING_RECORD(pLink, D2TaskStrc, pTaskQueueLink)->nType - nTickCount;
        }

        D2_ASSERT(pTask->nTaskQ < 32);

        LeaveCriticalSection(&gtTaskQueuesCriticalSections[pTask->nTaskQ].cs);
        return nResult;
    }

    LeaveCriticalSection(&gtTaskQueuesCriticalSections[nIndex].cs);
    return nResult;
}

static D2TaskStrc* TASK_GetTaskFromTaskQueueLink(D2LinkStrc* pInTaskQueueLink)
{
    return CONTAINING_RECORD(pInTaskQueueLink, D2TaskStrc, pTaskQueueLink);
}
static D2TaskStrc* TASK_GetTaskFromTaskType(int32_t* pTaskType)
{
    return CONTAINING_RECORD(pTaskType, D2TaskStrc, nType);
}

static bool TASK_LinkList_IsEmpty(D2LinkStrc* pListLink)
{
    return pListLink->pNext != pListLink;
}

static D2LinkStrc* TASK_GetLastTaskFromListHead(D2LinkStrc* pListHead)
{
    return TASK_LinkList_IsEmpty(pListHead) ? pListHead->pPrev : nullptr;
}

static void TASK_CloseGame(uint32_t nGameHashKey, char nTaskNumber)
{
    GAME_CloseGame(nGameHashKey);
    EnterCriticalSection(&gTaskSlotsCriticalSection.cs);

    if (D2TaskStrc* pTaskSlot = gtTaskSlots[nTaskNumber & 0x1F])
    {
        D2LinkStrc* ptTaskQTailLink = TASK_GetLastTaskFromListHead(&gpTaskQueueLink);
        D2_ASSERT(ptTaskQTailLink);

        const int32_t nTaskSlotType = pTaskSlot->nType;
        D2TaskStrc* ptTaskQTail = TASK_GetTaskFromTaskQueueLink(gpTaskQueueLink.pPrev);

        D2LinkStrc* pNewTaskLink = nullptr;
        if (TASK_GetTaskFromTaskQueueLink(gpTaskQueueLink.pNext)->nType == (nTaskSlotType + 1))
        {
            EnterCriticalSection(&gtTaskQueuesCriticalSections[ptTaskQTail->nTaskQ].cs);

            D2LinkStrc* ptTaskBalanceLink = TASK_GetLastTaskFromListHead(&ptTaskQTail->pTaskBalanceLink);
            D2_ASSERT(ptTaskBalanceLink);

            TASK_LinkList_Remove(ptTaskBalanceLink);
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[ptTaskQTail->nTaskQ].cs);
            EnterCriticalSection(&gtTaskQueuesCriticalSections[pTaskSlot->nTaskQ].cs);
            TASK_QueueIncrement(pTaskSlot, &(TASK_GetTaskFromTaskQueueLink(ptTaskBalanceLink)->nType), 0);
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[pTaskSlot->nTaskQ].cs);
            --ptTaskQTail->nType;
            TASK_LinkList_Remove(ptTaskQTailLink);
            pNewTaskLink = ptTaskQTailLink;
        }
        else
        {
            pTaskSlot->nType = nTaskSlotType - 1;
            pNewTaskLink = &pTaskSlot->pTaskQueueLink;
            TASK_LinkList_Remove(pNewTaskLink);
        }
        TASK_LinkList_PushFront(&gpTaskQueueLink, pNewTaskLink);
    }
    --gnGamesCount;
    LeaveCriticalSection(&gTaskSlotsCriticalSection.cs);
}

//D2Game.0x6FC40B30 (#10045) --------------------------------------------------------
void __fastcall TASK_ProcessGame(char nTaskNumber, D2TaskStrc* ptTask)
{
    D2_ASSERT(ptTask->nType == TASK_PROCESSGAME);

    const uint32_t nGameHashKey = (uint32_t)ptTask->pTaskBalanceLink.pPrev;
    if (D2GameStrc* pGame = GAME_LockGame(nGameHashKey))
    {
        GAME_UpdateProgress(pGame);
        sub_6FC39270(pGame, 0);
        const int32_t v7 = FOG_10055_GetSyncTime();
        const int32_t nSyncTimer = pGame->nSyncTimer;
        if (nSyncTimer == 1 || v7 - nSyncTimer > 1800)// I/O timeout
        {
            for (D2ClientStrc* pCurrentClient = pGame->pClientList; pCurrentClient != nullptr; pCurrentClient = pCurrentClient->pNext)
            {
                sub_6FC3C690(pCurrentClient->dwClientId);
                sub_6FC3C6B0(pCurrentClient->dwClientId);
                CLIENTS_RemoveClientFromGame(pGame, pCurrentClient->dwClientId, 0);
            }
            GAME_LogMessage(6, "[SERVER]  Deleting game from sSrvTaskProcessGame(), I/O timeout");
            GAME_LeaveGamesCriticalSection(pGame);
            TASK_CloseGame(nGameHashKey, nTaskNumber);
            return;
        }
        int32_t nTick = GetTickCount();
        if (pGame->nClients)
        {
            pGame->nTickCountSinceNoClients = 0;
        }
        else
        {
            if (!pGame->nTickCountSinceNoClients)
                pGame->nTickCountSinceNoClients = nTick;
            if (nTick - pGame->nTickCountSinceNoClients > 300000
                || pGame->dwGameFrame > 1500 && pGame->dwGameFrame < 7500)
            {
                GAME_LogMessage(6, "[SERVER]  Deleting game from sSrvTaskProcessGame(), empty game");
                GAME_LeaveGamesCriticalSection(pGame);
                TASK_CloseGame(nGameHashKey, nTaskNumber);
                return;
            }
        }
        
        GAME_LeaveGamesCriticalSection(pGame);
        const int32_t nTaskSlotIdx = nTaskNumber & 0x1F;
        EnterCriticalSection(&gtTaskQueuesCriticalSections[nTaskSlotIdx].cs);

        if (D2TaskStrc* ptTaskQ = gtTaskSlots[nTaskSlotIdx])
        {
            ptTask->nTaskQ += 40;
            D2LinkStrc* pTaskBalancePrevLink;
            for (pTaskBalancePrevLink = ptTaskQ->pTaskBalanceLink.pPrev; pTaskBalancePrevLink != &ptTaskQ->pTaskBalanceLink; pTaskBalancePrevLink = pTaskBalancePrevLink->pPrev)
            {
                if (TASK_GetTaskFromTaskQueueLink(pTaskBalancePrevLink)->nType <= ptTask->nTaskQ)
                    break;
            }
            // Something weird is going on here
            TASK_LinkList_Insert(pTaskBalancePrevLink, (D2LinkStrc*)&ptTask->pTaskBalanceLink.pNext);
            
            D2_ASSERT(ptTaskQ->nTaskQ < TASKQSLOTS);
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[ptTaskQ->nTaskQ].cs);
        }
        else
        {
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[nTaskSlotIdx].cs);
        }
    }
}

//D2Game.0x6FC40E40
void __fastcall TASK_QueueIncrement(D2TaskStrc* ptTaskQueue, int32_t* pTaskType, int nTaskTypeIncrement)
{
    D2LinkStrc* ptTaskBalanceLink = &ptTaskQueue->pTaskBalanceLink;
    *pTaskType += nTaskTypeIncrement;
    D2LinkStrc* ptPrev;
    for (ptPrev = ptTaskBalanceLink->pPrev; ptPrev != ptTaskBalanceLink; ptPrev = ptPrev->pPrev)
    {
        if (TASK_GetTaskFromTaskQueueLink(ptPrev)->nType <= *pTaskType)
            break;
    }
    
    D2TaskStrc* pTask = TASK_GetTaskFromTaskType(pTaskType);
    TASK_LinkList_Insert(ptPrev, &pTask->pTaskQueueLink);
}


//D2Game.0x6FC40ED0
void __fastcall TASK_LinkList_Insert(D2LinkStrc* pPrev, D2LinkStrc* pLink)
{
    D2_ASSERT(pPrev);
    D2_ASSERT(pLink);

    pLink->pPrev = pPrev;
    pLink->pNext = pPrev->pNext;
    pPrev->pNext->pPrev = pLink;
    pPrev->pNext = pLink;
}

// Exactly the same code as TASK_LinkList_Insert, except one is supposed to provide the list head instead of the previous element.
//D2Game.0x6FC40F20
void __fastcall TASK_LinkList_PushFront(D2LinkStrc* pList, D2LinkStrc* pLink)
{
    D2_ASSERT(pList);
    D2_ASSERT(pLink);

    pLink->pPrev = pList;
    pLink->pNext = pList->pNext;
    pList->pNext->pPrev = pLink;
    pList->pNext = pLink;
}

//D2Game.0x6FC40F70
void __fastcall TASK_LinkList_Remove(D2LinkStrc* pLink)
{
    D2_ASSERT(pLink);

    pLink->pPrev->pNext = pLink->pNext;
    pLink->pNext->pPrev = pLink->pPrev;
}



void __stdcall D2Game_10034_Return(int a1)
{
}

void __stdcall D2Game_10060_Return()
{
}

void __stdcall D2Game_10061_Return()
{
}
