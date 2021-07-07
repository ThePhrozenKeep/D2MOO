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

#include <D2Crc16.h>
#include <D2Lang.h>
#include <Fog.h>

/*
* A majority of this is extremely messy due to the use of macros to
* constant-initialize a lookup table. It would be faster to hardcode
* the table directly, but I wanted to provide explanations that could
* later be used to rewrite the lookup table with transformation
* matrices.
*/

#define CRC_N(n) (((n) * 0x1021) & 0xFFFF)

/*
* Each entry is in sets of 16 (i.e. to have 4x4 matrices). They are
* swapped, shifted, and subtracted from in order to produce the CRC-16
* lookup table entries.
*/

#define CRC_SET(crc, start) \
    crc(start), crc(start + 1), crc(start + 2), crc(start + 3), \
    crc(start + 4), crc(start + 5), crc(start + 6), crc(start + 7), \
    crc(start + 8), crc(start + 9), crc(start + 10), crc(start + 11), \
    crc(start + 12), crc(start + 13), crc(start + 14), crc(start + 15)

/**
 * Reverses the order of columns (for a single entry), resulting in:
 * [(n + 3), (n + 2), (n + 1), (n)]
 */
#define REVERSE_CRC_COL_ORDER(n) \
    ((n) + (3 - (((n) % 4) * 2)))

/**
 * Reverses the order of rows (for a single entry), resulting in:
 * [(n + 12), ...]
 * [(n + 8), ...]
 * [(n + 4), ...]
 * [(n), ...]
 */
#define REVERSE_CRC_ROW_ORDER(n) \
    ((n) + (12 - ((((n) % 16) / 4) * 8)))

/**
 * Swaps the columns 0 and 1 (for a single entry), resulting
 * in:
 * [(n + 4), n, (n + 12), (n + 8)]
 */
#define SWAP_CRC_COLS_0_AND_1(n) \
    (((n) % 2 == 0) ? ((n) + 1) : ((n) - 1))

/**
 * Swaps the columns [0, 1] and [2, 3] (for a single entry), resulting
 * in:
 * [(n + 8), (n + 12), n, (n + 4)]
 */
#define SWAP_CRC_COLS_0_1_AND_2_3(n) \
    (((n) % 4 < 2) ? ((n) + 2) : ((n) - 2))

/**
 * Swaps the rows 0 and 1 (for a single entry), resulting in:
 * [(n + 4), ...]
 * [(n), ...]
 * [(n + 12), ...]
 * [(n + 8), ...]
 */
#define SWAP_CRC_ROWS_0_AND_1(n) \
    (((n) % 8 < 4) ? ((n) + 4) : ((n) - 4))

/**
 * Swaps the rows [0, 1] and [2, 3] (for a single entry), resulting
 * in:
 * [(n + 8), ...]
 * [(n + 12), ...]
 * [(n), ...]
 * [(n + 4), ...]
 */
#define SWAP_CRC_ROWS_0_1_AND_2_3(n) \
    (((n) % 16 < 8) ? ((n) + 8) : ((n) - 8))


/*
* No special rules apply.
*/

#define CRC_0_15_IMPL(n) CRC_N(n)

#define CRC_0_15(n) (unsigned short)(CRC_0_15_IMPL((n)))

/*
* If n is even, calculate for n + 1, otherwise n - 1.
*/

#define CRC_16_31_IMPL(n) CRC_N(SWAP_CRC_COLS_0_AND_1(n))

#define CRC_16_31(n) (unsigned short)(CRC_16_31_IMPL((n)))

/*
* For n is divided by 4, calculate for the remainders:
* 0 - 1: n + 2
* 2 - 3: n - 2
*
* In addition, if n is odd, then subtract 64.
*/

#define CRC_32_47_IMPL(n) \
    CRC_N(SWAP_CRC_COLS_0_1_AND_2_3(n)) \
        - ((n % 2 == 0) ? 0 : 64)

#define CRC_32_47(n) (unsigned short)(CRC_32_47_IMPL((n)))

