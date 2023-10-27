#pragma once

#include "D2WinControlHeader.h"

#include <DrawMode.h>


#pragma pack(push, 1)
struct D2AnimatedImageDescriptor
{
	D2CellFileStrc** ppCellFile1;							//0x00
	D2CellFileStrc** ppCellFile2;							//0x04
	DrawMode eDrawMode;										//0x08
	uint32_t nAnimSpeed;									//0x0C
};

struct D2WinAnimImageStrc
{
	D2WinControlStrc controlHeader;							//0x00
	D2AnimatedImageDescriptor* pAnimatedImageDescriptor;	//0x40
	uint32_t nAnimSpeed;									//0x44
	uint32_t dwStartTickCount;								//0x48
	uint32_t nFrame;										//0x4C
	int32_t nAnimType;										//0x50
	DrawMode eDrawMode;										//0x54
	void(__stdcall* field_58)(D2WinMsgStrc*);				//0x58
	int32_t bIsRunning;										//0x5C
};
#pragma pack(pop)


//D2Win.0x6F8A53B0
D2WinAnimImageStrc* __fastcall ANIMIMAGE_Create(int nX, int nY, int nWidth, int nHeight, D2CellFileStrc* pCellFile, uint32_t nAnimSpeed, int(__stdcall* a7)(D2WinMsgStrc*), D2AnimatedImageDescriptor* pDescriptor, DrawMode eDrawMode, void(__stdcall* a10)(D2WinMsgStrc*));
//D2Win.0x6F8B1BF0
int __fastcall ANIMIMAGE_Destroy(D2WinControlStrc* pControl);
//D2Win.0x6F8A54A0
int __fastcall D2Win_10106(D2WinAnimImageStrc* pAnimImage);
//D2Win.0x6F8A54D0
int __fastcall D2Win_10104(D2WinAnimImageStrc* pAnimImage);
//D2Win.0x6F8A5530
int32_t __fastcall ANIMIMAGE_ShouldMouseInputBeHandled(D2WinControlStrc* pControl);
//D2Win.0x6F8A5610
int32_t __fastcall ANIMIMAGE_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8A5900
int __stdcall ANIMIMAGE_HandleMouseDown(D2WinMsgStrc* pMsg);
//D2Win.0x6F8A5980
int __stdcall ANIMIMAGE_SetIsRunning(D2WinAnimImageStrc* pAnimImage, int a2);
