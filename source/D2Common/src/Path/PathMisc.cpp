#include "Path/PathMisc.h"

#include "D2Collision.h"
#include "D2Dungeon.h"
#include "Path/Path.h"
#include "Path/Path_IDAStar.h"
#include "Units/UnitRoom.h"
#include "Units/Units.h"

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

//TODO: ...
extern uint16_t word_6FDD2118[];
extern uint16_t word_6FDD211C[];
extern BOOL __fastcall sub_6FDAAD10(D2DynamicPathStrc* a1, D2PathPointStrc* a2, D2PathPointStrc a3);


int __fastcall D2COMMON_PATHMISC_FIRST_6FDAA720(D2PathInfoStrc*)
{
	return 0;
}


////D2Common.0x6FDAA720) --------------------------------------------------------
//int __fastcall D2COMMON_PATHMISC_FIRST_6FDAA720(D2PathInfoStrc* pPathInfo)
//{
//	D2PathInfoStrc *v1; // edi@1
//	signed int v2; // ebp@1
//	int v3; // edx@1
//	D2DynamicPathStrc *v4; // ecx@1
//	D2PathPointStrc v5; // esi@1
//	int result; // eax@1
//	int v7; // ebx@1
//	int v9; // eax@6
//	int v10; // eax@9
//	bool v11; // sf@9
//	unsigned __int8 v12; // of@9
//	D2PathPointStrc v13; // [sp+10h] [bp-30h]@1
//	int v14; // [sp+14h] [bp-2Ch]@1
//	D2PathPointStrc a3; // [sp+18h] [bp-28h]@1
//	D2DynamicPathStrc *v16; // [sp+1Ch] [bp-24h]@1
//	int v17; // [sp+20h] [bp-20h]@1
//	D2PathPointStrc *v18; // [sp+24h] [bp-1Ch]@2
//	int a4; // [sp+28h] [bp-18h]@5
//	int v20; // [sp+2Ch] [bp-14h]@1
//	int a2[3]; // [sp+34h] [bp-Ch]@5
//
//	v1 = pPathInfo;
//	v2 = 0;
//	v20 = 255;
//	v17 = 0;
//	v4 = (D2DynamicPathStrc *)pPathInfo->pNext;
//	v5 = *(D2PathPointStrc *)&v1->pStartCoord.X;
//
//	result = v4->dwPathPoints;
//	v7 = *(_DWORD *)&v4->PathPoints[0].X;
//	a3 = *(D2PathPointStrc *)&v1->field_4.X;
//	v16 = v4;
//	v14 = v4->dwPathPoints;
//	v13 = v5;
//	if (pPathInfo->nDistMax > 0)
//	{
//		v18 = &v4->PathPoints[result + 1];
//		do
//		{
//			if (v13.X == a3.X && v13.Y == a3.Y)
//			{
//				break;
//			}
//
//			v2 = 0;
//			D2COMMON_PATHMISC_Unknown_6FDAB790(a2, 0, v5, a3);
//
//			a2[0] = ((uint8_t)v7 + LOBYTE(a2[0])) & 7;
//			a2[1] = ((uint8_t)v7 + LOBYTE(a2[1])) & 7;
//			a2[2] = ((uint8_t)v7 + LOBYTE(a2[2])) & 7;
//
//			if (D2COMMON_PATHMISC_Unknown_6FDAA880(v1, a2, v5, &a4))
//			{
//				v9 = a4;
//				if ((((uint8_t)a4 - 4) & 7) != v20)
//					continue;
//			}
//
//			result = v14;
//			v4 = v16;
//			goto LABEL_13;
//
//			v13.X += word_6FDD2118[4 * a4];
//			v13.Y += word_6FDD211C[4 * a4];
//
//			if (a4 != v20)
//			{
//				v2 = 1;
//				*(_DWORD *)&v18->X = v5;
//				++v18;
//				v9 = a4;
//				++v14;
//			}
//
//			v5 = v13;
//			v20 = v9;
//			v10 = v17 + 1;
//			v17 = v10;
//			v12 = __OFSUB__(v10, pPathInfo->nDistMax);
//			v11 = v10 - pPathInfo->nDistMax < 0;
//			v4 = v16;
//			result = v14;
//		}
//		while (v11 ^ v12);
//
//		if (v2)
//		{
//			goto LABEL_15;
//		}
//
//LABEL_13:
//		if (v17)
//		{
//			*(_DWORD *)&v4->PathPoints[result++ + 1].X = v5;
//		}
//	}
//
//LABEL_15:
//	v4->dwPathPoints = result;
//	return result;
//}

//D2Common.0x6FDAA880
BOOL __fastcall sub_6FDAA880(D2PathInfoStrc* pPathInfo, int* pTestDir, D2PathPointStrc pPoint, int* pDirection)
{
	D2UnitStrc* pUnit = pPathInfo->pDynamicPath->pUnit;

	D2_ASSERT(pUnit && (pUnit->dwUnitType == UNIT_PLAYER || pUnit->dwUnitType == UNIT_MONSTER));

	if (!COLLISION_CheckMaskWithPattern2(pPathInfo->pRoom, (pPoint.X + word_6FDD2118[4 * pTestDir[0]]), (pPoint.Y + word_6FDD211C[4 * pTestDir[0]]), pPathInfo->nCollisionPattern, pPathInfo->nCollisionType))
	{
		*pDirection = pTestDir[0];
		return TRUE;
	}

	if (!COLLISION_CheckMaskWithPattern2(pPathInfo->pRoom, (pPoint.X + word_6FDD2118[4 * pTestDir[1]]), (pPoint.Y + word_6FDD211C[4 * pTestDir[1]]), pPathInfo->nCollisionPattern, pPathInfo->nCollisionType))
	{
		*pDirection = pTestDir[1];
		return TRUE;
	}
#define PATH_DIR_NULL 255
	D2_ASSERT(pTestDir[2] != PATH_DIR_NULL);

	if (!COLLISION_CheckMaskWithPattern2(pPathInfo->pRoom, (pPoint.X + word_6FDD2118[4 * pTestDir[2]]), (pPoint.Y + word_6FDD211C[4 * pTestDir[2]]), pPathInfo->nCollisionPattern, pPathInfo->nCollisionType))
	{
		*pDirection = pTestDir[2];
		return TRUE;
	}

	return FALSE;
}

