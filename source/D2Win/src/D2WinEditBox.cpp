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

		const int32_t nVisibleLength = Unicode::strlen(wszVisible);
		if (pEditBox->dwEditBoxFlags & EDITBOX_PASSWORD)
		{
			Unicode wszStar[2] = {};
			Unicode::win2Unicode(wszStar, "*", 2);

			for (int32_t i = 0; i < nVisibleLength; ++i)
			{
				wszVisible[i] = wszStar[0];
			}
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
			if (nSelectionMin - pEditBox->nFirstVisibleChar > 0)
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

//D2Win.0x6F8A86C0
//1.14d: 0x004FF620
int32_t __fastcall EDITBOX_DrawMultiLine(D2WinControlStrc* pControl)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pControl;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (!(pEditBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pEditBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 1;
	}

	// Drag selection: the mouse-down handler runs every frame while the button is held
	if (!D2Win_IsMouseButtonPressed())
	{
		pEditBox->bMouseSelecting = 0;
	}

	if (pEditBox->bMouseSelecting)
	{
		SMSGHANDLER_PARAMS msg = {};
		msg.hWindow = (HWND)pEditBox;
		msg.nMessage = WM_LBUTTONDOWN;
		sub_6F8A97D0(&msg);
	}

	D2Win_10127_SetFont(pEditBox->eFont);

	const int32_t nX = pEditBox->nTextInsetX + pEditBox->controlHeader.nImageX;
	const int32_t nStartY = pEditBox->nTextInsetY + pEditBox->controlHeader.nImageY - pEditBox->controlHeader.nHeight + D2Win_10125();
	int32_t nY = nStartY;

	const uint8_t nSelectionColor = D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u);

	Unicode wszCaret[2] = {};
	Unicode::win2Unicode(wszCaret, "_", 2);
	const int32_t nCaretWidth = D2Win_10122(wszCaret, 2);

	const BOOL bDrawCaret = CONTROL_GetFocusedEditBox() == pEditBox && GetTickCount() / 250 & 1;
	BOOL bCaretDrawn = FALSE;

	Unicode wszLine[256] = {};
	char szText[256] = {};
	Unicode::unicode2Win(szText, pEditBox->wszText, 256);

	const char* pLineStart = szText;
	const char* pCurrent = szText;
	const int32_t nLineHeight = D2Win_10125();
	const int32_t nAvailableWidth = pEditBox->controlHeader.nWidth - 2 * pEditBox->nTextInsetX;
	const int32_t nAvailableHeight = pEditBox->controlHeader.nHeight - 2 * pEditBox->nTextInsetY;

	if (!szText[0])
	{
		if (bDrawCaret)
		{
			D2Win_10117_DrawText(wszCaret, nX, nY, pEditBox->nTextColor, 0);
		}
		return 1;
	}

	do
	{
		if (nY - nStartY >= nAvailableHeight)
		{
			return 1;
		}

		int32_t nCount = pCurrent - pLineStart;
		char szLine[256] = {};
		BOOL bWrapped = FALSE;
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

		if (pEditBox->nSelectionStart != -1 && pEditBox->nSelectionStart != pEditBox->nSelectionEnd)
		{
			const int32_t nSelectionMin = std::min(pEditBox->nSelectionStart, pEditBox->nSelectionEnd);
			const int32_t nSelectionMax = std::max(pEditBox->nSelectionStart, pEditBox->nSelectionEnd);
			const int32_t nLineStart = pLineStart - szText;
			const int32_t nLineEnd = pCurrent - szText;

			int32_t nWidth = 0;
			int32_t nHeight = 0;

			if (nSelectionMin < nLineStart && nSelectionMax >= nLineEnd)
			{
				// The whole line is selected
				strncpy(szLine, pLineStart, nCount);
				szLine[pCurrent - pLineStart] = 0;
				Unicode::win2Unicode(wszLine, szLine, 256);
				D2Win_10131_GetTextDimensions(wszLine, &nWidth, &nHeight);
				D2GFX_DrawBox(nX, nY - nLineHeight, nX + nWidth, nY, nSelectionColor, DRAWMODE_NORMAL);
			}
			else if ((nSelectionMin >= nLineStart && nSelectionMin < nLineEnd) || (nSelectionMax >= nLineStart && nSelectionMax < nLineEnd))
			{
				const int32_t nStart = std::max(nSelectionMin, nLineStart);
				const int32_t nEnd = std::min(nSelectionMax, nLineEnd);
				const int32_t nPrefixLength = nStart - nLineStart;

				int32_t nBoxX = nX;
				if (nStart != nLineStart)
				{
					strncpy(szLine, pLineStart, nPrefixLength);
					szLine[nPrefixLength] = 0;
					Unicode::win2Unicode(wszLine, szLine, 256);
					D2Win_10131_GetTextDimensions(wszLine, &nWidth, &nHeight);
					nBoxX = nX + nWidth;
				}

				const int32_t nSelectedLength = nEnd - nStart;
				strncpy(szLine, &pLineStart[nPrefixLength], nSelectedLength);
				szLine[nSelectedLength] = 0;
				Unicode::win2Unicode(wszLine, szLine, 256);
				D2Win_10131_GetTextDimensions(wszLine, &nWidth, &nHeight);
				D2GFX_DrawBox(nBoxX, nY - nLineHeight, nBoxX + nWidth, nY, nSelectionColor, DRAWMODE_NORMAL);
			}
		}

		strncpy(szLine, pLineStart, nCount);
		szLine[pCurrent - pLineStart] = 0;
		Unicode::win2Unicode(wszLine, szLine, 256);
		D2Win_10117_DrawText(wszLine, nX, nY, pEditBox->nTextColor, 0);

		if (bDrawCaret && !bCaretDrawn)
		{
			const int32_t nCaretColumn = pEditBox->nCaretPosition - (pLineStart - szText);
			if (nCaretColumn >= 0 && nCaretColumn <= nCount)
			{
				int32_t nCaretX = nX;
				if (nCaretColumn)
				{
					nCaretX = nX + D2Win_10122(wszLine, nCaretColumn);
				}

				D2Win_10117_DrawText(wszCaret, nCaretX, nY, pEditBox->nTextColor, 0);
				bCaretDrawn = TRUE;
			}
		}

		if (bWrapped)
		{
			nY += nLineHeight;
		}

		// The caret on an empty line is drawn without setting bCaretDrawn (both binaries)
		if (*pCurrent == '\r')
		{
			int32_t nPosition = pCurrent - szText;
			if (pCurrent[1] == '\n')
			{
				do
				{
					if (pCurrent[1] != '\n')
					{
						break;
					}

					pCurrent += 2;
					nPosition += 2;
					nY += nLineHeight;
					if (bDrawCaret && !bCaretDrawn && pEditBox->nCaretPosition == nPosition && nY - nStartY < nAvailableHeight)
					{
						D2Win_10117_DrawText(wszCaret, nX, nY, pEditBox->nTextColor, 0);
					}
				}
				while (*pCurrent == '\r');
			}
			else
			{
				do
				{
					++pCurrent;
					++nPosition;
					nY += nLineHeight;
					if (bDrawCaret && !bCaretDrawn && pEditBox->nCaretPosition == nPosition && nY - nStartY < nAvailableHeight)
					{
						D2Win_10117_DrawText(wszCaret, nX, nY, pEditBox->nTextColor, 0);
					}
				}
				while (*pCurrent == '\r');
			}
		}

		pLineStart = pCurrent;
	}
	while (*pCurrent);

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
		*pX -= pEditBox->controlHeader.nImageX + 1;
		*pY += pEditBox->controlHeader.nHeight - pEditBox->controlHeader.nImageY - 1;

		// The upper bound is applied last, so it wins in a box narrower than its insets.
		*pX = std::min(std::max(*pX, pEditBox->nTextInsetX), pEditBox->controlHeader.nWidth - pEditBox->nTextInsetX - 1);
		*pY = std::min(std::max(*pY, pEditBox->nTextInsetY), pEditBox->controlHeader.nHeight - pEditBox->nTextInsetY - 1);
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
		// The upper bound is applied last, so it wins in a box narrower than its insets.
		nX = std::min(std::max(nX, pEditBox->nTextInsetX), pEditBox->controlHeader.nWidth - pEditBox->nTextInsetX - 1);
		nY = std::min(std::max(nY, pEditBox->nTextInsetY), pEditBox->controlHeader.nHeight - pEditBox->nTextInsetY - 1);
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
		for (int32_t i = 0; i < nTextLength; ++i)
		{
			wszText[i] = wszStar[0];
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
