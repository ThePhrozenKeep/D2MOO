#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Units/UnitFinds.h>


BEGIN_VISIT(D2UnitFindArgStrc)
	FIELD(nFlags)
	FIELD(unk0x04)
	POINTER(pUnit)
	FIELD(nX)
	FIELD(nY)
	FIELD(nSize)
	FIELD(nMaxArrayEntries)
	FIELD(nIndex)
	POINTER(pField)
	//OBJECT(pfnUnitTest)
	FIELD(unk0x28)
	FIELD(unk0x2C)
	FIELD(unk0x30)
	FIELD(unk0x34)
END_VISIT()

BEGIN_VISIT(D2UnitFindDataStrc)
	OMIT(pMemPool)
	//POINTER(pUnitsArray)
	POINTER(pRoom)
	//OBJECT(pfnUnitTest)
	POINTER(pUnitFindArg)
	FIELD(nFlags)
	FIELD(nIndex)
	FIELD(nMaxArrayEntries)
	FIELD(nX)
	FIELD(nY)
	FIELD(nSize)
END_VISIT()
