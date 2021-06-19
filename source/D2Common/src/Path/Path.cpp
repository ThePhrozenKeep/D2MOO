#include "Path/Path.h"

#include "D2Collision.h"
#include "D2DataTbls.h"
#include "D2Dungeon.h"
#include "D2Inventory.h"
#include <D2BitManip.h>
#include "D2Monsters.h"
#include "Path/Path_IDAStar.h"
#include "Units/UnitRoom.h"
#include "Units/Units.h"
#include <DataTbls/MonsterIds.h>



static const int dword_6FDD1DE4[] =
{
	0, 1, 1, 2, 0, -1, 0, 0, 0, 0, 0, 0, 0, 1, -1, -1, 0, 0, 0, 0, 0, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, -1, -1, 0, 1, 1, 1, 1, -1, -1, -1, -1
};

static const int dword_6FDD1E98[] =
{
	0, 1, 1, 1, 1, -1, -1, -1, -1, 0, 1, 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, 0, 0, 0, 0, 0, 1, 1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 0
};

static const char byte_6FDD1F88[] =
{
	 0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,
	 1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,
	 1,  0,  0,  0,  4,  0,  0,  0,  4,  0,  0,  0,  4,  0,  0,  0,
	 4,  0,  0,  0,  4,  0,  0,  0,  4,  0,  0,  0,  4,  0,  0,  0,
	 4,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,
	 8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,
	 8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,
	 8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,
	-8, -1, -1, -1, -8, -1, -1, -1, -8, -1, -1, -1, -8, -1, -1, -1,
	-8, -1, -1, -1, -8, -1, -1, -1, -8, -1, -1, -1, -8, -1, -1, -1,
	-8, -1, -1, -1, -8, -1, -1, -1, -8, -1, -1, -1, -8, -1, -1, -1,
	-8, -1, -1, -1, -8, -1, -1, -1, -8, -1, -1, -1, -8, -1, -1, -1,
	-4, -1, -1, -1, -4, -1, -1, -1, -4, -1, -1, -1, -4, -1, -1, -1,
	-4, -1, -1, -1, -4, -1, -1, -1, -4, -1, -1, -1, -4, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

static const int dword_6FDD2088[] =
{
	0x21900,
	0x1900,
	0,
	0,
	0x60000,
	0,
	0,
	0x21900,
	0x1E600,
	0x1E800,
	0x60000,
	0x1E604,
	0,
	0x100,
	0x60000,
	0x800,
	0,
	0,
};

static const short word_6FDD20D0[] =
{
	 0,  0,
	 0,  0,
	 0,  0,
	 0,  0,
	 0,  0,
	 2,  0,
	-2, -1,
	 0,  0,
	 0,  0,
	 0,  0,
	 0,  0,
	 0,  0,
	-4, -1,
	 0,  0,
	 0,  0,
	 0,  0,
	 0,  0,
	 0,  0,
};


//TODO: Remove
int __stdcall D2Common_10142(D2DynamicPathStrc*, D2UnitStrc*, int)
{
	REMOVE_LATER_WriteToLogFile("D2Common_10142: Not yet implemented");
	return 0;
}
void __stdcall sub_6FDAC790(D2DynamicPathStrc*,int,int)
{
	REMOVE_LATER_WriteToLogFile("sub_6FDAC790: Not yet implemented");
}
void __stdcall D2Common_10233(D2DynamicPathStrc*)
{
	REMOVE_LATER_WriteToLogFile("D2Common_10233: Not yet implemented");
}
void __stdcall sub_6FDAC5E0(D2CoordStrc*, int*, int, int, int, int)
{
	REMOVE_LATER_WriteToLogFile("sub_6FDAC5E0: Not yet implemented");
}
unsigned short* word_6FDD211C;
unsigned short* word_6FDD2118;
int __fastcall sub_6FDAAD10(D2DynamicPathStrc*, D2PathPointStrc*, D2PathPointStrc)
{
	REMOVE_LATER_WriteToLogFile("sub_6FDAAD10: Not yet implemented");
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////


//D2Common.0x6FDA8220
void __fastcall sub_6FDA8220(D2DynamicPathStrc* pDynamicPath)
{
	pDynamicPath->dwFlags &= 0xFFFFFFFE;

	if (pDynamicPath->pRoom)
	{
		for (int i = 0; i < pDynamicPath->dwPathPoints; ++i)
		{
			if (pDynamicPath->PathPoints[i + 1].X < pDynamicPath->pRoom->nSubtileX)
			{
				pDynamicPath->dwFlags |= 0x00000001;
				break;
			}

			if (pDynamicPath->PathPoints[i + 1].X >= pDynamicPath->pRoom->nSubtileX + pDynamicPath->pRoom->nSubtileWidth)
			{
				pDynamicPath->dwFlags |= 0x00000001;
				break;
			}

			if (pDynamicPath->PathPoints[i + 1].Y < pDynamicPath->pRoom->nSubtileY)
			{
				pDynamicPath->dwFlags |= 0x00000001;
				break;
			}

			if (pDynamicPath->PathPoints[i + 1].Y >= pDynamicPath->pRoom->nSubtileY + pDynamicPath->pRoom->nSubtileHeight)
			{
				pDynamicPath->dwFlags |= 0x00000001;
				break;
			}
		}
	}
}

//D2Common.0x6FDA82A0 (#10141)
//TODO: Find a name
void __stdcall D2Common_10141(D2UnitStrc* pUnit, int* pX, int* pY)
{
	if (pUnit && UNITS_IsInMovingMode(pUnit) && pUnit->pDynamicPath->dwPathPoints)
	{
		*pX = pUnit->pDynamicPath->unk0x72;
		*pY = pUnit->pDynamicPath->unk0x76;
		*pX >>= 11;
		*pY >>= 11;
		DUNGEON_FlattenCoords_IsoToCartesian(pX, pY);
	}
	else
	{
		*pX = 0;
		*pY = 0;
	}
}

//D2Common.0x6FDA8320 (#10222)
//TODO: Find a name
void __stdcall D2Common_10222(D2UnitStrc* pUnit)
{
	D2CoordStrc pCoords = {};

	UNITS_GetCoords(pUnit, &pCoords);

	if (!pUnit || pUnit->dwUnitType < 0)
	{
		COLLISION_SetMaskWithSize(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), UNITS_GetCollisionType(pUnit));
	}
	else
	{
		if (pUnit->dwUnitType > 1)
		{
			if (pUnit->dwUnitType == UNIT_OBJECT)
			{
				COLLISION_SetMaskWithSizeXY(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), UNITS_GetUnitSizeY(pUnit), UNITS_GetCollisionType(pUnit));
			}
			else
			{
				COLLISION_SetMaskWithSize(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), UNITS_GetCollisionType(pUnit));
			}
		}
		else
		{
			COLLISION_SetMaskWithPattern(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, pUnit->pDynamicPath->dwCollisionPattern, UNITS_GetCollisionType(pUnit));
		}
	}
}

//D2Common.0x6FDA8450 (#10223)
//TODO: Find a name
BOOL __stdcall D2Common_10223(D2UnitStrc* pUnit, int a2)
{
	int nUnitType = 0;
	D2CoordStrc pCoords = {};

	if (pUnit)
	{
		nUnitType = pUnit->dwUnitType;
	}
	else
	{
		nUnitType = 6;
	}

	UNITS_GetCoords(pUnit, &pCoords);

	if (nUnitType == UNIT_PLAYER)
	{
		if (pUnit->dwAnimMode != PLRMODE_DEAD && pUnit->dwAnimMode != PLRMODE_DEATH)
		{
			COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, pUnit->pDynamicPath->dwCollisionPattern, UNITS_GetCollisionType(pUnit));
			return TRUE;
		}
	}
	else if (nUnitType == UNIT_MONSTER)
	{
		if (pUnit->dwAnimMode != MONMODE_DEAD && pUnit->dwAnimMode != MONMODE_DEATH)
		{
			COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, pUnit->pDynamicPath->dwCollisionPattern, UNITS_GetCollisionType(pUnit));
			return TRUE;
		}
	}
	else if (nUnitType == UNIT_OBJECT)
	{
		if (UNITS_HasCollision(pUnit) || a2)
		{
			COLLISION_ResetMaskWithSizeXY(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), UNITS_GetUnitSizeY(pUnit), UNITS_GetCollisionType(pUnit));
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		COLLISION_ResetMaskWithSize(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, UNITS_GetUnitSizeX(pUnit), UNITS_GetCollisionType(pUnit));
		return TRUE;
	}

	if (!a2)
	{
		return FALSE;
	}

	COLLISION_ResetMaskWithPattern(UNITS_GetRoom(pUnit), pCoords.nX, pCoords.nY, pUnit->pDynamicPath->dwCollisionPattern, UNITS_GetCollisionType(pUnit));
	return TRUE;
}

