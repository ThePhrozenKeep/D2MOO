#include "CmnSubtile.h"

#include <algorithm>
#include <iterator>

#include <D2Math.h>
#include <Fog.h>
#include <Storm.h>

#include "DisplayType.h"
#include "D2Gfx.h"


extern int32_t gnViewMatrixAdjust;
extern DisplayType gnDisplayType;


constexpr int32_t GOURAUD_SHIFT = 24;
constexpr int32_t VISTILE_SIZE = 32;
constexpr int32_t ISOBLOCK_LINES = 15;

//D2Gfx.0x6FA81448
uint8_t** gpTransPalettes;

//D2Gfx.0x6FA81460
uint8_t* gpPaletteTable[72];
//D2Gfx.0x6FA81580
D2PaletteStrc gShadowLightPalettes[49];

//D2Gfx.0x6FA8144C
uint8_t* gCmnInfo_pBuffer;
//D2Gfx.0x6FA81450
int32_t gCmnInfo_nPitch;
//D2Gfx.0x6FA81454
int32_t gCmnInfo_nHeight;
//D2Gfx.0x6FA81458
int32_t gCmnInfo_nUnknown;

// Offset of each line in the back buffer, starting at line -32
//D2Gfx.0x6FA846A8
int32_t gnLineOffsets[732];
//D2Gfx.0x6FA84680
int32_t gnLineOffsetsPitch;

// Clip rectangle of the spans
//D2Gfx.0x6FA8D220
int32_t gnClipLeft;
//D2Gfx.0x6FA84690
int32_t gnClipTop;
//D2Gfx.0x6FA8469C
int32_t gnClipRight;
//D2Gfx.0x6FA8D240
int32_t gnClipBottom;

// Positions of 32x32 vis tiles that are drawn at all / drawn without clipping
//D2Gfx.0x6FA84688
int32_t gnVisTileMinX;
//D2Gfx.0x6FA85218
int32_t gnVisTileMaxX;
//D2Gfx.0x6FA8D230
int32_t gnVisTileMinY;
//D2Gfx.0x6FA8D24C
int32_t gnVisTileMaxY;
//D2Gfx.0x6FA846A0
int32_t gnVisTileUnclippedMinX;
//D2Gfx.0x6FA8D234
int32_t gnVisTileUnclippedMaxX;
//D2Gfx.0x6FA8D238
int32_t gnVisTileUnclippedMinY;
//D2Gfx.0x6FA8D248
int32_t gnVisTileUnclippedMaxY;

// Positions of 32x15 floor blocks that are drawn at all / drawn without clipping
//D2Gfx.0x6FA8D244
int32_t gnFloorBlockMinX;
//D2Gfx.0x6FA84698
int32_t gnFloorBlockMaxX;
//D2Gfx.0x6FA8D224
int32_t gnFloorBlockMinY;
//D2Gfx.0x6FA8D23C
int32_t gnFloorBlockMaxY;
//D2Gfx.0x6FA8468C
int32_t gnFloorBlockUnclippedMinX;
//D2Gfx.0x6FA8D22C
int32_t gnFloorBlockUnclippedMaxX;
//D2Gfx.0x6FA8D228
int32_t gnFloorBlockUnclippedMinY;
//D2Gfx.0x6FA84694
int32_t gnFloorBlockUnclippedMaxY;

// Never set in the original dll, so the viewport is recomputed every frame
//D2Gfx.0x6FA7E040
int32_t gnViewportViewMatrixAdjust = -1;

//D2Gfx.0x6FA8D250
int32_t gbForceFlatLighting;

// [nLeftIntensity][nRightIntensity][nX]: intensity interpolated over 32 pixels
//D2Gfx.0x6FA85220
uint8_t gGouraudCache[32][32][32];


// Returns the line of the gouraud cache interpolating between the two intensities
static inline uint8_t* D2GFX_GetGouraudLine(int32_t nLeftIntensity, int32_t nRightIntensity, uint8_t nIntensityShift)
{
    return &gGouraudCache[0][0][0] + 32 * ((nRightIntensity >> nIntensityShift) + 32 * (nLeftIntensity >> nIntensityShift));
}


// Layout of the lines of a 32x15 isometric diamond
//D2Gfx.0x6FA7E064
constexpr int32_t gnIsoBlockLineX[ISOBLOCK_LINES] =
{
    14, 12, 10, 8, 6, 4, 2, 0, 2, 4, 6, 8, 10, 12, 14
};

//D2Gfx.0x6FA7E0A0
constexpr int32_t gnIsoBlockLineWidth[ISOBLOCK_LINES] =
{
    4, 8, 12, 16, 20, 24, 28, 32, 28, 24, 20, 16, 12, 8, 4
};

//D2Gfx.0x6FA7E0DC
constexpr int32_t gnIsoBlockLineDataOffset[ISOBLOCK_LINES] =
{
    0, 4, 12, 24, 40, 60, 84, 112, 144, 172, 196, 216, 232, 244, 252
};


