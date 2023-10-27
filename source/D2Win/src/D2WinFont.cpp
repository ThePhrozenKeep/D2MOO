#include "D2WinFont.h"

#include <D2Math.h>

#include <D2CMP.h>
#include <Archive.h>
#include <D2Lang.h>
#include <D2Unicode.h>
#include <D2StrTable.h>
#include <Fog.h>

#include <DisplayType.h>
#include <D2Gfx.h>
#include <Texture.h>
#include <Window.h>

#include "D2WinArchive.h"
#include "D2WinPalette.h"


#pragma warning (disable : 28159)


constexpr const char* gszFontNames_6F8BA534[NUM_FONTS] =
{
	"Font8",
	"Font16",
	"Font30",
	"Font42",
	"FontFormal10",
	"FontFormal12",
	"Font6",
	"Font24",
	"FontFormal11",
	"FontExocet10",
	"FontRidiculous",
	"FontExocet8",
	"ReallyTheLastSucker",
	"FontInGameChat"
};

D2FontCacheStrc stru_6F8FD8C0[14];
char byte_6F8FD9D8[20];
D2CellFileStrc* ghCellfileFontMonsterIndicators_6F8FD9EC;
Unicode gszFramedText_Buffer_6F8FD9F0[1024];
Font gnFontSize_6F8BDB24;
int dword_6F8BDB28;
int dword_6F8BDB2C;
int dword_6F8BDB30;
int gnFramedText_X_6F8FE1F0;
int gnFramedText_Y_6F8FE1F4;
int gnFramedText_TextColor_6F8FE1F8;
int gnFramedText_Align_6F8FE1FC;
int gnFramedText_RectangleColor_6F8FE200;
Unicode word_6F8FE204[2];
Unicode word_6F8FE208[2];
D2CharStrc* (__fastcall* dword_6F8FE20C)(Unicode);
D2FontCacheStrc* gpFontCache;
DWORD gdwLastShiftedTickCount_6F8FE214;
BYTE byte_6F8FE218;
bool byte_6F8FE21A;


//D2Win.0x6F8A9B00
void __stdcall sub_6F8A9B00()
{
	memset(gszFramedText_Buffer_6F8FD9F0, 0, sizeof(gszFramedText_Buffer_6F8FD9F0));
}

//D2Win.0x6F8A9B20
void __stdcall sub_6F8A9B20()
{
	memset(word_6F8FE208, 0, sizeof(word_6F8FE208));
}

//D2Win.0x6F8A9B40
void __stdcall sub_6F8A9B40()
{
	memset(word_6F8FE204, 0, sizeof(word_6F8FE204));
}

//D2Win.0x6F8A9B60 (#10115)
void __fastcall D2Win_10115_FONT()
{
	byte_6F8FE21A = 0;
	dword_6F8BDB28 = 16;
	dword_6F8BDB2C = 17;
	dword_6F8BDB30 = 3;

	switch (STRTABLE_GetLanguage())
	{
	case LANGUAGE_ENGLISH:
	case LANGUAGE_SPANISH:
	case LANGUAGE_GERMAN:
	case LANGUAGE_FRENCH:
	case LANGUAGE_PORTUGUESE:
	case LANGUAGE_ITALIAN:
		strcpy_s(byte_6F8FD9D8, "Latin\\");
		dword_6F8BDB2C = 16;
		dword_6F8FE20C = sub_6F8AA100;
		break;

	case LANGUAGE_JAPANESE:
		dword_6F8BDB28 = 14;
		dword_6F8BDB2C = 15;
		dword_6F8BDB30 = 5;
		strcpy_s(byte_6F8FD9D8, "JPN\\");
		dword_6F8FE20C = sub_6F8AA140;
		break;

	case LANGUAGE_KOREAN:
		dword_6F8BDB30 = 5;
		strcpy_s(byte_6F8FD9D8, "KOR\\");
		dword_6F8FE20C = sub_6F8AA140;
		break;

	case LANGUAGE_CHINESESIN:
	case LANGUAGE_CHINESETWN:
		dword_6F8BDB30 = 2;
		dword_6F8BDB28 = 14;
		dword_6F8BDB2C = 14;
		strcpy_s(byte_6F8FD9D8, "CHI\\");
		dword_6F8FE20C = sub_6F8AA140;
		break;

	case LANGUAGE_POLISH:
		dword_6F8BDB28 = 13;
		dword_6F8BDB2C = 13;
		strcpy_s(byte_6F8FD9D8, "LATIN2\\");
		dword_6F8FE20C = sub_6F8AA140;
		break;

	case LANGUAGE_RUSSIAN:
		strcpy_s(byte_6F8FD9D8, "CYR\\");
		dword_6F8FE20C = sub_6F8AA140;
		break;

	case LANGUAGE_ENGLISHKOR:
		byte_6F8FE21A = 1;
		strcpy_s(byte_6F8FD9D8, "LATIN\\");
		dword_6F8FE20C = sub_6F8AA100;
		break;

	default:
		strcpy_s(byte_6F8FD9D8, "Latin\\");
		dword_6F8FE20C = sub_6F8AA140;
		break;
	}

	if (!stru_6F8FD8C0[1].pCellFile)
	{
		char szFile[80] = {};

		D2Win_FONT_6F8A9DC0(1, szFile, 79);
		stru_6F8FD8C0[1].pCellFile = ARCHIVE_LoadCellFile(szFile, 0);
		sub_6F8A9E90(1);
	}

	if (!ghCellfileFontMonsterIndicators_6F8FD9EC)
	{
		char szPath[260] = {};

		wsprintfA(szPath, "%s\\UI\\Font\\MonsterIndicators", "DATA\\GLOBAL");
		ghCellfileFontMonsterIndicators_6F8FD9EC = ARCHIVE_LoadCellFile(szPath, 0);
	}

	memset(gszFramedText_Buffer_6F8FD9F0, 0, sizeof(gszFramedText_Buffer_6F8FD9F0));

	gpFontCache = &stru_6F8FD8C0[1];

	Unicode::toUnicode(word_6F8FE208, "c", 2);
	Unicode::toUnicode(word_6F8FE204, "m", 2);
}

