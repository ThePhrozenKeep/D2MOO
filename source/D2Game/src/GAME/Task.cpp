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

        if (D2TaskQueueStrc* pTaskQueue = gtTaskSlots[i])
        {
            TASK_LinkList_Remove(&pTaskQueue->tQueueLink);
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
        D2TaskQueueStrc* pTaskQueue = D2_ALLOC_STRC_POOL(nullptr, D2TaskQueueStrc);
        D2LinkStrc* pGameTasks = &pTaskQueue->tGameTasks;
        pTaskQueue->nQueueIndex = nTaskSlotIndex;
        pTaskQueue->nGameCount = 0;
        pTaskQueue->tGameTasks.pPrev = &pTaskQueue->tGameTasks;
        pTaskQueue->tGameTasks.pNext = &pTaskQueue->tGameTasks;
        int nTasksCreated = gnNumberOfTasksCreated;
        uint32_t nGamesPerQueue = gnGamesCount / (uint32_t)gnNumberOfTasksCreated;
        uint32_t nExtraGames = gnGamesCount % (uint32_t)gnNumberOfTasksCreated;
        gtTaskSlots[nTaskSlotIndex] = pTaskQueue;
        int nOtherQueueCount = nTasksCreated - 1;
        if (nTasksCreated - 1 >= (unsigned int)TASKQSLOTS)
            nOtherQueueCount = 32;
        int nQueuesWithExtraGame = nOtherQueueCount - nExtraGames;
        for (D2LinkStrc* pLink = gpTaskQueueLink.pNext; pLink != &gpTaskQueueLink; pLink = pLink->pNext)
        {
            bool bNoExtraGame = (int)--nQueuesWithExtraGame < 0;
#define D2_PAIR64(high, low) ((uint64_t(uint32_t(high)) << 32) |(uint32_t(low)))
            int nWalkSteps = (D2_PAIR64(CONTAINING_RECORD(pLink, D2TaskQueueStrc, tQueueLink)->nGameCount, nGamesPerQueue + !bNoExtraGame) - 1) >> 32;
            EnterCriticalSection(&gtTaskQueuesCriticalSections[CONTAINING_RECORD( pLink, D2TaskQueueStrc, tQueueLink)->nQueueIndex].cs);
            D2LinkStrc* pOtherGameLink = pLink[-1].pPrev;
            D2LinkStrc* pOtherQueuedGames = pLink - 1;
            D2LinkStrc* pLastMovedLink = pOtherGameLink;
            for (; nWalkSteps; pOtherGameLink = pOtherGameLink->pNext)
            {
                --nWalkSteps;
                if (pOtherGameLink == pOtherQueuedGames)
                    break;
            }
            D2LinkStrc* pMovedLink = pOtherGameLink->pNext;
            pOtherGameLink->pNext = pOtherQueuedGames;
            pOtherQueuedGames->pPrev = pOtherGameLink;
            LeaveCriticalSection(&gtTaskQueuesCriticalSections[CONTAINING_RECORD( pLink, D2TaskQueueStrc, tQueueLink)->nQueueIndex].cs);
            if (pMovedLink != pOtherQueuedGames)
            {
                D2LinkStrc* pMovedTail = pLastMovedLink;
                pLastMovedLink->pNext = 0;
                D2LinkStrc* pInsertAfter;
                for (pInsertAfter = pGameTasks->pPrev;
                    pInsertAfter != pGameTasks;
                    pInsertAfter = pInsertAfter->pPrev)
                {
                    if ((char*)pMovedLink[-2].pNext - (char*)pInsertAfter[-2].pNext >= 0)// [-2].pNext: D2GameTaskStrc::nScheduledTime
                        break;
                }
                if (pInsertAfter->pNext == pGameTasks)
                {
                LABEL_22:
                    pMovedLink->pPrev = pInsertAfter;
                    pMovedTail->pNext = pGameTasks;
                    pInsertAfter->pNext = pMovedLink;
                    pGameTasks->pPrev = pMovedTail;
                }
                else
                {
                    while (1)
                    {
                        D2LinkStrc* pNextMoved = pMovedLink->pNext;
                        pMovedLink->pPrev = pInsertAfter;
                        D2LinkStrc* pInsertBefore = pInsertAfter->pNext;
                        pMovedLink->pNext = pInsertBefore;
                        pInsertBefore->pPrev = pMovedLink;
                        pInsertAfter->pNext = pMovedLink;
                        if (!pNextMoved)
                            break;
                        pInsertAfter = pMovedLink;
                        pMovedLink = pNextMoved;
                        D2LinkStrc* pScanLink = pInsertAfter->pNext;
                        if (pScanLink != pGameTasks)
                        {
                            D2LinkStrc* pNextMovedTime = pNextMoved[-2u].pNext; // NOLINT TODO proper list
                            do
                            {
                                if ((char*)pNextMovedTime - (char*)pScanLink[-2].pNext <= 0) // NOLINT TODO proper list
                                    break;
                                pInsertAfter = pScanLink;
                                pScanLink = pScanLink->pNext;
                            } while (pScanLink != pGameTasks);
                        }
                        if (pInsertAfter->pNext == pGameTasks)
                        {
                            pMovedTail = pLastMovedLink;
                            goto LABEL_22;
                        }
                    }
                }
            }
        }
        TASK_LinkList_PushFront(&gpTaskQueueLink, &pTaskQueue->tQueueLink);
    }
    LeaveCriticalSection(pTaskQueueCriticalSection);
    LeaveCriticalSection(&gTaskSlotsCriticalSection.cs);
    return nTaskSlotIndex;
}

