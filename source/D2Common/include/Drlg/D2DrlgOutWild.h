#pragma once

#include "CommonDefinitions.h"

#pragma pack(1)


#pragma pack()

//D2Common.0x6FD84CA0
void __fastcall DRLGOUTWILD_GetBridgeCoords(D2DrlgLevelStrc* pLevel, int* pX, int* pY);
//D2Common.0x6FD84D30
void __fastcall DRLGOUTWILD_InitAct1OutdoorLevel(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD85060
BOOL __fastcall DRLGOUTWILD_TestSpawnRiver(D2DrlgLevelStrc* pLevel, int nX);
//D2Common.0x6FD850B0
void __fastcall DRLGOUTWILD_SpawnRiver(D2DrlgLevelStrc* pLevel, int nX);
//D2Common.0x6FD85300
BOOL __fastcall sub_6FD85300(D2DrlgVertexStrc* pDrlgVertex);
//D2Common.0x6FD85350
BOOL __fastcall sub_6FD85350(D2DrlgVertexStrc* pDrlgVertex);
//D2Common.0x6FD85390
BOOL __fastcall DRLGOUTWILD_SpawnCliffCaves(D2DrlgLevelStrc* pLevel, int nX, int nY);
//D2Common.0x6FD853F0
void __fastcall DRLGOUTWILD_SpawnTownTransitionsAndCaves(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD85520
void __fastcall DRLGOUTWILD_SpawnSpecialPresets(D2DrlgLevelStrc* pLevel);
//D2Common.0x6FD85920
void __fastcall DRLGOUTWILD_SpawnCottage(D2DrlgLevelStrc* pLevel, int nLvlPrestId, int a3);
