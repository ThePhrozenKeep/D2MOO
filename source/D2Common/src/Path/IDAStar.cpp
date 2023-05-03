#include "Path/Path.h"
#include "Path/Path_IDAStar.h"

#include "D2Collision.h"


const int dword_6FDD17E0[] =
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

const int dword_6FDD19E0[] =
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

//D2Common.0x6FDA69E0
int __fastcall sub_6FDA69E0(D2PathInfoStrc* pPathInfo)
{
	D2_ASSERTM(false, "Not implemented yet.");
	return 0;
}

//D2Common.0x6FDA6D10
void __fastcall sub_6FDA6D10(D2PathInfoStrc** ppPathInfo, D2PathInfoStrc* pPathPoint)
{
	int nIndex = ((uint8_t)dword_6FDD17E0[pPathPoint->pStartCoord.X & 0x7F] + (uint8_t)dword_6FDD19E0[pPathPoint->pStartCoord.Y & 0x7F]) & 0x7F;

	pPathPoint->pNext = ppPathInfo[nIndex + 128];
	ppPathInfo[nIndex + 128] = pPathPoint;
}

////D2Common.0x6FDA6D50) --------------------------------------------------------
//signed int __fastcall sub_6FDA6D50(int a1, int a2, int a3, int a4)
//{
//	unsigned __int16 v4; // di@1
//	unsigned __int16 v5; // bx@1
//	signed __int16 v6; // ax@3
//	__int16 v7; // bx@5
//	int v8; // eax@5
//	__int16 v9; // cx@23
//	int v10; // eax@24
//	int v11; // edi@43
//	__int16 v12; // ax@44
//	signed int result; // eax@61
//	int v14; // [sp+10h] [bp-1Ch]@1
//	int v15; // [sp+18h] [bp-14h]@1
//	int v16; // [sp+1Ch] [bp-10h]@1
//	int v17; // [sp+20h] [bp-Ch]@1
//	int v18; // [sp+24h] [bp-8h]@1
//	int v19; // [sp+24h] [bp-8h]@60
//	int v20; // [sp+24h] [bp-8h]@62
//	int v21; // [sp+24h] [bp-8h]@64
//	int v22; // [sp+24h] [bp-8h]@66
//	int v23; // [sp+24h] [bp-8h]@68
//	int v24; // [sp+24h] [bp-8h]@70
//	int v25; // [sp+24h] [bp-8h]@72
//
//	v14 = a2;
//	v4 = *(_WORD *)a3 - 1;
//	v5 = *(_WORD *)(a3 + 2) - 1;
//	LOWORD(v18) = v4;
//	HIWORD(v18) = v5;
//	v15 = *(_DWORD *)(a1 + 44);
//	v16 = *(_DWORD *)(a1 + 40);
//	v17 = *(_DWORD *)(a1 + 8);
//	if (!COLLISION_CheckAnyCollisionWithPattern(*(D2RoomStrc**)(a1 + 8), v4, v5, *(_DWORD *)(a1 + 40), *(_DWORD *)(a1 + 44)))
//	{
//		if (*(_WORD *)a3 == v4 || (v6 = 3, *(_WORD *)(a3 + 2) == v5))
//			v6 = 2;
//		v7 = v6 + *(_WORD *)(a3 + 8);
//		v8 = sub_6FDA7280((void*)v14, v18);
//		if (v8)
//		{
//			if (*(_DWORD *)(a3 + 16))
//			{
//				if (*(_DWORD *)(a3 + 20))
//				{
//					if (*(_DWORD *)(a3 + 24))
//					{
//						if (*(_DWORD *)(a3 + 28))
//						{
//							if (*(_DWORD *)(a3 + 32))
//							{
//								if (*(_DWORD *)(a3 + 36))
//								{
//									if (*(_DWORD *)(a3 + 40))
//									{
//										if (!*(_DWORD *)(a3 + 44))
//											*(_DWORD *)(a3 + 44) = v8;
//									}
//									else
//									{
//										*(_DWORD *)(a3 + 40) = v8;
//									}
//								}
//								else
//								{
//									*(_DWORD *)(a3 + 36) = v8;
//								}
//							}
//							else
//							{
//								*(_DWORD *)(a3 + 32) = v8;
//							}
//						}
//						else
//						{
//							*(_DWORD *)(a3 + 28) = v8;
//						}
//					}
//					else
//					{
//						*(_DWORD *)(a3 + 24) = v8;
//					}
//				}
//				else
//				{
//					*(_DWORD *)(a3 + 20) = v8;
//				}
//			}
//			else
//			{
//				*(_DWORD *)(a3 + 16) = v8;
//			}
//			if (v7 < *(_WORD *)(v8 + 8))
//			{
//				v9 = *(_WORD *)(v8 + 6);
//				*(_DWORD *)(v8 + 12) = a3;
//				*(_WORD *)(v8 + 8) = v7;
//				*(_WORD *)(v8 + 4) = v7 + v9;
//			}
//		}
//		else
//		{
//			v10 = sub_6FDA72D0((void*)v14, v18);
//			if (v10)
//			{
//				if (*(_DWORD *)(a3 + 16))
//				{
//					if (*(_DWORD *)(a3 + 20))
//					{
//						if (*(_DWORD *)(a3 + 24))
//						{
//							if (*(_DWORD *)(a3 + 28))
//							{
//								if (*(_DWORD *)(a3 + 32))
//								{
//									if (*(_DWORD *)(a3 + 36))
//									{
//										if (*(_DWORD *)(a3 + 40))
//										{
//											if (!*(_DWORD *)(a3 + 44))
//												*(_DWORD *)(a3 + 44) = v10;
//										}
//										else
//										{
//											*(_DWORD *)(a3 + 40) = v10;
//										}
//									}
//									else
//									{
//										*(_DWORD *)(a3 + 36) = v10;
//									}
//								}
//								else
//								{
//									*(_DWORD *)(a3 + 32) = v10;
//								}
//							}
//							else
//							{
//								*(_DWORD *)(a3 + 28) = v10;
//							}
//						}
//						else
//						{
//							*(_DWORD *)(a3 + 24) = v10;
//						}
//					}
//					else
//					{
//						*(_DWORD *)(a3 + 20) = v10;
//					}
//				}
//				else
//				{
//					*(_DWORD *)(a3 + 16) = v10;
//				}
//				if (v7 < *(_WORD *)(v10 + 8))
//				{
//					*(_WORD *)(v10 + 4) = v7 + *(_WORD *)(v10 + 6);
//					*(_DWORD *)(v10 + 12) = a3;
//					*(_WORD *)(v10 + 8) = v7;
//					sub_6FDA7390(v14, v10);
//				}
//			}
//			else
//			{
//				v11 = sub_6FDA7450(v14);
//				if (!v11)
//					return 0;
//				*(_DWORD *)(v11 + 12) = a3;
//				*(_WORD *)(v11 + 8) = v7;
//				v12 = sub_6FDA7230(v18, a4);
//				*(_WORD *)(v11 + 6) = v12;
//				*(_WORD *)(v11 + 4) = v7 + v12;
//				*(_DWORD *)v11 = v18;
//				sub_6FDA7320(v14, v11);
//				if (*(_DWORD *)(a3 + 16))
//				{
//					if (*(_DWORD *)(a3 + 20))
//					{
//						if (*(_DWORD *)(a3 + 24))
//						{
//							if (*(_DWORD *)(a3 + 28))
//							{
//								if (*(_DWORD *)(a3 + 32))
//								{
//									if (*(_DWORD *)(a3 + 36))
//									{
//										if (*(_DWORD *)(a3 + 40))
//										{
//											if (!*(_DWORD *)(a3 + 44))
//												*(_DWORD *)(a3 + 44) = v11;
//										}
//										else
//										{
//											*(_DWORD *)(a3 + 40) = v11;
//										}
//									}
//									else
//									{
//										*(_DWORD *)(a3 + 36) = v11;
//									}
//								}
//								else
//								{
//									*(_DWORD *)(a3 + 32) = v11;
//								}
//							}
//							else
//							{
//								*(_DWORD *)(a3 + 28) = v11;
//							}
//						}
//						else
//						{
//							*(_DWORD *)(a3 + 24) = v11;
//						}
//					}
//					else
//					{
//						*(_DWORD *)(a3 + 20) = v11;
//					}
//				}
//				else
//				{
//					*(_DWORD *)(a3 + 16) = v11;
//				}
//			}
//		}
//	}
//	LOWORD(v19) = *(_WORD *)a3 - 1;
//	HIWORD(v19) = *(_WORD *)(a3 + 2) + 1;
//	if (COLLISION_CheckAnyCollisionWithPattern((D2RoomStrc*)v17, (unsigned __int16)v19, HIWORD(v19), v16, v15)
//		|| (result = sub_6FDA7490(v14, v17, a3, v19, a4)) != 0)
//	{
//		HIWORD(v20) = *(_WORD *)(a3 + 2) - 1;
//		LOWORD(v20) = *(_WORD *)a3 + 1;
//		if (COLLISION_CheckAnyCollisionWithPattern((D2RoomStrc*)v17, (unsigned __int16)(*(_WORD *)a3 + 1), HIWORD(v20), v16, v15)
//			|| (result = sub_6FDA7490(v14, v17, a3, v20, a4)) != 0)
//		{
//			HIWORD(v21) = *(_WORD *)(a3 + 2) + 1;
//			LOWORD(v21) = *(_WORD *)a3 + 1;
//			if (COLLISION_CheckAnyCollisionWithPattern((D2RoomStrc*)v17, (unsigned __int16)(*(_WORD *)a3 + 1), HIWORD(v21), v16, v15)
//				|| (result = sub_6FDA7490(v14, v17, a3, v21, a4)) != 0)
//			{
//				HIWORD(v22) = *(_WORD *)(a3 + 2);
//				LOWORD(v22) = *(_WORD *)a3 - 1;
//				if (COLLISION_CheckAnyCollisionWithPattern((D2RoomStrc*)v17, (unsigned __int16)(*(_WORD *)a3 - 1), HIWORD(v22), v16, v15)
//					|| (result = sub_6FDA7490(v14, v17, a3, v22, a4)) != 0)
//				{
//					LOWORD(v23) = *(_WORD *)a3;
//					HIWORD(v23) = *(_WORD *)(a3 + 2) - 1;
//					if (COLLISION_CheckAnyCollisionWithPattern((D2RoomStrc*)v17, *(_WORD *)a3, HIWORD(v23), v16, v15)
//						|| (result = sub_6FDA7490(v14, v17, a3, v23, a4)) != 0)
//					{
//						HIWORD(v24) = *(_WORD *)(a3 + 2);
//						LOWORD(v24) = *(_WORD *)a3 + 1;
//						if (COLLISION_CheckAnyCollisionWithPattern(//							(D2RoomStrc*)v17, //							(unsigned __int16)(*(_WORD *)a3 + 1), //							HIWORD(v24), //							v16, //							v15)
//							|| (result = sub_6FDA7490(v14, v17, a3, v24, a4)) != 0)
//						{
//							LOWORD(v25) = *(_WORD *)a3;
//							HIWORD(v25) = *(_WORD *)(a3 + 2) + 1;
//							if (!COLLISION_CheckAnyCollisionWithPattern((D2RoomStrc*)v17, *(_WORD *)a3, HIWORD(v25), v16, v15)
//								&& !sub_6FDA7490(v14, v17, a3, v25, a4))
//								return 0;
//							result = 1;
//						}
//					}
//				}
//			}
//		}
//	}
//	return result;
//}

