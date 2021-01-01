#pragma once

#include "CommonDefinitions.h"
#include "D2DrlgDrlgGrid.h"

//D2Common.0x6FD75480
void __fastcall DRLGANIM_InitCache(D2DrlgStrc* pDrlg, D2DrlgTileDataStrc* pTileData);
//D2Common.0x6FD75560
void __fastcall DRLGANIM_TestLoadAnimatedRoomTiles(D2RoomExStrc* pRoomEx, D2DrlgGridStrc* pDrlgGrid, D2DrlgGridStrc* pOrientationGrid, int a4, int nTileX, int nTileY);
//D2Common.0x6FD756B0
void __fastcall DRLGANIM_AnimateTiles(D2RoomExStrc* pRoomEx);
//D2Common.0x6FD75740
void __fastcall DRLGANIM_AllocAnimationTileGrids(D2RoomExStrc* pRoomEx, int a2, D2DrlgGridStrc* a3, int a4, D2DrlgGridStrc* a5, int a6, D2DrlgGridStrc* a7);
//D2Common.0x6FD757B0
void __fastcall DRLGANIM_AllocAnimationTileGrid(D2RoomExStrc* pRoomEx, int nAnimationSpeed, D2DrlgTileDataStrc* pTiles, int nTiles, D2DrlgGridStrc* pDrlgGrid, int nUnused);
//D2Common.0x6FD75B00
void __fastcall DRLGANIM_UpdateFrameInAdjacentRooms(D2RoomExStrc* pRoomEx1, D2RoomExStrc* pRoomEx2);
