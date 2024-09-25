#include "Path/PathMisc.h"

#include "D2Collision.h"
#include "D2Dungeon.h"
#include "Path/Path.h"
#include "Path/IDAStar.h"
#include "Path/AStar.h"
#include "Path/PathUtil.h"
#include "Path/Step.h"
#include "Units/UnitRoom.h"
#include "Units/Units.h"
#include <Fog.h>
#include <utility>
#include <cmath>

struct D2UnkPathStrc
{
	int unk0x00;
	int unk0x04;
	int unk0x08;
};

struct D2UnkPathStrc2
{
	int8_t unk0x00;
	int8_t unk0x01;
	int8_t unk0x02;
};

//1.10f: D2Common.0x6FDD2158
//1.13c: D2Common.0x6FDDC320
static const D2UnkPathStrc stru_6FDD2158[25] =
{
	{ 5, 4, 6 },
	{ 4, 5, 6 },
	{ 4, 3, 5 },
	{ 4, 3, 2 },
	{ 3, 4, 2 },
	{ 6, 5, 4 },
	{ 5, 4, 6 },
	{ 4, 3, 5 },
	{ 3, 4, 2 },
	{ 2, 3, 4 },
	{ 6, 7, 5 },
	{ 6, 7, 5 },
	{ 6, 7, 5 },
	{ 2, 1, 3 },
	{ 2, 1, 3 },
	{ 6, 7, 0 },
	{ 7, 0, 6 },
	{ 0, 1, 7 },
	{ 1, 0, 2 },
	{ 2, 1, 0 },
	{ 7, 0, 6 },
	{ 0, 7, 6 },
	{ 0, 1, 7 },
	{ 0, 1, 2 },
	{ 1, 0, 2 },
};

//1.10f: D2Common.0x6FDD2288
//1.13c: D2Common.0x6FDDC450
static const D2UnkPathStrc2 byte_6FDD2288[25] =
{
	{ 4, 6, -1 },
	{ 4, 6, -1 },
	{ 4, 2, 6 },
	{ 4, 2, -1 },
	{ 4, 2, -1 },
	{ 6, 4, -1 },
	{ 4, 6, -1 },
	{ 4, 2, 6 },
	{ 4, 2, -1 },
	{ 2, 4, -1 },
	{ 6, 0, 4 },
	{ 6, 0, 4 },
	{ 6, 0, 4 },
	{ 2, 0, 4 },
	{ 2, 0, 4 },
	{ 6, 0, -1 },
	{ 0, 6, -1 },
	{ 0, 2, 6 },
	{ 2, 0, -1 },
	{ 2, 0, -1 },
	{ 0, 6, -1 },
	{ 0, 6, -1 },
	{ 0, 2, 6 },
	{ 0, 2, -1 },
	{ 0, 2, -1 },
};

// 1.10f: D2Common.0x6FDD1DA0
// 1.10f: D2Common.0x6FDD2118
// 1.13c: D2Common.0x6FDDC5A0
// Seems to be kind of inlined everywhere
const D2CoordStrc gatDirectionToOffset[8] = {
	{ 1, 0},
	{ 1, 1},
	{ 0, 1},
	{ -1, 1},
	{ -1, 0},
	{ -1, -1},
	{ 0, -1},
	{ 1, -1},
};

//D2Common.0x6FDAA720
int __fastcall sub_6FDAA720(D2PathInfoStrc* pPathInfo)
{
	D2DynamicPathStrc* pDynamicPath = pPathInfo->pDynamicPath;
	int nbPathPoints = pDynamicPath->dwPathPoints;
	if (pPathInfo->nDistMax > 0)
	{
		const int nDirectionOffset = pDynamicPath->nDirOffset;
		const D2PathPointStrc tTargetCoord = pPathInfo->tTargetCoord;
		int nPrevDirection = PATH_DIR_NULL;
		D2PathPointStrc tLastSegmentEndCoord = pPathInfo->tStartCoord;
		bool bSegmentEndAlreadyAdded = false;
		
		D2PathPointStrc tCurCoords = tLastSegmentEndCoord;
		int nCurDistance;
		for(nCurDistance = 0;nCurDistance < pPathInfo->nDistMax; nCurDistance++)
		{
			// We reached the destination, stop here
			if (tCurCoords == tTargetCoord)
				break;
			bSegmentEndAlreadyAdded = false;
			int nDirections[3];
			PATH_GetDirections_6FDAB790(nDirections, tLastSegmentEndCoord, tTargetCoord);
			nDirections[0] = ((uint8_t)nDirectionOffset + (uint8_t)nDirections[0]) % 8;
			nDirections[1] = ((uint8_t)nDirectionOffset + (uint8_t)nDirections[1]) % 8;
			nDirections[2] = ((uint8_t)nDirectionOffset + (uint8_t)nDirections[2]) % 8;
			int nDirection;
			if (!sub_6FDAA880(pPathInfo, nDirections, tLastSegmentEndCoord, &nDirection)
				// If doing a 180, stop
				|| ((nDirection + 4) % 8) == nPrevDirection
				)
			{
				break;
			}
			tCurCoords.X += gatDirectionToOffset[nDirection].nX;
			tCurCoords.Y += gatDirectionToOffset[nDirection].nY;
			if (nDirection != nPrevDirection)
			{
				pDynamicPath->PathPoints[nbPathPoints++] = tLastSegmentEndCoord;
				bSegmentEndAlreadyAdded = true;
			}
			tLastSegmentEndCoord = tCurCoords;
			nPrevDirection = nDirection;
		}
		if(!bSegmentEndAlreadyAdded && nCurDistance != 0)
			pDynamicPath->PathPoints[nbPathPoints++] = tLastSegmentEndCoord;
	}
	pDynamicPath->dwPathPoints = nbPathPoints;
	return nbPathPoints;
}

