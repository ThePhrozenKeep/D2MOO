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

// TODO: Replace these HS type declarations and typedefs with an
// appropriate include once a proper place for these types are found.
// This should ideally be in a Storm-related header file.

struct HSFILE__;
using HSFILE = HSFILE__*;

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
BOOL __fastcall ARCHIVE_OpenFile(void* hArchive, const char* szFilePath, HSFILE* phFile, BOOL bFileNotFoundLogSkipped);

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
void __fastcall ARCHIVE_CloseFile(void* hArchive, HSFILE hFile);

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
 */
size_t __fastcall ARCHIVE_GetFileSize(void* hArchive, HSFILE hFile, size_t* pdwFileSizeHigh);

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
void __fastcall ARCHIVE_ReadFileToBuffer(void* hArchive, HSFILE hFile, void* pBuffer, size_t dwBytesToRead);

/**
 * Reads a file from the MPQ archives into a buffer allocated by
 * this function. Returns the buffer containing the file's contents,
 * or nullptr on failure.
 *
 * hArchive is the first parameter, but it is effectively unused.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10005029
 * 1.10: D2Lang.0x6FC14708 OR D2Common.0x6FDC4268
 * 1.13c: D2Lang.0x6FC07EF0
 * 1.14c: Game.0x00514D55
 */
void* __fastcall ARCHIVE_ReadFileToAllocBuffer(void* hArchive, const char* szFilePath, size_t* pdwBytesWritten, const char* szSrcPath, int nLine);

#define ARCHIVE_READ_FILE_TO_ALLOC_BUFFER(hArchive, szFilePath, pBytesWritten) ARCHIVE_ReadFileToAllocBuffer(hArchive, szFilePath, pBytesWritten, __FILE__, __LINE__)
