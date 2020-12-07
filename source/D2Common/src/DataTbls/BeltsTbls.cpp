#include "D2DataTbls.h"


//D2Common.0x6FD48880
void __fastcall DATATBLS_LoadBeltsTxt(void* pMemPool)
{
	int nRecordCount = 0;
	D2BinFieldStrc pTbl[] =
	{
		{ "numboxes", TXTFIELD_BYTE, 0, 4, NULL },
		{ "box1left", TXTFIELD_DWORD, 0, 8, NULL },
		{ "box1right", TXTFIELD_DWORD, 0, 12, NULL },
		{ "box1top", TXTFIELD_DWORD, 0, 16, NULL },
		{ "box1bottom", TXTFIELD_DWORD, 0, 20, NULL },
		{ "box2left", TXTFIELD_DWORD, 0, 24, NULL },
		{ "box2right", TXTFIELD_DWORD, 0, 28, NULL },
		{ "box2top", TXTFIELD_DWORD, 0, 32, NULL },
		{ "box2bottom", TXTFIELD_DWORD, 0, 36, NULL },
		{ "box3left", TXTFIELD_DWORD, 0, 40, NULL },
		{ "box3right", TXTFIELD_DWORD, 0, 44, NULL },
		{ "box3top", TXTFIELD_DWORD, 0, 48, NULL },
		{ "box3bottom", TXTFIELD_DWORD, 0, 52, NULL },
		{ "box4left", TXTFIELD_DWORD, 0, 56, NULL },
		{ "box4right", TXTFIELD_DWORD, 0, 60, NULL },
		{ "box4top", TXTFIELD_DWORD, 0, 64, NULL },
		{ "box4bottom", TXTFIELD_DWORD, 0, 68, NULL },
		{ "box5left", TXTFIELD_DWORD, 0, 72, NULL },
		{ "box5right", TXTFIELD_DWORD, 0, 76, NULL },
		{ "box5top", TXTFIELD_DWORD, 0, 80, NULL },
		{ "box5bottom", TXTFIELD_DWORD, 0, 84, NULL },
		{ "box6left", TXTFIELD_DWORD, 0, 88, NULL },
		{ "box6right", TXTFIELD_DWORD, 0, 92, NULL },
		{ "box6top", TXTFIELD_DWORD, 0, 96, NULL },
		{ "box6bottom", TXTFIELD_DWORD, 0, 100, NULL },
		{ "box7left", TXTFIELD_DWORD, 0, 104, NULL },
		{ "box7right", TXTFIELD_DWORD, 0, 108, NULL },
		{ "box7top", TXTFIELD_DWORD, 0, 112, NULL },
		{ "box7bottom", TXTFIELD_DWORD, 0, 116, NULL },
		{ "box8left", TXTFIELD_DWORD, 0, 120, NULL },
		{ "box8right", TXTFIELD_DWORD, 0, 124, NULL },
		{ "box8top", TXTFIELD_DWORD, 0, 128, NULL },
		{ "box8bottom", TXTFIELD_DWORD, 0, 132, NULL },
		{ "box9left", TXTFIELD_DWORD, 0, 136, NULL },
		{ "box9right", TXTFIELD_DWORD, 0, 140, NULL },
		{ "box9top", TXTFIELD_DWORD, 0, 144, NULL },
		{ "box9bottom", TXTFIELD_DWORD, 0, 148, NULL },
		{ "box10left", TXTFIELD_DWORD, 0, 152, NULL },
		{ "box10right", TXTFIELD_DWORD, 0, 156, NULL },
		{ "box10top", TXTFIELD_DWORD, 0, 160, NULL },
		{ "box10bottom", TXTFIELD_DWORD, 0, 164, NULL },
		{ "box11left", TXTFIELD_DWORD, 0, 168, NULL },
		{ "box11right", TXTFIELD_DWORD, 0, 172, NULL },
		{ "box11top", TXTFIELD_DWORD, 0, 176, NULL },
		{ "box11bottom", TXTFIELD_DWORD, 0, 180, NULL },
		{ "box12left", TXTFIELD_DWORD, 0, 184, NULL },
		{ "box12right", TXTFIELD_DWORD, 0, 188, NULL },
		{ "box12top", TXTFIELD_DWORD, 0, 192, NULL },
		{ "box12bottom", TXTFIELD_DWORD, 0, 196, NULL },
		{ "box13left", TXTFIELD_DWORD, 0, 200, NULL },
		{ "box13right", TXTFIELD_DWORD, 0, 204, NULL },
		{ "box13top", TXTFIELD_DWORD, 0, 208, NULL },
		{ "box13bottom", TXTFIELD_DWORD, 0, 212, NULL },
		{ "box14left", TXTFIELD_DWORD, 0, 216, NULL },
		{ "box14right", TXTFIELD_DWORD, 0, 220, NULL },
		{ "box14top", TXTFIELD_DWORD, 0, 224, NULL },
		{ "box14bottom", TXTFIELD_DWORD, 0, 228, NULL },
		{ "box15left", TXTFIELD_DWORD, 0, 232, NULL },
		{ "box15right", TXTFIELD_DWORD, 0, 236, NULL },
		{ "box15top", TXTFIELD_DWORD, 0, 240, NULL },
		{ "box15bottom", TXTFIELD_DWORD, 0, 244, NULL },
		{ "box16left", TXTFIELD_DWORD, 0, 248, NULL },
		{ "box16right", TXTFIELD_DWORD, 0, 252, NULL },
		{ "box16top", TXTFIELD_DWORD, 0, 256, NULL },
		{ "box16bottom", TXTFIELD_DWORD, 0, 260, NULL },
		{ "end", 0, 0, 0, NULL },
	};

	gpBeltsTxtTable = (D2BeltsTxt*)DATATBLS_CompileTxt(pMemPool, "belts", pTbl, &nRecordCount, sizeof(D2BeltsTxt));

	D2_ASSERT((nRecordCount / NUM_GAME_RESOLUTIONS) == 7);
}

//D2Common.0x6FD493A0
void __fastcall DATATBLS_UnloadBeltsTxt()
{
	DATATBLS_UnloadBin(gpBeltsTxtTable);
}

//D2Common.0x6FD493B0 (#10638)
void __stdcall DATATBLS_GetBeltsTxtRecord(int nIndex, int bHigherRes, D2BeltsTxt* pRecord)
{
	D2_ASSERT(gpBeltsTxtTable);
	memcpy(pRecord, &gpBeltsTxtTable[nIndex + 7 * bHigherRes], sizeof(D2BeltsTxt));
}

//D2Common.0x6FD49420 (#10639)
void __stdcall DATATBLS_GetInvRectFromBeltsTxt(int nIndex, int bHigherRes, D2InvRectStrc* pInvRect, int nBoxId)
{
	D2BeltsTxt* ptBeltsTxtRecord = NULL;

	D2_ASSERT(gpBeltsTxtTable);

	ptBeltsTxtRecord = &gpBeltsTxtTable[nIndex + 7 * bHigherRes];

	D2_ASSERT(ptBeltsTxtRecord);

	const D2InvRectStrc* ptBoxStats = &ptBeltsTxtRecord->beltBoxes[nBoxId];
	D2_ASSERT(ptBoxStats);

	pInvRect->nLeft = ptBoxStats->nLeft;
	pInvRect->nRight = ptBoxStats->nRight;
	pInvRect->nTop = ptBoxStats->nTop;
	pInvRect->nBottom = ptBoxStats->nBottom;
}
