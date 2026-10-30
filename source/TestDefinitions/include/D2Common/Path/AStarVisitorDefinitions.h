#pragma once


#include <DiffContext.h>
#include <DiffVisitor.h>
#include <Macros.h>


#include <Path/AStar.h>


BEGIN_VISIT(D2PathFoWallNodeStrc)
	OBJECT(tPoint)
	FIELD(nFScore)
	FIELD(nHeuristicDistanceToTarget)
	FIELD(nBestDistanceFromStart)
	FIELD(wPad)
	POINTER(pBestParent)
	ARRAY(pChildren)
	POINTER(pNextCachePoint)
	POINTER(pNextSortedByFScore)
END_VISIT()

BEGIN_VISIT(D2PathFoWallContextStrc)
	ARRAY(aPendingCache)
	ARRAY(aVisitedCache)
	POINTER(pSortedListByFScore)
	ARRAY(aNodesStorage)
	FIELD(nNodesCount)
	ARRAY(aPointsStack)
	FIELD(nStackCount)
END_VISIT()
