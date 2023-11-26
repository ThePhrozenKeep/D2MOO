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

#include <stddef.h>
#include <stdlib.h>
#include <windows.h>

#include <Fog.h>
#include <Storm.h>

/**
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: Inline
 * 1.10: Inline OR D2Common.0x6FDC40F0
 * 1.13c: Inline
 * 1.14c: Game.0x00514B24
 */
BOOL __fastcall ARCHIVE_OpenFile(HD2ARCHIVE hArchive, const char* szFilePath, HSFILE* phFile, BOOL bFileNotFoundLogSkipped)
{
	BOOL bFileOpenSucceeded = FOG_MPQFileOpen(szFilePath, phFile);
	if (!bFileOpenSucceeded)
	{
		DWORD dwLastError = GetLastError();
		if (!bFileNotFoundLogSkipped || dwLastError != ERROR_FILE_NOT_FOUND)
		{
			FOG_Trace("Error opening file: %s", szFilePath);
		}

		return FALSE;
	}

	return TRUE;
}

/**
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: Inline
 * 1.10: Inline OR D2Common.0x6FDC412C
 * 1.13c: Inline
 * 1.14c: Game.0x00514B60
 */
void __fastcall ARCHIVE_CloseFile(HD2ARCHIVE hArchive, HSFILE hFile)
{
	D2_ASSERT(hFile != nullptr);
	FOG_MPQFileClose(hFile);
}

/**
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10004DFE
 * 1.10: D2Lang.0x6FC145F2 OR D2Common.0x6FDC4152
 * 1.13c: Inline
 * 1.14c: Game.0x00514B87
 * D2XBeta: D2Server.dll.0x10009C76
 */
size_t __fastcall ARCHIVE_GetFileSize(HD2ARCHIVE hArchive, HSFILE hFile, size_t* pdwFileSizeHigh)
{
	D2_ASSERT(hFile != nullptr);

	size_t dwFileSize = FOG_MPQFileGetSize(hFile, pdwFileSizeHigh);
	if (dwFileSize == 0)
	{
		char szArchivePath[MAX_PATH];
		SFileGetFileName(hFile, szArchivePath, 260);
		FOG_DisplayError(3, szArchivePath, __FILE__, __LINE__);
		exit(-1);
	}

	return dwFileSize;
}

/**
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10004EE3
 * 1.10: D2Lang.0x6FC14661 OR D2Common.0x6FDC41C1
 * 1.13c: D2Lang.0x6FC07C00
 * 1.14c: Game.0x00514C61
 */
void __fastcall ARCHIVE_ReadFileToBuffer(HD2ARCHIVE hArchive, HSFILE hFile, void* pBuffer, size_t dwBytesToRead)
{
	D2_ASSERT(hFile != nullptr);

	size_t dwBytes;
	BOOL bFileReadSuccess = FOG_MPQFileRead(hFile, pBuffer, dwBytesToRead, (int*)&dwBytes, 0, 0, 0);
	if (!bFileReadSuccess)
	{
		char szArchivePath[MAX_PATH];
		SFileGetFileName(hFile, szArchivePath, sizeof(szArchivePath));
		FOG_DisplayError(3, szArchivePath, __FILE__, __LINE__);
		exit(-1);
	}

	D2_ASSERT(dwBytesToRead == dwBytes);
}

/**
 * hArchive identifier is confirmed via v1.00 #D2Common.0xFUN_100777f3.
 * 
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10005029
 * 1.10: D2Lang.0x6FC14708 OR D2Common.0x6FDC4268 OR D2Win.0x6F8B22F8
 * 1.13c: D2Lang.0x6FC07EF0
 * 1.14c: Game.0x00514D55
 */
void* __fastcall ARCHIVE_ReadFileToAllocBuffer(HD2ARCHIVE hArchive, const char* szFilePath, size_t* pdwBytesWritten, const char* szSrcPath, int nLine)
{
	HSFILE hFile;
	
	BOOL bOpenFileSucceeded = ARCHIVE_OpenFile(hArchive, szFilePath, &hFile, FALSE);
	if (!bOpenFileSucceeded)
	{
		return nullptr;
	}

	size_t dwFileSizeHigh;
	size_t dwFileSize = ARCHIVE_GetFileSize(hArchive, hFile, &dwFileSizeHigh);
	void* pBuffer = FOG_Alloc(dwFileSize + 800, szSrcPath, nLine, 0);
	if (pBuffer == nullptr)
	{
		return nullptr;
	}

	ARCHIVE_ReadFileToBuffer(hArchive, hFile, pBuffer, dwFileSize);
	ARCHIVE_CloseFile(hArchive, hFile);

	if (pdwBytesWritten != nullptr)
	{
		*pdwBytesWritten = dwFileSize;
	}

	return pBuffer;
}