//D2Win.0x6F8A9DC0
void __fastcall D2Win_FONT_6F8A9DC0(int nFont, char* pBuffer, int nLength)
{
	char szFileName[260] = {};

	wsprintfA(szFileName, "%s\\%s\\", "DATA\\LOCAL", "Font");

	D2_ASSERT(nLength > (int)strlen(szFileName) + 20);

	if (byte_6F8FE21A)
	{
		if (nFont == D2FONT_FONTINGAMECHAT)
		{
			wsprintfA(pBuffer, "%s%s%s", szFileName, "KOR\\", gszFontNames_6F8BA534[D2FONT_FONTINGAMECHAT]);
		}
		else
		{
			wsprintfA(pBuffer, "%s%s%s", szFileName, byte_6F8FD9D8, gszFontNames_6F8BA534[nFont]);
		}
	}
	else
	{
		wsprintfA(pBuffer, "%s%s%s", szFileName, byte_6F8FD9D8, gszFontNames_6F8BA534[nFont]);
	}
}

//D2Win.0x6F8A9E90
void __fastcall sub_6F8A9E90(int a1)
{
	char szFilename[80] = {};
	char szPath[260] = {};

	wsprintfA(szPath, "%s\\%s\\", "DATA\\LOCAL", "Font");

	D2_ASSERT(strlen(szFilename) + 20 < 79);

	if (byte_6F8FE21A && a1 == 13)
	{
		wsprintfA(szFilename, "%s%s%s.tbl", szPath, "KOR\\", "FontInGameChat");
	}
	else
	{
		wsprintfA(szFilename, "%s%s%s.tbl", szPath, byte_6F8FD9D8, gszFontNames_6F8BA534[a1]);
	}

	if (stru_6F8FD8C0[a1].pFontInfo[0])
	{
		D2_FREE(stru_6F8FD8C0[a1].pFontInfo[0]);
	}

	D2FontStrc* pFont = (D2FontStrc*)ARCHIVE_READ_FILE_TO_ALLOC_BUFFER(D2Win_GetMemPool(), szFilename, nullptr);	stru_6F8FD8C0[a1].pFontInfo[0] = pFont;
	stru_6F8FD8C0[a1].pFontInfo[1] = pFont;
	stru_6F8FD8C0[a1].pCharInfo = pFont->pChars;
}

//D2Win.0x6F8A9F90 (#10116)
void __stdcall D2Win_10116()
{
	for (int i = 0; i < NUM_FONTS; ++i)
	{
		D2FontCacheStrc* pFont = &stru_6F8FD8C0[i];

		if (pFont->pCellFile)
		{
			ARCHIVE_FreeCellFile(pFont->pCellFile);
			pFont->pCellFile = nullptr;
		}

		if (pFont->pFontInfo[0])
		{
			D2_FREE(pFont->pFontInfo[0]);
			pFont->pFontInfo[0] = nullptr;
		}
	}

	ARCHIVE_FreeCellFile(ghCellfileFontMonsterIndicators_6F8FD9EC);
	ghCellfileFontMonsterIndicators_6F8FD9EC = nullptr;
}

//D2Win.0x6F8A9FF0 (#10127)
Font __fastcall D2Win_10127_SetFont(Font nFontSize)
{
	const Font nOldFont = gnFontSize_6F8BDB24;

	const DWORD dwShiftedTickCount = GetTickCount() >> 10;

	if (gnFontSize_6F8BDB24 == nFontSize)
	{
		stru_6F8FD8C0[nFontSize].dwShiftedTickCount = dwShiftedTickCount;
	}
	else
	{
		gnFontSize_6F8BDB24 = nFontSize;

		if (!stru_6F8FD8C0[nFontSize].pCellFile)
		{
			char szFile[80] = {};

			D2Win_FONT_6F8A9DC0(nFontSize, szFile, 79);
			stru_6F8FD8C0[nFontSize].pCellFile = ARCHIVE_LoadCellFile(szFile, 0);
			sub_6F8A9E90(nFontSize);
			stru_6F8FD8C0[nFontSize].dwShiftedTickCount = dwShiftedTickCount;
		}

		if (dwShiftedTickCount - gdwLastShiftedTickCount_6F8FE214 > 600)
		{
			gdwLastShiftedTickCount_6F8FE214 = dwShiftedTickCount;

			for (int i = 0; i < NUM_FONTS; ++i)
			{
				D2FontCacheStrc* pFont = &stru_6F8FD8C0[i];
				if (i != nFontSize && dwShiftedTickCount - pFont->dwShiftedTickCount > 240 && pFont->pCellFile)
				{
					ARCHIVE_FreeCellFile(pFont->pCellFile);
					pFont->pCellFile = nullptr;
				}
			}
		}

		gpFontCache = &stru_6F8FD8C0[nFontSize];

		if (byte_6F8FE21A)
		{
			if (nFontSize != D2FONT_FONTINGAMECHAT)
			{
				dword_6F8FE20C = sub_6F8AA100;
			}
			else
			{
				dword_6F8FE20C = sub_6F8AA140;
			}
		}
	}

	return nOldFont;
}

