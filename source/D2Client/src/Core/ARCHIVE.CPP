#include <Core/ARCHIVE.h> 
#include <D2Gfx.h>
#include <D2CMP.h>
#include <Archive.h>
#include <D2WinArchive.h>

//1.10f:D2Client.0x6FAA1000
//1.13c:D2Client.0x6FADB420
D2CellFileStrc* __fastcall D2CLIENT_CELLFILE_GetCellFile(char* szFilename, BOOL bAllowCompressed)
{
	char szPathBuffer[MAX_PATH];
	strcpy(szPathBuffer, szFilename);
	strcat(szPathBuffer, D2CMP_GetGfxFileExtension(bAllowCompressed));
	D2CellFileStrc* pFileContent = (D2CellFileStrc*)ARCHIVE_ALLOC_BUFFER_AND_READ_FILE_TO_IT(D2Win_GetArchive(), szPathBuffer, 0);
	D2CellFileStrc* pOutCelFile = nullptr;
	D2CMP_CelFileNormalize(pFileContent, &pOutCelFile, __FILE__, __LINE__, -1, 0);
	return pOutCelFile;
}

//1.10f:D2Client.0x6FAA1140
void D2CLIENT_CELLFILE_FreeCellFile(D2CellFileStrc* pCellFile)
{
	D2CMP_CelFileFreeHardware(pCellFile);
	if(pCellFile) D2_FREE(pCellFile);
}


