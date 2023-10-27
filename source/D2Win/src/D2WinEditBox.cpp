#include "D2WinEditBox.h"

#include <algorithm>

#include <D2Math.h>

#include "D2CMP.h"
#include "D2Gfx.h"
#include "D2Lang.h"
#include "Fog.h"

#include <Texture.h>

#include <D2BitManip.h>

#include "D2WinFont.h"
#include "D2WinMain.h"
#include "D2WinPalette.h"

#pragma warning (disable : 28159)


D2WinEditBoxStrc* dword_6F8FD8B0;
int dword_6F8FD8B4;


//D2Win.0x6F8A6C80 (#10070)
D2WinEditBoxStrc* __fastcall D2Win_10070_EDITBOX_Create(int32_t nX, int32_t nY, int32_t nWidth, int32_t nHeight, int32_t a5, int32_t a6, D2CellFileStrc* pCellFile, int32_t a8, int32_t(__stdcall* a9)(SMSGHANDLER_PARAMS*), int32_t nEditBoxFlags, int32_t a11)
{
	D2WinEditBoxStrc* pEditBox = D2_CALLOC_STRC(D2WinEditBoxStrc);

	pEditBox->controlHeader.nType = D2WIN_EDITBOX;
	pEditBox->controlHeader.dwFlags |= gdwBitMasks[0] | gdwBitMasks[2];
	pEditBox->controlHeader.nImageX = nX;
	pEditBox->controlHeader.nImageY = nY;
	pEditBox->controlHeader.nWidth = nWidth;
	pEditBox->controlHeader.nHeight = nHeight;
	pEditBox->controlHeader.pCellFile = pCellFile;
	pEditBox->field_40 = a5;
	pEditBox->field_44 = a6;
	pEditBox->field_48 = 256;
	pEditBox->field_54 = -1;

	if (!(nEditBoxFlags & 8))
	{
		pEditBox->controlHeader.pfDraw = sub_6F8A82D0;
		pEditBox->controlHeader.pfHandleMouseDown = sub_6F8A9120;
	}
	else
	{
		pEditBox->controlHeader.pfDraw = sub_6F8A86C0;
		pEditBox->controlHeader.pfHandleMouseDown = sub_6F8A97D0;
	}

	pEditBox->controlHeader.pfHandleMouseUp = EDITBOX_HandleMouseUp;
	pEditBox->field_264 = a8;
	pEditBox->controlHeader.field_34 = a9;
	pEditBox->dwEditBoxFlags = nEditBoxFlags;

	if (a11)
	{
		pEditBox->eFont = *(Font*)a11;
		pEditBox->nTextColor = *(DWORD*)(a11 + 4);
	}
	else
	{
		pEditBox->eFont = D2FONT_FONT16;
		pEditBox->nTextColor = 0;
	}

	CONTROL_AppendToList(&pEditBox->controlHeader);

	return pEditBox;
}

//D2Win.0x6F8A6DC0 (#10071)
int32_t __fastcall D2Win_10071_EDITBOX_Destroy(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	return CONTROL_RemoveFromList(&pEditBox->controlHeader);
}

//D2Win.0x6F8A6E10 (#10072)
void __fastcall D2Win_10072(D2WinEditBoxStrc* pEditBox, int32_t a2)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_268 = a2;
}

//D2Win.0x6F8A6E60 (#10073)
void __fastcall D2Win_10073(D2WinEditBoxStrc* pEditBox, void(__stdcall* a2)(int32_t))
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_26C = a2;
}

//D2Win.0x6F8A6EB0 (#10083)
void __fastcall D2Win_10083(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_54 = -1;
	pEditBox->nTextLength = 0;
}

//D2Win.0x6F8A6F10 (#10082)
int32_t __fastcall D2Win_10082(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_54 = 0;
	pEditBox->nTextLength = Unicode::strlen(pEditBox->wszText);

	return pEditBox->nTextLength;
}

