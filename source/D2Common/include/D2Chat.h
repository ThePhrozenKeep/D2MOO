#pragma once

#include "D2CommonDefinitions.h"
#pragma pack(1)

struct D2HoverTextStrc
{
	uint32_t dwDisplayTime;					//0x00
	uint32_t dwExpireTime;						//0x04
	uint8_t nLangId;							//0x08
	uint8_t pad0x09[3];						//0x09
	BOOL bUsed;								//0x0C
	char szMsg[256];						//0x10
};
#pragma pack()

//D2Common.0x6FDC3BF0 (#10892)
D2COMMON_DLL_DECL D2HoverTextStrc* __stdcall CHAT_AllocHoverMsg(void* pMemPool, const char* szText, int nTimeout);
//D2Common.0x6FDC3C80 (#10893)
D2COMMON_DLL_DECL void __stdcall CHAT_FreeHoverMsg(void* pMemPool, D2HoverTextStrc* pHoverMsg);
//D2Common.0x6FDC3CA0 (#10894)
D2COMMON_DLL_DECL uint32_t __stdcall CHAT_GetDisplayTimeFromHoverMsg(D2HoverTextStrc* pHoverMsg);
//D2Common.0x6FDC3CB0 (#10895)
D2COMMON_DLL_DECL uint32_t __stdcall CHAT_GetTimeoutFromHoverMsg(D2HoverTextStrc* pHoverMsg);
//D2Common.0x6FDC3CC0 (#10896)
D2COMMON_DLL_DECL void __stdcall CHAT_CopyHoverMsgToBuffer(D2HoverTextStrc* pHoverMsg, char* szMessage);
//D2Common.0x6FDC3CE0 (#10897)
D2COMMON_DLL_DECL BOOL __stdcall CHAT_GetUsedFromHoverMsg(D2HoverTextStrc* pHoverMsg);
//D2Common.0x6FDC3CF0 (#10898)
D2COMMON_DLL_DECL void __stdcall CHAT_SetUsedInHoverMsg(D2HoverTextStrc* pHoverMsg, BOOL bUsed);
//D2Common.0x6FDC3D00 (#10899)
D2COMMON_DLL_DECL uint8_t __stdcall CHAT_GetLangIdFromHoverMsg(D2HoverTextStrc* pHoverMsg);
//D2Common.0x6FDC3D10 (#10900)
D2COMMON_DLL_DECL void __stdcall CHAT_SetLangIdInHoverMsg(D2HoverTextStrc* pHoverMsg, uint8_t nLangId);
