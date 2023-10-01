#include "Path/Path.h"
#include "Path/Path_IDAStar.h"

#include "D2Collision.h"
#include <cmath>

//1.10f: D2Common.0x6FDD17E0
//1.13c: D2Common.0x6FDDB458
const int gaIdaStar_PointCacheHashMaskX[IDASTAR_CACHE_SIZE] =
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
const int gaIdaStar_PointCacheHashMaskY[IDASTAR_CACHE_SIZE] =
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
BOOL PATH_TargetLocationHasEnoughRoom(D2PathInfoStrc* pPathInfo)
{
	if (!pPathInfo->pDynamicPath->pTargetUnit)
	{
		return TRUE;
	}
	D2RoomStrc* pStartRoom = pPathInfo->pStartRoom;
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
static D2IdaStarPathPointsListStrc* PATH_IdaStar_FindSortedByFScoreInsertionPoint(D2IdaStarPathContextStrc* pIdaStarContext, uint16_t nFScore)
{
	D2IdaStarPathPointsListStrc* pInsertionPoint = nullptr;
	for (D2IdaStarPathPointsListStrc* pCurrent = pIdaStarContext->pSortedListByFScore;
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
static D2IdaStarPathPointsListStrc* PATH_IdaStar_RemoveCache0Point(D2IdaStarPathContextStrc* pIdaStarContext, D2IdaStarPathPointsListStrc* pToRemove)
{
	D2IdaStarPathPointsListStrc* pPreviousPoint = nullptr;
	
	const uint8_t nCacheIndex = (gaIdaStar_PointCacheHashMaskX[pToRemove->tPoint.X & 0x7F] + gaIdaStar_PointCacheHashMaskY[pToRemove->tPoint.Y & 0x7F]) & 0x7F;
	for (D2IdaStarPathPointsListStrc* pCache0Entry = pIdaStarContext->aCache0[nCacheIndex];
		pCache0Entry != nullptr;
		pCache0Entry = pCache0Entry->pNextCachePoint)
	{
		if (pCache0Entry == pToRemove)
		{
			break;
		}

		pPreviousPoint = pCache0Entry;

		// Safety check, original game didn't actually stop on pCache0Entry != nullptr
		// This is because it expects the point to be in the list!
		D2_ASSERT(pCache0Entry->pNextCachePoint != nullptr);
	}

	if (pPreviousPoint)
	{
		pPreviousPoint->pNextCachePoint = pToRemove->pNextCachePoint;
	}
	else
	{
		pIdaStarContext->aCache0[nCacheIndex] = pToRemove->pNextCachePoint; // Handle the case where pIdaStarContext->aCache0[nCacheIndex] == pToRemove
	}
	pToRemove->pNextCachePoint = 0; // Not in the cache list anymore
	return pPreviousPoint;
}

//1.10f: Inlined
//1.13c: D2Common.0x6FDCB220
// TODO: rename?
D2IdaStarPathPointsListStrc* PATH_IdaStar_RemoveFromCandidates(D2IdaStarPathContextStrc* pIdaStarContext)
{
	D2IdaStarPathPointsListStrc* pPopped = pIdaStarContext->pSortedListByFScore;
	if (pPopped)
	{
		// pop from sorted list
		pIdaStarContext->pSortedListByFScore = pPopped->pNextSortedByFScore;
		// Transfer from cache0 to cache1
		PATH_IdaStar_RemoveCache0Point(pIdaStarContext, pPopped);
		PATH_IdaStar_PushPointListToCache1(pIdaStarContext, pPopped);
	}
	return pPopped;
}

//1.00:  D2Common.0x10056EC0
//1.10f: D2Common.0x6FDA69E0
//1.13c: D2Common.0x6FDCB630
int __fastcall PATH_IDAStar_ComputePath(D2PathInfoStrc* pPathInfo)
{
	if (!PATH_TargetLocationHasEnoughRoom(pPathInfo))
	{
		return FALSE;
	}

	const D2PathPointStrc tStartCoord = pPathInfo->tStartCoord;
	const D2PathPointStrc tTargetCoord = pPathInfo->tTargetCoord;
		
	D2IdaStarPathContextStrc tIDAStarContext{};
	tIDAStarContext.nIndexUsed = 1;

	const int nDistToTarget = PATH_IdaStar_Heuristic(tTargetCoord, tStartCoord);
	tIDAStarContext.pArrayPositions[0].nBestDistanceFromStart = 0;
	tIDAStarContext.pArrayPositions[0].nHeuristicDistanceToTarget = nDistToTarget;
	tIDAStarContext.pArrayPositions[0].nFScore = nDistToTarget;
	tIDAStarContext.pArrayPositions[0].tPoint = tStartCoord;
	PATH_IdaStar_MakeCandidate(&tIDAStarContext, tIDAStarContext.pArrayPositions);
	D2IdaStarPathPointsListStrc* pBestPathAttempt = 0;
	D2IdaStarPathPointsListStrc* pCurPathAttempt = PATH_IdaStar_RemoveFromCandidates(&tIDAStarContext);
	if (pCurPathAttempt)
	{
		do
		{
			if (pBestPathAttempt == nullptr
			    || pCurPathAttempt->nHeuristicDistanceToTarget < pBestPathAttempt->nHeuristicDistanceToTarget
				|| (pCurPathAttempt->nHeuristicDistanceToTarget == pBestPathAttempt->nHeuristicDistanceToTarget 
					&& pCurPathAttempt->nBestDistanceFromStart > (pBestPathAttempt->nBestDistanceFromStart + 5))
				)
			{
				pBestPathAttempt = pCurPathAttempt;
			}

			if (pCurPathAttempt->nHeuristicDistanceToTarget == 0) // Reached target point
				break;
			if (!sub_6FDA6D50(pPathInfo, &tIDAStarContext, pCurPathAttempt, tTargetCoord))
				break;
			pCurPathAttempt = PATH_IdaStar_RemoveFromCandidates(&tIDAStarContext);
		} while (pCurPathAttempt);
		return PATH_IdaStar_FlushPointListToDynamicPath(pBestPathAttempt, pPathInfo);
	}
	return FALSE;
}

//D2Common.0x6FDA6D10
int __fastcall PATH_IdaStar_PushPointListToCache1(D2IdaStarPathContextStrc* pIdaStarContext, D2IdaStarPathPointsListStrc* pPointList)
{
	const uint8_t nHash = (gaIdaStar_PointCacheHashMaskX[pPointList->tPoint.X & 0x7F] + gaIdaStar_PointCacheHashMaskY[pPointList->tPoint.Y & 0x7F]) & 0x7F;
	pPointList->pNextCachePoint = pIdaStarContext->aCache1[nHash];
	pIdaStarContext->aCache1[nHash] = pPointList;
	return nHash;
}

//1.10f: D2Common.0x6FDA6D50
//1.13c: D2Common.0x6FDCB3C0
BOOL __fastcall sub_6FDA6D50(D2PathInfoStrc* pPathInfo, D2IdaStarPathContextStrc* pIdaStarContext, D2IdaStarPathPointsListStrc* a3, D2PathPointStrc tTargetCoord)
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
			!PATH_IdaStar_EvaluateNeighbour(pPathInfo, pIdaStarContext, a3, tNewPointCoord, tTargetCoord))
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
int __stdcall PATH_IdaStar_Heuristic(D2PathPointStrc tPoint1, D2PathPointStrc tPoint2)
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
static int16_t PATH_IdaStar_HeuristicForNeighbour(D2PathPointStrc tPoint, D2PathPointStrc tNeighbour)
{
	if (tPoint.X == tNeighbour.X || tPoint.Y == tNeighbour.Y)
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
D2IdaStarPathPointsListStrc* __fastcall PATH_IdaStar_GetPointListFromCache0(D2IdaStarPathContextStrc* pIdaStarContext, D2PathPointStrc tPathPoint)
{
	const uint8_t nHash = uint8_t((gaIdaStar_PointCacheHashMaskX[tPathPoint.X & 0x7F] + gaIdaStar_PointCacheHashMaskY[tPathPoint.Y & 0x7F]) & 0x7F);
	for (D2IdaStarPathPointsListStrc* pPointList = pIdaStarContext->aCache0[nHash];
		pPointList != nullptr;
		pPointList = pPointList->pNextCachePoint)
	{
		if (tPathPoint.X == pPointList->tPoint.X && tPathPoint.Y == pPointList->tPoint.Y)
		{
			return pPointList;
		}
	}
	return nullptr;
}

//1.00:  D2Common.0x10057A10
//1.10f: D2Common.0x6FDA72D0
//1.13c: D2Common.0x6FDCAED0
D2IdaStarPathPointsListStrc* __fastcall PATH_IdaStar_GetPointListFromCache1(D2IdaStarPathContextStrc* pIdaStarContext, D2PathPointStrc tPathPoint)
{
	const uint8_t nHash = uint8_t((gaIdaStar_PointCacheHashMaskX[tPathPoint.X & 0x7F] + gaIdaStar_PointCacheHashMaskY[tPathPoint.Y & 0x7F]) & 0x7F);
	for (D2IdaStarPathPointsListStrc* pPointList = pIdaStarContext->aCache1[nHash];
		pPointList != nullptr;
		pPointList = pPointList->pNextCachePoint)
	{
		if (tPathPoint.X == pPointList->tPoint.X && tPathPoint.Y == pPointList->tPoint.Y)
		{
			return pPointList;
		}
	}
	return nullptr;
}

//1.10f: D2Common.0x6FDA7320
//1.13c: D2Common.0x6FDCAE20
void __fastcall PATH_IdaStar_MakeCandidate(D2IdaStarPathContextStrc* pIdaStarContext, D2IdaStarPathPointsListStrc* pPoint)
{
	const uint8_t nHash = (gaIdaStar_PointCacheHashMaskX[pPoint->tPoint.X & 0x7F] + gaIdaStar_PointCacheHashMaskY[pPoint->tPoint.Y & 0x7F]) & 0x7F;
	pPoint->pNextCachePoint = pIdaStarContext->aCache0[nHash];
	pIdaStarContext->aCache0[nHash] = pPoint;

	if (D2IdaStarPathPointsListStrc* pInsertionPoint = PATH_IdaStar_FindSortedByFScoreInsertionPoint(pIdaStarContext, pPoint->nFScore))
	{
		pPoint->pNextSortedByFScore = pInsertionPoint->pNextSortedByFScore;
		pInsertionPoint->pNextSortedByFScore = pPoint;
	}
	else
	{
		pPoint->pNextSortedByFScore = pIdaStarContext->pSortedListByFScore;
		pIdaStarContext->pSortedListByFScore = pPoint;
	}
}

//1.10f: D2Common.0x6FDA7390
//1.13c: D2Common.0x6FDCAC50
//Should be __thiscall but we have to use __fastcall, hence nUnused
void __fastcall sub_6FDA7390(D2IdaStarPathContextStrc* pIDAStarContext, int nUnused, D2IdaStarPathPointsListStrc* pNewPoint)
{
	D2_MAYBE_UNUSED(nUnused);

	pIDAStarContext->aPointsStack[pIDAStarContext->nStackCount++] = pNewPoint;
	while (pIDAStarContext->nStackCount != 0)
	{
		D2IdaStarPathPointsListStrc* pCurrentPoint = pIDAStarContext->aPointsStack[--pIDAStarContext->nStackCount];
		for (D2IdaStarPathPointsListStrc* pParent : pCurrentPoint->pParents)
		{
			if (!pParent)
			{
				break;
			}
			const int16_t nDistanceBetweenPoints = PATH_IdaStar_HeuristicForNeighbour(pCurrentPoint->tPoint, pParent->tPoint);
			const int16_t nDistanceFromStart = nDistanceBetweenPoints + pCurrentPoint->nBestDistanceFromStart;
			if (nDistanceFromStart < pParent->nBestDistanceFromStart)
			{
				pParent->pNextPathAttempt = pCurrentPoint;
				pParent->nBestDistanceFromStart = nDistanceFromStart;
				pParent->nFScore = nDistanceFromStart + pParent->nHeuristicDistanceToTarget;
				pIDAStarContext->aPointsStack[pIDAStarContext->nStackCount++] = pParent;
			}
		}
	}
}

//1.10f: D2Common.0x6FDA7450
//1.13c: Inlined
D2IdaStarPathPointsListStrc* __fastcall PATH_IdaStar_GetNewPathList(D2IdaStarPathContextStrc* pIdaStarContext)
{
	if (pIdaStarContext->nIndexUsed == 200)
		return 0;
	D2IdaStarPathPointsListStrc* pNewPathList = &pIdaStarContext->pArrayPositions[pIdaStarContext->nIndexUsed++];
	memset(pNewPathList, 0, sizeof(D2IdaStarPathPointsListStrc));
	return pNewPathList;
}

// Helper function
static void PATH_IdaStar_PathListAddParent(D2IdaStarPathPointsListStrc* pPointList, D2IdaStarPathPointsListStrc* pParent)
{
	for (D2IdaStarPathPointsListStrc*& pParentRef : pPointList->pParents)
	{
		if (!pParentRef)
		{
			pParentRef = pParent;
			break;
		}
	}
}

//1.10f: D2Common.0x6FDA7490
//1.13c: D2Common.0x6FDCAFB0
signed int __fastcall PATH_IdaStar_EvaluateNeighbour(D2PathInfoStrc* pPathInfo, D2IdaStarPathContextStrc* pIdaStarContext, D2IdaStarPathPointsListStrc* pCurrentPointList, D2PathPointStrc tNewPointCoord, D2PathPointStrc tTargetCoord)
{
	const int16_t nDistanceBetweenPoints = PATH_IdaStar_HeuristicForNeighbour(pCurrentPointList->tPoint, tNewPointCoord);
	const int16_t nNewPointDistance = pCurrentPointList->nBestDistanceFromStart + nDistanceBetweenPoints;
	if (D2IdaStarPathPointsListStrc* pNewPointList = PATH_IdaStar_GetPointListFromCache0(pIdaStarContext, tNewPointCoord))
	{
		PATH_IdaStar_PathListAddParent(pCurrentPointList, pNewPointList);
		if (nNewPointDistance < pNewPointList->nBestDistanceFromStart)
		{
			pNewPointList->pNextPathAttempt = pCurrentPointList;
			pNewPointList->nBestDistanceFromStart = nNewPointDistance;
			pNewPointList->nFScore = nNewPointDistance + pNewPointList->nHeuristicDistanceToTarget;
		}
		return TRUE;
	}
	else if(D2IdaStarPathPointsListStrc* pNewPointList = PATH_IdaStar_GetPointListFromCache1(pIdaStarContext, tNewPointCoord))
	{
		PATH_IdaStar_PathListAddParent(pCurrentPointList, pNewPointList);
		if (nNewPointDistance < pNewPointList->nBestDistanceFromStart)
		{
			pNewPointList->pNextPathAttempt = pCurrentPointList;
			pNewPointList->nBestDistanceFromStart = nNewPointDistance;
			pNewPointList->nFScore = nNewPointDistance + pNewPointList->nHeuristicDistanceToTarget;
			sub_6FDA7390(pIdaStarContext,/*unused*/0, pNewPointList);
		}
		return TRUE;
	}
	else if(D2IdaStarPathPointsListStrc* pNewPointList = PATH_IdaStar_GetNewPathList(pIdaStarContext))
	{
		PATH_IdaStar_PathListAddParent(pCurrentPointList, pNewPointList);

		pNewPointList->tPoint = tNewPointCoord;
		pNewPointList->nHeuristicDistanceToTarget = PATH_IdaStar_Heuristic(tNewPointCoord, tTargetCoord);

		pNewPointList->pNextPathAttempt = pCurrentPointList;
		pNewPointList->nBestDistanceFromStart = nNewPointDistance;
		pNewPointList->nFScore = pNewPointList->nBestDistanceFromStart + pNewPointList->nHeuristicDistanceToTarget;
		PATH_IdaStar_MakeCandidate(pIdaStarContext, pNewPointList);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//1.10f: D2Common.0x6FDA78A0
//1.13c: D2Common.0x6FDCAB50
// Takes a path list, and builds the path using straight lines into the path info.
// Note that the path list stores the path from the end to the beginning (in reverse order).
// It also seems like it won't copy the last  point of the path list (first of the array), not sure if it's intended or not.
signed int __fastcall PATH_IdaStar_FlushPointListToDynamicPath(D2IdaStarPathPointsListStrc* pCurPoint, D2PathInfoStrc* pPathInfo)
{
	if (!pCurPoint)
	{
		return 0;
	}

	int nbPoints = 0;
	D2PathPointStrc tempPathPoints[78];
	// Assumes all points are conex, so we can't have a delta of -2, hence used for init
	int prevDeltaX = -2;
	int prevDeltaY = -2;
	while (pCurPoint && pCurPoint->pNextPathAttempt && nbPoints < D2DynamicPathStrc::MAXPATHLEN)
	{
		D2IdaStarPathPointsListStrc* pNextPoint = pCurPoint->pNextPathAttempt;
		const int deltaX = pCurPoint->tPoint.X - pNextPoint->tPoint.X;
		const int deltaY = pCurPoint->tPoint.Y - pNextPoint->tPoint.Y;
		// If the direction doesn't change, then ignore the point
		if (deltaX != prevDeltaX || deltaY != prevDeltaY)
		{
			++nbPoints;
			// Store path in reverse order
			tempPathPoints[D2DynamicPathStrc::MAXPATHLEN  - nbPoints] = pCurPoint->tPoint;
			prevDeltaX = deltaX;
			prevDeltaY = deltaY;
		}
		pCurPoint = pNextPoint;
	}

	if (nbPoints < 1 || nbPoints >= D2DynamicPathStrc::MAXPATHLEN)
	{
		return 0;
	}

	memcpy(pPathInfo->pDynamicPath->PathPoints, &tempPathPoints[D2DynamicPathStrc::MAXPATHLEN - nbPoints], sizeof(D2PathPointStrc) * nbPoints);
	return nbPoints;
}

//D2Common.0x6FDA7970
int __fastcall PATH_IdaStar_6FDA7970(D2PathInfoStrc* pPathInfo)
{
	UNIMPLEMENTED();
	return 0;
}

////D2Common.0x6FDA7D40) --------------------------------------------------------
//int __fastcall sub_6FDA7D40(int a1, signed int a2, int a3)
//{
//	int v3; // ebx@1
//	int v4; // esi@1
//	unsigned __int16 v5; // cx@2
//	bool v6; // zf@3
//	bool v7; // sf@3
//	unsigned __int8 v8; // of@3
//	int v9; // eax@4
//	int v10; // eax@8
//	__int16 v11; // bp@8
//	unsigned __int16 v12; // ax@8
//	unsigned __int16 v13; // bp@8
//	int v14; // edx@8
//	int v15; // edi@8
//	int v16; // ecx@8
//	int v17; // ecx@8
//	int v18; // edx@11
//	signed __int16 v19; // ax@20
//	__int16 v20; // bp@22
//	int v21; // edx@25
//	int v22; // eax@32
//	int v23; // ecx@34
//	int v24; // edx@36
//	int v25; // edx@39
//	__int16 v26; // cx@39
//	int v27; // edx@41
//	int v28; // eax@49
//	int v29; // eax@50
//	int v30; // eax@54
//	int v31; // ecx@54
//	char* v32; // eax@54
//	int v33; // ecx@55
//	signed __int64 v34; // qax@55
//	int v36; // [sp+10h] [bp-20h]@1
//	int v37; // [sp+14h] [bp-1Ch]@4
//	int v38; // [sp+14h] [bp-1Ch]@8
//	int v39; // [sp+18h] [bp-18h]@8
//	signed int v40; // [sp+1Ch] [bp-14h]@1
//	int v41; // [sp+20h] [bp-10h]@8
//	int v42; // [sp+28h] [bp-8h]@1
//
//	v3 = a1;
//	v4 = *(_DWORD *)a1;
//	v40 = a2;
//	v42 = *(_DWORD *)(a3 + 4);
//	v36 = 0;
//	do
//	{
//		while (1)
//		{
//			while (1)
//			{
//				while (1)
//				{
//LABEL_2:
//					v5 = *(_WORD *)(v4 + 8);
//					if (__PAIR__(*(_WORD *)(v4 + 10), v5) == v42)
//						return v4;
//					v8 = __OFSUB__(v36 + 1, 10000);
//					v6 = v36 == 9999;
//					v7 = v36++ - 9999 < 0;
//					if (!((unsigned __int8)(v7 ^ v8) | v6))
//						return 0;
//					v9 = *(_DWORD *)(v4 + 8);
//					v37 = *(_DWORD *)(v4 + 8);
//					if ((unsigned __int16)v9 < *(_WORD *)(v3 + 25208)
//						|| (unsigned __int16)v9 > *(_WORD *)(v3 + 25212)
//						|| HIWORD(v37) < *(_WORD *)(v3 + 25210)
//						|| HIWORD(v37) > *(_WORD *)(v3 + 25214))
//						return v4;
//					v10 = 4 * *(_DWORD *)(v4 + 16);
//					v11 = word_6FDD1D60[v10];
//					v12 = *(_WORD *)(v4 + 10) + word_6FDD1D64[v10];
//					v13 = v5 + v11;
//					LOWORD(v41) = v13;
//					HIWORD(v41) = v12;
//					v14 = v12;
//					v15 = v13;
//					v39 = v12;
//					v16 = v13 + *(_DWORD *)(v3 + 25224) + *(_DWORD *)(v3 + 25220) * (v12 + *(_DWORD *)(v3 + 25228));
//					v6 = *(_DWORD *)(v3 + 4 * v16 + 25232) == 0;
//					v17 = v3 + 4 * v16 + 25232;
//					v38 = v17;
//					if (!v6)
//						break;
//					if (!COLLISION_CheckAnyCollisionWithPattern(*(D2RoomStrc**)(a3 + 8), v13, v12, *(_DWORD *)(a3 + 40), *(_DWORD *)(a3 + 44)))
//					{
//						v14 = v39;
//						v17 = v38;
//						v12 = HIWORD(v41);
//						break;
//					}
//					*(_DWORD *)v38 = 1;
//					if (*(_WORD *)(v4 + 6) < 4)
//					{
//						v18 = *(_DWORD *)(v4 + 12) + 4;
//						*(_DWORD *)(v4 + 12) = v18;
//						if (*(_DWORD *)(v3 + 225232))
//							*(_DWORD *)(v4 + 16) = ((unsigned __int8)*(_DWORD *)(v4 + 16)
//													+ (unsigned __int8)**(_DWORD **)(v4 + 12)
//													+ (unsigned __int8)dword_6FDD1CE0[SEED_RollRandomNumber((D2SeedStrc*)*(_DWORD *)(v3 + 225236)) & 0x1F]) & 7;
//						else
//							*(_DWORD *)(v4 + 16) = ((unsigned __int8)*(_DWORD *)(v4 + 16)
//													+ (unsigned __int8)*(_DWORD *)v18) & 7;
//					}
//					++*(_WORD *)(v4 + 6);
//					if (*(_WORD *)(v4 + 6) == 5)
//					{
//						while (v4 != *(_DWORD *)v3)
//						{
//							v4 = *(_DWORD *)(v4 + 20);
//							*(_DWORD *)(v4 + 12) += 4;
//							sub_6FDA81C0(v4, v3);
//							++*(_WORD *)(v4 + 6);
//							if (*(_WORD *)(v4 + 6) != 5)
//								goto LABEL_2;
//						}
//						return 0;
//					}
//				}
//				if (*(_WORD *)(v4 + 8) == v13 || (v6 = *(_WORD *)(v4 + 10) == v12, v19 = 3, v6))
//					v19 = 2;
//				v20 = v19 + *(_WORD *)(v4 + 4);
//				if (!*(_DWORD *)v17)
//					break;
//				if (*(_DWORD *)v17 >= (unsigned int)v20)
//					break;
//				if (*(_WORD *)(v4 + 6) < 4)
//				{
//					v21 = *(_DWORD *)(v4 + 12) + 4;
//					*(_DWORD *)(v4 + 12) = v21;
//					if (*(_DWORD *)(v3 + 225232))
//						*(_DWORD *)(v4 + 16) = ((unsigned __int8)*(_DWORD *)(v4 + 16)
//												+ (unsigned __int8)**(_DWORD **)(v4 + 12)
//												+ (unsigned __int8)dword_6FDD1CE0[SEED_RollRandomNumber((D2SeedStrc*)*(_DWORD *)(v3 + 225236)) & 0x1F]) & 7;
//					else
//						*(_DWORD *)(v4 + 16) = ((unsigned __int8)*(_DWORD *)(v4 + 16) + (unsigned __int8)*(_DWORD *)v21) & 7;
//				}
//				++*(_WORD *)(v4 + 6);
//				if (*(_WORD *)(v4 + 6) == 5)
//				{
//					while (v4 != *(_DWORD *)v3)
//					{
//						v4 = *(_DWORD *)(v4 + 20);
//						*(_DWORD *)(v4 + 12) += 4;
//						sub_6FDA81C0(v4, v3);
//						++*(_WORD *)(v4 + 6);
//						if (*(_WORD *)(v4 + 6) != 5)
//							goto LABEL_2;
//					}
//					return 0;
//				}
//			}
//			*(_DWORD *)v17 = v20;
//			v22 = (unsigned __int16)v42 - v15;
//			if ((unsigned __int16)v42 - v15 < 0)
//				v22 = -v22;
//			v23 = HIWORD(v42) - v14;
//			if (HIWORD(v42) - v14 < 0)
//				v23 = -v23;
//			v24 = v22;
//			if (v22 >= v23)
//			{
//				v24 = v23;
//				if (v22 > v23)
//					v23 = v22;
//			}
//			v25 = v24 + 2 * v23;
//			v26 = v25 + v20;
//			if ((signed __int16)(v25 + v20) > v40)
//				break;
//			if (!*(_DWORD *)(v4 + 24))
//			{
//				v28 = *(_DWORD *)(v3 + 25204);
//				if (v28 == 900)
//				{
//					v29 = 0;
//				}
//				else
//				{
//					*(_DWORD *)(v3 + 25204) = v28 + 1;
//					memset((void*)(v3 + 28 * v28 + 4), 0, 0x1Cu);
//					v29 = v3 + 28 * v28 + 4;
//					v26 = v25 + v20;
//				}
//				*(_DWORD *)(v4 + 24) = v29;
//				if (!v29)
//					return 0;
//				*(_DWORD *)(v29 + 20) = v4;
//			}
//			v4 = *(_DWORD *)(v4 + 24);
//			*(_WORD *)(v4 + 2) = v25;
//			*(_WORD *)(v4 + 4) = v20;
//			*(_WORD *)v4 = v26;
//			*(_WORD *)(v4 + 6) = 0;
//			v30 = sub_6FDAB770(v41, v42);
//			v31 = *(_DWORD *)(v4 + 20);
//			v32 = (char*)&unk_6FDD1BE0 + 32 * ((v30 - *(_DWORD *)(v31 + 16)) & 7);
//			*(_DWORD *)(v4 + 12) = v32;
//			if (*(_DWORD *)(v3 + 225232))
//			{
//				v33 = *(_DWORD *)(v3 + 225236);
//				v34 = *(_DWORD *)(v33 + 4) + 1791398085i64 * *(_DWORD *)v33;
//				*(_QWORD *)v33 = v34;
//				*(_DWORD *)(v4 + 16) = ((unsigned __int8)**(_DWORD **)(v4 + 12)
//										+ (unsigned __int8)*(_DWORD *)(*(_DWORD *)(v4 + 20) + 16)
//										+ (unsigned __int8)dword_6FDD1CE0[v34 & 0x1F]) & 7;
//			}
//			else
//			{
//				*(_DWORD *)(v4 + 16) = ((unsigned __int8)*(_DWORD *)v32 + (unsigned __int8)*(_DWORD *)(v31 + 16)) & 7;
//			}
//			*(_DWORD *)(v4 + 8) = v41;
//			if (*(_WORD *)(v4 + 2) < (signed __int16)*(_BYTE *)(a3 + 20))
//				return v4;
//		}
//		if (*(_WORD *)(v4 + 6) < 4)
//		{
//			v27 = *(_DWORD *)(v4 + 12) + 4;
//			*(_DWORD *)(v4 + 12) = v27;
//			if (*(_DWORD *)(v3 + 225232))
//				*(_DWORD *)(v4 + 16) = ((unsigned __int8)*(_DWORD *)(v4 + 16)
//										+ (unsigned __int8)**(_DWORD **)(v4 + 12)
//										+ (unsigned __int8)dword_6FDD1CE0[SEED_RollRandomNumber((D2SeedStrc*)*(_DWORD *)(v3 + 225236)) & 0x1F]) & 7;
//			else
//				*(_DWORD *)(v4 + 16) = ((unsigned __int8)*(_DWORD *)(v4 + 16) + (unsigned __int8)*(_DWORD *)v27) & 7;
//		}
//		++*(_WORD *)(v4 + 6);
//	}
//	while (*(_WORD *)(v4 + 6) != 5);
//	while (v4 != *(_DWORD *)v3)
//	{
//		v4 = *(_DWORD *)(v4 + 20);
//		*(_DWORD *)(v4 + 12) += 4;
//		sub_6FDA81C0(v4, v3);
//		++*(_WORD *)(v4 + 6);
//		if (*(_WORD *)(v4 + 6) != 5)
//			goto LABEL_2;
//	}
//	return 0;
//}

////D2Common.0x6FDA81C0) --------------------------------------------------------
//int __fastcall sub_6FDA81C0(int a1, int a2)
//{
//	D2SeedStrc*v2; // esi@2
//	signed __int64 v3; // qax@2
//	int result; // eax@2
//
//	if (*(_DWORD *)(a2 + 225232))
//	{
//		v2 = *(D2SeedStrc**)(a2 + 225236);
//		v3 = v2->nHighSeed + 1791398085i64 * v2->nLowSeed;
//		*(_QWORD *)&v2->nLowSeed = v3;
//		result = ((unsigned __int8)*(_DWORD *)(a1 + 16)
//				  + (unsigned __int8)**(_DWORD **)(a1 + 12)
//				  + (unsigned __int8)dword_6FDD1CE0[v3 & 31]) & 7;
//		*(_DWORD *)(a1 + 16) = result;
//	}
//	else
//	{
//		result = ((unsigned __int8)*(_DWORD *)(a1 + 16) + (unsigned __int8)**(_DWORD **)(a1 + 12)) & 7;
//		*(_DWORD *)(a1 + 16) = result;
//	}
//	return result;
//}