////D2Common.0x6FDA8600) --------------------------------------------------------
//int __stdcall D2Common_10142(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit, int a3)
//{
//	D2DynamicPathStrc*v3; // esi@1
//	int v4; // ebp@1
//	uint32_t v5; // eax@6
//	uint32_t v6; // eax@10
//	uint32_t result; // eax@10
//	uint32_t v8; // eax@11
//	uint32_t v9; // eax@12
//	uint32_t v10; // eax@14
//	D2UnitStrc*v11; // eax@15
//	signed int v12; // edx@16
//	D2DynamicPathStrc*v13; // eax@19
//	uint32_t v14; // ecx@20
//	int v15; // ecx@27
//	uint16_t v16; // cx@30
//	uint32_t v17; // ecx@32
//	uint32_t v18; // edx@32
//	int v19; // eax@32
//	D2UnitStrc*v20; // edi@32
//	uint32_t v21; // eax@33
//	uint32_t v22; // eax@37
//	uint32_t v23; // eax@40
//	int v24; // eax@44
//	bool v25; // zf@47
//	char v26; // al@49
//	int v27; // edi@59
//	int v28; // edi@60
//	int v29; // ebx@60
//	int v30; // eax@60
//	int v31; // ecx@60
//	D2RoomStrc*v32; // eax@64
//	signed int v33; // ecx@67
//	unsigned int v34; // edx@69
//	D2RoomStrc*v35; // eax@70
//	int v36; // ecx@73
//	D2UnitStrc*v37; // ebx@81
//	D2RoomStrc*v38; // eax@84
//	int v39; // edi@85
//	signed __int32 v40; // ebx@85
//	D2RoomStrc*v41; // eax@85
//	D2UnitStrc*v42; // edi@89
//	uint32_t v43; // eax@91
//	uint32_t v44; // eax@93
//	signed int v45; // edi@93
//	D2RoomStrc*v46; // edi@97
//	int v47; // ebp@97
//	int v48; // ebx@97
//	D2RoomStrc***v49; // eax@97
//	int v50; // eax@100
//	__int16 v51; // dx@103
//	__int16 v52; // cx@104
//	__int16 v53; // dx@106
//	__int16 v54; // ax@108
//	__int16 v55; // cx@108
//	uint16_t v56; // ax@117
//	uint16_t v57; // ax@128
//	uint32_t v58; // ecx@128
//	uint32_t v59; // eax@130
//	uint32_t v60; // eax@137
//	D2RoomStrc** pppRoom; // [sp+10h] [bp-68h]@69
//	int v62; // [sp+14h] [bp-64h]@30
//	int v63; // [sp+18h] [bp-60h]@89
//	int v64; // [sp+1Ch] [bp-5Ch]@100
//	int v65; // [sp+20h] [bp-58h]@104
//	int v66; // [sp+24h] [bp-54h]@105
//	int v67; // [sp+28h] [bp-50h]@107
//	int v68; // [sp+34h] [bp-44h]@100
//	int v69; // [sp+38h] [bp-40h]@100
//	int v70; // [sp+3Ch] [bp-3Ch]@100
//	int v71; // [sp+44h] [bp-34h]@30
//	int v72; // [sp+48h] [bp-30h]@59
//	D2RoomStrc*v73; // [sp+4Ch] [bp-2Ch]@64
//	D2RoomStrc* pRoom; // [sp+50h] [bp-28h]@77
//	int v75; // [sp+58h] [bp-20h]@57
//	int v76; // [sp+5Ch] [bp-1Ch]@32
//	int v77; // [sp+60h] [bp-18h]@32
//	uint32_t v78; // [sp+64h] [bp-14h]@89
//	uint32_t v79; // [sp+68h] [bp-10h]@32
//	int v80; // [sp+6Ch] [bp-Ch]@32
//	int v81; // [sp+70h] [bp-8h]@32
//	D2DynamicPathStrc*v82; // [sp+74h] [bp-4h]@32
//
//	v3 = pDynamicPath;
//	v4 = 0;
//	if (!pDynamicPath)
//		return 0;
//	if (pDynamicPath->pUnit != pUnit)
//	{
//		FOG_Assertion("ptPath->hUnit == hUnit", __FILE__, __LINE__);
//		exit(-1);
//	}
//	if (!(pDynamicPath->dwFlags & 0x40000))
//	{
//		v11 = pDynamicPath->pTargetUnit;
//		pDynamicPath->unk0x54 = 0;
//		if (!v11)
//		{
//LABEL_30:
//			v16 = v3->wPosY;
//			LOWORD(v62) = v3->wPosX;
//			HIWORD(v62) = v16;
//			v71 = v62;
//			if (!(_WORD)v62 && !v16)
//				goto LABEL_136;
//			v17 = v3->dwCollisionPattern;
//			v18 = v3->unk0x50;
//			v79 = v3->dwUnitSize;
//			v80 = v17;
//			v19 = v3->nDistMax;
//			LOBYTE(v17) = v3->unk0x92;
//			v82 = v3;
//			v81 = v18;
//			v76 = v19;
//			LOBYTE(v77) = v17;
//			v20 = v3->pTargetUnit;
//			if (v20)
//			{
//				v21 = v20->dwUnitType;
//				if (v20->dwUnitType == 2 || (signed int)v21 > 3 && (signed int)v21 <= 5)
//				{
//					v22 = v20->pDynamicPath->dwTargetY;
//				}
//				else
//				{
//					if (v20->pDynamicPath)
//						LOWORD(v22) = PATH_GetXPosition(v20->pDynamicPath);
//					else
//						LOWORD(v22) = 0;
//				}
//				v3->xSP1 = v22;
//				v23 = v20->dwUnitType;
//				if (v20->dwUnitType == 2 || (signed int)v23 > 3 && (signed int)v23 <= 5)
//				{
//					v24 = *(_DWORD *)&v20->pDynamicPath->xSP1;
//				}
//				else
//				{
//					if (v20->pDynamicPath)
//						LOWORD(v24) = PATH_GetYPosition(v20->pDynamicPath);
//					else
//						LOWORD(v24) = 0;
//				}
//				v25 = v3->xSP1 == 0;
//				v3->ySP1 = v24;
//				if (v25 && !(_WORD)v24)
//				{
//					v26 = 0;
//					goto LABEL_57;
//				}
//				switch (v20->dwUnitType)
//				{
//				case 4u:
//					goto LABEL_51;
//				case 2u:
//					if (UNITS_IsDoor(v20))
//						v26 = sub_6FDA90C0((int)&v71);
//					else
//						LABEL_51 :
//						v26 = 2;
//					goto LABEL_57;
//				case 0u:
//				case 1u:
//					if (v3->unk0x68[0])
//						sub_6FDAB890((int)v3);
//					break;
//				default:
//					break;
//				}
//			}
//			v26 = 1;
//LABEL_57:
//			v25 = v3->xSP1 == 0;
//			LOBYTE(v75) = v26;
//			if (v25 && !v3->ySP1
//				|| (v27 = *(_DWORD *)&v3->xSP1, v72 = *(_DWORD *)&v3->xSP1, v71 == v72)
//				|| (v28 = (unsigned __int16)v27, v29 = HIWORD(v72), v30 = v28 - (unsigned __int16)v71, v31 = HIWORD(v72) - HIWORD(v71), v30 > 100)
//				|| -v30 > 100
//				|| v31 > 100
//				|| -v31 > 100)
//				goto LABEL_136;
//			v73 = v3->pRoom;
//			v32 = v73;
//			if (v73)
//			{
//				if (v28 >= v73->nSubtileX)
//				{
//					if (v28 < v73->nSubtileX + v73->nSubtileWidth)
//					{
//						v33 = v73->nSubtileY;
//						if (HIWORD(v72) >= v33)
//						{
//							if (HIWORD(v72) < v33 + v73->nSubtileHeight)
//								goto LABEL_77;
//						}
//					}
//				}
//				pppRoom = 0;
//				pDynamicPath = 0;
//				DUNGEON_GetAdjacentRoomsListFromRoom(v73, &pppRoom, (int*)&pDynamicPath);
//				v34 = 0;
//				if ((unsigned int)pDynamicPath > 0)
//				{
//					while (1)
//					{
//						v35 = pppRoom[v34];
//						if (v35 != (D2RoomStrc*)v4)
//						{
//							if (v28 >= v35->nSubtileX)
//							{
//								if (v28 < v35->nSubtileX + v35->nSubtileWidth)
//								{
//									v36 = v35->nSubtileY;
//									if (v29 >= v36)
//									{
//										if (v29 < v36 + v35->nSubtileHeight)
//											break;
//									}
//								}
//							}
//						}
//						++v34;
//						v4 = 0;
//						if (v34 >= (unsigned int)pDynamicPath)
//							goto LABEL_76;
//					}
//					v32 = pppRoom[v34];
//					goto LABEL_77;
//				}
//			}
//LABEL_76:
//			v32 = 0;
//LABEL_77:
//			pRoom = v32;
//			v4 = 0;
//			if (!v73)
//			{
//				FOG_Assertion("tPathInfo.hRoomStart", __FILE__, __LINE__);
//				exit(-1);
//			}
//			v37 = pUnit;
//			if (pUnit && pUnit->dwUnitType == 1 && !MONSTERS_CanBeInTown(pUnit))
//			{
//				v38 = pRoom;
//				if (!pRoom)
//				{
//					v39 = HIWORD(v72);
//					v40 = (unsigned __int16)v72;
//					v41 = COLLISION_GetRoomBySubTileCoordinates(v73, (unsigned __int16)v71 + ((unsigned __int16)v72 - (unsigned __int16)v71) / 2, HIWORD(v71) + (HIWORD(v72) - HIWORD(v71)) / 2);
//					v38 = COLLISION_GetRoomBySubTileCoordinates(v41, v40, v39);
//				}
//				v4 = 0;
//				if (DUNGEON_IsRoomInTown(v38) && !a3)
//					goto LABEL_136;
//				v37 = pUnit;
//			}
//			v42 = v3->pTargetUnit;
//			v78 = v3->dwPathType;
//			v63 = v4;
//			D2Common_10223(v37, v4);
//			if (v42 != (D2UnitStrc*)v4)
//			{
//				if (UNITS_GetUnitSizeX(v42))
//				{
//					v43 = v3->dwFlags;
//					if (BYTE1(v43) & 8)
//						v63 = D2Common_10223(v42, v4);
//				}
//			}
//			v44 = v3->dwFlags;
//			v45 = 1;
//			if (BYTE1(v44) & 0x10
//				&& COLLISION_CheckMaskWithPattern2(v73, (unsigned __int16)v72, HIWORD(v72), v3->dwCollisionPattern, v3->unk0x50))
//			{
//				if (v82->dwFlags & 0x40000)
//					FOG_10025("!(ptPath->dwFlags & PATH_MISSILE_MASK)", __FILE__, __LINE__);
//				v46 = v73;
//				v47 = v80;
//				v48 = v81;
//				a3 = *(int*)((char*)&v71 + 2);
//				v49 = (D2RoomStrc***)v82->pUnit;
//				pDynamicPath = (D2DynamicPathStrc*)v71;
//				if (v49)
//					pppRoom = *v49;
//				else
//					pppRoom = (D2RoomStrc**)6;
//				v68 = v72;
//				v69 = v72;
//				v70 = v72;
//				sub_6FDAB7D0(&v64, v72, v71);
//				v50 = v64;
//				while (1)
//				{
//					if ((_WORD)pDynamicPath == (_WORD)v68 && (_WORD)a3 == HIWORD(v68))
//						goto LABEL_113;
//					v51 = word_6FDD1DA0[4 * v50];
//					HIWORD(v68) += word_6FDD1DA4[4 * v50];
//					LOWORD(v68) = v51 + v68;
//					if (!COLLISION_CheckMaskWithPattern2(v46, (unsigned __int16)v68, HIWORD(v68), v47, v48))
//					{
//						v55 = HIWORD(v68);
//						v54 = v68;
//						goto LABEL_111;
//					}
//					v52 = word_6FDD1DA0[4 * v65];
//					HIWORD(v69) += word_6FDD1DA4[4 * v65];
//					LOWORD(v69) = v52 + v69;
//					if (!COLLISION_CheckMaskWithPattern2(v46, (unsigned __int16)v69, HIWORD(v69), v47, v48))
//					{
//						v55 = HIWORD(v69);
//						v54 = v69;
//						goto LABEL_111;
//					}
//					if (v66 != 255)
//					{
//						v53 = word_6FDD1DA0[4 * v66];
//						HIWORD(v70) += word_6FDD1DA4[4 * v66];
//						LOWORD(v70) = v53 + v70;
//						if (!COLLISION_CheckMaskWithPattern2(v46, (unsigned __int16)v70, HIWORD(v70), v47, v48))
//							break;
//					}
//					sub_6FDAB7D0(&v67, v68, v71);
//					v50 = v67;
//					v64 = v67;
//				}
//				v55 = HIWORD(v70);
//				v54 = v70;
//LABEL_111:
//				LOWORD(v72) = v54;
//				HIWORD(v72) = v55;
//				if (v54 == (_WORD)pDynamicPath && v55 == (_WORD)a3)
//				{
//LABEL_113:
//					v45 = 0;
//					goto LABEL_117;
//				}
//				if (!pppRoom)
//					sub_6FDA8FE0((int)&v71);
//				v45 = 1;
//			}
//LABEL_117:
//			v56 = HIWORD(v72);
//			v4 = 0;
//			v3->xSP3 = v72;
//			v3->unk0x24 = 0;
//			v3->ySP3 = v56;
//			v3->dwPathPoints = 0;
//			v3->unk0x24 = 0;
//			if (v45)
//			{
//				if (IsBadCodePtr(*(&lpfn + v78)))
//				{
//					FOG_Assertion("scpfnPathFunction[tPathInfo.ePathType]", __FILE__, __LINE__);
//					exit(-1);
//				}
//				v3->dwPathPoints = ((signed int(__thiscall *)(int))*(&lpfn + v78))((int)&v71);
//			}
//			else
//			{
//				v3->dwPathPoints = 0;
//			}
//			if (v71 != v62)
//			{
//				FOG_Assertion("COORD_TEST_EQUAL(tPathInfo.tStartCoord, tStart)", __FILE__, __LINE__);
//				exit(-1);
//			}
//			if (v63)
//				D2Common_10222(v3->pTargetUnit);
//			D2Common_10222(pUnit);
//			if (!v3->dwPathPoints)
//				goto LABEL_137;
//			sub_6FDAC790(v3, 1, 1);
//			if ((signed __int32)v3->unk0x24 < (signed __int32)v3->dwPathPoints)
//			{
//				sub_6FDA8220((int)v3);
//				v57 = v3->xSP1;
//				v3->ySP2 = v3->ySP1;
//				v58 = v3->dwFlags;
//				v3->unk0x38 = 0;
//				v3->xSP2 = v57;
//				if (!(v58 & 0x10) && !v3->pTargetUnit)
//				{
//					v59 = v3->dwPathPoints;
//					v3->xSP1 = *((_WORD *)&v3->dwPathOffset + 2 * v59);
//					v3->ySP1 = *((_WORD *)&v3->dwPathOffset + 2 * v59 + 1);
//				}
//				result = v3->dwPathPoints;
//				if (result)
//					v3->dwFlags = v58 | 0x20;
//				else
//					v3->dwFlags = v58 & 0xFFFFFFDF;
//				return result;
//			}
//LABEL_136:
//			v3->unk0x24 = v4;
//			v3->dwPathPoints = v4;
//LABEL_137:
//			v60 = v3->dwFlags;
//			LOBYTE(v60) = v60 & 0xDF;
//			v3->dwFlags = v60;
//			return 0;
//		}
//		v12 = v11->dwUnitType;
//		if (v11->dwUnitType == 2 || v12 > 3 && v12 <= 5)
//		{
//			v13 = v11->pDynamicPath;
//			v14 = v13->dwTargetY;
//		}
//		else
//		{
//			v13 = v11->pDynamicPath;
//			if (!v13)
//				return 0;
//			v14 = v13->wPosX;
//		}
//		if (v14)
//		{
//			if (v12 == 2 || v12 > 3 && v12 <= 5)
//			{
//				v15 = *(_DWORD *)&v13->xSP1;
//			}
//			else
//			{
//				if (!v13)
//					return 0;
//				v15 = v13->wPosY;
//			}
//			if (v15)
//				goto LABEL_30;
//		}
//		return 0;
//	}
//	v5 = pDynamicPath->dwPathType;
//	if (v5 == 4)
//	{
//		if (!sub_6FDA8E30(pDynamicPath, (int)pUnit))
//			goto LABEL_10;
//		v10 = v3->dwFlags;
//		LOBYTE(v10) = v10 | 0x20;
//		v3->dwFlags = v10;
//		result = v3->dwPathPoints;
//	}
//	else
//	{
//		if (v5 == 10)
//		{
//			sub_6FDAB4A0((int)pDynamicPath, (int)&pUnit->pSeed);
//			v9 = v3->dwFlags;
//			LOBYTE(v9) = v9 | 0x20;
//			v3->dwFlags = v9;
//			result = v3->dwPathPoints;
//		}
//		else
//		{
//			if (v5 != 14)
//			{
//				FOG_10025("Not a missile path!", __FILE__, __LINE__);
//LABEL_10:
//				v6 = v3->dwFlags;
//				LOBYTE(v6) = v6 & 0xDF;
//				v3->dwFlags = v6;
//				return 0;
//			}
//			sub_6FDAB3C0((int)pDynamicPath);
//			v8 = v3->dwFlags;
//			LOBYTE(v8) = v8 | 0x20;
//			v3->dwFlags = v8;
//			result = v3->dwPathPoints;
//		}
//	}
//	return result;
//}

