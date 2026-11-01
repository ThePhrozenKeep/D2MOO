#include "D2WinEditBox.h"

#include <algorithm>
#include <cstring>

#include <D2Math.h>

#include "D2CMP.h"
#include "D2Gfx.h"
#include "D2Lang.h"
#include "Fog.h"

#include <Texture.h>
#include <Window.h>

#include <D2BitManip.h>

#include "D2WinFont.h"
#include "D2WinMain.h"
#include "D2WinPalette.h"

#pragma warning (disable : 28159)


//1.10f: D2Win.0x6F8FD8B0
//1.14d: 0x00841B78
D2WinEditBoxStrc* dword_6F8FD8B0;
//1.10f: D2Win.0x6F8FD8B4
//1.14d: 0x00841B7C
int dword_6F8FD8B4;
// The last text read from the clipboard (Ctrl+V)
//1.10f: D2Win.0x6F8FD6B0
//1.14d: 0x00841B80
static char gszClipboardText_6F8FD6B0[0x200];
// The memory handed to SetClipboardData by the last Ctrl+C / Ctrl+X
//1.10f: D2Win.0x6F8FD8B8
//1.14d: 0x00841D80
static HGLOBAL ghClipboardMemory_6F8FD8B8;


//D2Win.0x6F8A6C80 (#10070)
//1.14d: 0x005001D0
D2WinEditBoxStrc* __fastcall D2Win_10070_EDITBOX_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t a5, int32_t a6, D2CellFileStrc* pCellFile, int32_t pfOnReturn, int32_t(__stdcall* pfCallback)(SMSGHANDLER_PARAMS*), int32_t nEditBoxFlags, int32_t a11)
{
	D2WinEditBoxStrc* pEditBox = D2_CALLOC_STRC(D2WinEditBoxStrc);

	pEditBox->controlHeader.nType = D2WIN_EDITBOX;
	pEditBox->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pEditBox->controlHeader.nImageX = nX;
	pEditBox->controlHeader.nImageY = nY;
	pEditBox->controlHeader.nWidth = nWidth;
	pEditBox->controlHeader.nHeight = nHeight;
	pEditBox->controlHeader.pCellFile = pCellFile;
	pEditBox->nTextInsetX = a5;
	pEditBox->nTextInsetY = a6;
	pEditBox->nMaxLength = 256;
	pEditBox->nSelectionStart = -1;

	if (!(nEditBoxFlags & EDITBOX_MULTILINE))
	{
		pEditBox->controlHeader.pfDraw = EDITBOX_DrawSingleLine;
		pEditBox->controlHeader.pfHandleMouseDown = EDITBOX_HandleSingleLineMouseDown;
	}
	else
	{
		pEditBox->controlHeader.pfDraw = EDITBOX_DrawMultiLine;
		pEditBox->controlHeader.pfHandleMouseDown = sub_6F8A97D0;
	}

	pEditBox->controlHeader.pfHandleMouseUp = EDITBOX_HandleMouseUp;
	pEditBox->pfOnReturn = (EditBoxReturnCallback)pfOnReturn;
	pEditBox->controlHeader.pfCallback = pfCallback;
	pEditBox->dwEditBoxFlags = nEditBoxFlags;

	if (a11)
	{
		pEditBox->eFont = *(Font*)a11;
		pEditBox->nTextColor = *(DWORD*)(a11 + 4);
	}
	else
	{
		pEditBox->eFont = D2FONT_FONT16;
		pEditBox->nTextColor = 0;
	}

	CONTROL_AppendToList(&pEditBox->controlHeader);

	return pEditBox;
}

//D2Win.0x6F8A6DC0 (#10071)
//1.14d: 0x004FDAA0
int32_t __fastcall D2Win_10071_EDITBOX_Destroy(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	return CONTROL_RemoveFromList(&pEditBox->controlHeader);
}

//D2Win.0x6F8A6E10 (#10072)
//1.14d: 0x004FDAD0
void __fastcall EDITBOX_SetKeyFilter(D2WinEditBoxStrc* pEditBox, EditBoxKeyFilterCallback pfKeyFilter)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->pfKeyFilter = pfKeyFilter;
}

//D2Win.0x6F8A6E60 (#10073)
//1.14d: 0x004FDB00
void __fastcall EDITBOX_SetChangeCallback(D2WinEditBoxStrc* pEditBox, EditBoxChangeCallback pfOnChange)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->pfOnChange = pfOnChange;
}

//Inlined at various places
//1.14d: 0x004FDC70
BOOL __fastcall EDITBOX_HasSelection(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	return pEditBox->nSelectionStart != -1 && pEditBox->nSelectionStart != pEditBox->nSelectionEnd;
}

//D2Win.0x6F8A6EB0 (#10083)
//1.14d: 0x004FDCC0
void __fastcall EDITBOX_ClearSelection(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->nSelectionStart = -1;
	pEditBox->nSelectionEnd = 0;
}

//D2Win.0x6F8A6F10 (#10082)
int32_t __fastcall D2Win_10082(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->nSelectionStart = 0;
	pEditBox->nSelectionEnd = Unicode::strlen(pEditBox->wszText);

	return pEditBox->nSelectionEnd;
}

//Inlined in D2Win.0x6F8A6F70 (#10074)
//1.14d: 0x004FDB40
const char* __fastcall EDITBOX_GetClipboardText()
{
	if (!IsClipboardFormatAvailable(CF_TEXT))
	{
		return "";
	}

	if (!OpenClipboard(WINDOW_GetWindow()))
	{
		return "";
	}

	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData)
	{
		const char* szData = (const char*)GlobalLock(hData);
		if (szData)
		{
			strncpy(gszClipboardText_6F8FD6B0, szData, 0x200);
			GlobalUnlock(hData);
		}
	}

	CloseClipboard();
	return gszClipboardText_6F8FD6B0;
}

//Inlined in D2Win.0x6F8A6F70 (#10074)
//1.14d: 0x004FDBB0
BOOL __fastcall EDITBOX_SetClipboardText(const char* szText)
{
	if (ghClipboardMemory_6F8FD8B8)
	{
		GlobalFree(ghClipboardMemory_6F8FD8B8);
		ghClipboardMemory_6F8FD8B8 = nullptr;
	}

	if (!OpenClipboard(WINDOW_GetWindow()))
	{
		return FALSE;
	}

	EmptyClipboard();

	const size_t nLength = strlen(szText);
	ghClipboardMemory_6F8FD8B8 = GlobalAlloc(GMEM_DDESHARE, nLength + 1);
	if (!ghClipboardMemory_6F8FD8B8)
	{
		CloseClipboard();
		return FALSE;
	}

	char* pMemory = (char*)GlobalLock(ghClipboardMemory_6F8FD8B8);
	memcpy(pMemory, szText, nLength);
	pMemory[nLength] = 0;
	GlobalUnlock(ghClipboardMemory_6F8FD8B8);
	SetClipboardData(CF_TEXT, ghClipboardMemory_6F8FD8B8);
	CloseClipboard();
	return TRUE;
}

