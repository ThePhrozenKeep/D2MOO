#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <DataTbls/FieldTbls.h>


BEGIN_VISIT(D2CollisionFieldStrc)
	FIELD(nWidth)
	FIELD(nHeight)
	FIELD(nArea)
	FIELD(nCenterX)
	FIELD(nCenterY)
END_VISIT()

BEGIN_VISIT(D2FieldStrc)
	FIELD(nX)
	FIELD(nY)
END_VISIT()