//D2Gfx.0x6FA71000
void __fastcall CMNSUBTILE_InitGouraudCache()
{
    CMNSUBTILE_BuildGouraudCache();
    gCmnInfo_pBuffer = nullptr;
}

//D2Gfx.0x6FA71010
void __fastcall CMNSUBTILE_FillYBufferTable(uint8_t* pBuffer, int32_t nPitch, int32_t nHeight, int32_t nUnknown)
{
    gCmnInfo_pBuffer = pBuffer;
    gCmnInfo_nPitch = nPitch;
    gCmnInfo_nHeight = nHeight;
    gCmnInfo_nUnknown = nUnknown;

    if (nPitch != gnLineOffsetsPitch)
    {
        gCmnInfo_nPitch = nPitch;
        gnLineOffsetsPitch = nPitch;
        CMNSUBTILE_BuildLineOffsets();
    }

    if (gnViewMatrixAdjust != gnViewportViewMatrixAdjust)
    {
        CMNSUBTILE_SetViewport(nPitch, nHeight);
    }
}

//D2Gfx.0x6FA71070
void __fastcall CMNSUBTILE_SetPaletteTables(D2PaletteTableStrc* pPaletteTable)
{
    memcpy(gpPaletteTable, pPaletteTable, sizeof(gpPaletteTable));
    gpTransPalettes = gpPaletteTable;
    for (int32_t i = 0; i < std::size(gShadowLightPalettes); ++i)
    {
        memcpy(&gShadowLightPalettes[i], gpPaletteTable[i + 3], sizeof(D2PaletteStrc));
    }
}

//D2Gfx.0x6FA710C0
int32_t __fastcall CMNSUBTILE_SetContrastAndGamma()
{
    DWORD nContrast = gnDisplayType != DISPLAYTYPE_GLIDE ? 100 : 0;
    SRegLoadValue("Diablo II", "Contrast", 0, &nContrast);
    D2GFX_SetOption(11, nContrast);

    DWORD nGamma = 155;
    SRegLoadValue("Diablo II", "Gamma", 0, &nGamma);
    gGfxSettings.nGamma = nGamma;
    return D2GFX_SetGamma(nGamma);
}

//D2Gfx.0x6FA71130
void __fastcall CMNSUBTILE_DrawIsoBlockFlat(uint8_t* pSrc, uint8_t* pDst, uint8_t* pPalette)
{
    for (int32_t nLine = 0; nLine < ISOBLOCK_LINES; ++nLine)
    {
        const uint8_t* pLineSrc = &pSrc[gnIsoBlockLineDataOffset[nLine]];
        uint8_t* pLineDst = &pDst[gnIsoBlockLineX[nLine]];

        // Unrolled in original dll
        for (int32_t i = gnIsoBlockLineWidth[nLine] - 1; i >= 0; --i)
        {
            pLineDst[i] = pPalette[pLineSrc[i]];
        }

        pDst += gCmnInfo_nPitch;
    }
}

//D2Gfx.0x6FA71340
void __fastcall CMNSUBTILE_DrawIsoBlockGouraud(uint8_t* pSrc, uint8_t* pDst, int32_t nLeftIntensity, int32_t nLeftIntensityStep, int32_t nRightIntensity, int32_t nRightIntensityStep)
{
    for (int32_t nLine = 0; nLine < ISOBLOCK_LINES; ++nLine)
    {
        const int32_t nLineX = gnIsoBlockLineX[nLine];
        const uint8_t* pGouraud = D2GFX_GetGouraudLine(nLeftIntensity, nRightIntensity, 7) + nLineX;
        const uint8_t* pLineSrc = &pSrc[gnIsoBlockLineDataOffset[nLine]];
        uint8_t* pLineDst = &pDst[nLineX];

        // Unrolled in orginal dll
        for (int32_t i = gnIsoBlockLineWidth[nLine] - 1; i >= 0; --i)
        {
            pLineDst[i] = gShadowLightPalettes[pGouraud[i]].nRemap[pLineSrc[i]];
        }

        pDst += gCmnInfo_nPitch;
        nLeftIntensity += nLeftIntensityStep;
        nRightIntensity += nRightIntensityStep;
    }
}

//D2Gfx.0x6FA71720
void __fastcall CMNSUBTILE_DrawRleFlat(uint8_t* pSrc, uint8_t* pDst, uint8_t* pPalette, int32_t nLines)
{
    uint8_t* pSpanDst = pDst;
    while (nLines)
    {
        const uint8_t nSkip = pSrc[0];
        const uint8_t nLength = pSrc[1];
        pSrc += 2;

        if (nSkip || nLength)
        {
            pSpanDst += nSkip;

            // Unrolled in original dll
            for (int32_t i = nLength - 1; i >= 0; --i)
            {
                pSpanDst[i] = pPalette[pSrc[i]];
            }

            pSpanDst += nLength;
            pSrc += nLength;
        }
        else
        {
            pDst += gCmnInfo_nPitch;
            --nLines;
            pSpanDst = pDst;
        }
    }
}

