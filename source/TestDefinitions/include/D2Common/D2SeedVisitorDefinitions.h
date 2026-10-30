#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Seed.h>


BEGIN_VISIT(D2SeedStrc)
	//BEGIN_UNION()
		FIELD(nLowSeed)
		FIELD(nHighSeed)
		//FIELD(lSeed)
	//END_UNION()
END_VISIT()
