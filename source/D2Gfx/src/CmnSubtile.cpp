#include "CmnSubtile.h"

#include <algorithm>

#include <D2Math.h>
#include <Fog.h>
#include <Storm.h>

#include "DisplayType.h"
#include "D2Gfx.h"


extern int32_t gnViewMatrixAdjust_6FA8D254;
extern DisplayType gnDisplayType;


constexpr int32_t GOURAUD_SHIFT = 24;

PALETTEENTRY** dword_6FA81448;

PALETTEENTRY* gpPaletteTable_6FA81460[72];
D2PaletteStrc stru_6FA81580[49];

uint8_t* gCmnInfo_pBuffer_6FA8144C;
int32_t dword_6FA8D244;
int32_t dword_6FA84694;
int32_t dword_6FA84688;
int32_t dword_6FA8D23C;
int32_t dword_6FA8D24C;
int32_t dword_6FA8D240;
int32_t dword_6FA8468C;
int32_t dword_6FA846A0;
int32_t dword_6FA8D220;
int32_t dword_6FA8D224;
int32_t dword_6FA84698;
int32_t dword_6FA846A8[732];
int32_t dword_6FA8D228;
int32_t dword_6FA8D22C;
int32_t dword_6FA8D230;
int32_t dword_6FA85218;
int32_t dword_6FA8D238;
int32_t dword_6FA8D248;
int32_t dword_6FA8D234;
int32_t dword_6FA84690;
int32_t dword_6FA8469C;

int32_t dword_6FA8D250;

int32_t dword_6FA81450;
int32_t dword_6FA81454;
int32_t dword_6FA81458;

int32_t dword_6FA84680;
int32_t dword_6FA7E040;

uint8_t byte_6FA85220[32][32][32];


// TODO: Names
constexpr int32_t dword_6FA7E064[] =
{
    14, 12, 10, 8, 6, 4, 2, 0, 2, 4, 6, 8, 10, 12, 14
};

constexpr int32_t dword_6FA7E0A0[] =
{
    4, 8, 12, 16, 20, 24, 28, 32, 28, 24, 20, 16, 12, 8, 4
};

constexpr int32_t dword_6FA7E0DC[] =
{
    0, 4, 12, 24, 40, 60, 84, 112, 144, 172, 196, 216, 232, 244, 252
};



//D2Gfx.0x6FA71000
void __fastcall D2GFX_InitGouraudCache_6FA71000()
{
    DGFX_InitGouraudCache_6FA72570();
    gCmnInfo_pBuffer_6FA8144C = nullptr;
}

//D2Gfx.0x6FA71010
void __fastcall D2GFX_FillYBufferTable_6FA71010(uint8_t* a1, int32_t a2, int32_t a3, int32_t a4)
{
    gCmnInfo_pBuffer_6FA8144C = a1;
    dword_6FA81450 = a2;
    dword_6FA81454 = a3;
    dword_6FA81458 = a4;

    if (a2 != dword_6FA84680)
    {
        dword_6FA81450 = a2;
        dword_6FA84680 = a2;
        sub_6FA72700();
    }

    if (gnViewMatrixAdjust_6FA8D254 != dword_6FA7E040)
    {
        sub_6FA72630(a2, a3);
    }
}

//D2Gfx.0x6FA71070
void __fastcall sub_6FA71070(D2PaletteTableStrc* pPaletteTable)
{
    memcpy(gpPaletteTable_6FA81460, pPaletteTable, sizeof(gpPaletteTable_6FA81460));
    dword_6FA81448 = gpPaletteTable_6FA81460;
    for (int32_t i = 0; i < std::size(stru_6FA81580); ++i)
    {
        memcpy(&stru_6FA81580[i], gpPaletteTable_6FA81460[i + 3], sizeof(D2PaletteStrc));
    }
}

//D2Gfx.0x6FA710C0
int32_t __fastcall D2GFX_SetContrastAndGamma_6FA710C0()
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
void __fastcall sub_6FA71130(uint8_t* a1, uint8_t* a2, uint8_t* a3)
{
    // Unrolled in original dll
    for (int i = 0; i < 15; ++i)
    {
        uint8_t* v5 = &a1[dword_6FA7E0DC[i]];
        uint8_t* v6 = &a2[dword_6FA7E064[i]];
        v6[3] = a3[v5[3]];
        v6[2] = a3[v5[2]];
        v6[1] = a3[v5[1]];
        v6[0] = a3[v5[0]];
        a2 += dword_6FA81450;
    }
}

//D2Gfx.0x6FA71340
void __fastcall sub_6FA71340(uint8_t* a1, uint8_t* a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    uint8_t* v6 = a2;

    for (int32_t i = 0; i < std::size(dword_6FA7E064); ++i)
    {
        const int32_t v8 = dword_6FA7E064[i];
        uint8_t* v9 = (uint8_t*)((char*)byte_6FA85220 + 32 * ((a5 >> 7) + 32 * (a3 >> 7)) + v8);
        uint8_t* v10 = &a1[dword_6FA7E0DC[i]];

        // Unrolled in orginal dll
        const int nValues = dword_6FA7E0A0[i];
        for (int j = nValues-1; j >= 0; j--)
        {
            v6[v8 + j] = stru_6FA81580[v9[j]].unk0x00[v10[j]];
        }

        v6 += dword_6FA81450;
        a3 += a4;
        a5 += a6;
    }
}

//D2Gfx.0x6FA71720
void __fastcall sub_6FA71720(uint8_t* a1, uint8_t* a2, uint8_t* a3, int32_t a4)
{
    // TODO: Names
    uint8_t* v4 = a2;
    while (a4)
    {
        uint8_t v7 = a1[0];
        uint8_t v8 = a1[1];

        a1 += 2;

        if (v7 || v8)
        {
            uint8_t* v9 = &v4[v7];
            // Unrolled in original dll
            for (int j = v8 - 1; j >= 0; j--)
            {
                v9[j] = a3[a1[j]];
            }

            v4 = &v9[v8];
            a1 += v8;
        }
        else
        {
            a2 += dword_6FA81450;
            --a4;
            v4 = a2;
        }
    }
}

