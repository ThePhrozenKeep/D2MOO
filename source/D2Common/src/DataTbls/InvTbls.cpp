#include <DataTbls/InvTbls.h>
#include <D2DataTbls.h>

//D2Common.0x6FD542D0
void __fastcall DATATBLS_LoadInventoryTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "invLeft", TXTFIELD_DWORD, 0, 0, NULL },
		{ "invRight", TXTFIELD_DWORD, 0, 4, NULL },
		{ "invTop", TXTFIELD_DWORD, 0, 8, NULL },
		{ "invBottom", TXTFIELD_DWORD, 0, 12, NULL },
		{ "gridX", TXTFIELD_BYTE, 0, 16, NULL },
		{ "gridY", TXTFIELD_BYTE, 0, 17, NULL },
		{ "gridLeft", TXTFIELD_DWORD, 0, 20, NULL },
		{ "gridRight", TXTFIELD_DWORD, 0, 24, NULL },
		{ "gridTop", TXTFIELD_DWORD, 0, 28, NULL },
		{ "gridBottom", TXTFIELD_DWORD, 0, 32, NULL },
		{ "gridBoxWidth", TXTFIELD_BYTE, 0, 36, NULL },
		{ "gridBoxHeight", TXTFIELD_BYTE, 0, 37, NULL },
		{ "rArmLeft", TXTFIELD_DWORD, 0, 40, NULL },
		{ "rArmRight", TXTFIELD_DWORD, 0, 44, NULL },
		{ "rArmTop", TXTFIELD_DWORD, 0, 48, NULL },
		{ "rArmBottom", TXTFIELD_DWORD, 0, 52, NULL },
		{ "rArmWidth", TXTFIELD_BYTE, 0, 56, NULL },
		{ "rArmHeight", TXTFIELD_BYTE, 0, 57, NULL },
		{ "torsoLeft", TXTFIELD_DWORD, 0, 60, NULL },
		{ "torsoRight", TXTFIELD_DWORD, 0, 64, NULL },
		{ "torsoTop", TXTFIELD_DWORD, 0, 68, NULL },
		{ "torsoBottom", TXTFIELD_DWORD, 0, 72, NULL },
		{ "torsoWidth", TXTFIELD_BYTE, 0, 76, NULL },
		{ "torsoHeight", TXTFIELD_BYTE, 0, 77, NULL },
		{ "lArmLeft", TXTFIELD_DWORD, 0, 80, NULL },
		{ "lArmRight", TXTFIELD_DWORD, 0, 84, NULL },
		{ "lArmTop", TXTFIELD_DWORD, 0, 88, NULL },
		{ "lArmBottom", TXTFIELD_DWORD, 0, 92, NULL },
		{ "lArmWidth", TXTFIELD_BYTE, 0, 96, NULL },
		{ "lArmHeight", TXTFIELD_BYTE, 0, 97, NULL },
		{ "headLeft", TXTFIELD_DWORD, 0, 100, NULL },
		{ "headRight", TXTFIELD_DWORD, 0, 104, NULL },
		{ "headTop", TXTFIELD_DWORD, 0, 108, NULL },
		{ "headBottom", TXTFIELD_DWORD, 0, 112, NULL },
		{ "headWidth", TXTFIELD_BYTE, 0, 116, NULL },
		{ "headHeight", TXTFIELD_BYTE, 0, 117, NULL },
		{ "neckLeft", TXTFIELD_DWORD, 0, 120, NULL },
		{ "neckRight", TXTFIELD_DWORD, 0, 124, NULL },
		{ "neckTop", TXTFIELD_DWORD, 0, 128, NULL },
		{ "neckBottom", TXTFIELD_DWORD, 0, 132, NULL },
		{ "neckWidth", TXTFIELD_BYTE, 0, 136, NULL },
		{ "neckHeight", TXTFIELD_BYTE, 0, 137, NULL },
		{ "rHandLeft", TXTFIELD_DWORD, 0, 140, NULL },
		{ "rHandRight", TXTFIELD_DWORD, 0, 144, NULL },
		{ "rHandTop", TXTFIELD_DWORD, 0, 148, NULL },
		{ "rHandBottom", TXTFIELD_DWORD, 0, 152, NULL },
		{ "rHandWidth", TXTFIELD_BYTE, 0, 156, NULL },
		{ "rHandHeight", TXTFIELD_BYTE, 0, 157, NULL },
		{ "lHandLeft", TXTFIELD_DWORD, 0, 160, NULL },
		{ "lHandRight", TXTFIELD_DWORD, 0, 164, NULL },
		{ "lHandTop", TXTFIELD_DWORD, 0, 168, NULL },
		{ "lHandBottom", TXTFIELD_DWORD, 0, 172, NULL },
		{ "lHandWidth", TXTFIELD_BYTE, 0, 176, NULL },
		{ "lHandHeight", TXTFIELD_BYTE, 0, 177, NULL },
		{ "beltLeft", TXTFIELD_DWORD, 0, 180, NULL },
		{ "beltRight", TXTFIELD_DWORD, 0, 184, NULL },
		{ "beltTop", TXTFIELD_DWORD, 0, 188, NULL },
		{ "beltBottom", TXTFIELD_DWORD, 0, 192, NULL },
		{ "beltWidth", TXTFIELD_BYTE, 0, 196, NULL },
		{ "beltHeight", TXTFIELD_BYTE, 0, 197, NULL },
		{ "feetLeft", TXTFIELD_DWORD, 0, 200, NULL },
		{ "feetRight", TXTFIELD_DWORD, 0, 204, NULL },
		{ "feetTop", TXTFIELD_DWORD, 0, 208, NULL },
		{ "feetBottom", TXTFIELD_DWORD, 0, 212, NULL },
		{ "feetWidth", TXTFIELD_BYTE, 0, 216, NULL },
		{ "feetHeight", TXTFIELD_BYTE, 0, 217, NULL },
		{ "glovesLeft", TXTFIELD_DWORD, 0, 220, NULL },
		{ "glovesRight", TXTFIELD_DWORD, 0, 224, NULL },
		{ "glovesTop", TXTFIELD_DWORD, 0, 228, NULL },
		{ "glovesBottom", TXTFIELD_DWORD, 0, 232, NULL },
		{ "glovesWidth", TXTFIELD_BYTE, 0, 236, NULL },
		{ "glovesHeight", TXTFIELD_BYTE, 0, 237, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pInventoryTxt = (D2InventoryTxt*)DATATBLS_CompileTxt(pMemPool, "inventory", pTbl, &sgptDataTables->nInventoryTxtRecordCount, sizeof(D2InventoryTxt));
#define NUM_INVENTORY_PAGE_STATS 16
	D2_ASSERT(sgptDataTables->nInventoryTxtRecordCount == NUM_INVENTORY_PAGE_STATS * NUM_GAME_RESOLUTIONS);
}