//D2Common.0x6FDAA9F0) --------------------------------------------------------
int __fastcall sub_6FDAA9F0(D2PathInfoStrc *pPathInfo)
{
//	D2PathInfoStrc *v1; // ebp@1
//	D2DynamicPathStrc *v2; // esi@1
//	D2PathPointStrc *v3; // ecx@1
//	int result; // eax@2
//	D2PathPointStrc v5; // edi@7
//	int v6; // edx@9
//	int v7; // eax@9
//	D2PathPointStrc v8; // ecx@9
//	signed int v9; // ebx@9
//	int v10; // edx@10
//	int v11; // eax@15
//	int v12; // ecx@19
//	int v13; // eax@24
//	D2PathPointStrc a1; // [sp+8h] [bp-30h]@7
//	D2PathPointStrc pGameCoord; // [sp+Ch] [bp-2Ch]@3
//	int v16; // [sp+10h] [bp-28h]@9
//	int v17; // [sp+14h] [bp-24h]@9
//	int v18; // [sp+18h] [bp-20h]@9
//	int v19; // [sp+1Ch] [bp-1Ch]@10
//	int a4; // [sp+20h] [bp-18h]@14
//	D2PathPointStrc v21; // [sp+24h] [bp-14h]@16
//	int v22; // [sp+28h] [bp-10h]@9
//	int v23; // [sp+2Ch] [bp-Ch]@14
//
//	v1 = pPathInfo;
//	v2 = pPathInfo->pDynamicPath;
//	v3 = *(D2PathPointStrc **)&v2->PathPoints[0].X;
//	v2->unk0x24 = 0;
//	v2->dwPathPoints = 0;
//	if (v3)
//	{
//		result = D2COMMON_PATHMISC_FIRST_6FDAA720(v1);
//	}
//	else
//	{
//		pGameCoord = *(D2PathPointStrc *)&v1->field_4.X;
//		*(_DWORD *)&v2->PathPoints[1].X = pGameCoord;
//		if (D2COMMON_PATHMISC_Unknown_6FDAABF0(v2, &pGameCoord))
//		{
//			*(_DWORD *)&v2->PathPoints[v2->dwPathPoints + 1].X = pGameCoord;
//			result = v2->dwPathPoints + 1;
//			v2->dwPathPoints = result;
//		}
//		else
//		{
//			if (sub_6FDABA50(pGameCoord, v1->field_4) > v1->field_14)
//			{
//				v5 = *(D2PathPointStrc *)&v1->pStartCoord.X;
//				a1 = *(D2PathPointStrc *)&v1->pStartCoord.X;
//				if (*(_DWORD *)&v1->pStartCoord.X != pGameCoord)
//				{
//					*(_DWORD *)&v2->PathPoints[v2->dwPathPoints++ + 1].X = pGameCoord;
//					v5 = pGameCoord;
//					a1 = pGameCoord;
//				}
//				v6 = v1->nDistMax;
//				v7 = v2->dwPathPoints;
//				v8 = *(D2PathPointStrc *)&v1->field_4.X;
//				v9 = 255;
//				v16 = v2->dwPathPoints;
//				v22 = v6;
//				v17 = 0;
//				pGameCoord = v8;
//				v18 = 0;
//				if (v6 > 0)
//				{
//					v10 = v7;
//					v19 = (int)&v2->PathPoints[v7 + 1];
//					while (1)
//					{
//						if (a1.X == v8.X)
//						{
//							LOWORD(v10) = a1.Y;
//							if (a1.Y == pGameCoord.Y)
//								break;
//						}
//						v17 = 0;
//						D2COMMON_PATHMISC_Unknown_6FDAB790(&v23, v10, v5, v8);
//						if (!D2COMMON_PATHMISC_Unknown_6FDAA880(v1, &v23, v5, &a4)
//							|| (v11 = a4, (((_BYTE)a4 - 4) & 7) == v9))
//							goto LABEL_23;
//						LOWORD(v10) = word_6FDD2118[4 * a4];
//						a1.X += v10;
//						a1.Y += word_6FDD211C[4 * a4];
//						v21 = v5;
//						if (a4 != v9)
//						{
//							if (v5.X != v1->pStartCoord.X || (LOWORD(v10) = v21.Y, v21.Y != v1->pStartCoord.Y))
//							{
//								v12 = v19;
//								*(_DWORD *)v19 = v5;
//								++v16;
//								v19 = v12 + 4;
//							}
//							v17 = 1;
//						}
//						v5 = a1;
//						v9 = v11;
//						++v18;
//						if (v18 >= v22)
//							break;
//						v8 = pGameCoord;
//					}
//					if (v17)
//						goto LABEL_25;
//LABEL_23:
//					if (v18)
//					{
//						v13 = v16;
//						*(_DWORD *)&v2->PathPoints[v16 + 1].X = v5;
//						v16 = v13 + 1;
//					}
//				}
//LABEL_25:
//				result = v16;
//				v2->dwPathPoints = v16;
//			}
//			else
//			{
//				*(_DWORD *)&v2->PathPoints[v2->dwPathPoints + 1].X = pGameCoord;
//				result = v2->dwPathPoints + 1;
//				v2->dwPathPoints = result;
//			}
//		}
//	}
//	return result;
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

////D2Common.0x6FDAAD10) --------------------------------------------------------
//BOOL __fastcall sub_6FDAAD10(D2DynamicPathStrc* a1, D2PathPointStrc* a2, D2PathPointStrc a3)
//{
//	int v3; // ebp@1
//	D2RoomStrc*v4; // edi@1
//	int v5; // eax@1
//	int v6; // ecx@1
//	int v7; // edx@1
//	int v8; // ebx@3
//	int v9; // edx@3
//	int v10; // edx@5
//	int v11; // ebx@6
//	int v12; // ebx@7
//	unsigned __int16 v13; // si@8
//	int v15; // ebx@12
//	unsigned __int16 v16; // si@23
//	unsigned __int16 v17; // si@37
//	int v18; // [sp+10h] [bp-30h]@1
//	int v19; // [sp+14h] [bp-2Ch]@12
//	int v20; // [sp+14h] [bp-2Ch]@22
//	int v21; // [sp+14h] [bp-2Ch]@27
//	int v22; // [sp+14h] [bp-2Ch]@36
//	int v23; // [sp+18h] [bp-28h]@1
//	int v24; // [sp+1Ch] [bp-24h]@1
//	int v25; // [sp+20h] [bp-20h]@12
//	int v26; // [sp+20h] [bp-20h]@27
//	int v27; // [sp+20h] [bp-20h]@36
//	int v28; // [sp+24h] [bp-1Ch]@3
//	int v29; // [sp+28h] [bp-18h]@5
//	int v30; // [sp+30h] [bp-10h]@1
//	int v31; // [sp+38h] [bp-8h]@1
//
//	v3 = *(_DWORD *)(a1 + 72);
//	v4 = *(D2RoomStrc**)(a1 + 28);
//	v23 = *(_DWORD *)(a1 + 80);
//	v31 = *(_DWORD *)a2;
//	v24 = a2;
//	v6 = (unsigned __int16)(*(_DWORD *)a2 >> 16) - HIWORD(a3);
//	v18 = a3;
//	v30 = a3;
//	v7 = (*(_DWORD *)a2 & 0xFFFF) - (unsigned __int16)a3;
//	v5 = v7;
//	if (v7 < 0)
//		v7 = -v7;
//	v8 = v7 + 1;
//	v9 = HIWORD(v31) - HIWORD(a3);
//	v28 = v8;
//	if (v6 < 0)
//		v9 = -v6;
//	v10 = v9 + 1;
//	v29 = v10;
//	if (v8 > v10)
//	{
//		v11 = 0;
//		if (v5)
//		{
//			LOBYTE(v11) = v5 >= 0;
//			v19 = 2 * v11 - 1;
//			v25 = 2 * (v6 >= 0) - 1;
//			v15 = v10;
//			if ((_WORD)a3 != (_WORD)v31)
//			{
//				while (1)
//				{
//					LOWORD(v30) = v19 + v30;
//					if (COLLISION_CheckMaskWithPattern2(v4, (unsigned __int16)v30, HIWORD(v30), v3, v23))
//						break;
//					v15 += v29;
//					if (v15 >= v28)
//					{
//						v15 -= v28;
//						HIWORD(v30) += v25;
//						if (v15 > 0)
//						{
//							if (COLLISION_CheckMaskWithPattern2(v4, (unsigned __int16)v30, HIWORD(v30), v3, v23))
//								goto LABEL_41;
//						}
//					}
//					v18 = v30;
//					if ((_WORD)v30 == (_WORD)v31)
//						return 1;
//				}
//				*(_DWORD *)v24 = v18;
//				return 0;
//			}
//		}
//		else
//		{
//			LOBYTE(v11) = v6 >= 0;
//			v12 = 2 * v11 - 1;
//			if (HIWORD(a3) != HIWORD(v31))
//			{
//				v13 = HIWORD(a3);
//				while (1)
//				{
//					v13 += v12;
//					HIWORD(v30) = v13;
//					if (COLLISION_CheckMaskWithPattern2(v4, (unsigned __int16)a3, v13, v3, v23))
//						break;
//					v18 = v30;
//					if (v13 == HIWORD(v31))
//						return 1;
//				}
//LABEL_35:
//				*(_DWORD *)v24 = v18;
//				return 0;
//			}
//		}
//		return 1;
//	}
//	if (v8 >= v10)
//	{
//		v22 = 2 * (v5 >= 0) - 1;
//		v27 = 2 * (v6 >= 0) - 1;
//		if ((_WORD)a3 == (_WORD)v31)
//			return 1;
//		v17 = a3;
//		while (1)
//		{
//			v17 += v22;
//			HIWORD(v30) += v27;
//			LOWORD(v30) = v17;
//			if (COLLISION_CheckMaskWithPattern2(v4, v17, HIWORD(v30), v3, v23))
//				break;
//			v18 = v30;
//			if (v17 == (_WORD)v31)
//				return 1;
//		}
//		goto LABEL_41;
//	}
//	if (HIWORD(v31) == HIWORD(a3))
//	{
//		v20 = 2 * (v5 >= 0) - 1;
//		if ((_WORD)a3 == (_WORD)v31)
//			return 1;
//		v16 = a3;
//		while (1)
//		{
//			v16 += v20;
//			LOWORD(v30) = v16;
//			if (COLLISION_CheckMaskWithPattern2(v4, v16, HIWORD(a3), v3, v23))
//				break;
//			v18 = v30;
//			if (v16 == (_WORD)v31)
//				return 1;
//		}
//LABEL_41:
//		*(_DWORD *)v24 = v18;
//		return 0;
//	}
//	v21 = 2 * (v5 >= 0) - 1;
//	v26 = 2 * (v6 >= 0) - 1;
//	if (HIWORD(a3) == HIWORD(v31))
//		return 1;
//	while (1)
//	{
//		HIWORD(v30) += v26;
//		if (COLLISION_CheckMaskWithPattern2(v4, (unsigned __int16)v30, HIWORD(v30), v3, v23))
//			break;
//		v8 += v28;
//		if (v8 >= v29)
//		{
//			v8 -= v29;
//			LOWORD(v30) = v21 + v30;
//			if (v8 > 0)
//			{
//				if (COLLISION_CheckMaskWithPattern2(v4, (unsigned __int16)v30, HIWORD(v30), v3, v23))
//					goto LABEL_35;
//			}
//		}
//		v18 = v30;
//		if (HIWORD(v30) == HIWORD(v31))
//			return 1;
//	}
//	*(_DWORD *)v24 = v18;
//	return 0;
//}

//D2Common.0x6FDAB0B0
int __fastcall sub_6FDAB0B0(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->unk0x24 = 0;
	return sub_6FDAA9F0(pPathInfo);
}

//D2Common.0x6FDAB0C0
//TODO: result + v5
int __fastcall sub_6FDAB0C0(D2PathInfoStrc* pPathInfo)
{
	int result = 0;
	int v5 = 0;

	pPathInfo->pDynamicPath->unk0x24 = 0;

	pPathInfo->pDynamicPath->dwUnitTypeRelated = -4;

	result = D2COMMON_PATHMISC_FIRST_6FDAA720(pPathInfo);
	if (!result)
	{
		return 0;
	}

	pPathInfo->pDynamicPath->dwPathPoints = result;

	pPathInfo->pDynamicPath->dwUnitTypeRelated = -2;

	v5 = D2COMMON_PATHMISC_FIRST_6FDAA720(pPathInfo) - result;
	if (!v5)
	{
		pPathInfo->pDynamicPath->dwUnitTypeRelated = 2;

		v5 = D2COMMON_PATHMISC_FIRST_6FDAA720(pPathInfo) - result;
	}

	pPathInfo->pDynamicPath->dwUnitTypeRelated = -4;

	return result + v5;
}

////D2Common.0x6FDAB130) --------------------------------------------------------
//signed int __thiscall sub_6FDAB130(int this)
//{
//	int v1; // esi@1
//	int v2; // edi@1
//	int v3; // ebx@1
//	int v4; // edi@2
//	int v5; // eax@4
//	int v6; // ecx@6
//	int v7; // eax@6
//	signed int result; // eax@9
//
//	v1 = this;
//	v2 = *(_DWORD *)(this + 48);
//	*(_DWORD *)(v2 + 36) = 0;
//	*(_DWORD *)(v2 + 40) = 0;
//	v3 = sub_6FDAA9F0(this);
//	if (!v3
//		|| (v4 = *(_DWORD *)(v2 + 4 * v3 + 152), sub_6FDABA50(v4, *(_DWORD *)(v1 + 4)) > *(_BYTE *)(v1 + 20))
//		|| v4 == __PAIR__(*(_WORD *)(v1 + 2), *(_WORD *)v1))
//	{
//		v5 = *(_WORD *)v1 - *(_WORD *)(v1 + 4);
//		if (v5 < 0)
//			v5 = -v5;
//		v6 = v5;
//		v7 = *(_WORD *)(v1 + 2) - *(_WORD *)(v1 + 6);
//		if (v7 < 0)
//			v7 = -v7;
//		if (v6 * v6 + v7 * v7 <= 324)
//		{
//			result = D2Common_PATH_First_6FDA69E0(v1);
//			if (result)
//				return result;
//			if (v3)
//				v3 = sub_6FDAA9F0(v1);
//		}
//	}
//	return v3;
//}

////D2Common.0x6FDAB1E0) --------------------------------------------------------
//signed int __thiscall sub_6FDAB1E0(int this)
//{
//	int v1; // edi@1
//	int v2; // esi@1
//	signed int result; // eax@2
//	int v4; // [sp+0h] [bp-4h]@1
//
//	v4 = this;
//	v1 = this;
//	v2 = *(_DWORD *)(this + 48);
//	*(_DWORD *)(v2 + 36) = 0;
//	v4 = *(_DWORD *)(this + 4);
//	*(_DWORD *)(v2 + 156) = v4;
//	sub_6FDAABF0(v2, (int)&v4);
//	if (__PAIR__(*(_WORD *)(v1 + 2), *(_WORD *)v1) == v4)
//	{
//		result = 0;
//	}
//	else
//	{
//		*(_DWORD *)(v2 + 156) = v4;
//		result = 1;
//		*(_DWORD *)(v2 + 40) = 1;
//	}
//	return result;
//}

//D2Common.0x6FDAB240
int __fastcall sub_6FDAB240(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->pDynamicPath->unk0x24 = 0;
	pPathInfo->pDynamicPath->PathPoints[0] = pPathInfo->field_4;
	sub_6FDAABF0(pPathInfo->pDynamicPath, &pPathInfo->pDynamicPath->PathPoints[0]);
	return 1;
}

////D2Common.0x6FDAB270) --------------------------------------------------------
//signed int __thiscall sub_6FDAB270(int this)
//{
//	int v1; // ebx@1
//	int v2; // ebp@1
//	int v3; // eax@1
//	signed int v4; // ecx@2
//	D2DynamicPathStrc*v5; // eax@5
//	int v6; // esi@6
//	int v7; // ecx@9
//	signed int v8; // eax@9
//	D2DynamicPathStrc*v9; // ecx@12
//	int v10; // eax@13
//	int v11; // ecx@16
//	int v12; // esi@16
//	signed int v13; // edi@16
//	int v14; // eax@18
//	int v15; // esi@26
//	signed int result; // eax@27
//	unsigned int v17; // [sp+Ch] [bp-Ch]@1
//	int v18; // [sp+10h] [bp-8h]@2
//
//	v1 = this;
//	v2 = *(_DWORD *)(this + 48);
//	v17 = ((unsigned int)*(_BYTE *)(v2 + 144) >> 1) + 1;
//	v3 = *(_DWORD *)(v2 + 88);
//	if (v3)
//	{
//		v18 = *(_DWORD *)this;
//		v4 = *(_DWORD *)v3;
//		if (*(_DWORD *)v3 == 2 || v4 > 3 && v4 <= 5)
//		{
//			v6 = *(_DWORD *)(*(_DWORD *)(v3 + 44) + 12);
//		}
//		else
//		{
//			v5 = *(D2DynamicPathStrc**)(v3 + 44);
//			if (v5)
//				v6 = PATH_GetXPosition(v5);
//			else
//				v6 = 0;
//		}
//		v7 = *(_DWORD *)(v2 + 88);
//		v8 = *(_DWORD *)v7;
//		if (*(_DWORD *)v7 == 2 || v8 > 3 && v8 <= 5)
//		{
//			v10 = *(_DWORD *)(*(_DWORD *)(v7 + 44) + 16);
//		}
//		else
//		{
//			v9 = *(D2DynamicPathStrc**)(v7 + 44);
//			if (v9)
//				v10 = PATH_GetYPosition(v9);
//			else
//				v10 = 0;
//		}
//		v11 = *(_WORD *)v1 - v6;
//		v12 = *(_WORD *)(v1 + 2) - v10;
//		v13 = v12;
//		if (v12 < 0)
//			v13 = -v12;
//		v14 = v11;
//		if (v11 < 0)
//			v14 = -v11;
//		if (v14 > v13)
//			v13 = v14;
//		if (v13)
//		{
//			v11 = (signed int)(v11 * v17) / v13;
//			v12 = (signed int)(v12 * v17) / v13;
//		}
//		LOWORD(v18) = v11 + v18;
//		HIWORD(v18) += v12;
//	}
//	else
//	{
//		v18 = *(_DWORD *)(this + 4);
//	}
//	*(_DWORD *)(v2 + 156) = v18;
//	sub_6FDAAD10(v2, (int)&v18, *(_DWORD *)v1);
//	v15 = v18;
//	D2COMMON_10170_PathSetTargetPos((D2DynamicPathStrc*)v2, (unsigned __int16)v18, HIWORD(v18));
//	if (v18 == __PAIR__(*(_WORD *)(v1 + 2), *(_WORD *)v1))
//	{
//		result = 0;
//	}
//	else
//	{
//		result = 1;
//		*(_DWORD *)(v2 + 156) = v15;
//		*(_DWORD *)(v2 + 40) = 1;
//	}
//	return result;
//}

////D2Common.0x6FDAB3C0) --------------------------------------------------------
//signed int __thiscall sub_6FDAB3C0(int this)
//{
//	int v1; // ebp@1
//	int v2; // edi@1
//	int v3; // ebx@1
//	float v4; // ST1C_4@2
//	signed __int64 v5; // qax@2
//	unsigned int v6; // esi@2
//	int v7; // ecx@2
//	unsigned int v8; // eax@2
//	unsigned int v9; // ecx@2
//	signed int result; // eax@5
//	signed int v11; // [sp+10h] [bp-20h]@1
//	signed int v12; // [sp+14h] [bp-1Ch]@1
//	int v13; // [sp+1Ch] [bp-14h]@1
//	int v14; // [sp+20h] [bp-10h]@1
//	unsigned int v15; // [sp+24h] [bp-Ch]@1
//	int v16; // [sp+28h] [bp-8h]@1
//	int v17; // [sp+2Ch] [bp-4h]@1
//
//	v1 = this;
//	v2 = 0;
//	v16 = *(_DWORD *)this;
//	v13 = this;
//	v17 = *(_DWORD *)(this + 4);
//	v14 = *(_DWORD *)this;
//	v15 = *(_DWORD *)(this + 4);
//	v12 = 0;
//	v11 = 0;
//	v3 = this + 158;
//	do
//	{
//		v2 += 16;
//		v12 += 9600;
//		v4 = (double)v12;
//		v5 = (signed __int64)(Fog_10083(v2) * v4);
//		v6 = v16 + v5;
//		v8 = v17 + (unsigned __int64)(signed __int64)(Fog_10084(v7, HIDWORD(v5), v2) * v4);
//		v9 = v6 >> 16;
//		if (HIWORD(v6) != HIWORD(v14) || (v1 = v13, v8 >> 16 != v15 >> 16))
//		{
//			*(_WORD *)(v3 - 2) = v9;
//			v15 = v8;
//			HIWORD(v14) = HIWORD(v6);
//			*(_WORD *)v3 = HIWORD(v8);
//			++v11;
//			v3 += 4;
//		}
//		result = v11;
//	}
//	while (v11 < 77);
//	*(_DWORD *)(v1 + 40) = v11;
//	return result;
//}

////D2Common.0x6FDAB4A0) --------------------------------------------------------
//int __fastcall sub_6FDAB4A0(int a1, int a2)
//{
//	int v2; // ebp@1
//	unsigned int v3; // eax@1
//	int v4; // esi@1
//	__int16 v5; // dx@1
//	int v6; // ST04_4@1
//	unsigned int v7; // edi@1
//	int v8; // ecx@2
//	char v9; // di@3
//	signed __int64 v10; // qax@3
//	int result; // eax@4
//	int v12; // [sp+10h] [bp-98h]@1
//	unsigned int v13; // [sp+14h] [bp-94h]@1
//	unsigned int v14; // [sp+18h] [bp-90h]@2
//	int v15; // [sp+1Ch] [bp-8Ch]@1
//	int v16; // [sp+20h] [bp-88h]@1
//	int v17; // [sp+24h] [bp-84h]@1
//	int v18; // [sp+28h] [bp-80h]@1
//	int v19; // [sp+2Ch] [bp-7Ch]@1
//	int v20; // [sp+30h] [bp-78h]@1
//	int v21; // [sp+34h] [bp-74h]@1
//	int v22; // [sp+38h] [bp-70h]@1
//	int v23; // [sp+3Ch] [bp-6Ch]@1
//	int v24; // [sp+40h] [bp-68h]@1
//	int v25; // [sp+44h] [bp-64h]@1
//	int v26; // [sp+48h] [bp-60h]@1
//	int v27; // [sp+4Ch] [bp-5Ch]@1
//	int v28; // [sp+50h] [bp-58h]@1
//	int v29; // [sp+54h] [bp-54h]@1
//	int v30; // [sp+58h] [bp-50h]@1
//	int v31; // [sp+5Ch] [bp-4Ch]@1
//	int v32; // [sp+60h] [bp-48h]@1
//	int v33; // [sp+64h] [bp-44h]@1
//	int v34; // [sp+68h] [bp-40h]@1
//	int v35; // [sp+6Ch] [bp-3Ch]@1
//	int v36; // [sp+70h] [bp-38h]@1
//	int v37; // [sp+74h] [bp-34h]@1
//	int v38; // [sp+78h] [bp-30h]@1
//	int v39; // [sp+7Ch] [bp-2Ch]@1
//	int v40; // [sp+80h] [bp-28h]@1
//	int v41; // [sp+84h] [bp-24h]@1
//	int v42; // [sp+88h] [bp-20h]@1
//	int v43; // [sp+8Ch] [bp-1Ch]@1
//	int v44; // [sp+90h] [bp-18h]@1
//	int v45; // [sp+94h] [bp-14h]@1
//	int v46; // [sp+98h] [bp-10h]@1
//	int v47; // [sp+9Ch] [bp-Ch]@1
//
//	v2 = a1;
//	v17 = 1;
//	v20 = 1;
//	v23 = 1;
//	v26 = 1;
//	v29 = 1;
//	v32 = 1;
//	v35 = 1;
//	v38 = 1;
//	v41 = 1;
//	v44 = 1;
//	v46 = 1;
//	v3 = *(_BYTE *)(a1 + 145);
//	v4 = a2;
//	v5 = *(_WORD *)(a1 + 6);
//	v15 = -1;
//	v18 = -1;
//	v21 = -1;
//	v24 = -1;
//	v27 = -1;
//	v30 = -1;
//	v33 = -1;
//	v36 = -1;
//	v39 = -1;
//	v42 = -1;
//	v45 = -1;
//	LOWORD(v12) = *(_WORD *)(a1 + 2);
//	HIWORD(v12) = v5;
//	v6 = *(_DWORD *)(a1 + 16);
//	v16 = 0;
//	v19 = 0;
//	v22 = 0;
//	v25 = 0;
//	v28 = 0;
//	v31 = 0;
//	v34 = 0;
//	v37 = 0;
//	v40 = 0;
//	v43 = 0;
//	v13 = 0;
//	v7 = v3 >> 1;
//	sub_6FDAB790(&v47, v12, v6);
//	if ((signed int)v7 > 0)
//	{
//		v8 = v2 + 156;
//		v14 = v7;
//		v13 = v7;
//		do
//		{
//			v9 = v47;
//			v10 = *(_DWORD *)(v4 + 4) + 1791398085i64 * *(_DWORD *)v4;
//			*(_QWORD *)v4 = v10;
//			LODWORD(v10) = *(&v15 + (v10 & 0x1F));
//			*(_DWORD *)v8 = v12;
//			v8 += 4;
//			LODWORD(v10) = (v9 + (_BYTE)v10) & 7;
//			HIWORD(v12) += 2 * word_6FDD211C[4 * (signed int)v10];
//			LOWORD(v12) = 2 * word_6FDD2118[4 * (signed int)v10] + v12;
//			--v14;
//		}
//		while (v14);
//	}
//	*(_DWORD *)(v2 + 4 * v13 + 156) = v12;
//	result = v13 + 1;
//	*(_DWORD *)(v2 + 40) = v13 + 1;
//	return result;
//}

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
void __fastcall sub_6FDAB790(int* pTestDir, int nUnused, D2PathPointStrc pPoint1, D2PathPointStrc pPoint2)
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

////D2Common.0x6FDAB890) --------------------------------------------------------
//signed __int64 __thiscall sub_6FDAB890(int this)
//{
//	int v1; // esi@1
//	int v2; // eax@4
//	uint8_t v3; // al@7
//	signed int v4; // ecx@7
//	float v5; // ST10_4@7
//	int v6; // edi@7
//	signed __int64 v7; // qax@7
//	int v8; // ecx@7
//	signed __int64 result; // qax@7
//
//	v1 = this;
//	if (!this)
//	{
//		FOG_Assertion("ptPath", __FILE__, __LINE__);
//		exit(-1);
//	}
//	v2 = *(_DWORD *)(this + 88);
//	if (!v2)
//	{
//		FOG_Assertion(//			"ptPath->hTarget", //			__FILE__, __LINE__//			491);
//		exit(-1);
//	}
//	v3 = D2COMMON_10158_PATH_GetDirection(*(D2DynamicPathStrc**)(v2 + 44));
//	v4 = *(_BYTE *)(v1 + 104);
//	v5 = (double)v4;
//	v6 = (8 * v3 - ((_WORD)v4 << 8)) & 0x1FF;
//	v7 = (signed __int64)(Fog_10083((8 * v3 - ((_WORD)v4 << 8)) & 0x1FF) * v5);
//	*(_WORD *)(v1 + 16) += v7;
//	result = (signed __int64)(Fog_10084(v8, HIDWORD(v7), v6) * v5);
//	*(_WORD *)(v1 + 18) += result;
//	return result;
//}

////D2Common.0x6FDAB940) --------------------------------------------------------
//char __fastcall sub_6FDAB940(int a1, int a2)
//{
//	int v2; // edi@1
//	int v3; // esi@1
//	int v4; // ecx@4
//	signed int v5; // eax@7
//	D2DynamicPathStrc*v6; // ecx@10
//	int v7; // eax@11
//	int v8; // ecx@14
//	signed int v9; // eax@14
//	D2DynamicPathStrc*v10; // ecx@17
//	int v11; // eax@18
//	signed __int64 v12; // qax@21
//	uint8_t v13; // al@22
//	signed int v14; // ecx@22
//	float v15; // ST14_4@22
//	int v16; // edi@22
//	signed __int64 v17; // qax@22
//	int v18; // ecx@22
//
//	v2 = a2;
//	v3 = a1;
//	if (!a2)
//	{
//		FOG_Assertion("ptPath", __FILE__, __LINE__);
//		exit(-1);
//	}
//	v4 = *(_DWORD *)(a2 + 88);
//	if (!v4)
//	{
//		FOG_Assertion(//			"ptPath->hTarget", //			__FILE__, __LINE__//			514);
//		exit(-1);
//	}
//	v5 = *(_DWORD *)v4;
//	if (*(_DWORD *)v4 == 2 || v5 > 3 && v5 <= 5)
//	{
//		v7 = *(_DWORD *)(*(_DWORD *)(v4 + 44) + 12);
//	}
//	else
//	{
//		v6 = *(D2DynamicPathStrc**)(v4 + 44);
//		if (v6)
//			LOWORD(v7) = PATH_GetXPosition(v6);
//		else
//			LOWORD(v7) = 0;
//	}
//	*(_WORD *)v3 = v7;
//	v8 = *(_DWORD *)(v2 + 88);
//	v9 = *(_DWORD *)v8;
//	if (*(_DWORD *)v8 == 2 || v9 > 3 && v9 <= 5)
//	{
//		v11 = *(_DWORD *)(*(_DWORD *)(v8 + 44) + 16);
//	}
//	else
//	{
//		v10 = *(D2DynamicPathStrc**)(v8 + 44);
//		if (v10)
//			LOWORD(v11) = PATH_GetYPosition(v10);
//		else
//			LOWORD(v11) = 0;
//	}
//	*(_WORD *)(v3 + 2) = v11;
//	LOBYTE(v12) = *(_BYTE *)(v2 + 104);
//	if ((_BYTE)v12)
//	{
//		v13 = D2COMMON_10158_PATH_GetDirection(*(D2DynamicPathStrc**)(*(_DWORD *)(v2 + 88) + 44));
//		v14 = *(_BYTE *)(v2 + 104);
//		v15 = (double)v14;
//		v16 = (8 * v13 - ((_WORD)v14 << 8)) & 0x1FF;
//		v17 = (signed __int64)(Fog_10083((8 * v13 - ((_WORD)v14 << 8)) & 0x1FF) * v15);
//		*(_WORD *)v3 += v17;
//		v12 = (signed __int64)(Fog_10084(v18, HIDWORD(v17), v16) * v15);
//		*(_WORD *)(v3 + 2) += v12;
//	}
//	return v12;
//}

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

////D2Common.0x6FDABAC0) --------------------------------------------------------
//int __fastcall sub_6FDABAC0(int a1, int a2, int a3, int a4, int a5, signed int a6, int a7)
//{
//	int v7; // ebp@1
//	int v8; // edx@1
//	int v9; // esi@1
//	int* v10; // ebx@1
//	int v11; // ST44_4@1
//	int v12; // eax@1
//	D2RoomStrc*v13; // ebp@3
//	int v14; // eax@3
//	int v15; // ecx@7
//	__int16 v16; // di@7
//	__int16 v17; // ax@7
//	__int16 v18; // si@7
//	unsigned __int16 v19; // ax@7
//	int v20; // ecx@8
//	__int16 v21; // ax@8
//	unsigned __int16 v22; // ax@8
//	int v23; // ecx@9
//	__int16 v24; // ax@9
//	unsigned __int16 v25; // ax@9
//	int v26; // ecx@10
//	int v27; // eax@10
//	int v28; // ecx@10
//	int v29; // eax@13
//	int v30; // edx@18
//	int v31; // esi@18
//	int v32; // edi@18
//	int v33; // eax@18
//	signed int v34; // ecx@21
//	int v35; // eax@26
//	int v36; // edx@33
//	int v37; // ebp@33
//	int v38; // edi@33
//	int* v39; // eax@38
//	int result; // eax@42
//	int v41; // ebp@45
//	int v42; // ebp@45
//	int v43; // ST28_4@45
//	int v44; // ST30_4@45
//	int v45; // ecx@47
//	int v46; // eax@47
//	int v47; // ST30_4@47
//	int v48; // ecx@47
//	int v49; // edx@47
//	int v50; // eax@50
//	int v51; // edx@50
//	int v52; // ecx@50
//	__int16 *v53; // esi@51
//	int v54; // ecx@57
//	int v55; // [sp+10h] [bp-6BCh]@1
//	int* v56; // [sp+1Ch] [bp-6B0h]@1
//	int v57; // [sp+24h] [bp-6A8h]@1
//	int v58; // [sp+2Ch] [bp-6A0h]@3
//	int v59; // [sp+30h] [bp-69Ch]@3
//	int v60; // [sp+3Ch] [bp-690h]@1
//	int v61; // [sp+40h] [bp-68Ch]@1
//	int v62; // [sp+44h] [bp-688h]@1
//	__int16 v63; // [sp+48h] [bp-684h]@1
//	__int16 v64; // [sp+4Ah] [bp-682h]@1
//	int v65; // [sp+4Ch] [bp-680h]@1
//	int v66; // [sp+50h] [bp-67Ch]@1
//	int v67; // [sp+54h] [bp-678h]@1
//	int v68; // [sp+58h] [bp-674h]@1
//	int v69; // [sp+5Ch] [bp-670h]@1
//	__int16 v70; // [sp+60h] [bp-66Ch]@1
//	char v71; // [sp+62h] [bp-66Ah]@1
//	__int16 v72; // [sp+382h] [bp-34Ah]@1
//	int v73; // [sp+384h] [bp-348h]@1
//	int v74; // [sp+388h] [bp-344h]@1
//	int v75; // [sp+38Ch] [bp-340h]@1
//	__int16 v76; // [sp+390h] [bp-33Ch]@1
//	__int16 v77; // [sp+392h] [bp-33Ah]@1
//	int v78; // [sp+394h] [bp-338h]@1
//	int v79; // [sp+398h] [bp-334h]@1
//	int v80; // [sp+39Ch] [bp-330h]@1
//	int v81; // [sp+3A0h] [bp-32Ch]@1
//	int v82; // [sp+3A4h] [bp-328h]@1
//	__int16 v83; // [sp+3A8h] [bp-324h]@1
//	char v84; // [sp+3AAh] [bp-322h]@1
//	__int16 v85; // [sp+6CAh] [bp-2h]@1
//
//	v7 = a2;
//	v8 = a1;
//	v9 = 0;
//	v74 = 0;
//	v76 = 0;
//	v77 = 0;
//	v78 = 0;
//	v79 = (int)&unk_6FDD2480;
//	v80 = (int)&unk_6FDD2500;
//	v81 = 0;
//	v82 = 0;
//	v83 = 0;
//	v61 = 0;
//	memset(&v84, 0, 0x320u);
//	v85 = 0;
//	v63 = 0;
//	v64 = 0;
//	v65 = 0;
//	v66 = (int)&unk_6FDD24C0;
//	v67 = (int)&unk_6FDD2540;
//	v68 = 0;
//	v69 = 0;
//	v70 = 0;
//	v55 = v7;
//	memset(&v71, 0, 0x320u);
//	v57 = a1;
//	v72 = 0;
//	v56 = &v60;
//	v10 = &v73;
//	v11 = *(_DWORD *)(v7 + 4 * *(_DWORD *)a4);
//	v62 = a3;
//	v75 = a3;
//	v12 = dword_6FDD2468[(unsigned __int16)v11 + 3 * (HIWORD(v11) - HIWORD(a3)) - (unsigned __int16)a3];
//	v73 = v12 + 1;
//	v60 = v12 + 7;
//	do
//	{
//		if (v10[8] != v9)
//			goto LABEL_37;
//		v13 = *(D2RoomStrc**)(v8 + 8);
//		v59 = *(_DWORD *)(v8 + 40);
//		v58 = *(_DWORD *)(v8 + 44);
//		v14 = *(_DWORD *)(*(_DWORD *)(v8 + 48) + 48);
//		if (v14 == v9 || *(_DWORD *)v14 != v9 && (v14 == v9 || *(_DWORD *)v14 != 1))
//		{
//			FOG_Assertion(//				"(UnitGetType(ptPathInfo->ptPath->hUnit) == UNIT_PLAYER) || (UnitGetType(ptPathInfo->ptPath->hUnit) == UNIT_MONSTER)", //				__FILE__, __LINE__//				290);
//			exit(-1);
//		}
//		v15 = *v10;
//		v16 = *((_WORD *)v10 + 5);
//		v10[4] = v9;
//		v17 = word_6FDD23D8[4 * v15];
//		v18 = *((_WORD *)v10 + 4);
//		LOWORD(v15) = v16 + word_6FDD23DC[4 * v15];
//		v19 = v18 + v17;
//		*((_WORD *)v10 + 7) = v15;
//		*((_WORD *)v10 + 6) = v19;
//		if (COLLISION_CheckMaskWithPattern2(v13, v19, (unsigned __int16)v15, v59, v58))
//		{
//			v20 = *(_DWORD *)(v10[6] + 4 * *v10);
//			*v10 = v20;
//			v21 = word_6FDD23D8[4 * v20];
//			LOWORD(v20) = v16 + word_6FDD23DC[4 * v20];
//			v22 = v18 + v21;
//			*((_WORD *)v10 + 7) = v20;
//			*((_WORD *)v10 + 6) = v22;
//			if (COLLISION_CheckMaskWithPattern2(v13, v22, (unsigned __int16)v20, v59, v58))
//			{
//				v23 = *(_DWORD *)(v10[6] + 4 * *v10);
//				*v10 = v23;
//				v24 = word_6FDD23D8[4 * v23];
//				LOWORD(v23) = v16 + word_6FDD23DC[4 * v23];
//				v25 = v18 + v24;
//				*((_WORD *)v10 + 7) = v23;
//				*((_WORD *)v10 + 6) = v25;
//				if (COLLISION_CheckMaskWithPattern2(v13, v25, (unsigned __int16)v23, v59, v58))
//				{
//					v26 = *v10;
//					v27 = v10[6];
//					v10[4] = 1;
//					v28 = *(_DWORD *)(v27 + 4 * v26);
//					*v10 = v28;
//					LOWORD(v27) = word_6FDD23D8[4 * v28];
//					LOWORD(v28) = v16 + word_6FDD23DC[4 * v28];
//					LOWORD(v27) = v18 + v27;
//					*((_WORD *)v10 + 7) = v28;
//					*((_WORD *)v10 + 6) = v27;
//					if (COLLISION_CheckMaskWithPattern2(v13, (unsigned __int16)v27, (unsigned __int16)v28, v59, v58))
//					{
//						v8 = v57;
//						v9 = 0;
//LABEL_36:
//						v10[8] = 1;
//						goto LABEL_37;
//					}
//				}
//			}
//		}
//		if (v10[1] > 0)
//		{
//			switch (a7)
//			{
//			case 0:
//				v29 = HIWORD(a3) - *((_WORD *)v10 + 7);
//				goto LABEL_17;
//			case 1:
//				v29 = *((_WORD *)v10 + 6) - (unsigned __int16)a3;
//				goto LABEL_17;
//			case 2:
//				v29 = *((_WORD *)v10 + 7) - HIWORD(a3);
//				goto LABEL_17;
//			case 3:
//				v29 = (unsigned __int16)a3 - *((_WORD *)v10 + 6);
//LABEL_17:
//				if (v29 <= 0)
//					break;
//				v30 = a4;
//				v31 = a5;
//				v32 = v55;
//				v33 = v29 + *(_DWORD *)a4 - 1;
//				if (v33 >= *(_DWORD *)a5
//					|| *((_WORD *)v10 + 6) != *(_WORD *)(v55 + 4 * v33)
//					|| *((_WORD *)v10 + 7) != *(_WORD *)(v55 + 4 * v33 + 2))
//					goto LABEL_25;
//				v34 = v10[1];
//				if (v34 >= a6)
//				{
//					FOG_Assertion(//						"ptCurrPath->nLoopLen < nMaxLength", //						__FILE__, __LINE__//						443);
//					exit(-1);
//				}
//				v10[v34 + 9] = v10[3];
//				v41 = v10[1] + 1;
//				v10[1] = v41;
//				LOWORD(v10[v41 + 9]) = 0;
//				v42 = v10[1];
//				v43 = v10[1] - v33 + *(_DWORD *)a4 - 1;
//				v44 = v43 + v33 + 1;
//				memcpy(//					(void*)(v55 + 4 * v44), //					(const void*)(v55 + 4 * (v33 + 1)), //					4 * (*(_DWORD *)a5 - (v33 + 1)));
//				memcpy((void*)(v55 + 4 * *(_DWORD *)a4), v10 + 9, 4 * v42);
//				*(_DWORD *)a5 += v43;
//				*(_DWORD *)a4 = v44;
//				return v42;
//			default:
//				break;
//			}
//		}
//		v32 = v55;
//		v30 = a4;
//		v31 = a5;
//LABEL_25:
//		if (v56[1] > 1)
//		{
//			v35 = v10[7];
//			if (v35)
//			{
//				if (*((_WORD *)v10 + 6) == LOWORD(v56[v35 + 7]) && *((_WORD *)v10 + 7) == HIWORD(v56[v35 + 7]))
//				{
//					result = 0;
//					*(_WORD *)(v32 + 4 * *(_DWORD *)v30) = 0;
//					*(_DWORD *)v31 = *(_DWORD *)v30;
//					return result;
//				}
//				v10[7] = 0;
//			}
//			if (*((_WORD *)v10 + 6) == *((_WORD *)v56 + 4) && *((_WORD *)v10 + 7) == *((_WORD *)v56 + 5))
//				v10[7] = v56[1];
//		}
//		v10[v10[1] + 9] = v10[3];
//		v36 = *v10;
//		v37 = v10[1];
//		v38 = *(_DWORD *)v31;
//		v10[2] = v10[3];
//		v9 = 0;
//		*v10 = *(_DWORD *)(v10[5] + 4 * v36);
//		v8 = v57;
//		v10[1] = v37 + 1;
//		if (v37 + 1 >= a6 - v38 - 1)
//			goto LABEL_36;
//LABEL_37:
//		if (v10[7] == v9)
//		{
//			v39 = v10;
//			v10 = v56;
//			v56 = v39;
//		}
//	}
//	while (v82 == v9 && v69 == v9);
//	if (a6 > 80)
//	{
//		*(_WORD *)(v55 + 4 * *(_DWORD *)a4) = v9;
//		*(_DWORD *)a5 = *(_DWORD *)a4;
//		return 0;
//	}
//	v45 = *(_DWORD *)(v8 + 4);
//	v46 = ((*(&v82 + v74) & 0xFFFF) - (unsigned __int16)v45) * ((*(&v82 + v74) & 0xFFFF) - (unsigned __int16)v45)
//		+ ((unsigned __int16)((unsigned int)*(&v82 + v74) >> 16) - HIWORD(v45))
//		* ((unsigned __int16)((unsigned int)*(&v82 + v74) >> 16) - HIWORD(v45));
//	v47 = *(_DWORD *)(v8 + 4);
//	v48 = ((*(&v69 + v61) & 0xFFFF) - (unsigned __int16)v47) * ((*(&v69 + v61) & 0xFFFF) - (unsigned __int16)v47)
//		+ ((unsigned __int16)((unsigned int)*(&v69 + v61) >> 16) - HIWORD(v47))
//		* ((unsigned __int16)((unsigned int)*(&v69 + v61) >> 16) - HIWORD(v47));
//	v49 = ((*(_DWORD *)v8 & 0xFFFF) - (unsigned __int16)v47) * ((*(_DWORD *)v8 & 0xFFFF) - (unsigned __int16)v47)
//		+ ((unsigned __int16)(*(_DWORD *)v8 >> 16) - HIWORD(v47))
//		* ((unsigned __int16)(*(_DWORD *)v8 >> 16) - HIWORD(v47));
//	if (v48 <= v46)
//	{
//		if (v49 < v48)
//			return 0;
//		v50 = a4;
//		v51 = v61;
//		v52 = *(_DWORD *)a4;
//		if (v61 > v9)
//		{
//			v53 = &v70;
//			goto LABEL_56;
//		}
//		goto LABEL_57;
//	}
//	if (v49 >= v46)
//	{
//		v50 = a4;
//		v51 = v74;
//		v52 = *(_DWORD *)a4;
//		if (v74 > v9)
//		{
//			v53 = &v83;
//LABEL_56:
//			v50 = a4;
//			memcpy((void*)(v55 + 4 * v52), (const void*)v9, 4 * v51);
//			LOWORD(v9) = 0;
//		}
//LABEL_57:
//		v54 = v51 + *(_DWORD *)v50;
//		*(_DWORD *)v50 = v54;
//		*(_DWORD *)a5 = v54;
//		*(_WORD *)(v55 + 4 * *(_DWORD *)v50) = v9;
//		result = v10[1];
//	}
//	else
//	{
//		result = 0;
//	}
//	return result;
//}

////D2Common.0x6FDAC170) --------------------------------------------------------
//signed int __fastcall sub_6FDAC170(int a1, int a2, int a3, signed int a4)
//{
//	int v4; // eax@1
//	signed int v5; // ecx@1
//	signed int result; // eax@3
//	int v7; // edi@5
//	int v8; // esi@5
//	int v9; // ebx@6
//	int v10; // ecx@6
//	int v11; // [sp+0h] [bp-8h]@5
//	int v12; // [sp+4h] [bp-4h]@1
//	signed int i; // [sp+Ch] [bp+4h]@5
//
//	v4 = a1;
//	v5 = a4;
//	v12 = v4;
//	if (a4 >= 2)
//	{
//		v7 = *(_WORD *)a2 - (unsigned __int16)a3;
//		v8 = 0;
//		result = 0;
//		v11 = *(_WORD *)(a2 + 2) - HIWORD(a3);
//		for (i = 0; v8 < a4 - 1; ++v8)
//		{
//			v9 = *(_WORD *)(a2 + 4 * v8 + 4) - *(_WORD *)(a2 + 4 * v8);
//			v10 = *(_WORD *)(a2 + 4 * v8 + 6) - *(_WORD *)(a2 + 4 * v8 + 2);
//			if (v9 != v7 || v10 != v11)
//			{
//				if (i > 0 || v7 == v9 || v11 == v10)
//				{
//					i = 0;
//					*(_DWORD *)(v12 + 4 * result++) = *(_DWORD *)(a2 + 4 * v8);
//				}
//				else
//				{
//					i = 1;
//					v9 = -2;
//				}
//			}
//			else
//			{
//				++i;
//			}
//			v11 = v10;
//			v5 = a4;
//			v7 = v9;
//		}
//		if (v8 < v5)
//			*(_DWORD *)(v12 + 4 * result++) = *(_DWORD *)(a2 + 4 * v8);
//	}
//	else
//	{
//		if (a4 == 1)
//		{
//			*(_DWORD *)v4 = *(_DWORD *)a2;
//			result = 1;
//		}
//		else
//		{
//			result = 0;
//		}
//	}
//	return result;
//}

////D2Common.0x6FDAC270) --------------------------------------------------------
//signed int __thiscall sub_6FDAC270(int this)
//{
//	int v1; // edi@1
//	int v2; // esi@1
//	int v3; // eax@1
//	D2RoomStrc*v4; // eax@7
//	int v5; // edx@7
//	unsigned __int8 v6; // cl@7
//	int v7; // eax@7
//	int v8; // edx@7
//	signed int v9; // ebp@7
//	bool v10; // zf@7
//	int v11; // eax@10
//	__int16 v12; // dx@10
//	int v13; // esi@10
//	int v14; // ebp@10
//	int v15; // eax@10
//	signed int v16; // ecx@10
//	signed int v17; // ebx@14
//	int v18; // esi@18
//	int v19; // edi@22
//	int v20; // eax@23
//	int v21; // edx@26
//	int v22; // ebp@28
//	int v23; // edx@28
//	int v24; // ebx@31
//	int v25; // ebp@35
//	int v26; // edx@35
//	int v27; // ebx@38
//	signed int v28; // esi@41
//	signed int result; // eax@42
//	int v30; // ebp@44
//	int v31; // ebx@44
//	int v32; // [sp+10h] [bp-178h]@1
//	int v33; // [sp+14h] [bp-174h]@7
//	int v34; // [sp+18h] [bp-170h]@10
//	int v35; // [sp+1Ch] [bp-16Ch]@28
//	int v36; // [sp+20h] [bp-168h]@11
//	int v37; // [sp+24h] [bp-164h]@7
//	int v38; // [sp+2Ch] [bp-15Ch]@16
//	int v39; // [sp+30h] [bp-158h]@7
//	int v40; // [sp+38h] [bp-150h]@7
//	int v41; // [sp+3Ch] [bp-14Ch]@1
//	int v42; // [sp+40h] [bp-148h]@7
//	D2RoomStrc*v43; // [sp+44h] [bp-144h]@7
//	int v44; // [sp+48h] [bp-140h]@7
//	int v45; // [sp+4Ch] [bp-13Ch]@7
//	int v46[78]; // [sp+50h] [bp-138h]@20
//
//	v1 = this;
//	v32 = this;
//	v2 = *(_DWORD *)(this + 48);
//	v41 = v2;
//	v3 = *(_DWORD *)(v2 + 48);
//	if (!v3 || *(_DWORD *)v3 && (!v3 || *(_DWORD *)v3 != 1))
//	{
//		FOG_Assertion(//			"(UnitGetType(ptPathInfo->ptPath->hUnit) == UNIT_PLAYER) || (UnitGetType(ptPathInfo->ptPath->hUnit) == UNIT_MONSTER)", //			__FILE__, __LINE__//			685);
//		exit(-1);
//	}
//	v4 = *(D2RoomStrc**)(this + 8);
//	v5 = *(_DWORD *)(this + 44);
//	v42 = *(_DWORD *)(this + 40);
//	v6 = *(_BYTE *)(v2 + 145);
//	v43 = v4;
//	v7 = *(_DWORD *)(v1 + 4);
//	v44 = v5;
//	v8 = *(_DWORD *)v1;
//	v9 = v6;
//	v10 = *(_DWORD *)(v2 + 88) == 0;
//	v33 = 0;
//	v39 = v8;
//	v37 = v7;
//	v45 = v8;
//	v40 = v6;
//	if (!v10 && v6 < 0x28u)
//	{
//		v9 = 40;
//		v40 = 40;
//	}
//	v11 = (unsigned __int16)v7 - (unsigned __int16)v8;
//	v12 = HIWORD(v39);
//	v13 = v11;
//	v14 = v9 - 1;
//	v15 = HIWORD(v37) - HIWORD(v39);
//	v16 = 0;
//	v34 = 0;
//	if (v13 >= 0)
//	{
//		v36 = 1;
//	}
//	else
//	{
//		v13 = -v13;
//		v36 = -1;
//	}
//	if (v15 >= 0)
//	{
//		v17 = 1;
//	}
//	else
//	{
//		v15 = -v15;
//		v17 = -1;
//	}
//	v38 = v17;
//	if (!v13)
//	{
//		if (v15 > 0)
//		{
//			v18 = *(int*)((char*)&v39 + 2);
//			v34 = ((v17 <= 0) - 1) & 2;
//			if (v15 > v14)
//			{
//				v16 = 0;
//				LOWORD(v45) = 0;
//				goto LABEL_41;
//			}
//			do
//			{
//				v18 += v17;
//				LOWORD(v46[v16]) = v39;
//				HIWORD(v46[v16++]) = v18;
//				--v15;
//			}
//			while (v15);
//		}
//		goto LABEL_40;
//	}
//	v19 = v39;
//	v37 = *(int*)((char*)&v39 + 2);
//	if (v15)
//	{
//		v21 = 0;
//		if (v13 < v15)
//		{
//			v34 = ((v38 <= 0) - 1) & 2;
//			if (v15 <= v14)
//			{
//				v25 = v37;
//				v26 = 0;
//				v35 = v15;
//				do
//				{
//					v26 += v13;
//					v25 += v38;
//					if (v26 >= v15)
//					{
//						v26 -= v15;
//						v19 += v36;
//					}
//					v27 = v35;
//					LOWORD(v46[v16]) = v19;
//					HIWORD(v46[v16++]) = v25;
//					v35 = v27 - 1;
//				}
//				while (v27 != 1);
//				goto LABEL_39;
//			}
//		}
//		else
//		{
//			LOBYTE(v21) = v36 <= 0;
//			v34 = 2 * v21 + 1;
//			if (v13 <= v14)
//			{
//				v22 = v37;
//				v23 = 0;
//				v35 = v13;
//				do
//				{
//					v23 += v15;
//					v19 += v36;
//					if (v23 >= v13)
//					{
//						v23 -= v13;
//						v22 += v38;
//					}
//					v24 = v35;
//					LOWORD(v46[v16]) = v19;
//					HIWORD(v46[v16++]) = v22;
//					v35 = v24 - 1;
//				}
//				while (v24 != 1);
//				goto LABEL_39;
//			}
//		}
//LABEL_34:
//		v1 = v32;
//		v16 = 0;
//		LOWORD(v45) = 0;
//		goto LABEL_41;
//	}
//	v20 = v36;
//	v34 = 2 * (v36 <= 0) + 1;
//	if (v13 > v14)
//		goto LABEL_34;
//	do
//	{
//		v19 += v20;
//		HIWORD(v46[v16]) = v12;
//		LOWORD(v46[v16++]) = v19;
//		--v13;
//	}
//	while (v13);
//LABEL_39:
//	v1 = v32;
//LABEL_40:
//	LOWORD(v46[v16]) = 0;
//LABEL_41:
//	v28 = v16;
//	v37 = v16;
//	if (v16 <= 2)
//		return 0;
//	if (v16 > 0)
//	{
//		v30 = v45;
//		v31 = v33;
//		do
//		{
//			if (COLLISION_CheckMaskWithPattern2(v43, LOWORD(v46[v31]), HIWORD(v46[v31]), v42, v44))
//			{
//				if (!sub_6FDABAC0(v1, (int)v46, v30, (int)&v33, (int)&v37, v40 - v31, v34))
//					return sub_6FDAC170(v41 + 156, (int)v46, v39, v33);
//				v28 = v37;
//			}
//			v30 = v46[v33];
//			v31 = v33++ + 1;
//		}
//		while (v33 < v28);
//	}
//	if (v28)
//		result = sub_6FDAC170(v41 + 156, (int)v46, v39, v28);
//	else
//		result = 0;
//	return result;
//}

////D2Common.0x6FDAC5E0) --------------------------------------------------------
//int __fastcall sub_6FDAC5E0(void* a1, int* a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6)
//{
//	unsigned int v6; // edi@1
//	signed int v7; // ebp@1
//	unsigned int v8; // eax@2
//	unsigned int v9; // esi@4
//	unsigned int v10; // edx@5
//	signed int v11; // ecx@8
//	unsigned int v12; // ebx@9
//	unsigned int v13; // ebx@9
//	int v14; // eax@11
//	int v15; // edx@13
//	bool v16; // zf@13
//	int v17; // eax@13
//	int v18; // esi@13
//	void* v19; // ecx@13
//	int result; // eax@19
//	void* v21; // [sp+Ch] [bp-Ch]@1
//	signed int v22; // [sp+10h] [bp-8h]@2
//	int* v23; // [sp+14h] [bp-4h]@1
//
//	v6 = a5;
//	v23 = a2;
//	v21 = a1;
//	v7 = 1;
//	if (a3 > a5)
//	{
//		v8 = a3;
//		v22 = 0;
//	}
//	else
//	{
//		v8 = a5;
//		v6 = a3;
//		v22 = 1;
//	}
//	v9 = a4;
//	if (a4 > a6)
//	{
//		v10 = a4;
//		v9 = a6;
//		v7 = 0;
//	}
//	else
//	{
//		v10 = a6;
//	}
//	if ((signed int)(v8 - v6) > (signed int)(v10 - v9))
//	{
//		v12 = v8;
//		v8 = v10;
//		v10 = v12;
//		v13 = v6;
//		v6 = v9;
//		v11 = 1;
//		v9 = v13;
//	}
//	else
//	{
//		v11 = 0;
//	}
//	if (v10 == v9)
//		v14 = 0;
//	else
//		v14 = (signed int)(127 * (v8 - v6)) / (signed int)(v10 - v9);
//	v15 = 3 * v14;
//	v16 = v11 == 0;
//	v17 = dword_6FDD25A0[3 * v14];
//	v18 = dword_6FDD2598[v15];
//	v19 = v21;
//	if (v16)
//	{
//		*(_DWORD *)v21 = v18;
//		*((_DWORD *)v21 + 1) = dword_6FDD259C[v15];
//		v19 = v21;
//	}
//	else
//	{
//		*((_DWORD *)v21 + 1) = v18;
//		*(_DWORD *)v21 = dword_6FDD259C[v15];
//		LOBYTE(v17) = (-1 - v17) & 0xF;
//	}
//	if (!v7)
//	{
//		*((_DWORD *)v19 + 1) = -*((_DWORD *)v19 + 1);
//		LOBYTE(v17) = (-1 - v17) & 0x1F;
//	}
//	if (v22)
//	{
//		result = (((-1 - (_BYTE)v17) & 0x3F) + 8) & 0x3F;
//		*v23 = result;
//	}
//	else
//	{
//		*(_DWORD *)v19 = -*(_DWORD *)v19;
//		result = ((_BYTE)v17 + 8) & 0x3F;
//		*v23 = result;
//	}
//	return result;
//}

//D2Common.0x6FDAC700 (#10215)
//TODO: Find a name
int __stdcall D2Common_10215(int nX1, int nY1, int nX2, int nY2)
{
	D2CoordStrc pCoords = {};
	int nResult = 0;

	sub_6FDAC5E0(&pCoords, &nResult, (nX1 << 16) + 32768, (nY1 << 16) + 32768, (nX2 << 16) + 32768, (nY2 << 16) + 32768);

	return nResult;
}

//D2Common.0x6FDAC760
//TODO: Find a name
int __stdcall sub_6FDAC760(int nX1, int nY1, int nX2, int nY2)
{
	D2CoordStrc pCoords = {};
	int nResult = 0;

	sub_6FDAC5E0(&pCoords, &nResult, nX1, nY1, nX2, nY2);

	return nResult;
}

////D2Common.0x6FDAC790) --------------------------------------------------------
//signed int __fastcall sub_6FDAC790(D2DynamicPathStrc* a1, int a2, int a3)
//{
//	D2DynamicPathStrc*v3; // esi@1
//	int v4; // edi@1
//	uint32_t v5; // eax@1
//	int v6; // ebp@1
//	int v7; // ebx@1
//	int v8; // edi@1
//	int v9; // ebx@1
//	int v10; // ecx@2
//	uint32_t v11; // eax@4
//	uint32_t v12; // eax@5
//	int v13; // edi@5
//	uint32_t v14; // eax@6
//	int v15; // edx@6
//	int v16; // ecx@6
//	uint32_t v17; // ecx@8
//	uint32_t v18; // eax@8
//	signed int v19; // ecx@8
//	int v20; // eax@8
//	int v21; // ecx@8
//	int v22; // edi@10
//	uint32_t v23; // ecx@11
//	int v24; // eax@11
//	int v26; // [sp+10h] [bp-10h]@6
//	int v27; // [sp+14h] [bp-Ch]@1
//	int v28; // [sp+18h] [bp-8h]@6
//	int v29; // [sp+1Ch] [bp-4h]@6
//
//	v3 = a1;
//	HIWORD(v4) = 0;
//	v5 = a1->unk0x24;
//	v6 = *(_DWORD *)&a1->wOffsetX;
//	HIWORD(v7) = 0;
//	v27 = a2;
//	LOWORD(v4) = a1->PathPoints[v5].X;
//	LOWORD(v7) = a1->PathPoints[v5].Y;
//	v8 = (v4 << 16) + 32768;
//	v9 = (v7 << 16) + 32768;
//	if (v8 == v6)
//	{
//		v10 = *(_DWORD *)&a1->wOffsetY;
//		while (v9 == v10)
//		{
//			v11 = v3->unk0x24;
//			if ((signed int)v11 >= (signed __int32)(v3->dwPathPoints - 1))
//			{
//				*(_DWORD *)&v3->unk0x68[2] = dword_6FDD258C;
//				*(_DWORD *)&v3->unk0x68[6] = dword_6FDD2590;
//				v23 = v3->dwPathPoints;
//				v3->unk0x72 = dword_6FDD258C;
//				v24 = dword_6FDD2590;
//				v3->dwVelocity = 0;
//				v3->unk0x76 = v24;
//				v3->unk0x24 = v23;
//				return 1;
//			}
//			v12 = v11 + 1;
//			HIWORD(v13) = 0;
//			v3->unk0x24 = v12;
//			LOWORD(v13) = v3->PathPoints[v12].X;
//			v8 = (v13 << 16) + 32768;
//			v9 = (v3->PathPoints[v12].Y << 16) + 32768;
//			if (v8 != v6)
//				break;
//		}
//	}
//	sub_6FDAC5E0(&v28, &v26, *(_DWORD *)&v3->wOffsetX, *(_DWORD *)&v3->wOffsetY, v8, v9);
//	v14 = v3->dwFlags;
//	v15 = v28;
//	v16 = v29;
//	*(_DWORD *)&v3->unk0x68[2] = v28;
//	*(_DWORD *)&v3->unk0x68[6] = v16;
//	if (BYTE1(v14) & 2)
//		v26 = ((_BYTE)v26 - 32) & 0x3F;
//	v17 = v3->dwVelocity;
//	v18 = v17;
//	v19 = v29 * v17;
//	v20 = (signed int)(v15 * v18) >> 8;
//	v3->unk0x72 = v20;
//	v21 = v19 >> 8;
//	v3->unk0x76 = v21;
//	v22 = (unsigned int)v8 >> 16 == (unsigned int)(v20 + v6) >> 16
//		&& (unsigned int)v9 >> 16 == (unsigned int)(v21 + *(_DWORD *)&v3->wOffsetY) >> 16;
//	if (a3 || v22 && v27)
//		sub_6FDA9720(v3, v26);
//	return v22;
//}

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

	if (!(pDynamicPath->dwFlags & 0x10))
	{
		if (pUnit->dwUnitType == UNIT_MONSTER && !PATH_GetDistance(pUnit->pDynamicPath))
		{
			return 0;
		}

		UNITROOM_RefreshUnit(pUnit);
		pUnit->dwFlags |= UNITFLAG_DOUPDATE;

		PATH_AddToDistance(pDynamicPath, -pDynamicPath->unk0x24);
	}

	if (pDynamicPath->dwPathType != 2 && pDynamicPath->dwPathType != 13 && pDynamicPath->dwPathType != 15)
	{
		return D2Common_10142(pDynamicPath, pUnit, 0);
	}

	if (a2)
	{
		pDynamicPath->dwPathType = 13;
	}
	else
	{
		pDynamicPath->dwPathType = 2;
		pDynamicPath->SP1.X = pDynamicPath->SP3.X;
		pDynamicPath->SP1.Y = pDynamicPath->SP3.Y;
	}

	nResult = D2Common_10142(pDynamicPath, pUnit, 0);
	if (!nResult)
	{
		pDynamicPath->dwPathType = 15;
		nResult = D2Common_10142(pDynamicPath, pUnit, 0);
	}

	return nResult;
}