//D2Gfx.0x6FA71970
void __fastcall sub_6FA71970(uint8_t* a1, uint8_t* a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, uint8_t a8)
{
    // TODO: Names
    uint8_t* v8 = a1;
    uint8_t* v9 = a2;
    uint8_t* v10 = (uint8_t*)((char*)byte_6FA85220 + 32 * ((a5 >> a8) + 32 * (a3 >> a8)));
    
    while (a7)
    {
        const uint8_t v11 = v8[0];
        const uint8_t v12 = v8[1];
        v8 += 2;
        
        if (v11 || v12)
        {
            uint8_t* v13 = &v9[v11];
            uint8_t* v14 = &v10[v11];
            // Unrolled in original dll
            for (int i = v12 - 1; i >= 0; i--)
            {
                v13[i] = stru_6FA81580[v14[i]].unk0x00[v8[i]];
            }

            v9 = &v13[v12];
            v8 += v12;
            v10 = &v14[v12];
        }
        else
        {
            --a7;
            a3 += a4;
            a5 += a6;
            a2 += dword_6FA81450;
            v9 = a2;
            v10 = (BYTE*)((char*)byte_6FA85220 + 32 * ((a5 >> a8) + 32 * (a3 >> a8)));
        }
    }
}

//D2Gfx.0x6FA71D90) --------------------------------------------------------
void __fastcall sub_6FA71D90(int32_t a1, int32_t a2, uint8_t* a3, uint8_t* a4, uint8_t* a5, int32_t a6)
{
    BYTE* v6; // ebp@1
    BYTE* v7; // eax@1
    int v8; // ecx@2
    BYTE* v9; // edx@2
    int v10; // ebx@3
    BYTE* v11; // edx@3
    int v12; // edi@3
    int v13; // ebx@3
    BYTE* v14; // eax@7
    int v15; // edi@8
    int v16; // ebp@8
    int v17; // edi@10
    int v18; // ecx@10
    int v19; // ecx@10
    int v20; // ebx@12
    BYTE* v21; // eax@13
    BYTE* v22; // edx@13
    int v23; // [sp+10h] [bp-10h]@1
    int v24; // [sp+14h] [bp-Ch]@2
    int v25; // [sp+18h] [bp-8h]@1
    BYTE* v26; // [sp+1Ch] [bp-4h]@8
    BYTE* a3a; // [sp+24h] [bp+4h]@8

    v25 = a1;
    v6 = a4;
    v7 = a4;
    v23 = 0;
    v8 = a2;
    v9 = a3;
    v24 = v8;
    while (v23 < a6)
    {
        v10 = *v9;
        v11 = v9 + 1;
        v12 = v10;
        v13 = *v11;
        v9 = v11 + 1;

        if (v12 || v13)
        {
            if (v8 >= dword_6FA8D240)
                return;

            v14 = &v7[v12];
            if (v8 < dword_6FA84690)
            {
                v7 = &v14[v13];
                v9 += v13;
            }
            else
            {
                v15 = v25 + v14 - v6;
                a3a = &v9[v13];
                v16 = dword_6FA8D220 - v15;
                v26 = &v14[v13];

                if (dword_6FA8D220 - v15 <= 0)
                    v16 = 0;

                v17 = dword_6FA8469C - v15;
                v19 = v17 & (v17 <= 0 - 1);

                if (v19 < v13)
                    v13 = v19;

                v20 = v13 - v16;
                if (v20 > 0)
                {
                    v21 = &v14[v16];
                    v22 = &v9[v16];
                    // Unrolled in original dll
                    for (int i = v20 - 1; i >= 0; i--)
                    {
                        v21[i] = a5[v22[i]];
                    }
                }
                v9 = a3a;
                v7 = v26;
                v8 = v24;
                v6 = a4;
            }
        }
        else
        {
            ++v8;
            v6 += dword_6FA81450;
            ++v23;
            v24 = v8;
            a4 = v6;
            v7 = v6;
        }
    }
}

//D2Gfx.0x6FA72090) --------------------------------------------------------
void __fastcall sub_6FA72090(int32_t a1, int32_t a2, uint8_t* a3, uint8_t* a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8, int32_t a9, uint8_t a10)
{
    char v10; // cl@1
    BYTE* v11; // ebp@1
    BYTE* v12; // esi@1
    BYTE* v13; // eax@1
    BYTE* v14; // edx@2
    int v15; // ebx@3
    BYTE* v16; // edx@3
    int v17; // ebx@3
    BYTE* v18; // esi@7
    BYTE* v19; // eax@7
    int v20; // edi@8
    int v21; // ebp@8
    int v22; // ecx@10
    int v23; // ecx@12
    BYTE* v24; // esi@13
    BYTE* v25; // edx@13
    int v26; // [sp+10h] [bp-14h]@2
    int v27; // [sp+14h] [bp-10h]@1
    int v28; // [sp+18h] [bp-Ch]@1
    BYTE* v29; // [sp+20h] [bp-4h]@8
    int a3a; // [sp+28h] [bp+4h]@3
    BYTE* a3b; // [sp+28h] [bp+4h]@8

    v28 = a1;
    v10 = a10;
    v11 = a4;
    v27 = 0;
    v12 = a4;
    v13 = (BYTE*)((char*)byte_6FA85220 + 32 * ((a7 >> a10) + 32 * (a5 >> a10)));
    v26 = a2;
    v14 = a3;
    while (v27 < a9)
    {
        v15 = *v14;
        v16 = v14 + 1;
        a3a = v15;
        v17 = *v16;
        v14 = v16 + 1;
        if (a3a || v17)
        {
            if (v26 >= dword_6FA8D240)
                return;
            v18 = &v12[a3a];
            v19 = &v13[a3a];
            if (v26 < dword_6FA84690)
            {
                v12 = &v18[v17];
                v13 = &v19[v17];
                v14 += v17;
            }
            else
            {
                v29 = &v18[v17];
                v20 = v28 + v18 - v11;
                v21 = dword_6FA8D220 - v20;
                a3b = &v14[v17];
                if (dword_6FA8D220 - v20 <= 0)
                    v21 = 0;
                v22 = ((dword_6FA8469C - v20 <= 0) - 1) & (dword_6FA8469C - v20);
                if (v22 >= v17)
                    v22 = v17;
                v23 = v22 - v21;
                if (v23 > 0)
                {
                    v24 = &v18[v21];
                    v25 = &v14[v21];
                    for (int i = v23 - 1; i >= 0; i--)
                    {
                        v24[i] = stru_6FA81580[v19[i]].unk0x00[v25[i]];
                    }
                }

                v14 = a3b;
                v12 = v29;
                v10 = a10;
                v11 = a4;
                v13 = &v19[v17];
            }
        }
        else
        {
            ++v26;
            v11 += dword_6FA81450;
            a5 += a6;
            a7 += a8;
            ++v27;
            a4 = v11;
            v12 = v11;
            v13 = (BYTE*)((char*)byte_6FA85220 + 32 * ((a7 >> v10) + 32 * (a5 >> v10)));
        }
    }
}

