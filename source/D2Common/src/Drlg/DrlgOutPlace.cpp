#include "Drlg/D2DrlgOutPlace.h"

#include "D2DataTbls.h"
#include "Drlg/D2DrlgDrlg.h"
#include "Drlg/D2DrlgDrlgAnim.h"
#include "Drlg/D2DrlgDrlgGrid.h"
#include "Drlg/D2DrlgDrlgRoom.h"
#include "Drlg/D2DrlgDrlgVer.h"
#include "Drlg/D2DrlgDrlgWarp.h"
#include "Drlg/D2DrlgMaze.h"
#include "Drlg/D2DrlgOutdoors.h"
#include "Drlg/D2DrlgOutJung.h"
#include "Drlg/D2DrlgOutRoom.h"
#include "Drlg/D2DrlgOutSiege.h"
#include "Drlg/D2DrlgRoomTile.h"
#include "Drlg/D2DrlgTileSub.h"
#include "Drlg/D2DrlgPreset.h"
#include "Path/PathMisc.h"
#include "D2Seed.h"
#include <assert.h>
#include <DataTbls/LevelsIds.h>

#include <cmath>
#include <algorithm>

int dword_6FDEA6FC;

//D2Common.0x6FDCFE40
static D2DrlgLinkStrc gAct1WildernessDrlgLink[15] =
{
	{ sub_6FD81330, LEVEL_STONYFIELD, -1, -1 },
	{ sub_6FD81380, LEVEL_COLDPLAINS, 0, -1 },
	{ sub_6FD81950, LEVEL_BLOODMOOR, 1, -1 },
	{ sub_6FD81720, LEVEL_ROGUEENCAMPMENT, 2, -1 },
	{ sub_6FD81380, LEVEL_BURIALGROUNDS, 1, -1 },
	{ NULL, 0, -1, -1 },
};

//D2Common.0x6FDCFF30
static D2DrlgLinkStrc gAct1MonasteryDrlgLink[15] =
{
	{ sub_6FD81330, LEVEL_MOOMOOFARM, -1, -1 },
	{ sub_6FD81330, LEVEL_MONASTERYGATE, -1, -1 },
	{ sub_6FD81AD0, LEVEL_TAMOEHIGHLAND, 1, -1 },
	{ sub_6FD81380, LEVEL_BLACKMARSH, 2, -1 },
	{ sub_6FD81380, LEVEL_DARKWOOD, 3, -1 },
	{ NULL, 0, -1, -1 },
};

//D2Common.0x6FDD0020
static D2DrlgLinkStrc gAct2OutdoorDrlgLink[15] =
{
	{ sub_6FD81330, LEVEL_LUTGHOLEIN, -1, -1 },
	{ sub_6FD81B30, LEVEL_ROCKYWASTE, 0, -1 },
	{ sub_6FD81530, LEVEL_DRYHILLS, 1, -1 },
	{ sub_6FD81530, LEVEL_FAROASIS, 2, -1 },
	{ sub_6FD81530, LEVEL_LOSTCITY, 3, -1 },
	{ sub_6FD81BF0, LEVEL_VALLEYOFSNAKES, 4, -1 },
	{ NULL, 0, -1, -1 },
};

//D2Common.0x6FDD0110
static D2DrlgLinkStrc gAct2CanyonDrlgLink[15] =
{
	{ sub_6FD81330, LEVEL_CANYONOFTHEMAGI, -1, -1 },
	{ NULL, 0, -1, -1 },
};

//D2Common.0x6FDD0200
static D2DrlgLinkStrc gAct4OutdoorDrlgLink[15] =
{
	{ sub_6FD81330, LEVEL_THEPANDEMONIUMFORTRESS, -1, -1 },
	{ sub_6FD81CA0, LEVEL_OUTERSTEPPES, 0, -1 },
	{ sub_6FD81380, LEVEL_PLAINSOFDESPAIR, 1, -1 },
	{ sub_6FD81380, LEVEL_CITYOFTHEDAMNED, 2, -1 },
	{ NULL, 0, -1, -1 },
};

//D2Common.0x6FDD02F0
static D2DrlgLinkStrc gAct4ChaosSanctumDrlgLink[15] =
{
	{ sub_6FD81330, LEVEL_CHAOSSANCTUM, -1, -1 },
	{ NULL, 0, -1, -1 },
};

//D2Common.0x6FDD03E0
static D2DrlgLinkStrc gAct5OutdoorDrlgLink[15] =
{
	{ sub_6FD81330, LEVEL_HARROGATH, -1, -1 },
	{ sub_6FD81330, LEVEL_BLOODYFOOTHILLS, 0, -1 },
	{ DRLGOUTROOM_LinkLevelsByLevelCoords, LEVEL_ID_ACT5_BARRICADE_1, 1, -1 },
	{ DRLGOUTROOM_LinkLevelsByOffsetCoords, LEVEL_ARREATPLATEAU, 2, -1 },
	{ NULL, 0, -1, -1 },
};

//D2Common.0x6FDD04D0
static D2DrlgLinkStrc gAct5TundraDrlgLink[15] =
{
	{ DRLGOUTROOM_LinkLevelsByLevelDef, LEVEL_TUNDRAWASTELANDS, -1, -1 },
	{ NULL, 0, -1, -1 },
};

static const int gnBarricadeCliffBorderIds[14][2] =
{
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_1,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_1_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_2,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_2_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_3,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_3_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_4,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_4_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_5,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_5_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_6,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_6_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_7,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_7_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_8,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_8_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_9,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_9_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_10,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_10_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_11,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_11_SNOW },
	{ LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_12,		LVLPREST_ACT5_BARRICADE_CLIFF_BORDER_12_SNOW },
	{ -1,											1 },
	{ -1,											0 },
};


//D2Common.0x6FD80480
void __fastcall DRLGOUTPLACE_BuildKurast(D2DrlgLevelStrc* pLevel)
{
	switch (pLevel->nLevelId)
	{
	case LEVEL_LOWERKURAST:
		DRLGOUTJUNG_BuildLowerKurast(pLevel);
		break;

	case LEVEL_KURASTBAZAAR:
		DRLGOUTJUNG_BuildKurastBazaar(pLevel);
		break;

	case LEVEL_UPPERKURAST:
		DRLGOUTJUNG_BuildUpperKurast(pLevel);
		break;
	}

	switch (pLevel->nLevelId)
	{
	case LEVEL_LOWERKURAST:
		DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, LVLPREST_ACT3_BURBS_WAYPOINT, 0, 0, 15);
		DRLGOUTJUNG_SpawnRandomPreset(pLevel, LVLPREST_ACT3_SLUMS_16X16, LVLPREST_ACT3_SLUMS_16X16, 4);
		DRLGOUTJUNG_SpawnRandomPreset(pLevel, LVLPREST_ACT3_SLUMS_08X16, LVLPREST_ACT3_SLUMS_16X08, 0);
		DRLGOUTJUNG_SpawnRandomPreset(pLevel, LVLPREST_ACT3_SLUMS_08X08, LVLPREST_ACT3_SLUMS_08X08, 0);
		break;

	case LEVEL_KURASTBAZAAR:
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 3, 3, LVLPREST_ACT3_BURBS_SEWER, 0, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, pLevel->nWidth / 8 - 4, 3, LVLPREST_ACT3_BURBS_SEWER, 1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, LVLPREST_ACT3_BURBS_TEMPLE, 0, 0, 15);
		DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, LVLPREST_ACT3_BURBS_TEMPLE, 1, 0, 15);
		DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, LVLPREST_ACT3_BURBS_WAYPOINT, 0, 0, 15);
		DRLGOUTJUNG_SpawnRandomPreset(pLevel, LVLPREST_ACT3_BURBS_16X16, LVLPREST_ACT3_BURBS_16X16, 4);
		DRLGOUTJUNG_SpawnRandomPreset(pLevel, LVLPREST_ACT3_BURBS_08X16, LVLPREST_ACT3_BURBS_16X08, 0);
		DRLGOUTJUNG_SpawnRandomPreset(pLevel, LVLPREST_ACT3_BURBS_08X08, LVLPREST_ACT3_BURBS_08X08, 0);
		break;

	case LEVEL_UPPERKURAST:
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 3, pLevel->nHeight / 8 - 4, LVLPREST_ACT3_METRO_SEWER, 0, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, pLevel->nWidth / 8 - 4, pLevel->nHeight / 8 - 4, LVLPREST_ACT3_METRO_SEWER, 1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, LVLPREST_ACT3_METROTEMPLE, 0, 0, 15);
		DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, LVLPREST_ACT3_METROTEMPLE, 1, 0, 15);
		DRLGOUTDOORS_SpawnOutdoorLevelPreset(pLevel, LVLPREST_ACT3_BURBS_WAYPOINT, 0, 0, 15);
		DRLGOUTJUNG_SpawnRandomPreset(pLevel, LVLPREST_ACT3_METRO_16X16, LVLPREST_ACT3_METRO_16X16, 4);
		DRLGOUTJUNG_SpawnRandomPreset(pLevel, LVLPREST_ACT3_METRO_08X16, LVLPREST_ACT3_METRO_16X08, 0);
		DRLGOUTJUNG_SpawnRandomPreset(pLevel, LVLPREST_ACT3_METRO_08X08, LVLPREST_ACT3_METRO_08X08, 0);
		break;

	case LEVEL_KURASTCAUSEWAY:
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, 0, LVLPREST_ACT3_BRIDGE, 0, 0);
		break;

	default:
		return;
	}
}

//D2Common.0x6FD806A0
void __fastcall DRLGOUTPLACE_InitAct3OutdoorLevel(D2DrlgLevelStrc* pLevel)
{
	DRLGOUTPLACE_SetOutGridLinkFlags(pLevel);
	DRLGOUTJUNG_BuildJungle(pLevel);
	DRLGOUTPLACE_BuildKurast(pLevel);

	if (pLevel->nLevelId == LEVEL_TRAVINCAL)
	{
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, 0, LVLPREST_ACT3_TRAVINCAL_NW, -1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 2, 0, LVLPREST_ACT3_TRAVINCAL_N, -1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 6, 0, LVLPREST_ACT3_TRAVINCAL_NE, -1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 0, 4, LVLPREST_ACT3_TRAVINCAL_SW, -1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 2, 4, LVLPREST_ACT3_TRAVINCAL_S, -1, 0);
		DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, 6, 4, LVLPREST_ACT3_TRAVINCAL_SE, -1, 0);
	}
}

#pragma pack(1)
struct D2UnkOutPlaceStrc12
{
	int field_0;						//0x00
	int field_4;						//0x04
	int field_8;						//0x08
	int nX;								//0x0C
	int nY;								//0x10
	int field_14;						//0x14
	uint8_t* field_18;					//0x18
	uint8_t nIndex;						//0x1C
	uint8_t field_1D;					//0x1D
	uint8_t field_1E;					//0x1E
	uint8_t field_1F;					//0x1F
	D2UnkOutPlaceStrc12* pNext;			//0x20
	D2UnkOutPlaceStrc12* field_24;		//0x24
};
#pragma pack()

uint8_t byte_6FDCFB70[8] = { 0, 1, 2, 3, 0, 1, 1, 1 };

char byte_6FDCFB80[] = { 1, 0, -1, 0 };
char byte_6FDCFB84[] = { 0, 1, 0, -1 };

