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

#pragma once

#include <stddef.h>
#include <windows.h>

// TODO: Replace this include with the header file that defines
// HSFILE.
#include <Fog.h>

#pragma pack(push, 1)
struct D2ArchiveHandleStrc
{
	HSARCHIVE hArchive;
	char szPath[MAX_PATH];
};
#pragma pack(pop)

// Name comes from D2Lang: Unicode::loadSysMap(struct HD2ARCHIVE__ *hArchive, char *szFileName)
// Most likely not the same as storm's HARCHIVE and probably D2ArchiveHandleStrc?
typedef struct HD2ARCHIVE__* HD2ARCHIVE; // NOLINT(bugprone-reserved-identifier)

/**
 * Opens a file in the MPQ archives and returns the handle to the
 * file.
 *
 * hArchive is the first parameter, but it is effectively unused.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: Inline
 * 1.10: Inline OR D2Common.0x6FDC40F0
 * 1.13c: Inline
 * 1.14c: Game.0x00514B24
 */
BOOL __fastcall ARCHIVE_OpenFile(HD2ARCHIVE hArchive, const char* szFilePath, HSFILE* phFile, BOOL bFileNotFoundLogSkipped);

/**
 * Closes a file handle to a file from the MPQ archives.
 *
 * hArchive is the first parameter, but it is effectively unused.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: Inline
 * 1.10: Inline OR D2Common.0x6FDC412C
 * 1.13c: Inline
 * 1.14c: Game.0x00514B60
 */
void __fastcall ARCHIVE_CloseFile(HD2ARCHIVE hArchive, HSFILE hFile);

/**
 * Gets the uncompressed size of a file in the MPQ archives.
 *
 * hArchive is the first parameter, but it is effectively unused.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10004DFE
 * 1.10: D2Lang.0x6FC145F2 OR D2Common.0x6FDC4152
 * 1.13c: Inline
 * 1.14c: Game.0x00514B87
 * D2XBeta: D2Server.dll.0x10009C76
 */
uint32_t __fastcall ARCHIVE_GetFileSize(HD2ARCHIVE hArchive, HSFILE hFile, uint32_t* pdwFileSizeHigh);

/**
 * Sets the file pointer of a file in the MPQ archives.
 *
 * hArchive is the first parameter, but it is effectively unused.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.10: D2CMP.0x6FE0047A
 */
uint32_t __fastcall ARCHIVE_SetFilePointer(HD2ARCHIVE hArchive, HSFILE hFile, int32_t lDistanceToMove, int32_t* lpDistanceToMoveHigh, uint32_t dwMoveMethod);

/**
 * Reads a file from the MPQ archives into a specified buffer.
 *
 * hArchive is the first parameter, but it is effectively unused.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10004EE3
 * 1.10: D2Lang.0x6FC14661 OR D2Common.0x6FDC41C1
 * 1.13c: D2Lang.0x6FC07C00
 * 1.14c: Game.0x00514C61
 */
void __fastcall ARCHIVE_ReadFileToBuffer(HD2ARCHIVE hArchive, HSFILE hFile, void* pBuffer, size_t dwBytesToRead);

/**
 * Reads a file from the MPQ archives into a buffer allocated by
 * this function. Returns the buffer containing the file's contents,
 * or nullptr on failure.
 *
 * hArchive is the first parameter, but it is effectively unused.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10005029
 * 1.10: D2Lang.0x6FC14708 OR D2Common.0x6FDC4268 OR D2Win.0x6F8B22F8
 * 1.13c: D2Lang.0x6FC07EF0
 * 1.14c: Game.0x00514D55
 */
void* __fastcall ARCHIVE_ReadFileToAllocBuffer(HD2ARCHIVE hArchive, const char* szFilePath, size_t* pdwBytesWritten, const char* szSrcPath, int nLine);

#define ARCHIVE_READ_FILE_TO_ALLOC_BUFFER(hArchive, szFilePath, pBytesWritten) ARCHIVE_ReadFileToAllocBuffer(hArchive, szFilePath, pBytesWritten, __FILE__, __LINE__)

using ARCHIVE_ShowMessageFunctionPtr = BOOL (__stdcall*)();

//1.10f: D2Win.0x6F8B2399
D2ArchiveHandleStrc* __fastcall ARCHIVE_LoadMPQFile(const char* szModuleName, const char* szFileName, const char* szLabel, int a4, HANDLE hFile, ARCHIVE_ShowMessageFunctionPtr pfShowMessage, int nPriority);
//1.10f: D2Win.0x6F8B2548
void __fastcall ARCHIVE_UnloadMPQFile(D2ArchiveHandleStrc* pMPQHandle);