//1.00:  D2Common.0x1005B6D0
//1.10f: D2Common.0x6FDAA880
BOOL __fastcall sub_6FDAA880(D2PathInfoStrc* pPathInfo, int* pTestDir, D2PathPointStrc pPoint, int* pDirection)
{
	D2UnitStrc* pUnit = pPathInfo->pDynamicPath->pUnit;

	D2_ASSERT(pUnit && (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER));

	if (!COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pStartRoom, (pPoint.X + gatDirectionToOffset[pTestDir[0]].nX), (pPoint.Y + gatDirectionToOffset[pTestDir[0]].nY), pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask))
	{
		*pDirection = pTestDir[0];
		return TRUE;
	}

	if (!COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pStartRoom, (pPoint.X + gatDirectionToOffset[pTestDir[1]].nX), (pPoint.Y + gatDirectionToOffset[pTestDir[1]].nY), pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask))
	{
		*pDirection = pTestDir[1];
		return TRUE;
	}

	D2_ASSERT(pTestDir[2] != PATH_DIR_NULL);

	if (!COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pStartRoom, (pPoint.X + gatDirectionToOffset[pTestDir[2]].nX), (pPoint.Y + gatDirectionToOffset[pTestDir[2]].nY), pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask))
	{
		*pDirection = pTestDir[2];
		return TRUE;
	}

	return FALSE;
}

// Very similar to D2Common.0x6FDAA880, except it doesn't assert on pTestDir[2] != PATH_DIR_NULL, simply checks it.
// It also accumulates positions.
BOOL __fastcall PATH_CheckTestDirForCollisions(const D2PathInfoStrc* pPathInfo, const int* pTestDir, D2PathPointStrc* pPoints, int* pFreeOfCollisionDirIndex)
{
	D2UnitStrc* pUnit = pPathInfo->pDynamicPath->pUnit;

	D2_ASSERT(pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER);

	pPoints[0].X += gatDirectionToOffset[pTestDir[0]].nX;
	pPoints[0].Y += gatDirectionToOffset[pTestDir[0]].nY;
	if (!COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pStartRoom, pPoints[0].X, pPoints[0].Y, pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask))
	{
		*pFreeOfCollisionDirIndex = 0;
		return TRUE;
	}

	pPoints[1].X += gatDirectionToOffset[pTestDir[1]].nX;
	pPoints[1].Y += gatDirectionToOffset[pTestDir[1]].nY;
	if (!COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pStartRoom, pPoints[1].X, pPoints[1].Y, pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask))
	{
		*pFreeOfCollisionDirIndex = 1;
		return TRUE;
	}

	if (pTestDir[2] != PATH_DIR_NULL)
	{
		pPoints[2].X += gatDirectionToOffset[pTestDir[2]].nX;
		pPoints[2].Y += gatDirectionToOffset[pTestDir[2]].nY;
		if (!COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pStartRoom, pPoints[2].X, pPoints[2].Y, pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask))
		{
			*pFreeOfCollisionDirIndex = 2;
			return TRUE;
		}
	}

	return FALSE;
}

//1.10f: Inlined
//1.13c: D2Common.0x6FD85E00
BOOL PATH_PreparePathTargetForPathUpdate(D2PathInfoStrc* pPathInfo)
{
	int pTestDir[3];

	D2DynamicPathStrc* pPath = pPathInfo->pDynamicPath;
	D2_ASSERT(!(pPath->dwFlags & PATH_MISSILE_MASK));

	D2PathPointStrc aTestDirPathPoints[3] = { pPathInfo->tTargetCoord, pPathInfo->tTargetCoord, pPathInfo->tTargetCoord };

	sub_6FDAB7D0(pTestDir, 0 /*unused*/, pPathInfo->tTargetCoord, pPathInfo->tStartCoord);
	while (1)
	{
		if (pPathInfo->tStartCoord == aTestDirPathPoints[0])
		{
			return 0;
		}

		int nFreeOfCollisionDirectionIndex = -1;
		if (PATH_CheckTestDirForCollisions(pPathInfo, pTestDir, aTestDirPathPoints, &nFreeOfCollisionDirectionIndex))
		{
			pPathInfo->tTargetCoord = aTestDirPathPoints[nFreeOfCollisionDirectionIndex];
			if (pPathInfo->tStartCoord == aTestDirPathPoints[0])
			{
				return 0;
			}
			if (pPath->pUnit && pPath->pUnit->dwUnitType == UNIT_PLAYER)
			{
				PATH_FindValidTargetCoordsByMovingOrthogonally(pPathInfo);
			}
			return 1;
		}
		pTestDir[0] = byte_6FDD2288[sub_6FDAB6A0(aTestDirPathPoints[0], pPathInfo->tStartCoord)].unk0x00;
	}
}


//1.00:  D2Common.0x1005B890
//1.10f: D2Common.0x6FDAA9F0
int __fastcall PATH_Toward_6FDAA9F0(D2PathInfoStrc *pPathInfo)
{
	D2DynamicPathStrc* pPath = pPathInfo->pDynamicPath;
	pPath->dwCurrentPointIdx = 0;
	pPath->dwPathPoints = 0;
	if (pPath->nDirOffset)
		return sub_6FDAA720(pPathInfo);
	D2PathPointStrc nNextPoint = pPathInfo->tTargetCoord;
	pPath->PathPoints[0] = nNextPoint;

	if(!PATH_CheckCollisionsToNextPosition(pPath, &nNextPoint) && sub_6FDABA50(nNextPoint, pPathInfo->tTargetCoord) > pPathInfo->field_14)
	{
		D2PathPointStrc tPoint = pPathInfo->tStartCoord;
		D2PathPointStrc tCurCoords = pPathInfo->tStartCoord;
		if (pPathInfo->tStartCoord != nNextPoint)
		{
			pPath->PathPoints[pPath->dwPathPoints++] = nNextPoint;
			tPoint = nNextPoint;
			tCurCoords = nNextPoint;
		}
		nNextPoint = pPathInfo->tTargetCoord;
		int nPrevDir = PATH_DIR_NULL;
		int nNewPointsCount = pPath->dwPathPoints;
		BOOL bNotStraightLine = FALSE;
		int nCurDist = 0;
		if (pPathInfo->nDistMax > 0)
		{
			D2PathPointStrc* pPathPointsEnd = &pPath->PathPoints[pPath->dwPathPoints];
			do
			{
				if (tCurCoords == nNextPoint)
					break;
				bNotStraightLine = FALSE;
				int aTestDir[3];
				PATH_GetDirections_6FDAB790(aTestDir, tPoint, nNextPoint);
				int nDirection;
				if (!sub_6FDAA880(pPathInfo, aTestDir, tPoint, &nDirection) || (((uint8_t)nDirection - 4) & 7) == nPrevDir)
					goto LABEL_20;
				tCurCoords.X += gatDirectionToOffset[nDirection].nX;
				tCurCoords.Y += gatDirectionToOffset[nDirection].nY;
				if (nDirection != nPrevDir)
				{
					if (tPoint != pPathInfo->tStartCoord)
					{
						*pPathPointsEnd = tPoint;
						++nNewPointsCount;
						++pPathPointsEnd;
					}
					bNotStraightLine = TRUE;
				}
				tPoint = tCurCoords;
				nPrevDir = nDirection;
				++nCurDist;
			} while (nCurDist < pPathInfo->nDistMax);
			if (bNotStraightLine)
				goto LABEL_22;
		LABEL_20:
			if (nCurDist)
				pPath->PathPoints[nNewPointsCount++] = tPoint;
		}
	LABEL_22:
		pPath->dwPathPoints = nNewPointsCount;
	}
	else
	{
		pPath->PathPoints[pPath->dwPathPoints] = nNextPoint;
		pPath->dwPathPoints++;
	}
	return pPath->dwPathPoints;
}