//D2Common.0x6FDAC9A0) --------------------------------------------------------
BOOL __stdcall D2Common_10226(D2UnitStrc* pUnit, signed int a2)
{
//	D2DynamicPathStrc*v2; // esi@1
//	signed int v3; // ebx@1
//	uint32_t v4; // edi@2
//	uint32_t v5; // eax@2
//	uint32_t v6; // eax@7
//	uint32_t v8; // eax@9
//	D2UnitStrc*v9; // eax@12
//	D2UnitStrc*v10; // edi@12
//	uint32_t v11; // eax@16
//	uint16_t v12; // bx@18
//	uint16_t v13; // bp@18
//	D2UnitStrc*v14; // eax@18
//	signed int v15; // eax@19
//	int v16; // eax@21
//	int v17; // ecx@21
//	signed int v18; // edx@36
//	uint32_t v19; // eax@38
//	uint32_t v20; // ecx@39
//	uint32_t v21; // ebp@40
//	uint32_t v22; // eax@40
//	signed __int32 v23; // eax@45
//	int v24; // ecx@45
//	int v25; // edx@45
//	int v26; // [sp+10h] [bp-10h]@18
//	unsigned int v27; // [sp+18h] [bp-8h]@47
//	unsigned int v28; // [sp+1Ch] [bp-4h]@47
//
//	v2 = pUnit->pDynamicPath;
//	v3 = 0;
//	if (!v2)
//		return 0;
//	v4 = v2->dwFlags & 0xFFFFFFF7;
//	v5 = v4;
//	v2->dwFlags = v4;
//	if (v5 & 0x40000)
//		v2->unk0x54 = 0;
//	if (!(v4 & 0x20) || (signed __int32)v2->dwPathPoints <= 0 || !v2->dwVelocity)
//	{
//		sub_6FDACC40((int)v2, 0, (*(_DWORD *)&v2->wOffsetX & 0xFFFF0000) + 32768, (*(_DWORD *)&v2->wOffsetY & 0xFFFF0000) + 32768);
//		v6 = v2->dwFlags;
//		v2->dwPathPoints = 0;
//		LOBYTE(v6) = v6 & 0xDF;
//		v2->unk0x24 = 0;
//		v2->dwFlags = v6;
//		v2->unk0x72 = dword_6FDD258C;
//		v2->unk0x76 = dword_6FDD2590;
//		return 0;
//	}
//	if (!(v2->dwFlags & 0x40000))
//	{
//		v8 = v2->dwPathType;
//		if ((signed int)v8 < 5 || (signed int)v8 > 6 || (signed __int32)v2->unk0x24 < (signed __int32)v2->dwPathPoints)
//		{
//			v9 = v2->pTargetUnit;
//			v10 = v2->pUnit;
//			if (v9)
//			{
//				if (D2Common_10399(v9, v2->pUnit) <= v2->nStepNum)
//				{
//LABEL_32:
//					sub_6FDAD330((int)v2);
//					return 0;
//				}
//				v12 = v2->wPosX;
//				v13 = v2->wPosY;
//				sub_6FDAB940((int)&v26, (int)v2);
//				v14 = v2->pTargetUnit;
//				if (!v14 || (v15 = v14->dwUnitType, v15 < 0) || v15 > 1)
//					goto LABEL_54;
//				v16 = v2->xSP2 - (unsigned __int16)v26;
//				v17 = v2->ySP2 - HIWORD(v26);
//				if (v16 < 0)
//					v16 = -v16;
//				if (v16 <= 5)
//				{
//					if (v17 < 0)
//						v17 = -v17;
//					if (v17 <= 5)
//					{
//LABEL_54:
//						if ((signed __int32)v2->unk0x24 < (signed __int32)v2->dwPathPoints
//							|| v2->xSP3 == v12 && v2->ySP3 == v13)
//						{
//							v3 = 0;
//							goto LABEL_35;
//						}
//					}
//				}
//				v11 = D2Common_10236(v10, 1);
//				v3 = 0;
//			}
//			else
//			{
//				if ((signed __int32)v2->unk0x24 < (signed __int32)v2->dwPathPoints
//					|| v2->wPosX == v2->xSP3 && v2->wPosY == v2->ySP3)
//					goto LABEL_35;
//				v11 = D2Common_10236(v10, 0);
//			}
//			if (v11 == v3)
//				goto LABEL_32;
//		}
//	}
//LABEL_35:
//	if ((signed __int32)v2->unk0x24 >= (signed __int32)v2->dwPathPoints)
//		goto LABEL_32;
//	v18 = a2;
//	if (a2 <= v3)
//		v18 = 1024;
//	v19 = v2->unk0x88[0];
//	if (v19 != v3)
//	{
//		v20 = v2->unk0x88[1] + 1;
//		v2->unk0x88[1] = v20;
//		if ((signed int)v20 >= 5)
//		{
//			v21 = v19 + v2->dwVelocity;
//			v22 = v2->dwVelocity2;
//			v2->dwVelocity = v21;
//			if ((signed int)v21 <= (signed int)v22)
//			{
//				if ((signed int)v21 < v3)
//					v2->dwVelocity = v3;
//			}
//			else
//			{
//				v2->dwVelocity = v22;
//				v2->unk0x88[0] = v3;
//			}
//			v2->unk0x88[1] = v3;
//		}
//	}
//	v23 = (signed __int32)(v18 * v2->dwVelocity) >> 6;
//	v24 = v23 * *(_DWORD *)&v2->unk0x68[2] >> 12;
//	v25 = v23 * *(_DWORD *)&v2->unk0x68[6] >> 12;
//	v2->unk0x72 = v24;
//	v2->unk0x76 = v25;
//	if (v24 == v3)
//	{
//		if (v25 == v3)
//			goto LABEL_32;
//	}
//	sub_6FDACEC0((int)v2, (int)&v27, (int)&pUnit);
//	sub_6FDACC40((int)v2, 0, v27, v28);
//	if (v2->dwPathType != 4)
//	{
//		if ((signed __int32)v2->unk0x24 < (signed __int32)v2->dwPathPoints)
//			sub_6FDAC790(v2, 1, 1);
//	}
//	if ((signed __int32)v2->unk0x24 >= (signed __int32)v2->dwPathPoints)
//		goto LABEL_32;
	return 1;
}

