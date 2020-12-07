#pragma once

#include <Windows.h>

#define ARRAY_SIZE(Array) (sizeof(Array) / sizeof(Array[0]))

//#define LOBYTE(x)	(*((BYTE*)&(x)))   // low byte
//#define LOWORD(x)	(*((WORD*)&(x)))   // low word
#define LODWORD(x)	(*((DWORD*)&(x)))  // low dword
//#define HIBYTE(x)	(*((BYTE*)&(x)+1))
//#define HIWORD(x)	(*((WORD*)&(x)+1))
#define HIDWORD(x)	(*((DWORD*)&(x)+1))
#define BYTEn(x,n)	(*((BYTE*)&(x)+n))
#define WORDn(x,n)	(*((WORD*)&(x)+n))
#define BYTE0(x)	BYTEn(x,  0)
#define BYTE1(x)	BYTEn(x,  1)         // byte 1 (counting from 0)
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
#define WORD2(x)	WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)	WORDn(x,  3)
#define WORD4(x)	WORDn(x,  4)
#define WORD5(x)	WORDn(x,  5)
#define WORD6(x)	WORDn(x,  6)
#define WORD7(x)	WORDn(x,  7)

extern const DWORD gdwBitMasks[];
extern const DWORD gdwInvBitMasks[];

template<class T>
constexpr const T& D2Clamp(const T& v, const T& low, const T& high)
{
    return (v < low) ? low : (high < v) ? high : v;
}

template<class T>
T D2Lerp(T v0, T v1, T t) {
    return (1 - t) * v0 + t * v1;
}
