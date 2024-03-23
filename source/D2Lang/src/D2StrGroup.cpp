/**
 * D2MOO
 * Copyright (c) 2020-2024  The Phrozen Keep community
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

#include <D2StrGroup.h>

#include <stdlib.h>
#include <string.h>

#include <D2Lang.h>
#include <D2Unicode.h>
#include <Fog.h>

inline static void GroupCStrDigits(Unicode* pUnicode, const char* pcSrc, int nMaxLength)
{
  size_t nBufferLength = strlen(pcSrc);

  int nPuncuatedLength = ((nBufferLength - 1) / 3) + nBufferLength;
  if (nBufferLength == 0 || nPuncuatedLength <= 0 || nPuncuatedLength >= nMaxLength - 1)
  {
    Unicode asteriskStr[2];
    Unicode::toUnicode(asteriskStr, "*", 2);
    Unicode::strcpy(pUnicode, asteriskStr);
    return;
  }

  pUnicode[nPuncuatedLength] = '\0';
  int nDistanceToComma = 0;
  // Like C++ reverse iterators, i and j point to the index after the
  // insertion index.
  // i = destination index (naming is to preserve accurate assert msg)
  // j = source index
  for (int i = nPuncuatedLength, j = nBufferLength; j > 0; --i, --j)
  {
    D2_ASSERT(i);

    pUnicode[i - 1] = pcSrc[j - 1];
    ++nDistanceToComma;
    if (nDistanceToComma < 3)
    {
      continue;
    }

    // Inefficient: Calls strlen again.
    if (strlen(pcSrc) <= 3 || i - 1 <= 0)
    {
      continue;
    }

    nDistanceToComma = 0;
    pUnicode[i - 2] = L',';
    --i;
  }
}

void __stdcall STR_GroupIntDigits(Unicode* pUnicode, int nValue, int nMaxLength)
{
  D2_ASSERT(nMaxLength > 2);

  if (nValue < 0)
  {
    pUnicode[0] = L'-';
    ++pUnicode;
    nMaxLength -= 1;
    nValue = -nValue;
  }

  char chBuffer[64];
  _itoa(nValue, chBuffer, 10);

  GroupCStrDigits(pUnicode, chBuffer, nMaxLength);
}

void __stdcall STR_GroupUintDigits(Unicode* pUnicode, unsigned int dwValue, int nMaxLength)
{
  D2_ASSERT(nMaxLength > 2);

  char chBuffer[64];
  _ultoa(dwValue, chBuffer, 10);

  GroupCStrDigits(pUnicode, chBuffer, nMaxLength);
}
