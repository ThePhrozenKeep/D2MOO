#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Drlg/D2DrlgOutRoom.h>


BEGIN_VISIT(D2DrlgOutdoorRoomStrc)
	OBJECT(pTileTypeGrid)
	OBJECT(pWallGrid)
	OBJECT(pFloorGrid)
	OBJECT(pDirtPathGrid)
	POINTER(pVertex)
	FIELD(dwFlags)
	FIELD(dwFlagsEx)
	FIELD(unk0x5C)
	FIELD(unk0x60)
	FIELD(nSubType)
	FIELD(nSubTheme)
	FIELD(nSubThemePicked)
END_VISIT()
