#pragma once

#include "CommonDefinitions.h"
#include <D2Seed.h>
#include "Path.h"

#pragma pack(1)

#pragma pack()

// D2Common.0x6FDABAC0 (1.10f)
// D2Common.0x6FD68830 (1.13C)
int __fastcall PATH_FindSubpathWithoutObstacles(D2PathInfoStrc* pInfo, D2PathPointStrc tSubPathStart, D2PathPointStrc* pPathPoints, int* pSubPathStartIdx, int* nMaxIndex, int nMaxLength, int nMajorDirection);
//D2Common.0x6FDAC170 (1.10f)
//D2Common.0x6FD68310 (1.13c)
int __fastcall PATH_SimplifyToLines(D2PathPointStrc* pOutPathPoints, D2PathPointStrc* pInputPoints, D2PathPointStrc tStartCoord, signed int nbTempPoints);
// D2Common.0x6FDAC270 (1.10f) 
// D2Common.0x6FD68C40 (1.13C)
int __fastcall PATH_ComputePathOrSlideAlongObstacles(D2PathInfoStrc* ptPathInfo);
