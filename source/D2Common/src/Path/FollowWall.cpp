#include "Path/Path.h"
#include "Path/FollowWall.h"

#include "D2Collision.h"
#include "D2Dungeon.h"
#include <cmath>

//1.10f: D2Common.0x6FDD17E0
//1.13c: D2Common.0x6FDDB458
const int gaIdaStar_PointCacheHashMaskX[D2PathFoWallContextStrc::CACHE_SIZE] =
{
	57, 92, 72, 94, 119, 80, 14, 84, 93, 17, 20, 37, 87, 70, 100, 108,
	30, 91, 85, 60, 40, 3, 118, 78, 81, 109, 43, 26, 67, 27, 49, 75,
	69, 2, 58, 36, 55, 5, 122, 125, 11, 35, 77, 59, 29, 18, 28, 82,
	63, 86, 83, 105, 90, 48, 101, 38, 115, 9, 41, 89, 95, 19, 1, 61,
	51, 7, 110, 34, 126, 47, 15, 6, 99, 22, 104, 4, 71, 97, 121, 114,
	0, 112, 62, 54, 23, 111, 74, 123, 45, 120, 44, 50, 31, 39, 42, 66,
	33, 113, 53, 64, 16, 65, 106, 96, 88, 24, 46, 56, 103, 68, 10, 117,
	25, 32, 124, 127, 76, 8, 79, 21, 102, 52, 107, 73, 13, 116, 98, 12
};

//1.10f: D2Common.0x6FDD19E0
//1.13c: D2Common.0x6FDDB658
const int gaIdaStar_PointCacheHashMaskY[D2PathFoWallContextStrc::CACHE_SIZE] =
{
	114, 80, 68, 91, 97, 73, 127, 88, 18, 58, 101, 41, 19, 108, 6, 8,
	24, 99, 83, 33, 77, 56, 74, 37, 43, 107, 85, 12, 75, 30, 27, 23,
	92, 69, 115, 2, 84, 7, 17, 20, 117, 67, 31, 121, 111, 105, 120, 96,
	94, 122, 47, 25, 4, 112, 34, 62, 10, 89, 1, 40, 76, 71, 49, 51,
	90, 55, 118, 50, 116, 106, 109, 42, 45, 65, 28, 60, 123, 36, 98, 48,
	124, 22, 64, 38, 15, 102, 110, 79, 57, 125, 59, 21, 70, 63, 93, 86,
	66, 35, 16, 53, 61, 103, 29, 52, 119, 46, 100, 3, 0, 126, 32, 81,
	87, 9, 26, 14, 95, 113, 13, 54, 104, 82, 44, 72, 5, 78, 11, 39
};


//1.10f: Inlined
//1.13c: D2Common.0x6FDCB2C0
BOOL PATH_FoWall_TargetLocationHasEnoughRoom(D2PathInfoStrc* pPathInfo)
{
	if (!pPathInfo->pDynamicPath->pTargetUnit)
	{
		return TRUE;
	}
	D2ActiveRoomStrc* pStartRoom = pPathInfo->pStartRoom;
	const int nTargetY = pPathInfo->tTargetCoord.Y;
	const int nTargetX = pPathInfo->tTargetCoord.X;
	const int32_t nCollisionMask = pPathInfo->nCollisionMask;
	const int32_t nCollisionPattern = pPathInfo->nCollisionPattern;
	return !COLLISION_CheckAnyCollisionWithPattern(pStartRoom, nTargetX - 2, nTargetY - 2, nCollisionPattern, nCollisionMask)
		|| !COLLISION_CheckAnyCollisionWithPattern(pStartRoom, nTargetX - 2, nTargetY + 2, nCollisionPattern, nCollisionMask)
		|| !COLLISION_CheckAnyCollisionWithPattern(pStartRoom, nTargetX + 2, nTargetY - 2, nCollisionPattern, nCollisionMask)
		|| !COLLISION_CheckAnyCollisionWithPattern(pStartRoom, nTargetX + 2, nTargetY + 2, nCollisionPattern, nCollisionMask)
		|| !COLLISION_CheckAnyCollisionWithPattern(pStartRoom, nTargetX - 2, nTargetY + 0, nCollisionPattern, nCollisionMask)
		|| !COLLISION_CheckAnyCollisionWithPattern(pStartRoom, nTargetX + 0, nTargetY - 2, nCollisionPattern, nCollisionMask)
		|| !COLLISION_CheckAnyCollisionWithPattern(pStartRoom, nTargetX + 2, nTargetY + 0, nCollisionPattern, nCollisionMask)
		|| !COLLISION_CheckAnyCollisionWithPattern(pStartRoom, nTargetX + 0, nTargetY + 2, nCollisionPattern, nCollisionMask);
}