//D2Win.0x6F8AA100
D2CharStrc* __fastcall sub_6F8AA100(Unicode wszChar)
{
	if (wszChar <= 0xFF)
	{
		return &stru_6F8FD8C0[gnFontSize_6F8BDB24].pCharInfo[wszChar];
	}
	else
	{
		return stru_6F8FD8C0[gnFontSize_6F8BDB24].pCharInfo;
	}
}

//D2Win.0x6F8AA140
D2CharStrc* __fastcall sub_6F8AA140(Unicode wszChar)
{
	D2FontCacheStrc* pFontCache = &stru_6F8FD8C0[gnFontSize_6F8BDB24];

	int nMin = 0;
	int nMax = pFontCache->pFontInfo[1]->unk0x08 - 1;

	for (int i = 0; i <= 1000 && nMin <= nMax; ++i)
	{
		int nMidpoint = (nMax + nMin) / 2;
		if (wszChar == pFontCache->pCharInfo[nMidpoint].wChar)
		{
			return &pFontCache->pCharInfo[nMidpoint];
		}

		if (wszChar >= pFontCache->pCharInfo[nMidpoint].wChar)
		{
			nMin = nMidpoint + 1;
		}
		else
		{
			nMax = nMidpoint - 1;
		}
	}

	return pFontCache->pCharInfo + 31;
}

//D2Win.0x6F8AA1D0 (#10123)
int __fastcall D2Win_10123(const Unicode* wszText, int nTextLength)
{
	// TODO: Names
	const Unicode* v4 = wszText;
	int v2 = 0;
	int v3 = 0;
	int v5 = 3;
	while (*v4)
	{
		if (v2 >= nTextLength)
		{
			return v3;
		}

		if (*v4 != '\n')
		{
			if (v5 >= nTextLength || *v4 >= 0x100u || *v4 != 0xFF || v4[1] != 'c' || v4[2] < '0' || v4[2] >= '7')
			{
				v3 += dword_6F8FE20C(*v4)->nWidth;
			}
			else
			{
				v4 += 3;
				v2 += 3;
				v5 += 3;
			}
		}

		++v4;
		++v2;
		++v5;
	}

	return v3;
}

//D2Win.0x6F8AA260 (#10122)
int __fastcall D2Win_10122(const Unicode* wszText, int nTextLength)
{
	// TODO: Names
	int v2 = 0;
	int v3 = 0;
	for (const Unicode* i = wszText; *i; ++i)
	{
		if (v2 >= nTextLength)
		{
			break;
		}

		if (*i != '\n')
		{
			v3 += dword_6F8FE20C(*i)->nWidth;
		}

		++v2;
	}

	return v3;
}

//D2Win.0x6F8AA2A0 (#10121)
int __fastcall D2Win_10121_GetTextWidth(const Unicode* wszText)
{
	return D2Win_10122(wszText, Unicode::strlen(wszText));
}

//D2Win.0x6F8AA2F0 (#10125)
int __stdcall D2Win_10125()
{
	return stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight;
}

//D2Win.0x6F8AA310 (#10119)
void __fastcall D2Win_10119_DrawCroppedText(const Unicode* wszText, int32_t nX, int32_t nY, int32_t a4, int32_t a5, int32_t a6, int32_t a7)
{
	// TODO: Names
	if (a5 < 0)
	{
		return;
	}

	const int32_t nDisplayType = D2GFX_GetRenderMode();

	D2GfxDataStrc gfxData = {};
	gfxData.nDirection = 0;

	const Unicode* v7 = wszText;
	const Unicode* v11 = v7 - 1;
	
	int32_t v8 = nX;

	int32_t v9 = 0;
	const int32_t v28 = Unicode::strlen(v7);
	while (v9 < v28)
	{
		const Unicode v12 = wszText[v9];
		++v11;
		++v9;

		if (*v11 >= 0x100u || *v11 != 0xFF)
		{
			if (v12 != '\n')
			{
				D2CharStrc* v16 = dword_6F8FE20C(v12);
				if (nDisplayType != DISPLAYTYPE_DIRECT3D)
				{
					gfxData.pCellFile = gpFontCache->pCellFile;
					gfxData.nFrame = v16->nImageIndex;

					const int32_t v18 = D2CMP_CelGetHeight(D2CMP_CelGetHandle(&gfxData));
					if (a6 + a5 > v18)
					{
						TEXTURE_CelDrawEx(&gfxData, v8, nY, 0, v18, DRAWMODE_NORMAL);
					}
					else
					{
						TEXTURE_CelDrawEx(&gfxData, v8, nY, a5, a6, DRAWMODE_NORMAL);
					}
				}

				v8 += v16->nWidth;
			}
			else
			{
				v8 = nX;
				nY += -(16 * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10 + 1);
			}
		}
		else
		{
			const Unicode v28 = wszText[v9];
			++v9;
			++v11;

			if (!Unicode::strncmp(&v28, word_6F8FE208, 1))
			{
				++v9;
				++v11;
			}
			else if (!Unicode::strncmp(&v28, word_6F8FE204, 1))
			{
				const Unicode v13 = *v11;
				++v9;
				++v11;
				
				D2CharStrc* v14 = dword_6F8FE20C(v13);
				gfxData.pCellFile = ghCellfileFontMonsterIndicators_6F8FD9EC;
				gfxData.nFrame = v14->nImageIndex;
				TEXTURE_CelDraw(&gfxData, v8, nY, -1u, DRAWMODE_NORMAL, 0);

				nY += 16 * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10;
			}
		}
	}
}

