#include "D2DataTbls.h"


//D2Common.0x6FD720A0
void __fastcall DATATBLS_LoadOverlayTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "overlay", TXTFIELD_NAMETOINDEX, 0, 0, &gpDataTables.pOverlayLinker },
		{ "Filename", TXTFIELD_ASCII, 64, TXTFIELD_DWORD, NULL },
		{ "version", TXTFIELD_WORD, 0, 66, NULL },
		{ "Frames", TXTFIELD_DWORD, 0, 68, NULL },
		{ "PreDraw", TXTFIELD_BYTE, 0, 72, NULL },
		{ "1ofN", TXTFIELD_DWORD, 0, 76, NULL },
		{ "Dir", TXTFIELD_BYTE, 0, 80, NULL },
		{ "Open", TXTFIELD_BYTE, 0, 81, NULL },
		{ "Beta", TXTFIELD_BYTE, 0, 82, NULL },
		{ "Xoffset", TXTFIELD_DWORD, 0, 84, NULL },
		{ "Yoffset", TXTFIELD_DWORD, 0, 88, NULL },
		{ "Height1", TXTFIELD_DWORD, 0, 92, NULL },
		{ "Height2", TXTFIELD_DWORD, 0, 96, NULL },
		{ "Height3", TXTFIELD_DWORD, 0, 100, NULL },
		{ "Height4", TXTFIELD_DWORD, 0, 104, NULL },
		{ "AnimRate", TXTFIELD_DWORD, 0, 108, NULL },
		{ "LoopWaitTime", TXTFIELD_DWORD, 0, 120, NULL },
		{ "Trans", TXTFIELD_BYTE, 0, 124, NULL },
		{ "InitRadius", TXTFIELD_DWORD, 0, 112, NULL },
		{ "Radius", TXTFIELD_DWORD, 0, 116, NULL },
		{ "Red", TXTFIELD_BYTE, 0, 125, NULL },
		{ "Green", TXTFIELD_BYTE, 0, 126, NULL },
		{ "Blue", TXTFIELD_BYTE, 0, 127, NULL },
		{ "NumDirections", TXTFIELD_BYTE, 0, 128, NULL },
		{ "LocalBlood", TXTFIELD_BYTE, 0, 129, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpDataTables.pOverlayLinker = (D2TxtLinkStrc*)FOG_AllocLinker(__FILE__, __LINE__);

	gpDataTables.pOverlayTxt = (D2OverlayTxt*)DATATBLS_CompileTxt(pMemPool, "overlay", pTbl, &gpDataTables.nOverlayTxtRecordCount, sizeof(D2OverlayTxt));
}

//D2Common.0x6FD72500
void __fastcall DATATBLS_UnloadOverlayTxt()
{
	FOG_FreeLinker(gpDataTables.pOverlayLinker);
	DATATBLS_UnloadBin(gpDataTables.pOverlayTxt);
	gpDataTables.pOverlayTxt = NULL;
	gpDataTables.nOverlayTxtRecordCount = 0;
}

//D2Common.0x6FD72530 (#10674)
DWORD __stdcall DATATBLS_GetFramesFromOverlayTxt(int nOverlayId)
{
	D2OverlayTxt* pOverlayTxt = DATATBLS_GetOverlayTxtRecord(nOverlayId);

	if (pOverlayTxt)
	{
		return pOverlayTxt->dwFrames;
	}
	
	return 0;
}

//D2Common.0x6FD72570 (#10675)
int __stdcall DATABLS_GetPreDrawFromOverlayTxt(int nOverlayId)
{
	D2OverlayTxt* pOverlayTxt = DATATBLS_GetOverlayTxtRecord(nOverlayId);

	if (pOverlayTxt)
	{
		return pOverlayTxt->nPreDraw;
	}

	return 0;
}

//D2Common.0x6FD725B0 (#10676)
DWORD __stdcall DATATBLS_GetOffsetXFromOverlayTxt(int nOverlayId)
{
	D2OverlayTxt* pOverlayTxt = DATATBLS_GetOverlayTxtRecord(nOverlayId);

	if (pOverlayTxt)
	{
		return pOverlayTxt->dwOffsetX;
	}

	return 0;
}

//D2Common.0x6FD725F0 (#10677)
DWORD __stdcall DATATBLS_GetOffsetYFromOverlayTxt(int nOverlayId)
{
	D2OverlayTxt* pOverlayTxt = DATATBLS_GetOverlayTxtRecord(nOverlayId);

	if (pOverlayTxt)
	{
		return pOverlayTxt->dwOffsetY;
	}

	return 0;
}

//D2Common.0x6FD72630 (#10678)
int __stdcall DATATBLS_GetTransFromOverlayTxt(int nOverlayId)
{
	D2OverlayTxt* pOverlayTxt = DATATBLS_GetOverlayTxtRecord(nOverlayId);

	if (pOverlayTxt)
	{
		return pOverlayTxt->nTrans;
	}

	return 0;
}

//D2Common.0x6FD72670 (#10679)
int __stdcall DATATBLS_GetRadiusAndColorFromOverlayTxt(int nOverlayId, int* pInitRadius, int* pRadius, BYTE* pRed, BYTE* pGreen, BYTE* pBlue)
{
	D2OverlayTxt* pOverlayTxt = DATATBLS_GetOverlayTxtRecord(nOverlayId);

	if (pOverlayTxt)
	{
		*pInitRadius = pOverlayTxt->dwInitRadius;
		*pRadius = pOverlayTxt->dwRadius;
		*pRed = pOverlayTxt->nRed;
		*pGreen = pOverlayTxt->nGreen;
		*pBlue = pOverlayTxt->nBlue;
		return *pRadius || *pInitRadius;
	}

	*pInitRadius = 0;
	*pRadius = 0;
	*pRed = 0;
	*pGreen = 0;
	*pBlue = 0;

	return FALSE;
}

//D2Common.0x6FD72720 (#10680)
DWORD __stdcall DATATBLS_Get1OfNFromOverlayTxt(int nOverlayId)
{
	D2OverlayTxt* pOverlayTxt = DATATBLS_GetOverlayTxtRecord(nOverlayId);

	if (pOverlayTxt)
	{
		return pOverlayTxt->dw1ofN;
	}

	return 0;
}

//D2Common.0x6FD72760 (#10681)
int __stdcall DATATBLS_GetDirFromOverlayTxt(int nOverlayId)
{
	D2OverlayTxt* pOverlayTxt = DATATBLS_GetOverlayTxtRecord(nOverlayId);

	if (pOverlayTxt)
	{
		return pOverlayTxt->nDir;
	}

	return 0;
}

//Inlined at various places
D2OverlayTxt* __fastcall DATATBLS_GetOverlayTxtRecord(int nOverlay)
{
	if (nOverlay >= 0 && nOverlay < gpDataTables.nOverlayTxtRecordCount)
	{
		return &gpDataTables.pOverlayTxt[nOverlay];
	}

	return NULL;
}
