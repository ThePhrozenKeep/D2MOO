#pragma once

#include "D2CommonDefinitions.h"
#include <D2Seed.h>
#include "Path.h"

#pragma pack(1)

#pragma pack()

//1.10f: D2Common.0x6FDABAC0
//1.13C: D2Common.0x6FD68830
int __fastcall PATH_FindSubpathWithoutObstacles(D2PathInfoStrc* pInfo, D2PathPointStrc tSubPathStart, D2PathPointStrc* pPathPoints, int* pSubPathStartIdx, int* nMaxIndex, int nMaxLength, int nMajorDirection);
//1.10f: D2Common.0x6FDAC170
//1.13C: D2Common.0x6FD68310
int __fastcall PATH_SimplifyToLines(D2PathPointStrc* pOutPathPoints, D2PathPointStrc* pInputPoints, D2PathPointStrc tStartCoord, signed int nbTempPoints);
//1.10f: D2Common.0x6FDAC270
//1.13C: D2Common.0x6FD68C40
int __fastcall PATH_ComputePathOrSlideAlongObstacles(D2PathInfoStrc* ptPathInfo);
