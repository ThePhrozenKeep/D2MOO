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

#include <D2Unicode.h>

unsigned short Unicode::_toLowerTable[256] = {
  // C0 Controls
  '\0', 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, '\a', '\b', '\t', '\n',
  '\v', '\f', '\r', 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
  0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,

  // Basic Latin
  ' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',',
  '-', '.', '/',

  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

  ':', ';', '<', '=', '>', '\?', '@',

  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

  '[', '\\', ']', '^', '_', '`',

  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

  '{', '|', '}', '~', 0x7F,

  // C1 Controls
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99,

  0x8A, 0x9B, 0x8C, 0x9D, 0x9E, 0x9F,

  // Latin-1 Supplement
  // Vanilla bug: The characters are uppercase.
  0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
  0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
  0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
  0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,

  0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
  0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
  0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
  0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,

  0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
  0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,

  0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,

  0xF7,

  0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE,

  0x9F
};

unsigned short Unicode::_toUpperTable[256] = {
  // C0 Controls
  L'\0', 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, L'\a', L'\b', L'\t', L'\n',
  L'\v', L'\f', L'\r', 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
  0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,

  // Basic Latin
  L' ', L'!', L'\"', L'#', L'$', L'%', L'&', L'\'', L'(', L')', L'*', L'+',
  L',', L'-', L'.', L'/',

  L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9',

  L':', L';', L'<', L'=', L'>', L'\?', L'@',

  L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I', L'J', L'K', L'L',
  L'M', L'N', L'O', L'P', L'Q', L'R', L'S', L'T', L'U', L'V', L'W', L'X',
  L'Y', L'Z',

  L'[', L'\\', L']', L'^', L'_', L'`',

  L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I', L'J', L'K', L'L',
  L'M', L'N', L'O', L'P', L'Q', L'R', L'S', L'T', L'U', L'V', L'W', L'X',
  L'Y', L'Z',

  L'{', L'|', L'}', L'~', 0x7F,

  // C1 Controls
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99,

  0x8A, 0x9B, 0x8C, 0x9D, 0x9E, 0x9F,

  // Latin-1 Supplement
  0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
  0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
  0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
  0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,

  0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
  0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
  0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
  0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,

  0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
  0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,

  0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,

  0xF7,

  0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE,

  0x9F
};

Unicode::Unicode(unsigned short ch) : ch(ch) {
}

Unicode& Unicode::operator=(const Unicode& str) {
  this->ch = str.ch;

  return *this;
}

Unicode::operator unsigned short() const {
  return this->ch;
}

Unicode::Direction Unicode::directionality() {
  /*
   * Hebrew, Arabic, Syriac, Arabic Supplement, Thaana, NKo,
   * Samaritan, Mandaic, Syriac Supplement, Arabic Extended-A
   */
  if (this->ch >= 0x590 && this->ch <= 0x8FF) {
    return DIR_RIGHT_TO_LEFT;
  }

  /* Lower case a to z */
  if (this->ch >= L'a' && this->ch <= L'z') {
    return DIR_LEFT_TO_RIGHT;
  }

  /* @ character, upper case A to Z */
  if (this->ch >= L'@' && this->ch <= L'Z') {
    return DIR_LEFT_TO_RIGHT;
  }

  /* Numbers 0 to 9 */
  if (this->ch >= L'0' && this->ch <= L'9') {
    return DIR_LEFT_TO_RIGHT;
  }

  /*
   * The characters \0 to [SPACE] and the following:
   * !"#$%'()*+,-./:;<=>?
   *
   * & character is a left-to-right case
   */
  if (this->ch <= L'\?') {
    return (this->ch == L'&')
        ? DIR_LEFT_TO_RIGHT
        : DIR_NEUTRAL;
  }

  /*
   * Hiragana, Katakana, Bopomofo, Hangul Compatibility Jamo, Kanbun,
   * Bopomofo Extended, CJK Strokes, Katakana Phonetic Extensions,
   * Enclosed CJK Letters and Months, CJK Compatibility,
   * CJK Unified Ideographs Extension A, Yijing Hexagram Symbols,
   * CJK Unified Ideographs (up to 0x8BFF)
   */
  if (this->ch >= 0x3040 && this->ch <= 0x8BFF) {
    return DIR_LEFT_TO_RIGHT;
  }

  /*
   * Latin-1 Supplement (from 0xBF), Latin Extended-A,
   * Latin Extended-B, IPA Extensions, Spacing Modifier Letters,
   * Combining Diacritical Marks, Greek and Coptic, Cyrillic,
   * Cyrillic Supplement, Armenian
   *
   * Excludes 0xD7 (multiplication sign) and 0xF7 (division sign)
   */
  if (this->ch >= 0xC0 && this->ch <= 0x58F
      && this->ch != 0xD7 && this->ch != 0xF7) {
    return DIR_LEFT_TO_RIGHT;
  }

  /*
   * Devanagari, Bengali, Gurmukhi, Gujarati, Oriya, Tamil, Telugu,
   * Kannada, Malayalam, Sinhala, Thai, Lao, Tibetan, Tibetan,
   * Georgian
   */
  if (this->ch >= 0x900 && this->ch <= 0x10FF) {
    return DIR_LEFT_TO_RIGHT;
  }

  /*
   * General Punctuation, Superscripts and Subscripts,
   * Currency Symbols, Combining Diacritical Marks for Symbols,
   * Letterlike Symbols, Number Forms, Arrows
   *
   * Bitwise form of: `this->ch >= 0x2000 && this->ch <= 0x21FF`
   */
  if ((this->ch & 0xFE00) == 0x2000) {
    /* Number Forms (Roman Numerals only) */
    if (this->ch >= 0x2160 && this->ch <= 0x2182) {
      return DIR_LEFT_TO_RIGHT;
    }

    /* Combining Diacritical Marks for Symbols */
    if (this->ch >= 0x20D0 && this->ch <= 0x20FF) {
      return DIR_LEFT_TO_RIGHT;
    }

    /* Left-to-Right Mark */
    if (this->ch == 0x200E) {
      return DIR_LEFT_TO_RIGHT;
    }

    /* Right-to-Left Mark */
    if (this->ch == 0x200F) {
      return DIR_RIGHT_TO_LEFT;
    }
  }

  return DIR_NEUTRAL;
}

BOOL Unicode::isASCII() const {
  return this->ch < 0x80;
}

BOOL Unicode::isAlpha() const {
  return (isASCII() && ::isalpha((char)this->ch));
}

BOOL Unicode::isNewline() const {
  return this->ch == L'\n';
}

BOOL Unicode::isPipe() const {
  if (this->ch < 0x100) {
    return this->ch == 0xFF;
  }

  return FALSE;
}

Unicode Unicode::toLower() const {
  if (this->ch > 0xFF) {
    return this->ch;
  }

  // Vanilla bug: Returns uppercase for characters in the
  // "Latin-1 Supplement" block.
  return Unicode::_toLowerTable[this->ch];
}

Unicode Unicode::toUpper() const {
  if (this->ch > 0xFF) {
    return this->ch;
  }

  return Unicode::_toUpperTable[this->ch];
}