//D2Win.0x6F8AA510
void __fastcall sub_6F8AA510(const Unicode* wszText, int32_t nX, int32_t nY, int32_t a4, int32_t a5, int32_t a6)
{
	// TODO: Names
	D2GfxDataStrc gfxData = {};
	gfxData.pCellFile = gpFontCache->pCellFile;
	gfxData.nDirection = 0;

	const Unicode* v7 = wszText;
	
	int32_t v9 = nX + a5;
	int32_t v12 = a4;

	const int32_t v22 = a6 + nX;
	const int32_t v8 = Unicode::strlen(wszText);
	int32_t v10 = 0;
	while (v10 < v8)
	{
		if (v9 > v22)
		{
			break;
		}

		const Unicode v13 = *v7;
		++v7;
		++v10;

		if (v13 >= 0x100u || v13 != 0xFF)
		{
			if (v13 == '\n')
			{
				v9 = nX;
				nY += -16 * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10;
			}
			else
			{
				D2CharStrc* v18 = dword_6F8FE20C(v13);
				gfxData.nFrame = v18->nImageIndex;
				if (v9 > nX)
				{
					TEXTURE_CelDrawColor(&gfxData, v9, nY, -1u, DRAWMODE_NORMAL, v12);
				}

				v9 += v18->nWidth;
			}
		}
		else
		{
			const Unicode v17 = *v7;
			++v10;
			++v7;

			if (Unicode::strncmp(&v17, word_6F8FE208, 1) != 0)
			{
				--v10;
				--v7;

				D2CharStrc* v16 = dword_6F8FE20C(v13);
				gfxData.nFrame = v16->nImageIndex;
				if (v9 > nX)
				{
					TEXTURE_CelDrawColor(&gfxData, v9, nY, -1u, DRAWMODE_NORMAL, v12);
				}

				v9 += v16->nWidth;
			}
			else
			{
				v12 = 0;
				if (v10 < v8)
				{
					const Unicode v15 = *v7;
					++v10;
					++v7;
					
					v12 = v15 - '0';
					if (v12 >= '\r')
					{
						v12 = 0;
					}
				}
			}
		}
	}
}

//D2Win.0x6F8AA6E0 (#10124)
void __stdcall D2Win_10124()
{
	// TODO: Names
	Font v29 = gnFontSize_6F8BDB24;
	Font v24 = gnFontSize_6F8BDB24;

	if (!Unicode::strlen(gszFramedText_Buffer_6F8FD9F0))
	{
		return;
	}

	int32_t v1 = sub_6F8AA910(gszFramedText_Buffer_6F8FD9F0) + 8;

	int32_t nScreenWidth = 0;
	int32_t nScreenHeight = 0;

	int32_t v5 = 0;
	int32_t v6 = 0;
	int32_t v7 = 0;
	while (1)
	{
		int32_t v2 = 1;
		int32_t v3 = Unicode::strlen(gszFramedText_Buffer_6F8FD9F0);
		const Unicode* v4 = gszFramedText_Buffer_6F8FD9F0;
		if (v3 > 0)
		{
			do
			{
				if (*v4 == '\n')
				{
					++v2;
				}
				++v4;
				--v3;
			}
			while (v3);
		}

		v5 = v2 * dword_6F8BDB2C * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10;

		v6 = gnFramedText_X_6F8FE1F0;
		if (gnFramedText_Align_6F8FE1FC == 1)
		{
			v6 = gnFramedText_X_6F8FE1F0 - (v1 >> 1);
		}

		v7 = gnFramedText_Y_6F8FE1F4 + 2;
		WINDOW_GetDimensions(&nScreenWidth, &nScreenHeight);

		if (v5 <= nScreenHeight - 10)
		{
			break;
		}

		if (v24 == D2FONT_FONT16)
		{
			D2Win_10127_SetFont(D2FONT_FONT8);
			v24 = D2FONT_FONT8;
		}
		else
		{
			if (gnFontSize_6F8BDB24)
			{
				break;
			}

			D2Win_10127_SetFont(D2FONT_FONT6);
			v24 = D2FONT_FONT6;
		}

		v1 = sub_6F8AA910(gszFramedText_Buffer_6F8FD9F0) + 8;
	}

	const int32_t v11 = D2Clamp(v6, 0, nScreenWidth - v1);
	const int32_t v13 = D2Clamp(v7, v5 - 5, nScreenHeight - 5);

	if (gnFramedText_RectangleColor_6F8FE200)
	{
		if (!byte_6F8FE218)
		{
			byte_6F8FE218 = D2Win_10034_MixRGB(0x50u, 0, 0);
		}

		const int32_t v28 = v1 * gnFramedText_RectangleColor_6F8FE200 >> 7;
		D2GFX_DrawBox(v11, v13 - v5, v28 + v11, v13, byte_6F8FE218, DRAWMODE_TRANS50);

		if (v1 != v28)
		{
			D2GFX_DrawBox(v11 + v28, v13 - v5, v11 + v1, v13, 0, DRAWMODE_TRANS75);
		}
	}
	else
	{
		D2GFX_DrawBox(v11, v13 - v5, v11 + v1, v13, 0, DRAWMODE_TRANS75);
	}

	sub_6F8AA9E0(
		gszFramedText_Buffer_6F8FD9F0,
		v11,
		v13 - dword_6F8BDB30 * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10,
		v1,
		gnFramedText_TextColor_6F8FE1F8,
		1);

	if (v24 != v29)
	{
		D2Win_10127_SetFont(v29);
	}
}

