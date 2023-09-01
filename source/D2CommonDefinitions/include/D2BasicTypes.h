#pragma once

#include <cstdint>
#include <Windows.h>

typedef int32_t BOOL;

// We want all of the structures to be packed since they come from reverse engineering
// This also means that if we use external headers, they should be included before this header.
#pragma warning(error:4820 4121)
#pragma warning(disable:28612) // "The enum type 'xxxx' is unscoped. Prefer 'enum class' over 'enum'"

// Windows.h defines its own, replace it
#undef LOBYTE
#undef HIBYTE
#undef LOWORD
#undef HIWORD
#undef LODWORD
#undef HIDWORD

#define LOBYTE(w)           ((uint8_t ) (( (uint16_t)(w))        & 0xff  ))
#define HIBYTE(w)           ((uint8_t ) ((((uint16_t)(w)) >>  8) & 0xff  ))
#define LOWORD(l)           ((uint16_t) (( (uint32_t)(l))        & 0xffff))
#define HIWORD(l)           ((uint16_t) ((((uint32_t)(l)) >> 16) & 0xffff))
#define LODWORD(l)          ((uint32_t) (( (uint64_t)(l))        & 0xffff))
#define HIDWORD(l)          ((uint32_t) ((((uint64_t)(l)) >> 16) & 0xffff))

#define BYTEn(x,n)	((uint8_t ) ((((uint64_t)(x)) >> ( 8*n)) & 0xff  ))
#define WORDn(x,n)	((uint16_t) ((((uint64_t)(x)) >> (16*n)) & 0xffff))
#define BYTE0(x)	BYTEn(x,  0)
#define BYTE1(x)	BYTEn(x,  1)
#define BYTE2(x)	BYTEn(x,  2)
#define BYTE3(x)	BYTEn(x,  3)
#define BYTE4(x)	BYTEn(x,  4)
#define BYTE5(x)	BYTEn(x,  5)
#define BYTE6(x)	BYTEn(x,  6)
#define BYTE7(x)	BYTEn(x,  7)
#define BYTE8(x)	BYTEn(x,  8)
#define BYTE9(x)	BYTEn(x,  9)
#define BYTE10(x)	BYTEn(x, 10)
#define BYTE11(x)	BYTEn(x, 11)
#define BYTE12(x)	BYTEn(x, 12)
#define BYTE13(x)	BYTEn(x, 13)
#define BYTE14(x)	BYTEn(x, 14)
#define BYTE15(x)	BYTEn(x, 15)
#define WORD0(x)	WORDn(x,  0)
#define WORD1(x)	WORDn(x,  1)
#define WORD2(x)	WORDn(x,  2)
#define WORD3(x)	WORDn(x,  3)
#define WORD4(x)	WORDn(x,  4)
#define WORD5(x)	WORDn(x,  5)
#define WORD6(x)	WORDn(x,  6)
#define WORD7(x)	WORDn(x,  7)

#define ARRAY_SIZE(Array) (sizeof(Array) / sizeof(Array[0]))

#define D2_MAYBE_UNUSED(x) (void)x

#pragma pack(push, 1)

using D2UnitGUID = uint32_t;
constexpr D2UnitGUID D2UnitInvalidGUID = (D2UnitGUID)-1;

struct D2CoordStrc							//sizeof 0x08
{
	int nX;									//0x00
	int nY;									//0x04
};

#pragma pack(pop)