/*
* For n is divided by 4, calculate for the remainders:
* 0: n + 3
* 1: n + 1
* 2: n - 1
* 3: n - 3
*
* In addition, if n is even, then subtract 64.
*/

#define CRC_48_63_IMPL(n) \
    CRC_N(REVERSE_CRC_COL_ORDER(n)) \
        - ((n % 2 == 0) ? 64 : 0)

#define CRC_48_63(n) (unsigned short)(CRC_48_63_IMPL((n)))

/*
* For n is divided by 8, calculate for the remainders:
* 0 - 3: n + 4
* 4 - 7: n - 4
*
* In addition, if after dividing n by 4, the remainder is greater than
* or equal to 2, then subtract 128.
*/

#define CRC_64_79_IMPL(n) \
    CRC_N(SWAP_CRC_ROWS_0_AND_1(n)) \
        - ((n % 4 < 2) ? 0 : 128)

#define CRC_64_79(n) (unsigned short)(CRC_64_79_IMPL((n)))

/*
* For n is divided by 8, calculate for the remainders:
* 0: n + 5
* 1: n + 3
* 2: n + 5
* 3: n + 3
* 4: n - 3
* 5: n - 5
* 6: n - 3
* 7: n - 5
*
* In addition, if after dividing n by 4, the remainder is greater than
* or equal to 2, then subtract 128.
*/

#define CRC_80_95_IMPL(n) \
    CRC_N(SWAP_CRC_ROWS_0_AND_1(SWAP_CRC_COLS_0_AND_1(n))) \
        - ((n % 4 < 2) ? 0 : 128)

#define CRC_80_95(n) (unsigned short)(CRC_80_95_IMPL((n)))


/*
* For n is divided by 8, calculate for the remainders:
* 0: n + 6
* 1: n + 6
* 2: n + 2
* 3: n + 2
* 4: n - 2
* 5: n - 2
* 6: n - 6
* 7: n - 6
*
* In addition, for n is divided by 4, subtract for the remainders:
* 0: 128
* 1: 192
* 2: 0
* 3: 64
*/

#define CRC_96_111_IMPL(n) \
    CRC_N(SWAP_CRC_ROWS_0_AND_1(SWAP_CRC_COLS_0_1_AND_2_3(n))) \
        - (((n + 2) % 4) * 64)

#define CRC_96_111(n) (unsigned short)(CRC_96_111_IMPL((n)))

/*
* For n is divided by 4, calculate for the remainders:
* 0: n + 7
* 1: n + 5
* 2: n + 3
* 3: n + 1
* 4: n - 1
* 5: n - 3
* 6: n - 5
* 7: n - 7
*
* In addition, for n is divided by 4, subtract for the remainders:
* 0: 192
* 1: 128
* 2: 64
* 3: 0
*/

#define CRC_112_127_IMPL(n) \
    CRC_N(SWAP_CRC_ROWS_0_AND_1(REVERSE_CRC_COL_ORDER(n))) \
        - ((3 - (n % 4)) * 64)

#define CRC_112_127(n) (unsigned short)(CRC_112_127_IMPL((n)))

/*
* For n is divided by 16, calculate for the remainders:
* 0 - 7: n + 8
* 8 - 15: n - 8
*
* In addition, if n is odd, then subtract by 8192.
*
* In addition, for n is divided by 8, if the remainder is greater than
* or equal to 4, then subtract by 256.
*/

#define CRC_128_143_IMPL(n) \
    CRC_N(SWAP_CRC_ROWS_0_1_AND_2_3(n)) \
        - ((n % 2 == 0) ? 0 : 8192) \
        - ((n % 8 < 4) ? 0 : 256)

#define CRC_128_143(n) (unsigned short)(CRC_128_143_IMPL((n)))

/*
* For n is divided by 16, calculate for the remainders:
* 0 - 7: n + 8
* 8 - 15: n - 8
*
* On top of that, for n is divided by 2, calculate for the remainders:
* 0: n + 1
* 1: n - 1
*
* In addition, if n is even, then subtract by 8192.
*
* In addition, for n is divided by 8, if the remainder is greater than
* or equal to 4, then subtract by 256.
*/