//D2Common.0x6FDA7230
int __stdcall sub_6FDA7230(D2PathPointStrc pPoint1, D2PathPointStrc pPoint2)
{
	int nDiffX = 0;
	int nDiffY = 0;

	nDiffX = pPoint1.X - pPoint2.X;
	if (nDiffX < 0)
	{
		nDiffX = -nDiffX;
	}

	nDiffY = pPoint1.Y - pPoint2.Y;
	if (nDiffY < 0)
	{
		nDiffY = -nDiffY;
	}

	if (nDiffX < nDiffY)
	{
		return nDiffX + 2 * nDiffY;
	}
	else
	{
		return nDiffY + 2 * nDiffX;
	}
}

//D2Common.0x6FDA7280
D2PathInfoStrc* __fastcall sub_6FDA7280(D2PathInfoStrc** ppPathInfo, D2PathPointStrc pPathPoint)
{
	for (D2PathInfoStrc* result = ppPathInfo[(dword_6FDD17E0[pPathPoint.X & 0x7F] + dword_6FDD19E0[pPathPoint.Y & 0x7F]) & 0x7F]; result; result = result->pNext)
	{
		if (pPathPoint.X == result->pStartCoord.X && pPathPoint.Y == result->pStartCoord.Y)
		{
			return result;
		}		
	}

	return NULL;
}

