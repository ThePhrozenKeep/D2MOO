#include "Path/PathMisc.h"

#include "D2Collision.h"
#include "D2Dungeon.h"
#include "Path/Path.h"
#include "Path/Path_IDAStar.h"
#include "Units/UnitRoom.h"
#include "Units/Units.h"
#include <Fog.h>
#include <utility>

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

// D2Common.0x6FDD2118
D2CoordStrc gatDirectionToOffset_6FDD2118[8] = {
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
		D2PathPointStrc tLastSegmentEndCoord = pPathInfo->pStartCoord;
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
			tCurCoords.X += gatDirectionToOffset_6FDD2118[nDirection].nX;
			tCurCoords.Y += gatDirectionToOffset_6FDD2118[nDirection].nY;
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

//D2Common.0x6FDAA880
BOOL __fastcall sub_6FDAA880(D2PathInfoStrc* pPathInfo, int* pTestDir, D2PathPointStrc pPoint, int* pDirection)
{
	D2UnitStrc* pUnit = pPathInfo->pDynamicPath->pUnit;

	D2_ASSERT(pUnit && (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER));

	if (!COLLISION_CheckMaskWithPattern2(pPathInfo->pRoom, (pPoint.X + gatDirectionToOffset_6FDD2118[pTestDir[0]].nX), (pPoint.Y + gatDirectionToOffset_6FDD2118[pTestDir[0]].nY), pPathInfo->nCollisionPattern, pPathInfo->nCollisionType))
	{
		*pDirection = pTestDir[0];
		return TRUE;
	}

	if (!COLLISION_CheckMaskWithPattern2(pPathInfo->pRoom, (pPoint.X + gatDirectionToOffset_6FDD2118[pTestDir[1]].nX), (pPoint.Y + gatDirectionToOffset_6FDD2118[pTestDir[1]].nY), pPathInfo->nCollisionPattern, pPathInfo->nCollisionType))
	{
		*pDirection = pTestDir[1];
		return TRUE;
	}

	D2_ASSERT(pTestDir[2] != PATH_DIR_NULL);

	if (!COLLISION_CheckMaskWithPattern2(pPathInfo->pRoom, (pPoint.X + gatDirectionToOffset_6FDD2118[pTestDir[2]].nX), (pPoint.Y + gatDirectionToOffset_6FDD2118[pTestDir[2]].nY), pPathInfo->nCollisionPattern, pPathInfo->nCollisionType))
	{
		*pDirection = pTestDir[2];
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDAA9F0
int __fastcall sub_6FDAA9F0(D2PathInfoStrc *pPathInfo)
{
	UNIMPLEMENTED();
	return 0;
}

//D2Common.0x6FDAABF0
BOOL __fastcall sub_6FDAABF0(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc* pGameCoord)
{
	D2PathPointStrc pPoint = {};

	D2_ASSERT(pDynamicPath->pUnit && (pDynamicPath->pUnit->dwUnitType == UNIT_PLAYER || pDynamicPath->pUnit->dwUnitType == UNIT_MONSTER));

	D2_ASSERT(pDynamicPath->dwPathType != PATHTYPE_MISSILE);
	D2_ASSERT(pGameCoord->X != 0);
	D2_ASSERT(pGameCoord->Y != 0);

	if (!pDynamicPath->dwVelocity)
	{
		pGameCoord->X = pDynamicPath->wPosX;
		pGameCoord->Y = pDynamicPath->wPosY;
		return FALSE;
	}

	sub_6FDAC790(pDynamicPath, 1, 1);

	if (pDynamicPath->dwVelocity)
	{
		pPoint.X = pDynamicPath->wPosX;
		pPoint.Y = pDynamicPath->wPosY;
		return sub_6FDAAD10(pDynamicPath, pGameCoord, pPoint);
	}
	else
	{
		pGameCoord->X = pDynamicPath->wPosX;
		pGameCoord->Y = pDynamicPath->wPosY;
		return FALSE;
	}
}

//D2Common.0x6FDAAD10
BOOL __fastcall sub_6FDAAD10(D2DynamicPathStrc* a1, D2PathPointStrc* a2, D2PathPointStrc a3)
{
	UNIMPLEMENTED();
}

//D2Common.0x6FDAB0B0
int __fastcall sub_6FDAB0B0(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;
	return sub_6FDAA9F0(pPathInfo);
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

//D2Common.6FDAB130
int __fastcall sub_6FDAB130(D2PathInfoStrc* pPathInfo)
{

	D2DynamicPathStrc* pDynamicPath = pPathInfo->pDynamicPath;
	pDynamicPath->dwCurrentPointIdx = 0;
	pDynamicPath->dwPathPoints = 0;
	int v3 = sub_6FDAA9F0(pPathInfo);
	
	D2PathPointStrc v4 = {0,0};

	if (v3 > 0)
	{
		v4 = pDynamicPath->PathPoints[v3 - 1];
		if (sub_6FDABA50(v4, pPathInfo->tTargetCoord) <= pPathInfo->field_14)
			return v3;
		else if (v4.X != pPathInfo->pStartCoord.X || v4.Y != pPathInfo->pStartCoord.Y)
			return v3;
	}
	int nDiffX = pPathInfo->pStartCoord.X - pPathInfo->tTargetCoord.X;
	if (nDiffX < 0)
		nDiffX = pPathInfo->tTargetCoord.X - pPathInfo->pStartCoord.X;;
	int nDiffY = pPathInfo->pStartCoord.Y - pPathInfo->tTargetCoord.Y;
	if (nDiffY < 0)
		nDiffY = pPathInfo->tTargetCoord.Y - pPathInfo->pStartCoord.Y;
	const int nSquaredDistance = nDiffX * nDiffX + nDiffY * nDiffY;
	const int nMaxDist = 18;
	const int nMaxDistSquared = nMaxDist * nMaxDist;
	if (nSquaredDistance <= nMaxDistSquared)
	{
		int result = sub_6FDA69E0(pPathInfo);
		if (result)
			return result;
		if (v3)
			v3 = sub_6FDAA9F0(pPathInfo);
	}
	return v3;
}

//D2Common.0x6FDAB1E0
int __fastcall sub_6FDAB1E0(D2PathInfoStrc* pPathInfo)
{
	D2DynamicPathStrc* pDynamicPath = pPathInfo->pDynamicPath;
	pDynamicPath->dwCurrentPointIdx = 0;
	D2PathPointStrc pGameCoord = pPathInfo->tTargetCoord;
	pDynamicPath->PathPoints[0] = pGameCoord;
	sub_6FDAABF0(pDynamicPath, &pGameCoord);
	if (pPathInfo->pStartCoord == pGameCoord)
		return 0;

	pDynamicPath->PathPoints[0] = pGameCoord;
	pDynamicPath->dwPathPoints = 1;
	return pDynamicPath->dwPathPoints;
}

//D2Common.0x6FDAB240
int __fastcall sub_6FDAB240(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;
	pPathInfo->pDynamicPath->PathPoints[0] = pPathInfo->tTargetCoord;
	sub_6FDAABF0(pPathInfo->pDynamicPath, &pPathInfo->pDynamicPath->PathPoints[0]);
	return 1;
}

//D2Common.0x6FDAB270
int __fastcall sub_6FDAB270(D2PathInfoStrc* pPathInfo)
{
	// Belongs to PathWF.cpp
	UNIMPLEMENTED();
	return 0;
}

//D2Common.0x6FDAB3C0
signed int __fastcall PATH_ComputePathBlessedHammer_6FDAB3C0(D2DynamicPathStrc* pDynamicPath)
{
	const uint32_t dwOriginPrecisionX = pDynamicPath->dwPrecisionX;
	const uint32_t dwOriginPrecisionY = pDynamicPath->dwPrecisionY;
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
	nPrevPoint.X = pDynamicPath->wPosX;
	nPrevPoint.Y = pDynamicPath->wPosY;

	int v16[3];
	PATH_GetDirections_6FDAB790(v16, nPrevPoint, pDynamicPath->SP1);

	int nMaxPathPoints = pDynamicPath->nDistMax >> 1;
	for (int nCurPointIdx = 0; nCurPointIdx < nMaxPathPoints; nCurPointIdx++)
	{
		pDynamicPath->PathPoints[nCurPointIdx] = nPrevPoint;

		char nMainDirection = v16[0];
		DWORD nDir = getChargedBoltDirOffset(SEED_RollRandomNumber(pSeed));
		nDir = (nMainDirection + (char)nDir) % _countof(gatDirectionToOffset_6FDD2118);
		nPrevPoint.X += 2 * gatDirectionToOffset_6FDD2118[nDir].nX;
		nPrevPoint.Y += 2 * gatDirectionToOffset_6FDD2118[nDir].nY;
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

//D2Common.0x6FDAB6A0
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

//D2Common.0x6FDAB7D0
void __fastcall sub_6FDAB7D0(int* pTestDir, int nUnused, D2PathPointStrc pPoint1, D2PathPointStrc pPoint2)
{
	int nIndex = sub_6FDAB6A0(pPoint1, pPoint2);

	pTestDir[0] = byte_6FDD2288[nIndex].unk0x00;
	pTestDir[1] = byte_6FDD2288[nIndex].unk0x01;
	pTestDir[2] = byte_6FDD2288[nIndex].unk0x02;
}

//D2Common.0x6FDAB810
void __fastcall sub_6FDAB810(int* a1, int* a2)
{
	if (*a1 < 0)
	{
		while (*a1 < -65536)
		{
			*a1 >>= 1;
			*a2 >>= 1;
		}
	}
	else
	{
		while (*a1 > 65536)
		{
			*a1 >>= 1;
			*a2 >>= 1;
		}
	}

	if (*a2 < 0)
	{
		while (*a2 < -65536)
		{
			*a1 >>= 1;
			*a2 >>= 1;
		}
	}
	else
	{
		while (*a2 > 65536)
		{
			*a1 >>= 1;
			*a2 >>= 1;
		}
	}
}

//D2Common.0x6FDAB890
// Should be in PathUtil.cpp
void __fastcall PATH_MoveForward_6FDAB890(D2DynamicPathStrc* ptPath)
{
	D2_ASSERT(ptPath);
	D2UnitStrc* ptTarget = ptPath->pTargetUnit;
	D2_ASSERT(ptTarget);
	uint8_t nDirection = PATH_GetDirection(ptTarget->pDynamicPath);
	int v6 = ptPath->unk0x68[0]; // Some kind of stepping distance or speed ?
	uint16_t v4 = (uint16_t)v6 << 8;
	const float nDistance = v6;
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
		pOutPathPoint->X = pTargetUnit->pStaticPath->nXPos;
		pOutPathPoint->Y = pTargetUnit->pStaticPath->nYPos;
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

	if (ptPath->unk0x68[0])
	{
		uint8_t nDir = PATH_GetDirection(ptPath->pTargetUnit->pDynamicPath);
		const int v15 = ptPath->unk0x68[0];
		const uint16_t v13 = (uint16_t)v15 << 8;
		const float nDistance = (float)v15;
		const uint16_t nAngleRadians_512 = (8 * nDir - v13) & 0x1FF;
		pOutPathPoint->X += (int64_t)(FOG_10083_Cos_LUT(nAngleRadians_512) * nDistance);
		pOutPathPoint->Y += (int64_t)(FOG_10084_Sin_LUT(nAngleRadians_512) * nDistance);
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

//D2Common.0x6FDABAC0
int __fastcall sub_6FDABAC0(D2PathInfoStrc* ptPathInfo, D2PathPointStrc* a2, int a3, int a4, int a5, signed int nMaxLength, int a7)
{
	UNIMPLEMENTED();
	return 0;
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


//D2Common.0x6FDAC270
int __fastcall sub_6FDAC270(D2PathInfoStrc* pPathInfo)
{
	UNIMPLEMENTED();
	return 0;
}

struct TanToDirectionLutEntryStrc {
	D2CoordStrc dirVector;
	uint8_t nAngle;
};

//D2Common.0x6FDD2598 
TanToDirectionLutEntryStrc lutTanToDirection[] = {
	{{ 0x0000, 0x1000 } , 0 },
	{{ 0x0020, 0x0FFF } , 0 },
	{{ 0x0040, 0x0FFF } , 0 },
	{{ 0x0060, 0x0FFE } , 0 },
	{{ 0x0080, 0x0FFD } , 0 },
	{{ 0x00A1, 0x0FFC } , 0 },
	{{ 0x00C1, 0x0FFB } , 0 },
	{{ 0x00E1, 0x0FF9 } , 0 },
	{{ 0x0101, 0x0FF7 } , 0 },
	{{ 0x0121, 0x0FF5 } , 0 },
	{{ 0x0141, 0x0FF3 } , 0 },
	{{ 0x0161, 0x0FF0 } , 0 },
	{{ 0x0181, 0x0FED } , 0 },
	{{ 0x01A1, 0x0FEA } , 1 },
	{{ 0x01C0, 0x0FE7 } , 1 },
	{{ 0x01E0, 0x0FE3 } , 1 },
	{{ 0x01FF, 0x0FDF } , 1 },
	{{ 0x021F, 0x0FDB } , 1 },
	{{ 0x023E, 0x0FD7 } , 1 },
	{{ 0x025E, 0x0FD2 } , 1 },
	{{ 0x027D, 0x0FCE } , 1 },
	{{ 0x029C, 0x0FC9 } , 1 },
	{{ 0x02BB, 0x0FC3 } , 1 },
	{{ 0x02D9, 0x0FBE } , 1 },
	{{ 0x02F8, 0x0FB8 } , 1 },
	{{ 0x0317, 0x0FB2 } , 1 },
	{{ 0x0335, 0x0FAC } , 2 },
	{{ 0x0353, 0x0FA6 } , 2 },
	{{ 0x0371, 0x0F9F } , 2 },
	{{ 0x038F, 0x0F99 } , 2 },
	{{ 0x03AD, 0x0F92 } , 2 },
	{{ 0x03CB, 0x0F8B } , 2 },
	{{ 0x03E8, 0x0F83 } , 2 },
	{{ 0x0406, 0x0F7C } , 2 },
	{{ 0x0423, 0x0F74 } , 2 },
	{{ 0x0440, 0x0F6C } , 2 },
	{{ 0x045D, 0x0F64 } , 2 },
	{{ 0x0479, 0x0F5C } , 2 },
	{{ 0x0496, 0x0F54 } , 2 },
	{{ 0x04B2, 0x0F4B } , 3 },
	{{ 0x04CE, 0x0F42 } , 3 },
	{{ 0x04EA, 0x0F39 } , 3 },
	{{ 0x0506, 0x0F30 } , 3 },
	{{ 0x0521, 0x0F27 } , 3 },
	{{ 0x053C, 0x0F1E } , 3 },
	{{ 0x0558, 0x0F14 } , 3 },
	{{ 0x0572, 0x0F0B } , 3 },
	{{ 0x058D, 0x0F01 } , 3 },
	{{ 0x05A8, 0x0EF7 } , 3 },
	{{ 0x05C2, 0x0EED } , 3 },
	{{ 0x05DC, 0x0EE3 } , 3 },
	{{ 0x05F6, 0x0ED8 } , 3 },
	{{ 0x0610, 0x0ECE } , 3 },
	{{ 0x0629, 0x0EC4 } , 4 },
	{{ 0x0642, 0x0EB9 } , 4 },
	{{ 0x065B, 0x0EAE } , 4 },
	{{ 0x0674, 0x0EA3 } , 4 },
	{{ 0x068D, 0x0E98 } , 4 },
	{{ 0x06A5, 0x0E8D } , 4 },
	{{ 0x06BD, 0x0E82 } , 4 },
	{{ 0x06D5, 0x0E77 } , 4 },
	{{ 0x06ED, 0x0E6C } , 4 },
	{{ 0x0704, 0x0E60 } , 4 },
	{{ 0x071C, 0x0E55 } , 4 },
	{{ 0x0733, 0x0E49 } , 4 },
	{{ 0x074A, 0x0E3E } , 4 },
	{{ 0x0760, 0x0E32 } , 4 },
	{{ 0x0777, 0x0E26 } , 4 },
	{{ 0x078D, 0x0E1A } , 5 },
	{{ 0x07A3, 0x0E0F } , 5 },
	{{ 0x07B9, 0x0E03 } , 5 },
	{{ 0x07CE, 0x0DF7 } , 5 },
	{{ 0x07E4, 0x0DEB } , 5 },
	{{ 0x07F9, 0x0DDF } , 5 },
	{{ 0x080E, 0x0DD3 } , 5 },
	{{ 0x0822, 0x0DC6 } , 5 },
	{{ 0x0837, 0x0DBA } , 5 },
	{{ 0x084B, 0x0DAE } , 5 },
	{{ 0x085F, 0x0DA2 } , 5 },
	{{ 0x0873, 0x0D96 } , 5 },
	{{ 0x0887, 0x0D89 } , 5 },
	{{ 0x089A, 0x0D7D } , 5 },
	{{ 0x08AD, 0x0D71 } , 5 },
	{{ 0x08C0, 0x0D64 } , 5 },
	{{ 0x08D3, 0x0D58 } , 5 },
	{{ 0x08E6, 0x0D4B } , 6 },
	{{ 0x08F8, 0x0D3F } , 6 },
	{{ 0x090A, 0x0D33 } , 6 },
	{{ 0x091C, 0x0D26 } , 6 },
	{{ 0x092E, 0x0D1A } , 6 },
	{{ 0x0940, 0x0D0D } , 6 },
	{{ 0x0951, 0x0D01 } , 6 },
	{{ 0x0962, 0x0CF5 } , 6 },
	{{ 0x0973, 0x0CE8 } , 6 },
	{{ 0x0984, 0x0CDC } , 6 },
	{{ 0x0995, 0x0CCF } , 6 },
	{{ 0x09A5, 0x0CC3 } , 6 },
	{{ 0x09B6, 0x0CB7 } , 6 },
	{{ 0x09C6, 0x0CAA } , 6 },
	{{ 0x09D6, 0x0C9E } , 6 },
	{{ 0x09E5, 0x0C92 } , 6 },
	{{ 0x09F5, 0x0C85 } , 6 },
	{{ 0x0A04, 0x0C79 } , 6 },
	{{ 0x0A14, 0x0C6D } , 6 },
	{{ 0x0A23, 0x0C61 } , 6 },
	{{ 0x0A31, 0x0C54 } , 7 },
	{{ 0x0A40, 0x0C48 } , 7 },
	{{ 0x0A4F, 0x0C3C } , 7 },
	{{ 0x0A5D, 0x0C30 } , 7 },
	{{ 0x0A6B, 0x0C24 } , 7 },
	{{ 0x0A79, 0x0C18 } , 7 },
	{{ 0x0A87, 0x0C0C } , 7 },
	{{ 0x0A95, 0x0C00 } , 7 },
	{{ 0x0AA2, 0x0BF4 } , 7 },
	{{ 0x0AB0, 0x0BE8 } , 7 },
	{{ 0x0ABD, 0x0BDC } , 7 },
	{{ 0x0ACA, 0x0BD0 } , 7 },
	{{ 0x0AD7, 0x0BC4 } , 7 },
	{{ 0x0AE4, 0x0BB8 } , 7 },
	{{ 0x0AF0, 0x0BAC } , 7 },
	{{ 0x0AFD, 0x0BA1 } , 7 },
	{{ 0x0B09, 0x0B95 } , 7 },
	{{ 0x0B15, 0x0B89 } , 7 },
	{{ 0x0B21, 0x0B7E } , 7 },
	{{ 0x0B2D, 0x0B72 } , 7 },
	{{ 0x0B39, 0x0B67 } , 7 },
	{{ 0x0B44, 0x0B5B } , 7 },
	{{ 0x0B50, 0x0B50 } , 7 },
};
static_assert(_countof(lutTanToDirection) == 128, "There should be 128 entries in this lookup table");

//D2Common.0x6FDAC5E0
void __fastcall PATH_GetDirectionVector_6FDAC5E0(D2CoordStrc* pDirectionVector, int* pOutDirection, DWORD dwStartPrecisionX, DWORD dwStartPrecisionY, DWORD dwTargetPrecisionX, DWORD dwTargetPrecisionY)
{
	bool bXStartLessThanTarget;
	DWORD dwMinPrecisionX;
	DWORD dwMaxPrecisionX;
	if (dwStartPrecisionX > dwTargetPrecisionX)
	{
		dwMinPrecisionX = dwTargetPrecisionX;
		dwMaxPrecisionX = dwStartPrecisionX;
		bXStartLessThanTarget = false;
	}
	else
	{
		dwMaxPrecisionX = dwTargetPrecisionX;
		dwMinPrecisionX = dwStartPrecisionX;
		bXStartLessThanTarget = true;
	}

	bool bYStartLessThanTarget;
	DWORD dwMinPrecisionY;
	DWORD dwMaxPrecisionY;
	if (dwStartPrecisionY > dwTargetPrecisionY)
	{
		dwMaxPrecisionY = dwStartPrecisionY;
		dwMinPrecisionY = dwTargetPrecisionY;
		bYStartLessThanTarget = false;
	}
	else
	{
		dwMinPrecisionY = dwStartPrecisionY;
		dwMaxPrecisionY = dwTargetPrecisionY;
		bYStartLessThanTarget = true;
	}

	bool bDeltaXGreaterThanY;
	if ((int)(dwMaxPrecisionX - dwMinPrecisionX) > (int)(dwMaxPrecisionY - dwMinPrecisionY))
	{
		std::swap(dwMinPrecisionX, dwMinPrecisionY);
		std::swap(dwMaxPrecisionX, dwMaxPrecisionY);
		bDeltaXGreaterThanY = TRUE;
	}
	else
	{
		bDeltaXGreaterThanY = FALSE;
	}
	int tangent = 0;
	if (dwMaxPrecisionY != dwMinPrecisionY)
		tangent = (int)(127 * (dwMaxPrecisionX - dwMinPrecisionX)) / (int)(dwMaxPrecisionY - dwMinPrecisionY);
	
	const auto& lutEntry = lutTanToDirection[tangent];

	uint8_t nAngle = lutEntry.nAngle;
	int nDirectionVectorX = lutEntry.dirVector.nX;
	if (!bDeltaXGreaterThanY)
	{
		pDirectionVector->nX = nDirectionVectorX;
		pDirectionVector->nY = lutEntry.dirVector.nY;
	}
	else
	{
		pDirectionVector->nY = nDirectionVectorX;
		pDirectionVector->nX = lutEntry.dirVector.nY;
		nAngle = (-1 - nAngle) & 0xF;
	}
	if (!bYStartLessThanTarget)
	{
		pDirectionVector->nY = -pDirectionVector->nY;
		nAngle = (-1 - nAngle) & 0x1F;
	}

	if (bXStartLessThanTarget)
	{
		*pOutDirection = (((-1 - nAngle) & 0x3F) + 8) & 0x3F;
	}
	else
	{
		pDirectionVector->nX = -pDirectionVector->nX;
		*pOutDirection = (nAngle + 8) & 0x3F;
	}
}

//D2Common.0x6FDAC700 (#10215)
int __stdcall PATH_ComputeDirection(int nX1, int nY1, int nX2, int nY2)
{
	D2CoordStrc pCoords = {};
	int nDirection = 0;

	PATH_GetDirectionVector_6FDAC5E0(&pCoords, &nDirection, PATH_ToFP16(nX1), PATH_ToFP16(nY1), PATH_ToFP16(nX2), PATH_ToFP16(nY2));

	return nDirection;
}

//D2Common.0x6FDAC760
int __stdcall PATH_ComputeDirectionFromPreciseCoords_6FDAC760(DWORD dwStartPrecisionX, DWORD dwStartPrecisionY, DWORD dwTargetPrecisionX, DWORD dwTargetPrecisionY)
{
	D2CoordStrc pCoords = {};
	int nDirection = 0;

	PATH_GetDirectionVector_6FDAC5E0(&pCoords, &nDirection, dwStartPrecisionX, dwStartPrecisionY, dwTargetPrecisionX, dwTargetPrecisionY);

	return nDirection;
}

//D2Common.0x6FDAC790
void __stdcall sub_6FDAC790(D2DynamicPathStrc* pPath, int a2, int a3)
{
	const DWORD dwPrecisionX = pPath->dwPrecisionX;
	const DWORD dwPrecisionY = pPath->dwPrecisionY;
	DWORD nPointFP16X = PATH_ToFP16(pPath->PathPoints[pPath->dwCurrentPointIdx].X);
	DWORD nPointFP16Y = PATH_ToFP16(pPath->PathPoints[pPath->dwCurrentPointIdx].Y);
	while (nPointFP16X == dwPrecisionX && nPointFP16Y == dwPrecisionY)
	{
		if (pPath->dwCurrentPointIdx++ < (pPath->dwPathPoints - 1))
		{
			nPointFP16X = PATH_ToFP16(pPath->PathPoints[pPath->dwCurrentPointIdx].X);
			nPointFP16Y = PATH_ToFP16(pPath->PathPoints[pPath->dwCurrentPointIdx].Y);
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
	PATH_GetDirectionVector_6FDAC5E0( 
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
	
	if (a3 || nextPosInSamePoint && a2)
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

	if (pDynamicPath->dwPathType != PATHTYPE_TOWARD && pDynamicPath->dwPathType != PATHTYPE_MON_OTHER_2 && pDynamicPath->dwPathType != PATHTYPE_MOTION)
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
		pDynamicPath->dwPathType = PATHTYPE_MOTION;
		nResult = D2Common_10142(pDynamicPath, pUnit, 0);
	}

	return nResult;
}

//D2Common.0x6FDAC9A0
BOOL __stdcall D2Common_10226(D2UnitStrc* pUnit, signed int a2)
{
	UNIMPLEMENTED();
	return 1;
}

//D2Common.0x6FDACC40
void __fastcall sub_6FDACC40(int a1, D2RoomStrc* a2, unsigned int a3, unsigned int a4)
{
	UNIMPLEMENTED();
}

//D2Common.0x6FDACEC0
signed int __fastcall sub_6FDACEC0(int a1, int a2, int a3)
{
	UNIMPLEMENTED();
	return 0;
}

//D2Common.0x6FDAD330
int __fastcall sub_6FDAD330(D2PathInfoStrc* ptPathInfo)
{
	UNIMPLEMENTED();
	return 0;
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
			return pDynamicPath->wPosX == pDynamicPath->SP1.X && pDynamicPath->wPosY == pDynamicPath->SP1.Y;
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


D2PathPointStrc sgctZeroGameCoord = { 0,0 };

bool COORD_TEST_EQUAL(D2PathPointStrc lhs, D2PathPointStrc rhs) { return lhs == rhs; }

D2RoomStrc* DungeonFindRoomGame(D2RoomStrc* pRoom, uint16_t x, uint16_t y)
{	
	if (!pRoom)
		return nullptr;

	if (   (x < pRoom->nSubtileX) || (x >= pRoom->nSubtileX + pRoom->nSubtileWidth )
		|| (y < pRoom->nSubtileY) || (y >= pRoom->nSubtileY + pRoom->nSubtileHeight) )
	{
		D2RoomStrc** pRoomsList = nullptr;
		int nbRooms = 0;
		DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &pRoomsList, &nbRooms);
		for (int nRoomIdx = 0; nRoomIdx < nbRooms; nRoomIdx++)
		{
			if (D2RoomStrc* pCurRoom = pRoomsList[nRoomIdx])
			{

				if (   x >= pCurRoom->nSubtileX && x < (pCurRoom->nSubtileX + pCurRoom->nSubtileWidth )
					&& y >= pCurRoom->nSubtileY && y < (pCurRoom->nSubtileY + pCurRoom->nSubtileHeight) )
				{
					return pCurRoom;
				}
			}
		}
		return nullptr;
	}
	else
	{
		return pRoom;
	}
}

//D2Common.0x6FDAD5E0
// Should be in Step.cpp
BOOL __fastcall sub_6FDAD5E0(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pDestRoom, D2PathPointStrc tDest)
{
	D2UnitStrc* pUnit = pDynamicPath->pUnit;
	if (pUnit && pUnit->dwUnitType == UNIT_MISSILE)
	{
		if (tDest == D2PathPointStrc{ 0, 0 })
		{
			COLLISION_ResetMaskWithSize(
				pDynamicPath->pRoom, pDynamicPath->wPosX, pDynamicPath->wPosY,
				pDynamicPath->dwUnitSize,
				pDynamicPath->dwCollisionType
			);
			pDynamicPath->unk0x54 = 0;
		}
		else
		{
			D2PathPointStrc tPathPos = { pDynamicPath->wPosX , pDynamicPath->wPosY };
			if (pDynamicPath->wPosX == tDest.X && pDynamicPath->wPosY == tDest.Y)
			{
				pDynamicPath->dwFlags &= ~PATH_UNKNOWN_FLAG_0x00008;
			}
			else
			{
				pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00008;
			}
			pDynamicPath->unk0x54 = sub_6FD44BB0(
				pDynamicPath->pRoom, pDynamicPath->wPosX, pDynamicPath->wPosY,
				pDestRoom, tDest.X, tDest.Y,
				pDynamicPath->dwUnitSize,
				pDynamicPath->dwCollisionType,
				pDynamicPath->unk0x50
			);
			pDynamicPath->nSavedStepsCount = 1;
			pDynamicPath->SavedSteps[0] = tDest;
		}
	}
	else
	{
		if (tDest.X || tDest.Y)
		{
			D2Common_10133(
				pDynamicPath->pRoom, pDynamicPath->wPosX, pDynamicPath->wPosY,
				pDestRoom, tDest.X, tDest.Y,
				pDynamicPath->dwCollisionPattern,
				pDynamicPath->dwCollisionType
			);
		}
		else
		{
			COLLISION_ResetMaskWithPattern(
				pDynamicPath->pRoom, pDynamicPath->wPosX, pDynamicPath->wPosY,
				pDynamicPath->dwCollisionPattern,
				pDynamicPath->dwCollisionType
			);
		}
	}
	D2RoomStrc* pPathRoom = pDynamicPath->pRoom;
	if (pDynamicPath->pRoom != pDestRoom)
		pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00001;

	bool bDestinationIsValid = true;
	if (pDynamicPath->dwFlags & PATH_MISSILE_MASK)
	{
		if (!DungeonFindRoomGame(pDynamicPath->pRoom, tDest.X, tDest.Y))
		{
			pDynamicPath->dwPathPoints = 0;
			bDestinationIsValid = false;
		}
	}

	if (bDestinationIsValid)
	{
		pDynamicPath->dwPrecisionX = PATH_ToFP16(tDest.X);
		pDynamicPath->dwPrecisionY = PATH_ToFP16(tDest.Y);
		int dwPrecisionX = pDynamicPath->dwPrecisionX >> 11;
		int dwPrecisionY = pDynamicPath->dwPrecisionY >> 11;
		DUNGEON_FlattenCoords_IsoToCartesian(&dwPrecisionX, &dwPrecisionY);
		pDynamicPath->dwTargetX = dwPrecisionX;
		pDynamicPath->dwTargetY = dwPrecisionY;
		if (pDynamicPath->pUnit && (pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00001) != 0)
			sub_6FDADA20(pDynamicPath, pDestRoom);
	}

	D2_ASSERT(COORD_TEST_EQUAL(tDest, sgctZeroGameCoord) || DungeonFindRoomGame(pDestRoom, tDest.X, tDest.Y));

	uint32_t dwPrecisionRoundedX = (pDynamicPath->dwPrecisionX & 0xFFFF0000) + 0x8000;
	uint32_t dwPrecisionRoundedY = (pDynamicPath->dwPrecisionY & 0xFFFF0000) + 0x8000;
	if ((pDynamicPath->dwFlags & PATH_MISSILE_MASK) == 0
		|| COLLISION_GetRoomBySubTileCoordinates(pDynamicPath->pRoom, PATH_FromFP16(dwPrecisionRoundedX), PATH_FromFP16(dwPrecisionRoundedY)))
	{
		pDynamicPath->dwPrecisionX = dwPrecisionRoundedX;
		pDynamicPath->dwPrecisionY = dwPrecisionRoundedY;
		int dwTargetX = pDynamicPath->dwPrecisionX >> 11;
		int dwTargetY = pDynamicPath->dwPrecisionY >> 11;
		DUNGEON_FlattenCoords_IsoToCartesian(&dwTargetX, &dwTargetY);
		pDynamicPath->dwTargetX = dwTargetX;
		pDynamicPath->dwTargetY = dwTargetY;
		if (pDynamicPath->pUnit && (pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00001) != 0)
			sub_6FDADA20(pDynamicPath, 0);
	}
	else
	{
		pDynamicPath->dwPathPoints = 0;
	}

	pDynamicPath->dwFlags &= ~PATH_UNKNOWN_FLAG_0x00020;
	pDynamicPath->dwPathPoints = 0;
	pDynamicPath->dwCurrentPointIdx = 0;
	pDynamicPath->tVelocityVector.nX = 0;
	pDynamicPath->tVelocityVector.nY = 0;
	return TRUE;
}


//D2Common.0x6FDADA20
void __fastcall sub_6FDADA20(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pRoom)
{
	UNIMPLEMENTED();
}

//D2Common.0x6FDADC20
signed int __stdcall D2Common_10231(int a1, int a2, int pNumRooms, unsigned __int16 a4, D2RoomStrc** pppRoom)
{
	UNIMPLEMENTED();
	return 1;
}

//D2Common.0x6FDADF00
BOOL __stdcall D2Common_10230(D2DynamicPathStrc* pDynamicPath, int a2, D2RoomStrc* a3, unsigned int a4, __int16 a5)
{
	UNIMPLEMENTED();
	return FALSE;
}

//D2Common.0x6FDADF50) --------------------------------------------------------
signed int __stdcall D2Common_10232(D2DynamicPathStrc* a1, D2UnitStrc* a2, unsigned int pNumRooms, int a4, int pppRoom)
{
	UNIMPLEMENTED();
	return 1;
}

//D2Common.0x6FDAE250
void __fastcall PATHMISC_UpdateRoom(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pRoom)
{
	pDynamicPath->pRoomNext = pDynamicPath->pRoom;
	if (pDynamicPath->pRoom)
	{
		UNITROOM_RemoveUnitFromRoom(pDynamicPath->pUnit);
	}

	pDynamicPath->pRoom = pRoom;
	pDynamicPath->dwFlags |= UNITFLAG_TARGETABLE;

	if (pRoom)
	{
		UNITROOM_AddUnitToRoom(pDynamicPath->pUnit, pRoom);
		UNITROOM_RefreshUnit(pDynamicPath->pUnit);
	}
}

//D2Common.0x6FDAE290
void __stdcall D2Common_10233(D2DynamicPathStrc* pDynamicPath)
{
	// Probably belongs to Path/Step.cpp
	UNIMPLEMENTED();
}

//D2Common.0x6FDAE500 (#10234)
//TODO: Find a name
BOOL __stdcall D2Common_10234(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath)
	{
		return pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00008;
	}

	return FALSE;
}

//D2Common.0x6FDAE520) --------------------------------------------------------
void __stdcall D2Common_10235_PATH_Last(D2UnitStrc* pRiderUnit, D2UnitStrc* pHorseUnit)
{
	D2DynamicPathStrc *pRiderPath; // ebx@7
	D2DynamicPathStrc *pHorsePath; // eax@7
	D2RoomStrc *v5; // ecx@11
	unsigned int v6; // ebp@13
	unsigned int v7; // edx@13
	uint32_t v8; // eax@13
	int v9; // esi@14
	int v10; // edi@14
	int v11; // eax@17
	unsigned int v12; // edx@20
	D2RoomStrc *v13; // eax@21
	int v14; // ecx@24
	D2RoomStrc *v17; // eax@34
	uint16_t v18; // di@34
	uint16_t v19; // si@34
	int v21; // esi@39
	int v22; // edi@39
	unsigned int v24; // edx@45
	int v25; // eax@46
	int v26; // ecx@49
	D2RoomStrc **pppRoom; // [sp+10h] [bp-18h]@20
	int pX; // [sp+14h] [bp-14h]@32
	int pY; // [sp+18h] [bp-10h]@32
	D2RoomStrc *pRoom; // [sp+1Ch] [bp-Ch]@13
	unsigned int v31; // [sp+24h] [bp-4h]@13

	D2_ASSERT(pRiderUnit);
	D2_ASSERT(pHorseUnit);

	pRiderPath = pRiderUnit->pDynamicPath;
	pHorsePath = pHorseUnit->pDynamicPath;

	if (pRiderPath->wPosX != pHorsePath->wPosX || pRiderPath->wPosY != pHorsePath->wPosY)
	{
		pRiderPath->dwFlags |= 8;
	}
	else
	{
		pRiderPath->dwFlags &= 0xFFFFFFF7;
	}

	if (pRiderPath->pRoom != pHorsePath->pRoom)
	{
		pRiderPath->dwFlags |= 1;
	}

	v5 = pRiderPath->pRoom;
	v6 = pHorsePath->dwPrecisionX;
	pRoom = pHorsePath->pRoom;
	v7 = pHorsePath->dwPrecisionY;
	v8 = pRiderPath->dwFlags;
	v31 = v7;

	if (v8 & 0x40000)
	{
		v9 = v7 >> 16;
		v10 = v6 >> 16;
		if (!v5)
		{
			pRiderPath->dwPathPoints = 0;
			return;
		}

		if (v10 < v5->nSubtileX || v10 >= v5->nSubtileX + v5->nSubtileWidth || (v11 = v5->nSubtileY, v9 < v11) || v9 >= v11 + v5->nSubtileHeight)
		{
			pppRoom = 0;
			pHorseUnit = 0;
			DUNGEON_GetAdjacentRoomsListFromRoom(v5, &pppRoom, (int *)&pHorseUnit);
			v12 = 0;
			if (pHorseUnit)
			{
				while (1)
				{
					v13 = pppRoom[v12];
					if (v13 && v10 >= v13->nSubtileX)
					{
						if (v10 < v13->nSubtileX + v13->nSubtileWidth)
						{
							v14 = v13->nSubtileY;
							if (v9 >= v14)
							{
								if (v9 < v14 + v13->nSubtileHeight)
								{
									v5 = pppRoom[v12];
									v7 = v31;
									goto LABEL_30;
								}
							}
						}
					}

					++v12;
					if (v12 >= (unsigned int)pHorseUnit)
					{
						pRiderPath->dwPathPoints = 0;
						return;
					}
				}
			}

			pRiderPath->dwPathPoints = 0;
			return;
		}

LABEL_30:
		if (!v5)
		{
			pRiderPath->dwPathPoints = 0;
			return;
		}
	}

	pRiderPath->dwPrecisionX = v6;
	pRiderPath->dwPrecisionY = v7;

	pX = v6 >> 11;
	pY = v7 >> 11;
	DUNGEON_FlattenCoords_IsoToCartesian(&pX, &pY);


	pRiderPath->dwTargetX = pX;
	pRiderPath->dwTargetY = pY;
	if (pRiderPath->pUnit)
	{
		if (pRiderPath->dwFlags & 1)
		{
			v17 = pRiderPath->pRoom;
			v18 = pRiderPath->wPosX;
			v19 = pRiderPath->wPosY;

			if (!v17 || v18 < v17->nSubtileX || v18 >= v17->nSubtileX + v17->nSubtileWidth || v19 < v17->nSubtileY || v19 >= v17->nSubtileY + v17->nSubtileHeight)
			{
				v21 = pRiderPath->wPosY;
				v22 = pRiderPath->wPosX;
				if (!v17)
				{
					v17 = 0;
					if (!pRoom || (v17 = COLLISION_GetRoomBySubTileCoordinates(pRoom, v22, v21)) == 0)
					{
						if (pRiderPath->dwFlags & PATH_MISSILE_MASK)
						{
							pRiderPath->dwPathPoints = 0;
							return;
						}
					}
					PATHMISC_UpdateRoom(pRiderPath, v17);
					return;
				}

				if (v22 < v17->nSubtileX || v22 >= v17->nSubtileX + v17->nSubtileWidth || v21 < v17->nSubtileY || v21 >= v17->nSubtileY + v17->nSubtileHeight)
				{
					pHorseUnit = 0;
					pRiderUnit = 0;
					DUNGEON_GetAdjacentRoomsListFromRoom(v17, (D2RoomStrc ***)&pHorseUnit, (int *)&pRiderUnit);
					v24 = 0;
					if (!pRiderUnit)
					{
						v17 = 0;
						if (!pRoom || (v17 = COLLISION_GetRoomBySubTileCoordinates(pRoom, v22, v21)) == 0)
						{
							if (pRiderPath->dwFlags & PATH_MISSILE_MASK)
							{
								pRiderPath->dwPathPoints = 0;
								return;
							}
						}
						PATHMISC_UpdateRoom(pRiderPath, v17);
						return;
					}

					while (1)
					{
						v25 = *(&pHorseUnit->dwUnitType + v24);
						if (v25)
						{
							if (v22 >= *(int *)v25)
							{
								if (v22 < *(int *)v25 + *(int *)(v25 + 8))
								{
									v26 = *(int *)(v25 + 4);
									if (v21 >= v26)
									{
										if (v21 < v26 + *(int *)(v25 + 12))
											break;
									}
								}
							}
						}
						++v24;
						if (v24 >= (unsigned int)pRiderUnit)
						{
							v17 = 0;
							if (!pRoom || (v17 = COLLISION_GetRoomBySubTileCoordinates(pRoom, v22, v21)) == 0)
							{
								if (pRiderPath->dwFlags & PATH_MISSILE_MASK)
								{
									pRiderPath->dwPathPoints = 0;
									return;
								}
							}
							PATHMISC_UpdateRoom(pRiderPath, v17);
							return;
						}
					}
					v17 = (D2RoomStrc *)*(&pHorseUnit->dwUnitType + v24);
				}
				if (v17)
				{
					PATHMISC_UpdateRoom(pRiderPath, v17);
					return;
				}
				if (!pRoom || (v17 = COLLISION_GetRoomBySubTileCoordinates(pRoom, v22, v21)) == 0)
				{
					if (pRiderPath->dwFlags & PATH_MISSILE_MASK)
					{
						pRiderPath->dwPathPoints = 0;
						return;
					}
				}
				PATHMISC_UpdateRoom(pRiderPath, v17);
				return;
			}
		}
	}
}
