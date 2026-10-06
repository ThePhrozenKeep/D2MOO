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

#include <stddef.h>

#include <D2Lang.h>
#include <Fog.h>

#define CRC_LOOKUP_TABLE_COUNT 256

/* Upstream built this table by calling GenerateCrc() once per index. That function and
 * its table are kept below verbatim, since they define the contents, but the literals
 * that actually get compiled are the expanded ones underneath: VC6 has no constexpr,
 * and retail carries a finished table in .rdata rather than building one at run time.
 *
 * tools/gen_crc16_table.py regenerates the literals from the function below and checks
 * them against retail 1.10f, where all 256 entries come out byte-identical.
 */

// /**
//  * Generates and returns the CRC-16 value that would be used in the
//  * CRC-16 lookup table, for a given index in the lookup table.
//  *
//  * More information regarding the generation of the table:
//  * https://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks#Generating_the_tables
//  */
// static constexpr unsigned short GenerateCrc(unsigned char i) {
//   unsigned short crc = i;
//
//   for (size_t bit = 0; bit < 16; ++bit) {
//     if (crc & 0x8000) {
//       crc = (crc << 1) ^ 0x1021;
//     } else {
//       crc <<= 1;
//     }
//   }
//
//   return crc;
// }
//
// /* [6FC1D7F0 to 6FC1D9F0) */
// static constexpr unsigned short sgCrc16LookupTable[CRC_LOOKUP_TABLE_COUNT] = {
//     GenerateCrc(0), GenerateCrc(1), GenerateCrc(2), GenerateCrc(3),
//     GenerateCrc(4), GenerateCrc(5), GenerateCrc(6), GenerateCrc(7),
//     GenerateCrc(8), GenerateCrc(9), GenerateCrc(10), GenerateCrc(11),
//     GenerateCrc(12), GenerateCrc(13), GenerateCrc(14), GenerateCrc(15),
//     GenerateCrc(16), GenerateCrc(17), GenerateCrc(18), GenerateCrc(19),
//     GenerateCrc(20), GenerateCrc(21), GenerateCrc(22), GenerateCrc(23),
//     GenerateCrc(24), GenerateCrc(25), GenerateCrc(26), GenerateCrc(27),
//     GenerateCrc(28), GenerateCrc(29), GenerateCrc(30), GenerateCrc(31),
//     GenerateCrc(32), GenerateCrc(33), GenerateCrc(34), GenerateCrc(35),
//     GenerateCrc(36), GenerateCrc(37), GenerateCrc(38), GenerateCrc(39),
//     GenerateCrc(40), GenerateCrc(41), GenerateCrc(42), GenerateCrc(43),
//     GenerateCrc(44), GenerateCrc(45), GenerateCrc(46), GenerateCrc(47),
//     GenerateCrc(48), GenerateCrc(49), GenerateCrc(50), GenerateCrc(51),
//     GenerateCrc(52), GenerateCrc(53), GenerateCrc(54), GenerateCrc(55),
//     GenerateCrc(56), GenerateCrc(57), GenerateCrc(58), GenerateCrc(59),
//     GenerateCrc(60), GenerateCrc(61), GenerateCrc(62), GenerateCrc(63),
//     GenerateCrc(64), GenerateCrc(65), GenerateCrc(66), GenerateCrc(67),
//     GenerateCrc(68), GenerateCrc(69), GenerateCrc(70), GenerateCrc(71),
//     GenerateCrc(72), GenerateCrc(73), GenerateCrc(74), GenerateCrc(75),
//     GenerateCrc(76), GenerateCrc(77), GenerateCrc(78), GenerateCrc(79),
//     GenerateCrc(80), GenerateCrc(81), GenerateCrc(82), GenerateCrc(83),
//     GenerateCrc(84), GenerateCrc(85), GenerateCrc(86), GenerateCrc(87),
//     GenerateCrc(88), GenerateCrc(89), GenerateCrc(90), GenerateCrc(91),
//     GenerateCrc(92), GenerateCrc(93), GenerateCrc(94), GenerateCrc(95),
//     GenerateCrc(96), GenerateCrc(97), GenerateCrc(98), GenerateCrc(99),
//     GenerateCrc(100), GenerateCrc(101), GenerateCrc(102), GenerateCrc(103),
//     GenerateCrc(104), GenerateCrc(105), GenerateCrc(106), GenerateCrc(107),
//     GenerateCrc(108), GenerateCrc(109), GenerateCrc(110), GenerateCrc(111),
//     GenerateCrc(112), GenerateCrc(113), GenerateCrc(114), GenerateCrc(115),
//     GenerateCrc(116), GenerateCrc(117), GenerateCrc(118), GenerateCrc(119),
//     GenerateCrc(120), GenerateCrc(121), GenerateCrc(122), GenerateCrc(123),
//     GenerateCrc(124), GenerateCrc(125), GenerateCrc(126), GenerateCrc(127),
//     GenerateCrc(128), GenerateCrc(129), GenerateCrc(130), GenerateCrc(131),
//     GenerateCrc(132), GenerateCrc(133), GenerateCrc(134), GenerateCrc(135),
//     GenerateCrc(136), GenerateCrc(137), GenerateCrc(138), GenerateCrc(139),
//     GenerateCrc(140), GenerateCrc(141), GenerateCrc(142), GenerateCrc(143),
//     GenerateCrc(144), GenerateCrc(145), GenerateCrc(146), GenerateCrc(147),
//     GenerateCrc(148), GenerateCrc(149), GenerateCrc(150), GenerateCrc(151),
//     GenerateCrc(152), GenerateCrc(153), GenerateCrc(154), GenerateCrc(155),
//     GenerateCrc(156), GenerateCrc(157), GenerateCrc(158), GenerateCrc(159),
//     GenerateCrc(160), GenerateCrc(161), GenerateCrc(162), GenerateCrc(163),
//     GenerateCrc(164), GenerateCrc(165), GenerateCrc(166), GenerateCrc(167),
//     GenerateCrc(168), GenerateCrc(169), GenerateCrc(170), GenerateCrc(171),
//     GenerateCrc(172), GenerateCrc(173), GenerateCrc(174), GenerateCrc(175),
//     GenerateCrc(176), GenerateCrc(177), GenerateCrc(178), GenerateCrc(179),
//     GenerateCrc(180), GenerateCrc(181), GenerateCrc(182), GenerateCrc(183),
//     GenerateCrc(184), GenerateCrc(185), GenerateCrc(186), GenerateCrc(187),
//     GenerateCrc(188), GenerateCrc(189), GenerateCrc(190), GenerateCrc(191),
//     GenerateCrc(192), GenerateCrc(193), GenerateCrc(194), GenerateCrc(195),
//     GenerateCrc(196), GenerateCrc(197), GenerateCrc(198), GenerateCrc(199),
//     GenerateCrc(200), GenerateCrc(201), GenerateCrc(202), GenerateCrc(203),
//     GenerateCrc(204), GenerateCrc(205), GenerateCrc(206), GenerateCrc(207),
//     GenerateCrc(208), GenerateCrc(209), GenerateCrc(210), GenerateCrc(211),
//     GenerateCrc(212), GenerateCrc(213), GenerateCrc(214), GenerateCrc(215),
//     GenerateCrc(216), GenerateCrc(217), GenerateCrc(218), GenerateCrc(219),
//     GenerateCrc(220), GenerateCrc(221), GenerateCrc(222), GenerateCrc(223),
//     GenerateCrc(224), GenerateCrc(225), GenerateCrc(226), GenerateCrc(227),
//     GenerateCrc(228), GenerateCrc(229), GenerateCrc(230), GenerateCrc(231),
//     GenerateCrc(232), GenerateCrc(233), GenerateCrc(234), GenerateCrc(235),
//     GenerateCrc(236), GenerateCrc(237), GenerateCrc(238), GenerateCrc(239),
//     GenerateCrc(240), GenerateCrc(241), GenerateCrc(242), GenerateCrc(243),
//     GenerateCrc(244), GenerateCrc(245), GenerateCrc(246), GenerateCrc(247),
//     GenerateCrc(248), GenerateCrc(249), GenerateCrc(250), GenerateCrc(251),
//     GenerateCrc(252), GenerateCrc(253), GenerateCrc(254), GenerateCrc(255),
// };

/* [6FC1D7F0 to 6FC1D9F0) */
static D2_CONSTEXPR unsigned short sgCrc16LookupTable[CRC_LOOKUP_TABLE_COUNT] = {
    0x0000, 0x1021, 0x2042, 0x3063,
    0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B,
    0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252,
    0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A,
    0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401,
    0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509,
    0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630,
    0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738,
    0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7,
    0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF,
    0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96,
    0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E,
    0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5,
    0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD,
    0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4,
    0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC,
    0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB,
    0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3,
    0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA,
    0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2,
    0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589,
    0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481,
    0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8,
    0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0,
    0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F,
    0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827,
    0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E,
    0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16,
    0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D,
    0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45,
    0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C,
    0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74,
    0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
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
    *crc16_checksum_out ^= sgCrc16LookupTable[lookup_index];
  }
}
