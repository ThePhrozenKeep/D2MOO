#pragma once

#include "CommonDefinitions.h"

struct D2DrlgLevelStrc;

#pragma pack(1)


#pragma pack()


//Inlined in D2Common.0x6FD7D430
void __fastcall DRLGOUTDESR_PlaceDesertTransitionToTown(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7D430
void __fastcall DRLGOUTDESR_InitAct2OutdoorLevel(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7D870
void __fastcall DRLGOUTDESR_PlacePresetVariants(D2DrlgLevelStrc* pLevel, const int* pLevelPrestIds, unsigned int nVariants, BOOL bIterateFiles);
//D2Common.6FD7D950
void __fastcall DRLGOUTDESR_PlaceCliffs(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7D9B0
void __fastcall DRLGOUTDESR_PlaceBorders(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7D9F0
void __fastcall DRLGOUTDESR_AddExits(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7DA60
void __fastcall DRLGOUTDESR_PlaceFillsInFarOasis(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7DAC0
void __fastcall DRLGOUTDESR_PlaceRuinsInLostCity(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7DB00
void __fastcall DRLGOUTDESR_PlaceFillsInLostCity(D2DrlgLevelStrc* pLevel);
//D2Common.6FD7DB70
void __fastcall DRLGOUTDESR_PlaceTombEntriesInCanyon(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7DBC0
void __fastcall DRLGOUTDESR_PlaceFillsInCanyon(D2DrlgLevelStrc* pLevel);
