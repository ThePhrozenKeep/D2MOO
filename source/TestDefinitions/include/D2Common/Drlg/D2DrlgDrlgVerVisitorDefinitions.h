#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Drlg/D2DrlgDrlgVer.h>


BEGIN_VISIT(D2DrlgCoordStrc)
	FIELD(nPosX)
	FIELD(nPosY)
	FIELD(nWidth)
	FIELD(nHeight)
END_VISIT()

BEGIN_VISIT(D2DrlgVertexStrc)
	FIELD(nPosX)
	FIELD(nPosY)
	FIELD(nDirection)
	ARRAY(pad0x09)
	FIELD(dwFlags)
	POINTER(pNext)
END_VISIT()
