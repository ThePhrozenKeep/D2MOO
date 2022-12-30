#pragma once

#include <Units/Units.h>
#include "Game/Event.h"
#include "SUnitDmg.h"

using D2UnitEventCallbackFunction = int32_t(__fastcall*)(D2GameStrc*, int32_t, D2UnitStrc*, D2UnitStrc*, D2DamageStrc*, int32_t, int32_t);

struct D2UnitEventStrc
{
	uint8_t nTimerType;						//0x00 D2C_UnitEventCallbackTypes
	uint8_t unk0x01;						//0x01
	uint16_t nTimerFlags;					//0x02
	uint32_t nQueueNo;						//0x04
	int32_t unk0x08;						//0x08
	int32_t nGUID1;							//0x0C First identifier
	int32_t nGUID2;							//0x10 Second identifier
	D2UnitEventCallbackFunction pCallback;	//0x14
	D2UnitEventStrc* pPrevious;				//0x18  Seems to be any kind of id, not necessariyl the prevtimer ?
	D2UnitEventStrc* pNext;					//0x1C
};

//D2Game.0x6FCC3610
void __fastcall SUNITEVENT_FreeTimerList(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCC3650
D2UnitEventStrc* __fastcall SUNITEVENT_AllocTimer(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t nTimerType, int32_t nGUID1, int32_t nGUID2, D2UnitEventCallbackFunction pCallback, uint32_t nQueueNo, int32_t a8);
//D2Game.0x6FCC36D0
void __fastcall SUNITEVENT_FreeTimer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTimerQueueNo, int32_t a4);
//D2Game.0x6FCC3750
D2UnitEventStrc* __fastcall SUNITEVENT_GetTimer(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nTimerQueueNo, int32_t a4, int32_t nGUID1);
//D2Game.0x6FCC3790
int32_t __fastcall SUNITEVENT_EventFunc_Handler(D2GameStrc* pGame, int32_t nEvent, D2UnitStrc* pAttacker, D2UnitStrc* pUnit, D2DamageStrc* pDamage);
