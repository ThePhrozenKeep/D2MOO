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

int Unicode::isASCII() const {
  return this->ch < 0x80;
}