//D2Win.0x6F8A6F70) --------------------------------------------------------
int32_t __fastcall D2Win_10074(D2WinEditBoxStrc* pEditBox, uint8_t a2)
{
//	D2WinEditBoxStrc* v2; // ebx@1
//	int v3; // eax@7
//	unsigned __int16 v5; // si@14
//	bool v6; // zf@14
//	char v7; // al@14
//	int v8; // edx@15
//	int v9; // ecx@26
//	int v10; // ecx@35
//	char *v11; // esi@41
//	signed int v12; // edi@41
//	char *v13; // esi@43
//	const char *v14; // edi@45
//	HWND v15; // eax@46
//	HANDLE v16; // eax@48
//	void *v17; // esi@48
//	const char *v18; // eax@49
//	unsigned __int8 *v19; // esi@54
//	unsigned int v20; // kr0C_4@54
//	int v21; // ebp@54
//	Unicode *v22; // esi@59
//	signed int v23; // edi@59
//	char *v24; // esi@61
//	signed int v25; // edi@61
//	char *v26; // esi@63
//	signed int v27; // edi@63
//	int v28; // eax@65
//	int v29; // ebp@66
//	int v30; // esi@66
//	int v31; // ecx@67
//	char *v32; // edx@70
//	const char *v33; // eax@71
//	char *v34; // edx@71
//	const char v35; // cl@72
//	const char *v36; // ecx@74
//	const char i; // al@74
//	unsigned int v38; // ecx@83
//	int v39; // eax@87
//	int v40; // ecx@88
//	int v41; // edx@89
//	HWND v42; // eax@93
//	SIZE_T v43; // kr24_4@94
//	unsigned int v44; // ebp@94
//	HGLOBAL v45; // eax@94
//	void *v46; // eax@96
//	int v47; // eax@100
//	int v48; // ecx@101
//	int v49; // edx@102
//	HWND v50; // eax@106
//	SIZE_T v51; // kr2C_4@107
//	unsigned int v52; // ebp@107
//	HGLOBAL v53; // eax@107
//	void *v54; // eax@108
//	int v55; // eax@120
//	int v56; // eax@129
//	char *v57; // esi@129
//	signed int v58; // edi@129
//	int v59; // ebp@129
//	Unicode *v60; // esi@131
//	signed int v61; // edi@131
//	char *v62; // edi@133
//	char *v63; // esi@133
//	int v64; // eax@133
//	int v65; // ecx@133
//	int v66; // ebp@134
//	char v67; // al@134
//	int v68; // eax@137
//	const char *v69; // eax@141
//	char v70; // dl@146
//	char v71; // dl@148
//	int v72; // ebx@153
//	int v73; // [sp+34h] [bp-BBCh]@1
//	const char *v74; // [sp+38h] [bp-BB8h]@52
//	char *v75; // [sp+3Ch] [bp-BB4h]@43
//	int v76; // [sp+40h] [bp-BB0h]@55
//	int v77; // [sp+44h] [bp-BACh]@41
//	char v78; // [sp+48h] [bp-BA8h]@129
//	int v79; // [sp+4Ch] [bp-BA4h]@134
//	int v80; // [sp+50h] [bp-BA0h]@131
//	int v81; // [sp+54h] [bp-B9Ch]@129
//	int v82; // [sp+58h] [bp-B98h]@125
//	int v83; // [sp+5Ch] [bp-B94h]@133
//	char v84[400]; // [sp+60h] [bp-B90h]@70
//	char v85; // [sp+1F0h] [bp-A00h]@61
//	char v86; // [sp+1F1h] [bp-9FFh]@118
//	char v87; // [sp+1F2h] [bp-9FEh]@118
//	char v88[1024]; // [sp+3F0h] [bp-800h]@41
//	Unicode v89[256]; // [sp+7F0h] [bp-400h]@59
//	char v90; // [sp+9F0h] [bp-200h]@63
//
//	v2 = pEditBox;
//	LOBYTE(v73) = a2;
//	D2_ASSERT(pEditBox);
//	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);
//
//	v3 = pEditBox->controlHeader.dwFlags;
//	if (!(v3 & gdwBitMasks[2]) || !(v3 & gdwBitMasks[0]))
//		return 0;
//	if (a2 == 9 && pEditBox->dwEditBoxFlags & 4)
//	{
//		if (pEditBox->field_268)
//		{
//			++dword_6F8FE254;
//			((void(__stdcall *)(D2WinEditBoxStrc* , int, int *))pEditBox->field_268)(pEditBox, pEditBox->field_25C, &v73);
//			--dword_6F8FE254;
//		}
//		return 0;
//	}
//	v5 = a2;
//	v6 = D2Win_10128(a2) == 0;
//	v7 = v73;
//	if (v6
//		&& ((v8 = v2->dwEditBoxFlags, !(v8 & 0x20)) || v5 >= 0x100u || v5 != 255)
//		&& (!(v8 & 8) || (BYTE)v73 != 13)
//		&& (!(v8 & 0x10) || (BYTE)v73 != 14)
//		&& (BYTE)v73 != 22
//		&& (BYTE)v73 != 3
//		&& (BYTE)v73 != 24
//		&& (BYTE)v73 != 1
//		|| (v9 = v2->field_268) != 0
//		&& (v6 = ((int(__stdcall *)(D2WinEditBoxStrc* , int, int *))v9)(v2, v2->field_25C, &v73) == 0, v7 = v73, v6)
//		&& (BYTE)v73 != 22
//		&& (BYTE)v73 != 3
//		&& (BYTE)v73 != 24
//		&& (BYTE)v73 != 1)
//		return 0;
//	if (v2->controlHeader.nType != 1)
//	{
//		FOG_DisplayAssert("ptEditData->eType == D2WIN_EDITBOX", __FILE__, __LINE__);
//		exit(-1);
//	}
//	v10 = v2->field_54;
//	if (v10 != -1 && v10 != v2->nTextLength && v7 != 3 && v7 != 24 && v7 != 1)
//		sub_6F8A7970(v2);
//	v77 = Unicode::strlen(v2->wszText);
//	v11 = v88;
//	v12 = 512;
//	do
//	{
//		D2LANG__default_constructor_closure_(v11);
//		v11 += 2;
//		--v12;
//	}
//	while (v12);
//	Unicode::strcpy(v88, v2->wszText);
//	v13 = &v88[2 * v2->field_25C];
//	v75 = &v88[2 * v2->field_25C];
//	switch ((char)v73)
//	{
//	case WM_ENDSESSION:
//		if (IsClipboardFormatAvailable(1u))
//		{
//			v15 = D2GFX_10027_GetWindow();
//			if (OpenClipboard(v15))
//			{
//				v16 = GetClipboardData(1u);
//				v17 = v16;
//				if (v16)
//				{
//					v18 = (const char *)GlobalLock(v16);
//					if (v18)
//					{
//						strncpy(Dest, v18, 0x200u);
//						GlobalUnlock(v17);
//					}
//				}
//				CloseClipboard();
//				v14 = Dest;
//			}
//			else
//			{
//				v14 = (const char *)&unk_6F8FD678;
//			}
//		}
//		else
//		{
//			v14 = (const char *)&unk_6F8FD678;
//		}
//		v74 = v14;
//		if ((signed int)(v77 + strlen(v14)) > v2->field_48 - 1)
//			return 0;
//		if (v2->field_268)
//		{
//			v19 = (unsigned __int8 *)v74;
//			v20 = strlen(v74) + 1;
//			v21 = 0;
//			if ((signed int)(v20 - 1) > 0)
//			{
//				v76 = v2->field_25C - (DWORD)v74;
//				do
//				{
//					if (!D2Win_10128(*v19)
//						|| !((int(__stdcall *)(D2WinEditBoxStrc* , unsigned __int8 *, unsigned __int8 *))v2->field_268)(v2, &v19[v76], v19))
//						return 0;
//					++v21;
//					++v19;
//				}
//				while (v21 < (signed int)(v20 - 1));
//			}
//		}
//		v22 = v89;
//		v23 = 256;
//		do
//		{
//			D2LANG__default_constructor_closure_(v22);
//			++v22;
//			--v23;
//		}
//		while (v23);
//		v24 = &v85;
//		v25 = 256;
//		do
//		{
//			D2LANG__default_constructor_closure_(v24);
//			v24 += 2;
//			--v25;
//		}
//		while (v25);
//		v26 = &v90;
//		v27 = 256;
//		do
//		{
//			D2LANG__default_constructor_closure_(v26);
//			v26 += 2;
//			--v27;
//		}
//		while (v27);
//		v28 = v2->field_54;
//		if (v28 == -1)
//		{
//			v29 = v2->field_25C;
//			v30 = v2->field_25C;
//		}
//		else
//		{
//			v31 = v2->nTextLength;
//			v29 = v2->field_54;
//			if (v28 < v31 || (v29 = v2->nTextLength, v30 = v2->field_54, v28 <= v31))
//				v30 = v2->nTextLength;
//		}
//		v32 = v84;
//		if (v2->dwEditBoxFlags & 8)
//		{
//			v33 = v74;
//			v34 = (char *)(v84 - v74);
//			do
//			{
//				v35 = *v33;
//				v33[(DWORD)v34] = *v33;
//				++v33;
//			}
//			while (v35);
//		}
//		else
//		{
//			v36 = v74;
//			for (i = *v74; i; ++v36)
//			{
//				if (i != 13 && i != 10)
//					*v32++ = i;
//				i = v36[1];
//			}
//			*v32 = 0;
//		}
//		Unicode::strncpy(v89, v88, v29);
//		Unicode::strcpy(&v90, &v88[2 * v30]);
//		Unicode::win2Unicode(&v85, v84, strlen(v84) + 1);
//		Unicode::strcpy(v88, v89);
//		Unicode::strcat(v88, &v85);
//		Unicode::strcat(v88, &v90);
//		if (v2->controlHeader.nType != 1)
//		{
//			FOG_DisplayAssert("ptEditData->eType == D2WIN_EDITBOX", __FILE__, __LINE__);
//			exit(-1);
//		}
//		v2->field_54 = -1;
//		v38 = v29 + strlen(v84);
//		v13 = &v88[2 * v38];
//		v75 = &v88[2 * v38];
//		goto LABEL_124;
//	case WM_MOVE:
//		if (v2->controlHeader.nType != 1)
//		{
//			FOG_DisplayAssert("ptEditData->eType == D2WIN_EDITBOX", __FILE__, __LINE__);
//			exit(-1);
//		}
//		v39 = v2->field_54;
//		if (v39 != -1)
//		{
//			v40 = v2->nTextLength;
//			if (v39 != v40)
//			{
//				v41 = v2->field_54;
//				if (v39 >= v40)
//					v41 = v2->nTextLength;
//				D2LANG_unicode2Win(v84, &v88[2 * v41]);
//				if (hMem)
//				{
//					GlobalFree(hMem);
//					hMem = 0;
//				}
//				v42 = D2GFX_10027_GetWindow();
//				if (OpenClipboard(v42))
//				{
//					EmptyClipboard();
//					v43 = strlen(v84) + 1;
//					v44 = v43 - 1;
//					v45 = GlobalAlloc(0x2000u, v43);
//					hMem = v45;
//					if (v45)
//					{
//						v46 = GlobalLock(v45);
//						memcpy(v46, v84, v44);
//						*((BYTE *)v46 + v44) = 0;
//						GlobalUnlock(hMem);
//						SetClipboardData(1u, hMem);
//						CloseClipboard();
//						v13 = v75;
//					}
//					else
//					{
//						CloseClipboard();
//						v13 = v75;
//					}
//				}
//			}
//		}
//		break;
//	case WM_SHOWWINDOW:
//		if (v2->controlHeader.nType != 1)
//		{
//			FOG_DisplayAssert("ptEditData->eType == D2WIN_EDITBOX", __FILE__, __LINE__);
//			exit(-1);
//		}
//		v47 = v2->field_54;
//		if (v47 != -1)
//		{
//			v48 = v2->nTextLength;
//			if (v47 != v48)
//			{
//				v49 = v2->field_54;
//				if (v47 >= v48)
//					v49 = v2->nTextLength;
//				v76 = (int)&v88[2 * v49];
//				D2LANG_unicode2Win(v84, v76);
//				if (hMem)
//				{
//					GlobalFree(hMem);
//					hMem = 0;
//				}
//				v50 = D2GFX_10027_GetWindow();
//				if (OpenClipboard(v50))
//				{
//					EmptyClipboard();
//					v51 = strlen(v84) + 1;
//					v52 = v51 - 1;
//					v53 = GlobalAlloc(0x2000u, v51);
//					hMem = v53;
//					if (v53)
//					{
//						v54 = GlobalLock(v53);
//						memcpy(v54, v84, v52);
//						*((BYTE *)v54 + v52) = 0;
//						GlobalUnlock(hMem);
//						SetClipboardData(1u, hMem);
//					}
//					CloseClipboard();
//				}
//				sub_6F8A7970(v2);
//				Unicode::strlen(v2->wszText);
//				Unicode::strcpy(v88, v2->wszText);
//				v75 = (char *)v76;
//				v13 = (char *)v76;
//			}
//		}
//		break;
//	case WM_CREATE:
//		if (v2->controlHeader.nType != 1)
//		{
//			FOG_DisplayAssert("ptEditData->eType == D2WIN_EDITBOX", __FILE__, __LINE__);
//			exit(-1);
//		}
//		v2->field_54 = 0;
//		v2->nTextLength = Unicode::strlen(v2->wszText);
//		break;
//	default:
//		if (v77 >= v2->field_48 - 1)
//			return 0;
//		if (v77 < 255)
//		{
//			if ((BYTE)v73 == 13)
//			{
//				D2LANG_unicode2Win(&v87, v13);
//				v85 = 13;
//				v86 = 10;
//				Unicode::win2Unicode(v13, &v85, 256);
//				v13 += 4;
//			}
//			else
//			{
//				D2LANG_unicode2Win(&v86, v13);
//				v85 = v73;
//				Unicode::win2Unicode(v13, &v85, 256);
//				v13 += 2;
//			}
//			v55 = v2->controlHeader.nType;
//			v75 = v13;
//			if (v55 != 1)
//			{
//				FOG_DisplayAssert("ptEditData->eType == D2WIN_EDITBOX", __FILE__, __LINE__);
//				exit(-1);
//			}
//			v2->field_54 = -1;
//LABEL_124:
//			v2->nTextLength = 0;
//		}
//		break;
//	}
//	v82 = v2->controlHeader.nHeight - 2 * v2->field_44;
//	if (v2->dwEditBoxFlags & 8)
//	{
//		if (v2->controlHeader.nType != 1)
//		{
//			FOG_DisplayAssert("ptEditData->eType == D2WIN_EDITBOX", __FILE__, __LINE__);
//			exit(-1);
//		}
//		D2Win_10127_SetFont(v2->nFont);
//		v56 = D2Win_10125();
//		v57 = &v78;
//		v58 = 2;
//		v59 = v2->field_44 + v2->controlHeader.nImageY - v2->controlHeader.nHeight + v56;
//		v81 = v2->field_44 + v2->controlHeader.nImageY - v2->controlHeader.nHeight + v56;
//		LOWORD(v77) = word_6F8BDADC;
//		do
//		{
//			D2LANG__default_constructor_closure_(v57);
//			v57 += 2;
//			--v58;
//		}
//		while (v58);
//		Unicode::win2Unicode(&v78, &v77, 2);
//		v80 = D2Win_10122(&v78, 2);
//		v60 = v89;
//		v61 = 256;
//		do
//		{
//			D2LANG__default_constructor_closure_(v60);
//			++v60;
//			--v61;
//		}
//		while (v61);
//		D2LANG_unicode2Win(&v85, v88);
//		v62 = &v85;
//		v63 = &v85;
//		v74 = (const char *)v59;
//		v64 = D2Win_10125();
//		v65 = v2->field_40;
//		v76 = v64;
//		v83 = v2->controlHeader.nWidth - 2 * v65;
//		if (v85)
//		{
//			do
//			{
//				v66 = v63 - v62;
//				v79 = 0;
//				memset(v84, 0, 0x100u);
//				v67 = *v63;
//				if (*v63)
//				{
//					while (v67 != '\r' && v67 != '\n')
//					{
//						v84[v66] = v67;
//						Unicode::win2Unicode(v89, v84, 256);
//						v68 = D2Win_10122((int)v89, v66);
//						if (v80 + v68 > v83)
//						{
//							v79 = 1;
//							break;
//						}
//						v67 = (v63++)[1];
//						++v66;
//						if (!v67)
//							break;
//					}
//				}
//				Unicode::win2Unicode(v89, v84, 256);
//				v69 = v74;
//				if (v79)
//				{
//					v69 = &v74[v76];
//					v74 += v76;
//				}
//				if (*v63 == '\r')
//				{
//					if (v63[1] == '\n')
//					{
//						do
//						{
//							if (v63[1] != '\n')
//								break;
//							v70 = v63[2];
//							v63 += 2;
//							v69 += v76;
//						}
//						while (v70 == '\r');
//					}
//					else
//					{
//						do
//						{
//							v71 = (v63++)[1];
//							v69 += v76;
//						}
//						while (v71 == '\r');
//					}
//					v74 = v69;
//				}
//				v62 = v63;
//			}
//			while (*v63);
//		}
//		if ((signed int)&v74[-v81] >= v82)
//			return 1;
//		v13 = v75;
//	}
//	Unicode::strcpy(v2->wszText, v88);
//	v2->field_25C = (signed int)(v13 - v88) >> 1;
//	sub_6F8A98A0(v2);
//	v72 = v2->field_26C;
//	if (v72)
//		((void(__stdcall *)(DWORD))v72)(0);
	return 1;
}

