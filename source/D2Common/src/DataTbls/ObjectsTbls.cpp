#include "D2DataTbls.h"
#include <D2Lang.h>


//D2Common.0x6FD6FDF0
void __fastcall DATATBLS_LoadObjectsTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Name", TXTFIELD_ASCII, 63, 0, NULL },
		{ "Token", TXTFIELD_ASCII, 2, 192, NULL },
		{ "SpawnMax", TXTFIELD_BYTE, 0, 195, NULL },
		{ "Selectable0", TXTFIELD_BYTE, 0, 196, NULL },
		{ "Selectable1", TXTFIELD_BYTE, 0, 197, NULL },
		{ "Selectable2", TXTFIELD_BYTE, 0, 198, NULL },
		{ "Selectable3", TXTFIELD_BYTE, 0, 199, NULL },
		{ "Selectable4", TXTFIELD_BYTE, 0, 200, NULL },
		{ "Selectable5", TXTFIELD_BYTE, 0, 201, NULL },
		{ "Selectable6", TXTFIELD_BYTE, 0, 202, NULL },
		{ "Selectable7", TXTFIELD_BYTE, 0, 203, NULL },
		{ "TrapProb", TXTFIELD_BYTE, 0, 204, NULL },
		{ "SizeX", TXTFIELD_DWORD, 0, 208, NULL },
		{ "SizeY", TXTFIELD_DWORD, 0, 212, NULL },
		{ "FrameCnt0", TXTFIELD_DWORD, 0, 216, NULL },
		{ "FrameCnt1", TXTFIELD_DWORD, 0, 220, NULL },
		{ "FrameCnt2", TXTFIELD_DWORD, 0, 224, NULL },
		{ "FrameCnt3", TXTFIELD_DWORD, 0, 228, NULL },
		{ "FrameCnt4", TXTFIELD_DWORD, 0, 232, NULL },
		{ "FrameCnt5", TXTFIELD_DWORD, 0, 236, NULL },
		{ "FrameCnt6", TXTFIELD_DWORD, 0, 240, NULL },
		{ "FrameCnt7", TXTFIELD_DWORD, 0, 244, NULL },
		{ "FrameDelta0", TXTFIELD_WORD, 0, 248, NULL },
		{ "FrameDelta1", TXTFIELD_WORD, 0, 250, NULL },
		{ "FrameDelta2", TXTFIELD_WORD, 0, 252, NULL },
		{ "FrameDelta3", TXTFIELD_WORD, 0, 254, NULL },
		{ "FrameDelta4", TXTFIELD_WORD, 0, 256, NULL },
		{ "FrameDelta5", TXTFIELD_WORD, 0, 258, NULL },
		{ "FrameDelta6", TXTFIELD_WORD, 0, 260, NULL },
		{ "FrameDelta7", TXTFIELD_WORD, 0, 262, NULL },
		{ "CycleAnim0", TXTFIELD_BYTE, 0, 264, NULL },
		{ "CycleAnim1", TXTFIELD_BYTE, 0, 265, NULL },
		{ "CycleAnim2", TXTFIELD_BYTE, 0, 266, NULL },
		{ "CycleAnim3", TXTFIELD_BYTE, 0, 267, NULL },
		{ "CycleAnim4", TXTFIELD_BYTE, 0, 268, NULL },
		{ "CycleAnim5", TXTFIELD_BYTE, 0, 269, NULL },
		{ "CycleAnim6", TXTFIELD_BYTE, 0, 270, NULL },
		{ "CycleAnim7", TXTFIELD_BYTE, 0, 271, NULL },
		{ "Lit0", TXTFIELD_BYTE, 0, 272, NULL },
		{ "Lit1", TXTFIELD_BYTE, 0, 273, NULL },
		{ "Lit2", TXTFIELD_BYTE, 0, 274, NULL },
		{ "Lit3", TXTFIELD_BYTE, 0, 275, NULL },
		{ "Lit4", TXTFIELD_BYTE, 0, 276, NULL },
		{ "Lit5", TXTFIELD_BYTE, 0, 277, NULL },
		{ "Lit6", TXTFIELD_BYTE, 0, 278, NULL },
		{ "Lit7", TXTFIELD_BYTE, 0, 279, NULL },
		{ "BlocksLight0", TXTFIELD_BYTE, 0, 280, NULL },
		{ "BlocksLight1", TXTFIELD_BYTE, 0, 281, NULL },
		{ "BlocksLight2", TXTFIELD_BYTE, 0, 282, NULL },
		{ "BlocksLight3", TXTFIELD_BYTE, 0, 283, NULL },
		{ "BlocksLight4", TXTFIELD_BYTE, 0, 284, NULL },
		{ "BlocksLight5", TXTFIELD_BYTE, 0, 285, NULL },
		{ "BlocksLight6", TXTFIELD_BYTE, 0, 286, NULL },
		{ "BlocksLight7", TXTFIELD_BYTE, 0, 287, NULL },
		{ "HasCollision0", TXTFIELD_BYTE, 0, 288, NULL },
		{ "HasCollision1", TXTFIELD_BYTE, 0, 289, NULL },
		{ "HasCollision2", TXTFIELD_BYTE, 0, 290, NULL },
		{ "HasCollision3", TXTFIELD_BYTE, 0, 291, NULL },
		{ "HasCollision4", TXTFIELD_BYTE, 0, 292, NULL },
		{ "HasCollision5", TXTFIELD_BYTE, 0, 293, NULL },
		{ "HasCollision6", TXTFIELD_BYTE, 0, 294, NULL },
		{ "HasCollision7", TXTFIELD_BYTE, 0, 295, NULL },
		{ "IsAttackable0", TXTFIELD_BYTE, 0, 296, NULL },
		{ "Start0", TXTFIELD_BYTE, 0, 297, NULL },
		{ "Start1", TXTFIELD_BYTE, 0, 298, NULL },
		{ "Start2", TXTFIELD_BYTE, 0, 299, NULL },
		{ "Start3", TXTFIELD_BYTE, 0, 300, NULL },
		{ "Start4", TXTFIELD_BYTE, 0, 301, NULL },
		{ "Start5", TXTFIELD_BYTE, 0, 302, NULL },
		{ "Start6", TXTFIELD_BYTE, 0, 303, NULL },
		{ "Start7", TXTFIELD_BYTE, 0, 304, NULL },
		{ "OrderFlag0", TXTFIELD_BYTE, 0, 305, NULL },
		{ "OrderFlag1", TXTFIELD_BYTE, 0, 306, NULL },
		{ "OrderFlag2", TXTFIELD_BYTE, 0, 307, NULL },
		{ "OrderFlag3", TXTFIELD_BYTE, 0, 308, NULL },
		{ "OrderFlag4", TXTFIELD_BYTE, 0, 309, NULL },
		{ "OrderFlag5", TXTFIELD_BYTE, 0, 310, NULL },
		{ "OrderFlag6", TXTFIELD_BYTE, 0, 311, NULL },
		{ "OrderFlag7", TXTFIELD_BYTE, 0, 312, NULL },
		{ "EnvEffect", TXTFIELD_BYTE, 0, 313, NULL },
		{ "IsDoor", TXTFIELD_BYTE, 0, 314, NULL },
		{ "BlocksVis", TXTFIELD_BYTE, 0, 315, NULL },
		{ "Orientation", TXTFIELD_BYTE, 0, 316, NULL },
		{ "Trans", TXTFIELD_BYTE, 0, 318, NULL },
		{ "PreOperate", TXTFIELD_BYTE, 0, 317, NULL },
		{ "Mode0", TXTFIELD_BYTE, 0, 319, NULL },
		{ "Mode1", TXTFIELD_BYTE, 0, 320, NULL },
		{ "Mode2", TXTFIELD_BYTE, 0, 321, NULL },
		{ "Mode3", TXTFIELD_BYTE, 0, 322, NULL },
		{ "Mode4", TXTFIELD_BYTE, 0, 323, NULL },
		{ "Mode5", TXTFIELD_BYTE, 0, 324, NULL },
		{ "Mode6", TXTFIELD_BYTE, 0, 325, NULL },
		{ "Mode7", TXTFIELD_BYTE, 0, 326, NULL },
		{ "Xoffset", TXTFIELD_DWORD, 0, 328, NULL },
		{ "Yoffset", TXTFIELD_DWORD, 0, 332, NULL },
		{ "Draw", TXTFIELD_BYTE, 0, 336, NULL },
		{ "HD", TXTFIELD_BYTE, 0, 337, NULL },
		{ "TR", TXTFIELD_BYTE, 0, 338, NULL },
		{ "LG", TXTFIELD_BYTE, 0, 339, NULL },
		{ "RA", TXTFIELD_BYTE, 0, 340, NULL },
		{ "LA", TXTFIELD_BYTE, 0, 341, NULL },
		{ "RH", TXTFIELD_BYTE, 0, 342, NULL },
		{ "LH", TXTFIELD_BYTE, 0, 343, NULL },
		{ "SH", TXTFIELD_BYTE, 0, 344, NULL },
		{ "S1", TXTFIELD_BYTE, 0, 345, NULL },
		{ "S2", TXTFIELD_BYTE, 0, 346, NULL },
		{ "S3", TXTFIELD_BYTE, 0, 347, NULL },
		{ "S4", TXTFIELD_BYTE, 0, 348, NULL },
		{ "S5", TXTFIELD_BYTE, 0, 349, NULL },
		{ "S6", TXTFIELD_BYTE, 0, 350, NULL },
		{ "S7", TXTFIELD_BYTE, 0, 351, NULL },
		{ "S8", TXTFIELD_BYTE, 0, 352, NULL },
		{ "TotalPieces", TXTFIELD_BYTE, 0, 353, NULL },
		{ "XSpace", TXTFIELD_BYTE, 0, 354, NULL },
		{ "YSpace", TXTFIELD_BYTE, 0, 355, NULL },
		{ "Red", TXTFIELD_BYTE, 0, 356, NULL },
		{ "Green", TXTFIELD_BYTE, 0, 357, NULL },
		{ "Blue", TXTFIELD_BYTE, 0, 358, NULL },
		{ "SubClass", TXTFIELD_BYTE, 0, 359, NULL },
		{ "NameOffset", TXTFIELD_DWORD, 0, 360, NULL },
		{ "MonsterOK", TXTFIELD_BYTE, 0, 365, NULL },
		{ "OperateRange", TXTFIELD_BYTE, 0, 366, NULL },
		{ "ShrineFunction", TXTFIELD_BYTE, 0, 367, NULL },
		{ "Act", TXTFIELD_BYTE, 0, 368, NULL },
		{ "Restore", TXTFIELD_BYTE, 0, 371, NULL },
		{ "RestoreVirgins", TXTFIELD_BYTE, 0, 372, NULL },
		{ "Parm0", TXTFIELD_DWORD, 0, 376, NULL },
		{ "Parm1", TXTFIELD_DWORD, 0, 380, NULL },
		{ "Parm2", TXTFIELD_DWORD, 0, 384, NULL },
		{ "Parm3", TXTFIELD_DWORD, 0, 388, NULL },
		{ "Parm4", TXTFIELD_DWORD, 0, 392, NULL },
		{ "Parm5", TXTFIELD_DWORD, 0, 396, NULL },
		{ "Parm6", TXTFIELD_DWORD, 0, 400, NULL },
		{ "Parm7", TXTFIELD_DWORD, 0, 404, NULL },
		{ "Lockable", TXTFIELD_BYTE, 0, 369, NULL },
		{ "Gore", TXTFIELD_BYTE, 0, 370, NULL },
		{ "Sync", TXTFIELD_BYTE, 0, 373, NULL },
		{ "Beta", TXTFIELD_BYTE, 0, 432, NULL },
		{ "nTgtFX", TXTFIELD_BYTE, 0, 408, NULL },
		{ "nTgtFY", TXTFIELD_BYTE, 0, 409, NULL },
		{ "nTgtBX", TXTFIELD_BYTE, 0, 410, NULL },
		{ "nTgtBY", TXTFIELD_BYTE, 0, 411, NULL },
		{ "Damage", TXTFIELD_BYTE, 0, 412, NULL },
		{ "CollisionSubst", TXTFIELD_BYTE, 0, 413, NULL },
		{ "Left", TXTFIELD_DWORD, 0, 416, NULL },
		{ "Top", TXTFIELD_DWORD, 0, 420, NULL },
		{ "Width", TXTFIELD_DWORD, 0, 424, NULL },
		{ "Height", TXTFIELD_DWORD, 0, 428, NULL },
		{ "OperateFn", TXTFIELD_BYTE, 0, 435, NULL },
		{ "PopulateFn", TXTFIELD_BYTE, 0, 434, NULL },
		{ "ClientFn", TXTFIELD_BYTE, 0, 436, NULL },
		{ "InitFn", TXTFIELD_BYTE, 0, 433, NULL },
		{ "Overlay", TXTFIELD_BYTE, 0, 437, NULL },
		{ "BlockMissile", TXTFIELD_BYTE, 0, 438, NULL },
		{ "DrawUnder", TXTFIELD_BYTE, 0, 439, NULL },
		{ "OpenWarp", TXTFIELD_BYTE, 0, 440, NULL },
		{ "AutoMap", TXTFIELD_DWORD, 0, 444, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};
	
	sgptDataTables->pObjectsTxt = (D2ObjectsTxt*)DATATBLS_CompileTxt(pMemPool, "objects", pTbl, &sgptDataTables->nObjectsTxtRecordCount, sizeof(D2ObjectsTxt));

	for (int i = 0; i < sgptDataTables->nObjectsTxtRecordCount; ++i)
	{
		memset(sgptDataTables->pObjectsTxt[i].wszName, 0x00, sizeof(sgptDataTables->pObjectsTxt[i].wszName));
		wcsncpy_s(sgptDataTables->pObjectsTxt[i].wszName, D2LANG_GetStringByReferenceString(sgptDataTables->pObjectsTxt[i].szName), sizeof(sgptDataTables->pObjectsTxt[i].szName));

		for (int j = 0; j < 8; ++j)
		{
			sgptDataTables->pObjectsTxt[i].dwFrameCnt[j] <<= 8;
		}
	}
}

