#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Drlg/D2DrlgPreset.h>


BEGIN_VISIT(D2LevelFileListStrc)
	ARRAY(szPath)
	FIELD(nRefCount)
	POINTER(pFile)
	POINTER(pNext)
END_VISIT()

BEGIN_VISIT(D2PresetUnitStrc)
	FIELD(nUnitType)
	FIELD(nIndex)
	FIELD(nMode)
	FIELD(nXpos)
	FIELD(nYpos)
	FIELD(bSpawned)
	POINTER(pMapAI)
	POINTER(pNext)
END_VISIT()

BEGIN_VISIT(D2DrlgMapStrc)
	FIELD(nLevelPrest)
	FIELD(nPickedFile)
	POINTER(pLvlPrestTxtRecord)
	POINTER(pFile)
	OBJECT(pDrlgCoord)
	FIELD(bHasInfo)
	OBJECT(pMapGrid)
	POINTER(pPresetUnit)
	FIELD(bInited)
	FIELD(nPops)
	//POINTER(pPopsIndex)
	//POINTER(pPopsSubIndex)
	//POINTER(pPopsOrientation)
	POINTER(pPopsLocation)
	POINTER(pNext)
END_VISIT()

BEGIN_VISIT(D2DrlgPresetRoomStrc)
	FIELD(nLevelPrest)
	FIELD(nPickedFile)
	POINTER(pMap)
	//BEGIN_UNION()
		FIELD(nFlags)
		ARRAY(unk0x0D)
		//FIELD(dwFlags)
	//END_UNION()
	ARRAY(pWallGrid)
	ARRAY(pTileTypeGrid)
	ARRAY(pFloorGrid)
	OBJECT(pCellGrid)
	POINTER(pMazeGrid)
	POINTER(pTombStoneTiles)
	FIELD(nTombStoneTiles)
END_VISIT()
