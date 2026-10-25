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
D2TaskQueueStrc* gtTaskSlots[TASKQSLOTS];
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

        if (D2TaskQueueStrc* pTask = gtTaskSlots[i])
        {
            TASK_LinkList_Remove(&pTask->tQueueLink);
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
int __cdecl TASK_CreateQueue()
{
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
        D2TaskQueueStrc* ptTask = D2_ALLOC_STRC_POOL(nullptr, D2TaskQueueStrc);
        D2LinkStrc* pTaskBalanceLink = &ptTask->tGameTasks;
        ptTask->nQueueIndex = nTaskSlotIndex;
        ptTask->nGameCount = 0;
        ptTask->tGameTasks.pPrev = &ptTask->tGameTasks;
        ptTask->tGameTasks.pNext = &ptTask->tGameTasks;
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
            int v10 = (D2_PAIR64(CONTAINING_RECORD(pLink, D2TaskQueueStrc, tQueueLink)->nGameCount, v4 + !v9) - 1) >> 32;
            EnterCriticalSection(&gtTaskQueuesCriticalSections[CONTAINING_RECORD( pLink, D2TaskQueueStrc, tQueueLink)->nQueueIndex].cs);
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
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[CONTAINING_RECORD( pLink, D2TaskQueueStrc, tQueueLink)->nQueueIndex].cs);
            if (pTaskBalanceIteratorpNext != pTaskBalancePrevLink)
            {
                D2LinkStrc* v14 = i;
                i->pNext = 0;
                D2LinkStrc* nPreviousTypeTaskLink;
                for (nPreviousTypeTaskLink = pTaskBalanceLink->pPrev;
                    nPreviousTypeTaskLink != pTaskBalanceLink;
                    nPreviousTypeTaskLink = nPreviousTypeTaskLink->pPrev)
                {
                    if ((char*)pTaskBalanceIteratorpNext[-2].pNext - (char*)nPreviousTypeTaskLink[-2].pNext >= 0)// CONTAINING_RECORD(pLink,D2TaskQueueStrc,tGameTasks)->nQueueIndex
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
        TASK_LinkList_PushFront(&gpTaskQueueLink, &ptTask->tQueueLink);
    }
    LeaveCriticalSection(pTaskQueueCriticalSection);
    LeaveCriticalSection(&gTaskSlotsCriticalSection.cs);
    return nTaskSlotIndex;
}

//D2Game.0x6FC407A0 (#10042) --------------------------------------------------------
BOOL __fastcall TASK_QueueGame(D2GameTaskStrc* pTask, int nTaskType, uint32_t nGameHashKey)
{
    BOOL nResult = FALSE;
    
    pTask->nScheduledTime = TASK_GetClockTime();
    pTask->nType = nTaskType;
    pTask->nGameHashKey = nGameHashKey;
    EnterCriticalSection(&gTaskSlotsCriticalSection.cs);
    if (gpTaskQueueLink.pPrev)
    {
        D2LinkStrc* pTaskQueueHeadLink = gpTaskQueueLink.pNext;
        if (gpTaskQueueLink.pNext != &gpTaskQueueLink && gpTaskQueueLink.pNext)
        {
            D2TaskQueueStrc* ptTask = CONTAINING_RECORD(gpTaskQueueLink.pNext, D2TaskQueueStrc, tQueueLink);
            EnterCriticalSection(&gtTaskQueuesCriticalSections[ptTask->nQueueIndex].cs);
            D2TaskQueueStrc* ptTaskQueueHead = CONTAINING_RECORD(pTaskQueueHeadLink, D2TaskQueueStrc, tQueueLink);

            TASK_LinkList_PushFront(&ptTaskQueueHead->tGameTasks, &pTask->tTaskLink);

            LeaveCriticalSection(&gtTaskQueuesCriticalSections[ptTask->nQueueIndex].cs);
            ++ptTask->nGameCount;
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
int32_t __fastcall TASK_DequeueGame(int8_t nTaskNumber, D2GameTaskStrc** ppTask)
{
    const int8_t nIndex = nTaskNumber & 31;
    int32_t nResult = 100;

    *ppTask = nullptr;

    EnterCriticalSection(&gtTaskQueuesCriticalSections[nIndex].cs);

    if (D2TaskQueueStrc* pTask = gtTaskSlots[nIndex])
    {
        D2LinkStrc* pLink = pTask->tGameTasks.pNext;
        if (pLink != &pTask->tGameTasks && pLink)
        {
            const int64_t nTickCount = TASK_GetClockTime();
            nResult = CONTAINING_RECORD(pLink, D2GameTaskStrc, tTaskLink)->nScheduledTime - nTickCount;
            if (nResult <= 0)
            {
                TASK_LinkList_Remove(pLink);
                *ppTask = CONTAINING_RECORD(pLink, D2GameTaskStrc, tTaskLink);
            }
        }

        D2_ASSERT(pTask->nQueueIndex < 32);

        LeaveCriticalSection(&gtTaskQueuesCriticalSections[pTask->nQueueIndex].cs);
        return nResult;
    }

    LeaveCriticalSection(&gtTaskQueuesCriticalSections[nIndex].cs);
    return 100;
}

//D2Game.0x6FC40A40 (#10044)
int32_t __fastcall TASK_GetQueueDelay(int8_t nTaskNumber)
{
    const int8_t nIndex = nTaskNumber & 31;
    int32_t nResult = 100;
    EnterCriticalSection(&gtTaskQueuesCriticalSections[nIndex].cs);
    
    if (D2TaskQueueStrc* pTask = gtTaskSlots[nIndex])
    {
        D2LinkStrc* pLink = pTask->tGameTasks.pNext;
        if (pLink != &pTask->tGameTasks && pLink)
        {
            const int64_t nTickCount = TASK_GetClockTime();
            nResult = CONTAINING_RECORD(pLink, D2GameTaskStrc, tTaskLink)->nScheduledTime - nTickCount;
        }

        D2_ASSERT(pTask->nQueueIndex < 32);

        LeaveCriticalSection(&gtTaskQueuesCriticalSections[pTask->nQueueIndex].cs);
        return nResult;
    }

    LeaveCriticalSection(&gtTaskQueuesCriticalSections[nIndex].cs);
    return nResult;
}

static D2TaskQueueStrc* TASK_GetTaskFromTaskQueueLink(D2LinkStrc* pInTaskQueueLink)
{
    return CONTAINING_RECORD(pInTaskQueueLink, D2TaskQueueStrc, tQueueLink);
}
static D2GameTaskStrc* TASK_GetGameTaskFromLink(D2LinkStrc* pTaskLink)
{
    return CONTAINING_RECORD(pTaskLink, D2GameTaskStrc, tTaskLink);
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

    if (D2TaskQueueStrc* pTaskSlot = gtTaskSlots[nTaskNumber & 0x1F])
    {
        D2LinkStrc* ptTaskQTailLink = TASK_GetLastTaskFromListHead(&gpTaskQueueLink);
        D2_ASSERT(ptTaskQTailLink);

        const int32_t nTaskSlotType = pTaskSlot->nGameCount;
        D2TaskQueueStrc* ptTaskQTail = TASK_GetTaskFromTaskQueueLink(gpTaskQueueLink.pPrev);

        D2LinkStrc* pNewTaskLink = nullptr;
        if (TASK_GetTaskFromTaskQueueLink(gpTaskQueueLink.pNext)->nGameCount == (nTaskSlotType + 1))
        {
            EnterCriticalSection(&gtTaskQueuesCriticalSections[ptTaskQTail->nQueueIndex].cs);

            D2LinkStrc* ptTaskBalanceLink = TASK_GetLastTaskFromListHead(&ptTaskQTail->tGameTasks);
            D2_ASSERT(ptTaskBalanceLink);

            TASK_LinkList_Remove(ptTaskBalanceLink);
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[ptTaskQTail->nQueueIndex].cs);
            EnterCriticalSection(&gtTaskQueuesCriticalSections[pTaskSlot->nQueueIndex].cs);
            TASK_QueueIncrement(pTaskSlot, TASK_GetGameTaskFromLink(ptTaskBalanceLink), 0);
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[pTaskSlot->nQueueIndex].cs);
            --ptTaskQTail->nGameCount;
            TASK_LinkList_Remove(ptTaskQTailLink);
            pNewTaskLink = ptTaskQTailLink;
        }
        else
        {
            pTaskSlot->nGameCount = nTaskSlotType - 1;
            pNewTaskLink = &pTaskSlot->tQueueLink;
            TASK_LinkList_Remove(pNewTaskLink);
        }
        TASK_LinkList_PushFront(&gpTaskQueueLink, pNewTaskLink);
    }
    --gnGamesCount;
    LeaveCriticalSection(&gTaskSlotsCriticalSection.cs);
}

//D2Game.0x6FC40B30 (#10045) --------------------------------------------------------
void __fastcall TASK_ProcessGame(char nTaskNumber, D2GameTaskStrc* ptTask)
{
    D2_ASSERT(ptTask->nType == TASK_PROCESSGAME);

    const uint32_t nGameHashKey = ptTask->nGameHashKey;
    if (D2GameStrc* pGame = GAME_LockGame(nGameHashKey))
    {
        GAME_UpdateProgress(pGame);
        GAME_SendAllClientPackets(pGame, 0);
        const int32_t v7 = FOG_10055_GetSyncTime();
        const int32_t nSyncTimer = pGame->nSyncTimer;
        if (nSyncTimer == 1 || v7 - nSyncTimer > 1800)// I/O timeout
        {
            D2ClientStrc* pNextClient = nullptr;
            for (D2ClientStrc* pCurrentClient = pGame->pClientList; pCurrentClient != nullptr; pCurrentClient = pNextClient)
            {
                pNextClient = pCurrentClient->pNext;
                D2GAME_PACKETS_SendGameLeft(pCurrentClient->dwClientId);
                D2GAME_PACKETS_SendGameExit(pCurrentClient->dwClientId);
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

        if (D2TaskQueueStrc* ptTaskQ = gtTaskSlots[nTaskSlotIdx])
        {
            ptTask->nScheduledTime += 40;
            D2LinkStrc* pTaskBalancePrevLink;
            for (pTaskBalancePrevLink = ptTaskQ->tGameTasks.pPrev; pTaskBalancePrevLink != &ptTaskQ->tGameTasks; pTaskBalancePrevLink = pTaskBalancePrevLink->pPrev)
            {
                if (TASK_GetGameTaskFromLink(pTaskBalancePrevLink)->nScheduledTime <= ptTask->nScheduledTime)
                    break;
            }
            // Keep the queue ordered by nScheduledTime
            TASK_LinkList_Insert(pTaskBalancePrevLink, &ptTask->tTaskLink);
            
            D2_ASSERT(ptTaskQ->nQueueIndex < TASKQSLOTS);
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[ptTaskQ->nQueueIndex].cs);
        }
        else
        {
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[nTaskSlotIdx].cs);
        }
    }
}

//D2Game.0x6FC40E40
void __fastcall TASK_QueueIncrement(D2TaskQueueStrc* ptTaskQueue, D2GameTaskStrc* pGameTask, int nDelay)
{
    D2LinkStrc* ptTaskBalanceLink = &ptTaskQueue->tGameTasks;
    pGameTask->nScheduledTime += nDelay;
    D2LinkStrc* ptPrev;
    for (ptPrev = ptTaskBalanceLink->pPrev; ptPrev != ptTaskBalanceLink; ptPrev = ptPrev->pPrev)
    {
        if (TASK_GetGameTaskFromLink(ptPrev)->nScheduledTime <= pGameTask->nScheduledTime)
            break;
    }
    
    D2LinkStrc* pGameTaskLink = &pGameTask->tTaskLink;
    TASK_LinkList_Insert(ptPrev, pGameTaskLink);
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



void __stdcall D2Game_10034_Return(int nUnused)
{
}

void __stdcall D2Game_10060_Return()
{
}

void __stdcall D2Game_10061_Return()
{
}