//D2Common.0x6FD80750) --------------------------------------------------------
//TODO: Finish
BOOL __fastcall sub_6FD80750(D2DrlgLevelStrc* pLevel, int nVertexId)
{
	int v18; // eax@7
	D2UnkOutPlaceStrc12 *v21; // esi@15
	int v23; // eax@18
	int v24; // ebp@18
	int v25; // ebx@18
	D2UnkOutPlaceStrc12 *v28; // eax@22
	signed int v29; // eax@27
	int v30; // edx@29
	int v31; // eax@29
	int v32; // ecx@31
	int v33; // edi@33
	int v34; // eax@36
	int v35; // ecx@36
	int v36; // eax@37
	char v37; // bl@38
	uint8_t *v38; // ecx@38
	int v39; // eax@39
	char v40; // bl@41
	uint8_t *v41; // ebp@41
	int v42; // eax@41
	D2UnkOutPlaceStrc12 *v50; // edi@51
	void *v51; // edi@52
	D2UnkOutPlaceStrc12 *v52; // ST3C_4@52
	int v53; // eax@55
	uint8_t *v54; // ecx@55
	int v55; // ecx@57
	int v58; // [sp+10h] [bp-8D08h]@14
	int v64; // [sp+38h] [bp-8CE0h]@14
	D2UnkOutPlaceStrc12 v66; // [sp+48h] [bp-8CD0h]@7
	D2UnkOutPlaceStrc12 *v67; // [sp+70h] [bp-8CA8h]@14
	D2UnkOutPlaceStrc12 v68[900]; // [sp+74h] [bp-8CA4h]@14
	int v69; // [sp+8D14h] [bp-4h]@14


	D2DrlgVertexStrc* pPreviousVertex = NULL;
	D2DrlgVertexStrc* pDrlgVertex = NULL;
	int nXDiff = 0;
	int nYDiff = 0;
	int nMin = 0;
	int nMax = 0;
	int nX1 = 0;
	int nY1 = 0;
	int nX2 = 0;
	int nY2 = 0;


	nX1 = (pLevel->pOutdoors->pVertices[6 + nVertexId].nPosX - pLevel->nPosX) / 8;
	nY1 = (pLevel->pOutdoors->pVertices[6 + nVertexId].nPosY - pLevel->nPosY) / 8;

	nX2 = (pLevel->pOutdoors->pVertices[12 + nVertexId].nPosX - pLevel->nPosX) / 8;
	nY2 = (pLevel->pOutdoors->pVertices[12 + nVertexId].nPosY - pLevel->nPosY) / 8;

	nXDiff = nX1 - nX2;
	if (nXDiff < 0)
	{
		nXDiff = -nXDiff;
	}

	nYDiff = nY1 - nY2;
	if (nYDiff < 0)
	{
		nYDiff = -nYDiff;
	}

	if (nXDiff + nYDiff >= 2)
	{
		v18 = sub_6FDAB750(nX1, nY1, nX2, nY2) / 2;

		if (nXDiff < nYDiff)
		{
			nMin = nXDiff;
			nMax = nYDiff;
		}
		else
		{
			nMin = nYDiff;
			nMax = nXDiff;
		}

		memset(v68, 0x00, sizeof(D2UnkOutPlaceStrc12));

		v66.field_0 = nMin + 2 * nMax;
		v66.field_4 = v66.field_0;
		v66.field_8 = 0;
		v66.nX = nX1;
		v66.nY = nY1;
		v66.field_14 = -1;
		v66.field_18 = byte_6FDCFB70;
		v66.nIndex = v18 & 3;
		v66.pNext = NULL;
		v66.field_24 = NULL;

		v58 = v66.field_4 + v66.field_4 / 2;
		v64 = v58 + 35;

		v67 = v68;

		v69 = 1;
		while (1)
		{
			memcpy(v67, &v66, sizeof(D2UnkOutPlaceStrc12));
			v21 = v67;

LABEL_16:
			while (v21->nX != nX2 || v21->nY != nY2)
			{
				v23 = v21->nIndex;
				v24 = v21->nX + byte_6FDCFB80[v23];
				v25 = v21->nY + byte_6FDCFB84[v23];
				if (v24 == nX2 && v25 == nY2)
				{
LABEL_26:
					if (v21->nX == v24 || (v29 = 3, v21->nY == v25))
					{
						v29 = 2;
					}

					v30 = v29 + v21->field_8;
					v31 = v24 - nX2;
					if (v31 < 0)
					{
						v31 = -v31;
					}

					v32 = v25 - nY2;
					if (v32 < 0)
					{
						v32 = -v32;
					}

					v33 = v31;
					if (v31 >= v32)
					{
						v33 = v32;
						if (v31 > v32)
						{
							v32 = v31;
						}
					}

					v34 = v33 + 2 * v32;
					v35 = v34 + v30;
					if (v35 <= v58)
					{
						if (!v21->field_24)
						{
							if (v69 == 900)
							{
								v50 = 0;
							}
							else
							{
								v51 = &v68[v69];
								v52 = &v68[v69];
								++v69;
								memset(v51, 0x00, sizeof(D2UnkOutPlaceStrc12));
								v50 = v52;
								v35 = v34 + v30;
							}

							v21->field_24 = v50;
							if (!v50)
							{
								v21 = 0;
								break;
							}
							v50->pNext = v21;
						}

						v21 = v21->field_24;

						v21->field_0 = v35;
						v21->field_8 = v30;
						v21->field_4 = v34;
						v21->field_14 = 0;
						v53 = sub_6FDAB750(v24, v25, nX2, nY2) / 2;
						v54 = &byte_6FDCFB70[4 * ((v21->pNext->nIndex - (uint8_t)v53) & 3)];
						v21->field_18 = v54;
						v21->nX = v24;
						v21->nY = v25;
						v21->nIndex = (v53 + *v54) & 3;
					}
					else
					{
						v36 = v21->field_14;
						if (v36 < 4)
						{
							v37 = v21->nIndex;
							v38 = v21->field_18 + 1;
							v21->field_18 = v38;
							v21->nIndex = (v37 + *v38) & 3;
						}

						v39 = v36 + 1;
						v21->field_14 = v39;
						if (v39 == 3)
						{
							while (v21 != v67)
							{
								v21 = v21->pNext;
								v40 = v21->nIndex;
								v41 = v21->field_18 + 1;
								v21->field_18 = v41;
								v21->nIndex = (v40 + *v41) & 3;
								v42 = v21->field_14 + 1;
								v21->field_14 = v42;
								if (v42 != 3)
								{
									goto LABEL_16;
								}
							}
							v21 = 0;
							break;
						}
					}
				}
				else
				{
					if (DRLGROOM_AreXYInsideCoordinates(&pLevel->pOutdoors->pCoord, v24, v25))
					{
						if (!(DRLGGRID_GetGridEntry(&pLevel->pOutdoors->pGrid[2], v24, v25) & 0x200))
						{
							v28 = v21;
							while (v28->nX != v24 || v28->nY != v25)
							{
								v28 = v28->pNext;
								if (!v28)
								{
									goto LABEL_26;
								}
							}
						}
					}

					if (v21->field_14 < 4)
					{
						++v21->field_18;
						v21->nIndex = (v21->nIndex + *v21->field_18) & 3;
					}

					++v21->field_14;
					if (v21->field_14 == 3)
					{
						while (v21 != v67)
						{
							v21 = v21->pNext;

							++v21->field_18;
							v21->nIndex = (v21->nIndex + *v21->field_18) & 3;
							++v21->field_14;
							if (v21->field_14 != 3)
							{
								goto LABEL_16;
							}
						}

						v21 = 0;
						break;
					}
				}
			}

			v55 = v58 + 5;
			v58 += 5;
			if (v69 >= 900)
			{
				return 0;
			}

			v69 = 1;
			if (v21)
			{
				break;
			}

			if (v55 >= v64)
			{
				return 0;
			}
		}

		pPreviousVertex = NULL;
		do
		{
			if (pLevel->pOutdoors->pPathStarts[nVertexId])
			{
				pDrlgVertex = DRLGVER_AllocVertex(pLevel->pDrlg->pMempool, 0);
				pPreviousVertex->pNext = pDrlgVertex;
			}
			else
			{
				pDrlgVertex = DRLGVER_AllocVertex(pLevel->pDrlg->pMempool, 0);
				pLevel->pOutdoors->pPathStarts[nVertexId] = pDrlgVertex;
			}

			pPreviousVertex = pDrlgVertex;
			pDrlgVertex->nPosX = v21->nX;
			pDrlgVertex->nPosY = v21->nY;

			v21 = v21->pNext;
		}
		while (v21);
	}
	else
	{
		pLevel->pOutdoors->pPathStarts[nVertexId] = DRLGVER_AllocVertex(pLevel->pDrlg->pMempool, 0);
		pLevel->pOutdoors->pPathStarts[nVertexId]->nPosX = nX1;
		pLevel->pOutdoors->pPathStarts[nVertexId]->nPosY = nY1;

		pLevel->pOutdoors->pPathStarts[nVertexId]->pNext = DRLGVER_AllocVertex(pLevel->pDrlg->pMempool, 0);
		pLevel->pOutdoors->pPathStarts[nVertexId]->pNext->nPosX = nX2;
		pLevel->pOutdoors->pPathStarts[nVertexId]->pNext->nPosY = nY2;
	}

	return TRUE;
}


//TODO: Remove
//D2FUNC(D2COMMON, 6FD80750, BOOL, __fastcall, (D2DrlgLevelStrc*, int), 0x40750)
//BOOL __fastcall sub_6FD80750(D2DrlgLevelStrc* pLevel, int nVertexId)
//{
//	return D2COMMON_6FD80750(pLevel, nVertexId);
//}
//////////////


static const int nBorderIndices[] =
{
	-1,  1, -1,  0, -1,  2, -1,  3, -1,  0,
	 1,  9,  9, -1, -1,  1,  8, -1, -1, 12,
	-1, -1, -1, -1, 12,  4, -1, -1,  5,  2,
	 2, 10, -1, -1, -1, -1, 10,  1,  9,  9,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	// Second array
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, 11, 11,  3, 12, -1, -1, -1, -1, 12,
	 4,  4,  7, -1, -1,  2, 10, -1, -1, -1,
	-1, 10, -1, -1,  6,  3, -1, -1, 11, 11,
	 3,
};

static const int nBorderIndicesOffset_sub_6FD80BE0 = 4;
static const int nBorderIndicesOffset_sub_6FD80C10 = 50;

static const D2C_LvlPrestIds levelPrestBorder[13][4] = {
	// In the game we have invalid data in row 0, probably resulting from optimization?
	// Need to find out if we can actually have index = 0 in those functions or not
	D2C_LvlPrestIds(0x2010203), D2C_LvlPrestIds(0x1020300), D2C_LvlPrestIds(0x0FF0001), D2C_LvlPrestIds(0x0FF000100),
	LVLPREST_NONE,						LVLPREST_ACT1_WILD_BORDER_1,	LVLPREST_ACT2_DESERT_BORDER_1,	LVLPREST_ACT4_MESA_BORDER_1,
	LVLPREST_ACT1_WILD_CLIFF_BORDER_2,	LVLPREST_ACT1_WILD_BORDER_2,	LVLPREST_ACT2_DESERT_BORDER_2,	LVLPREST_ACT4_MESA_BORDER_2,
	LVLPREST_ACT1_WILD_CLIFF_BORDER_3,	LVLPREST_ACT1_WILD_BORDER_3,	LVLPREST_ACT2_DESERT_BORDER_3,	LVLPREST_ACT4_MESA_BORDER_3,
	LVLPREST_NONE,						LVLPREST_ACT1_WILD_BORDER_4,	LVLPREST_ACT2_DESERT_BORDER_4,	LVLPREST_ACT4_MESA_BORDER_4,
	LVLPREST_ACT1_WILD_CLIFF_BORDER_5,	LVLPREST_ACT1_WILD_BORDER_5,	LVLPREST_ACT2_DESERT_BORDER_5,	LVLPREST_ACT4_MESA_BORDER_5,
	LVLPREST_ACT1_WILD_CLIFF_BORDER_6A,	LVLPREST_ACT1_WILD_BORDER_6,	LVLPREST_ACT2_DESERT_BORDER_6,	LVLPREST_ACT4_MESA_BORDER_6,
	LVLPREST_ACT1_WILD_CLIFF_BORDER_7,	LVLPREST_ACT1_WILD_BORDER_7,	LVLPREST_ACT2_DESERT_BORDER_7,	LVLPREST_ACT4_MESA_BORDER_7,
	LVLPREST_NONE,						LVLPREST_ACT1_WILD_BORDER_8,	LVLPREST_ACT2_DESERT_BORDER_8,	LVLPREST_ACT4_MESA_BORDER_8,
	LVLPREST_NONE,						LVLPREST_ACT1_WILD_BORDER_9,	LVLPREST_ACT2_DESERT_BORDER_9,	LVLPREST_ACT4_MESA_BORDER_9,
	LVLPREST_ACT1_WILD_CLIFF_BORDER_10,	LVLPREST_ACT1_WILD_BORDER_10,	LVLPREST_ACT2_DESERT_BORDER_10, LVLPREST_ACT4_MESA_BORDER_10,
	LVLPREST_NONE,						LVLPREST_ACT1_WILD_BORDER_11,	LVLPREST_ACT2_DESERT_BORDER_11, LVLPREST_ACT4_MESA_BORDER_11,
	LVLPREST_NONE,						LVLPREST_ACT1_WILD_BORDER_12,	LVLPREST_ACT2_DESERT_BORDER_12, LVLPREST_ACT4_MESA_BORDER_12,
};

//D2Common.0x6FD80BE0
int __fastcall sub_6FD80BE0(int a1, int a2, int a3)
{
	const int lutIndex = a1 + 3 * a2 + 4;
	assert(lutIndex >= 0 && lutIndex <= ARRAY_SIZE(nBorderIndices));
	int nIndex = nBorderIndices[a1 + 3 * a2 + nBorderIndicesOffset_sub_6FD80BE0];

	if (a3 < 4)
	{
		return levelPrestBorder[nIndex+1][a3];
	}
	else
	{
		return gnBarricadeCliffBorderIds[nIndex][a3 - 4];
	}
}

//D2Common.0x6FD80C10
int __fastcall sub_6FD80C10(int a1, int a2, int a3, int a4, int a5)
{
	int v6 = 0;

	if (a1 >= 0)
	{
		if (a1 > 0)
		{
			a1 += 2;
		}
	}
	else
	{
		a1 -= 2;
	}

	if (a3 >= 0)
	{
		if (a3 > 0)
		{
			a3 += 2;
		}
	}
	else
	{
		a3 -= 2;
	}
	const int lutIndex = a2 + a1 + 9 * (a4 + a3) + nBorderIndicesOffset_sub_6FD80C10;
	assert(lutIndex >= 0 && lutIndex <= ARRAY_SIZE(nBorderIndices));

	v6 = nBorderIndices[lutIndex];

	if (v6 == -1)
	{
		return 0;
	}
	else if (a5 < 4)
	{
		return levelPrestBorder[v6][a5];
	}
	else
	{
		return gnBarricadeCliffBorderIds[v6 - 1][a5 - 4];
	}
}

//D2Common.0x6FD80C80
void __fastcall DRLGOUTPLACE_SetBlankBorderGridCells(D2DrlgLevelStrc* pLevel)
{
	D2DrlgGridStrc* pDrlgGrid = NULL;
	int nOffsetX = 0;
	int nOffsetY = 0;
	int nX = 0;
	int nY = 0;
	int nCoordinates[] = { 0, 0, 1, 1, 1, 0, -1, 1, 0, 1, 1, -1, 1, 1, -1, -1 };

	pDrlgGrid = &pLevel->pOutdoors->pGrid[2];

	for (int k = 0; k < 4; ++k)
	{
		nX = nCoordinates[4 * k + 0];
		if (nX)
		{
			nX = pLevel->pOutdoors->nGridWidth - 1;
		}

		nY = nCoordinates[4 * k + 1];
		if (nY)
		{
			nY = pLevel->pOutdoors->nGridHeight - 1;
		}

		nOffsetX = nCoordinates[4 * k + 2];
		nOffsetY = nCoordinates[4 * k + 3];

		for (int j = nY; !(DRLGGRID_GetGridEntry(pDrlgGrid, nX, j) & 1); j += nOffsetY)
		{
			for (int i = nX; !(DRLGGRID_GetGridEntry(pDrlgGrid, i, j) & 1); i += nOffsetX)
			{
				DRLGGRID_AlterGridFlag(pDrlgGrid, i, j, 256, FLAG_OPERATION_OR);
			}
		}
	}
}

