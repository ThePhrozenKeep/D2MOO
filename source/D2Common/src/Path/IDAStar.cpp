#include "Path/Path.h"
#include "Path/IDAStar.h"

#include "D2Collision.h"
#include "D2Dungeon.h"
#include <cmath>

//1.10f: Inlined
//1.13c: D2Common.0x6FDC0BB0
int __fastcall PATH_IdaStar_ComputePathWithRooms(D2DrlgCoordsStrc* pRoomCoords, D2PathInfoStrc* pInfo)
{
	UNIMPLEMENTED();
}

//1.10f: D2Common.0x6FDA7970
//1.13c: D2Common.0x6FDC0E40
int __fastcall PATH_IdaStar_6FDA7970(D2PathInfoStrc* pPathInfo)
{
	pPathInfo->field_14 *= 2;
	D2DrlgCoordsStrc tStartRoomCoords;
	DUNGEON_GetRoomCoordinates(pPathInfo->pStartRoom, &tStartRoomCoords);
	D2DrlgCoordsStrc tPathRoomsAABB = tStartRoomCoords;

	int nAABBHeight = 0;
	int nAABBWidth = 0;

	if (!pPathInfo->pTargetRoom || pPathInfo->pStartRoom == pPathInfo->pTargetRoom)
	{
		nAABBHeight = tPathRoomsAABB.nSubtileHeight;
		nAABBWidth = tPathRoomsAABB.nSubtileWidth;
	}
	else
	{
		D2DrlgCoordsStrc tTargetRoomCoords;
		DUNGEON_GetRoomCoordinates(pPathInfo->pTargetRoom, &tTargetRoomCoords);
		if (tTargetRoomCoords.nSubtileX >= tStartRoomCoords.nSubtileX)
		{
			tPathRoomsAABB.nSubtileX = tStartRoomCoords.nSubtileX;
			nAABBWidth = tTargetRoomCoords.nSubtileX + tTargetRoomCoords.nSubtileWidth - tStartRoomCoords.nSubtileX;
		}
		else
		{
			tPathRoomsAABB.nSubtileX = tTargetRoomCoords.nSubtileX;
			nAABBWidth = tStartRoomCoords.nSubtileX + tStartRoomCoords.nSubtileWidth - tTargetRoomCoords.nSubtileX;
		}
		tPathRoomsAABB.nSubtileWidth = nAABBWidth;
		if (tTargetRoomCoords.nSubtileY >= tStartRoomCoords.nSubtileY)
		{
			nAABBHeight = tTargetRoomCoords.nSubtileY + tTargetRoomCoords.nSubtileHeight - tStartRoomCoords.nSubtileY;
			tPathRoomsAABB.nSubtileY = tStartRoomCoords.nSubtileY;
		}
		else
		{
			nAABBHeight = tStartRoomCoords.nSubtileY + tStartRoomCoords.nSubtileHeight - tTargetRoomCoords.nSubtileY;
			tPathRoomsAABB.nSubtileY = tTargetRoomCoords.nSubtileY;
		}
		tPathRoomsAABB.nSubtileHeight = nAABBHeight;
	}

	// allow pathing at a distance of +/-10 subtiles if AABB is the same size as the start room
	if (nAABBWidth == tStartRoomCoords.nSubtileWidth)
	{
		tPathRoomsAABB.nSubtileX -= 10;
		tPathRoomsAABB.nSubtileWidth = nAABBWidth + 20;
	}
	if (nAABBHeight == tStartRoomCoords.nSubtileHeight)
	{
		nAABBHeight += 20;
		tPathRoomsAABB.nSubtileY -= 10;
		tPathRoomsAABB.nSubtileHeight = nAABBHeight;
	}
	return PATH_IdaStar_ComputePathWithRooms(&tPathRoomsAABB, pPathInfo);
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
