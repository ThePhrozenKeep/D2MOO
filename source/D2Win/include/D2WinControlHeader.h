#pragma once

#include <Windows.h>
#include <cstdint>
#include <Storm.h>

enum D2ControlTypes
{
	D2WIN_EDITBOX = 1,
	D2WIN_IMAGE = 2,
	D2WIN_ANIMIMAGE = 3,
	D2WIN_TEXTBOX = 4,
	D2WIN_SCROLLBAR = 5,
	D2WIN_BUTTON = 6,
	D2WIN_LIST = 7,
	D2WIN_TIMER = 8,
	D2WIN_SMACK = 9,
	D2WIN_PROGRESSBAR = 10,
	D2WIN_POPUP = 11,
	D2WIN_ACCOUNTLIST = 12,
	D2WIN_IMAGE2 = 13,
};


struct D2CellFileStrc;

#pragma pack(push, 1)

struct D2WinControlStrc
{
	int32_t nType;															//0x00
	D2CellFileStrc* pCellFile;												//0x04
	uint32_t dwFlags;														//0x08
	int32_t nImageX;														//0x0C
	int32_t nImageY;														//0x10
	int32_t nWidth;															//0x14
	int32_t nHeight;														//0x18
	int32_t(__fastcall* pfDraw)(D2WinControlStrc*);							//0x1C
	int32_t(__fastcall* pfInitialize)(D2WinControlStrc*);					//0x20
	int32_t(__stdcall* pfHandleMouseDown)(SMSGHANDLER_PARAMS*);				//0x24
	int32_t(__stdcall* pfHandleMouseUp)(SMSGHANDLER_PARAMS*);				//0x28
	int32_t(__stdcall* pfHandleCharInput)(SMSGHANDLER_PARAMS*);				//0x2C
	int32_t(__stdcall* pfHandleVirtualKeyInput)(SMSGHANDLER_PARAMS*);		//0x30
	int32_t(__stdcall* field_34)(SMSGHANDLER_PARAMS*);						//0x34
	int32_t(__fastcall* pfShouldMouseInputBeHandled)(D2WinControlStrc*);	//0x38
	D2WinControlStrc* pNext;												//0x3C
};
#pragma pack(pop)
