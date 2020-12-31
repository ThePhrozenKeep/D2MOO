#include "Drlg/D2DrlgDrlgVer.h"


//D2Common.0x6FD782A0
D2DrlgVertexStrc* __fastcall DRLGVER_AllocVertex(void* pMemPool, uint8_t nDirection)
{
	D2DrlgVertexStrc* pDrlgVertex = (D2DrlgVertexStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2DrlgVertexStrc), __FILE__, __LINE__, 0);
	memset(pDrlgVertex, 0x00, sizeof(D2DrlgVertexStrc));

	pDrlgVertex->nDirection = nDirection;

	return pDrlgVertex;
}

//D2Common.0x6FD782D0
//TODO: v15, v16, v21, v23
void __fastcall DRLGVER_CreateVertices(void* pMemPool, D2DrlgVertexStrc** ppVertices, D2DrlgCoordStrc* pDrlgCoord, uint8_t nDirection, D2DrlgOrthStrc* pDrlgRoomData)
{
	D2DrlgOrthStrc* pCurrentRoomData = NULL;
	D2DrlgVertexStrc* pPreviousVertex = NULL;
	D2DrlgVertexStrc* pCurrentVertex = NULL;
	D2DrlgVertexStrc* pNextVertex = NULL;
	D2DrlgVertexStrc* pLastVertex = NULL;
	D2DrlgVertexStrc* pNewVertex = NULL;
	D2DrlgVertexStrc* pVertex = NULL;
	D2DrlgCoordStrc* pCurrentCoords = NULL;
	int bDirection = 0;
	int nSign = 0;
	int v15 = 0;
	int v16 = 0;
	int v21 = 0;
	int v23 = 0;

	--pDrlgCoord->nWidth;
	--pDrlgCoord->nHeight;

	pCurrentRoomData = pDrlgRoomData;
	while (pCurrentRoomData)
	{
		--pCurrentRoomData->pBox->nWidth;
		--pCurrentRoomData->pBox->nHeight;

		pCurrentRoomData = pCurrentRoomData->pNext;
	}

	pCurrentVertex = DRLGVER_AllocVertex(pMemPool, nDirection);
	*ppVertices = pCurrentVertex;
	pCurrentVertex->nPosX = pDrlgCoord->nPosX;
	pCurrentVertex->nPosY = pDrlgCoord->nHeight + pDrlgCoord->nPosY;

	pNextVertex = DRLGVER_AllocVertex(pMemPool, nDirection);
	pCurrentVertex->pNext = pNextVertex;
	pNextVertex->nPosX = pDrlgCoord->nPosX;
	pNextVertex->nPosY = pDrlgCoord->nPosY;

	pLastVertex = DRLGVER_AllocVertex(pMemPool, nDirection);
	pNextVertex->pNext = pLastVertex;
	pLastVertex->nPosX = pDrlgCoord->nWidth + pDrlgCoord->nPosX;
	pLastVertex->nPosY = pDrlgCoord->nPosY;

	pPreviousVertex = DRLGVER_AllocVertex(pMemPool, nDirection);
	pLastVertex->pNext = pPreviousVertex;
	pPreviousVertex->nPosX = pDrlgCoord->nWidth + pDrlgCoord->nPosX;
	pPreviousVertex->nPosY = pDrlgCoord->nHeight + pDrlgCoord->nPosY;
	pPreviousVertex->pNext = *ppVertices;

	pCurrentVertex = *ppVertices;
	pNextVertex = (*ppVertices)->pNext;
	pLastVertex = pNextVertex->pNext;
	pPreviousVertex = pLastVertex->pNext;

	pCurrentRoomData = pDrlgRoomData;
	while (pCurrentRoomData)
	{
		if (pCurrentRoomData->pBox)
		{
			pCurrentCoords = pCurrentRoomData->pBox;
		}
		else
		{
			pCurrentCoords = pDrlgCoord;
		}

		switch (pCurrentRoomData->nDirection)
		{
		case 0:
			pVertex = pCurrentVertex;
			bDirection = 1;
			v16 = pCurrentCoords->nPosY + pCurrentCoords->nHeight;
			v15 = pCurrentCoords->nPosY;
			v21 = pCurrentVertex->nPosY;
			v23 = pNextVertex->nPosY;
			nSign = -1;
			break;

		case 1:
			pVertex = pNextVertex;
			bDirection = 0;
			v16 = pCurrentCoords->nPosX;
			v15 = pCurrentCoords->nPosX + pCurrentCoords->nWidth;
			v21 = pNextVertex->nPosX;
			v23 = pLastVertex->nPosX;
			nSign = 1;
			break;

		case 2:
			pVertex = pLastVertex;
			bDirection = 1;
			v16 = pCurrentCoords->nPosY;
			v15 = pCurrentCoords->nPosY + pCurrentCoords->nHeight;
			v21 = pLastVertex->nPosY;
			v23 = pPreviousVertex->nPosY;
			nSign = 1;
			break;

		case 3:
			pVertex = pPreviousVertex;
			bDirection = 0;
			v16 = pCurrentCoords->nPosX + pCurrentCoords->nWidth;
			v15 = pCurrentCoords->nPosX;
			v21 = pVertex->nPosX;
			v23 = pCurrentVertex->nPosX;
			nSign = -1;
			break;

		default:
			FOG_10025("FALSE", __FILE__, __LINE__);
			exit(-1);

		}

		if (nSign * v16 > nSign * v21)
		{
			if (nSign * v16 <= nSign * v23)
			{
				pNewVertex = DRLGVER_AllocVertex(pMemPool, nDirection);
				if (bDirection)
				{
					pNewVertex->nPosY = v16;
					pNewVertex->nPosX = pVertex->nPosX;
				}
				else
				{
					pNewVertex->nPosX = v16;
					pNewVertex->nPosY = pVertex->nPosY;
				}

				pNewVertex->pNext = pVertex->pNext;
				pVertex->pNext = pNewVertex;
				pVertex = pNewVertex;

				pVertex->dwFlags |= 1;
				if (pCurrentRoomData->bPreset)
				{
					pVertex->dwFlags |= 2;
				}

				if (nSign * v15 < nSign * v23)
				{
					pNewVertex = DRLGVER_AllocVertex(pMemPool, nDirection);
					if (bDirection)
					{
						pNewVertex->nPosY = v15;
						pNewVertex->nPosX = pVertex->nPosX;
					}
					else
					{
						pNewVertex->nPosX = v15;
						pNewVertex->nPosY = pVertex->nPosY;
					}

					pNewVertex->pNext = pVertex->pNext;
					pVertex->pNext = pNewVertex;
				}
			}
		}
		else if (nSign * v15 >= nSign * v21)
		{
			pVertex->dwFlags |= 1;
			if (pCurrentRoomData->bPreset)
			{
				pVertex->dwFlags |= 2;
			}

			if (nSign * v15 < nSign * v23)
			{
				pNewVertex = DRLGVER_AllocVertex(pMemPool, nDirection);
				if (bDirection)
				{
					pNewVertex->nPosY = v15;
					pNewVertex->nPosX = pVertex->nPosX;
				}
				else
				{
					pNewVertex->nPosX = v15;
					pNewVertex->nPosY = pVertex->nPosY;
				}

				pNewVertex->pNext = pVertex->pNext;
				pVertex->pNext = pNewVertex;
			}
		}

		pCurrentRoomData = pCurrentRoomData->pNext;
	}

	pCurrentVertex = *ppVertices;
	do
	{
		pCurrentVertex->nPosX -= pDrlgCoord->nPosX;
		pCurrentVertex->nPosY -= pDrlgCoord->nPosY;

		pCurrentVertex = pCurrentVertex->pNext;
	}
	while (pCurrentVertex != *ppVertices);

	++pDrlgCoord->nWidth;
	++pDrlgCoord->nHeight;

	pCurrentRoomData = pDrlgRoomData;
	while (pCurrentRoomData)
	{
		++pCurrentRoomData->pBox->nWidth;
		++pCurrentRoomData->pBox->nHeight;

		pCurrentRoomData = pCurrentRoomData->pNext;
	}
}