//1.00:  D2Common.0x1005BB30
//1.10f: D2Common.0x6FDAABF0
//1.13c: D2Common.0x6FDB7630
BOOL __fastcall PATH_CheckCollisionsToNextPosition(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc* pGameCoord)
{
	D2PathPointStrc pPoint = {};

	D2_ASSERT(pDynamicPath->pUnit && (pDynamicPath->pUnit->dwUnitType == UNIT_PLAYER || pDynamicPath->pUnit->dwUnitType == UNIT_MONSTER));

	D2_ASSERT(pDynamicPath->dwPathType != PATHTYPE_MISSILE);
	D2_ASSERT(pGameCoord->X != 0);
	D2_ASSERT(pGameCoord->Y != 0);

	if (!pDynamicPath->dwVelocity)
	{
		pGameCoord->X = pDynamicPath->tGameCoords.wPosX;
		pGameCoord->Y = pDynamicPath->tGameCoords.wPosY;
		return FALSE;
	}

	PATH_ComputeVelocityAndDirectionVectorsToNextPoint(pDynamicPath, 1, 1);

	if (pDynamicPath->dwVelocity)
	{
		pPoint.X = pDynamicPath->tGameCoords.wPosX;
		pPoint.Y = pDynamicPath->tGameCoords.wPosY;
		return PATH_RayTrace(pDynamicPath, pGameCoord, pPoint);
	}
	else
	{
		pGameCoord->X = pDynamicPath->tGameCoords.wPosX;
		pGameCoord->Y = pDynamicPath->tGameCoords.wPosY;
		return FALSE;
	}
}

//1.00:  D2Common.0x1005BCA0
//1.10f: D2Common.0x6FDAAD10
//1.13c: D2Common.0x6FDB6C20
BOOL __fastcall PATH_RayTrace(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc* pPathDestination, D2PathPointStrc tStartCoord)
{
	const uint32_t nCollisionPattern = pDynamicPath->dwCollisionPattern;
	const uint32_t nMoveTestCollisionMask = pDynamicPath->nMoveTestCollisionMask;
	D2ActiveRoomStrc* pPathRoom = pDynamicPath->pRoom;
	D2PathPointStrc tTargetPoint = *pPathDestination;

	D2PathPointStrc tLastPointWithoutCollision = tStartCoord;
	D2PathPointStrc tCurPoint = tStartCoord;
	const int nDeltaX = tTargetPoint.X - tStartCoord.X;
	const int nDeltaY = tTargetPoint.Y - tStartCoord.Y;
	const int nPointsX = std::abs(nDeltaX) + 1;
	const int nPointsY = std::abs(nDeltaY) + 1;

	if (nPointsX > nPointsY)
	{
		if (nDeltaX == 0)
		{
			if (tStartCoord.Y != tTargetPoint.Y)
			{
				const int nYIncrement = 2 * int(nDeltaY >= 0) - 1;
				int nCurY = tStartCoord.Y;
				while (nCurY != tTargetPoint.Y)
				{
					nCurY += nYIncrement;
					tCurPoint.Y = nCurY;
					if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
						tStartCoord.X, nCurY,
						nCollisionPattern, nMoveTestCollisionMask))
					{
						*pPathDestination = tLastPointWithoutCollision;
						return FALSE;
					}
					tLastPointWithoutCollision = tCurPoint;
				}
				return TRUE;
			}
		}
		else // (nDeltaX != 0)
		{
			if (tStartCoord.X != tTargetPoint.X)
			{
				const int nXIncrement = 2 * int(nDeltaX >= 0) - 1;
				const int nYIncrement = 2 * int(nDeltaY >= 0) - 1;
				int nYOffset = nPointsY;
				while (1)
				{
					tCurPoint.X += nXIncrement;
					if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
						tCurPoint.X, tCurPoint.Y,
						nCollisionPattern, nMoveTestCollisionMask))
					{
						*pPathDestination = tLastPointWithoutCollision;
						return FALSE;
					}
					nYOffset += nPointsY;
					if (nYOffset >= nPointsX)
					{
						nYOffset -= nPointsX;
						tCurPoint.Y += nYIncrement;
						if (nYOffset > 0)
						{
							if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
								tCurPoint.X, tCurPoint.Y,
								nCollisionPattern, nMoveTestCollisionMask))
							{
								*pPathDestination = tLastPointWithoutCollision;
								return FALSE;
							}
						}
					}
					tLastPointWithoutCollision = tCurPoint;
					if (tCurPoint.X == tTargetPoint.X)
						return TRUE;
				}
			}
		}
		return TRUE;
	}
	
	if (nPointsX >= nPointsY)
	{
		const int nXIncrement = 2 * (nDeltaX >= 0) - 1;
		const int nYIncrement = 2 * (nDeltaY >= 0) - 1;
		if (tStartCoord.X == tTargetPoint.X)
			return TRUE;
		
		tCurPoint.X = tStartCoord.X;
		while (tCurPoint.X != tTargetPoint.X)
		{
			tCurPoint.Y += nYIncrement;
			tCurPoint.X += nXIncrement;
			if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
				tCurPoint.X, tCurPoint.Y,
				nCollisionPattern, nMoveTestCollisionMask))
			{
				*pPathDestination = tLastPointWithoutCollision;
				return FALSE;
			}
			tLastPointWithoutCollision = tCurPoint;
		}
		return TRUE;
	}
	if (nDeltaY == 0)
	{
		const int nXIncrement = 2 * (nDeltaX >= 0) - 1;
		if (tStartCoord.X == tTargetPoint.X)
			return TRUE;
		tCurPoint.X = tStartCoord.X;
		while (tCurPoint.X != tTargetPoint.X)
		{
			tCurPoint.X += nXIncrement;
			if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
				tCurPoint.X, tStartCoord.Y,
				nCollisionPattern, nMoveTestCollisionMask))
			{
				*pPathDestination = tLastPointWithoutCollision;
				return FALSE;
			}
			tLastPointWithoutCollision = tCurPoint;
		}
		return TRUE;
	}
	else // nDeltaY != 0
	{
		const int nXIncrement = 2 * (nDeltaX >= 0) - 1;
		const int nYIncrement = 2 * (nDeltaY >= 0) - 1;
		if (tStartCoord.Y == tTargetPoint.Y)
			return TRUE;

		int nEndOffsetX = nPointsX;
		while (tCurPoint.Y != tTargetPoint.Y)
		{
			tCurPoint.Y += nYIncrement;
			if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
				tCurPoint.X, tCurPoint.Y,
				nCollisionPattern, nMoveTestCollisionMask))
			{
				*pPathDestination = tLastPointWithoutCollision;
				return 0;
			}

			nEndOffsetX += nPointsX;
			if (nEndOffsetX >= nPointsY)
			{
				nEndOffsetX -= nPointsY;
				tCurPoint.X += nXIncrement;
				if (nEndOffsetX > 0)
				{
					if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
						tCurPoint.X, tCurPoint.Y,
						nCollisionPattern, nMoveTestCollisionMask))
					{
						*pPathDestination = tLastPointWithoutCollision;
						return 0;
					}
				}
			}
			tLastPointWithoutCollision = tCurPoint;
		}
		return TRUE;
	}
}

