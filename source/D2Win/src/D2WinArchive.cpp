#include "D2WinArchive.h"

#include <algorithm>

#include <D2CMP.h>
#include <Archive.h>
#include <Fog.h>
#include <Storm.h>

#include <D2Gfx.h>
#include <Window.h>


D2ArchiveHandleStrc* gpD2MusicMPQ;
D2ArchiveHandleStrc* gpD2SpeechMPQ;
D2ArchiveHandleStrc* gpD2CharMPQ;
D2ArchiveHandleStrc* gpD2XMusicMPQ;
D2ArchiveHandleStrc* gpD2DataMPQ;
D2ArchiveHandleStrc* gpD2VideoMPQ;
D2ArchiveHandleStrc* gpD2DeltaMPQ;
D2ArchiveHandleStrc* gpD2PatchMPQ;
D2ArchiveHandleStrc* gpD2kfixupMPQ;
D2ArchiveHandleStrc* gpD2XVideoMPQ;
D2ArchiveHandleStrc* gpD2XTalkMPQ;
D2ArchiveHandleStrc* gpD2ExpMPQ;
D2ArchiveHandleStrc* gpD2SfxMPQ;


//D2Win.0x6F8A59C0 (#10039)
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFile(const char* szFile, int32_t nType)
{
	return ARCHIVE_LoadCellFileWithFileSize(szFile, nullptr, nType);
}

//D2Win.0x6F8A5A50 (#10040)
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileWithFileSize(const char* szFile, uint32_t* pFileSize, int32_t nType)
{
	char szPath[260] = {};

	strcpy_s(szPath, szFile);
	strcat_s(szPath, D2CMP_GetGfxFileExtension(nType));

	void* pFile = ARCHIVE_READ_FILE_TO_ALLOC_BUFFER(nullptr, szPath, pFileSize);

	D2CellFileStrc* pCellFile = nullptr;
	D2CMP_CelFileNormalize((D2CellFileStrc*)pFile, &pCellFile, __FILE__, __LINE__, -1, 0);
	return pCellFile;
}

//D2Win.0x6F8A5AE0 (#10041)
void __fastcall ARCHIVE_FreeCellFile(D2CellFileStrc* pCellFile)
{
	D2CMP_CelFileFreeHardware(pCellFile);

	if (pCellFile)
	{
		D2_FREE(pCellFile);
	}
}

//D2Win.0x6F8A5B00 (#10205)
BOOL __stdcall ARCHIVE_ShowInsertExpansionDiscMessage()
{
	return MessageBoxA(WINDOW_GetWindow(), "Insert Expansion Disc", "Diablo II", MB_ICONWARNING | MB_OKCANCEL) == 1;
}

//D2Win.0x6F8A5B20 (#10174)
BOOL __stdcall ARCHIVE_ShowInsertPlayDiscMessage()
{
	return MessageBoxA(WINDOW_GetWindow(), "Insert Play Disc", "Diablo II", MB_ICONWARNING | MB_OKCANCEL) == 1;
}

//D2Win.0x6F8A5B40 (#10183)
BOOL __stdcall ARCHIVE_ShowInsertCinematicsDisc()
{
	return MessageBoxA(WINDOW_GetWindow(), "Insert Cinematics Disc", "Diablo II", MB_ICONWARNING | MB_OKCANCEL) == 1;
}

//D2Win.0x6F8A5B60 (#10037)
int32_t __fastcall ARCHIVE_LoadArchives()
{
	gpD2DataMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2data.mpq", "D2DATA", 0, 0, nullptr, 1000);
	gpD2SfxMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2sfx.mpq", "D2SFX", 0, 0, nullptr, 1000);
	gpD2SpeechMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2speech.mpq", "D2SPEECH", 0, 0, nullptr, 1000);
	gpD2DeltaMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2delta.mpq", "D2DELTA", 0, 0, nullptr, 1000);
	gpD2kfixupMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2kfixup.mpq", "D2KOREANFIXUP", 0, 0, nullptr, 1000);
	gpD2PatchMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "patch_d2.mpq", "PATCH_D2", 0, 0, nullptr, 5000);
	gpD2ExpMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2exp.mpq", "D2EXPANSION", 0, 0, nullptr, 3000);

	return FOG_UseDirect() || gpD2DataMPQ && gpD2SfxMPQ && gpD2SpeechMPQ && (!FOG_IsExpansion() || gpD2ExpMPQ);
}

//D2Win.0x6F8A5CB0 (#10036)
void __fastcall ARCHIVE_FreeArchives()
{
	if (gpD2DataMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2DataMPQ);
		gpD2DataMPQ = nullptr;
	}

	if (gpD2SpeechMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2SpeechMPQ);
		gpD2SpeechMPQ = nullptr;
	}

	if (gpD2SfxMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2SfxMPQ);
		gpD2SfxMPQ = nullptr;
	}

	if (gpD2DeltaMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2DeltaMPQ);
		gpD2DeltaMPQ = nullptr;
	}

	if (gpD2kfixupMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2kfixupMPQ);
		gpD2kfixupMPQ = nullptr;
	}

	if (gpD2ExpMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2ExpMPQ);
		gpD2ExpMPQ = nullptr;
	}

	if (gpD2XMusicMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2XMusicMPQ);
		gpD2XMusicMPQ = nullptr;
	}

	if (gpD2PatchMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2PatchMPQ);
		gpD2PatchMPQ = nullptr;
	}

	ARCHIVE_UnloadExpansionArchives();
}

