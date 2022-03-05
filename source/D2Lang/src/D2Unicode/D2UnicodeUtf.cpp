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

#include <Fog.h>

#pragma pack(push, 1)

struct UtfConvertSpec {
  uint16_t leading_unusable_bits;
  uint16_t leading_bits;
  uint16_t num_trailing_code_point_bits;
  int32_t last_code_point;
  int32_t first_code_point;
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
      uint16_t remaining_bits =
          sgUtf8ConvertTable[table_index].num_trailing_code_point_bits;
      uint8_t leading_bits =
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


/**
 * Converts one or more UTF-8 code point to a single UCS-2 character.
 * Returns the number of UTF-8 code units that were converted.
 *
 * 1.10: Inline
 * 1.13D: D2Lang.0x6FC08C40
 */
static int Utf8CodePointToUnicodeChar(Unicode* dest, const char* src, int count) {
  if (count < 1) {
    return -1;
  }

  uint16_t ucs2 = src[0];

  for (int i = 0; sgUtf8ConvertTable[i].leading_unusable_bits != 0; ++i) {
    uint16_t leading_bits = sgUtf8ConvertTable[i].leading_unusable_bits
        & src[0];

    if (leading_bits == sgUtf8ConvertTable[i].leading_bits) {
      ucs2 &= sgUtf8ConvertTable[i].last_code_point;
      if (ucs2 < sgUtf8ConvertTable[i].first_code_point) {
        return -1;
      }

      dest->ch = ucs2;
      return i + 1;
    }

    if ((i + 1) >= count) {
      return -1;
    }

    /*
     * Check if the trailing bytes are valid:
     * 10XX'XXXX
     */
    uint8_t trailing_bits = src[i + 1] ^ 0x80u;
    if ((trailing_bits & (unsigned char)0xC0) != 0) {
      return -1;
    }

    ucs2 = (ucs2 << 6) | trailing_bits;
  }

  return -1;
}

Unicode* __fastcall Unicode::toUnicode(
    Unicode* dest,
    const char* src,
    int count) {
  D2_ASSERT(count >= 0);

  size_t src_length = ::strlen(src);

  /*
    * Vanilla bug: (count - 1) can result in undefined behavior if
    * count is INT_MIN.
    */
  int i_dest = 0;
  int i_src = 0; 
  while (src[i_src] != '\0' && i_dest < (count - 1)) {
    int num_code_units = Utf8CodePointToUnicodeChar(
        &dest[i_dest],
        &src[i_src],
        src_length - i_src);

    ++i_dest;

    if (num_code_units == -1) {
      break;
    }

    i_src += num_code_units;
  }

  dest[i_dest].ch = L'\0';
  return dest;
}

char* __fastcall Unicode::toUtf(char* dest, const Unicode* src, int count) {
  D2_ASSERT(count >= 0);

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