//D2Common.0x6FDAB0B0
int __fastcall sub_6FDAB0B0(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;
	return PATH_Toward_6FDAA9F0(pPathInfo);
}

//D2Common.0x6FDAB0C0
int __fastcall PATH_BackupTurn_Compute(D2PathInfoStrc* pPathInfo)
{
	int result = 0;
	int v5 = 0;

	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;

	pPathInfo->pDynamicPath->nDirOffset = -4;

	result = sub_6FDAA720(pPathInfo);
	if (!result)
	{
		return 0;
	}

	pPathInfo->pDynamicPath->dwPathPoints = result;

	pPathInfo->pDynamicPath->nDirOffset = -2;

	v5 = sub_6FDAA720(pPathInfo) - result;
	if (!v5)
	{
		pPathInfo->pDynamicPath->nDirOffset = 2;

		v5 = sub_6FDAA720(pPathInfo) - result;
	}

	pPathInfo->pDynamicPath->nDirOffset = -4;

	return result + v5;
}

//1.10f: D2Common.0x6FDAB130
//1.13c: D2Common.0x6FDB7AD0
int __fastcall PATH_Straight_Compute(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;
	pPathInfo->pDynamicPath->dwPathPoints = 0;

	int nPathPoints = PATH_Toward_6FDAA9F0(pPathInfo);
	if (nPathPoints)
	{
		D2PathPointStrc tPathLastPoint = pPathInfo->pDynamicPath->PathPoints[nPathPoints - 1];
		if (sub_6FDABA50(tPathLastPoint, pPathInfo->tTargetCoord) <= pPathInfo->field_14 // If heuristic distance is too small
			&& tPathLastPoint != pPathInfo->tStartCoord)								 // and we couldn't reach the target
		{
			return nPathPoints;															 // Then stop here
		}
	}

	
	// If distance is short enough, try to go around obstacles.
	const int nMaxDist = 18;
	const int nMaxDistSquared = nMaxDist * nMaxDist;

	const int nDiffX = std::abs(pPathInfo->tStartCoord.X - pPathInfo->tTargetCoord.X);
	const int nDiffY = std::abs(pPathInfo->tStartCoord.Y - pPathInfo->tTargetCoord.Y);
	const int nSquaredDistance = nDiffX * nDiffX + nDiffY * nDiffY;

	if (nSquaredDistance <= nMaxDistSquared)
	{
		if (int nAStarPathPoints = PATH_AStar_ComputePath(pPathInfo))
		{
			return nAStarPathPoints;
		}
		else if(nPathPoints) // Recompute the previous path result if a path was found
		{
			nPathPoints = PATH_Toward_6FDAA9F0(pPathInfo);
		}
	}
	return nPathPoints;
}

//1.10f: D2Common.0x6FDAB1E0
//1.13c: D2Common.0x6FDB77F0
int __fastcall PATH_Leap_6FDAB1E0(D2PathInfoStrc* pPathInfo)
{
	D2DynamicPathStrc* pDynamicPath = pPathInfo->pDynamicPath;
	pDynamicPath->dwCurrentPointIdx = 0;
	D2PathPointStrc pGameCoord = pPathInfo->tTargetCoord;
	pDynamicPath->PathPoints[0] = pGameCoord;
	PATH_CheckCollisionsToNextPosition(pDynamicPath, &pGameCoord);
	if (pPathInfo->tStartCoord == pGameCoord)
		return 0;

	pDynamicPath->PathPoints[0] = pGameCoord;
	pDynamicPath->dwPathPoints = 1;
	return pDynamicPath->dwPathPoints;
}

//D2Common.0x6FDAB240
int __fastcall PATH_Knockback_Client(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;
	pPathInfo->pDynamicPath->PathPoints[0] = pPathInfo->tTargetCoord;
	PATH_CheckCollisionsToNextPosition(pPathInfo->pDynamicPath, &pPathInfo->pDynamicPath->PathPoints[0]);
	return 1;
}

//D2Common.0x6FDAB270
int __fastcall PATH_Knockback_Server(D2PathInfoStrc* pPathInfo)
{
	D2DynamicPathStrc* pDynamicPath = pPathInfo->pDynamicPath;
	int nHalfDist = (pDynamicPath->nDist >> 1) + 1;
	D2PathPointStrc tFinalCoords;
	if (D2UnitStrc* pTargetUnit = pDynamicPath->pTargetUnit)
	{
		tFinalCoords = pPathInfo->tStartCoord;

		const int nTargetUnitX = UNITS_GetXPosition(pTargetUnit);
		const int nTargetUnitY = UNITS_GetYPosition(pTargetUnit);
		int nDeltaX = pPathInfo->tStartCoord.X - nTargetUnitX;
		int nDeltaY = pPathInfo->tStartCoord.Y - nTargetUnitY;
		if (const int nMaxAbsDelta = std::max(std::abs(nDeltaX), std::abs(nDeltaY)))
		{
			nDeltaX = (nDeltaX * nHalfDist) / nMaxAbsDelta;
			nDeltaY = (nDeltaY * nHalfDist) / nMaxAbsDelta;
		}
		tFinalCoords.X += nDeltaX;
		tFinalCoords.Y += nDeltaY;
	}
	else
	{
		tFinalCoords = pPathInfo->tTargetCoord;
	}
	pDynamicPath->PathPoints[0] = tFinalCoords;
	PATH_RayTrace(pDynamicPath, &tFinalCoords, pPathInfo->tStartCoord);
	D2COMMON_10170_PathSetTargetPos(pDynamicPath, tFinalCoords.X, tFinalCoords.Y);
	if (tFinalCoords == pPathInfo->tStartCoord)
	{
		return 0;
	}
	pDynamicPath->PathPoints[0] = tFinalCoords;
	pDynamicPath->dwPathPoints = 1;
	return pDynamicPath->dwPathPoints;
}

