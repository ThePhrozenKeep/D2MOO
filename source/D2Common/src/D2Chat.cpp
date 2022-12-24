#include <cstring>

#include <D2Chat.h>
#include <D2Lang.h>
#include <D2StrTable.h>

//D2Common.0x6FDC3BF0 (#10892)
D2HoverTextStrc* __stdcall CHAT_AllocHoverMsg(void* pMemPool, const char* szText, int nTimeout)
{
	D2HoverTextStrc* pHoverMsg = NULL;
	size_t nTextLength = 0;
	int nLength = 0;

	nTextLength = strlen(szText);
	if (nTextLength == 0)
	{
		return NULL;
	}

	if (nTextLength < 255)
	{
		nLength = nTextLength;
	}
	else
	{
		nLength = 254;
	}

	pHoverMsg = (D2HoverTextStrc*)FOG_AllocPool(pMemPool, sizeof(D2HoverTextStrc), __FILE__, __LINE__, 0);
	memset(pHoverMsg, 0x00, sizeof(D2HoverTextStrc));

	pHoverMsg->dwDisplayTime = 8 * nLength + 125;
	pHoverMsg->dwExpireTime = 8 * nLength + 125 + nTimeout;
	pHoverMsg->nLangId = STRTABLE_GetLanguage();
	pHoverMsg->bUsed = FALSE;

	strncpy_s(pHoverMsg->szMsg, szText, nLength + 1);
	pHoverMsg->szMsg[nLength + 1] = '\0';

	return pHoverMsg;
}

//D2Common.0x6FDC3C80 (#10893)
void __stdcall CHAT_FreeHoverMsg(void* pMemPool, D2HoverTextStrc* pHoverMsg)
{
	if (pHoverMsg)
	{
		FOG_FreePool(pMemPool, pHoverMsg, __FILE__, __LINE__, 0);
	}
}

//D2Common.0x6FDC3CA0 (#10894)
uint32_t __stdcall CHAT_GetDisplayTimeFromHoverMsg(D2HoverTextStrc* pHoverMsg)
{
	return pHoverMsg->dwDisplayTime;
}

//D2Common.0x6FDC3CB0 (#10895)
uint32_t __stdcall CHAT_GetTimeoutFromHoverMsg(D2HoverTextStrc* pHoverMsg)
{
	return pHoverMsg->dwExpireTime;
}

//D2Common.0x6FDC3CC0 (#10896)
void __stdcall CHAT_CopyHoverMsgToBuffer(D2HoverTextStrc* pHoverMsg, char* szMessage)
{
	int nCounter = 0;

	do
	{
		szMessage[nCounter] = pHoverMsg->szMsg[nCounter];

		++nCounter;
	}
	while (pHoverMsg->szMsg[nCounter - 1]);
}

//D2Common.0x6FDC3CE0 (#10897)
BOOL __stdcall CHAT_GetUsedFromHoverMsg(D2HoverTextStrc* pHoverMsg)
{
	if (pHoverMsg)
	{
		return pHoverMsg->bUsed;
	}

	REMOVE_LATER_Trace("CHAT_GetUsedFromHoverMsg: NULL pointer");
	return FALSE;
}

//D2Common.0x6FDC3CF0 (#10898)
void __stdcall CHAT_SetUsedInHoverMsg(D2HoverTextStrc* pHoverMsg, BOOL bUsed)
{
	pHoverMsg->bUsed = bUsed;
}

//D2Common.0x6FDC3D00 (#10899)
uint8_t __stdcall CHAT_GetLangIdFromHoverMsg(D2HoverTextStrc* pHoverMsg)
{
	return pHoverMsg->nLangId;
}

//D2Common.0x6FDC3D10 (#10900)
void __stdcall CHAT_SetLangIdInHoverMsg(D2HoverTextStrc* pHoverMsg, uint8_t nLangId)
{
	pHoverMsg->nLangId = nLangId;
}