// Height in pixels the text takes in a multi-line box: lines wrap where a character would not fit in front of the
// caret's width, CR / CR LF runs start new lines.
//Inlined in D2Win.0x6F8A6F70 (#10074)
//1.14d: 0x004FDE30
int32_t __fastcall EDITBOX_GetTextHeight(D2WinEditBoxStrc* pEditBox, const Unicode* wszText)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	D2Win_10127_SetFont(pEditBox->eFont);

	const int32_t nStartY = pEditBox->nTextInsetY + pEditBox->controlHeader.nImageY - pEditBox->controlHeader.nHeight + D2Win_10125();

	Unicode wszCaret[2] = {};
	Unicode::win2Unicode(wszCaret, "_", 2);
	const int32_t nCaretWidth = D2Win_10122(wszCaret, 2);

	Unicode wszLine[256] = {};
	char szText[256] = {};
	Unicode::unicode2Win(szText, wszText, 256);

	const char* pCurrent = szText;
	const char* pLineStart = szText;
	int32_t nY = nStartY;
	const int32_t nLineHeight = D2Win_10125();
	const int32_t nAvailableWidth = pEditBox->controlHeader.nWidth - 2 * pEditBox->nTextInsetX;

	while (*pCurrent)
	{
		int32_t nCount = pCurrent - pLineStart;
		BOOL bWrapped = FALSE;
		char szLine[256] = {};
		char nChar = *pCurrent;
		if (nChar)
		{
			while (nChar != '\r' && nChar != '\n')
			{
				szLine[nCount] = nChar;
				Unicode::win2Unicode(wszLine, szLine, 256);
				if (nCaretWidth + D2Win_10122(wszLine, nCount) > nAvailableWidth)
				{
					bWrapped = TRUE;
					break;
				}

				nChar = *++pCurrent;
				++nCount;
				if (!nChar)
				{
					break;
				}
			}
		}

		Unicode::win2Unicode(wszLine, szLine, 256);

		if (bWrapped)
		{
			nY += nLineHeight;
		}

		if (*pCurrent == '\r')
		{
			if (pCurrent[1] == '\n')
			{
				do
				{
					if (pCurrent[1] != '\n')
					{
						break;
					}

					pCurrent += 2;
					nY += nLineHeight;
				}
				while (*pCurrent == '\r');
			}
			else
			{
				do
				{
					++pCurrent;
					nY += nLineHeight;
				}
				while (*pCurrent == '\r');
			}
		}

		pLineStart = pCurrent;
	}

	return nY - nStartY;
}