//D2Common.0x6FDAB3C0
signed int __fastcall PATH_ComputePathBlessedHammer_6FDAB3C0(D2DynamicPathStrc* pDynamicPath)
{
	const uint32_t dwOriginPrecisionX = pDynamicPath->tGameCoords.dwPrecisionX;
	const uint32_t dwOriginPrecisionY = pDynamicPath->tGameCoords.dwPrecisionY;
	D2PathPointStrc previousPoint = { PATH_FromFP16(dwOriginPrecisionX) , PATH_FromFP16(dwOriginPrecisionY) };

	int nAngleRadians_512 = 0;
	int nDistanceToOrigin = 0;
	
	// Looks like we keep one additional slot for the last point ?
	// Could be an error in the original game
	const int nbPointsToGenerate = D2DynamicPathStrc::MAXPATHLEN - 1;
	for (int curPointIdx = 0; curPointIdx < nbPointsToGenerate; curPointIdx++)
	{
		nAngleRadians_512 += (512 / 32); // 32 points to make a full circle
		nDistanceToOrigin += 9600; // Fixed point, around 0.1
		const float fDistanceToOrigin = (float)nDistanceToOrigin;
		D2PathPointStrc newPoint;
		newPoint.X = PATH_FromFP16(dwOriginPrecisionX + (__int64)(FOG_10083_Cos_LUT(nAngleRadians_512) * fDistanceToOrigin));
		newPoint.Y = PATH_FromFP16(dwOriginPrecisionY + (__int64)(FOG_10084_Sin_LUT(nAngleRadians_512) * fDistanceToOrigin));
		// If we have a new point, use it
		if (newPoint != previousPoint)
		{
			pDynamicPath->PathPoints[curPointIdx] = newPoint;
			previousPoint = newPoint;
		}
		else // Try again at next position
		{
			curPointIdx--;
		}
	}
	pDynamicPath->dwPathPoints = nbPointsToGenerate;
	return nbPointsToGenerate;
}

static DWORD getChargedBoltDirOffset(uint64_t i)
{
	// Note the discontinuity in the sequence. Fixing it is actually a *bad* idea as the bolts will spread less.
	int dirOffsetSequence[32] = {
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1,
		-1, /*0,*/ 1,
	};
	return dirOffsetSequence[i & 31];
}

//D2Common.0x6FDAB4A0
int __fastcall PATH_ComputePathChargedBolt_6FDAB4A0(D2DynamicPathStrc* pDynamicPath, D2SeedStrc* pSeed)
{
	D2PathPointStrc nPrevPoint;
	nPrevPoint.X = pDynamicPath->tGameCoords.wPosX;
	nPrevPoint.Y = pDynamicPath->tGameCoords.wPosY;

	int v16[3];
	PATH_GetDirections_6FDAB790(v16, nPrevPoint, pDynamicPath->tTargetCoord);

	int nMaxPathPoints = pDynamicPath->nDistMax >> 1;
	for (int nCurPointIdx = 0; nCurPointIdx < nMaxPathPoints; nCurPointIdx++)
	{
		pDynamicPath->PathPoints[nCurPointIdx] = nPrevPoint;

		char nMainDirection = v16[0];
		DWORD nDir = getChargedBoltDirOffset(SEED_RollRandomNumber(pSeed));
		nDir = (nMainDirection + (char)nDir) % ARRAY_SIZE(gatDirectionToOffset);
		nPrevPoint.X += 2 * gatDirectionToOffset[nDir].nX;
		nPrevPoint.Y += 2 * gatDirectionToOffset[nDir].nY;
	}
	pDynamicPath->PathPoints[nMaxPathPoints] = nPrevPoint;
	pDynamicPath->dwPathPoints = nMaxPathPoints + 1;
	return pDynamicPath->dwPathPoints;
}

//D2Common.0x6FDAB610
int __fastcall sub_6FDAB610(int nX1, int nY1, int nX2, int nY2)
{
	int nAbsDiffX = 0;
	int nAbsDiffY = 0;
	int nDiffX = 0;
	int nDiffY = 0;

	nDiffX = nX2 - nX1;
	nDiffY = nY2 - nY1;

	nAbsDiffX = nDiffX;
	nAbsDiffY = nDiffY;

	if (nAbsDiffX < 0)
	{
		nAbsDiffX = -nAbsDiffX;
	}

	if (nAbsDiffY < 0)
	{
		nAbsDiffY = -nAbsDiffY;
	}

	if (nAbsDiffX < 2 * nAbsDiffY)
	{
		if (nAbsDiffY >= 2 * nAbsDiffX)
		{
			if (nDiffX < 0)
			{
				if (nDiffY < -1)
				{
					return 5;
				}
				else if (nDiffY > 1)
				{
					nDiffY = 2;
				}

				return nDiffY + 7;
			}

			nDiffX &= 1;
		}
	}
	else
	{
		if (nDiffY >= 0)
		{
			nDiffY &= 1;
		}
		else
		{
			nDiffY = -1;
		}
	}

	if (nDiffX < -1)
	{
		nDiffX = -2;
	}
	else if (nDiffX > 1)
	{
		nDiffX = 2;
	}

	if (nDiffY < -1)
	{
		return 5 * nDiffX + 10;
	}
	else if (nDiffY > 1)
	{
		nDiffY = 2;
	}

	return nDiffY + 5 * nDiffX + 12;
}

//1.10f: D2Common.0x6FDAB6A0
//1.13c: D2Common.0x6FD8E190
int __stdcall sub_6FDAB6A0(D2PathPointStrc pPoint1, D2PathPointStrc pPoint2)
{
	return sub_6FDAB610(pPoint1.X, pPoint1.Y, pPoint2.X, pPoint2.Y);
}

