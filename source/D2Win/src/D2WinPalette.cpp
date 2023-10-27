#include "D2WinPalette.h"

#include <algorithm>
#include <cstdio>

#include "D2CMP.h"
#include "D2Gfx.h"
//#include "D2Hell.h"
#include "Fog.h"

#include "D2WinArchive.h"
#include "D2WinComp.h"


#pragma warning (disable : 28159)


#pragma pack(push, 1)
struct D2PL2FileStrc
{
	uint32_t rgbaPalette[256];			//0x00
	uint8_t shadows[32][256];			//0x400
	uint8_t light[16][256];				//0x2400
	uint8_t gamma[256];					//0x3400
	uint8_t trans[3][256][256];			//0x3500
	uint8_t screen[256][256];			//0x33500
	uint8_t luminance[256][256];		//0x43500
	uint8_t states[128][256];			//0x53500
	uint8_t darkBlend[256][256];		//0x5B500
	uint8_t darkenPalette[256];			//0x6B500
	uint8_t standardColors[13][3];		//0x6B600
	uint8_t standardShifts[13][256];	//0x6B627    
};
#pragma pack(pop)


int32_t dword_6F8BE008;

PALETTEENTRY gScreenPalette_6F8FE290[256][64];

uint32_t dword_6F90E290;
uint32_t dword_6F90E294;
PALETTEENTRY gDarkBlendPalette_6F90E298[256][64];
D2ShadowLightGammaPaletteStrc gShadowsLightGammaPalette_6F91E298;
PALETTEENTRY gDarkenPalette_6F921398[64];
PALETTEENTRY gStandardShiftsPalette_6F921498[13][64];
PALETTEENTRY gTransPalette_6F922198[3][256][64];
uint8_t byte_6F952198[256];
PALETTEENTRY gLuminancePalette_6F952298[256][64];
uint8_t gStandardColorsPalette_6F962298[13][3];
PALETTEENTRY gRGBAPalette_6F9622C0[256];
uint8_t gStatesPalette_6F9626C0[128][256];
uint8_t byte_6F96A6C0[256];
uint32_t dword_6F96A7C0;
uint32_t dword_6F96A7C4;


//D2Win.0x6F8AE550 (#10177)
int32_t __fastcall D2Win_10177(uint8_t nAct)
{
	int32_t nActNo = nAct + 1;
	if (nActNo < 1 || nActNo > 5)
	{
		nActNo = 1;
	}

	char szPL2Filename[260] = {};
	char szDatFilename[260] = {};

	sprintf_s(szDatFilename, "%s\\palette\\act%d\\%s", "DATA\\GLOBAL", nActNo, "pal.dat");
	sprintf_s(szPL2Filename, "%s\\palette\\act%d\\%s", "DATA\\GLOBAL", nActNo, "pal.pl2");

	const int32_t nResult = sub_6F8AE5E0(szDatFilename, szPL2Filename);

	D2GFX_SetPalette(gRGBAPalette_6F9622C0);

	return nResult;
}

//D2Win.0x6F8AE5E0
int32_t __fastcall sub_6F8AE5E0(const char* szDatFileName, const char* szPL2FileName)
{
	D2PaletteTableStrc paletteTable = {};

	uint8_t* pDatFile = (uint8_t*)D2Hell_ARCHIVE_OpenFile_6F8B22F8(D2Win_10038_Return0(), szDatFileName, nullptr, __FILE__, __LINE__);
	for (int32_t i = 0; i < std::size(paletteTable.datFilePalette); ++i)
	{
		paletteTable.datFilePalette[i].peBlue = *pDatFile++;
		paletteTable.datFilePalette[i].peGreen = *pDatFile++;
		paletteTable.datFilePalette[i].peRed = *pDatFile++;
		paletteTable.datFilePalette[i].peFlags = 0;
	}

	D2PL2FileStrc* pPL2File = (D2PL2FileStrc*)D2Hell_ARCHIVE_OpenFile_6F8B22F8(D2Win_10038_Return0(), szPL2FileName, nullptr, __FILE__, __LINE__);
	memcpy(gRGBAPalette_6F9622C0, pPL2File->rgbaPalette, sizeof(gRGBAPalette_6F9622C0));
	memcpy(gTransPalette_6F922198, pPL2File->trans, sizeof(gTransPalette_6F922198));
	memcpy(&gShadowsLightGammaPalette_6F91E298.shadowsPalette, pPL2File->shadows, sizeof(gShadowsLightGammaPalette_6F91E298.shadowsPalette));
	memcpy(gLuminancePalette_6F952298, pPL2File->luminance, sizeof(gLuminancePalette_6F952298));
	memcpy(gScreenPalette_6F8FE290, pPL2File->screen, sizeof(gScreenPalette_6F8FE290));
	memcpy(&gShadowsLightGammaPalette_6F91E298.lightPalette, pPL2File->light, sizeof(gShadowsLightGammaPalette_6F91E298.lightPalette));
	memcpy(&gShadowsLightGammaPalette_6F91E298.gammaPalette, pPL2File->gamma, sizeof(gShadowsLightGammaPalette_6F91E298.gammaPalette));
	memcpy(gStatesPalette_6F9626C0, pPL2File->states, sizeof(gStatesPalette_6F9626C0));
	memcpy(gDarkBlendPalette_6F90E298, pPL2File->darkBlend, sizeof(gDarkBlendPalette_6F90E298));
	memcpy(gDarkenPalette_6F921398, pPL2File->darkenPalette, sizeof(gDarkenPalette_6F921398));
	memcpy(gStandardColorsPalette_6F962298, pPL2File->standardColors, sizeof(gStandardColorsPalette_6F962298));
	memcpy(gStandardShiftsPalette_6F921498, pPL2File->standardShifts, sizeof(gStandardShiftsPalette_6F921498));

	for (int32_t i = 0; i < std::size(gRGBAPalette_6F9622C0); ++i)
	{
		gRGBAPalette_6F9622C0[i].peFlags = 5;
	}

	for (int32_t i = 0; i < std::size(gRGBAPalette_6F9622C0); ++i)
	{
		const uint8_t red = std::min(170 * gRGBAPalette_6F9622C0[i].peRed / 100, 255);
		const uint8_t green = std::min(170 * gRGBAPalette_6F9622C0[i].peGreen / 100, 255);
		const uint8_t blue = std::min(170 * gRGBAPalette_6F9622C0[i].peBlue / 100, 255);

		byte_6F96A6C0[i] = D2CMP_10004_GetNearestPaletteIndex(gRGBAPalette_6F9622C0, 0x100u, red, green, blue);
		byte_6F952198[i] = D2CMP_10004_GetNearestPaletteIndex(gRGBAPalette_6F9622C0, 0x100u, gRGBAPalette_6F9622C0[i].peRed, 0, 0);
	}

	for (int32_t i = 0; i < std::size(paletteTable.transPalettes); ++i)
	{
		paletteTable.transPalettes[i] = &gTransPalette_6F922198[i];
	}

	for (int32_t i = 0; i < std::size(paletteTable.shadowLightGammaPalettes); ++i)
	{
		paletteTable.shadowLightGammaPalettes[i] = &gShadowsLightGammaPalette_6F91E298.palettes[i];
	}

	for (int32_t i = 0; i < std::size(paletteTable.standardShiftPalettes); ++i)
	{
		paletteTable.standardShiftPalettes[i] = &gStandardShiftsPalette_6F921498[i];
	}

	paletteTable.pScreenPalette = gScreenPalette_6F8FE290;
	paletteTable.pLuminancePalette = gLuminancePalette_6F952298;
	paletteTable.pDarkBlendPalette = gDarkBlendPalette_6F90E298;
	paletteTable.pDarkenPalette = gDarkenPalette_6F921398;
	paletteTable.pStandardColorsPalette = gStandardColorsPalette_6F962298;
	paletteTable.unk0x118 = byte_6F96A6C0;
	paletteTable.unk0x11C = byte_6F952198;
	D2GFX_SetPaletteTables(&paletteTable);
	D2GFX_SetPalette(gRGBAPalette_6F9622C0);

	D2_FREE(pDatFile);
	D2_FREE(pPL2File);

	return 1;
}