//D2Gfx.0x6FA71970
void __fastcall CMNSUBTILE_DrawRleGouraud(uint8_t* pSrc, uint8_t* pDst, int32_t nLeftIntensity, int32_t nLeftIntensityStep, int32_t nRightIntensity, int32_t nRightIntensityStep, int32_t nLines, uint8_t nIntensityShift)
{
    uint8_t* pSpanDst = pDst;
    uint8_t* pGouraud = D2GFX_GetGouraudLine(nLeftIntensity, nRightIntensity, nIntensityShift);

    while (nLines)
    {
        const uint8_t nSkip = pSrc[0];
        const uint8_t nLength = pSrc[1];
        pSrc += 2;

        if (nSkip || nLength)
        {
            pSpanDst += nSkip;
            pGouraud += nSkip;

            // Unrolled in original dll
            for (int32_t i = nLength - 1; i >= 0; --i)
            {
                pSpanDst[i] = gShadowLightPalettes[pGouraud[i]].nRemap[pSrc[i]];
            }

            pSpanDst += nLength;
            pSrc += nLength;
            pGouraud += nLength;
        }
        else
        {
            --nLines;
            nLeftIntensity += nLeftIntensityStep;
            nRightIntensity += nRightIntensityStep;
            pDst += gCmnInfo_nPitch;
            pSpanDst = pDst;
            pGouraud = D2GFX_GetGouraudLine(nLeftIntensity, nRightIntensity, nIntensityShift);
        }
    }
}

//D2Gfx.0x6FA71D90
void __fastcall CMNSUBTILE_DrawRleFlatClipped(int32_t nX, int32_t nY, uint8_t* pSrc, uint8_t* pDst, uint8_t* pPalette, int32_t nLines)
{
    uint8_t* pSpanDst = pDst;

    int32_t nLine = 0;
    while (nLine < nLines)
    {
        const uint8_t nSkip = pSrc[0];
        const uint8_t nLength = pSrc[1];
        pSrc += 2;

        if (nSkip || nLength)
        {
            if (nY >= gnClipBottom)
            {
                return;
            }

            pSpanDst += nSkip;

            if (nY >= gnClipTop)
            {
                const int32_t nSpanX = nX + (int32_t)(pSpanDst - pDst);
                const int32_t nStart = std::max(gnClipLeft - nSpanX, 0);
                const int32_t nEnd = std::min(std::max(gnClipRight - nSpanX, 0), (int32_t)nLength);

                // Unrolled in original dll
                for (int32_t i = nEnd - 1; i >= nStart; --i)
                {
                    pSpanDst[i] = pPalette[pSrc[i]];
                }
            }

            pSpanDst += nLength;
            pSrc += nLength;
        }
        else
        {
            ++nLine;
            ++nY;
            pDst += gCmnInfo_nPitch;
            pSpanDst = pDst;
        }
    }
}

//D2Gfx.0x6FA72090
void __fastcall CMNSUBTILE_DrawRleGouraudClipped(int32_t nX, int32_t nY, uint8_t* pSrc, uint8_t* pDst, int32_t nLeftIntensity, int32_t nLeftIntensityStep, int32_t nRightIntensity, int32_t nRightIntensityStep, int32_t nLines, uint8_t nIntensityShift)
{
    uint8_t* pSpanDst = pDst;
    uint8_t* pGouraud = D2GFX_GetGouraudLine(nLeftIntensity, nRightIntensity, nIntensityShift);

    int32_t nLine = 0;
    while (nLine < nLines)
    {
        const uint8_t nSkip = pSrc[0];
        const uint8_t nLength = pSrc[1];
        pSrc += 2;

        if (nSkip || nLength)
        {
            if (nY >= gnClipBottom)
            {
                return;
            }

            pSpanDst += nSkip;
            pGouraud += nSkip;

            if (nY >= gnClipTop)
            {
                const int32_t nSpanX = nX + (int32_t)(pSpanDst - pDst);
                const int32_t nStart = std::max(gnClipLeft - nSpanX, 0);
                const int32_t nEnd = std::min(std::max(gnClipRight - nSpanX, 0), (int32_t)nLength);

                // Unrolled in original dll. Note: the gouraud line is not offset by nStart in the original dll
                for (int32_t i = nEnd - 1; i >= nStart; --i)
                {
                    pSpanDst[i] = gShadowLightPalettes[pGouraud[i - nStart]].nRemap[pSrc[i]];
                }
            }

            pSpanDst += nLength;
            pGouraud += nLength;
            pSrc += nLength;
        }
        else
        {
            ++nLine;
            ++nY;
            pDst += gCmnInfo_nPitch;
            pSpanDst = pDst;
            nLeftIntensity += nLeftIntensityStep;
            nRightIntensity += nRightIntensityStep;
            pGouraud = D2GFX_GetGouraudLine(nLeftIntensity, nRightIntensity, nIntensityShift);
        }
    }
}

