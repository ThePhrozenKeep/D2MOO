#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Chat.h>


BEGIN_VISIT(D2HoverTextStrc)
	FIELD(dwDisplayTime)
	FIELD(dwExpireTime)
	FIELD(nLangId)
	ARRAY(pad0x09)
	FIELD(bUsed)
	ARRAY(szMsg)
END_VISIT()
