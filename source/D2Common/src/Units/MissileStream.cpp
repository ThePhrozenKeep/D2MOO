#include "Units/MissileStream.h"

#include "D2Collision.h"
#include "Units/Missile.h"
#include "Path/Path.h"
#include "Path/PathMisc.h"
#include "Path/Path_IDAStar.h"
#include "Units/Units.h"


//D2Common.0x6FDBC230 (#11213)
void __stdcall MISSTREAM_AllocMissileStream(D2UnitStrc* pMissile)
{
	D2MissileStreamStrc* pStream = nullptr;

	if (!MISSILE_GetStream(pMissile))
	{
		pStream = D2_ALLOC_STRC_POOL(pMissile ? pMissile->pMemoryPool : nullptr, D2MissileStreamStrc);
		pStream->unk0x00 = 0;
		pStream->unk0x04 = 0;

		MISSILE_SetStream(pMissile, pStream);
	}
}

//D2Common.0x6FDBC280 (#11214)
void __stdcall MISSTREAM_FreeMissileStream(D2UnitStrc* pMissile)
{
	D2MissileStreamStrc* pStream = MISSILE_GetStream(pMissile);

	if (pStream)
	{
		if (pStream->unk0x00)
		{
			D2_FREE_POOL(pMissile ? pMissile->pMemoryPool : NULL, (void*)pStream->unk0x00);
		}

		D2_FREE_POOL(pMissile ? pMissile->pMemoryPool : NULL, pStream);
	}
}

//D2Common.0x6FDBC2E0 (#11215)
void __stdcall MISSTREAM_ExecuteHit(D2UnitStrc* pUnit, int nCollisionMask, int nCollisionPattern, void (__fastcall* pfnHit)(D2UnitStrc*, D2UnitStrc*))
{
	D2PathPointStrc* ppPathPoints = NULL;
	D2UnitStrc* pTargetUnit = NULL;
	D2RoomStrc* pRoom = NULL;
	int nPathPoints = 0;

	PATH_SetType(pUnit->pDynamicPath, 16);
	D2Common_10142(pUnit->pDynamicPath, pUnit, 0);

	nPathPoints = PATH_GetPathPoints(pUnit->pDynamicPath, &ppPathPoints);

	pRoom = UNITS_GetRoom(pUnit);

	for (int i = 0; i < nPathPoints; ++i)
	{
		if (COLLISION_CheckMaskWithSize(pRoom, ppPathPoints[i].X, ppPathPoints[i].Y, nCollisionPattern, nCollisionMask))
		{
			pTargetUnit = D2Common_10407(pRoom, ppPathPoints[i].X, ppPathPoints[i].Y, MISSTREAM_Return1, 0, nCollisionPattern);
			if (pTargetUnit)
			{
				D2_ASSERT(!IsBadCodePtr((FARPROC)pfnHit));

				pfnHit(pUnit, pTargetUnit);
			}
		}
	}
}

//D2Common.0x6FDBC3B0
int __fastcall MISSTREAM_Return1(D2UnitStrc* pUnit1, void* pUnit2)
{
	return 1;
}

//D2Common.0x6FDBC3C0 (#11216)
//TODO: Variable names
void __stdcall MISSTREAM_Update(D2UnitStrc* a1, D2UnitStrc* pMissile, int a3, int a4, int (__fastcall* pfCreate)(D2UnitStrc*, int, int, int, int, int), void (__fastcall* a6)(int))
{
	D2MissileStreamStrc* pStream = NULL;
	D2PathPointStrc* pPathPoints = NULL;
	BOOL bCreate = FALSE;
	int nPathPoints = 0;
	int nCounter = 0;
	int nPoints = 0;
	int nRatio = 0;
	int nIndex = 0;
	int nTemp = 0;
	int nMax = 0;
	int nX = 0;
	int nY = 0;
	int v20 = 0;
	int v24 = 0;
	int v27 = 0;
	int v29 = 0;
	char v23 = 0;

	PATH_SetType(pMissile->pDynamicPath, 16);
	D2Common_10142(pMissile->pDynamicPath, pMissile, 0);

	nPathPoints = PATH_GetPathPoints(pMissile->pDynamicPath, &pPathPoints);

	if (nPathPoints != 0 && nPathPoints != 1)
	{
		pStream = MISSILE_GetStream(pMissile);
		D2_ASSERT(pStream);

		nTemp = nPathPoints - 1;
		nRatio = a3 / a4;

		if (nTemp < nRatio)
		{
			nPoints = nRatio;
			bCreate = TRUE;
		}
		else
		{
			nPoints = nTemp;
			bCreate = FALSE;
		}

		if (pStream->unk0x04)
		{
			if (pStream->unk0x04 < nPoints)
			{
				pStream->unk0x00 = (int*)D2_REALLOC_POOL(pMissile->pMemoryPool, pStream->unk0x00, sizeof(int) * nPoints);
			}
		}
		else
		{
			pStream->unk0x04 = nPoints;
			pStream->unk0x00 = (int*)D2_CALLOC_POOL(pMissile->pMemoryPool, sizeof(int) * nPoints);
		}

		nMax = nTemp / nRatio;
		if (bCreate)
		{
			v29 = 4 * (nRatio - nTemp) / nTemp;
		}
		else
		{
			v29 = 0;
		}

		D2_ASSERT(!IsBadCodePtr((FARPROC)pfCreate));

		v20 = a3;
		v27 = 4;
		nIndex = 0;
		nCounter = 0;
		while (nIndex < nTemp)
		{
			nX = pPathPoints[nIndex].X;
			nY = pPathPoints[nIndex].Y;

			v23 = PATH_ComputeDirection(pPathPoints[nIndex + 1].X, pPathPoints[nIndex + 1].Y, nX, nY);
			if (nCounter >= nMax && a3)
			{
				nCounter = 1;

				pStream->unk0x00[nIndex] = pfCreate(pMissile, pStream->unk0x00[nIndex], nX, nY, v20, 1);
				v20 -= a4;

				if (bCreate)
				{
					v24 = v29 / 4;
					if (v27 < v29 % 4)
					{
						++v24;
					}

					--v27;
					if (v27 <= 0)
					{
						v27 = 4;
					}

					for (int i = 0; i < v24; ++i)
					{
						pStream->unk0x00[nIndex] = pfCreate(a1, pStream->unk0x00[nIndex], nX, nY, v20, 2);
						v20 -= a4;
					}
				}
			}
			else
			{
				++nCounter;
				pStream->unk0x00[nIndex] = pfCreate(a1, pStream->unk0x00[nIndex], nX, nY, v20, 0);
			}

			++nIndex;
		}

		while (nIndex < pStream->unk0x04)
		{
			a6(pStream->unk0x00[nIndex]);
			pStream->unk0x00[nIndex] = 0;
			++nIndex;
		}
	}
}
