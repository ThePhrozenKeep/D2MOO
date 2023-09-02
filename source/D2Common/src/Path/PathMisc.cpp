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

	if (!COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pRoom, (pPoint.X + gatDirectionToOffset_6FDD2118[pTestDir[0]].nX), (pPoint.Y + gatDirectionToOffset_6FDD2118[pTestDir[0]].nY), pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask))
	{
		*pDirection = pTestDir[0];
		return TRUE;
	}

	if (!COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pRoom, (pPoint.X + gatDirectionToOffset_6FDD2118[pTestDir[1]].nX), (pPoint.Y + gatDirectionToOffset_6FDD2118[pTestDir[1]].nY), pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask))
	{
		*pDirection = pTestDir[1];
		return TRUE;
	}

	D2_ASSERT(pTestDir[2] != PATH_DIR_NULL);

	if (!COLLISION_CheckAnyCollisionWithPattern(pPathInfo->pRoom, (pPoint.X + gatDirectionToOffset_6FDD2118[pTestDir[2]].nX), (pPoint.Y + gatDirectionToOffset_6FDD2118[pTestDir[2]].nY), pPathInfo->nCollisionPattern, pPathInfo->nCollisionMask))
	{
		*pDirection = pTestDir[2];
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDAA9F0
int __fastcall PATH_Toward_6FDAA9F0(D2PathInfoStrc *pPathInfo)
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
		pGameCoord->X = pDynamicPath->tGameCoords.wPosX;
		pGameCoord->Y = pDynamicPath->tGameCoords.wPosY;
		return FALSE;
	}

	sub_6FDAC790(pDynamicPath, 1, 1);

	if (pDynamicPath->dwVelocity)
	{
		pPoint.X = pDynamicPath->tGameCoords.wPosX;
		pPoint.Y = pDynamicPath->tGameCoords.wPosY;
		return sub_6FDAAD10(pDynamicPath, pGameCoord, pPoint);
	}
	else
	{
		pGameCoord->X = pDynamicPath->tGameCoords.wPosX;
		pGameCoord->Y = pDynamicPath->tGameCoords.wPosY;
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

//D2Common.6FDAB130
int __fastcall sub_6FDAB130(D2PathInfoStrc* pPathInfo)
{

	D2DynamicPathStrc* pDynamicPath = pPathInfo->pDynamicPath;
	pDynamicPath->dwCurrentPointIdx = 0;
	pDynamicPath->dwPathPoints = 0;
	int v3 = PATH_Toward_6FDAA9F0(pPathInfo);
	
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
			v3 = PATH_Toward_6FDAA9F0(pPathInfo);
	}
	return v3;
}

//D2Common.0x6FDAB1E0
int __fastcall PATH_Leap_6FDAB1E0(D2PathInfoStrc* pPathInfo)
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
int __fastcall PATH_Knockback_Client_6FDAB240(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->dwCurrentPointIdx = 0;
	pPathInfo->pDynamicPath->PathPoints[0] = pPathInfo->tTargetCoord;
	sub_6FDAABF0(pPathInfo->pDynamicPath, &pPathInfo->pDynamicPath->PathPoints[0]);
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
		nDir = (nMainDirection + (char)nDir) % ARRAY_SIZE(gatDirectionToOffset_6FDD2118);
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
	D2_MAYBE_UNUSED(nUnused);
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
static_assert(ARRAY_SIZE(lutTanToDirection) == 128, "There should be 128 entries in this lookup table");

//D2Common.0x6FDAC5E0
// Should be in Step.cpp
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
	if (!bDeltaXGreaterThanY)
	{
		pDirectionVector->nX = lutEntry.dirVector.nX;
		pDirectionVector->nY = lutEntry.dirVector.nY;
	}
	else
	{
		pDirectionVector->nY = lutEntry.dirVector.nX;
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
		nAngle = (-1 - nAngle) & 0x3F;
	}
	else
	{
		pDirectionVector->nX = -pDirectionVector->nX;
	}
	*pOutDirection = (nAngle + 8) & 0x3F;
}

