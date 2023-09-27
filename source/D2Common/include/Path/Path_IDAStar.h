#pragma once

#include "CommonDefinitions.h"
#include "Path.h"

#pragma pack(1)

enum class PATH_IDASTAR_CONSTANTS
{
	MAXPROOM = 50000, // Max size of a room for IDAStar (ptRoomCoords->nSizeGameX * ptRoomCoords->nSizeGameY <= MAXPROOM)
};

struct D2IdaStarPathPointsListStrc					  //sizeof 0x38
{
	D2PathPointStrc tPoint;							  //0x00
	uint16_t nDist;									  //0x04
	uint16_t nMaxDist;								  //0x06
	uint16_t nMinDist;								  //0x08
	uint16_t wPad;									  //0x0A
	D2IdaStarPathPointsListStrc* pNextPathAttempt;	  //0x0C
	D2IdaStarPathPointsListStrc* pParents[8];		  //0x10
	D2IdaStarPathPointsListStrc* pLastPoint;		  //0x30
	D2IdaStarPathPointsListStrc* pNext;				  //0x34
} ;

struct D2IdaStarPathContextStrc						  //sizeof 0x32EC
{
	D2IdaStarPathPointsListStrc* pPointsCache[128];	  //0x0000
	D2IdaStarPathPointsListStrc* pPositions[128];	  //0x0200
	D2IdaStarPathPointsListStrc* pArgs;				  //0x0400
	D2IdaStarPathPointsListStrc pArrayPositions[200]; //0x0404
	int nIndexUsed;									  //0x2FC4
	D2IdaStarPathPointsListStrc* aPointsStack[200];	  //0x2FC8
	int nStackCount;								  //0x32E8
};

#pragma pack()

//D2Common.0x6FDA69E0
int __fastcall PATH_IDAStar_ComputePath(D2PathInfoStrc* pPathInfo);
////D2Common.0x6FDA6D10
//int __fastcall sub_6FDA6D10(int a1, int a2);

//1.10f: D2Common.0x6FDA6D50
//1.13c: D2Common.0x6FDCB3C0
BOOL __fastcall sub_6FDA6D50(D2PathInfoStrc* pPathInfo, D2IdaStarPathContextStrc* pIDAStarContext, D2IdaStarPathPointsListStrc* a3, D2PathPointStrc tTargetCoord);

//1.10f: D2Common.0x6FDA7230
//1.13c: D2Common.0x6FDCAF70
int __stdcall PATH_IdaStar_Heuristic(D2PathPointStrc pPoint1, D2PathPointStrc pPoint2);

////D2Common.0x6FDA7280
//int __fastcall sub_6FDA7280(void* a1, int a2);
////D2Common.0x6FDA72D0
//int __fastcall sub_6FDA72D0(void* a1, int a2);
////D2Common.0x6FDA7320
//signed int __fastcall sub_6FDA7320(int a1, int a2);
////D2Common.0x6FDA7390
//int __fastcall sub_6FDA7390(int a1, int a2);
////D2Common.0x6FDA7450
//int __fastcall sub_6FDA7450(int a1);
////D2Common.0x6FDA7490
//signed int __userpurge sub_6FDA7490<eax>(int a1<edx>, int a2<edi>, int a3, int a4, int a5);
//D2Common.0x6FDA78A0
signed int __fastcall PATH_IdaStar_FlushPointListToDynamicPath(D2IdaStarPathPointsListStrc* pCurPoint, D2PathInfoStrc* pPathInfo);
//D2Common.0x6FDA7970
int __fastcall PATH_IdaStar_6FDA7970(D2PathInfoStrc* pPathInfo);
////D2Common.0x6FDA7D40
//int __fastcall sub_6FDA7D40(int a1, signed int a2, int a3);
////D2Common.0x6FDA81C0
//int __fastcall sub_6FDA81C0(int a1, int a2);
