/**
 * Copyright (c) 2021 Mir Drualga
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

#include <wchar.h>

#include "D2Lang.h"

/*
* This type must be struct in order to make certain functions'
* mangled names the same as the ones in D2Lang.
*/
struct D2LANG_DLL_DECL Unicode {
 public:
  unsigned short ch;

  /**
   * Default parameter value generates the default constructor
   * closure, but also requires dllexport to generate.
   *
   * D2Lang.0x6FC11200 (#10017) ??_FUnicode@@QAEXXZ
   */

  // D2Lang.0x6FC11010 (#10014) ??0Unicode@@QAE@G@Z
  Unicode(unsigned short ch = 0);

  // D2Lang.0x6FC111F0 (#10015) ??4Unicode@@QAEAAU0@ABU0@@Z
  Unicode& operator=(const Unicode& str);

  // D2Lang.0x6FC11020 (#10016) ??BUnicode@@QBEGXZ
  operator unsigned short() const;

  /**
   * Returns the length of the null-terminated string. If the string
   * pointer is NULL, the function return 0;
   *
   * D2Lang.0x6FC114C0 (#10040) ?strlen@Unicode@@SIHPBU1@@Z
   */
  static int __fastcall strlen(const Unicode* str);
};
