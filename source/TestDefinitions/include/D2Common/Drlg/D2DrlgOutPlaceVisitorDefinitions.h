#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Drlg/D2DrlgOutPlace.h>


BEGIN_VISIT(D2JungleStrc)
	OBJECT(pDrlgCoord)
	FIELD(field_10)
	FIELD(nBranch)
	POINTER(pBasedOnJungle)
	ARRAY(pJungleBranches)
	FIELD(nPresetsBlocksX)
	FIELD(nPresetsBlocksY)
	//POINTER(pJungleDefs)
	FIELD(nJungleDefs)
END_VISIT()