//D2Win.0x6F8A7970
void __fastcall sub_6F8A7970(D2WinEditBoxStrc* pEditBox)
{
	// TODO: Names
	Unicode v11[256] = {};
	Unicode v12[256] = {};

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	int32_t v6 = pEditBox->nTextLength;
	int32_t v7 = pEditBox->field_54;
	int32_t v8 = pEditBox->field_54;

	int32_t v9;

	if (v7 < v6 || (v8 = pEditBox->nTextLength, v7 <= v6))
	{
		v9 = pEditBox->nTextLength;
	}
	else
	{
		v9 = pEditBox->field_54;
	}

	if (v8)
	{
		Unicode::strncpy(v11, pEditBox->wszText, v8);
	}
	else
	{
		v11[0] = 0;
	}

	Unicode::strcpy(v12, &pEditBox->wszText[v9]);
	Unicode::strcpy(pEditBox->wszText, v11);
	Unicode::strcat(pEditBox->wszText, v12);

	int32_t v10 = Unicode::strlen(pEditBox->wszText);
	if (v8 > v10)
	{
		pEditBox->field_25C = v10;
	}
	else
	{
		pEditBox->field_25C = v8;
	}

	D2Win_10083(pEditBox);
}

//D2Win.0x6F8A7AB0) --------------------------------------------------------
int32_t __fastcall sub_6F8A7AB0(D2WinEditBoxStrc* pEditBox, char a2)
{
//	unsigned int v2; // ebp@0
//	D2WinEditBoxStrc* v3; // esi@1
//	int v4; // eax@7
//	int v5; // eax@9
//	signed int v6; // ebx@9
//	Unicode *v7; // edi@9
//	int v8; // ebp@9
//	int v9; // eax@13
//	int v10; // edx@15
//	int v11; // eax@18
//	int v12; // eax@27
//	int v13; // esi@31
//	int result; // eax@33
//	bool v15; // zf@36
//	int v16; // eax@38
//	void *v17; // edx@38
//	int v18; // eax@40
//	Unicode v19; // ax@54
//	size_t v20; // ebx@63
//	int v21; // ebp@68
//	int v22; // edi@74
//	D2WinEditBoxStrc* v23; // edi@80
//	int v24; // eax@82
//	int v25; // eax@88
//	const void *v26; // [sp-8h] [bp-48h]@65
//	int v27; // [sp-4h] [bp-44h]@38
//	size_t v28; // [sp-4h] [bp-44h]@65
//	char v29; // [sp+10h] [bp-30h]@1
//	int v30; // [sp+14h] [bp-2Ch]@9
//	int a2a; // [sp+18h] [bp-28h]@38
//	SMSGHANDLER_PARAMS a1; // [sp+1Ch] [bp-24h]@38
//
//	v3 = pEditBox;
//	v29 = a2;
//	D2_ASSERT(pEditBox);
//	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);
//
//	v4 = pEditBox->controlHeader.dwFlags;
//	if (v4 & gdwBitMasks[2] && v4 & gdwBitMasks[0])
//	{
//		v5 = pEditBox->field_25C;
//		v6 = 0;
//		v30 = 0;
//		v7 = &pEditBox->wszText[v5];
//		LOWORD(v2) = GetKeyState(VK_SHIFT);
//		v8 = (v2 >> 15) & 1;
//		switch (v29)
//		{
//		case '\b':
//		case '.':
//			if (v3->controlHeader.nType != 1)
//			{
//				FOG_DisplayAssert("ptEditData->eType == D2WIN_EDITBOX", __FILE__, __LINE__);
//				exit(-1);
//			}
//			v9 = v3->field_54;
//			if (v9 != -1 && v9 != v3->nTextLength)
//			{
//				sub_6F8A7970(v3);
//				v10 = v3->field_25C;
//				v6 = 1;
//				v30 = 1;
//				v7 = &v3->wszText[v10];
//			}
//			break;
//		case '#':
//		case '$':
//		case '%':
//		case '&':
//		case '\'':
//		case '(':
//			if (v8)
//			{
//				if (v3->field_54 == -1)
//				{
//					v11 = v3->field_25C;
//					v3->field_54 = v11;
//					v3->nTextLength = v11;
//				}
//			}
//			else
//			{
//				D2Win_10083(v3);
//			}
//			break;
//		default:
//			break;
//		}
//		switch (v29)
//		{
//		case 36:
//			goto LABEL_24;
//		default:
//			goto LABEL_26;
//		case 35:
//			for (; *v7; ++v7)
//				;
//			v15 = v8 == 0;
//			goto LABEL_59;
//		case 38:
//			if (v3->dwEditBoxFlags & 8)
//			{
//				sub_6F8A8D60(v3, (int *)&a1, &a2a);
//				v16 = D2Win_10125();
//				v17 = a1.hWindow;
//				v27 = a2a - v16;
//				goto LABEL_41;
//			}
//LABEL_24:
//			v7 = v3->wszText;
//			if (v8)
//				v3->nTextLength = 0;
//			goto LABEL_26;
//		case 40:
//			if (v3->dwEditBoxFlags & 8)
//			{
//				sub_6F8A8D60(v3, &a2a, (int *)&a1);
//				v18 = D2Win_10125();
//				v17 = (void *)a2a;
//				v27 = (int)((char *)a1.hWindow + v18);
//LABEL_41:
//				a1.uMessage = (UINT)v3;
//				a1.wParam = 0;
//				sub_6F8A9410((SMSGHANDLER_PARAMS*)((char *)&a1 + 4), (int)v17, v27);
//				v7 = &v3->wszText[v3->field_25C];
//			}
//			else
//			{
//				for (; *v7; ++v7)
//					;
//				if (v8)
//					v3->nTextLength = (signed int)((char *)v7 - (char *)v3 - 92) >> 1;
//			}
//LABEL_26:
//			v3->field_25C = (signed int)((char *)v7 - (char *)v3 - 92) >> 1;
//			sub_6F8A98A0(v3);
//			if (v30)
//			{
//				v12 = v3->field_268;
//				if (v12)
//				{
//					if (!((int(__stdcall *)(D2WinEditBoxStrc* , int, char *))v12)(v3, v3->field_25C, &v29)
//						&& v29 != 8
//						&& v29 != 46)
//						goto LABEL_99;
//				}
//			}
//			v13 = v3->field_26C;
//			if (v13)
//				((void(__stdcall *)(DWORD))v13)(0);
//			return 1;
//		case 37:
//			--v7;
//			if (*v7 == 10)
//				--v7;
//			if ((unsigned int)v7 >= (unsigned int)v3->wszText)
//			{
//				if (v8 && v3->field_54 != -1)
//					v3->nTextLength = (signed int)((char *)v7 - (char *)v3 - 92) >> 1;
//			}
//			else
//			{
//				v7 = v3->wszText;
//			}
//			goto LABEL_26;
//		case 39:
//			if (*v7)
//			{
//				v19 = v7[1];
//				++v7;
//				if (v19 && v19 == 10)
//					++v7;
//				if (v8)
//				{
//					v15 = v3->field_54 == -1;
//LABEL_59:
//					if (!v15)
//						v3->nTextLength = (signed int)((char *)v7 - (char *)v3 - 92) >> 1;
//				}
//			}
//			goto LABEL_26;
//		case 8:
//			if ((unsigned int)v7 <= (unsigned int)v3->wszText || v6)
//				goto LABEL_26;
//			--v7;
//			v20 = 2 * (Unicode::strlen(v7 + 1) + 1);
//			memcpy(v7, v7 + 1, v20);
//			if ((unsigned int)v7 <= (unsigned int)v3->wszText || *(v7 - 1) != 13)
//				goto LABEL_72;
//			--v7;
//			v28 = v20;
//			v26 = v7 + 1;
//			goto LABEL_71;
//		case 46:
//			if (*v7 && !v6)
//			{
//				v21 = Unicode::strlen(v7 + 1) + 1;
//				memcpy(v7, v7 + 1, 2 * v21);
//				if (*v7 && *v7 == 10)
//				{
//					v28 = 2 * v21 - 2;
//					v26 = v7 + 1;
//LABEL_71:
//					memcpy(v7, v26, v28);
//				}
//LABEL_72:
//				v30 = 1;
//			}
//			goto LABEL_26;
//		case 13:
//			if (!v3->field_264)
//				goto LABEL_26;
//			v22 = v3->dwEditBoxFlags & 2;
//			++dword_6F8FE254;
//			((void(__stdcall *)(Unicode *))v3->field_264)(v3->wszText);
//			--dword_6F8FE254;
//			if (!v22)
//				return 1;
//			v7 = &v3->wszText[v3->field_25C];
//			goto LABEL_26;
//		case 27:
//			if (v3->field_264)
//			{
//				++dword_6F8FE254;
//				((void(__stdcall *)(DWORD))v3->field_264)(0);
//				--dword_6F8FE254;
//			}
//			return 1;
//		case 9:
//			if (GetKeyState(VK_SHIFT) < 0)
//			{
//				v23 = v3->pPrevious;
//				if (!v23)
//					goto LABEL_98;
//				do
//				{
//					if (v23 == v3)
//						break;
//					v25 = v23->controlHeader.dwFlags;
//					if (gdwBitMasks[2] & v25)
//					{
//						if (v25 & gdwBitMasks[0])
//							break;
//					}
//					v23 = v23->pPrevious;
//				}
//				while (v23);
//			}
//			else
//			{
//				v23 = v3->pNext;
//				if (!v23)
//					goto LABEL_98;
//				do
//				{
//					if (v23 == v3)
//						break;
//					v24 = v23->controlHeader.dwFlags;
//					if (gdwBitMasks[2] & v24)
//					{
//						if (v24 & gdwBitMasks[0])
//							break;
//					}
//					v23 = v23->pNext;
//				}
//				while (v23);
//			}
//			D2Win_10012(v23);
//			D2_ASSERT(v23);
//			D2_ASSERT(v23->controlHeader.nType == D2WIN_EDITBOX);
//
//			v23->field_54 = 0;
//			v23->nTextLength = Unicode::strlen(v23->wszText);
//LABEL_98:
//			result = 1;
//			break;
//		case 112:
//			goto LABEL_99;
//		}
//	}
//	else
//	{
//LABEL_99:
//		result = 0;
//	}
//	return result;

return 0;
}

