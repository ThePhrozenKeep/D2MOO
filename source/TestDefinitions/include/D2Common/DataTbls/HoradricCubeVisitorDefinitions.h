#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <DataTbls/HoradricCube.h>


#ifdef D2_VERSION_HAS_UBERS

#endif

BEGIN_VISIT(D2CubeInputItem)
	FIELD(wInputFlags)
	FIELD(wItem)
	FIELD(wItemID)
	FIELD(nQuality)
	FIELD(nQuantity)
END_VISIT()

BEGIN_VISIT(D2CubeOutputParamStrc)
	FIELD(nMod)
	FIELD(nModParam)
	FIELD(nModMin)
	FIELD(nModMax)
	FIELD(nModChance)
END_VISIT()

BEGIN_VISIT(D2CubeOutputItem)
	FIELD(wItemFlags)
	FIELD(wBaseItemId)
	FIELD(wItemID)
	FIELD(nQuality)
	FIELD(nQuantity)
	FIELD(nType)
	FIELD(nLvl)
	FIELD(nPLvl)
	FIELD(nILvl)
	ARRAY(wPrefixId)
	ARRAY(wSuffixId)
	ARRAY(pParam)
END_VISIT()

BEGIN_VISIT(D2CubeMainTxt)
	FIELD(nEnabled)
	FIELD(nLadder)
	FIELD(nMinDiff)
	FIELD(nClass)
	FIELD(nOp)
	ARRAY(pad0x05)
	FIELD(nParam)
	FIELD(nValue)
	FIELD(nNumInputs)
	FIELD(pad0x11)
	FIELD(wVersion)
	ARRAY(pInputItem)
	ARRAY(pOutputItem)
END_VISIT()
