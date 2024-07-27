#pragma once

#include "D2CommonDefinitions.h"
#include "D2DrlgDrlg.h"
#include "D2DrlgDrlgGrid.h"

#pragma pack(1)


#pragma pack()

//D2Common.0x6FD75480
void __fastcall DRLGANIM_InitCache(D2DrlgStrc* pDrlg, D2DrlgTileDataStrc* pTileData);
//D2Common.0x6FD75560
void __fastcall DRLGANIM_TestLoadAnimatedRoomTiles(D2DrlgRoomStrc* pDrlgRoom, D2DrlgGridStrc* pDrlgGrid, D2DrlgGridStrc* pTileTypeGrid, int nTileType, int nTileX, int nTileY);
//D2Common.0x6FD756B0
void __fastcall DRLGANIM_AnimateTiles(D2DrlgRoomStrc* pDrlgRoom);
//D2Common.0x6FD75740
void __fastcall DRLGANIM_AllocAnimationTileGrids(D2DrlgRoomStrc* pDrlgRoom, int nAnimationSpeed, D2DrlgGridStrc* pWallGrid, int nWalls, D2DrlgGridStrc* pFloorGrid, int nFloors, D2DrlgGridStrc* pShadowGrid);
//D2Common.0x6FD757B0
void __fastcall DRLGANIM_AllocAnimationTileGrid(D2DrlgRoomStrc* pDrlgRoom, int nAnimationSpeed, D2DrlgTileDataStrc* pTiles, int nTiles, D2DrlgGridStrc* pDrlgGrid, int nUnused);
//D2Common.0x6FD75B00
void __fastcall DRLGANIM_UpdateFrameInAdjacentRooms(D2DrlgRoomStrc* pDrlgRoom1, D2DrlgRoomStrc* pDrlgRoom2);