//D2Common.0x6FD54F10
void __fastcall DATATBLS_UnloadInventoryTxt()
{
	DATATBLS_UnloadBin(sgptDataTables->pInventoryTxt);
}

//D2Common.0x6FD54F20 (#10635)
void __stdcall DATATBLS_GetInventoryRect(int nInventoryTxtId, int bHigherRes, D2InvRectStrc* pInvRect)
{

	D2_ASSERT(sgptDataTables->pInventoryTxt);

	const int nIndex = nInventoryTxtId + 16 * bHigherRes;
	D2_ASSERT(nIndex < sgptDataTables->nInventoryTxtRecordCount);
	const D2InventoryTxt* pInventoryTxtRecord = &sgptDataTables->pInventoryTxt[nIndex];
	D2_ASSERT(pInventoryTxtRecord);

	pInvRect->nLeft = pInventoryTxtRecord->pRect.nLeft;
	pInvRect->nRight = pInventoryTxtRecord->pRect.nRight;
	pInvRect->nTop = pInventoryTxtRecord->pRect.nTop;
	pInvRect->nBottom = pInventoryTxtRecord->pRect.nBottom;
}

//D2Common.0x6FD54FB0 (#10636)
void __stdcall DATATBLS_GetInventoryGridInfo(int nInventoryTxtId, int bHigherRes, D2InventoryGridInfoStrc* pInventoryGridInfo)
{
	D2InventoryTxt* pInventoryTxtRecord = NULL;
	int nIndex = nInventoryTxtId + 16 * bHigherRes;

	D2_ASSERT(sgptDataTables->pInventoryTxt);
	D2_ASSERT(nIndex < sgptDataTables->nInventoryTxtRecordCount);
	pInventoryTxtRecord = &sgptDataTables->pInventoryTxt[nIndex];
	D2_ASSERT(pInventoryTxtRecord);

	memcpy(pInventoryGridInfo, &pInventoryTxtRecord->pGridInfo, sizeof(D2InventoryGridInfoStrc));
}

//D2Common.0x6FD55030 (#10637)
void __stdcall DATATBLS_GetInventoryComponentGrid(int nInventoryTxtId, int bHigherRes, D2InvCompGridStrc* pInvCompGrid, int nComponent)
{
	int nIndex = nInventoryTxtId + 16 * bHigherRes;

	D2_ASSERT(sgptDataTables->pInventoryTxt);
	D2_ASSERT(nIndex < sgptDataTables->nInventoryTxtRecordCount);

	const D2InventoryTxt* pInventoryTxtRecord = &sgptDataTables->pInventoryTxt[nIndex];
	D2_ASSERT(pInventoryTxtRecord);

	const D2InvCompGridStrc* ptBodyStats = &pInventoryTxtRecord->pComponents[nComponent];
	D2_ASSERT(ptBodyStats);

	memcpy(pInvCompGrid, ptBodyStats, sizeof(D2InvCompGridStrc));
}
