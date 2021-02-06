#include <Drlg/D2DrlgDrlg.h>
#include <Drlg/D2DrlgDrlgGrid.h>
#include <Drlg/D2DrlgDrlgRoom.h>


void(__fastcall* gpfFlagOperations[])(int*, int) =
{
	DRGLGRID_OrFlag,
	DRGLGRID_AndFlag,
	DRGLGRID_XorFlag,
	DRGLGRID_OverwriteFlag,
	DRGLGRID_OverwriteFlagIfZero,
	DRGLGRID_AndNegatedFlag
};


//D2Common.0x6FD75BA0
void __fastcall DRGLGRID_OverwriteFlag(int* pFlag, int nFlag)
{
	*pFlag = nFlag;
}

//D2Common.0x6FD75BB0
void __fastcall DRGLGRID_OrFlag(int* pFlag, int nFlag)
{
	*pFlag |= nFlag;
}

//D2Common.0x6FD75BC0
void __fastcall DRGLGRID_AndFlag(int* pFlag, int nFlag)
{
	*pFlag &= nFlag;
}

//D2Common.0x6FD75BD0
void __fastcall DRGLGRID_XorFlag(int* pFlag, int nFlag)
{
	*pFlag ^= nFlag;
}

//D2Common.6FD75BE0
void __fastcall DRGLGRID_OverwriteFlagIfZero(int* pFlag, int nFlag)
{
	if (*pFlag == 0)
	{
		*pFlag = nFlag;
	}
}

//D2Common.6FD75BF0
void __fastcall DRGLGRID_AndNegatedFlag(int* pFlag, int nFlag)
{
	*pFlag &= ~nFlag;
}

//D2Common.0x6FD75C00
BOOL __fastcall DRLGGRID_IsGridValid(D2DrlgGridStrc* pDrlgGrid)
{
	return pDrlgGrid && pDrlgGrid->pCellsFlags;
}

//D2Common.0x6FD75C20
BOOL __fastcall DRLGGRID_IsPointInsideGridArea(D2DrlgGridStrc* pDrlgGrid, int nX, int nY)
{
	return nX >= 0 && nX < pDrlgGrid->nWidth && nY >= 0 && nY < pDrlgGrid->nHeight;
}

//D2Common.0x6FD75C50
void __fastcall DRLGGRID_AlterGridFlag(D2DrlgGridStrc* pDrlgGrid, int nX, int nY, int nFlag, FlagOperation eOperation)
{
	gpfFlagOperations[eOperation](&pDrlgGrid->pCellsFlags[nX + pDrlgGrid->pCellsRowOffsets[nY]], nFlag);
}

//D2Common.0x6FD75C80
int* __fastcall DRLGGRID_GetGridFlagsPointer(D2DrlgGridStrc* pDrlgGrid, int nX, int nY)
{
	return &pDrlgGrid->pCellsFlags[nX + pDrlgGrid->pCellsRowOffsets[nY]];
}

//D2Common.0x6FD75CA0
int __fastcall DRLGGRID_GetGridFlags(D2DrlgGridStrc* pDrlgGrid, int nX, int nY)
{
	return pDrlgGrid->pCellsFlags[nX + pDrlgGrid->pCellsRowOffsets[nY]];
}

//D2Common.0x6FD75CC0
void __fastcall DRLGGRID_AlterAllGridFlags(D2DrlgGridStrc* pDrlgGrid, int nFlag, FlagOperation eOperation)
{
	for (int nY = 0; nY < pDrlgGrid->nHeight; ++nY)
	{
		for (int nX = 0; nX < pDrlgGrid->nWidth; ++nX)
		{
			DRLGGRID_AlterGridFlag(pDrlgGrid, nX, nY, nFlag, eOperation);
		}
	}
}

