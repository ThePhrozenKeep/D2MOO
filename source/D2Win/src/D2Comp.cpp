#include "D2WinComp.h"

#include <algorithm>

#include <D2Math.h>

#include <Fog.h>
#include <D2CMP.h>
#include <Archive.h>
#include <D2Lang.h>

#include <D2Gfx.h>
#include <D2CommonDefinitions.h>

#include "D2WinArchive.h"
#include "D2WinPalette.h"

// Hack to get access to txt files structures...
// Need to find a decent way to do this. Should datatables structures and Ids be moved to D2CommonDefinitions?
#undef D2COMMON_DLL_DECL
#define D2COMMON_DLL_DECL
#include <../../D2Common/include/DataTbls/ItemsTbls.h>

//1.10f: D2Win.0x6F8A2CE0
void* __stdcall D2WIN_DATATBLS_CompileTxt(HD2ARCHIVE hArchive, const char* szName, D2BinFieldStrc* pBinField, int* pRecordCount, int nRecordSize);

struct D2HitClassTxt
{
	uint32_t dwCode;						//0x00
};

D2CompositeUnitStrc* dword_6F8FD650;
D2CofNodeStrc* dword_6F8FD654;

//1.10f: D2Win.0x6F8FD248
int32_t gnItemTypesTxtRecordCount;
//1.10f: D2Win.0x6F8FD244
D2ItemTypesTxt* gpItemTypesTxt;

//1.10f: D2Win.0x6F8FD234
D2HitClassTxt* gpHitClassTxt;

uint8_t byte_6F8FD218[10];
uint8_t byte_6F8FD224[10];

char byte_6F8FD678[4];

D2CellFileStrc* ghSquelchCelFile_6F8FD65C;
int gnSquelchCelFileFrameCount_6F8FD660;
D2CellFileStrc* ghSelectedCelFile_6F8FD668;
int gnSelectedCelFileFrameCount_6F8FD66C;
//1.10f: D2Win.0x6F8BCF94
BOOL gDATATBLS_LoadFromBin = TRUE;
int dword_6F8BC0A4;
int gnPlayerModeTokenTblSize_6F8BC0F8;
int dword_6F8BC154[15] =
{
	0,
	' hth',
	' th1',
	' th2',
	' sh1',
	' sh2',
	' wob',
	' wbx',
	' fts',
	' sj1',
	' tj1',
	' ss1',
	' ts1',
	' 1th',
	' 2th',
};
int dword_6F8BC190 = 15;



//D2Win.0x6F8A1000) --------------------------------------------------------
//void __fastcall sub_6F8A1000(D2CompositeUnitStrc* pCompositeUnit, D2GfxDataStrc* pGfxData)
//{
//	D2GfxInfoStrc* pGfxInfo; // ecx@3
//	D2CofDataStrc* pCofData; // eax@6
//	int v12; // ecx@9
//	unsigned int v18; // eax@9
//	char v16; // al@24
//	char v17; // cl@26
//	int v19; // [sp+14h] [bp-8h]@6
//	int v20; // [sp+18h] [bp-4h]@6
//
//	pGfxData->nUnitIndex = pCompositeUnit->nUnitIndex;
//	pGfxData->nMode = pCompositeUnit->nMode;
//	
//	const int nComponent = pGfxData->nComponent;
//	pGfxData->dwComponentToken = dword_6F8BC100[nComponent];
//	pGfxData->dwWClassToken = dword_6F8BC154[pCompositeUnit->nWClassTokenIndex];
//	pGfxData->dwUnitToken = dword_6F8BC040[pCompositeUnit->nUnitIndex];
//	if (pCompositeUnit->dwArmorType[nComponent])
//	{
//		pGfxData->dwArmorType = pCompositeUnit->dwArmorType[nComponent];
//	}
//	else
//	{
//		pGfxData->dwArmorType = ' til';
//	}
//
//	pGfxInfo = pCompositeUnit->pGfxInfo;
//
//	D2_ASSERT(pGfxInfo);
//
//	v19 = 0;
//	v20 = 0;
//	pCofData = sub_6F8A1250(pGfxInfo, pCompositeUnit->nUnitIndex, pCompositeUnit->nMode, &v20, &v19, dword_6F8BC154[pCompositeUnit->nWClassTokenIndex]);
//
//	D2_ASSERT(pCofData);
//
//	v12 = (int)&pCofData[1];
//	for (int i = 0; i < pCofData->unk0x00; ++i)
//	{
//		D2_ASSERT(nComponent < 16);
//
//		if (*(_BYTE*)v12 == nComponent)
//		{
//			pGfxData->nUnitType = pGfxData->nUnitIndex >= 7;
//			pGfxData->dwMode = dword_6F8BC0A8[pGfxData->nMode];
//
//			if (!pGfxData->dwArmorType)
//			{
//				Fog_10024("[sGetCelContextInfo] This should not happen.\n", __FILE__, __LINE__);
//				exit(-1);
//			}
//
//			if (*(_BYTE*)(v12 + 5))
//			{
//				LOBYTE(v18) = *(_BYTE*)(v12 + 5);
//				v16 = *(_BYTE*)(v12 + 6);
//				if (v16)
//				{
//					v17 = *(_BYTE*)(v12 + 7);
//					BYTE1(v18) = v16;
//					if (v17)
//					{
//						BYTE2(v18) = v17;
//						BYTE3(v18) = ' ';
//					}
//					else
//					{
//						HIWORD(v18) = '  ';
//					}
//				}
//				else
//				{
//					*(_WORD*)((char*)&v18 + 1) = '  ';
//					BYTE3(v18) = ' ';
//				}
//			}
//			else
//			{
//				v18 = '    ';
//			}
//			pGfxData->dwWClassToken = v18;
//			break;
//		}
//
//		v12 += 9;
//	}
//
//	D2_ASSERT(pGfxData->dwComponentToken);
//
//	D2_ASSERT(pGfxData->dwUnitToken);
//
//	D2_ASSERT(pGfxData->dwWClassToken);
//
//	if (!pGfxData->dwArmorType)
//	{
//		pGfxData->dwArmorType = ' xxx';
//	}
//
//	if (!pGfxData->dwMode)
//	{
//		pGfxData->dwMode = ' xxx';
//	}
//}

//D2Win.0x6F8A1250) --------------------------------------------------------
D2CofDataStrc* __fastcall sub_6F8A1250(D2GfxInfoStrc* a1, unsigned int nClass, unsigned int nMode, int* a4, int* a5, int a6)
{
//	unsigned int v6; // ebp@1
//	int v7; // eax@4
//	unsigned int nMode2; // esi@5
//	int v9; // eax@16
//	int v10; // ecx@16
//	int v11; // eax@16
//	int result; // eax@20
//	void *v13; // eax@22
//	int v14; // ecx@22
//	int v15; // eax@25
//	void *v16; // esi@30
//	int v17; // edx@30
//	int v18; // eax@32
//	int v19; // eax@32
//	int v20; // eax@43
//	int v21; // [sp+28h] [bp-124h]@16
//	char v22; // [sp+2Ch] [bp-120h]@16
//	char v23; // [sp+2Dh] [bp-11Fh]@16
//	__int16 v24; // [sp+2Eh] [bp-11Eh]@16
//	int v25; // [sp+30h] [bp-11Ch]@16
//	int v26; // [sp+34h] [bp-118h]@16
//	int v27; // [sp+3Ch] [bp-110h]@16
//	int v28; // [sp+44h] [bp-108h]@1
//	CHAR v29; // [sp+48h] [bp-104h]@10
//	char v30; // [sp+49h] [bp-103h]@10
//	__int16 v31; // [sp+149h] [bp-3h]@10
//	char v32; // [sp+14Bh] [bp-1h]@10
//
//	v6 = nClass;
//	v28 = a1;
//	if (!a1)
//	{
//		FOG_DisplayAssert("ptGfxInfo", __FILE__, __LINE__);
//		exit(-1);
//	}
//	v7 = *(DWORD*)a1;
//	if (*(DWORD*)a1)
//	{
//		while (1)
//		{
//			nMode2 = nMode;
//			if (*(DWORD*)(*(DWORD*)v7 + 4) == nMode)
//			{
//				if (*(DWORD*)(*(DWORD*)v7 + 12) == a6)
//					break;
//			}
//			v7 = *(DWORD*)(v7 + 4);
//			if (!v7)
//				goto LABEL_10;
//		}
//	}
//	else
//	{
//		nMode2 = nMode;
//LABEL_10:
//		v29 = 0;
//		memset(&v30, 0, 0x100u);
//		v31 = 0;
//		v32 = 0;
//		if (nClass >= 0x19)
//		{
//			FOG_DisplayAssert(//				"nClass < COMPOSITEUNITCLASS_NUMCLASSES", //				__FILE__, __LINE__//				1149);
//			exit(-1);
//		}
//		if (nMode2 >= dword_6F8BC0F8)
//		{
//			FOG_DisplayAssert(//				"nMode < sgnPlayerModeTokenTblSize", //				__FILE__, __LINE__//				1150);
//			exit(-1);
//		}
//		v9 = dword_6F8BC040[nClass];
//		LOBYTE(v25) = a6 & (((BYTE)a6 == 32) - 1);
//		v26 = v9;
//		v10 = dword_6F8BC0A8[nMode2];
//		BYTE1(v25) = BYTE1(a6) & ((BYTE1(a6) == 32) - 1);
//		v27 = v10;
//		HIWORD(v25) = (unsigned __int8)(BYTE2(a6) & ((BYTE2(a6) == 32) - 1));
//		v22 = v9 & (((BYTE)v9 == 32) - 1);
//		v23 = BYTE1(v26) & ((BYTE1(v9) == 32) - 1);
//		v24 = (unsigned __int8)(BYTE2(v9) & ((BYTE2(v9) == 32) - 1));
//		LOBYTE(v21) = v10 & (((BYTE)v10 == 32) - 1);
//		BYTE1(v21) = BYTE1(v27) & ((BYTE1(v10) == 32) - 1);
//		HIWORD(v21) = (unsigned __int8)(BYTE2(v10) & ((BYTE2(v10) == 32) - 1));
//		v11 = (int)"DATA\\GLOBAL\\CHARS";
//		if (nClass >= 7)
//			v11 = (int)"DATA\\GLOBAL\\MONSTERS";
//		wsprintfA(&v29, "%s\\%s\\COF\\%s%s%s.COF", v11, &v22, &v22, &v21, &v25);
//		if (!Fog_10102(&v29, &v26) || (Fog_10103(v26), !v29))
//			return 0;
//		v13 = FOG_Alloc(8, __FILE__, __LINE__, 0);
//		v14 = (int)v13;
//		v21 = (int)v13;
//		if (!v13)
//		{
//			FOG_DisplayAssert(//				"ptUnitCofNode", //				__FILE__, __LINE__//				1230);
//			exit(-1);
//		}
//		*(DWORD*)v13 = 0;
//		*((DWORD*)v13 + 1) = 0;
//		v15 = dword_6F8FD654;
//		if (dword_6F8FD654)
//		{
//			while (*(DWORD*)(v15 + 4) != nMode2 || *(DWORD*)(v15 + 8) != v6 || *(DWORD*)(v15 + 12) != a6)
//			{
//				v15 = *(DWORD*)(v15 + 24);
//				if (!v15)
//					goto LABEL_30;
//			}
//			++*(DWORD*)v15;
//		}
//		else
//		{
//LABEL_30:
//			v16 = FOG_Alloc(//				32, //				__FILE__, __LINE__//				1068, //				0);
//			memset(v16, 0, 0x20u);
//			v17 = *(DWORD*)v16 + 1;
//			*((DWORD*)v16 + 1) = nMode;
//			*((DWORD*)v16 + 2) = v6;
//			*(DWORD*)v16 = v17;
//			*((DWORD*)v16 + 3) = a6;
//			*((DWORD*)v16 + 6) = dword_6F8FD654;
//			*((DWORD*)v16 + 7) = 0;
//			if (dword_6F8FD654)
//				*(DWORD*)(dword_6F8FD654 + 28) = v16;
//			dword_6F8FD654 = (int)v16;
//			v18 = D2Win_GetArchive();
//			v19 = D2Hell_ARCHIVE_OpenFile_6F8B22F8(//				v18, //				&v29, //				(int)((char*)v16 + 16), //				__FILE__, __LINE__//				992);
//			if (!v19)
//			{
//				FOG_DisplayAssert(//					"lpbCofData", //					__FILE__, __LINE__//					993);
//				exit(-1);
//			}
//			if (*(BYTE*)(v19 + 3) != 20)
//			{
//				FOG_DisplayAssert(//					"((PTCOFHEADER)lpbCofData)->bVersion == COF_VERSION", //					__FILE__, __LINE__//					1002);
//				exit(-1);
//			}
//			v14 = v21;
//			*((DWORD*)v16 + 5) = v19;
//			v15 = (int)v16;
//		}
//		if (!v15)
//		{
//			FOG_DisplayAssert("ptCofNode", __FILE__, __LINE__);
//			exit(-1);
//		}
//		*(DWORD*)v14 = v15;
//		v20 = v28;
//		*(DWORD*)(v14 + 4) = *(DWORD*)v28;
//		*(DWORD*)v20 = v14;
//		v7 = v21;
//	}
//	result = *(DWORD*)(*(DWORD*)v7 + 20);
//	if (!result)
//	{
//		FOG_DisplayAssert(//			"ptUnitCofNode->ptCofNode->lpbCofFile", //			__FILE__, __LINE__//			1239);
//		exit(-1);
//	}
//	return result;
return 0;
}

