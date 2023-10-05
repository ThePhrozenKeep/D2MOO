#include "Path/PathMisc.h"

#include "D2Collision.h"
#include "D2Dungeon.h"
#include "Path/Path.h"
#include "Path/IDAStar.h"
#include "Path/FollowWall.h"
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
	char unk0x00;
	char unk0x01;
	char unk0x02;
};

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
		const int nDirectionOffset = pDynamicPath->dwUnitTypeRelated;
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
			nDirections[0] = (nDirectionOffset + nDirections[0]) % 8;
			nDirections[1] = (nDirectionOffset + nDirections[1]) % 8;
			nDirections[2] = (nDirectionOffset + nDirections[2]) % 8;
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
				sub_6FDA8FE0(pPathInfo);
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
	if (pPath->dwUnitTypeRelated)
		return sub_6FDAA720(pPathInfo);
	D2PathPointStrc nNextPoint = pPathInfo->tTargetCoord;
	pPath->PathPoints[0] = nNextPoint;
	if (PATH_CheckCollisionsToNextPosition(pPath, &nNextPoint))
	{
		pPath->PathPoints[pPath->dwPathPoints] = nNextPoint;
		pPath->dwPathPoints++;
	}
	else if (sub_6FDABA50(nNextPoint, pPathInfo->tTargetCoord) > pPathInfo->field_14)
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
	D2RoomStrc* pPathRoom = pDynamicPath->pRoom;
	D2PathPointStrc tTargetPoint = *pPathDestination;

	D2PathPointStrc tLastPointWithoutCollision = tStartCoord;
	D2PathPointStrc tCurPoint = tStartCoord;
	const int nDeltaX = tTargetPoint.X - tStartCoord.X;
	const int nDeltaY = tTargetPoint.Y - tStartCoord.Y;
	const int nAbsDeltaY = std::abs(nDeltaY);
	const int nPointsX = std::abs(nDeltaX) + 1;
	const int nPointsY = nAbsDeltaY + 1;

	if (nPointsX > nPointsY)
	{
		if (nDeltaX)
		{
			const int nXIncrement = 2 * int(nDeltaX >= 0) - 1;
			const int nYIncrement = 2 * int(nDeltaY >= 0) - 1;
			int nYOffset = nPointsY;
			if (tStartCoord.X != tTargetPoint.X)
			{
				while (1)
				{
					tCurPoint.X += nXIncrement;
					if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
						tCurPoint.X, tCurPoint.Y,
						nCollisionPattern, nMoveTestCollisionMask))
					{
						break;
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
								return 0;
							}
						}
					}
					tLastPointWithoutCollision = tCurPoint;
					if (tCurPoint.X == tTargetPoint.X)
						return 1;
				}
				*pPathDestination = tLastPointWithoutCollision;
				return 0;
			}
		}
		else
		{
			const int nYIncrement = 2 * int(nDeltaY >= 0) - 1;
			if (tStartCoord.Y != tTargetPoint.Y)
			{
				int nCurY = tStartCoord.Y;
				while (1)
				{
					nCurY += nYIncrement;
					tCurPoint.Y = nCurY;
					if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
						tStartCoord.X, nCurY,
						nCollisionPattern, nMoveTestCollisionMask))
					{
						break;
					}
					tLastPointWithoutCollision = tCurPoint;
					if (nCurY == tTargetPoint.Y)
						return 1;
				}
				*pPathDestination = tLastPointWithoutCollision;
				return 0;
			}
		}
		return 1;
	}
	if (nPointsX >= nPointsY)
	{
		const int nXIncrement = 2 * (nDeltaX >= 0) - 1;
		const int nYIncrement = 2 * (nDeltaY >= 0) - 1;
		if (tStartCoord.X == tTargetPoint.X)
			return 1;
		int nCurX = tStartCoord.X;
		while (1)
		{
			nCurX += nXIncrement;
			tCurPoint.Y += nYIncrement;
			tCurPoint.X = nCurX;
			if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
				nCurX, tCurPoint.Y,
				nCollisionPattern, nMoveTestCollisionMask))
			{
				break;
			}
			tLastPointWithoutCollision = tCurPoint;
			if (nCurX == tTargetPoint.X)
				return 1;
		}
		*pPathDestination = tLastPointWithoutCollision;
		return 0;
	}
	if (nDeltaY == 0)
	{
		const int nXIncrement = 2 * (nDeltaX >= 0) - 1;
		if (tStartCoord.X == tTargetPoint.X)
			return 1;
		int nCurX = tStartCoord.X;
		while (1)
		{
			nCurX += nXIncrement;
			tCurPoint.X = nCurX;
			if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
				nCurX, tStartCoord.Y,
				nCollisionPattern, nMoveTestCollisionMask))
			{
				break;
			}
			tLastPointWithoutCollision = tCurPoint;
			if (nCurX == tTargetPoint.X)
				return 1;
		}
		*pPathDestination = tLastPointWithoutCollision;
		return 0;
	}

	const int nXIncrement = 2 * (nDeltaX >= 0) - 1;
	const int nYIncrement = 2 * (nDeltaY >= 0) - 1;
	if (tStartCoord.Y == tTargetPoint.Y)
		return 1;


	int nEndOffsetX = nPointsX;
	while (1)
	{
		tCurPoint.Y += nYIncrement;
		if (COLLISION_CheckAnyCollisionWithPattern(pPathRoom,
			tCurPoint.X, tCurPoint.Y,
			nCollisionPattern, nMoveTestCollisionMask))
		{
			break;
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
		if (tCurPoint.Y == tTargetPoint.Y)
			return 1;
	}
	*pPathDestination = tLastPointWithoutCollision;
	return 0;
}