//D2Win.0x6F8A6F70 (#10074)
//1.14d: 0x004FEAE0
int32_t __fastcall D2Win_10074(D2WinEditBoxStrc* pEditBox, uint8_t a2)
{
	char nChar = a2;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (!(pEditBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pEditBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	if (a2 == '\t' && pEditBox->dwEditBoxFlags & EDITBOX_TAB_TO_KEY_FILTER)
	{
		if (pEditBox->pfKeyFilter)
		{
			++dword_6F8FE254;
			pEditBox->pfKeyFilter(pEditBox, pEditBox->nCaretPosition, &nChar);
			--dword_6F8FE254;
		}
		return 0;
	}

	// Ctrl+V, Ctrl+C, Ctrl+X, Ctrl+A
	constexpr char CHAR_PASTE = 22;
	constexpr char CHAR_COPY = 3;
	constexpr char CHAR_CUT = 24;
	constexpr char CHAR_SELECT_ALL = 1;

	// The Ctrl+C / Ctrl+X copy buffer and the typed-character insert buffer
	// 1.10f: 0x190 and 0x200 bytes (frame [ebp-B90h] and [ebp-A00h])
	// 1.14d: one 0x100-byte slot shared by both (frame [ebp-110h]); the Enter path's unicode2Win writes 2 bytes past it
#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 14
	constexpr int32_t COPY_BUFFER_SIZE = 0x100;
	constexpr int32_t INSERT_BUFFER_SIZE = 0x100;
#else
	constexpr int32_t COPY_BUFFER_SIZE = 0x190;
	constexpr int32_t INSERT_BUFFER_SIZE = 0x200;
#endif

	if (!D2Win_10128(a2))
	{
		const uint32_t dwEditBoxFlags = pEditBox->dwEditBoxFlags;
		if ((!(dwEditBoxFlags & EDITBOX_ALLOW_COLOR_CODE) || a2 != 0xFF)
			&& (!(dwEditBoxFlags & EDITBOX_MULTILINE) || nChar != '\r')
			&& (!(dwEditBoxFlags & EDITBOX_ALLOW_CHAR_14) || nChar != 14)
			&& nChar != CHAR_PASTE && nChar != CHAR_COPY && nChar != CHAR_CUT && nChar != CHAR_SELECT_ALL)
		{
			return 0;
		}
	}

	if (pEditBox->pfKeyFilter && !pEditBox->pfKeyFilter(pEditBox, pEditBox->nCaretPosition, &nChar)
		&& nChar != CHAR_PASTE && nChar != CHAR_COPY && nChar != CHAR_CUT && nChar != CHAR_SELECT_ALL)
	{
		return 0;
	}

	if (EDITBOX_HasSelection(pEditBox) && nChar != CHAR_COPY && nChar != CHAR_CUT && nChar != CHAR_SELECT_ALL)
	{
		sub_6F8A7970(pEditBox);
	}

	const int32_t nTextLength = Unicode::strlen(pEditBox->wszText);

	Unicode wszNewText[512] = {};
	Unicode::strcpy(wszNewText, pEditBox->wszText);
	Unicode* pCaret = &wszNewText[pEditBox->nCaretPosition];

	switch (nChar)
	{
	case CHAR_SELECT_ALL:
		D2Win_10082(pEditBox);
		break;

	case CHAR_COPY:
		if (EDITBOX_HasSelection(pEditBox))
		{
			const int32_t nSelectionMin = std::min(pEditBox->nSelectionStart, pEditBox->nSelectionEnd);
			const int32_t nSelectionMax = std::max(pEditBox->nSelectionStart, pEditBox->nSelectionEnd);

			char szCopy[COPY_BUFFER_SIZE] = {};
			Unicode::unicode2Win(szCopy, &wszNewText[nSelectionMin], std::max(nSelectionMax - nSelectionMin + 1, 2));
			EDITBOX_SetClipboardText(szCopy);
		}
		break;

	case CHAR_PASTE:
	{
		const char* szPaste = EDITBOX_GetClipboardText();
		if ((int32_t)(nTextLength + strlen(szPaste)) > pEditBox->nMaxLength - 1)
		{
			return 0;
		}

		if (pEditBox->pfKeyFilter)
		{
			const int32_t nPasteLength = strlen(szPaste);
			for (int32_t i = 0; i < nPasteLength; ++i)
			{
				if (!D2Win_10128(szPaste[i]) || !pEditBox->pfKeyFilter(pEditBox, pEditBox->nCaretPosition + i, (char*)&szPaste[i]))
				{
					return 0;
				}
			}
		}

		Unicode wszBefore[256] = {};
		Unicode wszInserted[256] = {};
		Unicode wszAfter[256] = {};

		int32_t nInsertStart = pEditBox->nCaretPosition;
		int32_t nInsertEnd = pEditBox->nCaretPosition;
		if (pEditBox->nSelectionStart != -1)
		{
			nInsertStart = std::min(pEditBox->nSelectionStart, pEditBox->nSelectionEnd);
			nInsertEnd = std::max(pEditBox->nSelectionStart, pEditBox->nSelectionEnd);
		}

		char szInserted[400] = {};
		if (pEditBox->dwEditBoxFlags & EDITBOX_MULTILINE)
		{
			strcpy(szInserted, szPaste);
		}
		else
		{
			char* pDest = szInserted;
			for (const char* pSource = szPaste; *pSource; ++pSource)
			{
				if (*pSource != '\r' && *pSource != '\n')
				{
					*pDest++ = *pSource;
				}
			}
			*pDest = 0;
		}

		Unicode::strncpy(wszBefore, wszNewText, nInsertStart);
		Unicode::strcpy(wszAfter, &wszNewText[nInsertEnd]);
		Unicode::win2Unicode(wszInserted, szInserted, strlen(szInserted) + 1);
		Unicode::strcpy(wszNewText, wszBefore);
		Unicode::strcat(wszNewText, wszInserted);
		Unicode::strcat(wszNewText, wszAfter);

		EDITBOX_ClearSelection(pEditBox);
		pCaret = &wszNewText[nInsertStart + strlen(szInserted)];
		break;
	}

	case CHAR_CUT:
		if (EDITBOX_HasSelection(pEditBox))
		{
			const int32_t nSelectionMin = std::min(pEditBox->nSelectionStart, pEditBox->nSelectionEnd);
			const int32_t nSelectionMax = std::max(pEditBox->nSelectionStart, pEditBox->nSelectionEnd);

			pCaret = &wszNewText[nSelectionMin];

			char szCopy[COPY_BUFFER_SIZE] = {};
			Unicode::unicode2Win(szCopy, pCaret, std::max(nSelectionMax - nSelectionMin + 1, 2));
			EDITBOX_SetClipboardText(szCopy);

			sub_6F8A7970(pEditBox);
			Unicode::strlen(pEditBox->wszText);
			Unicode::strcpy(wszNewText, pEditBox->wszText);
		}
		break;

	default:
		if (nTextLength >= pEditBox->nMaxLength - 1)
		{
			return 0;
		}

		if (nTextLength < 255)
		{
			// The tail of the text from the caret goes through an ANSI buffer behind the typed character(s)
			char szInsert[INSERT_BUFFER_SIZE] = {};
			if (nChar == '\r')
			{
				Unicode::unicode2Win(&szInsert[2], pCaret, 256);
				szInsert[0] = '\r';
				szInsert[1] = '\n';
				Unicode::win2Unicode(pCaret, szInsert, 256);
				pCaret += 2;
			}
			else
			{
				Unicode::unicode2Win(&szInsert[1], pCaret, 256);
				szInsert[0] = nChar;
				Unicode::win2Unicode(pCaret, szInsert, 256);
				++pCaret;
			}

			EDITBOX_ClearSelection(pEditBox);
		}
		break;
	}

	if (pEditBox->dwEditBoxFlags & EDITBOX_MULTILINE)
	{
		if (EDITBOX_GetTextHeight(pEditBox, wszNewText) >= pEditBox->controlHeader.nHeight - 2 * pEditBox->nTextInsetY)
		{
			return 1;
		}
	}

	Unicode::strcpy(pEditBox->wszText, wszNewText);
	pEditBox->nCaretPosition = pCaret - wszNewText;
	sub_6F8A98A0(pEditBox);

	if (pEditBox->pfOnChange)
	{
		pEditBox->pfOnChange(0);
	}

	return 1;
}

//D2Win.0x6F8A7970
//1.14d: 0x004FE9F0
void __fastcall sub_6F8A7970(D2WinEditBoxStrc* pEditBox)
{
	Unicode wszBefore[256] = {};
	Unicode wszAfter[256] = {};

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	int32_t nEnd = pEditBox->nSelectionEnd;
	int32_t nStart = pEditBox->nSelectionStart;
	int32_t nSelectionMin = pEditBox->nSelectionStart;

	int32_t nSelectionMax = pEditBox->nSelectionEnd;

	if (nStart >= nEnd)
	{
		nSelectionMin = pEditBox->nSelectionEnd;
		if (nStart > nEnd)
		{
			nSelectionMax = pEditBox->nSelectionStart;
		}
	}

	if (nSelectionMin)
	{
		Unicode::strncpy(wszBefore, pEditBox->wszText, nSelectionMin);
	}
	else
	{
		wszBefore[0] = 0;
	}

	Unicode::strcpy(wszAfter, &pEditBox->wszText[nSelectionMax]);
	Unicode::strcpy(pEditBox->wszText, wszBefore);
	Unicode::strcat(pEditBox->wszText, wszAfter);

	int32_t nTextLength = Unicode::strlen(pEditBox->wszText);
	if (nSelectionMin > nTextLength)
	{
		pEditBox->nCaretPosition = nTextLength;
	}
	else
	{
		pEditBox->nCaretPosition = nSelectionMin;
	}

	EDITBOX_ClearSelection(pEditBox);
}

//D2Win.0x6F8A7AB0
//1.14d: 0x004FF050
int32_t __fastcall EDITBOX_HandleKeyDown(D2WinEditBoxStrc* pEditBox, char nVirtualKey)
{
	char nKey = nVirtualKey;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (!(pEditBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pEditBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	int32_t nCaret = pEditBox->nCaretPosition;
	BOOL bSelectionDeleted = FALSE;
	BOOL bTextChanged = FALSE;
	const BOOL bShift = GetKeyState(VK_SHIFT) < 0;

	switch (nKey)
	{
	case VK_BACK:
	case VK_DELETE:
		if (EDITBOX_HasSelection(pEditBox))
		{
			sub_6F8A7970(pEditBox);
			bSelectionDeleted = TRUE;
			bTextChanged = TRUE;
			nCaret = pEditBox->nCaretPosition;
		}
		break;

	case VK_END:
	case VK_HOME:
	case VK_LEFT:
	case VK_UP:
	case VK_RIGHT:
	case VK_DOWN:
		if (bShift)
		{
			if (pEditBox->nSelectionStart == -1)
			{
				pEditBox->nSelectionStart = pEditBox->nCaretPosition;
				pEditBox->nSelectionEnd = pEditBox->nCaretPosition;
			}
		}
		else
		{
			EDITBOX_ClearSelection(pEditBox);
		}
		break;

	default:
		break;
	}

	Unicode* wszText = pEditBox->wszText;

	switch (nKey)
	{
	case VK_BACK:
		if (nCaret <= 0 || bSelectionDeleted)
		{
			break;
		}

		{
			const size_t nSize = sizeof(Unicode) * (Unicode::strlen(&wszText[nCaret]) + 1);
			--nCaret;
			memmove(&wszText[nCaret], &wszText[nCaret + 1], nSize);
			// A CR in front of the deleted character (the LF of a CR LF) goes too
			if (nCaret > 0 && wszText[nCaret - 1] == '\r')
			{
				--nCaret;
				memmove(&wszText[nCaret], &wszText[nCaret + 1], nSize);
			}
		}
		bTextChanged = TRUE;
		break;

	case VK_TAB:
	{
		D2WinEditBoxStrc* pTarget = nullptr;
		if (GetKeyState(VK_SHIFT) < 0)
		{
			pTarget = pEditBox->pPrevious;
			if (!pTarget)
			{
				return 1;
			}

			do
			{
				if (pTarget == pEditBox || (gdwBitMasks[2] & pTarget->controlHeader.dwFlags && pTarget->controlHeader.dwFlags & gdwBitMasks[0]))
				{
					break;
				}

				pTarget = pTarget->pPrevious;
			}
			while (pTarget);
		}
		else
		{
			pTarget = pEditBox->pNext;
			if (!pTarget)
			{
				return 1;
			}

			do
			{
				if (pTarget == pEditBox || (gdwBitMasks[2] & pTarget->controlHeader.dwFlags && pTarget->controlHeader.dwFlags & gdwBitMasks[0]))
				{
					break;
				}

				pTarget = pTarget->pNext;
			}
			while (pTarget);
		}

		// A chain that ends in nullptr before reaching an enabled box asserts in D2Win_10082 (both binaries)
		CONTROL_SetFocusedEditBox(pTarget);
		D2Win_10082(pTarget);
		return 1;
	}

	case VK_RETURN:
		if (!pEditBox->pfOnReturn)
		{
			break;
		}

		{
			const uint32_t bKeepEditing = pEditBox->dwEditBoxFlags & EDITBOX_RETURN_KEEPS_EDITING;
			++dword_6F8FE254;
			pEditBox->pfOnReturn(pEditBox->wszText);
			--dword_6F8FE254;
			if (!bKeepEditing)
			{
				return 1;
			}
		}
		nCaret = pEditBox->nCaretPosition;
		break;

	case VK_ESCAPE:
		if (pEditBox->pfOnReturn)
		{
			++dword_6F8FE254;
			pEditBox->pfOnReturn(nullptr);
			--dword_6F8FE254;
		}
		return 1;

	case VK_END:
		while (wszText[nCaret])
		{
			++nCaret;
		}

		if (bShift)
		{
			pEditBox->nSelectionEnd = nCaret;
		}
		break;

	case VK_HOME:
		nCaret = 0;
		if (bShift)
		{
			pEditBox->nSelectionEnd = 0;
		}
		break;

	case VK_LEFT:
		--nCaret;
		if (wszText[nCaret] == '\n')
		{
			--nCaret;
		}

		if (nCaret >= 0)
		{
			if (bShift && pEditBox->nSelectionStart != -1)
			{
				pEditBox->nSelectionEnd = nCaret;
			}
		}
		else
		{
			nCaret = 0;
		}
		break;

	case VK_UP:
		if (pEditBox->dwEditBoxFlags & EDITBOX_MULTILINE)
		{
			int32_t nX = 0;
			int32_t nY = 0;
			EDITBOX_GetCaretPosition(pEditBox, &nX, &nY);

			SMSGHANDLER_PARAMS msg = {};
			msg.hWindow = (HWND)pEditBox;
			msg.nMessage = 0;
			EDITBOX_PlaceMultiLineCaret(&msg, nX, nY - D2Win_10125());
			nCaret = pEditBox->nCaretPosition;
		}
		else
		{
			nCaret = 0;
			if (bShift)
			{
				pEditBox->nSelectionEnd = 0;
			}
		}
		break;

	case VK_RIGHT:
		if (wszText[nCaret])
		{
			const Unicode nNext = wszText[nCaret + 1];
			++nCaret;
			if (nNext == '\n')
			{
				++nCaret;
			}

			if (bShift && pEditBox->nSelectionStart != -1)
			{
				pEditBox->nSelectionEnd = nCaret;
			}
		}
		break;

	case VK_DOWN:
		if (pEditBox->dwEditBoxFlags & EDITBOX_MULTILINE)
		{
			int32_t nX = 0;
			int32_t nY = 0;
			EDITBOX_GetCaretPosition(pEditBox, &nX, &nY);

			SMSGHANDLER_PARAMS msg = {};
			msg.hWindow = (HWND)pEditBox;
			msg.nMessage = 0;
			EDITBOX_PlaceMultiLineCaret(&msg, nX, nY + D2Win_10125());
			nCaret = pEditBox->nCaretPosition;
		}
		else
		{
			while (wszText[nCaret])
			{
				++nCaret;
			}

			if (bShift)
			{
				pEditBox->nSelectionEnd = nCaret;
			}
		}
		break;

	case VK_DELETE:
		if (wszText[nCaret] && !bSelectionDeleted)
		{
			const int32_t nCount = Unicode::strlen(&wszText[nCaret + 1]) + 1;
			memmove(&wszText[nCaret], &wszText[nCaret + 1], sizeof(Unicode) * nCount);
			// Deleting the CR of a CR LF deletes the LF too
			if (wszText[nCaret] == '\n')
			{
				memmove(&wszText[nCaret], &wszText[nCaret + 1], sizeof(Unicode) * nCount - sizeof(Unicode));
			}
			bTextChanged = TRUE;
		}
		break;

	case VK_F1:
		return 0;

	default:
		break;
	}

	pEditBox->nCaretPosition = nCaret;
	sub_6F8A98A0(pEditBox);

	if (bTextChanged && pEditBox->pfKeyFilter && !pEditBox->pfKeyFilter(pEditBox, pEditBox->nCaretPosition, &nKey) && nKey != VK_BACK && nKey != VK_DELETE)
	{
		return 0;
	}

	if (pEditBox->pfOnChange)
	{
		pEditBox->pfOnChange(0);
	}

	return 1;
}

//D2Win.0x6F8A80A0 (#10081)
//1.14d: 0x004FDD50
void __fastcall EDITBOX_SetMaxLength(D2WinEditBoxStrc* pEditBox, int32_t nMaxLength)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->nMaxLength = nMaxLength;
}

//D2Win.0x6F8A80F0 (#10076)
//1.14d: 0x004FFE10
int32_t __fastcall D2Win_10076(D2WinEditBoxStrc* pEditBox, const char* szText)
{
	Unicode wszText[256] = {};

	Unicode::win2Unicode(wszText, szText, 256);
	return EDITBOX_SetText(pEditBox, wszText);
}

//D2Win.0x6F8A8140 (#10075)
//1.14d: 0x004FF5A0
int32_t __fastcall EDITBOX_SetText(D2WinEditBoxStrc* pEditBox, const Unicode* pText)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->nCaretPosition = Unicode::strlen(pText);
	if (pEditBox->nCaretPosition)
	{
		Unicode::strcpy(pEditBox->wszText, pText);
	}
	else
	{
		pEditBox->wszText[0] = 0;
	}

	sub_6F8A98A0(pEditBox);

	if (pEditBox->pfOnChange)
	{
		pEditBox->pfOnChange(0);
	}

	return 1;
}

//D2Win.0x6F8A81E0 (#10077)
//1.14d: 0x004FDD90
const Unicode* __fastcall D2Win_10077_EDITBOX_GetText(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	return pEditBox->wszText;
}

//D2Win.0x6F8A8230 (#10080)
//1.14d: 0x004FDDD0
void __fastcall D2Win_10080_EDITBOX_SetNextEditBox(D2WinEditBoxStrc* pEditBox, D2WinEditBoxStrc* pNext)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	D2_ASSERT(pNext);
	D2_ASSERT(pNext->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->pNext = pNext;
	pNext->pPrevious = pEditBox;
}

//D2Win.0x6F8A82D0
//1.14d: 0x004FFE50
int32_t __fastcall EDITBOX_DrawSingleLine(D2WinControlStrc* pControl)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pControl;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (pEditBox->controlHeader.dwFlags & gdwBitMasks[2] && pEditBox->controlHeader.dwFlags & gdwBitMasks[0])
	{
		if (!D2Win_IsMouseButtonPressed())
		{
			pEditBox->bMouseSelecting = 0;
		}

		if (pEditBox->bMouseSelecting)
		{
			SMSGHANDLER_PARAMS msg = {};
			msg.hWindow = (HWND)pEditBox;
			msg.nMessage = WM_LBUTTONDOWN;
			EDITBOX_HandleSingleLineMouseDown(&msg);
		}

		if (pEditBox->controlHeader.pCellFile)
		{
			D2GfxDataStrc gfxData = {};
			gfxData.pCellFile = pEditBox->controlHeader.pCellFile;
			gfxData.nDirection = 0;
			gfxData.nFrame = 0;
			TEXTURE_CelDraw(&gfxData, pEditBox->controlHeader.nImageX, pEditBox->controlHeader.nImageY - 1, -1, DRAWMODE_NORMAL, 0);
		}

		D2Win_10127_SetFont(pEditBox->eFont);

		int32_t nX = pEditBox->controlHeader.nImageX + pEditBox->nTextInsetX;
		const int32_t nY = pEditBox->nTextInsetY - pEditBox->controlHeader.nHeight + pEditBox->controlHeader.nImageY + 16;

		Unicode wszVisible[300] = {};
		Unicode::strncpy(wszVisible, &pEditBox->wszText[pEditBox->nFirstVisibleChar], pEditBox->nLastVisibleChar - pEditBox->nFirstVisibleChar + 1);

		//const int32_t v50 = Unicode::strlen(wszVisible);
		if (pEditBox->dwEditBoxFlags & EDITBOX_PASSWORD)
		{
			Unicode wszStar[2] = {};
			Unicode::win2Unicode(wszStar, "*", 2);

			//TODO: Set text
			//if (v50 > 0)
			//{
			//	LOWORD(pEditBox2) = wszStar[0];
			//	pEditBox3 = pEditBox2 << 16;
			//	LOWORD(pEditBox3) = wszStar[0];
			//	pEditBox4 = v50 & 1;
			//	pEditBox5 = v50 >> 1;
			//	memset32(wszVisible, pEditBox3, v50 >> 1);
			//	pEditBox6 = &wszVisible[2 * pEditBox5];
			//	for (i = pEditBox4; i; --i)
			//	{
			//		*pEditBox6 = pEditBox3;
			//		++pEditBox6;
			//	}
			//}
		}

		if (pEditBox->nSelectionStart == -1 || pEditBox->nSelectionStart == pEditBox->nSelectionEnd)
		{
			D2Win_10117_DrawText(wszVisible, nX, nY, pEditBox->nTextColor, 0);
		}
		else
		{
			Unicode wszSelected[256] = {};
			Unicode wszBefore[256] = {};
			Unicode wszAfter[256] = {};

			int32_t nSelectionMin = pEditBox->nSelectionStart;
			if (pEditBox->nSelectionStart >= pEditBox->nSelectionEnd)
			{
				nSelectionMin = pEditBox->nSelectionEnd;
			}

			int32_t nBeforeLength = 0;
			if (nBeforeLength > nSelectionMin - pEditBox->nFirstVisibleChar)
			{
				nBeforeLength = nSelectionMin - pEditBox->nFirstVisibleChar;
			}

			int32_t nSelectionMax = pEditBox->nSelectionEnd;
			if (pEditBox->nSelectionStart > nSelectionMax)
			{
				nSelectionMax = pEditBox->nSelectionStart;
			}

			int32_t nSelectedLength = 0;
			if (nSelectionMax - pEditBox->nFirstVisibleChar - nBeforeLength > 0)
			{
				nSelectedLength = nSelectionMax - pEditBox->nFirstVisibleChar - nBeforeLength;
			}

			Unicode::strncpy(wszBefore, wszVisible, nBeforeLength);
			Unicode::strncpy(wszSelected, &wszVisible[nBeforeLength], nSelectedLength);
			Unicode::strcpy(wszAfter, &wszVisible[nBeforeLength + nSelectedLength]);

			int32_t nBeforeWidth = 0;
			int32_t nSelectedWidth = 0;
			int32_t nUnused = 0;
			D2Win_10131_GetTextDimensions(wszBefore, &nBeforeWidth, &nUnused);
			D2Win_10131_GetTextDimensions(wszSelected, &nSelectedWidth, &nUnused);
			D2Win_10117_DrawText(wszBefore, nX, nY, pEditBox->nTextColor, 0);
			D2Win_10133(wszSelected, nX + nBeforeWidth, nY, D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u), DRAWMODE_NORMAL, pEditBox->nTextColor);
			D2Win_10117_DrawText(wszAfter, nX + nBeforeWidth + nSelectedWidth, nY, pEditBox->nTextColor, 0);
		}

		if (CONTROL_GetFocusedEditBox() == pEditBox && GetTickCount() / 250 & 1)
		{
			Unicode wszCaret[2] = {};
			Unicode::win2Unicode(wszCaret, "_", 2);
			if (pEditBox->nCaretPosition != pEditBox->nFirstVisibleChar)
			{
				nX += D2Win_10122(wszVisible, pEditBox->nCaretPosition - pEditBox->nFirstVisibleChar);
			}

			D2Win_10117_DrawText(wszCaret, nX, nY, pEditBox->nTextColor, 0);
		}
	}
	return 1;
}