//D2Common.0x6FD80DA0
void __fastcall DRLGOUTPLACE_SetOutGridLinkFlags(D2DrlgLevelStrc* pLevel)
{
	D2DrlgVertexStrc* pVertex = pLevel->pOutdoors->pVertex;
	D2DrlgVertexStrc* pNext = pVertex->pNext;

	do
	{
		if (pVertex->dwFlags & 1)
		{
			sub_6FD75DE0(&pLevel->pOutdoors->pGrid[1], pVertex, DRLGOUTDOORS_GetOutLinkVisFlag(pLevel, pVertex), FLAG_OPERATION_OR, 1);
			sub_6FD75DE0(&pLevel->pOutdoors->pGrid[2], pVertex, 2 * (pVertex->nDirection != 0) + 1, FLAG_OPERATION_OR, 1);
		}

		pVertex = pNext;
		pNext = pVertex->pNext;
	}
	while (pVertex != pLevel->pOutdoors->pVertex);
}

//D2Common.0x6FD80E10
//TODO: v41, v61
void __fastcall DRLGOUTPLACE_PlaceAct1245OutdoorBorders(D2DrlgLevelStrc* pLevel)
{
	static const int nDesertBorderIds[5][2] =
	{
		{ LVLPREST_ACT2_DESERT_BORDER_10,	LVLPREST_ACT2_DESERT_BORDER_9 },
		{ LVLPREST_ACT2_DESERT_BORDER_9,	LVLPREST_ACT2_DESERT_BORDER_12 },
		{ LVLPREST_NONE,					LVLPREST_NONE },
		{ LVLPREST_ACT2_DESERT_BORDER_10,	LVLPREST_ACT2_DESERT_BORDER_11 },
		{ LVLPREST_ACT2_DESERT_BORDER_11,	LVLPREST_ACT2_DESERT_BORDER_12 },
	};

	D2DrlgVertexStrc* pDrlgVertex = NULL;
	D2DrlgVertexStrc* pNextVertex = NULL;
	int nCurrentDiffXAbs = 0;
	int nCurrentDiffYAbs = 0;
	int nCurrentDiffX = 0;
	int nCurrentDiffY = 0;
	int nLevelPrestId = 0;
	int nNextDiffX = 0;
	int nNextDiffY = 0;
	int nCurrentX = 0;
	int nCurrentY = 0;
	int nNextX = 0;
	int nNextY = 0;
	int nIndex = 0;
	int nDiff = 0;
	int nX = 0;
	int nY = 0;
	int v41 = 0;
	int v61 = 0;
	uint8_t nDirection = 0;
	uint8_t nAct = 0;

	nAct = DRLG_GetActNoFromLevelId(pLevel->nLevelId);
	pDrlgVertex = pLevel->pOutdoors->pVertex;
	pNextVertex = pDrlgVertex->pNext;
	do
	{
		DRLGVER_GetCoordDiff(pDrlgVertex, &nCurrentDiffX, &nCurrentDiffY);
		DRLGVER_GetCoordDiff(pNextVertex, &nNextDiffX, &nNextDiffY);

		if (nCurrentDiffX < 0)
		{
			nCurrentDiffXAbs = -nCurrentDiffX;
		}
		else
		{
			nCurrentDiffXAbs = nCurrentDiffX;
		}

		if (nCurrentDiffY < 0)
		{
			nCurrentDiffYAbs = -nCurrentDiffY;
		}
		else
		{
			nCurrentDiffYAbs = nCurrentDiffY;
		}

		nCurrentX = pDrlgVertex->nPosX;
		nCurrentY = pDrlgVertex->nPosY;
		nNextX = pNextVertex->nPosX;
		nNextY = pNextVertex->nPosY;
		if (nCurrentDiffXAbs)
		{
			nDiff = nCurrentX - nNextX;
		}
		else
		{
			nDiff = nCurrentY - nNextY;
		}

		if (nDiff < 0)
		{
			nDiff = -nDiff;
		}

		v61 = 2 * (pDrlgVertex->nDirection != 0) + 1;

		switch (pLevel->nLevelType)
		{
		case LVLTYPE_ACT1_WILDERNESS:
			nLevelPrestId = sub_6FD80BE0(nCurrentDiffX, nCurrentDiffY, pDrlgVertex->nDirection == 0);
			break;

		case LVLTYPE_ACT2_DESERT:
			nLevelPrestId = sub_6FD80BE0(nCurrentDiffX, nCurrentDiffY, 2);
			break;

		case LVLTYPE_ACT4_MESA:
			nLevelPrestId = sub_6FD80BE0(nCurrentDiffX, nCurrentDiffY, 3);
			break;

		case LVLTYPE_ACT5_BARRICADE:
			nLevelPrestId = sub_6FD80BE0(nCurrentDiffX, nCurrentDiffY, sub_6FD84100(pLevel));
			break;

		default:
			nLevelPrestId = sub_6FD80BE0(nCurrentDiffX, nCurrentDiffY, -1);
			break;
		}

		if (!(pDrlgVertex->dwFlags & 2))
		{
			while (nCurrentX != nNextX || nCurrentY != nNextY)
			{
				nCurrentX += nCurrentDiffX;
				nCurrentY += nCurrentDiffY;
				DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nCurrentX, nCurrentY, nLevelPrestId, -1, 0);
				DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nCurrentX, nCurrentY, v61, FLAG_OPERATION_OR);
			}
		}

		if (pDrlgVertex->dwFlags & 1 && !(pDrlgVertex->dwFlags & 2))
		{
			switch (nAct)
			{
			case ACT_I:
			case ACT_V:
				if (pDrlgVertex->nPosX < pNextVertex->nPosX)
				{
					nX = pDrlgVertex->nPosX;
				}
				else
				{
					nX = pNextVertex->nPosX;
				}
				nX += nCurrentDiffXAbs * nDiff / 2;

				if (pDrlgVertex->nPosY < pNextVertex->nPosY)
				{
					nY = pDrlgVertex->nPosY;
				}
				else
				{
					nY = pNextVertex->nPosY;
				}
				nY += nCurrentDiffYAbs * nDiff / 2;

				if (pLevel->nLevelId == LEVEL_BURIALGROUNDS)
				{
					DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nX, nY, 0xF0000, FLAG_OPERATION_AND_NEGATED);
					DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nX, nY, 0x40400, FLAG_OPERATION_OR);
				}
				else
				{
					DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nX, nY, 0xF0000, FLAG_OPERATION_AND_NEGATED);
					DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nX, nY, 0x30400, FLAG_OPERATION_OR);
				}
				break;

			case ACT_IV:
				if (pDrlgVertex->nPosX < pNextVertex->nPosX)
				{
					nX = pDrlgVertex->nPosX;
				}
				else
				{
					nX = pNextVertex->nPosX;
				}
				nX += nCurrentDiffXAbs * nDiff / 2;

				if (pDrlgVertex->nPosY < pNextVertex->nPosY)
				{
					nY = pDrlgVertex->nPosY;
				}
				else
				{
					nY = pNextVertex->nPosY;
				}
				nY += nCurrentDiffYAbs * nDiff / 2;

				DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nX, nY, 0xF0000, FLAG_OPERATION_AND_NEGATED);
				DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nX, nY, 0x30400, FLAG_OPERATION_OR);
				break;

			case ACT_II:
				if (pDrlgVertex->nPosX < pNextVertex->nPosX)
				{
					nX = pDrlgVertex->nPosX;
				}
				else
				{
					nX = pNextVertex->nPosX;
				}
				nX += nCurrentDiffXAbs * nDiff / 2;

				if (pDrlgVertex->nPosY < pNextVertex->nPosY)
				{
					nY = pDrlgVertex->nPosY;
				}
				else
				{
					nY = pNextVertex->nPosY;
				}
				nY += nCurrentDiffYAbs * nDiff / 2;

				nIndex = nCurrentDiffX + 2 * nCurrentDiffY + 2;
				DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nX, nY, nDesertBorderIds[nIndex][0], -1, 0);
				DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nX + nCurrentDiffXAbs, nY + nCurrentDiffYAbs, nDesertBorderIds[nIndex][1], -1, 0);
				break;

			default:
				break;
			}
		}

		nDirection = pDrlgVertex->nDirection;
		if (nDirection)
		{
			v61 |= 2;
		}
		else
		{
			nDirection = pNextVertex->nDirection;
			if (nDirection)
			{
				v61 |= 2;
			}
		}

		switch (pLevel->nLevelType)
		{
		case LVLTYPE_ACT1_WILDERNESS:
			v41 = nDirection == 0;
			break;

		case LVLTYPE_ACT2_DESERT:
			v41 = 2;
			break;

		case LVLTYPE_ACT4_MESA:
			v41 = 3;
			break;

		case LVLTYPE_ACT5_BARRICADE:
			v41 = sub_6FD84100(pLevel);
			break;

		default:
			v41 = -1;
			break;
		}

		if (pDrlgVertex->dwFlags & 2)
		{
			if (pNextVertex->dwFlags & 2)
			{
				nLevelPrestId = sub_6FD80C10(nCurrentDiffX, nCurrentDiffY, nNextDiffX, nNextDiffY, v41);
			}
			else
			{
				nLevelPrestId = sub_6FD80C10(nCurrentDiffX, nCurrentDiffY, 2 * nNextDiffX, 2 * nNextDiffY, v41);
			}
		}
		else
		{
			if (pNextVertex->dwFlags & 2)
			{
				nLevelPrestId = sub_6FD80C10(2 * nCurrentDiffX, 2 * nCurrentDiffY, nNextDiffX, nNextDiffY, v41);
			}
			else
			{
				nLevelPrestId = sub_6FD80C10(2 * nCurrentDiffX, 2 * nCurrentDiffY, 2 * nNextDiffX, 2 * nNextDiffY, v41);
			}
		}

		if (nLevelPrestId == LVLPREST_ACT1_WILD_CLIFF_BORDER_6A)
		{
			if (pDrlgVertex->nDirection == 1)
			{
				if (pNextVertex->nDirection != 1)
				{
					nLevelPrestId = LVLPREST_ACT1_WILD_CLIFF_BORDER_6B;
				}
			}
			else
			{
				nLevelPrestId = LVLPREST_ACT1_WILD_CLIFF_BORDER_6C;
			}

			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nNextX, nNextY, nLevelPrestId, -1, 0);
			DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nNextX, nNextY, v61, FLAG_OPERATION_OR);
		}
		else if (nLevelPrestId != LVLPREST_NONE)
		{
			DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(pLevel, nNextX, nNextY, nLevelPrestId, -1, 0);
			DRLGGRID_AlterGridFlag(&pLevel->pOutdoors->pGrid[2], nNextX, nNextY, v61, FLAG_OPERATION_OR);
		}

		pDrlgVertex = pNextVertex;
		pNextVertex = pNextVertex->pNext;
	}
	while (pDrlgVertex != pLevel->pOutdoors->pVertex);

	DRLGOUTPLACE_SetBlankBorderGridCells(pLevel);
}

//D2Common.0x6FD81330
BOOL __fastcall sub_6FD81330(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	D2LevelDefBin* pLevelDefBinRecord = NULL;

	if (pLevelLinkData->nRand[1][pLevelLinkData->nIteration] == -1)
	{
		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = -1;
	}

	pLevelDefBinRecord = DATATBLS_GetLevelDefRecord(pLevelLinkData->nCurrentLevel);
	pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nPosX = pLevelDefBinRecord->dwOffsetX;
	pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nPosY = pLevelDefBinRecord->dwOffsetY;

	return TRUE;
}

//D2Common.0x6FD81380
BOOL __fastcall sub_6FD81380(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	if (pLevelLinkData->nRand[1][pLevelLinkData->nIteration] == -1)
	{
		pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 3;
		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = pLevelLinkData->nRand[1][pLevelLinkData->nIteration];
	}
	else
	{
		if (((pLevelLinkData->nRand[0][pLevelLinkData->nIteration] + 1) % 4) == pLevelLinkData->nRand[1][pLevelLinkData->nIteration])
		{
			return FALSE;
		}

		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = (pLevelLinkData->nRand[0][pLevelLinkData->nIteration] + 1) % 4;
	}

	sub_6FD81430(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 1);

	return TRUE;
}

//D2Common.0x6FD81430
void __fastcall sub_6FD81430(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int a3, int a4)
{
	switch (a3)
	{
	case 0:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY + pDrlgCoord1->nHeight;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosX -= 16;
		}
		break;

	case 1:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX - pDrlgCoord2->nWidth;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosY -= 16;
		}
		else if (a4 == 2)
		{
			pDrlgCoord2->nPosY += 8;
		}
		break;

	case 2:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX + pDrlgCoord1->nWidth - pDrlgCoord2->nWidth;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY - pDrlgCoord2->nHeight;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosX += 16;
		}
		break;

	case 3:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX + pDrlgCoord1->nWidth;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY + pDrlgCoord1->nHeight - pDrlgCoord2->nHeight;

		switch (a4)
		{
		case 1:
			pDrlgCoord2->nPosY += 16;
			break;

		case 2:
			pDrlgCoord2->nPosY -= 8;
			break;

		case 3:
			pDrlgCoord2->nPosY += 8;
			break;
		}
		break;

	default:
		return;
	}
}

//D2Common.0x6FD81530
BOOL __fastcall sub_6FD81530(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	if (pLevelLinkData->nRand[1][pLevelLinkData->nIteration] == -1)
	{
		pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 7;
		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = pLevelLinkData->nRand[1][pLevelLinkData->nIteration];
	}
	else
	{
		if (((pLevelLinkData->nRand[0][pLevelLinkData->nIteration] + 1) % 8) == pLevelLinkData->nRand[1][pLevelLinkData->nIteration])
		{
			return FALSE;
		}
		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = ((pLevelLinkData->nRand[0][pLevelLinkData->nIteration] + 1) % 8);
	}

	sub_6FD815E0(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 1);

	return TRUE;
}

