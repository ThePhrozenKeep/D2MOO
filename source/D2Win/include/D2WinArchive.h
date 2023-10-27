#pragma once

#include <Windows.h>


struct D2CellFileStrc;


//D2Win.0x6F8A59C0
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFile(const char* szFile, int nType);
//D2Win.0x6F8A5A50
D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileWithFileSize(const char* szFile, unsigned int* pFileSize, int nType);
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
int __stdcall D2Win_10038_Return0();
//D2Win.0x6F8A5DF0
int __stdcall D2Win_10185();
//D2Win.0x6F8A5E80
BOOL __fastcall ARCHIVE_LoadExpansionArchives(int(*pfShowInsertPlayDisc)(), int(*pfShowInsertExpansionDisc)(), HANDLE a3, void *a4);
//D2Win.0x6F8A60A0
BOOL __fastcall ARCHIVE_LoadVideoArchives(int(*a1)(), HANDLE a2, int bExpansion);
//D2Win.0x6F8A6110
void __fastcall ARCHIVE_UnloadExpansionArchives();
