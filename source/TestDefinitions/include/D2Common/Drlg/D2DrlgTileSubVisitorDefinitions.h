#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Drlg/D2DrlgTileSub.h>


BEGIN_VISIT(D2UnkOutdoorStrc)
	POINTER(pLevel)
	//POINTER(field_4)
	POINTER(pGrid1)
	POINTER(pGrid2)
	FIELD(nLevelPrestId)
	FIELD(field_14)
	FIELD(nLvlSubId)
END_VISIT()

BEGIN_VISIT(D2UnkOutdoorStrc2)
	POINTER(pDrlgRoom)
	ARRAY(pOutdoorRooms)
	ARRAY(pWallsGrids)
	POINTER(pFloorGrid)
	FIELD(field_28)
	FIELD(field_2C)
	FIELD(nSubWaypoint_Shrine)
	FIELD(nSubTheme)
	FIELD(nSubThemePicked)
END_VISIT()

BEGIN_VISIT(D2UnkOutdoorStrc3)
	FIELD(nLevelId)
	FIELD(nExcludedLevel1)
	FIELD(nExcludedLevel2)
	FIELD(nRand)
	FIELD(nNextRand)
	FIELD(nFlags)
END_VISIT()

BEGIN_VISIT(D2DrlgSubstGroupStrc)
	OBJECT(tBox)
	FIELD(field_10)
	FIELD(field_14)
END_VISIT()
