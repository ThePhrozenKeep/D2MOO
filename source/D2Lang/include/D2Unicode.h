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

#include <D2BasicTypes.h>
#include "D2Lang.h"

/*
* This type must be struct in order to make certain functions'
* mangled names the same as the ones in D2Lang.
*/
struct D2LANG_DLL_DECL Unicode {
 public:
  enum Direction {
    DIR_LEFT_TO_RIGHT = 0xF0,
    DIR_RIGHT_TO_LEFT = 0xF1,
    DIR_NEUTRAL = 0xF2,
  };

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
   * Returns whether the specified character in the string is the last
   * character of a word. In this function, only characters in the
   * English alphabet or Arabic numerals are considered characters in
   * a word. The first character is also not considered the end of a
   * word.
   *
   * D2Lang.0x6FC11190 (#10031) ?isWordEnd@Unicode@@SIHPBU1@I@Z
   */
  static BOOL __fastcall isWordEnd(const Unicode* str, size_t index);

  /**
   * Appends a null-terminated string to the end of a null-terminated
   * destination string. Returns the destination string.
   *
   * D2Lang.0x6FC113F0 (#10034) ?strcat@Unicode@@SIPAU1@PAU1@PBU1@@Z
   */
  static Unicode* __fastcall strcat(Unicode* dest, const Unicode* src);

  /**
   * Compares two null-terminated strings lexicographically. Returns
   * -1, 0, or 1, depending on the results of the comparison.
   *
   * D2Lang.0x6FC11210 (#10036) ?strcmp@Unicode@@SIHPBU1@0@Z
   */
  static int __fastcall strcmp(const Unicode* str1, const Unicode* str2);

  /**
   * Copies the characters from a null-terminated source string into a
   * null-terminated destination string. Returns the destination
   * string.
   *
   * D2Lang.0x6FC114A0 (#10038) ?strcpy@Unicode@@SIPAU1@PAU1@PBU1@@Z
   */
  static Unicode* __fastcall strcpy(Unicode* dest, const Unicode* src);

  /**
   * Returns the length of the null-terminated string. If the string
   * pointer is NULL, the function returns 0;
   *
   * D2Lang.0x6FC114C0 (#10040) ?strlen@Unicode@@SIHPBU1@@Z
   */
  static int __fastcall strlen(const Unicode* str);

  /**
   * Converts a null-terminated UCS-2 string into a null-terminated
   * UTF-8 string. The count indicates the capacity of the destination
   * in terms of UTF-8 code units. Returns the pointer to the
   * destination string.
   *
   * Vanilla bug: Specifying INT_MIN for the count results in
   * undefined behavior. Do not specify INT_MIN as the count.
   *
   * Vanilla bug: Count is insufficient for preventing buffer overrun
   * for code points that require three UTF-8 code units. To work
   * around this, specify a lower count value.
   *
   * D2Lang.0x6FC12B60 (#10053) ?toUtf@Unicode@@SIPADPADPBU1@H@Z
   */
  static char* __fastcall toUtf(char* dest, const Unicode* src, int count);

  // D2Lang.0x6FC11C50 (#10023) ?directionality@Unicode@@QAE?AW4Direction@1@XZ
  Direction directionality();

  /**
   * Returns whether or not this Unicode character is in 
   * "C0 Controls and Basic Latin" character block, otherwise known as
   * the ASCII character block.
   *
   * D2Lang.0x6FC11080 (#10024) ?isASCII@Unicode@@QBEHXZ
   */
  BOOL isASCII() const;

  // D2Lang.0x6FC11050 (#10028) ?isNewline@Unicode@@QBEHXZ
  BOOL isNewline() const;

  // D2Lang.0x6FC11060 (#10029) ?isPipe@Unicode@@QBEHXZ
  BOOL isPipe() const;

  /**
   * Returns this character's lowercase if there is a lowercase for
   * this character. Otherwise, returns a copy of this character.
   *
   * Vanilla bug: Returns uppercase for characters in the
   * "Latin-1 Supplement" block.
   *
   * D2Lang.0x6FC110E0 (#10050) ?toLower@Unicode@@QBE?AU1@XZ
   */
  Unicode toLower() const;

  /**
   * Returns this character's uppercase if there is an uppercase for
   * this character. Otherwise, returns a copy of this character.
   *
   * D2Lang.0x6FC110B0 (#10052) ?toUpper@Unicode@@QBE?AU1@XZ
   */
  Unicode toUpper() const;

 private:
  /**
   * Vanilla bug: The characters in the Latin-1 Supplement block are
   * uppercase.
   *
   * D2Lang.[0x6FC1D508 to 0x6FC1D708) (#10019) ?_toLowerTable@Unicode@@0PAGA
   */
  static unsigned short _toLowerTable[256];

  // D2Lang.[0x6FC1D308 to 0x6FC1D508) (#10020) ?_toUpperTable@Unicode@@0PAGA
  static unsigned short _toUpperTable[256];
};