//D2Gfx.0x6FA72570
void __fastcall CMNSUBTILE_BuildGouraudCache()
{
    // Fixed point intensities with GOURAUD_SHIFT bits of fraction
    int32_t nFirstRightDelta = 0;
    for (int32_t nLeftIntensity = 0; nLeftIntensity < 32; ++nLeftIntensity)
    {
        const int32_t nStartValue = nLeftIntensity << GOURAUD_SHIFT;
        int32_t nDelta = nFirstRightDelta; // (nRightIntensity - nLeftIntensity) << GOURAUD_SHIFT
        for (int32_t nRightIntensity = 0; nRightIntensity < 32; ++nRightIntensity)
        {
            int32_t nVal = nStartValue;
            for (int32_t nX = 0; nX < 32; ++nX)
            {
                nVal = D2Clamp(nVal, 0, 0x1F000000);
                gGouraudCache[nLeftIntensity][nRightIntensity][nX] = (nVal >> GOURAUD_SHIFT) & 0xFF;
                nVal += nDelta >> 5;
            }

            D2_ASSERT((nVal >> GOURAUD_SHIFT) == nRightIntensity);

            nDelta += (1 << GOURAUD_SHIFT);
        }

        nFirstRightDelta -= (1 << GOURAUD_SHIFT);
    }
}

//D2Gfx.0x6FA72630
void __fastcall CMNSUBTILE_SetViewport(int32_t nWidth, int32_t nHeight)
{
    int32_t nLeft = 0;
    if (gnViewMatrixAdjust == 1)
    {
        nWidth -= nWidth / 2;
    }
    else if (gnViewMatrixAdjust == 2)
    {
        nLeft = nWidth / 2;
    }

    gnFloorBlockMinX = nLeft - 32;
    gnFloorBlockUnclippedMaxY = nHeight - 63;
    gnVisTileMinX = nLeft - 32;
    gnFloorBlockMaxY = nHeight - 47;
    gnVisTileMaxY = nHeight - 47;
    gnClipBottom = nHeight - 47;
    gnFloorBlockUnclippedMinX = nLeft;
    gnVisTileUnclippedMinX = nLeft;
    gnClipLeft = nLeft;
    gnFloorBlockMinY = -16;
    gnFloorBlockMaxX = nWidth;
    gnFloorBlockUnclippedMinY = 0;
    gnFloorBlockUnclippedMaxX = nWidth - 32;
    gnVisTileMinY = -32;
    gnVisTileMaxX = nWidth;
    gnVisTileUnclippedMinY = 0;
    gnVisTileUnclippedMaxY = nHeight - 79;
    gnVisTileUnclippedMaxX = nWidth - 32;
    gnClipTop = 0;
    gnClipRight = nWidth;
}

//D2Gfx.0x6FA72700
void __fastcall CMNSUBTILE_BuildLineOffsets()
{
    int32_t nOffset = -32 * gCmnInfo_nPitch;
    for (int32_t i = 0; i < std::size(gnLineOffsets); ++i)
    {
        gnLineOffsets[i] = nOffset;
        nOffset += gCmnInfo_nPitch;
    }
}

//D2Gfx.0x6FA72730
void __fastcall CMNSUBTILE_DrawVisTile(uint8_t* pSrc, int32_t nX, int32_t nY)
{
    if (nY < gnVisTileMinY || nY >= gnVisTileMaxY || nX < gnVisTileMinX || nX >= gnVisTileMaxX)
    {
        return;
    }

    D2_ASSERT(gCmnInfo_pBuffer);

    uint8_t* pLine = &gCmnInfo_pBuffer[gnLineOffsets[nY + 32]] + nX;
    uint8_t* pDst = pLine;

    if (nY < gnVisTileUnclippedMinY || nY >= gnVisTileUnclippedMaxY || nX < gnVisTileUnclippedMinX || nX >= gnVisTileUnclippedMaxX)
    {
        int32_t nCurrentY = nY;
        int32_t nLine = 0;
        while (nLine < VISTILE_SIZE)
        {
            const uint8_t nSkip = pSrc[0];
            const uint8_t nLength = pSrc[1];
            pSrc += 2;

            if (nSkip || nLength)
            {
                if (nCurrentY >= gnClipBottom)
                {
                    return;
                }

                pDst += nSkip;

                if (nCurrentY >= gnClipTop)
                {
                    const int32_t nPosX = nX + (int32_t)(pDst - pLine);
                    const int32_t nStart = std::max(gnClipLeft - nPosX, 0);
                    const int32_t nEnd = std::min(std::max(gnClipRight - nPosX, 0), (int32_t)nLength);
                    if (nEnd - nStart > 0)
                    {
                        memcpy(&pDst[nStart], &pSrc[nStart], nEnd - nStart);
                    }
                }

                pDst += nLength;
                pSrc += nLength;
            }
            else
            {
                ++nLine;
                ++nCurrentY;
                pLine += gCmnInfo_nPitch;
                pDst = pLine;
            }
        }
    }
    else
    {
        int32_t nLines = VISTILE_SIZE;
        while (nLines)
        {
            const uint8_t nSkip = pSrc[0];
            const uint8_t nLength = pSrc[1];
            pSrc += 2;

            if (nSkip || nLength)
            {
                pDst += nSkip;
                memcpy(pDst, pSrc, nLength);
                pDst += nLength;
                pSrc += nLength;
            }
            else
            {
                --nLines;
                pLine += gCmnInfo_nPitch;
                pDst = pLine;
            }
        }
    }
}

