#pragma once

#include <Windows.h>
#include <cstdint>

#include "DrawMode.h"
#include "Font.h"
#include <D2Unicode.h>

struct D2CellFileStrc;


#pragma pack(push, 1)
struct D2CharStrc
{
	WORD wChar;										//0x00
	BYTE unk0x02;									//0x02
	BYTE nWidth;									//0x03
	BYTE nHeight;									//0x04
	BYTE unk0x05;									//0x05
	WORD unk0x06;									//0x06
	WORD nImageIndex;								//0x08
	DWORD unk0x0A;									//0x0A
};

struct D2FontStrc
{
	DWORD dwHeader;									//0x00 - 'Woo!'
	WORD unk0x04;									//0x04
	WORD unk0x06;									//0x06
	WORD unk0x08;									//0x08
	BYTE nHeight;									//0x0A
	BYTE nWidth;									//0x0B
	D2CharStrc pChars[256];							//0x0C
};

struct D2FontCacheStrc
{
	D2CellFileStrc* pCellFile;						//0x00
	D2FontStrc* pFontInfo[2];						//0x04
	D2CharStrc* pCharInfo;							//0x0C
	DWORD dwShiftedTickCount;						//0x10
};

struct D2SplittedTextStrc
{
	Unicode* wszLine;								//0x00
	D2SplittedTextStrc* pNextLine;					//0x04
};
#pragma pack(pop)


//D2Win.0x6F8A9B00
void __stdcall sub_6F8A9B00();
//D2Win.0x6F8A9B20
void __stdcall sub_6F8A9B20();
//D2Win.0x6F8A9B40
void __stdcall sub_6F8A9B40();
//D2Win.0x6F8A9B60
void __fastcall D2Win_10115_FONT();
//D2Win.0x6F8A9DC0
void __fastcall D2Win_FONT_6F8A9DC0(int nFont, char* pBuffer, int nLength);
//D2Win.0x6F8A9E90
void __fastcall sub_6F8A9E90(int a1);
//D2Win.0x6F8A9F90
void __stdcall D2Win_10116();
//D2Win.0x6F8A9FF0
Font __fastcall D2Win_10127_SetFont(Font nFontSize);
//D2Win.0x6F8AA100
D2CharStrc* __fastcall sub_6F8AA100(Unicode wszChar);
//D2Win.0x6F8AA140
D2CharStrc* __fastcall sub_6F8AA140(Unicode wszChar);
//D2Win.0x6F8AA1D0
int __fastcall D2Win_10123(const Unicode* wszText, int nTextLength);
//D2Win.0x6F8AA260
int __fastcall D2Win_10122(const Unicode* wszText, int nTextLength);
//D2Win.0x6F8AA2A0
int __fastcall D2Win_10121_GetTextWidth(const Unicode* wszText);
//D2Win.0x6F8AA2F0
int __stdcall D2Win_10125();
//D2Win.0x6F8AA310
void __fastcall D2Win_10119_DrawCroppedText(const Unicode* wszText, int32_t nX, int32_t nY, int32_t a4, int32_t a5, int32_t a6, int32_t a7);
//D2Win.0x6F8AA510
void __fastcall sub_6F8AA510(const Unicode* wszText, int32_t nX, int32_t nY, int32_t nColorShift, int32_t nPaddingX, int32_t nMaxWidth);
//D2Win.0x6F8AA6E0
void __stdcall D2Win_10124();
//D2Win.0x6F8AA910
int __fastcall FONT_GetMultilineTextWidth(const Unicode* pStr);
//D2Win.0x6F8AA9E0
void __fastcall sub_6F8AA9E0(const Unicode* wszText, int32_t nX, int32_t nY, int32_t nCenterPaddingX, int32_t nColorShift, int32_t bCenter);
//D2Win.0x6F8AABB0
int __fastcall FONT_GetSinglelineTextWidth(const Unicode* pStr);
//D2Win.0x6F8AAC60
void __fastcall D2Win_10129_DrawFramedText(const Unicode* wszText, int nX, int nY, int nTextColor, int nAlign);
//D2Win.0x6F8AACD0
void __fastcall D2Win_10130_DrawFramedTextEx(const Unicode* wszText, int nX, int nY, int nTextColor, int nAlign, int nRectangleColor);
//D2Win.0x6F8AAD40
void __fastcall D2Win_10117_DrawText(const Unicode* wszText, int nX, int nY, int nColor, BOOL bCentered);
//D2Win.0x6F8AAD80
void __fastcall D2Win_10118_DrawBlendedText(const Unicode* wszText, int32_t nX, int32_t nY, int32_t nColorShift, int32_t bCentered, DrawMode eDrawMode);
//D2Win.0x6F8AAF80
void __fastcall D2Win_10126_DrawTextFromCache(Unicode *wszText, int nX, int nY);
//D2Win.0x6F8AB080
void __fastcall D2Win_10132(const Unicode* wszText, int nX, int nY, DWORD dwColor, DrawMode eDrawMode, int nColor);
//D2Win.0x6F8AB1B0
void __fastcall D2Win_10133(const Unicode* pText, int nX, int nY, DWORD dwColor, DrawMode eDrawMode, int nPaletteShift);
//D2Win.0x6F8AB260
void __fastcall D2Win_10131_GetTextDimensions(const Unicode* pText, int* pWidth, int* pHeight);
//D2Win.0x6F8AB2F0
unsigned int __fastcall D2Win_10128(uint8_t a1);
//D2Win.0x6F8AB360
void __fastcall sub_6F8AB360(const Unicode* pStr, int nX, int nY, int nBoxWidth, DWORD dwColor, DrawMode eDrawMode, int nPaletteShift);
//D2Win.0x6F8AB480
void __fastcall D2Win_10134_DrawCellFile(D2CellFileStrc* pCellFile, int nX, int nY, int nDiv, DrawMode eDrawMode, int nColor);
//D2Win.0x6F8AB560
void __fastcall D2Win_10120(const Unicode* wszText, int nX, int nY, int a4, int a5, int a6);
//D2Win.0x6F8AB730
void __fastcall D2Win_10200(D2SplittedTextStrc* pSplitText);
//D2Win.0x6F8AB770
D2SplittedTextStrc* __fastcall D2Win_10199(const Unicode *wszText, int *pLines, int nMaxLength);

using DrawFramedTextPtr = decltype(D2Win_10129_DrawFramedText)*;
//D2Win.0x6F8ABA70
DrawFramedTextPtr __stdcall D2Win_10206();
using GetTextDimensionsPtr = decltype(D2Win_10131_GetTextDimensions)*;
//D2Win.0x6F8ABA80
GetTextDimensionsPtr __stdcall D2Win_10207();
