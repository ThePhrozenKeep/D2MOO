#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Collision.h>


BEGIN_VISIT(D2BoundingBoxStrc)
	FIELD(nLeft)
	FIELD(nBottom)
	FIELD(nRight)
	FIELD(nTop)
END_VISIT()

BEGIN_VISIT(D2RoomCollisionGridStrc)
	OBJECT(pRoomCoords)
	//POINTER(pCollisionMask)
END_VISIT()
