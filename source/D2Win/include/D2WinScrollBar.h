#pragma once

#include "D2WinControlHeader.h"


struct D2WinTextBoxStrc;

#pragma pack(push, 1)
struct D2WinScrollBarStrc
{
	D2WinControlStrc controlHeader;					//0x00
	int32_t bIsUpperArrowPressed;					//0x40
	int32_t bIsLowerArrowPressed;					//0x44
	int32_t nMaxSteps;								//0x48
	int32_t nCurrentStep;							//0x4C
	int32_t nPageStep;								//0x50
	D2WinTextBoxStrc* pTextBox;						//0x54
	int32_t bIsStepIndicatorDragged;				//0x58
	void(__stdcall* pfOnStepChange)(SMSGHANDLER_PARAMS*);		//0x5C
};
#pragma pack(pop)


//D2Win.0x6F8AF040
//1.14d: 0x005084F0
D2WinScrollBarStrc* __fastcall SCROLLBAR_Create(int nX, int nY, int nWidth, int nHeight, D2CellFileStrc* pCellFile, int(__stdcall* pfCallback)(SMSGHANDLER_PARAMS*));
//D2Win.0x6F8B1BF0
//1.14d: 0x00507EE0
int __fastcall SCROLLBAR_Destroy(D2WinControlStrc* pControl);
//D2Win.0x6F8AF100
//1.14d: 0x00507EF0
int __fastcall SCROLLBAR_GetMaxSteps(D2WinScrollBarStrc* pScrollBar);
//D2Win.0x6F8AF130
//1.14d: 0x00507F20
int32_t __fastcall SCROLLBAR_SetMaxSteps(D2WinScrollBarStrc* pScrollBar, int32_t nMaxSteps);
//D2Win.0x6F8AF170
//1.14d: 0x00507F60
int32_t __fastcall SCROLLBAR_SetCurrentStep(D2WinScrollBarStrc* pScrollBar, int32_t nCurrentStep);
//D2Win.0x6F8AF1B0
//1.14d: 0x00507FA0
int __fastcall SCROLLBAR_GetCurrentStep(D2WinScrollBarStrc* pScrollBar);
//D2Win.0x6F8AF1E0
//1.14d: 0x00507FD0
int __fastcall SCROLLBAR_SetTextBox(D2WinScrollBarStrc* pScrollBar, D2WinTextBoxStrc* pTextBox);
//D2Win.0x6F8AF240
//1.14d: 0x00508030
int __fastcall sub_6F8AF240(SMSGHANDLER_PARAMS* pMsg, D2WinScrollBarStrc* pScrollBar, int a3);
//D2Win.0x6F8AF2D0
//1.14d: 0x005080C0
void __fastcall D2Win_10202(D2WinScrollBarStrc* pScrollBar, int nPosition);
//D2Win.0x6F8AF300
int __fastcall D2Win_10201(D2WinScrollBarStrc* pScrollBar);
//D2Win.0x6F8AF330
//1.14d: 0x00508370
int32_t __fastcall SCROLLBAR_Draw(D2WinControlStrc* pControl);
//D2Win.0x6F8AF4D0
//1.14d: 0x00508120
int __stdcall SCROLLBAR_HandleMouseDown(SMSGHANDLER_PARAMS* pMsg);
//D2Win.0x6F8AF6B0
//1.14d: 0x00508310
int __stdcall SCROLLBAR_HandleMouseUp(SMSGHANDLER_PARAMS* pMsg);
