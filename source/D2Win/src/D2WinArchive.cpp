#include "D2WinArchive.h"

#include <algorithm>

#include <D2CMP.h>
//#include <D2Hell.h>
#include <Fog.h>
#include <Storm.h>

#include <D2Gfx.h>
#include <Window.h>


D2MPQHandleStrc* gpD2MusicMPQ;
D2MPQHandleStrc* gpD2SpeechMPQ;
D2MPQHandleStrc* gpD2CharMPQ;
D2MPQHandleStrc* gpD2XMusicMPQ;
D2MPQHandleStrc* gpD2DataMPQ;
D2MPQHandleStrc* gpD2VideoMPQ;
D2MPQHandleStrc* gpD2DeltaMPQ;
D2MPQHandleStrc* gpD2PatchMPQ;
D2MPQHandleStrc* gpD2kfixupMPQ;
D2MPQHandleStrc* gpD2XVideoMPQ;
D2MPQHandleStrc* gpD2XTalkMPQ;
D2MPQHandleStrc* gpD2ExpMPQ;
D2MPQHandleStrc* gpD2SfxMPQ;


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
	strcat_s(szPath, D2CMP_10051_GetGfxFileExtension(nType));

	void* pFile = D2Hell_ARCHIVE_OpenFile_6F8B22F8(0, szPath, pFileSize, __FILE__, __LINE__);

	D2CellFileStrc* pCellFile = nullptr;
	D2CMP_10024_CelFileNormalize((D2CellFileStrc*)pFile, &pCellFile, __FILE__, __LINE__, -1, 0);
	return pCellFile;
}

//D2Win.0x6F8A5AE0 (#10041)
void __fastcall ARCHIVE_FreeCellFile(D2CellFileStrc* pCellFile)
{
	D2CMP_10032_CelFileFreeHardware(pCellFile);

	if (pCellFile)
	{
		D2_FREE(pCellFile);
	}
}

//D2Win.0x6F8A5B00 (#10205)
bool __stdcall ARCHIVE_ShowInsertExpansionDiscMessage()
{
	return MessageBoxA(WINDOW_GetWindow(), "Insert Expansion Disc", "Diablo II", MB_ICONWARNING | MB_OKCANCEL) == 1;
}

//D2Win.0x6F8A5B20 (#10174)
bool __stdcall ARCHIVE_ShowInsertPlayDiscMessage()
{
	return MessageBoxA(WINDOW_GetWindow(), "Insert Play Disc", "Diablo II", MB_ICONWARNING | MB_OKCANCEL) == 1;
}

//D2Win.0x6F8A5B40 (#10183)
bool __stdcall ARCHIVE_ShowInsertCinematicsDisc()
{
	return MessageBoxA(WINDOW_GetWindow(), "Insert Cinematics Disc", "Diablo II", MB_ICONWARNING | MB_OKCANCEL) == 1;
}

//D2Win.0x6F8A5B60 (#10037)
int32_t __fastcall ARCHIVE_LoadArchives()
{
	gpD2DataMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2data.mpq", "D2DATA", 0, 0, nullptr, 1000);
	gpD2SfxMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2sfx.mpq", "D2SFX", 0, 0, nullptr, 1000);
	gpD2SpeechMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2speech.mpq", "D2SPEECH", 0, 0, nullptr, 1000);
	gpD2DeltaMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2delta.mpq", "D2DELTA", 0, 0, nullptr, 1000);
	gpD2kfixupMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2kfixup.mpq", "D2KOREANFIXUP", 0, 0, nullptr, 1000);
	gpD2PatchMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "patch_d2.mpq", "PATCH_D2", 0, 0, nullptr, 5000);
	gpD2ExpMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2exp.mpq", "D2EXPANSION", 0, 0, nullptr, 3000);

	return Fog_10117_UseDirect() || gpD2DataMPQ && gpD2SfxMPQ && gpD2SpeechMPQ && (!FOG_10227_CheckLodOrClassic() || gpD2ExpMPQ);
}

