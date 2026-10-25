#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <DataTbls/BeltsTbls.h>


BEGIN_VISIT(D2BeltsTxt)
	FIELD(nIndex)
	ARRAY(pad0x01)
	FIELD(nBoxes)
	ARRAY(beltBoxes)
END_VISIT()