//D2Common.0x6FDAB750
int __fastcall sub_6FDAB750(int nX1, int nY1, int nX2, int nY2)
{
	return stru_6FDD2158[sub_6FDAB610(nX1, nY1, nX2, nY2)].unk0x00;
}

//1.10f: D2Common.0x6FDAB770
//1.13c: D2Common.0x6FD8E5F0 (Inlined everywhere, but not stripped)
int __stdcall sub_6FDAB770(D2PathPointStrc pPoint1, D2PathPointStrc pPoint2)
{
	return stru_6FDD2158[sub_6FDAB6A0(pPoint1, pPoint2)].unk0x00;
}

//D2Common.0x6FDAB790
void __fastcall PATH_GetDirections_6FDAB790(int* pTestDir, D2PathPointStrc pPoint1, D2PathPointStrc pPoint2)
{
	int nIndex = sub_6FDAB6A0(pPoint1, pPoint2);

	pTestDir[0] = stru_6FDD2158[nIndex].unk0x00;
	pTestDir[1] = stru_6FDD2158[nIndex].unk0x04;
	pTestDir[2] = stru_6FDD2158[nIndex].unk0x08;
}

//1.00:  D2Common.0x1005D100
//1.10f: D2Common.0x6FDAB7D0
//1.13c: Inlined
//Should be __thiscall but we have to use __fastcall, hence nUnused
void __fastcall sub_6FDAB7D0(int* pTestDir, int nUnused, D2PathPointStrc pPoint1, D2PathPointStrc pPoint2)
{
	D2_MAYBE_UNUSED(nUnused);
	int nIndex = sub_6FDAB6A0(pPoint1, pPoint2);

	pTestDir[0] = byte_6FDD2288[nIndex].unk0x00;
	pTestDir[1] = byte_6FDD2288[nIndex].unk0x01;
	pTestDir[2] = byte_6FDD2288[nIndex].unk0x02;
}

//D2Common.0x6FDABA50
int __stdcall sub_6FDABA50(D2PathPointStrc pPoint1, D2PathPointStrc pPoint2)
{
	static const int dword_6FDD22D8[] =
	{
		-1, -1, -1, 0, 2, 4, 6, 8,
		-1, -1, 0, 1, 2, 4, 6, 8,
		-1, 0, 0, 2, 3, 5, 7, 8,
		0, 1, 2, 2, 4, 5, 7, 8,
		2, 2, 3, 4, 5, 6, 7, 9,
		4, 4, 5, 5, 6, 7, 8, 9,
		6, 6, 7, 7, 7, 8, 10, 10,
		8, 8, 8, 8, 9, 9, 10, 11
	};

	int nDiffX = 0;
	int nDiffY = 0;
	int nResult = 0;

	nDiffX = pPoint1.X - pPoint2.X;
	if (pPoint1.X - pPoint2.X < 0)
	{
		nDiffX = -nDiffX;
	}

	nDiffY = pPoint1.Y - pPoint2.Y;
	if (pPoint1.Y - pPoint2.Y < 0)
	{
		nDiffY = -nDiffY;
	}

	if (nDiffX >= 8 || nDiffY >= 8)
	{
		if (nDiffX <= nDiffY)
		{
			return nDiffX + 2 * nDiffY;
		}
		else
		{
			return nDiffY + 2 * nDiffX;
		}
	}
	else
	{
		nResult = dword_6FDD22D8[nDiffX + 8 * nDiffY];
		if (nResult >= 0)
		{
			return nResult + 1;
		}
		else
		{
			return 0;
		}
	}
}


//D2Common.0x6FDAC170
int __fastcall PATH_SimplifyToLines_6FDAC170(D2PathPointStrc* pOutPathPoints, D2PathPointStrc* pInputPoints, D2PathPointStrc tStartCoord, signed int nbTempPoints)
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
			else if (nbPointsInLine > 0 || prevDeltaX == deltaX || prevDeltaY == deltaY)
			{
				nbPointsInLine = 0;
				pOutPathPoints[nbOutPoints++] = pInputPoints[nCurrentPointIdx];
			}
			else
			{
				nbPointsInLine = 1;
				deltaX = -2;
			}
			prevDeltaY = deltaY;
			prevDeltaX = deltaX;
		}
		if (nCurrentPointIdx < nbTempPoints)
			pOutPathPoints[nbOutPoints++] = pInputPoints[nCurrentPointIdx];
	}
	else if (nbTempPoints == 1)
	{
		*pOutPathPoints = *pInputPoints;
		return 1;
	}
	return 0;
}

//D2Common.0x6FDAC700 (#10215)
int __stdcall PATH_ComputeDirection(int nX1, int nY1, int nX2, int nY2)
{
	D2CoordStrc pCoords = {};
	int nDirection = 0;

	PATH_GetDirectionVector(&pCoords, &nDirection, PATH_ToFP16Center(nX1), PATH_ToFP16Center(nY1), PATH_ToFP16Center(nX2), PATH_ToFP16Center(nY2));

	return nDirection;
}

//D2Common.0x6FDAC760
int __stdcall PATH_ComputeDirectionFromPreciseCoords_6FDAC760(DWORD dwStartPrecisionX, DWORD dwStartPrecisionY, DWORD dwTargetPrecisionX, DWORD dwTargetPrecisionY)
{
	D2CoordStrc pCoords = {};
	int nDirection = 0;

	PATH_GetDirectionVector(&pCoords, &nDirection, dwStartPrecisionX, dwStartPrecisionY, dwTargetPrecisionX, dwTargetPrecisionY);

	return nDirection;
}