//D2Gfx.0x6FA72570
void __fastcall DGFX_InitGouraudCache_6FA72570()
{
    // TODO: Check, Names
    int32_t v1 = 0;
    int32_t v10 = 0;
    for (int32_t i = 0; i < 32; ++i)
    {
        int32_t nVal = i << GOURAUD_SHIFT;
        const int32_t nOriginalValue = nVal;
        for (int32_t j = 0; j < 32; ++j)
        {
            for (int32_t k = 0; k < 32; ++k)
            {
                nVal = D2Clamp(nVal, 0, 0x1F000000);
                byte_6FA85220[i][j][k] = (nVal >> GOURAUD_SHIFT) & 0XFF;
                nVal += v1 >> 5;
            }

            D2_ASSERT((nVal >> GOURAUD_SHIFT) == j);

            v1 += (1 << GOURAUD_SHIFT);
            nVal = nOriginalValue;
        }

        v10 -= (1 << GOURAUD_SHIFT);
        v1 = v10;
    }
}

//D2Gfx.0x6FA72630
void __fastcall sub_6FA72630(int32_t a1, int32_t a2)
{
    //TODO: Names
    int32_t v2 = 0;
    if (gnViewMatrixAdjust_6FA8D254 == 1)
    {
        a1 -= a1 / 2;
    }
    else if (gnViewMatrixAdjust_6FA8D254 == 2)
    {
        v2 = a1 / 2;
    }

    dword_6FA8D244 = v2 - 32;
    dword_6FA84694 = a2 - 63;
    dword_6FA84688 = v2 - 32;
    dword_6FA8D23C = a2 - 47;
    dword_6FA8D24C = a2 - 47;
    dword_6FA8D240 = a2 - 47;
    dword_6FA8468C = v2;
    dword_6FA846A0 = v2;
    dword_6FA8D220 = v2;
    dword_6FA8D224 = -16;
    dword_6FA84698 = a1;
    dword_6FA8D228 = 0;
    dword_6FA8D22C = a1 - 32;
    dword_6FA8D230 = -32;
    dword_6FA85218 = a1;
    dword_6FA8D238 = 0;
    dword_6FA8D248 = a2 - 79;
    dword_6FA8D234 = a1 - 32;
    dword_6FA84690 = 0;
    dword_6FA8469C = a1;
}

//D2Gfx.0x6FA72700
void __fastcall sub_6FA72700()
{
    int32_t v2 = -32 * dword_6FA81450;
    for (int32_t i = 0; i < std::size(dword_6FA846A8); ++i)
    {
        dword_6FA846A8[i] = v2;
        v2 += dword_6FA81450;
    }
}

//D2Gfx.0x6FA72730) --------------------------------------------------------
void __fastcall D2GFX_DrawVisTile_6FA72730(uint8_t* a1, int32_t nX, int32_t nY)
{
//    int32_t v3; // eax@1
//    int32_t v4; // ebp@1
//    int32_t v5; // esi@1
//    int32_t v6; // edx@8
//    int32_t v7; // ebx@8
//    int32_t v8; // eax@13
//    int32_t v9; // ebp@13
//    int32_t v10; // esi@13
//    unsigned int32_t v11; // eax@13
//    void* v12; // edi@16
//    const void* v13; // esi@16
//    int32_t v14; // edi@19
//    int32_t v15; // eax@20
//    int32_t v16; // ebp@20
//    int32_t v17; // esi@20
//    int32_t v18; // eax@20
//    int32_t v19; // ebx@24
//    int32_t v20; // ecx@25
//    int32_t v21; // eax@25
//    int32_t v22; // esi@27
//    int32_t v23; // zf@27
//    int32_t v24; // ecx@27
//    int32_t v25; // ecx@27
//    int32_t v26; // esi@27
//    unsigned int32_t v27; // ecx@29
//    int32_t v28; // [sp+10h] [bp-Ch]@8
//    int32_t v29; // [sp+10h] [bp-Ch]@20
//    int32_t v30; // [sp+14h] [bp-8h]@19
//    int32_t v31; // [sp+18h] [bp-4h]@1
//    signed int32_t nYa; // [sp+20h] [bp+4h]@12
//    signed int32_t nYb; // [sp+20h] [bp+4h]@19
//
//    v3 = nY;
//    v4 = a1;
//    v5 = nX;
//    v31 = nX;
//    if (nY >= dword_6FA8D230 && nY < dword_6FA8D24C && nX >= dword_6FA84688 && nX < dword_6FA85218)
//    {
//        D2_ASSERT(gCmnInfo_pBuffer_6FA8144C);
//
//        v6 = nX + gCmnInfo_pBuffer_6FA8144C + dword_6FA84728[nY];
//        v28 = v5 + gCmnInfo_pBuffer_6FA8144C + dword_6FA84728[nY];
//        v7 = v5 + gCmnInfo_pBuffer_6FA8144C + dword_6FA84728[nY];
//        if (nY < dword_6FA8D238 || nY >= dword_6FA8D248 || v5 < dword_6FA846A0 || v5 >= dword_6FA8D234)
//        {
//            v14 = nY;
//            nYb = 0;
//            v30 = v3;
//            do
//            {
//                v15 = *(_uint8_t*)v4;
//                v16 = v4 + 1;
//                v17 = v15;
//                v18 = *(_uint8_t*)v16;
//                v4 = v16 + 1;
//                v29 = v18;
//                if (v17 || v18)
//                {
//                    if (v14 >= dword_6FA8D240)
//                        return;
//                    v19 = v17 + v7;
//                    if (v14 < dword_6FA84690)
//                    {
//                        v7 = v18 + v19;
//                        v4 += v18;
//                    }
//                    else
//                    {
//                        v20 = v31 + v19 - v6;
//                        v21 = dword_6FA8D220 - v20;
//                        if (dword_6FA8D220 - v20 <= 0)
//                            v21 = 0;
//                        v22 = dword_6FA8469C - v20;
//                        v23 = dword_6FA8469C == v20;
//                        v24 = dword_6FA8469C - v20 < 0;
//                        LOBYTE(v24) = v24 | v23;
//                        v25 = v22 & (v24 - 1);
//                        v26 = v29;
//                        if (v25 >= v29)
//                            v25 = v29;
//                        v27 = v25 - v21;
//                        if ((signed int32_t)v27 > 0)
//                        {
//                            memcpy((void*)(v21 + v19), (const void*)(v21 + v4), v27);
//                            v26 = v29;
//                        }
//                        v14 = v30;
//                        v4 += v26;
//                        v7 = v26 + v19;
//                    }
//                }
//                else
//                {
//                    ++v14;
//                    v6 += dword_6FA81450;
//                    ++nYb;
//                    v30 = v14;
//                    v7 = v6;
//                }
//            }
//            while (nYb < 32);
//        }
//        else
//        {
//            nYa = 32;
//            do
//            {
//                v8 = *(_uint8_t*)v4;
//                v9 = v4 + 1;
//                v10 = v8;
//                v11 = *(_uint8_t*)v9;
//                v4 = v9 + 1;
//                if (v10 || v11)
//                {
//                    v12 = (void*)(v10 + v7);
//                    v7 += v11 + v10;
//                    memcpy(v12, (const void*)v4, 4 * (v11 >> 2));
//                    v13 = (const void*)(v4 + 4 * (v11 >> 2));
//                    v6 = v28;
//                    v4 += v11;
//                    memcpy((char*)v12 + 4 * (v11 >> 2), v13, v11 & 3);
//                }
//                else
//                {
//                    v6 += dword_6FA81450;
//                    --nYa;
//                    v28 = v6;
//                    v7 = v6;
//                }
//            }
//            while (nYa);
//        }
//    }
}