//D2Common.0x6FDA8E30
//TODO: Find a name
int __fastcall sub_6FDA8E30(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit)
{
	D2RoomStrc* pRoom = NULL;
	int nXDistance = 0;
	int nYDistance = 0;
	int nOldX = 0;
	int nOldY = 0;
	char szText[256] = {};
	D2CoordStrc pCoords = {};

	nOldX = pDynamicPath->wPosX;
	nOldY = pDynamicPath->wPosY;

	pDynamicPath->unk0x24 = 0;

	if (pDynamicPath->pTargetUnit)
	{
		UNITS_GetCoords(pDynamicPath->pTargetUnit, &pCoords);

		pDynamicPath->xSP1 = pCoords.nX;
		pDynamicPath->ySP1 = pCoords.nY;
	}

	nXDistance = nOldX - pDynamicPath->xSP1;
	if (nXDistance < 0)
	{
		nXDistance = -nXDistance;
	}

	D2_ASSERTM(nXDistance < 100, FOG_10018(szText, "Missile Firing too far : %d", pUnit ? pUnit->dwClassId : -1));

	nYDistance = nOldY - pDynamicPath->ySP1;
	if (nYDistance < 0)
	{
		nYDistance = -nYDistance;
	}

	D2_ASSERTM(nYDistance < 100, FOG_10018(szText, "Missile Firing too far : %d", pUnit ? pUnit->dwClassId : -1));

	if (pDynamicPath->xSP1 && pDynamicPath->ySP1)
	{
		pDynamicPath->PathPoints[1].X = pDynamicPath->xSP1;
		pDynamicPath->PathPoints[1].Y = pDynamicPath->ySP1;

		pDynamicPath->dwPathPoints = 1;
		pDynamicPath->unk0x38 = 0;
		sub_6FDAC790(pDynamicPath, 1, 1);

		pRoom = pDynamicPath->pRoom;

		if (!pRoom || pDynamicPath->xSP1 < pRoom->nSubtileX || pDynamicPath->xSP1 >= pRoom->nSubtileX + pRoom->nSubtileWidth || pDynamicPath->ySP1 < pRoom->nSubtileY || pDynamicPath->ySP1 >= pRoom->nSubtileY + pRoom->nSubtileHeight)
		{
			pDynamicPath->dwFlags |= 1;
		}

		return pDynamicPath->dwPathPoints;
	}

	return 0;
}

