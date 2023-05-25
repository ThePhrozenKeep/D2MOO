#pragma once

#include <D2BasicTypes.h>

#include <Fog.h>

#pragma pack(1)

extern FOG_DLL_DECL const uint32_t gdwBitMasks[];
extern FOG_DLL_DECL const uint32_t gdwInvBitMasks[];


struct D2BitBufferStrc
{
	uint8_t* pBuffer;							//0x00
	int32_t nBits;								//0x04
	int32_t nPos;								//0x08
	int32_t nPosBits;							//0x0C
	BOOL bFull;									//0x10
};

#pragma pack()

/*
Function:		BITMANIP_Initialize
Address:		Fog.#10126
Notes:
*/
FOG_DLL_DECL void __stdcall BITMANIP_Initialize(D2BitBufferStrc* pBuffer, uint8_t* pBitStream, size_t nSize);

/*
Function:		BITMANIP_GetSize
Address:		Fog.#10127
Notes:
*/
FOG_DLL_DECL size_t __stdcall BITMANIP_GetSize(D2BitBufferStrc* pBuffer);

/*
Function:		BITMANIP_Write
Address:		Fog.#10128
Notes:
*/
FOG_DLL_DECL void __stdcall BITMANIP_Write(D2BitBufferStrc* pBuffer, uint32_t dwValue, uint32_t dwBits);

/*
Function:		BITMANIP_ReadSigned
Address:		Fog.#10129
Notes:
*/
FOG_DLL_DECL int __stdcall BITMANIP_ReadSigned(D2BitBufferStrc* pBuffer, int nBits);

/*
Function:		BITMANIP_Read
Address:		Fog.#10130
Notes:
*/
FOG_DLL_DECL uint32_t __stdcall BITMANIP_Read(D2BitBufferStrc* pBuffer, int nBits);

/*
Function:		BITMANIP_GoToNextByte
Address:		Fog.#10131
Notes:
*/
FOG_DLL_DECL void __stdcall BITMANIP_GoToNextByte(D2BitBufferStrc* pBuffer);




/*
Function:		BITMANIP_SetBitState
Address:		Fog.#10118
Notes:
*/
void __stdcall BITMANIP_SetBitState(uint8_t* pBitStream, int nBit);

/*
Function:		BITMANIP_GoToNextByte
Address:		Fog.#10119
Notes:
*/
int __stdcall BITMANIP_GetBitState(uint8_t* pBitStream, int nBit);

/*
Function:		BITMANIP_GoToNextByte
Address:		Fog.#10120
Notes:
*/
void __stdcall BITMANIP_MaskBitstate(uint8_t* pBitStream, int nBit);


// Sets bits of the mask to newBitValue, and returns wether the value changed
template<typename T>
bool BITMANIP_SetBitsValueForMask(T& var, uint64_t mask, bool newBitValue)
{
	T previousValue = var;
	if (newBitValue)
	{
		var |= mask;
	}
	else
	{
		var &= ~mask;
	}
	return previousValue != var;
}