//D2Game.0x6FC407A0 (#10042) --------------------------------------------------------
BOOL __fastcall TASK_QueueGame(D2GameTaskStrc* pGameTask, int nTaskType, uint32_t nGameHashKey)
{
    BOOL nResult = FALSE;
    
    pGameTask->nScheduledTime = TASK_GetClockTime();
    pGameTask->nType = nTaskType;
    pGameTask->nGameHashKey = nGameHashKey;
    EnterCriticalSection(&gTaskSlotsCriticalSection.cs);
    if (gpTaskQueueLink.pPrev)
    {
        D2LinkStrc* pTaskQueueHeadLink = gpTaskQueueLink.pNext;
        if (gpTaskQueueLink.pNext != &gpTaskQueueLink && gpTaskQueueLink.pNext)
        {
            D2TaskQueueStrc* pTaskQueue = CONTAINING_RECORD(gpTaskQueueLink.pNext, D2TaskQueueStrc, tQueueLink);
            EnterCriticalSection(&gtTaskQueuesCriticalSections[pTaskQueue->nQueueIndex].cs);
            D2TaskQueueStrc* ptTaskQueueHead = CONTAINING_RECORD(pTaskQueueHeadLink, D2TaskQueueStrc, tQueueLink);

            TASK_LinkList_PushFront(&ptTaskQueueHead->tGameTasks, &pGameTask->tTaskLink);

            LeaveCriticalSection(&gtTaskQueuesCriticalSections[pTaskQueue->nQueueIndex].cs);
            ++pTaskQueue->nGameCount;
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

static D2TaskQueueStrc* TASK_GetTaskQueueFromLink(D2LinkStrc* pInTaskQueueLink)
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

        const int32_t nGameCount = pTaskSlot->nGameCount;
        D2TaskQueueStrc* ptTaskQTail = TASK_GetTaskQueueFromLink(gpTaskQueueLink.pPrev);

        D2LinkStrc* pNewTaskLink = nullptr;
        if (TASK_GetTaskQueueFromLink(gpTaskQueueLink.pNext)->nGameCount == (nGameCount + 1))
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
            pTaskSlot->nGameCount = nGameCount - 1;
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
        const int32_t nSyncTime = FOG_10055_GetSyncTime();
        const int32_t nSyncTimer = pGame->nSyncTimer;
        if (nSyncTimer == 1 || nSyncTime - nSyncTimer > 1800)// I/O timeout
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
            D2LinkStrc* pPrevGameLink;
            for (pPrevGameLink = ptTaskQ->tGameTasks.pPrev; pPrevGameLink != &ptTaskQ->tGameTasks; pPrevGameLink = pPrevGameLink->pPrev)
            {
                if (TASK_GetGameTaskFromLink(pPrevGameLink)->nScheduledTime <= ptTask->nScheduledTime)
                    break;
            }
            // Keep the queue ordered by nScheduledTime
            TASK_LinkList_Insert(pPrevGameLink, &ptTask->tTaskLink);
            
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
    D2LinkStrc* pGameTasks = &ptTaskQueue->tGameTasks;
    pGameTask->nScheduledTime += nDelay;
    D2LinkStrc* ptPrev;
    for (ptPrev = pGameTasks->pPrev; ptPrev != pGameTasks; ptPrev = ptPrev->pPrev)
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