//D2Common.0x6FDA8FE0
//TODO: Find a name
void __fastcall sub_6FDA8FE0(D2PathInfoStrc* pPathInfo)
{
	int nOffsetX = 0;
	int nOffsetY = 0;
	int nDiffX = 0;
	int nDiffY = 0;
	int nX = 0;
	int nY = 0;

	nX = pPathInfo->field_4.X;
	nY = pPathInfo->field_4.Y;

	nDiffX = nX - pPathInfo->pStartCoord.X;
	if (nDiffX < 0)
	{
		nDiffX = -nDiffX;
	}

	nDiffY = nY - pPathInfo->pStartCoord.Y;
	if (nDiffY < 0)
	{
		nDiffY = -nDiffY;
	}

	if (nDiffX < 5 && nDiffY < 5 && (nDiffX || nDiffY))
	{
		nOffsetX = dword_6FDD1E98[nDiffX + 9 * nDiffY];
		nOffsetY = dword_6FDD1E98[nDiffY + 9 * nDiffX];

		if (nDiffX <= nDiffY)
		{
			nDiffX = nDiffY;
		}

		while (nDiffX < 5)
		{
			nX += nOffsetX;
			nY += nOffsetY;

			if (COLLISION_CheckMaskWithPattern2(pPathInfo->pRoom, nX, nY, pPathInfo->nCollisionPattern, pPathInfo->nCollisionType))
			{
				break;
			}

			++nDiffX;
			pPathInfo->field_4.X = nX;
			pPathInfo->field_4.Y = nY;
		}
	}
}

//D2Common.0x6FDA90C0
//TODO: Find a name
uint8_t __fastcall sub_6FDA90C0(D2PathInfoStrc* pPathInfo)
{
	D2UnitStrc* pTargetUnit = pPathInfo->pDynamicPath->pTargetUnit;

	D2_ASSERT(UNITS_IsDoor(pTargetUnit));

	if (!UNITS_CheckIfObjectOrientationIs1(pTargetUnit))
	{
		if (pPathInfo->pDynamicPath->wPosX >= UNITS_GetXPosition(pTargetUnit))
		{
			pPathInfo->pDynamicPath->xSP1 += 2;
		}
		else
		{
			pPathInfo->pDynamicPath->xSP1 -= 2;
		}
	}
	else
	{
		if (pPathInfo->pDynamicPath->wPosY >= UNITS_GetYPosition(pTargetUnit))
		{
			pPathInfo->pDynamicPath->ySP1 += 2;
		}
		else
		{
			pPathInfo->pDynamicPath->ySP1 -= 2;
		}
	}

	return 2;
}

//D2Common.0x6FDA9190 (#10156)
void __stdcall PATH_FreeDynamicPath(void* pMemPool, D2DynamicPathStrc* pDynamicPath)
{
	FOG_FreeServerMemory(pMemPool, pDynamicPath, __FILE__, __LINE__, 0);
}

//D2Common.0x6FDA91B0 (#11282)
//TODO: Find a name
int __stdcall D2Common_11282_Unused(int nMonsterId)
{
	D2MonStats2Txt* pMonStats2TxtRecord = NULL;
	int nCollisionPattern = 0;

	pMonStats2TxtRecord = UNITS_GetMonStats2TxtRecordFromMonsterId(nMonsterId);
	if (pMonStats2TxtRecord && pMonStats2TxtRecord->nSizeX >= 0 && pMonStats2TxtRecord->nSizeX < 4)
	{
		nCollisionPattern = dword_6FDD1DE4[pMonStats2TxtRecord->nSizeX];

		if (pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_REVIVE] || pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_SMALL])
		{
			if (!(pMonStats2TxtRecord->dwFlags & gdwBitMasks[MONSTATS2FLAGINDEX_ISATT]))
			{
				if (nCollisionPattern == 1)
				{
					nCollisionPattern = 3;
				}
				else if (nCollisionPattern == 2)
				{
					nCollisionPattern = 4;
				}
			}
		}

		return nCollisionPattern;
	}

	return 1;
}

//D2Common.0x6FDA9250 (#11281)
//TODO: Find a name
int __stdcall D2Common_11281_Unused(D2UnitStrc* pUnit, int nSize)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nCollisionPattern = 0;

	if (nSize >= 0 && nSize < 4)
	{
		nCollisionPattern = dword_6FDD1DE4[nSize];
		if (pUnit && pUnit->dwUnitType == UNIT_MONSTER && MONSTERS_CanBeInTown(pUnit))
		{
			pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(pUnit->dwClassId);
			if (!pMonStatsTxtRecord || !(pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_INTERACT]))
			{
				if (nCollisionPattern == 1)
				{
					return 3;
				}
				else if (nCollisionPattern == 2)
				{
					return 4;
				}
			}
		}

		return nCollisionPattern;
	}

	return 1;
}