//D2Gfx.0x6FA72900
void __fastcall CMNSUBTILE_DrawFlatLitVisTile(uint8_t* pSrc, int32_t nX, int32_t nY, uint8_t nIntensity)
{
    if (nY < gnVisTileMinY || nY >= gnVisTileMaxY || nX < gnVisTileMinX || nX >= gnVisTileMaxX)
    {
        return;
    }

    uint8_t* pDst = &gCmnInfo_pBuffer[gnLineOffsets[nY + 32]] + nX;

    D2_ASSERT(gCmnInfo_pBuffer);

    uint8_t* pPalette = gShadowLightPalettes[nIntensity >> 3].nRemap;
    if (nY < gnVisTileUnclippedMinY || nY >= gnVisTileUnclippedMaxY || nX < gnVisTileUnclippedMinX || nX >= gnVisTileUnclippedMaxX)
    {
        CMNSUBTILE_DrawRleFlatClipped(nX, nY, pSrc, pDst, pPalette, VISTILE_SIZE);
    }
    else
    {
        CMNSUBTILE_DrawRleFlat(pSrc, pDst, pPalette, VISTILE_SIZE);
    }
}

//D2Gfx.0x6FA729D0
void __fastcall CMNSUBTILE_TileDrawLit(uint8_t* pSrc, int32_t nX, int32_t nY, int32_t* pIntensities)
{
    if (gbForceFlatLighting)
    {
        if ((*(uint8_t*)pIntensities & 0xF8) == 0xF8)
        {
            CMNSUBTILE_DrawVisTile(pSrc, nX, nY);
        }
        else
        {
            CMNSUBTILE_DrawFlatLitVisTile(pSrc, nX, nY, *(uint8_t*)pIntensities);
        }
    }
    else
    {
        const int32_t nLeftIntensity = 32 * pIntensities[0];
        const int32_t nLeftIntensityStep = pIntensities[3] - pIntensities[0];
        const int32_t nRightIntensityStep = pIntensities[2] - pIntensities[1];
        const int32_t nRightIntensity = 32 * pIntensities[1];

        if (gGfxSettings.bLowQuality || std::abs(pIntensities[1] - pIntensities[0]) + std::abs(nLeftIntensityStep) + std::abs(nRightIntensityStep) < 10)
        {
            if ((nLeftIntensity & 0xFFFFFF00) == 0x1F00)
            {
                // Full intensity
                CMNSUBTILE_DrawVisTile(pSrc, nX, nY);
            }
            else
            {
                CMNSUBTILE_DrawFlatLitVisTile(pSrc, nX, nY, *(uint8_t*)pIntensities);
            }
        }
        else
        {
            if (nY >= gnVisTileMinY && nY < gnVisTileMaxY && nX >= gnVisTileMinX && nX < gnVisTileMaxX)
            {
                D2_ASSERT(gCmnInfo_pBuffer);

                uint8_t* pDst = &gCmnInfo_pBuffer[gnLineOffsets[nY + 32]] + nX;
                if (nY < gnVisTileUnclippedMinY || nY >= gnVisTileUnclippedMaxY || nX < gnVisTileUnclippedMinX || nX >= gnVisTileUnclippedMaxX)
                {
                    CMNSUBTILE_DrawRleGouraudClipped(nX, nY, pSrc, pDst, nLeftIntensity, nLeftIntensityStep, nRightIntensity, nRightIntensityStep, VISTILE_SIZE, 8);
                }
                else
                {
                    CMNSUBTILE_DrawRleGouraud(pSrc, pDst, nLeftIntensity, nLeftIntensityStep, nRightIntensity, nRightIntensityStep, VISTILE_SIZE, 8);
                }
            }
        }
    }
}

