#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Items.h>


BEGIN_VISIT(D2ItemSaveStrc)
	FIELD(nClassId)
	FIELD(nX)
	FIELD(nY)
	FIELD(nAnimMode)
	ARRAY(pad0x09)
	FIELD(dwFlags)
	FIELD(nStorePage)
	FIELD(nBodyloc)
	ARRAY(pad0x12)
	FIELD(nItemFileIndex)
END_VISIT()