////D2Win.0x6F8A15E0) --------------------------------------------------------
//int __thiscall sub_6F8A15E0(int this)
//{
//	int v1; // esi@1
//	int result; // eax@4
//	int *v3; // eax@6
//	int v4; // edi@14
//	int v5; // ecx@17
//	signed int v6; // edi@17
//	DWORD v7; // ebp@17
//	signed int nComponent; // edx@23
//	int v9; // eax@26
//	unsigned int v10; // ebx@31
//	signed int v11; // edx@32
//	int v12; // [sp+2Ch] [bp-68h]@14
//	signed int v13; // [sp+30h] [bp-64h]@20
//	int v14; // [sp+34h] [bp-60h]@20
//	int v15; // [sp+38h] [bp-5Ch]@20
//	int v16; // [sp+3Ch] [bp-58h]@17
//	unsigned __int8 v17; // [sp+40h] [bp-54h]@20
//	int nYpos; // [sp+44h] [bp-50h]@6
//	int nXpos; // [sp+48h] [bp-4Ch]@6
//	D2GfxDataStrc a2; // [sp+4Ch] [bp-48h]@24
//
//	v1 = this;
//	if (!this)
//	{
//		FOG_DisplayAssert("hCompositeUnit", __FILE__, __LINE__);
//		exit(-1);
//	}
//	result = *(DWORD*)this;
//	if (*(DWORD*)this & gdwBitMasks[0] && result & gdwBitMasks[2])
//	{
//		nXpos = *(DWORD*)(this + 20) - 2;
//		nYpos = *(DWORD*)(this + 24);
//		v3 = *(int **)(this + 184);
//		if (!v3)
//		{
//			FOG_DisplayAssert("ptGfxInfo", __FILE__, __LINE__);
//			exit(-1);
//		}
//		result = *v3;
//		if (result)
//		{
//			while (*(DWORD*)(*(DWORD*)result + 4) != *(DWORD*)(this + 32)
//				   || *(DWORD*)(*(DWORD*)result + 12) != dword_6F8BC154[*(DWORD*)(this + 36)])
//			{
//				result = *(DWORD*)(result + 4);
//				if (!result)
//					return result;
//			}
//			v4 = *(DWORD*)(*(DWORD*)result + 20);
//			v12 = *(DWORD*)(*(DWORD*)result + 20);
//			if (!v4)
//			{
//				FOG_DisplayAssert(//					"lpbCofData", //					__FILE__, __LINE__//					1472);
//				exit(-1);
//			}
//			v5 = *(BYTE*)(v4 + 1);
//			v6 = *(BYTE*)v12;
//			v7 = *(DWORD*)(v1 + 8) >> 8;
//			result = v5 + v6 * (*(BYTE*)(v1 + 4) * v5 + v7 + 9) + v12 + 28;
//			v16 = v5 + v6 * (*(BYTE*)(v1 + 4) * v5 + v7 + 9) + v12 + 28;
//			if (v5 + v6 * (*(BYTE*)(v1 + 4) * v5 + v7 + 9) + v12 == -28)
//			{
//				FOG_DisplayAssert(//					"lpbFirstByte", //					__FILE__, __LINE__//					1021);
//				exit(-1);
//			}
//			v15 = 0;
//			v17 = *(BYTE*)(v1 + 4);
//			v14 = 0;
//			v13 = 0;
//			if (v6 > 0)
//			{
//				while (1)
//				{
//					nComponent = *(BYTE*)(v13 + result);
//					if (nComponent >= 16)
//					{
//						FOG_DisplayAssert(//							"eComponent < NUM_COMPONENTS", //							__FILE__, __LINE__//							1411);
//						exit(-1);
//					}
//					memset(&a2, 0, sizeof(a2));
//					a2.pCellFile = 0;
//					a2.nComponent = nComponent;
//					a2.nDirection = v17;
//					a2.nFrame = v7;
//					sub_6F8A1000(v1, &a2);
//					a2.fFlags |= 2u;
//					if (D2CMP_10055_CacheCurrentCell(&a2, 0, 1) && (DWORD)a2.pCurrentCell)
//					{
//						v9 = *(DWORD*)(v1 + 20) + D2CMP_CelGetOffsetY(a2.pCurrentCell);
//						if (v9 > v15)
//							v15 = v9;
//						++v14;
//					}
//					++v13;
//					if (v13 >= *(BYTE*)v12)
//						break;
//					result = v16;
//				}
//				result = v14;
//				if (v14 > 0)
//				{
//					v10 = 0;
//					if (*(BYTE*)v12)
//					{
//						do
//						{
//							v11 = *(BYTE*)(v16 + v10);
//							if (v11 >= 16)
//							{
//								FOG_DisplayAssert(//									"eComponent < NUM_COMPONENTS", //									__FILE__, __LINE__//									1487);
//								exit(-1);
//							}
//							memset(&a2, 0, sizeof(a2));
//							a2.nComponent = v11;
//							a2.nDirection = *(BYTE*)(v1 + 4);
//							a2.pCellFile = 0;
//							a2.nFrame = v7;
//							sub_6F8A1000(v1, &a2);
//							a2.fFlags |= 2u;
//							D2GFX_10075_DrawShadow(&a2, nXpos, nYpos);
//							++v10;
//							result = v12;
//						}
//						while (v10 < *(BYTE*)v12);
//					}
//				}
//			}
//		}
//	}
//	return result;
//}
//// 6F8BC154: using guessed type int dword_6F8BC154[];

////D2Win.0x6F8A1890) --------------------------------------------------------
//signed int __fastcall sub_6F8A1890(int a1, DWORD a2)
//{
//	int v2; // esi@1
//	int v3; // ebp@4
//	int v5; // ecx@10
//	int v6; // edx@10
//	D2CellFileStrc *v7; // edx@11
//	char v8; // al@12
//	int v9; // eax@12
//	int v10; // ST3C_4@12
//	int v11; // eax@13
//	int v12; // eax@16
//	int v13; // edx@21
//	int v14; // eax@24
//	int v15; // edx@24
//	DWORD v16; // ebx@24
//	int v17; // ecx@24
//	int v18; // eax@27
//	signed int v19; // ebp@31
//	signed int v20; // ecx@32
//	DWORD v21; // eax@34
//	int v22; // ebx@44
//	BYTE *v23; // eax@46
//	signed int v24; // ecx@46
//	signed int v25; // eax@47
//	int v26; // ecx@47
//	int v27; // eax@63
//	int v28; // edx@63
//	int nDrawMode; // [sp+5Ch] [bp-164h]@29
//	int v30; // [sp+60h] [bp-160h]@21
//	unsigned __int8 v31; // [sp+64h] [bp-15Ch]@45
//	DWORD v32; // [sp+68h] [bp-158h]@24
//	DWORD dwGamma; // [sp+6Ch] [bp-154h]@1
//	int v34; // [sp+70h] [bp-150h]@24
//	D2GfxDataStrc v35; // [sp+78h] [bp-148h]@10
//	BYTE pPalette; // [sp+C0h] [bp-100h]@12
//	char v37; // [sp+C1h] [bp-FFh]@12
//	char v38; // [sp+C2h] [bp-FEh]@12
//
//	v2 = a1;
//	dwGamma = a2;
//	if (!a1)
//	{
//		FOG_DisplayAssert("hCompositeUnit", __FILE__, __LINE__);
//		exit(-1);
//	}
//	v3 = gdwBitMasks[0];
//	if (!(*(DWORD*)a1 & gdwBitMasks[0]))
//		return 0;
//	if (*(DWORD*)a1 & gdwBitMasks[10])
//	{
//		if (!ghSelectedCelFile_6F8FD668)
//		{
//			FOG_DisplayAssert(//				"sghSelectedCelFile", //				__FILE__, __LINE__//				1523);
//			exit(-1);
//		}
//		memset(&v35, 0, sizeof(v35));
//		v5 = *(DWORD*)(a1 + 24);
//		v35.pCellFile = ghSelectedCelFile_6F8FD668;
//		v6 = *(DWORD*)(v2 + 20);
//		v35.nFrame = dword_6F8FD670;
//		v35.nDirection = 0;
//		TEXTURE_CelDraw(&v35, v6 + 5, v5 - 5, 0xFFFFFFFFu, 3, 0);
//		v3 = gdwBitMasks[0];
//	}
//	v7 = *(D2CellFileStrc **)(v2 + 188);
//	if (v7)
//	{
//		v8 = *(BYTE*)(v2 + 193);
//		v37 = *(BYTE*)(v2 + 192);
//		v38 = v8;
//		memset(&v35, 0, sizeof(v35));
//		v9 = *(DWORD*)(v2 + 20);
//		v35.pCellFile = v7;
//		v10 = *(DWORD*)(v2 + 24) - 60;
//		v35.nDirection = 0;
//		v35.nFrame = 0;
//		v35.nMaxDirections = 1;
//		TEXTURE_CelDraw(&v35, v9 - 20, v10, 0xFFFFFFFFu, 5, &pPalette);
//		v3 = gdwBitMasks[0];
//	}
//	v11 = *(DWORD*)(v2 + 184);
//	if (!v11)
//	{
//		FOG_DisplayAssert("ptGfxInfo", __FILE__, __LINE__);
//		exit(-1);
//	}
//	v12 = *(DWORD*)v11;
//	if (!v12)
//		return 0;
//	while (*(DWORD*)(*(DWORD*)v12 + 4) != *(DWORD*)(v2 + 32)
//		   || *(DWORD*)(*(DWORD*)v12 + 12) != dword_6F8BC154[*(DWORD*)(v2 + 36)])
//	{
//		v12 = *(DWORD*)(v12 + 4);
//		if (!v12)
//			return 0;
//	}
//	v13 = *(DWORD*)(*(DWORD*)v12 + 20);
//	v30 = *(DWORD*)(*(DWORD*)v12 + 20);
//	if (!v13)
//	{
//		FOG_DisplayAssert("lpbCofData", __FILE__, __LINE__);
//		exit(-1);
//	}
//	v14 = *(BYTE*)(v13 + 1);
//	v15 = *(BYTE*)v13;
//	v32 = *(DWORD*)(v2 + 8) >> 8;
//	v16 = 0;
//	v17 = v14 + v15 * (*(BYTE*)(v2 + 4) * v14 + v32 + 9) + v30 + 28;
//	v34 = v14 + v15 * (*(BYTE*)(v2 + 4) * v14 + v32 + 9) + v30 + 28;
//	if (v14 + v15 * (*(BYTE*)(v2 + 4) * v14 + v32 + 9) + v30 == -28)
//	{
//		FOG_DisplayAssert("lpbFirstByte", __FILE__, __LINE__);
//		exit(-1);
//	}
//	v18 = *(DWORD*)v2;
//	if (*(DWORD*)(v3 + 20) & *(DWORD*)v2 && v18 & *(DWORD*)(v3 + 16)
//		|| (nDrawMode = 5, *(DWORD*)(v3 + 52) & v18))
//		nDrawMode = 1;
//	v19 = 0;
//	if (v15 > 0)
//	{
//		while (1)
//		{
//			v22 = *(BYTE*)(v17 + v19);
//			memset(&v35, 0, sizeof(v35));
//			if (*(BYTE*)(v2 + 4) >= 0x40u)
//			{
//				FOG_DisplayAssert(//					"bDirection < CELFILE_NUMGAMEDIRECTIONS", //					__FILE__, __LINE__//					1387);
//				exit(-1);
//			}
//			v31 = *(&byte_6F8BA370[64 * sub_6F8B256F(*(BYTE*)(v2 + 5))] + *(BYTE*)(v2 + 4));
//			if (v31 > 0x40u)
//			{
//				FOG_DisplayAssert(//					"bDir <= CELFILE_NUMGAMEDIRECTIONS", //					__FILE__, __LINE__//					1389);
//				exit(-1);
//			}
//			v35.nDirection = v31;
//			v35.pCellFile = 0;
//			v35.nFrame = v32;
//			v35.nComponent = v22;
//			sub_6F8A1000(v2, &v35);
//			v23 = 0;
//			v35.fFlags |= 2u;
//			v24 = *(BYTE*)(v22 + v2 + 40);
//			if (v24 != 255)
//			{
//				v25 = v24 >> 5;
//				v26 = v24 & 0x1F;
//				if (v26 >= 0 && v26 < 21)
//				{
//					if (!v25)
//					{
//						v25 = 8;
//LABEL_55:
//						v23 = (BYTE*)((char*)&unk_6F8C2118 + 256 * (v26 + 84 * v25 + 21 * v25));
//						goto LABEL_57;
//					}
//					if (v25 >= 0 && v25 < 9 && (v25 <= 2 || v25 > 4))
//						goto LABEL_55;
//				}
//				v23 = 0;
//			}
//LABEL_57:
//			TEXTURE_CelDraw(&v35, *(DWORD*)(v2 + 20), *(DWORD*)(v2 + 24), dwGamma, nDrawMode, v23);
//			++v19;
//			if (v19 >= *(BYTE*)v30)
//			{
//				v16 = 0;
//				break;
//			}
//			v17 = v34;
//		}
//	}
//	v20 = *(DWORD*)(v2 + 16) + *(DWORD*)(v2 + 8);
//	*(DWORD*)(v2 + 8) = v20;
//	if (v20 >> 8 >= *(DWORD*)(v2 + 12) - 1)
//		*(DWORD*)(v2 + 8) = v16;
//	v21 = GetTickCount();
//	if (v21 > dword_6F8FD658)
//	{
//		dword_6F8FD658 = v21 + 66;
//		++dword_6F8FD664;
//		if (dword_6F8FD664 >= dword_6F8FD660)
//			dword_6F8FD664 = v16;
//		++dword_6F8FD670;
//		if (dword_6F8FD670 >= dword_6F8FD66C)
//			dword_6F8FD670 = v16;
//	}
//	if (gdwBitMasks[9] & *(DWORD*)v2)
//	{
//		if (ghSquelchCelFile_6F8FD65C == (D2CellFileStrc*)v16)
//		{
//			FOG_DisplayAssert(//				"sghSquelchCelFile", //				__FILE__, __LINE__//				1638);
//			exit(-1);
//		}
//		memset(&v35, 0, sizeof(v35));
//		v27 = *(DWORD*)(v2 + 20);
//		v35.pCellFile = ghSquelchCelFile_6F8FD65C;
//		v28 = *(DWORD*)(v2 + 24) + 10;
//		v35.nFrame = dword_6F8FD664;
//		v35.nDirection = v16;
//		TEXTURE_CelDraw(&v35, v27, v28, 0xFFFFFFFFu, 3, (BYTE*)v16);
//	}
//	return 1;
//}
//// 6F8A1890: using guessed type int __fastcall sub_6F8A1890(DWORD, DWORD);
//// 6F8B256F: using guessed type DWORD __cdecl sub_6F8B256F(DWORD);
//// 6F8BC154: using guessed type int dword_6F8BC154[];
//// 6F8FD658: using guessed type int dword_6F8FD658;
//// 6F8FD660: using guessed type int dword_6F8FD660;
//// 6F8FD664: using guessed type int dword_6F8FD664;
//// 6F8FD66C: using guessed type int dword_6F8FD66C;
//// 6F8FD670: using guessed type int dword_6F8FD670;