//D2Common.0x6FD815E0
void __fastcall sub_6FD815E0(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int a3, int a4)
{
	switch (a3)
	{
	case 0:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY + pDrlgCoord1->nHeight;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosX -= pDrlgCoord2->nWidth / 2 + 8;
		}
		break;

	case 1:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY + pDrlgCoord1->nHeight;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosX += pDrlgCoord2->nWidth / 2 + 8;
		}
		break;

	case 2:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX - pDrlgCoord2->nWidth;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosY -= pDrlgCoord2->nHeight / 2 + 8;
		}
		break;

	case 3:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX - pDrlgCoord2->nWidth;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosY += pDrlgCoord2->nHeight / 2 + 8;
		}
		break;

	case 4:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY - pDrlgCoord2->nHeight;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosX -= pDrlgCoord2->nWidth / 2 + 8;
		}
		break;

	case 5:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY - pDrlgCoord2->nHeight;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosX += pDrlgCoord2->nWidth / 2 + 8;
		}
		break;

	case 6:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX + pDrlgCoord1->nWidth;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosY -= pDrlgCoord2->nHeight / 2 + 8;
		}
		break;

	case 7:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX + pDrlgCoord1->nWidth;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosY += pDrlgCoord2->nHeight / 2 + 8;
		}
		break;

	default:
		return;
	}
}

//D2Common.0x6FD81720
BOOL __fastcall sub_6FD81720(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	int nRand2 = 0;
	int nRand0 = 0;

	if (pLevelLinkData->nRand[1][pLevelLinkData->nIteration] == -1)
	{
		pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 3;
		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = pLevelLinkData->nRand[1][pLevelLinkData->nIteration];
		pLevelLinkData->nRand[3][pLevelLinkData->nIteration] = SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 1;

		nRand2 = pLevelLinkData->nRand[3][pLevelLinkData->nIteration];
	}
	else
	{
		nRand0 = (pLevelLinkData->nRand[2][pLevelLinkData->nIteration] + pLevelLinkData->nRand[0][pLevelLinkData->nIteration]) % 4;
		nRand2 = (pLevelLinkData->nRand[2][pLevelLinkData->nIteration] + 1) % 2;

		if (nRand0 == pLevelLinkData->nRand[1][pLevelLinkData->nIteration] && nRand2 == pLevelLinkData->nRand[3][pLevelLinkData->nIteration])
		{
			return FALSE;
		}

		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = nRand0;
	}
	pLevelLinkData->nRand[2][pLevelLinkData->nIteration] = nRand2;

	if (pLevelLinkData->nRand[2][pLevelLinkData->nIteration] == 1)
	{
		sub_6FD81430(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 2);
	}
	else
	{
		sub_6FD81850(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 2);
	}

	return TRUE;
}

//D2Common.0x6FD81850
void __fastcall sub_6FD81850(D2DrlgCoordStrc* pDrlgCoord1, D2DrlgCoordStrc* pDrlgCoord2, int a3, int a4)
{
	switch (a3)
	{
	case 0:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX + pDrlgCoord1->nWidth - pDrlgCoord2->nWidth;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY + pDrlgCoord1->nHeight;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosX += 16;
		}
		break;

	case 1:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX - pDrlgCoord2->nWidth;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY + pDrlgCoord1->nHeight - pDrlgCoord2->nHeight;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosY += 16;
		}
		else if (a4 == 2)
		{
			pDrlgCoord2->nPosY -= 8;
		}
		break;

	case 2:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY - pDrlgCoord2->nHeight;

		if (a4 == 1)
		{
			pDrlgCoord2->nPosX -= 16;
		}
		break;

	case 3:
		pDrlgCoord2->nPosX = pDrlgCoord1->nPosX + pDrlgCoord1->nWidth;
		pDrlgCoord2->nPosY = pDrlgCoord1->nPosY;

		switch (a4)
		{
		case 1:
			pDrlgCoord2->nPosY -= 16;
			break;

		case 2:
			pDrlgCoord2->nPosY += 8;
			break;

		case 3:
			pDrlgCoord2->nPosY -= 8;
			break;
		}
		break;

	default:
		return;
	}
}

//D2Common.0x6FD81950
BOOL __fastcall sub_6FD81950(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	int nRand2 = 0;
	int nRand0 = 0;

	if (pLevelLinkData->nRand[1][pLevelLinkData->nIteration] == -1)
	{
		pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 3;
		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = pLevelLinkData->nRand[1][pLevelLinkData->nIteration];
		pLevelLinkData->nRand[3][pLevelLinkData->nIteration] = SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 1;

		nRand2 = pLevelLinkData->nRand[3][pLevelLinkData->nIteration];
	}
	else
	{
		nRand0 = (pLevelLinkData->nRand[2][pLevelLinkData->nIteration] + pLevelLinkData->nRand[0][pLevelLinkData->nIteration]) % 4;
		nRand2 = (pLevelLinkData->nRand[2][pLevelLinkData->nIteration] + 1) % 2;

		if (nRand0 == pLevelLinkData->nRand[1][pLevelLinkData->nIteration] && nRand2 == pLevelLinkData->nRand[3][pLevelLinkData->nIteration])
		{
			return FALSE;
		}

		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = nRand0;
	}
	pLevelLinkData->nRand[2][pLevelLinkData->nIteration] = nRand2;

	pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nWidth = pLevelLinkData->nRand[0][pLevelLinkData->nIteration] % 2 != 0 ? 96 : 56;
	pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration].nHeight = pLevelLinkData->nRand[0][pLevelLinkData->nIteration] % 2 != 0 ? 56 : 96;

	if (pLevelLinkData->nRand[2][pLevelLinkData->nIteration] == 1)
	{
		sub_6FD81430(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 1);
	}
	else
	{
		sub_6FD81850(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 1);
	}

	return TRUE;
}

//D2Common.0x6FD81AD0
BOOL __fastcall sub_6FD81AD0(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = 0;
	pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = pLevelLinkData->nRand[1][pLevelLinkData->nIteration];

	sub_6FD81430(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 0);

	return TRUE;
}

//D2Common.0x6FD81B30
BOOL __fastcall sub_6FD81B30(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	int nRand = 0;

	if (pLevelLinkData->nRand[1][pLevelLinkData->nIteration] == -1)
	{
		pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = (SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 1) + 1;
		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = pLevelLinkData->nRand[1][pLevelLinkData->nIteration];
	}
	else
	{
		nRand = 2 - (pLevelLinkData->nRand[0][pLevelLinkData->nIteration] != 1);

		if (nRand == pLevelLinkData->nRand[1][pLevelLinkData->nIteration])
		{
			return FALSE;
		}

		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = nRand;
	}

	if (pLevelLinkData->nRand[0][pLevelLinkData->nIteration] == 1)
	{
		sub_6FD81430(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], 1, 0);
	}
	else
	{
		sub_6FD81850(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 0);
	}

	return TRUE;
}

//D2Common.0x6FD81BF0
BOOL __fastcall sub_6FD81BF0(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	int nRand = 0;

	if (pLevelLinkData->nRand[1][pLevelLinkData->nIteration] == -1)
	{
		pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 7;
		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = pLevelLinkData->nRand[1][pLevelLinkData->nIteration];
	}
	else
	{
		nRand = (pLevelLinkData->nRand[0][pLevelLinkData->nIteration] + 1) % 8;

		if (nRand == pLevelLinkData->nRand[1][pLevelLinkData->nIteration])
		{
			return 0;
		}

		pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = nRand;
	}

	sub_6FD815E0(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 0);

	return TRUE;
}

//D2Common.0x6FD81CA0
BOOL __fastcall sub_6FD81CA0(D2DrlgLevelLinkDataStrc* pLevelLinkData)
{
	pLevelLinkData->nRand[1][pLevelLinkData->nIteration] = 3;
	pLevelLinkData->nRand[0][pLevelLinkData->nIteration] = pLevelLinkData->nRand[1][pLevelLinkData->nIteration];

	if (!(SEED_RollRandomNumber(&pLevelLinkData->pSeed) & 1))
	{
		sub_6FD81850(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 3);
		dword_6FDEA6FC = 0x400000;
	}
	else
	{
		sub_6FD81430(&pLevelLinkData->pLevelCoord[pLevelLinkData->pLink[pLevelLinkData->nIteration].nLevelLink], &pLevelLinkData->pLevelCoord[pLevelLinkData->nIteration], pLevelLinkData->nRand[0][pLevelLinkData->nIteration], 3);
		dword_6FDEA6FC = 0x800000;
	}

	return TRUE;
}

//D2Common.0x6FD81D60
void __fastcall DRLGOUTPLACE_CreateLevelConnections(D2DrlgStrc* pDrlg, uint8_t nAct)
{
	D2LevelDefBin* pLevelDefBinRecord = NULL;
	D2DrlgLevelStrc* pAdjacentLevel = NULL;
	D2DrlgLevelStrc* pLevel = NULL;
	int nSizeX = 0;
	int nSizeY = 0;
	int nPosY = 0;

	switch (nAct)
	{
	case ACT_I:
		sub_6FD823C0(pDrlg, gAct1WildernessDrlgLink, sub_6FD82050, sub_6FD82360);
		sub_6FD823C0(pDrlg, gAct1MonasteryDrlgLink, sub_6FD82130, sub_6FD82360);

		sub_6FD82750(pDrlg, LEVEL_ROGUEENCAMPMENT, LEVEL_BURIALGROUNDS);
		break;

	case ACT_II:
		sub_6FD823C0(pDrlg, gAct2OutdoorDrlgLink, DRLGOUTPLACE_LinkAct2Outdoors, 0);
		sub_6FD823C0(pDrlg, gAct2CanyonDrlgLink, DRLGOUTPLACE_LinkAct2Canyon, 0);

		sub_6FD82750(pDrlg, LEVEL_LUTGHOLEIN, LEVEL_CANYONOFTHEMAGI);
		break;

	case ACT_III:
		pLevel = DRLG_GetLevel(pDrlg, LEVEL_KURASTDOCKTOWN);
		pLevelDefBinRecord = DATATBLS_GetLevelDefRecord(LEVEL_KURASTDOCKTOWN);

		pLevel->nPosX = pLevelDefBinRecord->dwOffsetX;
		pLevel->nPosY = pLevelDefBinRecord->dwOffsetY;
		pLevel->nWidth = pLevelDefBinRecord->dwSizeX[pDrlg->nDifficulty];
		pLevel->nHeight = pLevelDefBinRecord->dwSizeY[pDrlg->nDifficulty];

		pLevel = DRLG_GenerateJungles(pLevel);

		nPosY = 0;
		for (int i = LEVEL_LOWERKURAST; i <= LEVEL_TRAVINCAL; ++i)
		{
			pLevelDefBinRecord = DATATBLS_GetLevelDefRecord(i);

			nSizeX = pLevelDefBinRecord->dwSizeX[pLevel->pDrlg->nDifficulty];
			nSizeY = pLevelDefBinRecord->dwSizeY[pLevel->pDrlg->nDifficulty];

			nPosY -= nSizeY;

			pAdjacentLevel = DRLG_GetLevel(pLevel->pDrlg, i);

			pAdjacentLevel->nPosX = pLevel->nWidth / 2 + pLevel->nPosX - nSizeX / 2;
			pAdjacentLevel->nWidth = nSizeX;
			pAdjacentLevel->nPosY = nPosY + pLevel->nPosY;
			pAdjacentLevel->nHeight = nSizeY;
		}

		sub_6FD826D0(pDrlg, LEVEL_KURASTDOCKTOWN, LEVEL_TRAVINCAL);
		sub_6FD82750(pDrlg, LEVEL_KURASTDOCKTOWN, LEVEL_TRAVINCAL);
		break;

	case ACT_IV:
		sub_6FD823C0(pDrlg, gAct4OutdoorDrlgLink, DRLGOUTPLACE_LinkAct4Outdoors, NULL);
		sub_6FD823C0(pDrlg, gAct4ChaosSanctumDrlgLink, DRLGOUTPLACE_LinkAct4ChaosSanctum, NULL);

		pLevel = DRLG_GetLevel(pDrlg, LEVEL_OUTERSTEPPES);
		pLevel->pOutdoors->dwFlags |= dword_6FDEA6FC;

		sub_6FD82750(pDrlg, LEVEL_THEPANDEMONIUMFORTRESS, LEVEL_CITYOFTHEDAMNED);
		break;

	case ACT_V:
		sub_6FD823C0(pDrlg, gAct5OutdoorDrlgLink, NULL, NULL);
		sub_6FD823C0(pDrlg, gAct5TundraDrlgLink, NULL, NULL);

		sub_6FD826D0(pDrlg, LEVEL_ID_ACT5_BARRICADE_1, LEVEL_ARREATPLATEAU);
		sub_6FD82750(pDrlg, LEVEL_ID_ACT5_BARRICADE_1, LEVEL_ARREATPLATEAU);

		sub_6FD826D0(pDrlg, LEVEL_BLOODYFOOTHILLS, LEVEL_ID_ACT5_BARRICADE_1);

		sub_6FD826D0(pDrlg, LEVEL_HARROGATH, LEVEL_BLOODYFOOTHILLS);
		break;

	default:
		return;
	}
}

//D2Common.0x6FD82050
//TODO: Rename variables
BOOL __fastcall sub_6FD82050(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration)
{
	static const BOOL dword_6FDD05C0[] =
	{
		1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1,
	};

	int nLevelLink = gAct1WildernessDrlgLink[nIteration].nLevelLink;

	for (int i = 0; i < nIteration; ++i)
	{
		if (i != nLevelLink)
		{
			if (!sub_6FD777B0(&pLevelLinkData->pLevelCoord[nIteration], &pLevelLinkData->pLevelCoord[i], 0))
			{
				return FALSE;
			}
		}
	}

	if (gAct1WildernessDrlgLink[nIteration].nLevel != LEVEL_ROGUEENCAMPMENT)
	{
		if (gAct1WildernessDrlgLink[nIteration].nLevel == LEVEL_BURIALGROUNDS)
		{
			for (int i = 0; i < 15; ++i)
			{
				if (i != nIteration && gAct1WildernessDrlgLink[i].nLevelLink == nLevelLink && pLevelLinkData->nRand2[nIteration] == pLevelLinkData->nRand2[i])
				{
					return FALSE;
				}
			}
		}

		return TRUE;
	}
	else
	{
		return dword_6FDD05C0[pLevelLinkData->nRand[0][nIteration] + 4 * (pLevelLinkData->nRand[2][nIteration] + 2 * (pLevelLinkData->nRand[0][nLevelLink] + 4 * pLevelLinkData->nRand[2][nLevelLink]))];
	}
}