//D2Gfx.0x6FA72900
void __fastcall sub_6FA72900(uint8_t* a1, int32_t nX, int32_t nY, uint8_t a4)
{
    if (nY < dword_6FA8D230 || nY >= dword_6FA8D24C || nX < dword_6FA84688 || nX >= dword_6FA85218)
    {
        return;
    }

    uint8_t* pBuffer = &gCmnInfo_pBuffer_6FA8144C[dword_6FA846A8[nY + 32]] + nX;

    D2_ASSERT(gCmnInfo_pBuffer_6FA8144C);

    if (nY < dword_6FA8D238 || nY >= dword_6FA8D248 || nX < dword_6FA846A0 || nX >= dword_6FA8D234)
    {
        sub_6FA71D90(nX, nY, a1, pBuffer, stru_6FA81580[(uint32_t)a4 >> 3].unk0x00, 32);
    }
    else
    {
        sub_6FA71720(a1, pBuffer, stru_6FA81580[(uint32_t)a4 >> 3].unk0x00, 32);
    }
}

//D2Gfx.0x6FA729D0
void __fastcall TILE_TileDrawLit_6FA729D0(uint8_t* a1, int32_t nX, int32_t nY, int32_t* pIntensities)
{
    if (dword_6FA8D250)
    {
        if ((*(uint8_t*)pIntensities & 0xF8) == 0xF8)
        {
            D2GFX_DrawVisTile_6FA72730(a1, nX, nY);
        }
        else
        {
            sub_6FA72900(a1, nX, nY, *(uint8_t*)pIntensities);
        }
    }
    else
    {
        // TODO: Names
        const int32_t v6 = 32 * pIntensities[0];
        const int32_t v7 = pIntensities[3] - pIntensities[0];
        const int32_t v8 = pIntensities[2] - pIntensities[1];
        const int32_t v9 = 32 * pIntensities[1];

        if (gGfxSettings.bLowQuality || std::abs(pIntensities[1] - pIntensities[0]) + std::abs(v7) + std::abs(v8) < 10)
        {
            if ((v6 & 0xFFFFFF00) == 0x1F00)
            {
                D2GFX_DrawVisTile_6FA72730(a1, nX, nY);
            }
            else
            {
                sub_6FA72900(a1, nX, nY, *(uint8_t*)pIntensities);
            }
        }
        else
        {
            if (nY >= dword_6FA8D230 && nY < dword_6FA8D24C && nX >= dword_6FA84688 && nX < dword_6FA85218)
            {
                D2_ASSERT(gCmnInfo_pBuffer_6FA8144C);

                uint8_t* pBuffer = &gCmnInfo_pBuffer_6FA8144C[dword_6FA846A8[nY + 32]] + nX;
                if (nY < dword_6FA8D238 || nY >= dword_6FA8D248 || nX < dword_6FA846A0 || nX >= dword_6FA8D234)
                {
                    sub_6FA72090(nX, nY, a1, pBuffer, v6, v7, v9, v8, 32, 8);
                }
                else
                {
                    sub_6FA71970(a1, pBuffer, v6, v7, v9, v8, 32, 8);
                }
            }
        }
    }
}

