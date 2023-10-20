#include "Path/PathWF.h"
#include "Path/PathMisc.h"
#include <D2Collision.h>
#include <Units/Units.h>

#include <algorithm>

struct D2PathMovStrc
{
	int nDirectionIndex;
	int nPoints; //nLoopLen in 1.10f
	D2PathPointStrc tCurrentCoord;
	D2PathPointStrc tTargetCoord;
	BOOL bReachedDeadEnd; // Only used for debugging ?
	int* pPrevDirection;
	int* pNextDirection;
	int nLastSyncPointWithOtherPathIndex;
	BOOL bPathFinished;
	D2PathPointStrc pPoints[200];
};

// D2Common.0x6FD68400 (1.13C)
// Inlined (1.10f)
void __vectorcall PATH_ReplaceSubpathPoints(D2PathMovStrc* pMov, D2PathPointStrc* pPoints, int* pSubPathStartIdx, int nSubPathLastIdx, int* nMaxIndex)
{
	// negative if shrinking, positive is growing
	const int nPointsDiff = (*pSubPathStartIdx - nSubPathLastIdx) + (pMov->nPoints - 1);
	const int nLastReplacedPointIdx = nSubPathLastIdx + nPointsDiff;

	memmove(&pPoints[nLastReplacedPointIdx + 1], &pPoints[nSubPathLastIdx + 1], ((*nMaxIndex) - nSubPathLastIdx - 1) * sizeof(D2PathPointStrc));
	memcpy(&pPoints[*pSubPathStartIdx], &pMov->pPoints[0], pMov->nPoints * sizeof(D2PathPointStrc));

	(*nMaxIndex) += nPointsDiff;
	(*pSubPathStartIdx) = nLastReplacedPointIdx + 1;
}

//1.10f: 0x6FDE5198
//1.13C: 0x6FDD2458
static int gnCellOffsetsToDirections[] =
{
	/*(-1,-1)*/ 7, /*( 0,-1)*/ 0, /*( 1,-1)*/ 1,
	/*(-1, 0)*/ 6, /*( 0, 0)*/-1, /*( 1, 0)*/ 2,
	/*(-1, 1)*/ 5, /*( 0, 1)*/ 4, /*( 1 ,1)*/ 3
};

//1.10f: D2Common.0x6FDD23D8
//1.13C: D2Common.0x6fddca38
static const D2CoordStrc gnDirectionsToAdjacentCellOffsets[16] =
{
	{0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1},
	{0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}
};

//1.10f: Inlined
//1.13C: D2Common.0x6FD686D0
BOOL __vectorcall PATH_FindNonCollidingTargetPoint(D2PathInfoStrc* pInfo, D2PathMovStrc* pMov)
{
	D2_ASSERT(pInfo && pMov && pInfo->pDynamicPath->pUnit && (pInfo->pDynamicPath->pUnit->dwUnitType == UNIT_PLAYER || pInfo->pDynamicPath->pUnit->dwUnitType == UNIT_MONSTER));

	auto HasCollisionForNewTarget = [&] {
		pMov->tTargetCoord.X = gnDirectionsToAdjacentCellOffsets[pMov->nDirectionIndex].nX + (pMov->tCurrentCoord).X;
		pMov->tTargetCoord.Y = gnDirectionsToAdjacentCellOffsets[pMov->nDirectionIndex].nY + (pMov->tCurrentCoord).Y;
		return COLLISION_CheckAnyCollisionWithPattern(pInfo->pStartRoom, pMov->tTargetCoord.X, pMov->tTargetCoord.Y, pInfo->nCollisionPattern, (uint16_t)pInfo->nCollisionMask);
	};

	pMov->bReachedDeadEnd = FALSE;
	if (!HasCollisionForNewTarget()) { return TRUE; }

	pMov->nDirectionIndex = pMov->pNextDirection[pMov->nDirectionIndex];
	if (!HasCollisionForNewTarget()) { return TRUE; }

	pMov->nDirectionIndex = pMov->pNextDirection[pMov->nDirectionIndex];
	if (!HasCollisionForNewTarget()) { return TRUE; }

	pMov->nDirectionIndex = pMov->pNextDirection[pMov->nDirectionIndex];
	// We circled back to the original direction.
	pMov->bReachedDeadEnd = TRUE;
	return !HasCollisionForNewTarget();
}