////D2Win.0x6F8A1D10) --------------------------------------------------------
//signed int __usercall sub_6F8A1D10<eax>(int a1<ecx>, int a2<ebx>)
//{
//	int v2; // ebp@1
//	signed int result; // eax@5
//	int v4; // ebx@6
//	Unicode *v5; // esi@6
//	signed int v6; // edi@6
//	int v7; // edx@8
//	unsigned int v8; // eax@14
//	int v9; // eax@18
//	int v10; // edx@21
//	void(__fastcall *v11)(DWORD, DWORD); // ebx@22
//	char *v12; // ebx@24
//	char *v13; // esi@24
//	signed int v14; // edi@24
//	char *v15; // esi@27
//	signed int v16; // edi@27
//	int v17; // esi@31
//	signed int v18; // ebp@31
//	unsigned int v19; // kr04_4@31
//	int v20; // edi@32
//	char *v21; // edi@37
//	signed int v22; // ecx@37
//	bool v23; // zf@39
//	int nColor; // [sp+18h] [bp-618h]@8
//	signed int v25; // [sp+1Ch] [bp-614h]@8
//	int nY; // [sp+20h] [bp-610h]@6
//	int v27; // [sp+24h] [bp-60Ch]@1
//	size_t pWidth; // [sp+28h] [bp-608h]@41
//	size_t pHeight; // [sp+2Ch] [bp-604h]@41
//	Unicode wszText; // [sp+30h] [bp-600h]@6
//	char v31; // [sp+34h] [bp-5FCh]@32
//	char v32; // [sp+38h] [bp-5F8h]@26
//	char v33; // [sp+230h] [bp-400h]@24
//	char v34[4]; // [sp+234h] [bp-3FCh]@31
//	char v35; // [sp+238h] [bp-3F8h]@26
//	char v36; // [sp+438h] [bp-1F8h]@27
//
//	v2 = a1;
//	v27 = a1;
//	if (!a1)
//	{
//		FOG_DisplayAssert("hCompositeUnit", __FILE__, __LINE__);
//		exit(-1);
//	}
//	if (gdwBitMasks[1] & *(DWORD*)a1)
//	{
//		v4 = *(DWORD*)(a1 + 24) + 6;
//		nY = *(DWORD*)(a1 + 24) + 6;
//		v5 = &wszText;
//		v6 = 256;
//		do
//		{
//			D2LANG__default_constructor_closure_(v5);
//			++v5;
//			--v6;
//		}
//		while (v6);
//		v7 = *(DWORD*)(v2 + 28);
//		nColor = 0;
//		v25 = 0;
//		if (v7 == 20)
//		{
//			v25 = 1;
//			nColor = 3;
//		}
//		else
//		{
//			if (v7 == 18)
//			{
//				nColor = 3;
//				v25 = 1;
//			}
//			else
//			{
//				if (gdwBitMasks[5] & *(DWORD*)v2)
//					nColor = 1;
//			}
//		}
//		v8 = *(DWORD*)v2;
//		if (gdwBitMasks[8] & *(DWORD*)v2)
//			nY = v4 + 15;
//		if (v8 & gdwBitMasks[6] || v8 & gdwBitMasks[7])
//		{
//			v12 = D2Win_10160_GetPlayerTitle(//				v7, //				((v8 >> 6) & 1) + 3 * ((v8 >> 5) & 1) + 2 * ((v8 >> 7) & 1), //				(v8 >> 12) & 1);
//			v13 = &v33;
//			v14 = 256;
//			do
//			{
//				D2LANG__default_constructor_closure_(v13);
//				v13 += 2;
//				--v14;
//			}
//			while (v14);
//			Unicode::win2Unicode(&v33, v12, 255);
//			memset(&v32, 0, 0x200u);
//			Unicode::strcpy(&v32, &v35);
//			v11 = Unicode::strcat;
//			Unicode::strcat(&v32, v2 + 332);
//		}
//		else
//		{
//			v9 = *(DWORD*)(v2 + 560);
//			if (v9 == 'D2DV' || v9 == 'D2ST' || v9 == 'D2XP')
//				v10 = v2 + 332;
//			else
//				v10 = v2 + 204;
//			Unicode::strcpy(&wszText, v10);
//			v11 = Unicode::strcat;
//		}
//		v15 = &v36;
//		v16 = 256;
//		do
//		{
//			D2LANG__default_constructor_closure_(v15);
//			v15 += 2;
//			--v16;
//		}
//		while (v16);
//		if (Unicode::strlen(v2 + 460))
//		{
//			D2LANG_toUnicode(&v36, &unk_6F8BD288, 256);
//			v11(&v32, &v36);
//			v11(&v32, v2 + 460);
//			D2LANG_toUnicode(&v36, &unk_6F8BD284, 256);
//			v11(&v32, &v36);
//		}
//		D2Win_10127_SetFont(12);
//		v17 = 0;
//		D2LANG_unicode2Win(&v35, &v32);
//		v18 = 0;
//		v19 = strlen(v34) + 1;
//		if ((signed int)(v19 - 1) > 0)
//		{
//			while (1)
//			{
//				v20 = v17 + 1;
//				if (D2Win_10122((int)&v31, v17 + 1) >= 120)
//					break;
//				++v17;
//				if (v20 >= (signed int)(v19 - 1))
//					goto LABEL_36;
//			}
//			v18 = 1;
//		}
//LABEL_36:
//		v34[v17] = 0;
//		if (v18)
//		{
//			v21 = v34;
//			v22 = -1;
//			do
//			{
//				if (!v22)
//					break;
//				v23 = *v21++ == 0;
//				--v22;
//			}
//			while (!v23);
//			*(DWORD*)(v21 - 1) = *(DWORD*)Source;
//		}
//		Unicode::win2Unicode(&v31, v34, 299);
//		D2Win_10131_GetTextDimensions(&wszText, &pWidth, &pHeight);
//		if (v25)
//			D2Win_10117_DrawText(&wszText, *(DWORD*)(v27 + 20) - ((signed int)pWidth >> 1) - 1, nY - 1, 6, 0);
//		D2Win_10117_DrawText(&wszText, *(DWORD*)(v27 + 20) - ((signed int)pWidth >> 1), nY, nColor, 0);
//		result = 1;
//	}
//	else
//	{
//		result = 0;
//	}
//	return result;
//}
//// 6F8A1D10: could not find valid save-restore pair for esi
//// 6F8BA05C: using guessed type int __thiscall Unicode___default constructor closure_(DWORD);
//// 6F8BA060: using guessed type int __fastcall Unicode__strcpy(DWORD, DWORD);
//// 6F8BA064: using guessed type int __fastcall Unicode__strcat(DWORD, DWORD);
//// 6F8BA068: using guessed type int __fastcall Unicode__win2Unicode(DWORD, DWORD, DWORD);
//// 6F8BA06C: using guessed type int __thiscall Unicode__strlen(DWORD);
//// 6F8BA074: using guessed type int __fastcall Unicode__unicode2Win(DWORD, DWORD);
//// 6F8A1D10: using guessed type char var_3FC[4];

//D2Win.0x6F8A2040
void __stdcall sub_6F8A2040(const Unicode* a1, int a2, int a3, void(__fastcall* a4)(const Unicode*))
{
	const Unicode* v4 = a1;
	for (int i = 0; i < a3; ++i)
	{
		a4(v4);
		v4 = (const Unicode*)((char*)v4 + a2);
	}
}

