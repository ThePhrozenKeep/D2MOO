#pragma once

#pragma pack(1)

// Note: These are actually called "Events", not timers, in the game

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
	EVENT_DEATH,
	EVENT_ABSORBDAMAGE,
	EVENT_LEVELUP,
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


struct D2TimerArgStrc
{
	void* pUnk;
	uint32_t dwUnitId;
};

struct D2TimerQueueStrc
{
	uint32_t dwQueueNo;
	D2TimerStrc* pActiveTimerList[5][256];
	D2TimerStrc* pExpiredTimerList[5][256];
	D2TimerStrc* pTimerListByType[5];
	void* pTimerList;
};

struct D2TimerStrc
{
	uint16_t wTimerType;					//0x00 D2C_UnitEventCallbackTypes
	uint16_t wTimerId;						//0x02
	uint32_t nQueueNo;						//0x04
	D2UnitStrc* pUnit;						//0x08
	uint32_t dwUnitId;						//0x0C
	uint32_t dwUnitType;					//0x10
	uint32_t dwCallbackParam0;				//0x14
	D2TimerStrc* pPrevTimer;				//0x18 Seems to be any kind of id, not necessariyl the prevtimer ?
	D2TimerStrc* pNextTimer;				//0x1C
	D2TimerStrc* pPrevTimerOnUnit;			//0x20
	D2TimerStrc* pNextTimerOnUnit;			//0x24
	void* pTimerList;						//0x28
	void* fpTimerFunction;					//0x2C
};

#pragma pack()