// D2Common.0x6FDABAC0 (1.10f)
// D2Common.0x6FD68830 (1.13C)
// The general idea of the algorithm is to evaluate two paths: One on the right and another on the left of the line between start and target points.
// We try to advance both paths at the same pace (one point by one point), and both evaluate paths to avoid but stay close to obstacles.
// This effectively makes the unit slide along walls
// If the path would be too long, then it goes straight to the first collision along the line.
int __fastcall PATH_FindSubpathWithoutObstacles(D2PathInfoStrc* pInfo, D2PathPointStrc tSubPathStart, D2PathPointStrc* pPathPoints, int* pSubPathStartIdx, int* nMaxIndex, int nMaxLength, int nMajorDirection)
{
	//1.10f: D2Common.0x6FDD2480
	//1.13C: D2Common.0x6FDE5158
	static int gnPreviousDirection1[16] =
	{
		6,6,0,0,2,2,4,4,
		6,6,0,0,2,2,4,4,
	};

	//1.10f: D2Common.0x6FDD24C0
	//1.13C: D2Common.0x6FDE5118
	static int gnPreviousDirection2[16] =
	{
		// Opposite direction of gnDirIndexCache1List
		2,2,4,4,6,6,0,0,
		2,2,4,4,6,6,0,0
	};

	//1.10f: D2Common.0x6FDD2500
	//1.13C: D2Common.0x6FDE50D8
	static int gnNextDirection1[16] =
	{
		2,2,4,4,6,6,0,0,
		2,2,4,4,6,6,0,0
	};

	//1.10f: D2Common.0x6FDD2540
	//1.13C: D2Common.0x6FDE5098
	static int gnNextDirection2[16] =
	{
		6,0,0,2,2,4,4,6,
		6,0,0,2,2,4,4,6
	};

	D2PathMovStrc tCache2 = {};
	D2PathMovStrc tCache1 = {};

	tCache1.pPrevDirection = gnPreviousDirection1;
	tCache1.pNextDirection = gnNextDirection1;

	tCache2.pPrevDirection = gnPreviousDirection2;
	tCache2.pNextDirection = gnNextDirection2;

	const int nDirX = pPathPoints[*pSubPathStartIdx].X - tSubPathStart.X;
	const int nDirY = pPathPoints[*pSubPathStartIdx].Y - tSubPathStart.Y;
	// Assume points are adjacent
	D2_ASSERT(nDirX >= -1 && nDirX <= 1);
	D2_ASSERT(nDirY >= -1 && nDirY <= 1);
	// Index between -1 and 7 included. -1 center, 0 top, increments clockwise
	const int nAdjacentCellIndex = gnCellOffsetsToDirections[4 + (nDirY * 3) + nDirX];
	tCache1.nDirectionIndex = nAdjacentCellIndex + 1;
	tCache2.nDirectionIndex = nAdjacentCellIndex + 7;

	tCache2.tCurrentCoord = tSubPathStart;
	tCache1.tCurrentCoord = tSubPathStart;

	D2PathMovStrc* pCurPath = &tCache1;
	D2PathMovStrc* pOtherPath = &tCache2;


	while (!(tCache1.bPathFinished || tCache2.bPathFinished))
	{
		if (!pCurPath->bPathFinished)
		{
			if (!PATH_FindNonCollidingTargetPoint(pInfo, pCurPath))
			{
				// Stop if we find no collision-free position
				pCurPath->bPathFinished = TRUE;
			}
			else // Add the new point to the path
			{
				if (pCurPath->nPoints > 0)
				{
					int nMajorDirectionDistance = 0;
					switch (nMajorDirection)
					{
					case 0:
					{
						nMajorDirectionDistance = tSubPathStart.Y - (pCurPath->tTargetCoord).Y;
						break;
					}
					case 1:
					{
						nMajorDirectionDistance = (pCurPath->tTargetCoord).X - tSubPathStart.X;
						break;
					}
					case 2:
					{
						nMajorDirectionDistance = (pCurPath->tTargetCoord).Y - tSubPathStart.Y;
						break;
					}
					case 3:
					{
						nMajorDirectionDistance = tSubPathStart.X - (pCurPath->tTargetCoord).X;
						break;
					}
					}

					if (nMajorDirectionDistance > 0)
					{
						// Early out if the suggest path can be copied inplace.
						// This will replace the subpath by a slight variation using the same number of points.
						// The same number of points implies that we always go in the same major direction.
						// For example:
						// 
						//		the following path    O-------______    
						//                                          ----X
						//		                              __
						//		could be replaced by  O-_  _--  -___   
						//		                         --         ----X

						const int nPointInMajorDirectionWithDistance = nMajorDirectionDistance - 1 + *pSubPathStartIdx;
						if (nPointInMajorDirectionWithDistance < *nMaxIndex && pCurPath->tTargetCoord == pPathPoints[nPointInMajorDirectionWithDistance])
						{
							D2_ASSERT(pCurPath->nPoints < nMaxLength);

							pCurPath->pPoints[pCurPath->nPoints] = pCurPath->tTargetCoord;
							pCurPath->nPoints++;

							pCurPath->pPoints[pCurPath->nPoints].X = 0;

							PATH_ReplaceSubpathPoints(pCurPath, pPathPoints, pSubPathStartIdx, nPointInMajorDirectionWithDistance, nMaxIndex);
							return pCurPath->nPoints;
						}
					}
				}

				if (pOtherPath->nPoints > 1)
				{
					// We had formed a loop with the other path
					if (pCurPath->nLastSyncPointWithOtherPathIndex > 0)
					{
						if (pCurPath->tTargetCoord == pOtherPath->pPoints[pCurPath->nLastSyncPointWithOtherPathIndex - 2])
						{
							// We started walking back along the other path!
							// This means we are effectively evaluating the same path in reverse order, which means there is no available path.
							// We stop here to avoid an infinite loop.
							pPathPoints[*pSubPathStartIdx].X = 0;
							*nMaxIndex = *pSubPathStartIdx;
							return 0;
						}
						else
						{
							// The two paths are diverging again, forget about the sync point.
							pCurPath->nLastSyncPointWithOtherPathIndex = 0;
						}
					}

					// We reached the same position as the other path, record the point index so that we can detect if we're walking back along the other path
					if (pCurPath->tTargetCoord == pOtherPath->tCurrentCoord)
					{
						pCurPath->nLastSyncPointWithOtherPathIndex = pOtherPath->nPoints;
					}
				}

				// We want to re-evaluate the current direction in the next iteration.
				pCurPath->nDirectionIndex = pCurPath->pPrevDirection[pCurPath->nDirectionIndex];

				pCurPath->pPoints[pCurPath->nPoints] = pCurPath->tTargetCoord;
				pCurPath->nPoints++;
				pCurPath->tCurrentCoord = pCurPath->tTargetCoord;
				
				
				if ((nMaxLength - *nMaxIndex - 1) <= pCurPath->nPoints)
				{
					pCurPath->bPathFinished = TRUE;
				}
			}
		}

		if (!pCurPath->nLastSyncPointWithOtherPathIndex)
		{
			std::swap(pCurPath, pOtherPath);
		}
	}

	if (nMaxLength > 80)
	{
		pPathPoints[*pSubPathStartIdx].X = 0;
		*nMaxIndex = *pSubPathStartIdx;
		return 0;
	}

	auto ComputeSquaredDistToTarget = [&](const D2PathMovStrc& rPath) {
		// Note: original game does not clamp the index, and would use memory from pCache1.bWithCollision, which would be set to 0
		// This means the distance would be the norm of tTargetCoord, which means we would have the bigger distance of the two paths
		// It makes more sense to use INT_MAX as it means we will never pick this path.
		if (rPath.nPoints > 0)
		{
			return rPath.pPoints[rPath.nPoints - 1].SquaredDistance(pInfo->tTargetCoord);
		}
		return INT_MAX;
	};

	const int nSquaredDist1 = ComputeSquaredDistToTarget(tCache1);
	const int nSquaredDist2 = ComputeSquaredDistToTarget(tCache2);
	const int nSquaredDistStartToTarget = pInfo->tStartCoord.SquaredDistance(pInfo->tTargetCoord);

	D2PathMovStrc* pBestPath;
	if (nSquaredDist1 < nSquaredDist2)
	{
		if (nSquaredDistStartToTarget < nSquaredDist1)
		{
			return 0;
		}
		pBestPath = &tCache1;
	}
	else
	{
		if (nSquaredDistStartToTarget < nSquaredDist2)
		{
			return 0;
		}
		pBestPath = &tCache2;
	}

	if (pBestPath->nPoints > 0)
	{
		memcpy(pPathPoints + *pSubPathStartIdx, pBestPath->pPoints, pBestPath->nPoints * sizeof(D2PathPointStrc));
	}

	int nNewSubPathStart = *pSubPathStartIdx + pBestPath->nPoints;
	*pSubPathStartIdx  = nNewSubPathStart;
	*nMaxIndex		   = nNewSubPathStart;
	pPathPoints[nNewSubPathStart].X = 0;

	return pCurPath->nPoints;
}

