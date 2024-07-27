#pragma once

#include "D2CommonDefinitions.h"
#include "Path.h"
#include "D2Seed.h"

// Note: Very similar to D1's path finding, but using a cache for the open/closed set

#pragma pack(1)

struct D2PathFoWallNodeStrc					    //sizeof 0x38
{
	D2PathPointStrc tPoint;						//0x00
	int16_t nFScore;							//0x04
	int16_t nHeuristicDistanceToTarget;			//0x06
	int16_t nBestDistanceFromStart;				//0x08
	uint16_t wPad;								//0x0A
	D2PathFoWallNodeStrc* pBestParent;		    //0x0C
	D2PathFoWallNodeStrc* pChildren[8];			//0x10
	D2PathFoWallNodeStrc* pNextCachePoint;		//0x30
	D2PathFoWallNodeStrc* pNextSortedByFScore;	//0x34
};

struct D2PathFoWallContextStrc						  //sizeof 0x32EC
{
	static const size_t CACHE_SIZE = 128;
	static const size_t STORAGE_SIZE = 200;
	static const size_t STACK_SIZE = 200;

	D2PathFoWallNodeStrc* aPendingCache[CACHE_SIZE];  //0x0000 Open set
	D2PathFoWallNodeStrc* aVisitedCache[CACHE_SIZE];  //0x0200 Closed set
	D2PathFoWallNodeStrc* pSortedListByFScore;		  //0x0400 Sorted in ascending order (best first)
	D2PathFoWallNodeStrc aNodesStorage[STORAGE_SIZE]; //0x0404 Allocator
	int32_t nNodesCount;							  //0x2FC4 Number of allocated nodes
	D2PathFoWallNodeStrc* aPointsStack[STACK_SIZE];   //0x2FC8 Stack used for propagation of best distance
	int32_t nStackCount;							  //0x32E8
};


#pragma pack()

//D2Common.0x6FDA69E0
int __fastcall PATH_AStar_ComputePath(D2PathInfoStrc* pPathInfo);

//D2Common.0x6FDA6D10
int __fastcall PATH_AStar_PushToVisitedCache(D2PathFoWallContextStrc* pContext, D2PathFoWallNodeStrc* pNode);

//1.10f: D2Common.0x6FDA6D50
//1.13c: D2Common.0x6FDCB3C0
BOOL __fastcall PATH_AStar_ExploreChildren(D2PathInfoStrc* pPathInfo, D2PathFoWallContextStrc* pContext, D2PathFoWallNodeStrc* a3, D2PathPointStrc tTargetCoord);

//1.10f: D2Common.0x6FDA7230
//1.13c: D2Common.0x6FDCAF70
int __stdcall PATH_AStar_Heuristic(D2PathPointStrc tPoint1, D2PathPointStrc tPoint2);
// Helper function
int16_t PATH_AStar_HeuristicForNeighbor(D2PathPointStrc tPoint, D2PathPointStrc tNeighbor);

//1.00:  D2Common.0x10057A10
//1.10f: D2Common.0x6FDA7280
//1.13c: D2Common.0x6FDCAF20
D2PathFoWallNodeStrc* __fastcall PATH_AStar_GetNodeFromPendingCache(D2PathFoWallContextStrc* pContext, D2PathPointStrc tPathPoint);
//1.00:  D2Common.0x10057A10
//1.10f: D2Common.0x6FDA72D0
//1.13c: D2Common.0x6FDCAED0
D2PathFoWallNodeStrc* __fastcall PATH_AStar_FindPointInVisitedCache(D2PathFoWallContextStrc* pContext, D2PathPointStrc tPathPoint);

//1.10f: D2Common.0x6FDA7320
//1.13c: D2Common.0x6FDCAE20
void __fastcall PATH_AStar_MakeCandidate(D2PathFoWallContextStrc* pContext, D2PathFoWallNodeStrc* pNode);

//1.10f: D2Common.0x6FDA7390
//1.13c: D2Common.0x6FDCAC50
void __fastcall PATH_AStar_PropagateNewFScoreToChildren(D2PathFoWallContextStrc* pContext, int nUnused, D2PathFoWallNodeStrc* pNewNode);

//1.10f: D2Common.0x6FDA7450
//1.13c: Inlined
D2PathFoWallNodeStrc* __fastcall PATH_AStar_GetNewNode(D2PathFoWallContextStrc* pContext);

//1.10f: D2Common.0x6FDA7490
//1.13c: D2Common.0x6FDCAFB0
BOOL __fastcall PATH_AStar_EvaluateNeighbor(D2PathInfoStrc* pPathInfo, D2PathFoWallContextStrc* pContext, D2PathFoWallNodeStrc* pCurrentNode, D2PathPointStrc tNewPointCoord, D2PathPointStrc tTargetCoord);

//D2Common.0x6FDA78A0
signed int __fastcall PATH_AStar_FlushNodeToDynamicPath(D2PathFoWallNodeStrc* pNode, D2PathInfoStrc* pPathInfo);

