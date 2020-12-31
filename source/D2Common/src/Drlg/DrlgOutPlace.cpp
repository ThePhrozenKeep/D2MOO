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
#include "Path/PathMisc.h"
#include "D2Seed.h"
#include <assert.h>

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
						if (!(DRLGGRID_GetGridFlags(&pLevel->pOutdoors->pGrid[2], v24, v25) & 0x200))
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
			if (pLevel->pOutdoors->unk0x68[nVertexId])
			{
				pDrlgVertex = DRLGVER_AllocVertex(pLevel->pDrlg->pMempool, 0);
				pPreviousVertex->pNext = pDrlgVertex;
			}
			else
			{
				pDrlgVertex = DRLGVER_AllocVertex(pLevel->pDrlg->pMempool, 0);
				pLevel->pOutdoors->unk0x68[nVertexId] = pDrlgVertex;
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
		pLevel->pOutdoors->unk0x68[nVertexId] = DRLGVER_AllocVertex(pLevel->pDrlg->pMempool, 0);
		pLevel->pOutdoors->unk0x68[nVertexId]->nPosX = nX1;
		pLevel->pOutdoors->unk0x68[nVertexId]->nPosY = nY1;

		pLevel->pOutdoors->unk0x68[nVertexId]->pNext = DRLGVER_AllocVertex(pLevel->pDrlg->pMempool, 0);
		pLevel->pOutdoors->unk0x68[nVertexId]->pNext->nPosX = nX2;
		pLevel->pOutdoors->unk0x68[nVertexId]->pNext->nPosY = nY2;
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
	assert(lutIndex >= 0 && lutIndex <= _countof(nBorderIndices));
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
	assert(lutIndex >= 0 && lutIndex <= _countof(nBorderIndices));

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

		for (int j = nY; !(DRLGGRID_GetGridFlags(pDrlgGrid, nX, j) & 1); j += nOffsetY)
		{
			for (int i = nX; !(DRLGGRID_GetGridFlags(pDrlgGrid, i, j) & 1); i += nOffsetX)
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

		pLevel = sub_6FD82820(pLevel);

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


int dword_6FDD0828[] = { 0, 545, 546, 547, 548, 0, 549, 550, 0, 0, 551, 552, 553, 554, 0, 555, 0, 556, 0, 557, 558, 0, 0, 559, 0, 0, 0, 560, 561, 562, 563, 0, 0, 564, 565, 0, 566, 0, 567, 0, 0, 0, 568, 0, 569, 570, 0, 0, 0, 571, 0, 0, 572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int dword_6FDD0924[] = { 0, 575, 576, 577, 578, 579, 580, 0, 581, 582, 583, 0, 584, 0, 0, 0, 0 };

////----- (6FD82820) --------------------------------------------------------
////TODO: Finish
//D2DrlgLevelStrc* __fastcall sub_6FD82820(D2DrlgLevelStrc* pLevel)
//{
//	D2LevelDefBin *pLevelDef; // eax@1
//	unsigned int nBaseOn; // ebx@3
//	int *v15; // edi@13
//	int v16; // ecx@13
//	bool v19; // zf@23
//	signed int v20; // esi@27
//	bool v21; // zf@27
//	int v22; // eax@31
//	int v23; // esi@31
//	__int64 v32; // qax@44
//	signed int v33; // ecx@44
//	__int64 v34; // qax@44
//	int v35; // edi@44
//	int v37; // ebx@49
//	int v38; // edi@51
//	int v39; // eax@51
//	int v40; // ebx@54
//	int v41; // ecx@56
//	int v42; // ebx@56
//	bool v43; // sf@56
//	signed int v44; // ebx@56
//	int v45; // eax@61
//	unsigned int v46; // ebp@66
//	int v47; // ecx@67
//	int m; // eax@67
//	signed __int64 v49; // qax@71
//	unsigned __int8 v50; // of@75
//	int v51; // eax@77
//	int v52; // edx@77
//	int v53; // eax@77
//	signed int v54; // edi@83
//	int v59; // esi@89
//	int n; // ecx@91
//	int v61; // eax@93
//	int v62; // ebp@103
//	signed int v63; // ebx@104
//	signed int v64; // edi@104
//	int v65; // ecx@105
//	signed int v66; // eax@105
//	int v67; // eax@109
//	signed int v68; // esi@109
//	signed int v69; // edx@118
//	int v70; // esi@120
//	int v71; // ecx@143
//	int v72; // eax@144
//	int v73; // eax@148
//	int v74; // eax@152
//	int v75; // ecx@156
//	int jj; // ecx@162
//	int v77; // esi@165
//	int v78; // eax@165
//	int v79; // edx@165
//	D2DrlgStrc *v80; // ecx@165
//	int v81; // ebx@167
//	int v82; // edi@167
//	signed __int64 v83; // qax@167
//	int v84; // ebp@167
//	int v85; // eax@167
//	signed int v86; // esi@168
//	signed int v87; // edx@168
//	int v88; // ecx@171
//	signed int v89; // ecx@172
//	int v90; // ecx@175
//	signed int v91; // ecx@176
//	signed int v92; // ecx@180
//	signed int v93; // ecx@184
//	D2DrlgStrc *v94; // ecx@189
//	signed int v95; // edx@191
//	signed int v96; // esi@191
//	int v97; // ecx@194
//	int v98; // ecx@197
//	int v100; // esi@210
//	int v102; // eax@212
//	signed int v103; // edx@212
//	signed int v104; // ecx@215
//	signed int v105; // ecx@219
//	signed int v106; // ecx@223
//	signed int v107; // ecx@227
//	int v108; // eax@234
//	int v109; // eax@236
//	int v110; // edx@236
//	int v111; // eax@238
//	int v112; // eax@240
//	int v113; // edx@242
//	int v114; // ebx@247
//	void *v115; // ebp@247
//	signed int *v116; // edi@249
//	signed int v117; // eax@250
//	int nPDef; // esi@250
//	int v119; // eax@270
//	int nCounter; // ecx@271
//	int v123; // edx@272
//	int v124; // eax@273
//	bool bNotSorted; // ecx@280
//	char *v126; // eax@280
//	D2DrlgLevelStrc *pJungleLevel; // esi@286
//	signed int v135; // [sp+44h] [bp-14Ch]@54
//	int v136; // [sp+44h] [bp-14Ch]@88
//	int ii; // [sp+44h] [bp-14Ch]@101
//	int v138; // [sp+44h] [bp-14Ch]@163
//	unsigned int pDrlgCoord2a; // [sp+48h] [bp-148h]@66
//	int pDrlgCoord2b; // [sp+48h] [bp-148h]@88
//	int pDrlgCoord2c; // [sp+48h] [bp-148h]@103
//	int pDrlgCoord2d; // [sp+48h] [bp-148h]@165
//	int nMaxX; // [sp+4Ch] [bp-144h]@1
//	int nMaxXa; // [sp+4Ch] [bp-144h]@49
//	D2UnkOutPlaceStrc **nMaxXb; // [sp+4Ch] [bp-144h]@57
//	int nMaxXc; // [sp+4Ch] [bp-144h]@66
//	unsigned int nMaxXd; // [sp+4Ch] [bp-144h]@84
//	int nMaxXe; // [sp+4Ch] [bp-144h]@103
//	int nMaxXf; // [sp+4Ch] [bp-144h]@167
//	int v151; // [sp+50h] [bp-140h]@31
//	int v152; // [sp+54h] [bp-13Ch]@54
//	int v153; // [sp+54h] [bp-13Ch]@102
//	int v154; // [sp+54h] [bp-13Ch]@164
//	int v155; // [sp+58h] [bp-138h]@44
//	int v156; // [sp+58h] [bp-138h]@103
//	int v157; // [sp+58h] [bp-138h]@165
//	int nSizeY; // [sp+5Ch] [bp-134h]@1
//	int nSizeYa; // [sp+5Ch] [bp-134h]@44
//	signed int nSizeYb; // [sp+5Ch] [bp-134h]@104
//	int nSizeYc; // [sp+5Ch] [bp-134h]@165
//	int *v163; // [sp+60h] [bp-130h]@31
//	int *pFree; // [sp+64h] [bp-12Ch]@31
//	int pDrlgCoord; // [sp+68h] [bp-128h]@47
//	int pDrlgCoorda; // [sp+68h] [bp-128h]@103
//	int pDrlgCoordb; // [sp+68h] [bp-128h]@167
//	signed int v169; // [sp+6Ch] [bp-124h]@7
//	int v170; // [sp+6Ch] [bp-124h]@32
//	signed int v171; // [sp+70h] [bp-120h]@45
//	int v172; // [sp+70h] [bp-120h]@104
//	int *v173; // [sp+74h] [bp-11Ch]@31
//	D2DrlgStrc *pDrlg; // [sp+78h] [bp-118h]@1
//	int v175; // [sp+7Ch] [bp-114h]@1
//	int *v176; // [sp+80h] [bp-110h]@31
//	int v179; // [sp+88h] [bp-108h]@1
//	int nMaxY; // [sp+8Ch] [bp-104h]@1
//	int v181; // [sp+8Ch] [bp-104h]@69
//	int v182; // [sp+8Ch] [bp-104h]@103
//	int nSizeX; // [sp+90h] [bp-100h]@1
//	int nSizeXa; // [sp+90h] [bp-100h]@103
//	int v185; // [sp+94h] [bp-FCh]@44
//	int v186; // [sp+94h] [bp-FCh]@103
//	int nMinX; // [sp+98h] [bp-F8h]@1
//	int nMinY; // [sp+9Ch] [bp-F4h]@1
//	D2UnkOutPlaceStrc pJungles[3] = {};
//	D2UnkOutPlaceStrc pTmp = {};
//
//	pLevelDef = DATATBLS_GetLevelDefRecord(LEVEL_SPIDERFOREST);
//	pDrlg = pLevel->pDrlg;
//	nSizeY = pLevelDef->dwSizeY[pLevel->pDrlg->nDifficulty];
//	nSizeX = pLevelDef->dwSizeX[pLevel->pDrlg->nDifficulty];
//	v179 = nSizeX / 32;
//	v175 = nSizeY / 32;
//	nMinX = pLevel->nPosX;
//	nMaxY = pLevel->nPosY;
//	nMinY = nMaxY - nSizeY;
//	nMaxX = nSizeX + nMinX;
//
//	sub_6FD83970(&pLevel->pLevelCoords, pJungles, 0, nSizeX, nSizeY);
//
//	for (int i = 1; i < 3; ++i)
//	{
//		nBaseOn = SEED_RollLimitedRandomNumber(&pDrlg->pSeed, i);
//
//		sub_6FD83970(&pJungles[nBaseOn].pDrlgCoord, &pJungles[i], (unsigned int)SEED_RollRandomNumber(&pDrlg->pSeed) % 5, nSizeX, nSizeY);
//
//		v169 = 0;
//		while (1)
//		{
//			if (!sub_6FD777B0(&pJungles[v169].pDrlgCoord, &pJungles[i].pDrlgCoord, 0))
//			{
//				break;
//			}
//
//			++v169;
//			if (v169 >= i)
//			{
//				goto LABEL_13;
//			}
//		}
//		if (v169 < i)
//		{
//			continue;
//		}
//
//LABEL_13:
//		pJungles[nBaseOn].field_1C[pJungles[nBaseOn].nBranch] = &pJungles[i];
//
//		pJungles[i].field_18 = &pJungles[nBaseOn];
//		++pJungles[nBaseOn].nBranch;
//
//		if (pJungles[nBaseOn].nBranch > 3)
//		{
//			FOG_10025("ptJungle[nBaseOn].nBranch <= JUNGLE_MAX_ATTACH", "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1570);
//		}
//
//		if (nMinX > pJungles[i].pDrlgCoord.nPosX)
//		{
//			nMinX = pJungles[i].pDrlgCoord.nPosX;
//		}
//
//		if (nMinY > pJungles[i].pDrlgCoord.nPosY)
//		{
//			nMinY = pJungles[i].pDrlgCoord.nPosY;
//		}
//
//		if (nMaxX < pJungles[i].pDrlgCoord.nWidth + pJungles[i].pDrlgCoord.nPosX)
//		{
//			nMaxX = pJungles[i].pDrlgCoord.nWidth + pJungles[i].pDrlgCoord.nPosX;
//		}
//	}
//
//	v19 = ((nMaxX - nMinX) & 0x8000001F) == 0;
//	if (((nMaxX - nMinX) & 0x8000001F) < 0)
//	{
//		v19 = ((((nMaxX - nMinX) & 0x8000001F) - 1) | 0xFFFFFFE0) == -1;
//	}
//
//	if (!v19)
//	{
//		FOG_10025("(nMaxX - nMinX) % nPreset == 0", "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1585);
//	}
//
//	v20 = nMaxY - nMinY;
//	v21 = ((nMaxY - nMinY) & 0x8000001F) == 0;
//	if (((nMaxY - nMinY) & 0x8000001F) < 0)
//	{
//		v21 = ((((nMaxY - nMinY) & 0x8000001F) - 1) | 0xFFFFFFE0) == -1;
//	}
//
//	if (!v21)
//	{
//		FOG_10025("(nMaxY - nMinY) % nPreset == 0", "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1586);
//	}
//
//	v22 = v20 / 32 + 2;
//	v151 = (nMaxX - nMinX) / 32 + 2;
//	v23 = 4 * v151 * v22;
//
//	pFree = (int*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, v23, "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1596, 0);
//	v176 = (int*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, v23, "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1597, 0);
//	v173 = (int*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, v23, "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1598, 0);
//	v163 = (int*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, v23, "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1599, 0);
//
//LABEL_32:
//	memset(pFree, 0, v23);
//	memset(v176, 0, v23);
//	memset(v173, 0, v23);
//	memset(v163, 0, v23);
//
////	v170 = 0;
////	do
////	{
////		v185 = v170;
////		v32 = pJungles[v170].pDrlgCoord.nPosX - nMinX;
////		v33 = (BYTE4(v32) & 0x1F) + v32;
////		v34 = pJungles[v170].pDrlgCoord.nPosY - nMinY;
////		nSizeYa = (v33 >> 5) + 1;
////		v35 = (((BYTE4(v34) & 0x1F) + (signed int)v34) >> 5) + 1;
////		pJungles[v170].field_28 = nSizeYa;
////		v155 = v35;
////		pJungles[v170].field_2C = v35;
////		pJungles[v170].pJungleDefs = (int*)FOG_AllocServerMemory(pLevel->pDrlg->pMempool, 4 * v179 * v175, "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1619, 0);
////		if (v170)
////		{
////			v171 = pJungles[v170].field_10 % 2 == 1;
////		}
////		else
////		{
////			v171 = SEED_RollRandomNumber(&pDrlg->pSeed) & 1;
////		}
////
////		pDrlgCoord = v35 + v175 - 1;
////		do
////		{
////			if (v175 > 0)
////			{
////				nMaxXa = v175;
////				v37 = (int)((char *)v176 + 4 * (nSizeYa + v151 * v155));
////				do
////				{
////					if (v179 > 0)
////					{
////						v38 = v179;
////						v39 = v37;
////						do
////						{
////							*(_DWORD *)(pFree - v176 + v39) = v170 + 1;
////							*(_DWORD *)v39 = 0;
////							*(_DWORD *)(v173 - v176 + v39) = 0;
////							*(_DWORD *)(v163 - v176 + v39) = 0;
////							v39 += 4;
////							--v38;
////						}
////						while (v38);
////					}
////					v37 += 4 * v151;
////					--nMaxXa;
////				}
////				while (nMaxXa);
////			}
////
////			v40 = nSizeYa + v171;
////			v135 = pDrlgCoord;
////			v152 = nSizeYa + v171;
////			if (v170)
////			{
////				*((_DWORD *)v173 + pDrlgCoord * v151 + v40) = 1;
////				*((_DWORD *)v173 + v40 + pDrlgCoord * v151 + 2 * (v171 == 0) - 1) = 2;
////			}
////
////			v41 = 0;
////			v42 = pJungles[v185].nBranch;
////			v19 = v42 == 0;
////			v43 = v42 < 0;
////			v44 = v155;
////			if (!(v43 | v19))
////			{
////				nMaxXb = pJungles[v185].field_1C;
////				do
////				{
////					switch ((*nMaxXb)->field_10)
////					{
////					case 0:
////						*((_DWORD *)v173 + nSizeYa + v151 * v155) = 1;
////						break;
////					case 1:
////						*((_DWORD *)v173 + nSizeYa + v151 * (v155 + 3)) = 1;
////						break;
////					case 2:
////						v45 = v155 + 3;
////						*((_DWORD *)v173 + nSizeYa + v151 * v45 + 1) = 1;
////						break;
////					case 3:
////						*((_DWORD *)v173 + nSizeYa + v151 * (v155 + 1)) = 1;
////						break;
////					case 4:
////						v45 = v155 + 1;
////						*((_DWORD *)v173 + nSizeYa + v151 * v45 + 1) = 1;
////						break;
////					default:
////						break;
////					}
////					++v41;
////					++nMaxXb;
////				}
////				while (v41 < pJungles[v185].nBranch);
////			}
////
////			v46 = v170 != 0;
////			pDrlgCoord2a = v170 != 0;
////			nMaxXc = 0;
////			if (pDrlgCoord >= v155)
////			{
////				v47 = pDrlgCoord * v151;
////				for (m = 20 * (5 * v170 + 5); ; m = v181)
////				{
////					*((_DWORD *)v176 + v47 + v152) = m;
////					v181 = m + 1;
////					if (!nMaxXc
////						|| v135 == v44
////						|| (v49 = pDrlg->pSeed.nHighSeed + 1791398085i64 * pDrlg->pSeed.nLowSeed, *(_QWORD *)&pDrlg->pSeed.nLowSeed = v49, v44 = v155, (unsigned int)v49 % 3))
////					{
////						v46 = pDrlgCoord2a;
////						v50 = __OFSUB__(nMaxXc + 1, 2);
////						v43 = nMaxXc++ - 1 < 0;
////						if (!(v43 ^ v50))
////						{
////							if (v135 > 1)
////							{
////								v51 = v152 + v47 + 2 * (v171 == 0) - 1;
////								v52 = *((_DWORD *)v173 + v51);
////								v53 = (int)((char *)v173 + 4 * v51);
////								if (!v52)
////								{
////									v46 = pDrlgCoord2a + 1;
////									*(_DWORD *)v53 = 2;
////									++pDrlgCoord2a;
////								}
////							}
////						}
////						v47 -= v151;
////						--v135;
////					}
////					else
////					{
////						v46 = pDrlgCoord2a;
////						nMaxXc = 0;
////						if (v171)
////						{
////							v171 = 0;
////							--v152;
////						}
////						else
////						{
////							v171 = 1;
////							++v152;
////						}
////					}
////					if (v135 < v44)
////						break;
////				}
////			}
////		}
////		while ((signed int)v46 < 2);
////
////		while ((signed int)v46 > 3)
////		{
////			v54 = 0;
////			nMaxXd = SEED_RollLimitedRandomNumber(&pDrlg->pSeed, v46);
////
////			pDrlgCoord2b = 0;
////			v136 = 0;
////			if (v175 > 0)
////			{
////				v59 = nSizeYa + v151 * v155;
////				do
////				{
////					if (v54)
////						break;
////					for (n = 0; n < v179; ++n)
////					{
////						if (v54)
////							break;
////						v61 = (int)((char *)v173 + 4 * (v59 + n));
////						if (*(_DWORD *)v61 == 2)
////						{
////							if (nMaxXd == pDrlgCoord2b)
////							{
////								*(_DWORD *)v61 = 0;
////								v54 = 1;
////								--v46;
////							}
////							++pDrlgCoord2b;
////						}
////					}
////					v59 += v151;
////					++v136;
////				}
////				while (v136 < v175);
////			}
////		}
////		v50 = __OFSUB__(v170 + 1, 3);
////		v43 = v170++ - 2 < 0;
////	}
////	while (v43 ^ v50);
////
////	for (ii = 0; ii < v22; ++ii)
////	{
////		v153 = 0;
////		if (v151 > 0)
////		{
////			nMaxXe = (int)((char *)pFree + 4 * ii * v151);
////			pDrlgCoord2c = (int)((char *)v176 + 4 * ii * v151 + 4);
////			v62 = (int)((char *)v163 + 4 * ii * v151 - 4);
////			v156 = (int)((char *)v163 + 4 * ii * v151 - 4);
////			nSizeXa = pFree - v163;
////			pDrlgCoorda = v176 - v163;
////			v186 = pFree - v176;
////			v182 = v173 - pFree;
////			do
////			{
////				v63 = 0;
////				v64 = 2147483647;
////				nSizeYb = 0;
////				v172 = *(_DWORD *)nMaxXe;
////				if (*(_DWORD *)(v182 + nMaxXe) == 1)
////				{
////					v65 = 4 * (v153 + v151 * (ii - 1));
////					v66 = *(_DWORD *)((char *)v176 + v65);
////					if (v66)
////					{
////						if (v66 < 2147483647)
////						{
////							v62 = v156;
////							if (*(_DWORD *)((char *)pFree + v65) == v172)
////							{
////								v63 = 8;
////								v64 = *(_DWORD *)((char *)v176 + v65);
////								nSizeYb = 8;
////							}
////						}
////					}
////					v67 = 4 * (v153 + v151 * (ii + 1));
////					v68 = *(_DWORD *)((char *)v176 + v67);
////					if (v68)
////					{
////						if (v68 < v64)
////						{
////							v62 = v156;
////							if (*(_DWORD *)((char *)pFree + v67) == v172)
////							{
////								v63 = 4;
////								v64 = *(_DWORD *)((char *)v176 + v67);
////								nSizeYb = 4;
////							}
////						}
////					}
////					if (*(_DWORD *)pDrlgCoord2c && *(_DWORD *)pDrlgCoord2c < v64)
////					{
////						v62 = v156;
////						if (*(_DWORD *)(pDrlgCoord2c + v186) == v172)
////						{
////							v63 = 2;
////							v64 = *(_DWORD *)pDrlgCoord2c;
////						}
////						else
////						{
////							v63 = nSizeYb;
////						}
////					}
////					v69 = *(_DWORD *)(pDrlgCoorda + v62);
////					if (v69 && v69 < v64)
////					{
////						v70 = *(_DWORD *)nMaxXe;
////						if (*(_DWORD *)(nSizeXa + v62) == v172)
////							v63 = 1;
////					}
////					else
////					{
////						v70 = *(_DWORD *)nMaxXe;
////					}
////					if (v63 & 8)
////						*(_DWORD *)((char *)v163 + v65) |= 4u;
////					if (v63 & 4)
////						*(_DWORD *)((char *)v163 + v67) |= 8u;
////					if (v63 & 2)
////						*(_DWORD *)(v62 + 8) |= 1u;
////					if (v63 & 1)
////						*(_DWORD *)v62 |= 2u;
////					if (*(_DWORD *)((char *)v173 + v65) == 1 && *(_DWORD *)((char *)pFree + v65) != v70)
////						v63 |= 8u;
////					if (*(_DWORD *)((char *)v173 + v67) == 1 && *(_DWORD *)((char *)pFree + v67) != v70)
////						v63 |= 4u;
////					if (*(_DWORD *)(pDrlgCoord2c + v186 + v182) == 1 && *(_DWORD *)(pDrlgCoord2c + v186) != v70)
////						v63 |= 2u;
////					if (*(_DWORD *)(v173 - v163 + v62) == 1 && *(_DWORD *)(nSizeXa + v62) != v70)
////						v63 |= 1u;
////				}
////				v71 = *(_DWORD *)(pDrlgCoord2c - 4);
////				if (v71)
////				{
////					v72 = v71 - *((_DWORD *)v176 + v153 + v151 * (ii - 1));
////					if (v72 < 0)
////						v72 = -v72;
////					if (v72 == 1)
////						v63 |= 8u;
////					v73 = v71 - *((_DWORD *)v176 + v153 + v151 * (ii + 1));
////					if (v73 < 0)
////						v73 = -v73;
////					if (v73 == 1)
////						v63 |= 4u;
////					v74 = v71 - *(_DWORD *)pDrlgCoord2c;
////					if (v71 - *(_DWORD *)pDrlgCoord2c < 0)
////						v74 = -v74;
////					if (v74 == 1)
////						v63 |= 2u;
////					v75 = v71 - *(_DWORD *)(pDrlgCoorda + v62);
////					if (v75 < 0)
////						v75 = -v75;
////					if (v75 == 1)
////						v63 |= 1u;
////				}
////				*(_DWORD *)(v62 + 4) |= v63;
////				v62 += 4;
////				++v153;
////				nMaxXe += 4;
////				pDrlgCoord2c += 4;
////				v156 = v62;
////			}
////			while (v153 < v151);
////		}
////	}
////
////	for (jj = 0; ; ++jj)
////	{
////		v138 = jj;
////		if (jj >= v22)
////			break;
////		v154 = 0;
////		if (v151 > 0)
////		{
////			pDrlgCoord2d = jj * v151;
////			v77 = pFree - v173;
////			v78 = (int)((char *)v163 + 4 * jj * v151);
////			v79 = v173 - v163;
////			v80 = pDrlg;
////			v157 = v78;
////			nSizeYc = pFree - v173;
////			while (1)
////			{
////				v81 = *(_DWORD *)v78;
////				nMaxXf = v78 + v79;
////				v82 = *(_DWORD *)(v78 + v79 + v77);
////				v83 = v80->pSeed.nHighSeed + 1791398085i64 * v80->pSeed.nLowSeed;
////				*(_QWORD *)&v80->pSeed.nLowSeed = v83;
////				v84 = v83 & 3;
////				pDrlgCoordb = v83 & 3;
////				v85 = v81 == 0;
////				if (*(_DWORD *)nMaxXf == 2)
////				{
////					v86 = 0;
////					v87 = 1;
////					do
////					{
////						if (!v87)
////							break;
////						switch ((v86 + v84) % 4)
////						{
////						case 0:
////							v88 = 4 * (v154 + v151 * (v138 - 1));
////							if (*(_DWORD *)((char *)pFree + v88) == v82)
////							{
////								v89 = *(_DWORD *)((char *)v163 + v88);
////								if (v89)
////								{
////									if (v89 < 15)
////									{
////										LOBYTE(v81) = v81 | 0x80;
////										goto LABEL_187;
////									}
////								}
////							}
////							break;
////						case 1:
////							v90 = 4 * (v154 + v151 * (v138 + 1));
////							if (*(_DWORD *)((char *)pFree + v90) == v82)
////							{
////								v91 = *(_DWORD *)((char *)v163 + v90);
////								if (v91)
////								{
////									if (v91 < 15)
////									{
////										v81 |= 0x40u;
////										goto LABEL_187;
////									}
////								}
////							}
////							break;
////						case 2:
////							if (*((_DWORD *)pFree + pDrlgCoord2d + 1) == v82)
////							{
////								v92 = *((_DWORD *)v163 + pDrlgCoord2d + 1);
////								if (v92)
////								{
////									if (v92 < 15)
////									{
////										v81 |= 0x20u;
////										goto LABEL_187;
////									}
////								}
////							}
////							break;
////						case 3:
////							if (*(_DWORD *)(nMaxXf + nSizeYc - 4) == v82)
////							{
////								v93 = *(_DWORD *)(v157 - 4);
////								if (v93)
////								{
////									if (v93 < 15)
////									{
////										v81 |= 0x10u;
////LABEL_187:
////										v87 = 0;
////									}
////								}
////							}
////							break;
////						default:
////							break;
////						}
////						v84 = pDrlgCoordb;
////						++v86;
////					}
////					while (v86 < 4);
////					v94 = pDrlg;
////					if (!v81)
////					{
////						goto LABEL_32;
////					}
////					if (v85)
////					{
////						v95 = 0;
////						v96 = 1;
////						do
////						{
////							if (!v96)
////								break;
////							switch ((v95 + v84) % 4)
////							{
////							case 0:
////								v97 = 4 * (v154 + v151 * (v138 - 1));
////								if (*(_DWORD *)((char *)pFree + v97) != v82 && *(_DWORD *)((char *)v173 + v97) == 2)
////								{
////									LOBYTE(v81) = v81 | 0x80;
////									goto LABEL_206;
////								}
////								break;
////							case 1:
////								v98 = 4 * (v154 + v151 * (v138 + 1));
////								if (*(_DWORD *)((char *)pFree + v98) != v82 && *(_DWORD *)((char *)v173 + v98) == 2)
////								{
////									v81 |= 0x40u;
////									goto LABEL_206;
////								}
////								break;
////							case 2:
////								if (*((_DWORD *)pFree + pDrlgCoord2d + 1) != v82
////									&& *((_DWORD *)v173 + pDrlgCoord2d + 1) == 2)
////								{
////									v81 |= 0x20u;
////									goto LABEL_206;
////								}
////								break;
////							case 3:
////								if (*(_DWORD *)(nMaxXf + nSizeYc - 4) != v82 && *(_DWORD *)(nMaxXf - 4) == 2)
////								{
////									v81 |= 0x10u;
////LABEL_206:
////									v96 = 0;
////								}
////								break;
////							default:
////								break;
////							}
////							v94 = pDrlg;
////							++v95;
////						}
////						while (v95 < 4);
////
////						v100 = SEED_RollRandomNumber(&v94->pSeed) & 1 && v96;
////						v102 = SEED_RollRandomNumber(&v94->pSeed) & 3;
////						v103 = 0;
////						do
////						{
////							if (!v100)
////								break;
////							switch ((v102 + v103) % 4)
////							{
////							case 0:
////								v104 = *((_DWORD *)v163 + v154 + v151 * (v138 - 1));
////								if (v104 && v104 < 15 && (char)v81 >= 0)
////								{
////									LOBYTE(v81) = v81 | 0x80;
////									goto LABEL_231;
////								}
////								break;
////							case 1:
////								v105 = *((_DWORD *)v163 + v154 + v151 * (v138 + 1));
////								if (v105 && v105 < 15 && !(v81 & 0x40))
////								{
////									v81 |= 0x40u;
////									goto LABEL_231;
////								}
////								break;
////							case 2:
////								v106 = *((_DWORD *)v163 + pDrlgCoord2d + 1);
////								if (v106 && v106 < 15 && !(v81 & 0x20))
////								{
////									v81 |= 0x20u;
////									goto LABEL_231;
////								}
////								break;
////							case 3:
////								v107 = *(_DWORD *)(v157 - 4);
////								if (v107 && v107 < 15 && !(v81 & 0x10))
////								{
////									v81 |= 0x10u;
////LABEL_231:
////									v100 = 0;
////								}
////								break;
////							default:
////								break;
////							}
////							++v103;
////						}
////						while (v103 < 4);
////					}
////					if ((char)v81 < 0)
////					{
////						v108 = (int)((char *)v163 + 4 * (v154 + v151 * (v138 - 1)));
////						*(_DWORD *)v108 |= 0x40u;
////					}
////					if (v81 & 0x40)
////					{
////						v109 = (int)((char *)v163 + 4 * (v154 + v151 * (v138 + 1)));
////						v110 = *(_DWORD *)v109;
////						LOBYTE(v110) = *(_DWORD *)v109 | 0x80;
////						*(_DWORD *)v109 = v110;
////					}
////					if (v81 & 0x20)
////					{
////						v111 = *(_DWORD *)(v157 + 4);
////						LOBYTE(v111) = v111 | 0x10;
////						*(_DWORD *)(v157 + 4) = v111;
////					}
////					if (v81 & 0x10)
////					{
////						v112 = *(_DWORD *)(v157 - 4);
////						LOBYTE(v112) = v112 | 0x20;
////						*(_DWORD *)(v157 - 4) = v112;
////					}
////					*(_DWORD *)nMaxXf = 0;
////					v80 = pDrlg;
////				}
////				v78 = v157 + 4;
////				v113 = v81 | *(_DWORD *)v157;
////				v157 = v78;
////				*(_DWORD *)(v78 - 4) = v113;
////				v50 = __OFSUB__(v154 + 1, v151);
////				v43 = v154++ + 1 - v151 < 0;
////				++pDrlgCoord2d;
////				if (!(v43 ^ v50))
////				{
////					jj = v138;
////					break;
////				}
////				v77 = pFree - v173;
////				v79 = v173 - v163;
////			}
////		}
////	}
//
//	v114 = v151;
//	v115 = v163;
//
//	for (int i = v22; i > 0; --i)
//	{
//		v116 = (signed int *)v115;
//
//		while (v114 > 0)
//		{
//			v117 = *v116;
//			nPDef = *v116 % 16;
//			if (nPDef)
//			{
//				if (v117 < 16)
//				{
//					nPDef += 529;
//				}
//				else
//				{
//					if (v117 & 0x10)
//					{
//						nPDef = dword_6FDD0828[4 * nPDef - 4];
//					}
//
//					if (v117 & 0x20)
//					{
//						nPDef = dword_6FDD0828[4 * nPDef - 3];
//					}
//
//					if (v117 & 0x40)
//					{
//						nPDef = dword_6FDD0828[4 * nPDef - 2];
//					}
//
//					if (v117 & 0x80)
//					{
//						nPDef = dword_6FDD0828[4 * nPDef - 1];
//					}
//
//					if (!nPDef)
//					{
//						FOG_10025("nPDef != PRESET_DEF_NONE", "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1904);
//					}
//				}
//			}
//			else
//			{
//				if (v117 >= 16)
//				{
//					nPDef = dword_6FDD0924[v117 >> 4];
//					if (!nPDef)
//					{
//						FOG_10025("nPDef != PRESET_DEF_NONE", "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1917);
//					}
//				}
//			}
//
//			*v116 = nPDef;
//			++v116;
//
//			--v114;
//		}
//
//		v114 = v151;
//		v115 = (char *)v115 + 4 * v114;
//	}
//
//	for (int i = 0; i < 3; ++i)
//	{
//		nCounter = 0;
//		for (int j = 0; j < v175; ++j)
//		{
//			for (int k = 0; k < v179; ++k)
//			{
//				pJungles[i].pJungleDefs[nCounter] = v163[k + pJungles[i].field_28 + v151 * (j + pJungles[i].field_2C)];
//				if (pJungles[i].pJungleDefs[nCounter] > 574)
//				{
//					++pJungles[i].nJungleDefs;
//				}
//				++nCounter;
//			}
//		}
//	}
//
//	do
//	{
//		bNotSorted = false;
//		for (int i = 0; i < 2; ++i)
//		{
//			if (pJungles[i + 1].pDrlgCoord.nPosY > pJungles[i].pDrlgCoord.nPosY)
//			{
//				memcpy(&pTmp, &pJungles[i + 1], sizeof(D2UnkOutPlaceStrc));
//				memcpy(&pJungles[i + 1], &pJungles[i], sizeof(D2UnkOutPlaceStrc));
//				memcpy(&pJungles[i], &pTmp, sizeof(D2UnkOutPlaceStrc));
//				bNotSorted = true;
//			}
//		}
//	}
//	while (bNotSorted);
//
//	for (int i = 0; i < 3; ++i)
//	{
//		pJungleLevel = DRLG_GetLevel(pDrlg, i + LEVEL_SPIDERFOREST);
//
//		pJungleLevel->pJungleDefs = pJungles[i].pJungleDefs;
//		pJungleLevel->nJungleDefs = pJungles[i].nJungleDefs;
//		pJungleLevel->nPosX = pJungles[i].pDrlgCoord.nPosX;
//		pJungleLevel->nPosY = pJungles[i].pDrlgCoord.nPosY;
//		pJungleLevel->nWidth = pJungles[i].pDrlgCoord.nWidth;
//		pJungleLevel->nHeight = pJungles[i].pDrlgCoord.nHeight;
//	}
//
//	FOG_FreeServerMemory(pLevel->pDrlg->pMempool, pFree, "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1974, 0);
//	FOG_FreeServerMemory(pLevel->pDrlg->pMempool, v176, "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1975, 0);
//	FOG_FreeServerMemory(pLevel->pDrlg->pMempool, v173, "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1976, 0);
//	FOG_FreeServerMemory(pLevel->pDrlg->pMempool, v163, "C:\\projects\\D2\\head\\Diablo2\\Source\\D2Common\\DRLG\\OutPlace.cpp", 1977, 0);
//
//	return pJungleLevel;
//}


//TODO:Remove
D2DrlgLevelStrc* __fastcall sub_6FD82820(D2DrlgLevelStrc* pLevel)
{
	REMOVE_LATER_WriteToLogFile("sub_6FD82820: Not yet implemented");
	return 0;
}
/////////////



//D2Common.0x6FD83970
void __fastcall sub_6FD83970(D2DrlgCoordStrc* pDrlgCoord, D2UnkOutPlaceStrc* a2, int nRand, int nSizeX, int nSizeY)
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

	a2->pDrlgCoord.nPosX = nX + pDrlgCoord->nPosX;
	a2->pDrlgCoord.nHeight = nSizeY;
	a2->pDrlgCoord.nWidth = nSizeX;
	a2->pDrlgCoord.nPosY = nY + pDrlgCoord->nPosY;
	a2->field_10 = nRand;
}

//D2Common.0x6FD83A20
void __fastcall DRLGOUTPLACE_InitOutdoorRoomGrids(D2RoomExStrc* pRoomEx)
{
	D2LevelDefBin* pLevelDefBinRecord = NULL;
	unsigned int nFlags = 0;
	int nWaypointSubTheme = 0;
	int nShrineSubTheme = 0;
	int nHeight = 0;
	int nWidth = 0;
	D2UnkOutdoorStrc2 a1 = {};

	pLevelDefBinRecord = DATATBLS_GetLevelDefRecord(pRoomEx->pLevel->nLevelId);

	nWaypointSubTheme = (pRoomEx->dwFlags & (ROOMEXFLAG_HAS_WAYPOINT|ROOMEXFLAG_HAS_WAYPOINT_SMALL)) >> 16;
	nShrineSubTheme = (uint16_t)pRoomEx->dwFlags >> 12;

	nWidth = pRoomEx->nTileWidth + 1;
	nHeight = pRoomEx->nTileHeight + 1;

	DRLGGRID_InitializeGridCells(pRoomEx->pLevel->pDrlg->pMempool, &pRoomEx->pOutdoor->pOrientationGrid, nWidth, nHeight);
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
		sub_6FD7EFE0(pRoomEx->pLevel, pRoomEx);
	}

	DRLGROOMTILE_AllocTileGrid(pRoomEx);

	memset(&a1, 0, sizeof(a1));
	a1.pRoomEx = pRoomEx;
	a1.pOutdoorRoom = pRoomEx->pOutdoor;
	a1.pWallGrid = &pRoomEx->pOutdoor->pWallGrid;
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

	for (int i = 0; i < nHeight; ++i)
	{
		for (int j = 0; j < nWidth; ++j)
		{
			if (!(DRLGGRID_GetGridFlags(&pRoomEx->pOutdoor->pFloorGrid, j, i) & 0x3F0FF80))
			{
				DRLGGRID_AlterGridFlag(&pRoomEx->pOutdoor->pFloorGrid, j, i, nFlags, FLAG_OPERATION_OR);
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