//D2Gfx.0x6FA72B80) --------------------------------------------------------
void __fastcall D2GFX_DrawBlendedVisTile_6FA72B80(int32_t a1, int32_t nX, int32_t nY, uint8_t nAlpha)
{
//    LPPALETTEENTRY v4; // eax@2
//    int32_t v5; // esi@7
//    int32_t v6; // edi@14
//    int32_t v7; // eax@14
//    int32_t v8; // edx@19
//    int32_t v9; // ecx@19
//    int32_t v10; // ebp@19
//    int32_t v11; // edx@19
//    int32_t v12; // esi@22
//    int32_t v13; // ebx@23
//    int32_t v14; // ebp@23
//    int32_t v15; // ebx@26
//    int32_t v16; // edx@27
//    int32_t v17; // ecx@27
//    int32_t v18; // esi@27
//    int32_t v19; // edx@27
//    int32_t v20; // ebp@27
//    int32_t v21; // eax@31
//    int32_t v22; // esi@32
//    int32_t v23; // edi@32
//    int32_t v24; // edx@34
//    int32_t v25; // edx@34
//    int32_t v26; // ebp@36
//    int32_t v27; // ecx@37
//    int32_t v28; // eax@37
//    int32_t i; // esi@37
//    int32_t v30; // ebx@38
//    int32_t v31; // ebp@38
//    int32_t v32; // [sp+10h] [bp-14h]@26
//    int32_t v33; // [sp+14h] [bp-10h]@14
//    int32_t v34; // [sp+18h] [bp-Ch]@1
//    int32_t v35; // [sp+1Ch] [bp-8h]@32
//    int32_t v36; // [sp+20h] [bp-4h]@32
//    signed int32_t nYa; // [sp+28h] [bp+4h]@18
//    signed int32_t nYb; // [sp+28h] [bp+4h]@26
//    LPPALETTEENTRY nAlphaa; // [sp+2Ch] [bp+8h]@7
//
//    v34 = nX;
//    if (nAlpha < 0xC0u)
//    {
//        if (nAlpha < 0x80u)
//        {
//            if (nAlpha < 0x40u)
//            {
//                return;
//            }
//            v4 = dword_6FA81448[2];
//        }
//        else
//        {
//            v4 = dword_6FA81448[1];
//        }
//    }
//    else
//    {
//        v4 = *dword_6FA81448;
//    }
//    v5 = nY;
//    nAlphaa = v4;
//    if (nY >= dword_6FA8D230 && nY < dword_6FA8D24C && nX >= dword_6FA84688 && nX < dword_6FA85218)
//    {
//        D2_ASSERT(gCmnInfo_pBuffer_6FA8144C);
//
//        v6 = nX + gCmnInfo_pBuffer_6FA8144C + dword_6FA84728[nY];
//        v33 = nX + gCmnInfo_pBuffer_6FA8144C + dword_6FA84728[nY];
//        v7 = nX + gCmnInfo_pBuffer_6FA8144C + dword_6FA84728[nY];
//        if (nY < dword_6FA8D238 || nY >= dword_6FA8D248 || nX < dword_6FA846A0 || nX >= dword_6FA8D234)
//        {
//            v15 = nY;
//            nYb = 0;
//            v32 = v5;
//            do
//            {
//                v16 = *(_uint8_t*)a1;
//                v17 = a1 + 1;
//                v18 = v16;
//                v19 = *(_uint8_t*)v17;
//                a1 = v17 + 1;
//                v20 = v19;
//                if (v18 || v19)
//                {
//                    if (v15 >= dword_6FA8D240)
//                    {
//                        return;
//                    }
//                    v21 = v18 + v7;
//                    if (v15 < dword_6FA84690)
//                    {
//                        v7 = v19 + v21;
//                        a1 += v19;
//                    }
//                    else
//                    {
//                        v35 = a1 + v19;
//                        v36 = v21 + v19;
//                        v22 = v34 + v21 - v6;
//                        v23 = dword_6FA8D220 - v22;
//                        if (dword_6FA8D220 - v22 <= 0)
//                        {
//                            v23 = 0;
//                        }
//                        v24 = dword_6FA8469C - v22 < 0;
//                        LOBYTE(v24) = dword_6FA8469C - v22 <= 0;
//                        v25 = (dword_6FA8469C - v22) & (v24 - 1);
//                        if (v25 < v20)
//                        {
//                            v20 = v25;
//                        }
//                        v26 = v20 - v23;
//                        if (v26 > 0)
//                        {
//                            v27 = v23 + a1;
//                            v28 = v23 + v21;
//                            for (i = v27 + v26; v27 < (unsigned int32_t)i; *(_uint8_t*)(v28 - 1) = *(_uint8_t*)(v30 + v31))
//                            {
//                                v30 = *(_uint8_t*)v27;
//                                v31 = (int32_t)&nAlphaa[64 * *(_uint8_t*)v28];
//                                ++v27;
//                                ++v28;
//                            }
//                        }
//                        a1 = v35;
//                        v7 = v36;
//                        v15 = v32;
//                        v6 = v33;
//                    }
//                }
//                else
//                {
//                    ++v15;
//                    v6 += dword_6FA81450;
//                    ++nYb;
//                    v32 = v15;
//                    v33 = v6;
//                    v7 = v6;
//                }
//            }
//            while (nYb < 32);
//        }
//        else
//        {
//            nYa = 32;
//            do
//            {
//                v8 = *(_uint8_t*)a1;
//                v9 = a1 + 1;
//                v10 = v8;
//                v11 = *(_uint8_t*)v9;
//                a1 = v9 + 1;
//                if (v10 || v11)
//                {
//                    v12 = a1 + v11;
//                    v7 += v10;
//                    if (a1 < (unsigned int32_t)(a1 + v11))
//                    {
//                        do
//                        {
//                            v13 = *(_uint8_t*)a1;
//                            v14 = (int32_t)&nAlphaa[64 * *(_uint8_t*)v7];
//                            ++a1;
//                            ++v7;
//                            *(_uint8_t*)(v7 - 1) = *(_uint8_t*)(v13 + v14);
//                        }
//                        while (a1 < (unsigned int32_t)v12);
//                    }
//                }
//                else
//                {
//                    v6 += dword_6FA81450;
//                    --nYa;
//                    v7 = v6;
//                }
//            }
//            while (nYa);
//        }
//    }
}