//D2Win.0x6F8A80A0 (#10081)
void __fastcall D2Win_10081(D2WinEditBoxStrc* pEditBox, int32_t a2)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_48 = a2;
}

//D2Win.0x6F8A80F0 (#10076)
int32_t __fastcall D2Win_10076(D2WinEditBoxStrc* pEditBox, const char* szText)
{
	Unicode wszText[256] = {};

	Unicode::win2Unicode(wszText, szText, 256);
	return D2Win_10075(pEditBox, wszText);
}

//D2Win.0x6F8A8140 (#10075)
int32_t __fastcall D2Win_10075(D2WinEditBoxStrc* pEditBox, const Unicode* pText)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->field_25C = Unicode::strlen(pText);
	if (pEditBox->field_25C)
	{
		Unicode::strcpy(pEditBox->wszText, pText);
	}
	else
	{
		pEditBox->wszText[0] = 0;
	}

	sub_6F8A98A0(pEditBox);

	if (pEditBox->field_26C)
	{
		pEditBox->field_26C(0);
	}

	return 1;
}

//D2Win.0x6F8A81E0 (#10077)
const Unicode* __fastcall D2Win_10077_EDITBOX_GetText(D2WinEditBoxStrc* pEditBox)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	return pEditBox->wszText;
}

//D2Win.0x6F8A8230 (#10080)
void __fastcall D2Win_10080(D2WinEditBoxStrc* pEditBox, D2WinEditBoxStrc* pNext)
{
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	D2_ASSERT(pNext);
	D2_ASSERT(pNext->controlHeader.nType == D2WIN_EDITBOX);

	pEditBox->pNext = pNext;
	pNext->pPrevious = pEditBox;
}

