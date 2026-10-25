#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <DataTbls/InvTbls.h>


BEGIN_VISIT(D2InvCompGridStrc)
	OBJECT(rect)
	FIELD(nWidth)
	FIELD(nHeight)
	FIELD(pad0x12)
END_VISIT()

BEGIN_VISIT(D2InventoryGridInfoStrc)
	FIELD(nGridX)
	FIELD(nGridY)
	FIELD(pad0x02)
	FIELD(nGridLeft)
	FIELD(nGridRight)
	FIELD(nGridTop)
	FIELD(nGridBottom)
	FIELD(nGridBoxWidth)
	FIELD(nGridBoxHeight)
	FIELD(pad0x16)
END_VISIT()

BEGIN_VISIT(D2InventoryTxt)
	OBJECT(pRect)
	OBJECT(pGridInfo)
	ARRAY(pComponents)
END_VISIT()