////D2Win.0x6F8A2070) --------------------------------------------------------
//BOOL __thiscall sub_6F8A2070(void *this)
//{
//	void *v1; // edi@1
//	char *v2; // esi@4
//	signed int v3; // ebx@4
//	BOOL result; // eax@7
//	signed int v5; // ebx@9
//	int v6; // esi@9
//	signed int v7; // ebp@9
//	signed int v8; // esi@11
//	void(__fastcall *v9)(DWORD, DWORD, DWORD); // ebp@11
//	int v10; // eax@15
//	__int16 *v11; // edi@16
//	int v12; // ST1C_4@23
//	Unicode *v13; // eax@23
//	__int16 *v14; // edi@23
//	__int16 *v15; // edi@27
//	__int16 *v16; // edi@34
//	__int16 *v17; // edi@41
//	int v18; // eax@46
//	__int16 *v19; // edi@48
//	__int16 *v20; // edi@55
//	__int16 *v21; // edi@62
//	unsigned int v22; // eax@68
//	int v23; // edx@77
//	__int16 *v24; // edi@93
//	Unicode *v25; // eax@102
//	__int16 *v26; // edi@102
//	__int16 *v27; // edi@108
//	__int16 *v28; // edi@117
//	int v29; // esi@122
//	int v30; // edx@122
//	int v31; // eax@122
//	int v32; // ebp@122
//	int v33; // edi@126
//	int v34; // ebx@126
//	int i; // edi@129
//	int v36; // eax@130
//	int v37; // [sp+30h] [bp-1064h]@9
//	int v38; // [sp+34h] [bp-1060h]@19
//	void *v39; // [sp+3Ch] [bp-1058h]@1
//	int v40; // [sp+40h] [bp-1054h]@9
//	int v41; // [sp+40h] [bp-1054h]@126
//	signed int v42; // [sp+44h] [bp-1050h]@11
//	int nYEnd; // [sp+48h] [bp-104Ch]@9
//	struct tagRECT rc; // [sp+4Ch] [bp-1048h]@8
//	char v45; // [sp+5Ch] [bp-1038h]@4
//	__int16 wszText[256]; // [sp+25Ch] [bp-E38h]@9
//	size_t pWidth; // [sp+45Ch] [bp-C38h]@12
//	int pHeight[781]; // [sp+460h] [bp-C34h]@12
//
//	v1 = this;
//	v39 = this;
//	if (!this)
//	{
//		FOG_DisplayAssert("hCompositeUnit", __FILE__, __LINE__);
//		exit(-1);
//	}
//	v2 = &v45;
//	v3 = 256;
//	do
//	{
//		D2LANG__default_constructor_closure_(v2);
//		v2 += 2;
//		--v3;
//	}
//	while (v3);
//	if (!(gdwBitMasks[3] & *(DWORD*)v1))
//		return 0;
//	SetRect(&rc, *((DWORD*)v1 + 5) - 30, *((DWORD*)v1 + 6) - 80, *((DWORD*)v1 + 5) + 30, *((DWORD*)v1 + 6) + 20);
//	result = CONTROL_MouseInsideRect(&rc);
//	if (result)
//	{
//		v5 = 0;
//		v40 = *((DWORD*)v1 + 5);
//		nYEnd = *((DWORD*)v1 + 6) - 80;
//		v37 = 0;
//		v6 = (int)wszText;
//		v7 = 7;
//		do
//		{
//			sub_6F8A2040(v6, 2, 256, D2LANG__default_constructor_closure_);
//			v6 += 520;
//			--v7;
//		}
//		while (v7);
//		v8 = 0;
//		v42 = 0;
//		D2Win_10127_SetFont(1);
//		v9 = D2LANG_toUnicode;
//		if (*((DWORD*)v1 + 140) == 'D2XP')
//		{
//			D2LANG_toUnicode(&v45, "Expansion", 256);
//			Unicode::strcpy(wszText, &v45);
//			D2Win_10131_GetTextDimensions((Unicode*)wszText, &pWidth, (size_t*)pHeight);
//			if (((pWidth + 6) & 0x80000000u) == 0)
//				v5 = pWidth + 6;
//			v8 = 1;
//			v37 = pHeight[0];
//		}
//		v10 = *((DWORD*)v1 + 7);
//		if (v10 == 19)
//		{
//			D2LANG_toUnicode(&v45, "Moderator", 256);
//			v11 = &wszText[260 * v8];
//			if (v8 >= 7)
//			{
//				FOG_DisplayAssert(//					"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //					__FILE__, __LINE__//					1837);
//				exit(-1);
//			}
//			Unicode::strcpy(&wszText[260 * v8], &v45);
//			v38 = (int)(v11 + 258);
//			D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v11 + 128, (size_t*)v11 + 129);
//			if (v5 <= *((DWORD*)v11 + 128) + 6)
//				v5 = *((DWORD*)v11 + 128) + 6;
//			v1 = v39;
//			++v8;
//			v37 += *(DWORD*)v38;
//			goto LABEL_22;
//		}
//		if (v10 == 20)
//		{
//			D2LANG_toUnicode(&v45, "Sysop", 256);
//			v15 = &wszText[260 * v8];
//			if (v8 >= 7)
//			{
//				FOG_DisplayAssert(//					"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //					__FILE__, __LINE__//					1837);
//				exit(-1);
//			}
//			Unicode::strcpy(&wszText[260 * v8], &v45);
//			D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v15 + 128, (size_t*)v15 + 129);
//			if (v5 <= *((DWORD*)v15 + 128) + 6)
//				v5 = *((DWORD*)v15 + 128) + 6;
//			v42 = 3;
//			v37 += *((DWORD*)v15 + 129);
//			goto LABEL_116;
//		}
//		if (v10 == 18)
//		{
//			D2LANG_toUnicode(&v45, "Blizzard Rep", 256);
//			v16 = &wszText[260 * v8];
//			if (v8 >= 7)
//			{
//				FOG_DisplayAssert(//					"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //					__FILE__, __LINE__//					1837);
//				exit(-1);
//			}
//			Unicode::strcpy(&wszText[260 * v8], &v45);
//			D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v16 + 128, (size_t*)v16 + 129);
//			if (v5 <= *((DWORD*)v16 + 128) + 6)
//				v5 = *((DWORD*)v16 + 128) + 6;
//			v42 = 3;
//			v37 += *((DWORD*)v16 + 129);
//			goto LABEL_116;
//		}
//		if (v10 == 23)
//		{
//			D2LANG_toUnicode(&v45, "Speaker", 256);
//			v17 = &wszText[260 * v8];
//			if (v8 >= 7)
//			{
//				FOG_DisplayAssert(//					"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //					__FILE__, __LINE__//					1837);
//				exit(-1);
//			}
//			Unicode::strcpy(&wszText[260 * v8], &v45);
//			D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v17 + 128, (size_t*)v17 + 129);
//			if (v5 <= *((DWORD*)v17 + 128) + 6)
//				v5 = *((DWORD*)v17 + 128) + 6;
//			v18 = (int)(v17 + 258);
//			goto LABEL_115;
//		}
//		if (v10 == 21)
//		{
//			D2LANG_toUnicode(&v45, "Referee", 256);
//			v19 = &wszText[260 * v8];
//			if (v8 >= 7)
//			{
//				FOG_DisplayAssert(//					"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //					__FILE__, __LINE__//					1837);
//				exit(-1);
//			}
//			Unicode::strcpy(&wszText[260 * v8], &v45);
//			D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v19 + 128, (size_t*)v19 + 129);
//			if (v5 <= *((DWORD*)v19 + 128) + 6)
//				v5 = *((DWORD*)v19 + 128) + 6;
//			v18 = (int)(v19 + 258);
//LABEL_114:
//			v9 = D2LANG_toUnicode;
//LABEL_115:
//			v37 += *(DWORD*)v18;
//LABEL_116:
//			++v8;
//			goto LABEL_117;
//		}
//		if (v10 == 22)
//		{
//			D2LANG_toUnicode(&v45, "Chat", 256);
//			v20 = &wszText[260 * v8];
//			if (v8 >= 7)
//			{
//				FOG_DisplayAssert(//					"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //					__FILE__, __LINE__//					1837);
//				exit(-1);
//			}
//			Unicode::strcpy(&wszText[260 * v8], &v45);
//			D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v20 + 128, (size_t*)v20 + 129);
//			if (v5 <= *((DWORD*)v20 + 128) + 6)
//				v5 = *((DWORD*)v20 + 128) + 6;
//			v18 = (int)(v20 + 258);
//			goto LABEL_114;
//		}
//		if (v10 == 24)
//		{
//			D2LANG_toUnicode(&v45, "Open Character", 256);
//			v21 = &wszText[260 * v8];
//			if (v8 >= 7)
//			{
//				FOG_DisplayAssert(//					"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //					__FILE__, __LINE__//					1837);
//				exit(-1);
//			}
//			Unicode::strcpy(&wszText[260 * v8], &v45);
//			D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v21 + 128, (size_t*)v21 + 129);
//			if (v5 <= *((DWORD*)v21 + 128) + 6)
//				v5 = *((DWORD*)v21 + 128) + 6;
//			v18 = (int)(v21 + 258);
//			goto LABEL_114;
//		}
//		v22 = *((DWORD*)v1 + 140);
//		if (v22 == 'D2DV' || v22 == 'D2XP' || v22 == 'D2ST')
//		{
//LABEL_22:
//			if (*((DWORD*)v1 + 139) <= 0)
//			{
//LABEL_117:
//				v9(&v45, "Account Name: ", 256);
//				Unicode::strcat(&v45, (char*)v39 + 204);
//				v28 = &wszText[260 * v8];
//				if (v8 >= 7)
//				{
//					FOG_DisplayAssert(//						"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //						__FILE__, __LINE__//						1837);
//					exit(-1);
//				}
//				Unicode::strcpy(&wszText[260 * v8], &v45);
//				D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v28 + 128, (size_t*)v28 + 129);
//				if (v5 <= *((DWORD*)v28 + 128) + 6)
//					v5 = *((DWORD*)v28 + 128) + 6;
//				v29 = v8 + 1;
//				v30 = v37 + *((DWORD*)v28 + 129) + 6;
//				v31 = v40 - (v5 >> 1);
//				v32 = 0;
//				if (v40 - (v5 >> 1) >= 0)
//				{
//					if (v31 + v5 > 800)
//					{
//						v32 = 800 - v31 - v5;
//						v31 = 800 - v5;
//					}
//				}
//				else
//				{
//					v32 = -v31;
//					v31 = 0;
//				}
//				v33 = nYEnd - v30;
//				D2GFX_10055_DrawSolidRectEx(v31, nYEnd - v30, v31 + v5, nYEnd, 0, 2);
//				v34 = v33 + 3;
//				v41 = v32 + v40;
//				if (v29 > 7)
//				{
//					FOG_DisplayAssert(//						"nValidStrings <= MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //						__FILE__, __LINE__//						2041);
//					exit(-1);
//				}
//				for (i = 0; i < v29; ++i)
//				{
//					v34 += pHeight[130 * i];
//					v36 = 0;
//					if (!i)
//						v36 = v42;
//					D2Win_10117_DrawText(//						(const Unicode*)&wszText[260 * i], //						v41 - (*(DWORD*)&wszText[260 * i + 256] >> 1), //						v34, //						v36, //						0);
//				}
//				return 1;
//			}
//			v12 = *((DWORD*)v1 + 139);
//			memset(&v45, 0, 0x200u);
//			v13 = D2LANG_GetStringFromTblIndex(5017);
//			D2LANG_sprintf(256, &v45, v13, v12);
//			v14 = &wszText[260 * v8];
//			if (v8 >= 7)
//			{
//				FOG_DisplayAssert(//					"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //					__FILE__, __LINE__//					1837);
//				exit(-1);
//			}
//			Unicode::strcpy(&wszText[260 * v8], &v45);
//			D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v14 + 128, (size_t*)v14 + 129);
//			if (v5 <= *((DWORD*)v14 + 128) + 6)
//				v5 = *((DWORD*)v14 + 128) + 6;
//			++v8;
//			v37 += *((DWORD*)v14 + 129);
//			if (gdwBitMasks[5] & *(DWORD*)v39)
//			{
//				v25 = D2LANG_GetStringFromTblIndex(5016);
//				v26 = &wszText[260 * v8];
//				if (v8 >= 7)
//				{
//					FOG_DisplayAssert(//						"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //						__FILE__, __LINE__//						1837);
//					exit(-1);
//				}
//				Unicode::strcpy(&wszText[260 * v8], v25);
//				D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v26 + 128, (size_t*)v26 + 129);
//				if (v5 <= *((DWORD*)v26 + 128) + 6)
//					v5 = *((DWORD*)v26 + 128) + 6;
//				++v8;
//				v37 += *((DWORD*)v26 + 129);
//			}
//			D2LANG_toUnicode(&v45, "Realm: ", 256);
//			Unicode::strcat(&v45, (char*)v39 + 492);
//			v27 = &wszText[260 * v8];
//			if (v8 >= 7)
//			{
//				FOG_DisplayAssert(//					"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //					__FILE__, __LINE__//					1837);
//				exit(-1);
//			}
//			Unicode::strcpy(&wszText[260 * v8], &v45);
//			D2Win_10131_GetTextDimensions((Unicode*)&wszText[260 * v8], (size_t*)v27 + 128, (size_t*)v27 + 129);
//			if (v5 <= *((DWORD*)v27 + 128) + 6)
//				v5 = *((DWORD*)v27 + 128) + 6;
//			v18 = (int)(v27 + 258);
//			goto LABEL_114;
//		}
//		if (v22 > 'SSHR')
//		{
//			if (v22 > 'W3XP')
//			{
//				if (v22 == 'WAR3')
//				{
//					v23 = (int)"Warcraft III";
//					goto LABEL_93;
//				}
//				goto LABEL_91;
//			}
//			if (v22 == 'W3XP')
//			{
//				v23 = (int)"Frozen Throne";
//				goto LABEL_93;
//			}
//			if (v22 != 1398030674)
//			{
//				if (v22 == 1462911566)
//				{
//					v23 = (int)"Warcraft II";
//					goto LABEL_93;
//				}
//				goto LABEL_91;
//			}
//		}
//		else
//		{
//			if (v22 == 'SSHR')
//			{
//				v23 = (int)"Starcraft Shareware";
//				goto LABEL_93;
//			}
//			if (v22 <= 'DTST')
//			{
//				if (v22 == 'DTST' || v22 == 'DRTL')
//				{
//					v23 = (int)"Diablo";
//					goto LABEL_93;
//				}
//				if (v22 == 'DSHR')
//				{
//					v23 = (int)"Diablo Shareware";
//LABEL_93:
//					D2LANG_toUnicode(&v45, v23, 256);
//					v24 = &wszText[260 * v8];
//					if (v8 >= 7)
//					{
//						FOG_DisplayAssert(//							"*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS", //							__FILE__, __LINE__//							1837);
//						exit(-1);
//					}
//					Unicode::strcpy(&wszText[260 * v8], &v45);
//					D2Win_10131_GetTextDimensions(//						(Unicode*)&wszText[260 * v8], //						(size_t*)v24 + 128, //						(size_t*)v24 + 129);
//					if (v5 <= *((DWORD*)v24 + 128) + 6)
//						v5 = *((DWORD*)v24 + 128) + 6;
//					v18 = (int)(v24 + 258);
//					goto LABEL_114;
//				}
//LABEL_91:
//				v23 = (int)"Unknown";
//				goto LABEL_93;
//			}
//			if (v22 != 'JSTR')
//			{
//				if (v22 == 'SEXP')
//				{
//					v23 = (int)"Brood War";
//					goto LABEL_93;
//				}
//				goto LABEL_91;
//			}
//		}
//		v23 = (int)"Starcraft";
//		goto LABEL_93;
//	}
//	return result;
//}
//// 6F8BA058: using guessed type int __cdecl Unicode__sprintf(DWORD, DWORD, DWORD, DWORD);
//// 6F8BA05C: using guessed type int __thiscall Unicode___default constructor closure_(DWORD);
//// 6F8BA060: using guessed type int __fastcall Unicode__strcpy(DWORD, DWORD);
//// 6F8BA064: using guessed type int __fastcall Unicode__strcat(DWORD, DWORD);
//// 6F8A2070: using guessed type Unicode wszText[256];
//// 6F8A2070: using guessed type size_t pHeight[781];

//D2Win.0x6F8A2A20 (#10142)
void __stdcall D2Win_10142_CompUnitDestroy(D2CompositeUnitStrc* pCompositeUnit, int a2)
{
	D2_ASSERT(pCompositeUnit);

	D2GfxInfoStrc* pGfxInfo = pCompositeUnit->pGfxInfo;

	D2_ASSERT(pGfxInfo);

	D2CofInfoStrc* pCofInfo = pGfxInfo->pCofInfo;
	while (pCofInfo)
	{
		D2CofNodeStrc* pRemCofNode = pCofInfo->pCofNode;
		D2CofInfoStrc* pNextCofInfo = pCofInfo->pNext;

		D2_ASSERT(pRemCofNode);

		if (dword_6F8FD654)
		{
			--pRemCofNode->unk0x00;
			if (pRemCofNode->unk0x00 <= 0)
			{
				D2CofNodeStrc* v8 = pRemCofNode->unk0x1C;
				D2CofNodeStrc* v9 = pRemCofNode->unk0x18;

				D2_FREE(pRemCofNode->pCofData);
				pRemCofNode->pCofData = nullptr;

				if (v8)
				{
					if (v9)
					{
						v8->unk0x18 = v9;
						v9->unk0x1C = v8;
					}
					else
					{
						v8->unk0x18 = nullptr;
					}
				}
				else
				{
					dword_6F8FD654 = v9;
					if (v9)
					{
						v9->unk0x1C = nullptr;
					}
				}
				D2_FREE(pRemCofNode);
			}
		}

		D2_FREE(pCofInfo);
		pCofInfo = pNextCofInfo;
	}

	pGfxInfo->pCofInfo = nullptr;
	D2_FREE(pGfxInfo);

	if (pCompositeUnit->pEmblemCellFile)
	{
		ARCHIVE_FreeCellFile(pCompositeUnit->pEmblemCellFile);
		pCompositeUnit->pEmblemCellFile = nullptr;
	}

	if (a2)
	{
		D2CompositeUnitStrc* v11 = pCompositeUnit->unk0xC8;
		D2CompositeUnitStrc* v12 = pCompositeUnit->pNext;
		if (v11)
		{
			v11->pNext = v12;
			if (v12)
			{
				v12->unk0xC8 = v11;
			}
		}
		else
		{
			dword_6F8FD650 = pCompositeUnit->pNext;
			if (v12)
			{
				v12->unk0xC8 = nullptr;
			}
		}
		D2_FREE(pCompositeUnit);
	}
}

//D2Win.0x6F8A2BD0 (#10154)
void __stdcall D2Win_10154(D2CompositeUnitStrc* pCompositeUnit, const Unicode* a2, const Unicode* a3, const Unicode* a4, const Unicode* a5, int a6, int a7)
{
	D2_ASSERT(pCompositeUnit);

	Unicode::strcpy(pCompositeUnit->unk0xCC, a2);
	Unicode::strcpy(pCompositeUnit->unk0x14C, a3);
	Unicode::strcpy(pCompositeUnit->unk0x1CC, a4);
	Unicode::strcpy(pCompositeUnit->unk0x1EC, a5);
	pCompositeUnit->unk0x22C = a6;
	pCompositeUnit->unk0x230 = a7;
}

//D2Win.0x6F8A2C50 (#10143)
BOOL __fastcall D2Win_10143(D2CompositeUnitStrc* pCompositeUnit, int nMode)
{
	D2_ASSERT(pCompositeUnit);

	
	for (D2CofInfoStrc* pCofInfo = pCompositeUnit->pGfxInfo->pCofInfo; pCofInfo; pCofInfo = pCofInfo->pNext)
	{
		if (pCofInfo->pCofNode->nMode == nMode)
		{
			D2CofDataStrc* pCofData = pCofInfo->pCofNode->pCofData;

			D2_ASSERT(pCofData);

			pCompositeUnit->nMode = nMode;
			pCompositeUnit->unk0x08 = 0;
			pCompositeUnit->unk0x0C = pCofData->unk0x01;
			pCompositeUnit->unk0x10 = pCofData->unk0x18;
			pCompositeUnit->unk0x05 = pCofData->unk0x02;

			return TRUE;
		}
	}

	return FALSE;
}