//D2Common.0x6FD718F0 (#10626)
D2ObjectsTxt* __stdcall DATATBLS_GetObjectsTxtRecord(int nObjectId)
{
	D2_ASSERT(nObjectId < sgptDataTables->nObjectsTxtRecordCount);
	D2_ASSERT(nObjectId >= 0);
	return &sgptDataTables->pObjectsTxt[nObjectId];
}

//D2Common.0x6FD71960
void __fastcall DATATBLS_UnloadObjectsTxt()
{
	DATATBLS_UnloadBin(sgptDataTables->pObjectsTxt);
	sgptDataTables->pObjectsTxt = NULL;
	sgptDataTables->nObjectsTxtRecordCount = 0;
}

//D2Common.0x6FD71980
void __fastcall DATATBLS_UnloadObjGroupTxt()
{
	DATATBLS_UnloadBin(sgptDataTables->pObjGroupTxt);
	sgptDataTables->pObjGroupTxt = NULL;
	sgptDataTables->nObjGroupTxtRecordCount = 0;
}

//D2Common.0x6FD719A0
void __fastcall DATATBLS_LoadObjGroupTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "ID0", TXTFIELD_DWORD, 0, 0, NULL },
		{ "DENSITY0", TXTFIELD_BYTE, 0, 32, NULL },
		{ "PROB0", TXTFIELD_BYTE, 0, 40, NULL },
		{ "ID1", TXTFIELD_DWORD, 0, 4, NULL },
		{ "DENSITY1", TXTFIELD_BYTE, 0, 33, NULL },
		{ "PROB1", TXTFIELD_BYTE, 0, 41, NULL },
		{ "ID2", TXTFIELD_DWORD, 0, 8, NULL },
		{ "DENSITY2", TXTFIELD_BYTE, 0, 34, NULL },
		{ "PROB2", TXTFIELD_BYTE, 0, 42, NULL },
		{ "ID3", TXTFIELD_DWORD, 0, 12, NULL },
		{ "DENSITY3", TXTFIELD_BYTE, 0, 35, NULL },
		{ "PROB3", TXTFIELD_BYTE, 0, 43, NULL },
		{ "ID4", TXTFIELD_DWORD, 0, 16, NULL },
		{ "DENSITY4", TXTFIELD_BYTE, 0, 36, NULL },
		{ "PROB4", TXTFIELD_BYTE, 0, 44, NULL },
		{ "ID5", TXTFIELD_DWORD, 0, 20, NULL },
		{ "DENSITY5", TXTFIELD_BYTE, 0, 37, NULL },
		{ "PROB5", TXTFIELD_BYTE, 0, 45, NULL },
		{ "ID6", TXTFIELD_DWORD, 0, 24, NULL },
		{ "DENSITY6", TXTFIELD_BYTE, 0, 38, NULL },
		{ "PROB6", TXTFIELD_BYTE, 0, 46, NULL },
		{ "ID7", TXTFIELD_DWORD, 0, 28, NULL },
		{ "DENSITY7", TXTFIELD_BYTE, 0, 39, NULL },
		{ "PROB7", TXTFIELD_BYTE, 0, 47, NULL },
		{ "SHRINES", TXTFIELD_BYTE, 0, 48, NULL },
		{ "WELLS", TXTFIELD_BYTE, 0, 49, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pObjGroupTxt = (D2ObjGroupTxt*)DATATBLS_CompileTxt(pMemPool, "objgroup", pTbl, &sgptDataTables->nObjGroupTxtRecordCount, sizeof(D2ObjGroupTxt));
}

