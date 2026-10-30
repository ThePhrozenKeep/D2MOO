#include "Scale.h"

#include <algorithm>

#include <Fog.h>

#include "DisplayType.h"
#include "D2Gfx.h"


extern DisplayType gnDisplayType;


//D2Gfx.0x6FA7E4A8
int32_t gnChangedScaleFactor = 256;
//D2Gfx.0x6FA7E4AC
int32_t gnScaleFactor = 256;
//D2Gfx.0x6FA8D678
int32_t gnPerspectiveScaleX;
//D2Gfx.0x6FA8D67C
int32_t gnPerspectiveScaleY;


// Inlined in the original dll. Contrary to D2GFX_HardwareAcceleratedRenderMode (#10047), invalid display types are rejected.
static bool SCALE_IsPerspectiveDisplayType(DisplayType nDisplayType)
{
    return nDisplayType == DISPLAYTYPE_GLIDE || nDisplayType == DISPLAYTYPE_OPENGL || nDisplayType == DISPLAYTYPE_RAVE || nDisplayType == DISPLAYTYPE_DIRECT3D;
}

//D2Gfx.0x6FA73F30 (#10059)
void __stdcall SCALE_UpdateFactor()
{
    gnScaleFactor = gnChangedScaleFactor;
    return gpGraphicsInterface->pfSetGlobalScale(gnScaleFactor);
}

//D2Gfx.0x6FA73F50 (#10060)
int32_t __stdcall SCALE_GetFactor()
{
    return gnScaleFactor;
}

//D2Gfx.0x6FA73F60 (#10061)
void __stdcall SCALE_SetDefaultFactor()
{
    gnScaleFactor = 256;
    return gpGraphicsInterface->pfSetGlobalScale(gnScaleFactor);
}

//D2Gfx.0x6FA73F80 (#10062)
void __stdcall SCALE_IncreaseFactor()
{
    if (gnChangedScaleFactor < 512)
    {
        gnChangedScaleFactor = std::min(gnChangedScaleFactor + 32, 512);
    }
}

//D2Gfx.0x6FA73FB0 (#10063)
void __stdcall SCALE_DecreaseFactor()
{
    if (gnChangedScaleFactor > 256)
    {
        gnChangedScaleFactor = std::max(gnChangedScaleFactor - 32, 256);
    }
}

//D2Gfx.0x6FA73FE0 (#10064)
void __stdcall SCALE_SetPerspectiveScale(int32_t nXScale, int32_t nYScale)
{
    D2_ASSERT(SCALE_IsPerspectiveDisplayType(gnDisplayType));
    D2_ASSERT(gGfxSettings.bPerspectiveEnabled);

    gnPerspectiveScaleX = nXScale;
    gnPerspectiveScaleY = nYScale;
    return gpGraphicsInterface->pfUpdatePerspective(nXScale, nYScale);
}

//D2Gfx.0x6FA74060 (#10065)
int32_t __stdcall SCALE_CheckPerspectivePosition(int32_t nXPos, int32_t nYPos)
{
    D2_ASSERT(SCALE_IsPerspectiveDisplayType(gnDisplayType));
    D2_ASSERT(gGfxSettings.bPerspectiveEnabled);

    const int32_t nX = std::abs(nXPos - gnPerspectiveScaleX);
    if (nX > 0x320000)
    {
        return 0;
    }

    const int32_t nY = std::abs(nYPos - gnPerspectiveScaleY);
    if (nY > 0x320000)
    {
        return 0;
    }

    return 1;
}

//D2Gfx.0x6FA74100 (#10066)
void __stdcall SCALE_AdjustPerspectivePosition(int32_t nXPos, int32_t nYPos, int32_t nBais, int32_t* pXAdjust, int32_t* pYAdjust)
{
    D2_ASSERT(D2GFX_HardwareAcceleratedRenderMode());
    D2_ASSERT(gGfxSettings.bPerspectiveEnabled);

    return gpGraphicsInterface->pfPerspectiveTransform(nXPos, nYPos, nBais, pXAdjust, pYAdjust);
}

//D2Gfx.0x6FA74170 (#10067)
void __stdcall SCALE_ScalePerspectivePosition(int32_t nXPos, int32_t nYPos, int32_t nAngle, int32_t* pXAdjust, int32_t* pYAdjust, int32_t bOrder)
{
    D2_ASSERT(SCALE_IsPerspectiveDisplayType(gnDisplayType));
    D2_ASSERT(gGfxSettings.bPerspectiveEnabled);

    return gpGraphicsInterface->pfPerspectiveTransformScale(nXPos, nYPos, 0, pXAdjust, pYAdjust, bOrder);
}

//D2Gfx.0x6FA74200 (#10068)
void __stdcall SCALE_SetDefaultFactorPerspectiveFactor()
{
    D2_ASSERT(SCALE_IsPerspectiveDisplayType(gnDisplayType));
    D2_ASSERT(gGfxSettings.bPerspectiveEnabled);

    return gpGraphicsInterface->pfPerspectiveClearScale();
}