//D2Common.0x6FDA92F0 (#10214)
//TODO: Find a name
void __stdcall D2Common_10214(D2UnitStrc* pUnit)
{
	if (pUnit->pDynamicPath)
	{
		if (UNITS_GetRoom(pUnit))
		{
			D2Common_10223(pUnit, 1);
			pUnit->pDynamicPath->dwCollisionPattern = D2Common_11281_Unused(pUnit, pUnit->pDynamicPath->dwUnitSize);
			D2Common_10222(pUnit);
		}
		else
		{
			pUnit->pDynamicPath->dwCollisionPattern = D2Common_11281_Unused(pUnit, pUnit->pDynamicPath->dwUnitSize);
		}
	}
}

//D2Common.0x6FDA9480 (#10152)
void __stdcall PATH_AllocDynamicPath(void* pMemPool, D2RoomStrc* pRoom, int nX, int nY, D2UnitStrc* pUnit, BOOL bSetFlag)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	D2DynamicPathStrc* pDynamicPath = NULL;
	int nClassId = 0;
	int nTargetX = 0;
	int nTargetY = 0;

	pDynamicPath = (D2DynamicPathStrc*)FOG_AllocServerMemory(pMemPool, sizeof(D2DynamicPathStrc), __FILE__, __LINE__, 0);
	memset(pDynamicPath, 0x00, sizeof(D2DynamicPathStrc));

	pUnit->pDynamicPath = pDynamicPath;

	pDynamicPath->pUnit = pUnit;
	pDynamicPath->dwUnitSize = UNITS_GetUnitSizeX(pUnit);

	pUnit->pDynamicPath->dwCollisionPattern = D2Common_11281_Unused(pUnit, pDynamicPath->dwUnitSize);

	pDynamicPath->dwVelocity = 2048;
	pDynamicPath->dwPrecisionX = (nX << 16) + 32768;
	pDynamicPath->pRoom = pRoom;
	pDynamicPath->nStepNum = 0;
	pDynamicPath->dwPrecisionY = (nY << 16) + 32768;
	pDynamicPath->unk0x1D4 = 1;
	pDynamicPath->unk0x1D8[0].X = nX;
	pDynamicPath->unk0x1D8[0].Y = nY;

	if (pUnit->dwUnitType == UNIT_PLAYER)
	{
		pDynamicPath->dwCollisionType = 128;
		pDynamicPath->unk0x50 = 0x1C09;
		PATH_SetType(pDynamicPath, 7);
		pDynamicPath->nDistMax = 73;
		pDynamicPath->unk0x92 = 70;
	}
	else if (pUnit->dwUnitType == UNIT_MONSTER)
	{
		pDynamicPath->dwCollisionType = 256;
		PATH_SetType(pDynamicPath, 2);

		nClassId = pUnit->dwClassId;

		pMonStatsTxtRecord = DATATBLS_GetMonStatsTxtRecord(nClassId);
		if (pMonStatsTxtRecord)
		{
			nClassId = pMonStatsTxtRecord->nBaseId;
		}

		if (nClassId == MONSTER_WRAITH1)
		{
			pUnit->pDynamicPath->dwCollisionPattern = 5;
			pDynamicPath->unk0x50 = 0x804;
			pDynamicPath->nDistMax = 14;
		}
		else
		{
			if (pMonStatsTxtRecord && pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_FLYING])
			{
				pDynamicPath->nDistMax = 14;
				pDynamicPath->unk0x50 = 0x1804;
			}
			else
			{
				pDynamicPath->nDistMax = 14;
				pDynamicPath->unk0x50 = (pMonStatsTxtRecord && (pMonStatsTxtRecord->dwMonStatsFlags & gdwBitMasks[MONSTATSFLAGINDEX_OPENDOORS]) != 0) ? 0x3401 : 0x3C01;
			}
		}
	}
	else if (pUnit->dwUnitType == UNIT_MISSILE)
	{
		pDynamicPath->dwCollisionType = 0;
		pDynamicPath->unk0x50 = 0;
		PATH_SetType(pDynamicPath, 4);
	}

	if (pRoom)
	{
		D2Common_10222(pUnit);
		UNITROOM_AddUnitToRoom(pUnit, pDynamicPath->pRoom);
	}

	nTargetX = pDynamicPath->dwPrecisionX >> 11;
	nTargetY = pDynamicPath->dwPrecisionY >> 11;

	DUNGEON_FlattenCoords_IsoToCartesian(&nTargetX, &nTargetY);

	pDynamicPath->dwTargetX = nTargetX;
	pDynamicPath->dwTargetY = nTargetY;

	if (bSetFlag)
	{
		pDynamicPath->dwFlags |= 0x10;
	}
}

//D2Common.0x6FDA9720
//TODO: Find a name
void __fastcall sub_6FDA9720(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection)
{
	int nUnitType = 0;

	if (pDynamicPath->pUnit)
	{
		nUnitType = pDynamicPath->pUnit->dwUnitType;
	}
	else
	{
		nUnitType = 6;
	}

	if (nUnitType == UNIT_OBJECT)
	{
		pDynamicPath->nDirection = (nDirection & 63);
	}
	else if (nUnitType == UNIT_MISSILE)
	{
		if (!(pDynamicPath->dwFlags & 0x40))
		{
			pDynamicPath->nDirection = (nDirection & 63);
		}
	}
	else if (nUnitType == UNIT_ITEM)
	{
		pDynamicPath->nDirection = (nDirection & 63);
	}
	else
	{
		if ((nDirection & 63) != pDynamicPath->nNewDirection)
		{
			pDynamicPath->nNewDirection = (nDirection & 63);
			pDynamicPath->nDiffDirection = byte_6FDD1F88[4 * (((nDirection & 63) - pDynamicPath->nDirection) & 63)];
		}
	}
}

//D2Common.0x6FDA9770 (#10193)
//TODO: Check name
void __stdcall D2COMMON_10193_PATH_AdjustDirection(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath->nDirection != pDynamicPath->nNewDirection && !(pDynamicPath->dwFlags & 0x40))
	{
		pDynamicPath->nDirection = ((pDynamicPath->nDirection + pDynamicPath->nDiffDirection) & 63);

		if (((pDynamicPath->nDirection - pDynamicPath->nNewDirection) & 63) > 31)
		{
			if (pDynamicPath->nDiffDirection > 63)
			{
				pDynamicPath->nDirection = pDynamicPath->nNewDirection;
			}
		}
		else if (((pDynamicPath->nDirection - pDynamicPath->nNewDirection) & 63) < 31)
		{
			if (pDynamicPath->nDiffDirection < 63)
			{
				pDynamicPath->nDirection = pDynamicPath->nNewDirection;
			}
		}
	}
}

//D2Common.0x6FDA97C0 (#10216)
//TODO: Find a name
void __stdcall D2Common_10216(D2DynamicPathStrc* pDynamicPath, int nX, int nY, int a4)
{
	uint8_t nDirection = 0;

	if (pDynamicPath)
	{
		nDirection = sub_6FDAC760(pDynamicPath->dwPrecisionX, pDynamicPath->dwPrecisionY, (nX << 16) + 32768, (nY << 16) + 32768);
		if (a4)
		{
			pDynamicPath->nNewDirection = nDirection & 63;
			pDynamicPath->nDirection = nDirection & 63;
		}
		else
		{
			sub_6FDA9720(pDynamicPath, nDirection);
		}
	}
}

//D2Common.0x6FDA9850 (#10228)
//TODO: Find a name
void __stdcall D2Common_10228(D2UnitStrc* pUnit)
{
	if (pUnit->pDynamicPath)
	{
		D2Common_10223(pUnit, 1);
	}
}

//D2Common.0x6FDA9870 (#10143)
//TODO: Find a name
void __stdcall D2Common_10143(D2UnitStrc* pUnit, int a2)
{
	if (a2 || pUnit->dwUnitType != UNIT_MONSTER)
	{
		if (pUnit->pDynamicPath)
		{
			D2Common_10223(pUnit, 1);
		}
		COLLISION_SetMaskWithSizeXY(pUnit->pDynamicPath->pRoom, pUnit->pDynamicPath->wPosX, pUnit->pDynamicPath->wPosY, 3, 3, 32768);
		D2Common_10233(pUnit->pDynamicPath);
	}
	else
	{
		D2Common_10223(pUnit, 1);
		pUnit->pDynamicPath->dwCollisionPattern = 5;
		PATH_SetCollisionType(pUnit->pDynamicPath, 32768);
		D2Common_10222(pUnit);
		D2Common_10233(pUnit->pDynamicPath);
	}
}

