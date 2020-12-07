#pragma once

#include "CommonDefinitions.h"

//D2Common.0x6FD8A460
void __fastcall DRLGTILESUB_AddSecondaryBorder(D2UnkOutdoorStrc* a1);
//D2Common.0x6FD8A750
BOOL __fastcall DRLGTILESUB_TestReplaceSubPreset(int a1, int a2, D2UnkOutdoorStrc* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8A8E0
void __fastcall DRLGTILESUB_ReplaceSubPreset(int a1, int a2, D2UnkOutdoorStrc* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord, int a6);
//D2Common.0x6FD8AA80
void __fastcall sub_6FD8AA80(D2UnkOutdoorStrc2* a1);
//D2Common.0x6FD8ACE0
void __fastcall sub_6FD8ACE0(void* pMemPool, int edx0, int a3, D2UnkOutdoorStrc2* a4, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord, int a7);
//D2Common.0x6FD8B010
BOOL __fastcall sub_6FD8B010(int a1, int a2, D2UnkOutdoorStrc2* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8B130
BOOL __fastcall sub_6FD8B130(int a1, int a2, D2UnkOutdoorStrc2* a3, D2DrlgSubstGroupStrc* pSubstGroup, D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8B290
void __fastcall sub_6FD8B290(D2UnkOutdoorStrc2* a1, D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8B640
void __fastcall DRLGTILESUB_InitializeDrlgFile(void* pMemPool, D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8B770
void __fastcall DRLGTILESUB_FreeDrlgFile(D2LvlSubTxt* pLvlSubTxtRecord);
//D2Common.0x6FD8B7E0
int __fastcall DRLGTILESUB_PickSubThemes(D2RoomExStrc* pRoomEx, int nSubType, int nSubTheme);
