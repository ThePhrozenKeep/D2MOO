#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <CmnSubtile.h>


BEGIN_VISIT(D2PaletteStrc)
	ARRAY(nRemap)
END_VISIT()

BEGIN_VISIT(D2TileLibraryBlockStrc)
	FIELD(nPosX)
	FIELD(nPosY)
	FIELD(unk0x04)
	FIELD(nGridX)
	FIELD(nGridY)
	FIELD(nFormat)
	FIELD(nLength)
	FIELD(unk0x0E)
	//OMIT(dwOffset_pData) // Raw pixel data with a size depending on nFormat, compared by the tests when needed
END_VISIT()

BEGIN_VISIT(D2GfxTileLibraryEntryStrc)
	ARRAY(unk0x00)
	FIELD(nBlocks)
	DYNAMIC_ARRAY(pBlocks, nBlocks)
END_VISIT()