//D2Common.0x6FDAC700 (#10215)
int __stdcall PATH_ComputeDirection(int nX1, int nY1, int nX2, int nY2)
{
	D2CoordStrc pCoords = {};
	int nDirection = 0;

	PATH_GetDirectionVector_6FDAC5E0(&pCoords, &nDirection, PATH_ToFP16Center(nX1), PATH_ToFP16Center(nY1), PATH_ToFP16Center(nX2), PATH_ToFP16Center(nY2));

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
void __fastcall sub_6FDAC790(D2DynamicPathStrc* pPath, int a2, int a3)
{
	const DWORD dwPrecisionX = pPath->tGameCoords.dwPrecisionX;
	const DWORD dwPrecisionY = pPath->tGameCoords.dwPrecisionY;
	DWORD nPointFP16X = PATH_ToFP16Center(pPath->PathPoints[pPath->dwCurrentPointIdx].X);
	DWORD nPointFP16Y = PATH_ToFP16Center(pPath->PathPoints[pPath->dwCurrentPointIdx].Y);
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

//D2Common.0x6FDACC40
void __fastcall sub_6FDACC40(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* a2, unsigned int a3, unsigned int a4)
{
	unsigned int v4; // ebx
	unsigned int v6; // esi
	uint32_t v7; // eax
	D2RoomStrc* v8; // eax
	signed int v9; // esi
	signed int v10; // edi
	int v11; // ecx
	int v12; // edx
	D2RoomStrc* v13; // eax
	int v14; // ecx
	D2UnitStrc* v15; // eax
	int v16; // edx
	D2RoomStrc* v17; // eax
	uint16_t v18; // bx
	uint16_t v19; // di
	int v20; // edx
	D2RoomStrc* v21; // esi
	int v22; // edi
	int v23; // ebx
	int v24; // ecx
	int v25; // edx
	D2RoomStrc* v26; // eax
	int v27; // ecx
	D2RoomStrc* v28; // eax
	D2UnitStrc* v29; // edi
	uint32_t v30; // ecx
	DWORD pNumRooms; // [esp+10h] [ebp-14h] BYREF
	D2RoomStrc** pppRoom; // [esp+14h] [ebp-10h] BYREF
	int pX; // [esp+18h] [ebp-Ch] BYREF
	int pY; // [esp+1Ch] [ebp-8h] BYREF
	D2RoomStrc* pRoom; // [esp+20h] [ebp-4h]

	v4 = a3;
	v6 = a4;
	v7 = pDynamicPath->dwFlags;
	pRoom = a2;
	if ((v7 & PATH_MISSILE_MASK) == 0)
		goto LABEL_18;
	v8 = pDynamicPath->pRoom;
	v9 = HIWORD(a4);
	v10 = HIWORD(a3);
	if (!v8)
		goto LABEL_17;
	if (v10 >= v8->nSubtileX && v10 < v8->nSubtileX + v8->nSubtileWidth)
	{
		v11 = v8->nSubtileY;
		if (v9 >= v11 && v9 < v11 + v8->nSubtileHeight)
			goto LABEL_16;
	}
	pppRoom = 0;
	pNumRooms = 0;
	DUNGEON_GetAdjacentRoomsListFromRoom(v8, &pppRoom, (int*)&pNumRooms);
	v12 = 0;
	if (!pNumRooms)
	{
	LABEL_17:
		pDynamicPath->dwPathPoints = 0;
		return;
	}
	while (1)
	{
		v13 = pppRoom[v12];
		if (v13)
		{
			if (v10 >= v13->nSubtileX && v10 < v13->nSubtileX + v13->nSubtileWidth)
			{
				v14 = v13->nSubtileY;
				if (v9 >= v14 && v9 < v14 + v13->nSubtileHeight)
					break;
			}
		}
		if (++v12 >= pNumRooms)
		{
			pDynamicPath->dwPathPoints = 0;
			return;
		}
	}
	v4 = a3;
	v8 = pppRoom[v12];
LABEL_16:
	v6 = a4;
	if (!v8)
		goto LABEL_17;
LABEL_18:
	pDynamicPath->tGameCoords.dwPrecisionX = v4;
	pDynamicPath->tGameCoords.dwPrecisionY = v6;
	PATH_UpdateClientCoords(pDynamicPath);
	v15 = pDynamicPath->pUnit;
	if (!v15)
		return;
	if ((pDynamicPath->dwFlags & 1) == 0)
		return;
	v17 = pDynamicPath->pRoom;
	v18 = pDynamicPath->tGameCoords.wPosX;
	v19 = pDynamicPath->tGameCoords.wPosY;
	if (v17)
	{
		if (v18 >= v17->nSubtileX && v18 < v17->nSubtileX + v17->nSubtileWidth)
		{
			v20 = v17->nSubtileY;
			if (v19 >= v20 && v19 < v20 + v17->nSubtileHeight)
				return;
		}
	}
	v21 = 0;
	v22 = pDynamicPath->tGameCoords.wPosY;
	v23 = pDynamicPath->tGameCoords.wPosX;
	if (!v17)
		goto LABEL_32;
	if (v23 < v17->nSubtileX
		|| v23 >= v17->nSubtileX + v17->nSubtileWidth
		|| (v24 = v17->nSubtileY, v22 < v24)
		|| v22 >= v24 + v17->nSubtileHeight)
	{
		pppRoom = 0;
		a3 = 0;
		DUNGEON_GetAdjacentRoomsListFromRoom(v17, &pppRoom, (int*)&a3);
		v25 = 0;
		if (!a3)
		{
		LABEL_43:
			v21 = 0;
		LABEL_32:
			if ((!pRoom || (v21 = COLLISION_GetRoomBySubTileCoordinates(pRoom, v23, v22)) == 0)
				&& (pDynamicPath->dwFlags & PATH_MISSILE_MASK) != 0)
			{
				pDynamicPath->dwPathPoints = 0;
				return;
			}
			goto LABEL_45;
		}
		while (1)
		{
			v26 = pppRoom[v25];
			if (v26)
			{
				if (v23 >= v26->nSubtileX && v23 < v26->nSubtileX + v26->nSubtileWidth)
				{
					v27 = v26->nSubtileY;
					if (v22 >= v27 && v22 < v27 + v26->nSubtileHeight)
						break;
				}
			}
			if (++v25 >= a3)
				goto LABEL_43;
		}
		v21 = pppRoom[v25];
	}
	else
	{
		v21 = pDynamicPath->pRoom;
	}
	if (!v21)
		goto LABEL_32;
LABEL_45:
	v28 = pDynamicPath->pRoom;
	v29 = pDynamicPath->pUnit;
	pDynamicPath->pRoomNext = v28;
	if (v28)
		UNITROOM_RemoveUnitFromRoom(v29);
	v30 = pDynamicPath->dwFlags;
	pDynamicPath->pRoom = v21;
	pDynamicPath->dwFlags = v30 | PATH_CURRENT_ROOM_INVALID;
	if (v21)
	{
		UNITROOM_AddUnitToRoom(v29, v21);
		UNITROOM_RefreshUnit(v29);
	}
}

//D2Common.0x6FDACEC0
BOOL __fastcall sub_6FDACEC0(D2DynamicPathStrc* pDynamicPath, D2FP32_16* a2, D2UnitStrc** pUnit)
{
	D2UnitStrc* v4; // eax
	uint32_t v5; // eax
	int v7; // edi
	int v8; // ebx
	int32_t v9; // ecx
	int v10; // eax
	int v11; // ecx
	int v12; // eax
	int v13; // ecx
	int v14; // ebp
	int v15; // edx
	uint32_t v16; // eax
	uint32_t v17; // ecx
	uint32_t v18; // ebp
	uint32_t v19; // ecx
	uint8_t v20; // al
	uint32_t v21; // ecx
	uint32_t v22; // ebp
	uint32_t v23; // edi
	uint32_t v24; // eax
	uint32_t v25; // ebx
	uint32_t v26; // ecx
	D2PathPointStrc v27; // edx
	int v28; // ebx
	uint32_t v29; // eax
	D2UnitStrc* v30; // eax
	int v31; // ebx
	uint32_t v32; // eax
	uint16_t v33; // ax
	uint16_t v34; // ax
	signed int v35; // eax
	uint32_t v36; // edx
	int v37; // [esp+10h] [ebp-48h]
	D2PathPointStrc v38; // [esp+10h] [ebp-48h]
	int v40; // [esp+18h] [ebp-40h]
	int v41; // [esp+18h] [ebp-40h]
	int v42; // [esp+1Ch] [ebp-3Ch]
	int v43; // [esp+20h] [ebp-38h]
	D2UnitStrc* v44; // [esp+24h] [ebp-34h]
	D2PathPointStrc v45; // [esp+28h] [ebp-30h]
	D2PathPointStrc v46; // [esp+30h] [ebp-28h]
	signed int v47; // [esp+34h] [ebp-24h]
	signed int v48; // [esp+38h] [ebp-20h] BYREF
	signed int v49; // [esp+3Ch] [ebp-1Ch] BYREF
	uint32_t v50; // [esp+40h] [ebp-18h]
	uint32_t v51; // [esp+44h] [ebp-14h]
	int v52; // [esp+48h] [ebp-10h]
	uint32_t v53; // [esp+4Ch] [ebp-Ch]
	uint32_t v54; // [esp+50h] [ebp-8h]
	uint32_t v55; // [esp+54h] [ebp-4h]

	v4 = pDynamicPath->pUnit;
	pDynamicPath->unk0x54 = 0;
	v44 = v4;
	*pUnit = 0;
	if (!v4 || v4->dwUnitType != UNIT_MONSTER || (v42 = 1, (pDynamicPath->dwFlags & 0x10) == 0))
		v42 = 0;
	if (!pDynamicPath->tVelocityVector.nX && !pDynamicPath->tVelocityVector.nY)
	{
		v5 = pDynamicPath->tGameCoords.dwPrecisionX;
		pDynamicPath->dwPathPoints = 0;
		pDynamicPath->dwCurrentPointIdx = 0;
		a2->dwPrecisionX = (v5 & 0xFFFF0000) + 0x8000;
		a2->dwPrecisionY = (pDynamicPath->tGameCoords.dwPrecisionY & 0xFFFF0000) + 0x8000;
		return 0;
	}
	v7 = pDynamicPath->tVelocityVector.nY;
	v8 = pDynamicPath->tVelocityVector.nX;
	v47 = v7;
	if (pDynamicPath->dwPathType == 4)
	{
		v40 = 0;
	}
	else
	{
		v9 = pDynamicPath->dwCurrentPointIdx;
		v10 = (pDynamicPath->PathPoints[v9].X << 16) + 0x8000;
		v11 = (pDynamicPath->PathPoints[v9].Y << 16) + 0x8000;
		v12 = v10 - pDynamicPath->tGameCoords.dwPrecisionX;
		if (v12 < 0)
			v41 = -v12;
		else
			v41 = v12;
		v13 = v11 - pDynamicPath->tGameCoords.dwPrecisionY;
		if (v13 < 0)
			v37 = -v13;
		else
			v37 = v13;
		v14 = v7;
		if (v7 < 0)
			v14 = -v7;
		v15 = v8;
		if (v8 < 0)
			v15 = -v8;
		if (v15 <= v14)
			v15 = v14;
		if (v41 > v15 || v37 > v15)
		{
			v40 = 0;
		}
		else
		{
			v8 = v12;
			v47 = v13;
			v40 = 1;
			v7 = v13;
		}
	}
	v16 = pDynamicPath->tGameCoords.dwPrecisionX;
	v17 = pDynamicPath->tGameCoords.dwPrecisionY;
	v18 = v8 + pDynamicPath->tGameCoords.dwPrecisionX;
	v55 = v17 + v7;
	v54 = v18;
	v19 = HIWORD(v17);
	if (HIWORD(v16) != HIWORD(v18) || (WORD)v19 != HIWORD(v55))
	{
		v20 = pDynamicPath->nDist;
		if (v20)
		{
			v21 = pDynamicPath->dwPathType;
			if (v21 != 8 && v21 != 11)
				pDynamicPath->nDist = v20 - 1;
		}
		pDynamicPath->nSavedStepsCount = 0;
		v48 = v8;
		v49 = v47;
		sub_6FDAB810(&v48, &v49);
		v22 = pDynamicPath->tGameCoords.dwPrecisionY;
		v23 = pDynamicPath->tGameCoords.dwPrecisionX;
		v45.Y = HIWORD(v22);
		v24 = v8 + pDynamicPath->tGameCoords.dwPrecisionX;
		v25 = v47 + v22;
		v38.X = HIWORD(v24);
		v38.Y = (v47 + v22) >> 16;
		v26 = pDynamicPath->dwFlags;
		v50 = v24;
		v27.X = HIWORD(v23);
		v45.X = HIWORD(v23);
		v51 = v47 + v22;
		v43 = v26 & 0x20000;
		while (1)
		{
			if (D2PathPointStrc{ v27.X, v45.Y } == v38)
			{
			LABEL_58:
				if (pDynamicPath->nSavedStepsCount)
					pDynamicPath->dwFlags |= 8u;
				v18 = v54;
				a2->dwPrecisionX = v24;
				a2->dwPrecisionY = v25;
				*pUnit = (D2UnitStrc*)pDynamicPath->nSavedStepsCount;
				goto LABEL_61;
			}
			v28 = v48 + v23;
			v53 = v22 + v49;
			v52 = v48 + v23;
			v46.X = (v48 + v23) >> 16;
			v46.Y = (v22 + v49) >> 16;
			if (D2PathPointStrc{ v27.X,v45.Y } != v46)
				break;
		LABEL_56:
			v22 = v53;
			v23 = v28;
			v25 = v51;
			v45 = v46;
			v27.X = v46.X;
			v24 = v50;
		}
		v29 = pDynamicPath->dwFlags;
		if ((v29 & 4) != 0)
		{
			v30 = pDynamicPath->pUnit;
			if (!v30 || v30->dwUnitType && v30->dwUnitType != UNIT_MONSTER)
				FOG_DisplayWarning(
					"(UnitGetType(ptPath->hUnit) == UNIT_PLAYER) || (UnitGetType(ptPath->hUnit) == UNIT_MONSTER)",
					"C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\PATH\\Step.cpp",
					521);
			COLLISION_SetUnitCollisionMask(
				pDynamicPath->pRoom,
				v45.X,
				v45.Y,
				pDynamicPath->pRoom,
				v46.X,
				v46.Y,
				pDynamicPath->dwCollisionPattern,
				pDynamicPath->nFootprintCollisionMask);
		LABEL_53:
			if (v43)
				pDynamicPath->SavedSteps[pDynamicPath->nSavedStepsCount] = v46;
			v35 = pDynamicPath->nSavedStepsCount + 1;
			pDynamicPath->nSavedStepsCount = v35;
			if (v35 >= 10)
			{
				v24 = v50;
				v25 = v51;
				goto LABEL_58;
			}
			goto LABEL_56;
		}
		v31 = v29 & 0x40000;
		v32 = pDynamicPath->nMoveTestCollisionMask;
		if (v32 == 13313)
			v32 = 15361;
		if (v31)
			v33 = COLLISION_TryMoveUnitCollisionMask(
				pDynamicPath->pRoom,
				v45.X,
				v45.Y,
				v46.X,
				v46.Y,
				pDynamicPath->dwUnitSize,
				pDynamicPath->nFootprintCollisionMask,
				v32);
		else
			v33 = COLLISION_TryTeleportUnitCollisionMask(
				pDynamicPath->pRoom,
				v45.X,
				v45.Y,
				v46.X,
				v46.Y,
				pDynamicPath->dwCollisionPattern,
				pDynamicPath->nFootprintCollisionMask,
				v32);
		pDynamicPath->unk0x54 |= v33;
		v34 = pDynamicPath->unk0x54;
		if (v34)
		{
			if (!v31)
				goto LABEL_49;
		}
		else if (!v31)
		{
		LABEL_52:
			v28 = v52;
			goto LABEL_53;
		}
		if ((v34 & 5) != 0)
		{
		LABEL_49:
			a2->dwPrecisionX = (v23 & 0xFFFF0000) + 0x8000;
			a2->dwPrecisionY = (v22 & 0xFFFF0000) + 0x8000;
			*pUnit = (D2UnitStrc*)pDynamicPath->nSavedStepsCount;
			if (v42)
				return D2Common_10236(v44, pDynamicPath->pTargetUnit != 0) == 0;
			pDynamicPath->dwCurrentPointIdx = pDynamicPath->dwPathPoints;
			return 0;
		}
		goto LABEL_52;
	}
LABEL_61:
	v36 = v55;
	a2->dwPrecisionX = v18;
	a2->dwPrecisionY = v36;
	if (!v40)
		return 0;
	++pDynamicPath->dwCurrentPointIdx;
	return 1;
}

//D2Common.0x6FDAD330
int __fastcall sub_6FDAD330(D2DynamicPathStrc* pPath)
{
	uint32_t v2; // eax
	DWORD v3; // ebp
	DWORD v4; // edi
	D2RoomStrc* v5; // eax
	signed int v6; // edi
	signed int v7; // ebx
	int v8; // ecx
	DWORD v9; // edx
	D2RoomStrc* v10; // eax
	int v11; // ecx
	int v12; // edx
	D2UnitStrc* v13; // eax
	D2RoomStrc* v14; // ecx
	uint16_t v15; // dx
	uint16_t v16; // ax
	int v17; // ebx
	D2RoomStrc* v18; // edi
	D2RoomStrc* v19; // eax
	D2UnitStrc* v20; // ebx
	uint32_t v21; // ecx
	uint32_t v22; // eax
	int result; // eax
	DWORD pNumRooms; // [esp+10h] [ebp-1Ch] BYREF
	D2RoomStrc** pppRoom; // [esp+14h] [ebp-18h] BYREF
	DWORD v26; // [esp+18h] [ebp-14h]
	int pX; // [esp+1Ch] [ebp-10h] BYREF
	int pY; // [esp+20h] [ebp-Ch] BYREF
	DWORD v29; // [esp+28h] [ebp-4h]

	v2 = pPath->dwFlags;
	v3 = (pPath->tGameCoords.dwPrecisionX & 0xFFFF0000) + 0x8000;
	v4 = (pPath->tGameCoords.dwPrecisionY & 0xFFFF0000) + 0x8000;
	v29 = v4;
	if ((v2 & PATH_MISSILE_MASK) != 0)
	{
		v5 = pPath->pRoom;
		v6 = HIWORD(v4);
		v7 = HIWORD(v3);
		if (!v5)
			goto LABEL_18;
		if (v7 < v5->nSubtileX
			|| v7 >= v5->nSubtileX + v5->nSubtileWidth
			|| (v8 = v5->nSubtileY, v6 < v8)
			|| v6 >= v8 + v5->nSubtileHeight)
		{
			pppRoom = 0;
			pNumRooms = 0;
			DUNGEON_GetAdjacentRoomsListFromRoom(v5, &pppRoom, (int*)&pNumRooms);
			v9 = 0;
			v26 = 0;
			if (pNumRooms)
			{
				while (1)
				{
					v10 = pppRoom[v9];
					if (v10 && v7 >= v10->nSubtileX)
					{
						if (v7 < v10->nSubtileX + v10->nSubtileWidth)
						{
							v11 = v10->nSubtileY;
							if (v6 >= v11 && v6 < v11 + v10->nSubtileHeight)
							{
								v5 = pppRoom[v26];
								goto LABEL_17;
							}
						}
						v9 = v26;
					}
					v26 = ++v9;
					if (v9 >= pNumRooms)
					{
						pPath->dwPathPoints = 0;
						goto LABEL_34;
					}
				}
			}
			goto LABEL_18;
		}
	LABEL_17:
		if (!v5)
		{
		LABEL_18:
			pPath->dwPathPoints = 0;
			goto LABEL_34;
		}
		v4 = v29;
	}
	pPath->tGameCoords.dwPrecisionX = v3;
	pPath->tGameCoords.dwPrecisionY = v4;
	PATH_UpdateClientCoords(pPath);
	v13 = pPath->pUnit;
	if (v13)
	{
		if ((pPath->dwFlags & 1) != 0)
		{
			v14 = pPath->pRoom;
			v15 = pPath->tGameCoords.wPosX;
			v16 = pPath->tGameCoords.wPosY;
			if (!v14
				|| v15 < v14->nSubtileX
				|| v15 >= v14->nSubtileX + v14->nSubtileWidth
				|| (v17 = v14->nSubtileY, v16 < v17)
				|| v16 >= v17 + v14->nSubtileHeight)
			{
				v18 = COLLISION_GetRoomBySubTileCoordinates(
					v14,
					pPath->tGameCoords.wPosX,
					pPath->tGameCoords.wPosY);
				if (v18 || (pPath->dwFlags & 0x40000) == 0)
				{
					v19 = pPath->pRoom;
					v20 = pPath->pUnit;
					pPath->pRoomNext = v19;
					if (v19)
						UNITROOM_RemoveUnitFromRoom(v20);
					v21 = pPath->dwFlags;
					pPath->pRoom = v18;
					pPath->dwFlags = v21 | 2;
					if (v18)
					{
						UNITROOM_AddUnitToRoom(v20, v18);
						UNITROOM_RefreshUnit(v20);
					}
				}
				else
				{
					pPath->dwPathPoints = 0;
				}
			}
		}
	}
LABEL_34:
	pPath->dwFlags = pPath->dwFlags & (D2PathFlags)~PATH_UNKNOWN_FLAG_0x00020;
	result = 0;
	pPath->dwPathPoints = 0;
	pPath->dwCurrentPointIdx = 0;
	pPath->tVelocityVector.nX = 0;
	pPath->tVelocityVector.nY = 0;
	return result;
}

//D2Common.0x6FDAC9A0 (#10226)
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
	v3 = pDynamicPath->dwFlags & ~PATHTYPE_KNOCKBACK_SERVER;
	pDynamicPath->dwFlags = v3;
	if ((v3 & PATH_MISSILE_MASK) != 0)
		pDynamicPath->unk0x54 = 0;
	if ((v3 & 0x20) == 0 || (int)pDynamicPath->dwPathPoints <= 0 || !pDynamicPath->dwVelocity)
	{
		sub_6FDACC40(
			pDynamicPath,
			0,
			(pDynamicPath->tGameCoords.dwPrecisionX & 0xFFFF0000) + 0x8000,
			(pDynamicPath->tGameCoords.dwPrecisionY & 0xFFFF0000) + 0x8000);
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
	if (v6 >= 5 && v6 <= 6 && (signed int)pDynamicPath->dwCurrentPointIdx >= (signed int)pDynamicPath->dwPathPoints)
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
	if (pDynamicPath->dwPathType != 4
		&& (signed int)pDynamicPath->dwCurrentPointIdx < (signed int)pDynamicPath->dwPathPoints)
	{
		sub_6FDAC790(pDynamicPath, 1, 1);
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


D2PathPointStrc sgctZeroGameCoord = { 0,0 };

bool COORD_TEST_EQUAL(D2PathPointStrc lhs, D2PathPointStrc rhs) { return lhs == rhs; }

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
				pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY,
				pDynamicPath->dwUnitSize,
				pDynamicPath->nFootprintCollisionMask
			);
			pDynamicPath->unk0x54 = 0;
		}
		else
		{
			D2PathPointStrc tPathPos = { pDynamicPath->tGameCoords.wPosX , pDynamicPath->tGameCoords.wPosY };
			if (pDynamicPath->tGameCoords.wPosX == tDest.X && pDynamicPath->tGameCoords.wPosY == tDest.Y)
			{
				pDynamicPath->dwFlags &= ~PATH_UNKNOWN_FLAG_0x00008;
			}
			else
			{
				pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00008;
			}
			pDynamicPath->unk0x54 = COLLISION_ForceTeleportUnitCollisionMaskAndGetCollision(
				pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY,
				pDestRoom, tDest.X, tDest.Y,
				pDynamicPath->dwUnitSize,
				pDynamicPath->nFootprintCollisionMask,
				pDynamicPath->nMoveTestCollisionMask
			);
			pDynamicPath->nSavedStepsCount = 1;
			pDynamicPath->SavedSteps[0] = tDest;
		}
	}
	else
	{
		if (tDest.X || tDest.Y)
		{
			COLLISION_SetUnitCollisionMask(
				pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY,
				pDestRoom, tDest.X, tDest.Y,
				pDynamicPath->dwCollisionPattern,
				pDynamicPath->nFootprintCollisionMask
			);
		}
		else
		{
			COLLISION_ResetMaskWithPattern(
				pDynamicPath->pRoom, pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY,
				pDynamicPath->dwCollisionPattern,
				pDynamicPath->nFootprintCollisionMask
			);
		}
	}
	D2RoomStrc* pPathRoom = pDynamicPath->pRoom;
	if (pDynamicPath->pRoom != pDestRoom)
		pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00001;

	bool bDestinationIsValid = true;
	if (pDynamicPath->dwFlags & PATH_MISSILE_MASK)
	{
		if (!COLLISION_GetRoomBySubTileCoordinates(pDynamicPath->pRoom, tDest.X, tDest.Y))
		{
			pDynamicPath->dwPathPoints = 0;
			bDestinationIsValid = false;
		}
	}

	if (bDestinationIsValid)
	{
		pDynamicPath->tGameCoords.dwPrecisionX = PATH_ToFP16Center(tDest.X);
		pDynamicPath->tGameCoords.dwPrecisionY = PATH_ToFP16Center(tDest.Y);
		
		PATH_UpdateClientCoords(pDynamicPath);

		if (pDynamicPath->pUnit && (pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00001) != 0)
			PATH_RecacheRoom(pDynamicPath, pDestRoom);
	}

	D2_ASSERT(COORD_TEST_EQUAL(tDest, sgctZeroGameCoord) || COLLISION_GetRoomBySubTileCoordinates(pDestRoom, tDest.X, tDest.Y));

	uint32_t dwPrecisionRoundedX = (pDynamicPath->tGameCoords.dwPrecisionX & 0xFFFF0000) + 0x8000;
	uint32_t dwPrecisionRoundedY = (pDynamicPath->tGameCoords.dwPrecisionY & 0xFFFF0000) + 0x8000;
	if ((pDynamicPath->dwFlags & PATH_MISSILE_MASK) == 0
		|| COLLISION_GetRoomBySubTileCoordinates(pDynamicPath->pRoom, PATH_FromFP16(dwPrecisionRoundedX), PATH_FromFP16(dwPrecisionRoundedY)))
	{
		pDynamicPath->tGameCoords.dwPrecisionX = dwPrecisionRoundedX;
		pDynamicPath->tGameCoords.dwPrecisionY = dwPrecisionRoundedY;
		
		PATH_UpdateClientCoords(pDynamicPath);

		if (pDynamicPath->pUnit && (pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00001) != 0)
			PATH_RecacheRoom(pDynamicPath, 0);
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

//D2Common.0x6FDADC20 (#10231)
int __stdcall D2Common_10231(D2DynamicPathStrc* a1, D2UnitStrc* a2, D2RoomStrc* pRooms, int nX, int nY)
{
	D2DynamicPathStrc* v5; // esi
	D2UnitStrc* v6; // eax
	WORD v7; // dx
	D2RoomStrc* v8; // ecx
	uint32_t v9; // eax
	uint32_t v10; // eax
	int v11; // ebx
	unsigned int v12; // edi
	unsigned int v13; // ebp
	D2RoomStrc* v14; // edi
	int v15; // eax
	DWORD v16; // ebp
	D2RoomStrc* v17; // edx
	D2DynamicPathStrc* v18; // edx
	D2UnitStrc* v19; // eax
	int v20; // edx
	int* v21; // eax
	int v22; // ecx
	uint32_t v23; // eax
	int pY; // [esp+10h] [ebp-Ch] BYREF
	D2PathPointStrc v26; // [esp+14h] [ebp-8h]

	v5 = a1;
	v6 = a1->pUnit;
	v7 = a1->tGameCoords.wPosY;
	v26.X = a1->tGameCoords.wPosX;
	v26.Y = v7;
	if (v6 && v6->dwUnitType == UNIT_MISSILE)
	{
		if ((WORD)nX || (WORD)nY)
			COLLISION_TeleportUnitCollisionMask(
				a1->pRoom,
				v26.X,
				v26.Y,
				pRooms,
				(unsigned __int16)nX,
				(unsigned __int16)nY,
				a1->dwUnitSize,
				a1->nFootprintCollisionMask);
		else
			COLLISION_ResetMaskWithSize(a1->pRoom, v26.X, v26.Y, a1->dwUnitSize, a1->nFootprintCollisionMask);
	}
	else if ((WORD)nX || (WORD)nY)
	{
		COLLISION_SetUnitCollisionMask(
			a1->pRoom,
			v26.X,
			v26.Y,
			pRooms,
			(unsigned __int16)nX,
			(unsigned __int16)nY,
			a1->dwCollisionPattern,
			a1->nFootprintCollisionMask);
	}
	else
	{
		COLLISION_ResetMaskWithPattern(
			a1->pRoom,
			v26.X,
			v26.Y,
			a1->dwCollisionPattern,
			a1->nFootprintCollisionMask);
	}
	v8 = v5->pRoom;
	if (v8 != pRooms)
	{
		v9 = v5->dwFlags;
		v9 = v9 | PATH_UNKNOWN_FLAG_0x00001;
		v5->dwFlags = v9;
	}
	v10 = v5->dwFlags;
	v11 = (unsigned __int16)nX;
	DWORD v27 = (unsigned __int16)nY;
	v12 = ((unsigned __int16)nX << 16) + 0x8000;
	v13 = ((unsigned __int16)nY << 16) + 0x8000;
	if ((v10 & 0x40000) == 0 || COLLISION_GetRoomBySubTileCoordinates(v8, HIWORD(v12), HIWORD(v13)))
	{
		v5->tGameCoords.dwPrecisionX = v12;
		v5->tGameCoords.dwPrecisionY = v13;
		a1 = (D2DynamicPathStrc*)(v12 >> 11);
		pY = v13 >> 11;
		DUNGEON_GameToClientCoords((int*)&a1, &pY);
		v18 = a1;
		v5->dwClientCoordY = pY;
		v19 = v5->pUnit;
		v5->dwClientCoordX = (int32_t)v18;
		if (v19 && (v5->dwFlags & 1) != 0)
		{
			v14 = pRooms;
			PATH_RecacheRoom(v5, pRooms);
			goto LABEL_17;
		}
	}
	else
	{
		v5->dwPathPoints = 0;
	}
	v14 = pRooms;
LABEL_17:
	if (!(WORD)nX && !(WORD)nY)
		goto LABEL_40;
	if (!v14)
		goto LABEL_39;
	if (v11 < v14->nSubtileX || v11 >= v14->nSubtileX + v14->nSubtileWidth)
	{
		v16 = v27;
	}
	else
	{
		v15 = v14->nSubtileY;
		v16 = v27;
		if (*(DWORD*)&v27 >= v15 && *(DWORD*)&v27 < v15 + v14->nSubtileHeight)
		{
			v17 = v14;
			goto LABEL_38;
		}
	}
	nY = 0;
	pRooms = 0;
	DUNGEON_GetAdjacentRoomsListFromRoom(v14, (D2RoomStrc***)&nY, (int*)&pRooms);
	v20 = 0;
	if (!pRooms)
		goto LABEL_39;
	while (1)
	{
		v21 = *(int**)(nY + 4 * v20);
		if (v21)
		{
			if (v11 >= *v21 && v11 < *v21 + v21[2])
			{
				v22 = v21[1];
				if (*(DWORD*)&v16 >= v22 && *(DWORD*)&v16 < v22 + v21[3])
					break;
			}
		}
		if (++v20 >= (unsigned int)pRooms)
			goto LABEL_39;
	}
	v17 = *(D2RoomStrc**)(nY + 4 * v20);
LABEL_38:
	if (!v17)
	{
	LABEL_39:
		FOG_DisplayAssert(
			"COORD_TEST_EQUAL(tDest, sgctZeroGameCoord) || DungeonFindRoomGame( hRoomDest, tDest.wX, tDest.wY )",
			"C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\PATH\\Step.cpp",
			1228);
		exit(-1);
	}
LABEL_40:
	D2Common_10233(v5);
	v5->dwFlags = v5->dwFlags & (~PATH_UNKNOWN_FLAG_0x00020);
	v5->dwPathPoints = 0;
	v5->dwCurrentPointIdx = 0;
	v5->tVelocityVector.nX = 0;
	v5->tVelocityVector.nY = 0;
	v5->dwPathPoints = 0;
	return 1;
}

//D2Common.0x6FDADF00 (#10230)
BOOL __stdcall D2Common_10230(D2DynamicPathStrc* pDynamicPath, int a2, D2RoomStrc* pRoom, unsigned int a4, __int16 a5)
{
	D2PathPointStrc tPos;
	tPos.X = a4;
	tPos.Y = a5;
	BOOL result = sub_6FDAD5E0(pDynamicPath, pRoom, tPos);
	if (result)
	{
		pDynamicPath->unk0x38 = 15;
		result = 1;
	}
	return result;
}

//D2Common.0x6FDADF50) --------------------------------------------------------
signed int __stdcall D2Common_10232(D2DynamicPathStrc* a1, D2UnitStrc* a2, unsigned int pNumRooms, int a4, int pppRoom)
{
	UNIMPLEMENTED();
	return 1;
}

//D2Common.0x6FDAE250
void __fastcall PATHMISC_SetRoom(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pRoom)
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
	D2DynamicPathStrc* v1; // ecx
	DWORD v2; // ebx
	DWORD v3; // ebp
	D2RoomStrc* v4; // eax
	signed int v5; // esi
	int v6; // edi
	int v7; // ecx
	DWORD v8; // edx
	D2RoomStrc* v9; // eax
	int v10; // ecx
	int v12; // edx
	D2UnitStrc* v13; // eax
	D2RoomStrc* v14; // eax
	WORD v15; // cx
	WORD v16; // dx
	int v17; // edi
	int v18; // ecx
	int v19; // esi
	int v20; // edi
	int v21; // ecx
	int v22; // edx
	D2RoomStrc* v23; // eax
	int v24; // ecx
	int pNumRooms; // [esp+10h] [ebp-14h] BYREF
	D2RoomStrc** pppRoom; // [esp+14h] [ebp-10h] BYREF
	int v27; // [esp+18h] [ebp-Ch] BYREF
	int pX; // [esp+1Ch] [ebp-8h] BYREF
	int pY; // [esp+20h] [ebp-4h] BYREF

	v1 = pDynamicPath;
	v2 = (pDynamicPath->tGameCoords.dwPrecisionX & 0xFFFF0000) + 0x8000;
	v3 = (pDynamicPath->tGameCoords.dwPrecisionY & 0xFFFF0000) + 0x8000;
	if ((pDynamicPath->dwFlags & 0x40000) != 0)
	{
		v4 = pDynamicPath->pRoom;
		v5 = HIWORD(v3);
		v6 = HIWORD(v2);
		if (!v4)
		{
		LABEL_19:
			v1->dwPathPoints = 0;
			return;
		}
		if (v6 < v4->nSubtileX
			|| v6 >= v4->nSubtileX + v4->nSubtileWidth
			|| (v7 = v4->nSubtileY, v5 < v7)
			|| v5 >= v7 + v4->nSubtileHeight)
		{
			pppRoom = 0;
			pNumRooms = 0;
			DUNGEON_GetAdjacentRoomsListFromRoom(v4, &pppRoom, &pNumRooms);
			v8 = 0;
			v27 = 0;
			if (pNumRooms)
			{
				while (1)
				{
					v9 = pppRoom[v8];
					if (v9 && v6 >= v9->nSubtileX)
					{
						if (v6 < v9->nSubtileX + v9->nSubtileWidth)
						{
							v10 = v9->nSubtileY;
							if (v5 >= v10 && v5 < v10 + v9->nSubtileHeight)
							{
								v4 = pppRoom[v27];
								goto LABEL_17;
							}
						}
						v8 = v27;
					}
					v27 = ++v8;
					if (v8 >= pNumRooms)
					{
						pDynamicPath->dwPathPoints = 0;
						return;
					}
				}
			}
			goto LABEL_18;
		}
	LABEL_17:
		if (!v4)
		{
		LABEL_18:
			v1 = pDynamicPath;
			goto LABEL_19;
		}
		v1 = pDynamicPath;
	}
	pDynamicPath->tGameCoords.dwPrecisionX = v2;
	pDynamicPath->tGameCoords.dwPrecisionY = v3;
	PATH_UpdateClientCoords(pDynamicPath);
	v13 = pDynamicPath->pUnit;
	if (v13)
	{
		if ((pDynamicPath->dwFlags & 1) != 0)
		{
			v14 = pDynamicPath->pRoom;
			v15 = pDynamicPath->tGameCoords.wPosX;
			v16 = pDynamicPath->tGameCoords.wPosY;
			if (!v14
				|| v15 < v14->nSubtileX
				|| v15 >= v14->nSubtileX + v14->nSubtileWidth
				|| (v17 = v14->nSubtileY, v16 < v17)
				|| v16 >= v17 + v14->nSubtileHeight)
			{
				v18 = pDynamicPath->tGameCoords.wPosX;
				v19 = pDynamicPath->tGameCoords.wPosY;
				v20 = v18;
				if (v14)
				{
					if (v18 >= v14->nSubtileX && v18 < v14->nSubtileX + v14->nSubtileWidth)
					{
						v21 = v14->nSubtileY;
						if (v19 >= v21 && v19 < v21 + v14->nSubtileHeight)
						{
						LABEL_44:
							if (v14)
							{
							LABEL_47:
								PATHMISC_SetRoom(pDynamicPath, v14);
								return;
							}
						LABEL_45:
							if ((pDynamicPath->dwFlags & 0x40000) != 0)
							{
								pDynamicPath->dwPathPoints = 0;
								return;
							}
							goto LABEL_47;
						}
					}
					pppRoom = 0;
					v27 = 0;
					DUNGEON_GetAdjacentRoomsListFromRoom(v14, &pppRoom, &v27);
					v22 = 0;
					if (v27)
					{
						while (1)
						{
							v23 = pppRoom[v22];
							if (v23 && v20 >= v23->nSubtileX)
							{
								if (v20 < v23->nSubtileX + v23->nSubtileWidth)
								{
									v24 = v23->nSubtileY;
									if (v19 >= v24 && v19 < v24 + v23->nSubtileHeight)
									{
										v14 = pppRoom[v22];
										goto LABEL_44;
									}
								}
							}
							if (++v22 >= v27)
							{
								v14 = 0;
								goto LABEL_45;
							}
						}
					}
				}
				v14 = 0;
				goto LABEL_45;
			}
		}
	}
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

//D2Common.0x6FDAE520 (#10235)
void __stdcall D2Common_10235_PATH_UpdateRiderPath(D2UnitStrc* pRiderUnit, D2UnitStrc* pMountUnit)
{
	D2_ASSERT(pRiderUnit); // Named hRiderUnit in original game
	D2_ASSERT(pMountUnit); // Named hHorseUnit in original game

	D2DynamicPathStrc *pRiderPath = pRiderUnit->pDynamicPath;
	D2DynamicPathStrc *pMountPath = pMountUnit->pDynamicPath;

	if (pRiderPath->tGameCoords.wPosX != pMountPath->tGameCoords.wPosX || pRiderPath->tGameCoords.wPosY != pMountPath->tGameCoords.wPosY)
	{
		pRiderPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00008;
	}
	else
	{
		pRiderPath->dwFlags &= ~PATH_UNKNOWN_FLAG_0x00008;
	}

	if (pRiderPath->pRoom != pMountPath->pRoom)
	{
		// Rider is in different room than the mount 
		pRiderPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00001;
	}

	if (pRiderPath->dwFlags & PATH_MISSILE_MASK)
	{
		// Try to find the missile in the mount room list, if not, end its path
		if (!COLLISION_GetRoomBySubTileCoordinates(pRiderPath->pRoom, PATH_FromFP16(pMountPath->tGameCoords.dwPrecisionX), PATH_FromFP16(pMountPath->tGameCoords.dwPrecisionY)))
		{
			pRiderPath->dwPathPoints = 0;
			return;
		}
	}

	// Update the rider's target position
	pRiderPath->tGameCoords.dwPrecisionX = pMountPath->tGameCoords.dwPrecisionX;
	pRiderPath->tGameCoords.dwPrecisionY = pMountPath->tGameCoords.dwPrecisionY;
	PATH_UpdateClientCoords(pRiderPath);

	D2RoomStrc* pMountRoom = pMountPath->pRoom;
	
	// Rider was in a different room than the mount at some point
	if (pRiderPath->pUnit && (pRiderPath->dwFlags & PATH_UNKNOWN_FLAG_0x00001))
	{
		// Try finding room from rider's current room list
		D2RoomStrc* pRidersUpToDateRoom = COLLISION_GetRoomBySubTileCoordinates(pRiderPath->pRoom, pRiderPath->tGameCoords.wPosX, pRiderPath->tGameCoords.wPosY);
		
		if (!pRidersUpToDateRoom)
		{
			// If not in the rider's current room list, try with the mount's room list
			pRidersUpToDateRoom = COLLISION_GetRoomBySubTileCoordinates(pMountRoom, pRiderPath->tGameCoords.wPosX, pRiderPath->tGameCoords.wPosY);
			// If not found and it is a missile, just end its path
			if (!pRidersUpToDateRoom && (pRiderPath->dwFlags & PATH_MISSILE_MASK))
			{
				pRiderPath->dwPathPoints = 0;
				return;
			}
		}

		PATHMISC_SetRoom(pRiderPath, pRidersUpToDateRoom);
	}
}
