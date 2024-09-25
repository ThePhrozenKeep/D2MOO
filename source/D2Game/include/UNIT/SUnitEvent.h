#pragma once

#include <Units/Units.h>
#include "GAME/Event.h"
#include "SUnitDmg.h"

using D2UnitEventCallbackFunction = int32_t(__fastcall*)(D2GameStrc*, int32_t, D2UnitStrc*, D2UnitStrc*, D2DamageStrc*, int32_t, int32_t);

enum D2C_UnitEventTypes: uint8_t // Events.txt
{
	UNITEVENT_HITBYMISSILE,
	UNITEVENT_DAMAGEDINMELEE,
	UNITEVENT_DAMAGEDBYMISSILE,
	UNITEVENT_ATTACKEDINMELEE,
	UNITEVENT_DOACTIVE,
	UNITEVENT_DOMELEEDMG,
	UNITEVENT_DOMISSILEDMG,
	UNITEVENT_DOMELEEATTACK,
	UNITEVENT_DOMISSILEATTACK,
	UNITEVENT_KILL,
	UNITEVENT_KILLED,
	UNITEVENT_ABSORBDAMAGE,
	UNITEVENT_LEVELUP,
	UNITEVENT_DEATH, // Monster dies
};

enum D2C_UnitEventFlags : uint16_t {
	UNITEVENTFLAG_IN_CALLBACK = 1 << 0, // Prevents destruction during callback
	UNITEVENTFLAG_SHOULD_BE_FREED = 1 << 1,
};

struct D2UnitEventStrc
{
	D2C_UnitEventTypes nUnitEvent;			//0x00 Event from events.txt. NOT the same thing as timer events
	uint8_t unk0x01;						//0x01
	uint16_t nFlags;						//0x02 D2C_UnitEventFlags
	uint32_t nQueueNo;						//0x04
	int32_t unk0x08;						//0x08
	int32_t nGUID1;							//0x0C First identifier
	int32_t nGUID2;							//0x10 Second identifier
	D2UnitEventCallbackFunction pCallback;	//0x14
	D2UnitEventStrc* pPrevious;				//0x18  Seems to be any kind of id, not necessariyl the prevtimer ?
	D2UnitEventStrc* pNext;					//0x1C
};

//D2Game.0x6FCC3610
void __fastcall SUNITEVENT_FreeEventList(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCC3650
D2UnitEventStrc* __fastcall SUNITEVENT_Register(D2GameStrc* pGame, D2UnitStrc* pUnit, D2C_UnitEventTypes nUnitEvent, int32_t nGUID1, int32_t nGUID2, D2UnitEventCallbackFunction pCallback, uint32_t nQueueNo, int32_t a8);
//D2Game.0x6FCC36D0
void __fastcall SUNITEVENT_Unregister(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTimerQueueNo, int32_t a4);
//D2Game.0x6FCC3750
D2UnitEventStrc* __fastcall SUNITEVENT_GetEvent(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTimerQueueNo, int32_t a4, int32_t nGUID1);
//D2Game.0x6FCC3790
void __fastcall SUNITEVENT_Trigger(D2GameStrc* pGame, D2C_UnitEventTypes nUnitEvent, D2UnitStrc* pUnit, D2UnitStrc* pEventSourceUnit, D2DamageStrc* pDamage);
