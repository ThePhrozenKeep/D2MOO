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

struct D2TaskStrc
{
	int32_t nQueueIndex;						//0x00
	int32_t nGameCount;							//0x04 games queued on this task queue
	D2LinkStrc tGameTasks;						//0x08 the queued games (D2GameStrc task links)
	D2LinkStrc tQueueLink;						//0x10 link in the global list of task queues
};

struct CRITICAL_SECTION_WRAPPER
{
    CRITICAL_SECTION cs;

    CRITICAL_SECTION_WRAPPER() { InitializeCriticalSection(&cs); }
    ~CRITICAL_SECTION_WRAPPER() { DeleteCriticalSection(&cs); }
};
#pragma pack(pop)


//D2Game.0x6FC404E0 (#10039)
//1.14d: 0x00564320
void __stdcall TASK_InitializeClock();
//D2Game.0x6FC40500 (#10040)
//1.14d: 0x00564890
void __stdcall TASK_FreeAllQueueSlots();
//D2Game.0x6FC405B0 (#10041)
//1.14d: 0x00564960
int32_t __cdecl TASK_CreateQueue();
//D2Game.0x6FC407A0 (#10042)
//1.14d: 0x00564A00
BOOL __fastcall TASK_QueueGame(D2TaskStrc* pTask, int nTaskType, D2LinkStrc* pPrevTaskBalanceLink);
//D2Game.0x6FC40930 (#10043)
//1.14d: 0x00564B20
int32_t __fastcall TASK_DequeueGame(int8_t nTaskNumber, int32_t* pOutBalanceTaskType);
//D2Game.0x6FC40A40 (#10044)
//1.14d: 0x00564BE0
int32_t __fastcall TASK_GetQueueDelay(int8_t nTaskNumber);
//D2Game.0x6FC40B30 (#10045)
//1.14d: 0x005645E0
void __fastcall TASK_ProcessGame(char nTaskNumber, D2TaskStrc* ptTask);
//D2Game.0x6FC40E40
void __fastcall TASK_QueueIncrement(D2TaskStrc* ptTaskQueue, int32_t* pTaskType, int nTaskTypeIncrement);
//D2Game.0x6FC40ED0
void __fastcall TASK_LinkList_Insert(D2LinkStrc* pPrev, D2LinkStrc* pLink);
//D2Game.0x6FC40F20
//1.14d: 0x00564250
void __fastcall TASK_LinkList_PushFront(D2LinkStrc* pList, D2LinkStrc* pLink);
//D2Game.0x6FC40F70
//1.14d: 0x005642E0
void __fastcall TASK_LinkList_Remove(D2LinkStrc* pLink);



//1.10f: D2Game.0x6FCA4640 (#10034)
//1.14d: 0x00545990
void __stdcall D2Game_10034_Return(int nUnused);
void __stdcall D2Game_10060_Return();
void __stdcall D2Game_10061_Return();