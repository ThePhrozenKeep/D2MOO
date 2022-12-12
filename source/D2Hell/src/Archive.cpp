/**
 * D2MOO
 * Copyright (c) 2020-2022  The Phrozen Keep community
 *
 * This file belongs to D2MOO.
 * File authors: Lectem, Mir Drualga
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Archive.h"

#include "Fog.h"

/**
 * Opens a file in the MPQ archives and returns the handle to the
 * file.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: Inline
 * 1.10: Inline
 * 1.13c: Inline
 * 1.14c: Game.0x00514B24
 */
static BOOL __fastcall ARCHIVE_OpenFile(void* pArchiveHandle, const char* szFilePath, HSFILE* ppFileHandle, BOOL bBlockNotFoundLog)
{
	BOOL bFileOpenSucceeded = FOG_MPQFileOpen(szFilePath, ppFileHandle);
	if (!bFileOpenSucceeded)
	{
		DWORD dwLastError = GetLastError();
		if (!bBlockNotFoundLog || dwLastError != ERROR_FILE_NOT_FOUND)
		{
			FOG_Trace("Error opening file: %s", szFilePath);
		}

		return FALSE;
	}

	return TRUE;
}

/**
 * Gets the uncompressed size of a file in the MPQ archives.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10004DFE
 * 1.10: D2Lang.0x6FC145F2
 * 1.13c: Inline
 * 1.14c: Game.0x00514B87
 */
static size_t __fastcall ARCHIVE_GetFileSize(void* pArchiveHandle, HSFILE pFileHandle, size_t* pFileSizeHigh)
{
	D2_ASSERT(pFileHandle != nullptr);

	size_t dwFileSize = FOG_MPQFileGetSize(pFileHandle, pFileSizeHigh);
	if (dwFileSize == 0)
	{
		char szArchivePath[MAX_PATH];
#if 0  // TODO: Enable this code once the macro is set up.
		Storm_276_GetFileName(pFileHandle, szArchivePath, 260);
		FOG_DisplayError(3, szArchivePath, __FILE__, __LINE__);
#endif
		exit(-1);
	}

	return dwFileSize;
}

/**
 * Reads a file from the MPQ archives into a specified buffer.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10004EE3
 * 1.10: D2Lang.0x6FC14661
 * 1.13c: D2Lang.0x6FC07C00
 * 1.14c: Game.0x00514C61
 */
static void __fastcall ARCHIVE_ReadFileToBuffer(void* pArchiveHandle, HSFILE pFileHandle, void* pBuffer, size_t dwBytesToRead)
{
	D2_ASSERT(pFileHandle != nullptr);

	size_t dwBytes;
	BOOL bFileReadSuccess = FOG_MPQFileRead(pFileHandle, pBuffer, dwBytesToRead, (int*)&dwBytes, 0, 0, 0);
	if (!bFileReadSuccess)
	{
		char szArchivePath[MAX_PATH];
#if 0  // TODO: Enable this code once the macro is set up.
		Storm_276_GetFileName(pFileHandle, szArchivePath, sizeof(szArchivePath));
		FOG_DisplayError(3, szArchivePath, __FILE__, __LINE__);
#endif
		exit(-1);
	}

	D2_ASSERT(dwBytesToRead == dwBytes);
}

/**
 * Reads a file from the MPQ archives into a specified buffer.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: Inline
 * 1.10: Inline
 * 1.13c: Inline
 * 1.14c: Game.0x00514B60
 */
static void __fastcall ARCHIVE_CloseFile(void* pArchiveHandle, HSFILE pFileHandle)
{
	D2_ASSERT(pFileHandle != nullptr);
	FOG_MPQFileClose(pFileHandle);
}

/**
 * pArchiveHandle identifier is confirmed by 1.00 (originally hArchive).
 * 
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10005029
 * 1.10: D2Lang.0x6FC14708
 * 1.13c: D2Lang.0x6FC07EF0
 * 1.14c: Game.0x00514D55
 */
void* __fastcall ARCHIVE_ReadFile(void* pArchiveHandle, const char* szFilePath, size_t* pBytesWritten, const char* szSrcPath, int nLine)
{
	HSFILE pFileHandle;
	
	BOOL bOpenFileSucceeded = ARCHIVE_OpenFile(pArchiveHandle, szFilePath, &pFileHandle, FALSE);
	if (!bOpenFileSucceeded)
	{
		return nullptr;
	}

	size_t dwFileSizeHigh;
	size_t dwFileSize = ARCHIVE_GetFileSize(pArchiveHandle, pFileHandle, &dwFileSizeHigh);
	void* pBuffer = FOG_Alloc(dwFileSize + 800, szSrcPath, nLine, 0);
	if (pBuffer == nullptr)
	{
		return nullptr;
	}

	ARCHIVE_ReadFileToBuffer(pArchiveHandle, pFileHandle, pBuffer, dwFileSize);
	ARCHIVE_CloseFile(pArchiveHandle, pFileHandle);

	if (pBytesWritten == nullptr)
	{
		*pBytesWritten = dwFileSize;
	}

	return pBuffer;
}
