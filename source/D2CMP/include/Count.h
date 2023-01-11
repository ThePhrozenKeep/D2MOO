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

#include <windows.h>

/**
 * Returns the count of consecutive bytes that are equal to the first
 * byte in pSrc, up to nMaxCount bytes. Also sets pbCountEqualsDist if
 * the count equals the distance from pSrc to pEnd, and
 * pbIsRemainingSame if all remaining bytes after nMaxCount are the
 * same as bValue.
 *
 * 1.00: D2CMP.0x100085D0
 * 1.07 Beta: D2CMPd.0x6FC7DA50
 * 1.07: Inline or unused
 * 1.10: Inline or unused
 */
int __fastcall CountConsecutiveFirst(const BYTE* pSrc, const BYTE* pEnd, int nMaxCount, BOOL* pbCountEqualsDist, BOOL* pbIsRemainingSame);

/**
 * Returns the count of consecutive bytes that are equal to bValue, up
 * to nMaxCount bytes. Also sets pbCountEqualsDist if the count equals
 * the distance from pSrc to pEnd, and pbIsRemainingSame if all
 * remaining bytes after nMaxCount are the same as bValue.
 *
 * 1.00: D2CMP.0x10008730
 * 1.07 Beta: D2CMPd.0x6FC7DBE0
 * 1.07: D2CMP.0x6FE15FE0
 * 1.10: D2CMP.0x6FDF5AF0
 */
int __fastcall CountConsecutive(BYTE bValue, const BYTE* pSrc, const BYTE* pEnd, int nMaxCount, BOOL* pbCountEqualsDist, BOOL* pbIsRemainingSame);

/**
 * Returns the count of consecutive bytes that are not equal to
 * bValue, up to nMaxCount bytes. Also sets pbCountEqualsDist if the
 * count equals the distance from pSrc to pEnd, and pbIsRemainingSame
 * if all remaining bytes after nMaxCount are the same as bValue.
 *
 * 1.00: D2CMP.0x10008730
 * 1.07 Beta: D2CMPd.0x6FC7DD70
 * 1.07: D2CMP.0x6FE16060
 * 1.10: D2CMP.0x6FDF5B70
 */
int __fastcall CountConsecutiveDiff(const BYTE* pSrc, const BYTE* pEnd, BYTE bValue, int nMaxCount, BOOL* pbCountEqualsDist, BOOL* pbIsRemainingSame);
