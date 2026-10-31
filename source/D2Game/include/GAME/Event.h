#pragma once

#include <Units/Units.h>

#pragma pack(1)

enum D2C_EventFlags : uint16_t
{
	EVENTFLAG_EXECUTING = 0x1,
	EVENTFLAG_FREE = 0x2,
	EVENTFLAG_RECURRING = 0x4,
	EVENTFLAG_DELETE_PENDING = 0x8,
};

enum D2C_EventTypes : uint8_t
{
	EVENTTYPE_MODECHANGE = 0,
	EVENTTYPE_ENDANIM = 1,
	EVENTTYPE_AITHINK = 2,
	EVENTTYPE_STATREGEN = 3,
	EVENTTYPE_TRAP = 4,
	EVENTTYPE_ACTIVESTATE = 5,
	EVENTTYPE_RESET = 5,
	EVENTTYPE_FREEHOVER = 6,
	EVENTTYPE_MONUMOD = 7,
	EVENTTYPE_QUESTFN = 7,
	EVENTTYPE_PERIODICSKILLS = 8,
	EVENTTYPE_PERIODICSTATS = 9,
	EVENTTYPE_AIRESET = 10,
	EVENTTYPE_DELAYEDPORTAL = 11,
	EVENTTYPE_REMOVESTATE = 12,
	EVENTTYPE_UPDATETRADE = 13,
	EVENTTYPE_REFRESHVENDOR = 13,
	EVENTTYPE_REMOVESKILLCOOLDOWN = 14,
	EVENTTYPE_COUNT = 15,
	EVENTTYPE_CUSTOM = 16,
};


struct D2TimerArg2Strc
{
	int nUnitGUID;
	D2TimerArg2Strc* pNext;
};

struct D2TimerArgStrc
{
	D2TimerArg2Strc* unk0x00;
	uint32_t dwUnitId;
};

using EventTimerCallback = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*, D2C_EventTypes, int32_t, int32_t);
struct D2EventTimerStrc
{
	D2C_EventTypes nEventType;
	int8_t padding0x01;
	uint16_t nFlags;
	int32_t nExpireFrame;
	D2UnitStrc* pUnit;
	int32_t nUnitGUID;
	int32_t nUnitType;
	int32_t dwEventCustomId;
	int32_t dwEventCustomParam;
	D2EventTimerStrc* pNextQueuedTimer; // Also links unused slots in a slab.
	D2EventTimerStrc* pPreviousQueuedTimer;
	D2EventTimerStrc* pNextUnitTimer;
	D2EventTimerStrc* pPreviousUnitTimer;
	EventTimerCallback pCallback;
};

struct D2EventTimerSlabListStrc
{
	D2EventTimerStrc tEventTimersStorage[600];
	D2EventTimerStrc* pFreeEventTimerListHead;
	D2EventTimerSlabListStrc* pNextSlab;
};

struct D2EventTimerQueueStrc
{
	int32_t nArrayIndex;
	D2EventTimerStrc* pHead[5][64];			// first event per unit type per frame
	D2EventTimerStrc* pTail[5][64];			// last event per unit type per frame
	D2EventTimerStrc* pInfinite[5];			// events with no end frame
	D2EventTimerStrc* pNextTimerToExecute;
	D2EventTimerSlabListStrc* pSlabListHead;
};



#pragma pack()

//D2Game.0x6FC34840
//1.14d: 0x00540E60
void __fastcall D2GAME_EVENTS_Delete_6FC34840(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t nEventCustomId);
//D2Game.0x6FC34890
//1.14d: 0x00540CD0
void __fastcall EVENT_FreeTimer(D2GameStrc* pGame, D2EventTimerStrc* pTimer);
//D2Game.0x6FC349B0
//1.14d: 0x00540EA0
void __fastcall EVENT_DeleteTimersByCallback(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventType, EventTimerCallback pCallback);
//D2Game.0x6FC349F0
//1.14d: 0x00540EE0
void __fastcall EVENT_FreeUnitTimers(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC34A30
//1.14d: 0x00540F30
void __fastcall D2GAME_DeleteTimersOnUnit_6FC34A30(D2GameStrc* pGame, D2UnitStrc* pUnit);
//1.10f: D2Game.0x6FC34A80
//1.13c: D2Game.6FCAE4D0
//1.14d: 0x00541400
void __fastcall EVENT_FreeEventQueue(D2GameStrc* pGame);
//D2Game.0x6FC34AE0
//1.14d: 0x00541470
void __fastcall EVENT_AllocEventQueue(D2GameStrc* pGame);
//D2Game.0x6FC34BD0
//1.14d: 0x005414D0
void __fastcall EVENT_IterateEvents(D2GameStrc* pGame);
//D2Game.0x6FC34CC0
//1.14d: 0x00540F60
void __fastcall EVENT_ExecutePlayerEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t bTimedEvents);
//D2Game.0x6FC34DB0
//1.14d: 0x00541060
void __fastcall EVENT_ExecuteMonsterEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t bTimedEvents);
//D2Game.0x6FC34EA0
//1.14d: 0x00541160
void __fastcall EVENT_ExecuteObjectEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t bTimedEvents);
//D2Game.0x6FC34F90
//1.14d: 0x00541240
void __fastcall EVENT_ExecuteMissileEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t bTimedEvents);
//D2Game.0x6FC35080
//1.14d: 0x00541320
void __fastcall EVENT_ExecuteItemEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t bTimedEvents);
//D2Game.0x6FC35170
//1.14d: 0x005415A0
int32_t __fastcall EVENT_GetEventFrame(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEventType);
//D2Game.0x6FC351B0
//1.14d: 0x005417D0
void __fastcall EVENT_SetEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t nExpireFrame, int32_t dwEventCustomId, int32_t dwEventCustomParam);
//D2Game.0x6FC351D0
//1.14d: 0x005416B0
void __fastcall EVENT_ScheduleTimer(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t nExpireFrame, EventTimerCallback pfCallBack, int32_t dwEventCustomId, int32_t dwEventCustomParam);
//D2Game.0x6FC353D0
D2EventTimerStrc** __fastcall EVENT_GetTimerListHead(D2GameStrc* pGame, int32_t nUnitType, int32_t nExpireFrame);
//1.10f: D2Game.0x6FC35410
//1.13c: D2Game.0x6FCAE420
//1.14d: 0x00540BB0
D2EventTimerSlabListStrc* __fastcall EVENT_AllocTimerSlab(D2GameStrc* pGame);
//D2Game.0x6FC35460
//1.14d: 0x00540DF0
D2EventTimerStrc* __fastcall EVENT_AllocateUnitTimer(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC35570
//1.14d: 0x00541650
void __fastcall EVENT_SetRecurringEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t dwEventCustomId, int32_t dwEventCustomParam);
//D2Game.0x6FC356A0
//1.14d: 0x00541800
void __fastcall EVENT_SetEventWithCallback(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_EventTypes nEventType, int32_t nExpireFrame, EventTimerCallback pfCallBack, int32_t dwEventCustomId, int32_t dwEventCustomParam);