//D2Common.0x6FDA98F0 (#10144)
//TODO: Find a name
void __stdcall D2Common_10144(D2UnitStrc* pUnit, BOOL bDoNothing)
{
	D2MonStatsTxt* pMonStatsTxtRecord = NULL;
	int nCollisionPattern = 0;
	int nClassId = 0;

	if (!bDoNothing)
	{
		D2_ASSERT(pUnit && pUnit->dwUnitType == UNIT_MONSTER);

		if (pUnit->pDynamicPath && pUnit->pDynamicPath->dwCollisionType == 32768)
		{
			nCollisionPattern = D2Common_11281_Unused(pUnit, UNITS_GetUnitSizeX(pUnit));

			if (pUnit->dwUnitType == UNIT_MONSTER)
			{
				nClassId = MONSTERS_GetBaseIdFromMonsterId(pUnit->dwClassId);

				if (nClassId == MONSTER_WRAITH1 || nClassId == MONSTER_BIRD1 || nClassId == MONSTER_BIRD2 || nClassId == MONSTER_PARROT)
				{
					nCollisionPattern = 5;
				}
			}

			if (UNITS_GetRoom(pUnit))
			{
				D2Common_10223(pUnit, 1);
				pUnit->pDynamicPath->dwCollisionPattern = nCollisionPattern;
				PATH_SetCollisionType(pUnit->pDynamicPath, 256);
				D2Common_10222(pUnit);
			}
			else
			{
				pUnit->pDynamicPath->dwCollisionPattern = nCollisionPattern;
				PATH_SetCollisionType(pUnit->pDynamicPath, 256);
			}
		}
	}
}

//D2Common.0x6FDA9A70 (#10146)
void __stdcall PATH_SetVelocity(D2DynamicPathStrc* pDynamicPath, int nVelocity, char* szFile, int nLine)
{
	if (pDynamicPath)
	{
		if (nVelocity != pDynamicPath->dwVelocity)
		{
			pDynamicPath->unk0x38 = 15;
		}

		pDynamicPath->dwVelocity = nVelocity;
		pDynamicPath->dwVelocity2 = nVelocity;
	}
	else
	{
		FOG_WriteToLogFile("PathSetVelocity passed NULL hPath from FILE:%s  LINE:%d", szFile, nLine);
	}
}

//D2Common.0x6FDA9AB0 (#10147)
int __stdcall PATH_GetVelocity(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwVelocity;
}

//D2Common.0x6FDA9AC0 (#10148)
//TODO: Find a name
void __stdcall D2Common_10148(D2DynamicPathStrc* pDynamicPath, int a2)
{
	pDynamicPath->dwVelocity2 = a2;
}

//D2Common.0x6FDA9AE0 (#10149)
//TODO: Find a name
int __stdcall D2Common_10149(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwVelocity2;
}

//D2Common.0x6FDA9AF0 (#10150)
//TODO: Find a name
void __stdcall D2Common_10150(D2DynamicPathStrc* pDynamicPath, int a2)
{
	pDynamicPath->unk0x88[0] = a2;
}

//D2Common.0x6FDA9B10 (#10151)
//TODO: Find a name
int __stdcall D2Common_10151(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->unk0x88[0];
}

//D2Common.0x6FDA9B20 (#10153)
//TODO: Find a name
void __stdcall D2Common_10153(D2DynamicPathStrc* pDynamicPath)
{
	pDynamicPath->dwPathPoints = 0;
	pDynamicPath->dwFlags &= 0xFFFFFFDF;
}

//D2Common.0x6FDA9B40 (#10208)
//TODO: Check name
void __stdcall D2COMMON_10208_PathSetPathingFlag(D2DynamicPathStrc* pDynamicPath, BOOL bSet)
{
	if (bSet)
	{
		pDynamicPath->dwFlags |= 0x20;
	}
	else
	{
		pDynamicPath->dwFlags &= 0xFFFFFFDF;
	}
}

//D2Common.0x6FDA9B70 (#10209)
//TODO: Check name
BOOL __stdcall D2COMMON_10209_PathCheckPathingFlag(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwFlags & 0x20;
}

//D2Common.0x6FDA9B80 (#10154)
//TODO: Find a name
int __stdcall D2Common_10154(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwPathPoints;
}

//D2Common.0x6FDA9B90 (#11291)
//TODO: Find a name
void __stdcall D2Common_11291(D2DynamicPathStrc* pDynamicPath, int a2)
{
	if (a2 <= 77)
	{
		pDynamicPath->dwPathPoints = a2;
	}
	else
	{
		pDynamicPath->dwPathPoints = 77;
	}
}

//D2Common.0x6FDA9BC0 (#10155)
//TODO: Find a name
int __stdcall D2Common_10155(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->unk0x24;
}

//D2Common.0x6FDA9BD0 (#10157)
int __stdcall PATH_GetPathPoints(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc** ppPathPoints)
{
	*ppPathPoints = pDynamicPath->PathPoints;
	return pDynamicPath->dwPathPoints;
}

//D2Common.0x6FDA9BF0 (#10158)
uint8_t __stdcall PATH_GetDirection(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath)
	{
		return pDynamicPath->nDirection;
	}

	return 0;
}

//D2Common.0x6FDA9C10 (#10159)
uint8_t __stdcall PATH_GetNewDirection(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->nNewDirection;
}

//D2Common.0x6FDA9C20 (#10160)
//TODO: Check name
void __stdcall D2COMMON_10160_PathUpdateDirection(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection)
{
	D2_ASSERT(nDirection < 64);

	sub_6FDA9720(pDynamicPath, nDirection);
}

//D2Common.0x6FDA9C90 (#10161)
void __stdcall PATH_SetDirection(D2DynamicPathStrc* pDynamicPath, uint8_t nDirection)
{
	if (pDynamicPath)
	{
		pDynamicPath->nDirection = nDirection & 63;
		pDynamicPath->nNewDirection = nDirection & 63;
	}
}

//D2Common.0x6FDA9CB0 (#10162)
int __stdcall PATH_GetXPosition(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath);

	return pDynamicPath->wPosX;
}

//D2Common.0x6FDA9CF0 (#10163)
int __stdcall PATH_GetYPosition(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath);

	return pDynamicPath->wPosY;
}

//D2Common.0x6FDA9D30 (#10194)
int __stdcall PATH_GetPrecisionX(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath);

	return pDynamicPath->dwPrecisionX;
}

//D2Common.0x6FDA9D60 (#10195)
int __stdcall PATH_GetPrecisionY(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath);

	return pDynamicPath->dwPrecisionY;
}

//D2Common.0x6FDA9D90 (#10196)
void __stdcall PATH_SetPrecisionX(D2DynamicPathStrc* pDynamicPath, int nPrecisionX)
{
	pDynamicPath->dwPrecisionX = nPrecisionX;
}

//D2Common.0x6FDA9DA0 (#10197)
void __stdcall PATH_SetPrecisionY(D2DynamicPathStrc* pDynamicPath, int nPrecisionY)
{
	pDynamicPath->dwPrecisionY = nPrecisionY;
}

//D2Common.0x6FDA9DB0 (#10164)
int __stdcall PATH_GetTargetX(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwTargetX;
}

//D2Common.0x6FDC3CE0 (#10165)
int __stdcall PATH_GetTargetY(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwTargetY;
}

//D2Common.0x6FDA9DC0
void __fastcall PATH_SetTargetX(D2DynamicPathStrc* pDynamicPath, int nTargetX)
{
	pDynamicPath->dwTargetX = nTargetX;
}

//D2Common.0x6FDA9DD0
void __fastcall PATH_SetTargetY(D2DynamicPathStrc* pDynamicPath, int nTargetY)
{
	pDynamicPath->dwTargetY = nTargetY;
}

//D2Common.0x6FDA9DE0 (#10175)
//TODO: Check name
int __stdcall D2COMMON_10175_PathGetFirstPointX(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->xSP1;
}

//D2Common.0x6FDA9DF0 (#10176)
//TODO: Check name
int __stdcall D2COMMON_10176_PathGetFirstPointY(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->ySP1;
}

//D2Common.0x6FDA9E00 (#10224)
//TODO: Find a name
int __stdcall D2Common_10224(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->xSP3;
}

//D2Common.0x6FDA9E10 (#10225)
//TODO: Find a name
int __stdcall D2Common_10225(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->ySP3;
}

//D2Common.0x6FDA9E20 (#10177)
int __stdcall D2COMMON_10177_PATH_GetLastPointX(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath->dwPathPoints)
	{
		return pDynamicPath->PathPoints[pDynamicPath->dwPathPoints].X;
	}

	return 0;
}