//D2Common.0x6FD71E00 (#10627)
D2ObjGroupTxt* __stdcall DATATBLS_GetObjGroupTxtRecord(int nId)
{
	if (nId < sgptDataTables->nObjGroupTxtRecordCount)
	{
		return &sgptDataTables->pObjGroupTxt[nId];
	}

	return NULL;
}

//D2Common.0x6FD71E30
void __fastcall DATATBLS_LoadShrinesTxt(void* pMemPool)
{
	D2BinFieldStrc pTbl[] =
	{
		{ "Code", TXTFIELD_BYTE, 0, 0, NULL },
		{ "Arg0", TXTFIELD_DWORD, 0, 4, NULL },
		{ "Arg1", TXTFIELD_DWORD, 0, 8, NULL },
		{ "Duration in frames", TXTFIELD_DWORD, 0, 12, NULL },
		{ "reset time in minutes", TXTFIELD_BYTE, 0, 16, NULL },
		{ "rarity", TXTFIELD_BYTE, 0, 17, NULL },
		{ "view name", TXTFIELD_ASCII, 31, 18, NULL },
		{ "niftyphrase", TXTFIELD_ASCII, 127, 50, NULL },
		{ "effectclass", TXTFIELD_BYTE, 0, 178, NULL },
		{ "LevelMin", TXTFIELD_DWORD, 0, 180, NULL },
		{ "end", TXTFIELD_NONE, 0, 0, NULL },
	};

	sgptDataTables->pShrinesTxt = (D2ShrinesTxt*)DATATBLS_CompileTxt(pMemPool, "shrines", pTbl, &sgptDataTables->nShrinesTxtRecordCount, sizeof(D2ShrinesTxt));
}

//D2Common.0x6FD72000 (#10624)
D2ShrinesTxt* __stdcall DATATBLS_GetShrinesTxtRecord(int nShrineId)
{
	D2_ASSERT(nShrineId < sgptDataTables->nShrinesTxtRecordCount);
	D2_ASSERT(nShrineId >= 0);
	return &sgptDataTables->pShrinesTxt[nShrineId];
}

//D2Common.0x6FD72070 (#10625)
int __stdcall DATATBLS_GetShrinesTxtRecordCount()
{
	return sgptDataTables->nShrinesTxtRecordCount;
}

//D2Common.0x6FD72080
void __fastcall DATATBLS_UnloadShrinesTxt()
{
	DATATBLS_UnloadBin(sgptDataTables->pShrinesTxt);
	sgptDataTables->pShrinesTxt = NULL;
	sgptDataTables->nShrinesTxtRecordCount = 0;
}