//D2Gfx.0x6FA72B80
void __fastcall CMNSUBTILE_DrawBlendedVisTile(uint8_t* pSrc, int32_t nX, int32_t nY, uint8_t nAlpha)
{
    uint8_t* pBlendTable = nullptr;
    if (nAlpha >= 192)
    {
        pBlendTable = gpTransPalettes[0];
    }
    else if (nAlpha >= 128)
    {
        pBlendTable = gpTransPalettes[1];
    }
    else if (nAlpha >= 64)
    {
        pBlendTable = gpTransPalettes[2];
    }
    else
    {
        return;
    }

    if (nY < gnVisTileMinY || nY >= gnVisTileMaxY || nX < gnVisTileMinX || nX >= gnVisTileMaxX)
    {
        return;
    }

    D2_ASSERT(gCmnInfo_pBuffer);

    uint8_t* pLine = &gCmnInfo_pBuffer[gnLineOffsets[nY + 32]] + nX;
    uint8_t* pDst = pLine;

    if (nY < gnVisTileUnclippedMinY || nY >= gnVisTileUnclippedMaxY || nX < gnVisTileUnclippedMinX || nX >= gnVisTileUnclippedMaxX)
    {
        int32_t nCurrentY = nY;
        int32_t nLine = 0;
        while (nLine < VISTILE_SIZE)
        {
            const uint8_t nSkip = pSrc[0];
            const uint8_t nLength = pSrc[1];
            pSrc += 2;

            if (nSkip || nLength)
            {
                if (nCurrentY >= gnClipBottom)
                {
                    return;
                }

                pDst += nSkip;

                if (nCurrentY >= gnClipTop)
                {
                    const int32_t nPosX = nX + (int32_t)(pDst - pLine);
                    const int32_t nStart = std::max(gnClipLeft - nPosX, 0);
                    const int32_t nEnd = std::min(std::max(gnClipRight - nPosX, 0), (int32_t)nLength);
                    for (int32_t i = nStart; i < nEnd; ++i)
                    {
                        pDst[i] = pBlendTable[(pDst[i] << 8) + pSrc[i]];
                    }
                }

                pDst += nLength;
                pSrc += nLength;
            }
            else
            {
                ++nLine;
                ++nCurrentY;
                pLine += gCmnInfo_nPitch;
                pDst = pLine;
            }
        }
    }
    else
    {
        int32_t nLines = VISTILE_SIZE;
        while (nLines)
        {
            const uint8_t nSkip = pSrc[0];
            const uint8_t nLength = pSrc[1];
            pSrc += 2;

            if (nSkip || nLength)
            {
                pDst += nSkip;
                for (int32_t i = 0; i < nLength; ++i)
                {
                    pDst[i] = pBlendTable[(pDst[i] << 8) + pSrc[i]];
                }
                pDst += nLength;
                pSrc += nLength;
            }
            else
            {
                --nLines;
                pLine += gCmnInfo_nPitch;
                pDst = pLine;
            }
        }
    }
}

//D2Gfx.0x6FA72DC0
void __fastcall CMNSUBTILE_DrawRoofTile(uint8_t* pSrc, int32_t nX, int32_t nY, int32_t* pIntensities, uint8_t nAlpha)
{
    uint8_t* pBlendTable = nullptr;
    if (nAlpha >= 192)
    {
        pBlendTable = gpTransPalettes[0];
    }
    else if (nAlpha >= 128)
    {
        pBlendTable = gpTransPalettes[1];
    }
    else if (nAlpha >= 64)
    {
        pBlendTable = gpTransPalettes[2];
    }
    else
    {
        return;
    }

    const int32_t nLeftIntensityStep = pIntensities[3] - pIntensities[0];
    const int32_t nRightIntensityStep = pIntensities[2] - pIntensities[1];
    int32_t nLeftIntensity = 32 * pIntensities[0];
    int32_t nRightIntensity = 32 * pIntensities[1];

    if (nY < gnVisTileMinY || nY >= gnVisTileMaxY || nX < gnVisTileMinX || nX >= gnVisTileMaxX)
    {
        return;
    }

    D2_ASSERT(gCmnInfo_pBuffer);

    uint8_t* pLine = &gCmnInfo_pBuffer[gnLineOffsets[nY + 32]] + nX;
    uint8_t* pDst = pLine;
    uint8_t* pGouraud = D2GFX_GetGouraudLine(nLeftIntensity, nRightIntensity, 8);

    if (nY < gnVisTileUnclippedMinY || nY >= gnVisTileUnclippedMaxY || nX < gnVisTileUnclippedMinX || nX >= gnVisTileUnclippedMaxX)
    {
        int32_t nCurrentY = nY;
        int32_t nLine = 0;
        while (nLine < VISTILE_SIZE)
        {
            const uint8_t nSkip = pSrc[0];
            const uint8_t nLength = pSrc[1];
            pSrc += 2;

            if (nSkip || nLength)
            {
                if (nCurrentY >= gnClipBottom)
                {
                    return;
                }

                pDst += nSkip;
                pGouraud += nSkip;

                if (nCurrentY >= gnClipTop)
                {
                    const int32_t nPosX = nX + (int32_t)(pDst - pLine);
                    const int32_t nStart = std::max(gnClipLeft - nPosX, 0);
                    const int32_t nEnd = std::min(std::max(gnClipRight - nPosX, 0), (int32_t)nLength);
                    // Note: the gouraud row is not offset by nStart in the original dll
                    for (int32_t i = nStart; i < nEnd; ++i)
                    {
                        const uint8_t nLitColor = gShadowLightPalettes[pGouraud[i - nStart]].nRemap[pSrc[i]];
                        pDst[i] = pBlendTable[(nLitColor << 8) + pDst[i]];
                    }
                }

                pDst += nLength;
                pGouraud += nLength;
                pSrc += nLength;
            }
            else
            {
                ++nLine;
                ++nCurrentY;
                pLine += gCmnInfo_nPitch;
                pDst = pLine;
                nLeftIntensity += nLeftIntensityStep;
                nRightIntensity += nRightIntensityStep;
                pGouraud = D2GFX_GetGouraudLine(nLeftIntensity, nRightIntensity, 8);
            }
        }
    }
    else
    {
        int32_t nLines = VISTILE_SIZE;
        while (nLines)
        {
            const uint8_t nSkip = pSrc[0];
            const uint8_t nLength = pSrc[1];
            pSrc += 2;

            if (nSkip || nLength)
            {
                pDst += nSkip;
                pGouraud += nSkip;
                for (int32_t i = 0; i < nLength; ++i)
                {
                    const uint8_t nLitColor = gShadowLightPalettes[pGouraud[i]].nRemap[pSrc[i]];
                    pDst[i] = pBlendTable[(nLitColor << 8) + pDst[i]];
                }
                pDst += nLength;
                pGouraud += nLength;
                pSrc += nLength;
            }
            else
            {
                --nLines;
                pLine += gCmnInfo_nPitch;
                pDst = pLine;
                nLeftIntensity += nLeftIntensityStep;
                nRightIntensity += nRightIntensityStep;
                pGouraud = D2GFX_GetGouraudLine(nLeftIntensity, nRightIntensity, 8);
            }
        }
    }
}

