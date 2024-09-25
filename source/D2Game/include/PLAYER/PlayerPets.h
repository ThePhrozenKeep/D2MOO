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


//D2Game.0x6FC7CAF0
void __fastcall PLAYERPETS_AllocPetList(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7CD10
void __fastcall D2GAME_KillPlayerPets_6FC7CD10(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7CDC0
void __fastcall sub_6FC7CDC0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2PetListStrc* pPetList, int32_t a4, int32_t a5);
//D2Game.0x6FC7CF20
void __fastcall PLAYERPETS_PlayerIterate_SynchronizeWithClient(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg);
//D2Game.0x6FC7CF50
void __fastcall sub_6FC7CF50(D2GameStrc* pGame, int32_t nPetGUID);
//D2Game.0x6FC7D060
void __fastcall PLAYERPETS_FreePetsFromPlayerData(D2GameStrc* pGame, D2UnitStrc* pUnit);
//D2Game.0x6FC7D150
void __fastcall sub_6FC7D150(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPetType, int32_t nBaseMax);
//D2Game.0x6FC7D260
void __fastcall sub_6FC7D260(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPetGUID, int32_t a4);
//D2Game.0x6FC7D390
void __fastcall PLAYERPETS_RemovePetFromList(D2GameStrc* pGame, D2PetListStrc* pPetList, int32_t nUnitGUID, int32_t a4);
//D2Game.0x6FC7D470
void __fastcall sub_6FC7D470(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet);
//D2Game.0x6FC7D5F0
int32_t __fastcall sub_6FC7D5F0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet);
//D2Game.0x6FC7D720
void __fastcall PLAYERPETS_PlayerIterate_UpdateClient(D2GameStrc* pGame, D2UnitStrc* pPlayer, void* pArg);
//D2Game.0x6FC7D7A0
void __fastcall sub_6FC7D7A0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet, int32_t nPetType, int32_t nPetMax);
//D2Game.0x6FC7D9D0
int32_t __fastcall sub_6FC7D9D0(D2UnitStrc* pPlayer, int32_t nPetType);
//D2Game.0x6FC7DA40
int32_t __fastcall sub_6FC7DA40(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2PetListStrc* pPetList, D2UnitStrc* pPet, D2PetInfoStrc* pPetInfo);
//D2Game.0x6FC7DB90
void __fastcall PLAYERPETS_UpdatePetInfo(D2UnitStrc* pUnit, int32_t nPetType, int32_t nUnitGUID, D2PetInfoStrc* pPetInfo);
//D2Game.0x6FC7DBF0
void __fastcall sub_6FC7DBF0(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet, int32_t nPetType, D2PetInfoStrc* pPetInfo);
//D2Game.0x6FC7DD10
int32_t __fastcall PLAYERPETS_GetPetTypeFromPetGUID(D2UnitStrc* pPlayer, int32_t nPetGUID);
//D2Game.0x6FC7DD90
int32_t __fastcall sub_6FC7DD90(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pPet);
//D2Game.0x6FC7DEB0
D2PetInfoStrc* __fastcall PLAYERPETS_GetPetInfoFromPetGUID(D2UnitStrc* pUnit, int32_t nUnitGUID);
//D2Game.0x6FC7DF40
void __fastcall sub_6FC7DF40(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7E2E0
D2PetListStrc* __fastcall PLAYERPETS_GetPetListFromPetType(D2PlayerPetStrc* pPlayerPets, int32_t nPetType);
//D2Game.0x6FC7E310
void __fastcall sub_6FC7E310(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t a3, int32_t a4);
//D2Game.0x6FC7E550
void __fastcall sub_6FC7E550(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2PetDataStrc* pPetData, D2ActiveRoomStrc* pRoom, int32_t a5, int32_t a6);
//D2Game.0x6FC7E640
void __fastcall sub_6FC7E640(D2GameStrc* pGame, D2UnitStrc* pPlayer);
//D2Game.0x6FC7E7C0
void __fastcall PLAYERPETS_IteratePets(D2GameStrc* pGame, D2UnitStrc* pPlayer, void(__fastcall* pCallback)(D2GameStrc*, D2UnitStrc*, D2UnitStrc*, void*), void* a4);
//D2Game.0x6FC7E8B0
D2UnitStrc* __fastcall sub_6FC7E8B0(D2GameStrc* pGame, D2UnitStrc* pPlayer, int32_t nPetType, int32_t a4);
//D2Game.0x6FC7E930
int32_t __fastcall PLAYERPETS_GetTotalPetCount(D2UnitStrc* pPlayer);
//D2Game.0x6FC7E970
void __fastcall D2GAME_PLAYERPETS_Last_6FC7E970(D2ClientStrc* pClient, D2UnitStrc* pPlayer);