//D2Common.0x6FDAB0B0
int __fastcall sub_6FDAB0B0(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;
	return PATH_Toward_6FDAA9F0(pPathInfo);
}

//D2Common.0x6FDAB0C0
//TODO: result + v5
int __fastcall sub_6FDAB0C0(D2PathInfoStrc* pPathInfo)
{
	int result = 0;
	int v5 = 0;

	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;

	pPathInfo->pDynamicPath->dwUnitTypeRelated = -4;

	result = sub_6FDAA720(pPathInfo);
	if (!result)
	{
		return 0;
	}

	pPathInfo->pDynamicPath->dwPathPoints = result;

	pPathInfo->pDynamicPath->dwUnitTypeRelated = -2;

	v5 = sub_6FDAA720(pPathInfo) - result;
	if (!v5)
	{
		pPathInfo->pDynamicPath->dwUnitTypeRelated = 2;

		v5 = sub_6FDAA720(pPathInfo) - result;
	}

	pPathInfo->pDynamicPath->dwUnitTypeRelated = -4;

	return result + v5;
}

//1.10f: D2Common.0x6FDAB130
//1.13c: D2Common.0x6FDB7AD0
int __fastcall sub_6FDAB130(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;
	pPathInfo->pDynamicPath->dwPathPoints = 0;

	int nPathPoints = PATH_Toward_6FDAA9F0(pPathInfo);
	if (nPathPoints)
	{
		D2PathPointStrc tPathLastPoint = pPathInfo->pDynamicPath->PathPoints[nPathPoints - 1];
		if (sub_6FDABA50(tPathLastPoint, pPathInfo->tTargetCoord) <= pPathInfo->field_14 // If heuristic distance is too small
			|| tPathLastPoint != pPathInfo->tStartCoord)								 // Or we couldn't reach the target
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
		if (nPathPoints = PATH_FoWall_ComputePath(pPathInfo))
		{
			return nPathPoints;
		}
		else if (nPathPoints = PATH_Toward_6FDAA9F0(pPathInfo)) // Recompute the previous path result.
		{
			return nPathPoints;
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
int __fastcall PATH_Knockback_Client_6FDAB240(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;
	pPathInfo->pDynamicPath->PathPoints[0] = pPathInfo->tTargetCoord;
	PATH_CheckCollisionsToNextPosition(pPathInfo->pDynamicPath, &pPathInfo->pDynamicPath->PathPoints[0]);
	return 1;
}

//D2Common.0x6FDAB270
int __fastcall PATH_Knockback_Server_6FDAB270(D2PathInfoStrc* pPathInfo)
{
	// Belongs to PathWF.cpp
	UNIMPLEMENTED();
	return 0;
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
	PATH_GetDirections_6FDAB790(v16, nPrevPoint, pDynamicPath->SP1);

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

//D2Common.0x6FDAB770
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


//1.10f: D2Common.0x6FDAB890
//1.13c: D2Common.0x6FD8E080
// Should be in PathUtil.cpp
void __fastcall PATHUtil_AdvanceTowardsTarget_6FDAB890(D2DynamicPathStrc* ptPath)
{
	D2_ASSERT(ptPath);
	D2UnitStrc* ptTarget = ptPath->pTargetUnit;
	D2_ASSERT(ptTarget);
	uint8_t nDirection = PATH_GetDirection(ptTarget->pDynamicPath);
	int nSpeed = ptPath->dwSpeed;
	uint16_t v4 = (uint16_t)nSpeed << 8;
	const float nDistance = nSpeed;
	const uint16_t nAngleRad512 = (8 * nDirection - v4) & 0x1FF;
	ptPath->SP1.X += (int64_t)(FOG_10083_Cos_LUT(nAngleRad512) * nDistance);
	ptPath->SP1.Y += (int64_t)(FOG_10084_Sin_LUT(nAngleRad512) * nDistance);
}

//D2Common.0x6FDAB940
//Belongs to PathUtil.cpp
void __fastcall sub_6FDAB940(D2PathPointStrc* pOutPathPoint, D2DynamicPathStrc* ptPath)
{	
	D2_ASSERT(ptPath);
	D2_ASSERT(ptPath->pTargetUnit);
	D2UnitStrc* pTargetUnit = ptPath->pTargetUnit;
	D2C_UnitTypes dwTargetUnitType = (D2C_UnitTypes)pTargetUnit->dwUnitType;

	if (dwTargetUnitType == UNIT_OBJECT || dwTargetUnitType == UNIT_ITEM || dwTargetUnitType == UNIT_TILE)
	{
		pOutPathPoint->X = pTargetUnit->pStaticPath->tGameCoords.nX;
		pOutPathPoint->Y = pTargetUnit->pStaticPath->tGameCoords.nY;
	}
	else
	{
		if (D2DynamicPathStrc* pTargetPath = pTargetUnit->pDynamicPath)
		{
			pOutPathPoint->X = PATH_GetXPosition(pTargetPath);
			pOutPathPoint->Y = PATH_GetYPosition(pTargetPath);
		}
		else
		{
			*pOutPathPoint = { 0,0 };
		}
	}

	if (ptPath->dwSpeed)
	{
		PATHUtil_AdvanceTowardsTarget_6FDAB890(ptPath);
	}
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

//D2Common.0x6FDAC8F0 (#10236)
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

	if (pDynamicPath->dwPathType != PATHTYPE_TOWARD && pDynamicPath->dwPathType != PATHTYPE_MON_OTHER_2 && pDynamicPath->dwPathType != PATHTYPE_WF)
	{
		return D2Common_10142(pDynamicPath, pUnit, 0);
	}

	if (a2)
	{
		pDynamicPath->dwPathType = PATHTYPE_MON_OTHER_2;
	}
	else
	{
		pDynamicPath->dwPathType = PATHTYPE_TOWARD;
		pDynamicPath->SP1.X = pDynamicPath->SP3.X;
		pDynamicPath->SP1.Y = pDynamicPath->SP3.Y;
	}

	nResult = D2Common_10142(pDynamicPath, pUnit, 0);
	if (!nResult)
	{
		pDynamicPath->dwPathType = PATHTYPE_WF;
		nResult = D2Common_10142(pDynamicPath, pUnit, 0);
	}

	return nResult;
}

//1.10f: D2Common.0x1005EEB0 (#10223)
//1.10f: D2Common.0x6FDAC9A0 (#10226)
BOOL __stdcall D2Common_10226(D2UnitStrc* pUnit, signed int a2)
{
	D2DynamicPathStrc* pDynamicPath; // esi
	uint32_t v3; // edi
	uint32_t v4; // eax
	signed int v6; // eax
	D2UnitStrc* v7; // eax
	D2UnitStrc* v8; // edi
	int v9; // eax
	uint16_t v10; // bx
	uint16_t v11; // bp
	D2UnitStrc* v12; // eax
	int v13; // eax
	int v14; // eax
	int v15; // ecx
	signed int v16; // edx
	uint32_t v17; // eax
	signed int v18; // ecx
	signed int v19; // ebp
	signed int v20; // eax
	signed int v21; // eax
	int v22; // ecx
	int v23; // edx
	D2PathPointStrc pOutPathPoint; // [esp+10h] [ebp-10h] BYREF
	unsigned int a3[2]; // [esp+18h] [ebp-8h] BYREF

	pDynamicPath = pUnit->pDynamicPath;
	if (!pDynamicPath)
		return 0;
	v3 = pDynamicPath->dwFlags & ~PATH_UNKNOWN_FLAG_0x00008;
	pDynamicPath->dwFlags = v3;
	if ((v3 & PATH_MISSILE_MASK) != 0)
		pDynamicPath->nCollidedWithMask = 0;
	if ((v3 & 0x20) == 0 || (int)pDynamicPath->dwPathPoints <= 0 || !pDynamicPath->dwVelocity)
	{
		sub_6FDACC40(
			pDynamicPath,
			0,
			PATH_FP16FitToCenter(pDynamicPath->tGameCoords.dwPrecisionX),
			PATH_FP16FitToCenter(pDynamicPath->tGameCoords.dwPrecisionY));
		pDynamicPath->dwPathPoints = 0;
		pDynamicPath->dwCurrentPointIdx = 0;
		pDynamicPath->dwFlags = pDynamicPath->dwFlags & (D2PathFlags)~PATH_UNKNOWN_FLAG_0x00020;
		pDynamicPath->tVelocityVector.nX = 0;
		pDynamicPath->tVelocityVector.nY = 0;
		return 0;
	}
	if ((pDynamicPath->dwFlags & PATH_MISSILE_MASK) != 0)
		goto LABEL_34;
	v6 = pDynamicPath->dwPathType;
	if ((v6 == PATHTYPE_MON_CIRCLE_CW || v6 == PATHTYPE_MON_CIRCLE_CCW) && (signed int)pDynamicPath->dwCurrentPointIdx >= (signed int)pDynamicPath->dwPathPoints)
		goto LABEL_34;
	v7 = pDynamicPath->pTargetUnit;
	v8 = pDynamicPath->pUnit;
	if (!v7)
	{
		if ((signed int)pDynamicPath->dwCurrentPointIdx < (signed int)pDynamicPath->dwPathPoints
			|| pDynamicPath->tGameCoords.wPosX == pDynamicPath->SP3.X
			&& pDynamicPath->tGameCoords.wPosY == pDynamicPath->SP3.Y)
		{
			goto LABEL_34;
		}
		v9 = D2Common_10236(v8, 0);
		goto LABEL_31;
	}
	if (D2Common_10399(v7, pDynamicPath->pUnit) <= pDynamicPath->nStepNum)
	{
	LABEL_32:
		sub_6FDAD330(pDynamicPath);
		return 0;
	}
	v10 = pDynamicPath->tGameCoords.wPosX;
	v11 = pDynamicPath->tGameCoords.wPosY;
	sub_6FDAB940(&pOutPathPoint, pDynamicPath);
	v12 = pDynamicPath->pTargetUnit;
	if (v12)
	{
		v13 = v12->dwUnitType;
		if (v13 >= 0 && v13 <= 1)
		{
			v14 = pDynamicPath->SP2.X - pOutPathPoint.X;
			v15 = pDynamicPath->SP2.Y - pOutPathPoint.Y;
			if (v14 < 0)
				v14 = pOutPathPoint.X - pDynamicPath->SP2.X;
			if (v14 > 5)
				goto LABEL_30;
			if (v15 < 0)
				v15 = pOutPathPoint.Y - pDynamicPath->SP2.Y;
			if (v15 > 5)
				goto LABEL_30;
		}
	}
	if ((signed int)pDynamicPath->dwCurrentPointIdx >= (signed int)pDynamicPath->dwPathPoints
		&& (pDynamicPath->SP3.X != v10 || pDynamicPath->SP3.Y != v11))
	{
	LABEL_30:
		v9 = D2Common_10236(v8, 1);
	LABEL_31:
		if (!v9)
			goto LABEL_32;
	}
LABEL_34:
	if ((signed int)pDynamicPath->dwCurrentPointIdx >= (signed int)pDynamicPath->dwPathPoints)
		goto LABEL_32;
	v16 = a2;
	if (a2 <= 0)
		v16 = 1024;
	v17 = pDynamicPath->dwAcceleration;
	if (v17)
	{
		v18 = pDynamicPath->unk0x8C + 1;
		pDynamicPath->unk0x8C = v18;
		if (v18 >= 5)
		{
			v19 = v17 + pDynamicPath->dwVelocity;
			v20 = pDynamicPath->dwMaxVelocity;
			pDynamicPath->dwVelocity = v19;
			if (v19 <= v20)
			{
				if (v19 < 0)
					pDynamicPath->dwVelocity = 0;
			}
			else
			{
				pDynamicPath->dwVelocity = v20;
				pDynamicPath->dwAcceleration = 0;
			}
			pDynamicPath->unk0x8C = 0;
		}
	}
	v21 = (signed int)(v16 * pDynamicPath->dwVelocity) >> 6;
	v22 = (v21 * pDynamicPath->tDirectionVector.nX) >> 12;
	v23 = (v21 * pDynamicPath->tDirectionVector.nY) >> 12;
	pDynamicPath->tVelocityVector.nX = v22;
	pDynamicPath->tVelocityVector.nY = v23;
	if (!v22 && !v23)
		goto LABEL_32;
	sub_6FDACEC0(pDynamicPath, (D2FP32_16*)a3, &pUnit);
	sub_6FDACC40(pDynamicPath, 0, a3[0], a3[1]);
	if (pDynamicPath->dwPathType != PATHTYPE_MISSILE
		&& (signed int)pDynamicPath->dwCurrentPointIdx < (signed int)pDynamicPath->dwPathPoints)
	{
		PATH_ComputeVelocityAndDirectionVectorsToNextPoint(pDynamicPath, 1, 1);
	}
	if ((signed int)pDynamicPath->dwCurrentPointIdx >= (signed int)pDynamicPath->dwPathPoints)
		goto LABEL_32;
	return 1;
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
			return pDynamicPath->tGameCoords.wPosX == pDynamicPath->SP1.X && pDynamicPath->tGameCoords.wPosY == pDynamicPath->SP1.Y;
		}
	}
	else
	{
		return TRUE;
	}
}

//D2Common.0x6FDAD590 (#10229)
BOOL __stdcall D2Common_10229(D2DynamicPathStrc *pDynamicPath, D2UnitStrc *pUnit, D2RoomStrc *pDestRoom, uint32_t nDestX, uint32_t nDestY)
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
void __fastcall PATH_RecacheRoom(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pHintRoom)
{
	if (pDynamicPath->pRoom && DungeonTestRoomGame(pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY))
	{
		// Early out, room is already up to date.
		return;
	}

	// Try to find room using previous path room
	D2RoomStrc* pRoomAtLocation = DUNGEON_GetRoomAtPosition(pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY);
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
