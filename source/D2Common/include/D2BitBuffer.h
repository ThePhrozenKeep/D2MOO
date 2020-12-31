#pragma once

#include "CommonDefinitions.h"

#include "D2Math.h"

/*
Function:		BITBUFFER_Initialize
Address:		Fog.#10126
Notes:
*/
void __stdcall BITBUFFER_Initialize(D2BitBufferStrc* pBuffer, uint8_t* pBitStream, size_t nSize);

/*
Function:		BITBUFFER_GetSize
Address:		Fog.#10127
Notes:
*/
size_t __stdcall BITBUFFER_GetSize(D2BitBufferStrc* pBuffer);

/*
Function:		BITBUFFER_Write
Address:		Fog.#10128
Notes:
*/
void __stdcall BITBUFFER_Write(D2BitBufferStrc* pBuffer, uint32_t dwValue, uint32_t dwBits);

/*
Function:		BITBUFFER_ReadSigned
Address:		Fog.#10129
Notes:
*/
int __fastcall BITBUFFER_ReadSigned(D2BitBufferStrc* pBuffer, int nBits);

/*
Function:		BITBUFFER_Read
Address:		Fog.#10130
Notes:
*/
uint32_t __stdcall BITBUFFER_Read(D2BitBufferStrc* pBuffer, int nBits);

/*
Function:		BITBUFFER_GoToNextByte
Address:		Fog.#10131
Notes:
*/
void __stdcall BITBUFFER_GoToNextByte(D2BitBufferStrc* pBuffer);




/*
Function:		BITBUFFER_SetBitState
Address:		Fog.#10118
Notes:
*/
void __stdcall BITBUFFER_SetBitState(uint8_t* pBitBuffer, int nBit);

/*
Function:		BITBUFFER_GoToNextByte
Address:		Fog.#10119
Notes:
*/
int __stdcall BITBUFFER_GetBitState(uint8_t* pBitBuffer, int nBit);

/*
Function:		BITBUFFER_GoToNextByte
Address:		Fog.#10120
Notes:
*/
void __stdcall BITBUFFER_ResetBitstate(uint8_t* pBitBuffer, int nBit);