//D2Common.0x6FD75D20
void __fastcall DRLGGRID_AlterEdgeGridFlags(D2DrlgGridStrc* pDrlgGrid, int nFlag, FlagOperation eOperation)
{
	int* pFlagsFirstRow = &pDrlgGrid->pCellsFlags[pDrlgGrid->pCellsRowOffsets[0]];
	int* pFlagsLastRow = &pDrlgGrid->pCellsFlags[pDrlgGrid->pCellsRowOffsets[pDrlgGrid->nHeight - 1]];

	for (int i = 0; i < pDrlgGrid->nWidth; ++i)
	{
		gpfFlagOperations[eOperation](&pFlagsFirstRow[i], nFlag);
		gpfFlagOperations[eOperation](&pFlagsLastRow[i], nFlag);
	}

	for (int i = 1; i < pDrlgGrid->nHeight; ++i)
	{
		const int nCurRowOffset = pDrlgGrid->pCellsRowOffsets[i];
		gpfFlagOperations[eOperation](&pDrlgGrid->pCellsFlags[nCurRowOffset + 0                    ], nFlag);
		gpfFlagOperations[eOperation](&pDrlgGrid->pCellsFlags[nCurRowOffset + pDrlgGrid->nWidth - 1], nFlag);
	}
}

//D2Common.6FD75DE0
//TODO: Name
void __fastcall sub_6FD75DE0(D2DrlgGridStrc* pDrlgGrid, D2DrlgVertexStrc* pDrlgVertex, int nFlag, FlagOperation eOperation, BOOL bAlterNextVertex)
{
	D2DrlgVertexStrc* pNext = NULL;
	int nEndX = 0;
	int nEndY = 0;
	int nX = 0;
	int nY = 0;

	pNext = pDrlgVertex->pNext;

	if (pDrlgVertex->nPosX == pNext->nPosX && pDrlgVertex->nPosY == pNext->nPosY)
	{
		DRLGGRID_AlterGridFlag(pDrlgGrid, pDrlgVertex->nPosX, pDrlgVertex->nPosY, nFlag, eOperation);
		return;
	}

	if (pDrlgVertex->nPosX == pNext->nPosX)
	{
		nX = pDrlgVertex->nPosX;
		if (pDrlgVertex->nPosY >= pNext->nPosY)
		{
			nY = pNext->nPosY + 1;
			nEndY = pDrlgVertex->nPosY;
		}
		else
		{
			nY = pDrlgVertex->nPosY + 1;
			nEndY = pNext->nPosY;
		}

		while (nY != nEndY)
		{
			DRLGGRID_AlterGridFlag(pDrlgGrid, nX, nY, nFlag, eOperation);

			++nY;
		}
	}
	else
	{
		nY = pDrlgVertex->nPosY;
		if (pDrlgVertex->nPosX >= pNext->nPosX)
		{
			nEndX = pDrlgVertex->nPosX;
			nX = pNext->nPosX + 1;
		}
		else
		{
			nEndX = pNext->nPosX;
			nX = pDrlgVertex->nPosX + 1;
		}

		while (nX != nEndX)
		{
			DRLGGRID_AlterGridFlag(pDrlgGrid, nX, nY, nFlag, eOperation);

			++nX;
		}
	}

	DRLGGRID_AlterGridFlag(pDrlgGrid, pDrlgVertex->nPosX, pDrlgVertex->nPosY, nFlag, eOperation);

	if (bAlterNextVertex)
	{
		DRLGGRID_AlterGridFlag(pDrlgGrid, pDrlgVertex->pNext->nPosX, pDrlgVertex->pNext->nPosY, nFlag, eOperation);
	}
}

//D2Common.0x6FD75F10
void __fastcall DRLGGRID_SetVertexGridFlags(D2DrlgGridStrc* pDrlgGrid, D2DrlgVertexStrc* pDrlgVertex, int nFlag)
{
	D2DrlgVertexStrc* pVertex = NULL;
	int nX = 0;
	int nY = 0;

	pVertex = pDrlgVertex;

	while (pVertex)
	{
		nX = pVertex->nPosX;
		nY = pVertex->nPosY;
		pVertex = pVertex->pNext;

		if (nX >= 0 && nX < pDrlgGrid->nWidth && nY >= 0 && nY < pDrlgGrid->nHeight)
		{
			DRLGGRID_AlterGridFlag(pDrlgGrid, nX, nY, nFlag, FLAG_OPERATION_OR);
		}
	}
}