//D2Common.0x6FDAC170 (1.10f)
//D2Common.0x6FD68310 (1.13c)
int __fastcall PATH_SimplifyToLines(D2PathPointStrc* pOutPathPoints, D2PathPointStrc* pInputPoints, D2PathPointStrc tStartCoord, signed int nbTempPoints)
{
	if (nbTempPoints >= 2)
	{
		int nbOutPoints = 0;

		int prevDeltaX = pInputPoints->X - tStartCoord.X;
		int prevDeltaY = pInputPoints->Y - tStartCoord.Y;
		int nbPointsInLine = 0;
		int nCurrentPointIdx;
		for (nCurrentPointIdx = 0; nCurrentPointIdx < nbTempPoints - 1; ++nCurrentPointIdx)
		{
			int deltaX = pInputPoints[nCurrentPointIdx + 1].X - pInputPoints[nCurrentPointIdx].X;
			int deltaY = pInputPoints[nCurrentPointIdx + 1].Y - pInputPoints[nCurrentPointIdx].Y;
			if (deltaX == prevDeltaX && deltaY == prevDeltaY)
			{
				++nbPointsInLine;
			}
			else if (nbPointsInLine <= 0 && prevDeltaX != deltaX && prevDeltaY != deltaY)
			{
				// Force a new line for the next point, as deltaX can never be -2
				deltaX = -2;
				nbPointsInLine = 1;
			}
			else
			{
				// New line, output current line end point
				pOutPathPoints[nbOutPoints++] = pInputPoints[nCurrentPointIdx];
				nbPointsInLine = 0;
			}
			prevDeltaY = deltaY;
			prevDeltaX = deltaX;
		}
		if (nCurrentPointIdx < nbTempPoints)
		{
			pOutPathPoints[nbOutPoints++] = pInputPoints[nCurrentPointIdx];
		}
		return nbOutPoints;
	}
	else if (nbTempPoints == 1)
	{
		*pOutPathPoints = *pInputPoints;
		return 1;
	}
	return 0;
}