// Helper function
static D2PathFoWallNodeStrc* PATH_IdaStar_FindSortedByFScoreInsertionPoint(D2PathFoWallContextStrc* pContext, uint16_t nFScore)
{
	D2PathFoWallNodeStrc* pInsertionPoint = nullptr;
	for (D2PathFoWallNodeStrc* pCurrent = pContext->pSortedListByFScore;
		pCurrent != nullptr;
		pCurrent = pCurrent->pNextSortedByFScore)
	{
		if (pCurrent->nFScore >= nFScore)
		{
			break;
		}
		pInsertionPoint = pCurrent;
	}
	return pInsertionPoint;
}

// Helper function
static D2PathFoWallNodeStrc* PATH_FoWall_RemoveFromPendingCache(D2PathFoWallContextStrc* pContext, D2PathFoWallNodeStrc* pToRemove)
{
	D2PathFoWallNodeStrc* pPreviousPoint = nullptr;

	const uint8_t nCacheIndex = (gaIdaStar_PointCacheHashMaskX[pToRemove->tPoint.X & 0x7F] + gaIdaStar_PointCacheHashMaskY[pToRemove->tPoint.Y & 0x7F]) & 0x7F;
	for (D2PathFoWallNodeStrc* pCacheEntry = pContext->aPendingCache[nCacheIndex];
		pCacheEntry != nullptr;
		pCacheEntry = pCacheEntry->pNextCachePoint)
	{
		if (pCacheEntry == pToRemove)
		{
			break;
		}

		pPreviousPoint = pCacheEntry;

		// Safety check, original game didn't actually stop on pCache0Entry != nullptr
		// This is because it expects the point to be in the list!
		D2_ASSERT(pCacheEntry->pNextCachePoint != nullptr);
	}

	if (pPreviousPoint)
	{
		pPreviousPoint->pNextCachePoint = pToRemove->pNextCachePoint;
	}
	else
	{
		pContext->aPendingCache[nCacheIndex] = pToRemove->pNextCachePoint; // Handle the case where pContext->aCache0[nCacheIndex] == pToRemove
	}
	pToRemove->pNextCachePoint = 0; // Not in the cache list anymore
	return pPreviousPoint;
}

//1.10f: Inlined
//1.13c: D2Common.0x6FDCB220
// TODO: rename?
D2PathFoWallNodeStrc* PATH_FoWall_PopBestScoreForVisit(D2PathFoWallContextStrc* pContext)
{
	D2PathFoWallNodeStrc* pPopped = pContext->pSortedListByFScore;
	if (pPopped)
	{
		// pop from sorted list
		pContext->pSortedListByFScore = pPopped->pNextSortedByFScore;
		PATH_FoWall_RemoveFromPendingCache(pContext, pPopped);
		PATH_FoWall_PushToVisitedCache(pContext, pPopped);
	}
	return pPopped;
}

