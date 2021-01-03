#pragma once

#include "CommonDefinitions.h"
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"

struct D2DrlgOutdoorGridStrc
{
	int32_t dwFlags;						//0x00
	D2DrlgGridStrc* pSectors;				//0x04
	int32_t nWidth;							//0x08
	int32_t nHeight;						//0x0C
	BOOL bInit;								//0x10
};

struct D2DrlgOutdoorInfoStrc
{
	uint32_t dwFlags;							//0x00
	D2DrlgGridStrc pGrid[4];				//0x04
	union
	{
		struct
		{
			int32_t nWidth;								//0x54
			int32_t nHeight;							//0x58
			int32_t nGridWidth;							//0x5C
			int32_t nGridHeight;						//0x60
		};
		D2DrlgCoordStrc pCoord;
	};
	D2DrlgVertexStrc* pVertex;				//0x64
	D2DrlgVertexStrc* unk0x68[6];			//0x68
	D2DrlgVertexStrc pVertices[24];			//0x80
	int32_t nVertices;							//0x260
	D2DrlgOrthStrc* pRoomData;				//0x264
};

//D2Common.0x6FD7DC20
int __fastcall DRLGOUTDOORS_GetOutLinkVisFlag(D2DrlgLevelStrc* pLevel, D2DrlgVertexStrc* pDrlgVertex);
//D2Common.0x6FD7DD00
BOOL __fastcall DRLGOUTDOORS_GetPresetIndexFromGridCell(D2DrlgLevelStrc* pLevel, int nX, int nY);
//D2Common.0x6FD7DD40
void __fastcall DRLGOUTDOORS_AlterAdjacentPresetGridCells(D2DrlgLevelStrc* pLevel, int nX, int nY);
//D2Common.0x6FD7DD70
void __fastcall DRLGOUTDOORS_SetBlankGridCell(D2DrlgLevelStrc* pLevel, int nX, int nY);
//D2Common.0x6FD7DDB0
unsigned int __fastcall DRLGOUTDOORS_TestGridCellNonLvlLink(D2DrlgLevelStrc* pLevel, int nX, int nY);
//D2Common.0x6FD7DDD0
BOOL __fastcall DRLGOUTDOORS_TestGridCellSpawnValid(D2DrlgLevelStrc* pLevel, int nX, int nY);
//D2Common.0x6FD7DDF0
BOOL __fastcall DRLGOUTDOORS_TestOutdoorLevelPreset(D2DrlgLevelStrc* pLevel, int nX, int nY, int nId, int nOffset, char nFlags);
//D2Common.0x6FD7DEF0
void __fastcall DRLGOUTDOORS_SpawnOutdoorLevelPresetEx(D2DrlgLevelStrc* pLevel, int nX, int nY, int nLevelPrestId, int nRand, BOOL a6);
//D2Common.0x6FD7E0F0
BOOL __fastcall DRLGOUTDOORS_SpawnPresetFarAway(D2DrlgLevelStrc* pLevel, D2DrlgCoordStrc* pDrlgCoord, int nLvlPrestId, int nRand, int nOffset, char nFlags);
//D2Common.0x6FD7E330
BOOL __fastcall DRLGOUTDOORS_SpawnOutdoorLevelPreset(D2DrlgLevelStrc* pLevel, int nLevelPrestId, int nRand, int nOffset, char nFlags);
//D2Common.0x6FD7E4D0
BOOL __fastcall DRLGOUTDOORS_SpawnRandomOutdoorDS1(D2DrlgLevelStrc* pLevel, int nLvlPrestId, int nRand);
//D2Common.0x6FD7E6D0
void __fastcall DRLGOUTDOORS_SpawnAct12Waypoint(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7E940
void __fastcall DRLGOUTDOORS_SpawnAct12Shrines(D2DrlgLevelStrc* pLevel, int nShrines);
//D2Common.0x6FD7EB20
void __fastcall DRLGOUTDOORS_AddAct124SecondaryBorder(D2DrlgLevelStrc* pLevel, int a2, int a3);
//D2Common.0x6FD7EBA0
void __fastcall DRLGOUTDOORS_AllocOutdoorInfo(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7EBD0
void __fastcall DRLGOUTDOORS_GenerateLevel(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7EEE0
void __fastcall DRLGOUTDOORS_FreeOutdoorInfo(D2DrlgLevelStrc* pLevel, BOOL bKeepRoomData);
//D2Common.0x6FD7EFE0
void __fastcall sub_6FD7EFE0(D2DrlgLevelStrc* pLevel, D2RoomExStrc* pRoomEx);
//D2Common.0x6FD7F250
void __fastcall DRLGOUTDOORS_SpawnAct1DirtPaths(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7F500
void __fastcall DRLGOUTDOORS_CalculatePathCoordinates(D2DrlgLevelStrc* pLevel, D2DrlgVertexStrc* pVertex1, D2DrlgVertexStrc* pVertex2);
//D2Common.0x6FD7F5B0
void __fastcall sub_6FD7F5B0(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD7F810
void __fastcall sub_6FD7F810(D2DrlgLevelStrc* pLevel, int nVertexId);
//D2Common.0x6FD7F9B0
void __fastcall DRLGOUTDOORS_InitAct4OutdoorLevel(D2DrlgLevelStrc* pLevel);
