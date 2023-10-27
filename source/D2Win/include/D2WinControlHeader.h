#pragma once

#include <Windows.h>
#include <cstdint>


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
struct D2WinMsgStrc
{
	HWND hWnd;																//0x00
	UINT uMessage;															//0x04
	union																	//0x08
	{
		WPARAM wParam;														//0x08
		int32_t nKey;														//0x08
	};
	union
	{
		struct
		{
			WORD nXpos;														//0x0C
			WORD nYpos;														//0x0E
		};
		LPARAM lParam;														//0x0C
	};
	DWORD dwCommandSource;													//0x10 HIWORD(wParam) when uMessage == WM_COMMAND
	DWORD dwArg;															//0x14
	BOOL bReturn;															//0x18
	LRESULT lResult;														//0x1C
};

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
	int32_t(__stdcall* pfHandleMouseDown)(D2WinMsgStrc*);					//0x24
	int32_t(__stdcall* pfHandleMouseUp)(D2WinMsgStrc*);						//0x28
	int32_t(__stdcall* pfHandleCharInput)(D2WinMsgStrc*);					//0x2C
	int32_t(__stdcall* pfHandleVirtualKeyInput)(D2WinMsgStrc*);				//0x30
	int32_t(__stdcall* field_34)(D2WinMsgStrc*);							//0x34
	int32_t(__fastcall* pfShouldMouseInputBeHandled)(D2WinControlStrc*);	//0x38
	D2WinControlStrc* pNext;												//0x3C
};
#pragma pack(pop)