//1.00:  D2Common.0x10056EC0
//1.10f: D2Common.0x6FDA69E0
//1.13c: D2Common.0x6FDCB630
int __fastcall PATH_FoWall_ComputePath(D2PathInfoStrc* pPathInfo)
{
	if (!PATH_FoWall_TargetLocationHasEnoughRoom(pPathInfo))
	{
		return 0;
	}

	const D2PathPointStrc tStartCoord = pPathInfo->tStartCoord;
	const D2PathPointStrc tTargetCoord = pPathInfo->tTargetCoord;

	D2PathFoWallContextStrc tContext{};
	tContext.nNodesCount = 1;

	const int nDistToTarget = PATH_FoWall_Heuristic(tTargetCoord, tStartCoord);
	tContext.aNodesStorage[0].nBestDistanceFromStart = 0;
	tContext.aNodesStorage[0].nHeuristicDistanceToTarget = nDistToTarget;
	tContext.aNodesStorage[0].nFScore = nDistToTarget;
	tContext.aNodesStorage[0].tPoint = tStartCoord;
	PATH_FoWall_MakeCandidate(&tContext, &tContext.aNodesStorage[0]);
	D2PathFoWallNodeStrc* pBestNode = 0;
	for (D2PathFoWallNodeStrc* pCurNodeToVisit = PATH_FoWall_PopBestScoreForVisit(&tContext);
		pCurNodeToVisit != nullptr;
		pCurNodeToVisit = PATH_FoWall_PopBestScoreForVisit(&tContext)
		)
	{
		if (pBestNode == nullptr
			|| pCurNodeToVisit->nHeuristicDistanceToTarget < pBestNode->nHeuristicDistanceToTarget
			|| (pCurNodeToVisit->nHeuristicDistanceToTarget == pBestNode->nHeuristicDistanceToTarget
				&& pCurNodeToVisit->nBestDistanceFromStart > (pBestNode->nBestDistanceFromStart + 5))
			)
		{
			pBestNode = pCurNodeToVisit;
		}

		if (// If reached target point
			pCurNodeToVisit->nHeuristicDistanceToTarget == 0
			// or can't allocate new nodes
			|| !PATH_FoWall_ExploreChildren(pPathInfo, &tContext, pCurNodeToVisit, tTargetCoord)
			)
		{
			break; // Then stop here
		}
	}
	if (pBestNode)
	{
		return PATH_FoWall_FlushNodeToDynamicPath(pBestNode, pPathInfo);
	}
	return 0;
}

//D2Common.0x6FDA6D10
int __fastcall PATH_FoWall_PushToVisitedCache(D2PathFoWallContextStrc* pContext, D2PathFoWallNodeStrc* pNode)
{
	const uint8_t nHash = (gaIdaStar_PointCacheHashMaskX[pNode->tPoint.X & 0x7F] + gaIdaStar_PointCacheHashMaskY[pNode->tPoint.Y & 0x7F]) & 0x7F;
	pNode->pNextCachePoint = pContext->aVisitedCache[nHash];
	pContext->aVisitedCache[nHash] = pNode;
	return nHash;
}

//1.10f: D2Common.0x6FDA6D50
//1.13c: D2Common.0x6FDCB3C0
BOOL __fastcall PATH_FoWall_ExploreChildren(D2PathInfoStrc* pPathInfo, D2PathFoWallContextStrc* pContext, D2PathFoWallNodeStrc* a3, D2PathPointStrc tTargetCoord)
{
	static const D2CoordStrc aOffsets[] =
	{
		{-1,-1},
		{-1,1},
		{1,-1},
		{1,1},
		{-1,0},
		{0,-1},
		{1,0},
		{0,1}
	};

	for (D2CoordStrc tOffset : aOffsets)
	{
		D2PathPointStrc tNewPointCoord = a3->tPoint;
		tNewPointCoord.X += tOffset.nX;
		tNewPointCoord.Y += tOffset.nY;

		if (!COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pStartRoom, tNewPointCoord.X, tNewPointCoord.Y, pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask) &&
			!PATH_FoWall_EvaluateNeighbor(pPathInfo, pContext, a3, tNewPointCoord, tTargetCoord))
		{
			return FALSE;
		}
	}

	return TRUE;
}