//D2Win.0x6F8A86C0) --------------------------------------------------------
//1.14d: 0x004FF620
int32_t __fastcall EDITBOX_DrawMultiLine(D2WinControlStrc* pControl)
{
	//D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pControl;

//	int v5; // eax@18
//	int v6; // esi@18
//	unsigned int v9; // eax@21
//	char *v12; // ebp@25
//	char *v13; // ebx@25
//	int v14; // eax@25
//	int v15; // edx@25
//	char v16; // al@29
//	int v17; // edi@32
//	int v18; // eax@32
//	int v19; // eax@37
//	int v20; // ecx@38
//	int v21; // edx@39
//	int v22; // eax@43
//	int v23; // ecx@43
//	int v24; // edi@43
//	size_t v26; // edi@55
//	int v27; // edx@59
//	int v28; // eax@61
//	int v29; // ebp@64
//	int v30; // edi@68
//	int v31; // eax@68
//	int v32; // edi@77
//	int v33; // eax@77
//	int nX; // [sp+18h] [bp-6C4h]@18
//	int v38; // [sp+1Ch] [bp-6C0h]@41
//	int v39; // [sp+1Ch] [bp-6C0h]@49
//	int v42; // [sp+28h] [bp-6B4h]@29
//	int Count; // [sp+2Ch] [bp-6B0h]@29
//	int pWidth; // [sp+30h] [bp-6ACh]@45
//	int v45; // [sp+34h] [bp-6A8h]@21
//	int v46; // [sp+38h] [bp-6A4h]@23
//	int v47; // [sp+3Ch] [bp-6A0h]@25
//	int nY; // [sp+40h] [bp-69Ch]@18
//	int v49; // [sp+44h] [bp-698h]@25
//	int v52; // [sp+50h] [bp-68Ch]@43
//	int v53; // [sp+54h] [bp-688h]@53
//	int pHeight; // [sp+58h] [bp-684h]@45
//	int v55; // [sp+5Ch] [bp-680h]@20
//	int v56; // [sp+60h] [bp-67Ch]@25
//	char Dest[256]; // [sp+84h] [bp-658h]@29
//	char a2[256]; // [sp+184h] [bp-558h]@25
//
//	D2_ASSERT(pEditBox);
//	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);
//
//	if (pEditBox->controlHeader.dwFlags & gdwBitMasks[2] && pEditBox->controlHeader.dwFlags & gdwBitMasks[0])
//	{
//		if (!D2Win_IsMouseButtonPressed())
//		{
//			pEditBox->field_280 = 0;
//		}
//
//		if (pEditBox->field_280)
//		{
//			SMSGHANDLER_PARAMS msg = {};
//
//			msg.hWindow = (HWND)pEditBox;
//			msg.uMessage = WM_LBUTTONDOWN;
//
//			EDITBOX_PlaceMultiLineCaret(&msg, gMousePosition_6F8FE234.x - pEditBox->field_40 - pEditBox->controlHeader.nImageX, gMousePosition_6F8FE234.y + pEditBox->controlHeader.nHeight - pEditBox->field_44 - pEditBox->controlHeader.nImageY);
//		}
//
//		Unicode v61[300] = {};
//
//		D2Win_10127_SetFont(pEditBox->nFont);
//
//		nX = pEditBox->field_40 + pEditBox->controlHeader.nImageX;
//		v6 = pEditBox->field_44 + pEditBox->controlHeader.nImageY - pEditBox->controlHeader.nHeight + D2Win_10125();
//		nY = v6;
//
//		Unicode::strcpy(v61, pEditBox->wszText);
//
//		Unicode wszText[2] = {};
//
//		Unicode::win2Unicode(wszText, "_", 2);
//
//		v55 = D2Win_10122(wszText, 2);
//		if (CONTROL_GetFocusedEditBox() != pEditBox || !(GetTickCount() / 250 & 1))
//		{
//			v45 = 0;
//		}
//		else
//		{
//			v45 = 1;
//		}
//
//		v46 = 0;
//
//		Unicode v60[256] = {};
//
//		D2LANG_unicode2Win(a2, pEditBox->wszText);
//
//		v12 = a2;
//		v13 = a2;
//		v14 = D2Win_10125();
//		v15 = pEditBox->field_40;
//		v47 = v14;
//		v56 = pEditBox->controlHeader.nWidth - 2 * v15;
//		v49 = pEditBox->controlHeader.nHeight - 2 * pEditBox->field_44;
//
//		if (a2[0] || v45 && (D2Win_10117_DrawText(wszText, nX, nY, pEditBox->nTextColor, 0), v46 = 1, a2[0]))
//		{
//			while (1)
//			{
//				if (v6 - nY >= v49)
//				{
//					return 1;
//				}
//
//				Count = v13 - v12;
//				memset(Dest, 0, sizeof(Dest));
//				v16 = *v13;
//				v42 = 0;
//				if (v16)
//				{
//					while (v16 != '\r' && v16 != '\n')
//					{
//						v17 = Count;
//						Dest[Count] = v16;
//						Unicode::win2Unicode(v60, Dest, 256);
//						v18 = D2Win_10122(v60, v17);
//						if (v55 + v18 > v56)
//						{
//							v42 = 1;
//							break;
//						}
//
//						v16 = (v13++)[1];
//						Count = v17 + 1;
//						if (!v16)
//						{
//							break;
//						}
//					}
//				}
//
//				v19 = pEditBox->field_54;
//				if (v19 != -1)
//				{
//					v20 = pEditBox->nTextLength;
//					if (v19 != v20)
//					{
//						break;
//					}
//				}
//LABEL_57:
//				strncpy(Dest, v12, Count);
//				Dest[v13 - v12] = 0;
//				Unicode::win2Unicode(v60, Dest, 256);
//				D2Win_10117_DrawText(v60, nX, v6, pEditBox->nTextColor, 0);
//				if (v45)
//				{
//					if (!v46)
//					{
//						v27 = (int)&a2[pEditBox->field_25C - (_DWORD)v12];
//						if (v27 >= 0)
//						{
//							if (v27 <= Count)
//							{
//								v28 = nX;
//								if (v27)
//								{
//									v28 = nX + D2Win_10122(v60, v27);
//								}
//
//								D2Win_10117_DrawText(wszText, v28, v6, pEditBox->nTextColor, 0);
//								v46 = 1;
//							}
//						}
//					}
//				}
//
//				v29 = v47;
//				if (v42)
//				{
//					v6 += v47;
//				}
//
//				if (*v13 == '\r')
//				{
//					if (v13[1] == '\n')
//					{
//						v30 = v13 - a2;
//						v31 = v6 - nY;
//						do
//						{
//							if (v13[1] != '\n')
//							{
//								break;
//							}
//
//							v13 += 2;
//							v30 += 2;
//							v6 += v29;
//							v31 += v29;
//							v42 = v31;
//							if (v45 && !v46 && pEditBox->field_25C == v30 && v31 < v49)
//							{
//								D2Win_10117_DrawText(wszText, nX, v6, pEditBox->nTextColor, 0);
//								v31 = v42;
//							}
//						}
//						while (*v13 == '\r');
//					}
//					else
//					{
//						v32 = v13 - a2;
//						v33 = v6 - nY;
//						do
//						{
//							++v13;
//							++v32;
//							v6 += v29;
//							v33 += v29;
//							v42 = v33;
//							if (v45 && !v46 && pEditBox->field_25C == v32 && v33 < v49)
//							{
//								D2Win_10117_DrawText(wszText, nX, v6, pEditBox->nTextColor, 0);
//								v33 = v42;
//							}
//						}
//						while (*v13 == '\r');
//					}
//				}
//
//				v12 = v13;
//				if (!*v13)
//				{
//					return 1;
//				}
//			}
//
//			v21 = pEditBox->field_54;
//			if (v19 >= v20)
//			{
//				v21 = pEditBox->nTextLength;
//			}
//
//			v38 = pEditBox->field_54;
//			if (v19 <= v20)
//			{
//				v38 = pEditBox->nTextLength;
//			}
//
//			v22 = v12 - a2;
//			v23 = v13 - a2;
//			v52 = nX;
//			v24 = v38;
//			if (v21 >= (signed int)(v12 - a2))
//			{
//				if (v21 < v23)
//				{
//LABEL_49:
//					v39 = v21;
//					if (v21 <= v22)
//					{
//						v39 = v12 - a2;
//					}
//
//					if (v24 >= v23)
//					{
//						v24 = v13 - a2;
//					}
//
//					v53 = v39 - v22;
//					if (v39 != v22)
//					{
//						strncpy(Dest, v12, v39 - v22);
//						Dest[v53] = 0;
//						Unicode::win2Unicode(v60, Dest, 256);
//						D2Win_10131_GetTextDimensions(v60, &pWidth, &pHeight);
//						v52 = nX + pWidth;
//					}
//
//					v26 = v24 - v39;
//					strncpy(Dest, &v12[v53], v26);
//					Dest[v26] = 0;
//					Unicode::win2Unicode(v60, Dest, 256);
//					D2Win_10131_GetTextDimensions(v60, &pWidth, &pHeight);
//					D2GFX_10055_DrawSolidRectEx(v52, v6 - v47, v52 + pWidth, v6, D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u), DRAWMODE_NORMAL);
//					goto LABEL_57;
//				}
//			}
//			else
//			{
//				if (v38 >= v23)
//				{
//					strncpy(Dest, v12, Count);
//					Dest[v13 - v12] = 0;
//
//					Unicode::win2Unicode(v60, Dest, 256);
//
//					D2Win_10131_GetTextDimensions(v60, &pWidth, &pHeight);
//					D2GFX_10055_DrawSolidRectEx(nX, v6 - v47, nX + pWidth, v6, D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u), DRAWMODE_NORMAL);
//					goto LABEL_57;
//				}
//			}
//
//			if (v38 < v22 || v38 >= v23)
//			{
//				goto LABEL_57;
//			}
//
//			goto LABEL_49;
//		}
//	}
//
	return 1;
}