//D2Win.0x6F8A82D0
int32_t __fastcall sub_6F8A82D0(D2WinControlStrc* pControl)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pControl;

	//TODO: Names
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (pEditBox->controlHeader.dwFlags & gdwBitMasks[2] && pEditBox->controlHeader.dwFlags & gdwBitMasks[0])
	{
		if (!D2Win_IsMouseButtonPressed())
		{
			pEditBox->field_280 = 0;
		}

		if (pEditBox->field_280)
		{
			SMSGHANDLER_PARAMS msg = {};
			msg.hWindow = (HWND)pEditBox;
			msg.nMessage = WM_LBUTTONDOWN;
			sub_6F8A9120(&msg);
		}

		if (pEditBox->controlHeader.pCellFile)
		{
			D2GfxDataStrc gfxData = {};
			gfxData.pCellFile = pEditBox->controlHeader.pCellFile;
			gfxData.nDirection = 0;
			gfxData.nFrame = 0;
			TEXTURE_CelDraw(&gfxData, pEditBox->controlHeader.nImageX, pEditBox->controlHeader.nImageY - 1, -1, DRAWMODE_NORMAL, 0);
		}

		D2Win_10127_SetFont(pEditBox->eFont);

		int32_t nX = pEditBox->controlHeader.nImageX + pEditBox->field_40;
		const int32_t nY = pEditBox->field_44 - pEditBox->controlHeader.nHeight + pEditBox->controlHeader.nImageY + 16;

		Unicode v48[300] = {};
		Unicode::strncpy(v48, &pEditBox->wszText[pEditBox->field_4C], pEditBox->field_50 - pEditBox->field_4C + 1);

		//const int32_t v50 = Unicode::strlen(v48);
		if (pEditBox->dwEditBoxFlags & 1)
		{
			Unicode v41[2] = {};
			Unicode::win2Unicode(v41, "*", 2);

			//TODO: Set text
			//if (v50 > 0)
			//{
			//	LOWORD(pEditBox2) = v41[0];
			//	pEditBox3 = pEditBox2 << 16;
			//	LOWORD(pEditBox3) = v41[0];
			//	pEditBox4 = v50 & 1;
			//	pEditBox5 = v50 >> 1;
			//	memset32(v48, pEditBox3, v50 >> 1);
			//	pEditBox6 = &v48[2 * pEditBox5];
			//	for (i = pEditBox4; i; --i)
			//	{
			//		*pEditBox6 = pEditBox3;
			//		++pEditBox6;
			//	}
			//}
		}

		if (pEditBox->field_54 == -1 || pEditBox->field_54 == pEditBox->nTextLength)
		{
			D2Win_10117_DrawText(v48, nX, nY, pEditBox->nTextColor, 0);
		}
		else
		{
			Unicode v49[256] = {};
			Unicode wszText[256] = {};
			Unicode v51[256] = {};

			int32_t v27 = pEditBox->field_54;
			if (pEditBox->field_54 >= pEditBox->nTextLength)
			{
				v27 = pEditBox->nTextLength;
			}

			int32_t v29 = 0;
			if (v29 > v27 - pEditBox->field_4C)
			{
				v29 = v27 - pEditBox->field_4C;
			}

			int32_t v25 = pEditBox->nTextLength;
			if (pEditBox->field_54 > v25)
			{
				v25 = pEditBox->field_54;
			}

			int32_t v30 = 0;
			if (v25 - pEditBox->field_4C - v29 > 0)
			{
				v30 = v25 - pEditBox->field_4C - v29;
			}

			Unicode::strncpy(wszText, v48, v29);
			Unicode::strncpy(v49, &v48[v29], v30);
			Unicode::strcpy(v51, &v48[v29 + v30]);

			int32_t nWidth1 = 0;
			int32_t nWidth2 = 0;
			int32_t nUnused = 0;
			D2Win_10131_GetTextDimensions(wszText, &nWidth1, &nUnused);
			D2Win_10131_GetTextDimensions(v49, &nWidth2, &nUnused);
			D2Win_10117_DrawText(wszText, nX, nY, pEditBox->nTextColor, 0);
			D2Win_10133(v49, nX + nWidth1, nY, D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u), DRAWMODE_NORMAL, pEditBox->nTextColor);
			D2Win_10117_DrawText(v51, nX + nWidth1 + nWidth2, nY, pEditBox->nTextColor, 0);
		}

		if (D2Win_10023() == pEditBox && GetTickCount() / 250 & 1)
		{
			Unicode v41[2] = {};
			Unicode::win2Unicode(v41, "_", 2);
			if (pEditBox->field_25C != pEditBox->field_4C)
			{
				nX += D2Win_10122(v48, pEditBox->field_25C - pEditBox->field_4C);
			}

			D2Win_10117_DrawText(v41, nX, nY, pEditBox->nTextColor, 0);
		}
	}
	return 1;
}

