#pragma once

#include "CommonDefinitions.h"
#include "Path.h"

#pragma pack(1)

enum PATH_IDASTAR_CONSTANTS
{
	IDASTAR_MAXPROOM = 50000, // Max size of a room for IDAStar (ptRoomCoords->nSizeGameX * ptRoomCoords->nSizeGameY <= MAXPROOM)
	IDASTAR_CACHE_SIZE = 128,
};

struct D2IdaStarPathPointsListStrc					  //sizeof 0x38
{
	D2PathPointStrc tPoint;							  //0x00
	int16_t nFScore;								  //0x04
	int16_t nHeuristicDistanceToTarget;               //0x06
	int16_t nBestDistanceFromStart;                   //0x08
	uint16_t wPad;									  //0x0A
	D2IdaStarPathPointsListStrc* pNextPathAttempt;	  //0x0C
	D2IdaStarPathPointsListStrc* pParents[8];		  //0x10
	D2IdaStarPathPointsListStrc* pNextCachePoint;     //0x30
	D2IdaStarPathPointsListStrc* pNextSortedByFScore; //0x34
} ;

struct D2IdaStarPathContextStrc									//sizeof 0x32EC
{
	D2IdaStarPathPointsListStrc* aCache0[IDASTAR_CACHE_SIZE];	//0x0000
	D2IdaStarPathPointsListStrc* aCache1[IDASTAR_CACHE_SIZE];	//0x0200
	D2IdaStarPathPointsListStrc* pSortedListByFScore;			//0x0400
	D2IdaStarPathPointsListStrc pArrayPositions[200];			//0x0404
	int nIndexUsed;												//0x2FC4
	D2IdaStarPathPointsListStrc* aPointsStack[200];				//0x2FC8
	int nStackCount;											//0x32E8
};

#pragma pack()

//D2Common.0x6FDA69E0
int __fastcall PATH_IDAStar_ComputePath(D2PathInfoStrc* pPathInfo);

//D2Common.0x6FDA6D10
int __fastcall PATH_IdaStar_PushPointListToCache1(D2IdaStarPathContextStrc* pIdaStarContext, D2IdaStarPathPointsListStrc* pPointList);

//1.10f: D2Common.0x6FDA6D50
//1.13c: D2Common.0x6FDCB3C0
BOOL __fastcall sub_6FDA6D50(D2PathInfoStrc* pPathInfo, D2IdaStarPathContextStrc* pIdaStarContext, D2IdaStarPathPointsListStrc* a3, D2PathPointStrc tTargetCoord);

//1.10f: D2Common.0x6FDA7230
//1.13c: D2Common.0x6FDCAF70
int __stdcall PATH_IdaStar_Heuristic(D2PathPointStrc tPoint1, D2PathPointStrc tPoint2);

//1.00:  D2Common.0x10057A10
//1.10f: D2Common.0x6FDA7280
//1.13c: D2Common.0x6FDCAF20
D2IdaStarPathPointsListStrc* __fastcall PATH_IdaStar_GetPointListFromCache0(D2IdaStarPathContextStrc* pIdaStarContext, D2PathPointStrc tPathPoint);
//1.00:  D2Common.0x10057A10
//1.10f: D2Common.0x6FDA72D0
//1.13c: D2Common.0x6FDCAED0
D2IdaStarPathPointsListStrc* __fastcall PATH_IdaStar_GetPointListFromCache1(D2IdaStarPathContextStrc* pIdaStarContext, D2PathPointStrc tPathPoint);

//1.10f: D2Common.0x6FDA7320
//1.13c: D2Common.0x6FDCAE20
void __fastcall PATH_IdaStar_MakeCandidate(D2IdaStarPathContextStrc* pIdaStarContext, D2IdaStarPathPointsListStrc* pPoint);

//1.10f: D2Common.0x6FDA7390
//1.13c: D2Common.0x6FDCAC50
void __fastcall sub_6FDA7390(D2IdaStarPathContextStrc* pIDAStarContext, int nUnused, D2IdaStarPathPointsListStrc* pNewPoint);

//1.10f: D2Common.0x6FDA7450
//1.13c: Inlined
D2IdaStarPathPointsListStrc* __fastcall PATH_IdaStar_GetNewPathList(D2IdaStarPathContextStrc* pIdaStarContext);

//1.10f: D2Common.0x6FDA7490
//1.13c: D2Common.0x6FDCAFB0
signed int __fastcall PATH_IdaStar_EvaluateNeighbour(D2PathInfoStrc* pPathInfo, D2IdaStarPathContextStrc* pIdaStarContext, D2IdaStarPathPointsListStrc* pCurrentPointList, D2PathPointStrc tNewPointCoord, D2PathPointStrc tTargetCoord);

//D2Common.0x6FDA78A0
signed int __fastcall PATH_IdaStar_FlushPointListToDynamicPath(D2IdaStarPathPointsListStrc* pCurPoint, D2PathInfoStrc* pPathInfo);

//1.10f: D2Common.0x6FDA7970
//1.13c: D2Common.0x6FDC0E40
int __fastcall PATH_IdaStar_6FDA7970(D2PathInfoStrc* pPathInfo);

////D2Common.0x6FDA7D40
//int __fastcall sub_6FDA7D40(int a1, signed int a2, int a3);
////D2Common.0x6FDA81C0
//int __fastcall sub_6FDA81C0(int a1, int a2);
