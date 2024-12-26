#include <UI/UI.h> 
#include <Core/ARCHIVE.h>
#include <Storm.h>

//1.10f: D2Client.0x6FB20100
D2CellFileStrc* __fastcall D2CLIENT_CELLFILE_GetCellFileFromUIFolder(const char* szName)
{
	char szFilename[D2_MAX_PATH];
	wsprintfA(szFilename, "%s\\UI\\", "DATA\\GLOBAL");
	strcat(&szFilename[SStrLen(szFilename)], szName);
	return D2CLIENT_CELLFILE_GetCellFile(szFilename, 0);
}