//D2Win.0x6F8A86C0) --------------------------------------------------------
int32_t __fastcall sub_6F8A86C0(D2WinControlStrc* pControl)
{
	//D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pControl;

//	int v5; // eax@18
//	int v6; // esi@18
//	unsigned int v9; // eax@21
//	char *v12; // ebp@25
//	char *v13; // ebx@25
//	int v14; // eax@25
//	int v15; // edx@25
//	char v16; // al@29
//	int v17; // edi@32
//	int v18; // eax@32
//	int v19; // eax@37
//	int v20; // ecx@38
//	int v21; // edx@39
//	int v22; // eax@43
//	int v23; // ecx@43
//	int v24; // edi@43
//	size_t v26; // edi@55
//	int v27; // edx@59
//	int v28; // eax@61
//	int v29; // ebp@64
//	int v30; // edi@68
//	int v31; // eax@68
//	int v32; // edi@77
//	int v33; // eax@77
//	int nX; // [sp+18h] [bp-6C4h]@18
//	int v38; // [sp+1Ch] [bp-6C0h]@41
//	int v39; // [sp+1Ch] [bp-6C0h]@49
//	int v42; // [sp+28h] [bp-6B4h]@29
//	int Count; // [sp+2Ch] [bp-6B0h]@29
//	int pWidth; // [sp+30h] [bp-6ACh]@45
//	int v45; // [sp+34h] [bp-6A8h]@21
//	int v46; // [sp+38h] [bp-6A4h]@23
//	int v47; // [sp+3Ch] [bp-6A0h]@25
//	int nY; // [sp+40h] [bp-69Ch]@18
//	int v49; // [sp+44h] [bp-698h]@25
//	int v52; // [sp+50h] [bp-68Ch]@43
//	int v53; // [sp+54h] [bp-688h]@53
//	int pHeight; // [sp+58h] [bp-684h]@45
//	int v55; // [sp+5Ch] [bp-680h]@20
//	int v56; // [sp+60h] [bp-67Ch]@25
//	char Dest[256]; // [sp+84h] [bp-658h]@29
//	char a2[256]; // [sp+184h] [bp-558h]@25
//
//	D2_ASSERT(pEditBox);
//	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);
//
//	if (pEditBox->controlHeader.dwFlags & gdwBitMasks[2] && pEditBox->controlHeader.dwFlags & gdwBitMasks[0])
//	{
//		if (!D2Win_IsMouseButtonPressed())
//		{
//			pEditBox->field_280 = 0;
//		}
//
//		if (pEditBox->field_280)
//		{
//			SMSGHANDLER_PARAMS msg = {};
//
//			msg.hWindow = (HWND)pEditBox;
//			msg.uMessage = WM_LBUTTONDOWN;
//
//			sub_6F8A9410(&msg, gMousePosition_6F8FE234.x - pEditBox->field_40 - pEditBox->controlHeader.nImageX, gMousePosition_6F8FE234.y + pEditBox->controlHeader.nHeight - pEditBox->field_44 - pEditBox->controlHeader.nImageY);
//		}
//
//		Unicode v61[300] = {};
//
//		D2Win_10127_SetFont(pEditBox->nFont);
//
//		nX = pEditBox->field_40 + pEditBox->controlHeader.nImageX;
//		v6 = pEditBox->field_44 + pEditBox->controlHeader.nImageY - pEditBox->controlHeader.nHeight + D2Win_10125();
//		nY = v6;
//
//		Unicode::strcpy(v61, pEditBox->wszText);
//
//		Unicode wszText[2] = {};
//
//		Unicode::win2Unicode(wszText, "_", 2);
//
//		v55 = D2Win_10122(wszText, 2);
//		if (D2Win_10023() != pEditBox || !(GetTickCount() / 250 & 1))
//		{
//			v45 = 0;
//		}
//		else
//		{
//			v45 = 1;
//		}
//
//		v46 = 0;
//
//		Unicode v60[256] = {};
//
//		D2LANG_unicode2Win(a2, pEditBox->wszText);
//
//		v12 = a2;
//		v13 = a2;
//		v14 = D2Win_10125();
//		v15 = pEditBox->field_40;
//		v47 = v14;
//		v56 = pEditBox->controlHeader.nWidth - 2 * v15;
//		v49 = pEditBox->controlHeader.nHeight - 2 * pEditBox->field_44;
//
//		if (a2[0] || v45 && (D2Win_10117_DrawText(wszText, nX, nY, pEditBox->nTextColor, 0), v46 = 1, a2[0]))
//		{
//			while (1)
//			{
//				if (v6 - nY >= v49)
//				{
//					return 1;
//				}
//
//				Count = v13 - v12;
//				memset(Dest, 0, sizeof(Dest));
//				v16 = *v13;
//				v42 = 0;
//				if (v16)
//				{
//					while (v16 != '\r' && v16 != '\n')
//					{
//						v17 = Count;
//						Dest[Count] = v16;
//						Unicode::win2Unicode(v60, Dest, 256);
//						v18 = D2Win_10122(v60, v17);
//						if (v55 + v18 > v56)
//						{
//							v42 = 1;
//							break;
//						}
//
//						v16 = (v13++)[1];
//						Count = v17 + 1;
//						if (!v16)
//						{
//							break;
//						}
//					}
//				}
//
//				v19 = pEditBox->field_54;
//				if (v19 != -1)
//				{
//					v20 = pEditBox->nTextLength;
//					if (v19 != v20)
//					{
//						break;
//					}
//				}
//LABEL_57:
//				strncpy(Dest, v12, Count);
//				Dest[v13 - v12] = 0;
//				Unicode::win2Unicode(v60, Dest, 256);
//				D2Win_10117_DrawText(v60, nX, v6, pEditBox->nTextColor, 0);
//				if (v45)
//				{
//					if (!v46)
//					{
//						v27 = (int)&a2[pEditBox->field_25C - (_DWORD)v12];
//						if (v27 >= 0)
//						{
//							if (v27 <= Count)
//							{
//								v28 = nX;
//								if (v27)
//								{
//									v28 = nX + D2Win_10122(v60, v27);
//								}
//
//								D2Win_10117_DrawText(wszText, v28, v6, pEditBox->nTextColor, 0);
//								v46 = 1;
//							}
//						}
//					}
//				}
//
//				v29 = v47;
//				if (v42)
//				{
//					v6 += v47;
//				}
//
//				if (*v13 == '\r')
//				{
//					if (v13[1] == '\n')
//					{
//						v30 = v13 - a2;
//						v31 = v6 - nY;
//						do
//						{
//							if (v13[1] != '\n')
//							{
//								break;
//							}
//
//							v13 += 2;
//							v30 += 2;
//							v6 += v29;
//							v31 += v29;
//							v42 = v31;
//							if (v45 && !v46 && pEditBox->field_25C == v30 && v31 < v49)
//							{
//								D2Win_10117_DrawText(wszText, nX, v6, pEditBox->nTextColor, 0);
//								v31 = v42;
//							}
//						}
//						while (*v13 == '\r');
//					}
//					else
//					{
//						v32 = v13 - a2;
//						v33 = v6 - nY;
//						do
//						{
//							++v13;
//							++v32;
//							v6 += v29;
//							v33 += v29;
//							v42 = v33;
//							if (v45 && !v46 && pEditBox->field_25C == v32 && v33 < v49)
//							{
//								D2Win_10117_DrawText(wszText, nX, v6, pEditBox->nTextColor, 0);
//								v33 = v42;
//							}
//						}
//						while (*v13 == '\r');
//					}
//				}
//
//				v12 = v13;
//				if (!*v13)
//				{
//					return 1;
//				}
//			}
//
//			v21 = pEditBox->field_54;
//			if (v19 >= v20)
//			{
//				v21 = pEditBox->nTextLength;
//			}
//
//			v38 = pEditBox->field_54;
//			if (v19 <= v20)
//			{
//				v38 = pEditBox->nTextLength;
//			}
//
//			v22 = v12 - a2;
//			v23 = v13 - a2;
//			v52 = nX;
//			v24 = v38;
//			if (v21 >= (signed int)(v12 - a2))
//			{
//				if (v21 < v23)
//				{
//LABEL_49:
//					v39 = v21;
//					if (v21 <= v22)
//					{
//						v39 = v12 - a2;
//					}
//
//					if (v24 >= v23)
//					{
//						v24 = v13 - a2;
//					}
//
//					v53 = v39 - v22;
//					if (v39 != v22)
//					{
//						strncpy(Dest, v12, v39 - v22);
//						Dest[v53] = 0;
//						Unicode::win2Unicode(v60, Dest, 256);
//						D2Win_10131_GetTextDimensions(v60, &pWidth, &pHeight);
//						v52 = nX + pWidth;
//					}
//
//					v26 = v24 - v39;
//					strncpy(Dest, &v12[v53], v26);
//					Dest[v26] = 0;
//					Unicode::win2Unicode(v60, Dest, 256);
//					D2Win_10131_GetTextDimensions(v60, &pWidth, &pHeight);
//					D2GFX_10055_DrawSolidRectEx(v52, v6 - v47, v52 + pWidth, v6, D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u), DRAWMODE_NORMAL);
//					goto LABEL_57;
//				}
//			}
//			else
//			{
//				if (v38 >= v23)
//				{
//					strncpy(Dest, v12, Count);
//					Dest[v13 - v12] = 0;
//
//					Unicode::win2Unicode(v60, Dest, 256);
//
//					D2Win_10131_GetTextDimensions(v60, &pWidth, &pHeight);
//					D2GFX_10055_DrawSolidRectEx(nX, v6 - v47, nX + pWidth, v6, D2Win_10034_MixRGB(0x40u, 0x40u, 0x40u), DRAWMODE_NORMAL);
//					goto LABEL_57;
//				}
//			}
//
//			if (v38 < v22 || v38 >= v23)
//			{
//				goto LABEL_57;
//			}
//
//			goto LABEL_49;
//		}
//	}
//
	return 1;
}

