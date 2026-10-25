#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Waypoints.h>


BEGIN_VISIT(D2WaypointActTableStrc)
	//FIELD(bTableInitialized)
	//FIELD(nStartLevelId)
	//FIELD(nEndLevelId)
	//FIELD(nWpNo)
	//FIELD(nLastWpNo)
END_VISIT()

BEGIN_VISIT(D2WaypointCoordTableStrc)
	//FIELD(nX)
	//FIELD(nY)
	//FIELD(nTextX)
	//FIELD(nTextY)
	//FIELD(nClickX)
	//FIELD(nClickY)
END_VISIT()

BEGIN_VISIT(D2WaypointDataStrc)
	ARRAY(nFlags)
END_VISIT()

BEGIN_VISIT(D2WaypointFlagTableStrc)
	//FIELD(nArrayId)
	//FIELD(nFlag)
END_VISIT()

BEGIN_VISIT(D2WaypointTableStrc)
	//FIELD(nLevelId)
	//FIELD(bActivated)
END_VISIT()