//D2Common.0x6FD82130
BOOL __fastcall sub_6FD82130(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration)
{
	BOOL bResult = FALSE;
	int nCounter = 0;

	nCounter = 0;

	while (nCounter < nIteration)
	{
		if (nCounter != gAct1MonasteryDrlgLink[nIteration].nLevelLink && !sub_6FD777B0(&pLevelLinkData->pLevelCoord[nIteration], &pLevelLinkData->pLevelCoord[nCounter], 0))
		{
			return FALSE;
		}

		++nCounter;
	}

	bResult = TRUE;
	if (nIteration)
	{
		pLevelLinkData->pLevelCoord[0].nHeight += 200;
		pLevelLinkData->pLevelCoord[0].nPosY -= 200;

		bResult = sub_6FD777B0(pLevelLinkData->pLevelCoord, &pLevelLinkData->pLevelCoord[nCounter], 0);

		pLevelLinkData->pLevelCoord[0].nHeight -= 200;
		pLevelLinkData->pLevelCoord[0].nPosY += 200;
	}

	return bResult;
}

//D2Common.0x6FD821E0
BOOL __fastcall DRLGOUTPLACE_LinkAct2Outdoors(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration)
{
	int nLevelLink = gAct2OutdoorDrlgLink[nIteration].nLevelLink;

	for (int i = 0; i < nIteration; ++i)
	{
		if (i != nLevelLink && !sub_6FD777B0(&pLevelLinkData->pLevelCoord[nIteration], &pLevelLinkData->pLevelCoord[i], 0))
		{
			return FALSE;
		}
	}

	return TRUE;
}

//D2Common.0x6FD82240
BOOL __fastcall DRLGOUTPLACE_LinkAct2Canyon(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration)
{
	int nLevelLink = gAct2CanyonDrlgLink[nIteration].nLevelLink;

	for (int i = 0; i < nIteration; ++i)
	{
		if (i != nLevelLink && !sub_6FD777B0(&pLevelLinkData->pLevelCoord[nIteration], &pLevelLinkData->pLevelCoord[i], 0))
		{
			return FALSE;
		}
	}

	return TRUE;
}

//D2Comon.0x6FD822A0
BOOL __fastcall DRLGOUTPLACE_LinkAct4Outdoors(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration)
{
	int nLevelLink = gAct4OutdoorDrlgLink[nIteration].nLevelLink;

	for (int i = 0; i < nIteration; ++i)
	{
		if (i != nLevelLink && !sub_6FD777B0(&pLevelLinkData->pLevelCoord[nIteration], &pLevelLinkData->pLevelCoord[i], 0))
		{
			return FALSE;
		}
	}

	return TRUE;
}

//D2Common.0x6FD82300
BOOL __fastcall DRLGOUTPLACE_LinkAct4ChaosSanctum(D2DrlgLevelLinkDataStrc* pLevelLinkData, int nIteration)
{
	int nLevelLink = gAct4ChaosSanctumDrlgLink[nIteration].nLevelLink;

	for (int i = 0; i < nIteration; ++i)
	{
		if (i != nLevelLink && !sub_6FD777B0(&pLevelLinkData->pLevelCoord[nIteration], &pLevelLinkData->pLevelCoord[i], 0))
		{
			return FALSE;
		}
	}

	return TRUE;
}

//D2Common.0x6FD82360
void __fastcall sub_6FD82360(D2DrlgLevelStrc* pLevel, int nIteration, int* pRand)
{
	static const D2UnkOutdoorStrc3 stru_6FDD06C0[15] =
	{
		{ 0, LEVEL_BLOODMOOR, LEVEL_COLDPLAINS, 1, 0, 0x04 },
		{ 0, LEVEL_BLOODMOOR, LEVEL_COLDPLAINS, 2, 3, 0x04 },
		{ 0, LEVEL_COLDPLAINS, LEVEL_BURIALGROUNDS, 2, 1, 0x08 },
		{ 0, LEVEL_COLDPLAINS, LEVEL_BURIALGROUNDS, 3, 0, 0x08 },
		{ 0, LEVEL_COLDPLAINS, LEVEL_BURIALGROUNDS, 1, 1, 0x10 },
		{ 0, LEVEL_COLDPLAINS, LEVEL_BURIALGROUNDS, 3, 3, 0x10 },
		{ LEVEL_BLOODMOOR, 0, 0, 0, 0, 0x08 },
		{ LEVEL_BLOODMOOR, 0, 0, 2, 2, 0x08 },
		{ LEVEL_BLOODMOOR, 0, 0, 3, 0, 0x08 },
		{ LEVEL_BLOODMOOR, 0, 0, 3, 2, 0x08 },
		{ LEVEL_BLOODMOOR, 0, 0, 0, 1, 0x400 },
		{ LEVEL_BLOODMOOR, 0, 0, 1, 1, 0x400 },
		{ LEVEL_BLOODMOOR, 0, 0, 2, 1, 0x200 },
		{ LEVEL_BLOODMOOR, 0, 0, 2, 2, 0x80 },
		{ LEVEL_BLOODMOOR, 0, 0, 3, 2, 0x100 },
	};

	if (pLevel->nDrlgType == DRLGTYPE_OUTDOOR)
	{
		for (int i = 0; i < 15; ++i)
		{
			if (pLevel->nLevelId == stru_6FDD06C0[i].nLevelId || !stru_6FDD06C0[i].nLevelId)
			{
				if (pLevel->nLevelId != stru_6FDD06C0[i].nExcludedLevel1 && pLevel->nLevelId != stru_6FDD06C0[i].nExcludedLevel2 && pRand[nIteration] == stru_6FDD06C0[i].nRand)
				{
					if (pRand[nIteration + 1] == stru_6FDD06C0[i].nNextRand)
					{
						pLevel->pOutdoors->dwFlags |= stru_6FDD06C0[i].nFlags;
					}
				}
			}
		}
	}
}

//D2Common.0x6FD823C0
//TODO: v28 stuff
void __fastcall sub_6FD823C0(D2DrlgStrc* pDrlg, D2DrlgLinkStrc* pDrlgLink, int(__fastcall* a3)(D2DrlgLevelLinkDataStrc*, int), void(__fastcall* a4)(D2DrlgLevelStrc*, int, int*))
{
	D2DrlgLevelLinkDataStrc pLevelLinkData = {};
	D2LevelDefBin* pLevelDefBinRecord = NULL;
	D2DrlgPresetInfoStrc* pPresetInfo = NULL;
	D2DrlgWarpStrc* pDrlgWarp1 = NULL;
	D2DrlgWarpStrc* pDrlgWarp2 = NULL;
	D2DrlgLevelStrc* pLevel = NULL;
	int nCounter = 0;
	int nVisEx = 0;
	int nVis = 0;
	signed __int64 v28; // qax@34

	for (int i = 0; i < 15; ++i)
	{
		pLevelLinkData.nRand[0][i] = -1;
		pLevelLinkData.nRand[1][i] = -1;
		pLevelLinkData.nRand[2][i] = -1;
		pLevelLinkData.nRand[3][i] = -1;
	}

	pLevelLinkData.pSeed.nLowSeed = pDrlg->pSeed.nLowSeed;
	pLevelLinkData.pSeed.nHighSeed = pDrlg->pSeed.nHighSeed;
	pLevelLinkData.pLink = pDrlgLink;

	for (int i = 0; i < 15 && pDrlgLink[i].nLevel; ++i)
	{
		pLevelDefBinRecord = DATATBLS_GetLevelDefRecord(pDrlgLink[i].nLevel);

		pLevelLinkData.pLevelCoord[i].nWidth = pLevelDefBinRecord->dwSizeX[pDrlg->nDifficulty];
		pLevelLinkData.pLevelCoord[i].nHeight = pLevelDefBinRecord->dwSizeY[pDrlg->nDifficulty];
	}

	nCounter = 0;
	while (pDrlgLink[nCounter].nLevel)
	{
		pLevelLinkData.nIteration = nCounter;
		pLevelLinkData.nCurrentLevel = pDrlgLink[nCounter].nLevel;

		if (((BOOL(__fastcall*)(D2DrlgLevelLinkDataStrc*))pDrlgLink[nCounter].pfLinker)(&pLevelLinkData))
		{
			if (!a3 || a3(&pLevelLinkData, nCounter))
			{
				++nCounter;
			}
		}
		else
		{
			pLevelLinkData.nRand[0][nCounter] = -1;
			pLevelLinkData.nRand[1][nCounter] = -1;
			pLevelLinkData.nRand[2][nCounter] = -1;
			pLevelLinkData.nRand[3][nCounter] = -1;

			--nCounter;
		}
	}

	for (int i = 0; i < 15 && pDrlgLink[i].nLevel; ++i)
	{
		if (pDrlgLink[i].nLevelLink != -1)
		{
			nVis = pDrlgLink[pDrlgLink[i].nLevelLink].nLevel;
		}
		else
		{
			nVis = 0;
		}

		if (pDrlgLink[i].nLevelLinkEx != -1)
		{
			nVisEx = pDrlgLink[pDrlgLink[i].nLevelLinkEx].nLevel;
		}
		else
		{
			nVisEx = 0;
		}

		pLevel = DRLG_GetLevel(pDrlg, pDrlgLink[i].nLevel);

		pLevel->pLevelCoords.nPosX = pLevelLinkData.pLevelCoord[i].nPosX;
		pLevel->pLevelCoords.nPosY = pLevelLinkData.pLevelCoord[i].nPosY;
		pLevel->pLevelCoords.nWidth = pLevelLinkData.pLevelCoord[i].nWidth;
		pLevel->pLevelCoords.nHeight = pLevelLinkData.pLevelCoord[i].nHeight;

		if (pLevel->nDrlgType == DRLGTYPE_PRESET)
		{
			if (pLevel->nLevelId == LEVEL_ROGUEENCAMPMENT)
			{
				pLevel->pPreset->nDirection = pLevelLinkData.nRand[0][i];
			}
			else if (pLevel->nLevelId == LEVEL_LUTGHOLEIN)
			{
				pLevel->pPreset->nDirection = pLevelLinkData.nRand[0][i + 1];
			}
		}

		if (pLevel->nLevelId == LEVEL_BLACKMARSH)
		{
			pPresetInfo = DRLG_GetLevel(pDrlg, LEVEL_OUTERCLOISTER)->pPreset;
			if (pLevelLinkData.nRand[0][i] == 1)
			{
				v28 = pDrlg->pSeed.nHighSeed + 1791398085i64 * pDrlg->pSeed.nLowSeed;
				pDrlg->pSeed.lSeed = v28;
				pPresetInfo->nDirection = 2 - ((v28 & 1) != 0);
			}
			else if (pLevelLinkData.nRand[0][i] == 3)
			{
				v28 = 1791398085i64 * pDrlg->pSeed.nLowSeed + pDrlg->pSeed.nHighSeed;
				pDrlg->pSeed.lSeed = v28;
				pPresetInfo->nDirection = ~(uint8_t)v28 & 1;
			}
		}

		if (a4)
		{
			a4(pLevel, i, pLevelLinkData.nRand2);
		}

		if (pDrlg->nAct != ACT_V)
		{
			if (nVis)
			{
				pDrlgWarp1 = DRLG_GetDrlgWarpFromLevelId(pDrlg, pDrlgLink[i].nLevel);
				pDrlgWarp2 = DRLG_GetDrlgWarpFromLevelId(pDrlg, nVis);
				DRLG_SetWarpId(pDrlgWarp1, nVis, -1, -1);
				DRLG_SetWarpId(pDrlgWarp2, pDrlgLink[i].nLevel, -1, -1);
			}

			if (nVisEx)
			{
				pDrlgWarp1 = DRLG_GetDrlgWarpFromLevelId(pDrlg, pDrlgLink[i].nLevel);
				pDrlgWarp2 = DRLG_GetDrlgWarpFromLevelId(pDrlg, nVisEx);
				DRLG_SetWarpId(pDrlgWarp1, nVisEx, -1, -1);
				DRLG_SetWarpId(pDrlgWarp2, pDrlgLink[i].nLevel, -1, -1);
			}
		}
	}
}

//D2Common.0x6FD826D0
void __fastcall sub_6FD826D0(D2DrlgStrc* pDrlg, int nStartId, int nEndId)
{
	D2DrlgWarpStrc* pDrlgWarp = NULL;
	D2DrlgLevelStrc* pLevel = NULL;

	for (int i = nStartId; i <= nEndId; ++i)
	{
		pLevel = DRLG_GetLevel(pDrlg, i);
		pDrlgWarp = DRLG_GetDrlgWarpFromLevelId(pDrlg, i);

		for (int j = nStartId; j <= nEndId; ++j)
		{
			if (i != j && sub_6FD77800(&pLevel->pLevelCoords, &DRLG_GetLevel(pDrlg, j)->pLevelCoords, -1))
			{
				DRLG_SetWarpId(pDrlgWarp, j, -1, -1);
			}
		}
	}
}

//D2Common.0x6FD82750
void __fastcall sub_6FD82750(D2DrlgStrc* pDrlg, int nStartId, int nEndId)
{
	D2DrlgLevelStrc* pWarpLevel = NULL;
	D2DrlgLevelStrc* pLevel = NULL;
	int* pWarpIdArray = NULL;
	int* pVisArray = NULL;

	for (int i = nStartId; i <= nEndId; ++i)
	{
		pLevel = DRLG_GetLevel(pDrlg, i);

		if (pLevel->nDrlgType == DRLGTYPE_OUTDOOR)
		{
			pVisArray = DRLGROOM_GetVisArrayFromLevelId(pDrlg, i);
			pWarpIdArray = DRLGWARP_GetWarpIdArrayFromLevelId(pDrlg, i);

			for (int j = 0; j < 8; ++j)
			{
				if (pVisArray[j] && pWarpIdArray[j] == -1)
				{
					pWarpLevel = DRLG_GetLevel(pLevel->pDrlg, pVisArray[j]);
					DRLGROOM_AddOrth(&pLevel->pOutdoors->pRoomData, pWarpLevel, DRLG_GetDirectionFromCoordinates(&pLevel->pLevelCoords, &pWarpLevel->pLevelCoords), pWarpLevel->nDrlgType == DRLGTYPE_PRESET);
				}
			}
		}
	}
}



