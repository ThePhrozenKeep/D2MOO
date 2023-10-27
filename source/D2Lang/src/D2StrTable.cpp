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

#include "D2StrTable.h"

#include <Archive.h>
#include <D2Constants.h>
#include <Fog.h>

/**
 * This variable is never set.
 *
 * 1.00: D2Lang.0x100175EC
 * 1.10: D2Lang.0x6FC20CDC
 * 1.13c: D2Lang.0x6FC10A8C
 * 1.14c: Game.0x00879A64
 */
static void* gpMemPool = nullptr;

/**
 * 1.00: D2Lang.0x100175F4
 * 1.10: D2Lang.0x6FC20CE8
 * 1.13c: D2Lang.0x6FC10A20
 * 1.14c: Game.0x00879A78
 */
static bool gbTableLanguageInitialized = false;

/**
 * 1.00: D2Lang.0x100175F8
 * 1.10: D2Lang.0x6FC20CEC
 * 1.13c: D2Lang.0x6FC10A1C
 * 1.14c: Game.0x00879A74
 */
static D2C_Language gnTableLanguage = LANGUAGE_ENGLISH;

/**
 * 1.00: D2Lang.0x100010B9 (#10007)
 * 1.10: D2Lang.0x6FC13DF0 (#10007)
 * 1.13c: D2Lang.0x6FC09290 (#10009)
 * 1.14c: Game.0x00522610
 */
D2C_Language STRTABLE_GetLanguage()
{
	if (gbTableLanguageInitialized)
	{
		return gnTableLanguage;
	}
	gbTableLanguageInitialized = true;
	// Read locale from file.
	unsigned char* pBuffer = (unsigned char*) ARCHIVE_READ_FILE_TO_ALLOC_BUFFER(gpMemPool, "data\\local\\use", nullptr);
	gnTableLanguage = static_cast<D2C_Language>(*pBuffer);
	D2_FREE(pBuffer);

	if (gnTableLanguage >= NUM_LANGUAGES)
	{
		gnTableLanguage = LANGUAGE_ENGLISH;
	}

	return gnTableLanguage;
}
