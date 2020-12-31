#pragma once

#include "CommonDefinitions.h"

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