//D2Common.0x6FDD0828
static const D2C_LvlPrestIds gJunglePresets[] = {
	LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_W_E, LVLPREST_ACT3_JUNGLE_W_S, LVLPREST_ACT3_JUNGLE_W_N,
	LVLPREST_ACT3_JUNGLE_E_W, LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_E_S, LVLPREST_ACT3_JUNGLE_E_N,
	LVLPREST_NONE, LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_EW_S, LVLPREST_ACT3_JUNGLE_EW_N,
	LVLPREST_ACT3_JUNGLE_S_W, LVLPREST_ACT3_JUNGLE_S_E, LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_S_N,
	LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_SW_E, LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_SW_N,
	LVLPREST_ACT3_JUNGLE_SE_W, LVLPREST_NONE, LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_SE_N,
	LVLPREST_NONE, LVLPREST_NONE, LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_SEW_N,
	LVLPREST_ACT3_JUNGLE_N_W, LVLPREST_ACT3_JUNGLE_N_E, LVLPREST_ACT3_JUNGLE_N_S, LVLPREST_NONE,
	LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_NW_E, LVLPREST_ACT3_JUNGLE_NW_S, LVLPREST_NONE,
	LVLPREST_ACT3_JUNGLE_NE_W, LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_NE_S, LVLPREST_NONE,
	LVLPREST_NONE, LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_NEW_S, LVLPREST_NONE,
	LVLPREST_ACT3_JUNGLE_NS_W, LVLPREST_ACT3_JUNGLE_NS_E, LVLPREST_NONE, LVLPREST_NONE,
	LVLPREST_NONE, LVLPREST_ACT3_JUNGLE_NSW_E, LVLPREST_NONE, LVLPREST_NONE,
	LVLPREST_ACT3_JUNGLE_NSE_W, LVLPREST_NONE, LVLPREST_NONE, LVLPREST_NONE,
	LVLPREST_NONE, LVLPREST_NONE, LVLPREST_NONE, LVLPREST_NONE,
};
//D2Common.0x6FDD0924
static const D2C_LvlPrestIds gSpiderForestPresets[] = {
	LVLPREST_NONE, LVLPREST_ACT3_CLEARING_WEBBY_W, LVLPREST_ACT3_CLEARING_WEBBY_E, LVLPREST_ACT3_CLEARING_WEBBY_EW,
	LVLPREST_ACT3_CLEARING_WEBBY_S, LVLPREST_ACT3_CLEARING_WEBBY_SW, LVLPREST_ACT3_CLEARING_WEBBY_SE, LVLPREST_NONE,
	LVLPREST_ACT3_CLEARING_WEBBY_N, LVLPREST_ACT3_CLEARING_WEBBY_NW, LVLPREST_ACT3_CLEARING_WEBBY_NE, LVLPREST_NONE,
	LVLPREST_ACT3_CLEARING_WEBBY_NS, LVLPREST_NONE, LVLPREST_NONE, LVLPREST_NONE,
	LVLPREST_NONE,
};

static const int nPresetBlocksDimensions = 32;

static void DRLG_GenerateJunglesAttachPoints(
	D2DrlgStrc* pDrlg,
	D2JungleStrc* tJungles,
	int32_t nMinX, int32_t nMinY,
	int32_t nSpiderForestLevelSizeX, int32_t nSpiderForestLevelSizeY,
	int32_t nPresetsWidth, int32_t nPresetsHeight,
	int32_t* pPreset0, int32_t* pPreset1, int32_t* pPreset2, int32_t* pLevelPresetId
)
{
	const int32_t nPresets = nPresetsWidth * nPresetsHeight;
	memset(pPreset0, 0, nPresets * sizeof(int32_t));
	memset(pPreset1, 0, nPresets * sizeof(int32_t));
	memset(pPreset2, 0, nPresets * sizeof(int32_t));
	memset(pLevelPresetId, 0, nPresets * sizeof(int32_t));

	const int32_t nSpiderForestLevelPresetsBlocksSizeX = nSpiderForestLevelSizeX / nPresetBlocksDimensions;
	const int32_t nSpiderForestLevelPresetsBlocksSizeY = nSpiderForestLevelSizeY / nPresetBlocksDimensions;

	for (int nJungleIdx = 0; nJungleIdx < JUNGLE_MAX_ATTACH; nJungleIdx++)
	{
		D2JungleStrc& tCurrentJungle = tJungles[nJungleIdx];
		const int32_t nJungleOffsetX = tCurrentJungle.pDrlgCoord.nPosX - nMinX;
		const int32_t nJungleOffsetY = tCurrentJungle.pDrlgCoord.nPosY - nMinY;

		tCurrentJungle.nPresetsBlocksX = (((nJungleOffsetX & 0x1F) + nJungleOffsetX) >> 5) + 1;
		tCurrentJungle.nPresetsBlocksY = (((nJungleOffsetY & 0x1F) + nJungleOffsetY) >> 5) + 1;

		// Note: Original game could leak memory here (if jungle generation fails, we would realloc without freeing)
		if (tCurrentJungle.pJungleDefs)
		{
			D2_FREE_SERVER(pDrlg->pMempool, tCurrentJungle.pJungleDefs);
		}

		tCurrentJungle.pJungleDefs = (int32_t*)D2_ALLOC_SERVER(pDrlg->pMempool, nSpiderForestLevelPresetsBlocksSizeX * nSpiderForestLevelPresetsBlocksSizeY * sizeof(int32_t));

		// Could be the choice of direction on X axis
		bool bLineStartWithOffset;
		if (nJungleIdx != 0)
		{
			bLineStartWithOffset = tCurrentJungle.field_10 % 2;
		}
		else
		{
			bLineStartWithOffset = SEED_RollLimitedRandomNumber(&pDrlg->pSeed, 2);
		}

		// Generate potential attach points
		int nJungleAttachPoints = 0;
		while (nJungleAttachPoints < 2)
		{
			// Initialize data
			int32_t nPresetBlockRowOffset = tCurrentJungle.nPresetsBlocksX + nPresetsWidth * tCurrentJungle.nPresetsBlocksY;
			for (int32_t nPresetBlockY = 0; nPresetBlockY < nSpiderForestLevelPresetsBlocksSizeY; nPresetBlockY++)
			{
				for (int32_t nPresetBlockX = 0; nPresetBlockX < nSpiderForestLevelPresetsBlocksSizeX; nPresetBlockX++)
				{
					const int nPresetBlockIndex = nPresetBlockRowOffset + nPresetBlockX;
					pPreset0[nPresetBlockIndex] = nJungleIdx + 1;
					pPreset1[nPresetBlockIndex] = 0;
					pPreset2[nPresetBlockIndex] = 0;
					pLevelPresetId[nPresetBlockIndex] = 0;
				}
				nPresetBlockRowOffset += nPresetsWidth;
			}


			const int32_t nFirstPresetsBlockY = tCurrentJungle.nPresetsBlocksY;
			const int32_t nLastPresetsBlockY = nFirstPresetsBlockY + nSpiderForestLevelPresetsBlocksSizeY - 1;

			if (nJungleIdx != 0)
			{
				const int32_t nFirstPresetsBlockIndex = nLastPresetsBlockY * nPresetsWidth + tCurrentJungle.nPresetsBlocksX;
				// Either |1|2| or |2|1| based on bLineStartWithOffset
				pPreset2[nFirstPresetsBlockIndex + bLineStartWithOffset] = 1;
				pPreset2[nFirstPresetsBlockIndex + (!bLineStartWithOffset)] = JUNGLE_PRESET2_ATTACH_POINT;
			}


			for (int32_t nBranchIdx = 0; nBranchIdx < tCurrentJungle.nBranch; nBranchIdx++)
			{
				D2JungleStrc* pJungleBranch = tCurrentJungle.pJungleBranches[nBranchIdx];
				int32_t nPresetOffsetX = 0;
				int32_t nPresetOffsetY = 0;
				switch (pJungleBranch->field_10)
				{
				case 0:
					break;
				case 1:
					nPresetOffsetY = 3;
					break;
				case 2:
					nPresetOffsetX = 1;
					nPresetOffsetY = 3;
					break;
				case 3:
					nPresetOffsetY = 1;
					break;
				case 4:
					nPresetOffsetX = 1;
					nPresetOffsetY = 1;
					break;
				default:
					D2_UNREACHABLE;
					break;
				}
				pPreset2[tCurrentJungle.nPresetsBlocksX + nPresetOffsetX + nPresetsWidth * (tCurrentJungle.nPresetsBlocksY + nPresetOffsetY)] = 1;
			}
			
			nJungleAttachPoints = nJungleIdx != 0;


			// This loop is tracing a wave in the first two columns of the map, in zigzags, starting from last Y
			// X will either be nPresetsBlocksY or nPresetsBlocksY + 1
			//  0 1 2 3 4 5 6
			//  _ _ _ _ _ _ _
			// | |X| | | | | |
			// | |X| | | | | |
			// |X| | | | | | |
			// |X| | | | | | |
			// | |X| | | | | |
			// |X| | | | | | |
			// |X| | | | | | |
			int32_t nCurrentPresetsBlockX = tCurrentJungle.nPresetsBlocksX;
			int32_t i = 20 * (5 * nJungleIdx + 5);
			int32_t nColumnSize = 0;
			for (int32_t nCurrentPresetsBlockY = nLastPresetsBlockY; nCurrentPresetsBlockY >= nFirstPresetsBlockY; )
			{
				const int32_t nCurrentPresetsBlockRowOffset = nCurrentPresetsBlockY * nPresetsWidth;

				pPreset1[nCurrentPresetsBlockRowOffset + nCurrentPresetsBlockX + bLineStartWithOffset] = i;
				i++;

				if (nColumnSize == 0
					|| nCurrentPresetsBlockY == nFirstPresetsBlockY
					|| SEED_RollLimitedRandomNumber(&pDrlg->pSeed, 3))
				{
					nColumnSize++;

					// Mark opposite cell with '2' (attach point?) if not already set once the column has at least 2 pixels.
					if (nColumnSize >= 2 && nCurrentPresetsBlockY > 1)
					{
						int32_t nNextPresetIdx = nCurrentPresetsBlockRowOffset + nCurrentPresetsBlockX + (!bLineStartWithOffset);

						if (pPreset2[nNextPresetIdx] == 0)
						{
							pPreset2[nNextPresetIdx] = JUNGLE_PRESET2_ATTACH_POINT;
							nJungleAttachPoints++;
						}
					}
					nCurrentPresetsBlockY--;
				}
				else
				{
					nColumnSize = 0;
					bLineStartWithOffset = !bLineStartWithOffset;
				}
			}
		}

		// We now need to reduce the number of attach points to 3, as we may have generated a lot more
		while (nJungleAttachPoints > 3)
		{
			bool bFinished = false;
			uint32_t nAttachPointsFound = 0;
			uint32_t nAttachPointToRemove = SEED_RollLimitedRandomNumber(&pDrlg->pSeed, nJungleAttachPoints);

			// Starts from the first Y (opposite of previous step)
			for (int32_t nCurrentPresetsBlockY = 0; nCurrentPresetsBlockY < nSpiderForestLevelPresetsBlocksSizeY && !bFinished; nCurrentPresetsBlockY++)
			{
				int32_t nCurrentPresetsBlockOffset = tCurrentJungle.nPresetsBlocksX + (tCurrentJungle.nPresetsBlocksY+ nCurrentPresetsBlockY) * nPresetsWidth;

				for (int32_t nCurrentPresetsBlockX = 0; nCurrentPresetsBlockX < nSpiderForestLevelPresetsBlocksSizeX && !bFinished; nCurrentPresetsBlockX++)
				{
					// This is an attach point
					const int32_t nCurrentPresetBlockIdx = nCurrentPresetsBlockOffset + nCurrentPresetsBlockX;
					if (pPreset2[nCurrentPresetBlockIdx] == JUNGLE_PRESET2_ATTACH_POINT)
					{
						if (nAttachPointsFound == nAttachPointToRemove)
						{
							// Unmark this attach point
							pPreset2[nCurrentPresetBlockIdx] = 0;
							bFinished = true;
							nJungleAttachPoints--;
						}
						++nAttachPointsFound;
					}
				}
			}

		}

	}
}

