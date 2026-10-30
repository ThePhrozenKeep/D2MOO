#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Drlg/D2DrlgOutdoors.h>


BEGIN_VISIT(D2DrlgOutdoorGridStrc)
	FIELD(dwFlags)
	POINTER(pSectors)
	FIELD(nWidth)
	FIELD(nHeight)
	FIELD(bInit)
END_VISIT()

BEGIN_VISIT(D2DrlgOutdoorInfoStrc)
	FIELD(dwFlags)
	ARRAY(pGrid)
	//BEGIN_UNION()
		FIELD(nWidth)
		FIELD(nHeight)
		FIELD(nGridWidth)
		FIELD(nGridHeight)
		//OBJECT(pCoord)
	//END_UNION()
	POINTER(pVertex)
	ARRAY(pPathStarts)
	ARRAY(pVertices)
	FIELD(nVertices)
	POINTER(pRoomData)
END_VISIT()
