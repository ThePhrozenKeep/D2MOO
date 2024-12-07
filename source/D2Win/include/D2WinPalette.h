#pragma once

#include <cstdint>

#include <Windows.h>


#pragma pack(push, 1)
struct D2ShadowLightGammaPaletteStrc
{
	union
	{
		struct
		{
			PALETTEENTRY shadowsPalette[32][64];
			PALETTEENTRY lightPalette[16][64];
			PALETTEENTRY gammaPalette[64];
		};

		PALETTEENTRY palettes[49][64];
	};
};
#pragma pack(pop)


//D2Win.0x6F8AE550
int32_t __fastcall PALETTTE_LoadForAct(uint8_t nAct);
//D2Win.0x6F8AE5E0
int32_t __fastcall sub_6F8AE5E0(const char* szDatFileName, const char* szPL2FileName);
//D2Win.0x6F8AE930
uint8_t*__fastcall D2Win_10175_GetPL2Shift(int32_t nIndex);
//D2Win.0x6F8AE940
void __stdcall D2Win_10179();
//D2Win.0x6F8AE9A0
void __fastcall D2Win_10176(int32_t a1);
//D2Win.0x6F8AE9F0
int32_t __stdcall D2Win_10028(const char* pszPalDatFilename, const char* pszPalPL2Filename);
//D2Win.0x6F8AEA80
uint8_t __stdcall D2Win_10034_MixRGB(uint8_t nRed, uint8_t nGreen, uint8_t nBlue);
//D2Win.0x6F8AEAC0
void __fastcall D2Win_10178(D2ShadowLightGammaPaletteStrc* pBuffer);