//D2Win.0x6F8AA910
int __fastcall sub_6F8AA910(const Unicode* pStr)
{
	// TODO: Names
	D2_ASSERT(pStr);

	const int v8 = Unicode::strlen(pStr);
	if (v8 <= 0)
	{
		return 0;
	}

	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	const Unicode* v1 = pStr;

	while (*v1 && v4 < v8)
	{
		if (*v1 == '\n')
		{
			if (v2 > v3)
			{
				v3 = v2;
			}

			v2 = 0;
			++v1;
		}
		else
		{
			if (*v1 < 0x100u && *v1 == 0xFF)
			{
				if (!Unicode::strnicmp(v1 + 1, word_6F8FE204, 1))
				{
					v2 += dword_6F8FE20C(word_6F8FE204[0])->nWidth;
				}

				v1 += 3;
				v4 += 2;
			}
			else
			{
				v2 += dword_6F8FE20C(*v1)->nWidth;
				++v1;
			}
		}

		++v4;
	}

	if (v3 > v2)
	{
		v2 = v3;
	}

	return v2;
}

//D2Win.0x6F8AA9E0
void __fastcall sub_6F8AA9E0(const Unicode* wszText, int32_t nX, int32_t nY, int32_t a4, int32_t nGlobalPaletteShift, int32_t a6)
{
	// TODO: Names
	int32_t v6 = nX;
	if (a6)
	{
		v6 += (a4 - sub_6F8AABB0(wszText)) >> 1;
	}

	D2GfxDataStrc gfxData = {};
	gfxData.nDirection = 0;
	gfxData.pCellFile = gpFontCache->pCellFile;

	const Unicode* v10 = wszText;
	int32_t v11 = nGlobalPaletteShift;

	int32_t v8 = 0;
	const int32_t v21 = Unicode::strlen(wszText);
	while (v8 < v21)
	{
		const Unicode v12 = *v10;
		++v10;
		++v8;

		if (v12 >= 0x100u || v12 != 0xFF)
		{
			if (v12 == '\n')
			{
				v6 = nX;
				if (a6)
				{
					v6 += ((a4 - sub_6F8AABB0(v10)) >> 1);
				}

				nY += dword_6F8BDB28 * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / -10;
			}
			else
			{
				D2CharStrc* v18 = dword_6F8FE20C(v12);
				gfxData.nFrame = v18->nImageIndex;
				TEXTURE_CelDrawColor(&gfxData, v6, nY, -1u, DRAWMODE_NORMAL, v11);

				v6 += v18->nWidth;
			}
		}
		else
		{
			const Unicode v24 = *v10;

			++v8;
			++v10;

			if (Unicode::strncmp(&v24, word_6F8FE208, 1) != 0)
			{
				--v8;
				--v10;
				
				D2CharStrc* v16 = dword_6F8FE20C(v12);
				gfxData.nFrame = v16->nImageIndex;
				TEXTURE_CelDrawColor(&gfxData, v6, nY, -1u, DRAWMODE_NORMAL, v11);

				v6 += v16->nWidth;
			}
			else
			{
				v11 = 0;
				if (v8 < v21)
				{
					const Unicode v14 = *v10;

					++v8;
					++v10;

					v11 = v14 - '0';
					if (v11 >= '\r')
					{
						v11 = 0;
					}
				}
			}
		}
	}
}

//D2Win.0x6F8AABB0
int __fastcall sub_6F8AABB0(const Unicode* pStr)
{
	// TODO: Names
	D2_ASSERT(pStr);

	const int v7 = Unicode::strlen(pStr);
	int v2 = 0;
	int v3 = 0;
	const Unicode* v1 = pStr;
	while (v3 < v7)
	{
		if (*v1 == '\n')
		{
			break;
		}

		if (*v1 >= 0x100u || *v1 != 0xFF)
		{
			v2 += dword_6F8FE20C(*v1)->nWidth;
			++v1;
		}
		else
		{
			if (!Unicode::strnicmp(v1 + 1, word_6F8FE204, 1))
			{
				v2 += dword_6F8FE20C(word_6F8FE204[0])->nWidth;
			}

			v3 += 2;
			v1 += 3;
		}

		++v3;
	}

	return v2;
}

//D2Win.0x6F8AAC60 (#10129)
void __fastcall D2Win_10129_DrawFramedText(const Unicode* wszText, int nX, int nY, int nTextColor, int nAlign)
{
	D2Win_10130_DrawFramedTextEx(wszText, nX, nY, nTextColor, nAlign, 0);
}

//D2Win.0x6F8AACD0 (#10130)
void __fastcall D2Win_10130_DrawFramedTextEx(const Unicode* wszText, int nX, int nY, int nTextColor, int nAlign, int nRectangleColor)
{
	if (wszText && Unicode::strlen(wszText) < 1024)
	{
		Unicode::strcpy(gszFramedText_Buffer_6F8FD9F0, wszText);
	}
	else
	{
		memset(gszFramedText_Buffer_6F8FD9F0, 0, sizeof(gszFramedText_Buffer_6F8FD9F0));
	}

	gnFramedText_X_6F8FE1F0 = nX;
	gnFramedText_Y_6F8FE1F4 = nY;
	gnFramedText_TextColor_6F8FE1F8 = nTextColor;
	gnFramedText_Align_6F8FE1FC = nAlign;
	gnFramedText_RectangleColor_6F8FE200 = nRectangleColor;
}

