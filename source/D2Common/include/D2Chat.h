#pragma once

#include "CommonDefinitions.h"

//D2Common.0x6FDC3BF0 (#10892)
D2HoverTextStrc* __stdcall CHAT_AllocHoverMsg(void* pMemPool, const char* szText, int nTimeout);
//D2Common.0x6FDC3C80 (#10893)
void __stdcall CHAT_FreeHoverMsg(void* pMemPool, D2HoverTextStrc* pHoverMsg);
//D2Common.0x6FDC3CA0 (#10894)
uint32_t __stdcall CHAT_GetDisplayTimeFromHoverMsg(D2HoverTextStrc* pHoverMsg);
//D2Common.0x6FDC3CB0 (#10895)
uint32_t __stdcall CHAT_GetTimeoutFromHoverMsg(D2HoverTextStrc* pHoverMsg);
//D2Common.0x6FDC3CC0 (#10896)
void __stdcall CHAT_CopyHoverMsgToBuffer(D2HoverTextStrc* pHoverMsg, char* szMessage);
//D2Common.0x6FDC3CE0 (#10897)
BOOL __stdcall CHAT_GetUsedFromHoverMsg(D2HoverTextStrc* pHoverMsg);
//D2Common.0x6FDC3CF0 (#10898)
void __stdcall CHAT_SetUsedInHoverMsg(D2HoverTextStrc* pHoverMsg, BOOL bUsed);
//D2Common.0x6FDC3D00 (#10899)
uint8_t __stdcall CHAT_GetLangIdFromHoverMsg(D2HoverTextStrc* pHoverMsg);
//D2Common.0x6FDC3D10 (#10900)
void __stdcall CHAT_SetLangIdInHoverMsg(D2HoverTextStrc* pHoverMsg, uint8_t nLangId);
