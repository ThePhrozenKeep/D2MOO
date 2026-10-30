#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <D2Gfx.h>
#include <Window.h>


BEGIN_VISIT(PALETTEENTRY)
	FIELD(peRed)
	FIELD(peGreen)
	FIELD(peBlue)
	FIELD(peFlags)
END_VISIT()

BEGIN_VISIT(D2PaletteTableStrc)
	// Pointers to palettes without a visitor, only the pointers are compared
	FIELD(transPalettes)
	FIELD(shadowLightGammaPalettes)
	FIELD(standardShiftPalettes)
	FIELD(pScreenPalette)
	FIELD(pLuminancePalette)
	FIELD(pDarkBlendPalette)
	FIELD(pDarkenPalette)
	FIELD(pStandardColorsPalette)
	//OMIT(unk0x118) // uint8_t* would be printed as a string by the diff
	//OMIT(unk0x11C) // uint8_t* would be printed as a string by the diff
	ARRAY(datFilePalette)
END_VISIT()

BEGIN_VISIT(D2GfxLightStrc)
	FIELD(nIntensity)
	FIELD(nRed)
	FIELD(nGreen)
	FIELD(nBlue)
END_VISIT()

BEGIN_VISIT(D2GfxLightExStrc)
	FIELD(nIntensity)
	FIELD(nRed)
	FIELD(nGreen)
	FIELD(nBlue)
	FIELD(nX)
	FIELD(nY)
END_VISIT()

BEGIN_VISIT(D2GfxSettingsStrc)
	FIELD(bPerspectiveEnabled)
	FIELD(bPerspectiveCapable)
	FIELD(bLowQuality)
	FIELD(nGamma)
	FIELD(bVSync)
	FIELD(bBlendedShadows)
END_VISIT()

BEGIN_VISIT(POINT)
	FIELD(x)
	FIELD(y)
END_VISIT()

BEGIN_VISIT(RECT)
	FIELD(left)
	FIELD(top)
	FIELD(right)
	FIELD(bottom)
END_VISIT()

BEGIN_VISIT(WINDOWPLACEMENT)
	FIELD(length)
	FIELD(flags)
	FIELD(showCmd)
	OBJECT(ptMinPosition)
	OBJECT(ptMaxPosition)
	OBJECT(rcNormalPosition)
END_VISIT()

BEGIN_VISIT(D2WindowPlacementStrc)
	FIELD(hWnd)
	OBJECT(windowPlacement)
END_VISIT()