//D2Win.0x6F8AAD40 (#10117)
void __fastcall D2Win_10117_DrawText(const Unicode* wszText, int nX, int nY, int nColor, BOOL bCentered)
{
	if (bCentered)
	{
		sub_6F8AA9E0(wszText, nX, nY, sub_6F8AA910(wszText) + 8, nColor, bCentered);
	}
	else
	{
		sub_6F8AA9E0(wszText, nX, nY, 0, nColor, bCentered);
	}
}

//D2Win.0x6F8AAD80
void __fastcall D2Win_10118_DrawBlendedText(const Unicode* wszText, int32_t nX, int32_t nY, int32_t nColor, int32_t bCentered, DrawMode eDrawMode)
{
	// TODO: Names
	int32_t v21 = 0;
	if (bCentered)
	{
		v21 = sub_6F8AA910(wszText) + 8;
	}

	int32_t v7 = nX;
	if (bCentered)
	{
		v7 += ((v21 - sub_6F8AABB0(wszText)) >> 1);
	}

	D2GfxDataStrc gfxData = {};
	gfxData.nDirection = 0;
	gfxData.pCellFile = gpFontCache->pCellFile;

	const Unicode* v11 = wszText;

	int32_t v9 = nColor;

	const int32_t v23 = Unicode::strlen(wszText);
	int32_t v6 = 0;
	while (v6 < v23)
	{
		const Unicode v12 = *v11;

		++v11;
		++v6;

		if (v12 >= 0x100u || v12 != 0xFF)
		{
			if (v12 == '\n')
			{
				v7 = nX;
				if (bCentered)
				{
					v7 += ((v21 - sub_6F8AABB0(v11)) >> 1);
				}

				nY += -16 * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10;
			}
			else
			{
				D2CharStrc* v18 = dword_6F8FE20C(v12);
				gfxData.nFrame = v18->nImageIndex;
				TEXTURE_CelDrawColor(&gfxData, v7, nY, -1u, eDrawMode, v9);

				v7 += v18->nWidth;
			}
		}
		else
		{
			const Unicode v19 = *v11;

			++v6;
			++v11;

			if (Unicode::strncmp(&v19, word_6F8FE208, 1) != 0)
			{
				--v6;
				--v11;

				D2CharStrc* v16 = dword_6F8FE20C(v12);
				gfxData.nFrame = v16->nImageIndex;
				TEXTURE_CelDrawColor(&gfxData, v7, nY, -1u, eDrawMode, v9);

				v7 += v16->nWidth;
			}
			else
			{
				v9 = 0;
				if (v6 < v23)
				{
					const Unicode v14 = *v11;

					++v6;
					++v11;

					v9 = v14 - '0';
					if (v9 >= '\r')
					{
						v9 = 0;
					}
				}
			}
		}
	}
}

//D2Win.0x6F8AAF80 (#10126)
void __fastcall D2Win_10126_DrawTextFromCache(const Unicode* pText, int nX, int nY)
{
	D2GfxDataStrc gfxData = {};
	gfxData.pCellFile = gpFontCache->pCellFile;
	gfxData.nDirection = 0;

	const Unicode* wszText = pText;
	int nImageX = nX;
	int nImageY = nY;

	if (Unicode::strlen(pText) <= 0)
	{
		return;
	}

	while (1)
	{
		if (*wszText < 0x100u && *wszText == 0xFF)
		{
			wszText += 3;
		}
		else
		{
			if (*wszText == '\n')
			{
				nImageX = nX;
				nImageY += -15 * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10;
			}
			else
			{
				D2CharStrc* pFont = dword_6F8FE20C(*wszText);

				gfxData.nFrame = pFont->nImageIndex;
				TEXTURE_CelDraw(&gfxData, nImageX, nImageY, -1, DRAWMODE_NORMAL, 0);

				nImageX += pFont->nWidth;
			}

			++wszText;
		}
	}
}

//D2Win.0x6F8AB080 (#10132)
void __fastcall D2Win_10132(const Unicode* wszText, int nX, int nY, DWORD dwColor, DrawMode eDrawMode, int nColor)
{
	int nTextWidth = 0;
	int nTextHeight = 0;
	D2Win_10131_GetTextDimensions(wszText, &nTextWidth, &nTextHeight);

	nY += 2;

	int nScreenWidth = 0;
	int nScreenHeight = 0;
	WINDOW_GetDimensions(&nScreenWidth, &nScreenHeight);

	if (nX < 0)
	{
		nX = 0;
	}

	if (nX >= nScreenWidth - nTextWidth)
	{
		nX = nScreenWidth - nTextWidth;
	}

	if (nY <= nTextHeight)
	{
		nY = nTextHeight;
	}

	if (nY >= nScreenHeight - 31)
	{
		nY = nScreenHeight - 31;
	}

	D2GFX_DrawBox(nX, nY - nTextHeight, nX + nTextWidth, nY, dwColor, eDrawMode);
	sub_6F8AA9E0(wszText, nX, nY - 3 * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10, nTextWidth, nColor, 1);
}

//D2Win.0x6F8AB1B0 (#10133)
void __fastcall D2Win_10133(const Unicode* pText, int nX, int nY, DWORD dwColor, DrawMode eDrawMode, int nPaletteShift)
{
	nY += 2;

	int nRectWidth = sub_6F8AA910(pText);
	int nRectHeight = stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight;

	int nScreenWidth = 0;
	int nScreenHeight = 0;
	WINDOW_GetDimensions(&nScreenWidth, &nScreenHeight);

	if (nX > nScreenWidth - nRectWidth)
	{
		nX = nScreenWidth - nRectWidth;
	}

	if (nY > nScreenHeight - 31)
	{
		nY = nScreenHeight - 31;
	}

	if (nY < nRectHeight)
	{
		nY *= 2;
	}

	D2GFX_DrawBox(nX, nY - nRectHeight, nX + nRectWidth, nY, dwColor, eDrawMode);
	sub_6F8AA9E0(pText, nX, nY - 2, nRectWidth, nPaletteShift, 1);
}