//D2Gfx.0x6FA73130
void __fastcall CMNSUBTILE_DrawFloorBlockFlatClipped(D2TileLibraryBlockStrc* pBlock, int32_t nX, int32_t nY, uint8_t* pDst, uint8_t nIntensity)
{
    uint8_t* pPalette = gShadowLightPalettes[nIntensity >> 3].nRemap;

    if (pBlock->nFormat & TILEBLOCKFORMAT_RLE)
    {
        CMNSUBTILE_DrawRleFlatClipped(nX, nY, pBlock->dwOffset_pData, pDst, pPalette, ISOBLOCK_LINES);
    }
    else
    {
        const int32_t nLastLine = std::min(gnClipBottom - nY, ISOBLOCK_LINES);
        for (int32_t nLine = std::max(gnClipTop - nY, 0); nLine < nLastLine; ++nLine)
        {
            const int32_t nLineX = nX + gnIsoBlockLineX[nLine];
            const int32_t nStart = std::max(gnClipLeft - nLineX, 0);
            const int32_t nCount = D2Clamp(gnClipRight - nLineX, 0, gnIsoBlockLineWidth[nLine]) - nStart;
            if (nCount > 0)
            {
                uint8_t* pLineDst = &pDst[nLine * gCmnInfo_nPitch + gnIsoBlockLineX[nLine] + nStart];
                const uint8_t* pLineSrc = &pBlock->dwOffset_pData[gnIsoBlockLineDataOffset[nLine] + nStart];
                for (int32_t i = 0; i < nCount; ++i)
                {
                    pLineDst[i] = pPalette[pLineSrc[i]];
                }
            }
        }
    }
}

//D2Gfx.0x6FA73270
void __fastcall CMNSUBTILE_DrawFloorBlockGouraudClipped(D2TileLibraryBlockStrc* pBlock, int32_t nX, int32_t nY, uint8_t* pDst, int32_t* pIntensities)
{
    const int32_t nLeftIntensityStep = pIntensities[3] - pIntensities[0];
    const int32_t nRightIntensityStep = pIntensities[2] - pIntensities[1];
    int32_t nLeftIntensity = 16 * pIntensities[0];
    int32_t nRightIntensity = 16 * pIntensities[1];

    if (pBlock->nFormat & TILEBLOCKFORMAT_RLE)
    {
        CMNSUBTILE_DrawRleGouraudClipped(nX, nY, pBlock->dwOffset_pData, pDst, nLeftIntensity, nLeftIntensityStep, nRightIntensity, nRightIntensityStep, ISOBLOCK_LINES, 7);
    }
    else
    {
        // Note: the intensities are only interpolated for the visible lines in the original dll
        const int32_t nLastLine = std::min(gnClipBottom - nY, ISOBLOCK_LINES);
        for (int32_t nLine = std::max(gnClipTop - nY, 0); nLine < nLastLine; ++nLine)
        {
            const int32_t nLineX = nX + gnIsoBlockLineX[nLine];
            const int32_t nStart = std::max(gnClipLeft - nLineX, 0);
            const int32_t nCount = D2Clamp(gnClipRight - nLineX, 0, gnIsoBlockLineWidth[nLine]) - nStart;
            if (nCount > 0)
            {
                // Note: the gouraud line is not offset by nStart in the original dll
                const uint8_t* pGouraud = D2GFX_GetGouraudLine(nLeftIntensity, nRightIntensity, 7) + gnIsoBlockLineX[nLine];
                uint8_t* pLineDst = &pDst[nLine * gCmnInfo_nPitch + gnIsoBlockLineX[nLine] + nStart];
                const uint8_t* pLineSrc = &pBlock->dwOffset_pData[gnIsoBlockLineDataOffset[nLine] + nStart];
                for (int32_t i = 0; i < nCount; ++i)
                {
                    pLineDst[i] = gShadowLightPalettes[pGouraud[i]].nRemap[pLineSrc[i]];
                }
            }

            nLeftIntensity += nLeftIntensityStep;
            nRightIntensity += nRightIntensityStep;
        }
    }
}