////D2Common.0x6FDACC40) --------------------------------------------------------
//void __fastcall sub_6FDACC40(int a1, D2RoomStrc*a2, unsigned int a3, unsigned int a4)
//{
//	unsigned int v4; // ebx@1
//	int v5; // ebp@1
//	unsigned int v6; // esi@1
//	int v7; // eax@1
//	D2RoomStrc*v8; // eax@2
//	unsigned int v9; // esi@2
//	unsigned int v10; // edi@2
//	int v11; // ecx@5
//	unsigned int v12; // edx@7
//	D2RoomStrc*v13; // eax@8
//	int v14; // ecx@11
//	int v15; // eax@18
//	int v16; // edx@18
//	int v17; // eax@20
//	unsigned __int16 v18; // bx@20
//	unsigned __int16 v19; // di@20
//	signed int v20; // edx@23
//	D2RoomStrc*v21; // esi@25
//	int v22; // edi@25
//	signed __int32 v23; // ebx@25
//	int v24; // ecx@28
//	unsigned int v25; // edx@36
//	D2RoomStrc*v26; // eax@37
//	int v27; // ecx@40
//	int v28; // eax@45
//	D2UnitStrc*v29; // edi@45
//	int v30; // ecx@47
//	int pNumRooms; // [sp+10h] [bp-14h]@7
//	D2RoomStrc** pppRoom; // [sp+14h] [bp-10h]@7
//	int pX; // [sp+18h] [bp-Ch]@18
//	int pY; // [sp+1Ch] [bp-8h]@18
//	D2RoomStrc*v35; // [sp+20h] [bp-4h]@1
//
//	v4 = a3;
//	v5 = a1;
//	v6 = a4;
//	v7 = *(_DWORD *)(a1 + 52);
//	v35 = a2;
//	if (!(v7 & 0x40000))
//		goto LABEL_18;
//	v8 = *(D2RoomStrc**)(a1 + 28);
//	v9 = a4 >> 16;
//	v10 = a3 >> 16;
//	if (!v8)
//		goto LABEL_17;
//	if ((signed int)v10 >= v8->nSubtileX)
//	{
//		if ((signed int)v10 < v8->nSubtileX + v8->nSubtileWidth)
//		{
//			v11 = v8->nSubtileY;
//			if ((signed int)v9 >= v11)
//			{
//				if ((signed int)v9 < v11 + v8->nSubtileHeight)
//					goto LABEL_16;
//			}
//		}
//	}
//	pppRoom = 0;
//	pNumRooms = 0;
//	DUNGEON_GetAdjacentRoomsListFromRoom(v8, &pppRoom, &pNumRooms);
//	v12 = 0;
//	if (!pNumRooms)
//	{
//LABEL_17:
//		*(_DWORD *)(v5 + 40) = 0;
//		return;
//	}
//	while (1)
//	{
//		v13 = pppRoom[v12];
//		if (v13)
//		{
//			if ((signed int)v10 >= v13->nSubtileX)
//			{
//				if ((signed int)v10 < v13->nSubtileX + v13->nSubtileWidth)
//				{
//					v14 = v13->nSubtileY;
//					if ((signed int)v9 >= v14)
//					{
//						if ((signed int)v9 < v14 + v13->nSubtileHeight)
//							break;
//					}
//				}
//			}
//		}
//		++v12;
//		if (v12 >= pNumRooms)
//		{
//			*(_DWORD *)(v5 + 40) = 0;
//			return;
//		}
//	}
//	v4 = a3;
//	v8 = pppRoom[v12];
//LABEL_16:
//	v6 = a4;
//	if (!v8)
//		goto LABEL_17;
//LABEL_18:
//	*(_DWORD *)v5 = v4;
//	*(_DWORD *)(v5 + 4) = v6;
//	pX = v4 >> 11;
//	pY = v6 >> 11;
//	DUNGEON_FlattenCoords_IsoToCartesian(&pX, &pY);
//	v15 = *(_DWORD *)(v5 + 48);
//	v16 = pY;
//	*(_DWORD *)(v5 + 8) = pX;
//	*(_DWORD *)(v5 + 12) = v16;
//	if (!v15
//		|| !(*(_BYTE *)(v5 + 52) & 1)
//		|| (v17 = *(_DWORD *)(v5 + 28), v18 = *(_WORD *)(v5 + 2), v19 = *(_WORD *)(v5 + 6), v17)
//		&& (signed int)v18 >= *(_DWORD *)v17
//		&& v18 < *(_DWORD *)v17 + *(_DWORD *)(v17 + 8)
//		&& (v20 = *(_DWORD *)(v17 + 4), v19 >= v20)
//		&& v19 < v20 + *(_DWORD *)(v17 + 12))
//		return;
//	v21 = 0;
//	v22 = *(_WORD *)(v5 + 6);
//	v23 = *(_WORD *)(v5 + 2);
//	if (!v17)
//		goto LABEL_32;
//	if (v23 < *(_DWORD *)v17
//		|| v23 >= *(_DWORD *)v17 + *(_DWORD *)(v17 + 8)
//		|| (v24 = *(_DWORD *)(v17 + 4), v22 < v24)
//		|| v22 >= v24 + *(_DWORD *)(v17 + 12))
//	{
//		pppRoom = 0;
//		a3 = 0;
//		DUNGEON_GetAdjacentRoomsListFromRoom((D2RoomStrc*)v17, &pppRoom, (int*)&a3);
//		v25 = 0;
//		if (a3 <= 0)
//		{
//LABEL_43:
//			v21 = 0;
//LABEL_32:
//			if (!v35 || (v21 = COLLISION_GetRoomBySubTileCoordinates(v35, v23, v22)) == 0)
//			{
//				if (*(_DWORD *)(v5 + 52) & 0x40000)
//				{
//					*(_DWORD *)(v5 + 40) = 0;
//					return;
//				}
//			}
//			goto LABEL_45;
//		}
//		while (1)
//		{
//			v26 = pppRoom[v25];
//			if (v26)
//			{
//				if (v23 >= v26->nSubtileX)
//				{
//					if (v23 < v26->nSubtileX + v26->nSubtileWidth)
//					{
//						v27 = v26->nSubtileY;
//						if (v22 >= v27)
//						{
//							if (v22 < v27 + v26->nSubtileHeight)
//								break;
//						}
//					}
//				}
//			}
//			++v25;
//			if (v25 >= a3)
//				goto LABEL_43;
//		}
//		v21 = pppRoom[v25];
//	}
//	else
//	{
//		v21 = *(D2RoomStrc**)(v5 + 28);
//	}
//	if (!v21)
//		goto LABEL_32;
//LABEL_45:
//	v28 = *(_DWORD *)(v5 + 28);
//	v29 = *(D2UnitStrc**)(v5 + 48);
//	*(_DWORD *)(v5 + 32) = v28;
//	if (v28)
//		UNITROOM_RemoveUnitFromRoom(v29);
//	v30 = *(_DWORD *)(v5 + 52);
//	*(_DWORD *)(v5 + 28) = v21;
//	*(_DWORD *)(v5 + 52) = v30 | 2;
//	if (v21)
//	{
//		UNITROOM_AddUnitToRoom(v29, v21);
//		UNITROOM_RefreshUnit(v29);
//	}
//}