//D2Win.0x6F8A5DE0 (#10038)
HD2ARCHIVE __stdcall D2Win_GetArchive()
{
	return nullptr;
}

//D2Win.0x6F8A5DF0 (#10185)
int32_t __stdcall D2Win_10185()
{
	char szPath[260] = {};

	FOG_GetInstallPath(szPath, std::size(szPath));
	SStrNCat(szPath, "d2char.mpq", std::size(szPath));

	if (GetFileAttributesA(szPath) != -1)
	{
		return 0;
	}

	SStrCopy(szPath, "d2char.mpq", std::size(szPath));

	if (GetFileAttributesA(szPath) != -1)
	{
		return 0;
	}

	return !FOG_UseDirect();
}

//D2Win.0x6F8A5E80 (#10171)
BOOL __fastcall ARCHIVE_LoadExpansionArchives(ARCHIVE_ShowMessageFunctionPtr pfShowInsertPlayDisc, ARCHIVE_ShowMessageFunctionPtr pfShowInsertExpansionDisc, HANDLE hFile, D2ConfigStrc* pConfig)
{
	if (FOG_UseDirect())
	{
		pfShowInsertExpansionDisc = nullptr;
		pfShowInsertPlayDisc = nullptr;
	}

	gpD2XVideoMPQ = nullptr;

	if (FOG_IsExpansion() && (!pConfig || pConfig->pAllowExpansionCallback()))
	{
		gpD2CharMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2char.mpq", "D2CHAR", 0, nullptr, pfShowInsertPlayDisc, 1000);
		gpD2MusicMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2music.mpq", "D2MUSIC", 0, nullptr, pfShowInsertPlayDisc, 1000);
		gpD2XMusicMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2Xmusic.mpq", "D2EXPANSION", 0, hFile, pfShowInsertExpansionDisc, 3000);
		gpD2XTalkMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2Xtalk.mpq", "D2EXPANSION", 0, hFile, pfShowInsertExpansionDisc, 3000);
		gpD2XVideoMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2Xvideo.mpq", "D2EXPANSION", 0, hFile, pfShowInsertExpansionDisc, 3000);

		if (gpD2CharMPQ && gpD2MusicMPQ && gpD2XMusicMPQ && gpD2XTalkMPQ && gpD2XVideoMPQ)
		{
			return 1;
		}
	}
	else
	{
		gpD2CharMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2char.mpq", "D2CHAR", 0, hFile, pfShowInsertPlayDisc, 1000);
		gpD2MusicMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2music.mpq", "D2MUSIC", 0, hFile, pfShowInsertPlayDisc, 1000);
		gpD2XMusicMPQ = nullptr;
		gpD2XTalkMPQ = nullptr;

		if (gpD2CharMPQ && gpD2MusicMPQ)
		{
			return 1;
		}
	}

	ARCHIVE_UnloadExpansionArchives();

	return 0;
}

//D2Win.0x6F8A60A0 (#10172)
BOOL __fastcall ARCHIVE_LoadVideoArchives(ARCHIVE_ShowMessageFunctionPtr pfShowMessage, HANDLE hFile, int32_t bExpansion)
{
	if (FOG_UseDirect())
	{
		pfShowMessage = nullptr;
	}

	if (!bExpansion)
	{
		gpD2VideoMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2video.mpq", "D2VIDEO", 0, hFile, pfShowMessage, 1000);
		if (!gpD2VideoMPQ)
		{
			return 0;
		}
	}
	else
	{
		gpD2XVideoMPQ = ARCHIVE_LoadMPQFile("D2Win.dll", "d2Xvideo.mpq", "D2EXPANSION", 0, hFile, pfShowMessage, 1000);
		if (!gpD2XVideoMPQ)
		{
			return 0;
		}
	}

	return 1;
}

//D2Win.0x6F8A6110 (#10173)
void __fastcall ARCHIVE_UnloadExpansionArchives()
{
	if (gpD2MusicMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2MusicMPQ);
		gpD2MusicMPQ = nullptr;
	}

	if (gpD2CharMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2CharMPQ);
		gpD2CharMPQ = nullptr;
	}

	if (gpD2VideoMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2VideoMPQ);
		gpD2VideoMPQ = nullptr;
	}

	if (gpD2XMusicMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2XMusicMPQ);
		gpD2XMusicMPQ = nullptr;
	}

	if (gpD2XTalkMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2XTalkMPQ);
		gpD2XTalkMPQ = nullptr;
	}

	if (gpD2XVideoMPQ)
	{
		ARCHIVE_UnloadMPQFile(gpD2XVideoMPQ);
		gpD2XVideoMPQ = nullptr;
	}
}