//1.10f: D2Win.0x6F8A2CE0
void* __stdcall D2WIN_DATATBLS_CompileTxt(HD2ARCHIVE hArchive, const char* szName, D2BinFieldStrc* pBinField, int* pRecordCount, int nRecordSize)
{
	char szFilename[260] = {};

	if (gDATATBLS_LoadFromBin)
	{
		wsprintfA(szFilename, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", szName, ".bin");
	}
	else if (_strcmpi(szName, "leveldefs"))
	{
		wsprintfA(szFilename, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", szName, ".txt");
	}
	else
	{
		wsprintfA(szFilename, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", "levels", ".txt");
	}

	unsigned int nFileSize = 0;
	void* pFileData = ARCHIVE_ALLOC_BUFFER_AND_READ_FILE_TO_IT(hArchive, szFilename, &nFileSize);

	D2_ASSERT(pFileData);

	void* pBinData = nullptr;
	int nRecordCount = 0;
	if (gDATATBLS_LoadFromBin)
	{
		nRecordCount = *(int*)pFileData;
		pBinData = (char*)pFileData + 4;
	}
	else
	{
		D2BinFileStrc* pBinFile = FOG_CreateBinFile(pFileData, nFileSize);
		nRecordCount = FOG_GetRecordCountFromBinFile(pBinFile);
		pBinData = FOG_AllocPool(0, nRecordSize * nRecordCount, __FILE__, __LINE__, 0);
		memset(pBinData, 0x00, nRecordSize * nRecordCount);

		FOG_10207(pBinFile, pBinField, pBinData, nRecordCount, nRecordSize);
		FOG_FreeBinFile(pBinFile);
	}

	if (pRecordCount)
	{
		*pRecordCount = nRecordCount;
	}

	return pBinData;
}

////D2Win.0x6F8A2E70) --------------------------------------------------------
//int __thiscall sub_6F8A2E70(int this)
//{
//	int v1; // esi@1
//	int v2; // ebx@1
//	int v3; // ebp@1
//	int v4; // ST28_4@1
//	int v5; // ST24_4@1
//	int v6; // eax@1
//	char v7; // dl@1
//	unsigned int v8; // ecx@1
//	void *v9; // edi@1
//	const void *v10; // esi@1
//	int v11; // edx@1
//	int v12; // eax@1
//	void *v13; // edi@1
//	char v14; // dl@1
//	unsigned int v15; // ecx@1
//	int v16; // edi@1
//	int i; // ecx@1
//	int v18; // eax@4
//	int v19; // ecx@5
//	int v20; // edx@5
//	int result; // eax@9
//	int j; // esi@9
//	int v23; // [sp+30h] [bp-F0h]@1
//	int v24; // [sp+34h] [bp-ECh]@1
//	int v25; // [sp+38h] [bp-E8h]@1
//	int v26; // [sp+3Ch] [bp-E4h]@1
//	int v27; // [sp+40h] [bp-E0h]@1
//	int v28; // [sp+44h] [bp-DCh]@1
//	int v29; // [sp+48h] [bp-D8h]@1
//	int v30; // [sp+4Ch] [bp-D4h]@1
//	int v31; // [sp+50h] [bp-D0h]@1
//	int *v32; // [sp+54h] [bp-CCh]@1
//	int v33; // [sp+58h] [bp-C8h]@1
//	int v34; // [sp+5Ch] [bp-C4h]@1
//	int v35; // [sp+60h] [bp-C0h]@1
//	int v36; // [sp+64h] [bp-BCh]@1
//	int v37; // [sp+68h] [bp-B8h]@1
//	int v38; // [sp+6Ch] [bp-B4h]@1
//	int v39; // [sp+70h] [bp-B0h]@1
//	int v40; // [sp+74h] [bp-ACh]@1
//	int v41; // [sp+78h] [bp-A8h]@1
//	int v42; // [sp+7Ch] [bp-A4h]@1
//	int v43; // [sp+80h] [bp-A0h]@1
//	int v44; // [sp+84h] [bp-9Ch]@1
//	int v45; // [sp+88h] [bp-98h]@1
//	int v46; // [sp+8Ch] [bp-94h]@1
//	int v47; // [sp+90h] [bp-90h]@1
//	int v48; // [sp+94h] [bp-8Ch]@1
//	int v49; // [sp+98h] [bp-88h]@1
//	int v50; // [sp+9Ch] [bp-84h]@1
//	int v51; // [sp+A0h] [bp-80h]@1
//	int v52; // [sp+A4h] [bp-7Ch]@1
//	int v53; // [sp+A8h] [bp-78h]@1
//	int v54; // [sp+ACh] [bp-74h]@1
//	int v55; // [sp+B0h] [bp-70h]@1
//	int v56; // [sp+B4h] [bp-6Ch]@1
//	int v57; // [sp+B8h] [bp-68h]@1
//	int v58; // [sp+BCh] [bp-64h]@1
//	int v59; // [sp+C0h] [bp-60h]@1
//	int v60; // [sp+C4h] [bp-5Ch]@1
//	int v61; // [sp+C8h] [bp-58h]@1
//	int v62; // [sp+CCh] [bp-54h]@1
//	int v63; // [sp+D0h] [bp-50h]@1
//	int v64; // [sp+D4h] [bp-4Ch]@1
//	int v65; // [sp+D8h] [bp-48h]@1
//	int v66; // [sp+DCh] [bp-44h]@1
//	int v67; // [sp+E0h] [bp-40h]@1
//	int v68; // [sp+E4h] [bp-3Ch]@1
//	int v69; // [sp+E8h] [bp-38h]@1
//	int v70; // [sp+ECh] [bp-34h]@1
//	int v71; // [sp+F0h] [bp-30h]@1
//	int v72; // [sp+F4h] [bp-2Ch]@1
//	int v73; // [sp+F8h] [bp-28h]@1
//	int v74; // [sp+FCh] [bp-24h]@1
//	int v75; // [sp+100h] [bp-20h]@1
//	int v76; // [sp+104h] [bp-1Ch]@1
//	int v77; // [sp+108h] [bp-18h]@1
//	char v78[4]; // [sp+10Ch] [bp-14h]@1
//	int v79; // [sp+110h] [bp-10h]@1
//	int v80; // [sp+114h] [bp-Ch]@1
//	int v81; // [sp+118h] [bp-8h]@1
//	int v82; // [sp+11Ch] [bp-4h]@1
//
//	v44 = 4;
//	v49 = 4;
//	v54 = 4;
//	v74 = 4;
//	v1 = this;
//	v2 = 0;
//	v23 = (int)"version";
//	v24 = 3;
//	v25 = 0;
//	v26 = 246;
//	v27 = 0;
//	v28 = (int)"type";
//	v29 = 15;
//	v30 = 0;
//	v31 = 286;
//	v32 = &dword_6F8FD240;
//	v33 = (int)"code";
//	v34 = 9;
//	v35 = 0;
//	v36 = 128;
//	v37 = 0;
//	v38 = (int)"alternategfx";
//	v39 = 9;
//	v40 = 0;
//	v41 = 144;
//	v42 = 0;
//	v43 = (int)"spawnable";
//	v45 = 0;
//	v46 = 307;
//	v47 = 0;
//	v48 = (int)"level";
//	v50 = 0;
//	v51 = 253;
//	v52 = 0;
//	v53 = (int)"component";
//	v55 = 0;
//	v56 = 277;
//	v57 = 0;
//	v58 = (int)"wclass";
//	v59 = 9;
//	v60 = 0;
//	v61 = 192;
//	v62 = 0;
//	v63 = (int)"2handedwclass";
//	v64 = 9;
//	v65 = 0;
//	v66 = 196;
//	v67 = 0;
//	v68 = (int)"1or2handed";
//	v69 = 6;
//	v70 = 0;
//	v71 = 317;
//	v72 = 0;
//	v73 = (int)"Transform";
//	v75 = 0;
//	v76 = 321;
//	v77 = 0;
//	*(DWORD*)v78 = "end";
//	v79 = 0;
//	v80 = 0;
//	v81 = 0;
//	v82 = 0;
//	v3 = sub_6F8A2CE0(this, "weapons", (int)&v23, (int)&dword_6F8C1504, 424);
//	v4 = sub_6F8A2CE0(v1, "armor", (int)&v23, (int)&dword_6F8C150C, 424);
//	v5 = sub_6F8A2CE0(v1, "misc", (int)&v23, (int)&dword_6F8C1514, 424);
//	dword_6F8C14F8 = dword_6F8C1504 + dword_6F8C1514 + dword_6F8C150C;
//	v6 = (int)FOG_AllocPool(//		0, //		424 * dword_6F8C14F8, //		__FILE__, __LINE__//		2601, //		0);
//	dword_6F8C14FC = v6;
//	dword_6F8C1500 = v6;
//	v7 = -88 * dword_6F8C1504;
//	v8 = (unsigned int)(424 * dword_6F8C1504) >> 2;
//	memcpy((void*)v6, (const void*)v3, 4 * v8);
//	v10 = (const void*)(v3 + 4 * v8);
//	v9 = (void*)(v6 + 4 * v8);
//	LOBYTE(v8) = v7;
//	v11 = v6;
//	memcpy(v9, v10, v8 & 3);
//	v12 = v6 + 424 * dword_6F8C1504;
//	dword_6F8C1508 = v11 + 424 * dword_6F8C1504;
//	memcpy((void*)(v11 + 424 * dword_6F8C1504), (const void*)v4, 424 * dword_6F8C150C);
//	dword_6F8C1510 = v12 + 424 * dword_6F8C150C;
//	memcpy((void*)(v12 + 424 * dword_6F8C150C), (const void*)v5, 424 * dword_6F8C1514);
//	FOG_FreeServerMemory(//		0, //		(void*)(v3 - 4), //		__FILE__, __LINE__//		2617, //		0);
//	FOG_FreeServerMemory(//		0, //		(void*)(v4 - 4), //		__FILE__, __LINE__//		2618, //		0);
//	FOG_FreeServerMemory(//		0, //		(void*)(v5 - 4), //		__FILE__, __LINE__//		2619, //		0);
//	v13 = FOG_AllocPool(//		0, //		2 * dword_6F8C14F8, //		__FILE__, __LINE__//		2624, //		0);
//	v14 = 2 * dword_6F8C14F8;
//	dword_6F8C1518 = (int)v13;
//	v15 = (unsigned int)(2 * dword_6F8C14F8) >> 2;
//	memset(v13, 0, 4 * v15);
//	v16 = (int)((char*)v13 + 4 * v15);
//	for (i = v14 & 3; i; --i)
//		*(BYTE*)v16++ = 0;
//	v18 = 0;
//	if (dword_6F8C14F8 > 0)
//	{
//		v19 = 0;
//		v20 = dword_6F8C14FC + 246;
//		do
//		{
//			if (!*(WORD*)v20)
//			{
//				v19 += 2;
//				*(WORD*)(v19 + dword_6F8C1518 - 2) = v18;
//			}
//			v20 += 424;
//			++v18;
//		}
//		while (v18 < dword_6F8C14F8);
//	}
//	dword_6F8FD23C = Fog_10211(__FILE__, __LINE__);
//	result = dword_6F8C14F8;
//	for (j = 0; j < dword_6F8C14F8; v2 += 424)
//	{
//		Fog_10215(dword_6F8FD23C, *(DWORD*)(v2 + dword_6F8C14FC + 128));
//		result = dword_6F8C14F8;
//		++j;
//	}
//	return result;
//}

//D2Win.0x6F8A32B0
BOOL __stdcall sub_6F8A32B0(int a1, int nItemType)
{
	if (!nItemType)
	{
		return TRUE;
	}

	if (a1)
	{
		int itemTypeStack[65] = {};

		itemTypeStack[1] = a1;
		int nStackIndex = 1;
		while (nStackIndex)
		{
			const int v5 = itemTypeStack[nStackIndex];
			--nStackIndex;

			if (nItemType == v5)
			{
				return TRUE;
			}

			D2ItemTypesTxt* pItemTypesTxtRecord = &gpItemTypesTxt[v5];
			if (pItemTypesTxtRecord->nEquiv1)
			{
				++nStackIndex;
				itemTypeStack[nStackIndex] = pItemTypesTxtRecord->nEquiv1;

				if (pItemTypesTxtRecord->nEquiv2)
				{
					++nStackIndex;
					itemTypeStack[nStackIndex] = pItemTypesTxtRecord->nEquiv2;
				}
			}

			D2_ASSERT(nStackIndex < 64);
		}
	}

	return FALSE;
}

//D2Win.0x6F8A3360 (#10159)
void __stdcall D2Win_10159(D2CellFileStrc* pSquelchCelFile, D2CellFileStrc* pSelectedCelFile)
{
	ghSquelchCelFile_6F8FD65C = pSquelchCelFile;
	gnSquelchCelFileFrameCount_6F8FD660 = D2CMP_CelFileGetCelsPerDirection(pSquelchCelFile);
	ghSelectedCelFile_6F8FD668 = pSelectedCelFile;
	gnSelectedCelFileFrameCount_6F8FD66C = D2CMP_CelFileGetCelsPerDirection(pSelectedCelFile);
}

//D2Win.0x6F8A3390 (#10144)
//TODO: ...
void __stdcall D2Win_10144(unsigned int nUnitIndex, int nMode, BYTE* pComponents, BYTE* pComponentsColors)
{
	//D2CompositeUnitStrc* v4; // esi@1
	//D2GfxInfoStrc* v5; // eax@4
	//BYTE* v6; // ebx@7
	//int v7; // edi@9
	//int v8; // edi@9
	//int v9; // ecx@9
	//int* v10; // edi@9
	//int v11; // eax@9
	//int v12; // ebp@9
	//BYTE v13; // al@10
	//int v14; // eax@14
	//BYTE* v15; // edx@18
	//BYTE* v16; // eax@18
	//BYTE v17; // cl@19

	//v4 = (D2CompositeUnitStrc*)FOG_Alloc(
	//	564,
	//	"C:\\projects\\D2\\head\\Diablo2\\Source\\D2Win\\Src\\D2Comp.cpp",
	//	3173,
	//	0);
	//if (!v4)
	//{
	//	FOG_DisplayAssert("ptNewUnit", __FILE__, __LINE__);
	//	exit(-1);
	//}
	//memset(v4, 0, 0x234u);
	//v5 = (D2GfxInfoStrc*)FOG_Alloc(
	//	4,
	//	"C:\\projects\\D2\\head\\Diablo2\\Source\\D2Win\\Src\\D2Comp.cpp",
	//	2139,
	//	0);
	//if (!v5)
	//{
	//	FOG_DisplayAssert("ptGfxInfo", __FILE__, __LINE__);
	//	exit(-1);
	//}
	//v5->pCofInfo = 0;
	//v4->pGfxInfo = v5;
	//sub_6F8A3680();
	//v6 = a3;
	//v4->nUnitIndex = nUnitIndex;
	//if (nUnitIndex < 7)
	//{
	//	v14 = sub_6F8A3DC0(a3, nUnitIndex);
	//	v4->nWClassTokenIndex = v14;
	//	if (!v14)
	//		return;
	//}
	//else
	//{
	//	v4->nWClassTokenIndex = 1;
	//}
	//v7 = v4->dwFlags;
	//v4->nMode = nMode;
	//v8 = gdwBitMasks[0] | v7;
	//v4->dwFlags = v8;
	//v9 = v8;
	//v10 = v4->dwArmorType;
	//v11 = v9 | gdwBitMasks[1];
	//v4->dwFlags = v11;
	//v12 = 0;
	//v4->dwFlags = v11 | gdwBitMasks[2];
	//do
	//{
	//	v10[16] = 1;
	//	v13 = *v6;
	//	if (*v6 && v13 < dword_6F8FD230 && v13 != -1)
	//	{
	//		sub_6F8A4020(v12, v6);
	//		*v10 = stru_6F8C1520[*v6].dwItemCode;
	//		v10[16] = stru_6F8C1520[*v6].nItemType;
	//	}
	//	else
	//	{
	//		*v10 = 0;
	//	}
	//	++v6;
	//	++v12;
	//	++v10;
	//}
	//while ((unsigned int)v12 < 16);
	//v15 = a4;
	//v16 = v4->unk0x28;
	//do
	//{
	//	v17 = *v15++;
	//	*v16 = v17;
	//	if (v17 != -1)
	//		*v16 = v17 - 1;
	//	++v16;
	//}
	//while ((unsigned int)&v16[-40 - (_DWORD)v4] < 16);
	//if (!sub_6F8A3570(v4) || D2Win_10143(v4, nMode))
	//{
	//	v4->unk0xC8 = 0;
	//	v4->pNext = dword_6F8FD650;
	//	if (dword_6F8FD650)
	//		dword_6F8FD650->unk0xC8 = v4;
	//	dword_6F8FD650 = v4;
	//}
}

//D2Win.0x6F8A3570
int __fastcall sub_6F8A3570(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	if (pCompositeUnit->nUnitIndex >= dword_6F8BC0A4 || pCompositeUnit->nMode >= (unsigned int)gnPlayerModeTokenTblSize_6F8BC0F8 || pCompositeUnit->nWClassTokenIndex >= (unsigned int)dword_6F8BC190)
	{
		pCompositeUnit->nUnitIndex = 7;
		pCompositeUnit->nMode = 5;
		pCompositeUnit->nWClassTokenIndex = 1;
	}

	D2GfxInfoStrc* pGfxInfo = pCompositeUnit->pGfxInfo;

	D2_ASSERT(pGfxInfo);

	int v4 = 0;

	if (sub_6F8A1250(pGfxInfo, pCompositeUnit->nUnitIndex, pCompositeUnit->nMode, &v4, &v4, dword_6F8BC154[pCompositeUnit->nWClassTokenIndex]))
	{
		return 1;
	}

	pCompositeUnit->nMode = 5;
	pCompositeUnit->nUnitIndex = 7;
	pCompositeUnit->nWClassTokenIndex = 1;

	D2CofDataStrc* pCurrCofData = sub_6F8A1250(pGfxInfo, 7u, 5u, &v4, &v4, dword_6F8BC154[1]);

	D2_ASSERT(pCurrCofData);
	return 1;
}


char gtUnk_6F8C1520[0xBF4u];
//1.10f:DWin.0x6F8FD240
D2TxtLinkStrc* gpItemTypesLinker = nullptr;
//1.10f:D2Win.0x6F8FD238
D2TxtLinkStrc* gpHitClassLinker = nullptr;

//1.10f:DWin.0x6F8FD674
BOOL gbLoadedCompTxts = FALSE;

//D2Win.0x6F8A3680
void sub_6F8A3680()
{
	if (!gbLoadedCompTxts)
	{
		memset(gtUnk_6F8C1520, 0, sizeof(gtUnk_6F8C1520));
		gbLoadedCompTxts= TRUE;

		D2BinFieldStrc pTbl[] =
		{
			{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &gpItemTypesLinker },
			{ "equiv1", TXTFIELD_CODETOWORD, 0, TXTFIELD_BYTE, &gpItemTypesLinker },
			{ "equiv2", TXTFIELD_CODETOWORD, 0, 6, &gpItemTypesLinker },
			{ "repair", TXTFIELD_BYTE, 0, 8, NULL },
			{ "body", TXTFIELD_BYTE, 0, 9, NULL },
			//{ "bodyloc1", TXTFIELD_CODETOBYTE, 0, 10, &sgptDataTables->pBodyLocsLinker },
			//{ "bodyloc2", TXTFIELD_CODETOBYTE, 0, 11, &sgptDataTables->pBodyLocsLinker },
			{ "shoots", TXTFIELD_CODETOWORD, 0, 12, &gpItemTypesLinker },
			{ "quiver", TXTFIELD_CODETOWORD, 0, 14, &gpItemTypesLinker },
			{ "throwable", TXTFIELD_BYTE, 0, 16, NULL },
			{ "reload", TXTFIELD_BYTE, 0, 17, NULL },
			{ "reequip", TXTFIELD_BYTE, 0, 18, NULL },
			{ "autostack", TXTFIELD_BYTE, 0, 19, NULL },
			{ "magic", TXTFIELD_BYTE, 0, 20, NULL },
			//{ "rare", TXTFIELD_BYTE, 0, 21, NULL },
			{ "normal", TXTFIELD_BYTE, 0, 22, NULL },
			{ "charm", TXTFIELD_BYTE, 0, 23, NULL },
			{ "gem", TXTFIELD_BYTE, 0, 24, NULL },
			{ "beltable", TXTFIELD_BYTE, 0, 25, NULL },
			//{ "maxsock1", TXTFIELD_BYTE, 0, 26, NULL },
			//{ "maxsock25", TXTFIELD_BYTE, 0, 27, NULL },
			//{ "maxsock40", TXTFIELD_BYTE, 0, 28, NULL },
			//{ "treasureclass", TXTFIELD_BYTE, 0, 29, NULL },
			//{ "rarity", TXTFIELD_BYTE, 0, 30, NULL },
			//{ "staffmods", TXTFIELD_CODETOBYTE, 0, 31, &sgptDataTables->pPlayerClassLinker },
			{ "costformula", TXTFIELD_BYTE, 0, 32, NULL },
			//{ "class", TXTFIELD_CODETOBYTE, 0, 33, &sgptDataTables->pPlayerClassLinker },
			//{ "storepage", TXTFIELD_CODETOBYTE, 0, 34, &sgptDataTables->pStorePageLinker },
			{ "varinvgfx", TXTFIELD_BYTE, 0, 35, NULL },
			{ "invgfx1", TXTFIELD_ASCII, 31, 36, NULL },
			{ "invgfx2", TXTFIELD_ASCII, 31, 68, NULL },
			{ "invgfx3", TXTFIELD_ASCII, 31, 100, NULL },
			{ "invgfx4", TXTFIELD_ASCII, 31, 132, NULL },
			{ "invgfx5", TXTFIELD_ASCII, 31, 164, NULL },
			//{ "invgfx6", TXTFIELD_ASCII, 31, 196, NULL },
			{ "end", TXTFIELD_NONE, 0, 0, NULL },
		};

		gpItemTypesLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		gpItemTypesTxt = (D2ItemTypesTxt*)D2WIN_DATATBLS_CompileTxt(D2Win_GetArchive(), "itemtypes", pTbl, &gnItemTypesTxtRecordCount, sizeof(D2ItemTypesTxt));

		D2BinFieldStrc pHitClassTbl[] =
		{
			{ "code", TXTFIELD_ASCIITOCODE, 0, 0, &gpHitClassLinker },
			{ "end", TXTFIELD_NONE, 0, 0, NULL },
		};
		gpHitClassLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);
		int nRecordCount;
		gpHitClassTxt = (D2HitClassTxt*)D2WIN_DATATBLS_CompileTxt(D2Win_GetArchive(), "hitclass", pHitClassTbl, &nRecordCount, sizeof(D2HitClassTxt));

		UNIMPLEMENTED();
#if 0 // TODO !
		sub_6F8A2E70(D2Win_GetArchive());
		dword_6F8C1530 = dword_6F8BCF98[0];
		dword_6F8C153C = dword_6F8BCF98[0];
		dword_6F8C1548 = dword_6F8BCF98[0];
		result = dword_6F8C14F8;
		v4 = 4;
		dword_6F8C152C = ' til';
		dword_6F8C1534 = 3;
		dword_6F8C1538 = ' dem';
		dword_6F8C1540 = 3;
		dword_6F8C1544 = ' yvh';
		dword_6F8C154C = 3;
		v18 = 4;
		v23 = 0;
		if (dword_6F8C14F8 > 0)
		{
			v20 = 48;
			v19 = 0;
			do
			{
				v5 = v19 + dword_6F8C14FC;
				if (*(_DWORD*)(v19 + dword_6F8C14FC + 144))
				{
					v6 = *(_DWORD*)(v19 + dword_6F8C14FC + 144);
					v21 = v6;
				}
				else
				{
					v21 = *(_DWORD*)(v5 + 128);
					v6 = v21;
				}
				v22 = 1;
				if (!sub_6F8A32B0(*(__int16*)(v5 + 286), 45)
					&& !sub_6F8A32B0(*(__int16*)(v5 + 286), 3)
					&& !sub_6F8A32B0(*(__int16*)(v5 + 286), 51)
					&& !sub_6F8A32B0(*(__int16*)(v5 + 286), 37)
					|| sub_6F8A32B0(*(__int16*)(v5 + 286), 75))
				{
					v22 = 0;
				}
				v7 = 0;
				if (v20 <= 0)
				{
				LABEL_17:
					if (v22 == 1)
					{
						for (i = v20;
							sub_6F8A32B0(*(int*)((char*)&dword_6F8BC1E0 + i), 45) && sub_6F8A32B0(*(__int16*)(v5 + 286), 45)
							|| sub_6F8A32B0(*(int*)((char*)&dword_6F8BC1E0 + i), 50) && sub_6F8A32B0(*(__int16*)(v5 + 286), 50)
							|| *(int*)((char*)dword_6F8C1520 + i);
							i += 12)
						{
							++v4;
						}
						if (v4 >= 255)
							v4 = v18;
						v10 = 3 * v4;
						dword_6F8C1520[v10] = v21;
						v11 = dword_6F8BCF90;
						dword_6F8C1528[v10] = *(__int16*)(v5 + 286);
						v12 = 0;
						if (v11)
						{
							while (*(_DWORD*)(v5 + 192) != dword_6F8BCF28[2 * v12])
							{
								if (++v12 >= v11)
									goto LABEL_30;
							}
							v13 = dword_6F8BCF2C[2 * v12];
						}
						else
						{
						LABEL_30:
							v13 = 0;
						}
						dword_6F8C1524[3 * v4] = dword_6F8BCF98[v13];
						v14 = 0;
						if (v11)
						{
							v15 = *(_DWORD*)(v5 + 196);
							while (v15 != dword_6F8BCF28[2 * v14])
							{
								if (++v14 >= v11)
									goto LABEL_38;
							}
							v16 = dword_6F8BCF2C[2 * v14];
						}
						else
						{
						LABEL_38:
							v16 = 0;
						}
						dword_6F8FD250[v4] = dword_6F8BCF98[v16];
						if (v18 == v4)
						{
							++v18;
							v20 += 12;
						}
						v4 = v18;
					}
				}
				else
				{
					v8 = dword_6F8C1520;
					while (*v8 != v6)
					{
						++v7;
						v8 += 3;
						if (v7 >= v4)
							goto LABEL_17;
					}
				}
				result = v23 + 1;
				v17 = ++v23 < dword_6F8C14F8;
				v19 += 424;
			} while (v17);
		}
		dword_6F8FD230 = 255;
#endif
	}
}

////D2Win.0x6F8A3DC0) --------------------------------------------------------
//signed int __fastcall sub_6F8A3DC0(int a1, int a2)
//{
//	int v2; // ebp@1
//	char v3; // al@1
//	int v4; // esi@4
//	int v5; // eax@5
//	int v6; // eax@11
//	int v7; // edi@16
//	int v8; // eax@17
//	int v9; // eax@20
//	signed int result; // eax@35
//	signed int v11; // [sp+Ch] [bp-8h]@2
//	int v12; // [sp+10h] [bp-4h]@1
//
//	v2 = a1;
//	v3 = *(BYTE*)(a1 + 5);
//	v12 = a2;
//	if (v3 == -1 || (v11 = 1, *(BYTE*)(a1 + 6) == -1))
//		v11 = 0;
//	v4 = 0;
//	if (v3 != -1)
//	{
//		v5 = *(BYTE*)(a1 + 5);
//		if (v11)
//		{
//			v4 = dword_6F8FD250[v5];
//		}
//		else
//		{
//			v4 = dword_6F8C1524[3 * v5];
//			if (*(BYTE*)(a1 + 6) == -1 && *(BYTE*)(a1 + 7) == -1 && dword_6F8FD250[v5] != v4)
//				v4 = dword_6F8FD250[v5];
//		}
//		v6 = dword_6F8C1528[3 * v5];
//		if ((v4 == 13 || v4 == 14) && a2 != 6 || sub_6F8A32B0(v6, 50))
//			v4 = dword_6F8BC1DC[3 * *(BYTE*)(v2 + 5)];
//	}
//	v7 = 0;
//	if (*(BYTE*)(v2 + 6) != -1)
//	{
//		v8 = *(BYTE*)(v2 + 6);
//		v7 = v11 ? dword_6F8FD250[v8] : dword_6F8C1524[3 * v8];
//		if (((v9 = dword_6F8C1528[3 * v8], v7 == 13) || v7 == 14) && v12 != 6 || sub_6F8A32B0(v9, 50))
//			v7 = dword_6F8BC1DC[3 * *(BYTE*)(v2 + 6)];
//	}
//	if ((v4 == 13 || v4 == 14) && v12 != 6)
//		v4 = 0;
//	if (v7 == 13 || v7 == 14)
//	{
//		if (v12 != 6)
//			v7 = 0;
//	}
//	if (v4)
//	{
//		if (v4 == v7 && (v4 == 6 || v4 == 7))
//			return v4;
//		if (v4 == 8)
//			return 8;
//		if (!v7)
//			return v4;
//		if (v4 == 4)
//		{
//			if (v7 == 4)
//				return 11;
//			if (v7 == 2)
//				return 9;
//		}
//		else
//		{
//			if (v4 == 2)
//			{
//				if (v7 == 4)
//					return 12;
//				goto LABEL_60;
//			}
//			if (v4 == 5 && v7 == 2)
//				return 11;
//		}
//		if (v7 == 4)
//		{
//			if (v4 != 2)
//				goto LABEL_57;
//			return 11;
//		}
//LABEL_60:
//		if (v7 == 5)
//		{
//			if (v4 == 2)
//				return 11;
//		}
//		else
//		{
//			if (v4 == 2)
//			{
//				if (v7 == 2)
//					return 10;
//				return 0;
//			}
//		}
//LABEL_57:
//		if (v4 == 5)
//		{
//			if (v7 == 4 || v7 == 5)
//				return 11;
//		}
//		else
//		{
//			if (v4 != 4)
//			{
//				if (v4 == 13)
//				{
//					if (v7 != 13)
//						return 0;
//				}
//				else
//				{
//					if (v4 != 14)
//					{
//						if (v4 == 1 && v7 == 1)
//							return 1;
//						return 0;
//					}
//					if (v7 != 14)
//						return 0;
//				}
//				return 13;
//			}
//			if (v7 == 5)
//				return 11;
//		}
//		return 0;
//	}
//	if (v7)
//		result = v7;
//	else
//		result = 1;
//	return result;
//}

////D2Win.0x6F8A4020) --------------------------------------------------------
//void __fastcall sub_6F8A4020(int a1, int a2)
//{
//	int v2; // ebp@1
//	int v3; // esi@1
//	int v4; // eax@7
//	int v5; // ecx@8
//	int v6; // eax@15
//	int v7; // ecx@16
//	int v8; // eax@23
//	int v9; // ecx@24
//	int v10; // eax@31
//	int v11; // ecx@32
//	int v12; // eax@41
//	int v13; // ecx@42
//
//	v2 = a1;
//	v3 = a2;
//	if ((a1 == 1 || a1 == 2 || a1 == 3 || a1 == 4) && *(BYTE*)a2 > 3u)
//	{
//		if (sub_6F8A32B0(dword_6F8BC1E0[3 * *(BYTE*)a2], 3))
//		{
//			v4 = 0;
//			if (dword_6F8FD230 <= 0)
//				goto LABEL_13;
//			v5 = (int)dword_6F8C1520;
//			while (*(DWORD*)v5 != dword_6F8BC1D8[3 * *(BYTE*)v3])
//			{
//				++v4;
//				v5 += 12;
//				if (v4 >= dword_6F8FD230)
//					goto LABEL_13;
//			}
//			*(BYTE*)v3 = v4;
//			if (v4 >= dword_6F8FD230)
//				LABEL_13:
//			*(BYTE*)v3 = *(BYTE*)v3 % 3 + 1;
//		}
//		if (sub_6F8A32B0(dword_6F8BC1E0[3 * *(BYTE*)v3], 16))
//		{
//			v6 = 0;
//			if (dword_6F8FD230 <= 0)
//				goto LABEL_21;
//			v7 = (int)dword_6F8C1520;
//			while (*(DWORD*)v7 != dword_6F8BC1D8[3 * *(BYTE*)v3])
//			{
//				++v6;
//				v7 += 12;
//				if (v6 >= dword_6F8FD230)
//					goto LABEL_21;
//			}
//			*(BYTE*)v3 = v6;
//			if (v6 >= dword_6F8FD230)
//				LABEL_21:
//			*(BYTE*)v3 = *(BYTE*)v3 % 3 + 1;
//		}
//		if (sub_6F8A32B0(dword_6F8BC1E0[3 * *(BYTE*)v3], 15))
//		{
//			v8 = 0;
//			if (dword_6F8FD230 <= 0)
//				goto LABEL_29;
//			v9 = (int)dword_6F8C1520;
//			while (*(DWORD*)v9 != dword_6F8BC1D8[3 * *(BYTE*)v3])
//			{
//				++v8;
//				v9 += 12;
//				if (v8 >= dword_6F8FD230)
//					goto LABEL_29;
//			}
//			*(BYTE*)v3 = v8;
//			if (v8 >= dword_6F8FD230)
//				LABEL_29:
//			*(BYTE*)v3 = *(BYTE*)v3 % 3 + 1;
//		}
//		if (!sub_6F8A32B0(dword_6F8BC1E0[3 * *(BYTE*)v3], 51) || (v10 = 0, dword_6F8FD230 <= 0))
//			goto LABEL_37;
//		v11 = (int)dword_6F8C1520;
//		while (*(DWORD*)v11 != dword_6F8BC1D8[3 * *(BYTE*)v3])
//		{
//			++v10;
//			v11 += 12;
//			if (v10 >= dword_6F8FD230)
//				goto LABEL_37;
//		}
//		*(BYTE*)v3 = v10;
//		if (v10 >= dword_6F8FD230)
//			LABEL_37:
//		*(BYTE*)v3 = *(BYTE*)v3 % 3 + 1;
//	}
//	if (!v2 && !sub_6F8A32B0(dword_6F8C1528[3 * *(BYTE*)v3], 37))
//	{
//		if (!sub_6F8A32B0(dword_6F8BC1E0[3 * *(BYTE*)v3], 37) || (v12 = 0, dword_6F8FD230 <= 0))
//			goto LABEL_50;
//		v13 = (int)dword_6F8C1520;
//		while (*(DWORD*)v13 != dword_6F8BC1D8[3 * *(BYTE*)v3])
//		{
//			++v12;
//			v13 += 12;
//			if (v12 >= dword_6F8FD230)
//			{
//				*(BYTE*)v3 = 1;
//				return;
//			}
//		}
//		*(BYTE*)v3 = v12;
//		if (v12 >= dword_6F8FD230)
//			LABEL_50:
//		*(BYTE*)v3 = 1;
//	}
//}

//D2Win.0x6F8A4260 (#10145)
//TODO: ...
void __stdcall D2Win_10145(D2CompositeUnitStrc* pCompositeUnit, unsigned int nUnitIndex, int nMode, BYTE* a4, BYTE* a5)
{
	//D2GfxInfoStrc* pGfxInfo; // eax@4
	//BYTE* v6; // ebx@7
	//int v7; // eax@9
	//int v8; // esi@10
	//int v9; // ecx@10
	//int* v10; // esi@10
	//int v11; // eax@10
	//unsigned int v12; // ebp@10
	//BYTE* v13; // edx@16
	//BYTE* v14; // eax@16
	//BYTE v15; // cl@17

	//if (!pCompositeUnit)
	//{
	//	FOG_DisplayAssert("ptUnit", __FILE__, __LINE__);
	//	exit(-1);
	//}
	//pGfxInfo = (D2GfxInfoStrc*)FOG_Alloc(
	//	4,
	//	"C:\\projects\\D2\\head\\Diablo2\\Source\\D2Win\\Src\\D2Comp.cpp",
	//	2139,
	//	0);
	//if (!pGfxInfo)
	//{
	//	FOG_DisplayAssert("ptGfxInfo", __FILE__, __LINE__);
	//	exit(-1);
	//}
	//v6 = a4;
	//pGfxInfo->pCofInfo = 0;
	//pCompositeUnit->pGfxInfo = pGfxInfo;
	//pCompositeUnit->nUnitIndex = nUnitIndex;
	//if (nUnitIndex < 7)
	//{
	//	v7 = sub_6F8A3DC0(a4, nUnitIndex);
	//	pCompositeUnit->nWClassTokenIndex = v7;
	//	if (!v7)
	//		return;
	//}
	//else
	//{
	//	pCompositeUnit->nWClassTokenIndex = 1;
	//}
	//pCompositeUnit->nMode = nMode;
	//v8 = gdwBitMasks[0] | pCompositeUnit->dwFlags;
	//pCompositeUnit->dwFlags = v8;
	//v9 = v8;
	//v10 = pCompositeUnit->dwArmorType;
	//v11 = v9 | gdwBitMasks[1];
	//pCompositeUnit->dwFlags = v11;
	//v12 = 0;
	//pCompositeUnit->dwFlags = v11 | gdwBitMasks[2];
	//do
	//{
	//	v10[16] = 1;
	//	if (*v6 && *v6 < 255u)
	//	{
	//		sub_6F8A4020(v12, v6);
	//		*v10 = stru_6F8C1520[*v6].dwItemCode;
	//		v10[16] = stru_6F8C1520[*v6].nItemType;
	//	}
	//	else
	//	{
	//		*v10 = 0;
	//	}
	//	++v6;
	//	++v12;
	//	++v10;
	//}
	//while (v12 < 16);
	//v13 = a5;
	//v14 = pCompositeUnit->unk0x28;
	//do
	//{
	//	v15 = *v13++;
	//	*v14 = v15;
	//	if (v15 != -1)
	//		*v14 = v15 - 1;
	//	++v14;
	//}
	//while ((unsigned int)&v14[-40 - (_DWORD)pCompositeUnit] < 16);
	//if (sub_6F8A3570(pCompositeUnit))
	//{
	//	D2Win_10143(pCompositeUnit, nMode);
	//}
}

//D2Win.0x6F8A43C0 (#10146)
void __stdcall D2Win_10146()
{
	D2CompositeUnitStrc* pCompositeUnit = dword_6F8FD650;
	while (pCompositeUnit)
	{
		D2CompositeUnitStrc* pNext = pCompositeUnit->pNext;
		D2Win_10142_CompUnitDestroy(pCompositeUnit, 0);
		D2_FREE(pCompositeUnit);
		pCompositeUnit = pNext;
	}
	dword_6F8FD650 = nullptr;
}

//D2Win.0x6F8A4400 (#10147)
D2CompositeUnitStrc* __stdcall D2Win_10147()
{
	return dword_6F8FD650;
}

//D2Win.0x6F8A4410 (#10148)
D2CompositeUnitStrc* __stdcall D2Win_10148(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	return pCompositeUnit->pNext;
}

//D2Win.0x6F8A4440 (#10149)
void __stdcall D2Win_10149(D2CompositeUnitStrc* pCompositeUnit, uint8_t a2)
{
	D2_ASSERT(pCompositeUnit);

	pCompositeUnit->unk0x04 = D2Clamp(a2, 0ui8, pCompositeUnit->unk0x05);
}

//D2Win.0x6F8A4490 (#10150)
uint8_t __stdcall D2Win_10150(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	return pCompositeUnit->unk0x05;
}

//D2Win.0x6F8A44C0 (#10169)
int __stdcall D2Win_10169(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	return pCompositeUnit->unk0x0C;
}

//D2Win.0x6F8A44F0 (#10151)
void __stdcall D2Win_10151(D2CompositeUnitStrc* pCompositeUnit, int a2)
{
	D2_ASSERT(pCompositeUnit);

	const int nMax = pCompositeUnit->unk0x0C << 8;
	pCompositeUnit->unk0x08 = D2Clamp(a2, 0, nMax);
}

//D2Win.0x6F8A4540 (#10152)
void __stdcall D2Win_10152(D2CompositeUnitStrc* pCompositeUnit, int nX)
{
	D2_ASSERT(pCompositeUnit);

	pCompositeUnit->nX = nX;
}

//D2Win.0x6F8A4570 (#10153)
void __stdcall D2Win_10153(D2CompositeUnitStrc* pCompositeUnit, int nY)
{
	D2_ASSERT(pCompositeUnit);

	pCompositeUnit->nY = nY;
}

//D2Win.0x6F8A45A0 (#10155)
int __stdcall D2Win_10155(D2CompositeUnitStrc* pCompositeUnit)
{
	D2_ASSERT(pCompositeUnit);

	return pCompositeUnit->dwFlags;
}

//D2Win.0x6F8A45D0 (#10156)
void __stdcall D2Win_10156(D2CompositeUnitStrc* pCompositeUnit, int nFlags)
{
	D2_ASSERT(pCompositeUnit);

	pCompositeUnit->dwFlags = nFlags;
}

//D2Win.0x6F8A4600 (#10158)
void __stdcall D2Win_10158(D2CompositeUnitStrc* pCompositeUnit, uint8_t a2, uint8_t a3, uint8_t a4)
{
	D2_ASSERT(pCompositeUnit);

	if (a2 < 1u || a2 > 32u)
	{
		return;
	}

	char szFile[512] = {};

	wsprintfA(szFile, "%s\\ui\\emblems\\icon%02dc", "DATA\\GLOBAL", a2 - 1);
	pCompositeUnit->pEmblemCellFile = ARCHIVE_LoadCellFile(szFile, 0);

	if (a3 >= 1u && a3 <= 10u)
	{
		pCompositeUnit->unk0xC0 = byte_6F8FD224[a3 - 1];
	}
	else
	{
		pCompositeUnit->unk0xC0 = 0;
	}

	if (a4 >= 1u && a4 <= 10u)
	{
		pCompositeUnit->unk0xC1 = byte_6F8FD218[a4 - 1];
	}
	else
	{
		pCompositeUnit->unk0xC1 = 0;
	}
}

//D2Win.0x6F8A46E0 (#10157)
//TODO: ...
void __stdcall D2Win_10157()
{
//	int v0; // eax@1
//	char v1; // cl@2
//	char *v2; // edi@3
//	signed int v3; // ecx@3
//	bool v4; // zf@5
//	char v5; // cl@6
//	int v6; // edi@6
//	int v7; // edx@6
//	char *v8; // edi@6
//	signed int v9; // ecx@6
//	char v10; // al@9
//	int v11; // edi@9
//	int v12; // eax@12
//	char v13; // cl@13
//	char *v14; // edi@14
//	signed int v15; // ecx@14
//	__int16 v16; // ax@17
//	int v17; // edi@17
//	signed int v18; // ecx@17
//	char v19; // dl@17
//	char *v20; // edi@17
//	int v21; // edi@20
//	int v22; // eax@23
//	char v23; // cl@24
//	char *v24; // edi@25
//	signed int v25; // ecx@25
//	char v26; // dl@25
//	int v27; // edi@28
//	signed int v28; // ecx@28
//	char *v29; // edi@28
//	char v30; // cl@31
//	int v31; // edi@31
//	int v32; // eax@34
//	char v33; // cl@35
//	char *v34; // edi@36
//	signed int v35; // ecx@36
//	int v36; // edx@36
//	__int16 v37; // cx@39
//	int v38; // edi@39
//	char *v39; // edi@39
//	signed int v40; // ecx@39
//	char v41; // al@42
//	int v42; // edi@42
//	int v43; // eax@45
//	char v44; // cl@46
//	char *v45; // edi@47
//	signed int v46; // ecx@47
//	int v47; // eax@50
//	__int16 v48; // cx@50
//	int v49; // edi@50
//	int v50; // edx@50
//	char *v51; // edi@50
//	signed int v52; // ecx@50
//	char v53; // al@53
//	int v54; // edi@53
//	int v55; // eax@56
//	char v56; // cl@57
//	char *v57; // edi@58
//	signed int v58; // ecx@58
//	int v59; // eax@61
//	int v60; // edi@61
//	signed int v61; // ecx@61
//	char v62; // dl@61
//	char *v63; // edi@61
//	int v64; // edi@64
//	int v66; // [sp+0h] [bp-104h]@9
//	char v67[256]; // [sp+4h] [bp-100h]@2
//
//	v0 = 0;
//	do
//	{
//		v1 = byte_6F8BD670[v0];
//		v67[v0++] = v1;
//	}
//	while (v1);
//	v2 = v67;
//	v3 = -1;
//	do
//	{
//		if (!v3)
//			break;
//		v4 = *v2++ == 0;
//		--v3;
//	}
//	while (!v4);
//	v5 = byte_6F8BD66C;
//	v6 = (int)(v2 - 1);
//	v7 = dword_6F8BD660;
//	*(DWORD*)v6 = dword_6F8BD668;
//	*(BYTE*)(v6 + 4) = v5;
//	v8 = v67;
//	v9 = -1;
//	do
//	{
//		if (!v9)
//			break;
//		v4 = *v8++ == 0;
//		--v9;
//	}
//	while (!v4);
//	v10 = byte_6F8BD664;
//	v11 = (int)(v8 - 1);
//	*(DWORD*)v11 = v7;
//	*(BYTE*)(v11 + 4) = v10;
//	if (sub_6F8B2180(0, v67, (int)&v66, 1))
//	{
//		sub_6F8B2251(0, v66, (int)&unk_6F8C8A18, 5376);
//		sub_6F8B21BC(0, v66);
//	}
//	else
//	{
//		Fog_10025(&unk_6F8BD65C, __FILE__, __LINE__);
//		memset(&unk_6F8C8A18, 0, 0x1500u);
//	}
//	v12 = 0;
//	do
//	{
//		v13 = byte_6F8BD670[v12];
//		v67[v12++] = v13;
//	}
//	while (v13);
//	v14 = v67;
//	v15 = -1;
//	do
//	{
//		if (!v15)
//			break;
//		v4 = *v14++ == 0;
//		--v15;
//	}
//	while (!v4);
//	v16 = word_6F8BD658;
//	v17 = (int)(v14 - 1);
//	v18 = -1;
//	*(DWORD*)v17 = dword_6F8BD654;
//	v19 = byte_6F8BD664;
//	*(WORD*)(v17 + 4) = v16;
//	v20 = v67;
//	do
//	{
//		if (!v18)
//			break;
//		v4 = *v20++ == 0;
//		--v18;
//	}
//	while (!v4);
//	v21 = (int)(v20 - 1);
//	*(DWORD*)v21 = dword_6F8BD660;
//	*(BYTE*)(v21 + 4) = v19;
//	if (sub_6F8B2180(0, v67, (int)&v66, 1))
//	{
//		sub_6F8B2251(0, v66, (int)&unk_6F8CF318, 5376);
//		sub_6F8B21BC(0, v66);
//	}
//	else
//	{
//		Fog_10025(&unk_6F8BD65C, __FILE__, __LINE__);
//		memset(&unk_6F8CF318, 0, 0x1500u);
//	}
//	v22 = 0;
//	do
//	{
//		v23 = byte_6F8BD670[v22];
//		v67[v22++] = v23;
//	}
//	while (v23);
//	v24 = v67;
//	v25 = -1;
//	v26 = byte_6F8BD650;
//	do
//	{
//		if (!v25)
//			break;
//		v4 = *v24++ == 0;
//		--v25;
//	}
//	while (!v4);
//	v27 = (int)(v24 - 1);
//	*(DWORD*)v27 = dword_6F8BD64C;
//	v28 = -1;
//	*(BYTE*)(v27 + 4) = v26;
//	v29 = v67;
//	do
//	{
//		if (!v28)
//			break;
//		v4 = *v29++ == 0;
//		--v28;
//	}
//	while (!v4);
//	v30 = byte_6F8BD664;
//	v31 = (int)(v29 - 1);
//	*(DWORD*)v31 = dword_6F8BD660;
//	*(BYTE*)(v31 + 4) = v30;
//	if (sub_6F8B2180(0, v67, (int)&v66, 1))
//	{
//		sub_6F8B2251(0, v66, (int)&unk_6F8D5C18, 5376);
//		sub_6F8B21BC(0, v66);
//	}
//	else
//	{
//		Fog_10025(&unk_6F8BD65C, __FILE__, __LINE__);
//		memset(&unk_6F8D5C18, 0, 0x1500u);
//	}
//	v32 = 0;
//	do
//	{
//		v33 = byte_6F8BD670[v32];
//		v67[v32++] = v33;
//	}
//	while (v33);
//	v34 = v67;
//	v35 = -1;
//	v36 = dword_6F8BD660;
//	do
//	{
//		if (!v35)
//			break;
//		v4 = *v34++ == 0;
//		--v35;
//	}
//	while (!v4);
//	v37 = word_6F8BD648;
//	v38 = (int)(v34 - 1);
//	*(DWORD*)v38 = dword_6F8BD644;
//	*(WORD*)(v38 + 4) = v37;
//	v39 = v67;
//	v40 = -1;
//	do
//	{
//		if (!v40)
//			break;
//		v4 = *v39++ == 0;
//		--v40;
//	}
//	while (!v4);
//	v41 = byte_6F8BD664;
//	v42 = (int)(v39 - 1);
//	*(DWORD*)v42 = v36;
//	*(BYTE*)(v42 + 4) = v41;
//	if (sub_6F8B2180(0, v67, (int)&v66, 1))
//	{
//		sub_6F8B2251(0, v66, (int)&unk_6F8DC518, 5376);
//		sub_6F8B21BC(0, v66);
//	}
//	else
//	{
//		Fog_10025(&unk_6F8BD65C, __FILE__, __LINE__);
//		memset(&unk_6F8DC518, 0, 0x1500u);
//	}
//	v43 = 0;
//	do
//	{
//		v44 = byte_6F8BD670[v43];
//		v67[v43++] = v44;
//	}
//	while (v44);
//	v45 = v67;
//	v46 = -1;
//	do
//	{
//		if (!v46)
//			break;
//		v4 = *v45++ == 0;
//		--v46;
//	}
//	while (!v4);
//	v47 = dword_6F8BD63C;
//	v48 = word_6F8BD640;
//	v49 = (int)(v45 - 1);
//	*(DWORD*)v49 = dword_6F8BD638;
//	v50 = dword_6F8BD660;
//	*(DWORD*)(v49 + 4) = v47;
//	*(WORD*)(v49 + 8) = v48;
//	v51 = v67;
//	v52 = -1;
//	do
//	{
//		if (!v52)
//			break;
//		v4 = *v51++ == 0;
//		--v52;
//	}
//	while (!v4);
//	v53 = byte_6F8BD664;
//	v54 = (int)(v51 - 1);
//	*(DWORD*)v54 = v50;
//	*(BYTE*)(v54 + 4) = v53;
//	if (sub_6F8B2180(0, v67, (int)&v66, 1))
//	{
//		sub_6F8B2251(0, v66, (int)&unk_6F8E2E18, 5376);
//		sub_6F8B21BC(0, v66);
//	}
//	else
//	{
//		Fog_10025(&unk_6F8BD65C, __FILE__, __LINE__);
//		memset(&unk_6F8E2E18, 0, 0x1500u);
//	}
//	v55 = 0;
//	do
//	{
//		v56 = byte_6F8BD670[v55];
//		v67[v55++] = v56;
//	}
//	while (v56);
//	v57 = v67;
//	v58 = -1;
//	do
//	{
//		if (!v58)
//			break;
//		v4 = *v57++ == 0;
//		--v58;
//	}
//	while (!v4);
//	v59 = dword_6F8BD634;
//	v60 = (int)(v57 - 1);
//	v61 = -1;
//	*(DWORD*)v60 = dword_6F8BD630;
//	v62 = byte_6F8BD664;
//	*(DWORD*)(v60 + 4) = v59;
//	v63 = v67;
//	do
//	{
//		if (!v61)
//			break;
//		v4 = *v63++ == 0;
//		--v61;
//	}
//	while (!v4);
//	v64 = (int)(v63 - 1);
//	*(DWORD*)v64 = dword_6F8BD660;
//	*(BYTE*)(v64 + 4) = v62;
//	if (sub_6F8B2180(0, v67, (int)&v66, 1))
//	{
//		sub_6F8B2251(0, v66, (int)&unk_6F8E9718, 5376);
//		sub_6F8B21BC(0, v66);
//	}
//	else
//	{
//		Fog_10025(&unk_6F8BD65C, __FILE__, __LINE__);
//		memset(&unk_6F8E9718, 0, 0x1500u);
//	}
//	sub_6F8A4B10("invgrey2", 7);
//	return sub_6F8A4B10("invgreybrown", 8);
}

////D2Win.0x6F8A4B10) --------------------------------------------------------
//int __fastcall sub_6F8A4B10(int a1, int a2)
//{
//	int v2; // eax@1
//	int v3; // ebx@1
//	char v4; // dl@2
//	int v5; // edi@3
//	signed int v6; // ecx@3
//	bool v7; // zf@5
//	signed int v8; // ecx@6
//	const void *v9; // esi@6
//	unsigned int v10; // ebp@6
//	char *v11; // edi@6
//	signed int v12; // ecx@6
//	char *v13; // edi@9
//	signed int v14; // ecx@9
//	char v15; // cl@12
//	int v16; // edi@12
//	int result; // eax@13
//	int v18; // [sp+8h] [bp-104h]@12
//	char v19[256]; // [sp+Ch] [bp-100h]@2
//
//	v2 = 0;
//	v3 = a2;
//	do
//	{
//		v4 = byte_6F8BD670[v2];
//		v19[v2++] = v4;
//	}
//	while (v4);
//	v5 = a1;
//	v6 = -1;
//	do
//	{
//		if (!v6)
//			break;
//		v7 = *(BYTE*)v5++ == 0;
//		--v6;
//	}
//	while (!v7);
//	v8 = ~v6;
//	v9 = (const void*)(v5 - v8);
//	v10 = v8;
//	v11 = v19;
//	v12 = -1;
//	do
//	{
//		if (!v12)
//			break;
//		v7 = *v11++ == 0;
//		--v12;
//	}
//	while (!v7);
//	memcpy(v11 - 1, v9, v10);
//	v13 = v19;
//	v14 = -1;
//	do
//	{
//		if (!v14)
//			break;
//		v7 = *v13++ == 0;
//		--v14;
//	}
//	while (!v7);
//	v15 = byte_6F8BD664;
//	v16 = (int)(v13 - 1);
//	*(DWORD*)v16 = dword_6F8BD660;
//	*(BYTE*)(v16 + 4) = v15;
//	if (sub_6F8B2180(0, v19, (int)&v18, 1))
//	{
//		sub_6F8B2251(0, v18, (int)((char*)&unk_6F8C2118 + 26880 * v3), 5376);
//		result = sub_6F8B21BC(0, v18);
//	}
//	else
//	{
//		Fog_10025(&unk_6F8BD65C, __FILE__, __LINE__);
//		result = 0;
//		memset((char*)&unk_6F8C2118 + 26880 * v3, 0, 0x1500u);
//	}
//	return result;
//}

//D2Win.0x6F8A4C10
void __stdcall sub_6F8A4C10()
{
	constexpr uint8_t byte_6F8BC194[] =
	{
		0xBC, 0x00, 0x00,
		0xE0, 0x70, 0x20,
		0xFC, 0xD4, 0x5C,
		0xFC, 0xE4, 0xA4,
		0xF4, 0xF4, 0xF4,
		0xB8, 0xB8, 0xB8,
		0x74, 0x74, 0x74,
		0x4C, 0x6C, 0xAC,
		0x58, 0x90, 0x4C,
		0xB2, 0x7C, 0x58,
	};

	for (int32_t i = 0; i < std::size(byte_6F8FD224); ++i)
	{
		byte_6F8FD224[i] = D2Win_10034_MixRGB(byte_6F8BC194[3 * i], byte_6F8BC194[3 * i + 1], byte_6F8BC194[3 * i + 2]);
	}

	constexpr uint8_t byte_6F8BC1B4[] =
	{
		0x5C, 0x00, 0x00,
		0x90, 0x00, 0x00,
		0x30, 0x1C, 0x10,
		0x5C, 0x24, 0x08,
		0x04, 0x24, 0x10,
		0x24, 0x5C, 0x24,
		0x00, 0x00, 0x58,
		0x28, 0x28, 0x78,
		0x40, 0x40, 0x40,
		0x00, 0x00, 0x00,
	};

	for (int32_t i = 0; i < std::size(byte_6F8FD218); ++i)
	{
		byte_6F8FD218[i] = D2Win_10034_MixRGB(byte_6F8BC1B4[3 * i], byte_6F8BC1B4[3 * i + 1], byte_6F8BC1B4[3 * i + 2]);
	}
}

//D2Win.0x6F8A4C70 (#10161)
const char* __stdcall D2Win_10161(int a1, int a2, int a3, BOOL bSoftCore)
{
	int v4 = 0;

	if (bSoftCore)
	{
		if (a2 >= 5)
		{
			if (a2 < 10)
			{
				v4 = 1;
			}
			else if (a2 < 15)
			{
				v4 = 2;
			}
			else
			{
				v4 = 3;
			}
		}
	}
	else
	{
		if (a2 >= 4)
		{
			if (a2 < 8)
			{
				v4 = 1;
			}
			else if (a2 < 12)
			{
				v4 = 2;
			}
			else
			{
				v4 = 3;
			}
		}
	}

	if (a3 && v4)
	{
		v4 += 3;
	}

	return D2Win_10160_GetPlayerTitle(a1, v4, bSoftCore);
}

//D2Win.0x6F8A4CE0 (#10160)
const char* __stdcall D2Win_10160_GetPlayerTitle(int a1, int a2, BOOL bSoftCore)
{
	if (bSoftCore)
	{
		switch (a1)
		{
		case 0:
		case 1:
		case 6:
		case 9:
			switch (a2)
			{
			case 1:
				return "Slayer ";
			case 2:
				return "Champion ";
			case 3:
				return "Matriarch ";
			case 4:
				return "Destroyer ";
			case 5:
				return "Conqueror ";
			case 6:
				return "Guardian ";
			default:
				break;
			}
			break;

		default:
			switch (a2)
			{
			case 1:
				return "Slayer ";
			case 2:
				return "Champion ";
			case 3:
				return "Patriarch ";
			case 4:
				return "Destroyer ";
			case 5:
				return "Conqueror ";
			case 6:
				return "Guardian ";
			default:
				break;
			}
			break;
		}
	}
	else
	{
		switch (a1)
		{
		case 0:
		case 1:
		case 6:
		case 9:
			switch (a2)
			{
			case 1:
				return "Dame ";
			case 2:
				return "Lady ";
			case 3:
				return "Baroness ";
			case 4:
				return "Countess ";
			case 5:
				return "Duchess ";
			case 6:
				return "Queen ";
			default:
				break;
			}
			break;

		default:
			switch (a2)
			{
			case 1:
				return "Sir ";
			case 2:
				return "Lord ";
			case 3:
				return "Baron ";
			case 4:
				return "Count ";
			case 5:
				return "Duke ";
			case 6:
				return "King ";
			default:
				break;
			}
			break;
		}
	}

	return byte_6F8FD678;
}
