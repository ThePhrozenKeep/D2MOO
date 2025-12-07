#pragma once

#include <Windows.h>
#include <D2BasicTypes.h>
#include <Archive.h>
#include <D2Config.h>

struct D2CellFileStrc;


//D2Win.0x6F8A59C0 (#10039)
D2WIN_DLL_DECL D2CellFileStrc* __fastcall ARCHIVE_LoadCellFile(const char* szFile, int32_t nType);
//D2Win.0x6F8A5A50 (#10040)
D2WIN_DLL_DECL D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileWithFileSize(const char* szFile, uint32_t* pFileSize, int32_t nType);
//D2Win.0x6F8A5AE0 (#10041)
D2WIN_DLL_DECL void __fastcall ARCHIVE_FreeCellFile(D2CellFileStrc* pCellFile);
//D2Win.0x6F8A5B00 (#10205)
D2WIN_DLL_DECL BOOL __stdcall ARCHIVE_ShowInsertExpansionDiscMessage();
//D2Win.0x6F8A5B20 (#10174)
D2WIN_DLL_DECL BOOL __stdcall ARCHIVE_ShowInsertPlayDiscMessage();
//D2Win.0x6F8A5B40 (#10183)
D2WIN_DLL_DECL BOOL __stdcall ARCHIVE_ShowInsertCinematicsDisc();
//D2Win.0x6F8A5B60 (#10037)
D2WIN_DLL_DECL BOOL __fastcall ARCHIVE_LoadArchives();
//D2Win.0x6F8A5CB0 (#10036)
D2WIN_DLL_DECL void __fastcall ARCHIVE_FreeArchives();
//D2Win.0x6F8A5DE0 (#10038)
D2WIN_DLL_DECL HD2ARCHIVE __stdcall D2Win_GetArchive();
//D2Win.0x6F8A5DF0 (#10185)
D2WIN_DLL_DECL int __stdcall D2Win_10185();
//D2Win.0x6F8A5E80 (#10171)
D2WIN_DLL_DECL BOOL __fastcall ARCHIVE_LoadExpansionArchives(ARCHIVE_ShowMessageFunctionPtr pfShowInsertPlayDisc, ARCHIVE_ShowMessageFunctionPtr pfShowInsertExpansionDisc, HANDLE hFile, D2ConfigStrc* pConfig);
//D2Win.0x6F8A60A0 (#10172)
D2WIN_DLL_DECL BOOL __fastcall ARCHIVE_LoadVideoArchives(ARCHIVE_ShowMessageFunctionPtr pfShowMessage, HANDLE hFile, int32_t bExpansion);
//D2Win.0x6F8A6110 (#10173)
D2WIN_DLL_DECL void __fastcall ARCHIVE_UnloadExpansionArchives();
