#pragma once

#include <D2BasicTypes.h>

struct D2SeedStrc
{
	union
	{
		struct
		{
			int32_t nLowSeed;					//0x00
			int32_t nHighSeed;					//0x04
		};
		uint64_t lSeed;		//0x00
	};
};

//D2Common.0x6FDA5260 (#10916)
void __stdcall SEED_Return();
//D2Common.0x6FDAEA80 (#10920)
int __fastcall SEED_GetRandomValue(int nValue);
//D2Common.0x6FDAEAB0 (#10912)
void __fastcall SEED_InitSeed(D2SeedStrc* pSeed);
//D2Common.0x6FDAEAC0 (#10913)
void __fastcall SEED_InitLowSeed(D2SeedStrc* pSeed, int nLowSeed);
//D2Common.0x6FDAEAD0 (#10914)
int __fastcall SEED_GetLowSeed(D2SeedStrc* pSeed);
//D2Common.0x6FDAEAE0 (#10921)
void __fastcall SEED_SetSeeds(D2SeedStrc* pSeed, int nLowSeed, int nHighSeed);
//D2Common.0x6FDAEAF0 (#10922)
void __fastcall SEED_GetSeeds(D2SeedStrc* pSeed, int* pLowSeed, int* pHighSeed);
//D2Common.0x6FDAEB00 (#10915)
int __fastcall SEED_GetHighSeed(D2SeedStrc* pSeed);
//D2Common.0x6FD78E30 + Inlined at many places
unsigned long long int __fastcall SEED_RollRandomNumber(D2SeedStrc* pSeed);
//D2Common.0x6FD7D3E0
unsigned int __fastcall SEED_RollLimitedRandomNumber(D2SeedStrc* pSeed, int nMax);