//D2Common.0x6FDA9E40 (#10178)
int __stdcall D2COMMON_10178_PATH_GetLastPointY(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath->dwPathPoints)
	{
		return pDynamicPath->PathPoints[pDynamicPath->dwPathPoints].Y;
	}

	return 0;
}

//D2Common.0x6FDB9C10 (#10166)
D2RoomStrc* __stdcall PATH_GetRoom(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->pRoom;
}

//D2Common.0x6FDA9E60 (#10167)
void __stdcall PATH_SetRoom(D2DynamicPathStrc* pDynamicPath, D2RoomStrc* pRoom)
{
	pDynamicPath->pRoom = pRoom;
}

//D2Common.0x6FDA9E70 (#10168)
D2RoomStrc* __stdcall PATH_GetNextRoom(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->pRoomNext;
}

//D2Common.0x6FDA9E80 (#10169)
void __stdcall PATH_ClearNextRoom(D2DynamicPathStrc* pDynamicPath)
{
	pDynamicPath->pRoomNext = NULL;
}

//D2Common.0x6FDA9E90 (#10170)
//TODO: Check name
void __stdcall D2COMMON_10170_PathSetTargetPos(D2DynamicPathStrc* pDynamicPath, int nX, int nY)
{
	if (pDynamicPath)
	{
		pDynamicPath->xSP1 = nX;
		pDynamicPath->ySP1 = nY;
		pDynamicPath->pTargetUnit = NULL;
	}
}

//D2Common.0x6FDA9EC0 (#10172)
//TODO: Find a name
BOOL __stdcall D2Common_10172(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwFlags & 2;
}

//D2Common.0x6FDA9ED0 (#10173)
//TODO: Find a name
void __stdcall D2Common_10173(D2DynamicPathStrc* pDynamicPath, BOOL bSet)
{
	if (bSet)
	{
		pDynamicPath->dwFlags |= 2;
	}
	else
	{
		pDynamicPath->dwFlags &= 0xFFFFFFFD;
	}
}

//D2Common.0x6FDA9F00 (#10145)
//TODO: Find a name
void __stdcall D2Common_10145(D2DynamicPathStrc* pDynamicPath, BOOL bSet)
{
	if (bSet)
	{
		pDynamicPath->dwFlags |= 4;
	}
	else
	{
		pDynamicPath->dwFlags &= 0xFFFFFFFB;
	}
}

//D2Common.0x6FDA9F30 (#10174)
//TODO: Find a name
BOOL __stdcall D2Common_10174(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwFlags & 4;
}

//D2Common.0x6FDA9F40 (#10179)
void __stdcall PATH_SetTargetUnit(D2DynamicPathStrc* pDynamicPath, D2UnitStrc* pUnit)
{
	pDynamicPath->pTargetUnit = pUnit;

	if (pUnit)
	{
		pDynamicPath->dwTargetType = pUnit->dwUnitType;
		pDynamicPath->dwTargetId = pUnit->dwUnitId;
	}
}

//D2Common.0x6FDA9F60 (#10171)
void __stdcall PATH_GetTargetTypeAndGUID(D2DynamicPathStrc* pDynamicPath, int* pTargetType, int* pTargetGUID)
{
	D2_ASSERT(pDynamicPath->pTargetUnit);

	*pTargetType = pDynamicPath->dwTargetType;
	*pTargetGUID = pDynamicPath->dwTargetId;
}

//D2Common.0x6FDA9FA0 (#10180)
D2UnitStrc* __stdcall PATH_GetTargetUnit(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath)
	{
		return pDynamicPath->pTargetUnit;
	}

	return NULL;
}

//D2Common.0x6FDA9FC0 (#10181)
int __stdcall PATH_GetCollisionType(D2DynamicPathStrc* pDynamicPath)
{
	if (pDynamicPath)
	{
		return pDynamicPath->dwCollisionType;
	}

	return 65535;
}

//D2Common.0x6FDA9FE0 (#10182)
void __stdcall PATH_SetCollisionType(D2DynamicPathStrc* pDynamicPath, int nCollisionType)
{
	if (pDynamicPath->pRoom)
	{
		if (pDynamicPath->pUnit && pDynamicPath->pUnit->dwUnitType == UNIT_MISSILE)
		{
			COLLISION_ResetMaskWithSize(pDynamicPath->pRoom, pDynamicPath->wPosX, pDynamicPath->wPosY, pDynamicPath->dwUnitSize, pDynamicPath->dwCollisionType);
		}
		else
		{
			COLLISION_ResetMaskWithPattern(pDynamicPath->pRoom, pDynamicPath->wPosX, pDynamicPath->wPosY, pDynamicPath->dwCollisionPattern, pDynamicPath->dwCollisionType);
		}
	}

	pDynamicPath->dwCollisionType = nCollisionType;

	if (pDynamicPath->pRoom)
	{
		if (pDynamicPath->pUnit && pDynamicPath->pUnit->dwUnitType == UNIT_MISSILE)
		{
			COLLISION_SetMaskWithSize(pDynamicPath->pRoom, pDynamicPath->wPosX, pDynamicPath->wPosY, pDynamicPath->dwUnitSize, nCollisionType);
		}
		else
		{
			COLLISION_SetMaskWithPattern(pDynamicPath->pRoom, pDynamicPath->wPosX, pDynamicPath->wPosY, pDynamicPath->dwCollisionPattern, nCollisionType);
		}
	}
}

//D2Common.0x6FDAA0C0 (#10183)
//TODO: Find a name
int __stdcall D2Common_10183(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->unk0x50;
}

//D2Common.0x6FDAA0D0 (#10184)
//TODO: Find a name
void __stdcall D2Common_10184(D2DynamicPathStrc* pDynamicPath, int a2)
{
	pDynamicPath->unk0x50 = a2;
}

//D2Common.0x6FDAA0E0 (#10185)
void __stdcall PATH_SetType(D2DynamicPathStrc* pDynamicPath, int nPathType)
{
	int nFlag = 0;

	D2_ASSERT(!(pDynamicPath->pUnit && pDynamicPath->pUnit->dwUnitType == UNIT_PLAYER && nPathType == PATHTYPE_TOWARD));

	nFlag = dword_6FDD2088[nPathType];
	if (nFlag & 0x2000 && !(pDynamicPath->dwFlags & 0x4000))
	{
		pDynamicPath->dwPrevPathType = pDynamicPath->dwPathType;
	}

	if (nFlag & 0x8000 && !(pDynamicPath->dwFlags & 0x10000))
	{
		pDynamicPath->unk0x80 = pDynamicPath->dwVelocity;
	}

	pDynamicPath->dwPathType = nPathType;
	pDynamicPath->dwFlags = nFlag | pDynamicPath->dwFlags & 0xFFF800FF;
	pDynamicPath->PathPoints[0].X = word_6FDD20D0[2 * nPathType];
	pDynamicPath->PathPoints[0].Y = word_6FDD20D0[2 * nPathType + 1];

	D2_ASSERT(pDynamicPath->dwPrevPathType != PATHTYPE_KNOCKBACK_CLIENT);
	D2_ASSERT(pDynamicPath->dwPrevPathType != PATHTYPE_KNOCKBACK_SERVER);

#define MAXPATHLEN 78
	D2_ASSERT((nPathType != PATHTYPE_MISSILE) || (pDynamicPath->nDistMax < MAXPATHLEN));
}

//D2Common.0x6FDAA1E0 (#10186)
void __stdcall PATH_ResetToPreviousType(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath->pUnit);

	if (pDynamicPath->dwFlags & 0x8000)
	{
		pDynamicPath->dwVelocity = pDynamicPath->unk0x80;
	}

	if (pDynamicPath->pUnit->dwUnitType != UNIT_PLAYER)
	{
		if (pDynamicPath->dwFlags & 0x2000)
		{
			PATH_SetType(pDynamicPath, pDynamicPath->dwPrevPathType);
		}
	}
	else
	{
		PATH_SetType(pDynamicPath, PATHTYPE_UNKNOWN_7);
	}
}

//D2Common.0x6FDAA240 (#10187)
int __stdcall PATH_GetType(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->dwPathType;
}

//D2Common.0x6FDAA250 (#10190)
void __stdcall _10190_PATH_SetDistance(D2DynamicPathStrc* pDynamicPath, uint8_t nDistance)
{
	pDynamicPath->nDist = nDistance;
}

//D2Common.0x6FDAA270 (#10191)
uint8_t __stdcall _10191_PATH_GetDistance(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->nDist;
}

