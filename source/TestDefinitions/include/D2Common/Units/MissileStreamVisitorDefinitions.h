#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Units/MissileStream.h>


BEGIN_VISIT(D2MissileStreamStrc)
	DYNAMIC_ARRAY(unk0x00, unk0x04)
	FIELD(unk0x04)
END_VISIT()
