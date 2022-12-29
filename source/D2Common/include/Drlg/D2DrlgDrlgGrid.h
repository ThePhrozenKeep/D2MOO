#pragma once

#include "CommonDefinitions.h"

#pragma pack(1)

enum FlagOperation
{
	FLAG_OPERATION_OR,
	FLAG_OPERATION_AND,
	FLAG_OPERATION_XOR,
	FLAG_OPERATION_OVERWRITE,
	FLAG_OPERATION_OVERWRITE_IF_ZERO,
	FLAG_OPERATION_AND_NEGATED,
};


struct D2DrlgGridStrc
{
	int32_t* pCellsFlags;					//0x00
	int32_t* pCellsRowOffsets;				//0x04
	int32_t nWidth;							//0x08
	int32_t nHeight;						//0x0C
	int32_t unk0x10;						//0x10 Maybe indicates if uninitialized? No memset when set to 1
};

#pragma pack()

//D2Common.0x6FD75BA0
void __fastcall DRGLGRID_OverwriteFlag(int* pFlag, int nFlag);
//D2Common.0x6FD75BB0
void __fastcall DRGLGRID_OrFlag(int* pFlag, int nFlag);
//D2Common.0x6FD75BC0
void __fastcall DRGLGRID_AndFlag(int* pFlag, int nFlag);
//D2Common.0x6FD75BD0
void __fastcall DRGLGRID_XorFlag(int* pFlag, int nFlag);
//D2Common.6FD75BE0
void __fastcall DRGLGRID_OverwriteFlagIfZero(int* pFlag, int nFlag);
//D2Common.6FD75BF0
void __fastcall DRGLGRID_AndNegatedFlag(int* pFlag, int nFlag);
//D2Common.0x6FD75C00
BOOL __fastcall DRLGGRID_IsGridValid(D2DrlgGridStrc* pDrlgGrid);
//D2Common.0x6FD75C20
BOOL __fastcall DRLGGRID_IsPointInsideGridArea(D2DrlgGridStrc* pDrlgGrid, int nX, int nY);
//D2Common.0x6FD75C50
void __fastcall DRLGGRID_AlterGridFlag(D2DrlgGridStrc* pDrlgGrid, int nX, int nY, int nFlag, FlagOperation eOperation);
//D2Common.0x6FD75C80
int* __fastcall DRLGGRID_GetGridFlagsPointer(D2DrlgGridStrc* pDrlgGrid, int nX, int nY);
//D2Common.0x6FD75CA0
int __fastcall DRLGGRID_GetGridEntry(D2DrlgGridStrc* pDrlgGrid, int nX, int nY);
//D2Common.0x6FD75CC0
void __fastcall DRLGGRID_AlterAllGridFlags(D2DrlgGridStrc* pDrlgGrid, int nFlag, FlagOperation eOperation);
//D2Common.0x6FD75D20
void __fastcall DRLGGRID_AlterEdgeGridFlags(D2DrlgGridStrc* pDrlgGrid, int nFlag, FlagOperation eOperation);
//D2Common.6FD75DE0
void __fastcall sub_6FD75DE0(D2DrlgGridStrc* pDrlgGrid, D2DrlgVertexStrc* pDrlgVertex, int nFlag, FlagOperation eOperation, BOOL bAlterNextVertex);
//D2Common.0x6FD75F10
void __fastcall DRLGGRID_SetVertexGridFlags(D2DrlgGridStrc* pDrlgGrid, D2DrlgVertexStrc* pDrlgVertex, int nFlag);
//D2Common.0x6FD75F60
void __fastcall sub_6FD75F60(D2DrlgGridStrc* pDrlgGrid, D2DrlgVertexStrc* pDrlgVertex, D2DrlgCoordStrc* pDrlgCoord, int nFlag, FlagOperation eOperation, int nSize);
//D2Common.0x6FD76230
void __fastcall DRLGGRID_InitializeGridCells(void* pMemPool, D2DrlgGridStrc* pDrlgGrid, int nWidth, int nHeight);
//D2Common.0x6FD762B0
void __fastcall DRLGGRID_FillGrid(D2DrlgGridStrc* pDrlgGrid, int nWidth, int nHeight, int* pCellPos, int* pCellRowOffsets);
//D2Common.0x6FD76310
void __fastcall DRLGGRID_FillNewCellFlags(void* pMemPool, D2DrlgGridStrc* pDrlgGrid, int* pCellPos, D2DrlgCoordStrc* pDrlgCoord, int nWidth);
//D2Common.0x6FD76380
void __fastcall DRLGGRID_AssignCellsOffsetsAndFlags(D2DrlgGridStrc* pDrlgGrid, int* pCellPos, D2DrlgCoordStrc* pDrlgCoord, int nWidth, int* pCellFlags);
//D2Common.0x6FD763E0
void __fastcall DRLGGRID_FreeGrid(void* pMemPool, D2DrlgGridStrc* pDrlgGrid);
//D2Common.0x6FD76410
void __fastcall DRLGGRID_ResetGrid(D2DrlgGridStrc* pDrlgGrid);
