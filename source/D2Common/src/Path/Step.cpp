#include "Path/Step.h"

#include "D2Collision.h"
#include "D2Dungeon.h"
#include "Path/Path.h"
#include "Path/PathMisc.h"
#include "Units/Units.h"
#include "Units/UnitRoom.h"
#include <utility>

D2PathPointStrc sgctZeroGameCoord = { 0,0 };

bool COORD_TEST_EQUAL(D2PathPointStrc lhs, D2PathPointStrc rhs) { return lhs == rhs; }


struct TanToDirectionLutEntryStrc {
	D2CoordStrc dirVector;
	uint32_t nAngle;
};

//1.00:  D2Common.0x1008AF38
//1.10f: D2Common.0x6FDD2598 
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

//1.00:  D2Common.0x1005E6D0
//1.10f: D2Common.0x6FDAC5E0
void __fastcall PATH_GetDirectionVector(D2CoordStrc* pDirectionVector, int* pOutDirection, DWORD dwStartPrecisionX, DWORD dwStartPrecisionY, DWORD dwTargetPrecisionX, DWORD dwTargetPrecisionY)
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

	uint8_t nAngle = (uint8_t)lutEntry.nAngle;
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

//1.00:  D2Common.0x1005F420
//1.10f: D2Common.0x6FDACEC0
//1.13c: D2Common.0x6FD5D8E0
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
	uint16_t nCollidedWithMask; // ax
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
	pDynamicPath->nCollidedWithMask = 0;
	v44 = v4;
	*pUnit = 0;

	v42 = v4 && v4->dwUnitType == UNIT_MONSTER && (pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00010) != 0;

	if (!pDynamicPath->tVelocityVector.nX && !pDynamicPath->tVelocityVector.nY)
	{
		v5 = pDynamicPath->tGameCoords.dwPrecisionX;
		pDynamicPath->dwPathPoints = 0;
		pDynamicPath->dwCurrentPointIdx = 0;
		a2->dwPrecisionX = PATH_FP16FitToCenter(v5);
		a2->dwPrecisionY = PATH_FP16FitToCenter(pDynamicPath->tGameCoords.dwPrecisionY);
		return 0;
	}
	v7 = pDynamicPath->tVelocityVector.nY;
	v8 = pDynamicPath->tVelocityVector.nX;
	v47 = v7;
	if (pDynamicPath->dwPathType == PATHTYPE_MISSILE)
	{
		v40 = 0;
	}
	else
	{
		v9 = pDynamicPath->dwCurrentPointIdx;
		v10 = PATH_ToFP16Center(pDynamicPath->PathPoints[v9].X);
		v11 = PATH_ToFP16Center(pDynamicPath->PathPoints[v9].Y);
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
			if (v21 != PATHTYPE_KNOCKBACK_SERVER && v21 != PATHTYPE_KNOCKBACK_CLIENT)
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
		if ((pDynamicPath->dwFlags & 4) != 0)
		{
			D2_CHECK(pDynamicPath->pUnit && (pDynamicPath->pUnit->dwUnitType == UNIT_PLAYER || pDynamicPath->pUnit->dwUnitType != UNIT_MONSTER));
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
			pDynamicPath->nSavedStepsCount++;
			if (pDynamicPath->nSavedStepsCount >= PATH_MAX_STEP_LEN)
			{
				v24 = v50;
				v25 = v51;
				goto LABEL_58;
			}
			goto LABEL_56;
		}
		const bool bIsMissilePath = pDynamicPath->dwFlags & PATH_MISSILE_MASK;
		uint32_t nMoveTestColMask = pDynamicPath->nMoveTestCollisionMask;
		if (nMoveTestColMask == COLLIDE_MASK_MONSTER_THAT_CAN_OPEN_DOORS)
			nMoveTestColMask = COLLIDE_MASK_MONSTER_DEFAULT;
		if (bIsMissilePath)
			nCollidedWithMask = COLLISION_TryMoveUnitCollisionMask(
				pDynamicPath->pRoom,
				v45.X,
				v45.Y,
				v46.X,
				v46.Y,
				pDynamicPath->dwUnitSize,
				pDynamicPath->nFootprintCollisionMask,
				nMoveTestColMask);
		else
			nCollidedWithMask = COLLISION_TryTeleportUnitCollisionMask(
				pDynamicPath->pRoom,
				v45.X,
				v45.Y,
				v46.X,
				v46.Y,
				pDynamicPath->dwCollisionPattern,
				pDynamicPath->nFootprintCollisionMask,
				nMoveTestColMask);
		pDynamicPath->nCollidedWithMask |= nCollidedWithMask;
		if (pDynamicPath->nCollidedWithMask)
		{
			if (!bIsMissilePath)
				goto LABEL_49;
		}
		else if (!bIsMissilePath)
		{
		LABEL_52:
			v28 = v52;
			goto LABEL_53;
		}
		if ((pDynamicPath->nCollidedWithMask & (COLLIDE_BLOCK_PLAYER | COLLIDE_BARRIER)) != 0)
		{
		LABEL_49:
			a2->dwPrecisionX = PATH_FP16FitToCenter(v23);
			a2->dwPrecisionY = PATH_FP16FitToCenter(v22);
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

//1.00:  D2Common.0x1005FAB0
//1.10f: D2Common.0x6FDAD5E0
BOOL __fastcall sub_6FDAD5E0(D2DynamicPathStrc* pDynamicPath, D2ActiveRoomStrc* pDestRoom, D2PathPointStrc tDest)
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
			pDynamicPath->nCollidedWithMask = 0;
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
			pDynamicPath->nCollidedWithMask = COLLISION_ForceTeleportUnitCollisionMaskAndGetCollision(
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

	const uint32_t dwPrecisionSubileCenterX = PATH_FP16FitToCenter(pDynamicPath->tGameCoords.dwPrecisionX);
	const uint32_t dwPrecisionSubtileCenterY = PATH_FP16FitToCenter(pDynamicPath->tGameCoords.dwPrecisionY);
	if ((pDynamicPath->dwFlags & PATH_MISSILE_MASK) == 0
		|| COLLISION_GetRoomBySubTileCoordinates(pDynamicPath->pRoom, PATH_FromFP16(dwPrecisionSubileCenterX), PATH_FromFP16(dwPrecisionSubtileCenterY)))
	{
		pDynamicPath->tGameCoords.dwPrecisionX = dwPrecisionSubileCenterX;
		pDynamicPath->tGameCoords.dwPrecisionY = dwPrecisionSubtileCenterY;

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

//1.00:  D2Common.0x100604E0 Note: Changed since then. Used to look for the room using coordinates
//1.10f: D2Common.0x6FDAE250
void __fastcall PATHMISC_SetRoom(D2DynamicPathStrc* pPath, D2ActiveRoomStrc* pNewRoom)
{
	D2ActiveRoomStrc* pRoomDest = pNewRoom;
#ifdef VERSION_100
	if (!pPath->pRoom || !DungeonTestRoomGame(pPath->pRoom, pPath->tGameCoords.wPosX, pPath->tGameCoords.wPosY))
	{
		pRoomDest = COLLISION_GetRoomBySubTileCoordinates(pPath->pRoom, pPath->tGameCoords.wPosX, pPath->tGameCoords.wPosY);
		if(!pRoomDest && pNewRoom)
			pRoomDest = COLLISION_GetRoomBySubTileCoordinates(pNewRoom, pPath->tGameCoords.wPosX, pPath->tGameCoords.wPosY);
	}
	if(!pRoomDest && (pPath->dwFlags & PATH_MISSILE_MASK) != 0)
	{
		return;
	}
#endif
	pPath->pPreviousRoom = pPath->pRoom;
	if (pPath->pRoom)
	{
		UNITROOM_RemoveUnitFromRoom(pPath->pUnit);
	}

#ifdef VERSION_100
	D2_ASSERT(pRoomDest || (pPath->dwFlags & PATH_MISSILE_MASK) || (!PATH_FromFP16(pPath->tGameCoords.dwPrecisionX) && !PATH_FromFP16(pPath->tGameCoords.dwPrecisionY)));
#endif
	pPath->pRoom = pRoomDest;
	pPath->dwFlags |= PATH_CURRENT_ROOM_INVALID;

	if (pRoomDest)
	{
		UNITROOM_AddUnitToRoom(pPath->pUnit, pRoomDest);
		UNITROOM_RefreshUnit(pPath->pUnit);
	}
}



//1.00:  D2Common.0x10060640 (#10227)
//1.10f: D2Common.0x6FDADF00 (#10230)
BOOL __stdcall D2Common_10230(D2DynamicPathStrc* pDynamicPath, int a2, D2ActiveRoomStrc* pRoom, unsigned int a4, __int16 a5)
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

// TODO: Check name
//1.00:  Inlined
//1.10f: Inlined
//1.13c: D2Common.0x6FD5DC80
void PATH_StopMovement(D2DynamicPathStrc* pDynamicPath)
{
	PATH_RecacheRoomIfNeeded(pDynamicPath);
	pDynamicPath->dwFlags = pDynamicPath->dwFlags & (~PATH_UNKNOWN_FLAG_0x00020);
	pDynamicPath->dwPathPoints = 0;
	pDynamicPath->dwCurrentPointIdx = 0;
	pDynamicPath->tVelocityVector.nX = 0;
	pDynamicPath->tVelocityVector.nY = 0;
}

//1.00:  Inlined
//1.10f: Inlined
//1.13c: D2Common.0x6FD5DDC0
BOOL __stdcall D2Common_10231_Impl(D2DynamicPathStrc* pDynamicPath, D2ActiveRoomStrc* pRoomDest, D2PathPointStrc tDest)
{
	D2PathPointStrc tCurrentPos;
	tCurrentPos.X = pDynamicPath->tGameCoords.wPosX;
	tCurrentPos.Y = pDynamicPath->tGameCoords.wPosY;
	if (pDynamicPath->pUnit && pDynamicPath->pUnit->dwUnitType == UNIT_MISSILE)
	{
		if (tDest.X || tDest.Y)
		{
			COLLISION_TeleportUnitCollisionMask(pDynamicPath->pRoom, tCurrentPos.X, tCurrentPos.Y, pRoomDest, tDest.X, tDest.Y, pDynamicPath->dwUnitSize, pDynamicPath->nFootprintCollisionMask);
		}
		else
		{
			COLLISION_ResetMaskWithSize(pDynamicPath->pRoom, tCurrentPos.X, tCurrentPos.Y, pDynamicPath->dwUnitSize, pDynamicPath->nFootprintCollisionMask);
		}
	}
	else
	{
		if (tDest.X || tDest.Y)
		{
			COLLISION_SetUnitCollisionMask(pDynamicPath->pRoom, tCurrentPos.X, tCurrentPos.Y, pRoomDest, tDest.X, tDest.Y, pDynamicPath->dwCollisionPattern, pDynamicPath->nFootprintCollisionMask);
		}
		else
		{
			COLLISION_ResetMaskWithPattern(pDynamicPath->pRoom, tCurrentPos.X, tCurrentPos.Y, pDynamicPath->dwCollisionPattern, pDynamicPath->nFootprintCollisionMask);
		}
	}

	if (pDynamicPath->pRoom != pRoomDest)
	{
		pDynamicPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00001;
	}
	PATH_RecacheRoomAtCoordIfNeeded(pDynamicPath, pRoomDest, PATH_FP16FitToCenter(tDest.X), PATH_FP16FitToCenter(tDest.Y));

	D2_ASSERT(COORD_TEST_EQUAL(tDest, sgctZeroGameCoord) || COLLISION_GetRoomBySubTileCoordinates(pRoomDest, tDest.X, tDest.Y));
	PATH_StopMovement(pDynamicPath);
	return TRUE;
}

//1.00:  D2Common.0x10060120 (#10228)
//1.10f: D2Common.0x6FDADC20 (#10231)
//1.13c: D2Common.0x6FD5E1A0 (#10075)
int __stdcall D2Common_10231(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit_unused, D2ActiveRoomStrc* pRooms, uint16_t nX, uint16_t nY)
{
	D2_MAYBE_UNUSED(pUnit_unused);
	D2Common_10231_Impl(pDynamicPath, pRooms, D2PathPointStrc{ pDynamicPath->tGameCoords.wPosX, pDynamicPath->tGameCoords.wPosY });
	pDynamicPath->dwPathPoints = 0;
	return TRUE;
}



//1.00:  D2Common.0x1005F220
//1.10f: D2Common.0x6FDACC40
//1.13c: D2Common.0x6FD5DA40
void PATH_RecacheRoomAtCoordIfNeeded(D2DynamicPathStrc* pPath, D2ActiveRoomStrc* pHintRoom, uint32_t nPrecisionX, uint32_t nPrecisionY)
{
	if ((pPath->dwFlags & PATH_MISSILE_MASK) == 0
		|| COLLISION_GetRoomBySubTileCoordinates(pPath->pRoom, PATH_FromFP16(nPrecisionX), PATH_FromFP16(nPrecisionY)))
	{
		pPath->tGameCoords.dwPrecisionX = nPrecisionX;
		pPath->tGameCoords.dwPrecisionY = nPrecisionY;
		PATH_UpdateClientCoords(pPath);
		if (pPath->pUnit && (pPath->dwFlags & PATH_UNKNOWN_FLAG_0x00001) != 0)
		{
			PATH_RecacheRoom(pPath, pHintRoom);
		}
	}
	else
	{
		pPath->dwPathPoints = 0;
	}
}

//1.00:  Inlined
//1.10f: D2Common.0x6FDAD330
//1.13c: D2Common.0x6FD5DC80
void PATH_ResetMovement(D2DynamicPathStrc* pDynamicPath)
{
	PATH_RecacheRoomAtCoordIfNeeded(
		pDynamicPath,
		nullptr,
		PATH_FP16FitToCenter(pDynamicPath->tGameCoords.dwPrecisionX),
		PATH_FP16FitToCenter(pDynamicPath->tGameCoords.dwPrecisionY)
	);
	pDynamicPath->dwFlags &= ~PATH_UNKNOWN_FLAG_0x00020;
	pDynamicPath->dwPathPoints = 0;
	pDynamicPath->dwCurrentPointIdx = 0;
	pDynamicPath->tVelocityVector.nX = 0;
	pDynamicPath->tVelocityVector.nY = 0;
}

//1.00:  D2Common.0x100606B0 (#10229)
//1.10f: D2Common.0x6FDADF50 (#10232)
//1.13c: D2Common.0x6FD5DCE0 (#10223)
BOOL __stdcall D2Common_10232(D2DynamicPathStrc* pPath, D2UnitStrc* pUnit, D2ActiveRoomStrc* pDestRoom, int nTargetX, int nTargetY)
{
	D2_ASSERT(pUnit && (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER)); 
	
	if (COLLISION_TrySetUnitCollisionMask(
		pPath->pRoom, pPath->tGameCoords.wPosX, pPath->tGameCoords.wPosY,
		pDestRoom, nTargetX, nTargetY, 
		pPath->dwCollisionPattern, pPath->nFootprintCollisionMask, pPath->nMoveTestCollisionMask))
	{
		return FALSE;
	}
	if (pPath->pRoom != pDestRoom)
	{
		pPath->dwFlags |= PATH_UNKNOWN_FLAG_0x00001;
	}
#ifdef D2_VERSION_113C
	PATH_RecacheRoomIfNeeded(pPath, PATH_ToFP16Center(nTargetX), PATH_ToFP16Center(nTargetY), pDestRoom);
#endif
	D2_ASSERT(COLLISION_GetRoomBySubTileCoordinates(pDestRoom, nTargetX, nTargetY));
	PATH_ResetMovement(pPath);
	return TRUE;
}

//1.00:  D2Common.0x10060A60 (#10230)
//1.10f: D2Common.0x6FDAE290 (#10233)
//1.13c: D2Common.0x6FD5DB40 (#10770)
void __stdcall PATH_RecacheRoomIfNeeded(D2DynamicPathStrc* pDynamicPath)
{
	PATH_RecacheRoomAtCoordIfNeeded(pDynamicPath, nullptr, PATH_FP16FitToCenter(pDynamicPath->tGameCoords.dwPrecisionX), PATH_FP16FitToCenter(pDynamicPath->tGameCoords.dwPrecisionY));
}


//1.00:  D2Common.0x10060C40 (#10231)
//1.10f: D2Common.0x6FDAE500 (#10234)
//TODO: Find a name
BOOL __stdcall D2Common_10234(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath)
	{
		return pDynamicPath->dwFlags & PATH_UNKNOWN_FLAG_0x00008;
	}

	return FALSE;
}

//1.00:  D2Common.0x10060C60 (#10231)
//1.10f: D2Common.0x6FDAE520 (#10235)
void __stdcall D2Common_10235_PATH_UpdateRiderPath(D2UnitStrc* pRiderUnit, D2UnitStrc* pMountUnit)
{
	D2_ASSERT(pRiderUnit); // Named hRiderUnit in original game
	D2_ASSERT(pMountUnit); // Named hHorseUnit in original game

	D2DynamicPathStrc* pRiderPath = pRiderUnit->pDynamicPath;
	D2DynamicPathStrc* pMountPath = pMountUnit->pDynamicPath;

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

	D2ActiveRoomStrc* pMountRoom = pMountPath->pRoom;

	// Rider was in a different room than the mount at some point
	if (pRiderPath->pUnit && (pRiderPath->dwFlags & PATH_UNKNOWN_FLAG_0x00001))
	{
		// Try finding room from rider's current room list
		D2ActiveRoomStrc* pRidersUpToDateRoom = COLLISION_GetRoomBySubTileCoordinates(pRiderPath->pRoom, pRiderPath->tGameCoords.wPosX, pRiderPath->tGameCoords.wPosY);

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
