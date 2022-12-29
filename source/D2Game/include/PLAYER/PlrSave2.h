#pragma once

#include "D2Structs.h"
#include <Units/Units.h>


//D2Game.0x6FC8CCD0
void __fastcall PLRSAVE2_WriteMercData(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2SaveHeaderStrc* pMercData);
//D2Game.0x6FC8CD80
int32_t __fastcall PLRSAVE2_WriteSaveHeader(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, const uint8_t* pEnd);
//D2Game.0x6FC8D390
int32_t __fastcall PLRSAVE2_WriteWaypointData(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t** ppSection, const uint8_t* pEnd);
//D2Game.0x6FC8D440
int32_t __fastcall PLRSAVE2_WritePlrIntroSection(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t** ppSection, const uint8_t* pEnd);
//D2Game.0x6FC8D4F0
int32_t __fastcall PLRSAVE2_WritePlayerStats(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, const uint8_t* pEnd);
//D2Game.0x6FC8D710
int32_t __fastcall PLRSAVE2_WriteCorpsesSection(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, const uint8_t* pEnd, int32_t a5);
//D2Game.0x6FC8D880
int32_t __fastcall PLRSAVE2_WriteIronGolemSection(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, const uint8_t* pEnd, int32_t a5);
//D2Game.0x6FC8D940
int32_t __fastcall PLRSAVE2_CreateSaveFile(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t* pData, uint32_t* pSize, uint32_t nMaxSize, int32_t a6, int32_t a7);
//D2Game.0x6FC8DC20
int32_t __fastcall PLRSAVE2_ReadPlayerFlags(D2GameStrc* pGame, uint32_t dwFlags);
//D2Game.0x6FC8DD00
int32_t __fastcall PLRSAVE2_ReadSaveHeader(D2GameStrc* pGame, D2ClientStrc* pClient, uint8_t** ppSection, uint8_t* pEnd, D2UnitStrc** ppPlayer);
//D2Game.0x6FC8E070
int32_t __fastcall PLRSAVE2_LoadWaypointData(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t** ppSection, uint8_t* pEnd, int32_t nUnused);
//D2Game.0x6FC8E0F0
int32_t __fastcall PLRSAVE2_LoadStatsEx(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t** ppSection, uint8_t* pEnd, uint32_t nVersion);
//D2Game.0x6FC8E250
int32_t __fastcall PLRSAVE2_LoadStats(D2GameStrc* pGame, D2UnitStrc* pUnit, uint8_t** ppSection, uint8_t* pEnd, uint32_t nVersion, int32_t nStats);
//D2Game.0x6FC8E330
int32_t __fastcall PLRSAVE2_LoadSkills(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, uint8_t* pEnd, int32_t nUnused, int32_t nSkills);
//D2Game.0x6FC8E420
int32_t __fastcall PLRSAVE2_LoadCorpses(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, uint8_t* pEnd, uint32_t nVersion, int32_t* a6);
//D2Game.0x6FC8E670
D2UnitStrc* __fastcall PLRSAVE2_LoadMercData(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2MercSaveDataStrc* pData, uint32_t dwVersion);
//D2Game.0x6FC8E850
int32_t __fastcall PLRSAVE2_ReadPetSection(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc* pMerc, uint8_t** ppSection, uint8_t* pEnd, uint32_t dwVersion, int32_t* a6);
//D2Game.0x6FC8E920
int32_t __fastcall PLRSAVE2_CreateItem(D2GameStrc* pGame, D2UnitStrc* pPlayer, D2UnitStrc** ppItem, uint8_t** ppSection, uint8_t* pEnd, uint32_t dwVersion);
//D2Game.0x6FC8EAB0
int32_t __fastcall PLRSAVE2_ReadIronGolemSection(D2GameStrc* pGame, D2UnitStrc* pPlayer, uint8_t** ppSection, uint8_t* pEnd, uint32_t dwVersion);
//D2Game.0x6FC8EBA0
void __fastcall PLRSAVE2_InitializeStats(D2GameStrc* pGame, D2UnitStrc* pUnit, int32_t nHitpoints, int32_t nMana);
//D2Game.0x6FC8EF20
int32_t __fastcall PLRSAVE2_ProcessSaveFile(D2GameStrc* pGame, D2ClientStrc* pClient, uint8_t* pSaveFile, uint32_t nSize, D2UnitStrc** ppPlayer, int32_t nUnused1, int32_t nUnused2, int32_t nUnused3);
//D2Game.0x6FC8F3A0
D2CharStatsTxt* __fastcall PLRSAVE2_GetCharStatsTxtRecord(int32_t nClass);