//1.10f: D2Win.0x6F8B24B1
int __fastcall ARCHIVE_FindAndOpenArchiveFromCDRom(LPSTR szOutPath, LPCSTR szFileName, int nPriority, HSARCHIVE* phArchive)
{
	CHAR Buffer[MAX_PATH];
	DWORD nLength = GetLogicalDriveStringsA(MAX_PATH, Buffer);
	if (!nLength || nLength > MAX_PATH)
	{
		return FALSE;
	}
	
	while (*szFileName == '\\')
	{
		++szFileName;
	}
	
	for(const char* pCurrentDriveStr = Buffer; *pCurrentDriveStr != '\0'; pCurrentDriveStr = &pCurrentDriveStr[strlen(pCurrentDriveStr) + 1])
	{
		if (GetDriveTypeA(pCurrentDriveStr) == DRIVE_CDROM)
		{
			lstrcpyA(szOutPath, pCurrentDriveStr);
			lstrcatA(szOutPath, szFileName);
			if (SFileOpenArchive(szOutPath, nPriority, 2, phArchive))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

//1.10f: D2Win.0x6F8B2419
HSARCHIVE __fastcall ARCHIVE_FindAndOpenArchive(LPSTR szOutPath, LPCSTR szModuleName, LPCSTR szFileName, const char* szLabel, int nPriority, BOOL bSomething)
{
	D2_MAYBE_UNUSED(szModuleName); // __thiscall => __fastcall, unused.
	uint32_t bFileOpenFlags = bSomething ? 3 : 2;
	HSARCHIVE hArchiveFile;
	lstrcpyA(szOutPath, szFileName);
	if (SFileOpenArchive(szOutPath, nPriority, bFileOpenFlags, &hArchiveFile))
	{
		return hArchiveFile;
	}
	if (!GetModuleFileNameA(0, szOutPath, MAX_PATH))
	{
		*szOutPath = 0;
	}
	
	char* pLastBackslash =  strrchr(szOutPath, '\\');
	if (pLastBackslash)
	{
		*(pLastBackslash + 1) = 0;
	}

	SStrNCat(szOutPath, szFileName, INT_MAX);
	if (SFileOpenArchive(szOutPath, nPriority, bFileOpenFlags, &hArchiveFile))
	{
		return hArchiveFile;
	}
	if (ARCHIVE_FindAndOpenArchiveFromCDRom(szOutPath, szFileName, nPriority, &hArchiveFile) != 0)
	{
		return hArchiveFile;
	}
	return NULL;
}

//1.10f: D2Win.0x6F8B2399
D2ArchiveHandleStrc* __fastcall ARCHIVE_LoadMPQFile(const char* szModuleName, const char* szFileName, const char* szLabel, int a4, HANDLE hFile, ARCHIVE_ShowMessageFunctionPtr pfShowMessage, int nPriority)
{
	D2ArchiveHandleStrc* pMpqHandle = D2_ALLOC_STRC(D2ArchiveHandleStrc);
	pMpqHandle->hArchive = NULL;
	while (1)
	{
		pMpqHandle->hArchive = ARCHIVE_FindAndOpenArchive(pMpqHandle->szPath, szFileName, szModuleName, szLabel, nPriority, hFile != 0);
		if (pMpqHandle->hArchive)
		{
			return pMpqHandle;
		}
		if (!pfShowMessage || !pfShowMessage())
			break;
		Sleep(100);
		if (pMpqHandle->hArchive)
		{
			return pMpqHandle;
		}
	}
	D2_FREE(pMpqHandle);
	return nullptr;
}

//1.10f: D2Win.0x6F8B2548
void __fastcall ARCHIVE_UnloadMPQFile(D2ArchiveHandleStrc* pMPQHandle)
{
	if (pMPQHandle->hArchive)
	{
		SFileCloseArchive(pMPQHandle->hArchive);
	}
	pMPQHandle->hArchive = 0;
	D2_FREE(pMPQHandle);
}