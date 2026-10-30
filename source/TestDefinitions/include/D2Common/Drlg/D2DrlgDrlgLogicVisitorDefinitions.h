#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Drlg/D2DrlgDrlgLogic.h>


BEGIN_VISIT(D2UnkDrlgLogicStrc)
	POINTER(pDrlgRoom)
	POINTER(field_4)
	POINTER(pTileTypeGrid)
	POINTER(pWallGrid)
	POINTER(pFloorGrid)
	POINTER(field_14)
	FIELD(field_18)
	FIELD(nFlags)
END_VISIT()

BEGIN_VISIT(D2DrlgLogicalRoomInfoStrc)
	FIELD(dwFlags)
	FIELD(nLists)
	OBJECT(pIndexX)
	OBJECT(pIndexY)
	POINTER(pCoordList)
END_VISIT()