////D2Common.0x6FDACEC0) --------------------------------------------------------
//signed int __fastcall sub_6FDACEC0(int a1, int a2, int a3)
//{
//	int v3; // esi@1
//	D2UnitStrc*v4; // eax@1
//	int v5; // eax@7
//	signed int result; // eax@8
//	int v7; // edi@9
//	int v8; // ebx@9
//	int v9; // ecx@11
//	int v10; // edx@11
//	int v11; // eax@11
//	int v12; // ecx@14
//	int v13; // ebp@17
//	int v14; // edx@19
//	unsigned int v15; // eax@27
//	unsigned int v16; // ecx@27
//	int v17; // ebp@27
//	unsigned int v18; // ecx@27
//	char v19; // al@29
//	int v20; // ecx@30
//	int v21; // ebp@33
//	unsigned int v22; // edi@33
//	int v23; // eax@33
//	int v24; // ebx@33
//	int v25; // ecx@33
//	unsigned int v26; // edx@33
//	int v27; // ebx@36
//	int v28; // eax@38
//	int v29; // eax@39
//	int v30; // ebx@45
//	int v31; // eax@45
//	signed __int16 v32; // ax@48
//	__int16 v33; // ax@50
//	int v34; // eax@58
//	int v35; // edx@64
//	int v36; // [sp+10h] [bp-48h]@15
//	__int16 v37; // [sp+10h] [bp-48h]@33
//	__int16 v38; // [sp+12h] [bp-46h]@33
//	int v39; // [sp+14h] [bp-44h]@1
//	signed int v40; // [sp+18h] [bp-40h]@10
//	int v41; // [sp+18h] [bp-40h]@12
//	signed int v42; // [sp+1Ch] [bp-3Ch]@3
//	int v43; // [sp+20h] [bp-38h]@33
//	D2UnitStrc*v44; // [sp+24h] [bp-34h]@1
//	int v45; // [sp+28h] [bp-30h]@33
//	int v46; // [sp+30h] [bp-28h]@36
//	int v47; // [sp+34h] [bp-24h]@9
//	int v48; // [sp+38h] [bp-20h]@33
//	int v49; // [sp+3Ch] [bp-1Ch]@33
//	int v50; // [sp+40h] [bp-18h]@33
//	int v51; // [sp+44h] [bp-14h]@33
//	int v52; // [sp+48h] [bp-10h]@36
//	int v53; // [sp+4Ch] [bp-Ch]@36
//	int v54; // [sp+50h] [bp-8h]@27
//	int v55; // [sp+54h] [bp-4h]@27
//
//	v3 = a1;
//	v4 = *(D2UnitStrc**)(a1 + 48);
//	*(_WORD *)(a1 + 84) = 0;
//	v39 = a2;
//	v44 = v4;
//	*(_DWORD *)a3 = 0;
//	if (!v4 || v4->dwUnitType != 1 || (v42 = 1, !(*(_BYTE *)(a1 + 52) & 0x10)))
//		v42 = 0;
//	if (!*(_DWORD *)(a1 + 114) && !*(_DWORD *)(a1 + 118))
//	{
//		v5 = *(_DWORD *)a1;
//		*(_DWORD *)(a1 + 40) = 0;
//		*(_DWORD *)(a1 + 36) = 0;
//		*(_DWORD *)a2 = (v5 & 0xFFFF0000) + 32768;
//		*(_DWORD *)(a2 + 4) = (*(_DWORD *)(a1 + 4) & 0xFFFF0000) + 32768;
//		return 0;
//	}
//	v7 = *(_DWORD *)(a1 + 118);
//	v8 = *(_DWORD *)(a1 + 114);
//	v47 = *(_DWORD *)(a1 + 118);
//	if (*(_DWORD *)(a1 + 60) == 4)
//	{
//		v40 = 0;
//	}
//	else
//	{
//		v9 = *(_DWORD *)(a1 + 36);
//		HIWORD(v10) = 0;
//		v11 = (*(_WORD *)(v3 + 4 * v9 + 156) << 16) + 32768 - *(_DWORD *)v3;
//		if (v11 < 0)
//			v41 = -v11;
//		else
//			v41 = (*(_WORD *)(v3 + 4 * v9 + 156) << 16) + 32768 - *(_DWORD *)v3;
//		LOWORD(v10) = *(_WORD *)(v3 + 4 * v9 + 158);
//		v12 = (v10 << 16) + 32768 - *(_DWORD *)(v3 + 4);
//		if (v12 < 0)
//			v36 = -v12;
//		else
//			v36 = (v10 << 16) + 32768 - *(_DWORD *)(v3 + 4);
//		v13 = v7;
//		if (v7 < 0)
//			v13 = -v7;
//		v14 = v8;
//		if (v8 < 0)
//			v14 = -v8;
//		if (v14 <= v13)
//			v14 = v13;
//		if (v41 > v14 || v36 > v14)
//		{
//			v40 = 0;
//		}
//		else
//		{
//			v8 = v11;
//			v47 = v12;
//			v40 = 1;
//			v7 = v12;
//		}
//	}
//	v15 = *(_DWORD *)v3;
//	v16 = *(_DWORD *)(v3 + 4);
//	v17 = v8 + *(_DWORD *)v3;
//	v55 = v16 + v7;
//	v54 = v17;
//	v18 = v16 >> 16;
//	if (HIWORD(v15) != HIWORD(v17) || (_WORD)v18 != HIWORD(v55))
//	{
//		v19 = *(_BYTE *)(v3 + 144);
//		if (v19)
//		{
//			v20 = *(_DWORD *)(v3 + 60);
//			if (v20 != 8)
//			{
//				if (v20 != 11)
//					*(_BYTE *)(v3 + 144) = v19 - 1;
//			}
//		}
//		*(_DWORD *)(v3 + 468) = 0;
//		v48 = v8;
//		v49 = v47;
//		sub_6FDAB810(&v48, &v49);
//		v21 = *(_DWORD *)(v3 + 4);
//		v22 = *(_DWORD *)v3;
//		HIWORD(v45) = *(_DWORD *)(v3 + 4) >> 16;
//		v23 = v8 + *(_DWORD *)v3;
//		v24 = v47 + v21;
//		v37 = HIWORD(v23);
//		v38 = (unsigned int)(v47 + v21) >> 16;
//		v25 = *(_DWORD *)(v3 + 52);
//		v50 = v23;
//		v26 = v22 >> 16;
//		LOWORD(v45) = HIWORD(v22);
//		v51 = v47 + v21;
//		v43 = v25 & 0x20000;
//		while (1)
//		{
//			if ((_WORD)v26 == v37 && HIWORD(v45) == v38)
//			{
//LABEL_61:
//				if (*(_DWORD *)(v3 + 468))
//					*(_DWORD *)(v3 + 52) |= 8u;
//				v17 = v54;
//				*(_DWORD *)v39 = v23;
//				*(_DWORD *)(v39 + 4) = v24;
//				*(_DWORD *)a3 = *(_DWORD *)(v3 + 468);
//				goto LABEL_64;
//			}
//			v27 = v48 + v22;
//			v53 = v21 + v49;
//			v52 = v48 + v22;
//			LOWORD(v46) = (v48 + v22) >> 16;
//			HIWORD(v46) = (unsigned int)(v21 + v49) >> 16;
//			if ((_WORD)v26 != (v48 + v22) >> 16)
//				break;
//			if (HIWORD(v45) != (unsigned int)(v21 + v49) >> 16)
//				break;
//LABEL_59:
//			v21 = v53;
//			v22 = v27;
//			v24 = v51;
//			v45 = v46;
//			LOWORD(v26) = v46;
//			v23 = v50;
//		}
//		v28 = *(_DWORD *)(v3 + 52);
//		if (v28 & 4)
//		{
//			v29 = *(_DWORD *)(v3 + 48);
//			if (!v29 || *(_DWORD *)v29 && (!v29 || *(_DWORD *)v29 != 1))
//				FOG_10025(//					"(UnitGetType(ptPath->hUnit) == UNIT_PLAYER) || (UnitGetType(ptPath->hUnit) == UNIT_MONSTER)", //					__FILE__, __LINE__//					521);
//			D2Common_10133(//				*(D2RoomStrc**)(v3 + 28), //				(unsigned __int16)v45, //				HIWORD(v45), //				*(D2RoomStrc**)(v3 + 28), //				(unsigned __int16)v46, //				HIWORD(v46), //				*(_DWORD *)(v3 + 72), //				*(_DWORD *)(v3 + 76));
//LABEL_56:
//			if (v43)
//				*(_DWORD *)(v3 + 4 * *(_DWORD *)(v3 + 468) + 472) = v46;
//			v34 = *(_DWORD *)(v3 + 468) + 1;
//			*(_DWORD *)(v3 + 468) = v34;
//			if (v34 >= 10)
//			{
//				v23 = v50;
//				v24 = v51;
//				goto LABEL_61;
//			}
//			goto LABEL_59;
//		}
//		v30 = v28 & 0x40000;
//		v31 = *(_DWORD *)(v3 + 80);
//		if (v31 == 13313)
//			v31 = 15361;
//		if (v30)
//			v32 = D2Common_10131(//				*(D2RoomStrc**)(v3 + 28), //				(unsigned __int16)v45, //				HIWORD(v45), //				(unsigned __int16)v46, //				HIWORD(v46), //				*(_DWORD *)(v3 + 68), //				*(_DWORD *)(v3 + 76), //				v31);
//		else
//			v32 = D2Common_10132(//				*(D2RoomStrc**)(v3 + 28), //				(unsigned __int16)v45, //				HIWORD(v45), //				(unsigned __int16)v46, //				HIWORD(v46), //				*(_DWORD *)(v3 + 72), //				*(_DWORD *)(v3 + 76), //				v31);
//		*(_WORD *)(v3 + 84) |= v32;
//		v33 = *(_WORD *)(v3 + 84);
//		if (v33)
//		{
//			if (!v30)
//				goto LABEL_52;
//		}
//		else
//		{
//			if (!v30)
//			{
//LABEL_55:
//				v27 = v52;
//				goto LABEL_56;
//			}
//		}
//		if (v33 & 5)
//		{
//LABEL_52:
//			*(_DWORD *)v39 = (v22 & 0xFFFF0000) + 32768;
//			*(_DWORD *)(v39 + 4) = (v21 & 0xFFFF0000) + 32768;
//			*(_DWORD *)a3 = *(_DWORD *)(v3 + 468);
//			if (v42)
//			{
//				result = D2Common_10236(v44, *(_DWORD *)(v3 + 88) != 0) == 0;
//			}
//			else
//			{
//				*(_DWORD *)(v3 + 36) = *(_DWORD *)(v3 + 40);
//				result = 0;
//			}
//			return result;
//		}
//		goto LABEL_55;
//	}
//LABEL_64:
//	v35 = v55;
//	*(_DWORD *)v39 = v17;
//	*(_DWORD *)(v39 + 4) = v35;
//	if (!v40)
//		return 0;
//	++*(_DWORD *)(v3 + 36);
//	return 1;
//}

