#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Drlg/D2DrlgDrlgGrid.h>


BEGIN_VISIT(D2DrlgGridStrc)
	//POINTER(pCellsFlags)
	//POINTER(pCellsRowOffsets)
	FIELD(nWidth)
	FIELD(nHeight)
	FIELD(unk0x10)
END_VISIT()
