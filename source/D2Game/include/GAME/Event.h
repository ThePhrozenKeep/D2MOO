#pragma once

#include <Units/Units.h>

#pragma pack(1)

enum D2C_EventFlags : uint16_t
{
	EVENTFLAG_0x1 = 0x1, // Some kind of lock or to prevent reentrancy ?
	EVENTFLAG_0x2 = 0x2,
	EVENTFLAG_0x4 = 0x4,
	EVENTFLAG_0x8 = 0x8,
};

enum D2C_EventTypes
{
	EVENT_HITBYMISSILE,
	EVENT_DAMAGEDINMELEE,
	EVENT_DAMAGEDBYMISSILE,
	EVENT_ATTACKEDINMELEE,
	EVENT_DOACTIVE,
	EVENT_DOMELEEDMG,
	EVENT_DOMISSILEDMG,
	EVENT_DOMELEEATTACK,
	EVENT_DOMISSILEATTACK,
	EVENT_KILL,
	EVENT_KILLED,
	EVENT_ABSORBDAMAGE,
	EVENT_LEVELUP,
	EVENT_DEATH, // Monster dies
};

enum D2C_UnitEventCallbackTypes
{
	UNITEVENTCALLBACK_MODECHANGE = 0,
	UNITEVENTCALLBACK_ENDANIM = 1,
	UNITEVENTCALLBACK_AITHINK = 2,
	UNITEVENTCALLBACK_STATREGEN = 3,
	UNITEVENTCALLBACK_TRAP = 4,
	UNITEVENTCALLBACK_ACTIVESTATE = 5,
	UNITEVENTCALLBACK_RESET = 5,
	UNITEVENTCALLBACK_FREEHOVER = 6,
	UNITEVENTCALLBACK_MONUMOD = 7,
	UNITEVENTCALLBACK_QUESTFN = 7,
	UNITEVENTCALLBACK_PERIODICSKILLS = 8,
	UNITEVENTCALLBACK_PERIODICSTATS = 9,
	UNITEVENTCALLBACK_AIRESET = 10,
	UNITEVENTCALLBACK_DELAYEDPORTAL = 11,
	UNITEVENTCALLBACK_REMOVESTATE = 12,
	UNITEVENTCALLBACK_UPDATETRADE = 13,
	UNITEVENTCALLBACK_REFRESHVENDOR = 13,
	UNITEVENTCALLBACK_REMOVESKILLCOOLDOWN = 14,
	UNITEVENTCALLBACK_COUNT = 15,
	UNITEVENTCALLBACK_CUSTOM = 16,
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

using EventTimerCallback = int32_t(__fastcall*)(D2GameStrc*, D2UnitStrc*, int32_t, int32_t, int32_t);
struct D2EventTimerStrc
{
	int8_t nEvent;
	int8_t padding0x01;
	uint16_t nFlags;
	int32_t nExpireFrame;
	D2UnitStrc* pUnit;
	int32_t nUnitGUID;
	int32_t nUnitType;
	int32_t nSkillId;
	int32_t nSkillLevel;
	D2EventTimerStrc* pNextFreeEventTimer;
	D2EventTimerStrc* unk0x20;
	D2EventTimerStrc* pNext;
	D2EventTimerStrc* pPrevious;
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
	D2EventTimerStrc* unk0x04[10][64];
	D2EventTimerStrc* unk0xA04[5];
	D2EventTimerStrc* unk0xA18;
	D2EventTimerSlabListStrc* pSlabListHead;
};



#pragma pack()

//D2Game.0x6FC34840
void __fastcall D2GAME_EVENTS_Delete_6FC34840(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEvent, int32_t nSkillId);
//D2Game.0x6FC34890
void __fastcall sub_6FC34890(D2GameStrc* pGame, D2EventTimerStrc* pTimer);
//D2Game.0x6FC349B0
void __fastcall sub_6FC349B0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEvent, EventTimerCallback pCallback);
//D2Game.0x6FC349F0
void __fastcall sub_6FC349F0(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC34A30
void __fastcall D2GAME_DeleteTimersOnUnit_6FC34A30(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC34A80
void __fastcall EVENT_FreeEventQueue(D2GameStrc* pGame);
//D2Game.0x6FC34AE0
void __fastcall EVENT_AllocEventQueue(D2GameStrc* pGame);
//D2Game.0x6FC34BD0
void __fastcall EVENT_IterateEvents(D2GameStrc* pGame);
//D2Game.0x6FC34CC0
void __fastcall EVENT_ExecutePlayerEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4);
//D2Game.0x6FC34DB0
void __fastcall EVENT_ExecuteMonsterEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4);
//D2Game.0x6FC34EA0
void __fastcall EVENT_ExecuteObjectEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4);
//D2Game.0x6FC34F90
void __fastcall EVENT_ExecuteMissileEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4);
//D2Game.0x6FC35080
void __fastcall EVENT_ExecuteItemEvents(D2GameStrc* pGame, D2EventTimerQueueStrc* pTimerQueue, D2EventTimerStrc* pEventTimer, int32_t a4);
//D2Game.0x6FC35170
int32_t __fastcall EVENT_GetEventFrame(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEvent);
//D2Game.0x6FC351B0
void __fastcall EVENT_SetEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTimerType, int32_t nExpireFrame, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FC351D0
void __fastcall D2GAME_InitTimer_6FC351D0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTimerType, int32_t nExpireFrame, EventTimerCallback pfCallBack, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FC353D0
D2EventTimerStrc** __fastcall sub_6FC353D0(D2GameStrc* pGame, int32_t nUnitType, int32_t nExpireFrame);
//D2Game.0x6FC35410
D2EventTimerSlabListStrc* __fastcall EVENT_AllocTimerSlab(D2GameStrc* pGame);
//D2Game.0x6FC35460
D2EventTimerStrc* __fastcall sub_6FC35460(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC35570
void __fastcall sub_6FC35570(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nEvent, int32_t nSkillId, int32_t nSkillLevel);
//D2Game.0x6FC351D0
void __fastcall j_D2GAME_InitTimer_6FC351D0(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTimerType, int32_t nExpireFrame, EventTimerCallback pfCallBack, int32_t nSkillId, int32_t nSkillLevel);