//D2Common.0x6FD75F60
//TODO: Name
void __fastcall sub_6FD75F60(D2DrlgGridStrc* pDrlgGrid, D2DrlgVertexStrc* pDrlgVertex, D2DrlgCoordStrc* pDrlgCoord, int nFlag, FlagOperation eOperation, int nSize)
{
	int nIndexX = 0;
	int nIndexY = 0;
	int nCheck = 0;
	int nXDiff = 0;
	int nYDiff = 0;
	int nXInc = 0;
	int nYInc = 0;
	int nX = 0;
	int nY = 0;

	nX = pDrlgVertex->nPosX;
	nY = pDrlgVertex->nPosY;

	nXDiff = pDrlgVertex->pNext->nPosX - nX;
	nYDiff = pDrlgVertex->pNext->nPosY - nY;

	if (nXDiff >= 0)
	{
		nXInc = 1;
	}
	else
	{
		nXDiff = -nXDiff;
		nXInc = -1;
	}

	if (nYDiff >= 0)
	{
		nYInc = 1;
	}
	else
	{
		nYDiff = -nYDiff;
		nYInc = -1;
	}

	nIndexX = nX - pDrlgCoord->nPosX;
	nIndexY = nY - pDrlgCoord->nPosY;

	if (nXDiff >= nYDiff)
	{
		for (int i = 0; i < nSize; ++i)
		{
			if (DRLGROOM_AreXYInsideCoordinates(pDrlgCoord, nX, nY + i))
			{
				DRLGGRID_AlterGridFlag(pDrlgGrid, nIndexX, nIndexY + i, nFlag, eOperation);
			}
		}

		for (int j = 0; j < nXDiff; ++j)
		{
			nX += nXInc;
			nCheck += nYDiff;

			if (nCheck > nXDiff)
			{
				nY += nYInc;
				nCheck -= nXDiff;
			}

			nIndexX = nX - pDrlgCoord->nPosX;
			nIndexY = nY - pDrlgCoord->nPosY;

			for (int i = 0; i < nSize; ++i)
			{
				if (DRLGROOM_AreXYInsideCoordinates(pDrlgCoord, nX, nY + i))
				{
					DRLGGRID_AlterGridFlag(pDrlgGrid, nIndexX, nIndexY + i, nFlag, eOperation);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < nSize; ++i)
		{
			if (DRLGROOM_AreXYInsideCoordinates(pDrlgCoord, nX + i, nY))
			{
				DRLGGRID_AlterGridFlag(pDrlgGrid, nIndexX + i, nIndexY, nFlag, eOperation);
			}
		}

		for (int j = 0; j < nYDiff; ++j)
		{
			nY += nYInc;
			nCheck += nXDiff;

			if (nCheck > nYDiff)
			{
				nX += nXInc;
				nCheck -= nYDiff;
			}

			nIndexX = nX - pDrlgCoord->nPosX;
			nIndexY = nY - pDrlgCoord->nPosY;

			for (int i = 0; i < nSize; ++i)
			{
				if (DRLGROOM_AreXYInsideCoordinates(pDrlgCoord, nX + i, nY))
				{
					DRLGGRID_AlterGridFlag(pDrlgGrid, nIndexX + i, nIndexY, nFlag, eOperation);
				}
			}
		}
	}
}

//D2Common.0x6FD76230
void __fastcall DRLGGRID_InitializeGridCells(void* pMemPool, D2DrlgGridStrc* pDrlgGrid, int nWidth, int nHeight)
{
	int nRowOffset = 0;

	pDrlgGrid->nWidth = nWidth;
	pDrlgGrid->nHeight = nHeight;

	// nHeight for the row offsets, nHeight + nWidth for the cells
	pDrlgGrid->pCellsRowOffsets = (int*)D2_CALLOC_SERVER(pMemPool, sizeof(int) * nHeight * (nWidth + 1));
	
	pDrlgGrid->pCellsFlags = &pDrlgGrid->pCellsRowOffsets[nHeight];
	for (int i = 0; i < nHeight; ++i)
	{
		pDrlgGrid->pCellsRowOffsets[i] = nRowOffset;
		nRowOffset += nWidth;
	}

	pDrlgGrid->unk0x10 = 0;
}

//D2Common.0x6FD762B0
void __fastcall DRLGGRID_FillGrid(D2DrlgGridStrc* pDrlgGrid, int nWidth, int nHeight, int* pCellPos, int* pCellRowOffsets)
{
	int nRowOffset = 0;

	pDrlgGrid->nWidth = nWidth;
	pDrlgGrid->nHeight = nHeight;

	pDrlgGrid->pCellsFlags = pCellPos;
	memset(pCellPos, 0x00, sizeof(int) * nHeight * nWidth);

	pDrlgGrid->pCellsRowOffsets = pCellRowOffsets;

	for (int i = 0; i < nHeight; ++i)
	{
		pDrlgGrid->pCellsRowOffsets[i] = nRowOffset;
		nRowOffset += nWidth;
	}

	pDrlgGrid->unk0x10 = 0;
}

//D2Common.0x6FD76310
void __fastcall DRLGGRID_FillNewCellFlags(void* pMemPool, D2DrlgGridStrc* pDrlgGrid, int* pCellPos, D2DrlgCoordStrc* pDrlgCoord, int nWidth)
{
	int nFlag = 0;

	pDrlgGrid->nWidth = pDrlgCoord->nWidth;
	pDrlgGrid->nHeight = pDrlgCoord->nHeight;

	pDrlgGrid->pCellsFlags = &pCellPos[pDrlgCoord->nPosX + nWidth * pDrlgCoord->nPosY];

	pDrlgGrid->pCellsRowOffsets = (int32_t*)D2_ALLOC_SERVER(pMemPool, sizeof(int) * pDrlgCoord->nHeight);

	for (int i = 0; i < pDrlgCoord->nHeight; ++i)
	{
		pDrlgGrid->pCellsRowOffsets[i] = nFlag;
		nFlag += nWidth;
	}

	pDrlgGrid->unk0x10 = 1;
}

//D2Common.0x6FD76380
void __fastcall DRLGGRID_AssignCellsOffsetsAndFlags(D2DrlgGridStrc* pDrlgGrid, int* pCellPos, D2DrlgCoordStrc* pDrlgCoord, int nWidth, int* pCellFlags)
{
	pDrlgGrid->nWidth = pDrlgCoord->nWidth;
	pDrlgGrid->nHeight = pDrlgCoord->nHeight;

	pDrlgGrid->pCellsFlags = &pCellPos[pDrlgCoord->nPosX + nWidth * pDrlgCoord->nPosY];
	pDrlgGrid->pCellsRowOffsets = pCellFlags;

	for (int i = 0; i < pDrlgCoord->nHeight; ++i)
	{
		pDrlgGrid->pCellsRowOffsets[i] = i * nWidth;
	}

	pDrlgGrid->unk0x10 = 1;
}

//D2Common.0x6FD763E0
void __fastcall DRLGGRID_FreeGrid(void* pMemPool, D2DrlgGridStrc* pDrlgGrid)
{
	if (pDrlgGrid->pCellsRowOffsets)
	{
		D2_FREE_SERVER(pMemPool, pDrlgGrid->pCellsRowOffsets);
	}

	pDrlgGrid->pCellsFlags = NULL;
	pDrlgGrid->pCellsRowOffsets = NULL;
}

//D2Common.0x6FD76410
void __fastcall DRLGGRID_ResetGrid(D2DrlgGridStrc* pDrlgGrid)
{
	pDrlgGrid->pCellsFlags = 0;
	pDrlgGrid->pCellsRowOffsets = 0;
}
