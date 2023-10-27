#pragma once

#include "D2WinControlHeader.h"

#include <DrawMode.h>


#pragma pack(push, 1)
struct D2WinImageClickRectStrc
{
	int32_t nX;										//0x00
	int32_t nY;										//0x04
	int32_t nWidth;									//0x08
	int32_t nHeight;								//0x0C
	int32_t(__stdcall* pCallback)(D2WinMsgStrc*);	//0x14
};

struct D2WinImageStrc
{
	D2WinControlStrc controlHeader;					//0x00
	int32_t nFrame;									//0x40
	DrawMode eDrawMode;								//0x44
	D2WinImageClickRectStrc* pClickRect;			//0x48
	int32_t field_4C;								//0x4C
	int32_t field_50;								//0x50
};

struct D2WinImage2Strc
{
	D2WinControlStrc controlHeader;					//0x00
	int32_t nFrame;									//0x40
	DrawMode eDrawMode;								//0x44
	D2WinImageClickRectStrc* pClickRect;			//0x48
};
#pragma pack(pop)


//D2Win.0x6F8ABA90
D2WinImage2Strc* __fastcall IMAGE2_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, D2CellFileStrc* pCellFile, int32_t(__stdcall* a6)(D2WinMsgStrc*), D2WinImageClickRectStrc* pClickRect, int32_t(__stdcall* a8)(D2WinMsgStrc*));
//D2Win.0x6F8B1BF0
int __fastcall IMAGE_Destroy(D2WinControlStrc* pControl);
//D2Win.0x6F8ABB40
D2WinImageStrc* __fastcall IMAGE_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, D2CellFileStrc* pCellFile, int32_t a6, int32_t a7);
//D2Win.0x6F8ABC00
int32_t __stdcall IMAGE_SetCellFile(D2WinImageStrc* pImage, D2CellFileStrc* pCellFile, D2WinImageClickRectStrc* pClickRect);
//D2Win.0x6F8ABC40
int32_t __stdcall IMAGE_SetCoordinates(D2WinImageStrc* pImage, int32_t nX, int32_t nY);
//D2Win.0x6F8ABC80
int32_t __stdcall IMAGE_SetFrame(D2WinImageStrc* pImage, int32_t nFrame);
//D2Win.0x6F8ABCD0
int32_t __stdcall IMAGE_ForceNormalDrawMode(D2WinImageStrc* pImage, int32_t nUnused);
//D2Win.0x6F8ABD10
int32_t __stdcall IMAGE_GetFrameCount(D2WinImageStrc* pImage);
//D2Win.0x6F8ABD50
int32_t __fastcall IMAGE2_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8ABEA0
int32_t __fastcall IMAGE_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8AC170
int32_t __stdcall IMAGE2_HandleMouseDown(D2WinMsgStrc* pMsg);
