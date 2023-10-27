#pragma once

#include <Windows.h>
#include <D2BasicTypes.h>

struct D2CellFileStrc;


//D2Win.0x6F8A59C0
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFile(const char* szFile, int32_t nType);
//D2Win.0x6F8A5A50
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileWithFileSize(const char* szFile, uint32_t* pFileSize, int32_t nType);
//D2Win.0x6F8A5AE0
void __fastcall ARCHIVE_FreeCellFile(D2CellFileStrc* pCellFile);
//D2Win.0x6F8A5B00
bool __stdcall ARCHIVE_ShowInsertExpansionDiscMessage();
//D2Win.0x6F8A5B20
bool __stdcall ARCHIVE_ShowInsertPlayDiscMessage();
//D2Win.0x6F8A5B40
bool __stdcall ARCHIVE_ShowInsertCinematicsDisc();
//D2Win.0x6F8A5B60
BOOL __fastcall ARCHIVE_LoadArchives();
//D2Win.0x6F8A5CB0
void __fastcall ARCHIVE_FreeArchives();
//D2Win.0x6F8A5DE0
void* __stdcall D2Win_GetMemPool();
//D2Win.0x6F8A5DF0
int __stdcall D2Win_10185();
//D2Win.0x6F8A5E80
BOOL __fastcall ARCHIVE_LoadExpansionArchives(int32_t(*pfShowInsertPlayDisc)(), int32_t(*pfShowInsertExpansionDisc)(), HANDLE hFile, void* a4);
//D2Win.0x6F8A60A0
BOOL __fastcall ARCHIVE_LoadVideoArchives(int32_t(*pfShowMessage)(), HANDLE hFile, int32_t bExpansion);
//D2Win.0x6F8A6110
void __fastcall ARCHIVE_UnloadExpansionArchives();
