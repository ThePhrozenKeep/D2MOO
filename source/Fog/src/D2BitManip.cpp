#include "D2BitManip.h"

// Fog.6FF71308
const uint32_t gdwBitMasks[] =
{
	0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000,
};

// Fog.6FF71388
const uint32_t gdwInvBitMasks[] =
{
	0xFFFFFFFE, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF7, 0xFFFFFFEF, 0xFFFFFFDF, 0xFFFFFFBF, 0xFFFFFF7F,
	0xFFFFFEFF, 0xFFFFFDFF, 0xFFFFFBFF, 0xFFFFF7FF, 0xFFFFEFFF, 0xFFFFDFFF, 0xFFFFBFFF, 0xFFFF7FFF,
	0xFFFEFFFF, 0xFFFDFFFF, 0xFFFBFFFF, 0xFFF7FFFF, 0xFFEFFFFF, 0xFFDFFFFF, 0xFFBFFFFF, 0xFF7FFFFF,
	0xFEFFFFFF, 0xFDFFFFFF, 0xFBFFFFFF, 0xF7FFFFFF, 0xEFFFFFFF, 0xDFFFFFFF, 0xBFFFFFFF, 0x7FFFFFFF,
};

//Fog.0x6FF71408
uint32_t gdw_6FF71408[] =
{
	0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF
};

//Fog.0x6FF7142C
uint8_t gn_6FF7142C[] =
{
	0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00
};


/*
Function:		BITMANIP_Initialize
Address:		Fog.#10126
Notes:			
*/
void __stdcall BITMANIP_Initialize(D2BitBufferStrc* pBuffer, uint8_t* pBitStream, size_t nSize)
{
	if (!pBuffer)
	{
		return;
	}

	pBuffer->nPos = 0;
	pBuffer->pBuffer = pBitStream;
	pBuffer->nBits = 8 * nSize;
	pBuffer->nPosBits = 0;
	pBuffer->bFull = FALSE;
}

/*
Function:		BITMANIP_GetSize
Address:		Fog.#10127
Notes:
*/
size_t __stdcall BITMANIP_GetSize(D2BitBufferStrc* pBuffer)
{
	return (pBuffer->nPosBits) ? (pBuffer->nPos + 1) : pBuffer->nPos;
}

/*
Function:		BITMANIP_Write
Address:		Fog.#10128
Notes:
*/
void __stdcall BITMANIP_Write(D2BitBufferStrc* pBuffer, uint32_t dwValue, uint32_t dwBits)
{
	uint32_t dwBitsLeft = dwBits;
	uint32_t dwValueEx = dwValue;

	if ((signed int)(dwBits + pBuffer->nPosBits + 8 * pBuffer->nPos) <= (signed int)pBuffer->nBits)
	{
		if (!pBuffer->nPosBits)
		{
			*pBuffer->pBuffer = 0;
		}

		if (dwBits > 0)
		{
			do
			{
				size_t n = 8 - pBuffer->nPosBits;
				if (n > dwBitsLeft)
				{
					n = dwBitsLeft;
				}

				*pBuffer->pBuffer |= ((uint8_t)dwValueEx & gdw_6FF71408[n]) << pBuffer->nPosBits;

				pBuffer->nPosBits += n;
				if (pBuffer->nPosBits != 8)
				{
					break;
				}

				pBuffer->pBuffer = pBuffer->pBuffer + 1;
				++pBuffer->nPos;
				pBuffer->nPosBits = 0;

				if ((signed int)(8 * pBuffer->nPos) < (signed int)pBuffer->nBits)
				{
					*pBuffer->pBuffer = 0;
				}

				dwBitsLeft -= n;
				dwValueEx >>= n;
			}
			while (dwBitsLeft > 0);
		}
	}
	else
	{
		pBuffer->bFull = TRUE;
		pBuffer->nPosBits = 0;
		pBuffer->nPos = pBuffer->nBits / 8;
	}
}

/*
Function:		BITMANIP_ReadSigned
Address:		Fog.#10129
Notes:
*/
int __fastcall BITMANIP_ReadSigned(D2BitBufferStrc* pBuffer, int nBits)
{
	int nResult = BITMANIP_Read(pBuffer, nBits);

	if (nBits < 32)
	{
		if ((1 << (nBits - 1)) & nResult)
		{
			nResult |= ~((1 << nBits) - 1);
		}
	}

	return nResult;
}

/*
Function:		BITMANIP_Read
Address:		Fog.#10130
Notes:
*/
uint32_t __stdcall BITMANIP_Read(D2BitBufferStrc* pBuffer, int nBits)
{
	int n = pBuffer->nPosBits + nBits + 8 * pBuffer->nPos - pBuffer->nBits;

	if ((signed int)n < 0)
	{
		n = 0;
	}

	uint32_t dwBitsLeft = nBits - n;
	if (n)
	{
		pBuffer->bFull = TRUE;
	}

	uint32_t dwValue = 0;
	uint32_t dw = 0;

	if (dwBitsLeft > 0)
	{
		do
		{
			size_t i = 8 - pBuffer->nPosBits;
			if (i > dwBitsLeft)
			{
				i = dwBitsLeft;
			}

			dwBitsLeft -= i;
			dwValue += (gdw_6FF71408[i] & ((uint8_t)(*pBuffer->pBuffer & gn_6FF7142C[pBuffer->nPosBits]) >> pBuffer->nPosBits)) << dw;
			dw += i;
			pBuffer->nPosBits += i;

			if (pBuffer->nPosBits == 8)
			{
				++pBuffer->nPos;
				pBuffer->pBuffer = pBuffer->pBuffer + 1;
				pBuffer->nPosBits = 0;
			}
		}
		while (dwBitsLeft > 0);
	}

	return dwValue;
}

/*
Function:		BITMANIP_GoToNextByte
Address:		Fog.#10131
Notes:
*/
void __stdcall BITMANIP_GoToNextByte(D2BitBufferStrc* pBuffer)
{
	if (pBuffer->nPosBits)
	{
		++pBuffer->nPos;
		pBuffer->pBuffer = pBuffer->pBuffer + 1;
		pBuffer->nPosBits = 0;

		if ((signed int)(8 * pBuffer->nPos) < (signed int)pBuffer->nBits)
		{
			*pBuffer->pBuffer = 0;
		}
	}
}



/*
Function:		BITMANIP_SetBitState
Address:		Fog.#10118
Notes:
*/
void __stdcall BITMANIP_SetBitState(uint8_t* pBitStream, int nBit)
{
	pBitStream[nBit >> 3] |= (uint8_t)gdwBitMasks[nBit & 7];
}

/*
Function:		BITMANIP_GoToNextByte
Address:		Fog.#10119
Notes:
*/
int __stdcall BITMANIP_GetBitState(uint8_t* pBitStream, int nBit)
{
	return gdwBitMasks[nBit & 7] & pBitStream[nBit >> 3];
}

/*
Function:		BITMANIP_GoToNextByte
Address:		Fog.#10120
Notes:
*/
void __stdcall BITMANIP_MaskBitstate(uint8_t* pBitStream, int nBit)
{
	pBitStream[nBit >> 3] &= LOBYTE(gdwInvBitMasks[nBit & 7]);
}