//D2Win.0x6F8AE930 (#10175)
uint8_t* __fastcall D2Win_10175_GetPL2Shift(int32_t nIndex)
{
	return gStatesPalette_6F9626C0[nIndex];
}

//D2Win.0x6F8AE940 (#10179)
void __stdcall D2Win_10179()
{
	if (!dword_6F8BE008)
	{
		return;
	}

	if (dword_6F96A7C0)
	{
		if (GetTickCount() - dword_6F96A7C4 <= 2000)
		{
			return;
		}

		dword_6F96A7C4 = GetTickCount();

		--dword_6F90E290;
		if (dword_6F90E290 > 0)
		{
			D2GFX_SetPalette(gRGBAPalette_6F9622C0);
			return;
		}

		dword_6F96A7C0 = 0;
	}

	dword_6F8BE008 = 0;
	D2GFX_SetPalette(gRGBAPalette_6F9622C0);
}

//D2Win.0x6F8AE9A0 (#10176)
void __fastcall D2Win_10176(int32_t a1)
{
	if (a1)
	{
		dword_6F96A7C0 = 1;
		dword_6F96A7C4 = GetTickCount();
		dword_6F90E290 = 2;
		dword_6F8BE008 = 1;
		D2GFX_SetPalette(gRGBAPalette_6F9622C0);
	}
	else
	{
		dword_6F96A7C0 = 0;
		dword_6F8BE008 = 0;
		D2GFX_SetPalette(gRGBAPalette_6F9622C0);
	}
}

//D2Win.0x6F8AE9F0 (#10028)
int32_t __stdcall D2Win_10028(const char* pszPalDatFilename, const char* pszPalPL2Filename)
{
	if (pszPalDatFilename)
	{
		sub_6F8AE5E0(pszPalDatFilename, pszPalPL2Filename);
		sub_6F8A4C10();
		return 1;
	}

	char szPalDatFilename[260] = {};
	char szPalPL2Filename[260] = {};

	wsprintfA(szPalDatFilename, "%s\\%s", "DATA\\GLOBAL", "palette\\act1\\pal.dat");
	wsprintfA(szPalPL2Filename, "%s\\%s", "DATA\\GLOBAL", "palette\\act1\\pal.pl2");

	sub_6F8AE5E0(szPalDatFilename, szPalPL2Filename);
	sub_6F8A4C10();

	return 1;
}

//D2Win.0x6F8AEA80 (#10034)
uint8_t __stdcall D2Win_10034_MixRGB(uint8_t nRed, uint8_t nGreen, uint8_t nBlue)
{
	return D2CMP_10004_GetNearestPaletteIndex(gRGBAPalette_6F9622C0, 0x100u, nRed, nGreen, nBlue);
}

//D2Win.0x6F8AEAC0 (#10178)
void __fastcall D2Win_10178(D2ShadowLightGammaPaletteStrc* pBuffer)
{
	memcpy(pBuffer, &gShadowsLightGammaPalette_6F91E298, sizeof(gShadowsLightGammaPalette_6F91E298));
}