//D2Win.0x6F8A8D60
void __fastcall sub_6F8A8D60(D2WinEditBoxStrc* pEditBox, int32_t* a2, int32_t* a3)
{
	//Unicode v48[300] = {};

	//TODO: Names
	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	D2Win_10127_SetFont(pEditBox->eFont);

	const int32_t v37 = pEditBox->controlHeader.nImageX + pEditBox->field_40;
	const int32_t v6 = D2Win_10125();
	const int32_t v38 = pEditBox->field_44 + pEditBox->controlHeader.nImageY - pEditBox->controlHeader.nHeight + v6;
	int32_t v7 = v38;
	//Unicode::strcpy(v48, pEditBox->wszText);

	Unicode a1[2] = {};
	Unicode::win2Unicode(a1, "_", 2);

	const int32_t v43 = D2Win_10122(a1, 2);

	char v45[256] = {};
	Unicode::unicode2Win(v45, pEditBox->wszText, ARRAY_SIZE(v45));
	const char* v10 = v45;
	const char* v32 = v45;
	const int32_t v44 = pEditBox->controlHeader.nWidth - 2 * pEditBox->field_40;
	const int32_t v39 = pEditBox->controlHeader.nHeight - 2 * pEditBox->field_44;

	*a2 = -1;
	*a3 = -1;

	while (*v10)
	{
		if (v7 - v38 >= v39)
		{
			break;
		}

		int32_t v16 = v10 - v32;
		char v46[256] = {};
		Unicode v47[256] = {};
		BOOL v41 = 0;
		while (*v10 && *v10 != '\r' && *v10 != '\n')
		{
			v46[v16] = *v10;
			Unicode::win2Unicode(v47, v46, 256);
			if (v43 + D2Win_10122(v47, v16) > v44)
			{
				v41 = 1;
				break;
			}

			++v10;
			++v16;
		}

		const int32_t v19 = (int32_t)&v45[pEditBox->field_25C - (int32_t)v32];
		if (*a2 == -1 && v19 >= 0 && v19 <= v16)
		{
			*a2 = v37;
			if (v19)
			{
				Unicode::win2Unicode(v47, v46, 256);
				*a2 += D2Win_10122(v47, v19);
			}
			*a3 = v7;
		}

		if (v41)
		{
			v7 += v6;
		}

		if (*v10 == '\r')
		{
			if (v10[1] == '\n')
			{
				int32_t v22 = v10 - v45;
				int32_t v23 = v7 - v38;
				do
				{
					if (v10[1] != '\n')
					{
						break;
					}
					v10 += 2;
					v22 += 2;
					v7 += v6;
					v23 += v6;
					if (*a2 == -1 && pEditBox->field_25C == v22 && v23 < v39)
					{
						*a2 = v37;
						*a3 = v7;
					}
				}
				while (*v10 == '\r');
			}
			else
			{
				int32_t v24 = v10 - v45;
				int32_t v25 = v7 - v38;
				do
				{
					++v10;
					++v24;
					v7 += v6;
					v25 += v6;
					if (*a2 == -1 && pEditBox->field_25C == v24 && v25 < v39)
					{
						*a2 = v37;
						*a3 = v7;
					}
				}
				while (*v10 == '\r');
			}
		}
		v32 = v10;
	}

	if (*a2 != -1)
	{
		*a2 -= pEditBox->controlHeader.nImageX - 1;
		*a3 += pEditBox->controlHeader.nHeight - pEditBox->controlHeader.nImageY - 1;

		*a2 = D2Clamp(*a2, pEditBox->field_40, pEditBox->controlHeader.nWidth - pEditBox->field_40 - 1);
		*a3 = D2Clamp(*a2, pEditBox->field_44, pEditBox->controlHeader.nHeight - pEditBox->field_44 - 1);
	}
}

