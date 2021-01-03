#pragma once

#include "CommonDefinitions.h"
#pragma pack(1)

struct D2WaypointActTableStrc
{
	bool bTableInitialized;					//0x00
	int32_t nStartLevelId;						//0x01
	int32_t nEndLevelId;						//0x05
	char nWpNo;								//0x09
	char nLastWpNo;							//0x0A
};

struct D2WaypointCoordTableStrc
{
	int32_t nX;									//0x00
	int32_t nY;									//0x04
	int32_t nTextX;								//0x08
	int32_t nTextY;								//0x0C
	int32_t nClickX;							//0x10
	int32_t nClickY;							//0x14
};

struct D2WaypointDataStrc
{
	// Called "history" in the original game
	uint16_t nFlags[8];							//0x00
};

struct D2WaypointFlagTableStrc
{
	uint16_t nArrayId;							//0x00
	uint16_t nFlag;								//0x02
};

struct D2WaypointTableStrc
{
	int32_t nLevelId;							//0x00
	bool bActivated;						//0x04
};
#pragma pack()

//D2Common.0x6FDC3D20 (#11153)
BOOL __stdcall WAYPOINTS_GetLevelIdFromWaypointNo(short nWaypointNo, int* pLevelId);
//D2Common.0x6FDC3D90 (#11152)
BOOL __stdcall WAYPOINTS_GetWaypointNoFromLevelId(int nLevelId, short* pWaypointNo);
//D2Common.0x6FDC3DE0 (#11146)
int __stdcall WAYPOINTS_IsActivated(D2WaypointDataStrc* pData, uint16_t wField);
//D2Common.0x6FDC3E80 (#11147)
void __stdcall WAYPOINTS_ActivateWaypoint(D2WaypointDataStrc* pData, uint16_t wField);
//D2Common.0x6FDC3F20 (#11148)
D2WaypointDataStrc* __stdcall WAYPOINTS_AllocWaypointData(void* pMemPool);
//D2Common.0x6FDC3F70 (#11149)
void __stdcall WAYPOINTS_FreeWaypointData(void* pMemPool, D2WaypointDataStrc* pData);
//D2Common.0x6FDC3FD0 (#11150)
void __stdcall WAYPOINTS_CopyAndValidateWaypointData(D2WaypointDataStrc* pDestination, D2WaypointDataStrc* pSource);
//D2Common.0x6FDC4060 (#11151)
void __stdcall WAYPOINTS_ValidateAndCopyWaypointData(D2WaypointDataStrc* pSource, D2WaypointDataStrc* pDestination);
