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
	int32_t nTaskQ;								//0x00
	int32_t nType;								//0x04 D2C_TaskTypes, but somehow also used for timeout ?
	D2LinkStrc pTaskBalanceLink;				//0x08 May actually link to &pTaskQueueLink !
	D2LinkStrc pTaskQueueLink;					//0x10
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
int32_t __cdecl D2Game_10041_TASK_Create();
//D2Game.0x6FC407A0 (#10042)
BOOL __fastcall D2Game_10042(D2TaskStrc* pTask, int nTaskType, D2LinkStrc* pPrevTaskBalanceLink);
//D2Game.0x6FC40930 (#10043)
int32_t __fastcall D2Game_10043(int8_t a1, int32_t* pOutBalanceTaskType);
//D2Game.0x6FC40A40 (#10044)
int32_t __fastcall D2Game_10044(int8_t nTaskNumber);
//D2Game.0x6FC40B30 (#10045)
void __fastcall TASK_ProcessGame(char nTaskNumber, D2TaskStrc* ptTask);
//D2Game.0x6FC40E40
void __fastcall TASK_QueueIncrement(D2TaskStrc* ptTaskQueue, int32_t* pTaskType, int nTaskTypeIncrement);
//D2Game.0x6FC40ED0
void __fastcall TASK_LinkList_Insert(D2LinkStrc* pPrev, D2LinkStrc* pLink);
//D2Game.0x6FC40F20
void __fastcall TASK_LinkList_PushFront(D2LinkStrc* pList, D2LinkStrc* pLink);
//D2Game.0x6FC40F70
void __fastcall TASK_LinkList_Remove(D2LinkStrc* pLink);



void __stdcall D2Game_10034_Return(int a1);
void __stdcall D2Game_10060_Return();
void __stdcall D2Game_10061_Return();