//D2Win.0x6F8AB260 (#1031)
void __fastcall D2Win_10131_GetTextDimensions(const Unicode* pText, int* pWidth, int* pHeight)
{
	*pWidth = sub_6F8AA910(pText);

	D2_ASSERT(pText);

	int nLines = 1;
	const int nTextLength = Unicode::strlen(pText);
	for (int i = 0; i < nTextLength; ++i)
	{
		if (pText[i] == '\n')
		{
			++nLines;
		}
	}

	*pHeight = nLines * dword_6F8BDB2C * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10;
}

//D2Win.0x6F8AB2F0 (#10128)
unsigned int __fastcall D2Win_10128(uint8_t a1)
{
	//TODO: Names
	unsigned int dword_6F8BA56C[] =
	{
		0, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF, 0x00060000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};

	if (a1 != 0xFF)
	{
		const uint32_t dwOffset = a1 >> 5;

		D2_ASSERT(dwOffset < 8);

		uint8_t v3 = a1 & 0x1F;
		if (v3 & 0x80)
		{
			v3 = ((v3 - 1) | 0xE0) + 1;
		}

		return (dword_6F8BA56C[dwOffset] >> v3) & 1;
	}

	return 0;
}

//D2Win.0x6F8AB360
void __fastcall sub_6F8AB360(const Unicode* pStr, int nX, int nY, int nBoxWidth, DWORD dwColor, DrawMode eDrawMode, int nPaletteShift)
{
	//TODO: Names
	D2_ASSERT(pStr);

	int nLines = 1;
	for (int i = 0; i < Unicode::strlen(pStr); ++i)
	{
		if (pStr[i] == '\n')
		{
			++nLines;
		}
	}

	unsigned int v12 = nLines * dword_6F8BDB2C * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10;
	int v13 = nY + 2;
	int v18 = ((unsigned int)v12 >> 31) + v12;
	int v15 = 4 * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10;

	int nScreenWidth = 0;
	int nScreenHeight = 0;
	WINDOW_GetDimensions(&nScreenWidth, &nScreenHeight);

	if (nX > nScreenWidth - nBoxWidth)
	{
		nX = nScreenWidth - nBoxWidth;
	}

	if (v13 > nScreenHeight - 31)
	{
		v13 = nScreenHeight - 31;
	}

	if (v13 < v18)
	{
		v13 *= 2;
	}

	D2GFX_DrawBox(nX - 4, v13 + v15 - v18, nX - 4 + nBoxWidth, v13, dwColor, eDrawMode);
	sub_6F8AA9E0(pStr, nX, v13 - 2, 0, nPaletteShift, 0);
}

//D2Win.0x6F8AB480 (#10134)
void __fastcall D2Win_10134_DrawCellFile(D2CellFileStrc* pCellFile, int nX, int nY, int nDiv, DrawMode eDrawMode, int nColor)
{
	D2GfxDataStrc gfxData = {};

	gfxData.pCellFile = pCellFile;
	gfxData.nDirection = 0;

	const int nFramesPerDirection = D2CMP_CelFileGetCelsPerDirection(pCellFile);
	
	if (nDiv == 1 || nDiv == 2)
	{
		int nAccumulatedWidth = 0;
		for (int nFrame = 0; nFrame < nFramesPerDirection; ++nFrame)
		{
			gfxData.nFrame = nFrame;
			nAccumulatedWidth += D2CMP_CelGetWidth(D2CMP_CelGetHandle(&gfxData));
		}

		if (nDiv == 1)
		{
			nX += -1 - (nAccumulatedWidth >> 1);
		}
		else
		{
			nX -= nAccumulatedWidth;
		}
	}

	for (int nFrame = 0; nFrame < nFramesPerDirection; ++nFrame)
	{
		gfxData.nFrame = nFrame;
		if (nColor == -1)
		{
			TEXTURE_CelDraw(&gfxData, nX + 256 * nFrame, nY, -1, eDrawMode, 0);
		}
		else
		{
			TEXTURE_CelDrawColor(&gfxData, nX + 256 * nFrame, nY, -1, eDrawMode, nColor);
		}
	}
}

//D2Win.0x6F8AB560 (#10120)
void __fastcall D2Win_10120(const Unicode* wszText, int nX, int nY, int a4, int a5, int a6)
{
	//TODO: Names
	if (Unicode::strlen(wszText))
	{
		int nTextWidth = 0;
		int nTextHeight = 0;
		D2Win_10131_GetTextDimensions(wszText, &nTextWidth, &nTextHeight);

		const int v8 = nTextWidth + 8;

		int v12 = nX;
		if (a5 == 1)
		{
			v12 = nX - (v8 >> 1);
		}

		int v13 = nY + 2;

		int nScreenWidth = 0;
		int nScreenHeight = 0;
		WINDOW_GetDimensions(&nScreenWidth, &nScreenHeight);

		if (v12 < 0)
		{
			v12 = 0;
		}

		if (v12 >= nScreenWidth - v8)
		{
			v12 = nScreenWidth - v8;
		}

		if (v13 <= nTextHeight)
		{
			v13 = nTextHeight;
		}

		if (v13 >= nScreenHeight - 31)
		{
			v13 = nScreenHeight - 31;
		}

		if (a6)
		{
			if (!byte_6F8FE218)
			{
				byte_6F8FE218 = D2Win_10034_MixRGB(0x50u, 0, 0);
			}

			const int v17 = a6 * v8 >> 7;
			D2GFX_DrawBox(v12, v13 - nTextHeight, v17 + v12, v13, byte_6F8FE218, DRAWMODE_TRANS50);

			if (v8 != v17)
			{
				D2GFX_DrawBox(v17 + v12, v13 - nTextHeight, v17 + v12 + v8 - v17, v13, 0, DRAWMODE_TRANS75);
			}
		}
		else
		{
			D2GFX_DrawBox(v12, v13 - nTextHeight, v8 + v12, v13, 0, DRAWMODE_TRANS75);
		}

		sub_6F8AA9E0(wszText, v12, v13 - 3 * stru_6F8FD8C0[gnFontSize_6F8BDB24].pFontInfo[1]->nHeight / 10, v8, a4, 1);
	}
}