//D2Gfx.0x6FA72DC0) --------------------------------------------------------
void __fastcall D2GFX_DrawRoofTile_6FA72DC0(int32_t a1, int32_t nX, int32_t nY, int32_t* pIntensities, uint8_t nAlpha)
{
//    LPPALETTEENTRY v5; // eax@2
//    signed int32_t v6; // edi@7
//    signed int32_t v7; // ebx@7
//    int32_t v8; // ebp@14
//    int32_t v9; // edx@14
//    char* v10; // eax@14
//    int32_t v11; // esi@14
//    int32_t v12; // edx@19
//    int32_t v13; // ecx@19
//    int32_t v14; // ST20_4@19
//    int32_t v15; // edx@19
//    int32_t v16; // edx@19
//    int32_t v17; // ebx@24
//    unsigned __int32_t8 v18; // ST34_1@24
//    int32_t v19; // ebx@24
//    int32_t v20; // ebx@30
//    int32_t v21; // esi@34
//    int32_t v22; // eax@34
//    int32_t v23; // edi@35
//    int32_t v24; // ebp@35
//    int32_t v25; // edi@37
//    int32_t v26; // edx@37
//    int32_t v27; // edx@37
//    int32_t v28; // ebx@39
//    int32_t v29; // ecx@40
//    int32_t v30; // esi@40
//    int32_t i; // edi@40
//    int32_t v32; // ebx@41
//    unsigned __int32_t8 v33; // ST38_1@41
//    int32_t v34; // ebx@41
//    LPPALETTEENTRY v35; // [sp+10h] [bp-2Ch]@7
//    signed int32_t v36; // [sp+14h] [bp-28h]@29
//    int32_t v37; // [sp+18h] [bp-24h]@1
//    int32_t v38; // [sp+18h] [bp-24h]@19
//    int32_t v39; // [sp+1Ch] [bp-20h]@7
//    int32_t v40; // [sp+20h] [bp-1Ch]@7
//    int32_t v41; // [sp+24h] [bp-18h]@23
//    int32_t v42; // [sp+24h] [bp-18h]@30
//    int32_t v43; // [sp+28h] [bp-14h]@14
//    int32_t v44; // [sp+30h] [bp-Ch]@35
//    int32_t v45; // [sp+34h] [bp-8h]@35
//    int32_t v46; // [sp+38h] [bp-4h]@35
//    signed int32_t nYa; // [sp+40h] [bp+4h]@18
//    int32_t nYb; // [sp+40h] [bp+4h]@29
//    int32_t pIntensitiesa; // [sp+44h] [bp+8h]@7
//    signed int32_t nAlphaa; // [sp+48h] [bp+Ch]@7
//
//    v37 = nX;
//    if (nAlpha < 0xC0u)
//    {
//        if (nAlpha < 0x80u)
//        {
//            if (nAlpha < 0x40u)
//            {
//                return;
//            }
//            v5 = dword_6FA81448[2];
//        }
//        else
//        {
//            v5 = dword_6FA81448[1];
//        }
//    }
//    else
//    {
//        v5 = *dword_6FA81448;
//    }
//    v35 = v5;
//    v39 = pIntensities[3] - *pIntensities;
//    v40 = pIntensities[2] - pIntensities[1];
//    v6 = 32 * *pIntensities;
//    v7 = 32 * pIntensities[1];
//    pIntensitiesa = 32 * *pIntensities;
//    nAlphaa = v7;
//    if (nY >= dword_6FA8D230 && nY < dword_6FA8D24C && nX >= dword_6FA84688 && nX < dword_6FA85218)
//    {
//        D2_ASSERT(gCmnInfo_pBuffer_6FA8144C);
//
//        v8 = nX + gCmnInfo_pBuffer_6FA8144C + dword_6FA84728[nY];
//        v9 = nY;
//        v10 = (char*)&unk_6FA85220 + 32 * ((v7 >> 8) + 32 * (v6 >> 8));
//        v43 = v8;
//        v11 = v8;
//        if (nY >= dword_6FA8D238 && nY < dword_6FA8D248)
//        {
//            if (v37 >= dword_6FA846A0 && v37 < dword_6FA8D234)
//            {
//                nYa = 32;
//                while (1)
//                {
//                    v12 = *(_uint8_t*)a1;
//                    v13 = a1 + 1;
//                    v14 = v12;
//                    v15 = *(_uint8_t*)v13;
//                    a1 = v13 + 1;
//                    v38 = v15;
//                    v16 = v14;
//                    if (v14)
//                    {
//                        goto LABEL_23;
//                    }
//                    if (v38)
//                    {
//                        break;
//                    }
//                    --nYa;
//                    v6 += v39;
//                    v7 += v40;
//                    v8 += dword_6FA81450;
//                    v11 = v8;
//                    pIntensitiesa = v6;
//                    nAlphaa = v7;
//                    v10 = (char*)&unk_6FA85220 + 32 * ((v7 >> 8) + 32 * (v6 >> 8));
//LABEL_26:
//                    if (!nYa)
//                    {
//                        return;
//                    }
//                }
//                v16 = 0;
//LABEL_23:
//                v11 += v16;
//                v10 += v16;
//                v41 = a1 + v38;
//                if (a1 < (unsigned int32_t)(a1 + v38))
//                {
//                    do
//                    {
//                        v17 = *(_uint8_t*)a1++;
//                        v18 = *(&byte_6FA81580[256 * (unsigned __int32_t8)*v10] + v17);
//                        v19 = *(_uint8_t*)v11;
//                        ++v10;
//                        ++v11;
//                        *(_uint8_t*)(v11 - 1) = *(&v35[64 * v18].peRed + v19);
//                    }
//                    while (a1 < (unsigned int32_t)v41);
//                    v7 = nAlphaa;
//                    v6 = pIntensitiesa;
//                }
//                goto LABEL_26;
//            }
//            v9 = nY;
//        }
//        v36 = 0;
//        nYb = v9;
//        do
//        {
//            v20 = *(_uint8_t*)(a1 + 1);
//            v42 = *(_uint8_t*)a1;
//            a1 += 2;
//            if (v42 || v20)
//            {
//                if (nYb >= dword_6FA8D240)
//                {
//                    return;
//                }
//                v21 = v42 + v11;
//                v22 = (int32_t)&v10[v42];
//                if (nYb < dword_6FA84690)
//                {
//                    v11 = v20 + v21;
//                    v10 = (char*)(v20 + v22);
//                    a1 += v20;
//                }
//                else
//                {
//                    v44 = v20 + a1;
//                    v23 = v37 + v21 - v8;
//                    v45 = v20 + v21;
//                    v24 = dword_6FA8D220 - v23;
//                    v46 = v20 + v22;
//                    if (dword_6FA8D220 - v23 <= 0)
//                    {
//                        v24 = 0;
//                    }
//                    v25 = dword_6FA8469C - v23;
//                    v26 = v25 < 0;
//                    LOBYTE(v26) = v25 <= 0;
//                    v27 = v25 & (v26 - 1);
//                    if (v27 < v20)
//                    {
//                        v20 = v27;
//                    }
//                    v28 = v20 - v24;
//                    if (v28 > 0)
//                    {
//                        v29 = v24 + a1;
//                        v30 = v24 + v21;
//                        for (i = v28 + v29; v29 < (unsigned int32_t)i; *(_uint8_t*)(v30 - 1) = *(&v35[64 * v33].peRed + v34))
//                        {
//                            v32 = *(_uint8_t*)v29++;
//                            v33 = *(&byte_6FA81580[256 * *(_uint8_t*)v22] + v32);
//                            v34 = *(_uint8_t*)v30;
//                            ++v22;
//                            ++v30;
//                        }
//                    }
//                    a1 = v44;
//                    v11 = v45;
//                    v10 = (char*)v46;
//                    v6 = pIntensitiesa;
//                    v8 = v43;
//                }
//            }
//            else
//            {
//                v6 += v39;
//                ++v36;
//                nAlphaa += v40;
//                v8 += dword_6FA81450;
//                ++nYb;
//                v43 = v8;
//                v11 = v8;
//                pIntensitiesa = v6;
//                v10 = (char*)&unk_6FA85220 + 32 * ((nAlphaa >> 8) + 32 * (v6 >> 8));
//            }
//        }
//        while (v36 < 32);
//    }
}