//1.00:  D2Common.0x1005E9C0
//1.10f: D2Common.0x6FDAC790
//1.13c: D2Common.0x6FD5D240
void __fastcall PATH_ComputeVelocityAndDirectionVectorsToNextPoint(D2DynamicPathStrc* pPath, BOOL bNormalizeDirectionIfSamePos, BOOL bForceDirectionNormalization)
{
	const DWORD dwPrecisionX = pPath->tGameCoords.dwPrecisionX;
	const DWORD dwPrecisionY = pPath->tGameCoords.dwPrecisionY;
	DWORD nPointFP16X = PATH_ToFP16Center(pPath->PathPoints[pPath->dwCurrentPointIdx].X);
	DWORD nPointFP16Y = PATH_ToFP16Center(pPath->PathPoints[pPath->dwCurrentPointIdx].Y);
	// Remove duplicates
	while (nPointFP16X == dwPrecisionX && nPointFP16Y == dwPrecisionY)
	{
		if (pPath->dwCurrentPointIdx++ < (pPath->dwPathPoints - 1))
		{
			nPointFP16X = PATH_ToFP16Center(pPath->PathPoints[pPath->dwCurrentPointIdx].X);
			nPointFP16Y = PATH_ToFP16Center(pPath->PathPoints[pPath->dwCurrentPointIdx].Y);
		}
		else
		{
			pPath->tDirectionVector.nX = 0;
			pPath->tDirectionVector.nY = 0;
			pPath->tVelocityVector.nX = 0;
			pPath->tVelocityVector.nY = 0;
			pPath->dwVelocity = 0;
			pPath->dwCurrentPointIdx = pPath->dwPathPoints;
			return;
		}
	}

	D2CoordStrc tDirectionVector;
	int nDirection;
	PATH_GetDirectionVector( 
		&tDirectionVector, &nDirection,
		dwPrecisionX, dwPrecisionY,
		nPointFP16X, nPointFP16Y
	);

	if ((pPath->dwFlags & PATH_UNKNOWN_FLAG_0x00200) != 0)
		nDirection = PATH_NormalizeDirection(nDirection - 32);

	pPath->tDirectionVector.nX = tDirectionVector.nX;
	pPath->tDirectionVector.nY = tDirectionVector.nY;
	pPath->tVelocityVector.nX = (tDirectionVector.nX * pPath->dwVelocity) >> 8;
	pPath->tVelocityVector.nY = (tDirectionVector.nY * pPath->dwVelocity) >> 8;

	bool nextPosInSamePoint = 
		PATH_FromFP16(nPointFP16X) == PATH_FromFP16(dwPrecisionX + pPath->tVelocityVector.nX)
		&& PATH_FromFP16(nPointFP16Y) == PATH_FromFP16(dwPrecisionY + pPath->tVelocityVector.nY);
	
	if (bForceDirectionNormalization || nextPosInSamePoint && bNormalizeDirectionIfSamePos)
	{
		sub_6FDA9720(pPath, nDirection);
	}
}

//1.10f: D2Common.0x6FDAC8F0 (#10236)
//1.13c: D2Common.0x6FD5D820 (#10831)
//TODO: Find a name
int __stdcall D2Common_10236(D2UnitStrc* pUnit, int a2)
{
	D2DynamicPathStrc* pDynamicPath = NULL;
	int nResult = 0;

	pDynamicPath = pUnit->pDynamicPath;
	if (!pDynamicPath)
	{
		return 0;
	}

	if (!(pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00010))
	{
		if (pUnit->dwUnitType == UNIT_MONSTER && !PATH_GetDistance(pUnit->pDynamicPath))
		{
			return 0;
		}

		UNITROOM_RefreshUnit(pUnit);
		pUnit->dwFlags |= UNITFLAG_DOUPDATE;

		PATH_AddToDistance(pDynamicPath, -pDynamicPath->dwCurrentPointIdx);
	}

	if (pDynamicPath->dwPathType != PATHTYPE_TOWARD && pDynamicPath->dwPathType != PATHTYPE_TOWARD_FINISH && pDynamicPath->dwPathType != PATHTYPE_WALL_FOLLOW)
	{
		return D2Common_10142(pDynamicPath, pUnit, 0);
	}

	if (a2)
	{
		pDynamicPath->dwPathType = PATHTYPE_TOWARD_FINISH;
	}
	else
	{
		pDynamicPath->dwPathType = PATHTYPE_TOWARD;
		pDynamicPath->tTargetCoord.X = pDynamicPath->tFinalTargetCoord.X;
		pDynamicPath->tTargetCoord.Y = pDynamicPath->tFinalTargetCoord.Y;
	}

	nResult = D2Common_10142(pDynamicPath, pUnit, 0);
	if (!nResult)
	{
		pDynamicPath->dwPathType = PATHTYPE_WALL_FOLLOW;
		nResult = D2Common_10142(pDynamicPath, pUnit, 0);
	}

	return nResult;
}

//1.10f: Inlined
//1.13c: D2Common.0x6FD5CEB0
void sub_6FD5CEB0(D2DynamicPathStrc* pDynamicPath, int nBaseVelocity)
{
	if (nBaseVelocity <= 0)
	{
		nBaseVelocity = 1024;
	}
	// Actually assumed to be deceleration?
	if (int nAcceleration = pDynamicPath->dwAcceleration)
	{
		pDynamicPath->dwAccelerationsCounter++;
		if (pDynamicPath->dwAccelerationsCounter >= 5)
		{
			const int nOldVelocity = pDynamicPath->dwMaxVelocity;
			pDynamicPath->dwVelocity = nAcceleration + pDynamicPath->dwVelocity;
			if (pDynamicPath->dwVelocity >= nOldVelocity)
			{
				pDynamicPath->dwVelocity = nOldVelocity;
				pDynamicPath->dwAcceleration = 0;
			}
			else
			{
				if (pDynamicPath->dwVelocity < 0)
				{
					pDynamicPath->dwVelocity = 0;
				}
			}
			pDynamicPath->dwAccelerationsCounter = 0;
		}
	}
	const int nVelocityMagnitude = (signed int)(nBaseVelocity * pDynamicPath->dwVelocity) >> 6;
	pDynamicPath->tVelocityVector.nX = (nVelocityMagnitude * pDynamicPath->tDirectionVector.nX) >> 12;
	pDynamicPath->tVelocityVector.nY = (nVelocityMagnitude * pDynamicPath->tDirectionVector.nY) >> 12;
}