//D2Win.0x6F8A9120
int32_t __stdcall sub_6F8A9120(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (!(pEditBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pEditBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	D2Win_10012(pEditBox);

	const int32_t nX = gMousePosition_6F8FE234.x - pEditBox->field_40 - pEditBox->controlHeader.nImageX;
	const int32_t nY = pEditBox->controlHeader.nHeight - pEditBox->field_44 - pEditBox->controlHeader.nImageY + gMousePosition_6F8FE234.y;

	const bool bInside = nX >= -pEditBox->field_40 && nX < pEditBox->controlHeader.nWidth - pEditBox->field_40 && nY > -pEditBox->field_44 && nY < pEditBox->controlHeader.nHeight - pEditBox->field_44;
	const uint32_t dwTickCount = GetTickCount();

	if (bInside && !pEditBox->field_280 && pEditBox == dword_6F8FD8B0 && dwTickCount - dword_6F8FD8B4 < 300)
	{
		D2Win_10082(pEditBox);
	}
	else
	{
		if (pEditBox->field_280 || bInside)
		{
			D2Win_10127_SetFont(pEditBox->eFont);

			if (pEditBox->field_280)
			{
				if (nX < -pEditBox->field_40)
				{
					if (pEditBox->field_25C - 1 < 0)
					{
						pEditBox->field_25C = 0;
					}
					else
					{
						--pEditBox->field_25C;
					}

					sub_6F8A98A0(pEditBox);
				}
				else if (nX >= pEditBox->controlHeader.nWidth - pEditBox->field_40)
				{
					const int nLength = Unicode::strlen(pEditBox->wszText) - 1;
					if (pEditBox->field_25C + 1 > nLength)
					{
						pEditBox->field_25C = nLength;
					}
					else
					{
						++pEditBox->field_25C;
					}

					sub_6F8A98A0(pEditBox);
				}
			}

			int nCounter = 1;
			while (nX > D2Win_10122(&pEditBox->wszText[pEditBox->field_4C], nCounter))
			{
				if (nCounter > Unicode::strlen(&pEditBox->wszText[pEditBox->field_4C]))
				{
					break;
				}
				++nCounter;
			}

			const int nOldValue = pEditBox->field_25C;
			pEditBox->field_25C = pEditBox->field_4C + nCounter - 1;

			if (GetKeyState(VK_SHIFT) & 0x8000 || pEditBox->field_280)
			{
				if (pEditBox->field_54 == -1)
				{
					pEditBox->field_54 = nOldValue;
				}

				pEditBox->nTextLength = pEditBox->field_25C;
			}
			else
			{
				D2Win_10083(pEditBox);
			}
		}

		if (pMsg->nMessage == WM_LBUTTONDOWN)
		{
			pEditBox->field_280 = 1;
		}
	}

	dword_6F8FD8B0 = pEditBox;
	dword_6F8FD8B4 = dwTickCount;

	if (pEditBox->controlHeader.field_34)
	{
		return pEditBox->controlHeader.field_34(pMsg);
	}

	return 0;
}

//D2Win.0x6F8A93A0
int32_t __stdcall EDITBOX_HandleMouseUp(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (pEditBox->field_280)
	{
		pEditBox->field_280 = 0;
		return 1;
	}

	return 0;
}

//D2Win.0x6F8A9410
int32_t __fastcall sub_6F8A9410(SMSGHANDLER_PARAMS* pMsg, int32_t nX, int32_t nY)
{
	//TODO: Names
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	if (!(pEditBox->controlHeader.dwFlags & gdwBitMasks[2]) || !(pEditBox->controlHeader.dwFlags & gdwBitMasks[0]))
	{
		return 0;
	}

	D2Win_10012(pEditBox);

	if (pEditBox->field_280)
	{
		nX = D2Clamp(nX, pEditBox->field_40, pEditBox->controlHeader.nWidth - pEditBox->field_40 - 1);
		nY = D2Clamp(nY, pEditBox->field_44, pEditBox->controlHeader.nHeight - pEditBox->field_44 - 1);
	}

	if (nX >= -pEditBox->field_40 && nX < pEditBox->controlHeader.nWidth - pEditBox->field_40 && nY >= -pEditBox->field_44 && nY < pEditBox->controlHeader.nHeight - pEditBox->field_44)
	{
		D2Win_10127_SetFont(pEditBox->eFont);

		Unicode wszText[2] = {};
		Unicode::win2Unicode(wszText, "_", 2);

		const int32_t v36 = D2Win_10121_GetTextWidth(wszText);

		char v38[256] = {};
		Unicode::unicode2Win(v38, pEditBox->wszText, ARRAYSIZE(v38));
		const char* v30 = v38;
		const char* v13 = v38;
		int32_t v26 = 0;
		const int32_t v28 = D2Win_10125();
		int32_t v31 = 0;
		int32_t v35 = -1;

		while (*v13)
		{
			char v39[256] = {};
			Unicode v40[256] = {};

			int32_t v34 = 0;
			int32_t v18 = v13 - v30;
			while (*v13 && *v13 != '\r' && *v13 != '\n')
			{
				v39[v18] = *v13;
				Unicode::win2Unicode(v40, v39, 256);
				if (v36 + D2Win_10122(v40, v18) > pEditBox->controlHeader.nWidth - 2 * pEditBox->field_40)
				{
					v34 = 1;
					break;
				}
				++v13;
				++v18;
			}

			if (nY >= v26 && nY < v28 + v26)
			{
				int32_t v21 = 1;
				Unicode::win2Unicode(v40, v39, 256);
				while (nX > D2Win_10122(v40, v21))
				{
					if (v21 > v18)
					{
						break;
					}
					++v21;
				}
				v35 = pEditBox->field_25C;
				v31 = 1;
				pEditBox->field_25C = (int32_t)&v30[v21 - (int32_t)v38 - 1];
			}

			if (!v34)
			{
				if (*v13 == '\r')
				{
					if (v13[1] == '\n')
					{
						v13 += 2;
					}
					else
					{
						++v13;
					}
					v26 += v28;
				}
			}
			else
			{
				v26 += v28;
			}

			v30 = v13;
		}

		int32_t v23 = v35;
		if (!v31)
		{
			v23 = pEditBox->field_25C;
			pEditBox->field_25C = Unicode::strlen(pEditBox->wszText);
		}

		if ((GetKeyState(VK_SHIFT) & 0x8000 || pEditBox->field_280) && v23 != -1)
		{
			if (pEditBox->field_54 == -1)
			{
				pEditBox->field_54 = v23;
			}
			pEditBox->nTextLength = pEditBox->field_25C;
		}
		else
		{
			D2Win_10083(pEditBox);
		}
	}

	if (pMsg->nMessage == WM_LBUTTONDOWN)
	{
		pEditBox->field_280 = 1;
	}

	if (pEditBox->controlHeader.field_34)
	{
		return pEditBox->controlHeader.field_34(pMsg);
	}

	return 0;
}

//D2Win.0x6F8A97D0
int32_t __stdcall sub_6F8A97D0(SMSGHANDLER_PARAMS* pMsg)
{
	D2WinEditBoxStrc* pEditBox = (D2WinEditBoxStrc*)pMsg->hWindow;

	D2_ASSERT(pEditBox);
	D2_ASSERT(pEditBox->controlHeader.nType == D2WIN_EDITBOX);

	return sub_6F8A9410(pMsg, gMousePosition_6F8FE234.x - pEditBox->field_40 - pEditBox->controlHeader.nImageX, gMousePosition_6F8FE234.y + pEditBox->controlHeader.nHeight - pEditBox->field_44 - pEditBox->controlHeader.nImageY);
}

//D2Win.0x6F8A9850 (#10078)
bool __stdcall D2Win_10078_IsNumber(int32_t nUnused1, int32_t nUnused2, char* szChar)
{
	return *szChar >= '0' && *szChar <= '9';
}

//D2Win.0x6F8A9870 (#10079)
bool __stdcall D2Win_10079_IsLetter(int nUnused1, int nUnused2, char* szChar)
{
	return *szChar >= 'a' && *szChar <= 'z' || *szChar >= 'A' && *szChar <= 'Z';
}

//D2Win.0x6F8A98A0
void __fastcall sub_6F8A98A0(D2WinEditBoxStrc* pEditBox)
{
	//TODO: Names
	D2Win_10127_SetFont(pEditBox->eFont);

	const int32_t v2 = Unicode::strlen(pEditBox->wszText);

	Unicode wszText[300] = {};
	if (pEditBox->dwEditBoxFlags & EDITBOX_PASSWORD)
	{
		Unicode wszStar[2] = {};
		Unicode::win2Unicode(wszStar, "*", 2);
		if (v2 > 0)
		{
			//TODO: Set text

			//LOWORD(v7) = wszStar;
			//v8 = v7 << 16;
			//LOWORD(v8) = wszStar;
			//memset32(wszText, v8, (unsigned int)v2 >> 1);
			//v9 = &wszText[2 * ((unsigned int)v2 >> 1)];
			//for (int i = v2 & 1; i; --i)
			//{
			//	*v9 = v8;
			//	++v9;
			//}
		}
	}
	else
	{
		Unicode::strcpy(wszText, pEditBox->wszText);
	}

	Unicode wszDash[2] = {};
	Unicode::win2Unicode(wszDash, "_", 2);

	const int32_t nDashWidth = D2Win_10121_GetTextWidth(wszDash);

	int32_t nTextWidth = D2Win_10121_GetTextWidth(wszText);
	if (pEditBox->field_25C == v2)
	{
		nTextWidth += nDashWidth;
	}

	if (nTextWidth <= pEditBox->controlHeader.nWidth - 2 * pEditBox->field_40)
	{
		pEditBox->field_4C = 0;
		pEditBox->field_50 = v2 - 1;
	}
	else
	{
		int32_t v16 = pEditBox->controlHeader.nWidth - 2 * pEditBox->field_40;
		if (pEditBox->field_25C == v2)
		{
			v16 -= nDashWidth;
		}

		if (pEditBox->field_25C > pEditBox->field_50)
		{
			pEditBox->field_50 = pEditBox->field_25C;
			pEditBox->field_4C = pEditBox->field_25C - 1;
			while (pEditBox->field_4C)
			{
				if (D2Win_10122(&wszText[pEditBox->field_4C], pEditBox->field_50 - pEditBox->field_4C + 1) >= v16)
				{
					break;
				}

				--pEditBox->field_4C;
			}

			++pEditBox->field_4C;
		}
		else if (pEditBox->field_25C > pEditBox->field_4C + 1)
		{
			if (D2Win_10121_GetTextWidth(&wszText[pEditBox->field_4C]) >= v16)
			{
				pEditBox->field_50 = pEditBox->field_4C + 1;
				while (pEditBox->field_50 < v2)
				{
					if (D2Win_10122(&wszText[pEditBox->field_4C], pEditBox->field_50 - pEditBox->field_4C + 1) >= v16)
					{
						break;
					}

					++pEditBox->field_50;
				}

				--pEditBox->field_50;
			}
			else
			{
				pEditBox->field_50 = v2 - 1;
				pEditBox->field_4C = v2 - 2;
				while (pEditBox->field_4C)
				{
					if (D2Win_10121_GetTextWidth(&wszText[pEditBox->field_4C]) >= v16)
					{
						break;
					}

					--pEditBox->field_4C;
				}

				++pEditBox->field_4C;
			}
		}
		else
		{
			const int32_t v19 = std::max(pEditBox->field_25C - 1, 0);

			pEditBox->field_4C = v19;
			pEditBox->field_50 = v19 + 1;
			while (pEditBox->field_50 < v2)
			{
				if (D2Win_10122(&wszText[pEditBox->field_4C], pEditBox->field_50 - pEditBox->field_4C + 1) >= v16)
				{
					break;
				}

				++pEditBox->field_50;
			}

			--pEditBox->field_50;
		}
	}
}