//D2Gfx.0x6FA73130
void __fastcall sub_6FA73130(D2TileLibraryBlockStrc* a1, int32_t a2, int32_t a3, uint8_t* a4, uint8_t a5)
{
    D2PaletteStrc* pPalette = &stru_6FA81580[(uint32_t)a5 >> 3];

    if (a1->nFormat & 4)
    {
        sub_6FA71D90(a2, a3, a1->dwOffset_pData, a4, pPalette->unk0x00, 15);
    }
    else
    {
        const int32_t nEnd = std::min(dword_6FA8D240 - a3, 15);
        for (int32_t nStart = std::max(dword_6FA84690 - a3, 0); nStart < nEnd; ++nStart)
        {
            const int32_t nOffset = a2 + dword_6FA7E064[nStart];
            const int32_t nIndex = std::max(dword_6FA8D220 - nOffset, 0);
            const int32_t nSize = D2Clamp(dword_6FA8469C - nOffset, 0, dword_6FA7E0A0[nStart]) - nIndex;
            if (nSize > 0)
            {
                uint8_t* pCurrent = &a4[nStart * dword_6FA81450 + dword_6FA7E064[nStart] + nIndex];
                uint8_t* pStart = &a1->dwOffset_pData[dword_6FA7E0DC[nStart] + nIndex];
                uint8_t* pEnd = &pStart[nSize];
                while (pStart < pEnd)
                {
                    *pCurrent = pPalette->unk0x00[*pStart];
                    ++pCurrent;
                    ++pStart;
                }
            }
        }
    }
}

//D2Gfx.0x6FA73270
void __fastcall sub_6FA73270(D2TileLibraryBlockStrc* a1, int32_t a2, int32_t a3, uint8_t* a4, int32_t* a5)
{
    // TODO: Names
    const int32_t v7 = a5[3] - a5[0];
    const int32_t v9 = a5[2] - a5[1];
    int32_t v11 = 16 * a5[0];
    int32_t v12 = 16 * a5[1];

    if (a1->nFormat & 4)
    {
        sub_6FA72090(a2, a3, a1->dwOffset_pData, a4, v11, v7, v12, v9, 15, 7);
    }
    else
    {
        const int32_t nEnd = std::min(dword_6FA8D240 - a3, 15);
        for (int32_t nStart = std::max(dword_6FA84690 - a3, 0); nStart < nEnd; ++nStart)
        {
            const int32_t nOffset = a2 + dword_6FA7E064[nStart];
            const int32_t nIndex = std::max(dword_6FA8D220 - nOffset, 0);
            const int32_t nSize = D2Clamp(dword_6FA8469C - nOffset, 0, dword_6FA7E0A0[nStart]) - nIndex;
            if (nSize > 0)
            {
                uint8_t* pIndex = (uint8_t*)((char*)byte_6FA85220 + 32 * ((v12 >> 7) + 32 * (v11 >> 7)) + dword_6FA7E064[nStart]);
                uint8_t* pCurrent = &a4[nStart * dword_6FA81450 + dword_6FA7E064[nStart] + nIndex];
                uint8_t* pStart = &a1->dwOffset_pData[dword_6FA7E0DC[nStart] + nIndex];
                uint8_t* pEnd = &pStart[nSize];
                while (pStart < pEnd)
                {
                    *pCurrent = stru_6FA81580[*pIndex].unk0x00[*pStart];
                    ++pCurrent;
                    ++pStart;
                    ++pIndex;
                }
            }

            v11 += v7;
            v12 += v9;
        }
    }
}