#define CRC_144_159_IMPL(n) \
    CRC_N(SWAP_CRC_ROWS_0_1_AND_2_3(SWAP_CRC_COLS_0_AND_1(n))) \
        - ((n % 8 < 4) ? 0 : 256) \
        - ((n % 2 == 0) ? 8192 : 0)

#define CRC_144_159(n) (unsigned short)(CRC_144_159_IMPL((n)))

/*
* For n is divided by 16, calculate for the remainders:
* 0 - 7: n + 8
* 8 - 15: n - 8
*
* On top of that, for n is divided by 4, calculate for the remainders:
* 0 - 1: n + 2
* 2 - 3: n - 2
*
* In addition, if n is odd, then subtract by 8256 (i.e. 8192 + 64).
*
* In addition, for n is divided by 8, if the remainder is greater than
* or equal to 4, then subtract by 256.
*/

#define CRC_160_175_IMPL(n) \
    CRC_N(SWAP_CRC_ROWS_0_1_AND_2_3(SWAP_CRC_COLS_0_1_AND_2_3(n))) \
        - ((n % 2 == 0) ? 0 : 8256) \
        - ((n % 8 < 4) ? 0 : 256)

#define CRC_160_175(n) (unsigned short)(CRC_160_175_IMPL((n)))

/*
* For n is divided by 16, calculate for the remainders:
* 0 - 7: n + 8
* 8 - 15: n - 8
*
* On top of that, for n is divided by 4, calculate for the remainders:
* 0: n + 3
* 1: n + 1
* 2: n - 1
* 3: n - 3
*
* In addition, if n is even, then subtract by 8256 (i.e. 8192 + 64).
*
* In addition, for n is divided by 8, if the remainder is greater than
* or equal to 4, then subtract by 256.
*/

#define CRC_176_191_IMPL(n) \
    CRC_N(SWAP_CRC_ROWS_0_1_AND_2_3(REVERSE_CRC_COL_ORDER(n))) \
        - ((n % 2 == 0) ? 8256 : 0) \
        - ((n % 8 < 4) ? 0 : 256)

#define CRC_176_191(n) (unsigned short)(CRC_176_191_IMPL((n)))

/*
* For n is divided by 16, calculate for the remainders:
* 0 - 3: n + 12
* 4 - 7: n + 4
* 8 - 11: n - 4
* 12 - 15: n - 12
*
* In addition, if n is odd, then subtract by 8192.
*
* In addition, for n is divided by 4, if the remainder is greater than
* or equal to 2, then subtract by 128.
*
* In addition, for n is divided by 8, if the remainder is less than 4,
* then subtract by 256.
*/

#define CRC_192_207_IMPL(n) \
    CRC_N(REVERSE_CRC_ROW_ORDER(n)) \
        - ((n % 2 == 0) ? 0 : 8192) \
        - ((n % 4 < 2) ? 0 : 128) \
        - ((n % 8 < 4) ? 256 : 0)

#define CRC_192_207(n) (unsigned short)(CRC_192_207_IMPL((n)))

/*
* For n is divided by 16, calculate for the remainders:
* 0 - 3: n + 12
* 4 - 7: n + 4
* 8 - 11: n - 4
* 12 - 15: n - 12
*
* On top of that, for n is divided by 2, calculate for the remainders:
* 0: n + 1
* 1: n - 1
*
* In addition, if n is even, then subtract by 8192.
*
* In addition, for n is divided by 4, if the remainder is greater than
* or equal to 2, then subtract by 128.
*
* In addition, for n is divided by 8, if the remainder is less than 4,
* then subtract by 256.
*/

#define CRC_208_223_IMPL(n) \
    CRC_N(REVERSE_CRC_ROW_ORDER(SWAP_CRC_COLS_0_AND_1(n))) \
        - ((n % 4 < 2) ? 0 : 128) \
        - ((n % 8 < 4) ? 256 : 0) \
        - ((n % 2 == 0) ? 8192 : 0)