//D2Win.0x6F8A5CB0 (#10036)
void __fastcall ARCHIVE_FreeArchives()
{
	if (gpD2DataMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2DataMPQ);
		gpD2DataMPQ = nullptr;
	}

	if (gpD2SpeechMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2SpeechMPQ);
		gpD2SpeechMPQ = nullptr;
	}

	if (gpD2SfxMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2SfxMPQ);
		gpD2SfxMPQ = nullptr;
	}

	if (gpD2DeltaMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2DeltaMPQ);
		gpD2DeltaMPQ = nullptr;
	}

	if (gpD2kfixupMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2kfixupMPQ);
		gpD2kfixupMPQ = nullptr;
	}

	if (gpD2ExpMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2ExpMPQ);
		gpD2ExpMPQ = nullptr;
	}

	if (gpD2XMusicMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2XMusicMPQ);
		gpD2XMusicMPQ = nullptr;
	}

	if (gpD2PatchMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2PatchMPQ);
		gpD2PatchMPQ = nullptr;
	}

	ARCHIVE_UnloadExpansionArchives();
}

//D2Win.0x6F8A5DE0 (#10038)
int32_t __stdcall D2Win_10038_Return0()
{
	return 0;
}

//D2Win.0x6F8A5DF0 (#10185)
int32_t __stdcall D2Win_10185()
{
	char szPath[260] = {};

	Fog_10116(szPath, std::size(szPath));
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

	return !Fog_10117_UseDirect();
}

//D2Win.0x6F8A5E80 (#10171)
int32_t __fastcall ARCHIVE_LoadExpansionArchives(int32_t(*pfShowInsertPlayDisc)(), int32_t(*pfShowInsertExpansionDisc)(), HANDLE hFile, void *a4)
{
	if (Fog_10117_UseDirect())
	{
		pfShowInsertExpansionDisc = nullptr;
		pfShowInsertPlayDisc = nullptr;
	}

	gpD2XVideoMPQ = nullptr;

	if (FOG_10227_CheckLodOrClassic() && (!a4 || (*((int32_t(**)())a4 + 131))()))
	{
		gpD2CharMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2char.mpq", "D2CHAR", 0, nullptr, pfShowInsertPlayDisc, 1000);
		gpD2MusicMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2music.mpq", "D2MUSIC", 0, nullptr, pfShowInsertPlayDisc, 1000);
		gpD2XMusicMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2Xmusic.mpq", "D2EXPANSION", 0, hFile, pfShowInsertExpansionDisc, 3000);
		gpD2XTalkMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2Xtalk.mpq", "D2EXPANSION", 0, hFile, pfShowInsertExpansionDisc, 3000);
		gpD2XVideoMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2Xvideo.mpq", "D2EXPANSION", 0, hFile, pfShowInsertExpansionDisc, 3000);

		if (gpD2CharMPQ && gpD2MusicMPQ && gpD2XMusicMPQ && gpD2XTalkMPQ && gpD2XVideoMPQ)
		{
			return 1;
		}
	}
	else
	{
		gpD2CharMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2char.mpq", "D2CHAR", 0, hFile, pfShowInsertPlayDisc, 1000);
		gpD2MusicMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2music.mpq", "D2MUSIC", 0, hFile, pfShowInsertPlayDisc, 1000);
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
int32_t __fastcall ARCHIVE_LoadVideoArchives(int32_t(*pfShowMessage)(), HANDLE hFile, int32_t bExpansion)
{
	if (Fog_10117_UseDirect())
	{
		pfShowMessage = nullptr;
	}

	if (!bExpansion)
	{
		gpD2VideoMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2video.mpq", "D2VIDEO", 0, hFile, pfShowMessage, 1000);
		if (!gpD2VideoMPQ)
		{
			return 0;
		}
	}
	else
	{
		gpD2XVideoMPQ = D2Hell_ARCHIVE_LoadMPQFile_6F8B2399("D2Win.dll", "d2Xvideo.mpq", "D2EXPANSION", 0, hFile, pfShowMessage, 1000);
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
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2MusicMPQ);
		gpD2MusicMPQ = nullptr;
	}

	if (gpD2CharMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2CharMPQ);
		gpD2CharMPQ = nullptr;
	}

	if (gpD2VideoMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2VideoMPQ);
		gpD2VideoMPQ = nullptr;
	}

	if (gpD2XMusicMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2XMusicMPQ);
		gpD2XMusicMPQ = nullptr;
	}

	if (gpD2XTalkMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2XTalkMPQ);
		gpD2XTalkMPQ = nullptr;
	}

	if (gpD2XVideoMPQ)
	{
		D2Hell_ARCHIVE_UnloadMPQFile_6F8B2548(gpD2XVideoMPQ);
		gpD2XVideoMPQ = nullptr;
	}
}
