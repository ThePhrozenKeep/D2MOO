#pragma once

#include <Units/Units.h>


enum D2PetTypes
{
	PETTYPE_NONE,
	PETTYPE_SINGLE,
	PETTYPE_VALKYRIE,
	PETTYPE_GOLEM,
	PETTYPE_SKELETON,
	PETTYPE_SKELETONMAGE,
	PETTYPE_REVIVE,
	PETTYPE_HIREABLE,
	PETTYPE_DOPPLEZON,
	PETTYPE_INVIS,
	PETTYPE_RAVEN,
	PETTYPE_SPIRITWOLF,
	PETTYPE_FENRIS,
	PETTYPE_TOTEM,
	PETTYPE_VINE,
	PETTYPE_GRIZZLY,
	PETTYPE_SHADOWWARRIOR,
	PETTYPE_ASSASSINTRAP,
	PETTYPE_PETTRAP,
	PETTYPE_HYDRA,
};

enum D2PetFlags
{
	PETFLAG_DEAD = 0x1,
};


//D2Game.0x6FC7CAF0
//1.14d: 0x00575AF0
void __fastcall PLAYERPETS_AllocPetList(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7CD10
//1.14d: 0x00575BC0
void __fastcall D2GAME_KillPlayerPets_6FC7CD10(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7CDC0
//1.14d: 0x00574570
void __fastcall PLAYERPETS_ClearPetList(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2PetListStrc* pPetList, int32_t bIsHirelingList, int32_t bKeepHireling);
//D2Game.0x6FC7CF20
//1.14d: 0x00574410
void __fastcall PLAYERPETS_PlayerIterate_SynchronizeWithClient(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg);
//D2Game.0x6FC7CF50
//1.14d: 0x00574450
void __fastcall PLAYERPETS_KillPet(D2GameStrc* pGame, int32_t nPetGUID);
//D2Game.0x6FC7D060
//1.14d: 0x005746D0
void __fastcall PLAYERPETS_FreePetsFromPlayerData(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC7D150
//1.14d: 0x00575850
void __fastcall PLAYERPETS_SetPetLimit(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPetType, int32_t nBaseMax);
//D2Game.0x6FC7D260
//1.14d: 0x005750E0
void __fastcall PLAYERPETS_RemovePet(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPetGUID, int32_t bKillPet);
//D2Game.0x6FC7D390
//1.14d: 0x00574850
void __fastcall PLAYERPETS_RemovePetFromList(D2GameStrc* pGame, D2PetListStrc* pPetList, int32_t nUnitGUID, int32_t bKillPet);
//D2Game.0x6FC7D470
//1.14d: 0x005751A0
void __fastcall PLAYERPETS_HandlePetDeath(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet);
//D2Game.0x6FC7D5F0
//1.14d: 0x005752B0
int32_t __fastcall PLAYERPETS_HandlePetDeactivation(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet);
//D2Game.0x6FC7D720
//1.14d: 0x00574930
void __fastcall PLAYERPETS_PlayerIterate_UpdateClient(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg);
//D2Game.0x6FC7D7A0
//1.14d: 0x00575D90
void __fastcall PLAYERPETS_AddSummonedPet(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet, int32_t nPetType, int32_t nPetMax);
//D2Game.0x6FC7D9D0
//1.14d: 0x005747B0
int32_t __fastcall PLAYERPETS_GetFirstPetGUID(D2UnitStrc* pPlayer, int32_t nPetType);
//D2Game.0x6FC7DA40
//1.14d: 0x00575C70
int32_t __fastcall PLAYERPETS_AppendPet(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2PetListStrc* pPetList, D2UnitStrc* pPet, D2PetInfoStrc* pPetInfo);
//D2Game.0x6FC7DB90
//1.14d: 0x005749B0
void __fastcall PLAYERPETS_UpdatePetInfo(D2UnitStrc* pUnit, int32_t nPetType, int32_t nUnitGUID, D2PetInfoStrc* pPetInfo);
//D2Game.0x6FC7DBF0
//1.14d: 0x00575E90
void __fastcall PLAYERPETS_AddPetWithInfo(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet, int32_t nPetType, D2PetInfoStrc* pPetInfo);
//D2Game.0x6FC7DD10
//1.14d: 0x00574A20
int32_t __fastcall PLAYERPETS_GetPetTypeFromPetGUID(D2UnitStrc* pPlayer, int32_t nPetGUID);
//D2Game.0x6FC7DD90
//1.14d: 0x00574AB0
int32_t __fastcall PLAYERPETS_MarkPetAlive(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet);
//D2Game.0x6FC7DEB0
//1.14d: 0x00574BD0
D2PetInfoStrc* __fastcall PLAYERPETS_GetPetInfoFromPetGUID(D2UnitStrc* pUnit, int32_t nUnitGUID);
//D2Game.0x6FC7DF40
//1.14d: 0x00575900
void __fastcall PLAYERPETS_UpdatePetLimits(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7E2E0
//1.14d: 0x00574540
D2PetListStrc* __fastcall PLAYERPETS_GetPetListFromPetType(D2PlayerPetStrc* pPlayerPets, int32_t nPetType);
//D2Game.0x6FC7E310
//1.14d: 0x005754B0
void __fastcall PLAYERPETS_UpdatePetsForOwnerMove(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nX, int32_t nY);
//D2Game.0x6FC7E550
//1.14d: 0x00574CC0
void __fastcall PLAYERPETS_RelocatePet(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2PetDataStrc* pPetData, D2ActiveRoomStrc* pRoom, int32_t nX, int32_t nY);
//D2Game.0x6FC7E640
//1.14d: 0x00575630
void __fastcall PLAYERPETS_UpdatePetStates(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7E7C0
//1.14d: 0x00574DE0
void __fastcall PLAYERPETS_IteratePets(D2GameStrc* pGame, D2UnitStrc* pPlayer, void(__fastcall* pCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*), void* pArg);
//D2Game.0x6FC7E8B0
D2UnitStrc* __fastcall PLAYERPETS_GetFirstPetByType(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPetType, int32_t bIncludeDead);
//D2Game.0x6FC7E930
//1.14d: 0x00574F40
int32_t __fastcall PLAYERPETS_GetTotalPetCount(D2UnitStrc* pPlayer);
//D2Game.0x6FC7E970
//1.14d: 0x00574F80
void __fastcall PLAYERPETS_SendPetsToClient(D2ClientStrc* pClient, D2UnitStrc* pPlayer);
