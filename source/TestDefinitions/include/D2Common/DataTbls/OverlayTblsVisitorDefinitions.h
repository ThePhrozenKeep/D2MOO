#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <DataTbls/OverlayTbls.h>


BEGIN_VISIT(D2OverlayTxt)
	FIELD(wOverlay)
	ARRAY(szFilename)
	FIELD(wVersion)
	FIELD(dwFrames)
	FIELD(nPreDraw)
	ARRAY(pad0x49)
	FIELD(dw1ofN)
	FIELD(nDir)
	FIELD(nOpen)
	FIELD(nBeta)
	FIELD(pad0x53)
	FIELD(dwOffsetX)
	FIELD(dwOffsetY)
	ARRAY(dwHeight)
	FIELD(dwAnimRate)
	FIELD(dwInitRadius)
	FIELD(dwRadius)
	FIELD(dwLoopWaitTime)
	FIELD(nTrans)
	FIELD(nRed)
	FIELD(nGreen)
	FIELD(nBlue)
	FIELD(nNumDirections)
	FIELD(nLocalBlood)
	FIELD(pad0x82)
END_VISIT()