////D2Common.0x6FDAD330) --------------------------------------------------------
//int __thiscall sub_6FDAD330(int this)
//{
//	int v1; // esi@1
//	int v2; // eax@1
//	unsigned int v3; // ebp@1
//	unsigned int v4; // edi@1
//	D2RoomStrc*v5; // eax@2
//	int v6; // edi@2
//	int v7; // ebx@2
//	int v8; // ecx@5
//	unsigned int v9; // edx@7
//	D2RoomStrc*v10; // eax@8
//	int v11; // ecx@11
//	int v12; // edx@20
//	int v13; // eax@20
//	int v14; // ecx@22
//	unsigned __int16 v15; // dx@22
//	unsigned __int16 v16; // ax@22
//	signed int v17; // ebx@25
//	D2RoomStrc*v18; // edi@27
//	int v19; // eax@30
//	D2UnitStrc*v20; // ebx@30
//	int v21; // ecx@32
//	int v22; // eax@34
//	int result; // eax@34
//	int pNumRooms; // [sp+10h] [bp-1Ch]@7
//	D2RoomStrc** pppRoom; // [sp+14h] [bp-18h]@7
//	unsigned int v26; // [sp+18h] [bp-14h]@7
//	int pX; // [sp+1Ch] [bp-10h]@20
//	int pY; // [sp+20h] [bp-Ch]@20
//	unsigned int v29; // [sp+28h] [bp-4h]@1
//
//	v1 = this;
//	v2 = *(_DWORD *)(this + 52);
//	v3 = (*(_DWORD *)this & 0xFFFF0000) + 32768;
//	v4 = (*(_DWORD *)(this + 4) & 0xFFFF0000) + 32768;
//	v29 = (*(_DWORD *)(this + 4) & 0xFFFF0000) + 32768;
//	if (v2 & 0x40000)
//	{
//		v5 = *(D2RoomStrc**)(this + 28);
//		v6 = v4 >> 16;
//		v7 = v3 >> 16;
//		if (!v5)
//			goto LABEL_18;
//		if (v7 < v5->nSubtileX
//			|| v7 >= v5->nSubtileX + v5->nSubtileWidth
//			|| (v8 = v5->nSubtileY, v6 < v8)
//			|| v6 >= v8 + v5->nSubtileHeight)
//		{
//			pppRoom = 0;
//			pNumRooms = 0;
//			DUNGEON_GetAdjacentRoomsListFromRoom(v5, &pppRoom, &pNumRooms);
//			v9 = 0;
//			v26 = 0;
//			if (pNumRooms)
//			{
//				while (1)
//				{
//					v10 = pppRoom[v9];
//					if (v10 && v7 >= v10->nSubtileX)
//					{
//						if (v7 < v10->nSubtileX + v10->nSubtileWidth)
//						{
//							v11 = v10->nSubtileY;
//							if (v6 >= v11)
//							{
//								if (v6 < v11 + v10->nSubtileHeight)
//								{
//									v5 = pppRoom[v26];
//									goto LABEL_17;
//								}
//							}
//						}
//						v9 = v26;
//					}
//					++v9;
//					v26 = v9;
//					if (v9 >= pNumRooms)
//					{
//						*(_DWORD *)(v1 + 40) = 0;
//						goto LABEL_34;
//					}
//				}
//			}
//			goto LABEL_18;
//		}
//LABEL_17:
//		if (!v5)
//		{
//LABEL_18:
//			*(_DWORD *)(v1 + 40) = 0;
//			goto LABEL_34;
//		}
//		v4 = v29;
//	}
//	*(_DWORD *)v1 = v3;
//	*(_DWORD *)(v1 + 4) = v4;
//	pX = v3 >> 11;
//	pY = v4 >> 11;
//	DUNGEON_FlattenCoords_IsoToCartesian(&pX, &pY);
//	v12 = pX;
//	*(_DWORD *)(v1 + 12) = pY;
//	v13 = *(_DWORD *)(v1 + 48);
//	*(_DWORD *)(v1 + 8) = v12;
//	if (v13)
//	{
//		if (*(_BYTE *)(v1 + 52) & 1)
//		{
//			v14 = *(_DWORD *)(v1 + 28);
//			v15 = *(_WORD *)(v1 + 2);
//			v16 = *(_WORD *)(v1 + 6);
//			if (!v14
//				|| (signed int)v15 < *(_DWORD *)v14
//				|| v15 >= *(_DWORD *)v14 + *(_DWORD *)(v14 + 8)
//				|| (v17 = *(_DWORD *)(v14 + 4), v16 < v17)
//				|| v16 >= v17 + *(_DWORD *)(v14 + 12))
//			{
//				v18 = COLLISION_GetRoomBySubTileCoordinates((D2RoomStrc*)v14, *(_WORD *)(v1 + 2), *(_WORD *)(v1 + 6));
//				if (v18 || !(*(_DWORD *)(v1 + 52) & 0x40000))
//				{
//					v19 = *(_DWORD *)(v1 + 28);
//					v20 = *(D2UnitStrc**)(v1 + 48);
//					*(_DWORD *)(v1 + 32) = v19;
//					if (v19)
//						UNITROOM_RemoveUnitFromRoom(v20);
//					v21 = *(_DWORD *)(v1 + 52);
//					*(_DWORD *)(v1 + 28) = v18;
//					*(_DWORD *)(v1 + 52) = v21 | 2;
//					if (v18)
//					{
//						UNITROOM_AddUnitToRoom(v20, v18);
//						UNITROOM_RefreshUnit(v20);
//					}
//				}
//				else
//				{
//					*(_DWORD *)(v1 + 40) = 0;
//				}
//			}
//		}
//	}
//LABEL_34:
//	v22 = *(_DWORD *)(v1 + 52);
//	LOBYTE(v22) = v22 & 0xDF;
//	*(_DWORD *)(v1 + 52) = v22;
//	result = 0;
//	*(_DWORD *)(v1 + 40) = 0;
//	*(_DWORD *)(v1 + 36) = 0;
//	*(_DWORD *)(v1 + 114) = dword_6FDD258C;
//	*(_DWORD *)(v1 + 118) = dword_6FDD2590;
//	return result;
//}

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

//D2Common.0x6FDAD590) --------------------------------------------------------
BOOL __stdcall D2Common_10229(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit, D2RoomStrc* a3, unsigned int a4, __int16 a5)
{
//	HIWORD(a4) = a5;
//	if (sub_6FDAD5E0(pDynamicPath, a3, a4))
//	{
//		pDynamicPath->dwPathPoints = 0;
//		return TRUE;
//	}
//
	return FALSE;
}