//1.10f: D2Common.0x6FDA7230
//1.13c: D2Common.0x6FDCAF70
// Diagonal distance with D=2 and D2=3 (2 to move to an adjacent pixel, 3 in diagonal)
// http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html#diagonal-distance 
// Note: This heuristic returns values in the same order as the Euclidian distance to pixel centers
//       It is both "admissible" and "consistent".
// Sample return values for the first quadrant:
// dY
// |6 7 8 9
// |4 5 6 8
// |2 3 5 7
// |0 2 4 6
// +-------dX
int __stdcall PATH_FoWall_Heuristic(D2PathPointStrc tPoint1, D2PathPointStrc tPoint2)
{
	const int nDiffX = std::abs(tPoint1.X - tPoint2.X);
	const int nDiffY = std::abs(tPoint1.Y - tPoint2.Y);
	if (nDiffX < nDiffY)
	{
		return nDiffX + 2 * nDiffY;
	}
	else
	{
		return nDiffY + 2 * nDiffX;
	}
}

// Helper function
int16_t PATH_FoWall_HeuristicForNeighbor(D2PathPointStrc tPoint, D2PathPointStrc tNeighbor)
{
	if (tPoint.X == tNeighbor.X || tPoint.Y == tNeighbor.Y)
	{
		return 2; // Adjacent pixel
	}
	else
	{
		return 3; // Diagonal pixel
	}
}

//1.00:  D2Common.0x10057A10
//1.10f: D2Common.0x6FDA7280
//1.13c: D2Common.0x6FDCAF20
D2PathFoWallNodeStrc* __fastcall PATH_FoWall_GetNodeFromPendingCache(D2PathFoWallContextStrc* pContext, D2PathPointStrc tPathPoint)
{
	const uint8_t nHash = uint8_t((gaIdaStar_PointCacheHashMaskX[tPathPoint.X & 0x7F] + gaIdaStar_PointCacheHashMaskY[tPathPoint.Y & 0x7F]) & 0x7F);
	for (D2PathFoWallNodeStrc* pNode = pContext->aPendingCache[nHash];
		pNode != nullptr;
		pNode = pNode->pNextCachePoint)
	{
		if (tPathPoint.X == pNode->tPoint.X && tPathPoint.Y == pNode->tPoint.Y)
		{
			return pNode;
		}
	}
	return nullptr;
}

//1.00:  D2Common.0x10057A10
//1.10f: D2Common.0x6FDA72D0
//1.13c: D2Common.0x6FDCAED0
D2PathFoWallNodeStrc* __fastcall PATH_FoWall_FindPointInVisitedCache(D2PathFoWallContextStrc* pContext, D2PathPointStrc tPathPoint)
{
	const uint8_t nHash = uint8_t((gaIdaStar_PointCacheHashMaskX[tPathPoint.X & 0x7F] + gaIdaStar_PointCacheHashMaskY[tPathPoint.Y & 0x7F]) & 0x7F);
	for (D2PathFoWallNodeStrc* pNode = pContext->aVisitedCache[nHash];
		pNode != nullptr;
		pNode = pNode->pNextCachePoint)
	{
		if (tPathPoint.X == pNode->tPoint.X && tPathPoint.Y == pNode->tPoint.Y)
		{
			return pNode;
		}
	}
	return nullptr;
}

