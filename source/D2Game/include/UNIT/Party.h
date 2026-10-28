#pragma once

#include <Units/Units.h>
#include "GAME/Game.h"

#pragma pack(1)

struct D2PartyNodeStrc
{
	int nUnitGUID;
	D2PartyNodeStrc* pNext;
};

struct D2PartyStrc
{
	int16_t nPartyId;
	int16_t field_2;
	D2PartyNodeStrc* pPartyNodes;
	D2PartyStrc* pNext;
};

struct D2PartyControlStrc
{
	int16_t nNextPartyId;
	int16_t field_2;
	D2PartyStrc* pParties;
};

using PartyCallbackFunction = void(__fastcall*)(D2GameStrc*, D2UnitStrc*, void*);

#pragma pack()

//D2Game.0x6FCB9B00
//1.14d: 0x0053FF90
void __fastcall PARTY_AllocPartyControl(D2GameStrc* pGame);
//D2Game.0x6FCB9BA0
//1.14d: 0x00540000
void __fastcall PARTY_FreePartyControl(D2GameStrc* pGame);
//D2Game.0x6FCB9C40
//1.14d: 0x00540100
int16_t __fastcall PARTY_CreateParty(D2GameStrc* pGame);
//D2Game.0x6FCB9D10
//1.14d: 0x00540320
int32_t __fastcall PARTY_AddPlayerToParty(D2GameStrc* pGame, int16_t nPartyId, D2UnitStrc* pPlayer);
//D2Game.0x6FCB9E80
//1.14d: 0x005407A0
void __fastcall PARTY_LeaveParty(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBA0C0
//1.14d: 0x005408E0
int32_t __fastcall PARTY_GetLivingPartyMemberCountInSameLevel(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBA0E0
//1.14d: 0x005404F0
void __fastcall PARTY_CountLivingUnits(D2GameStrc* pGame, D2UnitStrc* pUnit, void* pLivingUnits);
//D2Game.0x6FCBA100
//1.14d: 0x00540510
void __fastcall PARTY_IteratePartyMembers(D2GameStrc* pGame, int16_t nPartyId, PartyCallbackFunction pCallback, void* pArgs);
//D2Game.0x6FCBA190
//1.14d: 0x005405A0
void __fastcall PARTY_IteratePartyMembersInSameLevel(D2GameStrc* pGame, D2UnitStrc* pUnit, PartyCallbackFunction pCallback, void* pArgs);
//D2Game.0x6FCBA270
//1.14d: 0x00540900
int32_t __fastcall PARTY_ShareGoldDrop(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nGoldValue);
//D2Game.0x6FCBA510
//1.14d: 0x00540650
void __fastcall PARTY_CalculatePickAndDrop(D2UnitStrc* pUnit, int32_t nValue, int32_t* pGoldToPick, int32_t* pGoldToDrop);
//D2Game.0x6FCBA550
//1.14d: 0x005406A0
void __fastcall PARTY_SynchronizeWithClient(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FCBA5F0
//1.14d: 0x00540710
int16_t __fastcall PARTY_GetPartyIdForUnitOwner(D2GameStrc* pGame, D2UnitStrc* pUnit);