//D2Win.0x6F8A8D60
//1.14d: 0x004FDFF0
void __fastcall EDITBOX_GetCaretPosition(D2WinEditBoxStrc* pEditBox, int32_t* pX, int32_t* pY)
{
	//Unicode v48[300] = {};

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	D2Win_10127_SetFont(pEditBox->eFont);

	const int32_t nStartX = pEditBox->controlHeader.nImageX + pEditBox->nTextInsetX;
	const int32_t nLineHeight = D2Win_10125();
	const int32_t nStartY = pEditBox->nTextInsetY + pEditBox->controlHeader.nImageY - pEditBox->controlHeader.nHeight + nLineHeight;
	int32_t nY = nStartY;
	//Unicode::strcpy(v48, pEditBox->wszText);

	Unicode wszCaret[2] = {};
	Unicode::win2Unicode(wszCaret, "_", 2);

	const int32_t nCaretWidth = D2Win_10122(wszCaret, 2);

	char szText[256] = {};
	Unicode::unicode2Win(szText, pEditBox->wszText, ARRAY_SIZE(szText));
	const char* pCurrent = szText;
	const char* pLineStart = szText;
	const int32_t nAvailableWidth = pEditBox->controlHeader.nWidth - 2 * pEditBox->nTextInsetX;
	const int32_t nAvailableHeight = pEditBox->controlHeader.nHeight - 2 * pEditBox->nTextInsetY;

	*pX = -1;
	*pY = -1;

	while (*pCurrent)
	{
		if (nY - nStartY >= nAvailableHeight)
		{
			break;
		}

		int32_t nCount = pCurrent - pLineStart;
		char szLine[256] = {};
		Unicode wszLine[256] = {};
		BOOL bWrapped = 0;
		while (*pCurrent && *pCurrent != '\r' && *pCurrent != '\n')
		{
			szLine[nCount] = *pCurrent;
			Unicode::win2Unicode(wszLine, szLine, 256);
			if (nCaretWidth + D2Win_10122(wszLine, nCount) > nAvailableWidth)
			{
				bWrapped = 1;
				break;
			}

			++pCurrent;
			++nCount;
		}

		const int32_t nCaretColumn = (int32_t)&szText[pEditBox->nCaretPosition - (int32_t)pLineStart];
		if (*pX == -1 && nCaretColumn >= 0 && nCaretColumn <= nCount)
		{
			*pX = nStartX;
			if (nCaretColumn)
			{
				Unicode::win2Unicode(wszLine, szLine, 256);
				*pX += D2Win_10122(wszLine, nCaretColumn);
			}
			*pY = nY;
		}

		if (bWrapped)
		{
			nY += nLineHeight;
		}

		if (*pCurrent == '\r')
		{
			if (pCurrent[1] == '\n')
			{
				int32_t nPosition = pCurrent - szText;
				int32_t nOffsetY = nY - nStartY;
				do
				{
					if (pCurrent[1] != '\n')
					{
						break;
					}
					pCurrent += 2;
					nPosition += 2;
					nY += nLineHeight;
					nOffsetY += nLineHeight;
					if (*pX == -1 && pEditBox->nCaretPosition == nPosition && nOffsetY < nAvailableHeight)
					{
						*pX = nStartX;
						*pY = nY;
					}
				}
				while (*pCurrent == '\r');
			}
			else
			{
				int32_t nPosition = pCurrent - szText;
				int32_t nOffsetY = nY - nStartY;
				do
				{
					++pCurrent;
					++nPosition;
					nY += nLineHeight;
					nOffsetY += nLineHeight;
					if (*pX == -1 && pEditBox->nCaretPosition == nPosition && nOffsetY < nAvailableHeight)
					{
						*pX = nStartX;
						*pY = nY;
					}
				}
				while (*pCurrent == '\r');
			}
		}
		pLineStart = pCurrent;
	}

	if (*pX != -1)
	{
		*pX -= pEditBox->controlHeader.nImageX - 1;
		*pY += pEditBox->controlHeader.nHeight - pEditBox->controlHeader.nImageY - 1;

		*pX = D2Clamp(*pX, pEditBox->nTextInsetX, pEditBox->controlHeader.nWidth - pEditBox->nTextInsetX - 1);
		*pY = D2Clamp(*pX, pEditBox->nTextInsetY, pEditBox->controlHeader.nHeight - pEditBox->nTextInsetY - 1);
	}
}