//1.10f: D2Common.0x6FDA7320
//1.13c: D2Common.0x6FDCAE20
void __fastcall PATH_FoWall_MakeCandidate(D2PathFoWallContextStrc* pContext, D2PathFoWallNodeStrc* pNode)
{
	const uint8_t nHash = (gaIdaStar_PointCacheHashMaskX[pNode->tPoint.X & 0x7F] + gaIdaStar_PointCacheHashMaskY[pNode->tPoint.Y & 0x7F]) & 0x7F;
	pNode->pNextCachePoint = pContext->aPendingCache[nHash];
	pContext->aPendingCache[nHash] = pNode;

	if (D2PathFoWallNodeStrc* pInsertionPoint = PATH_IdaStar_FindSortedByFScoreInsertionPoint(pContext, pNode->nFScore))
	{
		pNode->pNextSortedByFScore = pInsertionPoint->pNextSortedByFScore;
		pInsertionPoint->pNextSortedByFScore = pNode;
	}
	else
	{
		pNode->pNextSortedByFScore = pContext->pSortedListByFScore;
		pContext->pSortedListByFScore = pNode;
	}
}

//1.10f: D2Common.0x6FDA7390
//1.13c: D2Common.0x6FDCAC50
//Should be __thiscall but we have to use __fastcall, hence nUnused
void __fastcall PATH_FoWall_PropagateNewFScoreToChildren(D2PathFoWallContextStrc* pContext, int nUnused, D2PathFoWallNodeStrc* pNewNode)
{
	D2_MAYBE_UNUSED(nUnused);

	pContext->aPointsStack[pContext->nStackCount++] = pNewNode;
	while (pContext->nStackCount != 0)
	{
		D2PathFoWallNodeStrc* pCurrentPoint = pContext->aPointsStack[--pContext->nStackCount];
		for (D2PathFoWallNodeStrc* pChild : pCurrentPoint->pChildren)
		{
			if (!pChild)
			{
				break;
			}
			const int16_t nDistanceBetweenPoints = PATH_FoWall_HeuristicForNeighbor(pCurrentPoint->tPoint, pChild->tPoint);
			const int16_t nDistanceFromStart = nDistanceBetweenPoints + pCurrentPoint->nBestDistanceFromStart;
			if (nDistanceFromStart < pChild->nBestDistanceFromStart)
			{
				pChild->pBestParent = pCurrentPoint;
				pChild->nBestDistanceFromStart = nDistanceFromStart;
				pChild->nFScore = nDistanceFromStart + pChild->nHeuristicDistanceToTarget;
				pContext->aPointsStack[pContext->nStackCount++] = pChild;
			}
		}
	}
}

//1.10f: D2Common.0x6FDA7450
//1.13c: Inlined
D2PathFoWallNodeStrc* __fastcall PATH_FoWall_GetNewNode(D2PathFoWallContextStrc* pContext)
{
	if (pContext->nNodesCount == ARRAY_SIZE(pContext->aNodesStorage))
	{
		return nullptr;
	}
	D2PathFoWallNodeStrc* pNewNode = &pContext->aNodesStorage[pContext->nNodesCount++];
	memset(pNewNode, 0, sizeof(*pNewNode));
	return pNewNode;
}

// Helper function
static void PATH_FoWall_AddChildToNode(D2PathFoWallNodeStrc* pNode, D2PathFoWallNodeStrc* pChild)
{
	for (D2PathFoWallNodeStrc*& pChildRef : pNode->pChildren)
	{
		if (pChildRef == nullptr)
		{
			pChildRef = pChild;
			break;
		}
	}
}

