#include "D2BitBuffer.h"


//Fog.0x6FF71408
DWORD gdw_6FF71408[] =
{
	0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF
};

//Fog.0x6FF7142C
BYTE gn_6FF7142C[] =
{
	0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00
};


/*
Function:		BITBUFFER_Initialize
Address:		Fog.#10126
Notes:			
*/
void __stdcall BITBUFFER_Initialize(D2BitBufferStrc* pBuffer, BYTE* pBitStream, size_t nSize)
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
Function:		BITBUFFER_GetSize
Address:		Fog.#10127
Notes:
*/
size_t __stdcall BITBUFFER_GetSize(D2BitBufferStrc* pBuffer)
{
	return (pBuffer->nPosBits) ? (pBuffer->nPos + 1) : pBuffer->nPos;
}

/*
Function:		BITBUFFER_Write
Address:		Fog.#10128
Notes:
*/
void __stdcall BITBUFFER_Write(D2BitBufferStrc* pBuffer, DWORD dwValue, DWORD dwBits)
{
	DWORD dwBitsLeft = dwBits;
	DWORD dwValueEx = dwValue;

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

				*pBuffer->pBuffer |= ((BYTE)dwValueEx & gdw_6FF71408[n]) << pBuffer->nPosBits;

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
Function:		BITBUFFER_ReadSigned
Address:		Fog.#10129
Notes:
*/
int __fastcall BITBUFFER_ReadSigned(D2BitBufferStrc* pBuffer, int nBits)
{
	int nResult = BITBUFFER_Read(pBuffer, nBits);

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
Function:		BITBUFFER_Read
Address:		Fog.#10130
Notes:
*/
DWORD __stdcall BITBUFFER_Read(D2BitBufferStrc* pBuffer, int nBits)
{
	int n = pBuffer->nPosBits + nBits + 8 * pBuffer->nPos - pBuffer->nBits;

	if ((signed int)n < 0)
	{
		n = 0;
	}

	DWORD dwBitsLeft = nBits - n;
	if (n)
	{
		pBuffer->bFull = TRUE;
	}

	DWORD dwValue = 0;
	DWORD dw = 0;

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
			dwValue += (gdw_6FF71408[i] & ((BYTE)(*pBuffer->pBuffer & gn_6FF7142C[pBuffer->nPosBits]) >> pBuffer->nPosBits)) << dw;
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
Function:		BITBUFFER_GoToNextByte
Address:		Fog.#10131
Notes:
*/
void __stdcall BITBUFFER_GoToNextByte(D2BitBufferStrc* pBuffer)
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
Function:		BITBUFFER_SetBitState
Address:		Fog.#10118
Notes:
*/
void __stdcall BITBUFFER_SetBitState(BYTE* pBitBuffer, int nBit)
{
	pBitBuffer[nBit >> 3] |= (BYTE)gdwBitMasks[nBit & 7];
}

/*
Function:		BITBUFFER_GoToNextByte
Address:		Fog.#10119
Notes:
*/
int __stdcall BITBUFFER_GetBitState(BYTE* pBitBuffer, int nBit)
{
	return gdwBitMasks[nBit & 7] & pBitBuffer[nBit >> 3];
}

/*
Function:		BITBUFFER_GoToNextByte
Address:		Fog.#10120
Notes:
*/
void __stdcall BITBUFFER_ResetBitstate(BYTE* pBitBuffer, int nBit)
{
	pBitBuffer[nBit >> 3] &= LOBYTE(gdwInvBitMasks[nBit & 7]);
}