#define CRC_208_223(n) (unsigned short)(CRC_208_223_IMPL((n)))

/*
* For n is divided by 16, calculate for the remainders:
* 0 - 3: n + 12
* 4 - 7: n + 4
* 8 - 11: n - 4
* 12 - 15: n - 12
*
* On top of that, for n is divided by 4, calculate for the remainders:
* 0 - 1: n + 2
* 2 - 3: n - 2
*
* In addition, if n is odd, then subtract by 8192.
*
* In addition, for n is divided by 8, if the remainder is less than 4,
* then subtract by 256.
*
* In addition, for n is divided by 4, subtract for the remainders:
* 0: 128
* 1: 192
* 2: 0
* 3: 64
*/

#define CRC_224_239_IMPL(n) \
    CRC_N(REVERSE_CRC_ROW_ORDER(SWAP_CRC_COLS_0_1_AND_2_3(n))) \
        - ((n % 2 == 0) ? 0 : 8192) \
        - ((n % 8 < 4) ? 256 : 0) \
        - (((n + 2) % 4) * 64)

#define CRC_224_239(n) (unsigned short)(CRC_224_239_IMPL((n)))

/*
* For n is divided by 16, calculate for the remainders:
* 0 - 3: n + 12
* 4 - 7: n + 4
* 8 - 11: n - 4
* 12 - 15: n - 12
*
* On top of that, for n is divided by 4, calculate for the remainders:
* 0: n + 3
* 1: n + 1
* 2: n - 1
* 3: n - 3
*
* In addition, if n is even, then subtract by 8192.
*
* In addition, for n is divided by 8, if the remainder is less than 4,
* then subtract by 256.
*
* In addition, for n is divided by 4, subtract for the remainders:
* 0: 192
* 1: 128
* 2: 64
* 3: 0
*/

#define CRC_240_255_IMPL(n) \
    CRC_N(REVERSE_CRC_ROW_ORDER(REVERSE_CRC_COL_ORDER(n))) \
        - ((n % 2 == 0) ? 8192 : 0) \
        - ((n % 8 < 4) ? 256 : 0) \
        - ((3 - (n % 4)) * 64)

#define CRC_240_255(n) (unsigned short)(CRC_240_255_IMPL((n)))

#define CRC_LOOKUP_TABLE_COUNT 256

/* [6FC1D7F0 to 6FC1D9F0) */
static const unsigned short crc16_lookup_table[CRC_LOOKUP_TABLE_COUNT] = {
  CRC_SET(CRC_0_15, 0),
  CRC_SET(CRC_16_31, 16),
  CRC_SET(CRC_32_47, 32),
  CRC_SET(CRC_48_63, 48),
  CRC_SET(CRC_64_79, 64),
  CRC_SET(CRC_80_95, 80),
  CRC_SET(CRC_96_111, 96),
  CRC_SET(CRC_112_127, 112),
  CRC_SET(CRC_128_143, 128),
  CRC_SET(CRC_144_159, 144),
  CRC_SET(CRC_160_175, 160),
  CRC_SET(CRC_176_191, 176),
  CRC_SET(CRC_192_207, 192),
  CRC_SET(CRC_208_223, 208),
  CRC_SET(CRC_224_239, 224),
  CRC_SET(CRC_240_255, 240),
};

void __stdcall CRC16_CalculateChecksum(
    unsigned short* crc16_checksum_out,
    const unsigned char* data,
    size_t start_index,
    size_t end_index) {
  size_t i;
  unsigned char high_byte;
  size_t lookup_index;

  D2_ASSERT(start_index < end_index);

  *crc16_checksum_out = 0xFFFF;

  for (i = start_index; i < end_index; ++i) {
    high_byte = *crc16_checksum_out >> 8;
    lookup_index = high_byte ^ data[i];

    *crc16_checksum_out <<= 8;
    *crc16_checksum_out ^= crc16_lookup_table[lookup_index];
  }
}