//1.10f: D2Common.0x6FDA7490
//1.13c: D2Common.0x6FDCAFB0
BOOL __fastcall PATH_FoWall_EvaluateNeighbor(D2PathInfoStrc* pPathInfo, D2PathFoWallContextStrc* pContext, D2PathFoWallNodeStrc* pCurrentNode, D2PathPointStrc tNewPointCoord, D2PathPointStrc tTargetCoord)
{
	const int16_t nDistanceBetweenPoints = PATH_FoWall_HeuristicForNeighbor(pCurrentNode->tPoint, tNewPointCoord);
	const int16_t nNewPointDistance = pCurrentNode->nBestDistanceFromStart + nDistanceBetweenPoints;
	if (D2PathFoWallNodeStrc* pNewNode = PATH_FoWall_GetNodeFromPendingCache(pContext, tNewPointCoord))
	{
		PATH_FoWall_AddChildToNode(pCurrentNode, pNewNode);
		if (nNewPointDistance < pNewNode->nBestDistanceFromStart)
		{
			pNewNode->pBestParent = pCurrentNode;
			pNewNode->nBestDistanceFromStart = nNewPointDistance;
			pNewNode->nFScore = nNewPointDistance + pNewNode->nHeuristicDistanceToTarget;
		}
		return TRUE;
	}
	else if (D2PathFoWallNodeStrc* pNewNode = PATH_FoWall_FindPointInVisitedCache(pContext, tNewPointCoord))
	{
		PATH_FoWall_AddChildToNode(pCurrentNode, pNewNode);
		if (nNewPointDistance < pNewNode->nBestDistanceFromStart)
		{
			pNewNode->pBestParent = pCurrentNode;
			pNewNode->nBestDistanceFromStart = nNewPointDistance;
			pNewNode->nFScore = nNewPointDistance + pNewNode->nHeuristicDistanceToTarget;
			PATH_FoWall_PropagateNewFScoreToChildren(pContext,/*unused*/0, pNewNode);
		}
		return TRUE;
	}
	else if (D2PathFoWallNodeStrc* pNewNode = PATH_FoWall_GetNewNode(pContext))
	{
		PATH_FoWall_AddChildToNode(pCurrentNode, pNewNode);

		pNewNode->tPoint = tNewPointCoord;
		pNewNode->nHeuristicDistanceToTarget = PATH_FoWall_Heuristic(tNewPointCoord, tTargetCoord);

		pNewNode->pBestParent = pCurrentNode;
		pNewNode->nBestDistanceFromStart = nNewPointDistance;
		pNewNode->nFScore = pNewNode->nBestDistanceFromStart + pNewNode->nHeuristicDistanceToTarget;
		PATH_FoWall_MakeCandidate(pContext, pNewNode);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//1.10f: D2Common.0x6FDA78A0
//1.13c: D2Common.0x6FDCAB50
// Takes a node list, and builds the path using straight lines into the path info.
// Note that the node list stores the path from the end to the beginning (in reverse order).
// It also seems like it won't copy the last point of the path list (first of the array), not sure if it's intended or not.
signed int __fastcall PATH_FoWall_FlushNodeToDynamicPath(D2PathFoWallNodeStrc* pNode, D2PathInfoStrc* pPathInfo)
{
	if (!pNode)
	{
		return 0;
	}

	int nbPoints = 0;
	D2PathPointStrc tempPathPoints[78];
	// Assumes all points are conex, so we can't have a delta of -2, hence used for init
	int prevDeltaX = -2;
	int prevDeltaY = -2;
	while (pNode && pNode->pBestParent && nbPoints < D2DynamicPathStrc::MAXPATHLEN)
	{
		D2PathFoWallNodeStrc* pNextPoint = pNode->pBestParent;
		const int deltaX = pNode->tPoint.X - pNextPoint->tPoint.X;
		const int deltaY = pNode->tPoint.Y - pNextPoint->tPoint.Y;
		// If the direction doesn't change, then ignore the point
		if (deltaX != prevDeltaX || deltaY != prevDeltaY)
		{
			++nbPoints;
			// Store path in reverse order
			tempPathPoints[D2DynamicPathStrc::MAXPATHLEN - nbPoints] = pNode->tPoint;
			prevDeltaX = deltaX;
			prevDeltaY = deltaY;
		}
		pNode = pNextPoint;
	}

	// Note: IDAStar returns 0 if only 1 point is found. Optimization ?
	if (nbPoints < 1 || nbPoints >= D2DynamicPathStrc::MAXPATHLEN)
	{
		return 0;
	}

	memcpy(pPathInfo->pDynamicPath->PathPoints, &tempPathPoints[D2DynamicPathStrc::MAXPATHLEN - nbPoints], sizeof(D2PathPointStrc) * nbPoints);
	return nbPoints;
}
