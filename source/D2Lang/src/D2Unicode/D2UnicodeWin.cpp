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

#include "D2Unicode.h"


/**
 * This implementation outputs the same binary from 1.00 and 1.10.
 *
 * 1.00: D2Lang.0x1000109B (#10053)
 * 1.10: D2Lang.0x6FC11C20 (#10055)
 */
char* __fastcall Unicode::unicode2Win(char* dest, const Unicode* src, int count)
{
	char* current_dest = dest;
	while (count > 1 && src->ch != L'\0')
	{
		const Unicode* current_src = src++;
		++current_dest;
		--count;

		*(current_dest - 1) = (char)current_src->ch;
	}

	*current_dest = '\0';
	return dest;
}

/**
 * This implementation outputs the same binary from 1.00.
 *
 * 1.00: D2Lang.0x10001122 (#10059)
 * 1.10: D2Lang.0x6FC11BD0 (#10062)
 */
Unicode* __fastcall Unicode::win2Unicode(Unicode* dest, const char* src, int count)
{
	int i;
	// Copy the source string as-is to the destination.
	for (i = 0; i < count; ++i)
	{
		unsigned char src_ch = src[i];
		if (src_ch == '\0')
		{
			break;
		}
		dest[i].ch = src_ch;
	}

	// Set the null-terminator.
	if (src[i] != '\0')
	{
		dest[i - 1].ch = L'\0';
		return dest;
	}

	dest[i].ch = L'\0';
	return dest;
}