//D2Win.0x6F8AB730 (#10200)
void __fastcall D2Win_10200(D2SplittedTextStrc* pSplitText)
{
	D2SplittedTextStrc* pNext = nullptr;
	for (D2SplittedTextStrc* i = pSplitText; i; i = pNext)
	{
		pNext = i->pNextLine;
		D2_FREE(i->wszLine);
		D2_FREE(i);
	}
}

// Helper function
D2SplittedTextStrc* __fastcall D2Win_AllocateSplitText(const Unicode* wszText, int nLength)
{
	D2SplittedTextStrc* pSplitText = (D2SplittedTextStrc*)FOG_Alloc(sizeof(D2SplittedTextStrc), __FILE__, __LINE__, 0);
	pSplitText->wszLine = (Unicode*)FOG_Alloc(sizeof(Unicode) * (nLength + 1), __FILE__, __LINE__, 0);
	pSplitText->pNextLine = nullptr;
	Unicode::strncpy(pSplitText->wszLine, wszText, nLength);
	pSplitText->wszLine[nLength] = L'\0';

	return pSplitText;
}

// Helper function
int __fastcall D2Win_UnkSplitText(const Unicode* wszText, int nTextLength, int nMaxLength, int v10, int* v12)
{
	*v12 = 0;

	int i = nTextLength;

	if (nTextLength > v10)
	{
		while (i > v10)
		{
			*v12 = D2Win_10123(&wszText[v10], 1 - v10 + i);
			if (*v12 <= nMaxLength)
			{
				break;
			}

			--i;
		}

		while (i > v10)
		{
			if (i == nTextLength || Unicode::isLineBreak(&wszText[v10], i - v10 + 1))
			{
				break;
			}

			--i;
		}
	}

	return i;
}

// Helper function
int __fastcall D2Win_UnkSplitText2(const Unicode* wszText, int* pLines, D2SplittedTextStrc** v31, D2SplittedTextStrc** v29, int i, int v10)
{
	int v16 = i - v10 + 1;
	const Unicode* v18 = &wszText[v10];
	if (*v18 < 0x100u && isspace(*v18))
	{
		--v16;
		++v18;
	}

	D2SplittedTextStrc* v15 = D2Win_AllocateSplitText(v18, v16);
	++*pLines;

	if (*v31)
	{
		(*v31)->pNextLine = v15;
	}

	*v31 = v15;
	if (!*v29)
	{
		*v29 = v15;
	}

	v10 = i + 1;

	return v10;
}

//D2Win.0x6F8AB770 (#10199)
D2SplittedTextStrc* __fastcall D2Win_10199(const Unicode* wszText, int* pLines, int nMaxLength)
{
	//TODO: Names
	const int nTextLength = Unicode::strlen(wszText);

	*pLines = 0;

	if (D2Win_10123(wszText, nTextLength) <= nMaxLength)
	{
		*pLines = 1;
		return D2Win_AllocateSplitText(wszText, nTextLength + 1);
	}

	D2SplittedTextStrc* v29 = nullptr;
	D2SplittedTextStrc* v31 = nullptr;

	int v10 = 0;
	do
	{
		int v12 = 0;
		int i = D2Win_UnkSplitText(wszText, nTextLength, nMaxLength, v10, &v12);
		if (i == v10)
		{
			for (i = nTextLength; i > v10; --i)
			{
				if (D2Win_10123(&wszText[v10], 1 - v10 + i) <= nMaxLength)
				{
					break;
				}
			}

			v10 = D2Win_UnkSplitText2(wszText, pLines, &v31, &v29, i, v10);
		}
		else if (v12 <= nMaxLength)
		{
			v10 = D2Win_UnkSplitText2(wszText, pLines, &v31, &v29, i, v10);
		}
		else
		{
			int nTemp = 0;
			int j = D2Win_UnkSplitText(wszText, nTextLength, nMaxLength, v10, &nTemp);
			if (j == v10)
			{
				for (j = nTextLength; j > v10; --j)
				{
					if (D2Win_10123(&wszText[v10], j + 1 - v10) <= nMaxLength)
					{
						break;
					}
				}
			}

			v10 = D2Win_UnkSplitText2(wszText, pLines, &v31, &v29, j, v10);
		}
	}
	while (v10 <= nTextLength);

	return v29;
}

//D2Win.0x6F8ABA70 (#10206)
DrawFramedTextPtr __stdcall D2Win_10206()
{
	return D2Win_10129_DrawFramedText;
}

//D2Win.0x6F8ABA80 (#10207)
GetTextDimensionsPtr __stdcall D2Win_10207()
{
	return D2Win_10131_GetTextDimensions;
}
