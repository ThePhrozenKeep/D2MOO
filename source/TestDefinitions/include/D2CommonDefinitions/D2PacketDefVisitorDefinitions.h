#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2PacketDef.h>


BEGIN_VISIT(D2MessageStrc)
	FIELD(nMenu)
	FIELD(pad0x01)
	FIELD(nStringId)
END_VISIT()

BEGIN_VISIT(D2MessageListStrc)
	FIELD(nCount)
	FIELD(pad0x01)
	ARRAY(pMessages)
END_VISIT()
