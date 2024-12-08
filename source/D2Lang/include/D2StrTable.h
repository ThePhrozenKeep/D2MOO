/**
 * D2MOO
 * Copyright (c) 2020-2022  The Phrozen Keep community
 *
 * This file belongs to D2MOO.
 * File authors: Mir Drualga
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

#include <D2Constants.h>
#include <D2BasicTypes.h>

#include "D2Lang.h"

// 1.10f: D2Lang.0x6FC12F90 (#10000)
D2LANG_DLL_DECL BOOL __fastcall STRTABLE_LoadStringTables(void* hArchive, char* szLanguageString, BOOL bLoadExpansion);

// 1.10f: D2Lang.0x6FC12F90 (#10001)
D2LANG_DLL_DECL void __fastcall STRTABLE_FreeStringTables();

// 1.10f: D2Lang.0x6FC13FB0 (#10006)
D2LANG_DLL_DECL void __stdcall STRTABLE_GetLanguageString(char* szLangString, int nUnk);

/**
 * Returns the string table language.
 *
 * 1.00: D2Lang.0x100010B9 (#10007)
 * 1.10: D2Lang.0x6FC13DF0 (#10007)
 * 1.13c: D2Lang.0x6FC09290 (#10009)
 * 1.14c: Game.0x00522610
 */
D2LANG_DLL_DECL D2C_Language STRTABLE_GetLanguage();

/**
 * Converts an integer value into a into a null-terminated UCS-2 string,
 * with commas separating every group of three digits. If the required
 * length of the converted string is greater than the nMaxLength, then
 * an asterisk character '*' will be written in place of the digits.
 *
 * The value of nMaxLength must be greater than 2, or an assertion will
 * fail.
 *
 * 1.00: D2Lang.0x100010AF (#10009)
 * 1.10: D2Lang.0x6FC140E0 (#10009)
 * 1.13c: D2Lang.0x6FC09BC0 (#10002)
 * 1.14c: Game.0x00523510
 */
D2LANG_DLL_DECL void __stdcall STR_GroupIntDigits(
		Unicode* pUnicode, int nValue, int nMaxLength);

/**
 * Converts a unsigned integer value into a into a null-terminated UCS-2
 * string, with commas separating every group of three digits. If the
 * required length of the converted string is greater than the
 * nMaxLength, then an asterisk character '*' will be written in place
 * of the digits.
 *
 * The value of nMaxLength must be greater than 2, or an assertion will
 * fail.
 *
 * 1.00: D2Lang.0x100010DC (#10010)
 * 1.10: D2Lang.0x6FC14210 (#10010)
 * 1.13c: D2Lang.0x6FC09A80 (#10006)
 * 1.14c: Game.0x005228A0
 */
D2LANG_DLL_DECL void __stdcall STR_GroupUintDigits(
		Unicode* pUnicode, unsigned int dwValue, int nMaxLength);