//D2Common.0x6FDA72D0
D2PathInfoStrc* __fastcall sub_6FDA72D0(D2PathInfoStrc** ppPathInfo, D2PathPointStrc pPathPoint)
{
	for (D2PathInfoStrc* result = ppPathInfo[((dword_6FDD17E0[pPathPoint.X & 0x7F] + dword_6FDD19E0[pPathPoint.Y & 0x7F]) & 0x7F) + 128]; result; result = result->pNext)
	{
		if (pPathPoint.X == result->pStartCoord.X && pPathPoint.Y == result->pStartCoord.Y)
		{
			return result;
		}
	}

	return NULL;
}

////D2Common.0x6FDA7320) --------------------------------------------------------
//signed int __fastcall sub_6FDA7320(int a1, int a2)
//{
//	int v2; // esi@1
//	int v3; // ebp@1
//	int v4; // eax@1
//	signed int result; // eax@1
//
//	v2 = *(_DWORD *)(a1 + 1024);
//	v3 = 0;
//	v4 = (dword_6FDD17E0[*(_BYTE *)a2 & 127] + dword_6FDD19E0[*(_BYTE *)(a2 + 2) & 127]) & 127;
//	*(_DWORD *)(a2 + 48) = *(_DWORD *)(a1 + 4 * v4);
//	*(_DWORD *)(a1 + 4 * v4) = a2;
//	result = *(_WORD *)(a2 + 4);
//	if (!v2)
//		goto LABEL_9;
//	do
//	{
//		if (*(_WORD *)(v2 + 4) >= result)
//			break;
//		v3 = v2;
//		v2 = *(_DWORD *)(v2 + 52);
//	}
//	while (v2);
//	if (v3)
//	{
//		*(_DWORD *)(v3 + 52) = a2;
//		*(_DWORD *)(a2 + 52) = v2;
//	}
//	else
//	{
//LABEL_9:
//		result = *(_DWORD *)(a1 + 1024);
//		*(_DWORD *)(a2 + 52) = result;
//		*(_DWORD *)(a1 + 1024) = a2;
//	}
//	return result;
//}