//1.10f: Inlined
//1.13c: D2Common.0x6FD5DB70
BOOL sub_6FD5DB70(D2DynamicPathStrc* pDynamicPath)
{
	if ((pDynamicPath->dwPathType == PATHTYPE_MON_CIRCLE_CW || pDynamicPath->dwPathType == PATHTYPE_MON_CIRCLE_CCW)
		&& pDynamicPath->dwCurrentPointIdx >= pDynamicPath->dwPathPoints)
	{
		return TRUE;
	}

	D2UnitStrc* pTargetUnit = pDynamicPath->pTargetUnit;
	if (!pTargetUnit)
	{
		if (pDynamicPath->dwCurrentPointIdx >= pDynamicPath->dwPathPoints
			&& pDynamicPath->tGameCoords.ToPathPoint() != pDynamicPath->tFinalTargetCoord)
		{
			return D2Common_10236(pDynamicPath->pUnit, FALSE);
		}
		return TRUE;
	}
	if (D2Common_10399(pTargetUnit, pDynamicPath->pUnit) <= (int)pDynamicPath->nStepNum)
	{
		return FALSE;
	}

	D2PathPointStrc a3;
	sub_6FDAB940(&a3, pDynamicPath);

	if (pTargetUnit)
	{
		if (pTargetUnit->dwUnitType == UNIT_PLAYER || pTargetUnit->dwUnitType == UNIT_MONSTER)
		{
			const int nAbsDiffX = std::abs(pDynamicPath->tPrevTargetCoord.X - a3.X);
			const int nAbsDiffY = std::abs(pDynamicPath->tPrevTargetCoord.Y - a3.Y);
			if (nAbsDiffX <= 5 && nAbsDiffY <= 5)
			{
				if (pDynamicPath->dwCurrentPointIdx < pDynamicPath->dwPathPoints
					|| pDynamicPath->tFinalTargetCoord == pDynamicPath->tGameCoords.ToPathPoint() )
				{
					return TRUE;
				}
			}
		}
	}
	return D2Common_10236(pDynamicPath->pUnit, TRUE);
}

//1.00:  D2Common.0x1005EEB0 (#10223)
//1.10f: D2Common.0x6FDAC9A0 (#10226)
//1.13c: D2Common.0x6FD5E230 (#10342)
BOOL __stdcall D2Common_10226(D2UnitStrc* pUnit, signed int a2)
{
	D2DynamicPathStrc* pDynamicPath = pUnit->pDynamicPath;
	if (!pDynamicPath)
	{
		return FALSE;
	}

	pDynamicPath->dwFlags &= ~PATH_UNKNOWN_FLAG_0x00008;
	if ((pDynamicPath->dwFlags & PATH_MISSILE_MASK) != 0)
	{
		pDynamicPath->nCollidedWithMask = 0;
	}

	if ((pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00020) != 0 && pDynamicPath->dwPathPoints > 0 && pDynamicPath->dwVelocity != 0)
	{
#ifdef D2_VERSION_113C // Optimisation added in 1.13c ? Not compatible with 1.10f
	// TODO name
		auto IsUnitSomething = [](D2UnitStrc* pUnit, D2DynamicPathStrc* pDynamicPath) -> bool {
			switch (pUnit->dwUnitType)
			{
			case UNIT_PLAYER:
				return (pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00010) == 0;
			case UNIT_MONSTER:
				return (pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00010) == 0
					&& (pUnit->dwFlags & UNITFLAG_ISASYNC) == 0;
			default:
				return false;
			}
			};
		if (IsUnitSomething(pUnit, pDynamicPath))
#endif
		{
			if(((pDynamicPath->dwFlags & PATH_MISSILE_MASK) != 0 || sub_6FD5DB70(pDynamicPath))
				&& pDynamicPath->dwCurrentPointIdx < pDynamicPath->dwPathPoints)
			{
				sub_6FD5CEB0(pDynamicPath, a2);
				if (pDynamicPath->tVelocityVector.nX || pDynamicPath->tVelocityVector.nY)
				{
					D2UnitStrc* nUnused = nullptr;
					D2_MAYBE_UNUSED(nUnused);
					D2FP32_16 tCoord;
					sub_6FDACEC0(pDynamicPath, &tCoord, &nUnused);
					PATH_RecacheRoomAtCoordIfNeeded(pDynamicPath, nullptr, tCoord.dwPrecisionX, tCoord.dwPrecisionY);
					if (pDynamicPath->dwPathType != PATHTYPE_MISSILE && pDynamicPath->dwCurrentPointIdx < pDynamicPath->dwPathPoints)
					{
						PATH_ComputeVelocityAndDirectionVectorsToNextPoint(pDynamicPath,TRUE, TRUE);
					}
					if (pDynamicPath->dwCurrentPointIdx < pDynamicPath->dwPathPoints)
					{
						return TRUE;
					}
				}

			}
		}
	}

	PATH_ResetMovement(pDynamicPath);
	return FALSE;
}

//D2Common.0x6FDAD530 (#10227)
//TODO: Find a name
BOOL __stdcall D2Common_10227(D2UnitStrc* pUnit)
{
	D2DynamicPathStrc*  pDynamicPath = pUnit->pDynamicPath;

	if (pDynamicPath)
	{
		if (pDynamicPath->pTargetUnit && D2Common_10399(pDynamicPath->pTargetUnit, pUnit) <= pDynamicPath->nStepNum)
		{
			return TRUE;
		}
		else
		{
			return pDynamicPath->tGameCoords.wPosX == pDynamicPath->tTargetCoord.X && pDynamicPath->tGameCoords.wPosY == pDynamicPath->tTargetCoord.Y;
		}
	}
	else
	{
		return TRUE;
	}
}

//D2Common.0x6FDAD590 (#10229)
BOOL __stdcall D2Common_10229(D2DynamicPathStrc *pDynamicPath, D2UnitStrc *pUnit, D2ActiveRoomStrc *pDestRoom, uint32_t nDestX, uint32_t nDestY)
{
	D2PathPointStrc tCoords = { uint16_t(nDestX), uint16_t(nDestY) };
	if (sub_6FDAD5E0(pDynamicPath, pDestRoom, tCoords))
	{
		pDynamicPath->dwPathPoints = 0;
		return TRUE;
	}
	return FALSE;
}

//1.10f: D2Common.0x6FDADA20
//1.13c: D2Common.0x6FD5D760
void __fastcall PATH_RecacheRoom(D2DynamicPathStrc* pDynamicPath, D2ActiveRoomStrc* pHintRoom)
{
	if (pDynamicPath->pRoom && DungeonTestRoomGame(pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY))
	{
		// Early out, room is already up to date.
		return;
	}

	// Try to find room using previous path room
	D2ActiveRoomStrc* pRoomAtLocation = DUNGEON_GetRoomAtPosition(pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY);
	if (!pRoomAtLocation)
	{
		// Try looking for the room using the hint room
		pRoomAtLocation = DUNGEON_GetRoomAtPosition(pHintRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY);
	}

	if (pRoomAtLocation)
	{
		PATHMISC_SetRoom(pDynamicPath, pRoomAtLocation);
	}
	else if((pDynamicPath->dwFlags & PATH_MISSILE_MASK) != 0)
	{
		pDynamicPath->dwPathPoints = 0;
	}
}
