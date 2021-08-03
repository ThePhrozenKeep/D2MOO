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

#include <stddef.h>

#pragma pack(push, 1)

struct UtfConvertSpec {
  unsigned short leading_unusable_bits;
  unsigned short leading_bits;
  unsigned short num_trailing_code_point_bits;
  int last_code_point;
  int first_code_point;
};

#pragma pack(pop)

// D2Lang.[0x6FC1D768 to 0x6FC1D7D8)
static const UtfConvertSpec sgUtf8ConvertTable[7] = {
    { 0x80, 0x00, 0, 0x7F, '\0' },
    { 0xE0, 0xC0, 6, 0x7FF, 0x80 },
    { 0xF0, 0xE0, 12, 0xFFFF, 0x800 },
    { 0xF8, 0xF0, 18, 0x1FFFFF, 0x10000 },
    { 0xFC, 0xF8, 24, 0x3FFFFFF, 0x200000 },
    { 0xFE, 0xFC, 30, 0x7FFFFFFF, 0x4000000 },
    { 0 },
};

/**
 * Converts a single UCS-2 character to one or more UTF-8 code units.
 * Returns the number of UTF-8 code units needed to store the
 * converted character.
 *
 * 1.10: Inline
 * 1.13D: D2Lang.0x6FC08DC0
 */
static int UnicodeCharToUtf8CodePoint(char* dest, unsigned short ch) {
  int code_point = ch;

  for (size_t table_index = 0;
      sgUtf8ConvertTable[table_index].leading_unusable_bits != 0;
      ++table_index) {
    if (code_point <= sgUtf8ConvertTable[table_index].last_code_point) {
      unsigned short remaining_bits =
          sgUtf8ConvertTable[table_index].num_trailing_code_point_bits;
      unsigned char leading_bits =
          sgUtf8ConvertTable[table_index].leading_bits;

      dest[0] = (code_point >> remaining_bits) | leading_bits;
      for (size_t dest_index = 1; remaining_bits > 0; ++dest_index) {
        remaining_bits -= 6;
        dest[dest_index] = ((code_point >> remaining_bits) & 0x3FU) | 0x80U;
      }

      return table_index + 1;
    }
  }

  /*
   * This should never happen, as the UTF-8 convert table accounts for
   * characters beyond 0xFFFF.
   */
  return -1;
}

char* __fastcall Unicode::toUtf(char* dest, const Unicode* src, int count) {
  int dest_length = 0;

  for (size_t src_index = 0; src[src_index].ch != L'\0'; ++src_index) {
    /*
     * Vanilla bug: (count - 1) can result in undefined behavior if
     * count is INT_MIN.
     *
     * This check is also insufficient for preventing buffer overrun
     * for code points that require three UTF-8 code units.
     */
    if (dest_length >= count - 1) {
      break;
    }

    int utf8_written_count = UnicodeCharToUtf8CodePoint(
        &dest[dest_length],
        src[src_index].ch);
    if (utf8_written_count == -1) {
       break;
    }

    dest_length += utf8_written_count;
  }

  dest[dest_length] = '\0';

  return dest;
}
