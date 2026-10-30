#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Text.h>


BEGIN_VISIT(D2TextNodeStrc)
	FIELD(nStringId)
	FIELD(pad0x02)
	FIELD(nMenu)
	POINTER(pNext)
END_VISIT()

BEGIN_VISIT(D2TextHeaderStrc)
	OMIT(pMemPool)
	FIELD(nCount)
	FIELD(pad0x06)
	POINTER(pNode)
END_VISIT()

BEGIN_VISIT(D2TextMessageStrc)
	//ARRAY(pStringLines)
	//FIELD(nLines)
	//FIELD(nColor)
	//FIELD(nEndTick)
	//POINTER(pNext)
END_VISIT()