//D2Gfx.0x6FA73410) --------------------------------------------------------
void __fastcall D2GFX_FloorTileDraw_6FA73410(D2TileLibraryEntryStrc* pTileLibraryEntry, int32_t nX, int32_t nY, D2GfxLightExStrc* pLightEx)
{
//    int32_t v4; // ebx@1
//    int32_t v5; // edi@1
//    int32_t v6; // ebp@2
//    int32_t v7; // ecx@4
//    int32_t v8; // edx@4
//    int32_t v9; // esi@4
//    int32_t v10; // edx@5
//    int32_t v11; // eax@5
//    int32_t v12; // ebx@9
//    signed int32_t v13; // edi@10
//    D2GfxLightExStrc* v14; // eax@11
//    int32_t v15; // edi@11
//    int32_t v16; // edx@11
//    int32_t v17; // ebx@11
//    int32_t v18; // ebx@13
//    int32_t v19; // ebx@15
//    int32_t v20; // ST58_4@19
//    int32_t v21; // ecx@19
//    char v22; // dl@28
//    int32_t v23; // ecx@28
//    int32_t v24; // eax@28
//    int32_t v25; // zf@31
//    int32_t v26; // ecx@31
//    int32_t v27; // edx@37
//    int32_t v28; // sf@37
//    unsigned __int32_t8 v29; // of@37
//    unsigned __int32_t8 a5; // [sp+Ch] [bp-54h]@9
//    int32_t v31; // [sp+10h] [bp-50h]@11
//    int32_t v32; // [sp+18h] [bp-48h]@2
//    int32_t v33; // [sp+1Ch] [bp-44h]@15
//    int32_t v34; // [sp+20h] [bp-40h]@13
//    int32_t v35; // [sp+24h] [bp-3Ch]@11
//    int32_t v36; // [sp+28h] [bp-38h]@5
//    int32_t a3; // [sp+2Ch] [bp-34h]@5
//    int32_t v38; // [sp+30h] [bp-30h]@1
//    int32_t v39; // [sp+34h] [bp-2Ch]@1
//    int32_t v40; // [sp+38h] [bp-28h]@1
//    int32_t v41; // [sp+44h] [bp-1Ch]@11
//    int32_t v42; // [sp+48h] [bp-18h]@11
//    D2TileLibraryEntryStrc* v43; // [sp+4Ch] [bp-14h]@1
//    int32_t v44; // [sp+50h] [bp-10h]@19
//    int32_t v45; // [sp+54h] [bp-Ch]@19
//    int32_t v46; // [sp+58h] [bp-8h]@2
//    int32_t v47; // [sp+5Ch] [bp-4h]@19
//
//    v4 = nX;
//    v5 = nX + gCmnInfo_pBuffer_6FA8144C + nY * dword_6FA81450;
//    v39 = nX;
//    v43 = pTileLibraryEntry;
//    v40 = nX + gCmnInfo_pBuffer_6FA8144C + nY * dword_6FA81450;
//    v38 = 0;
//    if (pTileLibraryEntry->field_50 > 0)
//    {
//        v6 = v46;
//        v32 = 0;
//        while (1)
//        {
//            v7 = v32 + pTileLibraryEntry->field_54;
//            v8 = *(_WORD*)v7;
//            v9 = v5 + v8 + dword_6FA81450 * *(_WORD*)(v7 + 2);
//            if (!(*(_uint8_t*)(v7 + 8) & 1)
//                || (v10 = v4 + v8,
//                    v11 = nY + *(_WORD*)(v7 + 2),
//                    a3 = v10,
//                    v36 = nY + *(_WORD*)(v7 + 2),
//                    v11 < dword_6FA8D224)
//                || v11 >= dword_6FA8D23C
//                || v10 < dword_6FA8D244
//                || v10 >= dword_6FA84698)
//                goto LABEL_37;
//            v12 = 0;
//            a5 = 0;
//            if (!dword_6FA8D250)
//                break;
//            LOBYTE(v12) = *(_uint8_t*)(v7 + 7);
//            v13 = 1;
//            a5 = pLightEx[*(_uint8_t*)(v7 + 6) + 8 * v12 + 9].nIntensity;
//LABEL_23:
//            if (v11 < dword_6FA8D228 || v11 >= dword_6FA84694 || v10 < dword_6FA8468C || v10 >= dword_6FA8D22C)
//            {
//                if (v13)
//                    sub_6FA73130(v7, v10, v11, v9, a5);
//                else
//                    sub_6FA73270(v7, v10, v11, v9, (int32_t)&v44);
//            }
//            else
//            {
//                if (v13)
//                {
//                    v22 = *(_uint8_t*)(v7 + 8);
//                    v23 = *(_uint32_t*)(v7 + 16);
//                    v24 = (int32_t)&byte_6FA81580[256 * ((unsigned int32_t)a5 >> 3)];
//                    if (v22 & 4)
//                        sub_6FA71720(v23, v9, v24, 15);
//                    else
//                        sub_6FA71130(v23, v9, v24);
//                }
//                else
//                {
//                    v25 = (*(_uint8_t*)(v7 + 8) & 4) == 0;
//                    v26 = *(_uint32_t*)(v7 + 16);
//                    if (v25)
//                        sub_6FA71340(v26, v9, 16 * v44, v47 - v44, 16 * v45, v6 - v45);
//                    else
//                        sub_6FA71970(v26, v9, 16 * v44, v47 - v44, 16 * v45, v6 - v45, 15, 7);
//                }
//            }
//LABEL_37:
//            pTileLibraryEntry = v43;
//            v27 = v43->field_50;
//            v29 = __OFSUB__(v38 + 1, v27);
//            v28 = v38++ + 1 - v27 < 0;
//            v32 += 20;
//            if (!(v28 ^ v29))
//                return;
//            v4 = v39;
//            v5 = v40;
//        }
//        LOBYTE(v12) = *(&pLightEx[8 * *(_uint8_t*)(v7 + 7) + 10].nIntensity + 12 * *(_uint8_t*)(v7 + 6));
//        v14 = &pLightEx[*(_uint8_t*)(v7 + 6) + 8 * *(_uint8_t*)(v7 + 7)];
//        v15 = v12;
//        v42 = v14[11].nIntensity;
//        v31 = v14[17].nIntensity;
//        v16 = v14[9].nIntensity;
//        v35 = v14[18].nIntensity;
//        v41 = v14[19].nIntensity;
//        v17 = v12 - v16;
//        if (v15 - v16 < 0)
//            v17 = -v17;
//        v34 = v17;
//        v18 = v31 - v16;
//        if (v31 - v16 < 0)
//            v18 = -v18;
//        v33 = v18;
//        v19 = v35 - v15;
//        if (v35 - v15 < 0)
//            v19 = -v19;
//        if (!gGfxSettings.bLowQuality)
//        {
//            if (v34 + v33 + v19 >= 10)
//            {
//                v20 = v7;
//                v21 = v14[1].nIntensity;
//                v44 = (v14[8].nIntensity + v16 + v14[16].nIntensity + v31) >> 2;
//                v45 = (v16 + v14[2].nIntensity + v15 + v21) >> 2;
//                v7 = v20;
//                v6 = (v15 + v42 + v41 + v31) >> 2;
//                v47 = (v31 + v14[26].nIntensity + v35 + v14[25].nIntensity) >> 2;
//                v13 = 0;
//                v46 = v6;
//LABEL_22:
//                v11 = v36;
//                v10 = a3;
//                goto LABEL_23;
//            }
//            v6 = v46;
//        }
//        v13 = 1;
//        a5 = v14[9].nIntensity;
//        goto LABEL_22;
//    }
}