//1.10f: Inlined
//1.13C: D2Common.0x6FD684C0
static int PATH_BresenhamLine(D2PathPointStrc tStartPoint, D2PathPointStrc tTargetPoint, int nDistMax, int* nMajorDirection, D2PathPointStrc* pOutPoints)
{
	// We kind of "null terminate" the path by setting the last point X to 0
	// Hence why we compare to nDistMax - 1 to avoid buffer overflow
	int nMaxManhattanDistMinusOne = nDistMax - 1;
	*nMajorDirection = 0;

	int nAbsDiffX = tTargetPoint.X - tStartPoint.X;
	int nAbsDiffY = tTargetPoint.Y - tStartPoint.Y;
	int nStepX = 1;
	int nStepY = 1;
	if (nAbsDiffX < 0)
	{
		nAbsDiffX = -nAbsDiffX;
		nStepX = -1;
	}
	if (nAbsDiffY < 0)
	{
		nAbsDiffY = -nAbsDiffY;
		nStepY = -1;
	}
	
	int nbPoints = 0;
	if (nAbsDiffX == 0 && nAbsDiffY == 0) // Start == Target
	{
		return 0;
	}
	else if (nAbsDiffX == 0 && nAbsDiffY != 0) // Vertical line
	{
		*nMajorDirection = nStepY <= 0 ? 0 : 2;
		if (nAbsDiffY > nMaxManhattanDistMinusOne)
		{
			return 0;
		}
		else
		{
			int nY = tStartPoint.Y;
			do
			{
				nY += nStepY;
				pOutPoints[nbPoints].X = tStartPoint.X;
				pOutPoints[nbPoints].Y = nY;
				nbPoints++;
				--nAbsDiffY;
			} while (nAbsDiffY);
		}
	}
	else if (nAbsDiffX != 0 && nAbsDiffY == 0) // Horizontal line
	{
		*nMajorDirection = (nStepX <= 0) ? 3 : 1;
		if (nAbsDiffX > nMaxManhattanDistMinusOne)
		{
			return 0;
		}
		else
		{
			int nX = tStartPoint.X;
			do
			{
				nX += nStepX;
				pOutPoints[nbPoints].X = nX;
				pOutPoints[nbPoints].Y = tStartPoint.Y;
				nbPoints++;
				--nAbsDiffX;
			} while (nAbsDiffX);
		}
	}
	else if (nAbsDiffX < nAbsDiffY)
	{
		*nMajorDirection = nStepY <= 0 ? 0 : 2;
		if (nAbsDiffY > nMaxManhattanDistMinusOne)
		{
			return 0;
		}
		else
		{
			D2PathPointStrc tCurrentPoint = tStartPoint;
			int nDeviation = 0;
			int nRemainingStepsY = nAbsDiffY;
			do
			{
				nDeviation += nAbsDiffX;
				tCurrentPoint.Y += nStepY;
				if (nDeviation >= nAbsDiffY)
				{
					nDeviation -= nAbsDiffY;
					tCurrentPoint.X += nStepX;
				}
				pOutPoints[nbPoints] = tCurrentPoint;
				nbPoints++;
				nRemainingStepsY--;
			} while (nRemainingStepsY != 0);
		}
	}
	else // (nAbsDiffX >= nAbsDiffY)
	{
		*nMajorDirection = (nStepX <= 0) ? 3 : 1;
		if (nAbsDiffX > nMaxManhattanDistMinusOne)
		{
			return 0;
		}
		else
		{
			int nRemainingStepsX = nAbsDiffX;
			int nDeviation = 0;
			D2PathPointStrc tCurrentCoord = tStartPoint;
			do
			{
				nDeviation += nAbsDiffY;
				tCurrentCoord.X += nStepX;
				if (nDeviation >= nAbsDiffX)
				{
					nDeviation -= nAbsDiffX;
					tCurrentCoord.Y += nStepY;
				}
				pOutPoints[nbPoints] = tCurrentCoord;
				nbPoints++;
				nRemainingStepsX--;
			} while (nRemainingStepsX != 0);
		}
	}
	
	// Somehow used to mark end of path
	pOutPoints[nbPoints].X = 0;
	return nbPoints;
}