//D2Win.0x6F8A9120
//1.14d: 0x004FFBF0
int32_t __stdcall EDITBOX_HandleSingleLineMouseDown(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (!(pEditBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pEditBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	CONTROL_SetFocusedEditBox(pEditBox);

	const int32_t nX = gMousePosition_6F8FE234.x - pEditBox->nTextInsetX - pEditBox->controlHeader.nImageX;
	const int32_t nY = pEditBox->controlHeader.nHeight - pEditBox->nTextInsetY - pEditBox->controlHeader.nImageY + gMousePosition_6F8FE234.y;

	const bool bInside = nX >= -pEditBox->nTextInsetX && nX < pEditBox->controlHeader.nWidth - pEditBox->nTextInsetX && nY > -pEditBox->nTextInsetY && nY < pEditBox->controlHeader.nHeight - pEditBox->nTextInsetY;
	const uint32_t dwTickCount = GetTickCount();

	if (bInside && !pEditBox->bMouseSelecting && pEditBox == dword_6F8FD8B0 && dwTickCount - dword_6F8FD8B4 < 300)
	{
		D2Win_10082(pEditBox);
	}
	else
	{
		if (pEditBox->bMouseSelecting || bInside)
		{
			D2Win_10127_SetFont(pEditBox->eFont);

			if (pEditBox->bMouseSelecting)
			{
				if (nX < -pEditBox->nTextInsetX)
				{
					if (pEditBox->nCaretPosition - 1 < 0)
					{
						pEditBox->nCaretPosition = 0;
					}
					else
					{
						--pEditBox->nCaretPosition;
					}

					sub_6F8A98A0(pEditBox);
				}
				else if (nX >= pEditBox->controlHeader.nWidth - pEditBox->nTextInsetX)
				{
					const int nLength = Unicode::strlen(pEditBox->wszText) - 1;
					if (pEditBox->nCaretPosition + 1 > nLength)
					{
						pEditBox->nCaretPosition = nLength;
					}
					else
					{
						++pEditBox->nCaretPosition;
					}

					sub_6F8A98A0(pEditBox);
				}
			}

			int nCounter = 1;
			while (nX > D2Win_10122(&pEditBox->wszText[pEditBox->nFirstVisibleChar], nCounter))
			{
				if (nCounter > Unicode::strlen(&pEditBox->wszText[pEditBox->nFirstVisibleChar]))
				{
					break;
				}
				++nCounter;
			}

			const int nOldValue = pEditBox->nCaretPosition;
			pEditBox->nCaretPosition = pEditBox->nFirstVisibleChar + nCounter - 1;

			if (GetKeyState(VK_SHIFT) & 0x8000 || pEditBox->bMouseSelecting)
			{
				if (pEditBox->nSelectionStart == -1)
				{
					pEditBox->nSelectionStart = nOldValue;
				}

				pEditBox->nSelectionEnd = pEditBox->nCaretPosition;
			}
			else
			{
				EDITBOX_ClearSelection(pEditBox);
			}
		}

		if (pMsg->nMessage == WM_LBUTTONDOWN)
		{
			pEditBox->bMouseSelecting = 1;
		}
	}

	dword_6F8FD8B0 = pEditBox;
	dword_6F8FD8B4 = dwTickCount;

	if (pEditBox->controlHeader.pfCallback)
	{
		return pEditBox->controlHeader.pfCallback(pMsg);
	}

	return 0;
}

//D2Win.0x6F8A93A0
//1.14d: 0x004FE350
int32_t __stdcall EDITBOX_HandleMouseUp(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (pEditBox->bMouseSelecting)
	{
		pEditBox->bMouseSelecting = 0;
		return 1;
	}

	return 0;
}

//D2Win.0x6F8A9410
//1.14d: 0x004FE3B0
int32_t __fastcall EDITBOX_PlaceMultiLineCaret(SMSGHANDLER_PARAMS* pMsg, int32_t nX, int32_t nY)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (!(pEditBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pEditBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	CONTROL_SetFocusedEditBox(pEditBox);

	if (pEditBox->bMouseSelecting)
	{
		nX = D2Clamp(nX, pEditBox->nTextInsetX, pEditBox->controlHeader.nWidth - pEditBox->nTextInsetX - 1);
		nY = D2Clamp(nY, pEditBox->nTextInsetY, pEditBox->controlHeader.nHeight - pEditBox->nTextInsetY - 1);
	}

	if (nX >= -pEditBox->nTextInsetX && nX < pEditBox->controlHeader.nWidth - pEditBox->nTextInsetX && nY >= -pEditBox->nTextInsetY && nY < pEditBox->controlHeader.nHeight - pEditBox->nTextInsetY)
	{
		D2Win_10127_SetFont(pEditBox->eFont);

		Unicode wszCaret[2] = {};
		Unicode::win2Unicode(wszCaret, "_", 2);

		const int32_t nCaretWidth = D2Win_10121_GetTextWidth(wszCaret);

		char szText[256] = {};
		Unicode::unicode2Win(szText, pEditBox->wszText, ARRAY_SIZE(szText));
		const char* pLineStart = szText;
		const char* pCurrent = szText;
		int32_t nLineY = 0;
		const int32_t nLineHeight = D2Win_10125();
		int32_t bLineHit = 0;
		int32_t nPreviousCaret = -1;

		while (*pCurrent)
		{
			char szLine[256] = {};
			Unicode wszLine[256] = {};

			int32_t bWrapped = 0;
			int32_t nCount = pCurrent - pLineStart;
			while (*pCurrent && *pCurrent != '\r' && *pCurrent != '\n')
			{
				szLine[nCount] = *pCurrent;
				Unicode::win2Unicode(wszLine, szLine, 256);
				if (nCaretWidth + D2Win_10122(wszLine, nCount) > pEditBox->controlHeader.nWidth - 2 * pEditBox->nTextInsetX)
				{
					bWrapped = 1;
					break;
				}
				++pCurrent;
				++nCount;
			}

			if (nY >= nLineY && nY < nLineHeight + nLineY)
			{
				int32_t nColumn = 1;
				Unicode::win2Unicode(wszLine, szLine, 256);
				while (nX > D2Win_10122(wszLine, nColumn))
				{
					if (nColumn > nCount)
					{
						break;
					}
					++nColumn;
				}
				nPreviousCaret = pEditBox->nCaretPosition;
				bLineHit = 1;
				pEditBox->nCaretPosition = (int32_t)&pLineStart[nColumn - (int32_t)szText - 1];
			}

			if (!bWrapped)
			{
				if (*pCurrent == '\r')
				{
					if (pCurrent[1] == '\n')
					{
						pCurrent += 2;
					}
					else
					{
						++pCurrent;
					}
					nLineY += nLineHeight;
				}
			}
			else
			{
				nLineY += nLineHeight;
			}

			pLineStart = pCurrent;
		}

		int32_t nOldCaret = nPreviousCaret;
		if (!bLineHit)
		{
			nOldCaret = pEditBox->nCaretPosition;
			pEditBox->nCaretPosition = Unicode::strlen(pEditBox->wszText);
		}

		if ((GetKeyState(VK_SHIFT) & 0x8000 || pEditBox->bMouseSelecting) && nOldCaret != -1)
		{
			if (pEditBox->nSelectionStart == -1)
			{
				pEditBox->nSelectionStart = nOldCaret;
			}
			pEditBox->nSelectionEnd = pEditBox->nCaretPosition;
		}
		else
		{
			EDITBOX_ClearSelection(pEditBox);
		}
	}

	if (pMsg->nMessage == WM_LBUTTONDOWN)
	{
		pEditBox->bMouseSelecting = 1;
	}

	if (pEditBox->controlHeader.pfCallback)
	{
		return pEditBox->controlHeader.pfCallback(pMsg);
	}

	return 0;
}

//D2Win.0x6F8A97D0
//1.14d: 0x004FE710
int32_t __stdcall sub_6F8A97D0(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	return EDITBOX_PlaceMultiLineCaret(pMsg, gMousePosition_6F8FE234.x - pEditBox->nTextInsetX - pEditBox->controlHeader.nImageX, gMousePosition_6F8FE234.y + pEditBox->controlHeader.nHeight - pEditBox->nTextInsetY - pEditBox->controlHeader.nImageY);
}

//D2Win.0x6F8A9850 (#10078)
//1.14d: 0x004FE770
BOOL __stdcall D2Win_10078_IsNumber(int32_t nUnused1, int32_t nUnused2, char* szChar)
{
	return *szChar >= '0' && *szChar <= '9';
}

//D2Win.0x6F8A9870 (#10079)
//1.14d: 0x004FE790
BOOL __stdcall D2Win_10079_IsLetter(int nUnused1, int nUnused2, char* szChar)
{
	return *szChar >= 'a' && *szChar <= 'z' || *szChar >= 'A' && *szChar <= 'Z';
}

//D2Win.0x6F8A98A0
//1.14d: 0x004FE7C0
void __fastcall sub_6F8A98A0(D2WinEditBoxStrc* pEditBox)
{
	D2Win_10127_SetFont(pEditBox->eFont);

	const int32_t nTextLength = Unicode::strlen(pEditBox->wszText);

	Unicode wszText[300] = {};
	if (pEditBox->dwEditBoxFlags & EDITBOX_PASSWORD)
	{
		Unicode wszStar[2] = {};
		Unicode::win2Unicode(wszStar, "*", 2);
		if (nTextLength > 0)
		{
			//TODO: Set text

			//LOWORD(v7) = wszStar;
			//v8 = v7 << 16;
			//LOWORD(v8) = wszStar;
			//memset32(wszText, v8, (unsigned int)nTextLength >> 1);
			//v9 = &wszText[2 * ((unsigned int)nTextLength >> 1)];
			//for (int i = nTextLength & 1; i; --i)
			//{
			//	*v9 = v8;
			//	++v9;
			//}
		}
	}
	else
	{
		Unicode::strcpy(wszText, pEditBox->wszText);
	}

	Unicode wszCaret[2] = {};
	Unicode::win2Unicode(wszCaret, "_", 2);

	const int32_t nCaretWidth = D2Win_10121_GetTextWidth(wszCaret);

	int32_t nTextWidth = D2Win_10121_GetTextWidth(wszText);
	if (pEditBox->nCaretPosition == nTextLength)
	{
		nTextWidth += nCaretWidth;
	}

	if (nTextWidth <= pEditBox->controlHeader.nWidth - 2 * pEditBox->nTextInsetX)
	{
		pEditBox->nFirstVisibleChar = 0;
		pEditBox->nLastVisibleChar = nTextLength - 1;
	}
	else
	{
		int32_t nAvailableWidth = pEditBox->controlHeader.nWidth - 2 * pEditBox->nTextInsetX;
		if (pEditBox->nCaretPosition == nTextLength)
		{
			nAvailableWidth -= nCaretWidth;
		}

		if (pEditBox->nCaretPosition > pEditBox->nLastVisibleChar)
		{
			pEditBox->nLastVisibleChar = pEditBox->nCaretPosition;
			pEditBox->nFirstVisibleChar = pEditBox->nCaretPosition - 1;
			while (pEditBox->nFirstVisibleChar)
			{
				if (D2Win_10122(&wszText[pEditBox->nFirstVisibleChar], pEditBox->nLastVisibleChar - pEditBox->nFirstVisibleChar + 1) >= nAvailableWidth)
				{
					break;
				}

				--pEditBox->nFirstVisibleChar;
			}

			++pEditBox->nFirstVisibleChar;
		}
		else if (pEditBox->nCaretPosition > pEditBox->nFirstVisibleChar + 1)
		{
			if (D2Win_10121_GetTextWidth(&wszText[pEditBox->nFirstVisibleChar]) >= nAvailableWidth)
			{
				pEditBox->nLastVisibleChar = pEditBox->nFirstVisibleChar + 1;
				while (pEditBox->nLastVisibleChar < nTextLength)
				{
					if (D2Win_10122(&wszText[pEditBox->nFirstVisibleChar], pEditBox->nLastVisibleChar - pEditBox->nFirstVisibleChar + 1) >= nAvailableWidth)
					{
						break;
					}

					++pEditBox->nLastVisibleChar;
				}

				--pEditBox->nLastVisibleChar;
			}
			else
			{
				pEditBox->nLastVisibleChar = nTextLength - 1;
				pEditBox->nFirstVisibleChar = nTextLength - 2;
				while (pEditBox->nFirstVisibleChar)
				{
					if (D2Win_10121_GetTextWidth(&wszText[pEditBox->nFirstVisibleChar]) >= nAvailableWidth)
					{
						break;
					}

					--pEditBox->nFirstVisibleChar;
				}

				++pEditBox->nFirstVisibleChar;
			}
		}
		else
		{
			const int32_t nFirstChar = std::max(pEditBox->nCaretPosition - 1, 0);

			pEditBox->nFirstVisibleChar = nFirstChar;
			pEditBox->nLastVisibleChar = nFirstChar + 1;
			while (pEditBox->nLastVisibleChar < nTextLength)
			{
				if (D2Win_10122(&wszText[pEditBox->nFirstVisibleChar], pEditBox->nLastVisibleChar - pEditBox->nFirstVisibleChar + 1) >= nAvailableWidth)
				{
					break;
				}

				++pEditBox->nLastVisibleChar;
			}

			--pEditBox->nLastVisibleChar;
		}
	}
}