//D2Gfx.0x6FA73410
void __fastcall CMNSUBTILE_FloorTileDraw(D2TileLibraryEntryStrc* pTileLibraryEntry, int32_t nX, int32_t nY, D2GfxLightExStrc* pLightEx)
{
    D2GfxTileLibraryEntryStrc* pTile = (D2GfxTileLibraryEntryStrc*)pTileLibraryEntry;
    uint8_t* pTileBuffer = gCmnInfo_pBuffer + nY * gCmnInfo_nPitch + nX;

    for (int32_t i = 0; i < pTile->nBlocks; ++i)
    {
        D2TileLibraryBlockStrc* pBlock = &pTile->pBlocks[i];
        uint8_t* pDst = pTileBuffer + pBlock->nPosY * gCmnInfo_nPitch + pBlock->nPosX;

        if (!(pBlock->nFormat & TILEBLOCKFORMAT_VISIBLE))
        {
            continue;
        }

        const int32_t nBlockX = nX + pBlock->nPosX;
        const int32_t nBlockY = nY + pBlock->nPosY;
        if (nBlockY < gnFloorBlockMinY || nBlockY >= gnFloorBlockMaxY || nBlockX < gnFloorBlockMinX || nBlockX >= gnFloorBlockMaxX)
        {
            continue;
        }

        // Light grid around the block, 8 entries per row
        const D2GfxLightExStrc* pLight = &pLightEx[pBlock->nGridX + 8 * pBlock->nGridY];

        int32_t bFlatLight = 0;
        uint8_t nIntensity = 0;
        int32_t pIntensities[4];

        if (gbForceFlatLighting)
        {
            bFlatLight = 1;
            nIntensity = pLight[9].nIntensity;
        }
        else
        {
            const int32_t nCenterIntensity = pLight[9].nIntensity;
            const int32_t nRightIntensity = pLight[10].nIntensity;
            const int32_t nBottomIntensity = pLight[17].nIntensity;
            const int32_t nBottomRightIntensity = pLight[18].nIntensity;

            if (gGfxSettings.bLowQuality
                || std::abs(nRightIntensity - nCenterIntensity) + std::abs(nBottomIntensity - nCenterIntensity) + std::abs(nBottomRightIntensity - nRightIntensity) < 10)
            {
                bFlatLight = 1;
                nIntensity = nCenterIntensity;
            }
            else
            {
                // Average the intensity at each corner of the block
                pIntensities[0] = (pLight[8].nIntensity + nCenterIntensity + pLight[16].nIntensity + nBottomIntensity) >> 2;
                pIntensities[1] = (pLight[1].nIntensity + pLight[2].nIntensity + nCenterIntensity + nRightIntensity) >> 2;
                pIntensities[2] = (nRightIntensity + pLight[11].nIntensity + nBottomIntensity + pLight[19].nIntensity) >> 2;
                pIntensities[3] = (nBottomIntensity + nBottomRightIntensity + pLight[25].nIntensity + pLight[26].nIntensity) >> 2;
            }
        }

        if (nBlockY >= gnFloorBlockUnclippedMinY && nBlockY < gnFloorBlockUnclippedMaxY && nBlockX >= gnFloorBlockUnclippedMinX && nBlockX < gnFloorBlockUnclippedMaxX)
        {
            if (bFlatLight)
            {
                uint8_t* pPalette = gShadowLightPalettes[nIntensity >> 3].nRemap;
                if (pBlock->nFormat & TILEBLOCKFORMAT_RLE)
                {
                    CMNSUBTILE_DrawRleFlat(pBlock->dwOffset_pData, pDst, pPalette, ISOBLOCK_LINES);
                }
                else
                {
                    CMNSUBTILE_DrawIsoBlockFlat(pBlock->dwOffset_pData, pDst, pPalette);
                }
            }
            else
            {
                if (pBlock->nFormat & TILEBLOCKFORMAT_RLE)
                {
                    CMNSUBTILE_DrawRleGouraud(pBlock->dwOffset_pData, pDst, 16 * pIntensities[0], pIntensities[3] - pIntensities[0], 16 * pIntensities[1], pIntensities[2] - pIntensities[1], ISOBLOCK_LINES, 7);
                }
                else
                {
                    CMNSUBTILE_DrawIsoBlockGouraud(pBlock->dwOffset_pData, pDst, 16 * pIntensities[0], pIntensities[3] - pIntensities[0], 16 * pIntensities[1], pIntensities[2] - pIntensities[1]);
                }
            }
        }
        else
        {
            if (bFlatLight)
            {
                CMNSUBTILE_DrawFloorBlockFlatClipped(pBlock, nBlockX, nBlockY, pDst, nIntensity);
            }
            else
            {
                CMNSUBTILE_DrawFloorBlockGouraudClipped(pBlock, nBlockX, nBlockY, pDst, pIntensities);
            }
        }
    }
}
