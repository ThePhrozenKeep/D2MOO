#pragma once  

#include <D2Gfx.h>

//1.10f: D2Client.0x6FAA1000
//1.13c: D2Client.0x6FADB420
D2CellFileStrc* __fastcall D2CLIENT_CELLFILE_GetCellFile(char* szFilename, BOOL bAllowCompressed);

//1.10f: D2Client.0x6FAA1140
void D2CLIENT_CELLFILE_FreeCellFile(D2CellFileStrc* pCellFile);

// Helper function, inlined
inline void SafeFreeCellFile(D2CellFileStrc** ppCellFile)
{
	if (*ppCellFile)
	{
		D2CLIENT_CELLFILE_FreeCellFile(*ppCellFile);
		*ppCellFile = nullptr;
	}
}