//D2Common.0x6FD786C0
void __fastcall DRLGVER_FreeVertices(void* pMemPool, D2DrlgVertexStrc** ppVertices)
{
	D2DrlgVertexStrc* pVertex = NULL;
	D2DrlgVertexStrc* pNext = NULL;

	if (*ppVertices)
	{
		pVertex = (*ppVertices)->pNext;

		if (pVertex)
		{
			do
			{
				pNext = pVertex->pNext;
				FOG_FreeServerMemory(pMemPool, pVertex, __FILE__, __LINE__, 0);
				pVertex = pNext;
			}
			while (pVertex != *ppVertices && pVertex);
		}

		FOG_FreeServerMemory(pMemPool, *ppVertices, __FILE__, __LINE__, 0);
		*ppVertices = NULL;
	}
}

//D2Common.0x6FD78730
void __fastcall DRLGVER_GetCoordDiff(D2DrlgVertexStrc* pDrlgVertex, int* pDiffX, int* pDiffY)
{
	*pDiffX = pDrlgVertex->pNext->nPosX - pDrlgVertex->nPosX;
	*pDiffY = pDrlgVertex->pNext->nPosY - pDrlgVertex->nPosY;

	if (*pDiffX >= 0)
	{
		if (*pDiffX > 0)
		{
			*pDiffX = 1;
		}
	}
	else
	{
		*pDiffX = -1;
	}

	if (*pDiffY >= 0)
	{
		if (*pDiffY > 0)
		{
			*pDiffY = 1;
		}
	}
	else
	{
		*pDiffY = -1;
	}
}
