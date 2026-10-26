#pragma once

#include <Units/Units.h>


#pragma pack(push, 1)

enum D2C_TaskConstants {
    TASKQSLOTS = 0x20
};

enum D2C_TaskTypes
{
    TASK_PROCESSGAME = 0x0,
};

// Note: comes from D2Headers/Linklist.h
struct D2LinkStrc
{
    D2LinkStrc* pPrev;
    D2LinkStrc* pNext;
};

// One of the TASKQSLOTS task queues
struct D2TaskQueueStrc
{
	int32_t nQueueIndex;						//0x00 index in the task queue slots
	int32_t nGameCount;							//0x04
	D2LinkStrc tGameTasks;						//0x08 D2GameTaskStrc::tTaskLink, ordered by nScheduledTime
	D2LinkStrc tQueueLink;						//0x10 link in the list of task queues
};

// A game's entry in a task queue (1.10f: D2GameStrc::tGameTask, 1.14d: allocated on its own)
struct D2GameTaskStrc
{
	int32_t nScheduledTime;						//0x00 clock time (ms) the game is processed next
	int32_t nType;								//0x04 D2C_TaskTypes
	uint32_t nGameHashKey;						//0x08
	D2LinkStrc tTaskLink;						//0x0C link in D2TaskQueueStrc::tGameTasks
};

struct CRITICAL_SECTION_WRAPPER
{
    CRITICAL_SECTION cs;

    CRITICAL_SECTION_WRAPPER() { InitializeCriticalSection(&cs); }
    ~CRITICAL_SECTION_WRAPPER() { DeleteCriticalSection(&cs); }
};
#pragma pack(pop)


//D2Game.0x6FC404E0 (#10039)
void __stdcall TASK_InitializeClock();
//D2Game.0x6FC40500 (#10040)
void __stdcall TASK_FreeAllQueueSlots();
//D2Game.0x6FC405B0 (#10041)
int32_t __cdecl TASK_CreateQueue();
//D2Game.0x6FC407A0 (#10042)
BOOL __fastcall TASK_QueueGame(D2GameTaskStrc* pGameTask, int nTaskType, uint32_t nGameHashKey);
//D2Game.0x6FC40930 (#10043)
int32_t __fastcall TASK_DequeueGame(int8_t nTaskNumber, D2GameTaskStrc** ppTask);
//D2Game.0x6FC40A40 (#10044)
int32_t __fastcall TASK_GetQueueDelay(int8_t nTaskNumber);
//D2Game.0x6FC40B30 (#10045)
void __fastcall TASK_ProcessGame(char nTaskNumber, D2GameTaskStrc* ptTask);
//D2Game.0x6FC40E40
void __fastcall TASK_QueueIncrement(D2TaskQueueStrc* ptTaskQueue, D2GameTaskStrc* pGameTask, int nDelay);
//D2Game.0x6FC40ED0
void __fastcall TASK_LinkList_Insert(D2LinkStrc* pPrev, D2LinkStrc* pLink);
//D2Game.0x6FC40F20
void __fastcall TASK_LinkList_PushFront(D2LinkStrc* pList, D2LinkStrc* pLink);
//D2Game.0x6FC40F70
void __fastcall TASK_LinkList_Remove(D2LinkStrc* pLink);



void __stdcall D2Game_10034_Return(int nUnused);
void __stdcall D2Game_10060_Return();
void __stdcall D2Game_10061_Return();