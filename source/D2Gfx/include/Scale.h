#pragma once

#include <cstdint>


//D2Gfx.0x6FA73F30 (#10059)
void __stdcall SCALE_UpdateFactor();
//D2Gfx.0x6FA73F50 (#10060)
int32_t __stdcall SCALE_GetFactor();
//D2Gfx.0x6FA73F60 (#10061)
void __stdcall SCALE_SetDefaultFactor();
//D2Gfx.0x6FA73F80 (#10062)
void __stdcall SCALE_IncreaseFactor();
//D2Gfx.0x6FA73FB0 (#10063)
void __stdcall SCALE_DecreaseFactor();
//D2Gfx.0x6FA73FE0 (#10064)
void __stdcall SCALE_SetPerspectiveScale(int32_t nXScale, int32_t nYScale);
//D2Gfx.0x6FA74060 (#10065)
int32_t __stdcall SCALE_CheckPerspectivePosition(int32_t nXPos, int32_t nYPos);
//D2Gfx.0x6FA74100 (#10066)
void __stdcall SCALE_AdjustPerspectivePosition(int32_t nXPos, int32_t nYPos, int32_t nBais, int32_t* pXAdjust, int32_t* pYAdjust);
//D2Gfx.0x6FA74170 (#10067)
void __stdcall SCALE_ScalePerspectivePosition(int32_t nXPos, int32_t nYPos, int32_t nAngle, int32_t* pXAdjust, int32_t* pYAdjust, int32_t bOrder);
//D2Gfx.0x6FA74200 (#10068)
void __stdcall SCALE_SetDefaultFactorPerspectiveFactor();
