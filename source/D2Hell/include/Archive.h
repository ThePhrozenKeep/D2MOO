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

/**
 * Reads a file from the MPQ archives into a buffer allocated by
 * this function, and returns that buffer.
 *
 * pArchive is the first parameter, but it is effectively unused.
 *
 * Static library; may be defined in multiple places than ones listed:
 * 1.00: D2Lang.0x10005029
 * 1.10: D2Lang.0x6FC14708
 * 1.13c: D2Lang.0x6FC07EF0
 * 1.14c: Game.0x00514D55
 */
void* __fastcall ARCHIVE_ReadFile(void* pArchiveHandle, const char* szFilePath, size_t* pBytesWritten, const char* szSrcPath, int nLine);