//D2Common.0x6FDAA280 (#10188)
void __stdcall PATH_SetNewDistance(D2DynamicPathStrc* pDynamicPath, uint8_t nNewDistance)
{
	if (pDynamicPath)
	{
		if (nNewDistance >= 77)
		{
			nNewDistance = 77;
		}

		pDynamicPath->nDistMax = nNewDistance;
		pDynamicPath->nDist = nNewDistance;
	}
}

//D2Common.0x6FDAA2B0 (#10189)
uint8_t __stdcall PATH_GetMaxDistance(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->nDistMax;
}

//D2Common.0x6FDAA2C0 (#10201)
//TODO: Find a name
uint16_t __stdcall D2Common_10201(D2DynamicPathStrc* pDynamicPath)
{
	if (!pDynamicPath->dwVelocity)
	{
		pDynamicPath->unk0x54 = COLLISION_CheckMaskWithSize(pDynamicPath->pRoom, pDynamicPath->wPosX, pDynamicPath->wPosY, pDynamicPath->dwUnitSize, 0x7FFF);
	}

	return pDynamicPath->unk0x54;
}

//D2Common.0x6FDAA300 (#10202)
//TODO: Find a name
uint16_t __stdcall D2Common_10202(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->unk0x54;
}

//D2Common.0x6FDAA310 (#10192)
//TODO: Check name
void __stdcall D2COMMON_10192_PathSetIDAMax(D2DynamicPathStrc* pDynamicPath, int a2)
{
	D2_ASSERT(pDynamicPath->dwPathType != PATHTYPE_IDASTAR);

	pDynamicPath->unk0x92 = a2;
}

//D2Common.0x6FDAA350 (#10198)
//TODO: Check name
int __stdcall D2COMMON_10198_PathGetSaveStep(D2DynamicPathStrc* pDynamicPath, D2PathPointStrc** ppPathPoints)
{
	D2_ASSERT(pDynamicPath->dwFlags & 0x20000);

	*ppPathPoints = pDynamicPath->unk0x1D8;
	return pDynamicPath->unk0x1D4;
}

//D2Common.0x6FDAA390 (#10199)
//TODO: Check name
int __stdcall D2COMMON_10199_PathGetSaveX(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath->dwFlags & 0x20000);

	if (pDynamicPath->unk0x1D4)
	{
		return pDynamicPath->unk0x1D8[pDynamicPath->unk0x1D4 -1].X;
	}

	return 0;
}

//D2Common.0x6FDAA3E0 (#10200)
//TODO: Check name
int __stdcall D2COMMON_10200_PathGetSaveY(D2DynamicPathStrc* pDynamicPath)
{
	D2_ASSERT(pDynamicPath->dwFlags & 0x20000);

	if (pDynamicPath->unk0x1D4)
	{
		return pDynamicPath->unk0x1D8[pDynamicPath->unk0x1D4 - 1].Y;
	}

	return 0;
}

//D2Common.0x6FDAA430 (#10203)
//TODO: Check name
void __stdcall D2COMMON_10203_PATH_SetRotateFlag(D2DynamicPathStrc* pDynamicPath, BOOL bReset)
{
	if (pDynamicPath)
	{
		if (bReset)
		{
			pDynamicPath->dwFlags &= 0xFFFFFFBF;
		}
		else
		{
			pDynamicPath->dwFlags |= 0x40;
		}
	}
}

//D2Common.0x6FDAA460 (#10204)
//TODO: Check name
void __stdcall D2COMMON_10204_PATH_ClearPoint2(D2DynamicPathStrc* pDynamicPath)
{
	pDynamicPath->xSP2 = 0;
	pDynamicPath->ySP2 = 0;
}

//D2Common.0x6FDAA480 (#10205)
void __stdcall PATH_SetStepNum(D2DynamicPathStrc* pDynamicPath, uint8_t nSteps)
{
	if (pDynamicPath)
	{
		if (nSteps && nSteps < 20)
		{
			pDynamicPath->nStepNum = nSteps - 1;
		}
		else
		{
			pDynamicPath->nStepNum = 0;
		}
	}
}

//D2Common.0x6FDAA4B0 (#10206)
int __stdcall PATH_GetStepNum(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->nStepNum + 1;
}

//D2Common.0x6FDAA4C0 (#10207)
//TODO: Find a name
void __stdcall D2Common_10207(D2DynamicPathStrc* pDynamicPath, char a2, char a3)
{
	pDynamicPath->unk0x67 = a2;
	pDynamicPath->unk0x68[0] = a3;
}

//D2Common.0x6FDAA4E0 (#10217)
void __stdcall PATH_SetDistance(D2DynamicPathStrc* pDynamicPath, int nDist)
{
	D2_ASSERT(nDist >= 0 && nDist <= 255);

	pDynamicPath->nDistance = nDist;
}

//D2Common.0x6FDAA520 (#10218)
int __stdcall PATH_GetDistance(D2DynamicPathStrc* pDynamicPath)
{
	return pDynamicPath->nDistance;
}

//D2Common.0x6FDAA530 (#10219)
void __stdcall PATH_AddToDistance(D2DynamicPathStrc* pDynamicPath, int nAddition)
{
	int nDistance = nAddition + pDynamicPath->nDistance;

	if (nDistance < 0)
	{
		pDynamicPath->nDistance = 0;
	}
	else if (nDistance > 255)
	{
		pDynamicPath->nDistance = 255;
	}
	else
	{
		pDynamicPath->nDistance = nDistance;
	}
}

//D2Common.0x6FDAA570 (#10210)
int __stdcall PATH_GetUnitCollisionPattern(D2UnitStrc* pUnit)
{
	return pUnit->pDynamicPath->dwCollisionPattern;
}

//D2Common.0x6FDAA580 (#10211)
void __stdcall PATH_SetUnitCollisionPattern(D2UnitStrc* pUnit, int nCollisionPattern)
{
	pUnit->pDynamicPath->dwCollisionPattern = nCollisionPattern;
}

//D2Common.0x6FDAA5A0 (#10212)
//TODO: Check name
void __stdcall D2COMMON_10212_PATH_SetMoveFlags(D2UnitStrc* pUnit, BOOL bSet)
{
	D2DynamicPathStrc* pDynamicPath = pUnit->pDynamicPath;

	D2_ASSERT(pDynamicPath);

	if (bSet)
	{
		pUnit->dwFlagEx |= 0x2000;
		pDynamicPath->unk0x38 = 0;
	}
	else
	{
		pUnit->dwFlagEx &= 0xFFFFDFFF;
		pDynamicPath->unk0x38 = 0;
	}
}

//D2Common.0x6FDAA600 (#10213)
//TODO: Find a name
void __stdcall D2Common_10213(D2UnitStrc* pUnit)
{
	D2DynamicPathStrc* pDynamicPath = NULL;
	D2UnitStrc* pTargetUnit = NULL;

	if (pUnit && !(pUnit->dwFlags & UNITFLAG_ISASYNC) && pUnit->dwFlagEx & 0x2000)
	{
		pDynamicPath = pUnit->pDynamicPath;
		D2_ASSERT(pDynamicPath);

		++pDynamicPath->unk0x38;

		if (pDynamicPath->unk0x38 > 1)
		{
			pDynamicPath->unk0x38 = 0;
			if (!D2Common_10227(pUnit))
			{
				if (pUnit->pDynamicPath)
				{
					pTargetUnit = pUnit->pDynamicPath->pTargetUnit;
				}

				if (D2Common_10236(pUnit, pTargetUnit != 0))
				{
					UNITS_ChangeAnimMode(pUnit, 2);
				}
			}
		}
	}
}

//D2Common.0x6FDAA6A0 (#10220)
//TODO: Find names for functions and arguments
int __stdcall D2Common_10220(int nX1, int nY1, int nX2, int nY2)
{
	return (nX2 - nX1) * (nX2 - nX1) + (nY2 - nY1) * (nY2 - nY1);
}

//D2Common.0x6FDAA6D0 (#10221)
//TODO: Find a name
void __stdcall D2Common_10221(D2UnitStrc* pUnit)
{
	if (pUnit)
	{
		D2Common_10222(pUnit);
	}
}

//D2Common.0x6FDAA6F0 (#10237)
//TODO: Find a name
BOOL __stdcall D2Common_10237(D2UnitStrc* pUnit)
{
	if (pUnit && pUnit->pDynamicPath)
	{
		return (pUnit->pDynamicPath->unk0x24 < pUnit->pDynamicPath->dwPathPoints);
	}

	return FALSE;
}