// Author: Araksson
// D2Common.0x6FDAC270 (1.10f) 
// D2Common.0x6FD68C40 (1.13C)
int __fastcall PATH_ComputePathOrSlideAlongObstacles(D2PathInfoStrc* ptPathInfo)
{
	D2_ASSERT(ptPathInfo->pDynamicPath->pUnit && (ptPathInfo->pDynamicPath->pUnit->dwUnitType == UNIT_PLAYER || ptPathInfo->pDynamicPath->pUnit->dwUnitType == UNIT_MONSTER));

	int nDist = ptPathInfo->pDynamicPath->nDistMax;
	if (ptPathInfo->pDynamicPath->pTargetUnit && ptPathInfo->pDynamicPath->nDistMax < 40)
	{
		nDist = 40;
	}

	int nMajorDirection = 0;
	D2PathPointStrc aPathPoints[D2DynamicPathStrc::MAXPATHLEN] = {}; // Could actually be D2DynamicPathStrc::MAXPATHLEN + 1 ? Why is 6FDABAC0 using 80 as maxlen?!
	D2PathPointStrc pStartPoint = ptPathInfo->tStartCoord;
	int nMovementPoints = PATH_BresenhamLine(pStartPoint, ptPathInfo->tTargetCoord, nDist, &nMajorDirection, aPathPoints);
	if (nMovementPoints > 2)
	{
		D2PathPointStrc tSubPathStartPoint = pStartPoint;
		for (int nSubPathStartIdx = 0; nSubPathStartIdx < nMovementPoints; nSubPathStartIdx++)
		{
			if (COLLISION_CheckAnyCollisionWithPattern(ptPathInfo->pStartRoom, aPathPoints[nSubPathStartIdx].X, aPathPoints[nSubPathStartIdx].Y, ptPathInfo->nCollisionPattern, (uint16_t)ptPathInfo->nCollisionMask))
			{
				int nCacheIndex = PATH_FindSubpathWithoutObstacles(ptPathInfo, tSubPathStartPoint, aPathPoints, &nSubPathStartIdx, &nMovementPoints, nDist - nSubPathStartIdx, nMajorDirection);
				if (!nCacheIndex)
				{
					return PATH_SimplifyToLines(ptPathInfo->pDynamicPath->PathPoints, aPathPoints, pStartPoint, nSubPathStartIdx);
				}
			}

			tSubPathStartPoint = aPathPoints[nSubPathStartIdx];
		}

		if (nMovementPoints)
		{
			return PATH_SimplifyToLines(ptPathInfo->pDynamicPath->PathPoints, aPathPoints, pStartPoint, nMovementPoints);
		}
	}

	return 0;
}