////D2Common.0x6FDA7390) --------------------------------------------------------
//int __thiscall sub_6FDA7390(int this, int a2)
//{
//	int result; // eax@1
//	int v3; // eax@2
//	signed int v4; // ebp@2
//	int v5; // esi@2
//	int v6; // ebx@2
//	int v7; // eax@3
//	signed __int16 v8; // di@5
//	__int16 v9; // dx@7
//	__int16 v10; // di@8
//
//	*(_DWORD *)(this + 4 * *(_DWORD *)(this + 13032) + 12232) = a2;
//	result = *(_DWORD *)(this + 13032) + 1;
//	for (*(_DWORD *)(this + 13032) = result; result; result = *(_DWORD *)(this + 13032))
//	{
//		v3 = *(_DWORD *)(this + 13032) - 1;
//		v4 = 0;
//		*(_DWORD *)(this + 13032) = v3;
//		v5 = *(_DWORD *)(this + 4 * v3 + 12232);
//		v6 = v5 + 16;
//		do
//		{
//			v7 = *(_DWORD *)v6;
//			if (!*(_DWORD *)v6)
//				break;
//			if (*(_WORD *)v5 != *(_WORD *)v7)
//			{
//				v8 = 3;
//				if (*(_WORD *)(v5 + 2) != *(_WORD *)(v7 + 2))
//					continue;
//			}
//			v8 = 2;
//			v9 = v8 + *(_WORD *)(v5 + 8);
//			if (v9 < *(_WORD *)(v7 + 8))
//			{
//				v10 = *(_WORD *)(v7 + 6);
//				*(_DWORD *)(v7 + 12) = v5;
//				*(_WORD *)(v7 + 8) = v9;
//				*(_WORD *)(v7 + 4) = v9 + v10;
//				*(_DWORD *)(this + 4 * (*(_DWORD *)(this + 13032))++ + 12232) = v7;
//			}
//			++v4;
//			v6 += 4;
//		}
//		while (v4 < 8);
//	}
//	return result;
//}