static void DRLG_JungleComputeConnexity(
	D2DrlgStrc* pDrlg,
	int32_t nPresetsWidth,
	int32_t nPresetsHeight,
	int32_t* pPreset0,
	int32_t* pPreset1,
	int32_t* pPreset2,
	int32_t* pLevelPresetId)
{


	// Seems to compute some kind of connexity graph
	for (int32_t nCurrentPresetY = 0; nCurrentPresetY < nPresetsHeight; nCurrentPresetY++)
	{
		for (int32_t nCurrentPresetX = 0; nCurrentPresetX < nPresetsWidth; nCurrentPresetX++)
		{

			static const int32_t nMinValue = 0;
			static const int32_t nMaxValue = 0x7FFFFFFF;

			int32_t nFlags = 0;
			int32_t nLastDirPreset1Value = nMaxValue;

			const int32_t nCurrentPresetIndex = nCurrentPresetY * nPresetsWidth + nCurrentPresetX;
			const int32_t nRightPresetIndex = nCurrentPresetY * nPresetsWidth + nCurrentPresetX + 1;
			const int32_t nLeftPresetIndex = nCurrentPresetY * nPresetsWidth + nCurrentPresetX - 1;
			const int32_t nTopPresetIndex = (nCurrentPresetY - 1) * nPresetsWidth + nCurrentPresetX;
			const int32_t nBottomPresetIndex = (nCurrentPresetY + 1) * nPresetsWidth + nCurrentPresetX;
			if (pPreset2[nCurrentPresetIndex] == 1)
			{
				const int32_t nPreset0CurrentValue = pPreset0[nCurrentPresetIndex];

				auto UpdateAccordingToPreset1 = [pPreset0, pPreset1, nPreset0CurrentValue, &nLastDirPreset1Value, &nFlags](int32_t nPresetIndex, int32_t nDirectionFlag)
				{
					const int32_t nPreset1Value = pPreset1[nPresetIndex];
					if (nPreset1Value != 0 && nPreset1Value < nLastDirPreset1Value
						&& pPreset0[nPresetIndex] == nPreset0CurrentValue)
					{
						nFlags = nDirectionFlag;
						nLastDirPreset1Value = nPreset1Value;
					}
				};

				UpdateAccordingToPreset1(nTopPresetIndex, JUNGLE_FLAG_TOP_);
				UpdateAccordingToPreset1(nBottomPresetIndex, JUNGLE_FLAG_BOTTOM_);
				UpdateAccordingToPreset1(nRightPresetIndex, JUNGLE_FLAG_RIGHT_);
				UpdateAccordingToPreset1(nLeftPresetIndex, JUNGLE_FLAG_LEFT_);



				if (nFlags & JUNGLE_FLAG_TOP_)	pLevelPresetId[nTopPresetIndex] |= JUNGLE_FLAG_BOTTOM_;
				if (nFlags & JUNGLE_FLAG_BOTTOM_) pLevelPresetId[nBottomPresetIndex] |= JUNGLE_FLAG_TOP_;
				if (nFlags & JUNGLE_FLAG_RIGHT_)	pLevelPresetId[nRightPresetIndex] |= JUNGLE_FLAG_LEFT_;
				if (nFlags & JUNGLE_FLAG_LEFT_)	pLevelPresetId[nLeftPresetIndex] |= JUNGLE_FLAG_RIGHT_;

				if (pPreset2[nTopPresetIndex] == 1 && pPreset0[nTopPresetIndex] != nPreset0CurrentValue)
				{
					nFlags |= JUNGLE_FLAG_TOP_;
				}
				if (pPreset2[nBottomPresetIndex] == 1 && pPreset0[nBottomPresetIndex] != nPreset0CurrentValue)
				{
					nFlags |= JUNGLE_FLAG_BOTTOM_;
				}
				if (pPreset2[nRightPresetIndex] == 1 && pPreset0[nRightPresetIndex] != nPreset0CurrentValue)
				{
					nFlags |= JUNGLE_FLAG_RIGHT_;
				}
				if (pPreset2[nLeftPresetIndex] == 1 && pPreset0[nLeftPresetIndex] != nPreset0CurrentValue)
				{
					nFlags |= JUNGLE_FLAG_LEFT_;
				}
			}
			if (const int32_t nPreset1CurrentValue = pPreset1[nCurrentPresetIndex])
			{
				if (1 == std::abs(pPreset1[nTopPresetIndex] - nPreset1CurrentValue))
				{
					nFlags |= JUNGLE_FLAG_TOP_;
				}
				if (1 == std::abs(pPreset1[nBottomPresetIndex] - nPreset1CurrentValue))
				{
					nFlags |= JUNGLE_FLAG_BOTTOM_;
				}
				if (1 == std::abs(pPreset1[nRightPresetIndex] - nPreset1CurrentValue))
				{
					nFlags |= JUNGLE_FLAG_RIGHT_;
				}
				if (1 == std::abs(pPreset1[nLeftPresetIndex] - nPreset1CurrentValue))
				{
					nFlags |= JUNGLE_FLAG_LEFT_;
				}
			}

			pLevelPresetId[nCurrentPresetIndex] |= nFlags;
		}
	}
}


// Returns false if we need to generate a new map due to invalid attach points
static bool DRLG_JungleUpdateAttachPointsDirections(
	D2DrlgStrc* pDrlg,
	int32_t nPresetsWidth,
	int32_t nPresetsHeight,
	int32_t* pPreset0,
	int32_t* pPreset2,
	int32_t* pLevelPresetId)
{

	for (int32_t nCurrentPresetY = 0; nCurrentPresetY < nPresetsHeight; nCurrentPresetY++)
	{
		for (int32_t nCurrentPresetX = 0; nCurrentPresetX < nPresetsWidth; nCurrentPresetX++)
		{
			const int32_t nCurrentPresetIndex = nCurrentPresetY * nPresetsWidth + nCurrentPresetX;

			int nDirectionBase = SEED_RollLimitedRandomNumber(&pDrlg->pSeed, DIRECTION_COUNT);

			if (pPreset2[nCurrentPresetIndex] == JUNGLE_PRESET2_ATTACH_POINT)
			{
				const int32_t nRightPresetIndex = nCurrentPresetY * nPresetsWidth + nCurrentPresetX + 1;
				const int32_t nLeftPresetIndex = nCurrentPresetY * nPresetsWidth + nCurrentPresetX - 1;
				const int32_t nTopPresetIndex = (nCurrentPresetY - 1) * nPresetsWidth + nCurrentPresetX;
				const int32_t nBottomPresetIndex = (nCurrentPresetY + 1) * nPresetsWidth + nCurrentPresetX;

				const int32_t nPreset0CurrentValue = pPreset0[nCurrentPresetIndex];


				int32_t nCurrentPresetLevelId = pLevelPresetId[nCurrentPresetIndex];
				const int32_t nPrevPreset3RowFlags = nCurrentPresetLevelId;
				bool bHadNoPresetLeveld = nCurrentPresetLevelId == 0;

				bool bLookAtNextDirection;
				auto LookForDirection = [&](auto&& CheckDirectionFunction)
				{
					for (int i = 0; i < DIRECTION_COUNT && bLookAtNextDirection; i++)
					{
						switch ((nDirectionBase + i) % DIRECTION_COUNT)
						{
						case DIRECTION_SOUTHWEST:
							CheckDirectionFunction(nTopPresetIndex, JUNGLE_FLAG_TOP_);
							break;
						case DIRECTION_NORTHWEST:
							CheckDirectionFunction(nBottomPresetIndex, JUNGLE_FLAG_BOTTOM_);
							break;
						case DIRECTION_SOUTHEAST:
							CheckDirectionFunction(nRightPresetIndex, JUNGLE_FLAG_RIGHT_);
							break;
						case DIRECTION_NORTHEAST:
							CheckDirectionFunction(nLeftPresetIndex, JUNGLE_FLAG_LEFT_);
							break;
						default:
							break;
						}
					}
				};


				bLookAtNextDirection = true;
				LookForDirection([&](int32_t nPresetIndex, D2JunglePresetFlags nFlagToAdd) {
					if (pPreset0[nPresetIndex] == nPreset0CurrentValue
						&& pLevelPresetId[nPresetIndex] && pLevelPresetId[nPresetIndex] < 15)
					{
						nCurrentPresetLevelId |= (nFlagToAdd << 4);
						bLookAtNextDirection = false;
					}
					}
				);

				// If we found nothing, start generation all over
				if (!nCurrentPresetLevelId)
				{
					return false;
				}
				if (bHadNoPresetLeveld)
				{
					bLookAtNextDirection = true;
					LookForDirection([&](int32_t nPresetIndex, D2JunglePresetFlags nFlagToAdd) {

						if (pPreset0[nPresetIndex] != nPreset0CurrentValue
							&& pPreset2[nPresetIndex] == JUNGLE_PRESET2_ATTACH_POINT)
						{
							nCurrentPresetLevelId |= (nFlagToAdd << 4);
							bLookAtNextDirection = false;
						}
						}
					);

					bLookAtNextDirection = SEED_RollLimitedRandomNumber(&pDrlg->pSeed, 2) != 0 && bLookAtNextDirection;

					// Roll a new direction start
					nDirectionBase = SEED_RollLimitedRandomNumber(&pDrlg->pSeed, DIRECTION_COUNT);

					LookForDirection([&](int32_t nPresetIndex, D2JunglePresetFlags nFlagToAdd) {

						if (pLevelPresetId[nPresetIndex] && pLevelPresetId[nPresetIndex] < 15
							&& ((nCurrentPresetLevelId & (nFlagToAdd << 4)) == 0)
							)
						{
							nCurrentPresetLevelId |= (nFlagToAdd << 4);
							bLookAtNextDirection = false;
						}
						}
					);
				}

				if ((nCurrentPresetLevelId & (JUNGLE_FLAG_TOP_ << 4)) != 0)
				{
					pLevelPresetId[nTopPresetIndex] |= (JUNGLE_FLAG_BOTTOM_ << 4);
				}
				if ((nCurrentPresetLevelId & (JUNGLE_FLAG_BOTTOM_ << 4)) != 0)
				{
					pLevelPresetId[nBottomPresetIndex] |= (JUNGLE_FLAG_TOP_ << 4);
				}
				if ((nCurrentPresetLevelId & (JUNGLE_FLAG_RIGHT_ << 4)) != 0)
				{
					pLevelPresetId[nRightPresetIndex] |= (JUNGLE_FLAG_LEFT_ << 4);
				}
				if ((nCurrentPresetLevelId & (JUNGLE_FLAG_LEFT_ << 4)) != 0)
				{
					pLevelPresetId[nLeftPresetIndex] |= (JUNGLE_FLAG_RIGHT_ << 4);
				}

				pPreset2[nCurrentPresetIndex] = 0;
				pLevelPresetId[nCurrentPresetIndex] |= nCurrentPresetLevelId;
			}
		}
	}

	return true;
}

static D2C_LvlPrestIds DRLG_JungleNormalizeLevelPresetId(int32_t nLevelPresetId)
{
	const int32_t nbDirections = 16;
	int32_t nPDef = nLevelPresetId % nbDirections;
	if (nPDef != 0)
	{
		if (nLevelPresetId < nbDirections)
		{
			nPDef += LVLPREST_ACT3_TOWN;
		}
		else
		{
			const int nPresetDefinitionOffset = 4 * (nPDef - 1); // Note: nPDef > 0
			if ((nLevelPresetId & (JUNGLE_FLAG_LEFT_ << 4)) != 0)
				nPDef = gJunglePresets[nPresetDefinitionOffset];
			if ((nLevelPresetId & (JUNGLE_FLAG_RIGHT_ << 4)) != 0)
				nPDef = gJunglePresets[nPresetDefinitionOffset + 1];
			if ((nLevelPresetId & (JUNGLE_FLAG_BOTTOM_ << 4)) != 0)
				nPDef = gJunglePresets[nPresetDefinitionOffset + 2];
			if ((nLevelPresetId & (JUNGLE_FLAG_TOP_ << 4)) != 0)
				nPDef = gJunglePresets[nPresetDefinitionOffset + 3];

			if (nPDef == LVLPREST_NONE) FOG_10025("nPDef != PRESET_DEF_NONE", __FILE__, __LINE__);
		}
	}
	else if (nLevelPresetId >= nbDirections)
	{
		nPDef = gSpiderForestPresets[nLevelPresetId >> 4];
		if (nPDef == LVLPREST_NONE) FOG_10025("nPDef != PRESET_DEF_NONE", __FILE__, __LINE__);
	}
	return (D2C_LvlPrestIds)nPDef;
}

