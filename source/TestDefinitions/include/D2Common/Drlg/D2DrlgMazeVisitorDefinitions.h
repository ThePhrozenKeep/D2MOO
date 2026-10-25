#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Drlg/D2DrlgMaze.h>


BEGIN_VISIT(D2MazeLevelIdStrc)
	FIELD(nLevelPrestId1)
	FIELD(nLevelPrestId2)
	FIELD(nPickedFile)
	FIELD(nDirection)
END_VISIT()