////D2Common.0x6FDA7450) --------------------------------------------------------
//int __thiscall sub_6FDA7450(int this)
//{
//	int v1; // eax@1
//	int result; // eax@2
//
//	v1 = *(_DWORD *)(this + 12228);
//	if (v1 == 200)
//	{
//		result = 0;
//	}
//	else
//	{
//		*(_DWORD *)(this + 12228) = v1 + 1;
//		memset((void*)(this + 56 * v1 + 1028), 0, 0x38u);
//		result = this + 56 * v1 + 1028;
//	}
//	return result;
//}

////D2Common.0x6FDA7490) --------------------------------------------------------
//signed int __userpurge sub_6FDA7490<eax>(int a1<edx>, int a2<edi>, int a3, int a4, int a5)
//{
//	int v5; // ebp@1
//	signed __int16 v6; // ax@2
//	int v7; // edi@4
//	int v8; // ecx@4
//	int i; // eax@4
//	int v10; // ecx@9
//	__int16 v11; // dx@30
//	signed int result; // eax@30
//	__int16 v13; // ax@48
//	int v14; // eax@48
//	int v15; // eax@49
//	signed int v16; // ebp@49
//	int v17; // esi@49
//	int v18; // ebx@49
//	int v19; // eax@50
//	signed __int16 v20; // di@52
//	__int16 v21; // cx@54
//	__int16 v22; // di@55
//	int v23; // eax@59
//	int v24; // esi@60
//	int v25; // ebx@62
//	int v26; // eax@64
//	int v27; // eax@67
//	int v28; // edi@69
//	int v29; // ecx@69
//	int v30; // eax@69
//	int v31; // [sp+18h] [bp+4h]@4
//
//	v5 = a3;
//	if (*(_WORD *)a3 == (_WORD)a4 || (v6 = 3, *(_WORD *)(a3 + 2) == HIWORD(a4)))
//		v6 = 2;
//	LOWORD(a2) = v6 + *(_WORD *)(a3 + 8);
//	v31 = a2;
//	v8 = (dword_6FDD19E0[HIWORD(a4) & 0x7F] + dword_6FDD17E0[a4 & 0x7F]) & 0x7F;
//	for (i = *(_DWORD *)(a1 + 4 * v8); i; i = *(_DWORD *)(i + 48))
//	{
//		if ((_WORD)a4 == *(_WORD *)i)
//		{
//			if (HIWORD(a4) == *(_WORD *)(i + 2))
//			{
//				if (*(_DWORD *)(v5 + 16))
//				{
//					if (*(_DWORD *)(v5 + 20))
//					{
//						if (*(_DWORD *)(v5 + 24))
//						{
//							if (*(_DWORD *)(v5 + 28))
//							{
//								if (*(_DWORD *)(v5 + 32))
//								{
//									if (*(_DWORD *)(v5 + 36))
//									{
//										if (*(_DWORD *)(v5 + 40))
//										{
//											if (!*(_DWORD *)(v5 + 44))
//												*(_DWORD *)(v5 + 44) = i;
//										}
//										else
//										{
//											*(_DWORD *)(v5 + 40) = i;
//										}
//									}
//									else
//									{
//										*(_DWORD *)(v5 + 36) = i;
//									}
//								}
//								else
//								{
//									*(_DWORD *)(v5 + 32) = i;
//								}
//							}
//							else
//							{
//								*(_DWORD *)(v5 + 28) = i;
//							}
//						}
//						else
//						{
//							*(_DWORD *)(v5 + 24) = i;
//						}
//					}
//					else
//					{
//						*(_DWORD *)(v5 + 20) = i;
//					}
//				}
//				else
//				{
//					*(_DWORD *)(v5 + 16) = i;
//				}
//				if ((signed __int16)v31 < *(_WORD *)(i + 8))
//				{
//					v11 = v31 + *(_WORD *)(i + 6);
//					*(_DWORD *)(i + 12) = v5;
//					*(_WORD *)(i + 8) = v31;
//					*(_WORD *)(i + 4) = v11;
//					return 1;
//				}
//				return 1;
//			}
//			LOWORD(v7) = v31;
//		}
//	}
//	v10 = *(_DWORD *)(a1 + 4 * v8 + 512);
//	if (v10)
//	{
//		while (a4 != __PAIR__(*(_WORD *)(v10 + 2), *(_WORD *)v10))
//		{
//			v10 = *(_DWORD *)(v10 + 48);
//			if (!v10)
//				goto LABEL_59;
//		}
//		if (*(_DWORD *)(v5 + 16))
//		{
//			if (*(_DWORD *)(v5 + 20))
//			{
//				if (*(_DWORD *)(v5 + 24))
//				{
//					if (*(_DWORD *)(v5 + 28))
//					{
//						if (*(_DWORD *)(v5 + 32))
//						{
//							if (*(_DWORD *)(v5 + 36))
//							{
//								if (*(_DWORD *)(v5 + 40))
//								{
//									if (!*(_DWORD *)(v5 + 44))
//										*(_DWORD *)(v5 + 44) = v10;
//								}
//								else
//								{
//									*(_DWORD *)(v5 + 40) = v10;
//								}
//							}
//							else
//							{
//								*(_DWORD *)(v5 + 36) = v10;
//							}
//						}
//						else
//						{
//							*(_DWORD *)(v5 + 32) = v10;
//						}
//					}
//					else
//					{
//						*(_DWORD *)(v5 + 28) = v10;
//					}
//				}
//				else
//				{
//					*(_DWORD *)(v5 + 24) = v10;
//				}
//			}
//			else
//			{
//				*(_DWORD *)(v5 + 20) = v10;
//			}
//		}
//		else
//		{
//			*(_DWORD *)(v5 + 16) = v10;
//		}
//		if ((signed __int16)v7 >= *(_WORD *)(v10 + 8)
//			|| (v13 = *(_WORD *)(v10 + 6), //				*(_DWORD *)(v10 + 12) = v5, //				*(_WORD *)(v10 + 8) = v7, //				*(_WORD *)(v10 + 4) = v7 + v13, //				*(_DWORD *)(a1 + 4 * *(_DWORD *)(a1 + 13032) + 12232) = v10, //				v14 = *(_DWORD *)(a1 + 13032) + 1, //				(*(_DWORD *)(a1 + 13032) = v14) == 0))
//			return 1;
//		do
//		{
//			v15 = *(_DWORD *)(a1 + 13032) - 1;
//			v16 = 0;
//			*(_DWORD *)(a1 + 13032) = v15;
//			v17 = *(_DWORD *)(a1 + 4 * v15 + 12232);
//			v18 = v17 + 16;
//			do
//			{
//				v19 = *(_DWORD *)v18;
//				if (!*(_DWORD *)v18)
//					break;
//				if (*(_WORD *)v17 != *(_WORD *)v19)
//				{
//					v20 = 3;
//					if (*(_WORD *)(v17 + 2) != *(_WORD *)(v19 + 2))
//						continue;
//				}
//				v20 = 2;
//				v21 = v20 + *(_WORD *)(v17 + 8);
//				if (v21 < *(_WORD *)(v19 + 8))
//				{
//					v22 = *(_WORD *)(v19 + 6);
//					*(_DWORD *)(v19 + 12) = v17;
//					*(_WORD *)(v19 + 8) = v21;
//					*(_WORD *)(v19 + 4) = v21 + v22;
//					*(_DWORD *)(a1 + 4 * (*(_DWORD *)(a1 + 13032))++ + 12232) = v19;
//				}
//				++v16;
//				v18 += 4;
//			}
//			while (v16 < 8);
//		}
//		while (*(_DWORD *)(a1 + 13032));
//		result = 1;
//	}
//	else
//	{
//LABEL_59:
//		v23 = *(_DWORD *)(a1 + 12228);
//		if (v23 != 200
//			&& (*(_DWORD *)(a1 + 12228) = v23 + 1, //				v24 = a1 + 56 * v23 + 1028, //				memset((void*)v24, 0, 0x38u), //				a1 + 56 * v23 != -1028))
//		{
//			*(_DWORD *)(a1 + 56 * v23 + 1040) = v5;
//			v25 = (unsigned __int16)a4 - (unsigned __int16)a5;
//			*(_WORD *)(a1 + 56 * v23 + 1036) = v31;
//			if (v25 < 0)
//				v25 = -v25;
//			v26 = HIWORD(a4) - HIWORD(a5);
//			if (HIWORD(a4) - HIWORD(a5) < 0)
//				v26 = -v26;
//			if (v25 < v26)
//				v27 = v25 + 2 * v26;
//			else
//				v27 = v26 + 2 * v25;
//			*(_WORD *)(v24 + 6) = v27;
//			*(_WORD *)(v24 + 4) = v31 + v27;
//			*(_DWORD *)v24 = a4;
//			v28 = *(_DWORD *)(a1 + 1024);
//			v29 = 0;
//			v30 = (dword_6FDD17E0[*(_BYTE *)v24 & 0x7F] + dword_6FDD19E0[*(_BYTE *)(v24 + 2) & 0x7F]) & 0x7F;
//			*(_DWORD *)(v24 + 48) = *(_DWORD *)(a1 + 4 * v30);
//			*(_DWORD *)(a1 + 4 * v30) = v24;
//			if (!v28)
//				goto LABEL_94;
//			do
//			{
//				if (*(_WORD *)(v28 + 4) >= (signed int)*(_WORD *)(v24 + 4))
//					break;
//				v29 = v28;
//				v28 = *(_DWORD *)(v28 + 52);
//			}
//			while (v28);
//			if (v29)
//			{
//				*(_DWORD *)(v29 + 52) = v24;
//				*(_DWORD *)(v24 + 52) = v28;
//			}
//			else
//			{
//LABEL_94:
//				*(_DWORD *)(v24 + 52) = *(_DWORD *)(a1 + 1024);
//				*(_DWORD *)(a1 + 1024) = v24;
//			}
//			if (*(_DWORD *)(v5 + 16))
//			{
//				if (*(_DWORD *)(v5 + 20))
//				{
//					if (*(_DWORD *)(v5 + 24))
//					{
//						if (*(_DWORD *)(v5 + 28))
//						{
//							if (*(_DWORD *)(v5 + 32))
//							{
//								if (*(_DWORD *)(v5 + 36))
//								{
//									if (*(_DWORD *)(v5 + 40))
//									{
//										if (!*(_DWORD *)(v5 + 44))
//											*(_DWORD *)(v5 + 44) = v24;
//										return 1;
//									}
//									*(_DWORD *)(v5 + 40) = v24;
//									result = 1;
//								}
//								else
//								{
//									*(_DWORD *)(v5 + 36) = v24;
//									result = 1;
//								}
//							}
//							else
//							{
//								*(_DWORD *)(v5 + 32) = v24;
//								result = 1;
//							}
//						}
//						else
//						{
//							*(_DWORD *)(v5 + 28) = v24;
//							result = 1;
//						}
//					}
//					else
//					{
//						*(_DWORD *)(v5 + 24) = v24;
//						result = 1;
//					}
//				}
//				else
//				{
//					*(_DWORD *)(v5 + 20) = v24;
//					result = 1;
//				}
//			}
//			else
//			{
//				*(_DWORD *)(v5 + 16) = v24;
//				result = 1;
//			}
//		}
//		else
//		{
//			result = 0;
//		}
//	}
//	return result;
//}

