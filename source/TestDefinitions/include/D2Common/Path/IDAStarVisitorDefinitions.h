#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Path/IDAStar.h>


BEGIN_VISIT(D2PathIDAStarNodeStrc)
	FIELD(nFScore)
	FIELD(nHeuristicDistanceToTarget)
	FIELD(nBestDistanceFromStart)
	FIELD(nEvaluationsCount)
	OBJECT(tCoord)
	//POINTER(pNeighborsSequence)
	FIELD(nNextNeighborIndex)
	POINTER(pParent)
	POINTER(pBestChild)
END_VISIT()

BEGIN_VISIT(D2PathIDAStarContextStrc)
	POINTER(pCurrentNode)
	ARRAY(aNodesStorage)
	FIELD(nNodesCount)
	ARRAY(nCoord)
	FIELD(nStride)
	FIELD(nXOffset)
	FIELD(nYOffset)
	ARRAY(aCoordData)
	FIELD(bRandomDirection)
	POINTER(pSeed)
END_VISIT()
