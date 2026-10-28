#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Environment.h>


BEGIN_VISIT(D2EnvironmentCycleStrc)
	FIELD(nTicksBegin)
	FIELD(nPeriodOfDay)
	FIELD(nRed)
	FIELD(nGreen)
	FIELD(nBlue)
	FIELD(nIntensity)
END_VISIT()

BEGIN_VISIT(D2DrlgEnvironmentStrc)
	FIELD(nCycleIndex)
	FIELD(nPeriodOfDay)
	FIELD(nTicks)
	FIELD(nIntensity)
	FIELD(dwInitTick)
	FIELD(unk0x14)
	FIELD(nRed)
	FIELD(nGreen)
	FIELD(nBlue)
	FIELD(pad0x1B)
	FIELD(fCos)
	FIELD(fLast)
	FIELD(fSin)
	FIELD(nTimeRate)
	FIELD(nTimeRateIndex)
	FIELD(bEclipse)
	FIELD(nPrev)
END_VISIT()