//D2Common.0x6FDA78A0
// Takes a path list, and builds the path using straight lines into the path info.
// Note that the path list stores the path from the end to the beginning (in reverse order).
// It also seems like it won't copy the last  point of the path list (first of the array), not sure if it's intended or not.
signed int __fastcall PATH_SimplifyToLines_6FDA78A0(D2PathPointsListStrc* pCurPoint, D2PathInfoStrc* pPathInfo)
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
	while (pCurPoint && pCurPoint->pNextEntry && nbPoints < D2DynamicPathStrc::MAXPATHLEN)
	{
		D2PathPointsListStrc* pNextPoint = pCurPoint->pNextEntry;
		const int deltaX = pCurPoint->point.X - pNextPoint->point.X;
		const int deltaY = pCurPoint->point.Y - pNextPoint->point.Y;
		// If the direction doesn't change, then ignore the point
		if (deltaX != prevDeltaX || deltaY != prevDeltaY)
		{
			++nbPoints;
			// Store path in reverse order
			tempPathPoints[D2DynamicPathStrc::MAXPATHLEN  - nbPoints] = pCurPoint->point;
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
int __fastcall sub_6FDA7970(D2PathInfoStrc* pPathInfo)
{
	D2_ASSERTM(false, "Not implemented yet.");
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
