/**
 * D2MOO
 * Copyright (c) 2020-2023  The Phrozen Keep community
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

#include "Count.h"

#include <windows.h>

#include <cstdlib>

#include "Fog.h"

/**
 * 1.00: D2CMP.0x100085D0
 * 1.07 Beta: D2CMPd.0x6FC7DA50
 * 1.07: Inline or unused
 * 1.10: Inline or unused
 *
 * This implementation outputs effectively the same binary from 1.00.
 */
int __fastcall CountConsecutiveFirst(const BYTE* pSrc, const BYTE* pEnd, int nMaxCount, BOOL* pbCountEqualsDist, BOOL* pbIsRemainingSame)
{
	return CountConsecutive(pSrc[0], pSrc, pEnd, nMaxCount, pbCountEqualsDist, pbIsRemainingSame);
}

/**
 * 1.00: D2CMP.0x10008730
 * 1.07 Beta: D2CMPd.0x6FC7DBE0
 * 1.07: D2CMP.0x6FE15FE0
 * 1.10: D2CMP.0x6FDF5AF0
 *
 * This implementation outputs effectively the same binary from 1.00, 1.07,
 * and 1.10.
 */
int __fastcall CountConsecutive(BYTE bValue, const BYTE* pSrc, const BYTE* pEnd, int nMaxCount, BOOL* pbCountEqualsDist, BOOL* pbIsRemainingSame)
{
	D2_ASSERT(pSrc < pEnd);

	int nCount;
	for (nCount = 0; pSrc < pEnd && bValue == *pSrc && nCount < nMaxCount; ++pSrc, ++nCount) {}
	if (pSrc == pEnd)
	{
		*pbCountEqualsDist = TRUE;
		*pbIsRemainingSame = FALSE;
		return nCount;
	}

	*pbCountEqualsDist = FALSE;
	for (; pSrc < pEnd && bValue == *pSrc; ++pSrc) {}

	if (pSrc == pEnd)
	{
		*pbIsRemainingSame = TRUE;
	}
	else
	{
		*pbIsRemainingSame = FALSE;
	}

	return nCount;
}

/**
 * 1.00: D2CMP.0x10008730
 * 1.07 Beta: D2CMPd.0x6FC7DD70
 * 1.07: D2CMP.0x6FE16060
 * 1.10: D2CMP.0x6FDF5B70
 *
 * This implementation outputs effectively the same binary from 1.00, 1.07,
 * and 1.10.
 */
int __fastcall CountConsecutiveDiff(const BYTE* pSrc, const BYTE* pEnd, BYTE bValue, int nMaxCount, BOOL* pbCountEqualsDist, BOOL* pbIsRemainingSame)
{
	D2_ASSERT(pSrc < pEnd);

	int nCount;
	for (nCount = 0; pSrc < pEnd && *pSrc != bValue && nCount < nMaxCount; ++pSrc, ++nCount) {}
	if (pSrc == pEnd)
	{
		*pbCountEqualsDist = TRUE;
		*pbIsRemainingSame = FALSE;
		return nCount;
	}

	*pbCountEqualsDist = FALSE;
	for (; pSrc < pEnd && bValue == *pSrc; ++pSrc) {}

	if (pSrc == pEnd)
	{
		*pbIsRemainingSame = TRUE;
	}
	else
	{
		*pbIsRemainingSame = FALSE;
	}

	return nCount;
}