////D2Common.0x6FDAD5E0) --------------------------------------------------------
//signed int __fastcall sub_6FDAD5E0(D2DynamicPathStrc*a1, D2RoomStrc*a2, unsigned int a3)
//{
//	int v3; // esi@1
//	int v4; // ebx@1
//	D2UnitStrc*v5; // eax@1
//	unsigned int v6; // edi@3
//	uint16_t v7; // ax@5
//	uint16_t v8; // cx@5
//	int v9; // eax@6
//	int v10; // edx@8
//	int v11; // ecx@8
//	uint16_t v12; // dx@9
//	D2RoomStrc*v13; // eax@13
//	int v14; // ecx@15
//	unsigned int v15; // edi@15
//	unsigned int v16; // ebp@15
//	int v17; // ebx@16
//	int v18; // ecx@19
//	unsigned int v19; // ecx@21
//	D2RoomStrc*v20; // eax@22
//	int v21; // ecx@25
//	int v22; // edx@34
//	int v23; // eax@34
//	unsigned int v24; // edi@37
//	int v25; // eax@41
//	int v26; // eax@44
//	int v27; // ecx@47
//	int v28; // eax@55
//	unsigned int v29; // edi@55
//	unsigned int v30; // ebp@55
//	int v31; // eax@58
//	int v32; // edx@58
//	int v33; // eax@61
//	int v35; // [sp+10h] [bp-28h]@16
//	int pNumRooms; // [sp+14h] [bp-24h]@21
//	D2RoomStrc** pppRoom; // [sp+18h] [bp-20h]@21
//	unsigned int v38; // [sp+1Ch] [bp-1Ch]@21
//	D2RoomStrc* pRoom; // [sp+20h] [bp-18h]@1
//	int pX; // [sp+24h] [bp-14h]@34
//	int pY; // [sp+28h] [bp-10h]@34
//	int v42; // [sp+2Ch] [bp-Ch]@15
//	unsigned int v43; // [sp+30h] [bp-8h]@5
//
//	v3 = (int)a1;
//	v4 = (int)a2;
//	v5 = a1->pUnit;
//	pRoom = a2;
//	if (v5 && v5->dwUnitType == 3)
//	{
//		v6 = a3;
//		if (a3 == __PAIR__(a3, 0))
//		{
//			COLLISION_ResetMaskWithSize(a1->pRoom, a1->wPosX, a1->wPosY, a1->dwUnitSize, a1->dwCollisionType);
//			*(_WORD *)(v3 + 84) = v6;
//		}
//		else
//		{
//			v7 = a1->wPosX;
//			v8 = a1->wPosY;
//			LOWORD(v43) = *(_WORD *)(v3 + 2);
//			HIWORD(v43) = v8;
//			if (__PAIR__(v8, v7) == a3)
//			{
//				v9 = *(_DWORD *)(v3 + 52);
//				LOBYTE(v9) = v9 & 0xF7;
//			}
//			else
//			{
//				v9 = *(_DWORD *)(v3 + 52);
//				LOBYTE(v9) = v9 | 8;
//			}
//			v10 = *(_DWORD *)(v3 + 80);
//			v11 = *(_DWORD *)(v3 + 68);
//			*(_DWORD *)(v3 + 52) = v9;
//			*(_WORD *)(v3 + 84) = sub_6FD44BB0(//				(unsigned __int16)v6, //				(unsigned __int16)v43, //				*(_DWORD *)(v3 + 28), //				HIWORD(v43), //				v4, //				(unsigned __int16)v6, //				HIWORD(a3), //				v11, //				*(_DWORD *)(v3 + 76), //				v10);
//			*(_DWORD *)(v3 + 468) = 1;
//			*(_DWORD *)(v3 + 472) = v6;
//		}
//	}
//	else
//	{
//		v12 = a1->wPosX;
//		HIWORD(v43) = a1->wPosY;
//		LOWORD(v43) = v12;
//		if (a3 == __PAIR__(a3, 0))
//			COLLISION_ResetMaskWithPattern(a1->pRoom, (unsigned __int16)v43, HIWORD(v43), a1->dwCollisionPattern, a1->dwCollisionType);
//		else
//			D2Common_10133(//				a1->pRoom, //				(unsigned __int16)v43, //				HIWORD(v43), //				(D2RoomStrc*)v4, //				(unsigned __int16)a3, //				HIWORD(a3), //				a1->dwCollisionPattern, //				a1->dwCollisionType);
//		v6 = a3;
//	}
//	v13 = *(D2RoomStrc**)(v3 + 28);
//	if (v13 != (D2RoomStrc*)v4)
//		*(_DWORD *)(v3 + 52) |= 1u;
//	v14 = *(_DWORD *)(v3 + 52);
//	v42 = (unsigned __int16)v6;
//	v43 = HIWORD(a3);
//	v15 = (v6 << 16) + 32768;
//	v16 = (HIWORD(a3) << 16) + 32768;
//	if (!(v14 & 0x40000))
//		goto LABEL_34;
//	v17 = v16 >> 16;
//	v35 = v15 >> 16;
//	if (v13)
//	{
//		if ((signed int)(v15 >> 16) < v13->nSubtileX
//			|| v35 >= v13->nSubtileX + v13->nSubtileWidth
//			|| (v18 = v13->nSubtileY, v17 < v18)
//			|| v17 >= v18 + v13->nSubtileHeight)
//		{
//			pppRoom = 0;
//			pNumRooms = 0;
//			DUNGEON_GetAdjacentRoomsListFromRoom(v13, &pppRoom, &pNumRooms);
//			v19 = 0;
//			v38 = 0;
//			if (pNumRooms)
//			{
//				while (1)
//				{
//					v20 = pppRoom[v19];
//					if (v20)
//					{
//						if (v35 >= v20->nSubtileX)
//						{
//							if (v35 < v20->nSubtileX + v20->nSubtileWidth)
//							{
//								v21 = v20->nSubtileY;
//								if (v17 >= v21)
//								{
//									if (v17 < v21 + v20->nSubtileHeight)
//									{
//										v13 = pppRoom[v38];
//										goto LABEL_31;
//									}
//								}
//							}
//						}
//						v19 = v38;
//					}
//					++v19;
//					v38 = v19;
//					if (v19 >= pNumRooms)
//					{
//						v4 = (int)pRoom;
//						*(_DWORD *)(v3 + 40) = 0;
//						goto LABEL_37;
//					}
//				}
//			}
//			goto LABEL_32;
//		}
//LABEL_31:
//		if (v13)
//		{
//			v4 = (int)pRoom;
//LABEL_34:
//			*(_DWORD *)v3 = v15;
//			*(_DWORD *)(v3 + 4) = v16;
//			pX = v15 >> 11;
//			pY = v16 >> 11;
//			DUNGEON_FlattenCoords_IsoToCartesian(&pX, &pY);
//			v22 = pX;
//			*(_DWORD *)(v3 + 12) = pY;
//			v23 = *(_DWORD *)(v3 + 48);
//			*(_DWORD *)(v3 + 8) = v22;
//			if (v23 && *(_BYTE *)(v3 + 52) & 1)
//				sub_6FDADA20(v3, (D2RoomStrc*)v4);
//			goto LABEL_37;
//		}
//	}
//LABEL_32:
//	v4 = (int)pRoom;
//	*(_DWORD *)(v3 + 40) = 0;
//LABEL_37:
//	v24 = 0;
//	if (!a3)
//		goto LABEL_55;
//	if (!v4)
//		goto LABEL_53;
//	if (v42 >= *(_DWORD *)v4)
//	{
//		if (v42 < *(_DWORD *)v4 + *(_DWORD *)(v4 + 8))
//		{
//			v25 = *(_DWORD *)(v4 + 4);
//			if ((signed int)v43 >= v25)
//			{
//				if ((signed int)v43 < v25 + *(_DWORD *)(v4 + 12))
//					goto LABEL_52;
//			}
//		}
//	}
//	pY = 0;
//	a3 = 0;
//	DUNGEON_GetAdjacentRoomsListFromRoom((D2RoomStrc*)v4, (D2RoomStrc***)&pY, (int*)&a3);
//	if (a3 <= 0)
//	{
//LABEL_53:
//		FOG_Assertion(//			"COORD_TEST_EQUAL(tDest, sgctZeroGameCoord) || DungeonFindRoomGame( hRoomDest, tDest.wX, tDest.wY )", //			__FILE__, __LINE__//			1161);
//		exit(-1);
//	}
//	while (1)
//	{
//		v26 = *(_DWORD *)(pY + 4 * v24);
//		if (v26)
//		{
//			if (v42 >= *(_DWORD *)v26)
//			{
//				if (v42 < *(_DWORD *)v26 + *(_DWORD *)(v26 + 8))
//				{
//					v27 = *(_DWORD *)(v26 + 4);
//					if ((signed int)v43 >= v27)
//					{
//						if ((signed int)v43 < v27 + *(_DWORD *)(v26 + 12))
//							break;
//					}
//				}
//			}
//		}
//		++v24;
//		if (v24 >= a3)
//			goto LABEL_53;
//	}
//	v4 = *(_DWORD *)(pY + 4 * v24);
//LABEL_52:
//	if (!v4)
//		goto LABEL_53;
//LABEL_55:
//	v28 = *(_DWORD *)(v3 + 52);
//	v29 = (*(_DWORD *)v3 & 0xFFFF0000) + 32768;
//	v30 = (*(_DWORD *)(v3 + 4) & 0xFFFF0000) + 32768;
//	if (!(v28 & 0x40000) || COLLISION_GetRoomBySubTileCoordinates(*(D2RoomStrc**)(v3 + 28), v29 >> 16, v30 >> 16))
//	{
//		*(_DWORD *)v3 = v29;
//		*(_DWORD *)(v3 + 4) = v30;
//		a3 = v29 >> 11;
//		v43 = v30 >> 11;
//		DUNGEON_FlattenCoords_IsoToCartesian((int*)&a3, (int*)&v43);
//		v31 = *(_DWORD *)(v3 + 48);
//		v32 = v43;
//		*(_DWORD *)(v3 + 8) = a3;
//		*(_DWORD *)(v3 + 12) = v32;
//		if (v31 && *(_BYTE *)(v3 + 52) & 1)
//			sub_6FDADA20(v3, 0);
//	}
//	else
//	{
//		*(_DWORD *)(v3 + 40) = 0;
//	}
//	v33 = *(_DWORD *)(v3 + 52);
//	LOBYTE(v33) = v33 & 0xDF;
//	*(_DWORD *)(v3 + 52) = v33;
//	*(_DWORD *)(v3 + 40) = 0;
//	*(_DWORD *)(v3 + 36) = 0;
//	*(_DWORD *)(v3 + 114) = dword_6FDD258C;
//	*(_DWORD *)(v3 + 118) = dword_6FDD2590;
//	return 1;
//}

////D2Common.0x6FDADA20) --------------------------------------------------------
//void __fastcall sub_6FDADA20(int a1, D2RoomStrc*a2)
//{
//	int v2; // ebp@1
//	int v3; // eax@1
//	unsigned __int16 v4; // dx@1
//	unsigned __int16 v5; // si@1
//	signed int v6; // edi@4
//	D2RoomStrc*v7; // esi@6
//	int v8; // edi@6
//	int v9; // ebx@6
//	int v10; // ecx@9
//	int v11; // eax@16
//	unsigned int v12; // edx@19
//	D2RoomStrc*v13; // eax@20
//	int v14; // ecx@23
//	unsigned int v15; // edx@29
//	D2RoomStrc*v16; // eax@30
//	int v17; // ecx@33
//	int v18; // eax@42
//	D2UnitStrc*v19; // edi@42
//	int v20; // ecx@44
//	int v21; // [sp+10h] [bp-10h]@1
//	int pNumRooms; // [sp+14h] [bp-Ch]@19
//	D2RoomStrc** pppRoom; // [sp+18h] [bp-8h]@19
//	D2RoomStrc* pRoom; // [sp+1Ch] [bp-4h]@1
//
//	v2 = a1;
//	pRoom = a2;
//	v3 = *(_DWORD *)(a1 + 28);
//	v4 = *(_WORD *)(a1 + 2);
//	v5 = *(_WORD *)(a1 + 6);
//	v21 = a1;
//	if (v3)
//	{
//		if ((signed int)v4 >= *(_DWORD *)v3)
//		{
//			if (v4 < *(_DWORD *)v3 + *(_DWORD *)(v3 + 8))
//			{
//				v6 = *(_DWORD *)(v3 + 4);
//				if (v5 >= v6)
//				{
//					if (v5 < v6 + *(_DWORD *)(v3 + 12))
//						return;
//				}
//			}
//		}
//	}
//	v7 = 0;
//	v8 = *(_WORD *)(a1 + 6);
//	v9 = v4;
//	if (v3)
//	{
//		if ((signed int)v4 < *(_DWORD *)v3
//			|| v4 >= *(_DWORD *)v3 + *(_DWORD *)(v3 + 8)
//			|| (v10 = *(_DWORD *)(v3 + 4), v8 < v10)
//			|| v8 >= v10 + *(_DWORD *)(v3 + 12))
//		{
//			pppRoom = 0;
//			pNumRooms = 0;
//			DUNGEON_GetAdjacentRoomsListFromRoom((D2RoomStrc*)v3, &pppRoom, &pNumRooms);
//			v12 = 0;
//			if (pNumRooms)
//			{
//				do
//				{
//					v13 = pppRoom[v12];
//					if (v13 && v9 >= v13->nSubtileX)
//					{
//						if (v9 < v13->nSubtileX + v13->nSubtileWidth)
//						{
//							v14 = v13->nSubtileY;
//							if (v8 >= v14)
//							{
//								if (v8 < v14 + v13->nSubtileHeight)
//								{
//									v7 = pppRoom[v12];
//									v2 = v21;
//									goto LABEL_12;
//								}
//							}
//						}
//						v2 = v21;
//					}
//					++v12;
//				}
//				while (v12 < pNumRooms);
//			}
//			v7 = 0;
//		}
//		else
//		{
//			v7 = (D2RoomStrc*)v3;
//LABEL_12:
//			if (v7)
//				goto LABEL_42;
//		}
//	}
//	if (!pRoom)
//		goto LABEL_40;
//	if (v9 >= pRoom->nSubtileX)
//	{
//		if (v9 < pRoom->nSubtileX + pRoom->nSubtileWidth)
//		{
//			v11 = pRoom->nSubtileY;
//			if (v8 >= v11)
//			{
//				if (v8 < v11 + pRoom->nSubtileHeight)
//				{
//					v7 = pRoom;
//					goto LABEL_39;
//				}
//			}
//		}
//	}
//	pppRoom = 0;
//	pNumRooms = 0;
//	DUNGEON_GetAdjacentRoomsListFromRoom(pRoom, &pppRoom, &pNumRooms);
//	v15 = 0;
//	if (!pNumRooms)
//	{
//LABEL_37:
//		v7 = 0;
//		goto LABEL_40;
//	}
//	while (1)
//	{
//		v16 = pppRoom[v15];
//		if (!v16 || v9 < v16->nSubtileX)
//			goto LABEL_36;
//		if (v9 < v16->nSubtileX + v16->nSubtileWidth)
//		{
//			v17 = v16->nSubtileY;
//			if (v8 >= v17)
//			{
//				if (v8 < v17 + v16->nSubtileHeight)
//					break;
//			}
//		}
//		v2 = v21;
//LABEL_36:
//		++v15;
//		if (v15 >= pNumRooms)
//			goto LABEL_37;
//	}
//	v7 = pppRoom[v15];
//	v2 = v21;
//LABEL_39:
//	if (!v7)
//	{
//LABEL_40:
//		if (*(_DWORD *)(v2 + 52) & 0x40000)
//		{
//			*(_DWORD *)(v2 + 40) = 0;
//			return;
//		}
//	}
//LABEL_42:
//	v18 = *(_DWORD *)(v2 + 28);
//	v19 = *(D2UnitStrc**)(v2 + 48);
//	*(_DWORD *)(v2 + 32) = v18;
//	if (v18)
//		UNITROOM_RemoveUnitFromRoom(v19);
//	v20 = *(_DWORD *)(v2 + 52);
//	*(_DWORD *)(v2 + 28) = v7;
//	*(_DWORD *)(v2 + 52) = v20 | 2;
//	if (v7)
//	{
//		UNITROOM_AddUnitToRoom(v19, v7);
//		UNITROOM_RefreshUnit(v19);
//	}
//}

//D2Common.0x6FDADC20) --------------------------------------------------------
signed int __stdcall D2Common_10231(int a1, int a2, int pNumRooms, unsigned __int16 a4, D2RoomStrc** pppRoom)
{
//	int v5; // esi@1
//	int v6; // eax@1
//	__int16 v7; // dx@1
//	D2RoomStrc*v8; // ecx@11
//	int v9; // eax@12
//	int v10; // eax@13
//	int v11; // ebx@13
//	unsigned int v12; // edi@13
//	unsigned int v13; // ebp@13
//	int v14; // edi@16
//	int v15; // eax@22
//	int v16; // ebp@22
//	D2RoomStrc*v17; // edx@24
//	int v18; // edx@25
//	int v19; // eax@25
//	unsigned int v20; // edx@29
//	D2RoomStrc*v21; // eax@30
//	int v22; // ecx@33
//	int v23; // eax@41
//	int pY; // [sp+10h] [bp-Ch]@25
//	int v26; // [sp+14h] [bp-8h]@1
//
//	v5 = a1;
//	v6 = *(_DWORD *)(a1 + 48);
//	v7 = *(_WORD *)(a1 + 6);
//	LOWORD(v26) = *(_WORD *)(a1 + 2);
//	HIWORD(v26) = v7;
//	if (v6 && *(_DWORD *)v6 == 3)
//	{
//		if (a4 || (_WORD)pppRoom)
//			sub_6FD44E00(//				*(_DWORD *)(a1 + 28), //				(unsigned __int16)v26, //				HIWORD(v26), //				pNumRooms, //				a4, //				(unsigned __int16)pppRoom, //				*(_DWORD *)(a1 + 68), //				*(_DWORD *)(a1 + 76));
//		else
//			COLLISION_ResetMaskWithSize(//				*(D2RoomStrc**)(a1 + 28), //				(unsigned __int16)v26, //				HIWORD(v26), //				*(_DWORD *)(a1 + 68), //				*(_DWORD *)(a1 + 76));
//	}
//	else
//	{
//		if (a4 || (_WORD)pppRoom)
//			D2Common_10133(//				*(D2RoomStrc**)(a1 + 28), //				(unsigned __int16)v26, //				HIWORD(v26), //				(D2RoomStrc*)pNumRooms, //				a4, //				(unsigned __int16)pppRoom, //				*(_DWORD *)(a1 + 72), //				*(_DWORD *)(a1 + 76));
//		else
//			COLLISION_ResetMaskWithPattern(//				*(D2RoomStrc**)(a1 + 28), //				(unsigned __int16)v26, //				HIWORD(v26), //				*(_DWORD *)(a1 + 72), //				*(_DWORD *)(a1 + 76));
//	}
//	v8 = *(D2RoomStrc**)(v5 + 28);
//	if (v8 != (D2RoomStrc*)pNumRooms)
//	{
//		v9 = *(_DWORD *)(v5 + 52);
//		LOBYTE(v9) = v9 | 1;
//		*(_DWORD *)(v5 + 52) = v9;
//	}
//	v10 = *(_DWORD *)(v5 + 52);
//	v11 = a4;
//	v26 = (unsigned __int16)pppRoom;
//	v12 = (a4 << 16) + 32768;
//	v13 = ((unsigned __int16)pppRoom << 16) + 32768;
//	if (v10 & 0x40000 && !COLLISION_GetRoomBySubTileCoordinates(v8, v12 >> 16, v13 >> 16))
//	{
//		*(_DWORD *)(v5 + 40) = 0;
//LABEL_16:
//		v14 = pNumRooms;
//		goto LABEL_17;
//	}
//	*(_DWORD *)v5 = v12;
//	*(_DWORD *)(v5 + 4) = v13;
//	a1 = v12 >> 11;
//	pY = v13 >> 11;
//	DUNGEON_FlattenCoords_IsoToCartesian(&a1, &pY);
//	v18 = a1;
//	*(_DWORD *)(v5 + 12) = pY;
//	v19 = *(_DWORD *)(v5 + 48);
//	*(_DWORD *)(v5 + 8) = v18;
//	if (!v19 || !(*(_BYTE *)(v5 + 52) & 1))
//		goto LABEL_16;
//	v14 = pNumRooms;
//	sub_6FDADA20(v5, (D2RoomStrc*)pNumRooms);
//LABEL_17:
//	if (!a4 && !(_WORD)pppRoom)
//		goto LABEL_41;
//	if (!v14)
//		goto LABEL_39;
//	if (v11 < *(_DWORD *)v14 || v11 >= *(_DWORD *)v14 + *(_DWORD *)(v14 + 8))
//	{
//		v16 = v26;
//	}
//	else
//	{
//		v15 = *(_DWORD *)(v14 + 4);
//		v16 = v26;
//		if (v26 >= v15 && v26 < v15 + *(_DWORD *)(v14 + 12))
//		{
//			v17 = (D2RoomStrc*)v14;
//			goto LABEL_38;
//		}
//	}
//	pppRoom = 0;
//	pNumRooms = 0;
//	DUNGEON_GetAdjacentRoomsListFromRoom((D2RoomStrc*)v14, &pppRoom, &pNumRooms);
//	v20 = 0;
//	if (!pNumRooms)
//	{
//LABEL_39:
//		FOG_Assertion(//			"COORD_TEST_EQUAL(tDest, sgctZeroGameCoord) || DungeonFindRoomGame( hRoomDest, tDest.wX, tDest.wY )", //			__FILE__, __LINE__//			1228);
//		exit(-1);
//	}
//	while (1)
//	{
//		v21 = pppRoom[v20];
//		if (v21)
//		{
//			if (v11 >= v21->nSubtileX)
//			{
//				if (v11 < v21->nSubtileX + v21->nSubtileWidth)
//				{
//					v22 = v21->nSubtileY;
//					if (v16 >= v22)
//					{
//						if (v16 < v22 + v21->nSubtileHeight)
//							break;
//					}
//				}
//			}
//		}
//		++v20;
//		if (v20 >= pNumRooms)
//			goto LABEL_39;
//	}
//	v17 = pppRoom[v20];
//LABEL_38:
//	if (!v17)
//		goto LABEL_39;
//LABEL_41:
//	D2Common_10233((D2DynamicPathStrc*)v5);
//	v23 = *(_DWORD *)(v5 + 52);
//	LOBYTE(v23) = v23 & 0xDF;
//	*(_DWORD *)(v5 + 52) = v23;
//	*(_DWORD *)(v5 + 40) = 0;
//	*(_DWORD *)(v5 + 36) = 0;
//	*(_DWORD *)(v5 + 114) = dword_6FDD258C;
//	*(_DWORD *)(v5 + 118) = dword_6FDD2590;
//	*(_DWORD *)(v5 + 40) = 0;
	return 1;
}