// D2Common.0x6FD82820
// Checked to be working correctly, but with a single save only
// See https://web.archive.org/web/20220928163841/http://www.dos486.com/diablo/jungle/ for an overview about act 3 jungles
D2DrlgLevelStrc* __fastcall DRLG_GenerateJungles(D2DrlgLevelStrc* pLevel)
{
	D2LevelDefBin* pSpiderForestLevelDef = DATATBLS_GetLevelDefRecord(LEVEL_SPIDERFOREST);
	D2DrlgStrc* pDrlg = pLevel->pDrlg;
	uint8_t nDifficulty = pDrlg->nDifficulty;
	int32_t nSpiderForestLevelSizeX = pSpiderForestLevelDef->dwSizeX[nDifficulty];
	int32_t nSpiderForestLevelSizeY = pSpiderForestLevelDef->dwSizeY[nDifficulty];
	int32_t nMinX = pLevel->nPosX;
	int32_t nMaxX = nSpiderForestLevelSizeX + nMinX;
	// y axis inverted
	int32_t nMaxY = pLevel->nPosY;
	int32_t nMinY = nMaxY - nSpiderForestLevelSizeY;


	D2JungleStrc tJungles[JUNGLE_MAX_ATTACH];
	memset(tJungles, 0, sizeof(tJungles));
	sub_6FD83970(&pLevel->pLevelCoords, &tJungles[0], 0, nSpiderForestLevelSizeX, nSpiderForestLevelSizeY);

	for (int nJungleAttachIdx = 1; nJungleAttachIdx < JUNGLE_MAX_ATTACH; nJungleAttachIdx++)
	{
		uint32_t nBaseOn = SEED_RollLimitedRandomNumber(&pDrlg->pSeed, nJungleAttachIdx);

		D2JungleStrc* pCurrentJungle = &tJungles[nJungleAttachIdx];
		sub_6FD83970(&tJungles[nBaseOn].pDrlgCoord, pCurrentJungle, SEED_RollLimitedRandomNumber(&pDrlg->pSeed, 5), nSpiderForestLevelSizeX, nSpiderForestLevelSizeY);

		// Look for the first overlapping jungle
		int nFirstOverlappingJungle;
		for (nFirstOverlappingJungle = 0; nFirstOverlappingJungle < nJungleAttachIdx; nFirstOverlappingJungle++)
		{
			// sub_6FD777B0 Returns true if not overlapping and or sharing border ( distance >= 0 )
			const bool levelsOverlaps = !sub_6FD777B0(&tJungles[nFirstOverlappingJungle].pDrlgCoord, &pCurrentJungle->pDrlgCoord, 0);
			if (levelsOverlaps)
			{
				break;
			}
		}
		// If we found a jungle that overlaps, we need to try again
		if (nFirstOverlappingJungle < nJungleAttachIdx)
		{
			// Try again
			nJungleAttachIdx--;
			continue;
		}
		
		// Link to the jungle we are based on
		pCurrentJungle->pBasedOnJungle = &tJungles[nBaseOn];

		// Note: this assert used to be done AFTER assignation, which could lead to overriding the next field. We do it before instead
		D2_ASSERT(tJungles[nBaseOn].nBranch < JUNGLE_MAX_ATTACH);
		// Link the jungle we are based on to current jungle
		tJungles[nBaseOn].pJungleBranches[tJungles[nBaseOn].nBranch++] = &tJungles[nJungleAttachIdx];

#if 0 // This assert would always come too late, overriding content.
		if (!(tJungles[nBaseOn].nBranch <= JUNGLE_MAX_ATTACH))
		{
			FOG_10025(
				"ptJungle[nBaseOn].nBranch <= JUNGLE_MAX_ATTACH",
				"C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp",
				1570);
		}
#endif

		if (nMinX > pCurrentJungle->pDrlgCoord.nPosX)
			nMinX = pCurrentJungle->pDrlgCoord.nPosX;
		if (nMinY > pCurrentJungle->pDrlgCoord.nPosY)
			nMinY = pCurrentJungle->pDrlgCoord.nPosY;
		if (nMaxX < pCurrentJungle->pDrlgCoord.nWidth + pCurrentJungle->pDrlgCoord.nPosX)
			nMaxX = pCurrentJungle->pDrlgCoord.nWidth + pCurrentJungle->pDrlgCoord.nPosX;
		// Original game does not change nMinY: error or just never happens ?
	}
	// Note: Original game uses FOG_10025 not FOG_Assertion
	D2_ASSERT((nMaxX - nMinX) % nPresetBlocksDimensions == 0);
	D2_ASSERT((nMaxY - nMinY) % nPresetBlocksDimensions == 0);


	const int32_t nPresetsWidth = (nMaxX - nMinX) / nPresetBlocksDimensions + 2;
	const int32_t nPresetsHeight = (nMaxY - nMinY) / nPresetBlocksDimensions + 2;
	const int32_t nPresets = nPresetsWidth * nPresetsHeight;

	const size_t nPresetsAllocSize = nPresets * sizeof(uint32_t);
	int32_t* pPreset0 = (int32_t*)D2_ALLOC_SERVER(pDrlg->pMempool, nPresetsAllocSize);
	int32_t* pPreset1 = (int32_t*)D2_ALLOC_SERVER(pDrlg->pMempool, nPresetsAllocSize); // Could be some kind of distance field ?
	int32_t* pPreset2 = (int32_t*)D2_ALLOC_SERVER(pDrlg->pMempool, nPresetsAllocSize);
	int32_t* pLevelPresetId = (int32_t*)D2_ALLOC_SERVER(pDrlg->pMempool, nPresetsAllocSize);

	do {
		DRLG_GenerateJunglesAttachPoints(pDrlg, tJungles,
			nMinX, nMinY,
			nSpiderForestLevelSizeX, nSpiderForestLevelSizeY,
			nPresetsWidth, nPresetsHeight,
			pPreset0, pPreset1, pPreset2, pLevelPresetId);
		DRLG_JungleComputeConnexity(pDrlg, nPresetsWidth, nPresetsHeight, pPreset0, pPreset1, pPreset2, pLevelPresetId);
	} while (!DRLG_JungleUpdateAttachPointsDirections(pDrlg, nPresetsWidth, nPresetsHeight, pPreset0, pPreset2, pLevelPresetId));


	for (int32_t nCurrentPresetY = 0; nCurrentPresetY < nPresetsHeight; nCurrentPresetY++)
	{
		for (int32_t nCurrentPresetX = 0; nCurrentPresetX < nPresetsWidth; nCurrentPresetX++)
		{
			const int32_t nCurrentPresetIndex = nCurrentPresetY * nPresetsWidth + nCurrentPresetX;
			pLevelPresetId[nCurrentPresetIndex] = DRLG_JungleNormalizeLevelPresetId(pLevelPresetId[nCurrentPresetIndex]);
		}
	}

	for (int nJungleIdx = 0; nJungleIdx < JUNGLE_MAX_ATTACH; ++nJungleIdx)
	{
		D2JungleStrc& tCurrentJungle = tJungles[nJungleIdx];
		const int32_t nSpiderForestLevelPresetsBlocksSizeX = nSpiderForestLevelSizeX / nPresetBlocksDimensions;
		const int32_t nSpiderForestLevelPresetsBlocksSizeY = nSpiderForestLevelSizeY / nPresetBlocksDimensions;

		int nJungleDefsIndex = 0;
		for (int nBlockY = 0; nBlockY < nSpiderForestLevelPresetsBlocksSizeY; ++nBlockY)
		{
			for (int nBlockX = 0; nBlockX < nSpiderForestLevelPresetsBlocksSizeX; ++nBlockX)
			{
				const int32_t nCurrentPresetIndex = nBlockX + tCurrentJungle.nPresetsBlocksX + nPresetsWidth * (nBlockY + tCurrentJungle.nPresetsBlocksY);
				const int32_t nLevelPresetId = pLevelPresetId[nCurrentPresetIndex];
				tCurrentJungle.pJungleDefs[nJungleDefsIndex] = nLevelPresetId;
				if (nLevelPresetId > LVLPREST_ACT3_JUNGLE_TAIL)
					++tCurrentJungle.nJungleDefs;
				++nJungleDefsIndex;
			}
		}
	}

	std::sort(tJungles, tJungles + JUNGLE_MAX_ATTACH, 
		[](const auto& lhs, const auto& rhs) 
		{ // Order from higher to lower nPosY
			return lhs.pDrlgCoord.nPosY > rhs.pDrlgCoord.nPosY; 
		});

	D2DrlgLevelStrc* pJungleLevel = nullptr;
	for (int i = 0; i < JUNGLE_MAX_ATTACH; ++i)
	{
		pJungleLevel = DRLG_GetLevel(pDrlg, i + LEVEL_SPIDERFOREST);
	
		pJungleLevel->pJungleDefs = tJungles[i].pJungleDefs;
		pJungleLevel->nJungleDefs = tJungles[i].nJungleDefs;
		pJungleLevel->nPosX       = tJungles[i].pDrlgCoord.nPosX;
		pJungleLevel->nPosY       = tJungles[i].pDrlgCoord.nPosY;
		pJungleLevel->nWidth      = tJungles[i].pDrlgCoord.nWidth;
		pJungleLevel->nHeight     = tJungles[i].pDrlgCoord.nHeight;
	}

	D2_FREE_SERVER(pDrlg->pMempool, pPreset0);
	D2_FREE_SERVER(pDrlg->pMempool, pPreset1);
	D2_FREE_SERVER(pDrlg->pMempool, pPreset2);
	D2_FREE_SERVER(pDrlg->pMempool, pLevelPresetId);

	return pJungleLevel;
}
/////////////



//D2Common.0x6FD83970
void __fastcall sub_6FD83970(D2DrlgCoordStrc* pDrlgCoord, D2JungleStrc* pJungle, int nRand, int nSizeX, int nSizeY)
{
	unsigned int nTemp = 0;
	int nX = 0;
	int nY = 0;

	switch (nRand)
	{
	case 0:
		nX = 0;
		nY = -nSizeY;
		break;

	case 1:
		nX = -nSizeX;
		nTemp = (signed int)((0x55555555ui64 * nSizeY >> 32) - nSizeY) >> 1;
		nY = (nTemp >> 31) + nTemp;
		break;

	case 2:
		nX = nSizeX;
		nTemp = (signed int)((0x55555555ui64 * nSizeY >> 32) - nSizeY) >> 1;
		nY = (nTemp >> 31) + nTemp;
		break;

	case 3:
		nX = -nSizeX;
		nTemp = 0xFFFFFFFF55555554ui64 * nSizeY >> 32;
		nY = (nTemp >> 31) + nTemp;
		break;

	case 4:
		nX = nSizeX;
		nTemp = 0xFFFFFFFF55555554ui64 * nSizeY >> 32;
		nY = (nTemp >> 31) + nTemp;
		break;

	default:
		break;
	}

	pJungle->pDrlgCoord.nPosX = nX + pDrlgCoord->nPosX;
	pJungle->pDrlgCoord.nHeight = nSizeY;
	pJungle->pDrlgCoord.nWidth = nSizeX;
	pJungle->pDrlgCoord.nPosY = nY + pDrlgCoord->nPosY;
	pJungle->field_10 = nRand;
}

//D2Common.0x6FD83A20
void __fastcall DRLGOUTPLACE_InitOutdoorRoomGrids(D2RoomExStrc* pRoomEx)
{
	D2UnkOutdoorStrc2 a1 = {};

	D2LevelDefBin* pLevelDefBinRecord = DATATBLS_GetLevelDefRecord(pRoomEx->pLevel->nLevelId);

	uint32_t nWaypointSubTheme = (pRoomEx->dwFlags & ROOMEXFLAG_HAS_WAYPOINT_MASK) >> ROOMEXFLAG_HAS_WAYPOINT_FIRST_BIT;
	uint32_t nShrineSubTheme = (pRoomEx->dwFlags & ROOMEXFLAG_SUBSHRINE_ROWS_MASK) >> ROOMEXFLAG_SUBSHRINE_ROWS_FIRST_BIT;

	int nWidth = pRoomEx->nTileWidth + 1;
	int nHeight = pRoomEx->nTileHeight + 1;

	DRLGGRID_InitializeGridCells(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pOutdoor->pTileTypeGrid, nWidth, nHeight);
	DRLGGRID_InitializeGridCells(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pOutdoor->pWallGrid, nWidth, nHeight);
	DRLGGRID_InitializeGridCells(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pOutdoor->pFloorGrid, nWidth, nHeight);

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			DRLGGRID_AlterGridFlag(&pRoomEx->pOutdoor->pFloorGrid, j, i, 0x40002, FLAG_OPERATION_OVERWRITE);
		}
	}

	if (DRLG_GetActNoFromLevelId(pRoomEx->pLevel->nLevelId) == ACT_I)
	{
		DRLG_OUTDOORS_GenerateDirtPath(pRoomEx->pLevel, pRoomEx);
	}

	DRLGROOMTILE_AllocTileGrid(pRoomEx);

	memset(&a1, 0, sizeof(a1));
	a1.pRoomEx = pRoomEx;
	a1.pOutdoorRooms[0] = pRoomEx->pOutdoor;
	a1.pWallsGrids[0] = &pRoomEx->pOutdoor->pWallGrid;
	a1.pFloorGrid = &pRoomEx->pOutdoor->pFloorGrid;
	a1.field_28 = 0;
	a1.field_2C = 1;

	if (nWaypointSubTheme)
	{
		a1.nSubTheme = 0;
		a1.nSubWaypoint_Shrine = pLevelDefBinRecord->dwSubWaypoint;
		a1.nSubThemePicked = nWaypointSubTheme;
		sub_6FD8AA80(&a1);
	}

	if (nShrineSubTheme)
	{
		a1.nSubTheme = 0;
		a1.nSubWaypoint_Shrine = pLevelDefBinRecord->dwSubShrine;
		a1.nSubThemePicked = nShrineSubTheme;
		sub_6FD8AA80(&a1);
	}

	a1.nSubWaypoint_Shrine = pRoomEx->pOutdoor->nSubType;
	a1.nSubTheme = pRoomEx->pOutdoor->nSubTheme;
	a1.nSubThemePicked = pRoomEx->pOutdoor->nSubThemePicked;
	sub_6FD8AA80(&a1);

	unsigned int nFlags = 0;
	switch (pRoomEx->pLevel->nLevelType)
	{
	case LVLTYPE_ACT2_DESERT:
		nFlags = 0x100;
		break;

	case LVLTYPE_ACT3_JUNGLE:
		nFlags = 0x120000;
		break;

	case LVLTYPE_ACT3_KURAST:
		nFlags = 0x100000;
		break;

	case LVLTYPE_ACT4_MESA:
		nFlags = 0xA00000;
		break;

	case LVLTYPE_ACT4_LAVA:
		nFlags = 0x1600000;
		break;

	case LVLTYPE_ACT5_BARRICADE:
		nFlags = (pRoomEx->pLevel->nLevelId == LEVEL_TUNDRAWASTELANDS) ? 0x600000 : 0;
		break;

	default:
		break;
	}

	for (int nY = 0; nY < nHeight; ++nY)
	{
		for (int nX = 0; nX < nWidth; ++nX)
		{
			if (!(DRLGGRID_GetGridEntry(&pRoomEx->pOutdoor->pFloorGrid, nX, nY) & 0x3F0FF80))
			{
				DRLGGRID_AlterGridFlag(&pRoomEx->pOutdoor->pFloorGrid, nX, nY, nFlags, FLAG_OPERATION_OR);
			}
		}
	}

	DRLGGRID_AlterEdgeGridFlags(&pRoomEx->pOutdoor->pWallGrid, 4, FLAG_OPERATION_OR);
	DRLGGRID_AlterEdgeGridFlags(&pRoomEx->pOutdoor->pFloorGrid, 4, FLAG_OPERATION_OR);
}

//D2Common.0x6FD83C90
void __fastcall DRLGOUTPLACE_CreateOutdoorRoomEx(D2DrlgLevelStrc* pLevel, int nX, int nY, int nWidth, int nHeight, int dwRoomFlags, int dwOutdoorFlags, int dwOutdoorFlagsEx, int dwDT1Mask)
{
	D2LevelDefBin* pLevelDefBinRecord = NULL;
	D2RoomExStrc* pRoomEx = NULL;

	pRoomEx = DRLGROOM_AllocRoomEx(pLevel, DRLGTYPE_MAZE);
	pRoomEx->nTileWidth = nWidth;
	pRoomEx->nTileHeight = nHeight;
	pRoomEx->nTileXPos = nX;
	pRoomEx->nTileYPos = nY;

	DRLGROOM_AddRoomExToLevel(pLevel, pRoomEx);

	pRoomEx->dwDT1Mask = dwDT1Mask;
	pRoomEx->dwFlags |= dwRoomFlags | ROOMEXFLAG_NO_LOS_DRAW;

	pRoomEx->pOutdoor->dwFlags = dwOutdoorFlags;
	pRoomEx->pOutdoor->dwFlagsEx = dwOutdoorFlagsEx;

	pLevelDefBinRecord = DATATBLS_GetLevelDefRecord(pLevel->nLevelId);
	pRoomEx->pOutdoor->nSubType = pLevelDefBinRecord->dwSubType;
	pRoomEx->pOutdoor->nSubTheme = pLevelDefBinRecord->dwSubTheme;
	pRoomEx->pOutdoor->nSubThemePicked = DRLGTILESUB_PickSubThemes(pRoomEx, pLevelDefBinRecord->dwSubType, pLevelDefBinRecord->dwSubTheme);
}