//D2Common.0x6FDADF00) --------------------------------------------------------
BOOL __stdcall D2Common_10230(D2DynamicPathStrc* pDynamicPath, int a2, D2RoomStrc* a3, unsigned int a4, __int16 a5)
{
	FOG_WriteToLogFile("D2Common_10230: Not yet implemented");
//	HIWORD(a4) = a5;
//	if (sub_6FDAD5E0(pDynamicPath, a3, a4))
//	{
//		pDynamicPath->unk0x38 = 15;
//		return TRUE;
//	}
//
	REMOVE_LATER_WriteToLogFile("D2Common_10230: Not yet implemented");
	return FALSE;
}

//D2Common.0x6FDADF50) --------------------------------------------------------
signed int __stdcall D2Common_10232(D2DynamicPathStrc* a1, D2UnitStrc* a2, unsigned int pNumRooms, int a4, int pppRoom)
{
	FOG_WriteToLogFile("D2Common_10232: Not yet implemented");
//	int v5; // ebp@5
//	D2DynamicPathStrc*v6; // esi@5
//	unsigned int v7; // ebx@5
//	int v8; // edi@5
//	signed int result; // eax@6
//	D2RoomStrc*v10; // ecx@7
//	uint32_t v11; // eax@8
//	uint32_t v12; // eax@9
//	unsigned int v13; // edi@9
//	unsigned int v14; // ebp@9
//	uint32_t v15; // edx@12
//	D2UnitStrc*v16; // eax@12
//	D2RoomStrc*v17; // ecx@14
//	uint16_t v18; // bp@14
//	uint16_t v19; // di@14
//	signed int v20; // edx@17
//	int v21; // edi@20
//	signed __int32 v22; // ebp@20
//	D2RoomStrc*v23; // eax@20
//	int v24; // ebp@28
//	int v25; // eax@30
//	int v26; // edi@30
//	unsigned int v27; // edx@34
//	int v28; // eax@35
//	int v29; // ecx@38
//	uint32_t v30; // eax@46
//	unsigned int v31; // edi@46
//	unsigned int v32; // ebp@46
//	unsigned int v33; // edx@49
//	D2UnitStrc*v34; // eax@49
//	uint32_t v35; // eax@52
//
//	if (!a2 || a2->dwUnitType && a2->dwUnitType != 1)
//		FOG_10025(//			"UnitGetType(hUnit) == UNIT_PLAYER || UnitGetType(hUnit) == UNIT_MONSTER", //			__FILE__, __LINE__//			1293);
//	v5 = pppRoom;
//	v6 = a1;
//	v7 = pNumRooms;
//	v8 = a4;
//	if (sub_6FD44FF0(//		a1->pRoom, //		a1->wPosX, //		a1->wPosY, //		(D2RoomStrc*)pNumRooms, //		a4, //		pppRoom, //		a1->dwCollisionPattern, //		a1->dwCollisionType, //		a1->unk0x50))
//		return 0;
//	v10 = v6->pRoom;
//	if (v10 != (D2RoomStrc*)v7)
//	{
//		v11 = v6->dwFlags;
//		LOBYTE(v11) = v11 | 1;
//		v6->dwFlags = v11;
//	}
//	v12 = v6->dwFlags;
//	v13 = (v8 << 16) + 32768;
//	v14 = (v5 << 16) + 32768;
//	if (!(v12 & 0x40000) || COLLISION_GetRoomBySubTileCoordinates(v10, v13 >> 16, v14 >> 16))
//	{
//		*(_DWORD *)&v6->wOffsetX = v13;
//		*(_DWORD *)&v6->wOffsetY = v14;
//		a2 = (D2UnitStrc*)(v13 >> 11);
//		a1 = (D2DynamicPathStrc*)(v14 >> 11);
//		DUNGEON_FlattenCoords_IsoToCartesian((int*)&a2, (int*)&a1);
//		v15 = (uint32_t)a2;
//		v6->dwTargetY = (uint32_t)a1;
//		v16 = v6->pUnit;
//		v6->dwTargetX = v15;
//		if (!v16 || !(v6->dwFlags & 1))
//			goto LABEL_27;
//		v17 = v6->pRoom;
//		v18 = v6->wPosX;
//		v19 = v6->wPosY;
//		if (v17 && v18 >= v17->nSubtileX)
//		{
//			if (v18 < v17->nSubtileX + v17->nSubtileWidth)
//			{
//				v20 = v17->nSubtileY;
//				if (v19 >= v20)
//				{
//					if (v19 < v20 + v17->nSubtileHeight)
//					{
//						v7 = pNumRooms;
//						goto LABEL_27;
//					}
//				}
//			}
//			v7 = pNumRooms;
//		}
//		v21 = v6->wPosY;
//		v22 = v6->wPosX;
//		v23 = COLLISION_GetRoomBySubTileCoordinates(v17, v6->wPosX, v6->wPosY);
//		if (v23
//			|| v7 && (v23 = COLLISION_GetRoomBySubTileCoordinates((D2RoomStrc*)v7, v22, v21)) != 0
//			|| !(v6->dwFlags & 0x40000))
//			PATHMISC_UpdateRoom(v6, v23);
//		else
//			v6->dwPathPoints = 0;
//		goto LABEL_27;
//	}
//	v6->dwPathPoints = 0;
//LABEL_27:
//	if (!v7)
//		goto LABEL_44;
//	v24 = a4;
//	if (a4 < *(_DWORD *)v7 || a4 >= *(_DWORD *)v7 + *(_DWORD *)(v7 + 8))
//	{
//		v26 = pppRoom;
//	}
//	else
//	{
//		v25 = *(_DWORD *)(v7 + 4);
//		v26 = pppRoom;
//		if (pppRoom >= v25 && pppRoom < v25 + *(_DWORD *)(v7 + 12))
//			goto LABEL_43;
//	}
//	pppRoom = 0;
//	pNumRooms = 0;
//	DUNGEON_GetAdjacentRoomsListFromRoom((D2RoomStrc*)v7, (D2RoomStrc***)&pppRoom, (int*)&pNumRooms);
//	v27 = 0;
//	if (!pNumRooms)
//	{
//LABEL_44:
//		FOG_Assertion(//			"DungeonFindRoomGame( hRoomDest, nX, nY )", //			__FILE__, __LINE__//			1316);
//		exit(-1);
//	}
//	while (1)
//	{
//		v28 = *(_DWORD *)(pppRoom + 4 * v27);
//		if (v28)
//		{
//			if (v24 >= *(_DWORD *)v28)
//			{
//				if (v24 < *(_DWORD *)v28 + *(_DWORD *)(v28 + 8))
//				{
//					v29 = *(_DWORD *)(v28 + 4);
//					if (v26 >= v29)
//					{
//						if (v26 < v29 + *(_DWORD *)(v28 + 12))
//							break;
//					}
//				}
//			}
//		}
//		++v27;
//		if (v27 >= pNumRooms)
//			goto LABEL_44;
//	}
//	v7 = *(_DWORD *)(pppRoom + 4 * v27);
//LABEL_43:
//	if (!v7)
//		goto LABEL_44;
//	v30 = v6->dwFlags;
//	v31 = (*(_DWORD *)&v6->wOffsetX & 0xFFFF0000) + 32768;
//	v32 = (*(_DWORD *)&v6->wOffsetY & 0xFFFF0000) + 32768;
//	if (!(v30 & 0x40000) || COLLISION_GetRoomBySubTileCoordinates(v6->pRoom, v31 >> 16, v32 >> 16))
//	{
//		*(_DWORD *)&v6->wOffsetX = v31;
//		*(_DWORD *)&v6->wOffsetY = v32;
//		pNumRooms = v31 >> 11;
//		pppRoom = v32 >> 11;
//		DUNGEON_FlattenCoords_IsoToCartesian((int*)&pNumRooms, &pppRoom);
//		v33 = pNumRooms;
//		v6->dwTargetY = pppRoom;
//		v34 = v6->pUnit;
//		v6->dwTargetX = v33;
//		if (v34 && v6->dwFlags & 1)
//			sub_6FDADA20((int)v6, 0);
//	}
//	else
//	{
//		v6->dwPathPoints = 0;
//	}
//	v35 = v6->dwFlags;
//	LOBYTE(v35) = v35 & 0xDF;
//	v6->dwFlags = v35;
//	v6->dwPathPoints = 0;
//	v6->unk0x24 = 0;
//	result = 1;
//	v6->unk0x72 = dword_6FDD258C;
//	v6->unk0x76 = dword_6FDD2590;
//	return result;

REMOVE_LATER_WriteToLogFile("D2Common_10232: Not yet implemented");
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

////D2Common.0x6FDAE290) --------------------------------------------------------
//void __stdcall D2Common_10233(D2DynamicPathStrc* pDynamicPath)
//{
//	D2DynamicPathStrc*v1; // ecx@1
//	uint32_t v2; // eax@1
//	unsigned int v3; // ebx@1
//	unsigned int v4; // ebp@1
//	D2RoomStrc*v5; // eax@2
//	int v6; // esi@2
//	int v7; // edi@2
//	int v8; // ecx@5
//	unsigned int v9; // edx@7
//	D2RoomStrc*v10; // eax@8
//	int v11; // ecx@11
//	D2DynamicPathStrc*v12; // ebp@21
//	int v13; // edx@21
//	D2UnitStrc*v14; // eax@21
//	D2RoomStrc*v15; // eax@23
//	uint16_t v16; // cx@23
//	uint16_t v17; // dx@23
//	signed int v18; // edi@26
//	int v19; // esi@28
//	int v20; // edi@28
//	int v21; // ecx@32
//	unsigned int v22; // edx@34
//	D2RoomStrc*v23; // eax@35
//	int v24; // ecx@38
//	int pNumRooms; // [sp+10h] [bp-14h]@7
//	D2RoomStrc** pppRoom; // [sp+14h] [bp-10h]@7
//	unsigned int v27; // [sp+18h] [bp-Ch]@7
//	int pX; // [sp+1Ch] [bp-8h]@21
//	int pY; // [sp+20h] [bp-4h]@21
//
//	v1 = pDynamicPath;
//	v2 = pDynamicPath->dwFlags;
//	v3 = (*(_DWORD *)&pDynamicPath->wOffsetX & 0xFFFF0000) + 32768;
//	v4 = (*(_DWORD *)&pDynamicPath->wOffsetY & 0xFFFF0000) + 32768;
//	if (v2 & 0x40000)
//	{
//		v5 = pDynamicPath->pRoom;
//		v6 = v4 >> 16;
//		v7 = v3 >> 16;
//		if (!v5)
//		{
//LABEL_19:
//			v1->dwPathPoints = 0;
//			return;
//		}
//		if (v7 < v5->nSubtileX
//			|| v7 >= v5->nSubtileX + v5->nSubtileWidth
//			|| (v8 = v5->nSubtileY, v6 < v8)
//			|| v6 >= v8 + v5->nSubtileHeight)
//		{
//			pppRoom = 0;
//			pNumRooms = 0;
//			DUNGEON_GetAdjacentRoomsListFromRoom(v5, &pppRoom, &pNumRooms);
//			v9 = 0;
//			v27 = 0;
//			if (pNumRooms)
//			{
//				while (1)
//				{
//					v10 = pppRoom[v9];
//					if (v10 && v7 >= v10->nSubtileX)
//					{
//						if (v7 < v10->nSubtileX + v10->nSubtileWidth)
//						{
//							v11 = v10->nSubtileY;
//							if (v6 >= v11)
//							{
//								if (v6 < v11 + v10->nSubtileHeight)
//								{
//									v5 = pppRoom[v27];
//									goto LABEL_17;
//								}
//							}
//						}
//						v9 = v27;
//					}
//					++v9;
//					v27 = v9;
//					if (v9 >= pNumRooms)
//					{
//						pDynamicPath->dwPathPoints = 0;
//						return;
//					}
//				}
//			}
//			goto LABEL_18;
//		}
//LABEL_17:
//		if (!v5)
//		{
//LABEL_18:
//			v1 = pDynamicPath;
//			goto LABEL_19;
//		}
//		v1 = pDynamicPath;
//	}
//	*(_DWORD *)&v1->wOffsetX = v3;
//	*(_DWORD *)&v1->wOffsetY = v4;
//	pX = v3 >> 11;
//	pY = v4 >> 11;
//	DUNGEON_FlattenCoords_IsoToCartesian(&pX, &pY);
//	v12 = pDynamicPath;
//	v13 = pX;
//	pDynamicPath->dwTargetY = pY;
//	v14 = pDynamicPath->pUnit;
//	pDynamicPath->dwTargetX = v13;
//	if (v14)
//	{
//		if (pDynamicPath->dwFlags & 1)
//		{
//			v15 = pDynamicPath->pRoom;
//			v16 = pDynamicPath->wPosX;
//			v17 = pDynamicPath->wPosY;
//			if (!v15
//				|| v16 < v15->nSubtileX
//				|| v16 >= v15->nSubtileX + v15->nSubtileWidth
//				|| (v18 = v15->nSubtileY, v17 < v18)
//				|| v17 >= v18 + v15->nSubtileHeight)
//			{
//				v19 = pDynamicPath->wPosY;
//				v20 = pDynamicPath->wPosX;
//				if (v15)
//				{
//					if (pDynamicPath->wPosX >= v15->nSubtileX)
//					{
//						if (v20 < v15->nSubtileX + v15->nSubtileWidth)
//						{
//							v21 = v15->nSubtileY;
//							if (v19 >= v21)
//							{
//								if (v19 < v21 + v15->nSubtileHeight)
//								{
//LABEL_44:
//									if (v15)
//									{
//LABEL_47:
//										PATHMISC_UpdateRoom(v12, v15);
//										return;
//									}
//LABEL_45:
//									if (v12->dwFlags & 0x40000)
//									{
//										v12->dwPathPoints = 0;
//										return;
//									}
//									goto LABEL_47;
//								}
//							}
//						}
//					}
//					pppRoom = 0;
//					v27 = 0;
//					DUNGEON_GetAdjacentRoomsListFromRoom(v15, &pppRoom, (int*)&v27);
//					v22 = 0;
//					if (v27)
//					{
//						while (1)
//						{
//							v23 = pppRoom[v22];
//							if (v23 && v20 >= v23->nSubtileX)
//							{
//								if (v20 < v23->nSubtileX + v23->nSubtileWidth)
//								{
//									v24 = v23->nSubtileY;
//									if (v19 >= v24)
//									{
//										if (v19 < v24 + v23->nSubtileHeight)
//										{
//											v15 = pppRoom[v22];
//											v12 = pDynamicPath;
//											goto LABEL_44;
//										}
//									}
//								}
//								v12 = pDynamicPath;
//							}
//							++v22;
//							if (v22 >= v27)
//							{
//								v15 = 0;
//								goto LABEL_45;
//							}
//						}
//					}
//				}
//				v15 = 0;
//				goto LABEL_45;
//			}
//		}
//	}
//}

//D2Common.0x6FDAE500 (#10234)
//TODO: Find a name
BOOL __stdcall D2Common_10234(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath)
	{
		return pDynamicPath->dwFlags & 8;
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
						if (pRiderPath->dwFlags & 0x40000)
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
							if (pRiderPath->dwFlags & 0x40000)
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
								if (pRiderPath->dwFlags & 0x40000)
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
					if (pRiderPath->dwFlags & 0x40000)
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
